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

    return 0;

}
