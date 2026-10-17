# Planned: application backup/restore and language startup

Status: **design draft, not implemented or released**. Track these two related
settings improvements in one PR. Keep `VERSION`, the stable catalog and firmware
unchanged while developing. This document is a specification, not an installation
or recovery procedure.

The backup suggestion and the observation of English text during startup came
from **[DamiBFryta](https://www.reddit.com/user/DamiBFryta/)**. Preserve that credit
when the implementation is released.

## 1. Backup outside the printer

Add **Settings → Backup and restore**, with a downloadable application backup,
a file preview, category selection and a separate restore confirmation. Download
to the browser/computer; do not call an on-printer copy an external backup.
The file must be usable after replacing a motherboard, once the compatible
community firmware and CC2 Control are installed and LAN access is configured.

### Data inventory from the current sources

`cc2-control/src/main.c` defines the configurable storage locations. Resolve
these application-owned paths internally; never accept paths from a backup.

| Category | Current storage | Intended backup contents |
| --- | --- | --- |
| Interface | `ui-preferences.json` | Language, theme, Quick Actions and persistent-camera preference |
| Materials | `material-presets.json` | User presets and their temperatures |
| Plates and nozzles | `bed-plates.json` | Plate/nozzle definitions, offsets, uncalibrated profiles and saved measurements with mesh points, bounds, temperature and timestamp |
| Spools | `spools.json` | Spool inventory, recorded weights/usage, retained event history and slot association information |

`plates_save()` and `spools_save()` serialize the authoritative application
stores. Spools can have unsaved consumption in memory: export must take a
consistent snapshot of the current store, not simply copy a stale disk file.
The existing `plates_load()` / `spools_load()` mutate globals and are **not**
suitable as trial validators for untrusted restore input. Extract or introduce
pure bounded parsers before implementing restore.

This is **not** a whole-printer image. Exclude LAN/SSH passwords, MQTT connection
configuration (`cc2-control.conf`), network credentials, tokens, signing keys,
firmware/executables, G-code files, camera recordings and temporary jobs. Do not
copy or restore the complete `/opt/usr/cfg/autosave.cfg`: it contains independent
firmware calibration. Library measurements are included; official A/B meshes
that have never been saved in the library are outside this initial scope.
State that limit in the export preview and user guide.

### Portable format and preview

- Use a versioned application-specific format, an explicit compatible model and
  firmware family, source app version, creation time and an allowlisted section
  manifest. The storage representation is not finalized in this design draft.
- Include corruption checks. A checksum detects corruption; it must not be
  presented as proof of a trusted author or authorization to restore.
- Validate the complete file before writing any application state: schema,
  bounds, finite numbers, supported versions, section sizes/counts, IDs,
  references, duplicate fields/IDs and checksums. Reject unknown required
  sections or newer unsupported formats; never silently restore only the part
  that happened to parse. Show errors without changing the existing libraries.
- Preview source version, creation time, included categories and counts. Missing
  optional categories are explicit and do not erase existing data.
- Let the user select categories. Use replacement, not implicit merging, in the
  first implementation and label that clearly. Preserve an automatic local
  recovery copy of the previous application state before committing a restore.

### Restore and physical state

Restore needs the existing same-origin/mutation protections, LAN-code
confirmation and fresh connected idle telemetry. Refuse printing, paused jobs,
calibration, active console commands, pending plate changes/restarts and active
application/file transfers. Recheck eligibility at commit, not just preview.
Never hold the HTTP main loop while receiving a slow large backup: its current
request buffer is bounded. Use a bounded staged upload and retain emergency
control responsiveness. No new large dependencies are needed.

Imported data is not evidence of the physical hardware state:

- Restore plate/nozzle definitions and measured values, but clear the mounted,
  pending and automatically applied state. Never load a mesh, apply a Z offset,
  rewrite firmware A/B slots or reboot merely because a backup was imported.
- Require the user to verify a restored measurement against the actual plate
  and hardware, particularly after a motherboard/sensor/mechanical change.
  Keep normal explicit mount/activation guards; restoring is not mounting.
- Preserve spool inventory, recorded usage and historical associations as data,
  but require confirmation before treating an association as physically current.
  Start tracking disabled and clear interrupted job/runout/question timers;
  never resume an old accounting session or debit a spool on import.
- Preserve the camera preference without starting the stream automatically.
  Do not overwrite the target printer's connection credentials.

Stage files privately on persistent storage, enforce quotas and maximum sizes,
use fixed allowlisted names and reject links/path traversal. Commit all selected
categories as a recoverable transaction: a disk-full, interrupted process or
power loss must not leave an unrecognized mixture of old and new libraries.
Define and test a recovery journal or equivalent restart recovery before
claiming atomic multi-file restore. A series of individual renames alone is
not an atomic multi-file transaction. Keep valid old state if validation or
staging fails, and clean up abandoned staging without deleting recovery copies.

## 2. Remove the English flash at startup

Observed code path: `web-src/src/lib/i18n.ts` starts with an empty active
dictionary and English bundled fallback; `main.tsx` starts
`loadUiPreferences()` without awaiting it and renders the application. The
non-English dictionary arrives later from `/i18n/<code>.json` and triggers a
browser render. This explains the brief English text; translation runs in the
browser. It is not evidence of a printer crash or measured CPU spike.

The backend serves HTML/locales with `Cache-Control: no-store`; no service
worker/offline application shell is implemented. Do not introduce one here.
A previously reported old offline page requires separate browser/proxy evidence.

### Intended behavior

- Initialize the first meaningful render with the selected dictionary when
  available, rather than render known-wrong English and then switch labels.
- If caching translations locally in the browser, cache only validated bounded
  dictionaries, keyed by locale and the exact UI build/version. Invalidate stale
  dictionaries after an app update. This must not cache an old HTML application
  or pretend the printer is reachable while offline.
- Printer-saved preferences remain authoritative. Browser preferences/cache
  provide startup hints only, including when visiting another printer.
- For a first uncached visit, use a short bounded initialization state rather
  than an unbounded blank page. Limit network waits, handle storage-disabled
  browsers, retain usable connection/error information and never leave emergency
  controls hidden indefinitely behind a failed translation request.
- Retain the existing English fallback for missing keys and the current-language
  behavior on failed language changes. Do not download all nine dictionaries or
  poll translations repeatedly. Reuse one in-flight request for a locale.
- Preserve title, dialogs, theme, language switching, camera-window behavior and
  mobile layout. Explain a genuine fallback without displaying a fake offline
  version of the application.

## Acceptance and implementation checklist

All items below are **pending**. Do not merge this design draft as a feature
implementation or publish a package on its strength.

- [ ] Pure bounded backup parsers and a documented versioned format.
- [ ] Consistent export, including unsaved spool changes, with no credentials.
- [ ] Settings export/import preview and category selection in all nine locales.
- [ ] Authenticated staged restore with fresh guards at preview and commit.
- [ ] Recoverable transaction and restart handling; interrupted/disk-full cases.
- [ ] Mesh/nozzle verification and inert physical state after restore.
- [ ] Spool usage/history retention without stale live associations/accounting.
- [ ] Browser language bootstrap, versioned dictionary caching or equivalent,
      bounded failure path and no stale offline HTML.
- [ ] Export/restore round trip with custom materials, an empty plate profile,
      calibrated measurements, nozzle corrections and spool records.
- [ ] Corrupt/truncated/oversized/unsupported/duplicate input, wrong firmware,
      traversal/symlink attempts, stale telemetry, print/calibration and auth
      refusals leave current data unchanged.
- [ ] Slow/nonresponding upload does not stall telemetry or emergency stop.
- [ ] Actual browser import confirmation and selected-category behavior;
      cancellation is inert; desktop/mobile layouts remain usable.
- [ ] Actual first-render tests in Italian/Polish/Hungarian, delayed locale and
      preferences, failed requests, disabled storage, language changes and stale
      cache after an app update. Verify all nine locales still build.
- [ ] Host/ARM, sanitizer and relevant browser checks; exact UI/overlay/snapshot
      and builder references refreshed only when application sources change.
- [ ] English and Italian user instructions, including backup coverage limits
      and the need to revalidate physical calibration after hardware changes.
- [ ] Controlled printer trial before feature merge/release. Keep trial revisions
      separate from release version numbering; do not bump versions per trial.

## Italiano

Un'unica PR in bozza raccoglie **Backup e ripristino** e **avvio della lingua
senza il passaggio visibile in inglese**. Il backup sarà scaricabile sul computer
e comprenderà i dati dell'applicazione elencati sopra, non firmware o password.
Il ripristino mostrerà prima il contenuto e richiederà una conferma; mesh, offset
e associazioni fisiche delle bobine non verranno riattivati automaticamente.
Le due funzioni devono ancora essere implementate e provate. Nessuna modifica
alla release 1.2.15 o al canale stabile in questa PR di pianificazione.

