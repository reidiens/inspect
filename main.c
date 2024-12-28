#include "pr_inf.h"
#include "filetypes.h"

#include <stdio.h>
#include <sys/stat.h>

void usage();

int main(int argc, char *argv[]) {
    /* check for args */
    if (argc < 2) {
        usage();
        return -1;
    }

    /* get file information */
    file_inf finf = ft_get_file_info(argv[1]);

    print_info(&finf);

    return 0;
}

void usage() {
    puts("usage: f <filename>");
    puts("View detailed information about <filename>\n");
}