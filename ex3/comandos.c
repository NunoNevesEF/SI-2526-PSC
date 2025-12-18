#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comandos.h"
#include "table.h"

struct command commands[] = {
    {"help", handle_help},
    {"exit", handle_exit},
    {"load", handle_load},
    {"save", handle_save},
    {"show", handle_show},
    {"filter", handle_filter}};

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

// TODO()
void handle_show(char *args[])
{
    if (args[1] == NULL)
    {
        printf("Wrong format, please try again: show <col><row>:<col><row>\n");
        return;
    }
}

// TODO()
void handle_filter(char *args[])
{
    if (args[1] == NULL || args[2] == NULL)
    {
        printf("Wrong format, please try again: filter <filename><data>\n");
        return;
    }
}