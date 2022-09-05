#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>


int main(int argc, char** argv) {

    if (argc < 2) {
        fprintf(stderr, "Specify at least 1 argument -- command");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error while fork: ");
        return 1;
    }
    if (pid == 0) { // child process
        int exec_status = execvp(argv[1], &argv[1]);
        if (exec_status == -1) {
            perror("Error while executing: ");
            return 1;
        }
    }

    int ret_status;
    pid_t ret;
    ret = wait(&ret_status);

    if (ret == -1) {
        perror("Error in wait(): ");
        return 1; 
    }

    if (WIFEXITED(ret_status) != 0) {
        int exit_status = WEXITSTATUS(ret_status);
        printf("Exit status: %d\n", exit_status);
    }

    if (WIFSIGNALED(ret_status) != 0) {
        int sig_num = WTERMSIG(ret_status);
        printf("Signal %d caused ending program\n", sig_num);
    }
    
    return 0;
}
