# Panel enhancements under validation

## Storage and lists

The file endpoint reports filesystem capacity, available bytes and free bytes for internal storage. USB space is unavailable when the configured USB path is absent or is an ordinary directory on the internal filesystem. Values are read during the existing file-list request; page navigation does not query the printer.

File Manager and History show 15 items per page. Filtering and sorting reset the File Manager page. Deleting the last item on the last page clamps the page to the remaining results. File selection is retained across pages; Select all selects only the current page. Clear History still applies to all completed tasks, with the existing confirmation.

## Live view

Settings → Appearance stores camera_persistent on the printer, default false. When enabled, the existing full-size camera card keeps the same image and stream across menu navigation, including a hidden tab or minimized browser. It appears in the normal Dashboard/Job camera position and is parked out of view on other pages; no floating window is created. Live view still requires an explicit Start. Pause, disabling the option, closing/leaving the page and a confirmed viewer takeover stop the request. A reload restores the option but never automatically starts video. Browser/OS process suspension or loss of network can still interrupt transmission. Persistent live view retains the resources of an active camera; it is not a RAM optimization.

The normal per-page stream remains unchanged with the option disabled. The live panel and separate camera window retain the existing one-viewer ownership rule.

## Signed application updates under validation

Settings → Updates now prepares signed local or GitHub-distributed application
packages. See [English instructions](APPLICATION_UPDATES.md) and
[Italian instructions](APPLICATION_UPDATES.it.md). The standalone TLS helper
does not depend on the printer's curl. The stable and experimental Canvas UDS
channels remain separate. Printer qualification is pending; firmware updating
through the original display path remains a separate future task.
