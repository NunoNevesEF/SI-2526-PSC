#ifndef COMANDOS_H
#define COMANDOS_H

typedef void (*command_handler_t)(char *args[]);

struct command
{
    const char *name;
    command_handler_t handler;
};

void handle_help();
void handle_exit();
void handle_load(char *args[]);
void handle_save(char *args[]);
void handle_show(char *args[]);
void handle_filter(char *args[]);
void common_handler(char *commandargs[]);

#endif