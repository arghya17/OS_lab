#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define READEND 0
#define WRITEEND 1

typedef struct student
{
    int roll;
    char name[100];
} student;

void swap(student *xp, student *yp)
{
    student temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(student arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j].roll > arr[j + 1].roll)
                swap(&arr[j], &arr[j + 1]);
}

int main()
{
    int pipefd1[2];
    int pipefd2[2];
    int status;
    status = pipe(pipefd1);
    if (status == -1)
    {
        perror("pipefd");
        exit(0);
    }
    status = pipe(pipefd2);
    if (status == -1)
    {
        perror("pipefd");
        exit(0);
    }
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
    }
    student ss[100];
    if (pid == 0)
    {
        // child process
        close(pipefd1[WRITEEND]);
        close(pipefd2[READEND]);
        int i, n;
        i = 0;
        int numbytes;
        while (1)
        {
            numbytes = read(pipefd1[READEND], &ss[i], sizeof(ss[i]));
            if (numbytes == 0)
            {
                printf("values reading finished \n");
                break;
            }
            i += 1;
        }
        n = i;
        close(pipefd1[READEND]);
        bubbleSort(ss, n);
        for (i = 0; i < n; i++)
        {
            write(pipefd2[WRITEEND], &ss[i], sizeof(ss[i]));
        }
        close(pipefd2[WRITEEND]);
    }
    else
    {
        // parent process
        close(pipefd1[READEND]);
        close(pipefd2[WRITEEND]);
        int n;
        printf("Enter number of students \n");
        scanf("%d", &n);
        int i = 0;
        while (i < n)
        {
            printf("Enter roll number and name \n");
            scanf("%d %s", &ss[i].roll, (ss[i].name));
            write(pipefd1[WRITEEND], &ss[i], sizeof(ss[i]));
            i += 1;
        }
        close(pipefd1[WRITEEND]);
        printf("sorted list \n");
        for (i = 0; i < n; i++)
        {
            read(pipefd2[READEND], &ss[i], sizeof(ss[i]));
            printf("%d %s \n", ss[i].roll, ss[i].name);
        }
        close(pipefd2[READEND]);
    }
    return 0;
}