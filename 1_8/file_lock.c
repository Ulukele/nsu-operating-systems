#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "specify file name\n");
        return 0;
    }
    struct flock lock;
    char command[strlen(argv[1]) + 5];

    int fd;

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror("Error while open file");
        return 0;
    }

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        if (errno == EACCES || errno == EAGAIN) {
            printf("%s busy -- try later\n", argv[1]);
            return 0;
        }
        perror("Error while lock file");
        return 1;
    }

    sprintf(command, "nano %s", argv[1]);
    system(command);

    lock.l_type = F_UNLCK;
    
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error while unlock file");
        return 1;
    }
    
    close(fd);
    return 0;
}
