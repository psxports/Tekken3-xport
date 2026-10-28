#include "game_functions.h"
#include <stdio.h>
#include <stdlib.h>

static uint32 t3_dispatch_game_callback(uint32 target, uint32 count, uint32 first, uint32 second, uint32 third)
{
    if (count == 1) {
        switch (target) {
        case 0x800613CC: return t3_800613cc(first);
        case 0x80061B88: return t3_80061b88(first);
        case 0x80061EF0: return t3_80061ef0(first);
        case 0x8006251C: return t3_8006251c(first);
        case 0x800628C4: return t3_800628c4(first);
        case 0x80062928: return t3_80062928();
        case 0x80062930: return t3_80062930();
        case 0x80062938: return t3_80062938();
        case 0x80062940: return t3_80062940(first);
        case 0x80062994: return t3_80062994();
        case 0x8006299C: return t3_8006299c(first);
        }
    } else if (count == 2) {
        switch (target) {
        case 0x80048E58: return t3_80048e58(first, second);
        case 0x80049848: return t3_80049848(first, second);
        case 0x800D494C: return t3_bns_07_800d494c(first, second);
        }
    }
#ifdef _DEBUG
    else if (count == 3 && target == 0x8007A458) {
        /* GPU diagnostics pass two 32-bit scalar arguments */
        uint32 length = 0;
        while (r_u8(first + length))
            ++length;
        return (uint32)printf((const char *)psx_addr(first, length + 1), second, (sint32)third);
    }
    fprintf(stderr, "Unresolved T3 callback %08X/%u\n", target, count);
#endif
    abort();
}

static sint32 t3_dispatch_bios_callback(void *context, uint32 target)
{
    (void)context;
    switch (target) {
    case 0x80029784:
        t3_80029784();
        return 1;
    case 0x800299E0:
        t3_800299e0();
        return 1;
    }
    return 0;
}

void t3_bind_game_callbacks(void)
{
    psx_bind_guest_dispatch(t3_dispatch_game_callback);
    psx_bios_bind_guest_callback_service(t3_dispatch_bios_callback, NULL);
}

uint32 t3_8007ae58(uint32 mode)
{
    exit((sint32)mode);
}

uint32 t3_80079e08(void)
{
    return xport_bios_start_card();
}

uint32 t3_80079e18(uint32 event_class, uint32 specification, uint32 mode, uint32 callback)
{
    return (uint32)OpenEventGuest(event_class, specification, mode, callback);
}

void t3_80079e48(uint32 event)
{
    EnableEvent((sint32)event);
}

void t3_80079fcc(uint32 counter)
{
    psx_root_counter_stop(counter);
}

uint32 t3_80085c58(uint32 callback)
{
    return VSyncCallbackPSX(callback);
}
