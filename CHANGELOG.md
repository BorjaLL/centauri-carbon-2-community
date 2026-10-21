# Changelog

This file records user-visible project changes. Signed artifacts and exact
checksums remain attached to each GitHub release.

## CC2 Control 1.2.19 — 2026-10-21

**[Download CC2 Control 1.2.19](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.19-V4.4)** · [Full release notes and PRs](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.19-V4.4) · [English guide](docs/UPDATE_CC2.md) · [Guida italiana](docs/UPDATE_CC2.it.md).

> **FULL PRINTER REBOOT REQUIRED AFTER INSTALLATION / RIAVVIO COMPLETO DELLA STAMPANTE NECESSARIO.** The camera fix activates at the next complete printer start. Reloading the page or restarting only CC2 Control is insufficient.

For **Community Firmware V4.4/V4.4.1**, qualified ELEGOO base **02.01.01.07**. Settings and libraries are retained. The complete firmware image and builder downloads above remain unchanged and will be updated separately.

| New since 1.2.18 | What changes | Credit / PRs |
| --- | --- | --- |
| Camera stability and recovery | Reclaims completed video-client thread stacks and restores readiness notifications needed by local/Matrix video after a camera-only restart through the patched launcher. Original camera executable retained; no watchdog, automatic restart, extra stream slots or Wi-Fi tuning. | Diagnosis and reproduction: **[tharje](https://github.com/tharje)**, [#160](https://github.com/Neme77/centauri-carbon-2-community/issues/160); [#183](https://github.com/Neme77/centauri-carbon-2-community/pull/183), [#186](https://github.com/Neme77/centauri-carbon-2-community/pull/186) |
| Interactive object map | Job → Object exclusion → Bed map: 256 × 256 mm view, numbered markers/outlines and linked list. Selection cyan, current object amber, excluded red. Exclusion keeps its confirmation and original G-code ID; no extra polling. | Idea/prototype: **Nick Plateringen-Paul**, [#188](https://github.com/Neme77/centauri-carbon-2-community/pull/188) |
| Reliable file-date presentation | Invalid/epoch dates shown as unavailable; undated files sort after valid dates. G-code and original timestamps are untouched. | [#185](https://github.com/Neme77/centauri-carbon-2-community/pull/185) |
| Canvas and mobile refinements | Prevent unchanged Enter saves and duplicate restart submissions; annotate slot observations with recent printer context; keep header actions and the heater external link accessible on phones. | **[pezso92](https://github.com/pezso92)**, [#175](https://github.com/Neme77/centauri-carbon-2-community/pull/175), [#176](https://github.com/Neme77/centauri-carbon-2-community/pull/176), [#177](https://github.com/Neme77/centauri-carbon-2-community/pull/177), integrated [#180](https://github.com/Neme77/centauri-carbon-2-community/pull/180) |
| Panda/DragonBreath metadata | Read-only port 7125 bridge reports filename and available filament/usage metadata for DragonBreath AUTO zones; empty WebSocket ping handling and file/freshness guards. Not direct MQTT heater control. | Original feature: **[Noahwhite2014](https://github.com/Noahwhite2014)**, [#181](https://github.com/Neme77/centauri-carbon-2-community/pull/181), integrated [#182](https://github.com/Neme77/centauri-carbon-2-community/pull/182) |

**Install:** Settings → Updates → Stable → Check for updates. Enter the printer **LAN access code** from the display, download/verify, then confirm installation separately. The LAN code differs from the SSH password. Use a connected, idle and cold printer (nozzle/bed ≤50 °C), targets zero, live viewers closed. After the success message, **manually reboot the full printer**. Alternatively upload the `.cc2update`; extract the multiplatform ZIP on the computer. Same-version trials use the PC installer. No factory reset or intermediate app release is needed.

Validated on the printer: object map/exclusion, Orca sync, upgrade preserving an existing personal camera patch, and a fresh 1.2.18 → panel upload → 1.2.19 installation after removing both personal startup hooks. After full reboot, bundled camera libraries activated and local/Matrix video worked. Seven earlier hours of printing with AI options enabled and streaming working do not individually qualify AI detections or every expired RTC session.

Thanks to **tharje, Nick Plateringen-Paul, pezso92, Noahwhite2014**, **Barry**, and **[DamiBFryta](https://www.reddit.com/user/DamiBFryta/), our ultimate beta tester**, for their contributions and testing. Earlier Spanish (**asiersan**), Hungarian (**pezso92**), punctuation (**Surfoo**), plate/mesh (**beanbo**) and other contribution credits remain in [Community credits](docs/CONTRIBUTORS.md).

**Italiano:** fix webcam con recupero video locale/Matrix, mappa oggetti, date G-code non affidabili, miglioramenti Canvas/mobile di pezso92 e metadati Panda/DragonBreath di Noahwhite2014. Grazie a tharje per la diagnosi, a Nick per il prototipo e a **DamiBFryta, il beta tester definitivo**. Dopo l'installazione riuscita **riavviare completamente la stampante**; preferenze e librerie conservate. Builder e immagine firmware saranno aggiornati in seguito.

## CC2 Control 1.2.18 — 2026-10-19

**[Download CC2 Control 1.2.18](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.18-V4.4)** · [English update guide](docs/UPDATE_CC2.md) · [Guida italiana](docs/UPDATE_CC2.it.md).

**Only Community Firmware V4.4/V4.4.1, ELEGOO base 02.01.01.07.** Update the application directly from an earlier compatible CC2 Control version. Existing preferences and libraries are retained; no firmware reinstall or factory reset is required.

| New in this update | What it does | Credit |
| --- | --- | --- |
| Canvas pull-back after the cut | Set the native 60–350 mm distance with backup, restore-default and restart now/later. Requires an idle printer; applies after restart. | **Noahwhite2014**, [#168](https://github.com/Neme77/centauri-carbon-2-community/pull/168) |
| Panda Breath / Dragon Breath page | Optional interface in Settings → Integrations, embedding the heater's own web page. Saved addresses are retained; the browser must reach the device. This does not use MQTT. | **Noahwhite2014**, [#166](https://github.com/Neme77/centauri-carbon-2-community/pull/166)/[#167](https://github.com/Neme77/centauri-carbon-2-community/pull/167) |
| Canvas recovery guidance | Explains native display recovery when a reboot leaves filament in the head without an assigned slot. No automatic slot selection, heating or feeding. | [#162](https://github.com/Neme77/centauri-carbon-2-community/pull/162) |
| Language refinements | Completes 88 Spanish, Russian and Simplified Chinese strings and clarifies updater compatibility in nine languages. | **pezso92**, [#163](https://github.com/Neme77/centauri-carbon-2-community/pull/163)/[#164](https://github.com/Neme77/centauri-carbon-2-community/pull/164), integrated [#165](https://github.com/Neme77/centauri-carbon-2-community/pull/165) |

Integrated in [#169](https://github.com/Neme77/centauri-carbon-2-community/pull/169), preserving original contribution commits. Includes stricter configuration parsing, restart-tracking/write-failure guards, matching translations and builder sources, and consistent Panda Breath / Dragon Breath display names. Cumulative R6 printer test passed; the release changes only the version and matching build references from those accepted functional sources ([#170](https://github.com/Neme77/centauri-carbon-2-community/pull/170)).

**Install:** Settings → Updates → Stable → Check for updates. Enter the printer's **LAN access code** from the display, download/verify and confirm installation separately. Use a connected, idle, cold printer (nozzle/bed ≤50 °C), targets zero and live view paused. Wait for completion, then Ctrl+F5. Checking needs no code; installation does. The LAN code is different from the SSH password. Alternatively upload the signed `.cc2update`; extract the installer ZIP on the computer.

Thanks to **Noahwhite2014**, **pezso92**, **Barry** for Panda Breath field feedback and **[DamiBFryta](https://www.reddit.com/user/DamiBFryta/), our ultimate beta tester**. All previous author/language credits remain in [Community credits](docs/CONTRIBUTORS.md), including Spanish by **asiersan**, Hungarian by **pezso92**, punctuation by **Surfoo**, and mesh/plate work by **beanbo**.

**Italiano:** ritiro Canvas dopo il taglio e pagina opzionale Panda Breath / Dragon Breath di **Noahwhite2014**, guida al recupero Canvas dopo il riavvio e completamenti delle traduzioni di **pezso92**. Solo V4.4/V4.4.1; preferenze e librerie conservate. Aggiornare dal pannello con codice LAN e conferma separata; lo ZIP si estrae sul computer. Grazie a Barry e a **DamiBFryta, il nostro beta tester definitivo**, e a tutti i collaboratori!


## CC2 Control 1.2.17 — 2026-10-18

**[Download CC2 Control 1.2.17](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.17-V4.4)** · [English guide](docs/UPDATE_CC2.md) · [Guida italiana](docs/UPDATE_CC2.it.md).

**Only Community Firmware V4.4/V4.4.1, ELEGOO base 02.01.01.07.** Application update; firmware and callback unchanged. Preferences and libraries are retained. No factory reset or intermediate application release is required.

- **Orca connection checks:** test the actual Moonraker and Canvas lane endpoints; distinguish a disconnected printer, incomplete lane data and endpoint errors. This helps diagnose configuration; it is not a guarantee that every OrcaSlicer version completes synchronization.
- **Manual diagnostics export:** download a small report from Settings → About. An explicit allowlist includes version, connection/memory status, temperatures and Canvas counts. No credentials, addresses, filenames or raw API responses are exported; no automatic upload occurs.
- **Observed Canvas timeline:** up to 24 recent slot-state changes in browser memory, with a clear button and no additional periodic requests. It resets on reload. Times indicate observations; this is not a complete physical-movement log.
- **Integration fixes:** correct the diagnostic Canvas envelope, reject ambiguous timeline rows, guard repeated Orca tests and register the timeline icon to prevent a rendering failure.

**Original feature credit: [pezso92](https://github.com/pezso92)**, [#155](https://github.com/Neme77/centauri-carbon-2-community/pull/155), [#156](https://github.com/Neme77/centauri-carbon-2-community/pull/156), [#157](https://github.com/Neme77/centauri-carbon-2-community/pull/157), integrated in [#158](https://github.com/Neme77/centauri-carbon-2-community/pull/158). Original commits and attribution retained. Automated checks and the real-printer trial passed: diagnostics, Canvas observations/loaded-head slot, Canvas synchronization and print submission from the newly installed OrcaSlicer. Exact Orca version was not recorded.

Install from **Settings → Updates → Stable → Check for updates**, enter the printer **LAN access code** from the display, download/verify and confirm installation separately. The LAN code differs from the SSH password. Printer must be connected, idle and cold (nozzle/bed ≤50 °C), heater targets zero, live view paused. Wait for completion, then Ctrl+F5. Alternatively upload the signed `.cc2update`; the installer ZIP is extracted on the computer. Notifications never install unattended.

Retains backup/restore, languages and all earlier fixes, including Spanish by **asiersan**, Hungarian by **pezso92**, punctuation by **Surfoo** and plate/mesh work by **beanbo**. Thanks to **[DamiBFryta](https://www.reddit.com/user/DamiBFryta/), our ultimate beta tester**, and all contributors/testers. Experimental Canvas ejection remains excluded.

**Italiano:** controlli Orca più precisi, esportazione diagnostica manuale e cronologia degli stati Canvas, proposti da **pezso92**. Solo V4.4/V4.4.1; firmware/callback e dati personali conservati. Aggiornare dal pannello con codice LAN e conferma separata. La cronologia osserva gli stati e non certifica movimenti fisici; il rapporto diagnostico non viene inviato automaticamente.

## CC2 Control 1.2.16 — 2026-10-18

**[Download CC2 Control 1.2.16](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.16-V4.4)** · [English guide](docs/UPDATE_CC2.md) · [Guida italiana](docs/UPDATE_CC2.it.md).

**Only Community Firmware V4.4/V4.4.1, ELEGOO base 02.01.01.07.** Updates CC2 Control while retaining firmware, callback and configuration. No factory reset or intermediate app release is required.

| New in 1.2.16 | What changes |
| --- | --- |
| Backup and selective restore | Export preferences, material presets, saved plate/nozzle meshes and spool inventory/history to your computer. Preview and select categories before restoring. Credentials, firmware, G-code/video and official A/B meshes outside the library are excluded. Verify imported hardware mappings; spool tracking starts disabled. |
| Saved-language startup | Load the saved language before the meaningful interface render, with bounded requests and fallback. No offline application cache added. |
| Hungarian and preferences | 42 refinements and failed-save feedback by **pezso92**; language menu ordered alphabetically. |
| Optional Console monitor | Bounded passive G-code reports over the existing connection. Manual commands retain their guards. It is not the full system log. |
| Print mesh status | Saved, confirmed missing and unknown states, with a read-only Refresh, by **pezso92**. Failed reads no longer silently select calibration; A/B status is independent. |
| Settings layout | All six tabs remain readable and wrap on narrow screens. |

Install from **Settings → Updates → Stable**, enter the configured **LAN code** from the display under **Settings → LAN only**, download/verify and confirm installation separately. The LAN code differs from the SSH password. Use a connected idle printer, nozzle/bed ≤50 °C, heater targets zero and live view paused. After completion use Ctrl+F5. Notifications and downloads never install unattended. Alternatively upload the signed `.cc2update`; extract the computer installer ZIP only if needed.

**Included PRs:** #144/#145 → [#148](https://github.com/Neme77/centauri-carbon-2-community/pull/148), [#147](https://github.com/Neme77/centauri-carbon-2-community/pull/147), [#149](https://github.com/Neme77/centauri-carbon-2-community/pull/149), #150 → [#151](https://github.com/Neme77/centauri-carbon-2-community/pull/151). Original commits and attribution retained. All eight checks passed; the maintainer confirmed the cumulative features, signed update, Settings layout and final print popup. Published payload is identical to accepted R4; no new long-duration-print qualification is claimed.

**Thanks to [pezso92](https://github.com/pezso92)** for Hungarian, preference-save feedback and print mesh readiness. Special thanks to **[DamiBFryta](https://www.reddit.com/user/DamiBFryta/), our ultimate beta tester**, for the backup/language ideas, first error 803 report and tireless tests. Retains Spanish by **asiersan**, punctuation by **Surfoo**, mesh work by **beanbo** and all previous credits. Experimental Canvas ejection remains excluded.

**Italiano:** backup/ripristino selettivo, avvio nella lingua salvata, correzioni ungheresi e del salvataggio di **pezso92**, lingue ordinate, monitor Console opzionale, popup mesh con stato sconosciuto e Aggiorna di **pezso92**, barra Impostazioni corretta. Solo V4.4/V4.4.1; firmware e callback invariati. **DamiBFryta, il beta tester definitivo**, grazie per idee e prove! Aggiornare dal pannello con codice LAN e conferma separata; nessun reset necessario.

## CC2 Control 1.2.15 — 2026-10-17

**[Download CC2 Control 1.2.15](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.15-V4.4)** · [English update guide](docs/UPDATE_CC2.md) · [Guida italiana](docs/UPDATE_CC2.it.md).

**Community Firmware V4.4 / V4.4.1 only, based on ELEGOO 02.01.01.07.** This is an application update: installed firmware, callback patch, preferences and profiles are retained. V4.3 and stock firmware are unsupported.

| New in 1.2.15 | What changes |
| --- | --- |
| Update notification bell | Beside Emergency Stop, indicates a newer compatible update from the verified signed catalog. Opens Settings → Updates; never downloads or installs automatically. |
| Library mesh selector | Choose a saved measurement by plate, side, temperature and timestamp. **Mount and activate** confirms the physical plate and verifies the actual active profile, measured matrix and bounds before claiming success. Merely selecting or viewing a graph changes nothing. |
| Clear calibration hold | A dropdown chooses 0–60 minutes of thermal stabilization after reaching the target temperature; new runs default to 2 minutes. The confirmation and countdown show the wait. Changing pages preserves the selected destination and timing. |
| Hungarian completion | 20 remaining Canvas, object-position and plate/calibration translations by **pezso92**. All nine languages remain supported. |

Official ELEGOO A/B selection in the print popup retains its existing behavior. A saved graph is not proof that its mesh is active: use the explicit library action with the corresponding physical plate. Restart is requested only when needed and with separate consent; pending verification is not reported as mounted.

**Install:** Settings → Updates → Stable → Check for updates, then enter the LAN code from the display under **Settings → LAN only**. Download and verify, then confirm installation separately. Pause live view; the printer must be connected, idle, with nozzle/bed ≤50 °C and heater targets zero. Check for updates needs no LAN code; download/upload and installation do. The LAN code differs from the SSH password. Alternatively upload the release's signed `.cc2update`; use the computer-assisted installer ZIP if the updater panel is absent. No factory reset or intermediate application release is required.

**Printer validation:** the combined trial installed successfully, the bell worked, and a new Side B mesh was calibrated, saved and loaded with the correct displayed result. The final hold-selector follow-up has automated interaction coverage; it changes the form, not the calibration engine. These checks do not claim a new long-duration print, first-layer comparison or every custom A/B combination. Full CI covers host/ARM/qemu, sanitizers, browser interactions, firmware builder and pinned sources.

**Included PRs:** [#139](https://github.com/Neme77/centauri-carbon-2-community/pull/139), [#140](https://github.com/Neme77/centauri-carbon-2-community/pull/140), [#138](https://github.com/Neme77/centauri-carbon-2-community/pull/138)/[#141](https://github.com/Neme77/centauri-carbon-2-community/pull/141), qualified together in [#142](https://github.com/Neme77/centauri-carbon-2-community/pull/142). Original commits and attribution are retained.

**Thanks:** **[pezso92](https://github.com/pezso92)** for Hungarian and **[beanbo](https://github.com/beanbo)** for the underlying plate/mesh library. A special salute to **[DamiBFryta](https://www.reddit.com/user/DamiBFryta/), our ultimate beta tester**: the first to report error 803, and still tireless in reproducing problems, checking fixes and lending us his time and printer. Thank you to every contributor and beta tester; [full community credits](docs/CONTRIBUTORS.md).

This cumulative update also retains Spanish by **asiersan**, earlier Hungarian work by **pezso92**, localized punctuation by **Surfoo**, and the previously released Canvas, camera, fans and Object Exclusion fixes. Full experimental Canvas ejection (#103/#114) has been retired and is not included.

**Italiano:** campanella degli aggiornamenti, selezione e attivazione verificata delle mesh dalla libreria piatti, attesa termica selezionabile da 0 a 60 minuti (2 predefiniti) e completamento ungherese di pezso92. Confermati sulla stampante aggiornamento, campanella e creazione/salvataggio/caricamento della mesh corretta. La scelta A/B ufficiale resta invariata. Solo V4.4/V4.4.1; firmware e callback invariati. **DamiBFryta, il beta tester definitivo**, merita un ringraziamento speciale per la prima segnalazione dell'errore 803 e le continue prove pazienti e precise.

## CC2 Control 1.2.14 — 2026-10-16

**[Download CC2 Control 1.2.14](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.14-V4.4)** · [English update guide](docs/UPDATE_CC2.md) · [Guida italiana](docs/UPDATE_CC2.it.md).

**Only Community Firmware V4.4 / V4.4.1. Do not install on V4.3 or stock ELEGOO firmware.** Updates the application while retaining the installed firmware and callback runtime. Existing preferences and profiles are retained.

| Improvement | Result |
| --- | --- |
| Object Exclusion | Spatial order from front to back and left to right, readable names and numbered copies; original G-code identifiers remain separate from display names. Handles ELEGOO's string-encoded coordinates and clears completed-print objects. |
| Canvas state | Distinguishes empty slots, filament present but unloaded from the head, loaded filament and transitions. Actions are enabled only when applicable. |
| Camera and fans | Persistent live keeps its original size; fan sliders drag correctly and numeric values still apply with Enter. |
| Plate workflow | Create a plate before its first mesh, select it for calibration and retain the destination when changing tabs. Uncalibrated profiles cannot be mounted. |
| Mesh details | Side A/B labels for default/default1, calibration from 30 to 110 °C, and measurement date plus exact time and timezone. |
| Languages and updates | Nine languages including Spanish, refined Hungarian, and localized update punctuation. |

Open **Settings → Updates → Stable → Check for updates**. Enter the configured LAN code, download and verify, then separately confirm installation. Pause live view and use an idle, connected printer with nozzle/bed at or below 50 °C and heater targets zero. No factory reset is needed. The LAN code is on the display under **Settings → LAN only** and differs from the SSH password. Without the updater panel, use the release's computer-assisted installer ZIP; panel upload accepts the signed .cc2update instead.

**Validated on the maintainer's printer:** Canvas, camera geometry, mouse fan controls, Spanish, localized punctuation, A/B labels, Object Exclusion and the new plate/calibration workflow. Native, browser and ARM tests passed; the published signed payload is identical to the tested candidate. This is not a new long-duration printing qualification.

**Credits:** Spanish by **[asiersan](https://github.com/asiersan)** ([#130](https://github.com/Neme77/centauri-carbon-2-community/pull/130), integration #133); Hungarian refinements by **[pezso92](https://github.com/pezso92)** ([#129](https://github.com/Neme77/centauri-carbon-2-community/pull/129), integration #132); localized punctuation by **[Surfoo](https://github.com/Surfoo)** ([#128](https://github.com/Neme77/centauri-carbon-2-community/pull/128), integration #135). Advanced mesh builds on **beanbo's #104**. Thanks to the beta testers for the reports and real-printer trials; [full credits](docs/CONTRIBUTORS.md).

**Library compatibility:** older applications enforcing a 40 °C minimum cannot read measurements created at 30–39 °C, and may discard uncalibrated profiles when saving. Back up the plate library before reverting to an older application. Experimental Canvas ejection, continuous Canvas UDS and webcam-service restart are excluded.

**Italiano:** la 1.2.14 aggiorna esclusivamente CC2 Control su V4.4/V4.4.1. Include esclusione oggetti corretta, stato Canvas, dimensioni del live, cursori ventole, lingue e gestione piatti migliorata. Si possono creare profili prima della prima mesh, calibrare da 30 °C e cambiare scheda senza perdere la destinazione. Installazione dal pannello con codice LAN e conferma separata; firmware e callback restano invariati.


## CC2 Control 1.2.11 — stable application update — 2026-10-14

[Release](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.11-V4.4) · [English installation](docs/UPDATE_CC2.md) · [Italiano](docs/UPDATE_CC2.it.md).

- Hide installed/older release descriptions in Settings → Updates.
- Select localized descriptions from the signed catalog in all eight app languages without another catalog fetch; explicitly label English fallback.
- Preserve all 1.2.10 features, contributor credits and saved configuration. Firmware/callback and MQTT Canvas are unchanged; only Community Firmware V4.4/V4.4.1 is supported.
- Publish a signed stable catalog for panel-based discovery, download/verification and separately confirmed installation with the configured LAN code.

[PR127](https://github.com/Neme77/centauri-carbon-2-community/pull/127): all seven CI jobs passed; the maintainer accepted the signed candidate and authorized stable distribution. Source/payload identity and Ed25519 signature verified. No new prolonged-print campaign is claimed. Signed package SHA-256: `2d08659473b4245bf6f43434857a143338af4342dfd6c40da19b8d26faa23074`.

## Community Firmware V4.4.1 — CC2 Control 1.2.10 — 2026-10-14

[Complete firmware and public builder](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/V4.4.1) · [First installation](docs/FIRST_RUN.md) · [Italiano](docs/FIRST_RUN.it.md).

- Combine the qualified CC2 Control 1.2.10 application and signed updater with the complete firmware: Settings → Updates works from first installation without a separate computer-assisted bootstrap.
- Stage the verifier, HTTPS downloader, CA bundle, public key, runner and notices in immutable rootfs. Use the qualified ARM archiver and require the complete helper set.
- Explicitly persist the advanced plate/nozzle library under `/opt/usr/cc2-control/bed-plates.json`.
- Preserve ELEGOO base 02.01.01.07, native callback/firmware patches and application compatibility family 4.4. Complete firmware artifact numbering is 4.4.1.
- Retain the application features and contributor credits listed below. Canvas uses MQTT; PR103/114 and full Canvas UDS remain excluded.
- Explain separate LAN-code authorization for package preparation/installation, with first-installation and update guides in English and Italian.

Integration [PR126](https://github.com/Neme77/centauri-carbon-2-community/pull/126): all seven CI jobs passed; the exact signed R3 artifact passed its WSL build and the maintainer confirmed the requested installation/connection/Canvas/profile-retention/restart trial. SHA-256: `61356c3a83be374988d6154d429c27c403985974663d3780b194164e774a22c1`. Earlier beta long-print reports apply to those tested builds, not a new long-duration test of this exact image.

## CC2 Control 1.2.10 — V4.4-only application update — 2026-10-14

[Release and downloads](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.10-V4.4) · [English installation](docs/UPDATE_CC2.md) · [Italiano](docs/UPDATE_CC2.it.md).

- Integrate Hungarian by pezso92, advanced temperature/nozzle mesh and soaked calibration by beanbo, and the V4.4-only installer with preservation/backup checks.
- Show available internal/mounted USB space and paginate Files/History at 15 items without extra page-driven printer requests.
- Offer persistent camera in Settings → Appearance: one original stream across navigation and hidden/minimized pages; manual Start/Pause, disable, close and viewer handover remain effective.
- Clear old Canvas mappings before external-feed starts and send calibrated multicolour mappings together (beanbo); keep Canvas telemetry on MQTT.
- Correct Orca bed-temperature estimates and sum all filament tool masses; unknown estimates remain unavailable.
- Add signed GitHub update discovery and bounded HTTPS download/verification in Settings, separate installation confirmation, signed local upload, actual firmware fingerprints and configuration-preserving installation.
- Remove the empty file-analysis notification/close icon when the Canvas print dialog opens.
- Retain all previous V4.4 controls, callback runtime and contributor attribution. PR103/114 and continuous Canvas UDS remain excluded.

Merged scope: #104, #115/#116, #117, #118, #119, #120, #121 and #124. Original authors and integration details are listed on the homepage and in [community credits](docs/CONTRIBUTORS.md).

Maintainer confirmed the complete application and camera trial, local signed upgrade, real-printer GitHub HTTPS checksum download, full online signed installation/restart/configuration retention, and the Canvas-dialog UI fix. Required automatic checks passed. Forced-failure rollback has host coverage, not a deliberately induced printer test. Published complete V4.4 firmware assets remain unchanged.

## CC2 Control 1.2.5 — application update for Community Firmware V4.4

- First separate application update for V4.4: Hungarian by pezso92 and temperature/nozzle mesh storage with soaked calibration by beanbo.
- Installer checks actual installed/running fingerprints before service stop and refuses V4.3, original firmware and unknown bases. Requires connected fresh Idle state and cold heaters with zero targets.
- Retains credentials, materials, preferences, plates/nozzles and spools with backup/rollback. Verifies installed binary/UI/locales and reconnection. Canvas stays on MQTT and the firmware/callback runtime is retained.


- Add the complete Hungarian interface (Magyar) by [@pezso92](https://github.com/pezso92), contributed in [#115](https://github.com/Neme77/centauri-carbon-2-community/pull/115). Preserve all 879 entries/placeholders, include the locale in the pinned builder snapshot, and test selection, saved preference, hu-HU detection and mobile layouts. Published V4.4 artifacts remain unchanged.

- Bed mesh calibration takes a bed temperature (40–110 °C, 60 by default), a soak time (10 minutes by default; the firmware itself probes as soon as the bed reads the temperature) and a nozzle, and can save the result to a build plate. It runs on CC2 Control, so the page may be closed, and can be stopped until the probing starts. A plate keeps one measurement per temperature and nozzle; a print started from CC2 Control uses the one nearest to the file's bed temperature (or the one chosen in the print dialog), loaded during the print start without a restart. A nozzle list adds each nozzle's Z correction to the plate's Z offset, so one mesh serves every nozzle; a measurement's temperature and nozzle can be corrected later.
- Clear the printer's Canvas colour table before a print started on the external feed, as the touchscreen does. The table left by an earlier Canvas print made the printer switch Canvas to that print's channel, so a directly fed print (for example TPU with the Canvas cable unplugged) paused at layer 0 with error 1210 or 1220. The print does not start unless the printer acknowledges the clear.
- Send the whole Canvas map of a multicolour print started with bed calibration in one colour-table command, as the printer's own start request does. Each command replaces the printer's table, so the earlier one-command-per-tool script left only the last tool mapped and the other colours were not switched.

## Community Firmware V4.4 / CC2 Control 1.2.3 — 2026-10-13

Built on official **02.01.01.07 only**. [Official release and verified downloads](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/V4.4): complete signed firmware, public source builder and SHA-256 checksums. CC2 Control 1.2.3 is included in the compatible firmware; no standalone application updater is supplied. Do not install this new application separately on V4.3.

- Port the existing firmware patches to the new base, retaining the qualified O0 callback runtime, authentication, native camera integration and protected live Z offset.
- Correct LAN/WAN transitions and registration recovery. Native light commands receive correlated acknowledgements and synchronize the printer display; own-printer PONG freshness keeps the connection indicator aligned with the link.
- Preserve UDS telemetry and homing/light readback; expose homed axes and light in diagnostics. Keep Canvas on the bounded MQTT path, without the full Canvas UDS experiment.
- Restore Input Shaper tools and add PID/Input Shaper Quick Actions; keep guarded hotend/bed PID calibration and fresh-state checks.
- Align Control panel cards and add fan percentage inputs (0–100) applied with Enter alongside the sliders, with input validation and readback.
- Accept internal spaces in material profile names, including Canvas commands, retaining the 16-character limit and command-character guards. Replace the crowded preheat button row with a full-width themed selector; selection loads targets and Apply sends them. Card dimensions and alignment are preserved.
- Add confirmed reset of material, colour and temperature records for all four Canvas slots while idle. It does not unload filament or delete the spool inventory. Empty slot icons are dimmed and neutral according to printer status.
- Keep live camera streaming while the page scrolls; retain manual stop, hidden-tab/navigation suspension and viewer handover.
- Improve G-code filament colour parsing and Canvas print mapping explanations.
- Add complete Polish and Czech UI translations and language preference tests. The interface now supports English, Italian, French, Simplified Chinese, Russian, Polish and Czech, with six themes.
- Retain spool tracking, the existing build-plate library, saved A/B meshes, adaptive/2D/3D mesh display, four-screw tramming, print tuning, native reports, history/timelapse, file management, Orca upload/print and filament sync, emergency recovery and the Panda Breath bridge.
- Add English/Italian first-installation and OrcaSlicer/Canvas guides linked directly from the homepage, with contributor and beta-tester acknowledgements.

**Reported beta validation:** 40-hour active printing sessions without reported problems and more than 20 confirmed long tests. Minor beta-reported issues were corrected. The latest updated builder was installed and positively tested by the maintainer. These reports cover the beta cycle; final release provenance and checksums must identify the exact published artifact.

PR103/104 and the experimental full Canvas UDS 1.2.4 variant remain excluded. The existing build-plate library is included; the temperature/nozzle mesh and soaked-calibration extension from PR104 is not.

## Community Firmware V4.3 / CC2 Control 1.2.0 — pre-release, 2026-10-09

- Publish the tested signed firmware, standalone callback updater, updated source builder and checksums. The source includes the effective-Z-offset wording clarification; the attached tested binaries predate that wording change.

- Integrate the fingerprint-gated experimental O0 callback runtime for vendor memory retention, with native preflight, fallback and a disable switch. The request log remains unbounded; encouraging hardware reports do not prove every OOM/803 cause is resolved.
- Provide a standalone CC2 + callback updater without reflashing; preserve spool inventory during rollback. Firmware and updater use matching CC2/module binaries.
- Analyze large G-code asynchronously with bounded queue/cache; correct expiry of queued complete HTTP requests.
- Add hotend/bed PID calibration, printing temperature changes, and printer error popups that do not recur solely from historical events.
- Align Control and start CC2 30 seconds after the vendor process appears.


- A new Spools page tracks filament spools in the Canvas slots and on the external holder, charged with what the printer really extrudes, purges and cancelled prints included. New filament in a slot asks on every open page which spool it is; a spool that runs out is set to zero; weigh-ins correct the count; the print dialog warns when a spool holds less than the file needs. The inventory is grouped by manufacturer, kind and colour, with identical spools on one row. Off until switched on.
- PID calibration and printer error reports are translated into Russian and Chinese; they were shown in English.
- Bed Levelling has a build-plate library: save each plate's mesh and Z offset once and mount the plate again later. A plate whose mesh is not in its side's slot is written there and the printer restarts.
- Bed mesh calibration from Bed Levelling homes X, Y and Z first when the printer is not homed, instead of being refused by the console.
- An OrcaSlicer upload whose name already exists is saved as `name (1).gcode` and so on instead of being refused with HTTP 409; nothing is overwritten.
- CC2 Control registers with the printer even when another app's Canvas reply arrives first after it connects. Before, it then never registered, so print and Canvas commands were refused and the updater rolled back.
- The mainboard fan no longer runs on forever after homing or calibration. The firmware never releases idle motors, and that fan runs while they are powered, although every fan in the UI reads 0 %. CC2 Control now sends `M84` after ten idle minutes with the heaters off, as stock Klipper does.
- Reduce requests that feed memory retention in the vendor firmware. CC2 Control previously asked it for `info` every two seconds. Now it asks only when the telemetry stream has been silent for three seconds; a working printer sends about two updates per second.
- The Job page no longer queries the printer every two seconds for print objects: the excluded and current objects come from the telemetry stream, the object list is asked once per job, and an idle Job page asks for nothing. Before a print defines its objects, the page says no objects are reported instead of "Object status unavailable".

## CC2 Control 1.1.31fix — 2026-10-04

- Restore and stabilize 3D mesh viewing, including meshes with large height offsets and camera rotation.
- Bound automatic Canvas discovery across MQTT reconnects and expose diagnostic counters; explicit Sync remains available.
- Identify G-code tools by colour and material during Canvas print setup.
- Fix intermittent uploads when headers and file data arrive together; report duplicate-file refusals clearly.
- List the newest 128 G-code files and show when older files are hidden; move upload controls above the file list.
- Windows launcher includes explicit SSH host-key recovery and forwards command-line arguments.
- Printer-tested installer includes all merged fixes; the thumbnail-priority experiment was discarded.

Internal service version remains 1.1.31. This standalone update does not replace the V4.2-R5 firmware release.

## Earlier integrated changes

- Files lists the 128 newest G-code files of a storage instead of an arbitrary 128, and says when older files are hidden.
- A browser upload refused by the printer, for example because the file already exists, reports that error instead of a network error.
- Show the G-code upload card above the Files list (above the file details on wide layouts), so a long list no longer pushes it out of reach.
- Frame the 3D bed mesh around its actual height range so meshes with large positive or negative offsets remain visible.
- Bound automatic Canvas discovery across MQTT reconnects, allow explicit Sync recovery and expose discovery counters in MQTT diagnostics.

- Print setup shows each G-code tool’s filament colour and material beside its Canvas slot selector, with an explicit fallback when metadata is missing.

- Select timelapse recording in the print popup, including calibrated starts.
- Delete individual or loaded completed print-history entries with confirmation and Idle guards.
- Coordinate live camera viewing across CC2 Control windows; failed ownership requests do not open a stream.
- Follow native timelapse rendering until machine state 12 ends before refreshing history.

- Keep object-query UDS sessions open, match response IDs and reject incomplete
  replies to reduce connection churn when opening Job. One supervised print
  completed without recurrence of the observed vendor dispatcher crash; its
  underlying cause remains unconfirmed.
- Show the real internal-light state in Dashboard Quick Actions.
- Require fresh idle telemetry for all-heaters-off commands and disable their
  buttons during printing.
- Let the standalone updater wait up to 240 seconds for startup and registration.

- Delay CC2 Control startup for 60 seconds after detecting `elegoo_printer`
  to give vendor hardware initialization more time to settle.

### Features

- Firmware builder: pin the complete printer-validated CC2 Control integration
  from commit `00f1f89`; run all component host tests and reject stale prepared
  manifests using the source commit and archive checksum. Full OTA validation
  remains separate from the successful CC2 Control beta tests.

- Translations are keyed by identifier (`files.upload_file`) instead of by the English sentence, so the English text can be reworded without touching the other languages.
- The interface follows the browser language (English, Italian, French, Chinese or Russian) until a language is saved on the printer.
- Added a Simplified Chinese (`zh`) interface translation, selectable in **Settings → Appearance**.
- Added a Russian (`ru`) interface translation, selectable in **Settings → Appearance**.
- Redesigned web interface: full-width header with the emergency stop, a side
  menu that collapses to icons, a link for every section (`#files`, `#bed`…) and
  Lucide icons throughout.
- One printer-link indicator (connected, waiting for the printer, reconnecting,
  unreachable) instead of two always-green badges.
- In-page confirmations, red dismissible error messages, print progress in the
  browser tab title, drag-and-drop G-code upload and command history in the
  console.
- Every message, machine state and accessible name is translated in Italian,
  French, Chinese and Russian.
- Four more colour themes (Dracula, Nord, Monokai, Solarized Light) next to Light and Dark, chosen in **Settings → Appearance** and saved on the printer like the language; `/api/preferences` now accepts any lowercase-hyphenated theme identifier.
- The machine state is followed by what the printer is doing within it (**Printing · Heating bed**, **Manual homing · Failed**…), using the vendor sub-state codes.
- Printer refusals of MQTT requests are shown with the vendor meaning of their code (busy, print file missing, no bed levelling data…) instead of passing silently after `202 Accepted`; `/api/printer` reports the latest one as `printer_error`.
- Invalidate the Canvas auto-refill readback on MQTT disconnect and request it
  again after registration, so external changes made while offline are shown.
- **Canvas → Auto refill** shows and changes the Canvas setting that continues from another slot with the same filament when a spool runs out.
- New **History** page: the print jobs recorded by the printer with start time, duration and result; ready time-lapse videos can be downloaded and recorded frames rendered into a video while the printer is Idle. The LAN access code stays on the printer.

### Removed

- Interface elements that showed nothing real: the printer-name field, the
  storage summary computed from the file list, the invented expert-console
  timeout and duplicated cards on the dashboard, job and control pages.

## CC2 Control 1.1.31 — 2026-09-27

### Features

- Added OrcaSlicer Moonraker-agent Canvas filament synchronization through
  read-only `/server/info` and `/server/database/item?namespace=lane_data`
  compatibility endpoints.
- Added LAN access-code replacement and revalidation from **Settings →
  Connection**.

### Fixes

- LAN-code changes are written atomically and restart only CC2 Control, leaving
  printer services and an active print untouched.
- Canvas tray material, colour and nozzle temperature are exposed to OrcaSlicer
  without adding a new polling loop or MQTT subscription.

Thanks to **@efiten** for the original OrcaSlicer compatibility proposal,
investigation and printer-side validation.

## V4.2 — CC2 Control 1.1.30 — 2026-09-27

### Features

- Unified Bed Levelling workflow.
- Added Side A and Side B saved-mesh visibility.
- Added screw corrections in microns and guarded reference optimisation.
- Added operational dashboard Quick Actions and global emergency stop.
- Added compact settings panels and persistent dark/light themes.
- Simplified print preparation to Side A/Side B plus one calibration option.
- Automatically uses the selected side's saved mesh, adaptive G-code probing or
  full-bed calibration as appropriate.

### Fixes

- Fixed file-manager, upload, Canvas, thumbnail and print handling.
- Removed remaining demonstration values from live job and object statistics.
- Completed elapsed, remaining and total-layer status handling.
- Added first-run service realignment after LAN-code registration, removing the
  old post-install manual reboot/power-cycle requirement.
- Rejected implausible G-code temperature metadata instead of displaying values
  such as 6211 °C in the file preview.
- Restored fast thumbnail display by loading the image before full metadata and
  caching metadata by storage, path, size and modification time.
- Requires full-bed calibration when the selected side has no saved mesh.

## V4.1 — CC2 Control 1.1.25 — 2026-09-26

### Features

- Integrated the local CC2 Control service.
- Established the V4.1/R8 reproducible builder snapshot.

### Fixes

- Fixed persistent-storage startup ordering.

Earlier release details remain available through GitHub Releases, tags and Git
history.
