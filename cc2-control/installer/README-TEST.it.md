# CC2 Control 1.2.19 — aggiornamento completo

**RIAVVIO COMPLETO NECESSARIO: questo aggiornamento comprende il fix webcam.
Al termine dell'installazione, riavviare manualmente la stampante quando è inattiva.
Il solo aggiornamento della pagina o riavvio di CC2 Control non attiva la patch.**

Include i fix camera della PR #183 e la gestione delle date G-code non attendibili
della PR #185. Le librerie webcam sono le stesse provate per sette ore di stampa
con IA attiva e streaming funzionante. Il nuovo installer integrato richiede una
prova completa sulla stampante prima della pubblicazione nel canale stabile.

Aggiornare soltanto con stampante connessa, inattiva, ugello e piatto a non più di
50 °C e target a zero. Chiudere le dirette prima dell'installazione. Il codice LAN
autorizza l'installazione dal pannello; non viene incluso nel pacchetto.

Dal pannello: Impostazioni → Aggiornamenti → carica il file .cc2update firmato.
In alternativa estrarre lo ZIP multiplatform e avviare l'installer per il proprio
sistema operativo. Conservare il percorso del backup indicato alla fine.

Il componente camera controlla hash del firmware, file di avvio e dipendenze;
interrompe l'installazione su configurazioni non riconosciute. Non sostituisce
ai_camera e non aumenta il limite di stream. Una precedente installazione R2
personale riconosciuta viene conservata, inclusa la sua scelta di attivazione.
Non vengono riavviati automaticamente la camera o la stampante.

Dopo il riavvio, verificare versione 1.2.19, connessione, video locale e Matrix.
Per una nuova installazione della camera:

```sh
sh /opt/usr/cc2-control/camera/manage.sh status
```

Chi ha già la precedente installazione personale usa invece:

```sh
sh /opt/usr/cc2-camera-recovery-r2/manage.sh status
```

Il ripristino dell'applicazione dal backup ripristina anche il precedente avvio
della camera se questo aggiornamento l'ha modificato, rifiutando modifiche
successive non riconosciute. Eseguire il ripristino a stampante inattiva e fredda;
riavviare poi completamente per cambiare la patch già caricata in memoria.

Le date palesemente errate dei file sono mostrate come non disponibili, senza
modificare G-code o timestamp originali. L'orologio deve comunque potersi
sincronizzare, anche sulle reti che bloccano l'accesso al cloud.

Grazie a tharje per diagnosi, riproduzione e analisi del problema webcam, e ai
beta tester della community per le prove. I singoli eventi IA e le sessioni RTC
scadute non sono qualificati da una semplice prova con IA abilitata.
