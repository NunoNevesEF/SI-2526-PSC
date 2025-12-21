#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "comandos.h"

extern Command commands[];
extern size_t n_commands;

void prepareArgs(char input[], char *commandargs[], size_t max_size)
{
    if (fgets(input, 100, stdin) == NULL)
    {
        return;
    }
    input[strcspn(input, "\n")] = '\0';
    char *args = strtok(input, " ");
    size_t i = 0;
    while (args != NULL && i < max_size)
    {
        commandargs[i++] = args;
        args = strtok(NULL, " ");
    }
    commandargs[i] = NULL;
}

void execComand(char *commandargs[], Command *commands, size_t command_count)
{
    if (commandargs == NULL || commandargs[0] == NULL)
    {
        printf("No command provided.\n");
        return;
    }

    for (size_t i = 0; i < command_count; i++)
    {
        if (commands[i].command_name == NULL)
            continue; /* segurança */
        if (strcmp(commandargs[0], commands[i].command_name) == 0)
        {
            if (commands[i].execute == NULL)
            {
                printf("Error: Command %s has no execute function.\n", commands[i].command_name);
                return;
            }
            commands[i].execute(commandargs);
            return;
        }
    }
    printf("No such command exists, please try again.\n");
}

int main()
{
    printf("Prog Starting...\n");
    char input[100];
    const size_t max_size = 5;
    char *commandargs[max_size];

    initial_commands(commands, &n_commands);

    while (true)
    {
        printf("$ ");
        prepareArgs(input, commandargs, max_size);
        execComand(commandargs, commands, n_commands);
    }
    return 0;
}
