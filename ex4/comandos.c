#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comandos.h"
#include "table.h"
#include <dlfcn.h>

struct command commands[] = {
    {"help", handle_help},
    {"exit", handle_exit},
    {"load", handle_load},
    {"save", handle_save},
    {"show", handle_show},
    {"filter", handle_filter},
    {"command", common_handler}};

const int n_commands = sizeof(commands) / sizeof(commands[0]);

// FIX THIS!! Not enough
void cleanTable(struct table *t)
{
    free(t->columns);
    free(t->rows);
    free(t);
}

/**
 * Implementation of initial commands with the start of the application.
 */

void handle_help()
{
    printf("The commands avaliable are the following ->\n");
    for (int i = 0; i < n_commands; i++)
    {
        printf(" > %s\n", commands[i].name);
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

void common_handler(char *commandargs[])
{
    if (commandargs[1] == NULL)
    {
        printf("Wrong format, please try again: command <libfile.so>\n");
        return;
    }

    void *handle = dlopen(commandargs[1], RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        return;
    }

    void (*f)(char *args[]);
    f = (void (*)(char *[]))dlsym(handle, "handle_remove_first");

    if (!f)
    {
        fprintf(stderr, "dlsym error: %s\n", dlerror());
        dlclose(handle);
        return;
    }

    f(NULL);
    dlclose(handle);
}
