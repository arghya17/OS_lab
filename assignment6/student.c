#include <stdio.h>
#include <sys/types.h> /* for semget(2) ftok(3) semop(2) semctl(2) */
#include <sys/ipc.h>   /* for semget(2) ftok(3) semop(2) semctl(2) */
#include <sys/sem.h>   /* for semget(2) semop(2) semctl(2) */
#include <unistd.h>    /* for fork(2) */

#include <stdlib.h> /* for exit(3) */

#define NO_SEM 1

#define P(s) semop(s, &Pop, 1); // semaphore -- also used to invoke wait
#define V(s) semop(s, &Vop, 1); // semaphore ++

struct sembuf Pop;
struct sembuf Vop;

int main()
{
    key_t key;
    pid_t pid;

    int semid;

    int status;

    union semun
    {
        int val;               /* Value for SETVAL */
        struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
        unsigned short *array; /* Array for GETALL, SETALL */
        struct seminfo *__buf; /* Buffer for IPC_INFO (Linux-specific) */
    } setvalArg;

    setvalArg.val = 3; // intialize semaphore value with 3

    Pop.sem_num = 0;
    Pop.sem_op = -1;
    Pop.sem_flg = SEM_UNDO;

    Vop.sem_num = 0;
    Vop.sem_op = 1;
    Vop.sem_flg = SEM_UNDO;

    // key_t ftok(const char *pathname, int proj_id);
    key = ftok("./bcd.txt", 1);

    if (key == -1)
    {
        perror("ftok() failed");
        exit(1);
    }

    //       int semget(key_t key, int nsems, int semflg);
    semid = semget(key, NO_SEM, IPC_CREAT | IPC_EXCL | 0666);
    if (semid == -1)
    {
        perror("semget() failed");
        exit(1);
    }

    // int semctl(int semid, int semnum, int cmd, ...);
    status = semctl(semid, 0, SETVAL, setvalArg);
    if (status == -1)
    {
        perror("semctl() failed");
        exit(1);
    }
}