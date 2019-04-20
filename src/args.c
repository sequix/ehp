#include "args.h"
#include "config.h"
#include <stdlib.h>

static void *args_table[args_num];
struct arg_lit *args_help;
struct arg_lit *args_version;
struct arg_lit *args_verbose;
struct arg_lit *args_daemon;
struct arg_file *args_config_file;
struct arg_file *args_log_file;
struct arg_end *args_err;

static void args_init(void)
{
    args_table[0] = args_help        = arg_litn(NULL, "help", 0, 1, "show help message");
    args_table[1] = args_version     = arg_litn(NULL, "version", 0, 1, "show version info");
    args_table[2] = args_verbose     = arg_litn("v", "verbose", 0, 1, "log verbosely");
    args_table[3] = args_daemon      = arg_litn("d", "daemon", 0, 1, "display version info and exit");
    args_table[4] = args_config_file = arg_file0("c", "config", NULL, "path to config file");
    args_table[5] = args_log_file    = arg_file0("l", "log", NULL, "path to log file");
    args_table[6] = args_err         = arg_end(10);
}

void args_print_help(void)
{
    printf("Usage: %s", NAME);
    arg_print_syntax(stdout, args_table, "\n");
    printf("\nA HTTP server for graduation design by zc.\n\n");
    printf("Options:\n");
    arg_print_glossary(stdout, args_table, "  %-25s %s\n");
}

void args_print_version(void)
{
    printf("%s: v%d.%d.%d\n", NAME, VERSION_MAJOR, VERSION_MINOR,
        VERSION_PATCH);
}

int args_handle(int argc, char *argv[])
{
    args_init();
    int nerrors = arg_parse(argc, argv, args_table);

    if (args_help->count > 0) {
        args_print_help();
        exit(0);
    }

    if (args_version->count > 0) {
        args_print_version();
        exit(0);
    }

    if (nerrors > 0) {
        arg_print_errors(stdout, args_err, NAME);
        printf("Try '%s --help' for more information.\n", NAME);
        exit(1);
    }

    // TODO: free table (will freeing make args_config_file broken?)
}
