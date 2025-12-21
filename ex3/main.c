#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "comandos.h"

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

void execComand(char *commandargs[])
{
    if (strcmp(commandargs[0], "help") == 0)
    {
        handle_help();
    }
    else if (strcmp(commandargs[0], "exit") == 0)
    {
        handle_exit(current_table);
    }
    else if (strcmp(commandargs[0], "load") == 0)
    {
        handle_load(commandargs);
    }
    else if (strcmp(commandargs[0], "save") == 0)
    {
        handle_save(commandargs);
    }
    else if (strcmp(commandargs[0], "show") == 0)
    {
        handle_show(commandargs);
    }
    else if (strcmp(commandargs[0], "filter") == 0)
    {
        handle_filter(commandargs);
    }
    else
    {
        printf("No such command exists, please try again.\n");
    }
}

int main()
{
    printf("Prog Starting...\n");
    char input[100];
    size_t max_size = 5;
    char *commandargs[max_size];
    while (true)
    {
        printf("$ ");
        prepareArgs(input, commandargs, max_size);
        execComand(commandargs);
    }
}