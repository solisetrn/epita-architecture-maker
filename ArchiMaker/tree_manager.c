#include "archimaker.h"

struct tree *init_tree(char *name) {

    struct tree *out = calloc(1, sizeof(struct tree));
    if (!out) {
        fprintf(stderr, "Failed to allocate memory for object: struct tree. Abort.\n");
        fflush(stderr);
        return NULL;
    }
    out->name = name;

    return out;

}

int add_child(struct tree *root, struct tree *child) {

    if (root->childN == 0)
        root->type = DIRECTORY;

   root->children = realloc(root->children, (root->childN + 1) * sizeof(struct tree));
   if (!root->children) {
       fprintf(stderr, "Couldn't add child to node: %s. Abort.\n", root->name);
       fflush(stderr);
       return 1;
   }

   root->children[root->childN] = child;
   root->childN++;
   
   return 0;

}

struct tree *parse_file(struct file_list *l);

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
