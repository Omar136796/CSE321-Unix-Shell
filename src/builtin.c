#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"
#include "history.h"

int is_builtin(char **args) {
    if (args[0] == NULL)
        return 0;

    return (
        strcmp(args[0], "cd") == 0 ||
        strcmp(args[0], "exit") == 0 ||
        strcmp(args[0], "history") == 0
    );
}

int run_builtin(char **args) {
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "cd: expected argument\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "history") == 0) {
        show_history();
        return 1;
    }

    return 0;
}

