# CC2 Control 1.2.7 — prova combinata, solo V4.4

Aggiorna esclusivamente CC2 Control. Non aggiorna il firmware della stampante,
elegoo_printer o la libreria callback. Compatibile soltanto con Community
Firmware V4.4 su base ELEGOO 02.01.01.07: l'installer verifica i binari effettivi.

Include PR117, 118, 119 e 120: installazione con backup/ripristino, lingua
ungherese, mesh avanzata, spazio disponibile, pagine da 15 file, live persistente
opzionale e correzioni Canvas e metadati OrcaSlicer. Conserva le precedenti
correzioni. Sono esclusi PR103/114, Canvas UDS continuo e updater dalle
Impostazioni. Un solo comando UDS confermato prepara gli avvii da bobina esterna;
la telemetria Canvas continua a usare MQTT.

## Installazione

1. Estrai completamente lo ZIP sul computer. Non copiare lo ZIP alla stampante.
2. Attendi la fine di stampe/calibrazioni; chiudi il live. La stampante deve
   essere connessa, inattiva, con ugello e piatto a massimo 50 °C e target zero.
3. Su Windows avvia `Install-Windows.cmd` e segui le richieste. Su Linux/macOS
   usa `sh ./install-cc2-control.sh INDIRIZZO-IP-STAMPANTE`.
4. Per SSH usa `root` e la tua password; quella predefinita è `MTY4ODE2`.
   La password SSH è distinta dal codice LAN. Non eliminare la configurazione:
   codice LAN, preferenze, materiali, piatti, ugelli e bobine vengono conservati.
5. L'installer crea il backup e verifica versione, binario in esecuzione e
   connessione MQTT. Attendi l'esito completo e conserva il percorso del backup
   e il comando di ripristino stampati a video.
6. Ricarica il browser e verifica versione 1.2.7; riavvia una volta da inattiva
   e ricontrolla connessione, Canvas e avvio automatico.

Se l'installer rifiuta la compatibilità o lo stato della stampante, non saltare
i controlli. Se fallisce, conserva il primo errore e l'esito del ripristino.
Non ripristinare le impostazioni di fabbrica per questa prova.

## Verifiche prima del merge

- Lingua ungherese e mesh avanzata; quick action e comandi già funzionanti.
- Spazio interno/USB, seconda pagina di File e Cronologia, filtri e selezione.
- Impostazioni → Aspetto: attiva il live persistente e avvialo; cambia pagina.
  Il riquadro resta quello normale di Dashboard/Stampa, senza finestra flottante.
  Minimizza il browser: il video deve continuare. Pausa, disattivazione del flag
  o chiusura della pagina lo fermano; un reload richiede un nuovo avvio manuale.
  Disattivando l'opzione si deve recuperare il comportamento precedente.
- Il file OrcaSlicer segnalato deve mostrare piatto **55 °C**, ugello **220 °C**
  e filamento stimato **21,2 g**. Non è il consumo reale del tracciamento bobine.
- PR119: avvio da bobina esterna dopo un precedente lavoro mappato Canvas,
  seguendo la normale configurazione prevista dalla stampante; poi avvio
  multicolore con calibrazione del piatto e tutti gli utensili mappati.
  Una mancata conferma dell'azzeramento deve impedire l'avvio.

La prova dell'autore di PR119 su alimentazione esterna è su V4.2-R5; il
comportamento su V4.4 e l'avvio multicolore calibrato richiedono questa prova.
Le PR restano aperte fino al riscontro sulla stampante. Questo pacchetto non
sostituisce la release ufficiale pubblicata.

## Crediti

@beanbo per mesh avanzata e PR119; @pezso92 per l'ungherese; grazie al beta tester
che ha fornito il G-code per riprodurre PR120 e a tutti i tester già ringraziati
nel progetto. L'integrazione conserva i commit originali di PR119.
