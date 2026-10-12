# Centauri Carbon 2 Community Firmware

Community firmware, local printer control and reproducible build tools for the **ELEGOO Centauri Carbon 2**, maintained by **Neme77**.

> Independent community project. Not affiliated with, endorsed by or supported
> by ELEGOO.

## Start here / Guide rapide

| Guide | English | Italiano |
| --- | --- | --- |
| First installation, printer IP and LAN access code | [First setup](docs/FIRST_RUN.md) | [Prima installazione](docs/FIRST_RUN.it.md) |
| OrcaSlicer connection, upload and Canvas filament sync | [OrcaSlicer guide](docs/ORCASLICER.md) | [Guida OrcaSlicer](docs/ORCASLICER.it.md) |
| All V4.4 improvements and retained features | [Changelog](CHANGELOG.md#unreleased) | [Novità della release](CHANGELOG.md#unreleased) |

## V4.4 release preparation

**V4.4 / CC2 Control 1.2.3 is in final beta validation, built on official CC2 base 02.01.01.07. Its official release has not been published yet.** The installation guides above cover this upcoming release; use the existing released downloads below until the V4.4 artifacts and their checksums are published.

| V4.4 highlight | What changes for the user |
| --- | --- |
| New official base and connection fixes | Ported firmware patches, LAN/WAN reconnection and native light commands with display-state acknowledgement. |
| Control panel | Aligned cards, fan percentage fields applied with Enter, guarded PID/Input Shaper tools and calibration Quick Actions. |
| Material profiles | Names such as **PLA MATTE** are accepted; a themed dropdown shows preheat presets without squeezing names into a row of buttons. Selecting a preset loads the fields; **Apply temperatures** starts heating. |
| Canvas | Confirmed reset of the four filament records; empty slots are dimmed without a colour. MQTT Canvas telemetry and Orca filament sync remain supported. |
| Camera | Live viewing continues while scrolling; hidden-tab, navigation and viewer-ownership controls remain active. |
| Languages | English, Italian, French, Simplified Chinese, Russian, **Polish and Czech**. |

The O0 callback runtime, protected live Z offset, spool inventory, build-plate library, mesh tools, history/timelapse, file uploads/downloads and Panda Breath compatibility are retained. PR103/104 and the experimental full Canvas UDS variant are not included in this candidate.

**Beta evidence reported by the maintainer and testers:** prints running for 40 hours without reported problems, more than 20 confirmed long tests, and reported minor issues corrected. These are community hardware results, not a guarantee for every printer or proof that every test used the final release artifact.

## Current release

| Component          |                                                                                         Version |
| ------------------ | ----------------------------------------------------------------------------------------------: |
| Community firmware |                 [V4.2-R5](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/V4.2-R5) |
| CC2 Control        | [1.1.31fix](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.1.31fix) |

Release binaries, standalone updaters, source archives and checksums are kept with the corresponding GitHub Release. The canonical development source is kept directly on `develop`.

## Experimental pre-release V4.3

[V4.3 is available as a pre-release](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/V4.3), with CC2 Control 1.2.0, the experimental O0 callback runtime,
all merged features through PR97, and the first-boot preparation fix.
The release includes complete signed firmware, a standalone callback updater,
the updated source builder, and checksums. The notes explain the original
vendor retention issue, test evidence, installation routes, and limitations.

The attached tested binaries predate the effective-Z-offset wording correction;
the updated source builder includes it and requires rebuilding to include that
wording in binaries. The stable download links below remain V4.2-R5 and 1.1.31fix.
See [candidate notes](docs/RELEASE_CANDIDATE.md) for details.

## Downloads

<p align="center">
  <a href="https://github.com/Neme77/centauri-carbon-2-community/releases/download/V4.2-R5/CC2_V4_2_STOCK_20261004_135126_9cead4b7.zip.sig"><img alt="Download Community Firmware V4.2-R5" src="https://img.shields.io/badge/Download-Firmware%20V4.2--R5-00b8d9?style=for-the-badge"></a>
  <a href="https://github.com/Neme77/centauri-carbon-2-community/releases/download/CC2-Control-1.1.31fix/CC2-Control-1.1.31fix-Multiplatform-Update.zip"><img alt="Download CC2 Control 1.1.31 updater" src="https://img.shields.io/badge/Update-CC2%20Control%201.1.31-22c55e?style=for-the-badge"></a>
  <a href="https://github.com/Neme77/centauri-carbon-2-community/releases/download/CC2-Control-1.1.31fix/CC2-Control-1.1.31fix-Source.zip"><img alt="Download CC2 Control 1.1.31 source" src="https://img.shields.io/badge/Source-CC2%20Control%201.1.31-6f42c1?style=for-the-badge"></a>
</p>

<p align="center">
  <a href="https://github.com/Neme77/centauri-carbon-2-community/releases/download/V4.2-R5/CC2-Builder-V4.2-R5-CC2-Control-1.1.31.zip"><img alt="Download firmware builder R5" src="https://img.shields.io/badge/Builder-Firmware%20R5-f59e0b?style=for-the-badge"></a>
  <a href="https://github.com/Neme77/centauri-carbon-2-community/releases/download/V4.2-R5/SHA256SUMS_V4_2_R5_RELEASE.txt"><img alt="Download SHA256 checksums" src="https://img.shields.io/badge/Verify-SHA256SUMS-64748b?style=for-the-badge"></a>
</p>

<p align="center">
  <a href="https://github.com/Neme77/centauri-carbon-2-community/releases/tag/V4.2-R5"><strong>Release notes, checksums and all V4.2-R5 downloads</strong></a>
</p>

| Choose this file                                                                                                                                                                                                      | When to use it                                                                                          |
| --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- |
| [`CC2_V4_2_STOCK_20261004_135126_9cead4b7.zip.sig`](https://github.com/Neme77/centauri-carbon-2-community/releases/download/V4.2-R5/CC2_V4_2_STOCK_20261004_135126_9cead4b7.zip.sig)                                     | New installation or complete Community Firmware V4.2-R5 update                                             |
| [`CC2-Control-1.1.31fix-Multiplatform-Update.zip`](https://github.com/Neme77/centauri-carbon-2-community/releases/download/CC2-Control-1.1.31fix/CC2-Control-1.1.31fix-Multiplatform-Update.zip)                               | Existing Community Firmware installation: update only CC2 Control, without reflashing the full firmware |
| [`CC2-Control-1.1.31fix-Source.zip`](https://github.com/Neme77/centauri-carbon-2-community/releases/download/CC2-Control-1.1.31fix/CC2-Control-1.1.31fix-Source.zip) | CC2 Control source; updater packaging tools are in `cc2-control/installer/`                                            |
| [`CC2-Builder-V4.2-R5-CC2-Control-1.1.31.zip`](https://github.com/Neme77/centauri-carbon-2-community/releases/download/V4.2-R5/CC2-Builder-V4.2-R5-CC2-Control-1.1.31.zip)                               | Firmware builder R5                                                                                     |
| [`SHA256SUMS_V4_2_R5_RELEASE.txt`](https://github.com/Neme77/centauri-carbon-2-community/releases/download/V4.2-R5/SHA256SUMS_V4_2_R5_RELEASE.txt)                                                                                             | Verify downloaded release files                                                                         |

## Already running an older Community Firmware?

If your printer already runs an earlier Community Firmware release, you do not need to reflash the complete firmware just to update CC2 Control.

Use the standalone updater:

[`CC2-Control-1.1.31fix-Multiplatform-Update.zip`](https://github.com/Neme77/centauri-carbon-2-community/releases/download/CC2-Control-1.1.31fix/CC2-Control-1.1.31fix-Multiplatform-Update.zip)

The standalone updater replaces only CC2 Control and preserves the persistent LAN access code, material presets and interface preferences. CC2 Control 1.1.31 also adds LAN-code revalidation and OrcaSlicer Moonraker-agent Canvas filament synchronization. After the update, CC2 Control realigns its local services automatically, so an additional printer reboot or full power cycle should not be necessary.

For detailed instructions, see [Installation](docs/INSTALL.md).

## Changed the printer LAN access code?

Open `http://PRINTER-IP:8081`, go to **Settings → Connection**, enter the replacement code and choose **Change / Revalidate**, then confirm. This saves the code and restarts the CC2 Control service; a separate “Save changes” step is not required for the LAN code.

Read [first setup and LAN-code recovery](docs/FIRST_RUN.md#if-the-lan-code-changes) or [prima configurazione e codice LAN](docs/FIRST_RUN.it.md#se-cambia-il-codice-lan). Advanced SSH recovery remains in [Installation](docs/INSTALL.md#recovery-if-the-old-code-blocks-configuration-access).

## Features

| Area | What you can do |
| --- | --- |
| Dashboard and print tuning | Monitor temperatures, fans, print progress and live movement; adjust **print speed (25–200%)** and **extrusion flow (50–150%)** during an active or paused print with fresh telemetry. |
| File manager | Browse internal and USB storage, search and sort files, inspect model thumbnails and print metadata, upload G-code, **download G-code to your computer**, delete files and start a protected print. |
| Canvas and print continuity | View four material slots, colours and presets; select, load or unload a spool; enable **Auto refill** so the printer can continue from another slot holding the same filament when one spool runs out. |
| Spools | Keep a filament inventory that knows which spool is in which Canvas slot or on the external holder, charged with what the printer really extrudes. New filament in a slot asks which spool it is, a spool that runs out is set to zero, weigh-ins correct the count, and the print popup warns when a spool holds less than the file needs. Off until switched on. |
| Print popup and timelapse | Choose filament-slot mapping and bed side before printing, use a saved mesh or a calibrated start, and **enable timelapse recording for that print**. |
| History | Review jobs recorded by the printer, including start time, duration and result; download ready timelapse videos and render recorded frames into a video while Idle. Delete individual records or clear loaded completed records with confirmation; G-code files are preserved. |
| Camera | Start live viewing on request, take snapshots and open the viewer in a separate window. CC2 coordinates camera ownership between its pages and windows and releases the viewer when it closes; the vendor camera service remains active. In V4.4, scrolling away from the camera card does not stop the stream. |
| Printer control | Home and move axes, set nozzle and bed targets, use material temperature presets, control fans and lights, extrude filament and adjust the live session Z offset. V4.4 adds numeric fan inputs and the themed preheat selector. |
| Bed levelling | View the mesh in 2D or interactive 3D, inspect values and saved profiles, choose Side A / Side B, use adaptive probing and measure four-screw adjustment with the nozzle load cell. |
| Quick Actions and safety | Configure dashboard shortcuts with real light-state feedback; use object exclusion, a protected console and global emergency stop. Heater-off actions require fresh Idle telemetry. |
| Printer status and connection | See native machine sub-states and refusal messages; obtain telemetry through persistent UDS sessions, discover the printer serial automatically using saved LAN credentials, and revalidate a changed LAN code. |
| Slicer and integrations | OrcaSlicer upload, Device UI and Moonraker-agent Canvas filament sync use port **8081**. The separate read-only Panda Breath compatibility bridge uses port **7125**. |
| Interface | Use English, Italian, French, Simplified Chinese, Russian, Polish or Czech; choose Light, Dark, Dracula, Nord, Monokai or Solarized Light themes; save appearance preferences and collapse the navigation menu. |

## Community contributions

Our sincere thanks to the contributors whose code, ideas, translations and validation helped build CC2 Control. **V4.4 retains their work from throughout the project's history**; credit follows original authorship, including work integrated through a later maintainer PR.

| Contributor | Contributions included in the project |
| --- | --- |
| **[@Surfoo](https://github.com/Surfoo)** | The complete interface redesign, Control-page and mobile/tablet improvements; French translation and JSON localization; CI, sanitizers and ARM smoke tests; upload-slot fixes, byte-exact inputs, LAN-code visibility and release/version tooling. Selected PRs: [#24](https://github.com/Neme77/centauri-carbon-2-community/pull/24), [#15](https://github.com/Neme77/centauri-carbon-2-community/pull/15), [#19](https://github.com/Neme77/centauri-carbon-2-community/pull/19), [#50](https://github.com/Neme77/centauri-carbon-2-community/pull/50). |
| **[@beanbo](https://github.com/beanbo)** | Spool inventory, the original build-plate library, persistent UDS subscriptions, printer states/refusals, Canvas auto-refill, camera handover, history, upload improvements, idle motor release and Russian translation. Selected PRs: [#97](https://github.com/Neme77/centauri-carbon-2-community/pull/97), [#83](https://github.com/Neme77/centauri-carbon-2-community/pull/83) → [#89](https://github.com/Neme77/centauri-carbon-2-community/pull/89), [#88](https://github.com/Neme77/centauri-carbon-2-community/pull/88), [#67](https://github.com/Neme77/centauri-carbon-2-community/pull/67). |
| **[@efiten](https://github.com/efiten)** | Original OrcaSlicer/Canvas filament-sync implementation proposal, analysis and printer-side validation: [#8](https://github.com/Neme77/centauri-carbon-2-community/pull/8), integrated through [#12](https://github.com/Neme77/centauri-carbon-2-community/pull/12). |
| **[@Skcycos](https://github.com/Skcycos)** | Simplified Chinese interface translation: [#26](https://github.com/Neme77/centauri-carbon-2-community/pull/26). |

Special recognition to **Surfoo** for reshaping the interface and its development foundations, and to **beanbo** for the extensive additions to everyday printing and printer integration. Thank you to every contributor for the care, time and follow-through behind these improvements.

We also thank **[@sauadhi](https://github.com/sauadhi)** for the full Canvas filament-ejection work in [#103](https://github.com/Neme77/centauri-carbon-2-community/pull/103), and **beanbo** for the further mesh/calibration work in [#104](https://github.com/Neme77/centauri-carbon-2-community/pull/104). These two extensions remain under review and **are not included in V4.4**; acknowledging the contribution does not announce its release.

**[@Neme77](https://github.com/Neme77)** maintains the project, develops and integrates the firmware/control fixes, coordinates hardware validation and prepares release artifacts. The recent integration includes the Control/Canvas changes, Polish/Czech translations and new firmware-base port ([#107](https://github.com/Neme77/centauri-carbon-2-community/pull/107), [#108](https://github.com/Neme77/centauri-carbon-2-community/pull/108), [#109](https://github.com/Neme77/centauri-carbon-2-community/pull/109), [#110](https://github.com/Neme77/centauri-carbon-2-community/pull/110)).

See [the detailed contributor credits and original PRs](docs/CONTRIBUTORS.md).

### Beta testers

Special thanks to **DamiBFryta**, our very first beta tester, for being reliable, impartial and always available throughout development.

Our sincere thanks also go to **Barry Green**, **Michael Husbyn**, **[Jiří Heider](https://www.facebook.com/heiderakcze)**, **James Morriss Hill** and **Brian Mcmillion**, and to **all the other beta testers not named here**. By offering their time and their printers, running long printing sessions and providing patient, precise feedback, they helped identify issues and improve the stability of this release.

## Source layout

| Path                                   | Contents                                                    |
| -------------------------------------- | ----------------------------------------------------------- |
| [`cc2-control/`](cc2-control/)         | C backend, web UI, scripts, tests and protocol notes        |
| [`builder/current/`](builder/current/) | Current firmware build logic and host-side tests            |
| [`docs/`](docs/)                       | Installation, architecture, build and testing documentation |
| [`CHANGELOG.md`](CHANGELOG.md)         | User-visible project history                                |
| [`AGENTS.md`](AGENTS.md)               | Repository working rules for contributors and coding agents |

Historical versions are preserved through Git tags, GitHub Releases and Git history instead of parallel version-specific documents on `main`.

## Documentation

- [First installation and LAN access code](docs/FIRST_RUN.md) / [Italiano](docs/FIRST_RUN.it.md)
- [OrcaSlicer and Canvas sync](docs/ORCASLICER.md) / [Italiano](docs/ORCASLICER.it.md)
- [Installation and advanced recovery](docs/INSTALL.md)
- [Build and reproduction](docs/BUILD.md)
- [Architecture](docs/ARCHITECTURE.md)
- [CC2 Control](docs/CC2_CONTROL.md)
- [Source and third-party inputs](docs/SOURCE.md)
- [Testing](docs/TESTING.md)
- [Contributor credits](docs/CONTRIBUTORS.md)
- [Contributing](CONTRIBUTING.md)

## Interface preview

Real-printer screenshots of **CC2 Control 1.2.3**, supplied on 13 October 2026 for the V4.4 release candidate. [Open the complete screenshot gallery](docs/SCREENSHOTS.md) to see all eleven views at full size.

### Dashboard

Live camera, Quick Actions, print status and temperature history in the Light theme.

<p align="center">
  <img src="docs/images/cc2-control/dashboard-light.png" alt="CC2 Control 1.2.3 dashboard with live camera, Quick Actions and temperature history" width="1200">
</p>

### Control panel and Canvas

The Control panel includes numeric fan fields applied with Enter and a preheat dropdown supporting names such as PLA MATTE. Canvas displays empty slots without a filament colour and provides Reset all filaments.

<table>
  <tr><td align="center"><strong>Control and preheat presets</strong></td><td align="center"><strong>Canvas and filament reset</strong></td></tr>
  <tr><td><a href="docs/images/cc2-control/control-presets.png"><img src="docs/images/cc2-control/control-presets.png" alt="Control panel with PLA MATTE preset, fan inputs and live Z offset"></a></td><td><a href="docs/images/cc2-control/canvas.png"><img src="docs/images/cc2-control/canvas.png" alt="Four Canvas slots, dimmed empty slots and Reset all filaments"></a></td></tr>
</table>

### Bed mesh and spool inventory

Interactive mesh inspection and optional tracking of the filament inventory.

<table>
  <tr><td align="center"><strong>3D bed mesh</strong></td><td align="center"><strong>Spool inventory</strong></td></tr>
  <tr><td><a href="docs/images/cc2-control/bed-mesh.png"><img src="docs/images/cc2-control/bed-mesh.png" alt="Interactive 3D bed mesh, saved profiles and height statistics"></a></td><td><a href="docs/images/cc2-control/spools.png"><img src="docs/images/cc2-control/spools.png" alt="Spool inventory and optional tracking controls"></a></td></tr>
</table>

The [complete gallery](docs/SCREENSHOTS.md) also shows PID/Input Shaper and material profiles, the Job page, G-code files and model previews, the protected console, integration checks and the six themes.

## AI-assisted development

This project uses AI-assisted development tools as part of the workflow, including support for code review, refactoring, debugging, documentation and selected code generation tasks.

A substantial part of the codebase is written and maintained directly by the project maintainer. Architecture decisions, reverse engineering, hardware testing, printer-side validation and release approval are performed by the maintainer and community testers.

AI-assisted changes are reviewed and validated before being included in a release.

## License and notices

Project code is provided under GPL-3.0-only where the contributors have the right to license it. Vendor firmware and third-party components keep their own terms. See [`LICENSE`](LICENSE) and [`NOTICE.md`](NOTICE.md).


The **1.1.31fix** standalone update includes stable 3D mesh framing, bounded Canvas discovery, clearer filament mapping, and File Manager upload/listing fixes. Its internal service version remains 1.1.31. The firmware and builder downloads above remain V4.2-R5.
