#include "archimaker.h"

struct tree *init_tree(enum type type, char *name) {

    struct tree *out = calloc(1, sizeof(struct tree));
    if (!out) {
        fprintf(stderr, "Error while allocating memory for object: struct tree. Abort.\n");
        return NULL;
    }

    out->type = type;
    out->name = name;

    return out;

}

int add_child(struct tree *root, struct tree *child) {

   root->children = realloc() 

}

void destroy_tree(struct tree *root) {

    // if leaf then free the pointer
    if (!root->children) {
        if (root->name)
            free(root->name);
        free(root);
        return;
    }

    // else free all children recursively before freeing the pointer itself
    size_t i = 0;
    struct tree *curr = root->children[i];
    while (curr) {
        destroy_tree(curr);
        i++;
        curr = root->children[i];
    }

    free(root->children);
    if (root->name)
        free(root->name);
    free(root);

}
