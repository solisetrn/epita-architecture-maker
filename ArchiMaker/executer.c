#include "archimaker.h"

char *add_path(char *prefix, char *filename) {

    if (!prefix)
        return strdup(filename);

    char *out = strdup(prefix);
    size_t i = strlen(out);
    if (out[i - 1] == '/') {
        out = realloc(out, i * sizeof(char));
        if (!out) {
            fprintf(stderr, "ArchiMaker: *** Failed to allocate memory in add_path(). Abort.\n");
            fflush(stderr);
            return NULL;
        }
        out[i - 1] = '\0';
        i--;
    }

    size_t j = 0;

    out = realloc(out, (i + 1) * sizeof(char));
    if (!out) {
            fprintf(stderr, "ArchiMaker: *** Failed to allocate memory in add_path(). Abort.\n");
            fflush(stderr);
            return NULL;
    }
    out[i] = '/';
    i++;

    while (filename[j] != '\0') {
        out = realloc(out, (i + 1) * sizeof(char));
        if (!out) {
            fprintf(stderr, "ArchiMaker: *** Failed to allocate memory in add_path(). Abort.\n");
            fflush(stderr);
            return NULL;
        }
        out[i] = filename[j];
        i++;
        j++;
    }

    out = realloc(out, (i + 1) * sizeof(char));
    if (!out) {
            fprintf(stderr, "ArchiMaker: *** Failed to allocate memory in add_path(). Abort.\n");
            fflush(stderr);
            return NULL;
    }
    out[i] = '\0';

    return out;

}

int executer(struct tree *root, char *name) {

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "ArchiMaker: Failed to fork in executer(). Abort.\n");
        fflush(stderr);
        free(name);
        return 1;
    }
    
    else if (pid == 0) { // child process

        if (root->type == DIRECTORY) {
            fprintf(stdout, "Creating %s directory...\n", name);
            fflush(stdout);
            char *argv[3] = { "mkdir", name, NULL };
            if (execvp(argv[0], argv) != 0) {
                fprintf(stderr, "ArchiMaker: Couldn't create directory: %s. Abort.\n", name);
                fflush(stderr);
                exit(1);
            }
        }
        else {
            fprintf(stdout, "Creating %s file...\n", name);
            fflush(stdout);
            char *argv[3] = { "touch", name, NULL };
            if (execvp(argv[0], argv) != 0) {
                fprintf(stderr, "ArchiMaker: Couldn't create file: %s. Abort.\n", name);
                fflush(stderr);
                exit(1);
            }
        }
        exit(0);

    }

    else { // parent process

        int status;
        waitpid(pid, &status, 0);
        free(name);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            return 1;

        return 0;

    }

}

int make_items(struct tree *root, char *prefix) {

    if (root->type == ROOT) {
        for (size_t i = 0; i < root->childN; i++) {
            if (make_items(root->children[i], prefix) != 0)
                return 1;
        }
        return 0;
    }

    if (executer(root, add_path(prefix, root->name)) != 0)
        return 1;

    for (size_t i = 0; i < root->childN; i++) {
        char *temp = add_path(prefix, root->name);
        if (!temp)
            return 1;
        if (make_items(root->children[i], temp) != 0)
            return 1;
        free(temp);
    }
    return 0;

}
