# **Server TCP Semplice con Gestione Utenti e Contatti**

### Descrizione

Questo progetto è un semplice server TCP multi-thread scritto in C che supporta l'autenticazione degli utenti e la gestione dei contatti. Il server permette ai client di registrarsi, effettuare il login e gestire una lista di contatti (aggiungere, eliminare, modificare e visualizzare i contatti).

### Funzionalità

* Registrazione Utenti: I client possono registrarsi con username e password.
* Autenticazione Utenti: I client devono effettuare il login per eseguire operazioni di gestione dei contatti.
* Aggiungi Contatto: Aggiungi un nuovo contatto con nome e numero di telefono.
* Elimina Contatto: Rimuovi un contatto per nome.
* Modifica Contatto: Cambia il numero di telefono di un contatto esistente.
* Visualizza Contatti: Elenca tutti i contatti.

### Installazione e Compilazione

1. Clona il repository:
   ```bash
   git clone https://github.com/sultansozoev/server-SO.git
   cd  server-SO
    ```
2. Compila il progetto:
   ```bash
   make
   ```
   E poi utilizza il comando seguente:
   ```bash
   gcc -o server main.c server.c commands.c users.c contacts.c -pthread
   ```

### Utilizzo
1. Avvia il server:
   ```bash
   ./server
   ```
2. Comandi dei Client:
   * `accedi`: Effettua il login con nome utente e password;
   * `registrati`: Registra un nuovo utente con nome utente e password;
   * `aggiungi`: Aggiunge un nuovo contatto (richiede autenticazione);
   * `elimina`: Elimina un contatto per nome (richiede autenticazione);
   * `modifica`: Modifica il numero di telefono di un contatto esistente (richiede autenticazione);
   * `stampa`: Elenca tutti i contatti;
   * `chiudi`: Chiudi la connessione con il server.

### Pulizia
Per eliminare i file .o e l'eseguibile, esegui il comando:
   ```bash
   make clean
   ```