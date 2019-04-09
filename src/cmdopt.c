#include "cmdopt.h"
#include "config.h"
#include <stdlib.h>

static void *ehp_args_table[ehp_args_num];
struct arg_lit *ehp_args_help;
struct arg_lit *ehp_args_version;
struct arg_lit *ehp_args_verbose;
struct arg_lit *ehp_args_daemon;
struct arg_file *ehp_args_config_file;
struct arg_end *ehp_args_err;

static void ehp_args_init(void)
{
    ehp_args_table[0] = ehp_args_help        = arg_litn(NULL, "help", 0, 1, "show help message");
    ehp_args_table[1] = ehp_args_version     = arg_litn(NULL, "version", 0, 1, "show version info");
    ehp_args_table[2] = ehp_args_verbose     = arg_litn("v", "verbose", 0, 1, "log verbosely");
    ehp_args_table[3] = ehp_args_daemon      = arg_litn("d", "daemon", 0, 1, "display version info and exit");
    ehp_args_table[4] = ehp_args_config_file = arg_file0("c", "config", NULL, "path to config file");
    ehp_args_table[5] = ehp_args_err         = arg_end(10);
}

void ehp_args_print_help(void)
{
    printf("Usage: %s", EHP_NAME);
    arg_print_syntax(stdout, ehp_args_table, "\n");
    printf("\nA HTTP server for graduation design by zc.\n\n");
    printf("Options:\n");
    arg_print_glossary(stdout, ehp_args_table, "  %-25s %s\n");
}

void ehp_args_print_version(void)
{
    printf("%s: v%d.%d.%d\n", EHP_NAME, EHP_VERSION_MAJOR, EHP_VERSION_MINOR,
        EHP_VERSION_PATCH);
}

int ehp_args_handle(int argc, char *argv[])
{
    ehp_args_init();
    int nerrors = arg_parse(argc, argv, ehp_args_table);

    if (ehp_args_help->count > 0) {
        ehp_args_print_help();
        exit(0);
    }

    if (ehp_args_version->count > 0) {
        ehp_args_print_version();
        exit(0);
    }

    if (nerrors > 0) {
        arg_print_errors(stdout, ehp_args_err, EHP_NAME);
        printf("Try '%s --help' for more information.\n", EHP_NAME);
        exit(1);
    }

    // TODO: free table (will freeing make ehp_args_config_file broken?)
}
