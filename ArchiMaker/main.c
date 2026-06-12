#include "archimaker.h"

void display_helper() {

    fprintf(stdout, "Usage: ./ArchiMaker [options]\n");
    fflush(stdout);
    fprintf(stdout, "\tOPTIONS:\n");
    fflush(stdout);
    fprintf(stdout, "\t\t-h, --help    displays the helper.\n");
    fflush(stdout);
    fprintf(stdout, "\t\t-f <filename> to change the make config file, \"Archifile\" by default\n");
    fflush(stdout);

}

int main(int argc, char *argv[]) {

    struct cmd *cmd = parse_command(argc, argv);
    if (!cmd)
        // error message handled by the function
        return 1;

    // -h, --help displays the helper and stops the process
    struct arg *curr = cmd->head;
    while (curr) {
        if (curr->type == HELP) {
            display_helper();
            destroy_cmd(cmd);
            return 0;
        }    
        curr = curr->next;   
    }

    // opening the file
    char *filename = NULL;
    FILE *file = NULL;
    curr = cmd->head;
    while (curr) {
        if (curr->type == OPEN) {
            
            if (curr->next) {
                if (filename)
                    free(filename);
                filename = strdup(curr->next->data);
            }
            else {
                fprintf(stderr, "ArchiMaker: option '-f' requires and argument <filename>. Abort.\n");
                if (filename)
                    free(filename);
                destroy_cmd(cmd);
                return 1;
            }

        }
        curr = curr->next;
    }

    // if no -f <filename> is given
    if (!filename) {

        file = fopen("Archifile", "r");
        if (!file) {
            fprintf(stderr, "ArchiMaker: No target file specified and no Archifile found. Abort.\n");
            destroy_cmd(cmd);
            return 1;
        }

    }
    // default behavior
    else {
        file = fopen(filename, "r");
        if (!file) {
            fprintf(stderr, "ArchiMaker: No file named %s. Abort.\n", filename);
            free(filename);
            destroy_cmd(cmd);
            return 1;
        }
    }

    // MEMLEAK MANAGEMENT
    if (cmd)
        destroy_cmd(cmd);
    if (filename)
        free(filename);
    fclose(file);

    return 0;

}
