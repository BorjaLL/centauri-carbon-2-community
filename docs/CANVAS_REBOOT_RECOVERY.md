# Canvas recovery after restart / Recupero dopo il riavvio

## English

If filament remains in the head during restart, the firmware may report head presence while losing the active Canvas channel. The observed state was `active_cid: -1`, four tray status values of `1`, and `filament_detected: true`. It was reproduced on original CC2 Control 1.2.16. Completing the native loading procedure for the confirmed last-used slot on the ELEGOO display restored coherent Canvas state, with additional purging.

CC2 Control now distinguishes this condition from other transitions using the `unassigned_head` reason. Load and Unload stay blocked. While connected and idle, the Canvas controls explain how to verify the last-used slot and complete the native display procedure. Never choose a different filament to load into an occupied head. If the slot is unknown or an external spool is in use, inspect the path and follow the display rather than guessing a Canvas slot.

There is no acknowledgement button, remembered-slot inference, automatic loading, heating, purging, or retry. The alert disappears when fresh printer readback confirms a consistent state. During printing, stale telemetry or lost connection, no recovery-loading instruction is shown. This is not automatic print resumption after power loss, and the original guards remain unchanged.

## Italiano

Se durante il riavvio rimane filamento in testina, il firmware può rilevarlo ma perdere il canale Canvas attivo. Il caso osservato riportava `active_cid: -1`, quattro slot con stato `1` e `filament_detected: true`, anche sulla 1.2.16 originale. Completando dal display ELEGOO il caricamento dello slot precedente, dopo averne verificato l’identità, gli stati si sono riallineati. È avvenuto ulteriore spurgo.

Il pannello distingue ora questa condizione dalle altre transizioni e mostra istruzioni mentre la stampante è connessa e inattiva. Carica e Scarica rimangono bloccati fino alla conferma reale della stampante. Non caricare un altro filamento in una testina occupata; se non conosci lo slot o usi la bobina esterna, controlla il percorso e segui le indicazioni del display.

Nessun caricamento automatico, nessun pulsante per ignorare il blocco e nessuna associazione basata su dati ricordati. L’avviso scompare quando i dati freschi tornano coerenti. Non gestisce la ripresa di una stampa interrotta dalla corrente.

## Validation

The native fixture checks the new reason and continued Load/Unload refusal, plus recovery only after actual channel/status confirmation. The actual browser test covers the captured condition, explanatory text, no outgoing control commands, reload/card selection, printing, stale telemetry, and disappearance after coherent readback. No new physical movement test is required to show guidance; the real display recovery above is recorded separately from automated coverage.
