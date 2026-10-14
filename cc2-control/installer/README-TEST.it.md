# CC2 Control 1.2.8 — prova del modulo aggiornamenti, solo V4.4

Questo pacchetto conserva tutte le modifiche della 1.2.7 validata e aggiorna
esclusivamente CC2 Control. Non cambia firmware o libreria callback.

1. Estrai completamente lo ZIP sul computer.
2. Attendi stampante inattiva, ugello e piatto ≤50 °C, target zero; chiudi il live.
3. Avvia `Install-Windows.cmd`, oppure su Linux/macOS
   `sh ./install-cc2-control.sh INDIRIZZO-IP-STAMPANTE`.
4. Per SSH usa root e la tua password SSH.
   Non cancellare la configurazione: dati, materiali e codice LAN vengono conservati.
5. Attendi l’esito dell’installer e conserva percorso e istruzioni del backup.
6. Ricarica il browser con Ctrl+F5, verifica la versione 1.2.8 e riavvia una volta
   a stampante inattiva. Ricontrolla connessione, Canvas e camera persistente.

Apri Impostazioni → Aggiornamenti. Il canale iniziale deve essere Stabile.
L’indice online viene pubblicato solo quando i pacchetti sono pronti:
la sua assenza non indica un guasto della stampante. Per provare una futura
versione firmata usa “Carica un pacchetto firmato dal computer”. La verifica
non deve installare nulla; serve poi una seconda conferma per l’installazione.
Il codice LAN è distinto dalla password SSH. Non inserire ZIP nel pannello.

La guida completa è inclusa nei sorgenti:
`docs/APPLICATION_UPDATES.it.md` e `docs/APPLICATION_UPDATES.md` (inglese).
Prima del merge vanno verificati sulla stampante download HTTPS, rifiuto di
pacchetti alterati/incompatibili, installazione firmata, riavvio e conservazione
della configurazione. Non ripristinare le impostazioni di fabbrica.
