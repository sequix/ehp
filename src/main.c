#include "args.h"
#include "log.h"
#include "config.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    ehp_args_handle(argc, argv);
    ehp_log_init(NULL);
    ehp_config_read_from(*ehp_args_config_file->filename);
}
