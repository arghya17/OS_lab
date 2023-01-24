#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int i;
    pid_t p;
    int n=atoi(argv[1]);
    int f=1;
    for(i=1;i<=n;i++)
    {
        f=f*i;
        p=fork();
        if(p!=0)
        {
            wait(NULL);
            break;     
        }
    }
    if(i<=n){
        printf("The factorial of %d is %d \n",i,f);
    }
}