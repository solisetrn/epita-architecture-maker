#include "archimaker.h"

struct node *make_node(char *name, int hier) {

    struct node *out = calloc(1, sizeof(struct node));
    if (!out) {
        fprintf(stderr, "Failed to allocate memory for object: struct node. Abort.\n");
        return NULL;
    }

    out->hier = hier;
    out->name = name;
    return out;

}

void dlist_append(struct file_list *l, struct node *node) {

    if (!l->head && !l->tail) {
        l->head = node;
        l->tail = node;
        return;
    }

    node->prev = l->tail;
    l->tail->next = node;
    l->tail = node;

}

void aux_destroy_dlist(struct node *node) {

    if (!node)
        return;

    aux_destroy_dlist(node->next);
    free(node->name);
    free(node);

}

void destroy_dlist(struct file_list *l) {

    aux_destroy_dlist(l->head);
    free(l);

}

void print_dlist(struct file_list *l) {

    if (!l->head && !l->tail) {
        fprintf(stdout, "list is empty.\n");
        fflush(stdout);
        return;
    }

    struct node *curr = l->head;
    fprintf(stdout, "(%s, %d)", curr->name, curr->hier);
    fflush(stdout);
    curr = curr->next;
    while (curr) {
        fprintf(stdout, " <-> (%s, %d)", curr->name, curr->hier);
        fflush(stdout);
        curr = curr->next;
    }
    putchar('\n');


}
