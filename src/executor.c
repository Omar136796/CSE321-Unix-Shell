#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<fcntl.h>

void handle_redirection(char **args){
    for(int i=0; args[i] != NULL;i++) {
        if (strcmp(args[i], ">") == 0) {
            int fd = open(args[i+1],O_CREAT | O_WRONLY | O_TRUNC, 0644 );
            if (fd < 0) {
                perror("couldnt open file...!");
                exit(1);
            } else {
                dup2(fd, STDOUT_FILENO);
                close(fd);
                args[i] = NULL;
            }
        } else if (strcmp(args[i], ">>") == 0) {
            int fd = open(args[i+1],O_CREAT | O_WRONLY | O_APPEND, 0644 );
            if (fd < 0) {
                perror("couldnt open file...!");
                exit(1);
            } else {
                dup2(fd, STDOUT_FILENO);
                close(fd);
                args[i] = NULL;
            }

        } else if (strcmp(args[i], "<") == 0) {
            int fd = open(args[i+1],O_RDONLY, 0644 );
            if (fd < 0) {
                perror("couldnt open file...!");
                exit(1);
            } else {
                dup2(fd, STDIN_FILENO);
                close(fd);
                args[i] = NULL;
            }

        }
   }
}


int execute_command(char **args, int background) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        handle_redirection(args);

        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        if (!background) {
            int status;
            waitpid(pid, &status, 0);
            return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
            return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 0 : -1;
        } else {
            printf("[Background PID: %d]\n", pid);
        }
    }

    return 0;
}





