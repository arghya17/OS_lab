#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include <signal.h>

typedef void (*sighandler_t)(int);
typedef struct att
{
    int roll;
    time_t tm;

} att;
int shmid;
int n;
typedef void (*sighandler_t)(int);
void releaseSHM(int signum) {
        int status;
        printf("You have pressed the ctrl-c and the students who have logged in are : \n");
        int i=0;
        att * shm;
        shm=shmat(shmid,NULL,0);
        for(i=0;i<n;i++)
        {
            if((shm+i)->roll != -1)
            {
                time_t t;
                t = (shm+i)->tm;
                struct tm tm = *localtime(&t);
                printf("student with roll no %d attended at %d-%d-%d",(shm+i)->roll, tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
            }
            else
            {
                break;
            }
        }
        printf("\n");
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
int main(int argc, char *argv[])
{
    sighandler_t shandler;


        /* install signal handler */
        // sighandler_t signal(int signum, sighandler_t handler);
    shandler =  signal(SIGINT, releaseSHM);//SigINT is for ctrl-c
    if (argc<2)
    {
        exit(1);
    }
    n=atoi(argv[1]);
    int projid=1;
    key_t shmkey=ftok("./abc.txt",projid);
    shmid=shmget(shmkey,n*sizeof(att),IPC_CREAT | 0666);
    if(shmid<0)
    {
        perror("shmget");
        return 1;
    }
    att * shm;
    shm=shmat(shmid,NULL,0);
    int i;
    for(i=0;i<n;i++)
    {
        (shm+i)->roll=-1;
        // (shm+i)->tm=time(NULL);
    }
    while(1)
    {
        sleep(500);
    }
    shmdt(shm);
}