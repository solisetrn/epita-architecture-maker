#ifndef ARCHIMAKER_H
#define ARCHIMAKER_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>

// dlist_manager.c
struct node {
    int hier;
    char *name;
    struct node *prev;
    struct node *next;
};

struct file_list {
    struct node *head;
    struct node *tail;
};

struct node *make_node(char *name, int hier);
void dlist_append(struct file_list *l, struct node *node);
void destroy_dlist(struct file_list *l);
void print_dlist(struct file_list *l); // DEBUG function

// tree_manager.c
enum type {
    FILENAME, // == 0
    DIRECTORY, // == 1
    ROOT // == 2
};

struct tree {
    enum type type; // FILE or DIRECTORY
    char *name; // Name of the directory or file
    size_t childN; // Number of children
    struct tree **children;
};

struct tree *init_tree(char *name);
int add_child(struct tree *root, struct tree *child);
struct tree *convert_to_tree(struct file_list *l);
void destroy_tree(struct tree *root);
void print_tree(struct tree *root); // DEBUG function

// parser.c
enum arg_type {
    HELP, // -h, --help
    OPEN, // -f
    DEST, // -d, --dest
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
int get_hier(char *line);
void print_cmd(struct cmd *cmd); //  DEBUG function

// executer.c
int make_items(struct tree *root, char *prefix);

// main.c
void display_helper();

#endif
