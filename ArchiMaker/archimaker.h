#ifndef ARCHIMAKER_H
#define ARCHIMAKER_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

enum type {
    DIR,
    FILE
};

struct tree {
    enum type type;
    char *name;
    struct tree **children;
}

#endif
