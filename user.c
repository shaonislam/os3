#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>

int main (int argc, char *argv[])
{



	/*_________See Master Clock_________*/

	int shm_id;
        key_t key;
        void* shm;
        key = 6789;
        shm_id = shmget(key, sizeof(int)*2, IPC_CREAT | 0666);
        shm = shmat(shm_id, NULL, 0);
	int* master_clock = shm;


        /*_________Setup shmMSG_________*/
	
	int shmMSG_id;
	key_t key_msg;
	void* shm_msg;
	key_msg = 1234;
	shmMSG_id = shmget(key_msg, sizeof(int)*2, IPC_CREAT | 0666);
        shm_msg = shmat(shmMSG_id, NULL, 0);


	/*_________Setup Semaphore_________*/

	sem_t *sem; /*synch semaphore*/
	sem = sem_open("thisSem", O_CREAT|O_EXCL, 0644, 10);
	sem_unlink("thisSem");
	fprintf(stderr, "*****semaphore initialized*******\n");



	/*_________Put Term_time in SHM _________*/
	
	int* shterm_time = shm_msg;;
	shterm_time[0] = 0;
	shterm_time[1] = 0;	
	shterm_time[2] = 0;
	

        int term_time[3] = {0,0,0}; /*sec,nano,process*/
       /* term_time[0] = 0; 
        term_time[1] = 0; */


        /*_________Setting Terminal Time _________*/

	int random_duration = atoi(argv[1]);
        term_time[1] = master_clock[1] + random_duration;
	fprintf(stderr, "Master Clock + Random = Term Time: : %d + %d = %d\n", master_clock[1], random_duration, term_time[1]);
        while (term_time[1] > 999999999)
        {
                term_time[1] = term_time[1] - 999999999;
                term_time[0] = term_time[0] + 1;
        }
	fprintf(stderr, "USER TERM TIME: %d:%d\n", term_time[0], term_time[1]);


	/*_________Check System Clock Until Deadline _________*/
	/*while(1)*/
	
		fprintf(stderr, "Current Master: %d:%d \nCurrent Term Time: %d:%d\n", master_clock[0], master_clock[1], term_time[0], term_time[1]); 
		if(master_clock[0] > term_time[0] || (master_clock[0] == term_time[0] && master_clock[1] > term_time[1]) )
		{
			/* deadline passed, send shMsg and self terminate*/

			fprintf(stderr, "DEADLINE MET: SEND shMSG\n");
			shterm_time[0] = term_time[0];
			shterm_time[1] = term_time[1];
			shterm_time[2] = getpid();
		
		} 
		else
		{
			fprintf(stderr, "*deadline not met yet*\n");
		}










	sem_destroy(sem);	
	return 0;
}
