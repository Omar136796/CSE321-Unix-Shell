#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "parser.h"
#include "pipe_executor.h"
#include "executor.h"

#define MAX_CMDS 10
#define MAX_ARGS 100

void execute_pipeline(char *input) {
    char *commands[MAX_CMDS];
    int num_cmds = parse_pipeline(input, commands);

    int pipefds[2 * (num_cmds - 1)];

    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe");
            exit(1);
        }
    }

    int cmd_idx = 0;
    int pid;
    for (int i = 0; i < num_cmds; i++) {
        char *args[MAX_ARGS];
        parse_input(commands[i], args);

        pid = fork();

        if (pid == 0) {

            if (i != 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
            }


            if (i != num_cmds - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
            }


            for (int j = 0; j < 2 * (num_cmds - 1); j++) {
                close(pipefds[j]);
            }
            
            handle_redirection(args);

            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
    }


    for (int i = 0; i < 2 * (num_cmds - 1); i++) {
        close(pipefds[i]);
    }

    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }
}
