#include "game_functions.h"

uint32 t3_bns_04_8010dc30(uint32 state, uint32 side)
{
    uint32 other_side = (side + 1) & 1;
    uint32 player = state + 36 + side * 124;
    uint32 other = state + 36 + other_side * 124;
    uint32 phase;
    uint32 value;
    uint32 source;
    uint32 offset;
    uint32 words[4];
    uint32 controller;
    uint32 held;
    uint32 pressed;
    uint32 direction;
    uint32 index;
    uint32 target;
    uint32 pointer;
    uint32 finished = 0;
    FUNCTION_MARKER(0x8010DC30, "SLUS_004.02-BNS-logical-04");
    phase = r_u32(player);
    switch (phase)
    {
    case 0:
        source = 0x800B8D58 + side * 40;
        for (offset = 0; offset < 32; offset += 16)
        {
            words[0] = r_u32(source + offset);
            words[1] = r_u32(source + offset + 4);
            words[2] = r_u32(source + offset + 8);
            words[3] = r_u32(source + offset + 12);
            w_u32(player + 84 + offset, words[0]);
            w_u32(player + 88 + offset, words[1]);
            w_u32(player + 92 + offset, words[2]);
            w_u32(player + 96 + offset, words[3]);
        }
        words[0] = r_u32(source + 32);
        words[1] = r_u32(source + 36);
        w_u32(player + 116, words[0]);
        w_u32(player + 120, words[1]);
        w_u32(player + 44, 0);
        w_u32(player + 40, 0);
        value = r_u32(state + 32);
        value = (sint32)value >= 11 ? (value - 10) * 3 + 60 : 60;
        w_u32(state + 24, value);
        w_u32(state + 20, value * 20);
        w_u32(player + 36, 0);
        w_u32(player + 48, 0);
        if (r_u16(0x800ADFBC + side * 2) != 0)
        {
            w_u32(player + 36, 90);
            w_u32(player + 48, 28);
        }
        t3_bns_04_8010d900(player, side);
        value = r_u32(state);
        if (value == 3 || value == 4 || value == 8)
            w_u32(player, r_u16(0x800AE1F8 + side * 2) != 0 ? 1 : 5);
        else if (value == 5)
            w_u32(player, r_u16(0x800AE1F8 + side * 2) != 0 ? 1 : 2);
        else
        {
            value = t3_80051114(side);
            if (value == 2)
                w_u32(player, 1);
            else if (value == 3)
                w_u32(player, 6);
            else
                w_u32(player, 4);
        }
        break;
    case 1:
    case 3:
        controller = phase == 1 ? side : other_side;
        index = r_u32(player + 4);
        held = r_u16(0x800ADF00 + controller * 2);
        pressed = r_u16(0x800ADF08 + controller * 2);
        direction = (held & 0xAFFF) | (pressed & 0x5000);
        if ((direction & 0xA000) != 0)
            direction &= 0xA000;
        target = 22;
        if (direction == 0x2000)
            target = r_u8(0x801296CC + index * 12);
        else if (direction == 0x1000)
            target = r_u8(0x801296C9 + index * 12);
        else if (direction == 0x4000)
            target = r_u8(0x801296CA + index * 12);
        else if (direction == 0x8000)
            target = r_u8(0x801296CB + index * 12);
        if (target != 22)
        {
            w_u32(player + 4, target);
            w_u32(player + 32, 0);
            w_u32(state + 12, 2);
            t3_800753bc(0, 0x55F5, 0);
        }
        index = r_u32(player + 4);
        value = (uint32)r_s16(0x801296CE + index * 12);
        w_u32(player + 28, value);
        if (phase == 3)
        {
            pressed = r_u16(0x800ADF08 + controller * 2);
            if ((pressed & 0x100) != 0)
            {
                w_u32(other, 1);
                w_u32(player, 2);
                w_u32(state + 12, 2);
                w_u32(player + 28, 22);
                w_u32(player + 32, 0);
                t3_800753bc(0, 0x50F4, 0);
                break;
            }
        }
        pressed = r_u16(0x800ADF08 + controller * 2);
        if (t3_bns_04_8010dab8(state, player, pressed) == 0)
        {
            w_u32(player + 36, 90);
            w_u32(player + 48, 28);
            w_u32(player, phase == 1 ? 6 : 7);
        }
        break;
    case 2:
        if (r_u32(other) == 6)
        {
            value = r_u32(other + 4);
            w_u32(player, 3);
            w_u32(player + 4, value);
        }
        break;
    case 4:
        if (r_u8(0x800AFA8D) != 0 && t3_80050f5c(side) != 0)
        {
            w_u32(player, 0);
            t3_800753bc(0, 0x49A1, 0);
        }
        else
            finished = 1;
        break;
    case 5:
        finished = 1;
        break;
    case 6:
    case 7:
        controller = phase == 6 ? side : other_side;
        if ((r_u16(0x800ADF08 + controller * 2) & 0x800) != 0)
            w_u32(player + 36, 0);
        value = r_u32(player + 36);
        if ((sint32)value > 0)
            w_u32(player + 36, value - 1);
        else
            finished = 1;
        break;
    }
    pointer = 0x800B904C + r_u32(player) * 2;
    value = r_u8(pointer);
    w_u32(player + 8, value);
    value = r_u8(pointer + 1);
    w_u32(player + 12, value);
    return finished;
}

uint32 t3_bns_04_8010dab8(uint32 state, uint32 player, uint32 buttons)
{
    uint32 character;
    uint32 mask;
    uint32 extra = 0;
    uint32 costume;
    uint32 side;
    uint32 offset;
    uint32 first;
    uint32 second;
    uint32 pointer;
    uint32 other;
    FUNCTION_MARKER(0x8010DAB8, "SLUS_004.02-BNS-logical-04");
    character = r_u32(player + 28);
    mask = r_u32(0x80097EF4);
    if ((mask & ((uint32)1 << (character & 31))) != 0)
        extra = 0x810;
    if (((extra | 0xF0) & buttons) == 0 && r_u32(state + 20) != 0)
        return 1;
    costume = (extra & buttons) != 0 ? 2 : ((buttons & 0x60) != 0);
    character = r_u32(player + 28);
    if ((sint32)character < 23)
    {
        w_u32(player + 32, costume);
        for (side = 0; side < 2; ++side)
        {
            offset = side * 124;
            first = r_u32(state + 64 + offset);
            second = r_u32(state + 68 + offset);
            w_u32(state + 104 + offset, first);
            w_u32(state + 108 + offset, second);
            pointer = 0x800B903C + r_u32(state + 36 + offset) * 2;
            first = r_u8(pointer);
            w_u32(state + 112 + offset, first);
            second = r_u8(pointer + 1);
            w_u32(state + 116 + offset, second);
        }
        other = state + 36;
        if (other == player)
            other = state + 160;
        t3_8004f04c(player + 68, other + 68);
        for (side = 0; side < 2; ++side)
        {
            offset = side * 124;
            first = r_u32(state + 104 + offset);
            second = r_u32(state + 108 + offset);
            w_u32(state + 64 + offset, first);
            w_u32(state + 68 + offset, second);
        }
        first = r_u32(player + 28);
        second = r_u32(player + 32);
        w_u32(player + 60, first);
        w_u32(player + 64, second);
        t3_800753bc(0, 0x50F4, 0);
    }
    return (sint32)character >= 22;
}

uint32 t3_bns_04_8010d900(uint32 player, uint32 side)
{
    uint32 mode;
    uint32 character;
    uint32 costume;
    uint32 index;
    uint32 found;
    FUNCTION_MARKER(0x8010D900, "SLUS_004.02-BNS-logical-04");
    mode = t3_80051114(side);
    if (mode == 0)
    {
        character = 22;
        costume = 0;
    }
    else if (mode == 1)
    {
        character = r_u16(0x800ADD5C + side * 2);
        costume = r_u16(0x800ADD98 + side * 2);
    }
    else
    {
        if (mode == 2)
        {
            character = r_u32(player + 60);
            costume = r_u32(player + 64);
        }
        else
        {
            /* The reviewed controller getter returns only zero through three */
            character = r_u16(0x800ADD5C + side * 2);
            costume = r_u16(0x800ADD98 + side * 2);
        }
        found = 0;
        if ((sint32)character < 22)
        {
            for (index = 0; index < 22; ++index)
            {
                if (character == (uint32)r_s16(0x801296CE + index * 12))
                {
                    w_u32(player + 4, index);
                    found = 1;
                    break;
                }
            }
        }
        if (found == 0)
        {
            index = r_u16(player + 120);
            w_u32(player + 4, index);
            character = (uint32)r_s16(0x801296CE + index * 12);
        }
    }
    w_u32(player + 20, 22);
    w_u32(player + 28, character);
    w_u32(player + 32, costume);
    w_u32(player + 24, 1);
    return 1;
}

uint32 t3_bns_04_8010d5e4(uint32 state)
{
    T3_SELECT_CHAIN chains[2];
    uint32 row;
    uint32 side;
    uint32 entry;
    uint32 mask;
    uint32 enabled = 0;
    uint32 character;
    uint32 count;
    uint32 found;
    uint32 regular;
    uint32 previous;
    uint32 next_id;
    uint32 previous_id;
    FUNCTION_MARKER(0x8010D5E4, "SLUS_004.02-BNS-logical-04");
    w_u32(state + 16, 0);
    mask = r_u32(0x80097EF0) & 0x1FFFFF;
    for (row = 0; row < 22; ++row)
    {
        entry = 0x801296C8 + row * 12;
        w_u8(entry, row);
        character = r_u8(0x800B9018 + row);
        if (((mask >> (character & 31)) & 1) != 0)
        {
            w_u16(entry + 6, character);
            if (row % 11 < 10)
                w_u8(entry + 5, 1);
            else
            {
                w_u8(entry + 5, 0);
                w_u32(state + 16, 180);
            }
            enabled |= (uint32)1 << (character & 31);
        }
        else
        {
            w_u16(entry + 6, 22);
            w_u8(entry + 5, 0);
        }
        w_u8(entry + 1, 22);
        w_u8(entry + 2, 22);
        w_u8(entry + 3, 22);
        w_u8(entry + 4, 22);
    }
    count = t3_8004ccac(enabled);
    w_u32(state + 32, count);
    w_u32(state + 28, enabled);
    for (side = 0; side < 2; ++side)
    {
        chains[side].side = (uint8)side;
        chains[side].phase = 0;
        chains[side].alternate = 0;
        chains[side].pause_at = 22;
        chains[side].leading = 0;
        chains[side].remaining = 0;
        chains[side].trailing = 0;
        chains[side].first = 0x801296C8 + side * 132;
        chains[side].current = chains[side].first;
        chains[side].last = chains[side].first;
        chains[side].x = 11;
        chains[side].y = (uint16)(side * 62);
    }
    for (side = 0; side < 2; ++side)
    {
        previous = chains[side].first;
        found = 0;
        count = 0;
        regular = 0;
        for (row = 0; row < 11; ++row)
        {
            entry = chains[side].first + row * 12;
            if (r_s16(entry + 6) != 22)
            {
                if (found != 0)
                {
                    next_id = r_u8(entry);
                    previous_id = r_u8(previous);
                    w_u8(previous + 4, next_id);
                    w_u8(entry + 3, previous_id);
                }
                else
                {
                    chains[side].current = entry;
                    found = 1;
                }
                ++count;
                previous = entry;
                if (row < 10)
                    ++regular;
            }
        }
        previous_id = r_u8(previous);
        next_id = r_u8(chains[side].current);
        w_u8(previous + 4, next_id);
        w_u8(chains[side].current + 3, previous_id);
        chains[side].last = previous;
        chains[side].total = (uint8)count;
        chains[side].remaining = (uint8)regular;
    }
    w_u32(state + 4, chains[1].remaining != 0);
    return t3_bns_04_8010d410(&chains[0], &chains[1]);
}

uint32 t3_bns_04_8010d410(T3_SELECT_CHAIN *first, T3_SELECT_CHAIN *second)
{
    uint32 first_count;
    uint32 second_count;
    T3_SELECT_CHAIN *longer;
    T3_SELECT_CHAIN *shorter;
    uint32 difference;
    uint32 value;
    uint32 x;
    uint32 long_entry;
    uint32 short_entry;
    uint32 count;
    uint32 row;
    uint32 offset;
    uint32 result;
    FUNCTION_MARKER(0x8010D410, "SLUS_004.02-BNS-logical-04");
    first_count = first->remaining;
    second_count = second->remaining;
    longer = first_count < second_count ? second : first;
    shorter = first_count < second_count ? first : second;
    if (longer->remaining == 0)
    {
        longer->current = 0x800B9030;
        longer->phase = 3;
    }
    if (shorter->remaining != 0)
    {
        value = longer->remaining;
        difference = value - shorter->remaining;
        if ((difference & 1) != 0)
            shorter->alternate = 1;
        value = shorter->remaining;
        shorter->trailing = (uint8)(difference >> 1);
        shorter->leading = (uint8)(difference >> 1);
        shorter->pause_at = (uint8)((value >> 1) + 1);
        value = (10 - shorter->remaining) * 35;
        x = shorter->x;
        result = (uint32)((sint32)value >> 1);
        shorter->x = (uint16)(x + result);
    }
    else
    {
        shorter->current = 0x800B9030;
        result = 3;
        shorter->phase = 3;
    }
    long_entry = longer->current;
    count = longer->remaining;
    short_entry = shorter->current;
    row = 0;
    while (row < count)
    {
        if (longer->phase < 3)
        {
            offset = longer->side != 0 ? 1 : 2;
            w_u8(long_entry + offset, r_u8(short_entry));
        }
        if (shorter->phase < 3)
        {
            offset = shorter->side != 0 ? 1 : 2;
            w_u8(short_entry + offset, r_u8(long_entry));
        }
        long_entry = t3_bns_04_8010d1f0(longer, long_entry);
        short_entry = t3_bns_04_8010d1f0(shorter, short_entry);
        ++row;
        result = row < count;
    }
    return result;
}

uint32 t3_bns_04_8010d1f0(T3_SELECT_CHAIN *control, uint32 entry)
{
    uint32 phase;
    uint32 value;
    uint32 remaining;
    uint32 x;
    FUNCTION_MARKER(0x8010D1F0, "SLUS_004.02-BNS-logical-04");
    phase = control->phase;
    if (phase == 2)
        goto finalize_entry;
    if (phase != 1)
    {
        if (phase != 0)
            return entry;
        if (control->leading != 0)
        {
            value = control->side != 0 ? r_u8(entry + 1) : r_u8(entry + 2);
            w_u8(entry + 3, value);
            control->leading = (uint8)(control->leading - 1);
            return entry;
        }
        if (control->alternate != 0)
        {
            value = control->side != 0 ? r_u8(entry + 1) : r_u8(entry + 2);
            w_u8(entry + 3, value);
        }
        control->phase = (uint8)(control->phase + 1);
    }
    remaining = control->remaining;
    if (remaining != 0)
    {
        if (control->alternate != 0 && remaining == control->pause_at)
        {
            control->pause_at = 255;
            return entry;
        }
        w_u16(entry + 8, control->x);
        w_u16(entry + 10, control->y);
        x = control->x;
        remaining = (control->remaining - 1) & 255;
        control->remaining = (uint8)remaining;
        control->x = (uint16)(x + 35);
        if (remaining != 0)
            return 0x801296C8 + r_u8(entry + 4) * 12;
    }
    control->phase = (uint8)(control->phase + 1);
finalize_entry:
    if (control->alternate != 0)
    {
        value = control->side != 0 ? r_u8(entry + 1) : r_u8(entry + 2);
        w_u8(entry + 4, value);
    }
    else if (control->trailing != 0)
    {
        value = control->side != 0 ? r_u8(entry + 1) : r_u8(entry + 2);
        w_u8(entry + 4, r_u8(0x801296CC + value * 12));
    }
    control->phase = (uint8)(control->phase + 1);
    return entry;
}

uint32 t3_bns_04_8010e328(uint32 state, uint32 selection)
{
    uint32 side;
    uint32 record;
    uint32 character;
    uint32 costume;
    uint32 mode;
    uint32 copy;
    FUNCTION_MARKER(0x8010E328, "SLUS_004.02-BNS-logical-04");
    for (side = 0; side < 2; ++side)
    {
        record = selection + side * 124;
        character = r_u32(record + 96);
        costume = r_u32(record + 100);
        mode = r_u32(state);
        if (mode != 5 || r_u16(0x800AE1F8 + side * 2) != 0)
        {
            w_u8(0x80097F0E + side, (character << 2) | (costume & 3));
            copy = r_u16(0x800AE1F8 + side * 2) != 0;
        }
        else
            copy = 0;
        if (copy == 0)
            copy = r_u32(selection) == 5;
        if (copy != 0)
        {
            w_u16(0x800ADD5C + side * 2, r_u16(record + 96));
            w_u16(0x800ADD98 + side * 2, r_u16(record + 100));
            w_u16(0x800ADFBC + side * 2, 1);
            w_u16(0x800A9128 + side * 2, 0);
        }
    }
    return 0;
}

void t3_800f18e8(void)
{
    t3_bns_09_800f18e8();
}

uint32 t3_bns_09_800f18e8(void)
{
    uint32 side;
    uint32 phase;
    uint32 value;
    uint32 pointer;
    uint32 character;
    uint32 costume;
    uint32 ordering;
    uint32 packet;
    uint32 level;
    uint32 buffer;
    uint32 saved_packet;
    uint32 y = 120;
    uint32 flags;
    uint32 color;
    uint32 result;
    FUNCTION_MARKER(0x800F18E8, "SLUS_004.02-BNS-logical-09");
    w_u32(0x800FA840, r_u32(0x800FA840) + 1);
    side = r_u8(0x800AFAC6);
    if ((r_u16(0x800ADF08 + side * 2) & 0x800) != 0)
    {
        if (r_s16(0x800AE224) == 2)
            w_u16(0x800AE224, 3);
    }
    phase = (uint32)r_s16(0x800AE224);
    switch (phase)
    {
    case 0:
        t3_bns_09_800f0aec();
        t3_8004f8b4();
        t3_8006ae14(6, 0);
        side &= 1;
        pointer = 0x800FA614 + side * 8;
        w_u32(0x800FA840, 0);
        w_u32(0x800FA858, 0);
        w_u16(0x800FA830, r_u16(pointer));
        w_u16(0x800FA832, r_u16(pointer + 2));
        w_u16(0x800FA834, r_u16(pointer + 4));
        w_u16(0x800FA836, r_u16(pointer + 6));
        character = r_u16(0x800ADD5C + side * 2);
        costume = r_u16(0x800ADD98 + side * 2);
        pointer = t3_8004b790((sint32)character, (sint32)costume);
        w_u32(0x800FA838, r_u32(pointer) >> 8);
        value = r_u8(pointer);
        w_u32(0x800FA844, 0);
        w_u32(0x800FA848, 0);
        w_u32(0x800FA84C, 0);
        w_u32(0x800FA850, 0);
        w_u16(0x800AE224, 1);
        w_u16(0x800FA83C, (value >> 5) & 1);
        t3_80075834();
        t3_8004b688(1);
        break;
    case 1:
        value = r_u32(0x800FA858) + 8;
        w_u32(0x800FA858, value);
        if ((sint32)value >= 256)
        {
            w_u32(0x800FA858, 256);
            w_u16(0x800AE224, 2);
        }
        break;
    case 2:
        if (r_s32(0x800FA840) >= 4681)
            w_u16(0x800AE224, 3);
        break;
    case 3:
        t3_8006b6c8(60);
        w_u32(0x800FA858, 256);
        w_u16(0x800AE224, 4);
        break;
    case 4:
        value = r_u32(0x800FA858) - 4;
        w_u32(0x800FA858, value);
        if ((sint32)value <= 0)
        {
            w_u32(0x800FA858, 0);
            w_u16(0x800AE224, 5);
        }
        break;
    case 5:
        w_u16(0x800AE204, 16);
        w_u16(0x800AE224, 0);
        break;
    }
    if (r_s16(0x800AE224) >= 2)
        w_u32(0x800FA844, r_u32(0x800FA844) + 1);
    ordering = r_u32(0x800A9218) + 28;
    level = r_u32(0x800FA858);
    buffer = r_u32(0x800ADEFC);
    saved_packet = r_u32(0x800ADF64);
    packet = 0x800FA860 + buffer * 15360;
    w_u32(0x800ADF64, packet);
    packet = t3_8004e000(ordering, packet, (sint32)level);
    w_u32(0x800ADF64, packet);
    t3_8004ce74(0x800B9264, 0, 2, 52, 28);
    if (r_s32(0x800FA844) >= 30)
    {
        t3_8004ce74(0x800B927C, 1, 5, 158, y);
        value = r_u32(0x800AFAC0);
        y = 198;
        t3_8004ce74(0x800B9290, 1, 6, 249, 146, value);
        if (r_u32(0x800FA844) == 30)
            t3_800753bc(0, 0x4C6C, 0);
    }
    if (r_s32(0x800FA844) >= 60)
    {
        t3_8004ce74(0x800B929C, 1, 5, 158, y);
        y += 26;
        value = t3_bns_02_800b5f20();
        t3_8004ce74(0x800B9290, 1, 6, 249, y, value);
        value = r_u8(0x800AFAC7);
        y += 26;
        if (value != 0)
        {
            color = (r_u32(0x800FA840) & 2) != 0 ? 6 : 2;
            t3_8004ce74(0x800B92B0, 1, color, 197, y);
        }
        if (r_u32(0x800FA844) == 60)
            t3_800753bc(0, 0x4C6C, 0);
    }
    flags = r_u8(0x800AFAC8);
    y += 78;
    if ((flags & 0x10) != 0)
    {
        value = (uint32)(r_s32(0x800FA844) >= 90);
        if (value != 0)
            w_u32(0x800FA848, r_u32(0x800FA848) + 1);
        y += 26;
        if (value != 0)
        {
            t3_8004ce74(0x800B934C, 1, 5, 39, y);
            ordering = r_u32(0x800A9218);
            packet = r_u32(0x800ADF64);
            value = r_u8(0x800AFAC9);
            packet = t3_8004b928(ordering, packet, 52, y + 26, value, 0);
            value = r_u32(0x800FA844);
            w_u32(0x800ADF64, packet);
            if (value == 90)
                t3_800753bc(0, 0x4C6C, 0);
        }
        if (r_s32(0x800FA848) >= 20)
        {
            ordering = r_u32(0x800A9218);
            packet = r_u32(0x800ADF64);
            value = r_u8(0x800AFACA);
            packet = t3_8004b928(ordering, packet, 88, y + 26, value, 0);
            value = r_u32(0x800FA848);
            w_u32(0x800ADF64, packet);
            if (value == 20)
                t3_800753bc(0, 0x4C6C, 0);
        }
        if (r_s32(0x800FA848) >= 40)
        {
            ordering = r_u32(0x800A9218);
            packet = r_u32(0x800ADF64);
            value = r_u8(0x800AFACB);
            packet = t3_8004b928(ordering, packet, 124, y + 26, value, 0);
            value = r_u32(0x800FA848);
            w_u32(0x800ADF64, packet);
            if (value == 40)
                t3_800753bc(0, 0x4C6C, 0);
        }
        if (r_s32(0x800FA848) >= 60)
        {
            ordering = r_u32(0x800A9218);
            packet = r_u32(0x800ADF64);
            value = r_u8(0x800AFACC);
            packet = t3_8004b928(ordering, packet, 160, y + 26, value, 0);
            value = r_u32(0x800FA848);
            w_u32(0x800ADF64, packet);
            if (value == 60)
                t3_800753bc(0, 0x4C6C, 0);
        }
        value = r_u32(0x800FA84C);
        if (value == 1)
        {
            t3_8004ce74(0x800B935C, 1, 5, 223, y);
            value = r_u32(0x800FA850);
            t3_8004ce74(0x800B936C, 1, 6, 301, y + 26, value);
            if ((r_u32(0x800FA844) & 1) == 0)
            {
                w_u32(0x800FA850, r_u32(0x800FA850) + 1);
                t3_800753bc(0, 0x55F5, 0);
            }
            value = r_u16(0x800AFAC4);
            character = r_u32(0x800FA850);
            if ((sint32)value < (sint32)character)
                w_u32(0x800FA84C, 2);
            else if (r_s16(0x800AE224) >= 3)
                w_u32(0x800FA84C, 2);
        }
        else if (value == 0)
        {
            if (r_s32(0x800FA848) >= 80)
            {
                w_u32(0x800FA84C, 1);
                w_u32(0x800FA850, 0);
            }
        }
        else if (value == 2)
        {
            t3_8004ce74(0x800B935C, 1, 5, 223, y);
            value = r_u16(0x800AFAC4);
            t3_8004ce74(0x800B936C, 1, 6, 301, y + 26, value);
        }
    }
    else if ((flags & 8) != 0)
    {
        if (r_s32(0x800FA844) >= 90)
        {
            color = (r_u32(0x800FA840) & 2) != 0 ? 6 : 2;
            t3_8004ce74(0x800B92C8, 1, 8, 15, y + 104, color);
            if (r_u32(0x800FA844) == 90)
                t3_800753bc(0, 0x50F4, 0);
        }
    }
    else
    {
        if (r_s32(0x800FA844) >= 90)
            t3_8004ce74(0x800B92F0, 1, 5, 158, y);
        if ((r_u8(0x800AFAC8) & 1) != 0 && r_s32(0x800FA844) >= 90)
        {
            t3_8004ce74(0x800B9300, 1, 2, 223, y);
            if (r_u32(0x800FA844) == 90)
                t3_800753bc(0, 0x4C6C, 0);
        }
        if ((r_u8(0x800AFAC8) & 2) != 0 && r_s32(0x800FA844) >= 120)
        {
            t3_8004ce74(0x800B9310, 1, 6, 223, y + 26);
            if (r_u32(0x800FA844) == 120)
                t3_800753bc(0, 0x4C6C, 0);
        }
        if ((r_u8(0x800AFAC8) & 4) != 0 && r_s32(0x800FA844) >= 150)
        {
            t3_8004ce74(0x800B9320, 1, 5, 223, y + 52);
            if (r_u32(0x800FA844) == 150)
                t3_800753bc(0, 0x4C6C, 0);
        }
        value = r_u32(0x800FA844);
        if ((sint32)value >= 180 && (value & 0x30) != 0)
            t3_8004ce74(0x800B9330, 1, 8, 67, y + 104);
    }
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    packet = t3_bns_09_800f0b30(ordering, packet);
    w_u32(0x800ADF64, packet);
    result = t3_bns_09_800f15e0(16, 100);
    w_u32(0x800ADF64, saved_packet);
    return result;
}

void t3_800f0458(void)
{
    t3_bns_09_800f0458();
}

uint32 t3_bns_09_800f0458(void)
{
    uint32 phase;
    uint32 value;
    uint32 first;
    uint32 second;
    uint32 row;
    uint32 packet;
    uint32 saved_packet;
    uint32 ordering;
    uint32 level;
    uint32 buffer;
    uint32 completed;
    FUNCTION_MARKER(0x800F0458, "SLUS_004.02-BNS-logical-09");
    w_u32(0x800FA824, r_u32(0x800FA824) + 1);
    first = r_u16(0x800ADF08);
    second = r_u16(0x800ADF0A);
    if (((first | second) & 0x800) != 0)
    {
        phase = r_u16(0x800AE224);
        if (phase - 2 < 6)
            w_u16(0x800AE224, 8);
    }
    phase = (uint32)r_s16(0x800AE224);
    switch (phase)
    {
    case 0:
    {
        uint32 pairs[44];
        uint32 filled = 0;
        uint32 mask = 0;
        uint32 count;
        t3_8004f8b4();
        t3_8006ae14(6, 0);
        t3_8004b688(1);
        t3_80075834();
        w_u32(0x800FA824, 0);
        w_u32(0x800FA858, 0);
        for (row = 0; row < 22; ++row)
        {
            if (((0x001FFFFFu >> row) & 1) != 0)
            {
                value = r_u16(0x800AFAE8 + row * 2);
                if (value != 0)
                {
                    pairs[filled * 2] = row;
                    pairs[filled * 2 + 1] = value;
                    ++filled;
                    mask |= 1u << row;
                }
            }
        }
        count = t3_8004ccac(mask);
        w_u32(0x800FA7E0, count);
        t3_8004cd80(pairs, count, 0);
        row = 0;
        value = r_u32(0x800FA7E0);
        if ((sint32)value > 0)
        {
            do
            {
                w_u8(0x800FA7F0 + row, pairs[row * 2] & 0xFF);
                value = r_u32(0x800FA7E0);
                ++row;
            } while ((sint32)row < (sint32)value);
        }
        w_u32(0x800AE214, 0);
        w_u32(0x800FA808, 0);
        w_u32(0x800FA818, 2);
        w_u32(0x800FA81C, 30);
        w_u32(0x800FA820, 0);
        w_u16(0x800AE224, 1);
        w_u32(0x800FA80C, r_u32(0x800AFAC0));
        w_u32(0x800FA814, r_u32(0x800AFAC4));
        w_u32(0x800FA810, r_u32(0x800AFAC8));
        break;
    }
    case 1:
        value = r_u32(0x800FA858) + 8;
        w_u32(0x800FA858, value);
        if ((sint32)value >= 256)
        {
            w_u32(0x800FA858, 256);
            w_u16(0x800AE224, 2);
            first = r_u32(0x800FA7E0);
            w_u32(0x800FA808, 0);
            w_u32(0x800FA828, 0);
            if ((sint32)first <= 0)
            {
                w_u16(0x800AE224, 4);
                w_u32(0x800FA808, first);
            }
        }
        break;
    case 2:
        w_u32(0x800FA828, 1);
        w_u32(0x800AE214, 4);
        value = r_u32(0x800FA820);
        w_u16(0x800AE224, 3);
        w_u32(0x800FA820, value + 1);
        /* Fall through to decrement the new counter */
    case 3:
        value = r_u32(0x800AE214) - 1;
        w_u32(0x800AE214, value);
        if ((sint32)value <= 0)
        {
            t3_800753bc(0, 0x55F5, 0);
            first = r_u32(0x800FA808);
            value = r_u8(0x800FA7F0 + first);
            second = r_u32(0x800FA828);
            value = r_u16(0x800AFAE8 + value * 2);
            if (second < value)
            {
                w_u32(0x800AE214, 4);
                value = r_u32(0x800FA820);
                w_u32(0x800FA828, second + 1);
                w_u32(0x800FA820, value + 1);
            }
            else
            {
                value = r_u32(0x800FA7E0);
                ++first;
                w_u32(0x800AE214, 0);
                w_u32(0x800FA808, first);
                if ((sint32)first < (sint32)value)
                    w_u16(0x800AE224, 2);
                else
                {
                    w_u16(0x800AE224, 4);
                    w_u32(0x800FA808, value);
                }
            }
        }
        break;
    case 4:
        w_u32(0x800AE214, 4);
        w_u16(0x800AE224, 5);
        break;
    case 5:
        w_u32(0x800FA818, 2);
        w_u32(0x800FA81C, 30);
        w_u16(0x800AE224, 6);
        /* Fall through to decrement the new countdown */
    case 6:
        value = r_u32(0x800FA81C) - 1;
        w_u32(0x800FA81C, value);
        if (value == 0)
        {
            w_u32(0x800FA81C, 0);
            t3_800753bc(0, 0x50F4, 0);
            if (r_u32(0x800FA810) != 0)
            {
                t3_8004efc8(r_u32(0x800FA814));
                value = t3_800757a8(r_u32(0x800FA80C));
                if (value != 0)
                    t3_800753bc(r_u32(0x800FA80C), 0x86CA, 0);
                else
                    t3_8007584c();
            }
            w_u16(0x800AE224, 7);
        }
        break;
    case 7:
        if (r_s32(0x800FA824) < 4680)
            break;
        w_u16(0x800AE224, 8);
        /* Fall through to start the fade */
    case 8:
        w_u32(0x800FA818, 0);
        t3_8006b6c8(60);
        w_u32(0x800FA858, 256);
        w_u16(0x800AE224, 9);
        break;
    case 9:
        value = r_u32(0x800FA858) - 4;
        w_u32(0x800FA858, value);
        if ((sint32)value <= 0)
        {
            w_u32(0x800FA858, 0);
            w_u16(0x800AE224, 10);
        }
        break;
    case 10:
        w_u16(0x800AE204, 16);
        w_u16(0x800AE224, 0);
        break;
    case 11:
        w_u16(0x800AE204, 8);
        w_u16(0x800AE224, 0);
        break;
    }
    t3_80075864();
    ordering = r_u32(0x800A9218) + 28;
    level = r_u32(0x800FA858);
    buffer = r_u32(0x800ADEFC);
    saved_packet = r_u32(0x800ADF64);
    packet = 0x800FA860 + buffer * 15360;
    w_u32(0x800ADF64, packet);
    packet = t3_8004e000(ordering, packet, (sint32)level);
    w_u32(0x800ADF64, packet);
    t3_8004ce74(0x800B90F0, 6, 1, 82, 28);
    if (r_s32(0x800FA818) >= 2)
    {
        value = r_u32(0x800FA820);
        t3_bns_09_800efea4((sint32)value, 120, 352);
        t3_8004ce74(0x800B90E0, 6, 1, 84, 328);
        if (r_u32(0x800FA81C) == 0)
            t3_bns_09_800f006c(r_s32(0x800FA810), 400);
    }
    completed = r_u32(0x800FA808);
    for (row = 0; (sint32)row < (sint32)completed; ++row)
    {
        uint32 x = r_u16(0x800B8FEC + row * 4);
        uint32 y = r_u16(0x800B8FEE + row * 4);
        uint32 character = r_u8(0x800FA7F0 + row);
        if ((sint32)row < (sint32)(completed - 1))
            value = r_u16(0x800AFAE8 + character * 2);
        else
            value = r_u32(0x800FA828);
        t3_bns_09_800f0238((sint32)value, character << 2, 64, x, y);
    }
    t3_bns_09_800f12dc(0xA0C000);
    w_u32(0x800ADF64, saved_packet);
    return 0x800B0000;
}

void t3_800eee8c(void)
{
    t3_bns_09_800eee8c();
}

uint32 t3_bns_09_800eee8c(void)
{
    uint32 phase;
    uint32 value;
    uint32 first;
    uint32 second;
    uint32 row;
    uint32 ordering;
    uint32 buffer;
    uint32 level;
    uint32 packet;
    uint32 saved_packet;
    FUNCTION_MARKER(0x800EEE8C, "SLUS_004.02-BNS-logical-09");
    if (t3_8005101c() != 0)
        return 1;
    if (r_s16(0x800AE224) >= 2)
    {
        for (row = 0; row < 2; ++row)
        {
            if ((r_u16(0x800ADF08 + row * 2) & 0x800) != 0)
            {
                w_u16(0x800AE1F8 + row * 2, 1);
                t3_8005118c(row);
                w_u16(0x800AE224, 10);
            }
        }
    }
    phase = (uint32)r_s16(0x800AE224);
    switch (phase)
    {
    case 0:
        t3_8006ae14(6, 0);
        t3_8004b688(1);
        first = 0;
        second = 0;
        value = r_u8(0x800AFAE0);
        for (row = 0; row < value; ++row)
        {
            uint32 kind;
            w_u32(0x800FA658 + row * 4, first);
            w_u32(0x800FA708 + row * 4, second);
            kind = r_u16(0x800AFAC0 + row * 2);
            if (kind == 2 || kind == 3)
                ++first;
            if (kind == 1 || kind == 3)
                ++second;
        }
        w_u32(0x800FA628, 0);
        w_u32(0x800FA858, 0);
        w_u32(0x800FA630, 0);
        w_u32(0x800FA634, 0);
        w_u32(0x800FA648, 0);
        w_u32(0x800FA640, 0);
        w_u32(0x800FA644, 0);
        w_u16(0x800AE224, 1);
        w_u32(0x800FA638, r_u8(0x800AFAED));
        w_u32(0x800FA63C, r_u8(0x800AFAFA));
        break;
    case 1:
        value = r_u32(0x800FA858) + 8;
        w_u32(0x800FA858, value);
        if ((sint32)value >= 256)
        {
            w_u32(0x800FA858, 256);
            w_u16(0x800AE224, 2);
        }
        break;
    case 2:
        value = r_u32(0x800FA648) + 1;
        w_u32(0x800FA648, value);
        if ((sint32)value < 9)
            break;
        value = r_u32(0x800FA628);
        w_u32(0x800FA648, 8);
        value = r_u16(0x800AFAC0 + value * 2);
        if (value == 2 || value == 3)
        {
            first = r_u32(0x800FA630);
            second = r_u32(0x800FA638);
            w_u32(0x800FA630, first + 1);
            w_u32(0x800FA638, second - 1);
        }
        if (value == 1 || value == 3)
        {
            first = r_u32(0x800FA634);
            second = r_u32(0x800FA63C);
            w_u32(0x800FA634, first + 1);
            w_u32(0x800FA63C, second - 1);
        }
        w_u32(0x800AE214, 0);
        w_u16(0x800AE224, 3);
        break;
    case 3:
        value = r_u32(0x800AE214) + 1;
        w_u32(0x800AE214, value);
        if ((sint32)value >= 2)
        {
            t3_800753bc(0, 0x55F5, 0);
            w_u32(0x800FA648, 0);
            value = r_u32(0x800FA628);
            first = r_u8(0x800AFAE0);
            ++value;
            w_u32(0x800FA628, value);
            w_u16(0x800AE224, (sint32)value < (sint32)first ? 2 : 4);
        }
        break;
    case 4:
        w_u32(0x800FA640, 1);
        w_u32(0x800FA644, 30);
        w_u16(0x800AE224, 5);
        /* Fall through to decrement the new countdown */
    case 5:
        value = r_u32(0x800FA644) - 1;
        w_u32(0x800FA644, value);
        if ((sint32)value <= 0)
        {
            w_u32(0x800FA644, 0);
            t3_800753bc(0, 0x50F4, 0);
            w_u16(0x800AE224, 6);
        }
        break;
    case 6:
        w_u32(0x800AE214, 0);
        w_u16(0x800AE224, 7);
        /* Fall through to handle the result screen input */
    case 7:
        first = r_u16(0x800ADF08);
        second = r_u16(0x800ADF0A);
        value = first | second;
        if (value == 0x100)
        {
            w_u16(0x800AE224, 0);
            break;
        }
        if ((value & 0xF0) != 0)
            w_u32(0x800FA640, (r_u32(0x800FA640) + 1) & 1);
        value = r_u32(0x800AE214) + 1;
        w_u32(0x800AE214, value);
        if ((sint32)value >= 1800)
        {
            w_u32(0x800FA640, 0);
            w_u32(0x800FA858, 256);
            w_u16(0x800AE224, 8);
        }
        break;
    case 8:
        value = r_u32(0x800FA858) - 4;
        w_u32(0x800FA858, value);
        if ((sint32)value <= 0)
        {
            w_u32(0x800FA858, 0);
            w_u16(0x800AE224, 9);
        }
        break;
    case 9:
        w_u16(0x800AE204, 16);
        w_u16(0x800AE224, 0);
        break;
    case 10:
        w_u16(0x800AE204, 8);
        w_u16(0x800AE224, 0);
        break;
    }
    ordering = r_u32(0x800A9218) + 28;
    buffer = r_u32(0x800ADEFC);
    level = r_u32(0x800FA858);
    packet = 0x800FA860 + buffer * 15360;
    saved_packet = r_u32(0x800ADF64);
    w_u32(0x800ADF64, packet);
    packet = t3_8004e000(ordering, packet, (sint32)level);
    w_u32(0x800ADF64, packet);
    t3_8004ce74(0x800B8E88, 6, 1, 63, 28);
    t3_bns_09_800edda0(0x800AFA88);
    t3_bns_09_800edae0(0x800AFA88);
    first = r_u32(0x800FA628);
    second = r_u32(0x800FA648);
    /* The final page32 call leaves A3 unchanged through its SDK callees */
    t3_bns_09_800eeab8(0x800AFA88, first, (sint32)second, 32);
    first = r_u32(0x800FA630);
    t3_bns_09_800ee074(0x800AFAE1, (sint32)first, 40, 100, 0);
    first = r_u8(0x800AFAED);
    second = r_u8(0x800AFAFA);
    value = r_u32(0x800FA634);
    t3_bns_09_800ee074(0x800AFAEE, (sint32)value, 40,
        (5u - (first + second + 1) / 3) * 30 + 192, 1);
    value = r_u32(0x800FA628);
    t3_bns_09_800ee250(0x800AFA88, value, 0x800AFAE1);
    value = t3_bns_09_800f12dc(0x1830F0);
    w_u32(0x800ADF64, saved_packet);
    return value;
}

uint32 t3_bns_09_800f15e0(uint32 x, uint32 y)
{
    uint32 texture_y;
    uint32 packet;
    uint32 ordering;
    uint32 texture_x;
    uint32 left;
    uint32 right;
    uint32 page;
    uint32 clut;
    uint32 screen_y = y + 6;
    uint32 remaining = 212;
    uint32 consumed = 20;
    uint32 boundary = 64;
    uint32 row;
    uint32 color;
    uint32 top;
    uint32 middle;
    uint32 bottom;
    FUNCTION_MARKER(0x800F15E0, "SLUS_004.02-BNS-logical-09");
    texture_y = (r_u16(0x800FA832) + 20) & 0xFF;
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218) + 4;
    texture_x = (r_u16(0x800FA830) << 1) & 0x7F;
    if (r_u16(0x800FA83C) != 0)
    {
        left = (x + 128) & 0xFFFF;
        right = (x + 2) & 0xFFFF;
    }
    else
    {
        left = (x + 2) & 0xFFFF;
        right = (x + 128) & 0xFFFF;
    }
    page = r_u16(0x800FA836);
    clut = r_u16(0x800FA834);
    do
    {
        uint32 height;
        uint32 start_v;
        uint32 end_v;
        uint32 strip_top;
        uint32 strip_bottom;
        if (boundary < consumed + remaining)
        {
            height = boundary - consumed;
            boundary += 64;
        }
        else
            height = remaining;
        strip_top = screen_y << 16;
        screen_y += height;
        strip_bottom = screen_y << 16;
        start_v = (texture_y & 0xFF) << 8;
        texture_y += height;
        end_v = (texture_y & 0xFF) << 8;
        packet = t3_8004d830(ordering, packet, 0x2D000000,
            left | strip_top, right | strip_top,
            left | strip_bottom, right | strip_bottom,
            texture_x | start_v | (clut << 16),
            (texture_x + 125) | start_v | (page << 16),
            texture_x | end_v | (((texture_x + 125) | end_v) << 16));
        clut += 4;
        remaining -= height;
        consumed += height;
    } while (remaining != 0);
    for (row = 0; row < 4; ++row)
    {
        uint32 descriptor = 0x800B9244 + row * 8;
        uint32 border_x = (uint32)r_s16(descriptor);
        uint32 border_y = (uint32)r_s16(descriptor + 2);
        uint32 height = (uint32)r_s16(descriptor + 6);
        uint32 width = r_u16(descriptor + 4);
        packet = t3_8004da10(ordering, packet, 0x60F0F0F0,
            ((border_x + x) & 0xFFFF) | ((border_y + y) << 16),
            width | (height << 16));
    }
    left = (x + 2) & 0xFFFF;
    right = (x + 128) & 0xFFFF;
    top = (y + 6) << 16;
    middle = (y + 112) << 16;
    color = r_u32(0x800FA838);
    packet = t3_8004d8b0(ordering, packet, 0x3A000000, 0, color, color,
        left | top, right | top, left | middle, right | middle);
    bottom = (y + 218) << 16;
    packet = t3_8004d8b0(ordering, packet, color | 0x3A000000, color, 0xC0C0C0, 0xC0C0C0,
        left | middle, right | middle, left | bottom, right | bottom);
    packet = t3_8004daa8(ordering, packet, 32);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_09_800f12dc(uint32 color)
{
    uint32 scroll;
    uint32 packet;
    uint32 ordering;
    uint32 row;
    uint32 start_x;
    uint32 ticker;
    uint32 top_left;
    uint32 top_right;
    uint32 bottom_left;
    uint32 bottom_right;
    sint32 edge;
    sint32 width;
    FUNCTION_MARKER(0x800F12DC, "SLUS_004.02-BNS-logical-09");
    scroll = r_u32(0x80102070) + 383;
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    scroll = (uint32)((sint32)scroll % 384);
    start_x = scroll - 384;
    w_u32(0x80102070, scroll);
    for (row = 0; row < 7; ++row)
    {
        uint32 descriptor = r_u32(0x800B9218 + row * 4);
        uint32 v = r_u8(0x800B9219 + row * 4);
        uint32 u = descriptor & 0xFF;
        uint32 right_u = (u + 24) & 0xFF;
        uint32 bottom_v = ((v + 40) & 0xFF) << 8;
        uint32 x = start_x;
        while ((sint32)x < 368)
        {
            if ((sint32)x >= -47)
            {
                uint32 left = x & 0xFFFF;
                uint32 right = (x + 48) & 0xFFFF;
                packet = t3_8004d830(ordering, packet, 0x2C606060,
                    left | 0x005F0000, right | 0x005F0000,
                    left | 0x00AF0000, right | 0x00AF0000,
                    u | (v << 8) | (descriptor & 0xFFFF0000),
                    right_u | (v << 8) | 0x006E0000,
                    u | bottom_v | ((right_u | bottom_v) << 16));
            }
            x += 384;
        }
        start_x += 48;
    }
    edge = r_s16(0x800AE230);
    ticker = r_u32(0x80102074);
    width = r_s16(0x800AE234);
    top_left = ((uint32)edge & 0xFFFF) | 0x00CB0000;
    top_right = ((uint32)(edge + width) & 0xFFFF) | 0x00CB0000;
    ticker += 834;
    bottom_left = r_u16(0x800AE230) | 0x00F50000;
    edge = r_s16(0x800AE230);
    width = r_s16(0x800AE234);
    bottom_right = ((uint32)(edge + width) & 0xFFFF) | 0x00F50000;
    ticker %= 836;
    w_u32(0x80102074, ticker);
    packet = t3_8004d7d4(ordering, packet, 0x2A606060,
        top_left, top_right, bottom_left, bottom_right);
    packet = t3_8004daa8(ordering, packet, 64);
    w_u32(0x800ADF64, packet);
    start_x = ticker - 836;
    while ((sint32)start_x < 368)
    {
        t3_8004ce74(0x800B9234, 0, 6, 2, start_x, 203, 0x800FA5EC);
        start_x += 836;
    }
    packet = r_u32(0x800ADF64);
    packet = t3_80054f14(ordering, packet, 0, color);
    packet = t3_8004da10(ordering, packet, 0x60000000, 0, 0x01E00170);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_09_800eeab8(uint32 state, uint32 selected, sint32 progress, uint32 color)
{
    uint32 count;
    uint32 result;
    uint32 row;
    FUNCTION_MARKER(0x800EEAB8, "SLUS_004.02-BNS-logical-09");
    count = r_u8(state + 88);
    result = (sint32)count < (sint32)selected;
    if (result != 0)
        selected = count;
    if (progress != 0)
    {
        uint32 first = r_u8(state + 101);
        uint32 second = r_u8(state + 114);
        uint32 first_x = r_u32(0x800FA658 + selected * 4) * 36 + 57;
        uint32 second_x = r_u32(0x800FA708 + selected * 4) * 36 + 57;
        uint32 delta = (5u - (first + second + 1) / 3) * 30;
        uint32 first_y = 164;
        uint32 second_y = delta + 192;
        uint32 kind = r_u16(state + 56 + selected * 2);
        uint32 step = progress < 9 ? (uint32)progress : 8;
        uint32 packet;
        uint32 ordering;
        if (kind == 1)
        {
            second_x = first_x + (uint32)((sint32)((second_x - first_x) * step) >> 3);
            second_y = 164 + (uint32)((sint32)((delta + 28) * step) >> 3);
            color = 0xF8;
        }
        else if (kind == 2)
        {
            first_x = second_x + (uint32)((sint32)((first_x - second_x) * step) >> 3);
            first_y = second_y + (uint32)((sint32)((164u - second_y) * step) >> 3);
            color = 0xE0F8;
        }
        else if (kind == 3)
        {
            uint32 middle_x = first_x + (uint32)((sint32)(second_x - first_x) >> 1);
            uint32 middle_y = 164 + (uint32)((sint32)(delta + 28) >> 1);
            first_x = middle_x + (uint32)((sint32)((first_x - middle_x) * step) >> 3);
            second_x = middle_x + (uint32)((sint32)((second_x - middle_x) * step) >> 3);
            first_y = middle_y + (uint32)((sint32)((164u - middle_y) * step) >> 3);
            second_y = middle_y + (uint32)((sint32)((second_y - middle_y) * step) >> 3);
            color = 0xE0E0E0;
        }
        packet = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        result = t3_8004d7d4(ordering + 28, packet, color | 0x28000000,
            (first_x & 0xFFFF) | (first_y << 16),
            ((first_x + 3) & 0xFFFF) | (first_y << 16),
            (second_x & 0xFFFF) | (second_y << 16),
            ((second_x + 3) & 0xFFFF) | (second_y << 16));
        w_u32(0x800ADF64, result);
    }
    row = selected - 1;
    while ((sint32)row >= 0)
    {
        uint32 first = r_u8(state + 101);
        uint32 second = r_u8(state + 114);
        uint32 first_x = r_u32(0x800FA658 + row * 4) * 36 + 57;
        uint32 second_x = r_u32(0x800FA708 + row * 4) * 36 + 57;
        uint32 second_y = ((5u - (first + second + 1) / 3) * 30 + 192) << 16;
        uint32 kind = r_u16(state + 56 + row * 2);
        uint32 packet;
        uint32 ordering;
        color = kind == 1 ? 0xF8 : kind == 2 ? 0xE0F8 : 0xE0E0E0;
        --row;
        packet = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        result = t3_8004d7d4(ordering + 28, packet, color | 0x28000000,
            (first_x & 0xFFFF) | 0x00A40000,
            ((first_x + 3) & 0xFFFF) | 0x00A40000,
            (second_x & 0xFFFF) | second_y,
            ((second_x + 3) & 0xFFFF) | second_y);
        w_u32(0x800ADF64, result);
    }
    return result;
}

uint32 t3_8004f8b4(void)
{
    uint32 value;
    uint32 result;
    FUNCTION_MARKER(0x8004F8B4, "SLUS_004.02");
    do
    {
        value = r_u8(0x80097F3E) + 1;
        if (value >= 4)
            value = 0;
        w_u8(0x80097F3E, value);
        result = r_u8(0x80097F3E) ^ 3;
    } while (result == 0);
    return result;
}

uint32 t3_80075834(void)
{
    FUNCTION_MARKER(0x80075834, "SLUS_004.02");
    w_u8(0x800A3400, 0);
    w_u8(0x800A3401, 15);
    return 15;
}

uint32 t3_8007584c(void)
{
    uint32 value;
    FUNCTION_MARKER(0x8007584C, "SLUS_004.02");
    value = r_u8(0x800A3400) + 1;
    w_u8(0x800A3400, value);
    return value;
}

void t3_800ef92c(void)
{
    t3_bns_09_800ef92c();
}

uint32 t3_bns_09_800ef92c(void)
{
    uint32 value;
    uint32 first;
    uint32 second;
    uint32 phase;
    uint32 ordering;
    uint32 packet;
    uint32 saved_packet;
    uint32 level;
    uint32 buffer;
    uint32 completed;
    uint32 progress;
    uint32 row;
    uint8 entry[8];
    FUNCTION_MARKER(0x800EF92C, "SLUS_004.02-BNS-logical-09");
    w_u32(0x800FA7D8, r_u32(0x800FA7D8) + 1);
    first = r_u16(0x800ADF08);
    second = r_u16(0x800ADF0A);
    if (((first | second) & 0x800) != 0)
    {
        phase = r_u16(0x800AE224);
        if (phase - 2 < 5)
            w_u16(0x800AE224, 7);
    }
    phase = (uint32)r_s16(0x800AE224);
    switch (phase)
    {
    case 0:
        t3_bns_09_800f0aa8();
        t3_8004f8b4();
        t3_8006ae14(6, 0);
        w_u32(0x800FA7D8, 0);
        w_u32(0x800FA7B8, 0);
        w_u32(0x800FA7BC, 0);
        w_u32(0x800FA7D0, 0);
        w_u32(0x800FA858, 0);
        w_u32(0x800FA7D4, 0);
        value = t3_bns_09_800f22c8();
        w_u32(0x800FA7CC, value);
        value = t3_bns_09_800f2368();
        w_u32(0x800FA7C0, value);
        w_u16(0x800AE224, 1);
        w_u32(0x800FA7C4, r_u32(0x800AFAC0));
        w_u32(0x800FA7C8, r_u32(0x800AFAC4));
        t3_80075834();
        t3_8004b688(1);
        break;
    case 1:
        value = r_u32(0x800FA858) + 8;
        w_u32(0x800FA858, value);
        if ((sint32)value >= 256)
        {
            w_u32(0x800FA858, 256);
            w_u16(0x800AE224, 2);
        }
        break;
    case 2:
        value = r_u32(0x800FA7BC) + 1;
        w_u32(0x800FA7BC, value);
        if ((sint32)value >= 17)
        {
            t3_800753bc(0, 0x55F5, 0);
            w_u32(0x800AE214, 0);
            value = r_u32(0x800FA7B8);
            w_u32(0x800FA7BC, 0);
            w_u16(0x800AE224, 3);
            w_u32(0x800FA7B8, value + 1);
        }
        break;
    case 3:
        value = r_u32(0x800AE214) + 1;
        w_u32(0x800AE214, value);
        if ((sint32)value >= 6)
            w_u16(0x800AE224, r_s32(0x800FA7B8) < 10 ? 2 : 4);
        break;
    case 4:
        w_u32(0x800FA7D0, 1);
        w_u32(0x800FA7D4, 30);
        w_u16(0x800AE224, 5);
        /* Fall through to decrement the new countdown */
    case 5:
        value = r_u32(0x800FA7D4) - 1;
        w_u32(0x800FA7D4, value);
        if ((sint32)value <= 0)
        {
            w_u32(0x800FA7D4, 0);
            t3_800753bc(0, 0x55F5, 0);
            t3_8004efc8(r_u32(0x800FA7C8));
            value = t3_800757a8(r_u32(0x800FA7C4));
            if (value != 0)
                t3_800753bc(r_u32(0x800FA7C4), 0x86CA, 0);
            else
                t3_8007584c();
            w_u16(0x800AE224, 6);
        }
        break;
    case 6:
        value = r_u32(0x800FA7C4);
        if ((r_u16(0x800ADF08 + value * 2) & 0xF0) != 0)
            w_u32(0x800FA7D0, (r_u32(0x800FA7D0) + 1) & 1);
        if (r_s32(0x800FA7D8) < 4680)
            break;
        w_u16(0x800AE224, 7);
        /* Fall through to start the fade */
    case 7:
        w_u32(0x800FA7D0, 0);
        w_u32(0x800FA858, 256);
        w_u16(0x800AE224, 8);
        break;
    case 8:
        value = r_u32(0x800FA858) - 4;
        w_u32(0x800FA858, value);
        if ((sint32)value <= 0)
        {
            w_u32(0x800FA858, 0);
            w_u16(0x800AE224, 9);
        }
        break;
    case 9:
        w_u16(0x800AE204, 16);
        w_u16(0x800AE224, 0);
        break;
    }
    t3_80075864();
    ordering = r_u32(0x800A9218) + 28;
    level = r_u32(0x800FA858);
    buffer = r_u32(0x800ADEFC);
    saved_packet = r_u32(0x800ADF64);
    packet = 0x800FA860 + buffer * 15360;
    w_u32(0x800ADF64, packet);
    packet = t3_8004e000(ordering, packet, (sint32)level);
    w_u32(0x800ADF64, packet);
    t3_8004ce74(0x800B8FA4, 6, 1, 67, 32);
    t3_bns_09_800ef6f4();
    completed = r_u32(0x800FA7B8);
    progress = r_u32(0x800FA7BC);
    row = 0;
    while ((sint32)row < (sint32)completed)
    {
        uint32 x = r_u16(0x800B8ED4 + row * 4);
        uint32 y = r_u16(0x800B8ED6 + row * 4);
        uint32 frames;
        uint32 character;
        uint32 costume;
        t3_bns_09_800f2310(entry, (sint32)row);
        frames = (uint32)entry[0] | ((uint32)entry[1] << 8)
            | ((uint32)entry[2] << 16) | ((uint32)entry[3] << 24);
        character = r_u8(0x800AFB18 + row * 4);
        costume = r_u8(0x800AFB19 + row * 4);
        t3_bns_09_800ef47c(row, (sint32)frames, (character << 2) | costume, 64, x, y);
        ++row;
    }
    if (progress != 0)
    {
        uint32 x = r_u16(0x800B8ED4 + row * 4);
        uint32 y = r_u16(0x800B8ED6 + row * 4);
        uint32 frames;
        uint32 character;
        uint32 costume;
        t3_bns_09_800f2310(entry, (sint32)row);
        frames = (uint32)entry[0] | ((uint32)entry[1] << 8)
            | ((uint32)entry[2] << 16) | ((uint32)entry[3] << 24);
        frames *= progress;
        if ((sint32)frames < 0)
            frames += 15;
        frames = (uint32)((sint32)frames >> 4);
        character = r_u8(0x800AFB18 + row * 4);
        costume = r_u8(0x800AFB19 + row * 4);
        t3_bns_09_800ef47c(row, (sint32)frames, (character << 2) | costume, 0, x, y);
    }
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    packet = t3_bns_09_800f0b30(ordering, packet);
    value = t3_8004da10(ordering, packet, 0x602030A0, 0, 0x01E00170);
    w_u32(0x800ADF64, saved_packet);
    return value;
}

uint32 t3_bns_09_800ef6f4(void)
{
    uint32 result;
    sint32 frames;
    sint32 total_seconds;
    sint32 minutes;
    sint32 seconds;
    sint32 hundredths;
    uint32 packet;
    uint32 ordering;
    uint32 code;
    uint32 phase;
    uint32 style;
    FUNCTION_MARKER(0x800EF6F4, "SLUS_004.02-BNS-logical-09");
    result = r_u32(0x800FA7D0);
    if (result == 0)
        return result;
    (void)r_u32(0x800FA7D4);
    t3_8004ce74(0x800B8F7C, 2, 1, 37, 352);
    frames = r_s32(0x800FA7CC);
    if (frames > 359999)
        frames = 359999;
    total_seconds = frames / 60;
    minutes = total_seconds / 60;
    seconds = total_seconds % 60;
    hundredths = (frames % 60) * 100 / 60;
    t3_8004ce74(0x800B8F50, 7, 2, 128, 380, 189, 200);
    t3_8004ce74(0x800B8F60, 7, 2, 94, 380,
        (uint32)minutes, 151, (uint32)seconds, 215, (uint32)hundredths);
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    code = r_u32(0x800FA7C8);
    packet = t3_8004b928(ordering + 28, packet, 289, 368, code, 0);
    phase = r_u32(0x800FA7D4);
    w_u32(0x800ADF64, packet);
    if (phase != 0)
        return packet;
    result = r_u32(0x800FA7C0);
    if (result == 0)
        return 0x800B0000;
    style = (r_u32(0x800AE218) & 3) != 0 ? 5 : 6;
    return t3_8004ce74(0x800B8F90, style, 1, 98, 424);
}

uint32 t3_bns_09_800f0cfc(uint32 ordering, uint32 packet, uint32 x, uint32 y, uint32 character, uint32 flags)
{
    uint32 left = x & 0xFFFF;
    uint32 right = (x + 15) & 0xFFFF;
    sint32 index;
    uint32 descriptor;
    uint32 texture;
    uint32 page;
    uint32 pieces;
    uint32 piece;
    FUNCTION_MARKER(0x800F0CFC, "SLUS_004.02-BNS-logical-09");
    if ((flags & 0x40) != 0)
    {
        uint32 top = y << 16;
        uint32 bottom = (y + 15) << 16;
        packet = t3_8004d830(ordering, packet, 0x2D000000,
            left | top, right | top, left | bottom, right | bottom,
            0x7F1DE000, 0x001FE01F, 0xFF1FFF00);
        top = (y + 16) << 16;
        bottom = (y + 31) << 16;
        packet = t3_8004d830(ordering, packet, 0x2D000000,
            left | top, right | top, left | bottom, right | bottom,
            0x7F1DE020, 0x001FE03F, 0xFF3FFF20);
    }
    if ((flags & 0x80) != 0)
        return packet;
    index = (sint32)character >> 2;
    if (character == 89)
        ++index;
    descriptor = 0x800B9158 + (uint32)index * 8;
    texture = r_u32(descriptor);
    page = r_u16(descriptor + 4) << 16;
    if ((flags & 0x20) != 0)
        texture = (texture & 0xFFFF) | 0x7D500000;
    pieces = index == 20 || index >= 22 ? 3 : 1;
    for (piece = 0; piece < pieces; ++piece)
    {
        uint32 piece_texture = texture;
        uint32 top_offset;
        uint32 bottom_offset;
        uint32 texture_height;
        uint32 top;
        uint32 bottom;
        uint32 u;
        uint32 v;
        uint32 right_u;
        uint32 bottom_v;
        uint32 uv0;
        uint32 uv1;
        uint32 uv_bottom;
        if (index == 20)
        {
            piece_texture += piece * 32;
            top_offset = piece * 12;
            bottom_offset = piece == 2 ? 28 : top_offset + 11;
            texture_height = piece == 2 ? 9 : 23;
        }
        else if (index < 22)
        {
            top_offset = 0;
            bottom_offset = 28;
            texture_height = 57;
        }
        else if (piece == 0)
        {
            top_offset = 0;
            bottom_offset = 4;
            texture_height = 9;
        }
        else
        {
            piece_texture = (texture & 0xFFFF00FF) + piece * 32;
            top_offset = piece == 1 ? 5 : 17;
            bottom_offset = piece == 1 ? 16 : 28;
            texture_height = 23;
        }
        top = (y + top_offset) << 16;
        bottom = (y + bottom_offset) << 16;
        u = piece_texture & 0xFF;
        v = (piece_texture >> 8) & 0xFF;
        right_u = (u + 31) & 0xFF;
        bottom_v = ((v + texture_height) & 0xFF) << 8;
        uv0 = u | (v << 8) | (piece_texture & 0xFFFF0000);
        uv1 = right_u | (v << 8) | page;
        uv_bottom = u | bottom_v | ((right_u | bottom_v) << 16);
        packet = t3_8004d830(ordering, packet, 0x2D000000,
            left | top, right | top, left | bottom, right | bottom,
            uv0, uv1, uv_bottom);
    }
    return packet;
}

uint32 t3_bns_09_800ef47c(uint32 stage, sint32 frames, uint32 character, uint32 flags, uint32 x, uint32 y)
{
    uint32 ordering;
    uint32 displayed_stage;
    uint32 format;
    sint32 total_seconds;
    sint32 minutes;
    sint32 seconds;
    sint32 hundredths;
    uint32 packet;
    uint32 left;
    uint32 right;
    uint32 top;
    uint32 bottom;
    FUNCTION_MARKER(0x800EF47C, "SLUS_004.02-BNS-logical-09");
    ordering = r_u32(0x800A9218) + 28;
    displayed_stage = stage + 1;
    format = (sint32)displayed_stage < 10 ? 0x800B8EFC : 0x800B8F10;
    t3_8004ce74(format, 6, 0, x + 34, y + 16, displayed_stage);
    if (frames > 359999)
        frames = 359999;
    total_seconds = frames / 60;
    minutes = total_seconds / 60;
    seconds = total_seconds % 60;
    hundredths = (frames % 60) * 100 / 60;
    t3_8004ce74(0x800B8F24, 6, 1, y + 32, x + 57, x + 92);
    t3_8004ce74(0x800B8F34, 6, 1, y + 36, x + 34,
        (uint32)minutes, x + 66, (uint32)seconds, x + 102, (uint32)hundredths);
    packet = r_u32(0x800ADF64);
    packet = t3_bns_09_800f0cfc(ordering, packet, x + 15, y + 30, character, flags);
    left = (x + 13) & 0xFFFF;
    right = (x + 141) & 0xFFFF;
    top = (y + 24) << 16;
    bottom = (y + 64) << 16;
    w_u32(0x800ADF64, packet);
    packet = t3_8004d8b0(ordering, packet, 0x3ADEDEDE, 0, 0x00DEDEDE, 0,
        left | top, right | top, left | bottom, right | bottom);
    w_u32(0x800ADF64, packet);
    packet = t3_8004daa8(ordering, packet, 64);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_09_800f0b30(uint32 ordering, uint32 packet)
{
    FUNCTION_MARKER(0x800F0B30, "SLUS_004.02-BNS-logical-09");
    /* Preserve the original unused seven-word local copy reads */
    (void)r_u32(0x800B913C);
    (void)r_u32(0x800B9140);
    (void)r_u32(0x800B9144);
    (void)r_u32(0x800B9148);
    (void)r_u32(0x800B914C);
    (void)r_u32(0x800B9150);
    (void)r_u32(0x800B9154);
    packet = t3_8004db9c(ordering, packet, 0, 0, 128, 256, 512, 0, 0x7FE0, 128);
    packet = t3_8004db9c(ordering, packet, 128, 0, 128, 256, 576, 0, 0x7FE0, 128);
    packet = t3_8004db9c(ordering, packet, 256, 0, 112, 256, 640, 0, 0x7FE0, 128);
    packet = t3_8004db9c(ordering, packet, 0, 256, 128, 224, 512, 256, 0x7FE0, 128);
    packet = t3_8004db9c(ordering, packet, 128, 256, 128, 224, 576, 256, 0x7FE0, 128);
    return t3_8004db9c(ordering, packet, 256, 256, 112, 224, 640, 256, 0x7FE0, 128);
}

uint32 t3_bns_09_800f0238(sint32 value, uint32 code, uint32 flags, uint32 x, uint32 y)
{
    uint32 ordering;
    uint32 packet;
    FUNCTION_MARKER(0x800F0238, "SLUS_004.02-BNS-logical-09");
    ordering = r_u32(0x800A9218) + 28;
    if (r_u32(0x800FA818) != 0)
    {
        sint32 quotient = value / 100;
        sint32 remainder = value % 100;
        uint32 style = r_u8(0x800B90DC + ((uint32)quotient & 3));
        uint32 digits = remainder < 10 ? 1 : 2;
        uint32 tail;
        uint32 format;
        /* The signed remainder is bounded to -99 through 99 */
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        t3_8004cc08(tail, 0x800B9060);
        format = r_u32(0x800F2374 + digits * 4);
        t3_8004cc08(tail + 2 - digits, format);
        t3_8004ce74(0x1F800000, style, 0, x + 20, y + 68, (uint32)remainder);
    }
    packet = r_u32(0x800ADF64);
    packet = t3_8004b928(ordering, packet, x + 2, y + 2, code, flags);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_09_800f006c(sint32 rank, uint32 y)
{
    uint32 tail;
    sint32 suffix;
    uint32 style;
    FUNCTION_MARKER(0x800F006C, "SLUS_004.02-BNS-logical-09");
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    suffix = rank;
    if (rank >= 5)
        suffix = 4;
    if (rank != 0)
    {
        sint32 displayed;
        sint32 tens;
        uint32 ones;
        uint32 text;
        tail = t3_8004cc08(tail, 0x800B90A0);
        displayed = rank;
        if (displayed >= 101)
            displayed = 99;
        tens = displayed / 10;
        ones = (uint32)displayed - (uint32)tens * 10;
        w_u8(tail, tens != 0 ? (uint32)tens + 48 : 32);
        w_u8(tail + 1, ones + 48);
        text = r_u32(0x800F2398 + (uint32)suffix * 4);
        t3_8004cc08(tail + 2, text);
        style = (r_u32(0x800AE218) & 2) != 0 ? 6 : 2;
        t3_8004ce74(0x1F800000, 5, 1, 86, y, style);
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        t3_8004cc08(tail, 0x800B90B0);
        return t3_8004ce74(0x1F800000, 5, 1, 67, y + 28);
    }
    style = (r_u32(0x800AE218) & 16) != 0 ? 5 : 2;
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    t3_8004cc08(tail, 0x800B90C4);
    return t3_8004ce74(0x1F800000, style, 1, 34, y + 26);
}

uint32 t3_bns_09_800efea4(sint32 wins, uint32 x, uint32 y)
{
    uint32 digits;
    uint32 displayed = (uint32)wins;
    uint32 tail;
    uint32 format;
    uint32 label;
    FUNCTION_MARKER(0x800EFEA4, "SLUS_004.02-BNS-logical-09");
    if (wins < 10)
        digits = 1;
    else if (wins < 100)
        digits = 2;
    else if (wins < 1000)
        digits = 3;
    else if (wins < 10000)
        digits = 4;
    else if (wins <= 99999)
        digits = 5;
    else if (wins <= 999999)
        digits = 6;
    else if (wins <= 9999999)
        digits = 7;
    else
    {
        digits = 8;
        if (wins > 99999999)
            displayed = 99999999;
    }
    if (digits > 3)
        digits = 3;
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    t3_8004cc08(tail, 0x800B9060);
    format = r_u32(0x800F2374 + digits * 4);
    t3_8004cc08(tail + 3 - digits, format);
    t3_8004ce74(0x1F800000, 0, 2, x + 26, y, displayed);
    label = wins < 2 ? 0x800B906C : 0x800B9078;
    return t3_8004ce74(label, 6, 1, x + 96, y + 14, displayed);
}

uint32 t3_bns_09_800edae0(uint32 state)
{
    uint32 tail;
    uint32 row;
    FUNCTION_MARKER(0x800EDAE0, "SLUS_004.02-BNS-logical-09");
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    for (row = 0; row < 2; ++row)
    {
        uint32 kind = row;
        uint32 descriptor;
        uint32 text_y;
        uint32 text_x;
        uint32 text;
        uint32 color;
        uint32 top;
        uint32 bottom;
        uint32 left;
        uint32 middle;
        uint32 right;
        uint32 ordering;
        uint32 packet;
        uint32 color0;
        uint32 color1;
        uint32 color2;
        if (r_u8(state + 28) == 1)
        {
            if (row == r_u8(state + 31))
                kind = row + 2;
        }
        descriptor = 0x800B8D58 + kind * 36;
        text_y = r_u16(descriptor + 6);
        text_x = r_u16(descriptor + 4);
        if (row == 1)
        {
            uint32 first = r_u8(state + 101);
            uint32 second = r_u8(state + 114);
            text_y += (5u - (first + second + 1) / 3) * 30;
        }
        /* Preserve the descriptor reads for the unused copy arguments */
        (void)r_u32(descriptor + 12);
        (void)r_u32(descriptor + 16);
        (void)r_u32(descriptor + 20);
        (void)r_u32(descriptor + 24);
        (void)r_u32(descriptor + 28);
        (void)r_u32(descriptor + 32);
        text = r_u32(descriptor);
        color = r_u32(descriptor + 8);
        (void)r_u32(descriptor + 4);
        t3_8004cc08(tail, text);
        t3_8004ce74(0x1F800000, color, 1, text_x, text_y, 1);
        top = r_u16(descriptor + 14);
        bottom = r_u16(descriptor + 20);
        left = r_u16(descriptor + 12);
        middle = r_u16(descriptor + 16);
        right = r_u16(descriptor + 18);
        if (row == 1)
        {
            uint32 first = r_u8(state + 101);
            uint32 second = r_u8(state + 114);
            uint32 delta = (5u - (first + second + 1) / 3) * 30;
            top += delta;
            bottom += delta;
        }
        ordering = r_u32(0x800A9218);
        top <<= 16;
        bottom <<= 16;
        packet = r_u32(0x800ADF64);
        color0 = r_u32(descriptor + 24) | 0x38000000;
        color1 = r_u32(descriptor + 28) | 0x38000000;
        color2 = r_u32(descriptor + 32) | 0x38000000;
        packet = t3_8004d8b0(ordering, packet, color0 | 0x3A000000,
            color1, color0, color1, left | top, middle | top,
            left | bottom, middle | bottom);
        ordering = r_u32(0x800A9218);
        w_u32(0x800ADF64, packet);
        packet = t3_8004d8b0(ordering, packet, color1 | 0x3A000000,
            color2, color1, color2, middle | top, right | top,
            middle | bottom, right | bottom);
        ordering = r_u32(0x800A9218);
        w_u32(0x800ADF64, packet);
        packet = t3_8004daa8(ordering, packet, 32);
        w_u32(0x800ADF64, packet);
    }
    return 0;
}
