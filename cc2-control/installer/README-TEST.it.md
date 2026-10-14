# Aggiornamento CC2 Control — esclusivamente Community Firmware V4.4

Il maintainer ha confermato installazione, aggiornamento firmato manuale e online
dal pannello, riavvio, connessione, Canvas e impostazioni conservate. Confermato
anche il fix della notifica di analisi vuota sopra la scelta Canvas.

Estrarre tutto lo ZIP sul computer. A stampante connessa e inattiva, ugello/piatto
<=50 C e target zero, avviare Install-Windows.cmd oppure
sh ./install-cc2-control.sh IP-STAMPANTE. Usare root e la propria password SSH.
Conservare il percorso/comando di ripristino fornito dall'installer; niente reset.
Ricaricare con Ctrl+F5 e verificare la versione della release e /api/health.

Per aggiornamenti successivi: Impostazioni > Aggiornamenti, Controlla, codice LAN,
Scarica e verifica, poi Installa con conferma separata. Sono supportati anche
file locali firmati .cc2update; lo ZIP dell'installer non va caricato nel pannello.
Chi ha gia la versione della release non deve reinstallare.

Guida: https://github.com/Neme77/centauri-carbon-2-community/blob/develop/docs/UPDATE_CC2.it.md
English: https://github.com/Neme77/centauri-carbon-2-community/blob/develop/docs/UPDATE_CC2.md

Firmware/callback invariati e Canvas su MQTT. Rollback forzato coperto da test
host, ma non provocato deliberatamente sulla stampante.
