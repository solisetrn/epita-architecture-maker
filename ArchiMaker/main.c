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
    fprintf(stdout, "\t                      If no file named 'Archifile' is found and no target is specified, ArchiMaker will create an fresh empty one.\n");
    fflush(stdout);
    fprintf(stdout, "\tIt is suggested that you move the 'ArchiMaker' binary to the root of your EPITA Practical repository.\n");
    fflush(stdout);
    fprintf(stdout, "\tArchimaker DOES create an empty 'README' file and empty '.gitignore' file for you. Don't forget to edit them later.\n");
    fflush(stdout);

    fprintf(stdout, "\nArchiMaker was designed to handle error to some extent. However, it still assumes you pasted the given file architecture correctly.\n");
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
                fflush(stderr);
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
            fflush(stdout);
            fprintf(stdout, "Creating Archifile...\n");
            fflush(stdout);
            file = fopen("Archifile", "w");
            if (!file) {
                fprintf(stderr, "Couldn't create 'Archifile' file. Abort.\n");
                fflush(stderr);
                return 1;
            }
            fprintf(stdout, "'Archifile' created!\n");
            fflush(stdout);
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
            fflush(stderr);
            free(filename);
            destroy_cmd(cmd);
            return 1;
        }
    }
    // MEMORY MANAGEMENT
    if (filename)
        free(filename);
    if (cmd)
        destroy_cmd(cmd);


    // file parsing and AST creation

    struct file_list *l = calloc(1, sizeof(struct file_list));
    if (!l) {
        fprintf(stderr, "Failed to allocate memory for object: struct file_list. Abort.\n");
        fflush(stderr);
        return 1;
    }

    size_t n = 0;
    char *line = NULL;
    while (getline(&line, &n, file) != -1) {
        if (strcmp(line, "\n") != 0) {

            char *name = get_name(line);
            if (!name) {
                // error message handled by the function
                destroy_dlist(l);
                free(line);
                return 1;
            }

            struct node *node = make_node(name, get_hier(line));
            if (!node) {
                // error message handled by the function
                free(name);
                destroy_dlist(l);
                free(line);
                return 1;
            }

            dlist_append(l, node);

        }
    }
    free(line);
    
    // l freed by the function below
    struct tree *AST = convert_to_tree(l);
    if (!AST)
        return 1;

    // print_tree(AST);

    // MEMORY MANAGEMENT
    destroy_tree(AST);
    fclose(file);

    return 0;

}
