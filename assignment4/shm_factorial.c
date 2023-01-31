#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include <signal.h>
int shmid;

typedef void (*sighandler_t)(int);
void releaseSHM(int signum) {
        int status;
        // int shmctl(int shmid, int cmd, struct shmid_ds *buf); /* Read the manual for shmctl() */
        status = shmctl(shmid, IPC_RMID, NULL); /* IPC_RMID is the command for destroying the shared memory*/
        if (status == 0) {
                fprintf(stderr, "Remove shared memory with id = %d.\n", shmid);
        } else if (status == -1) {
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
        } else {
                fprintf(stderr, "shmctl() returned wrong value while removing shared memory with id = %d.\n", shmid);
        }

        // int kill(pid_t pid, int sig);
        status = kill(0, SIGKILL);
        if (status == 0) {
                fprintf(stderr, "kill susccesful.\n"); /* this line may not be executed :P WHY?*/
        } else if (status == -1) {
                perror("kill failed.\n");
                fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
        } else {
                fprintf(stderr, "kill(2) returned wrong value.\n");
        }
}

int factorial(int n)
{
    int f=1;
    for(int i=1;i<=n;i++)
    {
        f=f*i;
    }
    return f;
}

int main()
{
    sighandler_t shandler;


        /* install signal handler */
        // sighandler_t signal(int signum, sighandler_t handler);
    shandler =  signal(SIGINT, releaseSHM); /* should we call this seperately in parent and child process */

    shmid= shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT | 0666);
    if(shmid<0)
    {
        perror("shmget");
        exit(1);
    }
    int *shm_ptr = (int *) shmat(shmid, NULL, 0);
    if ((void *) shm_ptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }
    //shm_ptr is shared by both parent and child
    pid_t pid=fork();
    if(pid==0)
    {//this child    
        while (1) {
            printf("Child: Factorial of %d is %d\n", *shm_ptr, factorial(*shm_ptr));
            // sleep(1);
        }
    }
    else
    {
        //parent process
        srand(time(NULL));
        while (1) {
            *shm_ptr = rand() % 10;
            printf("Parent: Generated %d\n", *shm_ptr);
            sleep(1);
        }
    }
    shmdt(shm_ptr);//detaching the shm 
    shmctl(shmid, IPC_RMID, NULL);//used to destroying the shm
    return 0;
}