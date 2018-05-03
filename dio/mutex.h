/*
 * mutex.h
 * This file is part of DelPozzoSoft
 *
 * Copyright (C) 2018 - DelPozzo
 *
 * DelPozzoSoft is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * DelPozzoSoft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DelPozzoSoft. If not, see <http://www.gnu.org/licenses/>.
 */

/*--------------------------------------------------------------------
  mutex.h 
	v 2.0 	(Febbraio 2018)
----------------------------------------------------------------------
	Libreria di interfaccia per la gestione di semplici semafori
	per mutua esclusione.
	
	IIS DelPozzo - Cuneo (CN) 
	Settore Informatica 
	Author: Tosello

	Se modificate questo sorgente per favore fatene
	avere una copia agli autori originali, cosi' da 
	permettere l'allineamento ed evitare la diffusione
	di cloni o fork che non garantiscano la compatibilita'
	verso il preesistente.

	Grazie.                     			L'autore
---------------------------------------------------------------------*/

#ifndef _MUTEX_H
#define	_MUTEX_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/sem.h>

/* strutture per le chiamate a semop() per modificare il contenuto del mutex */
struct sembuf sem_lock = {
		0,		/* numero del mutex */
		-1,		/* togliere uno al valore nel mutex */
		SEM_UNDO };	/* non esce dal processo fino a che non e' 
				   riuscito a modificare il contenuto del mutex */
struct sembuf sem_trylock = {
    0,    /* numero del mutex */
    -1,   /* togliere uno al valore nel mutex */
    IPC_NOWAIT }; /* esce dal processo se non e' 
           riuscito subito a modificare il contenuto del mutex */
struct sembuf sem_unlock = {
		0,		/* numero del mutex */
		+1,		/* aggiungere uno al valore nel mutex */
		SEM_UNDO };	/* non esce dal processo fino a che non e' 
				   riuscito a modificare il contenuto del mutex */

int mutex_create (key_t ipc_key, int starting_value);
int mutex_find (key_t ipc_key);
int mutex_read (int sem_id);
int mutex_lock(int sem_id);
int mutex_trylock(int sem_id);
int mutex_unlock(int sem_id);
int mutex_remove(int sem_id);

/* mutex_create - crea un mutex
   RICEVE: l'identificativo nel sistema dell'insieme semafori e il valore al quale inizializzare
   RESTITUISCE: un intero che identifica l'insieme di semafori */
int mutex_create (key_t ipc_key, int starting_value) 
 {
  int ret, sem_id;

  /* semget       crea un semaforo 
     ipc_key      --> chiave univoca associata al gruppo di semafori
     1            --> numero di semafori contenuti nell'insieme
     3o parametro --> operazione da compiere messe in OR:
 		      IPC_CREAT :crea l'insieme di semafori 
		      IPC_EXCL	:restituisce un valore non valido nel caso in cui
				la struttura esista gia'	
		      0666: accesso consentito a tutti */
   sem_id = semget(ipc_key, 1, IPC_CREAT|IPC_EXCL|0666);
   if (sem_id == -1) 
     return (sem_id);  // errore 

  /* semctl       inizializzazione semaforo 
      sem_id        --> id dell'insieme di semafori 
      0             --> numero del semaforo da inizializzare
      SETVAL        --> operazione di impostazione del valore del semaforo
      starting_value--> valore di inizializzazione */
    ret = semctl(sem_id, 0, SETVAL, starting_value); 

    if (ret == -1)
		return (ret);  // errore

    return (sem_id);
 }

/* mutex_find - recupera un mutex data la chiave
   RICEVE: l'identificativo nel sistema dell'insieme di oggetti IPC 
   RESTITUISCE: un intero che identifica l'insieme di semafori */
int mutex_find (key_t ipc_key) 
 {
     /*  semget 
     ipc_key      --> chiave univoca associata al gruppo di semafori
     il 2o parametro è ignorato; 
     il 3o parametro indica che non si compiono operazioni sul semaforo */
  return semget(ipc_key, 1, 0);
 }

/* mutexRead - legge un mutex (non affidabile)
   RICEVE: l'identificativo dell' insieme di semafori
   RESTITUISCE: il valore contenuto nel semaforo (l'unico dell'insieme) */
int mutex_read (int sem_id)
 {
   /* semctl       lettura valore del semaforo 
      sem_id       --> id dell'insieme di semafori 
      2o parametro --> numero del semaforo dal quale leggere il valore
      GETVAL       --> operazione di lettura del valore del semaforo */
  return  semctl(sem_id, 0, GETVAL);
 }

/* mutexLock - blocca un mutex - BLOCCANTE 
   RICEVE: id dell'insieme di semafori 
   RESTITUISCE: 0 in caso di successo */
int mutex_lock(int sem_id) 
 {
     /*  semop 
         sem_id     --> identificativo dell'insieme di semafori
         &sem_lock  --> operazione da effettuare
         1          --> numero di operazioni contenute nel secondo argomento */
    return semop(sem_id, &sem_lock, 1);
}

/* mutexTrylock - blocca un mutex - NON BLOCCANTE
   RICEVE: id dell'insieme di semafori 
   RESTITUISCE: 0 in caso di successo */
int mutex_trylock(int sem_id) 
 {
     /*  semop 
         sem_id     --> identificativo dell'insieme di semafori
         &sem_lock  --> operazione da effettuare
         1          --> numero di operazioni contenute nel secondo argomento */
    return semop(sem_id, &sem_trylock, 1);
}


/* mutexUnlock - sblocca un mutex
   RICEVE: id dell'insieme di semafori 
   RESTITUISCE: 0 in caso di successo */
int mutex_unlock(int sem_id) 
 {
     /*  semop 
         sem_id     --> identificativo dell'insieme di semafori
         &sem_unlock  --> operazione da effettuare
         1          --> numero di operazioni contenute nel secondo argomento */
    return semop(sem_id, &sem_unlock, 1);
 }

/* mutexRemove - rimuove un mutex
   RICEVE: id dell'insieme di semafori 
   RESTITUISCE: 0 in caso di successo */
int mutex_remove(int sem_id) 
 {
  /* semctl       lettura valore del semaforo 
     sem_id       --> id dell'insieme di semafori 
     2o parametro --> ignorato
     IPC_RMID     --> operazione di RIMOZIONE del semaforo */
  return semctl(sem_id,	0, IPC_RMID);	
}

#endif	/* _MUTEX_H */