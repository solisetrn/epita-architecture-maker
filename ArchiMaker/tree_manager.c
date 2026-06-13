#include "archimaker.h"

struct tree *init_tree(char *name) {

    struct tree *out = calloc(1, sizeof(struct tree));
    if (!out) {
        fprintf(stderr, "ArchiMaker: *** Failed to allocate memory for object: struct tree. Abort.\n");
        fflush(stderr);
        return NULL;
    }
    out->name = strdup(name);

    return out;

}

int add_child(struct tree *root, struct tree *child) {

    if (root->childN == FILENAME)
        root->type = DIRECTORY;

    root->children = realloc(root->children, (root->childN + 1) * sizeof(struct tree));
    if (!root->children) {
        fprintf(stderr, "ArchiMaker: Couldn't add child to node: %s. Abort.\n", root->name);
        fflush(stderr);
        return 1;
    }

    root->children[root->childN] = child;
    root->childN++;
   
    return 0;

}

struct tree *DFS(struct tree *root, char *name) {

    if (!root) // base case
        return NULL;

    if (strcmp(name, root->name) == 0) // found
        return root;

    for (size_t i = 0; i < root->childN; i++) {
        struct tree *temp = DFS(root->children[i], name);
        if (temp)
            return temp;
    }

    return NULL; // negative search

}

struct tree *convert_to_tree(struct file_list *l) {

    if (!l->head) { // empty dlist
        fprintf(stderr, "ArchiMaker: *** Couldn't fetch the file architecture correctly, it must be pasted in 'Archifile' or a specified file using '-f'. Abort.\n");
        fflush(stderr);
        destroy_dlist(l);
        return NULL;
    }

    struct tree *root = init_tree(l->head->name);
    if (!root) {
        // error message handled by the function
        destroy_dlist(l);
        return NULL;
    }

    struct node *parent = l->head; // file with hierarchy code 0, should be 'epita-prepa-computer-science-prog-X0X-p-0X-20XX-firstname.lastname'
    struct node *curr = l->head->next; // since no other file is supposed to have a hierarchy code of 0, it's the second file (with code 1)

    while (curr) { // 'curr' should just iterate through the dlist while 'parent' moves around

        struct tree *child = init_tree(curr->name);
        if (!child) {
            destroy_dlist(l);
            destroy_tree(root);
            return NULL;
        }

        parent = curr; // setting the parent to 'curr' and making it iterate backwards until the actual parent is found
                       // (yes that does mean that setting parent to the very first file was useless but whatevs)
        while (parent != NULL && parent->hier != curr->hier - 1) // i'm putting '!= NULL' just this once to make this more readable
            parent = parent->prev;

        if (!parent) {
            fprintf(stderr, "ArchiMaker: *** Couldn't fetch parent of node '%s'. Abort.\n", curr->name);
            fflush(stderr);
            destroy_dlist(l);
            destroy_tree(root);
            destroy_tree(child);
            return NULL;
        }

        struct tree *temp = DFS(root, parent->name);
        if (!temp) { // negative search
            fprintf(stderr,
                    "ArchiMaker: *** Couldn't fetch parent tree of tree '%s'. Abort.\nIf you see this message, this is VERY BAD, this is NOT supposed to happen in ANY case.\n",
                    parent->name);
            fflush(stderr);
            destroy_dlist(l);
            destroy_tree(root);
            destroy_tree(child);
            return NULL;
        }

        if (add_child(temp, child) != 0) { // realloc error in add_child function
            // error message handled by the function
            destroy_dlist(l);
            destroy_tree(root);
            destroy_tree(child);
            return NULL;
        }

        curr = curr->next;

    }

    if (l)
        destroy_dlist(l);

    return root; // you have no idea how good it felt typing this

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

void print_tree(struct tree *root) {

    if (!root)
        return;

    char *type[3] = { "file", "directory", "root" };
    fprintf(stdout, "'%s' %s:\n", root->name, type[root->type]);
    fflush(stdout);

    if (root->childN == 0) {
        fprintf(stdout, "(leaf)\n\n");
        fflush(stdout);
        return;
    }
    else {
        fprintf(stdout, "[%s]", root->children[0]->name);
        for (size_t i = 1; i < root->childN; i++) {
            fprintf(stdout, ", [%s]", root->children[i]->name);
            fflush(stdout);
        }
    }
    fprintf(stdout, "\n\n");
    fflush(stdout);

    for (size_t i = 0; i < root->childN; i++)
        print_tree(root->children[i]);

}
