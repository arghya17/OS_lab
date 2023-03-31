#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BLOCK_SIZE 2048
#define HEADER_SIZE 1024
typedef struct
{
    int n;   // total number of blocks
    int s;   // size of each block
    int ubn; // used blocks
    int fbn; // free blocks
    // char ub[0]; // bitmap showing which blocks are used
    // ub[0] is used as to initialize a character pointer
} file_header;

int init_File_dd(const char *fname, int bsize, int bno)
{
    FILE *fp;
    file_header header;
    int header_size, file_size, i;
    char *buffer;

    // calculate header size and file size
    header_size = HEADER_SIZE;
    file_size = header_size + bsize * bno;

    // allocate memory for header and buffer
    buffer = calloc(1, file_size);
    if (!buffer)
    {
        return -1; // memory allocation failed
    }

    // initialize header
    header.n = bno;
    header.s = bsize;
    header.ubn = 0;
    header.fbn = bno;

    // write header to buffer
    memcpy(buffer, &header, sizeof(file_header));

    // create file and write buffer to it
    fp = fopen(fname, "wb");
    if (!fp)
    {
        free(buffer);
        return -1; // failed to create file
    }
    fwrite(buffer, 1, file_size, fp); // writing buffer contents
    fclose(fp);

    free(buffer);

    return 0;
}

int get_freeblock(const char *fname)
{
    FILE *fp;
    file_header header;
    int i, j, ub_size, free_block = -1;
    char bit_mask;

    fp = fopen(fname, "rb+");
    if (!fp)
    {
        return -1; // failed to open file
    }

    // read header from file
    fread(&header, sizeof(file_header), 1, fp); // 1 is the size of each element to be written in bytes
    ub_size = header.n / 8 + (header.n % 8 ? 1 : 0);

    // ub_size is the number of bytes used in a bitmap
    char bitmap[1024 - sizeof(file_header)];
    fread(bitmap, sizeof(bitmap), 1, fp);
    // search for first free block in bitmap
    for (i = 0; i < ub_size; i++)
    {
        if ((int)bitmap[i] != 0xFF)
        { // if all bits are set, the block is not free
            for (j = 0; j < 8; j++)
            {
                bit_mask = 1 << (7 - j);
                if (!(bitmap[i] & bit_mask))
                { // found a free block
                    free_block = i * 8 + j;
                    bitmap[i] = bitmap[i] | bit_mask; // set bit in bitmap
                    header.ubn++;
                    header.fbn--;
                    char fill_block[header.s];
                    memset(fill_block, 0xff, header.s);
                    fseek(fp, 1024 + free_block * header.s, SEEK_SET); // fill block with ones
                    fwrite(fill_block, header.s, 1, fp);
                    // here we are filling the free block with bit 1 memset(start pointer, value,no of bytes to be filled)
                    break;
                }
            }
            break;
        }
    }

    // write header back to file
    fseek(fp, 0, SEEK_SET);
    fwrite(&header, HEADER_SIZE, 1, fp);
    // writing the file

    fclose(fp);

    return free_block;
}
int check_fs(const char *fname)
{
    int fd = open(fname, O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return 0;
    }
    file_header f;
    lseek(fd, 0, SEEK_SET);
    read(fd, &f, sizeof(file_header));
    return (f.ubn + f.fbn) == f.n;
}
int free_block(const char *fname, int bno)
{
    FILE *fp;
    file_header header;
    int i, j, ub_size, free_block = -1;
    fp = fopen(fname, "r+b");
    if (!fp)
    {
        return 0; // failed to open file
    }
    // read header from file
    fread(&header, sizeof(file_header), 1, fp); // 1 is the size of each element to be written in bytes
    i = bno / 8;
    j = bno % 8;
    char bitmask;
    bitmask = 1 << (7 - j);
    bitmask = ~bitmask;
    char bitmap[1024 - sizeof(file_header)];
    fread(bitmap, sizeof(bitmap), 1, fp);
    bitmap[i] &= bitmask;
    header.fbn += 1;
    header.ubn -= 1;
    char fill_block[header.s];
    memset(fill_block, 0x00, header.s);
    fseek(fp, 1024 + bno * header.s, SEEK_SET); // fill block with ones
    fwrite(fill_block, header.s, 1, fp);
    fseek(fp, 0, SEEK_SET);
    fwrite(&header, sizeof(file_header), 1, fp);
    fwrite(bitmap, sizeof(bitmap), 1, fp);
    // writing the file

    fclose(fp);

    return 1;
}
int main()
{
    char fname[] = "testfile.txt";
    int bsize = BLOCK_SIZE;
    int bno = 40;

    if (init_File_dd(fname, bsize, bno) == -1)
    {
        perror("Error initializing file");
        exit(EXIT_FAILURE);
    }

    printf("File initialized successfully\n");
    int i, block;
    for (i = 0; i < 2; i++)
    {

        block = get_freeblock(fname);
        if (block == -1)
        {
            perror("Error getting free block");
            exit(EXIT_FAILURE);
        }

        printf("Free block found at index %d\n", block);
    }

    if (free_block(fname, block) == 0)
    {
        perror("Error freeing block");
        exit(EXIT_FAILURE);
    }

    printf("Block %d freed successfully\n", block);

    if (check_fs(fname) == 1)
    {
        perror("Error checking file system");
        exit(EXIT_FAILURE);
    }

    printf("File system integrity checked successfully\n");

    return EXIT_SUCCESS;
}
