#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    FILE *fp = fopen("abc.txt", "w");
    if (fp == NULL) {
        printf("Error in file opening");
    }
    pid_t pid = fork();
    if (pid == 0) {
        fprintf(fp, "Child process writing in the file\n");
        fclose(fp);
    } else {
        // parent process
        fprintf(fp, "Parent process writing to file\n");
        fclose(fp);
    }
    return 0;
}
