#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>

#define BUFF_SIZE 99999

void copy_file();

void check_args(int argc) {

    if (argc < 2) {
        printf("Missing Argumnets:");
    }
    else if (argc < 3) {
        printf("Missing Destination:");
    }
}
int main(int argc, char *argv[]) {
    check_args(argc);

    // opening source file
    int sf = open(argv[1], O_RDONLY);

    // if empty
    if (sf < 0) {
        perror("Error Source Empty.");
    }
    // opening destination file
    int df = open(argv[2], O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
    
    // checking if file permisssion error
    if (errno == EEXIST) {
        char in;
        printf("Destination file exists, replace? [y/n]:");
        scanf("%c", &in);

        switch(in) {
            case 'y':
                df = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR |S_IWUSR);
                break;
            case 'n':
                close(sf);
                exit(EXIT_SUCCESS);
                break;
            default:
                exit(EXIT_FAILURE);
                break;
        }
    } 
    //else if (df < 0) {
    //    close(sf);
    //    perror("Error");
    //}
    char *src = argv[1];
    char *dest = argv[2];
    strcat(dest, src);
        printf("%s %s", src, dest); // needs debugging here
    copy_file(sf, df);
    close(sf);
    close(df);


    exit(EXIT_SUCCESS);
}

void copy_file(int sf, int df) { // this need directory copy rebuild
    int r;
    char data[BUFF_SIZE];
    while((r = read(sf, data, BUFF_SIZE)) > 0) {
        write(df, data, r);
    }
}
