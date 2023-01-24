# include <stdio.h>
# include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[])
{
    pid_t p;
    int i;
    int f;
    int stat;
    for(i=0;i<argc;i++)
    {
        p=fork();
        if(p==0)
        {
            char * args[]={argv[i], NULL};
            f=execv(args[0],args);
            if(f==-1){
               perror("error in exec function call");
            }
        }
        else
        {
            wait(&stat);
            if (WIFEXITED(stat))
                printf("Exit status: %d\n", WEXITSTATUS(stat));
            else if (WIFSIGNALED(stat))
                psignal(WTERMSIG(stat), "Exit signal");
        }
    }
	return 0;	
}
