#include <stdio.h>
#include <stdlib.h>
#include "mutex.h"
#include "shm.h"
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "keys.h"
/*
int main(int argc, char const *argv[])
{
    if(argc!=5){
        printf("USAGE: %s #POPOLAZIONE_INIZIALE, %s #POPOLAZIONE_MASSIMA, %s #CIBO_INIZIALE, %s #ACRI", argv[1], argv[2], argv[3], argv[4]);
        return-1;
    }
*/
int main(){
    char* argv[]={"./dio", "5", "100", "100", "100", NULL};

 	printf("Creazione di Cuneo in corso...\n");
	int popolazione_init, popolazione_max, grano_iniziale, acri_coltivabili, pid, i,tempo_shm_id, campi_shm_id, cimitero_shm_id, granaio_shm_id, cuneesi_shm_id, cuneesi_nati_fino_a_ieri, cuneesi_morti_fino_a_ieri, anagrafe_shm_id;
	popolazione_init=atoi(argv[1]);
	popolazione_max=atoi(argv[2]);
	grano_iniziale=atoi(argv[3]);
	acri_coltivabili=atoi(argv[4]);


    int mutex_granaio=mutex_create(GRANAIO_MUTEX_KEY, 0);
    int* granaio=(int*)shm_create(CIMITERO_KEY, sizeof(int), &granaio_shm_id);
    printf("Creato il granaio\n");
    int mutex_cimitero=mutex_create(CIMITERO_MUTEX_KEY, 0);
    int* cimitero=(int*)shm_create(CIMITERO_KEY, sizeof(int), &cimitero_shm_id);
    printf("Creato il cimitero\n");
    int mutex_anagrafe=mutex_create(ANAGRAFE_MUTEX_KEY, 0);
    int* anagrafe=(int*)shm_create(ANAGRAFE_KEY, sizeof(int), &anagrafe_shm_id);
    printf("Creato l'anagrafe\n");
    int mutex_cuneesi=mutex_create(CUNEESI_MUTEX_KEY, 0);
    int* numero_cuneesi=(int*)shm_create(CUNEESI_KEY, sizeof(int), &cuneesi_shm_id);
    printf("Creato l'istituto statistico\n");
    int* tempo=(int*)shm_create(TEMPO_KEY, sizeof(int)*4, &tempo_shm_id);
    printf("Creato il tempo\n");
    int*campi=(int*)shm_create(CAMPI_KEY, sizeof(int), &campi_shm_id);
    int mutex_campi=mutex_create(CAMPI_MUTEX_KEY, 0);
    printf("Creati i campi\n");

    cuneesi_nati_fino_a_ieri=0;
    cuneesi_morti_fino_a_ieri=0;

    *numero_cuneesi=0; /// Contadini totali
    printf("a\n");

    tempo[0]=0; /// Secondi passati dalla creazione
    tempo[1]=0; /// giorno
    tempo[2]=0; /// stagione
    tempo[3]=time(NULL); /// tempo del giorno 0

    printf("b\n");

    char* cmdCuneese[]={"./cuneese", popolazione_max, NULL};
    for(pid=getpid(), i=0; i<popolazione_init && pid; i++){
        if(!fork()){
            printf("Creato Cuneese\n");
            execvp(*cmdCuneese, cmdCuneese);
            printf("ERRORE: impossibile creare Cuneese\n");
            return -2;
        }
    }

    while(1){
        /// Cambio di giorno
        if(time(NULL)-tempo[0]>0){
            tempo[0]++;
            tempo[1]=(tempo[0]+1)%6;
            /// Cambio di stagione
            if(tempo[1]==0){
                tempo[2]=(tempo[2]+1)%4;
            }
            printf("-----Cuneo-----\nGiorni passati dalla Creazione: %d\n Giorno della stagione: %d \n Stagione: %d\n Numero di Cuneesi: %d\n Numero di Cuneesi nati oggi: %d\ Numero di Cuneesi morti oggi: %d\nNumero di Cuneesi nati globalmente: %d\ Numero di Cuneesi morti globalmente: %d\nNumero di campi coltivati: %d\nScorte alimentari: %d\n\n", tempo[0], tempo[1], tempo[2], *numero_cuneesi, *anagrafe-cuneesi_nati_fino_a_ieri, *cimitero-cuneesi_morti_fino_a_ieri, *anagrafe, *cimitero, *campi, *granaio);
        }

    }

    getc(stdin);
    return 0;
}
