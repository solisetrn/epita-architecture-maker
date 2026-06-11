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

   root->children = realloc(root->children, (root->childN + 1) * sizeof(struct tree));
   if (!root->children) {
       fprintf(stderr, "Error while adding child to node: %s. Abort.\n", root->name);
       return 1;
   }

   root->children[root->childN] = child;
   root->childN++;
   
   return 0;

}

void destroy_tree(struct tree *root) {

    // if the node is a leaf then free the pointer
    if (!root->children) {
        if (root->name)
            free(root->name);
        free(root);
        return;
    }

    // else free all children recursively before freeing the pointer itself
    size_t i = 0;
    while (i < root->childN) {
        destroy_tree(root->children[i]);
        i++;
    }

    free(root->children);
    if (root->name)
        free(root->name);
    free(root);

}
