#ifndef EXECUTOR_H
#define EXECUTOR_H

int execute_command(char **args, int bg);
void handle_redirection(char **args);

#endif
