# Recupero webcam R1 — prova temporanea

Questa prova conserva la libreria R2 già qualificata e aggiunge un annuncio della camera ai nuovi collegamenti di controllo, soltanto dopo il rilevamento nativo della webcam. ELEGOO printer può quindi usare la propria inizializzazione video e i parametri di sessione che conserva. Nessuna credenziale viene letta o copiata dalla patch.

Non è una correzione definitiva: video locale, Matrix, riavvio della sola camera e AI richiedono conferma sulla stampante. Le prove offline non dimostrano il recupero reale, né il rinnovo di token scaduti.

## Preparazione

Solo stampante inattiva, con video locale e Matrix funzionanti prima della prova. Conserva questa situazione iniziale; non eseguire cicli di connessione o prove durante una stampa. Non cambiare rete o parametri Wi-Fi contemporaneamente.

Dal computer copia `cc2-camera-recovery-r1.tar.gz` in `/tmp/`. Sulla stampante esegui separatamente:

```sh
mkdir -p /tmp/cc2-camera-recovery-r1
```

```sh
tar -xzf /tmp/cc2-camera-recovery-r1.tar.gz -C /tmp/cc2-camera-recovery-r1
```

```sh
sh /tmp/cc2-camera-recovery-r1/trial.sh check
```

Condividi il risultato prima di attivare. Il controllo non riavvia processi e non invia notifiche.

## Prova assistita

Dopo il controllo positivo, `sh /tmp/cc2-camera-recovery-r1/trial.sh start` riavvia esclusivamente ai_camera con R2 e recupero R1. Non riavvia elegoo_printer né modifica file di sistema. Verifica video locale, Matrix e poi `sh /tmp/cc2-camera-recovery-r1/trial.sh status`.

Solo dopo una prima conferma positiva si potrà provare un secondo riavvio camera con `sh /tmp/cc2-camera-recovery-r1/trial.sh restart`. Non automatizzare ripetizioni in caso di errore.

## Ripristino

Un normale riavvio della stampante inattiva elimina questa attivazione in memoria e ripristina l'avvio già configurato. L'eventuale precedente prova one-shot non viene riarmata da questo pacchetto. Nessun file originale viene sostituito. Se la prova fallisce, conserva solo status e risultati; i log completi possono contenere dati di sessione e non vanno pubblicati.

Grazie a tharje per la diagnosi e le catture della perdita di stack, e ai beta tester per le verifiche locali e remote.
