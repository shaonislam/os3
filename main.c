/*
 Main.c 
 Shaon Islam
 CS 4760: Project 3 Semaphores and OS Simulator
 October 2018
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>



void handle_terminate(int sig)
{
        fprintf(stderr, "Terminating due to 2 second program life span is over.\n");
        exit(1);
}


int main (int argc, char *argv[])
{

	int option, max_spawn, master_term;  
	srand((unsigned)time(NULL));
	int lflag = 0;
	FILE *fname;

	/*______Set up signal______*/
        signal(SIGALRM, handle_terminate);
        alarm(2);
 

	/*______Set GETOPT______*/
        while((option = getopt(argc, argv, "hs:t:l:")) != -1)
        {
                switch (option)
                        {
                        case 'h':
                                fprintf(stderr, "How it should run: \n");
                                fprintf(stderr, "	-s x: x is the max number of user process to spawn (default: 5)\n");
                                fprintf(stderr, "	-t z: z is the time in seconds when the master will terminate itself and all the children \n");
				fprintf(stderr, "	-l y: y is the name of filename for user process information to be writting to\n");
                                fprintf(stderr, "	-h: display options to execute\n");
                                exit(1);
                        case 's':
				max_spawn = atoi(optarg);
				fprintf(stderr,"Max user processes spawned: %d\n\n", max_spawn);
				break;
                        case 't':
				master_term = atoi(optarg);
				fprintf(stderr,"Master and children will terminate in  %d seconds\n", master_term);
	                       	break;
			case 'l':
				fname = fopen(optarg, "w+");
				lflag = 1;		
				break;
                        case '?':
                                fprintf(stderr, "Error\n");
                                break;
                        }
        
	}


	/* If spawn input isn't given, default to 5 */
	if (max_spawn == 0)
	{	
		max_spawn = 5;
		fprintf(stderr, "Max user processes spawned: %d\n\n", max_spawn);
	}
	
	/* Check for ile name */
	if (lflag == 0)
	{
		fprintf(stderr, "No filename inputted: Exiting Program\n");
		exit(1);
	}

	/*_________Setup Shared Memory For Master Clock_________*/

	int shm_id;
        key_t key;
        void* shm;
        key = 6789;
        shm_id = shmget(key, sizeof(int)*2, IPC_CREAT | 0666);
        if (shm_id == -1)
        {
                perror("shmget");
                exit(1);
        }
        shm = shmat(shm_id, NULL, 0);



	/*_________PUT THE CLOCK IN SHARED MEMORY_________ */

        int*  master_clock = shm;  
	int seconds = 0;
	int nanosec = 0;
	master_clock[0] = seconds;
	master_clock[1] = nanosec;


	/*_________Setup shmMSG_________*/

        int shmMSG_id;
        key_t key_msg;
        void* shm_msg;
        key_msg = 1234;
        shmMSG_id = shmget(key_msg, sizeof(int)*2, IPC_CREAT | 0666);
        shm_msg = shmat(shmMSG_id, NULL, 0);
	int* shterm_time = shm_msg;
	


	/*_________Creating User Processes_______*/

	/*int process_tracker = 0;*/
	int live_processes = 0;
	char arg1[10];
	int child;



	/*_________Increment System Clock_______*/

        /* Increment System Clock BY 100 Million Nanoseconds */

	while (master_clock[0] < 2) 
        {	
		master_clock[1] = master_clock[1] + 100000;
        	if (master_clock[1] > 999999999)
        	{
        		master_clock[1] = master_clock[1] %  1000000000;
                	master_clock[0] = master_clock[0] + 1;
        	}

		for(child = 0; child < max_spawn; child++)
		{			       
        		int random_duration = rand() % 1000000 + 1;
			snprintf(arg1 , 10, "%d", random_duration);

        		pid_t child_pid = 0;
        		child_pid = fork();

        		if (child_pid == 0)
        		{
				fprintf(stderr, "Child created!!!! with pid: %ld\n", (long)getpid());        
				execlp("./user", "./user", arg1, (char *)NULL);			
        		}

			if (child_pid)
			{
				live_processes++;
			}

		}		

	
		/* ___Check shmMSG for Message____ */
		if(shterm_time[0] != 0 || shterm_time[1] != 0)
		{
			/* Should have recieved a message */
			fprintf(stderr, "KILL: %d:%d for process %d\n", shterm_time[0], shterm_time[1], shterm_time[2]);

			fprintf(fname, "OSS: Child %d is terminating at my time %d seconds, %d nanoseconds because it reached %d seconds, %d nanoseconds in user\n", shterm_time[2], master_clock[0], master_clock[1], shterm_time[0], shterm_time[1]);
			/* Wait for user to finish */
			waitpid(shterm_time[2], NULL, 0);

			/* Decrement Number of Live Processes */
			live_processes--;	
				
			/*fprintf(stderr, "CLEAR shterm_time now\n");*/
			shterm_time[0] = 0;
			shterm_time[1] = 0;
			shterm_time[2] = 0;
		}
	
			/*wait(NULL); */
		
	}	
	


	/*______Clean Out Shared Memory_______*/
	shmctl(shm_id, IPC_RMID, NULL);
	shmctl(shmMSG_id, IPC_RMID, NULL);

	fclose(fname);
	return 0;
}
