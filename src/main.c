#include <gc.h>
#include "args.h"
#include "net.h"
#include "log.h"
#include "config.h"
#include "process.h"
#include "regexp.h"

int main(int argc, char *argv[])
{
    GC_INIT();
    args_init(argc, argv);
    config_init(args->config);
    log_init();
    regexp_init();

    if (prspool_init(config_get_int(CONFIG_WORKER))) {
        prspool_run_master();
    } else {
        prspool_run_worker();
    }
    return 0;
}
