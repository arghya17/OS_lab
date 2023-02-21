#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

char *match;
void listDir(char *dirName)

{

    DIR *dir;

    struct dirent *dirEntry;

    struct stat inode;

    char name[1000];

    dir = opendir(dirName);

    if (dir == NULL)
    {

        perror("Could not open directory : ");

        exit(1);
    }

    while ((dirEntry = readdir(dir)) != 0)
    {

        sprintf(name, "%s/%s", dirName, dirEntry->d_name);
        // printf("%s \n", dirEntry->d_name);
        lstat(name, &inode);

        if (S_ISDIR(inode.st_mode))
        {
            if (strstr(dirEntry->d_name, match))
            {
                printf("dir %s  matches %s\n", name, match);
            }

            if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
            {

                listDir(name);
            }
        }
        else if (S_ISREG(inode.st_mode))
        {
            // regular file
            if (strstr(dirEntry->d_name, match))
            {
                printf("file %s  matches %s\n", name, match);
            }
        }

        else
        {

            if (S_ISLNK(inode.st_mode))
            {
                if (strstr(dirEntry->d_name, match))
                {
                    printf("lnk %s  matches %s\n", name, match);
                }
            }
        }

        // printf(" %s\n", dirEntry->d_name);
    }
    return;
}

int main(int argc, char *argv[])

{

    if (argc == 1)
    {

        printf("Use : %s path matchname\n", argv[0]);

        exit(0);
    }
    match = argv[2];
    listDir(argv[1]);
}