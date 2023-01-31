#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>

int shmid;
int n,m,p;

void releaseSHM(int signum) {
    int status;
    status = shmctl(shmid, IPC_RMID, NULL);
    if (status == 0) {
        fprintf(stderr, "Remove shared memory with id = %d.\n", shmid);
    } else if (status == -1) {
        fprintf(stderr, "Cannot remove shared memory with id = %d.\n", shmid);
    } else {
        fprintf(stderr, "shmctl() returned wrong value while removing shared memory with id = %d.\n", shmid);
    }
    status = kill(0, SIGKILL);
    if (status == 0) {
        fprintf(stderr, "kill successful.\n");
    } else if (status == -1) {
        perror("kill failed.\n");
    } else {
        fprintf(stderr, "kill(2) returned wrong value.\n");
    }
}

void childProcess(int i) {
    int j,k;
    int *a,*b,*c;
    a = shmat(shmid, NULL, 0);
    b = a + n*m;
    c = b + m*p;
    for (j = 0; j < p; j++) {
        c[i*p+j] = 0;
        for (k = 0; k < m; k++) {
            c[i*p+j] += a[i*m+k] * b[k*p+j];
        }
    }
    shmdt(a);
    exit(0);
}

int main() {
    int i,j,k;
    pid_t pid;
    signal(SIGINT, releaseSHM);
    printf("Enter the number of rows of matrix A: ");
    scanf("%d", &n);
    printf("Enter the number of columns of matrix A and rows of matrix B: ");
    scanf("%d", &m);
    printf("Enter the number of columns of matrix B: ");
    scanf("%d", &p);
    shmid = shmget(IPC_PRIVATE,(n*p)*sizeof(int), IPC_CREAT | 0777);
    if (shmid == -1) {
        perror("shmget() failed: ");
        exit(1);
    }
    int *c;
    int a[n][m];
    int b[m][p];
    c = shmat(shmid, NULL, 0);
    printf("Enter the elements of matrix A: \n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            scanf("%d", &a[i][j]);
        }
    }
    printf("Enter the elements of matrix B: \n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < p; j++) {
            scanf("%d", &b[i][j]);
        }
    }
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) {
            int *d= shmat(shmid,NULL,0);
             for (j = 0; j < p; j++) {
                d[i*p+j] = 0;
                for (k = 0; k < m; k++) {
                    d[i*p+j] += a[i][k] * b[k][j];
                }
            }
            shmdt(d);
            exit(0);
        }
        else
        {
            wait(NULL);
        }
    }
    printf("The product matrix C is: \n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < p; j++) {
            printf("%d ", c[i*p+j]);
        }
        printf("\n");
    }
    shmdt(c);
    exit(0);
}
