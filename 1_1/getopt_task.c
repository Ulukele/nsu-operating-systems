#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <ulimit.h>
#include <stdlib.h>
#include <stdio.h>
extern char** environ;
extern char* optarg;

void printHelp(FILE* stream) {
    fprintf(stream, "Available options:\n"
    "-i:\tprints real and effective user and group ids.\n"
    "-s:\tmake the process a group leader.\n"
    "-p:\tprints the ids of the process, parent process, and process group.\n"
    "-u:\tprints the ulimit value.\n"
    "-Unew_ulimit:\tset value of ulimit to new_ulimit. (requires super-user)\n"
    "-c:\tprints the size in bytes of the core file that can be created.\n"
    "--Csize:\tset the size of the core file to size. (requires super-user)\n"
    "-d:\tprints the current working directory.\n"
    "-v:\tprints environment variables and their values.\n"
    "--Vname=value:\tadds a new variable to the environment or set the value of an existing variable.\n"
    "-h:\tsee help info about options\n");
}

int main(int argc, char *argv[]) {
    char options[] = "ispuU:cC:dvV:h"; /* valid options */

    if(argc < 2) {
        fprintf(stderr, "Usage: %s options\n", argv[0]);
        printHelp(stderr);
        exit(0);
    }

    int ch = 0;
    while ((ch = getopt(argc, argv, options)) != EOF) {
        switch (ch) {
            case 'i': {
                printf("Real user id:       %20ld\n", getuid());
                printf("Effective user id:  %20ld\n", geteuid());
                printf("Real group id:      %20ld\n", getgid());
                printf("Effective group id: %20ld\n", getegid());
                break;
            }
            case 's': {
                if (setpgid(0, 0) != 0) {
                    fprintf(stderr, "Failed to make the process a group leader.\n");
                }
                else {
                    printf("The process is now the leader of the group.\n");
                }
                break;
            }
            case 'p': {
                printf("Process number:        %20ld\n", getpid());
                printf("Parent process number: %20ld\n", getppid());
                printf("Group process number:  %20ld\n", getpgid(0));
                break;
            }
            case 'u': {
                printf("Ulimit: %ld bytes.\n", ulimit(UL_GETFSIZE));
                break;
            }
            case 'U': {
                long ulimit_val = atol(optarg);
                if(ulimit(UL_SETFSIZE, ulimit_val) != ulimit_val) {
                    fprintf(stderr,"You must be super-user to increase ulimit\n");
                }
                else {
                    printf("Set ulimit: %ld bytes.\n", ulimit_val);
                }
                break;
            }
            case 'c': {
                struct rlimit rl;
                getrlimit(RLIMIT_CORE, &rl);
                printf("Current core file limit: %20lu bytes\n", rl.rlim_cur);
                printf("Max core file limit:     %20lu bytes\n", rl.rlim_max);
                break;
            }
            case 'C': {
                struct rlimit rl;
                getrlimit(RLIMIT_CORE, &rl);
                long limit = atol(optarg);
                rl.rlim_cur = limit;
                if (limit > rl.rlim_max) {
                    rl.rlim_max = limit;
                }
                if (setrlimit(RLIMIT_CORE, &rl) != 0) {
                    fprintf(stderr, "You must be super-user to increase core file limit\n");
                }
                break;
            }
            case 'd': {
                printf("Current working directory is: %s\n", getcwd(NULL, 100));
                break;
            }
            case 'v': {
                printf("Environment variables:\n");
                char **env_var;
                for (env_var = environ; *env_var != NULL; ++env_var) {
                    printf("%s\n", *env_var);
                }
                break;
            }
            case 'V': {
                if (putenv(optarg) != 0) {
                    fprintf(stderr, "Failed to set %s\n", optarg);
                }
                break;
            }
            case 'h': {
                printHelp(stdout);
                break;
            }
        }
    }
    return 0;
}
