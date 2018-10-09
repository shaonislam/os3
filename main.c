/* Main.c 

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
		printf("Max user processes spawned: %d\n\n", max_spawn);
	}

	return 0;
}
