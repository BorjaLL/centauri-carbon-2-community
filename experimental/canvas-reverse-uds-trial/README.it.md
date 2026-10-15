# Prova Canvas R1: retromarcia breve sullo slot 2

**Prova sperimentale locale per V4.4 / V4.4.1, eseguibile SHA256 `19712b5943b10bb8cb6aab9a4ed498ee962b28538b23c91b7ed003eb9e590745`. Non e ancora l'espulsione automatica completa della PR114.**

La prima corsa ha obiettivo **10 mm**, parametro velocita **5**, accelerazione **100**, timeout **3 secondi**. Si usa solo lo slot 2 (canale interno 1), dopo scaricamento normale dalla testa. Gli 840 mm misurati non vengono comandati. Non sono disponibili ripetizione automatica, altri slot o parametri di corsa modificabili dalla riga di comando.

## Cosa cambia

Una piccola libreria corregge in memoria il confronto della distanza negativa in `CANVAS_MOTOR_CONTROL`, all'avvio del processo qualificato. Controlla i 24 byte originali, applica la correzione e ripristina la pagina come sola lettura/esecuzione. Crea un marker con il PID dopo la verifica dei byte corretti.

L'eseguibile su disco resta identico; la libreria callback resta identica. Il launcher del runtime viene sostituito con una variante che aggiunge la libreria sperimentale, mantenendo verifica del binario, verifica dei componenti, percorso originale e attivazione callback. I checksum del launcher nuovo e del modulo vengono aggiunti alla verifica del runtime. Firmware vendor, configurazioni, chiavi e componenti della release non vengono ridistribuiti o riscritti.

Questo pacchetto ha sorgenti e script di compilazione. Prima della compilazione non contiene binari ARM.

## 1. Compilazione sul PC

Estrarre sul PC Windows usato per il builder ed eseguire **Build-Trial.cmd**. Usa Ubuntu WSL e il compilatore gia presente in `/opt/cc2-cross/toolchain-out/bin/arm-cortex_a15-linux-gnueabihf-gcc`.

La compilazione esegue prima i test dei controlli e del trasporto su socket simulato, il controllo sintattico degli script e del modulo; poi controlla formato ARM hard-float e requisiti glibc dei risultati. Qualsiasi errore blocca la build. Incollare il primo errore, senza procedere all'installazione.

## 2. Preparazione della stampante

- Stampa conclusa; stampante inattiva.
- Eseguire lo scaricamento normale dal display per lo slot 2 e attenderne la conclusione. Il filamento deve restare inserito nel Canvas, con testa scarica e nessun canale attivo.
- Disattivare l'auto-inserimento; lasciare ugello e piatto con target zero e attendere temperature entro 50 gradi. Non avviare operazioni dal display o dallo slicer durante la prova.

Da PowerShell nella cartella del pacchetto, sostituire l'IP se necessario:

```powershell
scp -O -r .\output root@192.168.1.103:/tmp/cc2-canvas-reverse-r1
```

Con una destinazione non ancora esistente, la cartella `output` viene copiata come `/tmp/cc2-canvas-reverse-r1`. Verificare che contenga `install-trial.sh` prima di proseguire.

Da SSH:

```sh
sh /tmp/cc2-canvas-reverse-r1/install-trial.sh
```

Lo script richiede il runtime originale integro, con launcher e checksum corrispondenti al componente del firmware. Conserva launcher e lista checksum in `/opt/usr/cc2-reactor-runtime-v2/canvas-reverse-trial-backup`. Non ferma servizi, non riavvia e non muove motori.

## 3. Primo avvio: fermarsi qui per verificare

Riavviare manualmente la stampante inattiva:

```sh
reboot
```

Riconnettersi via SSH. La copia in `/tmp` potrebbe essere scomparsa: copiarla di nuovo dal PC con il comando precedente. NON eseguire di nuovo l'installer.

```sh
sh /tmp/cc2-canvas-reverse-r1/check-trial.sh
chmod 700 /tmp/cc2-canvas-reverse-r1/cc2-canvas-motion-probe
/tmp/cc2-canvas-reverse-r1/cc2-canvas-motion-probe check
```

**Incollare gli esiti prima della prima corsa.** I marker callback e Canvas devono riferirsi al PID attuale; la connessione di CC2 Control deve funzionare. `check` non invia comandi di movimento. Se un dato non e disponibile o un controllo fallisce, il programma rifiuta la prova.

## 4. Una sola corsa, dopo i controlli di avvio

```sh
/tmp/cc2-canvas-reverse-r1/cc2-canvas-motion-probe pulse-slot2
```

Il programma rilegge i dati e chiede di digitare esattamente **SLOT2** nel terminale SSH interattivo. Qualsiasi altra risposta annulla. Controlla di nuovo i dati prima dell'invio. Invia una sola retromarcia, poi un comando esplicito di stop anche in caso di errore o Ctrl+C; non ripete il movimento.

Prima della corsa, segnare il filo all'ingresso del Canvas. Durante la corsa non tirarlo o trattenerlo. Riportare se il filo esce dallo slot, di quanto si sposta il segno, se il feeder si ferma e l'output del programma. La distanza reale puo differire leggermente dall'obiettivo per inerzia e cadenza del controllo.

Un ACK significa risposta al comando, **non una misura dell'arresto fisico**. Questa prova richiede l'osservazione del feeder. In caso di comportamento inatteso non ripetere la prova; usare lo stop della stampante se il motore non si arresta.

## Ripristino

Il backup persistente permette il ripristino anche dopo la pulizia di `/tmp`:

```sh
sh /opt/usr/cc2-reactor-runtime-v2/canvas-reverse-trial-backup/rollback-trial.sh
reboot
```

Eseguirlo a stampante inattiva. Il processo gia avviato conserva la correzione in memoria fino alla sua uscita; il launcher originale torna attivo al riavvio. Gli extra della prova restano su disco, ma non sono piu caricati dal launcher originale. Nessun backup viene cancellato automaticamente.

## Stato della verifica e crediti

- Confronto ARM: 144 casi iniziali, bug originale riprodotto e comportamento in avanti conservato. Ulteriori risultati locali sono in `VALIDATION.json` quando presenti.
- Slot 2: transizione presenza `1 -> 0` osservata via UDS durante rimozione manuale; circa 840 mm residui misurati dall'utente dopo lo scaricamento normale.
- Compilazione e test nativi: eseguiti dalla build sul PC, salvo risultati aggiuntivi indicati nel file di validazione.
- Primo avvio con entrambi i moduli, direzione, distanza e arresto fisico: **ancora da provare sulla stampante**.
- Non qualifica gli altri slot, l'espulsione completa, la gestione di inceppamenti o il completamento della PR114.

Proposta originale e principio della correzione del confronto: **sauadhi**, PR103. La prova della nuova base resta separata dalla sua validazione sulla stampante.
