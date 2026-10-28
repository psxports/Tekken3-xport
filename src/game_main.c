#include "xport.h"
#include "game_functions.h"

/* Unresolved until the original PsyQ graph type storage is audited */
const uint32 xport_gpu_graph_type_address = 0u;

/* Unresolved CD bindings until original storage is audited */
const uint32 xport_cd_sync_callback_address = 0u;
const uint32 xport_cd_ready_callback_address = 0u;
const uint32 xport_cd_status_address = 0u;
const uint32 xport_cd_setloc_table_address = 0u;

void xport_main(void)
{
    static const PSX_EXE executable = {
        "DATA/TEKKEN3/SLUS_004.02",
        0x80010000,
        0x8009B9A8,
        0x800B0548,
        0x800A8A68,
        0x800B0548,
        0x00147AB0
    };

    if (!xport_psx_exe_load(&executable)) {
#ifdef _DEBUG
        fprintf(stderr, "Unable to load DATA/TEKKEN3/SLUS_004.02\n");
#endif
        return;
    }
    t3_bind_game_callbacks();
    t3_80028ba0();
}
