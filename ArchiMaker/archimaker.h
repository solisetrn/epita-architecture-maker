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
    DIRECTORY, // == 0
    FILENAME // == 1
};

struct tree {
    enum type type; // FILE or DIRECTORY
    char *name; // Name of the directory or file
    size_t childN; // Number of children
    struct tree **children;
};

struct tree *init_tree(enum type type, char *name);
int add_child(struct tree *root, struct tree *child);
void destroy_tree(struct tree *root);

#endif
