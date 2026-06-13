#include "archimaker.h"

void display_helper() {

    fprintf(stdout, "Usage: ./ArchiMaker [options]\n");
    fflush(stdout);
    fprintf(stdout, "\tOPTIONS:\n");
    fflush(stdout);
    fprintf(stdout, "\t\t-h, --help    displays the helper.\n");
    fflush(stdout);
    fprintf(stdout, "\t\t-f <filename> to change the make config file, \"Archifile\" by default.\n");
    fflush(stdout);
    fprintf(stdout, "\t                      If no file named 'Archifile' is found and no target is specified, ArchiMaker will create an empty one.\n");
    fflush(stdout);
    fprintf(stdout, "\tIt is suggested that you move the 'ArchiMaker' binary to the root of your EPITA Practical repository.\n");

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
            fprintf(stdout, "Archimaker: No target file specified and Archifile not found.\n");
            fprintf(stdout, "Creating Archifile...\n");
            file = fopen("Archifile", "w");
            if (!file) {
                fprintf(stderr, "Couldn't create 'Archifile' file. Abort.\n");
                return 1;
            }
            fprintf(stdout, "'Archifile' created!\n");
            fclose(file);
            destroy_cmd(cmd);
            return 2; // returns 2 in case of no Archifile but creates one successfully
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



    // file parsing and AST creation

    size_t n = 0;
    char *line = NULL;
    while (getline(&line, &n, file) != -1) {
        char *s = get_name(line);
        printf("%s, hierarchy rank: %zu\n", s, get_hier(line));
        free(s);
    }
        

    free(line);

    // MEMLEAK MANAGEMENT
    if (cmd)
        destroy_cmd(cmd);
    if (filename)
        free(filename);
    fclose(file);

    return 0;

}
