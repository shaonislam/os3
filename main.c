/*
 Main.c 

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




int main (int argc, char *argv[])
{

	int option, max_spawn, term_time;  

        while((option = getopt(argc, argv, "hs:t:")) != -1)
        {
                switch (option)
                        {
                        case 'h':
                                fprintf(stderr, "How it should run: \n");
                                fprintf(stderr, "	-s x: x is the max number of user process to spawn (default: 5)\n");
                                fprintf(stderr, "	-t z: z is the time in seconds when the master will terminate itself and all the children \n");
                                fprintf(stderr, "	-h: display options to execute\n");
                                exit(1);
                        case 's':
				max_spawn = atoi(optarg);
				printf("Max user processes spawned: %d\n\n", max_spawn);
				break;
                        case 't':
				term_time = atoi(optarg);
				printf("Master and children will terminate in  %d seconds\n", term_time);
	                       	break;
                        case '?':
                                fprintf(stderr, "Error\n");
                                break;
                        }
        
	}
	if (max_spawn == 0)
	{	
		max_spawn = 5;
		fprintf(stderr, "Max user processes spawned: %d\n\n", max_spawn);
	}



	/*_________Setup Shared Memory_________*/

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






	char arg1[10];
	snprintf(arg1 , 10, "%d", max_spawn);

	int child;
	for(child = 0; child < max_spawn; child++)
	{
		fprintf(stderr, "\n\nTRYING to create child %d\n", (child+1));
		
        	pid_t child_pid = 0;
        	child_pid = fork();
        	if (child_pid == 0)
        	{
	        	/* in the child process! */
        		fprintf(stderr, "i:%d 	process ID:%ld 	parent ID:%ld 	child ID:%ld\n", child, (long)getpid(), (long)getppid(), (long)child_pid);
			execlp("./user", "./user", arg1, (char *)NULL);
			exit(0); 
        	}
		wait(NULL);

	}

	return 0;
}
