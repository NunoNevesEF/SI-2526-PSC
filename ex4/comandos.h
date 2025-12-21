#ifndef COMANDOS_H
#define COMANDOS_H

#include <stddef.h>
#include "table.h"


typedef struct command {
    const char *command_name;
    void (*execute)(char *args[]);
    const char *lib_name;

} Command;

extern Command commands[20];
extern size_t n_commands;

void register_command(Command *commands, size_t *command_count, const char *name, void (*execute)(char *[]), const char *lib_name);


void handle_help();
void handle_exit();
void handle_load(char *args[]);
void handle_save(char *args[]);
void handle_show(char *args[]);
void handle_filter(char *args[]);
void common_handler(char *commandargs[]);

#endif