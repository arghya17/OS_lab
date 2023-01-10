#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int factorial(int n){
    if(n==1)
    {
        return 1;
    }
    return n*factorial(n-1);
}
int main(int argc, char* argv[])
{
	int n=argc;
    int i=0;
    pid_t p;
    for(i=0;i<n;i++)
    {
        p=fork();
        if(p==0)
        {
            printf("Factorial of %d is %d \n",atoi(argv[i]),factorial(atoi(argv[i])) );
            break;
        }
    }
	return 0;
}



