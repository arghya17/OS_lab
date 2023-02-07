#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define NO_SEM 4

#define P(semid, i) semop(semid, &p[i], 1)
#define V(semid, i) semop(semid, &v[i], 1)

struct sembuf p[NO_SEM];
struct sembuf v[NO_SEM];

int main()
{
    key_t key;
    pid_t pid;
    int semid;
    int status;
    // setting parameters of sembuf structure for p operation and v operation for sem0
    p[0].sem_num = 0;
    p[0].sem_op = -1;
    p[0].sem_flg = SEM_UNDO;

    v[0].sem_num = 0;
    v[0].sem_op = 1;
    v[0].sem_flg = SEM_UNDO;

    // for sem1
    p[1].sem_num = 1;
    p[1].sem_op = -1;
    p[1].sem_flg = SEM_UNDO;

    v[1].sem_num = 1;
    v[1].sem_op = 1;
    v[1].sem_flg = SEM_UNDO;

    // for sem2
    p[2].sem_num = 2;
    p[2].sem_op = -1;
    p[2].sem_flg = SEM_UNDO;

    v[2].sem_num = 2;
    v[2].sem_op = 1;
    v[2].sem_flg = SEM_UNDO;

    p[3].sem_num = 3;
    p[3].sem_op = -1;
    p[3].sem_flg = SEM_UNDO;

    v[3].sem_num = 3;
    v[3].sem_op = 1;
    v[3].sem_flg = SEM_UNDO;
    key = ftok("./bcd.txt", 0);
    if (key == -1)
    {
        perror("ftok failed");
        exit(1);
    }

    semid = semget(key, NO_SEM, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget failed");
        exit(1);
    }

    while (1)
    {
        P(semid, 2);
        printf("I am p3  at time %d\n", (int)time(NULL));
        sleep(5);
        V(semid, 3);
    }
}