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

int compare(const void *a, const void *b) {
    att *ia = (att *)a;
    att *ib = (att *)b;
    return ia->tm - ib->tm;
}

void releaseSHM(int signum) {
        int status;
        printf("You have pressed the ctrl-c and the students who have logged in are : \n");
        int i=0;
        att * shm, *s;
        shm=shmat(shmid,NULL,0);
        s=shm;
        // qsort(s, n, sizeof(att), compare);
        char buffer[80];
        for(i=0;i<n;i++)
        {
            if((shm+i)->roll!=-1)
            {
                time_t t;
                t = (shm+i)->tm;
                struct tm *tm = localtime(&t);
                strftime(buffer, 80, "%B %d, %Y, %T", tm);
                printf("\n student with roll no %d attended at %s ",(shm+i)->roll, buffer);
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
        (shm+i)->tm=time(NULL);
    }
    while(1)
    {
        sleep(5);
    }
    shmdt(shm);
}