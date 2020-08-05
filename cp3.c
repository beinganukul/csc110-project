#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFERSIZE 99999
#define COPYMORE 0644

void oops(char *, char*);
int copyDir(char *src, char *dest);
int copyFiles(char *src, char *dest);
int dostat(char *filename);

int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "usage:\n %s source destination\n", *argv);
    }

    char *src = argv[1];
    char *dest = argv[2];

    if (src[0] != '/' && dest[0] != '/') {
        copyFiles(src, dest);
    } // copy file inside working dir

    else if(src[0] != '/' && dest[0] == '/') {
        int i;
        //for (i = 1; i <= strlen(dest); i++) {
           // dest[(i-1)] = dest[i]; // this line removes the / form dest 
        //}
        //strcat(dest, "/"); // appends / in end of string
        strcat(dest, src);
        copyFiles(src, dest);
    } // copy file from working dir to another dir

    else if(src[0] == '/' && dest[0] == '/') {
        int i;
        //for (i = 1; i <= strlen(dest); i++) {
        //    dest[(i-1)] = dest[i];
        //}
        //for (i = 1; i <= strlen(src); i++) {
        //    src[(i-1)] = src[i];
        //}
        copyDir(src, dest);
        
    } // copy form dir1 to dir2
    else {
    fprintf(stderr, "usage: \n ./cp2 source destination\n");
    }
}

int copyDir(char *source, char *destination) {
    DIR *dir_ptr = NULL;
    struct dirent *direntp;
    char tempDest[strlen(destination)+1];
    char tempSrc[strlen(source)+1];
    strcat(destination, "/");
    //strcat(source, "/");
    strcpy(tempDest, destination);
    strcpy(tempSrc, source);

    struct stat fileinfo;

    if((dir_ptr = opendir(source)) == NULL) {
        fprintf(stderr, "cp1: cannot open %s for copying\n", source);
        return 0;
    }
    else {
        while ((direntp = readdir(dir_ptr))) { //////////from here tomorrow  hhhhmhmhmhmhmhmhmhm
            if(dostat(direntp->d_name)) {
                strcat(tempDest, direntp->d_name);
                strcat(tempSrc, direntp->d_name);
                copyFiles(tempSrc, tempDest);
                strcpy(tempDest, destination);
                strcpy(tempSrc, source);
            }
        }
        closedir(dir_ptr);
        return 1;
    }
}

int dostat(char *filename) {
    struct stat fileInfo;

    if (stat(filename, &fileInfo) >= 0)
    if(S_ISREG(fileInfo.st_mode))
        return 1;
    else
        return 0;
    return 0;
}

int copyFiles(char *source, char *destination) {
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];

    if ((in_fd = open(source, O_RDONLY)) == -1) {
        oops("Cannot open", source);
    }

    if ((out_fd = creat(destination, COPYMORE)) == -1) {
        oops("Cannot create ", destination);
    }

    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0) {
        if (write(out_fd, buf, n_chars) != n_chars) {
            oops("Write error to ", destination);
        }
        if(n_chars == -1) {
            oops("Read error form ", source);
        }
    }

    if(close(in_fd) == -1 || close(out_fd) == -1) {
        oops("Error closing files", "");
    }
    return 1;
}

void oops(char *s1, char *s2) {
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    exit(1);
}