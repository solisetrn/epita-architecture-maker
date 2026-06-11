#ifndef ARCHIMAKER_H
#define ARCHIMAKER_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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

// tree_manager.c
struct tree *init_tree(enum type type, char *name);
void destroy_tree(struct tree *root);

#endif
