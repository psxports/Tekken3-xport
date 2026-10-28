#include "game_functions.h"
#include "t3_data.h"
#include "psx_spu.h"
#include "psx_stream.h"
#include "xport_trace.h"
#include <stdlib.h>
#include <string.h>

uint32 t3_800591dc(uint32 first, uint32 state)
{
    uint32 random_value;
    uint32 callback;
    uint32 result;
    FUNCTION_MARKER(0x800591DC, "SLUS_004.02");
    if (r_s16(state + 0x8A) >= 0)
        return 0;
    random_value = r_u32(0x800ADCA0);
    w_u32(0x800ADCA0, random_value * 5 + 3);
    if ((sint32)(random_value & 0xFFF) >= r_s16(state + 0x272))
        return 0;
    callback = r_u32(state + (first ? 0x78 : 0x74));
    if (!callback)
        return 0;
    result = psx_dispatch_guest_call(callback, 1, state, 0, 0);
    if (result == 0xFFFFFFFE)
        return 1;
    if (result == 0xFFFFFFFF)
        return 0;
    if (result == 0xFFFFFFFD) {
        if (r_s16(state + 0x72) <= 0)
            return 0;
        return (sint32)t3_80058a84(state) >= 0;
    }
    if (result == 0x2000 || result == 0x8000) {
        w_u16(state + 0x38, 16);
        w_u16(state + 0x3A, 0);
        w_u16(state + 0x40, 56);
    }
    w_u16(state + 4, result);
    return 1;
}

uint32 t3_8004870c(sint32 stage)
{
    uint32 keys[40];
    uint32 count = 0;
    uint32 scene;
    uint32 lighting;
    uint32 mode;
    uint32 row;
    uint32 column;
    uint32 bank;
    sint32 stage_number = (sint16)(uint16)stage;
    FUNCTION_MARKER(0x8004870C, "SLUS_004.02");
    t3_800484f4();
    scene = t3_80048230();
    lighting = r_u16(scene + 0x324);
    w_u32(0x8009E7F8, scene);
    mode = r_u32(0x800AFA88);
    w_u16(0x8009E820, lighting);
    if (mode == 8) {
        if (r_u32(0x800AFAAC) == 4)
            w_u16(0x8009E820, 4);
    }
    else if (r_u8(0x800AFAA0) && !lighting)
        w_u16(0x8009E820, 2);
    for (row = 0; row < 10; ++row) {
        for (column = 0; column < 10; ++column) {
            uint32 key = r_u32(r_u32(0x8009E7F8) + 4 + row * 80 + column * 8) & 0x39FFFFFF;
            uint32 index;
            for (index = 0; index < count; ++index) {
                if (keys[index] == key)
                    break;
            }
            w_u8(0x8009E790 + row * 10 + column, index);
            if (index == count)
                keys[count++] = key;
        }
    }
    w_u16(0x8009E7F4, count);
    if (count >= 41)
        return 0xFFFFFFFF;
    if (r_s16(0x8009E820))
        return 0;
    for (bank = 0; bank < 2; ++bank) {
        uint32 base = r_u32(0x800ADD14);
        uint32 packet = base ? base + 1508 + bank * 5200 : 0;
        uint32 index;
        for (index = 0; index < 100; ++index) {
            uint32 raw_texture = r_s16(r_u32(0x8009E7F8) + 0x326) == 0;
            setShadeTex((POLY_GT4 *)psx_addr(packet, 52), raw_texture);
            packet += 52;
        }
    }
    w_u32(0x800AE21C, stage_number > 0 && (stage_number < 3 || stage_number == 7) ? 6500 : 10000);
    return 0;
}

uint32 t3_80048240(uint32 actor, uint32 opponent)
{
    uint32 disabled;
    uint32 callback;
    FUNCTION_MARKER(0x80048240, "SLUS_004.02");
    disabled = r_u32(0x800ADCA4);
    if (disabled)
        return disabled;
    t3_80045ca8(0x800A85C0, 0x800ADF70);
    callback = r_u32(0x8009E728);
    psx_dispatch_guest_call(callback, 2, actor, opponent, 0);
    return t3_80048470();
}

void t3_8006c2a0(uint32 event)
{
    sint32 bytes;
    sint32 words;
    uint32 remaining;
    uint32 destination;
    uint32 callback;
    uint32 node;
    uint32 offset;
    uint32 count;
    FUNCTION_MARKER(0x8006C2A0, "SLUS_004.02");
    event &= 0xFF;
    if (event != 1) {
        if (event != 5)
            return;
        w_u16(0x800A05DA, 1);
        w_u16(0x800A05D8, 9);
        w_u32(0x800A866C, 0);
        return;
    }
    if (r_u8(0x800A069E)) {
        w_u8(0x800A069F, 0);
        w_u16(0x800A05D8, 1);
        w_u32(0x800A866C, 0);
        w_u8(0x800A069E, 0);
        return;
    }
    bytes = r_s32(0x800A06A4);
    if (bytes > 2048)
        bytes = 2048;
    words = (sint32)((uint32)bytes + (bytes < 0 ? 3u : 0u)) >> 2;
    destination = r_u32(0x800A06A8);
    node = r_u32(0x800A0698);
    offset = r_u32(node + 16) - r_u32(0x800A06A4);
    count = (uint32)words * 4;
    if (!t3_bns_read(r_u32(node + 12), offset, psx_addr(destination, count), count)) {
        t3_8006c2a0(5);
        return;
    }
    remaining = r_u32(0x800A06A4);
    destination = r_u32(0x800A06A8) + 2048;
    remaining -= (uint32)bytes;
    w_u32(0x800A06A4, remaining);
    w_u32(0x800A06A8, destination);
    if ((sint32)remaining > 0)
        return;
    callback = r_u32(0x800A06AC);
    if (callback)
        psx_dispatch_guest_call(callback, 0, 0, 0, 0);
    node = r_u32(r_u32(0x800A0698) + 24);
    if (node) {
        w_u32(0x800A0698, node);
        w_u16(0x800A06A0, r_u16(node));
        w_u32(0x800A06A4, r_u32(node + 16));
        w_u32(0x800A06A8, r_u32(node + 8));
        w_u32(0x800A06AC, r_u32(node + 20));
        return;
    }
    w_u8(0x800A069F, 0);
    w_u16(0x800A05D8, 1);
    w_u32(0x800A866C, 0);
}

static void t3_allocate_work_block(uint32 slot, uint32 size)
{
    if (size) {
        uint32 base = r_u32(0x8009F2E0);
        w_u32(slot, base);
        w_u32(0x8009F2E0, base + ((size + 3) & 0xFFFFFFFC));
    }
}

uint32 t3_80055884(uint32 mode)
{
    FUNCTION_MARKER(0x80055884, "SLUS_004.02");
    w_u32(0x800B0408, 0);
    w_u32(0x800ADE68, 0);
    w_u32(0x800ADD10, 0);
    w_u32(0x800ADC80, 0);
    w_u32(0x800ADD14, 0);
    w_u32(0x800ADD58, 0);
    w_u32(0x800ADD0C, 0);
    w_u32(0x800ADC88, 0);
    w_u32(0x800A8D68, 0);
    w_u32(0x800ADE80, 0);
    w_u32(0x800ADD74, 0);
    w_u32(0x8009F2E4, 0);
    w_u32(0x8009F2E4, 0x800B8D58);
    w_u32(0x8009F2E0, 0x800B8D58);
    switch (mode) {
    case 1:
    case 2:
    case 3:
        t3_allocate_work_block(0x800B0408, t3_80079964(mode));
        t3_allocate_work_block(0x800ADE68, t3_80031d04());
        t3_allocate_work_block(0x800ADD10, t3_8004a404());
        t3_allocate_work_block(0x800ADC80, t3_8006e514());
        t3_allocate_work_block(0x800ADD14, t3_800484ec());
        if (mode == 1) {
            t3_allocate_work_block(0x800ADD58, t3_8006d00c());
            t3_allocate_work_block(0x800ADD0C, t3_8006c7f8());
        }
        else if (mode == 2) {
            // Logical 03 size getters B57D0 and B57D8
            t3_allocate_work_block(0x800ADD58, 0x1A40);
            t3_allocate_work_block(0x800ADD0C, 0x404);
        }
        else {
            // Logical 02 size getters B4574 and B457C
            t3_allocate_work_block(0x800ADD58, 0x930);
            t3_allocate_work_block(0x800ADD0C, 0x14A);
        }
        t3_allocate_work_block(0x800ADC88, t3_80048074());
        if (mode == 2) {
            // Logical 03 size getter B07A0
            t3_allocate_work_block(0x800ADD74, 0x1464);
        }
        t3_allocate_work_block(0x800ADE80, t3_800697b8(0));
        if (mode == 3) {
            // Logical 02 size getter B101C
            t3_allocate_work_block(0x800ADD74, 0xA00);
        }
        t3_allocate_work_block(0x800A8D68, t3_80079af4(mode));
        t3_80079b30(mode, r_u32(0x800A8D68));
        break;
    case 4:
        w_u32(0x8009F2E0, 0x801BFED4);
        w_u32(0x8009F2E4, 0x801BFED4);
        t3_allocate_work_block(0x800B0408, t3_80079964(mode));
        // Logical 07 size getter D4218
        t3_allocate_work_block(0x800ADD14, 0x32E4);
        t3_allocate_work_block(0x800ADD58, t3_8006d00c());
        w_u32(0x800ADD0C, 0x80104094);
        w_u32(0x800ADC88, 0x800CADC4);
        w_u32(0x800ADE80, 0x800DD818);
        break;
    case 5:
        w_u32(0x8009F2E0, 0x800D3144);
        w_u32(0x8009F2E4, 0x800D3144);
        t3_allocate_work_block(0x800B0408, t3_80079964(mode));
        t3_allocate_work_block(0x800ADD14, t3_800484ec());
        t3_allocate_work_block(0x800ADD58, t3_8006d00c());
        t3_allocate_work_block(0x800ADD0C, t3_8006c7f8());
        t3_allocate_work_block(0x800ADC88, t3_80048074());
        break;
    default:
        t3_allocate_work_block(0x800B0408, t3_80079964(mode));
        break;
    }
    return t3_800799a8(mode, r_u32(0x800B0408));
}

uint32 t3_800b07e4(uint32 seed)
{
    uint32 index;
    uint32 checksum = seed;
    FUNCTION_MARKER(0x800B07E4, "SLUS_004.02");
    for (index = 0; index < 256; ++index)
        checksum += r_u32(0x1F800000 + index * 4);
    srandPSX(checksum);
    w_u32(0x800A912C, checksum);
    return 0x800B0000;
}

uint32 t3_800b0548(void)
{
    uint32 index;
    uint32 active;
    uint32 work;
    FUNCTION_MARKER(0x800B0548, "SLUS_004.02");
    t3_80055884(0);
    ResetCallback();
    t3_8006ab64();
    t3_800b0788();
    t3_800b0840(0);
    t3_80074da4();
    t3_8004b61c(0);
    t3_800b0954();
    t3_800b0b9c();
    t3_80028f5c();
    t3_800294ac();
    t3_8006c568();
    t3_80029924(0);
    w_u32(0x800A8C54, 0x800A8590);
    for (index = 0; index < 2; ++index) {
        w_u32(0x800A8594 + index * 20, 0x800A6588 + index * 4096);
        w_u32(0x800A8590 + index * 20, 10);
    }
    w_u16(0x800A923A, 0);
    w_u16(0x800AAAC6, 1);
    w_u16(0x800AC352, 1);
    w_u8(0x800A9246, 0);
    w_u8(0x800AAAD2, 1);
    w_u8(0x800AC35E, 2);
    t3_800697d4(-1);
    SetGeomScreen(500);
    w_u16(0x800AE204, 0);
    w_u16(0x800AE224, 0);
    w_u32(0x800AFA4C, 0);
    w_u16(0x800AE1FA, 0);
    w_u16(0x800AE1F8, 0);
    w_u16(0x800ADED4, 1);
    SetRCnt(0xF2000001, 0xFFFF, 0x2000);
    StartRCnt(0xF2000001);
    active = GsGetActiveBuff();
    work = r_u32(0x800AE040 + active * 28);
    w_u32(0x800ADEFC, active);
    GsSetWorkBase((uint32 *)psx_addr(work, 4));
    t3_8004c684(r_u32(0x800ADEFC));
    GsClearOt(0, 0, psx_addr(r_u32(0x800A8C54), 20));
    GsSortClear(0, 0, 0, psx_addr(r_u32(0x800A8C54), 20));
    t3_800b07e4(0x800B0000);
    t3_80069708();
    t3_80067258();
    t3_80067be4();
    w_u32(0x80095468, 0);
    w_u32(0x80095494, 0);
    return t3_80029894();
}

uint32 t3_8004559c(uint32 prior_third_actor)
{
    uint32 candidates[3];
    uint32 count = 0;
    uint32 actor_count;
    uint32 index;
    uint32 first_index;
    uint32 second_index;
    uint32 third_index;
    uint32 first;
    uint32 second;
    uint32 third = prior_third_actor;
    uint32 opponent_index;
    uint32 mode;
    FUNCTION_MARKER(0x8004559C, "SLUS_004.02");
    actor_count = r_u32(0x800AFA88) == 8 ? 3 : 2;
    for (index = 0; index < actor_count; ++index) {
        uint32 actor = 0x800A9228 + index * 6284;
        if (r_u32(actor + 0x1A4) && r_s16(actor + 0x58) >= r_s16(actor + 0x198) && r_s16(actor + 0x19A) == 33)
            candidates[count++] = actor;
    }
    if (count) {
        uint32 selected = candidates[r_u32(0x800AFA4C) % count];
        first_index = r_u8(selected + 0x21);
        second_index = r_u8(selected + 0x1E);
        third_index = 3 - (first_index + second_index);
    }
    else {
        first_index = 0;
        second_index = 1;
        third_index = 2;
    }
    first = 0x800A9228 + first_index * 6284;
    second = 0x800A9228 + second_index * 6284;
    if (r_u32(0x800AFA88) == 8)
        third = 0x800A9228 + third_index * 6284;
    if (count) {
        opponent_index = r_u8(first + 0x21);
        t3_8002d75c(first, 0x800A9228 + opponent_index * 6284);
        opponent_index = r_u8(first + 0x21);
        t3_8002f3ac(first, 0x800A9228 + opponent_index * 6284);
        opponent_index = r_u8(second + 0x21);
    }
    else {
        opponent_index = r_u8(first + 0x1F);
        t3_8002f3ac(first, 0x800A9228 + opponent_index * 6284);
        opponent_index = r_u8(second + 0x1F);
    }
    t3_8002f3ac(second, 0x800A9228 + opponent_index * 6284);
    mode = r_u32(0x800AFA88);
    if (mode == 8) {
        opponent_index = r_u8(third + 0x1F);
        return t3_8002f3ac(third, 0x800A9228 + opponent_index * 6284);
    }
    return mode;
}

uint32 t3_8002e708(uint32 sequence, uint32 frame, uint32 actor, uint32 opponent, uint32 initial_return_sequence)
{
    uint32 return_sequence = initial_return_sequence;
    uint32 command;
    uint32 buttons;
    uint32 held;
    FUNCTION_MARKER(0x8002E708, "SLUS_004.02");
    command = r_u16(sequence);
    buttons = r_u16(actor + 0x402);
    held = r_u16(actor + 0x404);
    while (command != 0xC000) {
        uint32 condition;
        uint32 matches;
        if (command == 0xC00C) {
            uint32 target = r_u16(sequence + 6);
            return_sequence = sequence;
            sequence = 0x80017C78 + target * 12;
        }
        else if (command == 0xC00D) {
            sequence = return_sequence;
            goto advance;
        }
        if ((sint32)frame < r_u8(sequence + 9))
            goto advance;
        if ((sint32)r_u8(sequence + 10) < (sint32)frame)
            goto advance;
        if (!(t3_8002cc28(actor, sequence, buttons, held) & 0xFFFF))
            goto advance;
        condition = r_u8(sequence + 2);
        matches = 1;
        if (condition >= 93)
            matches = (sint32)(condition - 93) != r_s16(actor + 24);
        else if (condition >= 70)
            matches = (sint32)(condition - 70) == r_s16(actor + 24);
        else if (condition >= 47)
            matches = (sint32)(condition - 47) == r_s16(opponent + 22);
        else if (condition >= 24)
            matches = (sint32)(condition - 24) != r_s16(actor + 22);
        else if (condition)
            matches = (sint32)(condition - 1) == r_s16(actor + 22);
        if (matches && r_u8(sequence + 3) >= 69 && t3_8002e8d0(sequence, actor, opponent))
            return sequence;
advance:
        sequence += 12;
        command = r_u16(sequence);
    }
    return sequence;
}

uint32 t3_8002de24(uint32 sequence, uint32 frame, uint32 actor, uint32 opponent, uint32 initial_return_sequence)
{
    uint32 return_sequence = initial_return_sequence;
    uint32 controller;
    uint32 buttons;
    uint32 held;
    uint32 command;
    uint32 action;
    FUNCTION_MARKER(0x8002DE24, "SLUS_004.02");
    controller = r_u8(actor + 0x1E);
    buttons = r_u16(actor + 0x402);
    held = r_u16(actor + 0x404);
    t3_80031144(controller);
rescan:
    command = r_u16(sequence);
    while (command != 0xC000) {
        uint32 condition;
        uint32 matches;
        uint32 test;
        if (command == 0xC00C) {
            uint32 target = r_u16(sequence + 6);
            return_sequence = sequence;
            sequence = 0x80017C78 + target * 12;
        }
        else if (command == 0xC00D) {
            sequence = return_sequence;
            goto advance;
        }
        if (!(t3_8002cc28(actor, sequence, buttons, held) & 0xFFFF))
            goto advance;
        if ((sint32)frame < r_u8(sequence + 9))
            goto advance;
        if ((sint32)r_u8(sequence + 10) < (sint32)frame)
            goto advance;
        condition = r_u8(sequence + 2);
        matches = 1;
        if (condition >= 93)
            matches = (sint32)(condition - 93) != r_s16(actor + 24);
        else if (condition >= 70)
            matches = (sint32)(condition - 70) == r_s16(actor + 24);
        else if (condition >= 47)
            matches = (sint32)(condition - 47) == r_s16(opponent + 22);
        else if (condition >= 24)
            matches = (sint32)(condition - 24) != r_s16(actor + 22);
        else if (condition)
            matches = (sint32)(condition - 1) == r_s16(actor + 22);
        if (matches) {
            test = r_u8(sequence + 3);
            if (test < 69 && (!test || t3_8002e0bc(sequence, actor, opponent)))
                break;
        }
advance:
        sequence += 12;
        command = r_u16(sequence);
    }
    action = r_u8(sequence + 8);
    if (action == 44) {
        if (r_u16(sequence) != 0xC000) {
            do {
                sequence += 12;
                command = r_u16(sequence);
            } while (command != 0xC000);
        }
        action = r_u8(sequence + 8);
    }
    if (action == 28 && r_u16(sequence) != 0xC000 && r_s16(actor + 0x128)) {
        sequence += 12;
        goto rescan;
    }
    if (r_u32(0x800AFA88) == 8 && r_u8(sequence + 8) == 33
        && r_u16(sequence) != 0xC000 && !r_u8(actor + 0xC4) && r_u8(opponent + 0xC4)) {
        sequence += 12;
        goto rescan;
    }
    return sequence;
}

uint32 t3_8006a158(uint32 initial_asset_slot)
{
    uint32 asset_slot = initial_asset_slot;
    uint32 actor = 0x800A9228;
    uint32 index;
    FUNCTION_MARKER(0x8006A158, "SLUS_004.02");
    for (index = 0; index < 2; ++index) {
        sint32 character = r_s16(actor + 0x16);
        uint32 resource;
        if (character == r_s16(0x800A0510))
            asset_slot = 0;
        else if (character == r_s16(0x800A0518))
            asset_slot = 1;
        actor += 0x188C;
        resource = r_u32(0x800A0514 + (asset_slot << 3));
        w_u32(0x800ADC20 + index * 4, resource);
        t3_8002ce14((sint32)index);
    }
    return t3_8002d07c();
}

uint32 t3_8004f0e4(uint32 controller)
{
    uint32 selection[2][4];
    uint32 other = (controller + 1) & 1;
    uint32 offset = controller * 2;
    uint32 other_offset = other * 2;
    FUNCTION_MARKER(0x8004F0E4, "SLUS_004.02");
    selection[controller][0] = r_u16(0x800ADD5C + offset);
    selection[controller][1] = r_u16(0x800ADD98 + offset);
    selection[controller][2] = r_u16(0x800ADFBC + offset);
    selection[controller][3] = r_u16(0x800AE1F8 + offset);
    selection[other][0] = r_u16(0x800ADD5C + other_offset);
    selection[other][1] = r_u16(0x800ADD98 + other_offset);
    selection[other][2] = r_u16(0x800ADFBC + other_offset);
    selection[other][3] = r_u16(0x800AE1F8 + other_offset);
    if (selection[other][2] && selection[controller][0] == selection[other][0]
        && selection[controller][1] == selection[other][1]) {
        if (!selection[other][3] && selection[controller][3]) {
            uint32 variant = selection[controller][1];
            selection[other][1] = variant < 2 ? variant ^ 1 : 0;
        }
        else {
            uint32 variant = selection[other][1];
            selection[controller][1] = variant < 2 ? variant ^ 1 : 0;
        }
    }
    w_u16(0x800ADD5C + offset, (uint16)selection[controller][0]);
    w_u16(0x800ADD98 + offset, (uint16)selection[controller][1]);
    w_u16(0x800ADFBC + offset, 1);
    w_u16(0x800ADD5C + other_offset, (uint16)selection[other][0]);
    w_u16(0x800ADD98 + other_offset, (uint16)selection[other][1]);
    return 0x800ADD98 + other_offset;
}

uint32 t3_8004f3b0(uint32 context)
{
    uint32 mode;
    uint32 controller;
    uint32 key;
    uint32 descriptor;
    uint32 first;
    uint32 second;
    uint32 character;
    uint32 variant;
    FUNCTION_MARKER(0x8004F3B0, "SLUS_004.02");
    mode = r_u32(context);
    switch (mode) {
    case 1: {
        uint8 selections[2];
        uint32 other_character;
        uint32 other_variant;
        uint32 previous;
        character = r_u16(0x800ADD5C);
        variant = r_u16(0x800ADD98);
        if (character >= 22 || variant >= 4) {
            character = 22;
            variant = 0;
        }
        other_character = r_u16(0x800ADD5E);
        other_variant = r_u16(0x800ADD9A);
        selections[0] = (uint8)((character << 2) | variant);
        if (other_character >= 22 || other_variant >= 4) {
            other_character = 22;
            other_variant = 0;
        }
        controller = r_u8(0x800B053E);
        selections[1] = (uint8)((other_character << 2) | other_variant);
        previous = r_u32(context + 0x44);
        key = selections[controller];
        if (previous != 0xFFFFFFFF && selections[0] != r_u32(context + 0x48)
            && selections[1] != r_u32(context + 0x4C))
            key = previous;
        w_u32(context + 0x44, key);
        w_u32(context + 0x48, selections[0]);
        w_u32(context + 0x4C, selections[1]);
        goto lookup_descriptor;
    }
    case 2:
    case 5:
        controller = r_u8(0x800B053E);
        break;
    case 8:
        controller = r_u32(0x800B6BF4);
        character = r_u16(0x800ADD5C + controller * 2);
        variant = r_u16(0x800ADD98 + controller * 2);
        key = (character << 2) | variant;
        if ((sint32)key >= 93)
            key = 88;
        first = r_u32(context + 0x24) + 15;
        descriptor = r_u32(0x80097D40 + key * 4);
        second = r_u8(descriptor + 11);
        if ((sint32)first >= 19) {
            first = 15;
            second = 25;
        }
        goto save_result;
    default: {
        uint32 count = r_u8(context + 0x1C);
        // The original menu producer limits count to 0..2
        if (!count)
            controller = 0;
        else if (count == 1)
            controller = r_u8(context + 0x1F);
        else
            controller = r_u8(0x800B053E);
        break;
    }
    }
    character = r_u16(0x800ADD5C + controller * 2);
    variant = r_u16(0x800ADD98 + controller * 2);
    key = (character << 2) | variant;
lookup_descriptor:
    if ((sint32)key >= 93)
        key = 88;
    descriptor = r_u32(0x80097D40 + key * 4);
    if (mode == 7)
        first = 19;
    else
        first = r_u8(descriptor + 10);
    second = r_u8(descriptor + 11);
save_result:
    w_u16(0x800ADC84, (uint16)first);
    w_u16(0x800ADD08, (uint16)second);
    w_u8(0x800AFAA2, (uint8)first);
    w_u8(0x800AFAA3, (uint8)second);
    return 0x800AFA88;
}

uint32 t3_80047bbc(uint32 actor, uint32 opponent, uint32 output, uint32 initial_segment, uint32 initial_count)
{
    uint32 segment = initial_segment;
    uint32 count = initial_count;
    uint32 animation;
    uint32 model;
    uint32 part;
    FUNCTION_MARKER(0x80047BBC, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    model = r_u32(animation + 0x28);
    if (r_u8(model) < 24) {
        count = r_u8(actor + 0xE3);
        segment = actor + 0x1AC;
        if (count >= 5)
            count = 4;
    }
    else {
        uint32 side = r_u8(actor + 0x1E);
        if (side < 3) {
            if (!side)
                count = r_u32(0x800ADEAC);
            else
                count = r_u32(0x800ADEB0);
            segment = t3_8006e51c(side != 0, 0);
        }
    }
    for (part = 0; (sint32)part < (sint32)count; ++part) {
        uint32 sphere;
        uint32 index;
        if (r_s16(opponent + 0x104))
            return 0;
        sphere = opponent + 0x20C;
        for (index = 0; index < 14; ++index) {
            if (t3_8004770c(segment, sphere)) {
                uint32 value;
                uint32 start;
                w_u16(output + 0x18, (uint16)index);
                value = r_u32(segment + 12);
                w_u32(output, value);
                value = r_u32(segment + 16);
                w_u32(output + 4, value);
                value = r_u32(segment + 20);
                w_u32(output + 8, value);
                value = r_u16(segment + 12);
                start = r_u16(segment);
                w_u16(output + 16, (uint16)(value - start));
                value = r_u16(segment + 16);
                start = r_u16(segment + 4);
                w_u16(output + 18, (uint16)(value - start));
                value = r_u16(segment + 20);
                start = r_u16(segment + 8);
                w_u16(output + 20, (uint16)(value - start));
                return 1;
            }
            sphere += 20;
        }
        segment += 24;
    }
    return 0;
}

uint32 t3_80047d5c(uint32 actor, uint32 opponent, uint32 output, uint32 initial_segment, uint32 initial_count)
{
    uint32 result;
    FUNCTION_MARKER(0x80047D5C, "SLUS_004.02");
    result = t3_80047bbc(actor, opponent, output, initial_segment, initial_count);
    return (uint32)(sint32)(sint16)result;
}

uint32 t3_80045038(uint32 damage, uint32 index)
{
    uint32 weights[7];
    uint32 word;
    sint32 factor;
    sint32 product;
    sint32 scaled;
    FUNCTION_MARKER(0x80045038, "SLUS_004.02");
    for (word = 0; word < 7; ++word)
        weights[word] = r_u32(0x8001DEEC + word * 4);
    factor = (sint16)(weights[index >> 1] >> ((index & 1) * 16));
    product = (sint32)(sint16)damage * factor;
    scaled = product / 100;
    return (uint32)(sint32)(sint16)scaled;
}

uint32 t3_80043684(uint32 count)
{
    uint32 flags[3] = {0, 0, 0};
    uint32 index;
    sint32 first;
    FUNCTION_MARKER(0x80043684, "SLUS_004.02");
    for (index = 0; (sint32)index < (sint32)count; ++index) {
        uint32 actor = 0x800A9228 + index * 0x188C;
        flags[index] = r_u16(actor + 0x1278) >= 401;
        if (r_u32(0x800AFA88) == 8 && r_u8(actor + 0xC5)) {
            uint32 side = (r_u8(actor + 0x1E) - 1) & 1;
            uint32 blocked = 0;
            if (r_s16(0x800B6598 + side * 24 + 8) == 1)
                blocked = r_u32(0x800B6C04) == 0;
            flags[index] |= blocked;
        }
    }
    for (first = (sint32)(count - 1); first > 0; --first) {
        sint32 second;
        uint32 first_position = 0x800AA17C + (uint32)first * 0x188C;
        for (second = first - 1; second >= 0; --second) {
            uint32 second_position = 0x800AA17C + (uint32)second * 0x188C;
            uint32 first_x = r_u32(first_position + 20);
            uint32 second_x = r_u32(second_position + 20);
            uint32 first_z = r_u32(first_position + 28);
            uint32 second_z = r_u32(second_position + 28);
            uint32 dx = first_x - second_x;
            uint32 dz = first_z - second_z;
            uint32 pair = t3_800439a8((uint32)first, (uint32)second);
            uint32 distance = t3_8002a324(dx * dx + dz * dz);
            uint32 angle;
            w_u32(pair, distance);
            w_u32(pair + 4, dx);
            w_u32(pair + 8, dz);
            angle = t3_8002a224(dz, dx);
            w_u16(pair + 12, (uint16)angle);
        }
    }
    for (index = 0; (sint32)index < (sint32)count; ++index) {
        uint32 actor = 0x800A9228 + index * 0x188C;
        uint32 x = r_u32(actor + 0xF68);
        uint32 z = r_u32(actor + 0xF70);
        uint32 identity = r_u8(actor + 0x1E);
        uint32 opponent_index;
        uint32 opponent;
        uint32 pair;
        uint32 distance;
        uint32 value;
        uint32 angle;
        w_u32(actor + 0x12C, x);
        w_u32(actor + 0x130, z);
        opponent_index = t3_80045bb8(actor);
        pair = t3_800439a8(identity, opponent_index);
        distance = r_u32(pair);
        w_u32(actor + 0xF8, distance);
        opponent = t3_80045c18(actor);
        distance = r_u32(pair);
        value = t3_80043a40(actor, opponent, distance);
        w_u32(actor + 0xF4, value);
        value = r_u32(pair + 4);
        w_u32(actor + 0xFC, value);
        value = r_u32(pair + 8);
        w_u32(actor + 0x100, value);
        angle = r_u16(pair + 12);
        if ((sint32)opponent_index >= (sint32)identity)
            angle -= 0x8000;
        if (r_u8(actor + 0xC7) || flags[opponent_index]) {
            sint32 distance_angle = 180 - r_s16(actor + 0x1278);
            if (distance_angle < 0)
                distance_angle = -distance_angle;
            distance_angle = 180 - distance_angle;
            if (distance_angle < 0)
                distance_angle = -distance_angle;
            value = (uint32)distance_angle * 10;
            angle = r_u8(actor + 0xC8) ? 0xC000 : 0x4000;
            w_u16(actor + 0x2A, (uint16)angle);
            w_u32(actor + 0xF8, value);
            w_u32(actor + 0xFC, value);
            w_u32(actor + 0x100, 0);
        }
        else
            w_u16(actor + 0x2A, (uint16)angle);
    }
    w_u32(0x800AFA3C, r_u32(0x800A9320));
    return 0x800B0000;
}

uint32 t3_80036310(uint32 actor, uint32 *model_output, uint32 *extra_output)
{
    uint32 identity;
    uint32 slot;
    uint32 resource;
    uint32 load_required;
    uint32 value;
    uint32 cached_header;
    FUNCTION_MARKER(0x80036310, "SLUS_004.02");
    identity = r_u8(actor + 0x1E);
    slot = (uint32)r_s16(actor + 0x1C);
    resource = t3_80075200(identity);
    load_required = t3_8006c63c(identity == 0 ? 1 : 2, 0);
    t3_80029638(1);
    value = t3_80035ed0(identity);
    w_u32(0x8009BD38, value);
    value = r_u32(0x80095A9C + slot * 12);
    w_u32(0x8009BD3C, value);
    value = t3_80075240(identity);
    w_u32(0x8009BD40, value);
    value = r_u32(0x80095AA0 + slot * 12);
    w_u32(0x8009BD44, value);
    value = t3_800561a0();
    w_u32(0x8009BD48, value);
    value = r_u32(0x80095AA4 + slot * 12);
    w_u32(0x8009BD4C, value);
    if (load_required) {
        uint32 cached_asset;
        uint32 cache_index = t3_80069e2c(r_s16(actor + 0x16), &cached_asset);
        t3_8006bea8();
        t3_8006bf20(slot * 4 + 95, r_u32(0x8009BD38), 0);
        t3_8006bf20(slot * 4 + 96, r_u32(0x8009BD40), 0);
        t3_8006bf20(slot * 4 + 97, r_u32(0x8009BD48), 0);
        if (cached_asset)
            t3_8006bf20(slot * 4 + 98, cached_asset, 0);
        t3_8006c084();
        while (t3_8006c23c()) {
        }
        t3_80069ea4(cache_index);
    }
    *model_output = r_u32(0x8009BD38);
    cached_header = r_u32(0x8009BD40);
    value = t3_80035eec(identity);
    *extra_output = value;
    if (load_required) {
        uint32 archive0;
        uint32 archive1;
        uint32 archive2;
        uint32 archive4;
        uint32 source;
        uint32 archive;
        uint32 destination;
        uint32 size;
        uint32 offset;
        uint32 model;
        uint32 extra;
        uint32 current_identity;
        sint32 character;
        archive0 = t3_80031cbc(0, r_u32(0x8009BD48));
        archive1 = t3_80031cbc(1, r_u32(0x8009BD48));
        archive2 = t3_80031cbc(2, r_u32(0x8009BD48));
        t3_80031cbc(3, r_u32(0x8009BD48));
        archive4 = t3_80031cbc(4, r_u32(0x8009BD48));
        t3_80031cf0(3, r_u32(0x8009BD48));
        t3_80031cf0(4, r_u32(0x8009BD48));
        source = t3_80031cbc(3, r_u32(0x8009BD48));
        archive = r_u32(0x8009BD48);
        destination = *extra_output;
        size = t3_80031cf0(3, archive);
        for (offset = 0; (sint32)offset < (sint32)size; offset += 4) {
            value = r_u32(source);
            source += 4;
            w_u32(destination, value);
            destination += 4;
        }
        if (identity < 2 && r_s16(actor + 0x18) != 21)
            t3_800784e4(archive4, identity);
        model = *model_output;
        extra = *extra_output;
        t3_80035bc0(actor, archive0, model, extra);
        current_identity = r_u8(actor + 0x1E);
        character = r_s16(actor + 0x18);
        t3_8007619c((sint32)current_identity, character, archive1);
        t3_80075288(identity, resource, cached_header, archive2);
    }
    return t3_80029638(0);
}

uint32 t3_800780bc(uint32 paused)
{
    PSX_RECT rectangle;
    uint8 text[24];
    uint32 disabled;
    uint32 cooldown;
    uint32 ordering_entry;
    uint32 packet;
    uint32 x;
    uint32 y;
    uint32 width;
    uint32 height;
    uint32 menu_height = 65;
    uint32 count = 3;
    uint32 input;
    uint32 row;
    uint32 table;
    uint32 flags;
    uint32 display;
    FUNCTION_MARKER(0x800780BC, "SLUS_004.02");
    disabled = r_u32(0x800ADCA4);
    if (disabled)
        return disabled;
    cooldown = r_u8(0x800A8672);
    if (cooldown) {
        --cooldown;
        w_u8(0x800A8672, (uint8)cooldown);
        return cooldown;
    }
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    y = r_u16(0x800AE232);
    width = r_u16(0x800AE234);
    height = r_u16(0x800AE236);
    x = r_u16(0x800AE230);
    rectangle.x = (sint16)x;
    rectangle.y = (sint16)y;
    rectangle.w = (sint16)width;
    rectangle.h = (sint16)(height - 32);
    packet = t3_8004db28(ordering_entry, packet, &rectangle);
    w_u32(0x800ADF64, packet);
    if (r_u32(0x800AFA88) == 7) {
        menu_height = 85;
        count = 4;
    }
    input = r_u16(paused == 1 ? 0x800ADF08 : 0x800ADF0A);
    if (r_u8(0x800A8672))
        input = 0;
    if (input & 0x1000) {
        uint32 selected = r_u8(0x80098A40);
        selected = selected ? selected - 1 : count - 1;
        w_u8(0x80098A40, (uint8)selected);
        t3_800753bc(0, 0x546C, 0);
    }
    if (input & 0x4000) {
        uint32 selected = r_u8(0x80098A40) + 1;
        w_u8(0x80098A40, (uint8)(selected % count));
        t3_800753bc(0, 0x546C, 0);
    }
    if (input & 0x8F0) {
        uint32 selected;
        w_u8(0x800A8672, 2);
        selected = r_u8(0x80098A40);
        w_u8(0x80098A41, (uint8)(selected + 1));
        t3_800753bc(0, 0x50F4, 0);
    }
    y = 104;
    table = 0x80098A44;
    for (row = 0; row < count; ++row) {
        uint32 label = r_u32(table);
        uint32 length = (uint32)strlen((const char *)psx_addr(label, 8));
        uint32 centered_x = 184 - (((length << 3) + length) >> 1);
        uint8 *end = t3_copy_guest_string_to_native(text, 0x80097D08);
        uint32 color;
        end = t3_copy_guest_string_to_native(end, 0x80027BEC);
        t3_copy_guest_string_to_native(end, r_u32(table));
        color = row == r_u8(0x80098A40) ? 2 : 6;
        t3_render_native_text(text, color, (uint32)0, centered_x, y, (uint32)0);
        y += 20;
        table += 4;
    }
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    packet = t3_8004da10(ordering_entry, packet, 0x60301000, 0x00640094, (menu_height << 16) | 0x48);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering_entry, packet, 0x60E0E0E0, 0x00620093, ((menu_height + 4) << 16) | 0x4A);
    flags = r_u32(0x800AE218);
    w_u32(0x800ADF64, packet);
    if (flags & 0x20) {
        if (paused == 1)
            x = 32;
        else {
            x = (uint32)r_s16(0x800AE230);
            width = (uint32)r_s16(0x800AE234);
            x = x + width - 136;
        }
        t3_8004ce74(0x80027BF0, (uint32)7, (uint32)5, (uint32)1, x, (uint32)120, paused);
    }
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    x = r_u16(0x800AE230);
    width = r_u16(0x800AE234);
    rectangle.x = (sint16)x;
    rectangle.y = 98;
    rectangle.w = (sint16)width;
    rectangle.h = (sint16)(menu_height + 4);
    packet = t3_8004db28(ordering_entry, packet, &rectangle);
    x = r_u16(0x800AE230);
    y = r_u16(0x800AE232);
    display = r_u32(0x800A8C54);
    rectangle.w = 0;
    rectangle.h = 0;
    w_u32(0x800ADF64, packet);
    rectangle.x = (sint16)x;
    rectangle.y = (sint16)y;
    ordering_entry = r_u32(display + 4) + 8;
    packet = t3_8004db28(ordering_entry, packet, &rectangle);
    x = r_u16(0x800AE230);
    y = r_u16(0x800AE232);
    width = r_u16(0x800AE234);
    display = r_u32(0x800A8C54);
    rectangle.h = 162;
    w_u32(0x800ADF64, packet);
    rectangle.x = (sint16)x;
    rectangle.y = (sint16)y;
    rectangle.w = (sint16)width;
    ordering_entry = r_u32(display + 16);
    packet = t3_8004db28(ordering_entry, packet, &rectangle);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_80052520(void)
{
    uint32 draw_objects = 0;
    sint32 phase;
    uint32 result = 0;
    uint32 index;
    FUNCTION_MARKER(0x80052520, "SLUS_004.02");
    phase = r_s16(0x800AE224);
    switch (phase) {
    case 0:
        t3_8002960c(0);
        result = 1;
        w_u16(0x800AE224, 1);
        break;
    case 1:
    {
        uint32 selection[6];
        uint32 first;
        uint32 second;
        uint32 mode;
        uint32 resource;
        uint32 cached;
        for (index = 0; index < 64; ++index)
            w_u16(0x800B8D60 + index * 40, 0);
        selection[4] = r_u8(0x800AFAA4) != 1;
        first = r_u16(0x800ADD5C);
        second = r_u16(0x800ADD98);
        selection[0] = (first << 2) | second;
        first = r_u16(0x800ADD5E);
        second = r_u16(0x800ADD9A);
        selection[1] = (first << 2) | second;
        selection[2] = selection[0];
        selection[3] = selection[1];
        mode = r_u32(0x800AFA88);
        switch (mode) {
        case 1: t3_bns_00_800b38a8(selection); break;
        case 2: t3_bns_00_800b399c(selection); break;
        case 4: t3_bns_00_800b3ad0(selection); break;
        case 5: t3_bns_01_800b6b00(selection); break;
        case 6: t3_bns_00_800b3bc4(selection); break;
        case 7: t3_bns_03_800b50f0(selection); break;
        case 8: t3_bns_02_800b6164(selection); break;
        default: t3_bns_00_800b37a8(selection); break;
        }
        t3_80029638(1);
        resource = r_u16(0x800B8D58);
        cached = r_u16(0x80098104);
        if (resource != cached) {
            w_u16(0x80098104, (uint16)resource);
            t3_80052ad0(0x800C19A0, (sint32)resource);
            t3_8004ca40(0x800B9760, 0x800C19A0);
        }
        for (index = 0; index < 2; ++index) {
            uint32 variant = r_u8(0x800B8D5A + index);
            uint32 previous = r_u8(0x80098106 + index);
            if (variant != previous) {
                uint32 image_x;
                uint32 packed_clut;
                uint32 image_y;
                uint32 clut_x;
                uint32 clut_y;
                w_u8(0x80098106 + index, (uint8)variant);
                image_x = (uint32)r_s16(0x80098108 + index * 8);
                packed_clut = (uint32)r_s16(0x8009810C + index * 8);
                image_y = (uint32)r_s16(0x8009810A + index * 8);
                clut_x = (packed_clut & 63) << 4;
                clut_y = packed_clut >> 6;
                t3_80052ad0(0x800C19A0, (sint32)(variant + 12));
                t3_8004c7e4(0x800B9760, 0x800C19A0, clut_x, clut_y, image_x, image_y);
            }
        }
        t3_80029638(0);
        w_u32(0x800B8D5C, 8);
        result = 2;
        w_u16(0x800AE224, 2);
        break;
    }
    case 2:
    {
        uint32 timer = r_u32(0x800B8D5C) - 1;
        draw_objects = 1;
        w_u32(0x800B8D5C, timer);
        if ((sint32)timer <= 0)
            w_u16(0x800AE224, 3);
        break;
    }
    case 3:
        draw_objects = 1;
        w_u16(0x800AE224, 4);
        break;
    case 4:
        w_u16(0x800AE204, r_u8(0x800AFA9C));
        w_u16(0x800AE224, r_u8(0x800AFA9D));
        result = 0x800B0000;
        break;
    }
    if (draw_objects) {
        uint32 active_buffer = r_u32(0x800ADEFC);
        uint32 saved_packet = r_u32(0x800ADF64);
        uint32 offset = ((active_buffer << 4) - active_buffer) << 10;
        w_u32(0x800ADF64, 0x800B9760 + offset);
        for (index = 0; index < 64; ++index) {
            uint32 object = 0x800B8D60 + index * 40;
            uint32 kind = r_u16(object);
            switch (kind) {
            case 1: t3_800519b4(); break;
            case 2: t3_bns_00_800b3240(); break;
            case 3: t3_80051b8c(object); break;
            case 4: t3_800520ac(object); break;
            case 5: t3_bns_00_800b3350(object); break;
            case 6: t3_bns_00_800b33e8(object); break;
            case 7: case 8: t3_80052250(object); break;
            }
        }
        w_u32(0x800ADF64, saved_packet);
        result = 0x800B0000;
    }
    return result;
}

uint32 t3_8006d014(void)
{
    uint32 selection[36];
    uint16 depths[256];
    uint32 camera_x;
    uint32 camera_y;
    uint32 camera_z;
    uint32 history;
    uint32 history_offset;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    uint32 x;
    uint32 z;
    uint32 angle;
    uint32 step;
    uint32 enabled;
    uint32 flags;
    sint32 matrix_yx;
    sint32 matrix_yy;
    sint32 matrix_yz;
    uint32 record;
    uint32 packet;
    uint32 active_buffer;
    uint32 ordering;
    uint32 result;
    uint32 index = 0;
    FUNCTION_MARKER(0x8006D014, "SLUS_004.02");
    camera_x = r_u32(0x800A85D4);
    camera_y = r_u32(0x800A85D8);
    camera_z = r_u32(0x800A85DC);
    history = r_u32(0x800A08F0) + 1;
    w_u32(0x1F800060, camera_x);
    w_u32(0x1F800064, camera_y);
    history_offset = (history & 3) << 4;
    w_u32(0x1F800068, camera_z);
    w_u32(0x800A08F8 + history_offset, r_u32(0x800AA190));
    w_u32(0x800A0900 + history_offset, r_u32(0x800AA198));
    w_u32(0x800A0938 + history_offset, r_u32(0x800ABA1C));
    z = r_u32(0x800ABA24);
    w_u32(0x800A08F0, history);
    w_u32(0x800A0940 + history_offset, z);
    angle = t3_8006e1b8();
    angle = (r_u32(0x800A9188) + angle) & 0xFFF;
    w_u32(0x800A9188, angle);
    angle = r_u16(0x800A9188);
    w_u16(0x1F800054, 0);
    w_u16(0x1F800050, 0);
    w_u16(0x1F800052, (uint16)angle);
    t3_8003a490((const SVECTOR *)psx_addr(0x1F800050, 8), (MATRIX *)psx_addr(0x1F800000, 32));
    first = r_u32(0x800ABA1C);
    second = r_u32(0x800AA190);
    w_u16(0x1F800052, 0);
    third = r_u32(0x800ABA24);
    fourth = r_u32(0x800AA198);
    x = (uint32)((sint32)(first - second) >> 1);
    first = r_u16(0x800AA190);
    z = (uint32)((sint32)(third - fourth) >> 1);
    x += first;
    first = r_u16(0x1F800060);
    w_u16(0x1F800050, (uint16)(first - x));
    first = r_u16(0x800AA198);
    second = r_u16(0x1F800068);
    w_u16(0x1F800054, (uint16)(second - (first + z)));
    ApplyMatrix((MATRIX *)psx_addr(0x1F800000, 32), (SVECTOR *)psx_addr(0x1F800050, 8), (VECTOR *)psx_addr(0x1F800040, 12));
    if (r_s16(0x800AE204) == 6) {
        x = r_u32(0x1F800040);
        z = r_u32(0x1F800048);
        first = r_u32(0x800A85C4);
        angle = r_u32(0x800A9188);
        step = r_u32(0x800ADE00);
        t3_8006d95c(780, first - angle, x, z, step, selection);
        enabled = r_s32(0x1F800064) >= -1600;
        selection[10] = enabled;
        selection[16] = enabled;
        selection[22] = enabled;
        selection[28] = enabled;
        enabled = r_s32(0x1F800064) >= -2000;
    }
    else {
        x = r_u32(0x1F800040);
        z = r_u32(0x1F800048);
        first = r_u32(0x800A85C4);
        angle = r_u32(0x800A9188);
        step = r_u32(0x800ADE00);
        t3_8006d95c(600, first - angle, x, z, step, selection);
        camera_y = r_u32(0x1F800064);
        if ((sint32)camera_y < -5000)
            enabled = 0;
        else if ((sint32)camera_y >= -2800)
            enabled = 1;
        else {
            x = r_u32(0x1F800060);
            z = r_u32(0x1F800068);
            enabled = (sint32)t3_8004aedc(x * x + z * z) >= 10000;
        }
    }
    selection[14] = enabled;
    selection[15] = enabled;
    selection[20] = enabled;
    selection[21] = enabled;
    first = r_u16(0x800A85C0);
    w_u16(0x800A08E0, (uint16)(first & 0xFFF));
    first = r_u16(0x800A85C4);
    second = r_u16(0x800A85C8);
    w_u16(0x800A08E2, (uint16)(first & 0xFFF));
    w_u16(0x800A08E4, (uint16)((0 - second) & 0xFFF));
    t3_8003a490((const SVECTOR *)psx_addr(0x800A08E0, 8), (MATRIX *)psx_addr(0x1F800000, 32));
    angle = r_u16(0x800A9188);
    w_u16(0x1F800050, 0);
    w_u16(0x1F800054, 0);
    w_u16(0x1F800052, (uint16)((0 - angle) & 0xFFF));
    t3_8003a490((const SVECTOR *)psx_addr(0x1F800050, 8), (MATRIX *)psx_addr(0x1F800020, 32));
    matrix_yx = r_s16(0x1F800006) * 20;
    matrix_yy = r_s16(0x1F800008) * 20;
    matrix_yz = r_s16(0x1F80000A) * 20;
    w_u16(0x1F800006, (uint16)(matrix_yx / 12));
    w_u16(0x1F800008, (uint16)(matrix_yy / 12));
    flags = r_u8(0x800AFAA1);
    w_u16(0x1F80000A, (uint16)(matrix_yz / 12));
    if (flags) {
        x = r_u32(0x1F800060);
        z = r_u32(0x1F800068);
        w_u32(0x1F800080, 0 - x);
        w_u32(0x1F800088, 0 - z);
    }
    else {
        sint32 side = (sint32)t3_80062b04();
        if (side == -1) {
            first = r_u32(0x800ABA1C);
            second = r_u32(0x800AA190);
            camera_x = (uint32)r_s16(0x1F800060);
            third = r_u32(0x800AA198);
            x = (uint32)((sint32)(first + second) >> 1);
            fourth = r_u32(0x800ABA24);
            w_u32(0x1F800080, x - camera_x);
            camera_z = (uint32)r_s16(0x1F800068);
            z = (uint32)((sint32)(fourth + third) >> 1);
            w_u32(0x1F800088, z - camera_z);
        }
        else if (side == 0) {
            first = r_u32(0x800A08F8);
            second = r_u32(0x800A0908);
            fourth = r_u32(0x800A0928);
            first += second;
            third = r_u32(0x800A0918);
            x = (uint32)((sint32)(first + third + fourth) >> 2);
            first = r_u32(0x800A0900);
            second = r_u32(0x800A0910);
            w_u32(0x1F800090, x);
            third = r_u32(0x800A0920);
            fourth = r_u32(0x800A0930);
            z = (uint32)((sint32)(first + second + third + fourth) >> 2);
            x = r_u32(0x1F800090);
            first = r_u32(0x800ABA1C);
            x = (uint32)((sint32)(x + first) >> 1);
            w_u32(0x1F800098, z);
            camera_x = (uint32)r_s16(0x1F800060);
            first = r_u32(0x800ABA24);
            w_u32(0x1F800080, x - camera_x);
            camera_z = (uint32)r_s16(0x1F800068);
            z = (uint32)((sint32)(z + first) >> 1);
            w_u32(0x1F800088, z - camera_z);
        }
        else if (side == 1) {
            first = r_u32(0x800A0938);
            second = r_u32(0x800A0948);
            fourth = r_u32(0x800A0968);
            first += second;
            third = r_u32(0x800A0958);
            x = (uint32)((sint32)(first + third + fourth) >> 2);
            first = r_u32(0x800A0940);
            second = r_u32(0x800A0950);
            w_u32(0x1F8000A0, x);
            third = r_u32(0x800A0960);
            fourth = r_u32(0x800A0970);
            z = (uint32)((sint32)(first + second + third + fourth) >> 2);
            x = r_u32(0x1F8000A0);
            first = r_u32(0x800AA190);
            w_u32(0x1F8000A8, z);
            second = r_u32(0x800AA198);
            x = (uint32)((sint32)(x + first) >> 1);
            camera_x = (uint32)r_s16(0x1F800060);
            z = (uint32)((sint32)(z + second) >> 1);
            w_u32(0x1F800080, x - camera_x);
            camera_z = (uint32)r_s16(0x1F800068);
            w_u32(0x1F800088, z - camera_z);
        }
    }
    record = t3_8006c828(0);
    first = r_u16(0x1F800080);
    w_u16(0x1F800050, (uint16)first);
    first = r_u32(0x800B0460);
    second = r_u16(0x80025138 + (first << 2));
    third = r_u16(0x1F800088);
    w_u16(0x1F800054, (uint16)third);
    fourth = r_u16(0x1F800064);
    first = r_u16(0x1F800050);
    w_u16(0x1F800052, (uint16)(second - fourth));
    second = r_u16(0x1F800054);
    w_u16(0x1F800050, (uint16)((sint16)first >> 3));
    first = r_u16(0x1F800052);
    w_u16(0x1F800054, (uint16)((sint16)second >> 3));
    w_u16(0x1F800052, (uint16)((sint16)first >> 3));
    ApplyMatrix((MATRIX *)psx_addr(0x1F800000, 32), (SVECTOR *)psx_addr(0x1F800050, 8), (VECTOR *)psx_addr(0x1F800014, 12));
    MulMatrix2((MATRIX *)psx_addr(0x1F800000, 32), (MATRIX *)psx_addr(0x1F800020, 32));
    SetRotMatrix((MATRIX *)psx_addr(0x1F800020, 32));
    SetTransMatrix((MATRIX *)psx_addr(0x1F800000, 32));
    w_u32(0x1F800324, 200);
    packet = r_u32(0x800ADD58);
    active_buffer = r_u32(0x800ADEFC);
    if (packet)
        packet += active_buffer * 8400;
    w_u32(0x800A08DC, packet);
    active_buffer = r_u32(0x800ADEFC);
    ordering = 0x800A08CC + (active_buffer << 3);
    result = r_u32(0x800ADEC4);
    w_u32(0x800A08D8, ordering);
    if ((sint32)result <= 0)
        return result;
    do {
        result = r_u32(0x1F800324);
        if (!result)
            return result;
        first = r_u32(record);
        if ((sint32)selection[first] > 0) {
            uint32 vertices = r_u32(record + 12);
            uint32 count = r_u32(record + 16);
            t3_80036af8(vertices, (uint32 *)psx_addr(0x1F800000, 1024), depths, count);
            first = r_u32(record + 8);
            packet = r_u32(0x800A08DC);
            ordering = r_u32(0x800A08D8);
            w_u32(0x800A0978, first);
            second = r_u32(record + 4);
            w_u32(0x1F800320, second);
            t3_8006cc28(first, packet, ordering, 0x1F800320);
            first = r_u32(0x1F800320);
            packet = r_u32(0x800A08DC);
            w_u32(0x800A08DC, packet + ((uint32)((sint32)first >> 2) << 2));
        }
        ++index;
        record += 36;
        result = (sint32)index < r_s32(0x800ADEC4);
    } while (result);
    return result;
}

uint32 t3_80079e58(void)
{
    FUNCTION_MARKER(0x80079E58, "SLUS_004.02");
    return (uint32)EnterCriticalSectionPSX();
}

void t3_80079e68(void)
{
    FUNCTION_MARKER(0x80079E68, "SLUS_004.02");
    ExitCriticalSection();
}

void t3_8007af44(void)
{
    FUNCTION_MARKER(0x8007AF44, "SLUS_004.02");
    t3_80079e58();
    t3_8007b438();
    ChangeClearPAD(0);
    t3_80079e68();
}

uint32 t3_8007b428(uint32 mode)
{
    FUNCTION_MARKER(0x8007B428, "SLUS_004.02");
    return xport_bios_init_card(mode);
}

uint32 t3_8007b438(void)
{
    FUNCTION_MARKER(0x8007B438, "SLUS_004.02");
    return xport_bios_start_card();
}

void t3_8007aed8(uint32 mode)
{
    FUNCTION_MARKER(0x8007AED8, "SLUS_004.02");
    ChangeClearPAD(0);
    t3_80079e58();
    if (!ReadInitPadFlag())
        mode = 0;
    t3_8007b428(mode);
    // Native card services replace BIOS instruction patches
    t3_80079e68();
}

uint32 t3_800759d4(void)
{
    uint32 flags[3] = { 0, 0, 0 };
    uint32 mode, count, index, actor, event, blocked, target, selected, bits;
    FUNCTION_MARKER(0x800759D4, "SLUS_004.02");
    if (r_u32(0x800954A4))
        return 0x800B0000;
    mode = r_u32(0x800AFA88);
    if (mode == 7 && r_u32(0x800AFAC0))
        return 8;
    count = mode == 8 ? 3 : 2;
    for (index = 0; index < count; ++index) {
        actor = 0x800A9228 + index * 0x188C;
        event = t3_80075ef0(index);
        blocked = r_u8(actor + 0xCF);
        target = r_u8(actor + 0x22);
        selected = 0xFFFFFFFF;
        if (blocked) {
            t3_80028ec0(event, 16);
            flags[target] |= 1;
        }
        if (r_u8(actor + 0xD0)) {
            bits = r_s16(actor + 0xE8) < 21 ? 2 : 4;
            selected = target;
            t3_80028ec0(event, bits == 2 ? 17 : 18);
            flags[selected] |= bits;
            if (r_u8(actor + 0xD4)) {
                t3_80028ec0(event, 20);
                flags[target] |= 8;
            }
            if (r_u8(actor + 0xD3)) {
                t3_80028ec0(event, 21);
                flags[target] |= 16;
            }
        }
        if (r_s16(actor + 0xCC) > 0) {
            t3_80028ec0(event, 19);
            selected = r_u8(actor + 0x21);
            flags[selected] |= 32;
        }
        if (r_u8(actor + 0xDD))
            t3_80028ec0(event, 24);
        if (r_u8(actor + 0xDE)) {
            t3_80028ec0(event, 22);
            // Index -1 writes only an unread outgoing argument slot
            if (selected != 0xFFFFFFFF)
                flags[selected] |= 128;
        }
        if ((r_u32(r_u32(actor + 0x54) + 0x24) & 0x10000) && r_u8(actor + 0xDA)) {
            t3_80028ec0(event, 14);
            target = ((uint32)r_s16(actor + 0x12) + 1) & 1;
            flags[target] |= 64;
        }
    }
    for (index = 0; index < 2; ++index) {
        event = t3_80075ef0(index);
        if (flags[index] & 64)
            t3_80028ec0(event, 26);
        if (flags[index] & 1)
            t3_80028ec0(event, 4);
        if (flags[index] & 2)
            t3_80028ec0(event, 5);
        if (flags[index] & 4)
            t3_80028ec0(event, 6);
        if (flags[index] & 8)
            t3_80028ec0(event, 8);
        if (flags[index] & 16)
            t3_80028ec0(event, 9);
        if (flags[index] & 32)
            t3_80028ec0(event, 7);
        if (flags[index] & 128)
            t3_80028ec0(event, 11);
    }
    return 0;
}

uint32 t3_8008fc18(void)
{
    FUNCTION_MARKER(0x8008FC18, "SLUS_004.02");
    return (uint32)CdStatus() & 0xFF;
}

uint32 t3_8008f74c(void)
{
    FUNCTION_MARKER(0x8008F74C, "SLUS_004.02");
    return t3_8008fc18() & 0xFF;
}

uint32 t3_80090aa8(void *destination, sint32 words)
{
    FUNCTION_MARKER(0x80090AA8, "SLUS_004.02");
    return CdGetSector(destination, words) != 0;
}

uint32 t3_80090ac8(void *destination, sint32 words)
{
    FUNCTION_MARKER(0x80090AC8, "SLUS_004.02");
    return CdGetSector(destination, words) != 0;
}

uint32 t3_8008fc28(uint8 *result)
{
    uint8 response[8];
    sint32 completion;
    FUNCTION_MARKER(0x8008FC28, "SLUS_004.02");
    completion = CdSync(1, response);
    if (completion != CdlComplete && completion != CdlDiskError)
        return completion == -1 ? 0xFFFFFFFF : 0;
    if (result)
        memcpy(result, response, sizeof(response));
    return (uint32)completion;
}

void t3_80082728(uint32 x, uint32 y)
{
    FUNCTION_MARKER(0x80082728, "SLUS_004.02");
    SetGeomOffset((sint16)(uint16)x, (sint16)(uint16)y);
}

void t3_80082748(uint32 distance)
{
    FUNCTION_MARKER(0x80082748, "SLUS_004.02");
    SetGeomScreen((sint32)(uint16)distance);
}

void t3_80081238(uint32 distance)
{
    FUNCTION_MARKER(0x80081238, "SLUS_004.02");
    t3_80082748(distance);
}

void t3_800817c8(uint32 environment)
{
    FUNCTION_MARKER(0x800817C8, "SLUS_004.02");
    DrawOTag((uint32 *)psx_addr(r_u32(environment + 0x10), 4));
}

uint32 t3_8008672c(void)
{
    FUNCTION_MARKER(0x8008672C, "SLUS_004.02");
    return (uint32)GetVideoMode();
}

void t3_8008af48(void)
{
    FUNCTION_MARKER(0x8008AF48, "SLUS_004.02");
    SpuInit();
}

void t3_80087b28(void)
{
    FUNCTION_MARKER(0x80087B28, "SLUS_004.02");
    SsInitHot();
}

uint32 t3_8008a958(void)
{
    FUNCTION_MARKER(0x8008A958, "SLUS_004.02");
    xport_audio_lock();
    spu_vm_state()->mono_mode = 1;
    xport_audio_unlock();
    return 1;
}

void t3_80087c68(uint32 vblank_counter)
{
    FUNCTION_MARKER(0x80087C68, "SLUS_004.02");
    spu_sequence_start_timer(vblank_counter);
}

void t3_80087e98(void)
{
    FUNCTION_MARKER(0x80087E98, "SLUS_004.02");
    t3_80087c68(1);
}

uint32 t3_8008d6e8(const void *source, uint32 byte_count)
{
    FUNCTION_MARKER(0x8008D6E8, "SLUS_004.02");
    if (byte_count > 0x7EFF0)
        byte_count = 0x7EFF0;
    SpuWrite(source, byte_count);
    return byte_count;
}

uint32 t3_8008c818(void)
{
    SpuReverbControl state;
    FUNCTION_MARKER(0x8008C818, "SLUS_004.02");
    spu_reverb_export(&state);
    return (state.control & 0x80) != 0;
}

void t3_80081c38(uint32 work_base)
{
    FUNCTION_MARKER(0x80081C38, "SLUS_004.02");
    GsSetWorkBase((uint32 *)psx_addr(work_base, 4));
}

void t3_80081088(uint32 distance)
{
    FUNCTION_MARKER(0x80081088, "SLUS_004.02");
    gs_set_clip_distance((sint32)distance);
}

void t3_80081068(uint32 x, uint32 y)
{
    FUNCTION_MARKER(0x80081068, "SLUS_004.02");
    GsSetOrign((sint16)(uint16)x, (sint16)(uint16)y);
}

void t3_80081098(uint32 distance)
{
    FUNCTION_MARKER(0x80081098, "SLUS_004.02");
    GsSetFarClip(distance);
}

uint32 t3_80080f38(uint32 rectangle)
{
    PSX_RECT clip;
    FUNCTION_MARKER(0x80080F38, "SLUS_004.02");
    clip.x = (sint16)r_u16(rectangle);
    clip.y = (sint16)r_u16(rectangle + 2);
    clip.w = (sint16)r_u16(rectangle + 4);
    clip.h = (sint16)r_u16(rectangle + 6);
    GsSetClip2D(&clip);
    return (uint16)clip.h;
}

uint32 t3_8008d888(uint32 in_transfer)
{
    uint32 result;
    FUNCTION_MARKER(0x8008D888, "SLUS_004.02");
    xport_audio_lock();
    result = spu_sequence_vm_transfer_mode(in_transfer);
    xport_audio_unlock();
    return result;
}

uint32 t3_8008d8b0(void)
{
    uint32 busy;
    FUNCTION_MARKER(0x8008D8B0, "SLUS_004.02");
    xport_audio_lock();
    busy = spu_native_transfer_state->completion != 1;
    xport_audio_unlock();
    return busy;
}

uint32 t3_8008fbdc(void)
{
    FUNCTION_MARKER(0x8008FBDC, "SLUS_004.02");
    return (uint32)CdMode() & 0xFF;
}

uint32 t3_8007a418(uint32 text)
{
    const char *begin;
    const char *end;
    XportMemoryRegion region;
    uint32 offset;
    size_t span;
    FUNCTION_MARKER(0x8007A418, "SLUS_004.02");
    if (!text)
        return 0;
    begin = (const char *)xport_guest_cptr(text, 1);
    if (!xport_memory_pointer_identity(begin, 1, &region, &offset))
        abort();
    switch (region) {
    case XPORT_MEMORY_DRAM: span = 0x200000; break;
    case XPORT_MEMORY_SCRATCHPAD: span = 1024; break;
    case XPORT_MEMORY_BIOS: span = 0x80000; break;
    default: abort();
    }
    /* Bound the CRT search to the mapped memory object */
    span -= offset;
    begin = (const char *)xport_guest_cptr(text, span);
    if (!begin)
        abort();
    end = (const char *)memchr(begin, 0, span);
    if (!end)
        abort();
    return (uint32)(end - begin);
}

void t3_80084da8(uint32 mask, uint32 start, uint32 end)
{
    FUNCTION_MARKER(0x80084DA8, "SLUS_004.02");
    StSetMask(mask, start, end);
}

void t3_80082b98(void)
{
    FUNCTION_MARKER(0x80082B98, "SLUS_004.02");
    EnterCriticalSectionPSX();
    StUnSetRing();
    ExitCriticalSection();
}

uint32 t3_800932a0(uint32 owner, sint32 value, uint32 port)
{
    FUNCTION_MARKER(0x800932A0, "SLUS_004.02");
    return (uint32)psx_pad_serial_transfer(owner, value, port, 0, -20);
}

uint32 t3_80093478(uint32 owner, uint32 value, uint32 port)
{
    FUNCTION_MARKER(0x80093478, "SLUS_004.02");
    return (uint32)psx_pad_serial_transfer(owner, (uint8)value, port, 1, -2);
}

uint32 t3_8009373c(uint32 port)
{
    uint32 ready;
    FUNCTION_MARKER(0x8009373C, "SLUS_004.02");
    do {
        ready = psx_pad_serial_ready(port);
    } while (!ready);
    return ready;
}

CdlCB t3_8009268c(CdlCB callback)
{
    FUNCTION_MARKER(0x8009268C, "SLUS_004.02");
    return CdReadyCallback(callback);
}

uint32 t3_800936ac(uint32 port)
{
    FUNCTION_MARKER(0x800936AC, "SLUS_004.02");
    return psx_pad_serial_release_ack(port);
}

void t3_80090914(uint8 *destination, const uint8 *source)
{
    sint32 index;
    FUNCTION_MARKER(0x80090914, "SLUS_004.02");
    if (source) {
        if (destination) {
            for (index = 0; index < 4; ++index) {
                uint8 value = source[index];
                destination[index] = value;
            }
        }
    } else if (destination) {
        destination[0] = 0;
    }
}

void t3_8009095c(uint8 *destination, const uint8 *source)
{
    sint32 index;
    FUNCTION_MARKER(0x8009095C, "SLUS_004.02");
    if (source) {
        if (destination) {
            for (index = 0; index < 8; ++index) {
                uint8 value = source[index];
                destination[index] = value;
            }
        }
    } else if (destination) {
        destination[0] = 0;
    }
}

uint32 t3_80090d88(uint32 command, const void *parameter, void *result)
{
    FUNCTION_MARKER(0x80090D88, "SLUS_004.02");
    return (uint32)CdControlB((uint8)command, (uint8 *)parameter, (uint8 *)result);
}

uint32 t3_8008fc08(void)
{
    FUNCTION_MARKER(0x8008FC08, "SLUS_004.02");
    return (uint8)CdLastStreamCom();
}

uint32 t3_8008e558(uint32 record)
{
    sint32 offset;
    FUNCTION_MARKER(0x8008E558, "SLUS_004.02");
    w_u32(record, 0);
    w_u8(record + 4, 0);
    for (offset = 8; offset >= 5; --offset)
        w_u8(record + offset, 0);
    w_u32(record + 12, 0);
    w_u32(record + 16, 0);
    w_u32(record + 20, 0);
    return record - 1;
}

uint32 t3_80092ccc(PsxInterruptCallback callback)
{
    FUNCTION_MARKER(0x80092CCC, "SLUS_004.02");
    if (!(r_u32(0x1F801074) & 1))
        return 0;
    if (!(r_u32(0x1F801070) & 1))
        return 0;
    if (callback)
        callback();
    return 1;
}

uint32 t3_800942a0(uint32 owner, uint32 port, T3_PAD_NEXT_BYTE next_byte)
{
    uint32 multitap = 0;
    uint32 value;
    uint32 result;
    FUNCTION_MARKER(0x800942A0, "SLUS_004.02");
    if ((r_u8(r_u32(owner + 60)) >> 4) == 8)
        multitap = r_u8(owner + 54) == 0;
    value = (uint8)next_byte(owner, multitap);
    result = t3_80093478(owner, value, port);
    if (result == 0x5A || result == 0 || (sint32)result < 0)
        return result;
    return 0xFFFFFFF7;
}

uint32 t3_800940c8(uint32 owner, uint32 port, PsxPadOwnerCallback prepare, uint32 *prepared)
{
    FUNCTION_MARKER(0x800940C8, "SLUS_004.02");
    *prepared = prepare(owner);
    return t3_800932a0(owner, -2, port);
}

uint32 t3_800941e0(uint32 owner, uint32 port, uint32 prepared, const PsxPadOwnerCallback *prepare, const uint32 *command_mode, uint32 *remaining)
{
    uint32 child;
    uint32 value = 0;
    uint32 result;
    PsxPadOwnerCallback callback;
    FUNCTION_MARKER(0x800941E0, "SLUS_004.02");
    if (prepared) {
        child = r_u32(owner + 12) + 480;
        callback = *prepare;
        callback(child);
        child = r_u32(owner + 12) + 720;
        callback = *prepare;
        callback(child);
    }
    if (!r_u8(owner + 54))
        value = *command_mode;
    result = t3_80093478(owner, value, port);
    if ((sint32)result < 0)
        return result;
    if (!(result & 0xF0))
        return 0xFFFFFFF7;
    value = (result & 15) * 2;
    *remaining = value;
    if (!value)
        *remaining = 32;
    return 0;
}

uint32 t3_80039074(uint32 actor, uint32 opponent)
{
    uint32 record, animation, previous, enabled = 0, changed, offset, index;
    uint32 first[3], transformed[3], middle, difference, counter;
    sint32 threshold, angle, limit, step, sine, cosine, negative;
    uint32 product;
    SVECTOR direction, current_angles;
    VECTOR first_axis, second_axis;
    MATRIX rotation = {0};
    sint16 pitch, yaw, initial_yaw, initial_roll, initial_pitch;
    FUNCTION_MARKER(0x80039074, "SLUS_004.02");
    record = 0x8009E578 + (uint32)r_s16(actor + 18) * 20;
    animation = r_u32(actor + 84);
    if (((r_u32(animation + 36) >> 30) & 1) && t3_8002bb70()) {
        animation = r_u32(actor + 84);
        if (!r_u8(animation + 45)) {
            enabled = 1;
        } else {
            threshold = (sint32)r_u8(animation + 24) - 17;
            if ((sint32)r_u8(animation + 46) >= threshold)
                threshold = r_u8(animation + 46);
            enabled = r_s16(actor + 88) >= threshold;
        }
    }
    previous = r_u8(record + 16);
    changed = enabled != previous && enabled != 0;
    w_u8(record + 16, enabled);
    if (!enabled)
        return previous;
    if (!r_u8(0x80096C58)) {
        t3_8003a34c((const SVECTOR *)psx_addr(0x8001A0F4, 8), (MATRIX *)psx_addr(0x8009E558, 32));
        w_u8(0x80096C58, 1);
    }
    for (index = 0; index < 3; ++index) {
        offset = index * 4;
        middle = r_u32(opponent + 0x94C + offset);
        difference = r_u32(opponent + 0x990 + offset) - middle;
        first[index] = middle + (uint32)((sint32)difference >> 1);
    }
    direction.vx = (sint16)(first[0] - r_u16(actor + 0x990));
    direction.vy = (sint16)(first[1] - r_u16(actor + 0x994));
    direction.vz = (sint16)(first[2] - r_u16(actor + 0x998));
    SetRotMatrix((MATRIX *)psx_addr(actor + 0x938, 32));
    t3_8004b218(0x8001A0BC, first, &direction, transformed);
    product = transformed[1] * transformed[1] + transformed[2] * transformed[2];
    angle = (sint16)(0 - t3_8002a224((sint16)t3_8004aedc(product), (sint16)transformed[0]));
    if (angle >= 4552)
        angle = 4551;
    if (angle < -10922)
        angle = -10922;
    pitch = (sint16)angle;
    yaw = (sint16)t3_8002a224((sint16)transformed[1], (sint16)transformed[2]);
    if (r_s16(actor + 24) == 17)
        yaw = 0;
    else
        yaw = (sint16)t3_8002a224((sint16)transformed[1], (sint16)transformed[2]);
    product = first[0] * first[0] + first[2] * first[2];
    angle = (sint16)t3_8002a224((sint16)t3_8004aedc(product), (sint16)first[1]);
    if (angle + 0x4000 < 2731)
        limit = 14563;
    else if (angle + 0x4000 < 14563)
        limit = 14563 - 9102 * (angle + 13654) / 11833;
    else
        limit = 5461;
    if (yaw > limit)
        yaw = (sint16)limit;
    if (yaw < -limit)
        yaw = (sint16)-limit;
    if (changed || ((sint32)transformed[1] < 0 ? (sint32)(0 - transformed[1]) >= 400 : (sint32)transformed[1] >= 400) ||
        ((sint32)transformed[2] < 0 ? (sint32)(0 - transformed[2]) >= 300 : (sint32)transformed[2] >= 300)) {
        w_u8(record + 18, 0);
        w_u32(record + 8, (uint32)(uint16)pitch << 16);
        w_u16(record + 12, yaw);
    } else {
        counter = (uint8)(r_u8(record + 18) + 1);
        w_u8(record + 18, counter);
        if (counter >= 60) {
            w_u8(record + 18, 0);
            w_u32(record + 8, (uint32)(uint16)pitch << 16);
            w_u16(record + 12, yaw);
        }
    }
    if (r_u8(record + 17)) {
        w_u32(record + 8, (uint32)(uint16)pitch << 16);
        w_u16(record + 12, yaw);
        w_u32(record, r_u32(record + 8));
        w_u16(record + 4, r_u16(record + 12));
        w_u8(record + 17, 0);
    } else if (changed) {
        SetRotMatrix((MATRIX *)psx_addr(actor + 0xFB4, 32));
        ApplyRotMatrix((SVECTOR *)psx_addr(0x8001A0FC, 8), &first_axis);
        ApplyRotMatrix((SVECTOR *)psx_addr(0x8001A104, 8), &second_axis);
        initial_yaw = (sint16)t3_8002a224((sint16)first_axis.vy, (sint16)first_axis.vz);
        negative = -(sint32)initial_yaw;
        if (negative < 0)
            negative += 15;
        offset = (negative >> 3) & 0x1FFE;
        sine = r_s16(0x8001E8C4 + offset);
        cosine = r_s16(0x8001F0C4 + offset);
        product = (uint32)first_axis.vy * cosine - (uint32)first_axis.vz * sine;
        middle = (uint32)first_axis.vy * sine + (uint32)first_axis.vz * cosine;
        first_axis.vy = (sint32)product / 4096;
        first_axis.vz = (sint32)middle / 4096;
        product = (uint32)second_axis.vy * cosine - (uint32)second_axis.vz * sine;
        middle = (uint32)second_axis.vy * sine + (uint32)second_axis.vz * cosine;
        second_axis.vy = (sint32)product / 4096;
        second_axis.vz = (sint32)middle / 4096;
        initial_roll = (sint16)(t3_8002a224((sint16)first_axis.vx, (sint16)first_axis.vy) - 0x4000);
        negative = -(sint32)initial_roll;
        if (negative < 0)
            negative += 15;
        offset = (negative >> 3) & 0x1FFE;
        sine = r_s16(0x8001E8C4 + offset);
        cosine = r_s16(0x8001F0C4 + offset);
        product = (uint32)second_axis.vx * cosine - (uint32)second_axis.vy * sine;
        middle = (uint32)second_axis.vx * sine + (uint32)second_axis.vy * cosine;
        second_axis.vx = (sint32)product / 4096;
        second_axis.vy = (sint32)middle / 4096;
        initial_pitch = (sint16)(t3_8002a224((sint16)second_axis.vz, (sint16)second_axis.vx) - 0x4000);
        w_u16(record, initial_pitch);
        w_u16(record + 2, initial_roll);
        w_u16(record + 4, initial_yaw);
    } else {
        difference = (uint32)r_s16(record + 10) - (uint32)r_s16(record + 2);
        step = (sint32)difference - (sint32)difference / 2;
        if (step > 1820)
            step = 1820;
        if (step < -1820)
            step = -1820;
        w_u16(record + 2, r_u16(record + 2) + step);
        difference = (uint32)r_s16(record + 12) - (uint32)r_s16(record + 4);
        step = (sint32)difference - (sint32)difference / 2;
        if (step > 1820)
            step = 1820;
        if (step < -1820)
            step = -1820;
        w_u16(record + 4, r_u16(record + 4) + step);
        w_u16(record, r_u16(record + 8));
    }
    current_angles.vx = r_s16(record);
    current_angles.vy = r_s16(record + 2);
    current_angles.vz = r_s16(record + 4);
    t3_8003a34c(&current_angles, &rotation);
    MulMatrix0((MATRIX *)psx_addr(0x8009E558, 32), &rotation, (MATRIX *)psx_addr(actor + 0xFB4, 32));
    return actor + 0xFB4;
}
