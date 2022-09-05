#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libgen.h>


int main() {


    FILE* fptrs[2];
    if (p2open("/bin/sort -n", fptrs) == -1) {
        perror("Error while open pipe");
        return 1;
    }

    srand(time(NULL));
    int number;
    for (int i = 0; i < 100; ++i) {
        number = rand() % 100;
        fprintf(fptrs[0], "%d\n", number);
    }
    if (fclose(fptrs[0]) != 0) {
        perror("Error while close pipe");
        return 1;
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            fscanf(fptrs[1], "%d", &number);
            printf("%2d ", number);
        }
        printf("\n");
    }

    if (pclose(fptrs[1]) == -1) {
        perror("Error while close pipe");
        return 1;
    }

    return 0;
}
