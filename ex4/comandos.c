#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comandos.h"
#include "table.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>


Command commands[20];
size_t n_commands = 0;


// FIX THIS!! Not enough
void cleanTable(struct table *t)
{
    free(t->columns);
    free(t->rows);
    free(t);
}


void handle_help()
{
    printf("The commands avaliable are the following ->\n");
    for (int i = 0; i < n_commands; i++)
    {
        printf(" > %s\n", commands[i].command_name);
    }
}

void handle_exit()
{
    if (current_table != NULL)
    {
        cleanTable(current_table);
    }
    exit(0);
}

void handle_load(char *args[])
{
    if (args[1] == NULL)
    {
        printf("Wrong format, please try again: load <filename>\n");
        return;
    }

    if (current_table != NULL)
    {
        cleanTable(current_table);
    }

    current_table = table_load_csv(args[1]);
}

void handle_save(char *args[])
{
    if (args[1] == NULL)
    {
        printf("Wrong format, please try again: save <filename>\n");
        return;
    }

    table_save_csv(current_table, args[1]);
}

void handle_show(char *args[])
{
    if (args[1] == NULL)
    {
        printf("Wrong format, please try again: show <col><row>:<col><row>\n");
        return;
    }

    char initialColumn = args[1][0];
    char initialRow = args[1][1];
    char lastColumn = args[1][3];
    char lastRow = args[1][4];

    show_sub_table(initialColumn, initialRow, lastColumn, lastRow);
}

void handle_filter(char *args[])
{
    if (args[1] == NULL || args[2] == NULL)
    {
        printf("Wrong format, please try again: filter <column> <data>\n");
        return;
    }

    const char *filter_args[2];
    filter_args[0] = args[1];
    filter_args[1] = args[2];

    struct table *new_table = table_filter(current_table, filter_data_from_column, filter_args);

    current_table = new_table;
}





Command *find_command(const char *name)
{
    if (!name) return NULL;
    for (size_t i = 0; i < n_commands; ++i) {
        if (commands[i].command_name && strcmp(commands[i].command_name, name) == 0)
            return &commands[i];
    }
    return NULL;
}



void common_handler(char *commandargs[])
{

    Command *cmd = find_command(commandargs[0]);
    const char *funcname = cmd->command_name;
    const char *libname = cmd->lib_name;

    void *handle = dlopen(libname, RTLD_NOW | RTLD_LOCAL);
    if (!handle) {
        char altpath[4096];
        snprintf(altpath, sizeof(altpath), "./%s", libname);
        handle = dlopen(altpath, RTLD_NOW | RTLD_LOCAL);
    }

    if (!handle) {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        return;
    }

    dlerror();

    typedef void (*cmd_fn_t)(char **);
    cmd_fn_t f = (cmd_fn_t)dlsym(handle, funcname);
    char *err = dlerror();
    if (err) {
        fprintf(stderr, "dlsym error: %s\n", err);
        dlclose(handle);
        return;
    }


    f(NULL);
    dlclose(handle);
}








