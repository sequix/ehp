#include "log.h"
#include "args.h"
#include "mem.h"
#include "str.h"
#include <string.h>

// the global arguments
args_t args = NULL;

static const char* args_help_message = \
"Usage: ehp [-d] [--help] [--version] [-c <file>]\n" \
"\n" \
"A HTTP server for graduation design by zc.\n" \
"\n" \
"Options:\n" \
"  --help                    show help message\n" \
"  --version                 show version info\n" \
"  -d, --daemon              display version info and exit\n" \
"  -c, --config <file>       path to config file\n" \
"      --debug               output debug log\n";

static void args_print_unknown(str_t opt)
{
    str_printf("ehp: unknown option '%r'\n", opt);
    str_printf("ehp: see --help for more info\n");
}

static void args_print_unknown_short(char opt)
{
    str_printf("ehp: unknown option '%c'\n", opt);
    str_printf("ehp: see --help for more info\n");
}

static void args_print_version()
{
    str_printf("ehp v%d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

void args_init(int argc, char *argv[])
{
    args = talloc(args_st);
    if (!args) {
        log_odebug("args: talloc");
        exit(1);
    }
    args->is_daemon = false;
    args->is_debug = false;
    args->config = str_from(DEFAULT_CONFIG_FILE);

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                str_t word = str_from(&argv[i][2]);
                if (str_eq(word, str_from("help"))) {
                    str_printf(args_help_message);
                    exit(0);
                }
                if (str_eq(word, str_from("version"))) {
                    args_print_version();
                    exit(0);
                }
                if (str_eq(word, str_from("daemon"))) {
                    args->is_daemon = true;
                    continue;
                }
                if (str_eq(word, str_from("config"))) {
                    if (i + 1 >= argc) {
                       str_printf("ehp: expected config file path\n");
                       exit(1);
                    }
                    args->config = str_from(argv[i+1]);
                    ++i;
                    continue;
                }
                if (str_eq(word, str_from("debug"))) {
                    args->is_debug = true;
                    continue;
                }
                args_print_unknown(word);
                exit(1);
            } else {
                int len = strlen(argv[i]);
                for (int j = 1; j < len; ++j) {
                    char c = argv[i][j];
                    if (c == 'd') {
                        args->is_daemon = true;
                        continue;
                    }
                    if (c == 'c') {
                        if (i + 1 >= argc) {
                            str_printf("ehp: expected config file path\n");
                            exit(1);
                        }
                        args->config = str_from(argv[i+1]);
                        ++i;
                        break;
                    }
                    args_print_unknown_short(c);
                    exit(1);
                }
            }
        } else {
            args_print_unknown(argv[i]);
            exit(1);
        }
    }
    log_odebug("args: init success");
}
