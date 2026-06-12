#include "archimaker.h"

void cmd_append(struct cmd *cmd, struct arg *arg) {

    if (!cmd->head && !cmd->tail) {
        cmd->head = arg;
        cmd->tail = arg;
        return;
    }

    cmd->tail->next = arg;
    cmd->tail = arg;

}

void destroy_args(struct arg *arg) {

    if (!arg)
        return;
    destroy_args(arg->next);
    if (arg->data)
        free(arg->data);
    free(arg);

}

void destroy_cmd(struct cmd *cmd) {

    destroy_args(cmd->head);
    free(cmd);

}

struct cmd *parse_command(int argc, char *argv[]) {

    struct cmd *cmd = calloc(1, sizeof(struct cmd));
    if (!cmd) {
        fprintf(stderr, "Failed to allocate memory for object: struct cmd. Abort.\n");
        return NULL;
    }

    int i = 1;
    while (i < argc) {

        struct arg *node = calloc(1, sizeof(struct arg));
        if (!node) {
            fprintf(stderr, "Failed to allocate memory for object: struct arg. Abort.\n");
            destroy_cmd(cmd);
            return NULL;
        }

        if (strcmp(argv[i], "-f") == 0)
            node->type = OPEN;

        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
            node->type = HELP;

        else {
            if (argv[i][0] == '-') {
                fprintf(stderr, "*** ArchiMaker: Error. Invalid option '%s'.\n", argv[i]);
                destroy_cmd(cmd);
                display_helper();
                return NULL;
            }
            else
                node->type = STR;
        }

        cmd_append(cmd, node);
        node->data = strdup(argv[i]);

        i++;
    }

    return cmd;

}

char *get_name(char *line) {

    size_t i = 0;
    while (line[i] != '.' && !('a' <= line[i] && line[i] <= 'z') && !('A' <= line[i] && line[i] <= 'Z') && line[i] != '\n' && line[i] != '\0') // hi don't mind me
        i++;

    char *out = NULL;
    size_t j = 0;
    while (line[i] != '\n') {
        out = realloc(out, (j + 1) * sizeof(char));
        if (!out) {
            fprintf(stderr, "Failed to allocate memory in get_name(line). Abort.\n");
            return NULL;
        }
        out[j] = line[i];
        j++;
        i++;
    }

    out = realloc(out, (j + 1) * sizeof(char));
    if (!out) {
        fprintf(stderr, "Failed to allocate memory in get_name(line). Abort.\n");
        return NULL;
    }
    out[j] = '\0';
    return out;

}

void print_cmd(struct cmd *cmd) {

    if (!cmd->head) {
        fprintf(stdout, "list is empty.\n");
        fflush(stdout);
        return;
    }

    struct arg *curr = cmd->head;
    fprintf(stdout, "%s", curr->data);
    fflush(stdout);
    curr = curr->next;

    while (curr) {
        fprintf(stdout, " -> %s", curr->data);
        curr = curr->next;
    }

    putchar('\n');

}
