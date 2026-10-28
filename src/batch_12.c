#include "game_functions.h"

uint32 t3_80054b48(uint32 state)
{
    uint32 first, second, idle, elapsed, right, center, movement, text, source, length, width, x;
    FUNCTION_MARKER(0x80054B48, "SLUS_004.02");
    first = r_u16(0x800ADD68);
    second = r_u16(0x800ADD6A);
    if (first | second)
        w_u32(state + 4, 0);
    else
        w_u32(state + 4, r_u32(state + 4) + 1);
    idle = r_u32(state + 4);
    elapsed = (sint32)idle < 480 ? 0 : idle - 480;
    right = elapsed & 0x100;
    center = right ? 552 : 184;
    if ((elapsed & 0xE0) == 0xE0)
    {
        movement = ((elapsed & 31) * 368) >> 5;
        center = right ? center - movement : center + movement;
    }
    text = t3_8004cc08(0x1F800000, 0x80097D08);
    source = r_u32(0x800981C8 + r_u32(state) * 4);
    t3_8004cc08(text, source);
    length = t3_8007a418(text);
    width = length * 13;
    x = center - (uint32)((sint32)width >> 1);
    t3_8004ce74(0x1F800000, 6, 1, x, 28);
    source = r_u32(0x800981E0);
    t3_8004cc08(text, source);
    length = t3_8007a418(text);
    width = length * 9;
    x = center - (uint32)((sint32)width >> 1) - 368;
    return t3_8004ce74(0x1F800000, 6, 0, x, 32, 5, 6, 5, 6, 5);
}

uint32 t3_80068ba8(uint32 first_actor, uint32 second_actor, uint32 reset)
{
    uint32 random, velocity, angle, offset;
    sint32 mode, value;
    FUNCTION_MARKER(0x80068BA8, "SLUS_004.02");
    if ((sint32)reset < 0)
    {
        mode = r_s16(0x800A8CDC);
        if (mode == 0 || mode == 1 || mode == 2)
        {
            random = (uint32)randPSX();
            random &= mode == 0 ? 7 : mode == 1 ? 3 : 1;
            value = r_s16(0x80098550 + random * 2);
            if (mode == 1)
                value /= 2;
            else if (mode == 2)
                value /= 4;
            w_u32(0x80098564, (uint32)value);
        }
        random = (uint32)randPSX();
        w_u32(0x80098568, (random & 0xFFF) << 6);
        random = (uint32)randPSX();
        value = r_s16(0x80098560 + (random & 1) * 2);
        w_u32(0x8009856C, (uint32)value);
    }
    w_u32(0x800A8CA0, 0xFFFFFFFF);
    velocity = r_u32(0x8009856C);
    angle = r_u32(0x80098568);
    offset = r_u32(0x80098564);
    angle = (angle + velocity) & 0x3FFFF;
    w_u32(0x80098568, angle);
    return t3_80064e10(first_actor, second_actor, offset, angle, 1, 6130, 0);
}

uint32 t3_8004fff0(uint32 side)
{
    uint32 offset, buttons, value, numerator, quotient, remainder;
    FUNCTION_MARKER(0x8004FFF0, "SLUS_004.02");
    offset = side * 2;
    buttons = r_u16(0x800ADF08 + offset);
    if (buttons & 0x800)
    {
        w_u16(0x800AE1F8 + offset, 1);
        t3_8004b5f4();
        if (r_u8(0x800AFA95) == 0)
        {
            w_u16(0x800ADFBC + offset, 1);
            return 1;
        }
        w_u16(0x800ADFBC + offset, 0);
        return 2;
    }
    if (buttons & 0xF0)
    {
        value = r_u32(0x800AE214);
        w_u32(0x800AE214, (sint32)value < 90 ? 0 : value - 90);
    }
    value = r_u32(0x800AE214);
    if (value == 0)
        return 0xFFFFFFFF;
    if ((sint32)value > 0)
        w_u32(0x800AE214, value - 1);
    numerator = r_u32(0x800AE214) + 90;
    quotient = (uint32)((sint32)numerator / 90);
    remainder = numerator - quotient * 90;
    if (quotient < 9 && remainder == 89)
        t3_80040c00(quotient ? quotient + 0x1004 : 0x1017);
    t3_8004ce74(0x80022460, 9, 2, 63, 200, quotient);
    return 0;
}

uint32 t3_80074104(uint32 object)
{
    uint32 age, table, x, table_y, y, packet, u, v, page;
    sint32 signed_x, signed_y;
    FUNCTION_MARKER(0x80074104, "SLUS_004.02");
    age = r_u32(object + 72) + 1;
    w_u32(object + 72, age);
    if ((sint32)age >= 12)
        w_u32(object + 72, 0);
    age = r_u32(object + 72);
    x = r_u16(object + 68);
    table = 0x80025720 + age * 2;
    age = r_u8(table);
    table_y = r_u8(table + 1);
    x += age;
    signed_x = (sint16)x;
    y = r_u16(object + 70);
    y += table_y;
    packet = r_u32(0x800ADEFC) * 40 + object;
    y += 128;
    page = ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | 0x20 | ((y & 0x200) << 2);
    w_u16(packet + 102, page);
    u = ((uint32)(signed_x % 64) << 16) >> 14;
    signed_y = (sint16)y;
    v = (uint32)(signed_y % 256);
    w_u8(packet + 92, u);
    packet = r_u32(0x800ADEFC) * 40 + object;
    w_u8(packet + 93, v);
    packet = r_u32(0x800ADEFC) * 40 + object;
    w_u8(packet + 100, u + 31);
    packet = r_u32(0x800ADEFC) * 40 + object;
    w_u8(packet + 101, v);
    packet = r_u32(0x800ADEFC) * 40 + object;
    w_u8(packet + 108, u);
    packet = r_u32(0x800ADEFC) * 40 + object;
    v += 31;
    w_u8(packet + 109, v);
    packet = r_u32(0x800ADEFC) * 40 + object;
    w_u8(packet + 116, u + 31);
    packet = r_u32(0x800ADEFC) * 40 + object;
    w_u8(packet + 117, v);
    return packet;
}

uint32 t3_80072b88(uint32 object)
{
    uint32 state, alternate, flags, palette, index, packet, value, first, second, random, x, y;
    FUNCTION_MARKER(0x80072B88, "SLUS_004.02");
    state = r_u8(object + 14);
    alternate = r_u32(object + 76);
    w_u8(object + 14, state + 1);
    palette = alternate ? 0x800ADD80 : 0x800ADD90;
    w_u8(object + 13, r_u8(object + 13) | 0x40);
    for (index = 0; index < 2; ++index)
    {
        packet = object + 80 + index * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE74);
        w_u16(packet + 22, value);
        value = r_u16(palette);
        w_u16(packet + 14, value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    flags = r_u8(object + 13);
    w_u32(object + 8, 0x8002563C);
    w_u8(object + 13, flags | 2);
    first = (uint32)randPSX();
    second = (uint32)randPSX();
    w_u32(object + 72, 0xFFFFFFFF);
    random = first | (second << 16);
    w_u16(object + 24, 768);
    x = r_u16(object + 16);
    y = r_u16(object + 18);
    w_u16(object + 16, x - 127 + (random & 0xFF));
    w_u16(object + 18, y - 127 + (((sint32)random >> 2) & 0xFF));
    return t3_8006e120(object + 16);
}

uint32 t3_80071afc(uint32 object)
{
    uint32 state, flags, index, packet, value, first, second, random, x, y, z, angle_x, angle_y, size;
    FUNCTION_MARKER(0x80071AFC, "SLUS_004.02");
    state = r_u8(object + 14);
    flags = r_u8(object + 13);
    w_u8(object + 14, state + 1);
    w_u8(object + 13, flags | 0x40);
    for (index = 0; index < 2; ++index)
    {
        packet = object + 80 + index * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE70);
        w_u16(packet + 22, value);
        value = r_u16(0x800ADD84);
        w_u16(packet + 14, value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    w_u32(object + 8, 0x80025564);
    first = (uint32)randPSX();
    second = (uint32)randPSX();
    random = first | (second << 16);
    size = (((sint32)random >> 12) & 0x3FF) * 5;
    x = r_u32(object + 40);
    y = r_u32(object + 44);
    w_u32(object + 68, ((sint32)random >> 10) & 31);
    w_u32(object + 64, 0);
    w_u32(object + 40, x - 127 + (random & 0xFF));
    z = r_u32(object + 48);
    w_u32(object + 44, y - 127 + (((sint32)random >> 2) & 0xFF));
    angle_x = r_u16(object + 16);
    w_u32(object + 48, z - 127 + (((sint32)random >> 4) & 0xFF));
    angle_y = r_u16(object + 18);
    w_u16(object + 16, angle_x - 63 + (((sint32)random >> 6) & 0x7F));
    w_u16(object + 18, angle_y - 63 + (((sint32)random >> 8) & 0x7F));
    w_u16(object + 24, size / 4 + 512);
    return t3_8006e120(object + 16);
}

uint32 t3_8002d45c(uint32 actor, uint32 opponent, uint32 variant)
{
    uint32 buttons, value, remainder;
    sint32 selection;
    FUNCTION_MARKER(0x8002D45C, "SLUS_004.02");
    buttons = r_u16(r_u8(actor + 30) ? 0x800ADD6A : 0x800ADD68);
    if (buttons & 0x20)
        variant = 3;
    if (buttons & 0x40)
        variant = 2;
    if (buttons & 0x10)
        variant = 1;
    if (buttons & 0x80)
        variant = 0;
    selection = (sint16)variant;
    if (selection >= 0 && selection <= 3)
        w_u16(actor + 0x1A0, 0xD67 + selection);
    if (r_s16(actor + 28) == 38 && r_s16(actor + 0x1A0) == 0xD67)
    {
        remainder = r_u32(0x800AE218) % 3;
        value = remainder == 1 ? 0xD69 : remainder == 2 ? 0xD6A : 0xD68;
        w_u16(actor + 0x1A0, value);
    }
    value = t3_8002d178(actor, (uint32)r_s16(actor + 0x1A0));
    w_u32(actor + 0x54, value);
    w_u32(actor + 0x4C, value);
    w_u32(actor + 0x1A4, value);
    w_u16(actor + 0x58, 1);
    w_u16(actor + 0x198, 1);
    value = t3_80030f68(actor, 6);
    w_u16(actor + 0x19A, value);
    w_u16(actor + 0x19C, 0);
    w_u16(actor + 0x19E, 0);
    w_u32(actor + 4, 0);
    return t3_8002f3ac(actor, opponent);
}

uint32 t3_800729fc(uint32 event)
{
    uint32 kind, actor, animation, script, clear, state, x, y, z, dx, dy, dz, result;
    sint32 frame;
    FUNCTION_MARKER(0x800729FC, "SLUS_004.02");
    if (r_u8(event + 13) & 2)
    {
        kind = r_u32(event + 76) ? 28 : 25;
        actor = r_u8(event + 13) & 0x10 ? 0x800A9228 : 0x800AAAB4;
        animation = r_u32(actor + 0x54);
        script = r_u32(animation + 40);
        clear = r_u8(script) != kind;
        if (!clear)
        {
            frame = r_s16(actor + 0x58);
            clear = frame < (sint32)r_u8(animation + 45);
        }
        if (clear)
            w_u8(event + 13, r_u8(event + 13) & 0xFD);
    }
    state = r_u8(event + 14);
    if (state == 0)
    {
        t3_80072b88(event);
        t3_80072d5c(event);
    }
    else if (state == 1)
        t3_80072d5c(event);
    y = r_u32(event + 44);
    x = r_u32(event + 40);
    z = r_u32(event + 48);
    w_u32(event + 56, y);
    w_u32(event + 52, x);
    w_u32(event + 60, z);
    dx = (uint32)r_s16(event + 30);
    x = r_u32(event + 40);
    dy = (uint32)r_s16(event + 34);
    dz = (uint32)r_s16(event + 38);
    w_u32(event + 40, x + dx);
    y = r_u32(event + 44);
    z = r_u32(event + 48);
    result = y + dy;
    w_u32(event + 44, result);
    w_u32(event + 48, z + dz);
    return result;
}

uint32 t3_8004a6bc(uint32 update)
{
    union { uint32 words[6]; uint16 halves[12]; } payload;
    uint32 queue, record, remaining, origin, difference, product, next, mode, kind;
    sint32 count, type;
    FUNCTION_MARKER(0x8004A6BC, "SLUS_004.02");
    if (update)
    {
        queue = r_u32(0x8009E838);
        count = r_s16(queue + 4);
        record = r_u32(queue);
        if (count > 0)
        {
            remaining = (uint32)count;
            do
            {
                type = r_s16(record + 22);
                if (type == 0)
                {
                    payload.words[0] = r_u32(record);
                    payload.words[1] = r_u32(record + 4);
                    payload.words[2] = r_u32(record + 8);
                    payload.halves[8] = r_u16(record + 16);
                    payload.halves[9] = r_u16(record + 18);
                    payload.halves[10] = r_u16(record + 20);
                    mode = r_u16(record + 24);
                    kind = r_u16(record + 26);
                    t3_80076884(mode, kind, payload.words, 0);
                }
                else if (type == 1)
                    t3_8004a990(record);
                origin = r_u32(0x8009E848);
                record += 28;
                difference = record - origin;
                product = difference * 0xB6DB6DB7;
                remaining -= 1;
                if (((sint32)product >> 2) >= 64)
                    record = origin;
            } while ((sint32)(remaining << 16) > 0);
        }
        next = r_u32(0x8009E838);
        origin = r_u32(0x8009E844);
        next += 8;
        w_u32(0x8009E838, next);
        difference = next - origin;
        if (((sint32)difference >> 3) >= 300)
            w_u32(0x8009E838, origin);
    }
    t3_8004ac24((uint32)(sint32)(sint16)update);
    return t3_8007745c(update);
}

uint32 t3_80071540(uint32 object)
{
    uint32 state, flags, index, packet, value, x, y, z, random, angle_x, angle_y;
    FUNCTION_MARKER(0x80071540, "SLUS_004.02");
    state = r_u8(object + 14);
    flags = r_u8(object + 13);
    w_u8(object + 14, state + 1);
    w_u8(object + 13, flags | 0x40);
    for (index = 0; index < 2; ++index)
    {
        packet = object + 80 + index * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE70);
        w_u16(packet + 22, value);
        value = r_u16(0x800ADD88);
        w_u16(packet + 14, value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    x = r_u32(object + 40);
    y = r_u32(object + 44);
    z = r_u32(object + 48);
    w_u32(object + 52, x);
    w_u32(object + 56, y);
    w_u32(object + 60, z);
    random = (uint32)randPSX();
    w_u32(object + 8, random & 1 ? 0x800254C4 : 0x800254E4);
    w_u32(object + 68, (((sint32)random >> 2) & 3) + 5);
    w_u32(object + 72, 0xFFFFFFFF);
    w_u32(object + 76, 1);
    w_u16(object + 24, 2560);
    angle_x = r_u16(object + 16);
    angle_y = r_u16(object + 18);
    w_u32(object + 64, 0);
    angle_x = angle_x - 63 + (((sint32)random >> 4) & 0x7F);
    angle_y = angle_y - 31 + (((sint32)random >> 6) & 0x3F);
    w_u16(object + 16, angle_x);
    w_u16(object + 18, angle_y);
    return t3_8006e120(object + 16);
}

uint32 t3_800517ec(uint32 first_actor, uint32 second_actor)
{
    uint32 side, actor, mode, owner, current, maximum, index, numerator, multiplier, elapsed, result;
    FUNCTION_MARKER(0x800517EC, "SLUS_004.02");
    for (side = 0; side < 2; ++side)
    {
        actor = side ? second_actor : first_actor;
        mode = r_u32(0x800AFA88);
        if (mode == 2)
        {
            owner = r_u8(0x800AFAAA);
            current = r_u32(actor + 0x3F4);
            if (side != owner || current == 0)
            {
                w_u32(actor + 0x3FC, current);
                continue;
            }
            maximum = r_u32(actor + 0x3F8);
            numerator = current << 3;
            index = maximum ? numerator / maximum : 0xFFFFFFFF;
            if (index >= 8)
                index = 7;
            multiplier = r_u32(0x800225AC + index * 4);
        }
        else if (mode == 4)
        {
            owner = r_u8(0x800AFAAA);
            current = r_u32(actor + 0x3F4);
            if (side != owner)
            {
                w_u32(actor + 0x3FC, current);
                continue;
            }
            owner = r_u8(0x800AFAA6);
            if (side != owner || current == 0)
            {
                w_u32(actor + 0x3FC, current);
                continue;
            }
            elapsed = r_u32(0x800AFAB4);
            index = elapsed < 2100 ? elapsed / 300 : 7;
            maximum = r_u32(actor + 0x3F8);
            multiplier = r_u32(0x800225CC + index * 4);
        }
        else
        {
            w_u32(actor + 0x3FC, r_u32(actor + 0x3F4));
            continue;
        }
        current = r_u32(actor + 0x3F4);
        result = (uint32)((sint32)maximum >> 8) * multiplier + current;
        if (maximum < result)
            result = maximum;
        w_u32(actor + 0x3FC, result);
    }
    return 0;
}

uint32 t3_800587d8(uint32 state)
{
    uint32 seed, random, category, entry, pointer, code, selected;
    sint32 count, ordinal, flag;
    FUNCTION_MARKER(0x800587D8, "SLUS_004.02");
    seed = r_u32(0x800ADCA0);
    random = (seed >> 1) & 0xFFF;
    w_u32(0x800ADCA0, seed * 5 + 3);
    category = r_u32(state + 20);
    selected = 0;
    if ((category == 2 && random < 32) || (category == 3 && random < 1024))
    {
        count = r_s16(state + 0x70);
        if (count > 0)
        {
            entry = 0x8009F974;
            for (ordinal = 1; ordinal <= count; ++ordinal, entry += 12)
            {
                flag = r_s16(entry + 12);
                pointer = r_u32(entry + 8);
                code = r_u16(pointer);
                if (flag == 0 && code == 0xC001)
                {
                    w_u16(entry + 12, 1);
                    selected += 1;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + selected);
        }
    }
    if (r_u32(state + 0x230) & 1)
        return selected + t3_80057e94(state);
    return selected + t3_80057d58(state);
}

uint32 t3_80032f04(uint32 actor, uint32 value)
{
    uint32 side, mode, base, address, frame, index;
    FUNCTION_MARKER(0x80032F04, "SLUS_004.02");
    side = actor != 0x800A9228;
    mode = r_u32(0x8009BCA8);
    switch (mode)
    {
    case 3:
        base = r_u32(0x800ADE68);
        address = base ? base + side * 52 + 31200 : 0;
        w_u8(address + 48, value);
    case 1:
    case 6:
        frame = (uint32)(r_s32(0x8009BCBC) >> 1);
        base = r_u32(0x800ADE68);
        index = r_u32(0x8009BCAC);
        address = base ? base + index * 31200 + side * 15600 + frame * 52 : 0;
        return r_u8(address + 48);
    case 4:
        base = r_u32(0x800ADE68);
        address = base ? base + side * 52 + 31200 : 0;
        return r_u8(address + 48);
    case 7:
        base = r_u32(0x800ADE68);
        index = r_u32(0x8009BCB8);
        address = base ? base + side * 15600 + index * 52 : 0;
        w_u8(address + 48, value);
    default:
        return value;
    }
}

uint32 t3_8005766c(uint32 state)
{
    sint32 count, i;
    uint32 entry, action, flags, attributes, mask, category, selection, kind, selected;
    FUNCTION_MARKER(0x8005766C, "SLUS_004.02");
    count = r_s16(state + 0x70);
    selected = 0;
    if (count <= 0)
        return 0;
    entry = 0x8009F978;
    for (i = 0; i < count; ++i, entry += 12)
    {
        if (r_s16(entry + 8) != 0)
            continue;
        action = r_u32(entry);
        flags = r_u32(action + 4);
        attributes = r_u32(action + 8);
        if (flags & 0x20000)
            continue;
        if (r_u8(action + 45) == 0)
            continue;
        if (attributes & 0x400000)
            continue;
        if (r_u8(action + 25) != 0)
            continue;
        if (r_s16(action + 20) < 0)
            continue;
        mask = attributes & 0xFFFF0000;
        category = r_u32(state + 20);
        if ((mask & 0x1C0000) == 0)
            mask |= 0xC0000;
        if ((mask & r_u32(0x80098230 + category * 4)) == 0)
            continue;
        selection = r_u32(state + 0x230);
        kind = attributes & 0xFFFF;
        if (selection & 2)
        {
            if (kind == 0x412)
                continue;
        }
        else if (selection & 1)
        {
            if (kind == 0x412 || kind == 0x10F)
                continue;
        }
        else if (selection & 4)
        {
            if (kind == 0x412 || kind == 0x217)
                continue;
        }
        w_u16(entry + 8, 1);
        selected += 1;
    }
    w_u16(state + 0x72, r_u16(state + 0x72) + selected);
    return selected;
}

uint32 t3_800775c0(uint32 ordering_entry, uint32 packet, uint32 cursor, uint32 flags)
{
    uint32 index, color, uv, flips, first, second, third, fourth, y, x, xy, tag;
    FUNCTION_MARKER(0x800775C0, "SLUS_004.02");
    index = flags & 7;
    if ((flags & 0xFF) == 16)
        index = 8;
    color = r_u32(cursor + 12);
    uv = r_u16(0x80027AE8 + index * 4);
    flips = r_u16(0x80027AEA + index * 4);
    w_u32(packet + 4, color | 0x2C000000);
    w_u32(packet + 20, uv + 19);
    w_u32(packet + 12, uv);
    w_u32(packet + 28, uv + 0x1F00);
    w_u32(packet + 36, uv + 0x1F13);
    if (flips & 1)
    {
        first = r_u32(packet + 12);
        second = r_u32(packet + 20);
        fourth = r_u32(packet + 36);
        w_u32(packet + 20, first);
        third = r_u32(packet + 28);
        w_u32(packet + 12, second);
        w_u32(packet + 28, fourth);
        w_u32(packet + 36, third);
    }
    if (flips & 2)
    {
        first = r_u32(packet + 12);
        third = r_u32(packet + 28);
        fourth = r_u32(packet + 36);
        w_u32(packet + 28, first);
        second = r_u32(packet + 20);
        w_u32(packet + 12, third);
        w_u32(packet + 20, fourth);
        w_u32(packet + 36, second);
    }
    w_u32(packet + 20, r_u32(packet + 20) | 0x60000);
    uv = r_u32(packet + 12);
    w_u32(packet + 12, uv | ((flags & 8) ? 0x7FB30000 : 0x7FB40000));
    y = (uint32)r_s16(cursor + 2);
    x = (uint32)r_s16(cursor);
    xy = ((y - 12) << 16) | x;
    w_u32(packet + 16, xy + 19);
    w_u32(packet + 24, xy + 0x1F0000);
    w_u32(packet + 32, xy + 0x1F0013);
    w_u32(packet + 8, xy);
    w_u16(cursor, xy + 20);
    tag = r_u32(ordering_entry);
    w_u32(packet, (tag & 0xFFFFFF) | 0x09000000);
    tag = r_u32(ordering_entry);
    w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    return packet + 40;
}

uint32 t3_80054f14(uint32 ordering_entry, uint32 packet, uint32 translucent, uint32 replacement_color)
{
    uint32 table, count, opcode, row, i, colors[4], xy0, xy1, xy2, xy3;
    FUNCTION_MARKER(0x80054F14, "SLUS_004.02");
    table = translucent ? 0x80022B0C : 0x80022A8C;
    count = translucent ? 2 : 4;
    opcode = translucent ? 0x3A000000 : 0x38000000;
    for (row = 0; row < count; ++row, table += 32)
    {
        colors[0] = r_u32(table + 16);
        colors[1] = r_u32(table + 20);
        colors[2] = r_u32(table + 24);
        colors[3] = r_u32(table + 28);
        for (i = 0; i < 4; ++i)
            if (colors[i] == 0xFFFFFFFF)
                colors[i] = replacement_color;
        colors[0] |= opcode;
        xy0 = r_u32(table);
        xy1 = r_u32(table + 4);
        xy2 = r_u32(table + 8);
        xy3 = r_u32(table + 12);
        packet = t3_8004d8b0(ordering_entry, packet, colors[0], colors[1], colors[2], colors[3], xy0, xy1, xy2, xy3);
    }
    return packet;
}

uint32 t3_800748d4(uint32 object)
{
    uint32 elapsed, index, child, value;
    sint32 threshold;
    FUNCTION_MARKER(0x800748D4, "SLUS_004.02");
    elapsed = r_u32(object + 0x40);
    index = r_u32(object + 0x44);
    elapsed += 1;
    w_u32(object + 0x40, elapsed);
    threshold = r_s16(0x80025738 + index * 2);
    if ((sint32)elapsed < threshold)
        return (uint32)threshold;
    child = t3_8006f4f4();
    if (child != 0xFFFFFFFF)
    {
        w_u8(child + 0xC, 20);
        if (r_u8(object + 0xD) & 0x10)
            w_u8(child + 0xD, r_u8(child + 0xD) | 0x10);
        if (r_u8(object + 0xD) & 8)
            w_u8(child + 0xD, r_u8(child + 0xD) | 8);
        w_u32(child + 0x28, r_u32(object + 0x28));
        index = r_u32(object + 0x44);
        value = (uint32)r_s16(0x8002573C + index * 2);
        w_u32(child + 0x2C, 0 - value);
        w_u32(child + 0x30, r_u32(object + 0x30));
        index = r_u32(object + 0x44);
        value = r_u16(0x8002573A + index * 2);
        w_u16(child + 0x10, 0);
        w_u16(child + 0x18, value << 4);
        w_u16(child + 0x12, r_u16(object + 0x12));
    }
    index = r_u32(object + 0x44) + 3;
    w_u32(object + 0x44, index);
    if (r_s16(0x80025738 + index * 2) == -1)
        return t3_8006f5ac(object);
    return 0xFFFFFFFF;
}

uint32 t3_80047f14(uint32 first_actor, uint32 second_actor, uint32 records)
{
    uint32 actor, output, capsule, record, pass, category, record_index, mask, bit, end, start;
    FUNCTION_MARKER(0x80047F14, "SLUS_004.02");
    mask = 0;
    actor = first_actor;
    bit = 2;
    for (pass = 0; pass < 2; ++pass)
    {
        output = actor + 0x13C;
        capsule = actor + 0x20C;
        for (category = 0; category < 14; ++category, capsule += 20)
        {
            if (r_s16(capsule + 12) == 0)
                continue;
            record = records;
            for (record_index = 0; record_index < 4; ++record_index, record += 24)
            {
                if (!t3_8004770c(record, capsule))
                    continue;
                w_u16(output + 24, category);
                w_u32(output, r_u32(record + 12));
                w_u32(output + 4, r_u32(record + 16));
                w_u32(output + 8, r_u32(record + 20));
                end = r_u16(record + 12);
                start = r_u16(record);
                w_u16(output + 16, end - start);
                end = r_u16(record + 16);
                start = r_u16(record + 4);
                w_u16(output + 18, end - start);
                end = r_u16(record + 20);
                start = r_u16(record + 8);
                mask |= bit;
                w_u16(output + 20, end - start);
                break;
            }
            if (record_index < 4)
                break;
        }
        actor = second_actor;
        bit = 1;
    }
    return mask;
}

uint32 t3_80046270(uint32 actor, const uint32 *position, uint32 *camera)
{
    uint32 input[7], output[8], value, bound, x, z, origin_x, origin_z, dx, dz, squared, index;
    sint32 angle, sine, cosine;
    FUNCTION_MARKER(0x80046270, "SLUS_004.02");
    camera[5] = position[0];
    camera[6] = position[1];
    camera[7] = position[2];
    input[6] = 0;
    input[0] = camera[5];
    input[1] = camera[6];
    input[2] = camera[7];
    input[3] = r_u32(actor + 0xF68);
    input[4] = r_u32(actor + 0xF6C);
    input[5] = r_u32(actor + 0xF70);
    t3_80045fec(input, camera);
    input[4] = r_u32(actor + 0x994) - 290;
    t3_80045fec(input, output);
    bound = output[0] + 4004;
    value = camera[0];
    if ((sint32)bound < (sint32)value)
        value = bound;
    camera[0] = value;
    input[4] = 0;
    t3_80045fec(input, output);
    bound = output[0] - 6917;
    value = camera[0];
    if ((sint32)bound < (sint32)value)
        value = bound;
    camera[0] = value;
    if (r_s16(0x800A8CDC) && (sint32)value < 0)
        camera[0] = 0;
    x = camera[5];
    origin_x = r_u32(actor + 0xF68);
    dx = x - origin_x;
    origin_z = r_u32(actor + 0xF70);
    z = camera[7];
    dz = z - origin_z;
    squared = dx * dx + dz * dz;
    if ((sint32)squared >= 33640000)
        return 0;
    angle = (sint32)camera[1];
    value = angle;
    if (angle < 0)
        value += 63;
    index = ((sint32)value >> 5) & 0x1FFE;
    sine = r_s16(0x8001E8C4 + index) * 5800;
    x = origin_x + sine / 4096;
    cosine = r_s16(0x8001F0C4 + index) * 5800;
    z = origin_z - cosine / 4096;
    camera[5] = x;
    camera[7] = z;
    return z;
}

uint32 t3_80046460(uint32 actor)
{
    uint32 seed, index, value, z, position[3];
    sint32 heading, cosine, sine, mode;
    FUNCTION_MARKER(0x80046460, "SLUS_004.02");
    seed = r_u32(0x800A912C);
    heading = r_s16(actor + 14);
    seed = (seed + 1) * 69069;
    w_u32(0x800A912C, seed);
    index = ((((seed >> 16) * 683) >> 16) + 683 - (uint32)(heading / 16)) & 0xFFF;
    index *= 2;
    cosine = r_s16(0x8001F0C4 + index) * 3953;
    sine = r_s16(0x8001E8C4 + index) * 3953;
    value = r_u32(actor + 0xF68) + cosine / 4096;
    w_u32(0x8009E6B4, value);
    position[0] = value;
    z = r_u32(actor + 0xF70);
    mode = r_s16(0x800A8CDC);
    value = z + sine / 4096;
    w_u32(0x8009E6BC, value);
    position[2] = value;
    value = mode ? r_u32(actor + 0x994) - 290 : (uint32)-850;
    w_u32(0x8009E6B8, value);
    position[1] = value;
    return t3_80046270(actor, position, (uint32 *)psx_addr(0x8009E6A0, 32));
}

uint32 t3_800670fc(uint32 mode, uint32 category, uint32 side)
{
    uint32 selected, data, result;
    FUNCTION_MARKER(0x800670FC, "SLUS_004.02");
    if (mode) {
        --mode;
        if (mode >= 2)
            return 1;
        w_u8(0x800A0419, 0);
        w_u8(0x800A0418, mode);
        if (!mode) {
            side = 0;
            category = 43;
        } else {
            category = 75;
            if (r_s16(0x800A923E) == 14)
                side = 0;
            else if (r_s16(0x800AAACA) == 14)
                side = 1;
        }
        data = t3_80067260(category, 0, side, &selected);
        w_u16(0x800A8CA4, 0xFFFF);
        w_u16(0x800A8C7C, 4096);
        w_u16(0x800A8CBA, 0);
        w_u16(0x800A8C74, 0);
        w_u32(0x800A8CC4, 0);
        w_u32(0x800A8CC8, 0);
        w_u32(0x800A8CCC, 0);
        w_u32(0x800A8CB0, 0);
        w_u16(0x800A8CBC, 0);
        w_u16(0x800A8C78, 0);
        w_u16(0x800A8C7A, 0);
        w_u16(0x800A8CDA, 1);
        result = t3_800663f8(data, 0, 1);
        w_u16(0x800A041A, result - 1);
        t3_800663f8(0, 0x800A0428, 1);
        t3_80069708();
    }
    if (r_s16(0x800A8CA4) >= 0)
        return 1;
    w_u32(0x800A8CB0, 0);
    result = t3_80066a18();
    t3_80064080(2);
    return result;
}

uint32 t3_8004b7d0(uint32 ordering, uint32 packet, uint32 x, uint32 y, uint32 flags)
{
    uint32 colors[3], intensity, palette, index, table, first_xy, second_xy, color_index, position;
    FUNCTION_MARKER(0x8004B7D0, "SLUS_004.02");
    if (flags & 0x10)
        intensity = t3_8004dfe0(r_u32(0x800AE218) << 4);
    else
        intensity = 128;
    palette = 0x80021664 + (flags & 3) * 40;
    for (index = 0; index != 3; ++index)
        colors[index] = t3_8004df6c(r_u32(palette + index * 4), intensity << 1) | 0x40000000;
    position = (x & 0xFFFF) | (y << 16);
    table = 0x800215EC;
    for (index = 0; index != 10; ++index) {
        first_xy = r_u32(table + 4);
        second_xy = r_u32(table + 8);
        color_index = r_u32(table);
        packet = t3_8004da5c(ordering, packet, colors[color_index], position + first_xy, position + second_xy);
        table += 12;
    }
    return packet;
}

uint32 t3_80057d58(uint32 state)
{
    sint32 count, index;
    uint32 entry, action, attributes, kind, required, category, total, selected = 0;
    FUNCTION_MARKER(0x80057D58, "SLUS_004.02");
    count = r_s16(state + 112);
    if (count <= 0)
        return 0;
    for (index = 0; index < count; ++index) {
        entry = 0x8009F978 + (uint32)index * 12;
        if (r_s16(entry + 8))
            continue;
        action = r_u32(entry);
        attributes = r_u32(action + 8);
        kind = r_u8(action + 45);
        if ((attributes & 0x400000) || r_s16(action + 20) <= 0 || !kind)
            continue;
        if (r_u8(action + 25) && !(r_u32(action + 4) & 0x80000))
            continue;
        if ((sint32)kind >= r_s16(state + 0x20A))
            continue;
        if (r_u32(action + 4) & 0x20000)
            continue;
        required = attributes & 0xFFFF0000;
        category = r_u32(state + 20);
        if (!(required & 0x1C0000))
            required |= 0xC0000;
        if (required & r_u32(0x80098230 + category * 4)) {
            w_u16(entry + 8, 1);
            ++selected;
        }
    }
    total = r_u16(state + 114);
    w_u16(state + 114, total + selected);
    return selected;
}

uint32 t3_80072194(uint32 object)
{
    uint32 state, flags, index, packet, value, random;
    FUNCTION_MARKER(0x80072194, "SLUS_004.02");
    state = r_u8(object + 14);
    flags = r_u8(object + 13);
    w_u8(object + 14, state + 1);
    w_u8(object + 13, flags | 0x40);
    for (index = 0; index != 2; ++index) {
        packet = object + 80 + index * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE70);
        w_u16(packet + 22, value);
        value = r_u16(0x800ADD88);
        w_u16(packet + 14, value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    flags = r_u8(object + 13);
    w_u8(object + 13, flags | 2);
    random = (uint32)randPSX();
    w_u32(object + 8, random & 1 ? 0x80025584 : 0x800255A4);
    w_u32(object + 68, (((sint32)random >> 2) & 3) + 5);
    w_u32(object + 72, 0xFFFFFFFF);
    w_u32(object + 64, 0);
    w_u16(object + 24, 2048);
    return t3_8006e120(object + 16);
}

uint32 t3_800718e0(uint32 object)
{
    uint32 buffer, age, packet, table, result;
    FUNCTION_MARKER(0x800718E0, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    packet = object + buffer * 40;
    w_u16(packet + 94, r_u16(0x800ADD8C));
    age = r_u32(object + 72) + 1;
    w_u32(object + 72, age);
    if ((sint32)age >= 8)
        return t3_8006f5ac(object);
    table = 0x80025524 + (age << 1);
    packet += 80;
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    age = r_u32(object + 72);
    w_u32(object + 8, age & 1 ? 0x800254C4 : 0x800254E4);
    result = r_u32(object + 76);
    if (result) {
        age = r_u32(object + 72);
        result = r_u8(0x80025534 + age);
        if (!result)
            w_u32(object + 76, 0);
    }
    return result;
}

uint32 t3_80062f24(uint32 state, uint32 actor, uint32 opponent)
{
    uint32 previous, result, value, data;
    sint32 angle;
    FUNCTION_MARKER(0x80062F24, "SLUS_004.02");
    previous = r_u16(0x800A8C8E);
    w_u16(0x800A8C90, state);
    w_u16(0x800A8C9E, 0xFFFF);
    w_u16(0x800A8C98, previous);
    result = t3_8006782c(actor, opponent, state);
    if ((sint32)result < 0) {
        w_u32(0x800A8CA0, 0xFFFFFFFF);
    } else if (r_s16(0x800A8CA4) < 0) {
        angle = r_s16(opponent + 44);
        w_u32(0x800A8CB0, (1024 - angle / 16) & 0xFFF);
        value = r_u32(opponent + 0xF68);
        data = r_u32(0x800A8CA0);
        w_u32(0x800A8CC4, value);
        w_u32(0x800A8CC8, r_u32(opponent + 4));
        value = r_u32(opponent + 0xF70);
        w_u16(0x800A8CBA, 0);
        w_u32(0x800A8CCC, value);
        t3_80066dc4(data);
        w_u16(0x800A8CDE, 1);
        w_u16(0x800A8C92, 0xFFFF);
        w_u16(0x800A8C70, 6);
        w_u16(0x800A8C96, result);
        w_u16(0x800A8C94, result);
    } else {
        w_u16(0x800A8C92, 16);
        w_u16(0x800A8C70, 2);
        w_u16(0x800A8C96, 1);
        w_u16(0x800A8C94, 1);
        t3_800666e4();
    }
    w_u16(0x800A8C92, 0xFFFF);
    w_u16(0x800A8C7A, 4096);
    w_u16(0x800A8C78, 0);
    w_u16(0x800A8C7C, 0);
    w_u16(0x800A8C9A, 1);
    return 0x800A8C70;
}

uint32 t3_80074c74(uint32 object)
{
    uint32 age, packet, table, velocity, height, flags, result;
    FUNCTION_MARKER(0x80074C74, "SLUS_004.02");
    age = r_u32(object + 72) + 1;
    w_u32(object + 72, age);
    if ((sint32)age >= 54)
        return t3_8006f5ac(object);
    packet = object + r_u32(0x800ADEFC) * 40 + 80;
    table = 0x800257B0 + (age & 0x7FFE);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    velocity = r_u32(object + 32);
    height = r_u32(object + 44);
    flags = r_u8(object + 13);
    velocity -= 14745;
    w_u32(object + 32, velocity);
    w_u32(object + 44, height + (uint32)((sint32)velocity >> 16));
    result = flags & 2;
    if (result) {
        age = r_u32(object + 72);
        result = r_u8(0x800257E8 + (uint32)((sint32)age >> 1));
        if (!result) {
            result = flags & 0xFD;
            w_u8(object + 13, result);
        }
    }
    return result;
}

uint32 t3_80074b24(uint32 object)
{
    uint32 state, flags, index, packet, value, vx, vy, vz, x, y, z, result;
    FUNCTION_MARKER(0x80074B24, "SLUS_004.02");
    state = r_u8(object + 14);
    flags = r_u8(object + 13);
    w_u8(object + 14, state + 1);
    w_u8(object + 13, flags | 0x40);
    for (index = 0; index != 2; ++index) {
        packet = object + 80 + index * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE74);
        w_u16(packet + 22, value);
        value = r_u16(0x800ADD80);
        w_u16(packet + 14, value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    flags = r_u8(object + 13);
    w_u8(object + 13, flags | 2);
    value = ((uint32)randPSX() & 1) ? 0x80025770 : 0x80025790;
    w_u32(object + 8, value);
    w_u32(object + 72, 0xFFFFFFFF);
    t3_8006e120(object + 16);
    vx = r_s16(object + 30);
    x = r_u32(object + 40);
    vy = r_s16(object + 34);
    vz = r_s16(object + 38);
    w_u32(object + 40, x + vx);
    y = r_u32(object + 44);
    z = r_u32(object + 48);
    result = y + vy;
    w_u32(object + 44, result);
    w_u32(object + 48, z + vz);
    w_u32(object + 32, 0);
    return result;
}

uint32 t3_80036974(void)
{
    uint32 configuration, destination, buffer, source, count, offset, value, selection;
    FUNCTION_MARKER(0x80036974, "SLUS_004.02");
    configuration = t3_8006c800(0);
    destination = t3_8004807c();
    buffer = t3_800561a0();
    t3_8006c588(31);
    selection = r_u16(0x800ADC84);
    t3_8006c618(selection, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
    if (t3_8006c63c(16, 0)) {
        selection = r_u16(0x800ADC84);
        t3_80052ad0(buffer, selection + 34);
        source = t3_80031cbc(0, buffer);
        t3_8006e37c(source);
        source = t3_80031cbc(1, buffer);
        count = t3_80031cf0(1, buffer);
        offset = 0;
        while ((sint32)offset < (sint32)count) {
            value = r_u32(source);
            source += 4;
            offset += 4;
            w_u32(destination, value);
            destination += 4;
        }
        selection = r_u16(0x800ADC84);
        t3_80052ad0(configuration, selection + 54);
    }
    selection = r_u16(0x800ADC84);
    t3_8006c95c(selection, configuration);
    selection = r_s16(0x800ADC84);
    t3_8004808c(selection);
    return t3_8006c63c(16, 1);
}

uint32 t3_80057538(uint32 state)
{
    sint32 count, index, length;
    uint32 entry, action, flags, kind, attributes, required, category, total, selected = 0;
    FUNCTION_MARKER(0x80057538, "SLUS_004.02");
    count = r_s16(state + 112);
    if (count <= 0)
        return 0;
    for (index = 0; index < count; ++index) {
        entry = 0x8009F978 + (uint32)index * 12;
        if (r_s16(entry + 8))
            continue;
        action = r_u32(entry);
        length = r_s16(action + 20);
        flags = r_u32(action + 4);
        if (length <= 0)
            continue;
        kind = r_u8(action + 45);
        if (!kind || r_u8(action + 25))
            continue;
        attributes = r_u32(action + 8);
        if ((attributes & 0x400000) || kind >= 14 || (flags & 0x20000))
            continue;
        required = attributes & 0xFFFF0000;
        category = r_u32(state + 20);
        if (!(required & 0x1C0000))
            required |= 0xC0000;
        if ((required & r_u32(0x80098230 + category * 4)) || (flags & 0x10000)) {
            w_u16(entry + 8, 1);
            ++selected;
        }
    }
    total = r_u16(state + 114);
    w_u16(state + 114, total + selected);
    return selected;
}

uint32 t3_80042d70(void)
{
    uint32 initialized, index, packet;
    FUNCTION_MARKER(0x80042D70, "SLUS_004.02");
    initialized = r_u32(0x80097958);
    if (initialized)
        return initialized;
    w_u32(0x80097958, 1);
    for (index = 0; index != 2; ++index) {
        packet = 0x8009E650 + index * 16;
        t3_8007bca4(packet);
        w_u8(packet + 4, 255);
        w_u8(packet + 5, 255);
        w_u8(packet + 6, 255);
        w_u16(packet + 8, 0);
        w_u16(packet + 10, 0);
        w_u16(packet + 12, 0);
        w_u16(packet + 14, 0);
    }
    return 0;
}

uint32 t3_80042df8(uint32 first_x, uint32 first_y, uint32 first_z, uint32 second_x, uint32 second_y, uint32 second_z)
{
    SVECTOR first_vertex = {0}, second_vertex = {0};
    uint32 first_screen, second_screen, buffer, packet, ordering;
    FUNCTION_MARKER(0x80042DF8, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    packet = 0x8009E650 + (buffer << 4);
    t3_8007bca4(packet);
    w_u8(packet + 4, 255);
    w_u8(packet + 5, 255);
    w_u8(packet + 6, 0);
    t3_8004adc4(first_x, first_y, first_z);
    t3_80036ad4(&first_vertex, &first_screen);
    w_u16(packet + 8, first_screen);
    w_u16(packet + 10, (sint32)first_screen >> 16);
    t3_8004adc4(second_x, second_y, second_z);
    t3_80036ad4(&second_vertex, &second_screen);
    ordering = r_u32(0x800A9218);
    w_u16(packet + 12, second_screen);
    w_u16(packet + 14, (sint32)second_screen >> 16);
    return t3_8007ba74(ordering, packet);
}

uint32 t3_800742ac(uint32 object)
{
    uint32 buffer, age, packet, table, value;
    FUNCTION_MARKER(0x800742AC, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    age = r_u32(object + 0x48) + 1;
    packet = object + buffer * 40 + 0x50;
    w_u32(object + 0x48, age);
    table = 0x80025504 + (age << 1);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    if (r_s32(object + 0x48) < 7)
        return 1;
    value = r_u8(object + 14);
    w_u8(object + 14, value + 1);
    value = (uint32)randPSX();
    w_u32(object + 0x48, value);
    return value;
}

uint32 t3_80074a30(uint32 event)
{
    uint32 actor, animation, flags, x, y, z, state;
    sint32 frame;
    FUNCTION_MARKER(0x80074A30, "SLUS_004.02");
    if (r_u8(event + 13) & 2) {
        flags = r_u8(event + 13);
        actor = flags & 0x10 ? 0x800A9228 : 0x800AAAB4;
        animation = r_u32(actor + 0x54);
        if (r_u8(r_u32(animation + 0x28)) != 27) {
            flags = r_u8(event + 13);
            w_u8(event + 13, flags & 0xFD);
        } else {
            frame = r_s16(actor + 0x58);
            if (frame < r_u8(animation + 0x2D)) {
                flags = r_u8(event + 13);
                w_u8(event + 13, flags & 0xFD);
            }
        }
    }
    x = r_u32(event + 40);
    y = r_u32(event + 44);
    z = r_u32(event + 48);
    state = r_u8(event + 14);
    w_u32(event + 52, x);
    w_u32(event + 56, y);
    w_u32(event + 60, z);
    if (!state) {
        t3_80074b24(event);
        return t3_80074c74(event);
    }
    if (state == 1)
        return t3_80074c74(event);
    return 1;
}

uint32 t3_800756c4(uint32 index, uint32 header, uint32 bank, uint32 resource)
{
    uint32 body, sound_address;
    FUNCTION_MARKER(0x800756C4, "SLUS_004.02");
    body = t3_800561a0();
    t3_800529c4();
    t3_800529c4();
    if (bank == 3)
        sound_address = 0x80000 - r_u32(0x800278A8 + resource * 4);
    else
        sound_address = r_u32(0x8002790C + bank * 4);
    t3_8008aa28(header, (sint16)bank, sound_address);
    t3_8008ae58(body, (sint16)bank);
    return (uint32)(sint32)SsVabTransCompleted(1);
}

uint32 t3_800757a8(uint32 side)
{
    uint32 actor, sound;
    sint32 identity;
    FUNCTION_MARKER(0x800757A8, "SLUS_004.02");
    actor = 0x800A9228 + side * 6284;
    if (r_s16(actor + 20) == 34)
        return 1;
    sound = r_s16(actor + 26) == 21 ? 0xC64B : 0xC640;
    identity = r_s16(actor + 18);
    t3_800753bc(identity, sound, 0);
    return 0;
}

uint32 t3_80072f38(uint32 object)
{
    uint32 state, flags, index, packet, value, random, result;
    FUNCTION_MARKER(0x80072F38, "SLUS_004.02");
    state = r_u8(object + 14);
    flags = r_u8(object + 13);
    w_u8(object + 14, state + 1);
    w_u8(object + 13, flags | 0x40);
    for (index = 0; index != 2; ++index) {
        packet = object + 0x50 + index * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE70);
        w_u16(packet + 22, value);
        value = r_u16(0x800ADD88);
        w_u16(packet + 14, value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    flags = r_u8(object + 13);
    w_u32(object + 8, flags & 0x20 ? 0x800256C0 : 0x800256A0);
    random = (uint32)randPSX();
    result = (sint32)random >> 4;
    w_u32(object + 64, (random & 15) + 60);
    w_u32(object + 68, result);
    return result;
}

uint32 t3_800732a4(uint32 object)
{
    uint32 buffer, age, packet, table, velocity, height, result;
    FUNCTION_MARKER(0x800732A4, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    age = r_u32(object + 64);
    packet = object + buffer * 40 + 0x50;
    table = 0x800255CC + (age & 0xFFFFFFFE);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    velocity = r_u32(object + 32);
    height = r_u32(object + 44);
    age = r_u32(object + 64);
    velocity -= 98304;
    w_u32(object + 32, velocity);
    result = height + (uint32)((sint32)velocity >> 16);
    ++age;
    w_u32(object + 64, age);
    w_u32(object + 44, result);
    if ((sint32)age >= 7) {
        result = r_u8(object + 14);
        w_u32(object + 64, 0xFFFFFFFF);
        ++result;
        w_u8(object + 14, result);
    }
    return result;
}

uint32 t3_80073394(uint32 object)
{
    uint32 age, packet, table, buffer, palette, velocity, height;
    FUNCTION_MARKER(0x80073394, "SLUS_004.02");
    age = r_u32(object + 64) + 1;
    w_u32(object + 64, age);
    if ((sint32)age >= 5)
        return t3_8006f5ac(object);
    buffer = r_u32(0x800ADEFC);
    packet = object + buffer * 40 + 0x50;
    table = 0x800255D4 + (age & 0xFFFFFFFE);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    buffer = r_u32(0x800ADEFC);
    palette = r_u16(0x800ADD8C);
    w_u16(object + buffer * 40 + 0x5E, palette);
    velocity = r_u32(object + 32);
    height = r_u32(object + 44);
    velocity -= 98304;
    w_u32(object + 32, velocity);
    height += (uint32)((sint32)velocity >> 16);
    w_u32(object + 44, height);
    return height;
}

uint32 t3_8007270c(uint32 object)
{
    uint32 state, flags, index, packet, value, random, combined, result;
    FUNCTION_MARKER(0x8007270C, "SLUS_004.02");
    state = r_u8(object + 14);
    flags = r_u8(object + 13);
    w_u8(object + 14, state + 1);
    w_u8(object + 13, flags | 0x40);
    for (index = 0; index != 2; ++index) {
        packet = object + 0x50 + index * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE70);
        w_u16(packet + 22, value);
        value = r_u16(0x800ADD84);
        w_u16(packet + 14, value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    w_u32(object + 8, 0x8002561C);
    random = (uint32)randPSX();
    combined = random | ((uint32)randPSX() << 16);
    result = (sint32)combined >> 2;
    w_u32(object + 44, 0xFFFFFF00);
    w_u32(object + 32, 0);
    w_u32(object + 64, result);
    w_u32(object + 68, (combined & 31) + 60);
    return result;
}

uint32 t3_80072828(uint32 object)
{
    uint32 buffer, age, packet, table, result;
    FUNCTION_MARKER(0x80072828, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    age = r_u32(object + 0x40) + 1;
    packet = object + buffer * 40 + 0x50;
    w_u32(object + 0x40, age);
    table = 0x8002553C + (age & 14);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 15);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 15);
    w_u8(packet + 36, r_u8(table) + 15);
    w_u8(packet + 37, r_u8(table + 1) + 15);
    result = r_u32(object + 0x44) - 1;
    w_u32(object + 0x44, result);
    if ((sint32)result < 0) {
        result = r_u8(object + 14);
        w_u32(object + 0x40, 0xFFFFFFFF);
        ++result;
        w_u8(object + 14, result);
    }
    return result;
}

uint32 t3_80072900(uint32 object)
{
    uint32 age, packet, table, velocity, height;
    FUNCTION_MARKER(0x80072900, "SLUS_004.02");
    age = r_u32(object + 0x40) + 1;
    w_u32(object + 0x40, age);
    if ((sint32)age >= 16)
        return t3_8006f5ac(object);
    packet = object + r_u32(0x800ADEFC) * 40 + 0x50;
    table = 0x800255FC + (age << 1);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 15);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 15);
    w_u8(packet + 36, r_u8(table) + 15);
    w_u8(packet + 37, r_u8(table + 1) + 15);
    velocity = r_u32(object + 32);
    height = r_u32(object + 44);
    velocity -= 147456;
    w_u32(object + 32, velocity);
    height += (uint32)((sint32)velocity >> 16);
    w_u32(object + 44, height);
    return height;
}

uint32 t3_80072d5c(uint32 object)
{
    uint32 age, packet, table, flags, result;
    FUNCTION_MARKER(0x80072D5C, "SLUS_004.02");
    age = r_u32(object + 0x48) + 1;
    w_u32(object + 0x48, age);
    if ((sint32)age >= 21)
        return t3_8006f5ac(object);
    packet = object + r_u32(0x800ADEFC) * 40 + 0x50;
    table = 0x8002565C + (age << 1);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    flags = r_u8(object + 13);
    result = flags & 2;
    if (result) {
        age = r_u32(object + 0x48);
        result = r_u8(0x80025688 + age);
        if (!result) {
            result = flags & 0xFD;
            w_u8(object + 13, result);
        }
    }
    return result;
}

uint32 t3_800722d4(uint32 object)
{
    uint32 buffer, age, packet, table, value;
    FUNCTION_MARKER(0x800722D4, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    age = r_u32(object + 0x48) + 1;
    packet = object + buffer * 40 + 0x50;
    w_u32(object + 0x48, age);
    table = 0x800255C4 + (age & 0xFFFFFFFE);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    if (r_s32(object + 0x48) < 7)
        return 1;
    value = r_u8(object + 14);
    w_u8(object + 14, value + 1);
    value = (uint32)randPSX();
    w_u32(object + 0x48, value);
    return value;
}

uint32 t3_800723cc(uint32 object)
{
    uint32 buffer, age, packet, table, elapsed, duration;
    FUNCTION_MARKER(0x800723CC, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    age = r_u32(object + 0x48) + 1;
    packet = object + buffer * 40 + 0x50;
    w_u32(object + 0x48, age);
    table = 0x800255CC + (age & 6);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    elapsed = r_u32(object + 0x40);
    duration = r_u32(object + 0x44);
    ++elapsed;
    w_u32(object + 0x40, elapsed);
    if ((sint32)elapsed < (sint32)duration)
        return 4;
    w_u8(object + 14, 4);
    w_u32(object + 0x48, 0xFFFFFFFF);
    return 0xFFFFFFFF;
}

uint32 t3_800724c4(uint32 object)
{
    uint32 buffer, age, packet, table, flags, result;
    FUNCTION_MARKER(0x800724C4, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    packet = object + buffer * 40;
    w_u16(packet + 0x5E, r_u16(0x800ADD8C));
    age = r_u32(object + 0x48) + 1;
    w_u32(object + 0x48, age);
    if ((sint32)age >= 9)
        return t3_8006f5ac(object);
    table = 0x800255D4 + (age & 0xFFFFFFFE);
    packet += 0x50;
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    flags = r_u8(object + 13);
    result = flags & 2;
    if (result) {
        age = r_u32(object + 0x48);
        result = r_u8(0x800255E0 + age);
        if (!result) {
            result = flags & 0xFD;
            w_u8(object + 13, result);
        }
    }
    return result;
}

uint32 t3_800725ec(uint32 object)
{
    uint32 state, scaled, threshold, result, created, value;
    sint32 level;
    FUNCTION_MARKER(0x800725EC, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state) {
        t3_8007270c(object);
        t3_80072828(object);
    } else if (state == 1) {
        t3_80072828(object);
    } else if (state == 2) {
        t3_80072900(object);
    }
    level = r_s16(0x800A9194);
    scaled = ((80 - (uint32)level) * 10) << 14;
    threshold = (uint32)((sint32)scaled / 80000) << 2;
    result = (sint32)randPSX() < (sint32)threshold;
    if (result) {
        created = t3_8006f4f4();
        result = 0xFFFFFFFF;
        if (created != 0xFFFFFFFF) {
            w_u8(created + 12, 7);
            w_u32(created + 40, r_u32(object + 40));
            w_u32(created + 44, r_u32(object + 44));
            value = r_u32(object + 48);
            result = 1024;
            w_u16(created + 16, 1024);
            w_u16(created + 18, 0);
            w_u32(created + 48, value);
        }
    }
    return result;
}

uint32 t3_800717c4(uint32 object)
{
    uint32 buffer, age, packet, table, elapsed, duration, value;
    FUNCTION_MARKER(0x800717C4, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    age = r_u32(object + 0x48) + 1;
    packet = object + buffer * 40 + 0x50;
    w_u32(object + 0x48, age);
    table = 0x80025514 + (age & 7) * 2;
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    elapsed = r_u32(object + 0x40);
    duration = r_u32(object + 0x44);
    ++elapsed;
    w_u32(object + 0x40, elapsed);
    if ((sint32)elapsed < (sint32)duration)
        return 1;
    value = r_u8(object + 14);
    w_u32(object + 0x48, 0xFFFFFFFF);
    w_u8(object + 14, value + 1);
    value = ((uint32)randPSX() & 1) ? 0x800254C4 : 0x800254E4;
    w_u32(object + 8, value);
    return value;
}

uint32 t3_80071a24(uint32 event)
{
    uint32 state, value, x, y, z, vx, vy, vz;
    FUNCTION_MARKER(0x80071A24, "SLUS_004.02");
    state = r_u8(event + 14);
    if (!state) {
        t3_80071afc(event);
        t3_80071ca8(event);
    } else if (state == 1) {
        t3_80071ca8(event);
    } else if (state == 2) {
        t3_80071d80(event);
    }
    value = r_u32(event + 0x20);
    w_u32(event + 0x20, value - 196608);
    vx = r_s16(event + 0x1E);
    x = r_u32(event + 0x28);
    vy = r_s16(event + 0x22);
    vz = r_s16(event + 0x26);
    w_u32(event + 0x28, x + vx);
    y = r_u32(event + 0x2C);
    z = r_u32(event + 0x30);
    w_u32(event + 0x2C, y + vy);
    w_u32(event + 0x30, z + vz);
    value = r_u32(event + 0x2C);
    if ((sint32)value >= 0)
        return t3_8006f5ac(event);
    return value;
}

uint32 t3_80071ca8(uint32 object)
{
    uint32 buffer, age, packet, table, duration, value;
    FUNCTION_MARKER(0x80071CA8, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    age = r_u32(object + 0x40) + 1;
    packet = object + buffer * 40 + 0x50;
    w_u32(object + 0x40, age);
    table = 0x8002553C + (age & 14);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 15);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 15);
    w_u8(packet + 36, r_u8(table) + 15);
    w_u8(packet + 37, r_u8(table + 1) + 15);
    age = r_u32(object + 0x40);
    duration = r_u32(object + 0x44);
    if ((sint32)age < (sint32)duration)
        return 1;
    value = r_u8(object + 14);
    w_u32(object + 0x40, 0xFFFFFFFF);
    ++value;
    w_u8(object + 14, value);
    return value;
}

uint32 t3_80071d80(uint32 object)
{
    uint32 age, packet, table, value;
    FUNCTION_MARKER(0x80071D80, "SLUS_004.02");
    age = r_u32(object + 0x40) + 1;
    w_u32(object + 0x40, age);
    if ((sint32)age >= 12)
        return t3_8006f5ac(object);
    packet = object + r_u32(0x800ADEFC) * 40 + 0x50;
    table = 0x8002554C + (age << 1);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 15);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 15);
    w_u8(packet + 36, r_u8(table) + 15);
    value = r_u8(table + 1) + 15;
    w_u8(packet + 37, value);
    return value;
}

uint32 t3_8006b8bc(uint32 volume)
{
    CdlATV mix;
    uint32 scaled;
    sint32 mode;
    FUNCTION_MARKER(0x8006B8BC, "SLUS_004.02");
    mode = r_s16(0x800A061C);
    w_u16(0x800A068C, volume);
    if (!mode || mode == 2) {
        mix.val0 = 0;
        mix.val1 = 0;
        mix.val2 = 0;
        mix.val3 = 0;
    } else if (r_u8(0x80097F04)) {
        mix.val0 = volume;
        mix.val1 = 0;
        mix.val2 = volume;
        mix.val3 = 0;
    } else {
        scaled = volume * 80;
        mix.val3 = (sint32)scaled / 100;
        mix.val2 = mix.val3;
        mix.val1 = mix.val3;
        mix.val0 = mix.val3;
    }
    return t3_80090a88(&mix);
}

uint32 t3_8006c470(uint32 file, uint32 destination)
{
    uint32 entry, size;
    FUNCTION_MARKER(0x8006C470, "SLUS_004.02");
    if (r_u8(0x800A069F)) {
        t3_8006c1fc();
        while (r_u8(0x800A069E))
            ;
    }
    w_u8(0x800A069F, 0);
    w_u16(0x800A069C, 0);
    w_u32(0x800A0698, 0);
    w_u8(0x800A069E, 0);
    t3_8006bf20((sint32)file, destination, 0);
    t3_8006c084();
    while (r_u8(0x800A069F))
        ;
    entry = r_u16(0x80098574 + file * 2);
    size = r_u32(0x80024400 + entry * 8 + 4);
    return (size + 3) & 0xFFFFFFFC;
}

uint32 t3_8006c864(uint32 index, uint32 load)
{
    uint32 buffer, base;
    FUNCTION_MARKER(0x8006C864, "SLUS_004.02");
    if (r_s16(0x800AE204) == 6) {
        base = r_u32(0x800ADD0C);
        return base ? base + 1296 : 0;
    }
    if (load) {
        buffer = t3_800561a0();
        t3_80052ad0(buffer, (sint32)(index + 34));
        t3_8006e37c(buffer);
        if ((0x34AF >> (index & 31)) & 1) {
            buffer = t3_800561a0();
            DrawSync(0);
            t3_80052ad0(buffer, (sint32)(index + 54));
            t3_8006e37c(buffer);
        }
    }
    base = r_u32(0x800ADD0C);
    buffer = base ? base + 1296 : 0;
    if (load)
        t3_80052ad0(buffer, (sint32)(index + 98));
    return buffer;
}

uint32 t3_800716c0(uint32 object)
{
    uint32 buffer, age, packet, table, value;
    FUNCTION_MARKER(0x800716C0, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    age = r_u32(object + 0x48) + 1;
    packet = object + buffer * 40 + 0x50;
    w_u32(object + 0x48, age);
    table = 0x80025504 + (age << 1);
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    if (r_s32(object + 0x48) < 7)
        return 1;
    value = r_u8(object + 14);
    w_u8(object + 14, value + 1);
    value = (uint32)randPSX();
    w_u32(object + 0x48, value);
    w_u16(object + 24, 4096);
    return t3_8006e120(object + 16);
}

uint32 t3_80067070(uint32 mode, uint32 index, uint32 side, uint32 category)
{
    uint32 selected;
    FUNCTION_MARKER(0x80067070, "SLUS_004.02");
    if (!mode) {
        side = 0;
        category = (index << 1) + 43;
    } else if (mode == 1) {
        if (index)
            return 0;
        category = 75;
        if (r_s16(0x800A923E) == 14)
            side = 0;
        else if (r_s16(0x800AAACA) == 14)
            side = 1;
    }
    return t3_80067260(category, 0, side, &selected);
}

uint32 t3_800673e4(uint32 actor)
{
    uint32 animation, category, entry, owner, result, first, second;
    sint32 identity, alternate, selection;
    FUNCTION_MARKER(0x800673E4, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    identity = r_s16(actor + 0x12);
    category = r_u8(animation + 0x24);
    alternate = r_s16(0x800A8C90);
    if (category < 43) {
        entry = 0x80098378 + category * 8;
    } else {
        owner = r_u32(0x800ADC20 + (uint32)identity * 4);
        entry = r_u32(owner + 0x20) + (category - 43) * 8;
    }
    entry += 8;
    selection = r_s16(0x800A8CA6);
    w_u16(0x800A8CBC, 0);
    entry += (uint32)selection * 8;
    first = r_u16(entry);
    result = r_u32(entry + 4);
    if (first != 1 && alternate) {
        if (r_u16(entry + 8)) {
            first = r_u16(entry + 2);
            second = r_u16(entry + 10);
            if (first >= second)
                result = r_u32(entry + 12);
            else
                w_u16(0x800A8CBC, 1);
        } else {
            w_u16(0x800A8CBC, 1);
        }
    }
    if ((sint32)result < 0 && result != 0xFFFFFFFF) {
        owner = r_u32(0x800ADC20 + (uint32)identity * 4);
        result = (result & 0xFFFF) + r_u32(owner + 0x24);
    }
    return result;
}

uint32 t3_80069304(void)
{
    uint32 result, count, index, camera, position, destination, cached_x;
    sint32 signed_count, first_y, second_y;
    FUNCTION_MARKER(0x80069304, "SLUS_004.02");
    result = t3_80062b04();
    if ((sint32)result < 0) {
        w_u16(0x800A0490, 0xFFFF);
    } else {
        signed_count = r_s16(0x800A0490);
        count = r_u16(0x800A0490);
        w_u16(0x800A0490, signed_count < 0 ? 0 : count + 1);
    }
    for (index = 0; index != 2; ++index) {
        signed_count = r_s16(0x800A0490);
        count = r_u16(0x800A0490);
        if (signed_count < 0 || !(count & 3)) {
            camera = 0x800AA17C + index * 6284;
            position = 0x800A0478 + index * 4;
            destination = 0x800ADCA8 + index * 16;
            w_u32(position, r_u32(camera + 0x14));
            w_u32(position + 16, r_u32(camera + 0x1C));
            first_y = r_s32(0x800A9BBC + index * 6284);
            second_y = r_s32(camera + 0x18);
            if (second_y < first_y)
                first_y = second_y;
            cached_x = r_u32(position);
            w_u32(position + 8, first_y);
            w_u32(destination, cached_x);
            w_u32(destination + 4, r_u32(position + 8));
            w_u32(destination + 8, r_u32(position + 16));
        }
    }
    return 0;
}

uint32 t3_80069414(uint32 first, uint32 second)
{
    uint32 active, mode, distance, difference, scaled;
    sint32 offset, base_angle, angle, current, target;
    FUNCTION_MARKER(0x80069414, "SLUS_004.02");
    active = r_u8(0x800A04CD);
    if (!active)
        return active;
    t3_80069304();
    mode = r_u8(0x800A04CE);
    distance = r_u32(0x800A04C8);
    offset = r_s16(0x800A04D6);
    base_angle = r_s16(0x800A04D0);
    angle = r_s16(0x800A04D2);
    t3_80064e10(first, second, angle, (uint32)offset + (uint32)base_angle, mode, distance, 1);
    current = r_s16(0x800A04D6);
    target = r_s16(0x800A04D8);
    difference = (uint32)target - (uint32)current;
    w_u8(0x800A04CE, 0);
    if (difference & 0x20000)
        scaled = (difference | 0xFFFC0000) + 127;
    else
        scaled = difference & 0x3FFFF;
    w_u16(0x800A04D6, (uint32)current + (uint32)((sint32)scaled >> 7));
    distance = r_u32(0x800A04C8);
    scaled = (4413 - distance) * 6;
    if ((sint32)scaled < 0)
        scaled += 255;
    w_u32(0x800A04C8, distance + (uint32)((sint32)scaled >> 8));
    t3_80064080(1);
    w_u16(0x800A8C74, 0);
    return 0x800B0000;
}
