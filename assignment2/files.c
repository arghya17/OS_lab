

# include <stdio.h>
# include <string.h>
#include <unistd.h>
#include <stdio.h>
// To use time library of C
#include <time.h>
 
void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

int main( )
{

	// Declare the file pointer
	FILE *filePointer ;
	
	// Get the data to be written in file
	filePointer = fopen("abc.txt", "r") ;
    pid_t p=fork();
    char s1,s2,s3;
	
	// Check if this filePointer is null
	// which maybe if the file does not exist
	if ( filePointer == NULL )
	{
		printf( "abc.txt file failed to open." ) ;
	}
	else
	{
        fscanf(filePointer,"%c%c%c", &s1,&s2,&s3);
        if(p==0)
        {
            printf("%c %c %c are the read character by the child process \n",s1,s2,s3);
        }
        else
        {
            printf("%c %c %c are the read character by the parent process \n",s1,s2,s3);
        }
        if(p==0)
        {
            delay(1);
            fscanf(filePointer,"%c%c%c", &s1,&s2,&s3);
            printf("%c %c %c are the read character by the child process after parent closes file \n",s1,s2,s3);
        }
		// Closing the file using fclose()
		fclose(filePointer) ;
	}
	return 0;	
}
