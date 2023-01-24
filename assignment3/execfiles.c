# include <stdio.h>
# include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
 
// void delay(int number_of_seconds)
// {
//     // Converting time into milli_seconds
//     int milli_seconds = 1000 * number_of_seconds;
 
//     // Storing start time
//     clock_t start_time = clock();
 
//     // looping till required time is not achieved
//     while (clock() < start_time + milli_seconds)
//         ;
// }

int main(int argc, char * argv[])
{
    pid_t p;
    int i;
    int f;
    for(i=0;i<argc;i++)
    {
        p=fork();
        if(p==0)
        {
            char * args[]={argv[i], NULL};
            f=execv(args[0],args);
            if(f==-1){
               perror("Error during execv function");
            }   
        }
    }
	return 0;	
}
