#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "executor.h"
#include "builtin.h"
#include "history.h"
#include "pipe_executor.h"
#include "signal.h"


#define MAX_LINE 1024
#define MAX_ARGS 100



void process_line(char *line) {

    char line_copy[MAX_LINE];
    strcpy(line_copy, line);
    

    char *semicolon_saveptr = NULL;
    char *semicolon_cmd = strtok_r(line_copy, ";", &semicolon_saveptr);

    while (semicolon_cmd != NULL) {

        while (*semicolon_cmd == ' ') semicolon_cmd++;
        char *end = semicolon_cmd + strlen(semicolon_cmd) - 1;
        while (end > semicolon_cmd && *end == ' ') *end-- = '\0';

        if (strstr(semicolon_cmd, "&&") != NULL) {

            char and_copy[MAX_LINE];
            strcpy(and_copy, semicolon_cmd);
            
            char *and_saveptr = NULL;
            char *and_cmd = strtok_r(and_copy, "&&", &and_saveptr);
            int success = 1;  
            
            while (and_cmd != NULL && success) {

                while (*and_cmd == ' ') and_cmd++;
                end = and_cmd + strlen(and_cmd) - 1;
                while (end > and_cmd && *end == ' ') *end-- = '\0';


                char cmd_copy[MAX_LINE];
                strcpy(cmd_copy, and_cmd);
                
                char *args[MAX_ARGS];
                parse_input(cmd_copy, args);

                if (args[0] != NULL) { 
                    if (is_builtin(args)) {
                        success = (run_builtin(args) == 0);
                    } else {
                        success = (execute_command(args, 0) == 0);
                    }
                }

                and_cmd = strtok_r(NULL, "&&", &and_saveptr);
            }
        } else if (strchr(semicolon_cmd, '|') != NULL) {

            execute_pipeline(semicolon_cmd);
        } else {

            char cmd_copy[MAX_LINE];
            strcpy(cmd_copy, semicolon_cmd);
            
            char *args[MAX_ARGS];
            parse_input(cmd_copy, args);

            if (args[0] != NULL) {  
                if (is_builtin(args)) {
                    run_builtin(args);
                } else {
                    execute_command(args, 0);
                }
            }
        }

        semicolon_cmd = strtok_r(NULL, ";", &semicolon_saveptr);
    }
}


int main() {
    char input[MAX_LINE];

    setup_signal_handler();

    while (1) {
        printf("sh> ");
        fflush(stdout);

        if (fgets(input, MAX_LINE, stdin) == NULL) {
            printf("\n");
            break;
        }

        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';

        if (strlen(input) == 0)
            continue;

        add_history(input);
        char temp[MAX_LINE];
        strcpy(temp, input);
        process_line(input);
    }

    return 0;
}

