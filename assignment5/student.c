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
// typedef void (*sighandler_t)(int);
// void releaseSHM(int signum) {
//         int status;
//         printf("You have pressed the ctrl-c and the students who have logged in are : \n");
//         int i=0;
//         att * shm;
//         shm=shmat(shmid,NULL,0);
//         for(i;i<n;i++)
//         {
//             if((shm+i)->roll != -1)
//             {
//                 printf("%d , ",(shm+i)->roll);
//             }
//             else
//             {
//                 break;
//             }
//         }
//         printf("\n");
//         // int shmctl(int shmid, int cmd, struct shmid_ds *buf); /* Read the manual for shmctl() */
//         status = shmctl(shmid, IPC_RMID, NULL); /* IPC_RMID is the command for destroying the shared memory*/
//         if (status == 0) {
//                 fprintf(stderr, "Remove shared memory with id = %d.\n", shmid);
//         } else if (status == -1) {
//                 fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
//         } else {
//                 fprintf(stderr, "shmctl() returned wrong value while removing shared memory with id = %d.\n", shmid);
//         }

//         // int kill(pid_t pid, int sig);
//         status = kill(0, SIGKILL);
//         if (status == 0) {
//                 fprintf(stderr, "kill susccesful.\n"); /* this line may not be executed :P WHY?*/
//         } else if (status == -1) {
//                 perror("kill failed.\n");
//                 fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
//         } else {
//                 fprintf(stderr, "kill(2) returned wrong value.\n");
//         }
// }
int main(int argc, char *argv[])
{
    int projid=1;
    int n=atoi(argv[1]);
    key_t shmkey=ftok("./abc.txt",projid);
    shmid=shmget(shmkey,40,IPC_CREAT | 0666);
    printf("%d shmid\n",shmid);
    if(shmid<0)
    {
        perror("shmget");
        return 1;
    }
    att * shm;
    shm=shmat(shmid,NULL,0);
    while(1)
    {
        if(shm->roll!=-1)
        {
            shm->roll=n;
            shm->tm=time(NULL);
            break;
        }
        shm+=1;
    }
    shmdt(shm);
    return 0;
    
}