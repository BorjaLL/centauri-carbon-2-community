# Update CC2 Control 1.2.5 — esclusivamente Community Firmware V4.4

[English](UPDATE_CC2.md) · [Homepage](../README.md)

Include ungherese (Magyar) di [pezso92](https://github.com/pezso92) e mesh avanzata per temperatura/ugello con stabilizzazione termica di [beanbo](https://github.com/beanbo).

**Solo Community Firmware V4.4 già installato e configurato. Non installare sulla V4.3, sul firmware originale ELEGOO o su altri modelli.** Il solo numero base 02.01.01.07 non basta: prima di fermare il servizio, l’installer verifica binario community su disco, processo della stampante e daemon degli aggiornamenti. Impronte sconosciute vengono rifiutate.

È uno ZIP applicativo per aggiornare via SSH. Non installa firmware o SSH e non sostituisce il modulo callback. Canvas rimane su MQTT; fix LAN/WAN, luci e funzioni già validate restano inclusi.

## 1. Preparazione

1. Scaricare **CC2-Control-1.2.5-Multiplatform-Update.zip** e checksum dalla [release applicativa](https://github.com/Neme77/centauri-carbon-2-community/releases/tag/CC2-Control-1.2.5-V4.4).
2. Confrontare SHA-256 dello ZIP con il valore pubblicato:

~~~powershell
Get-FileHash -Algorithm SHA256 -LiteralPath '.\CC2-Control-1.2.5-Multiplatform-Update.zip'
~~~

3. Confermare V4.4 e **Stampante connessa** in CC2 Control.
4. Attendere **Inattiva**, senza stampa o calibrazioni, ugello e piatto al massimo a **50 °C**, target entrambi a **0 °C**. Chiudere camera live e altre pagine di controllo durante l’installazione.
5. Annotare l’IP. Il computer deve raggiungere la rete locale e disporre di OpenSSH (ssh e scp).
6. Estrarre tutto lo ZIP in una cartella nuova. Non avviare i launcher dallo ZIP compresso e non usare questo ZIP nel menu USB degli aggiornamenti firmware.

Codice LAN, materiali, preferenze, dati piatti/ugelli e inventario bobine vengono conservati. Il backup automatico registra applicazione precedente e launcher del servizio.

## 2. Installazione

### Windows

Aprire la cartella estratta, fare doppio clic su **Install-Windows.cmd** e inserire l’IP.
In alternativa, aprire PowerShell nella cartella:

~~~powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\Install-CC2-Control.ps1 -PrinterIp 192.168.1.50
~~~

Sostituire l’IP di esempio. Serve il client OpenSSH di Windows.

### Linux e macOS

Aprire un terminale nella cartella estratta:

~~~sh
sh ./install-cc2-control.sh 192.168.1.50
~~~

Sostituire l’IP di esempio.

### SSH e conclusione

Utente **root**, password predefinita **MTY4ODE2**, oppure quella personalizzata. La digitazione non viene mostrata e la password può essere richiesta più volte. È diversa dal codice LAN.

Alla prima connessione verificare l’impronta della chiave della stampante. Solo per cambiamenti previsti seguire il [recupero della chiave SSH](../cc2-control/installer/README.md#ssh-host-key-changes).

L’installer verifica pacchetto, base reale e stato; crea il backup; ferma **solo CC2 Control**; installa applicazione ARM/UI e riavvia il servizio. Attendere fino a **240 secondi** per salute, checksum del binario in esecuzione, registrazione MQTT e snapshot.

Il successo mostra **INSTALLATION PASSED**, percorso del backup e comando esatto di ripristino. In caso di errore leggere lo stato del rollback e conservare il percorso. Non stampare finché il servizio precedente o aggiornato non funziona.

## 3. Verifica

1. Aprire http://IP-STAMPANTE:8081 e ricaricare; Ctrl+F5 se rimane la vecchia UI.
2. Verificare **1.2.5**, **Stampante connessa**, temperature e slot Canvas. Il codice LAN salvato non dovrebbe essere richiesto di nuovo.
3. Se desiderato, scegliere **Magyar** da **Impostazioni → Aspetto**. Le lingue diventano otto.
4. Controllare piatti/ugelli in **Livellamento piatto**. La calibrazione accetta temperatura **40–110 °C**, stabilizzazione **0–60 minuti** (predefinita **10**) e ugello. Lasciare il piatto libero prima di confermare.
5. Ogni piatto conserva misure per temperatura e ugello. Il dialogo di stampa utilizza la misura a temperatura più vicina o quella scelta, applicando la correzione Z dell’ugello selezionato.
6. Riavviare una volta mentre la stampante è inattiva, poi verificare avvio automatico, connessione e Canvas.

Non servono ripristino di fabbrica o sostituzione del codice LAN. Il display mantiene la base vendor 02.01.01.07; l’applicazione è 1.2.5.

## 4. Ripristino dell’applicazione precedente

A stampante connessa, inattiva e fredda con target dei riscaldatori a zero, usare il backup esatto indicato dal proprio installer:

~~~sh
ssh root@IP-STAMPANTE
sh /opt/usr/cc2-control-backup-PROPRIO-TIMESTAMP-PID/restore.sh
~~~

Sostituire IP e percorso segnaposto. Credenziali, materiali, preferenze, piatti/ugelli e bobine correnti vengono mantenuti. Se la precedente app era integrata, il launcher può tornare al binario in /opt/inst; il backup di /opt/usr può contenere solo dati persistenti.

Si ripristina l’**applicazione**, non un firmware ELEGOO precedente. Il blocco del downgrade firmware non cambia. Non usare il backup di un’altra stampante.
