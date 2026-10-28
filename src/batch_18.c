#include "game_functions.h"
#include <stdarg.h>

uint32 t3_bns_01_800b848c(uint32 actor)
{
    FUNCTION_MARKER(0x800B848C, "SLUS_004.02-BNS-logical-01");
    return t3_bns_01_800b84cc(actor, 1);
}

uint32 t3_bns_01_800b84ac(uint32 actor)
{
    FUNCTION_MARKER(0x800B84AC, "SLUS_004.02-BNS-logical-01");
    return t3_bns_01_800b84cc(actor, 0);
}

uint32 t3_bns_01_800b84cc(uint32 actor, uint32 forward)
{
    uint32 animation, input;

    FUNCTION_MARKER(0x800B84CC, "SLUS_004.02-BNS-logical-01");
    if (r_u32(actor + 0x1A4))
        return 0;
    if (!r_u8(actor + 0x8A))
        return 0;
    animation = r_u32(actor + 0x54);
    if (!r_u8(animation + 0x19))
        return 0;
    if (!t3_bns_01_800b8574(actor))
        return 0;
    input = forward ? 0x60 : 0x90;
    if (!(r_u32(0x800AFA4C) & 1))
        input = 0;
    w_u16(actor + 0x408, input);
    return 0;
}

uint32 t3_bns_01_800b8574(uint32 actor)
{
    uint32 animation, entry;

    FUNCTION_MARKER(0x800B8574, "SLUS_004.02-BNS-logical-01");
    animation = r_u32(actor + 0x54);
    entry = r_u32(animation + 0xC);
    while (r_u16(entry) != 0xC000) {
        if (r_u8(entry + 8) == 28)
            return 1;
        entry += 12;
    }
    return 0;
}

uint32 t3_bns_01_800b85cc(uint32 actor, uint32 opponent)
{
    uint32 flags, animation, input;
    sint32 frame;

    FUNCTION_MARKER(0x800B85CC, "SLUS_004.02-BNS-logical-01");
    flags = r_u16(opponent + 0x64);
    if (!(flags & 0x18))
        return 0;
    animation = r_u32(opponent + 0x54);
    frame = r_s16(opponent + 0x58);
    if ((sint32)r_u8(animation + 0x2E) < frame)
        return 0;
    input = flags & 0x10 ? 0x8000 : 0xC000;
    w_u16(actor + 0x408, input);
    return 1;
}

void t3_bns_01_800b40e4(uint32 actor, uint32 opponent)
{
    uint32 control, result, destination;

    FUNCTION_MARKER(0x800B40E4, "SLUS_004.02-BNS-logical-01");
    control = r_u32(0x800B8A2C);
    if (!r_s8(control + 0x83))
        return;
    result = t3_bns_01_800b3040(actor, opponent, opponent);
    control = r_u32(0x800B8A2C);
    destination = control + (uint32)r_s16(actor + 0x12) + 0x88;
    w_u8(destination, result);
    result = t3_bns_01_800b3040(opponent, actor, opponent);
    control = r_u32(0x800B8A2C);
    destination = control + (uint32)r_s16(opponent + 0x12) + 0x88;
    w_u8(destination, result);
}

uint32 t3_bns_01_800b3040(uint32 actor, uint32 opponent, uint32 return_entry)
{
    uint32 animation, entry, opcode, kind, first_frame;
    sint32 frame;

    FUNCTION_MARKER(0x800B3040, "SLUS_004.02-BNS-logical-01");
    animation = r_u32(actor + 0x54);
    entry = r_u32(animation + 0xC);
    frame = r_s16(actor + 0x58);
    opcode = r_u16(entry);
    while (opcode != 0xC000) {
        if (opcode == 0xC00D) {
            entry = return_entry + 12;
            opcode = r_u16(entry);
            continue;
        }
        if (opcode == 0xC00C) {
            opcode = r_u16(entry + 6);
            return_entry = entry;
            entry = 0x80017C78 + opcode * 12;
        }
        if (frame >= (sint32)r_u8(entry + 9)
            && (sint32)r_u8(entry + 10) >= frame
            && t3_8002dd98(actor, opponent, r_u8(entry + 2))) {
            kind = r_u8(entry + 3);
            if (kind < 0x45
                && (r_u16(entry) != 16 || kind || r_u8(entry + 8) != 2)) {
                kind = r_u8(entry + 3);
                if (kind - 0x3A < 3 || t3_8002e0bc(entry, actor, opponent))
                    break;
            }
        }
        entry += 12;
        opcode = r_u16(entry);
    }
    if (r_u16(entry) != 0xC000)
        return 1;
    first_frame = r_u8(entry + 9);
    frame = r_s16(actor + 0x58);
    if (frame < (sint32)first_frame) {
        if (frame < (sint32)first_frame - 5)
            return r_u16(entry) != 0xC000;
    }
    else if ((sint32)r_u8(entry + 10) < frame)
        return r_u16(entry) != 0xC000;
    if (r_s16(actor + 0x6E) <= 0)
        return r_u16(entry) != 0xC000;
    animation = t3_8002d178(actor, (uint32)r_s16(entry + 6));
    entry = r_u32(animation + 0xC);
    opcode = r_u16(entry);
    while (opcode != 0xC000) {
        if (opcode == 0xC00D) {
            entry = return_entry + 12;
            opcode = r_u16(entry);
            continue;
        }
        if (opcode == 0xC00C) {
            opcode = r_u16(entry + 6);
            return_entry = entry;
            entry = 0x80017C78 + opcode * 12;
        }
        if (r_u8(entry + 9) <= 1 && r_u8(entry + 10) >= 1
            && t3_8002dd98(actor, opponent, r_u8(entry + 2))) {
            kind = r_u8(entry + 3);
            if (kind < 0x45
                && (r_u16(entry) != 16 || kind || r_u8(entry + 8) != 2)
                && t3_8002e0bc(entry, actor, opponent))
                break;
        }
        entry += 12;
        opcode = r_u16(entry);
    }
    return r_u16(entry) != 0xC000;
}

uint32 t3_8002dd98(uint32 actor, uint32 opponent, uint32 type)
{
    sint32 selector = (sint16)(uint16)type;

    FUNCTION_MARKER(0x8002DD98, "SLUS_004.02");
    if (!selector)
        return 1;
    if (selector >= 93)
        return r_s16(actor + 0x18) != selector - 93;
    if (selector >= 70)
        return r_s16(actor + 0x18) == selector - 70;
    if (selector >= 47)
        return r_s16(opponent + 0x16) == selector - 47;
    if (selector < 24)
        return r_s16(actor + 0x16) == selector - 1;
    return r_s16(actor + 0x16) != selector - 24;
}

void t3_bns_01_800b5290(void)
{
    uint32 control;
    sint32 mode;

    FUNCTION_MARKER(0x800B5290, "SLUS_004.02-BNS-logical-01");
    control = r_u32(0x800B8A2C);
    if (r_u8(control + 0x75)) {
        t3_bns_01_800b5318();
        return;
    }
    mode = r_s8(control + 0x77);
    if (!mode)
        t3_bns_01_800b545c();
    else if (mode == 1)
        t3_bns_01_800b5918();
    else if (mode == 2)
        t3_bns_01_800b5d18();
}

void t3_bns_01_800b4728(void)
{
    uint32 control;

    FUNCTION_MARKER(0x800B4728, "SLUS_004.02-BNS-logical-01");
    control = r_u32(0x800B8A2C);
    if (r_u8(control + 0x75))
        t3_bns_01_800b4770();
    else
        t3_bns_01_800b4930();
}

void t3_bns_01_800b6764(void)
{
    uint32 control, height, inner_xy, inner_size, outer_xy, outer_size;
    uint32 ordering_entry, packet, bottom;
    PSX_RECT rectangle;

    FUNCTION_MARKER(0x800B6764, "SLUS_004.02-BNS-logical-01");
    control = r_u32(0x800B8A2C);
    if (r_u8(control + 0x75)) {
        height = 192;
        inner_xy = 0x00280008;
        inner_size = 0x00C000AC;
        outer_xy = 0x00260007;
        outer_size = 0x00C400AE;
    }
    else {
        if (r_u8(control + 0x77) >= 3)
            return;
        height = 316;
        inner_xy = 0x00280040;
        inner_size = 0x013C00F0;
        outer_xy = 0x0026003F;
        outer_size = 0x014000F2;
    }
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    packet = t3_8004da10(ordering_entry, packet, 0x60301000, inner_xy, inner_size);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering_entry, packet, 0x60E0E0E0, outer_xy, outer_size);
    w_u32(0x800ADF64, packet);
    bottom = height + 42;
    rectangle.x = (sint16)r_u16(0x800AE230);
    rectangle.h = (sint16)(uint16)(r_u16(0x800AE232) + r_u16(0x800AE236) - bottom);
    rectangle.w = (sint16)r_u16(0x800AE234);
    rectangle.y = (sint16)bottom;
    ordering_entry = r_u32(r_u32(0x800A8C54) + 4) + 8;
    packet = r_u32(0x800ADF64);
    packet = t3_8004db28(ordering_entry, packet, &rectangle);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004db28(ordering_entry, packet, (const PSX_RECT *)psx_addr(0x800AE230, 8));
    ordering_entry = r_u32(r_u32(0x800A8C54) + 0x10);
    w_u32(0x800ADF64, packet);
    packet = t3_8004db28(ordering_entry, packet, &rectangle);
    w_u32(0x800ADF64, packet);
}

void t3_bns_01_800b44a8(void)
{
    uint32 control, value, index, offset;
    sint32 previous, mode;

    FUNCTION_MARKER(0x800B44A8, "SLUS_004.02-BNS-logical-01");
    control = r_u32(0x800B8A2C);
    if (!r_u8(control + 0x75)) {
        mode = r_s8(control + 0x77);
        if (mode < 0 || mode > 2)
            return;
        offset = 0x78 + (uint32)mode;
        value = (uint32)t3_bns_01_800b468c((uint32)r_s8(control + offset), 7);
        control = r_u32(0x800B8A2C);
        w_u8(control + offset, value);
        return;
    }
    previous = r_s8(control + 0x77);
    value = (uint32)t3_bns_01_800b468c((uint32)previous, 4);
    control = r_u32(0x800B8A2C);
    w_u8(control + 0x77, value);
    control = r_u32(0x800B8A2C);
    mode = r_s8(control + 0x77);
    if (mode == previous)
        return;
    switch (mode) {
    case 0:
        index = r_u8(control + 0x84);
        value = r_u8(control + 0x84);
        w_u8(control + 0x9D, 1);
        w_u8(control + 0x87, 0);
        w_u8(control + 0x9F, 0);
        w_u8(control + 0x86, 0);
        w_u8(control + 0x7C, 0);
        w_u8(control + 0x7B, value);
        t3_bns_01_800b7c24(index);
        t3_bns_01_800b744c(r_u8(control + 0x84));
        w_u8(control + 0xA3, 1);
        break;
    case 1:
        index = r_u8(control + 0x84);
        value = r_u8(control + 0x84);
        w_u8(control + 0x9E, 0);
        w_u8(control + 0x81, 0);
        w_u8(control + 0x87, 0);
        w_u8(control + 0x9F, 0);
        w_u8(control + 0x86, 0);
        w_u8(control + 0x7C, 0);
        w_u8(control + 0x7B, value);
        t3_bns_01_800b7c24(index);
        t3_bns_01_800b744c(r_u8(control + 0x84));
        w_u8(control + 0xA3, (uint32)mode);
        break;
    case 2:
        w_u8(control + 0x7B, r_u8(control + 0x84));
        index = (uint32)r_s8(control + 0x7B);
        w_u8(control + 0x9E, 0);
        w_u8(control + 0x81, 0);
        w_u8(control + 0xA0, 1);
        w_u8(control + 0x87, 0);
        w_u8(control + 0x9F, 0);
        w_u8(control + 0x86, 0);
        w_u8(control + 0x7D, 0);
        w_u8(control + 0x7C, 0);
        t3_bns_01_800b7c24(index);
        t3_bns_01_800b744c((uint32)r_s8(control + 0x7B));
        w_u8(control + 0xA3, (uint32)mode);
        break;
    }
}

sint32 t3_bns_01_800b468c(uint32 value, sint32 maximum)
{
    uint32 buttons;
    sint32 selected;

    FUNCTION_MARKER(0x800B468C, "SLUS_004.02-BNS-logical-01");
    buttons = r_u16(r_u32(0x800B8A2C) + 4);
    if (buttons & 0x4000) {
        ++value;
        t3_800753bc(0, 0x546C, 0);
    }
    else if (buttons & 0x1000) {
        --value;
        t3_800753bc(0, 0x546C, 0);
    }
    selected = (sint8)(uint8)value;
    if (selected < 0)
        value = (uint32)maximum;
    else if (maximum < selected)
        value = 0;
    return (sint8)(uint8)value;
}

void t3_bns_01_800b7c24(uint32 index)
{
    uint32 value;

    FUNCTION_MARKER(0x800B7C24, "SLUS_004.02-BNS-logical-01");
    if (r_s32(0x800B8D48 + (index << 2)) < 0) {
        value = r_u32(0x800B8CDC);
        w_u32(0x800B8CD8, 0);
        w_u32(0x800B8CDC, value & 0x8000FFFF);
        w_u8(0x800B8CDC, 0);
        w_u8(0x800B8CDD, 0);
    }
    else {
        value = r_u32(0x800B8C10);
        w_u32(0x800B8CD8, 0);
        w_u32(0x800B8CDC, value);
    }
    w_u32(0x800B8CE0, 0);
    t3_bns_01_800b7cb0();
}

uint32 t3_bns_01_800b7cb0(void)
{
    uint32 entry = 0x800B8CE4;
    uint32 index;

    FUNCTION_MARKER(0x800B7CB0, "SLUS_004.02-BNS-logical-01");
    for (index = 0; index < 50; ++index) {
        w_u16(entry, 0);
        entry += 2;
    }
    return entry;
}

sint32 t3_bns_01_800b7994(uint32 index, uint32 sequence)
{
    uint32 actor, table, descriptor, source, destination, row, value;
    sint32 character, count;

    FUNCTION_MARKER(0x800B7994, "SLUS_004.02-BNS-logical-01");
    actor = 0x800A9228 + index * 6284;
    character = r_s16(actor + 0x16);
    table = 0x800B25F8 + ((uint32)character << 3);
    if (!r_u32(table + 4))
        return 0;
    descriptor = r_u32(table) + sequence * 12;
    source = r_u32(descriptor);
    (void)r_u32(descriptor + 4);
    count = (sint16)(uint16)r_u32(descriptor + 8);
    if (!count) {
        destination = 0x800B8C10;
        for (row = 0; row < 50; ++row) {
            w_u8(destination, 0);
            w_u8(destination + 1, 0);
            value = r_u32(destination);
            w_u32(destination, value & 0x8000FFFF);
            destination += 4;
        }
        w_u32(0x800B8D48 + (index << 2), 0);
        w_u32(0x800B8B4C + (index << 2), 30);
        return 0;
    }
    w_u32(0x800B8D48 + (index << 2), (uint32)(count - 1));
    destination = 0x800B8C10;
    for (row = 0; (sint32)row < count; ++row) {
        value = r_u32(source);
        source += 4;
        w_u32(destination, value);
        destination += 4;
    }
    return count;
}

void t3_bns_01_800b3328(uint32 actor, uint32 opponent)
{
    uint32 control, value, buttons, current, other, record, model, row, animation;
    uint32 destination, point, screen, amount;
    sint32 mode, counter, previous, delta, first_damage;
    SVECTOR origin = { 0 };

    FUNCTION_MARKER(0x800B3328, "SLUS_004.02-BNS-logical-01");
    t3_bns_01_800b4168(actor, opponent);
    control = r_u32(0x800B8A2C);
    if (!r_u8(control + 0x87)) {
        w_u8(control + 0xA1, 15);
        w_u8(r_u32(0x800B8A2C) + 0xA2, 0);
        w_u8(r_u32(0x800B8A2C) + 0xA3, 1);
        w_u8(r_u32(0x800B8A2C) + 0xA4, 0);
        if (t3_bns_01_800b732c((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B)))
            t3_bns_01_800b744c((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
        t3_bns_01_800b74cc((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
    }
    else {
        switch (r_s8(control + 0x82)) {
        case 0:
            control = r_u32(0x800B8A2C);
            mode = r_s8(control + 0x77);
            if (mode == 2) {
                w_u8(control + 0xA2, 1);
                t3_bns_01_800b7c24((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
                t3_bns_01_800b8144(r_u32(0x800B8A2C));
                control = r_u32(0x800B8A2C);
                value = (uint32)t3_bns_01_800b7994((uint32)r_s8(control + 0x7B), (uint32)r_s8(control + 0x7C));
                w_u8(r_u32(0x800B8A2C) + 0xA1, value);
                if (!(value & 0xFF))
                    w_u8(r_u32(0x800B8A2C) + 0xA1, 15);
                w_u8(r_u32(0x800B8A2C) + 0x82, 1);
            }
            else if (!mode && r_u8(control + 0x87))
                w_u8(control + 0x82, 1);
            break;
        case 1:
            control = r_u32(0x800B8A2C);
            value = r_u8(control + 0xA4);
            if (value)
                w_u8(control + 0xA4, value - 1);
            else {
                w_u8(control + 0xA4, 0);
                w_u8(r_u32(0x800B8A2C) + 0xA3, 2);
            }
            control = r_u32(0x800B8A2C);
            mode = r_s8(control + 0x77);
            if (mode == 2 && t3_bns_01_800b7ae8(control)) {
                control = r_u32(0x800B8A2C);
                if (r_u16(control + 2) & 0x100) {
                    value = (uint32)r_s8(control + 0x7B);
                    t3_8002c050(0x800A9228 + value * 6284);
                    t3_8002bc70((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B), 2);
                    w_u8(r_u32(0x800B8A2C) + 0xA3, 4);
                    w_u8(r_u32(0x800B8A2C) + 0xA2, (uint32)mode);
                    w_u8(r_u32(0x800B8A2C) + 0x82, 4);
                    break;
                }
            }
            else
                control = r_u32(0x800B8A2C);
            if (r_s8(control + 0x77) || !r_u8(control + 0x87))
                break;
            if (!(r_u16(control + 2) & 0x100) && !r_u8(control + 0xA8))
                break;
            buttons = r_u16(control);
            if (buttons & 0x4000) {
                t3_bns_01_800b744c((uint32)r_s8(control + 0x7B));
                w_u8(r_u32(0x800B8A2C) + 0xA2, 2);
                w_u8(r_u32(0x800B8A2C) + 0xA3, 1);
                w_u8(r_u32(0x800B8A2C) + 0x82, 2);
                break;
            }
            if (buttons & 0xA000)
                w_u8(control + 0xA8, 1);
            value = (uint32)r_s8(r_u32(0x800B8A2C) + 0x7B);
            t3_8002c050(0x800A9228 + value * 6284);
            t3_bns_01_800b7c24((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
            t3_8002bc70((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B), 2);
            w_u8(r_u32(0x800B8A2C) + 0xA2, 2);
            w_u8(r_u32(0x800B8A2C) + 0xA3, 1);
            w_u8(r_u32(0x800B8A2C) + 0x82, 6);
            break;
        case 2:
            control = r_u32(0x800B8A2C);
            if (!(r_u16(control + 8) & 0xF0FF))
                break;
            w_u8(control + 0x82, r_u8(control + 0x82) + 1);
            /* Fall through */
        case 3:
            t3_bns_01_800b74cc((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
            control = r_u32(0x800B8A2C);
            if (!(r_u16(control + 2) & 0x100)
                && t3_bns_01_800b7908((uint32)r_s8(control + 0x7B)) != 49)
                break;
            t3_bns_01_800b7cd4((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
            w_u8(r_u32(0x800B8A2C) + 0x82, 1);
            break;
        case 4:
            value = (uint32)r_s8(r_u32(0x800B8A2C) + 0x7B);
            if (!t3_bns_01_800b82e4(0x800A9228 + value * 6284))
                break;
            value = (uint32)r_s8(r_u32(0x800B8A2C) + 0x7B);
            if (t3_bns_01_800b837c(0x800A9228 + value * 6284))
                w_u8(r_u32(0x800B8A2C) + 0x82, 5);
            break;
        case 5:
            if (!t3_bns_01_800b7d84((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B))) {
                w_u8(r_u32(0x800B8A2C) + 0xA4, 96);
                w_u8(r_u32(0x800B8A2C) + 0xA3, 3);
                w_u8(r_u32(0x800B8A2C) + 0x82, 0);
                break;
            }
            if (t3_bns_01_800b80d0(r_u32(0x800B8A2C))) {
                w_u8(r_u32(0x800B8A2C) + 0xA3, 2);
                w_u8(r_u32(0x800B8A2C) + 0x82, 0);
                break;
            }
            t3_bns_01_800b7ef0((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
            break;
        case 6:
            if (!t3_bns_01_800b7d84((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B)))
                w_u8(r_u32(0x800B8A2C) + 0x82, 7);
            else
                t3_bns_01_800b7ef0((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
            break;
        case 7:
            value = t3_8002d204();
            counter = r_s16(actor + 0xA0);
            if (counter == (sint16)(uint16)value
                || counter == (sint32)r_u16(r_u32(actor + 0x54) + 0x10)) {
                w_u8(r_u32(0x800B8A2C) + 0xA2, 1);
                w_u8(r_u32(0x800B8A2C) + 0x82, 0);
                control = r_u32(0x800B8A2C);
                if (r_u16(control) & 0x100)
                    w_u8(control + 0xA8, 0);
            }
            else
                t3_bns_01_800b7ef0((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
            break;
        case 9:
            if (!r_u8(r_u32(actor + 0x54) + 0x2D) || r_s16(actor + 0xA2) == 3)
                w_u8(r_u32(0x800B8A2C) + 0x82, 0);
            break;
        }
    }
    control = r_u32(0x800B8A2C);
    value = r_u16(actor + 0x408);
    buttons = r_u16(control + 0xE);
    w_u16(control + 0xC, value & (value ^ buttons));
    value = r_u16(actor + 0x408);
    amount = r_u8(control + 0xA2);
    w_u16(control + 0xE, value);
    if (amount && (r_s8(control + 0x77) || !r_u8(control + 0x87)))
        t3_bns_01_800b61bc((uint32)r_s16(actor + 0x12), (uint32)r_s16(control + 0xC));
    control = r_u32(0x800B8A2C);
    value = r_u8(control + 0x9E) != 0;
    w_u32(0x800954C0 + ((uint32)r_s16(opponent + 0x12) << 2), value);
    current = actor;
    for (row = 0; row < 2; ++row) {
        value = t3_bns_01_800b42fc(actor, opponent);
        control = r_u32(0x800B8A2C);
        w_u8(control + (uint32)r_s16(current + 0x12) + 0xAA, value);
        current = opponent;
    }
    current = actor;
    other = opponent;
    for (row = 0; row < 2; ++row) {
        value = (uint32)r_s16(current + 0x12);
        record = r_u32(0x800B8A2C) + (value << 5) + 0x14;
        counter = r_s16(current + 0xCC);
        model = r_u32(current + 0x194);
        if (counter)
            w_u16(record + 0x12, r_u16(record + 0x12) + 1);
        if (r_u8(other + 0xDA))
            w_u16(record + 0x12, 0);
        if (r_s16(other + 0x74) <= 0)
            w_u8(record + 0x1C, 0);
        if (r_u8(other + 0xDA)) {
            w_u8(record + 0x1F, r_u8(record + 0x1E));
            animation = r_u32(other + 0x54);
            w_u8(record + 0x1E, (r_u32(animation + 0x24) >> 13) & 1);
            counter = r_s16(other + 0x16);
            if (counter == 4) {
                mode = r_s16(other + 0xA0);
                if (mode == 0x12D || mode == 0x131 || mode == 0x136 || mode == 0x484)
                    w_u8(record + 0x1E, 1);
                counter = r_s16(other + 0x16);
            }
            if (counter == 7 && (uint32)(r_u16(other + 0xA0) - 0x5A1) < 3)
                w_u8(record + 0x1E, 1);
            if (r_s16(other + 0x16) == 12 && (uint32)(r_u16(other + 0xA0) - 0x832) < 3)
                w_u8(record + 0x1E, 1);
        }
        buttons = r_u8(current + 0xD0);
        if (buttons || r_s16(current + 0xCC)) {
            if (r_s16(current + 0x74) < 0) {
                if (r_s16(current + 0xCC)) {
                    if (r_s8(record + 0x1C) != r_s16(other + 0x74)) {
                        if (r_s16(other + 0x16) != 3 || r_s16(other + 0xA0) != 0xB82)
                            w_u8(record + 0x14, r_u8(record + 0x14) + 1);
                        w_u8(record + 0x1C, r_u8(other + 0x74));
                    }
                }
                else if (buttons)
                    w_u8(record + 0x14, r_u8(record + 0x14) + 1);
            }
            else if (r_u16(current + 0x80) || (r_u8(record + 0x1E) && r_u8(record + 0x1F)))
                w_u8(record + 0x14, r_u8(record + 0x14) + 1);
            else
                w_u8(record + 0x14, 1);
        }
        else if (r_u8(record + 0x1E) && r_u8(record + 0x1F)) {
            if (r_u8(other + 0xD2))
                w_u8(record + 0x14, 0);
        }
        else if (!r_u8(current + 0x87) && !r_u16(current + 0x80))
            w_u8(record + 0x14, 0);
        if (r_s8(record + 0x14) >= 100) {
            w_u8(record + 0x14, 0);
            w_u16(record + 0x10, 90);
        }
        counter = r_s8(record + 0x14);
        previous = r_s8(record + 0x17);
        value = r_u8(record + 0x14);
        w_u8(record + 0x15, 0);
        w_u8(record + 0x16, 0);
        if (counter != previous) {
            if (previous < counter) {
                w_u8(record + 0x15, value);
                w_u8(record + 0x1D, value);
            }
            if (!r_s8(record + 0x14))
                w_u8(record + 0x16, r_u8(record + 0x17));
        }
        if (r_s8(record + 0x14) >= 2) {
            if (r_s8(record + 0x15))
                w_u16(record + 0x10, 90);
        }
        if (r_s8(record + 0x14) < 2 && r_s16(record + 0x12) >= 2)
            w_u16(record + 0x10, 90);
        counter = r_s16(record + 0x10);
        value = r_u16(record + 0x10);
        if (counter > 0)
            w_u16(record + 0x10, value - 1);
        w_u8(record + 0x17, r_u8(record + 0x14));
        if (r_u8(current + 0xD0)) {
            w_u8(record + 0x1A, r_u8(current + 0xD3) != 0);
            w_u8(record + 0x1B, r_u8(current + 0xD4) != 0);
        }
        if (r_s16(current + 0x74) < 0) {
            w_u8(record + 0x1A, 0);
            w_u8(record + 0x1B, 0);
        }
        if (r_u8(current + 0xCE) || r_u8(current + 0xD0) || r_s16(current + 0xCC)) {
            delta = r_s16(current + 0xCC);
            first_damage = delta;
            if (!delta) {
                first_damage = 0;
                if (r_u8(current + 0xD0)) {
                    first_damage = r_s16(model + 0x1C);
                    delta = r_s16(model + 0x1E);
                }
            }
            counter = r_s8(record + 0x14);
            w_u16(record + 0xC, (uint32)first_damage);
            w_u16(record + 0xE, (uint32)delta);
            if (counter < 2 && r_s16(record + 0x12) < 2)
                w_u32(record + 4, (uint32)delta);
            else {
                value = r_u32(record + 4) + (uint32)delta;
                w_u32(record + 4, value);
                w_u32(record + 8, value);
            }
            if (r_s32(record + 4) >= 1000) {
                w_u32(record + 4, 0);
                w_u32(record + 8, 999);
            }
            if (r_s32(record + 4) < -999) {
                w_u32(record + 4, 0);
                w_u32(record + 8, (uint32)-999);
            }
            delta = r_s16(current + 0xCC);
            if (delta <= 0)
                delta = r_s16(model + 0x1E);
        }
        else
            delta = r_s16(model + 0x1E);
        w_u32(record, r_u32(record) + (uint32)delta);
        current = opponent;
        other = actor;
    }
    destination = r_u32(0x800B8A2C) + 0x56;
    for (row = 0; row < 4; ++row) {
        value = r_u16(destination) - 1;
        w_u16(destination, value);
        if ((sint16)(uint16)value < 0)
            w_u16(destination, 0);
        destination += 8;
    }
    if (!r_u8(opponent + 0xD0))
        return;
    control = r_u32(0x800B8A2C);
    w_u8(control + 0x74, r_u8(control + 0x74) + 1);
    control = r_u32(0x800B8A2C);
    if (r_u8(control + 0x74) >= 4)
        w_u8(control + 0x74, 0);
    control = r_u32(0x800B8A2C);
    animation = r_u32(actor + 0x54);
    value = r_u8(control + 0x74);
    animation = r_u32(animation + 0x28);
    destination = control + value * 8 + 0x54;
    if (r_u8(animation) >= 24) {
        model = r_u32(opponent + 0x194);
        point = r_u32(0x80096FB8 + ((uint32)r_s16(model + 0x18) << 2));
        point = opponent + point * 68;
        t3_8004adc4(r_u32(point + 0x908), r_u32(point + 0x90C), r_u32(point + 0x910));
    }
    else
        t3_8004adc4(r_u32(actor + 0x1B8), r_u32(actor + 0x1BC), r_u32(actor + 0x1C0));
    t3_80036ad4(&origin, &screen);
    value = r_u16(actor + 0x64);
    w_u16(destination + 6, screen >> 16);
    w_u16(destination + 4, screen);
    w_u16(destination + 2, 60);
    w_u16(destination, value);
}

uint32 t3_bns_01_800b7ae8(uint32 control)
{
    uint32 actor, table;
    sint32 index, character;

    FUNCTION_MARKER(0x800B7AE8, "SLUS_004.02-BNS-logical-01");
    index = r_s8(control + 0x7B);
    actor = 0x800A9228 + (uint32)index * 6284;
    character = r_s16(actor + 0x16);
    table = 0x800B25F8 + ((uint32)character << 3);
    return r_u32(table + 4);
}

uint32 t3_bns_01_800b7908(uint32 index)
{
    FUNCTION_MARKER(0x800B7908, "SLUS_004.02-BNS-logical-01");
    return r_u32(0x800B8D48 + (index << 2));
}

uint32 t3_bns_01_800b837c(uint32 actor)
{
    uint32 selected;
    sint32 action;

    FUNCTION_MARKER(0x800B837C, "SLUS_004.02-BNS-logical-01");
    if (r_s16(actor + 0x32)) {
        selected = r_u32(actor + 0x1A4);
        w_u16(actor + 0x408, selected ? 0 : 0x8000);
        return 0;
    }
    selected = t3_8002d204();
    action = r_s16(actor + 0xA0);
    if (action == (sint16)(uint16)selected || action == 3 || action == 4 || action == 5
        || action == (sint32)r_u16(r_u32(actor + 0x54) + 0x10)) {
        w_u16(actor + 0x408, 0);
        return 1;
    }
    return 0;
}

uint32 t3_bns_01_800b732c(uint32 index)
{
    uint32 actor, input, animation, timer, value;
    sint32 action;

    FUNCTION_MARKER(0x800B732C, "SLUS_004.02-BNS-logical-01");
    actor = 0x800A9228 + index * 6284;
    input = r_u16(0x800A8C48 + (index << 1));
    animation = r_u32(actor + 0x54);
    action = r_s16(actor + 0xA0);
    input = (input & 0xF0F0) != 0;
    timer = 0x800B8B4C + (index << 2);
    if (action == (sint32)r_u16(animation + 0x10))
        w_u32(timer, r_u32(timer) - 1);
    if (!input && (!r_u32(actor + 0x1A4) || r_s16(actor + 0x19A) == 4
        || r_s16(actor + 0xA0) == r_s16(actor + 0x1A0)))
        w_u32(timer, r_u32(timer) - 1);
    if (r_s32(timer) < 0)
        w_u32(timer, 0);
    if (!input)
        return 0;
    value = r_u32(timer);
    w_u32(timer, 30);
    return value == 0;
}

void t3_bns_01_800b74cc(uint32 index)
{
    uint32 position, input, entry, packed, word, actor, flags;
    sint32 count;

    FUNCTION_MARKER(0x800B74CC, "SLUS_004.02-BNS-logical-01");
    position = r_u32(0x800B8D48 + (index << 2));
    input = r_u16(0x800B8B54 + (index << 1)) & 0xF0F0;
    entry = 0x800B8C10 + (position << 2);
    packed = ((r_u8(entry) << 4) | (r_u8(entry + 1) << 12)) & 0xFFFF;
    if (packed != input) {
        actor = 0x800A9228 + index * 6284;
        ++position;
        if ((sint32)position < 0)
            position = 49;
        else if ((sint32)position >= 50)
            position = 0;
        entry = 0x800B8C10 + (position << 2);
        w_u8(entry, (input >> 4) & 15);
        w_u8(entry + 1, input >> 12);
        word = (r_u32(entry) & 0x8000FFFF) | 0x10000;
        w_u32(entry, word);
        flags = r_u8(actor + 0xC6) != 0;
        word = (word & 0x7FFFFFFF) | (flags << 31);
        w_u32(entry, word);
        w_u32(0x800B8D48 + (index << 2), position);
        return;
    }
    word = r_u32(entry);
    count = (sint32)(word << 1) >> 17;
    packed = (word & 0x8000FFFF) | (((uint32)(count + 1) & 0x7FFF) << 16);
    w_u32(entry, packed);
    count = (sint32)(packed << 1) >> 17;
    if (count >= 0x3FFF)
        w_u32(entry, (packed & 0x8000FFFF) | 0x3FFF0000);
}

void t3_bns_01_800b7cd4(uint32 index)
{
    uint32 slot, position, entry, value;

    FUNCTION_MARKER(0x800B7CD4, "SLUS_004.02-BNS-logical-01");
    slot = 0x800B8D48 + (index << 2);
    position = r_u32(slot);
    entry = 0x800B8C10 + (position << 2);
    value = (r_u32(entry) & 0x8000FFFF) | 0x10000;
    w_u32(entry, value);
    if (r_s32(slot) < 0) {
        value = r_u32(0x800B8CDC);
        w_u32(0x800B8CD8, 0);
        w_u32(0x800B8CDC, value & 0x8000FFFF);
        w_u8(0x800B8CDC, 0);
        w_u8(0x800B8CDD, 0);
    } else {
        value = r_u32(0x800B8C10);
        w_u32(0x800B8CD8, 0);
        w_u32(0x800B8CDC, value);
    }
    w_u32(0x800B8CE0, 0);
    t3_bns_01_800b7cb0();
}

uint32 t3_bns_01_800b7d84(uint32 index)
{
    uint32 actor, word, packed, next, limit, value, direction, input;
    sint32 count;

    FUNCTION_MARKER(0x800B7D84, "SLUS_004.02-BNS-logical-01");
    actor = 0x800A9228 + index * 6284;
    for (;;) {
        word = r_u32(0x800B8CDC);
        count = ((sint32)(word << 1) >> 17) - 1;
        packed = (word & 0x8000FFFF) | (((uint32)count & 0x7FFF) << 16);
        w_u32(0x800B8CDC, packed);
        if (!((uint32)count & 0x4000))
            break;
        next = r_u32(0x800B8CD8) + 1;
        w_u32(0x800B8CD8, next);
        limit = r_u32(0x800B8D48 + (index << 2));
        if ((sint32)limit < (sint32)next || (sint32)next >= 50) {
            if ((sint32)limit < 0) {
                value = r_u32(0x800B8CDC);
                w_u32(0x800B8CD8, 0);
                w_u32(0x800B8CDC, value & 0x8000FFFF);
                w_u8(0x800B8CDC, 0);
                w_u8(0x800B8CDD, 0);
            } else {
                value = r_u32(0x800B8C10);
                w_u32(0x800B8CD8, 0);
                w_u32(0x800B8CDC, value);
            }
            return 0;
        }
        value = r_u32(0x800B8C10 + (next << 2));
        w_u32(0x800B8CD8, next);
        w_u32(0x800B8CDC, value);
    }
    direction = r_u8(0x800B8CDD);
    if ((sint32)packed < 0) {
        if (direction & 8)
            direction = (direction & 0xFFF7) | 2;
        else if (direction & 2)
            direction = (direction & 0xFFFD) | 8;
    }
    input = (r_u8(0x800B8CDC) << 4) | (direction << 12);
    w_u16(actor + 0x408, input);
    w_u32(0x80095470 + (r_u8(actor + 0x1E) << 2), 2);
    return 1;
}

uint32 t3_bns_01_800b80d0(uint32 control)
{
    uint32 actor;

    FUNCTION_MARKER(0x800B80D0, "SLUS_004.02-BNS-logical-01");
    actor = 0x800A9228 + (uint32)r_s8(control + 0x7B) * 6284;
    if (r_u8(actor + 0xD0))
        return 1;
    if (!r_u32(actor + 0x1A4))
        return 0;
    return r_s16(actor + 0x19A) == 4;
}

void t3_bns_01_800b7ef0(uint32 index)
{
    uint32 actor, animation, frame, cursor, action, next;

    FUNCTION_MARKER(0x800B7EF0, "SLUS_004.02-BNS-logical-01");
    actor = 0x800A9228 + index * 6284;
    animation = r_u32(actor + 0x54);
    frame = r_u8(animation + 0x2D);
    if (!frame || r_s16(actor + 0x58) != (sint32)frame)
        return;
    cursor = r_u32(0x800B8CE0);
    action = r_u16(actor + 0xA0);
    next = cursor + 1;
    w_u16(0x800B8CE4 + (cursor << 1), action);
    if ((sint32)next >= 50)
        next = 0;
    w_u32(0x800B8CE0, next);
}

void t3_bns_01_800b61bc(uint32 index, uint32 pressed)
{
    FUNCTION_MARKER(0x800B61BC, "SLUS_004.02-BNS-logical-01");
    if (pressed & 0x80)
        t3_800753bc(index, 0x4C70, 0);
    if (pressed & 0x10)
        t3_800753bc(index, 0x4C71, 0);
    if (pressed & 0x40)
        t3_800753bc(index, 0x4C72, 0);
    if (pressed & 0x20)
        t3_800753bc(index, 0x4C73, 0);
}

uint32 t3_bns_01_800b42fc(uint32 actor, uint32 opponent)
{
    uint32 animation, resource, active, reacting;
    sint32 action;

    FUNCTION_MARKER(0x800B42FC, "SLUS_004.02-BNS-logical-01");
    animation = r_u32(actor + 0x54);
    active = 1;
    if (!r_u8(animation + 0x2D)) {
        resource = r_u32(actor + 0x1A4);
        active = 0;
        if (resource)
            active = r_u8(resource + 0x2D) != 0;
    }
    animation = r_u32(opponent + 0x54);
    action = r_s16(opponent + 0xA0);
    reacting = 0;
    if (action != (sint32)r_u16(animation + 0x10)) {
        reacting = 1;
        if (!r_u8(opponent + 0x87) && !r_u16(opponent + 0x80))
            reacting = 0;
    }
    return active || reacting;
}

void t3_bns_01_800b4770(void)
{
    uint32 control, index, sequence;
    sint32 mode;

    FUNCTION_MARKER(0x800B4770, "SLUS_004.02-BNS-logical-01");
    control = r_u32(0x800B8A2C);
    mode = r_s8(control + 0x77);
    if (mode < 0 || mode > 4)
        return;
    if (!(r_u16(control + 2) & 0x9F0))
        return;
    if (mode == 3) {
        w_u32(0x80096F2C, 9);
        t3_800753bc(0, 0x4CEB, 0);
        return;
    }
    if (mode == 4) {
        w_u32(0x80096F2C, 11);
        return;
    }
    w_u8(control + 0x75, 0);
    w_u8(r_u32(0x800B8A2C) + 0x78, 0);
    w_u8(r_u32(0x800B8A2C) + 0x79, 0);
    w_u8(r_u32(0x800B8A2C) + 0x7A, 0);
    w_u8(r_u32(0x800B8A2C) + 0xA7, 0);
    w_u8(r_u32(0x800B8A2C) + 0xA9, 3);
    control = r_u32(0x800B8A2C);
    if (r_s8(control + 0x77) == 2) {
        w_u8(control + 0x7B, r_u8(control + 0x84));
        w_u8(r_u32(0x800B8A2C) + 0x7C, 0);
        w_u8(r_u32(0x800B8A2C) + 0x87, 1);
        w_u8(r_u32(0x800B8A2C) + 0xA0, 1);
        w_u8(r_u32(0x800B8A2C) + 0xA3, 1);
        t3_bns_01_800b744c((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
        t3_bns_01_800b7c24((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
        control = r_u32(0x800B8A2C);
        index = (uint32)r_s8(control + 0x7B);
        sequence = (uint32)r_s8(control + 0x7C);
        t3_bns_01_800b7994(index, sequence);
    }
    t3_800753bc(0, 0x4CEB, 0);
    t3_800484c8();
}

void t3_bns_01_800b5318(void)
{
    uint32 control, offset;

    FUNCTION_MARKER(0x800B5318, "SLUS_004.02-BNS-logical-01");
    t3_8004ce74(0x800B074C, 10, 1, 16, 56);
    control = r_u32(0x800B8A2C);
    for (offset = 0; offset < 17; ++offset)
        w_u8(control + 0x8C + offset, 11);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x77);
    w_u8(control + offset + 0x8C, 10);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B0760, r_u8(control + 0x8C), 0, 16, 110);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B0778, r_u8(control + 0x8D), 0, 16, 132);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B0788, r_u8(control + 0x8E), 0, 16, 154);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07A0, r_u8(control + 0x8F), 0, 16, 176);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07B8, r_u8(control + 0x90), 0, 16, 198);
}

void t3_bns_01_800b545c(void)
{
    uint32 control, offset, color, text, length, x;
    sint32 mode;

    FUNCTION_MARKER(0x800B545C, "SLUS_004.02-BNS-logical-01");
    t3_8004ce74(0x800B0760, 10, 1, 72, 56);
    control = r_u32(0x800B8A2C);
    for (offset = 0; offset < 17; ++offset)
        w_u8(control + 0x8C + offset, 11);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x78);
    w_u8(control + offset + 0x8C, 10);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x8C);
    text = r_u32(0x800B8A38 + (r_u8(control + 0xA7) << 2));
    t3_8004ce74(0x800B07C8, color, 0, 72, 110, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x8D), 0, 72, 132, 0x800B07D4);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x8E), 0, 72, 162, 0x800B07E4);
    control = r_u32(0x800B8A2C);
    mode = r_s8(control + 0x7D);
    if (mode == 6 || mode == 7) {
        t3_bns_01_800b70c8(mode == 7, 189, 184, r_u8(control + 0x8E));
    } else {
        t3_8004ce74(0x800B07C8, r_u8(control + 0x8E), 0, 72, 184, 0x800B07F4);
        control = r_u32(0x800B8A2C);
        offset = (uint32)r_s8(control + 0x7D);
        text = r_u32(0x800B8A40 + (offset << 2));
        length = t3_8007a418(text);
        x = 72 + 9 * (25 - length);
        control = r_u32(0x800B8A2C);
        color = r_u8(control + 0x8E);
        offset = (uint32)r_s8(control + 0x7D);
        text = r_u32(0x800B8A40 + (offset << 2));
        t3_8004ce74(0x800B07C8, color, 0, x, 184, text);
    }
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x8F), 0, 72, 206, 0x800B07F8);
    control = r_u32(0x800B8A2C);
    text = r_u32(0x800B8A30 + (r_u8(control + 0x9E) << 2));
    length = t3_8007a418(text);
    x = 72 + 9 * (25 - length);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x8F);
    text = r_u32(0x800B8A30 + (r_u8(control + 0x9E) << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 206, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x90), 0, 72, 228, 0x800B0808);
    control = r_u32(0x800B8A2C);
    text = r_u32(0x800B8A30 + (r_u8(control + 0x9D) << 2));
    length = t3_8007a418(text);
    x = 72 + 9 * (25 - length);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x90);
    text = r_u32(0x800B8A30 + (r_u8(control + 0x9D) << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 228, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x91), 0, 72, 250, 0x800B0814);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x83);
    text = r_u32(0x800B8A30 + (offset << 2));
    length = t3_8007a418(text);
    x = 72 + 9 * (25 - length);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x91);
    offset = (uint32)r_s8(control + 0x83);
    text = r_u32(0x800B8A30 + (offset << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 250, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x92), 0, 72, 272, 0x800B0824);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x81);
    text = r_u32(0x800B8A64 + (offset << 2));
    length = t3_8007a418(text);
    control = r_u32(0x800B8A2C);
    x = 72 + 9 * (25 - length);
    color = r_u8(control + 0x92);
    offset = (uint32)r_s8(control + 0x81);
    text = r_u32(0x800B8A64 + (offset << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 272, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x93), 0, 72, 328, 0x800B0834);
}

void t3_bns_01_800b5918(void)
{
    uint32 control, offset, color, text, length, x;

    FUNCTION_MARKER(0x800B5918, "SLUS_004.02-BNS-logical-01");
    t3_8004ce74(0x800B084C, 10, 1, 72, 56);
    control = r_u32(0x800B8A2C);
    for (offset = 0; offset < 17; ++offset)
        w_u8(control + 0x8C + offset, 11);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x79);
    w_u8(control + offset + 0x8C, 10);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x8C);
    text = r_u32(0x800B8A38 + (r_u8(control + 0xA7) << 2));
    t3_8004ce74(0x800B07C8, color, 0, 72, 110, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x8D), 0, 72, 132, 0x800B07D4);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x8E), 0, 72, 162, 0x800B085C);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x7E);
    text = r_u32(0x800B8A84 + (offset << 2));
    length = t3_8007a418(text);
    x = 72 + 9 * (25 - length);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x8E);
    offset = (uint32)r_s8(control + 0x7E);
    text = r_u32(0x800B8A84 + (offset << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 162, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x8F), 0, 72, 184, 0x800B086C);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x7F);
    text = r_u32(0x800B8A90 + (offset << 2));
    length = t3_8007a418(text);
    x = 72 + 9 * (25 - length);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x8F);
    offset = (uint32)r_s8(control + 0x7F);
    text = r_u32(0x800B8A90 + (offset << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 184, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x90), 0, 72, 206, 0x800B0808);
    control = r_u32(0x800B8A2C);
    text = r_u32(0x800B8A30 + (r_u8(control + 0x9D) << 2));
    length = t3_8007a418(text);
    x = 72 + 9 * (25 - length);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x90);
    text = r_u32(0x800B8A30 + (r_u8(control + 0x9D) << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 206, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x91), 0, 72, 228, 0x800B0814);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x83);
    text = r_u32(0x800B8A30 + (offset << 2));
    length = t3_8007a418(text);
    x = 72 + 9 * (25 - length);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x91);
    offset = (uint32)r_s8(control + 0x83);
    text = r_u32(0x800B8A30 + (offset << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 228, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x92), 0, 72, 250, 0x800B0824);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x81);
    text = r_u32(0x800B8A64 + (offset << 2));
    length = t3_8007a418(text);
    control = r_u32(0x800B8A2C);
    x = 72 + 9 * (25 - length);
    color = r_u8(control + 0x92);
    offset = (uint32)r_s8(control + 0x81);
    text = r_u32(0x800B8A64 + (offset << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 250, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x93), 0, 72, 328, 0x800B0834);
}

void t3_bns_01_800b5d18(void)
{
    uint32 control, offset, color, text, length, x, actor, descriptor;

    FUNCTION_MARKER(0x800B5D18, "SLUS_004.02-BNS-logical-01");
    t3_8004ce74(0x800B0788, 10, 1, 72, 56);
    control = r_u32(0x800B8A2C);
    for (offset = 0; offset < 17; ++offset)
        w_u8(control + 0x8C + offset, 11);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x7A);
    w_u8(control + offset + 0x8C, 10);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x8C);
    text = r_u32(0x800B8A38 + (r_u8(control + 0xA7) << 2));
    t3_8004ce74(0x800B07C8, color, 0, 72, 110, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x8D), 0, 72, 132, 0x800B07D4);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x8E);
    offset = (uint32)r_s8(control + 0x7B) + 1;
    t3_8004ce74(0x800B0878, color, 0, 72, 162, offset);
    control = r_u32(0x800B8A2C);
    actor = 0x800A9228 + (uint32)r_s8(control + 0x7B) * 6284;
    descriptor = t3_8004efc8(r_s16(actor + 0x14));
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x8E);
    text = r_u32(descriptor);
    t3_8004ce74(0x800B07C8, color, 0, 207, 162, text);
    control = r_u32(0x800B8A2C);
    if (t3_bns_01_800b7ae8(control)) {
        control = r_u32(0x800B8A2C);
        t3_8004ce74(0x800B07C8, r_u8(control + 0x8F), 0, 72, 184, 0x800B0894);
        control = r_u32(0x800B8A2C);
        offset = (uint32)r_s8(control + 0x7C);
        text = r_u32(0x800B8AC4 + (offset << 2));
        length = t3_8007a418(text);
        x = 72 + 9 * (25 - length);
        control = r_u32(0x800B8A2C);
        color = r_u8(control + 0x8F);
        offset = (uint32)r_s8(control + 0x7C);
        text = r_u32(0x800B8AC4 + (offset << 2));
        t3_8004ce74(0x800B07C8, color, 0, x, 184, text);
    } else {
        control = r_u32(0x800B8A2C);
        t3_8004ce74(0x800B07C8, r_u8(control + 0x8F), 0, 72, 184, 0x800B0894);
        control = r_u32(0x800B8A2C);
        t3_8004ce74(0x800B07C8, r_u8(control + 0x8F), 0, 261, 184, 0x800B08A0);
    }
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x90), 0, 72, 206, 0x800B0808);
    control = r_u32(0x800B8A2C);
    text = r_u32(0x800B8A30 + (r_u8(control + 0x9D) << 2));
    length = t3_8007a418(text);
    x = 72 + 9 * (25 - length);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x90);
    text = r_u32(0x800B8A30 + (r_u8(control + 0x9D) << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 206, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x91), 0, 72, 228, 0x800B0814);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x83);
    text = r_u32(0x800B8A30 + (offset << 2));
    length = t3_8007a418(text);
    x = 72 + 9 * (25 - length);
    control = r_u32(0x800B8A2C);
    color = r_u8(control + 0x91);
    offset = (uint32)r_s8(control + 0x83);
    text = r_u32(0x800B8A30 + (offset << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 228, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x92), 0, 72, 250, 0x800B0824);
    control = r_u32(0x800B8A2C);
    offset = (uint32)r_s8(control + 0x81);
    text = r_u32(0x800B8A64 + (offset << 2));
    length = t3_8007a418(text);
    control = r_u32(0x800B8A2C);
    x = 72 + 9 * (25 - length);
    color = r_u8(control + 0x92);
    offset = (uint32)r_s8(control + 0x81);
    text = r_u32(0x800B8A64 + (offset << 2));
    t3_8004ce74(0x800B07C8, color, 0, x, 250, text);
    control = r_u32(0x800B8A2C);
    t3_8004ce74(0x800B07C8, r_u8(control + 0x93), 0, 72, 328, 0x800B0834);
}

void t3_bns_01_800b4920(void)
{
    FUNCTION_MARKER(0x800B4920, "SLUS_004.02-BNS-logical-01");
}

void t3_bns_01_800b4930(void)
{
    uint32 control, selection, offset, input, value, maximum, index, other, changed;
    sint32 mode, signed_value;

    FUNCTION_MARKER(0x800B4930, "SLUS_004.02-BNS-logical-01");
    control = r_u32(0x800B8A2C);
    mode = r_s8(control + 0x77);
    selection = 0;
    if (!mode)
        selection = r_u8(0x800B0548 + (uint32)r_s8(control + 0x78));
    else if (mode == 1)
        selection = r_u8(0x800B0550 + (uint32)r_s8(control + 0x79));
    else if (mode == 2)
        selection = r_u8(0x800B0558 + (uint32)r_s8(control + 0x7A));
    switch (selection) {
    case 0:
        control = r_u32(0x800B8A2C);
        if (r_u16(control + 2) & 0x9F0) {
            w_u32(0x80095488, 1);
            w_u8(control + 0xAC, 1);
        } else {
            t3_bns_01_800b4920();
        }
        return;
    case 1:
    case 2:
    case 3:
        offset = selection == 1 ? 0x9D : selection == 2 ? 0x9E : 0x83;
        control = r_u32(0x800B8A2C);
        input = r_u16(control + 2);
        if (input & 0x9F0)
            goto reset_selection;
        if (!(input & 0xA000))
            return;
        w_u8(control + offset, (r_u8(control + offset) + 1) & 1);
        goto settings_changed;
    case 4:
    case 9:
    case 10:
    case 13:
        offset = selection == 4 ? 0x81 : selection == 9 ? 0x7E : selection == 10 ? 0x7F : 0x7D;
        control = r_u32(0x800B8A2C);
        input = r_u16(control + 2);
        if (input & 0x9F0)
            goto reset_selection;
        if (input & 0x2000) {
            w_u8(control + offset, r_u8(control + offset) + 1);
            w_u8(r_u32(0x800B8A2C) + 0xA7, 0);
            t3_800753bc(0, 0x546C, 0);
        }
        control = r_u32(0x800B8A2C);
        if (r_u16(control + 2) & 0x8000) {
            w_u8(control + offset, r_u8(control + offset) - 1);
            w_u8(r_u32(0x800B8A2C) + 0xA7, 0);
            t3_800753bc(0, 0x546C, 0);
        }
        control = r_u32(0x800B8A2C);
        if (selection == 4)
            maximum = r_s8(control + 0x77) == 2 ? 3 : 4;
        else
            maximum = selection == 9 ? 2 : selection == 10 ? 9 : 8;
        signed_value = r_s8(control + offset);
        value = r_u8(control + offset);
        if (signed_value < 0)
            value = maximum;
        else if (signed_value > (sint32)maximum)
            value = 0;
        w_u8(control + offset, value);
        return;
    case 5:
        control = r_u32(0x800B8A2C);
        input = r_u16(control + 2);
        if (input & 0x9F0)
            goto reset_selection;
        if (!(input & 0xA000))
            return;
        w_u8(control + 0xA0, (r_u8(control + 0xA0) + 1) & 1);
        t3_bns_01_800b744c(r_u8(r_u32(0x800B8A2C) + 0x84));
        t3_bns_01_800b744c(r_u8(r_u32(0x800B8A2C) + 0x85));
        goto settings_changed;
    case 6:
        control = r_u32(0x800B8A2C);
        input = r_u16(control + 2);
        if (input & 0x9F0)
            goto reset_selection;
        if (!(input & 0xA000))
            return;
        w_u8(control + 0x87, (r_u8(control + 0x87) + 1) & 1);
        w_u8(r_u32(0x800B8A2C) + 0xA7, 0);
        t3_800753bc(0, 0x546C, 0);
        t3_bns_01_800b744c(0);
        t3_bns_01_800b744c(1);
        return;
    case 7:
        control = r_u32(0x800B8A2C);
        if (r_u16(control + 2) & 0x9F0) {
            w_u8(control + 0x9F, (r_u8(control + 0x9F) + 1) & 1);
            w_u8(r_u32(0x800B8A2C) + 0xA9, 3);
            t3_800484c8();
            control = r_u32(0x800B8A2C);
            if (r_u8(control + 0x9F)) {
                w_u8(0x800A8672, 3);
            } else {
                w_u8(control + 0xA7, 0);
                w_u8(r_u32(0x800B8A2C) + 0x78, 0);
                w_u8(r_u32(0x800B8A2C) + 0x79, 0);
                w_u8(r_u32(0x800B8A2C) + 0x7A, 0);
            }
        }
        control = r_u32(0x800B8A2C);
        if (!r_u8(control + 0x9F))
            return;
        signed_value = r_s8(control + 0x80);
        index = r_u8(control + 0x84);
        value = (uint32)1 << (index & 31);
        if (!signed_value) {
            other = r_u8(control + 0x85);
            value |= (uint32)1 << (other & 31);
        }
        t3_80078bcc(value, index);
        return;
    case 11:
        if (r_u16(r_u32(0x800B8A2C) + 2) & 0x9F0) {
            w_u32(0x80096F2C, 9);
            t3_800753bc(0, 0x546C, 0);
        }
        return;
    case 12:
        if (r_u16(r_u32(0x800B8A2C) + 2) & 0x9F0)
            w_u32(0x80096F2C, 11);
        return;
    case 14:
        control = r_u32(0x800B8A2C);
        if (!(r_u16(control + 2) & 0x9F0))
            return;
        w_u8(control + 0x75, 1);
        w_u8(r_u32(0x800B8A2C) + 0xA9, 3);
        t3_800484c8();
        control = r_u32(0x800B8A2C);
        if (r_s8(control + 0x77) == 2) {
            t3_bns_01_800b744c((uint32)r_s8(control + 0x7B));
            t3_bns_01_800b7c24((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
        }
        return;
    case 15:
        control = r_u32(0x800B8A2C);
        input = r_u16(control + 2);
        if (input & 0x9F0)
            goto reset_selection;
        if (!(input & 0xA000))
            return;
        w_u8(control + 0x7B, (r_u8(control + 0x7B) + 1) & 1);
        control = r_u32(0x800B8A2C);
        signed_value = r_s8(control + 0x7B);
        value = r_u8(control + 0x7B);
        if (signed_value < 0)
            value = 1;
        else if (signed_value >= 2)
            value = 0;
        w_u8(control + 0x7B, value);
        w_u8(r_u32(0x800B8A2C) + 0x7C, 0);
        goto reset_playback;
    case 16:
        control = r_u32(0x800B8A2C);
        input = r_u16(control + 2);
        if (input & 0x9F0)
            goto reset_selection;
        changed = 0;
        if (input & 0x2000) {
            value = r_u8(control + 0x7C);
            changed = 1;
            w_u8(control + 0x7C, value + 1);
        }
        control = r_u32(0x800B8A2C);
        if (r_u16(control + 2) & 0x8000) {
            value = r_u8(control + 0x7C);
            changed = 1;
            w_u8(control + 0x7C, value - 1);
        }
        control = r_u32(0x800B8A2C);
        signed_value = r_s8(control + 0x7C);
        value = t3_bns_01_800b7ae8(control) - 1;
        if (signed_value < 0) {
            w_u8(control + 0x7C, value);
        } else {
            if ((sint32)value < signed_value)
                value = 0;
            else
                value = r_u8(r_u32(0x800B8A2C) + 0x7C);
            w_u8(control + 0x7C, value);
        }
        if (changed)
            goto reset_playback;
        return;
    default:
        return;
    }

reset_playback:
    t3_bns_01_800b744c((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
    t3_bns_01_800b7c24((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
    control = r_u32(0x800B8A2C);
    index = (uint32)r_s8(control + 0x7B);
    other = (uint32)r_s8(control + 0x7C);
    t3_bns_01_800b7994(index, other);
settings_changed:
    w_u8(r_u32(0x800B8A2C) + 0xA7, 0);
    t3_800753bc(0, 0x546C, 0);
    return;

reset_selection:
    control = r_u32(0x800B8A2C);
    mode = r_s8(control + 0x77);
    if (!mode)
        w_u8(control + 0x78, 0);
    else if (mode == 1)
        w_u8(control + 0x79, 0);
    else if (mode == 2)
        w_u8(control + 0x7A, 0);
}

void t3_80078bcc(uint32 mask, uint32 index)
{
    uint32 value, side, ordering_entry, packet, x, y, width, height, context;
    PSX_RECT rectangle;

    FUNCTION_MARKER(0x80078BCC, "SLUS_004.02");
    if (r_u32(0x800ADCA4)) {
        for (side = 0; side < 2; ++side) {
            if ((mask >> side) & 1)
                t3_80078550(side, side);
        }
        return;
    }
    value = r_u8(0x800A8672);
    if (value) {
        w_u8(0x800A8672, value - 1);
        return;
    }
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    x = r_u16(0x800AE230);
    y = r_u16(0x800AE232);
    rectangle.w = 0;
    rectangle.h = 0;
    rectangle.x = (sint16)x;
    rectangle.y = (sint16)y;
    packet = t3_8004db28(ordering_entry, packet, &rectangle);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004db28(ordering_entry + 4, packet, &rectangle);
    w_u32(0x800ADF64, packet);
    t3_8004ce74(0x80027C1C, 0, 0, (index * 184) | 6, 440, 6, 5, 6, 5);
    for (side = 0; side < 2; ++side) {
        if ((mask >> side) & 1)
            t3_80078550(side, side);
    }
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    x = r_u16(0x800AE230);
    width = r_u16(0x800AE234);
    y = r_u16(0x800AE232);
    height = r_u16(0x800AE236);
    rectangle.y = 92;
    rectangle.x = (sint16)x;
    rectangle.w = (sint16)width;
    rectangle.h = (sint16)(uint16)(y + height - 92);
    packet = t3_8004db28(ordering_entry, packet, &rectangle);
    x = r_u16(0x800AE230);
    ordering_entry = r_u32(0x800A9218);
    rectangle.y = 116;
    rectangle.h = 319;
    width = r_u16(0x800AE234);
    w_u32(0x800ADF64, packet);
    rectangle.x = (sint16)x;
    rectangle.w = (sint16)width;
    packet = t3_8004db28(ordering_entry + 4, packet, &rectangle);
    x = r_u16(0x800AE230);
    y = r_u16(0x800AE232);
    context = r_u32(0x800A8C54);
    rectangle.w = 0;
    rectangle.h = 0;
    w_u32(0x800ADF64, packet);
    rectangle.x = (sint16)x;
    rectangle.y = (sint16)y;
    ordering_entry = r_u32(context + 0x10);
    packet = t3_8004db28(ordering_entry, packet, &rectangle);
    w_u32(0x800ADF64, packet);
}

void t3_bns_01_800b6248(void)
{
    uint32 control, text, color, stats, current, other, amount, shown, entry, x, y, kind, row;
    sint32 state, hits, damage, total, percentage, cursor, action;

    FUNCTION_MARKER(0x800B6248, "SLUS_004.02-BNS-logical-01");
    if (r_u32(0x800954BC))
        return;
    control = r_u32(0x800B8A2C);
    if (r_u8(control + 0x87)
        && (r_s8(control + 0x77) != 2 || t3_bns_01_800b7ae8(control))) {
        control = r_u32(0x800B8A2C);
        state = r_s8(control + 0x82);
        text = 0;
        color = 1;
        switch (state) {
        case 1:
            if (r_s8(r_u32(0x800B8A2C) + 0x77) == 2) {
                text = 0x800B08A8;
            } else {
                t3_bns_01_800b70c8(2, 9, 379, 7);
                text = 0x800B08BC;
                color = 5;
            }
            break;
        case 2:
            text = 0x800B08C4;
            break;
        case 3:
            text = 0x800B08E0;
            color = 2;
            break;
        case 5:
            text = 0x800B08F8;
            color = 5;
            break;
        case 8:
            text = 0x800B0904;
            color = 6;
            break;
        }
        if (text)
            t3_8004ce74(text, color, 1, 21);
    } else {
        control = r_u32(0x800B8A2C);
        if (!r_u8(control + 0x8A) && r_s8(control + 0x81) == 4 && !r_s8(control + 0x77))
            t3_8004ce74(0x800B0918, 1, 1, 21);
    }
    control = r_u32(0x800B8A2C);
    if (r_u8(control + 0x9D) && !r_u8(control + 0x8A)) {
        current = r_u8(control + 0x84);
        other = r_u8(control + 0x85);
        current = control + (current << 5) + 0x14;
        stats = control + (other << 5) + 0x14;
        damage = r_s16(stats + 0xE);
        total = r_s16(stats + 0xC);
        amount = r_u32(current);
        t3_8004ce74(0x800B0930, 1, 23, 3, amount);
        amount = r_u32(stats);
        t3_8004ce74(0x800B0930, 1, 1, 3, amount);
        hits = r_s8(stats + 0x1D);
        shown = 0;
        if (hits >= 2 && r_s16(stats + 0x10)) {
            amount = r_u32(stats + 8);
            if ((sint32)amount > 0) {
                t3_bns_01_800b691c(hits, amount);
                shown = 1;
            }
        }
        if (!shown && r_s16(stats + 0x12) >= 2)
            t3_bns_01_800b691c(0, r_u32(stats + 8));
        color = total < damage ? 7 : damage < total ? 3 : 6;
        percentage = 0;
        if (total && damage)
            percentage = damage * 100 / total;
        t3_8004ce74(0x800B0944, color, 1, 4, damage, percentage);
        if (r_u8(stats + 0x1A))
            t3_8004ce74(0x800B095C, 2, 1, 5);
        if (r_u8(stats + 0x1B))
            t3_8004ce74(0x800B096C, 5, 1, 6);
        cursor = r_u8(r_u32(0x800B8A2C) + 0x74);
        for (row = 0; row < 4; ++row, --cursor) {
            if (cursor < 0)
                cursor = 3;
            entry = r_u32(0x800B8A2C) + ((uint32)cursor << 3) + 0x54;
            if (!r_s16(entry + 2))
                continue;
            x = r_u16(entry + 4);
            action = r_s16(entry);
            y = r_u16(entry + 6);
            kind = 0;
            switch (action) {
            case 0x10F:
                kind = 2;
                break;
            case 0x217:
            case 0x31F:
            case 0x51F:
                kind = 1;
                break;
            case 0x607:
            case 0x706:
                kind = 3;
                break;
            case 0x800:
                continue;
            }
            t3_bns_01_800b6f70((sint16)(uint16)x, (sint16)(uint16)y, kind);
        }
    }
    control = r_u32(0x800B8A2C);
    if (r_u8(control + 0xA0))
        t3_bns_01_800b7648(control);
}

void t3_bns_01_800b691c(sint32 hits, uint32 amount)
{
    uint32 x, magnitude;

    FUNCTION_MARKER(0x800B691C, "SLUS_004.02-BNS-logical-01");
    if (hits) {
        t3_8004ce74(0x800B09A4, 12, 136, 1, 6, (uint32)hits);
        x = hits < 10 ? 32 : 42;
        t3_8004ce74(0x800B09B0, x, 136, 1, 5);
    }
    t3_8004ce74(0x800B09A4, 22, 166, 1, 6, amount);
    magnitude = (sint32)amount < 0 ? 0 - amount : amount;
    if ((sint32)magnitude < 100)
        x = (sint32)magnitude < 10 ? 42 : 53;
    else
        x = 64;
    if ((sint32)amount < 0)
        x += 13;
    t3_8004ce74(0x800B09C0, x, 176, 0, 7);
}

void t3_bns_01_800b6f70(sint32 x, sint32 y, uint32 kind)
{
    uint32 ordering_entry, packet, packed, texture, background;

    FUNCTION_MARKER(0x800B6F70, "SLUS_004.02-BNS-logical-01");
    ordering_entry = r_u32(0x800A9218);
    if (r_u32(0x800ADCA4))
        return;
    if (kind == 3) {
        ordering_entry += 8;
        packed = (((uint32)x - 6) & 0xFFFF) | (((uint32)y - 24) << 16);
        packet = r_u32(0x800ADF64);
        packet = t3_8004d9bc(ordering_entry, packet, 0x65000000, packed, 0x30000C, 0x7FF6C090);
        background = 0x7FF7C0D0;
    } else {
        ordering_entry += 4;
        packed = (((uint32)x - 24) & 0xFFFF) | (((uint32)y - 8) << 16);
        packet = r_u32(0x800ADF64);
        texture = r_u32(0x800B8B34 + (kind << 2));
        packet = t3_8004d9bc(ordering_entry, packet, 0x65000000, packed, 0x100030, texture);
        background = 0x7FB7C0D0;
    }
    w_u32(0x800ADF64, packet);
    packed = (((uint32)x - 24) & 0xFFFF) | (((uint32)y - 24) << 16);
    packet = r_u32(0x800ADF64);
    packet = t3_8004d9bc(ordering_entry, packet, 0x65000000, packed, 0x300030, background);
    w_u32(0x800ADF64, packet);
    packet = t3_8004daa8(ordering_entry, packet, 31);
    w_u32(0x800ADF64, packet);
}

void t3_bns_01_800b7648(uint32 control)
{
    uint16 history[100][2];
    uint32 position, state, played, direction_before, buttons_before, direction_after, buttons_after;
    uint32 entry, direction, buttons, previous_direction, previous_buttons, count, recent, row, shown, start, x;

    FUNCTION_MARKER(0x800B7648, "SLUS_004.02-BNS-logical-01");
    position = r_u32(0x800B8D48 + ((uint32)r_s8(control + 0x7B) << 2));
    state = r_u8(control + 0xA3);
    played = r_u32(0x800B8CD8);
    direction_before = 0;
    buttons_before = 1;
    direction_after = 0;
    buttons_after = 1;
    if (state == 2 || (state == 3 && (r_u8(control + 0xA4) & 0x10))) {
        direction_before = 3;
        buttons_before = 0;
        direction_after = 3;
        buttons_after = 0;
    } else if (state == 4 || state == 5) {
        direction_after = 3;
        buttons_after = 0;
    }
    ++position;
    if ((sint32)position < 0)
        position = 49;
    else if ((sint32)position >= 50)
        position = 0;
    previous_direction = 0;
    previous_buttons = 0;
    count = 0;
    recent = 0;
    for (row = 0; row < 50; ++row) {
        entry = 0x800B8C10 + (position << 2);
        if (!position)
            recent = 0;
        direction = r_u8(entry + 1);
        if (direction && direction != previous_direction) {
            history[count][0] = (uint16)(r_u8(entry + 1) << 12);
            history[count][1] = (uint16)((sint32)played < (sint32)position ? direction_after : direction_before);
            ++count;
            ++recent;
        }
        buttons = r_u8(entry);
        if (buttons & (buttons ^ previous_buttons)) {
            history[count][0] = (uint16)(r_u8(entry) << 4);
            history[count][1] = (uint16)((sint32)played < (sint32)position ? buttons_after : buttons_before);
            ++count;
            ++recent;
        }
        ++position;
        previous_direction = r_u8(entry + 1);
        previous_buttons = r_u8(entry);
        if ((sint32)position < 0)
            position = 49;
        else if ((sint32)position >= 50)
            position = 0;
    }
    if (r_u8(control + 0xA2)) {
        shown = recent < 16 ? recent : 15;
        if (count < shown)
            shown = count;
    } else {
        shown = count < 16 ? count : 15;
    }
    if (!r_u8(control + 0x87))
        return;
    if (!r_s8(control + 0x77) || !shown)
        return;
    start = count - shown;
    x = 12;
    for (row = 0; row < shown; ++row) {
        buttons = history[start][0];
        state = history[start][1];
        ++start;
        if (buttons & 0xF000) {
            t3_bns_01_800b6d18(x, 400, buttons, state);
            x += 23;
        } else if (buttons & 0xF0) {
            t3_bns_01_800b6be0(x, 400, buttons, state);
            x += 23;
        }
    }
}

void t3_bns_01_800b6d18(uint32 x, uint32 y, uint32 input, uint32 color)
{
    uint32 packet, ordering_entry, selected, uv, flags, first, second, third, fourth, palette, position, tag;

    FUNCTION_MARKER(0x800B6D18, "SLUS_004.02-BNS-logical-01");
    packet = r_u32(0x800ADF64);
    ordering_entry = r_u32(0x800A9218) + 12;
    if (r_u32(0x800ADCA4))
        return;
    switch (input & 0xF000) {
    case 0x1000: selected = 1; break;
    case 0x3000: selected = 2; break;
    case 0x2000: selected = 3; break;
    case 0x6000: selected = 4; break;
    case 0x4000: selected = 5; break;
    case 0xC000: selected = 6; break;
    case 0x8000: selected = 7; break;
    case 0x9000: selected = 8; break;
    default: selected = 0; break;
    }
    uv = r_u16(0x800B09F4 + (selected << 2));
    flags = r_u16(0x800B09F6 + (selected << 2));
    w_u32(packet + 4, 0x2D000000);
    w_u32(packet + 20, uv + 19);
    w_u32(packet + 12, uv);
    w_u32(packet + 28, uv + 0x1F00);
    w_u32(packet + 36, uv + 0x1F13);
    if (flags & 1) {
        first = r_u32(packet + 12);
        second = r_u32(packet + 20);
        third = r_u32(packet + 36);
        w_u32(packet + 20, first);
        fourth = r_u32(packet + 28);
        w_u32(packet + 12, second);
        w_u32(packet + 28, third);
        w_u32(packet + 36, fourth);
    }
    if (flags & 2) {
        first = r_u32(packet + 12);
        second = r_u32(packet + 28);
        third = r_u32(packet + 36);
        w_u32(packet + 28, first);
        fourth = r_u32(packet + 20);
        w_u32(packet + 12, second);
        w_u32(packet + 20, third);
        w_u32(packet + 36, fourth);
    }
    palette = color == 1 ? 0x7FB30000 : color == 2 ? 0x7FB40000 : color == 3 ? 0x7FB50000 : 0x7FB20000;
    uv = r_u32(packet + 12);
    w_u32(packet + 12, uv | palette);
    position = (y << 16) | x;
    w_u32(packet + 16, position + 19);
    w_u32(packet + 24, position + 0x1F0000);
    w_u32(packet + 8, position);
    w_u32(packet + 32, position + 0x1F0013);
    w_u32(packet + 20, r_u32(packet + 20) | 0x60000);
    tag = r_u32(ordering_entry);
    w_u32(packet, (tag & 0xFFFFFF) | 0x09000000);
    tag = r_u32(ordering_entry);
    w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    w_u32(0x800ADF64, packet + 40);
}

void t3_bns_01_800b6be0(uint32 x, uint32 y, uint32 input, uint32 color)
{
    uint32 ordering_entry, packet, buttons, texture, position;

    FUNCTION_MARKER(0x800B6BE0, "SLUS_004.02-BNS-logical-01");
    ordering_entry = r_u32(0x800A9218) + 12;
    packet = r_u32(0x800ADF64);
    if (r_u32(0x800ADCA4))
        return;
    buttons = (input & 0x80) != 0;
    if (input & 0x10)
        buttons |= 2;
    if (input & 0x40)
        buttons |= 4;
    if (input & 0x20)
        buttons |= 8;
    texture = (((buttons & 0x18) << 3) | 0x7F00 | (buttons & 7) | 0x30) << 16;
    texture |= 0xE000;
    position = (x & 0xFFFF) | (y << 16);
    packet = t3_8004d9bc(ordering_entry, packet, 0x65000000, position, 0x200014, texture);
    texture = ((color != 0 ? 0x7FB1 : 0x7FB0) << 16) | 0xE014;
    packet = t3_8004d9bc(ordering_entry, packet, 0x65000000, position, 0x200014, texture);
    packet = t3_8004daa8(ordering_entry, packet, 6);
    w_u32(0x800ADF64, packet);
}

static uint32 t3_command_text_core(uint32 text, const uint8 *native_text, va_list *arguments)
{
    uint32 packet = r_u32(0x800ADF64);
    uint32 origin_x = r_u16(0x800A3608);
    uint32 origin_y = r_u16(0x800A360A);
    uint32 x = r_u16(0x800A360C);
    uint32 y = r_u16(0x800A360E);
    uint32 color = r_u32(0x800A3610);
    uint32 font = 0x80027AC0 + r_u32(0x80098A3C) * 10;
    uint32 ordering_entry;
    uint32 offset = 0;
    uint32 command = 0;
    uint32 icons = 0;
    uint32 character;

    w_u16(0x1F800396, origin_y);
    w_u16(0x1F800394, origin_x);
    w_u16(0x1F800380, x);
    w_u16(0x1F800382, y);
    w_u32(0x1F80038C, color);
    w_u16(0x1F800390, r_u16(font));
    w_u16(0x1F800392, r_u16(font + 2));
    w_u16(0x1F800384, r_u16(font + 4));
    w_u16(0x1F800386, r_u16(font + 6));
    ordering_entry = r_u32(0x800A9218) + 4;
    w_u16(0x1F800388, r_u16(font + 8));
    for (;;) {
        character = native_text ? native_text[offset] : r_u8(text + offset);
        ++offset;
        if (!character)
            break;
        if (character == 255) {
            if (!command) {
                command = 1;
                continue;
            }
            command = 0;
        }
        if (command) {
            uint32 value;
            command = 0;
            switch (character) {
            case 'H':
            case 'h':
                value = va_arg(*arguments, uint32);
                if (character == 'h')
                    value *= (uint32)r_s16(0x1F800390);
                w_u16(0x1F800380, value);
                w_u16(0x1F800394, value);
                continue;
            case 'V':
            case 'v':
                value = va_arg(*arguments, uint32);
                if (character == 'v')
                    value *= (uint32)r_s16(0x1F800392);
                w_u16(0x1F800382, value);
                w_u16(0x1F800396, value);
                continue;
            case 'i':
                w_u32(0x1F80038C, va_arg(*arguments, uint32));
                continue;
            case 'n':
                goto command_newline;
            case 'p':
                value = va_arg(*arguments, uint32);
                packet = t3_8004daa8(ordering_entry, packet, r_u16(0x1F800388));
                ordering_entry = r_u32(0x800A9218) + value * 4;
                continue;
            case 't':
                value = va_arg(*arguments, uint32);
                packet = t3_8004daa8(ordering_entry, packet, r_u16(0x1F800388));
                font = 0x80027AC0 + value * 10;
                w_u16(0x1F800390, r_u16(font));
                w_u16(0x1F800392, r_u16(font + 2));
                w_u16(0x1F800384, r_u16(font + 4));
                w_u16(0x1F800386, r_u16(font + 6));
                w_u32(0x80098A3C, value);
                w_u16(0x1F800388, r_u16(font + 8));
                continue;
            case 's':
                value = va_arg(*arguments, uint32);
                character = r_u8(value++);
                while (character) {
                    uint32 glyph = character - 32;
                    uint32 position = r_u32(0x1F800380);
                    uint32 dimensions;
                    w_u32(packet + 4, r_u32(0x1F80038C) | 0x64000000);
                    w_u32(packet + 8, position);
                    x = r_u16(0x1F800390);
                    dimensions = r_u32(0x1F800384);
                    w_u16(0x1F800380, x + position);
                    w_u32(packet + 16, dimensions);
                    w_u32(packet + 12, (((12 * (glyph >> 6) + 16) & 255) << 8)
                        + 6 * (glyph & 15) + 64 | (((glyph & 48) >> 4) | 0x7ED8) << 16);
                    w_u32(packet, (r_u32(ordering_entry) & 0xFFFFFF) | 0x04000000);
                    w_u32(ordering_entry, (packet & 0xFFFFFF) | (r_u32(ordering_entry) & 0xFF000000));
                    packet += 20;
                    character = r_u8(value++);
                }
                continue;
            default:
                t3_8007ae58(0);
                break;
            }
        }
        if (((character + 111) & 255) < 16) {
            uint32 icon = character + 111;
            uint32 position;
            uint32 next;
            if (!icons)
                packet = t3_8004daa8(ordering_entry, packet, r_u16(0x1F800388));
            y = r_u16(0x1F800382);
            x = (uint32)r_s16(0x1F800380);
            color = r_u32(0x1F80038C);
            w_u32(packet + 12, ((((icon & 8 ? 1 : 0) + 508) << 6 | (icon & 7) | 48) << 16) | 0xE000);
            w_u32(packet + 16, 0x200014);
            w_u32(packet + 4, color | 0x64000000);
            position = ((y << 16) - 0xC0000) | x;
            w_u32(packet + 8, position);
            w_u32(packet, (r_u32(ordering_entry) & 0xFFFFFF) | 0x04000000);
            w_u32(ordering_entry, (packet & 0xFFFFFF) | (r_u32(ordering_entry) & 0xFF000000));
            color = r_u32(0x1F80038C);
            w_u32(packet + 32, 0x7FB1E014);
            w_u32(packet + 28, position);
            w_u32(packet + 36, 0x200014);
            w_u32(packet + 24, color | 0x64000000);
            w_u32(packet + 20, (r_u32(ordering_entry) & 0xFFFFFF) | 0x04000000);
            w_u32(ordering_entry, ((packet + 20) & 0xFFFFFF) | (r_u32(ordering_entry) & 0xFF000000));
            w_u16(0x1F800380, position + 20);
            icons = 1;
            next = native_text ? native_text[offset] : r_u8(text + offset);
            packet += 40;
            if (((next + 111) & 255) >= 16)
                packet = t3_8004daa8(ordering_entry, packet, r_u16(0x80027ADC));
            continue;
        }
        if (!icons)
            packet = t3_8004daa8(ordering_entry, packet, r_u16(0x1F800388));
        if (((character + 128) & 255) < 17) {
            icons = 1;
            packet = t3_8004daa8(ordering_entry, packet, r_u16(0x1F800388));
            packet = t3_800775c0(ordering_entry, packet, 0x1F800380, (character + 128) & 255);
            continue;
        }
        icons = 0;
        if (character == 252) {
command_newline:
            y = r_u16(0x1F800396) + r_u16(0x1F800392);
            x = r_u16(0x1F800394);
            w_u16(0x1F800380, x);
            w_u16(0x1F800396, y);
            w_u16(0x1F800382, y);
        } else if (character == 32 || character == 251 || character == 253 || character == 254) {
            uint32 advance;
            if (character == 251)
                advance = (uint32)r_s16(0x1F800390) * 10;
            else if (character == 253)
                advance = (uint32)r_s16(0x1F800390) * 2;
            else
                advance = r_u16(0x1F800390);
            w_u16(0x1F800380, r_u16(0x1F800380) + advance);
        } else {
            uint32 glyph = character - 32;
            uint32 position;
            uint32 texture;
            if ((glyph & 255) < 129) {
                color = r_u32(0x1F80038C);
                position = r_u32(0x1F800380);
                w_u32(packet + 4, color | 0x64000000);
                w_u32(packet + 8, position);
                x = r_u16(0x1F800390);
                y = r_u32(0x1F800384);
                w_u16(0x1F800380, x + position);
                w_u32(packet + 16, y);
                texture = (((12 * (glyph >> 6) + 16) & 255) << 8) + 6 * (glyph & 15) + 64;
                texture |= (((glyph & 48) >> 4) | 0x7ED8) << 16;
            } else {
                uint32 advance;
                uint32 width_address = character < 32 ? 0x800989F7 + character : 0x80098976 + character;
                uint32 page;
                uint32 column;
                advance = (uint32)r_s16(0x1F800390) * r_u8(width_address);
                position = r_u32(0x1F800380);
                color = r_u32(0x1F80038C);
                w_u32(packet + 8, position);
                w_u32(packet + 4, color | 0x64000000);
                y = r_u16(0x1F800386);
                w_u16(0x1F800380, advance + position);
                w_u32(packet + 16, (advance & 0xFFFF) | (y << 16));
                if (character < 32) {
                    if (character - 1 < 34) {
                        page = ((character + 5) >> 2) * 3072 + 4096;
                        column = ((character + 1) & 3) | 0x7ED8;
                        texture = 0x40;
                    } else {
                        page = ((character - 27) >> 2) * 3072 + 4096;
                        column = ((character - 35) & 3) | 0x7ED8;
                        texture = 0x7C;
                    }
                } else if (character - 130 < 34) {
                    page = ((character - 124) >> 2) * 3072 + 4096;
                    column = ((character - 128) & 3) | 0x7ED8;
                    texture = 0x40;
                } else {
                    page = ((character - 156) >> 2) * 3072 + 4096;
                    column = ((character - 164) & 3) | 0x7ED8;
                    texture = 0x7C;
                }
                texture |= page | (column << 16);
            }
            w_u32(packet + 12, texture);
            w_u32(packet, (r_u32(ordering_entry) & 0xFFFFFF) | 0x04000000);
            w_u32(ordering_entry, (packet & 0xFFFFFF) | (r_u32(ordering_entry) & 0xFF000000));
            packet += 20;
        }
    }
    w_u32(0x800ADF64, t3_8004daa8(ordering_entry, packet, r_u16(0x1F800388)));
    w_u16(0x800A3608, r_u16(0x1F800394));
    w_u16(0x800A360A, r_u16(0x1F800396));
    w_u16(0x800A360C, r_u16(0x1F800380));
    w_u16(0x800A360E, r_u16(0x1F800382));
    w_u32(0x800A3610, r_u32(0x1F80038C));
    return offset;
}

uint32 t3_80077724(uint32 text, ...)
{
    va_list arguments;
    uint32 consumed;
    FUNCTION_MARKER(0x80077724, "SLUS_004.02");
    va_start(arguments, text);
    consumed = t3_command_text_core(text, 0, &arguments);
    va_end(arguments);
    return text + consumed;
}

const uint8 *t3_command_native_text(const uint8 *text, ...)
{
    va_list arguments;
    uint32 consumed;
    va_start(arguments, text);
    consumed = t3_command_text_core(0, text, &arguments);
    va_end(arguments);
    return text + consumed;
}
