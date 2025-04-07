#define NIRVANA_IMPL
#define NIRVANA_ENABLE_LOG
#include "nirvana.h"

int main(void) {
    nir_cmd_goes_samsara();
    nir_cmd_t cmd = {0};
    
    if (!nir_cmd_init(&cmd)) {
        return 1;
    }
    
    nir_cmd_append(&cmd, "gcc");
    nir_cmd_append(&cmd, "-o");
    nir_cmd_append(&cmd, "horizon");
    nir_cmd_append(&cmd, "eh.c");
    nir_cmd_append(&cmd, "-lm");
    nir_cmd_append(&cmd, "-lX11");
    
    nir_cmd_run(&cmd);
    nir_cmd_cleanup(&cmd);
    
    return 0;
}
