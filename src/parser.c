#include<stdio.h>
#include<string.h>


int  parse_pipeline(char *input, char **commands){ 
    int i = 0;
    char *token = strtok(input, "|");

    while(token != NULL){
        commands[i] = token;
        token = strtok(NULL, "|");
        i++;
    }

    commands[i] = NULL;
    return i;
}

void parse_input(char *input, char **args){ 
    int i = 0;
    char *token = strtok(input, " \n");

    while(token != NULL){
        args[i] = token;
        token = strtok(NULL, " \n");
        i++;
    }

    args[i] = NULL;
}
