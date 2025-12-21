#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "comandos.h"

extern Command commands[];
extern size_t n_commands;

int getcommand(char input[], char *commandargs[], size_t max_size)
{
    if (fgets(input, 100, stdin) == NULL)
    {
        return 0;
    }
    input[strcspn(input, "\n")] = '\0';
    char *args = strtok(input, " ");
    size_t i = 0;
    while (args != NULL && i < max_size)
    {
        commandargs[i++] = args;
        args = strtok(NULL, " ");
    }
    return (int)i;
}


void execComand(char *commandargs[], Command *commands, size_t command_count) {
    if (commandargs == NULL || commandargs[0] == NULL) {
        printf("No command provided.\n");
        return;
    }

    for (size_t i = 0; i < command_count; i++) {
        if (commands[i].command_name == NULL) continue; /* segurança */
        if (strcmp(commandargs[0], commands[i].command_name) == 0) {
            if (commands[i].execute == NULL) {
                printf("Error: Command %s has no execute function.\n", commands[i].command_name);
                return;
            }
            commands[i].execute(commandargs);
            return;
        }
    }
    printf("No such command exists, please try again.\n");
}


void add_command_handler(char *commandargs[]) {
    if (commandargs[1] == NULL || commandargs[2] == NULL) {
        printf("Wrong format, please try again: command <libfile.so> <function>\n");
        return;
    }
    void (*f1)(char *[]) = common_handler;
    register_command(commands, &n_commands, commandargs[2], f1,commandargs[1]);
}



void initial_commands(Command *commands, size_t *command_count){
    register_command(commands, command_count, "help", handle_help,"default");
    register_command(commands, command_count, "exit", handle_exit,"default");
    register_command(commands, command_count, "load", handle_load,"default");
    register_command(commands, command_count, "save", handle_save,"default");
    register_command(commands, command_count, "show", handle_show,"default");
    register_command(commands, command_count, "filter", handle_filter,"default");
    register_command(commands, command_count, "command", add_command_handler,"default");

    printf("The commands avaliable are the following ->\n");
    for (size_t i = 0; i < *command_count; i++) {
        printf(" > %s\n", commands[i].command_name);
    }

}

void register_command(Command *commands, size_t *command_count, const char *name, void (*execute)(char *args[]),const char *libname) {
        commands[*command_count].command_name = strdup(name);
        commands[*command_count].execute = execute;
        commands[*command_count].lib_name = strdup(libname);
        (*command_count)++;
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
        int argc = getcommand(input, commandargs, max_size);
        execComand(commandargs, commands, n_commands);
    }
    return 0;
}
