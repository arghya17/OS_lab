#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int i;

    for (i = 1; i < argc; i+=2) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Error: fork failed");
            return 1;
        }

        if (pid == 0) {
            char *token;
            char *a[20];
            int j = 0;
            a[j]=argv[i];
            a[j+1]=argv[i+1];
            a[j+2] = NULL;

            if (execvp(a[0], a) == -1) {
                perror("execvp failed");
                return 1;
            }
        }
    }
    

    return 0;
}