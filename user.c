#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <getopt.h>
#include <sys/shm.h>
#include <sys/ipc.h>


int main (int argc, char *argv[])
{


	/*_________Setup Shared Memory_________*/

	int shm_id;
        key_t key;
        void* shm;
        key = 6789;
        shm_id = shmget(key, sizeof(int)*2, IPC_CREAT | 0666);
        shm = shmat(shm_id, NULL, 0);

	int* master_clock = shm;

	fprintf(stderr, "Seconds: %d\n", master_clock[0]);
        fprintf(stderr,	"Nanoseconds: %d\n", master_clock[1]);





	fprintf(stderr, "in the worker \n");
	return 0;
}
