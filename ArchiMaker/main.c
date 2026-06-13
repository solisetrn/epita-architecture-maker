#include "archimaker.h"

void display_helper() {

    fprintf(stdout, "Usage: ./ArchiMaker [options]\n");
    fflush(stdout);

    fprintf(stdout, "    OPTIONS:\n");
    fflush(stdout);
    // -h, --help
    fprintf(stdout, "        -h, --help            displays the helper.\n");
    fflush(stdout);
    // -f
    fprintf(stdout, "        -f <filename>         to change the architecture config file, 'Archifile' by default.\n");
    fflush(stdout);
    fprintf(stdout, "                              If no file named 'Archifile' is found and no target is specified, ArchiMaker will create a fresh empty one.\n");
    // -d, --dest
    fprintf(stdout, "        -d, --dest <filename> to change the destination directory, working directory by default.\n");
    fflush(stdout);

    fflush(stdout);
    fprintf(stdout, "\nArchimaker DOES create an empty 'README' file and empty '.gitignore' file for you. Don't forget to edit them later.\n");
    fflush(stdout);

    fprintf(stdout, "ArchiMaker was designed to handle error to some extent. However, it still assumes you pasted the given file architecture correctly.\n");
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
                fprintf(stderr, "ArchiMaker: *** Option '-f' requires an argument <filename>. Abort.\n");
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
            fprintf(stdout, "No target file specified and Archifile not found.\n");
            fflush(stdout);
            fprintf(stdout, "Creating Archifile...\n");
            fflush(stdout);
            file = fopen("Archifile", "w");
            if (!file) {
                fprintf(stderr, "ArchiMaker: *** Couldn't create 'Archifile' file. Abort.\n");
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
            fprintf(stderr, "ArchiMaker: *** No file named %s. Abort.\n", filename);
            fflush(stderr);
            free(filename);
            destroy_cmd(cmd);
            return 1;
        }
    }
    // MEMORY MANAGEMENT
    if (filename)
        free(filename);



    // finding the output file

    curr = cmd->head;
    char *dest_dir = NULL;
    while (curr) {

        if (curr->type == DEST) {

            if (!curr->next) {
                fprintf(stderr, "ArchiMaker: *** Option '%s' requires an argument <filename>. Abort.\n",
                        curr->data);
                fflush(stderr);
                destroy_cmd(cmd);
                if (dest_dir)
                    free(dest_dir);
                return 1;
            }

            if (dest_dir)
                free(dest_dir);
            dest_dir = strdup(curr->next->data);

        }

        curr = curr->next;

    }

    destroy_cmd(cmd);

    if (dest_dir) {

        DIR *directory = opendir(dest_dir);
        if (errno == ENOTDIR) {
            fprintf(stderr, "ArchiMaker: %s is not a valid directory. Abort.\n", dest_dir);
            fflush(stderr);
            free(dest_dir);
            return 1;
        }
        else if (!directory) {
            fprintf(stderr, "ArchiMaker: Couldn't open %s. Abort.\n", dest_dir);
            fflush(stderr);
            free(dest_dir);
            return 1;
        }

        else
            closedir(directory);

    }



    // file parsing and AST creation

    struct file_list *l = calloc(1, sizeof(struct file_list));
    if (!l) {
        fprintf(stderr, "ArchiMaker: *** Failed to allocate memory for object: struct file_list. Abort.\n");
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
    AST->type = ROOT; // helps to ignore the root

    make_items(AST, dest_dir); // literally builds EVERYTHING

    // MEMORY MANAGEMENT
    destroy_tree(AST);
    fclose(file);

    fprintf(stdout, "\nAll done! Thanks for using ArchiMaker!\n");
    fflush(stdout);

    return 0;

}
