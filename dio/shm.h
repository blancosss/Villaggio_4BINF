/*
 * shm.h
 * This file is part of VallauriSoft
 *
 * Copyright (C) 2012 - Vallauri
 *
 * Vall is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * IPC Vallauri is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with IPC Vallauri; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 */

/*--------------------------------------------------------------------
  SharedMem 
	v 2.0 	(Ottobre 2012)
----------------------------------------------------------------------
	Libreria di interfaccia per la gestione di aree di 
	memoria condivisa comprensiva di semaforo per mutua esclusione
	(versione ad oggetti) 

	IIS Vallauri - Fossano (CN) 
	Settore Informatica 
	Authors: Tosello, Bonino

	Se modificate questo sorgente per favore fatene
	avere una copia agli autori originali, cosi' da 
	permettere l'allineamento ed evitare la diffusione
	di cloni o fork che non garantiscano la compatibilita'
	verso il preesistente.

	Grazie.                     			Gli Autori
---------------------------------------------------------------------*/

#ifndef SHM_H
#define SHM_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>

/*shm_create	RICEVE: chiave ipc
			lunghezza dell'area da creare
			id ricevuto per referenza
		RESTITUISCE:	indirizzo della shared memory
*/
void* shm_create(int ipc_key,int len,int* shm_id)
{
  void* ret;

  *shm_id = shmget(ipc_key,len,IPC_CREAT | IPC_EXCL | 0666);
  if(*shm_id<0)
    return NULL;
  ret = shmat(*shm_id,NULL,0);
    return ret;
}

/* shm_remove   RICEVE: id della shared memory 
   	        RESTITUISCE: 0 in caso di successo 
*/

int shm_remove(int shm_id)
{
  int ret;
  ret = shmctl (shm_id, IPC_RMID,NULL);

  return ((ret!=-1)?0:1);
}

/* shm_find	 RICEVE: l'identificativo nel sistema della shared memory e l'identificativo nel processo.
   		 RESTITUISCE: un puntatore all'area di memoria 
*/

void* shm_find(int ipc_key,int* shm_id)
{
void* ret;

  *shm_id = shmget(ipc_key,0,IPC_EXCL);
  if(*shm_id<0)
    return NULL;
  ret = shmat(*shm_id,NULL,0);
    return ret;
}

#endif/*SHM_H*/
