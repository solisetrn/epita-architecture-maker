#ifndef ARCHIMAKER_H
#define ARCHIMAKER_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// tree_manager.c
enum type {
    FILENAME, // == 0
    DIRECTORY // == 1
};

struct tree {
    enum type type; // FILE or DIRECTORY
    char *name; // Name of the directory or file
    size_t childN; // Number of children
    struct tree **children;
};

struct tree *init_tree(char *name);
int add_child(struct tree *root, struct tree *child);
void destroy_tree(struct tree *root);

// parser.c
enum arg_type {
    HELP, // -h, --help
    OPEN, // -f
    STR // anything that's not a flag (doesn't start with '-')
};

struct arg { // NODES
    enum arg_type type;
    char *data;
    struct arg *next;
};

struct cmd { // LINKED LIST
    struct arg *head;
    struct arg *tail;
};

void cmd_append(struct cmd *cmd, struct arg *arg);
void destroy_args(struct arg *arg);
void destroy_cmd(struct cmd *cmd);
struct cmd *parse_command(int argc, char *argv[]);
char *get_name(char *line);
size_t get_hier(char *line);
void print_cmd(struct cmd *cmd); //  DEBUG function

// main.c
void display_helper();

#endif
