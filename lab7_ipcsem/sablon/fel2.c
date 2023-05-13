/* 
	alternalo megoldas apa es fiu kozott egy szemaforral 
 */

#include "myinclude.h"

#define N 10 //ciklusok száma
	  
int main()
{
	int semid;
	int i,j,k;
	pid_t pid;

// a szemafor szekvenciaja:
//
// értéke    művelet    ki végzi
// 0         wait0      a fiú zéróra var
// 0                    fiú a kritikus szakaszban
// 2         up         a fiú kilép és +2 -t ad hozzá           
// 1         down       apa várt eddig, most sikerül a -1    
// 1                    apa a kritikus szakaszban
// 0         down       apa kilép és utána 1-re var

//ha a szemafor kezdő értéke 0, a fiú indul elsőnek
//a feladat megoldható 2-es kezdő értékkel is, akkor az apa indul elsőnek

// a fiú szemafor műveletei
	struct sembuf wait0  = {0, 0, SEM_UNDO}; //zéróra var
	struct sembuf up = {0, 2, 0};            //kilép +2
//az apa műveletei
	struct sembuf wait1  = {0, -1, SEM_UNDO}; // down művelet, ha sikerül a szemafor 1 lesz
	struct sembuf down = {0, -1, SEM_UNDO};  // mindkét művelete ugyanaz 
	  
    unsigned int cnt; //szamlalo
	int key = ftok (".",'a');

	setbuf(stdout,NULL); 

	//TODO: itt megoldani a feladatot

	// letrehozzuk a szemafort
	semid = semget(key, 1, IPC_CREAT | 0660);
	if (semid < 0) {
		syserr("semid");
	}

	// szemafornak kezdeti erteket adunk
	if (semctl(semid, 0, SETVAL, 0) < 0) {
		syserr("semctl");
	}

	// forkolas
	pid = fork();
	if (pid < 0) {
		syserr("fork");
	}
	if (pid == 0) {
		// fiu folyamat
		for (cnt = 0; cnt < N; ++cnt) {
			if (semop(semid, &wait0, 1) < 0) {
				syserr("semop");
			}

			printf("tikk\n");

			if (semop(semid, &up, 1) < 0) {
				syserr("semop");
			}
		}
		
		exit(EXIT_SUCCESS);
	} else {
		// apa folyamat
		for (cnt = 0; cnt < N; ++cnt) {
			if (semop(semid, &down, 1) < 0) {
				syserr("semop");
			}
			
			printf("takk\n");
		
			if (semop(semid, &down, 1) < 0) {
				syserr("semop");
			}
		}

		wait(NULL);
	}

	if (semctl(semid, 0, IPC_RMID, 0) < 0) {
		syserr("semctl");
	}

	exit(EXIT_SUCCESS);
}	
