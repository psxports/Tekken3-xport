#include "game_functions.h"

uint32 t3_80054380(uint32 state)
{
    uint32 ordering;
    uint32 packet;
    uint32 index;
    uint32 side;
    uint32 selection;
    uint32 row;
    uint32 column;
    uint32 table;
    uint32 style;
    uint32 x;
    uint32 y;
    uint32 flags;
    uint32 color;
    uint32 ordering_group;
    uint32 pulse;
    uint32 code;
    uint32 mask;
    uint32 offset;
    uint32 uv;
    uint32 xy;
    uint32 entry;
    PSX_RECT rectangle;

    FUNCTION_MARKER(0x80054380, "SLUS_004.02");
    ordering = r_u32(0x800A9218) + 12;
    packet = r_u32(0x800ADF64);
    for (index = 0; index < 4; ++index)
        packet = t3_8004db28(ordering + index * 4, packet,
            (const PSX_RECT *)psx_addr(0x800AE230, 8));
    pulse = t3_8004dfe0(r_u32(0x800AE218) << 4);
    for (side = 0; side < 2; ++side) {
        selection = state + 24 + side * 172;
        row = r_u32(selection + 12);
        column = r_u32(selection + 8);
        table = 0x80022768 + (column + row * 7) * 6;
        style = r_u32(selection + 24);
        x = r_u16(table);
        y = r_u16(table + 2);
        /* The caller supplies styles 0 through 4 from the phase table */
        switch (style) {
        case 0:
            continue;
        case 1:
            flags = r_u32(selection + 120) | 0x10;
            goto pulse_cursor;
        case 3:
            flags = 0x93;
pulse_cursor:
            color = t3_8004df6c(0xFFFFFF, pulse);
            row = r_u32(selection + 12);
            column = r_u32(selection + 8);
            table = 0x80022768 + (column + row * 7) * 6;
            code = r_u8(table + 4);
            ordering_group = 2;
            mask = r_u32(selection + 36);
            mask &= (uint32)1 << ((code >> 2) & 31);
            if (!mask) {
                mask = r_u32(0x80097EF0);
                code = mask & ((uint32)1 << ((code >> 2) & 31)) ? 89 : 88;
            }
            if ((sint32)code >= 88)
                flags = 0x90;
            break;
        case 2:
            flags = r_u32(selection + 120);
            color = 0x808080;
            ordering_group = 0;
            break;
        case 4:
            flags = 0x83;
            color = 0x808080;
            ordering_group = 0;
            break;
        }
        style = r_u32(selection + 24);
        if (style - 3 < 2) {
            offset = r_u16(selection + 138);
            uv = r_u32(selection + 128);
        } else {
            offset = r_u16(selection + 136);
            uv = r_u32(selection + 124);
        }
        xy = ((x + offset) & 0xFFFF) | ((y - 4) << 16);
        packet = t3_8004d9bc(ordering + 16, packet, color | 0x64000000, xy, 0x00100014, uv);
        packet = t3_8004daa8(ordering + 16, packet, 14);
        rectangle.x = (sint16)x;
        rectangle.y = (sint16)y;
        rectangle.w = 18;
        rectangle.h = 64;
        for (index = 0; index < 2; ++index) {
            entry = ordering + ordering_group * 4;
            if (index == side)
                entry += 4;
            packet = t3_8004b7d0(entry, packet, x, y, flags);
            packet = t3_8004db28(entry, packet, &rectangle);
            rectangle.x = (sint16)((uint16)rectangle.x + 18);
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_80054694(uint32 state)
{
    uint32 side;
    uint32 selection;
    uint32 x;
    uint32 y;
    uint32 base_y;
    uint32 style;
    uint32 index;
    uint32 selected;
    uint32 count;
    uint32 tail;
    uint32 color;
    uint32 packet;
    uint32 ordering;
    uint32 frame;
    uint32 text;
    uint32 code;
    uint32 row;
    uint32 column;
    uint32 mask;
    uint32 has_character;
    uint32 width;
    uint32 right;
    uint32 mode;

    FUNCTION_MARKER(0x80054694, "SLUS_004.02");
    for (side = 0; side < 2; ++side) {
        selection = state + 24 + side * 172;
        x = r_u16(selection + 140);
        style = r_u32(selection + 28);
        y = r_u16(selection + 142);
        switch (style) {
        case 1:
            x = r_u32(selection + 16) + 8;
            base_y = r_u32(selection + 20);
            selected = r_u32(selection + 40) - 1;
            y = base_y + 24;
            tail = t3_8004cc08(0x1F800000, 0x80097D08);
            w_u8(tail + 1, 0);
            for (index = 0; index < 8; ++index) {
                color = 10;
                if (selected == index) {
                    packet = r_u32(0x800ADF64);
                    ordering = r_u32(0x800A9218) + 28;
                    packet = t3_8004da10(ordering, packet, 0x600000FF,
                        ((x - 2) & 0xFFFF) | ((base_y + 52) << 16), 0x00040011);
                    frame = r_u32(0x800AE218);
                    w_u32(0x800ADF64, packet);
                    color = frame & 2 ? 1 : 6;
                }
                w_u8(tail, index + '1');
                t3_8004ce74(0x1F800000, color, 1, x, y);
                x += 16;
            }
            x = r_u32(selection + 16);
            base_y = r_u32(selection + 20);
            count = r_u32(selection + 40);
            text = count < 2 ? 0x80022944 : 0x8002294C;
            count = r_u32(selection + 40);
            t3_8004ce74(0x80022954, 5, 1, x + 12, base_y + 72, count, text);
            break;
        case 2:
            row = r_u32(selection + 12);
            column = r_u32(selection + 8);
            code = r_u8(0x8002276C + (column + row * 7) * 6);
            mask = (uint32)1 << ((code >> 2) & 31);
            if (!(r_u32(selection + 36) & mask))
                code = r_u32(0x80097EF0) & mask ? 89 : 88;
            goto draw_character_name;
        case 3:
            selected = r_u32(selection + 44);
            count = r_u32(selection + 40);
            if (selected < count) {
                if ((sint32)selected > 0)
                    --selected;
            } else
                selected = count - 1;
            code = r_u32(selection + 56 + selected * 4);
draw_character_name:
            tail = t3_8004cc08(0x1F800000, 0x80097D08);
            if ((sint32)code < 88) {
                has_character = 1;
                text = t3_8004efc8((sint32)code);
                text = r_u32(text);
                color = 6;
            } else if (code == 88) {
                has_character = 0;
                text = 0x8002292C;
                color = 9;
            } else {
                has_character = 0;
                text = 0x80022938;
                color = 5;
            }
            t3_8004cc08(tail, text);
            width = t3_8007a418(tail) * 13;
            x -= (uint32)((sint32)width >> 1);
            t3_8004ce74(0x1F800000, color, 1, x, y);
            if (has_character) {
                ordering = r_u32(0x800A9218);
                packet = r_u32(0x800ADF64);
                packet = t3_8004e704(ordering, packet, x - 4, y + 18, width + 10, code & 3);
                w_u32(0x800ADF64, packet);
            }
            break;
        case 4:
            t3_8004eee8(side, x, y + 4, 10);
            break;
        case 5:
            mode = r_u32(state);
            /* The selection constructor supplies game modes 0 through 8 */
            switch (mode) {
            case 0:
            case 1:
            case 2:
            case 6:
            case 7:
                text = 0x800229A4;
                break;
            case 3:
                text = 0x8002296C;
                break;
            case 4:
                text = 0x8002297C;
                break;
            case 5:
                text = 0x80022988;
                break;
            case 8:
                text = 0x80022994;
                break;
            }
            width = t3_8007a418(text) * 13;
            x -= (uint32)((sint32)width >> 1);
            right = x + width;
            if ((sint32)x < 8) {
                x = 8;
                right = x + width;
            }
            if ((sint32)right >= 361)
                x = 360 - width;
            t3_8004ce74(0x800229B4, 5, 1, x, y, text);
            break;
        default:
            break;
        }
    }
    return 0;
}

uint32 t3_80055080(uint32 state)
{
    uint32 ordering;
    uint32 packet;
    uint32 mode;
    uint32 color;
    uint32 colors[4];
    uint32 table;
    uint32 index;
    uint32 corner;
    uint32 xy0;
    uint32 xy1;
    uint32 xy2;
    uint32 xy3;
    uint32 value;
    uint32 scroll;
    uint32 start;
    uint32 x;
    uint32 texture;
    uint32 u;
    uint32 v;
    uint32 right_u;
    uint32 top_v;
    uint32 bottom_v;
    uint32 right_x;

    FUNCTION_MARKER(0x80055080, "SLUS_004.02");
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    mode = r_u32(state);
    color = r_u32(0x80022B50 + mode * 8);
    for (index = 0; index < 2; ++index) {
        table = 0x80022B0C + index * 32;
        for (corner = 0; corner < 4; ++corner)
            colors[corner] = r_u32(table + 16 + corner * 4);
        for (corner = 0; corner < 4; ++corner) {
            if (colors[corner] == 0xFFFFFFFF)
                colors[corner] = color;
        }
        colors[0] |= 0x3A000000;
        xy0 = r_u32(table);
        xy1 = r_u32(table + 4);
        xy2 = r_u32(table + 8);
        xy3 = r_u32(table + 12);
        packet = t3_8004d8b0(ordering, packet, colors[0], colors[1], colors[2], colors[3],
            xy0, xy1, xy2, xy3);
    }
    packet = t3_8004daa8(ordering, packet, 32);
    value = r_u32(state + 16) + 383;
    scroll = (uint32)((sint32)value % 384);
    start = scroll - 384;
    w_u32(state + 16, scroll);
    for (index = 0; index < 7; ++index) {
        x = start;
        table = 0x800227E8 + index * 4;
        texture = r_u32(table);
        v = r_u8(table + 1);
        if ((sint32)start < 368) {
            u = texture & 0xFF;
            top_v = v << 8;
            right_u = (u + 24) & 0xFF;
            do {
                if ((sint32)x >= -47) {
                    right_x = (x + 48) & 0xFFFF;
                    bottom_v = ((v + 40) & 0xFF) << 8;
                    packet = t3_8004d830(ordering, packet, 0x2D000000,
                        (x & 0xFFFF) | 0x005F0000, right_x | 0x005F0000,
                        (x & 0xFFFF) | 0x00AF0000, right_x | 0x00AF0000,
                        u | top_v | (texture & 0xFFFF0000), right_u | top_v | 0x006E0000,
                        u | bottom_v | ((right_u | bottom_v) << 16));
                }
                x += 384;
            } while ((sint32)x < 368);
        }
        start += 48;
    }
    value = r_u32(state + 20) + 834;
    w_u32(0x800ADF64, packet);
    scroll = value % 836;
    x = scroll - 836;
    w_u32(state + 20, scroll);
    while ((sint32)x < 368) {
        t3_8004ce74(0x80022B94, 0, 6, 2, x, 203, 0x800981EC);
        x += 836;
    }
    mode = r_u32(state);
    packet = r_u32(0x800ADF64);
    color = r_u32(0x80022B4C + mode * 8);
    switch (mode) {
    case 0:
    case 3:
    case 4:
    case 6:
        packet = t3_bns_00_800b2f04(ordering, packet);
        break;
    case 1:
        packet = t3_bns_00_800b2fe0(ordering, packet);
        break;
    case 2:
        packet = t3_bns_00_800b31b4(ordering, packet, state);
        break;
    case 5:
        packet = t3_bns_01_800b6a24(ordering, packet);
        break;
    case 7:
        packet = t3_bns_03_800b4f1c(ordering, packet);
        break;
    case 8:
        packet = t3_bns_02_800b5f54(ordering, packet);
        break;
    default:
        break;
    }
    for (index = 0; index < 4; ++index) {
        table = 0x80022A8C + index * 32;
        for (corner = 0; corner < 4; ++corner)
            colors[corner] = r_u32(table + 16 + corner * 4);
        for (corner = 0; corner < 4; ++corner) {
            if (colors[corner] == 0xFFFFFFFF)
                colors[corner] = color;
        }
        colors[0] |= 0x38000000;
        xy0 = r_u32(table);
        xy1 = r_u32(table + 4);
        xy2 = r_u32(table + 8);
        xy3 = r_u32(table + 12);
        packet = t3_8004d8b0(ordering, packet, colors[0], colors[1], colors[2], colors[3],
            xy0, xy1, xy2, xy3);
    }
    packet = t3_8004da10(ordering, packet, 0x60000000, 0, 0x01E00170);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_80055590(void)
{
    uint32 state = 0x800B8D58;
    uint32 context = 0x800AFA88;
    uint32 mode;
    uint32 side;
    uint32 selection;
    uint32 value;
    uint32 first;
    uint32 second;
    uint32 bank;
    uint32 saved_packet;
    uint32 packet;
    uint32 ordering;
    uint32 fade;
    sint32 phase;

    FUNCTION_MARKER(0x80055590, "SLUS_004.02");
    phase = r_s16(0x800AE224);
    if (phase && !r_u32(state + 8) && t3_8005101c())
        return 1;
    phase = r_s16(0x800AE224);
    switch (phase) {
    case 0:
        value = t3_8002960c(0);
        w_u16(0x800AE224, 1);
        return value;
    case 1:
        mode = r_u32(context);
        w_u32(state + 8, 1);
        w_u32(state + 12, 0);
        w_u32(state + 4, 0);
        w_u32(state + 16, 0);
        w_u32(state + 20, 0);
        w_u32(state, mode);
        for (side = 0; side < 2; ++side) {
            selection = state + 24 + side * 172;
            w_u32(selection, 0);
            w_u32(selection + 4, side);
            value = r_u8(0x80097F0E + side);
            w_u32(selection + 100, value);
            t3_80053000(context, state, side);
        }
        if (r_u32(context) == 7)
            t3_bns_03_800b56a8();
        t3_8006ae14(1, 0);
        t3_8004b688(0);
        w_u16(0x800AE224, 2);
        break;
    case 2:
        first = t3_80053000(context, state, 0);
        second = t3_80053000(context, state, 1);
        if (first & second)
            w_u16(0x800AE224, 3);
        break;
    case 3:
        t3_80053fac(context, state);
        first = r_u8(context + 20);
        second = r_u8(context + 21);
        w_u16(0x800AE204, first);
        w_u16(0x800AE224, second);
        break;
    default:
        break;
    }
    bank = r_u32(0x800ADEFC);
    saved_packet = r_u32(0x800ADF64);
    packet = state + 368 + bank * 15360;
    value = r_u32(state + 8);
    w_u32(0x800ADF64, packet);
    if (value == 1) {
        ordering = r_u32(0x800A9218);
        fade = r_u32(state + 12);
        packet = t3_8004e000(ordering + 28, packet, (sint32)fade);
        fade += 16;
        w_u32(0x800ADF64, packet);
        if ((sint32)fade >= 257)
            w_u32(state + 8, 0);
        w_u32(state + 12, fade);
    }
    mode = r_u32(state);
    switch (mode) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 8:
        t3_8004ce74(0x8002290C, 7, 2, 161, 348);
        break;
    case 1:
        t3_bns_00_800b2d3c(state);
        t3_bns_00_800b287c(context);
        break;
    case 7:
        t3_bns_03_800b5478();
        break;
    default:
        break;
    }
    t3_80054194(state);
    t3_80054380(state);
    t3_80054694(state);
    t3_80054b48(state);
    t3_80054d00(state);
    t3_80055080(state);
    w_u32(0x800ADF64, saved_packet);
    return 0x800B0000;
}

uint32 t3_bns_00_800b2f04(uint32 ordering, uint32 packet)
{
    FUNCTION_MARKER(0x800B2F04, "SLUS_004.02-BNS-logical-00");
    packet = t3_8004d8b0(ordering, packet,
        0x3A000000, 0x00282828, 0, 0x00808080,
        0x01400050, 0x01400094, 0x01B00050, 0x01B00094);
    packet = t3_8004d8b0(ordering, packet,
        0x3A282828, 0, 0x00808080, 0,
        0x014000DC, 0x01400120, 0x01B000DC, 0x01B00120);
    return t3_8004daa8(ordering, packet, 64);
}

uint32 t3_bns_00_800b2fe0(uint32 ordering, uint32 packet)
{
    FUNCTION_MARKER(0x800B2FE0, "SLUS_004.02-BNS-logical-00");
    packet = t3_8004da10(ordering, packet, 0x62404040, 0x013B0058, 0x005000C0);
    packet = t3_8004d8b0(ordering, packet,
        0x3A000000, 0x00404040, 0, 0x00404040,
        0x013B0030, 0x013B0058, 0x018B0030, 0x018B0058);
    packet = t3_8004d8b0(ordering, packet,
        0x3A404040, 0, 0x00404040, 0,
        0x013B0118, 0x013B0140, 0x018B0118, 0x018B0140);
    packet = t3_8004da10(ordering, packet, 0x62404040, 0x018B00A8, 0x00160020);
    packet = t3_8004d8b0(ordering, packet,
        0x3A404040, 0x00404040, 0x00404040, 0x00404040,
        0x018B009A, 0x018B00A8, 0x01A100A6, 0x01A100A8);
    packet = t3_8004d8b0(ordering, packet,
        0x3A404040, 0x00404040, 0x00404040, 0x00404040,
        0x018B00C8, 0x018B00D6, 0x01A100C8, 0x01A100CA);
    return t3_8004daa8(ordering, packet, 64);
}

uint32 t3_bns_00_800b31b4(uint32 ordering, uint32 packet, uint32 state)
{
    uint32 side;
    uint32 x;
    uint32 y;

    FUNCTION_MARKER(0x800B31B4, "SLUS_004.02-BNS-logical-00");
    for (side = 0; side < 2; ++side) {
        y = r_u32(state + 44 + side * 172);
        x = r_u16(state + 40 + side * 172);
        packet = t3_8004da10(ordering, packet, 0x62000000,
            x | (y << 16), 0x00800090);
    }
    return t3_8004daa8(ordering, packet, 0);
}

uint32 t3_bns_01_800b6a24(uint32 ordering, uint32 packet)
{
    FUNCTION_MARKER(0x800B6A24, "SLUS_004.02-BNS-logical-01");
    packet = t3_8004d8b0(ordering, packet,
        0x3A000000, 0x00282828, 0, 0x00808080,
        0x01400050, 0x01400094, 0x01B00050, 0x01B00094);
    packet = t3_8004d8b0(ordering, packet,
        0x3A282828, 0, 0x00808080, 0,
        0x014000DC, 0x01400120, 0x01B000DC, 0x01B00120);
    return t3_8004daa8(ordering, packet, 64);
}

uint32 t3_bns_03_800b4f1c(uint32 ordering, uint32 packet)
{
    FUNCTION_MARKER(0x800B4F1C, "SLUS_004.02-BNS-logical-03");
    packet = t3_8004da10(ordering, packet, 0x62404040, 0x013B0058, 0x005000C0);
    packet = t3_8004d8b0(ordering, packet,
        0x3A000000, 0x00404040, 0, 0x00404040,
        0x013B0030, 0x013B0058, 0x018B0030, 0x018B0058);
    packet = t3_8004d8b0(ordering, packet,
        0x3A404040, 0, 0x00404040, 0,
        0x013B0118, 0x013B0140, 0x018B0118, 0x018B0140);
    packet = t3_8004da10(ordering, packet, 0x62404040, 0x018B00A8, 0x00160020);
    packet = t3_8004d8b0(ordering, packet,
        0x3A404040, 0x00404040, 0x00404040, 0x00404040,
        0x018B009A, 0x018B00A8, 0x01A100A6, 0x01A100A8);
    packet = t3_8004d8b0(ordering, packet,
        0x3A404040, 0x00404040, 0x00404040, 0x00404040,
        0x018B00C8, 0x018B00D6, 0x01A100C8, 0x01A100CA);
    return t3_8004daa8(ordering, packet, 64);
}

uint32 t3_bns_02_800b5f54(uint32 ordering, uint32 packet)
{
    FUNCTION_MARKER(0x800B5F54, "SLUS_004.02-BNS-logical-02");
    packet = t3_8004d8b0(ordering, packet,
        0x3A000000, 0x00282828, 0, 0x00808080,
        0x01400050, 0x01400094, 0x01B00050, 0x01B00094);
    packet = t3_8004d8b0(ordering, packet,
        0x3A282828, 0, 0x00808080, 0,
        0x014000DC, 0x01400120, 0x01B000DC, 0x01B00120);
    return t3_8004daa8(ordering, packet, 64);
}

uint32 t3_bns_00_800b287c(uint32 context)
{
    uint32 value;
    uint32 palette;

    FUNCTION_MARKER(0x800B287C, "SLUS_004.02-BNS-logical-00");
    value = r_u16(context + 56);
    palette = r_u8(0x800B49E0 + ((value / 100) & 3));
    t3_8004ce74(0x800B0630, palette, (uint32)2,
        (uint32)120, (uint32)334, value % 100);
    value = r_u16(context + 60);
    palette = r_u8(0x800B49E0 + ((value / 100) & 3));
    t3_8004ce74(0x800B0630, palette, (uint32)2,
        (uint32)204, (uint32)334, value % 100);
    value = r_u16(context + 64);
    palette = r_u8(0x800B49E4 + ((value / 100) & 3));
    t3_8004ce74(0x800B0630, palette, (uint32)1,
        (uint32)171, (uint32)385, value % 100);
    t3_8004ce74(0x800B0640, (uint32)7, (uint32)2,
        (uint32)173, (uint32)334);
    return t3_8004ce74(0x800B064C, (uint32)7, (uint32)0,
        (uint32)166, (uint32)365);
}

uint32 t3_bns_00_800b2d3c(uint32 state)
{
    uint32 ordering;
    uint32 packet;
    uint32 side;
    uint32 record;
    uint32 position;
    uint32 target;
    uint32 y;
    uint32 type;
    uint32 x;
    uint32 selected;
    uint32 suffix;
    uint32 text;

    FUNCTION_MARKER(0x800B2D3C, "SLUS_004.02-BNS-logical-00");
    ordering = r_u32(0x800A9218) + 28;
    packet = r_u32(0x800ADF64);
    for (side = 0; side < 2; ++side) {
        record = state + 112 + side * 172;
        position = r_u32(record + 8);
        target = r_u32(record + 4);
        if (position != target) {
            if ((sint32)target < (sint32)position) {
                if ((sint32)(position - target) < 24)
                    position = target;
                else
                    position -= 24;
            } else {
                if ((sint32)(target - position) < 24)
                    position = target;
                else
                    position += 24;
            }
        }
        y = r_u16(record + 70);
        type = r_u32(record);
        x = r_u16(record + 64);
        w_u32(record + 8, position);
        selected = r_u32(state + 24 + side * 172) == 4;
        packet = t3_bns_00_800b2a18(ordering, packet,
            position, y, x, type, selected);
    }
    w_u32(0x800ADF64, packet);
    suffix = t3_8004cc08(0x1F800000, 0x80097D08);
    for (side = 0; side < 2; ++side) {
        record = state + 112 + side * 172;
        x = (uint32)r_s16(record + 72);
        position = r_u32(record + 8);
        y = (uint32)r_s16(record + 74);
        x += position;
        t3_8004cc08(suffix, 0x800B06CC);
        t3_8004ce74(0x1F800000, (uint32)5, (uint32)0, x, y);
        y = (uint32)r_s16(record + 78);
        type = r_u32(record);
        x = (uint32)r_s16(record + 76);
        position = r_u32(record + 8);
        text = r_u32(0x800B49E8 + type * 4);
        x += position;
        t3_8004cc08(suffix, text);
        t3_8004ce74(0x1F800000, (uint32)6, (uint32)0, x, y);
    }
    return 0;
}

uint32 t3_bns_03_800b56a8(void)
{
    uint32 owner;
    uint32 pair;
    uint32 first;
    uint32 second;
    uint32 result;

    FUNCTION_MARKER(0x800B56A8, "SLUS_004.02-BNS-logical-03");
    owner = t3_bns_03_800b07a8();
    w_u8(0x800AFACB, 0);
    t3_bns_03_800b0910();
    w_u32(owner + 140, 544);
    w_u16(owner + 176, 1);
    w_u8(owner + 326, 10);
    w_u32(owner + 104, 0);
    w_u32(owner + 108, 0);
    w_u32(owner + 112, 0);
    w_u32(owner + 120, 0);
    for (pair = 0; pair < 4; ++pair) {
        first = r_u32(0x800ADF98 + pair * 8);
        second = r_u32(0x800ADF9C + pair * 8);
        w_u32(0x800ADF70 + pair * 8, first);
        w_u32(0x800ADF74 + pair * 8, second);
    }
    w_u32(0x800ADF84, (uint32)-64);
    w_u32(0x800ADF88, 1088);
    w_u32(0x800ADF8C, 4992);
    result = t3_80039960(5);
    SetBackColor(64, 64, 64);
    return result;
}

uint32 t3_bns_03_800b5478(void)
{
    uint32 owner;
    uint32 first;
    uint32 second;
    uint32 distance;
    uint32 suffix;
    uint32 text;
    uint32 length;
    uint32 x;
    uint32 damage;

    FUNCTION_MARKER(0x800B5478, "SLUS_004.02-BNS-logical-03");
    owner = t3_bns_03_800b07a8();
    w_u16(owner + 154, r_u16(owner + 154) + 8);
    t3_bns_03_800b2cd0(owner);
    if (r_u8(0x800AFACB) != 3)
        return 3;
    first = r_u32(owner + 104);
    second = r_u32(owner + 120);
    distance = first - second;
    if ((sint32)distance < 0)
        distance = second - first;
    if ((sint32)distance >= 64)
        return 0;
    suffix = t3_8004cc08(0x1F800000, 0x80097D08);
    text = r_u32(0x800B6464 + r_u8(0x800AFAC9) * 8);
    t3_8004cc08(suffix, text);
    text = r_u32(0x800B6464 + r_u8(0x800AFAC9) * 8);
    length = t3_8007a418(text);
    x = 184 - ((length * 9) >> 1);
    t3_8004ce74(0x1F800000, (uint32)11, (uint32)0, x, (uint32)324);
    suffix = t3_8004cc08(0x1F800000, 0x80097D08);
    text = r_u32(0x800B6468 + r_u8(0x800AFAC9) * 8);
    t3_8004cc08(suffix, text);
    text = r_u32(0x800B6468 + r_u8(0x800AFAC9) * 8);
    length = t3_8007a418(text);
    x = 184 - ((length * 13) >> 1);
    t3_8004ce74(0x1F800000, (uint32)4, (uint32)1, x, (uint32)342);
    suffix = t3_8004cc08(0x1F800000, 0x80097D08);
    t3_8004cc08(suffix, 0x800B0644);
    t3_8004ce74(0x1F800000, (uint32)5, (uint32)0,
        (uint32)135, (uint32)414);
    suffix = t3_8004cc08(0x1F800000, 0x80097D08);
    t3_8004cc08(suffix, 0x800B0650);
    damage = r_u8(0x800AFAC9) * 20 + 60;
    return t3_8004ce74(0x1F800000, (uint32)6, (uint32)1,
        (uint32)160, (uint32)432, damage, (uint32)0,
        (uint32)201, (uint32)438);
}

uint32 t3_bns_03_800b0910(void)
{
    FUNCTION_MARKER(0x800B0910, "SLUS_004.02-BNS-logical-03");
    t3_8003c250(0, 0x800B65A0);
    t3_8003c200(0x800B65F0);
    return t3_bns_03_800b0b14();
}

uint32 t3_bns_00_800b2a18(uint32 ordering, uint32 packet, uint32 position, uint32 y, uint32 side, uint32 type, uint32 selected)
{
    uint32 pulse;
    uint32 color;
    uint32 x;
    uint32 cursor;
    uint32 index;
    uint32 table;
    uint32 width;
    uint32 left;
    uint32 right;
    uint32 uv;
    uint32 top;
    uint32 bottom;

    FUNCTION_MARKER(0x800B2A18, "SLUS_004.02-BNS-logical-00");
    pulse = t3_8004dfe0(r_u32(0x800AE218) << 4);
    if (selected == 1)
        color = t3_8004df6c(0x00FFFFFF, pulse);
    else
        color = 0x00808080;
    x = side ? position + 87 : position - 95;
    packet = t3_8004d9bc(ordering, packet, 0x65000000,
        (x & 0xFFFF) | (y << 16), 0x000A0008, 0x7E55B6D6);
    cursor = side == 1 ? position + 2 : position - 1;
    top = (y + 4) << 16;
    bottom = (y + 20) << 16;
    for (index = 0; index < 8; ++index) {
        table = 0x800B065C + index * 6;
        if (index == 0 && r_u32(0x800AFA88) == 7)
            width = 62;
        else
            width = r_u8(table);
        left = cursor - width;
        if (side == 1) {
            left = cursor;
            cursor += width;
        } else {
            cursor = left;
        }
        if ((sint32)type < (sint32)index)
            uv = r_u16(table + 4);
        else
            uv = r_u16(table + 2);
        right = (left + width - 1) & 0xFFFF;
        left &= 0xFFFF;
        packet = t3_8004d830(ordering, packet, 0x2D000000,
            left | top, right | top, left | bottom, right | bottom,
            (uv << 16) | 0xACE8, 0x000DACE9, 0xBCE9BCE8);
    }
    if (side == 0)
        position -= 128;
    packet = t3_8004da10(ordering, packet, 0x60000000,
        ((position + 2) & 0xFFFF) | ((y + 4) << 16), 0x0010007C);
    top = y << 16;
    packet = t3_8004d9bc(ordering, packet, color | 0x64000000,
        (position & 0xFFFF) | top, 0x00180008, 0x7EDCA8E0);
    packet = t3_8004d9bc(ordering, packet, color | 0x64000000,
        ((position + 120) & 0xFFFF) | top, 0x00180008, 0x7EDCA8E8);
    left = (position + 8) & 0xFFFF;
    right = (position + 120) & 0xFFFF;
    bottom = (y + 24) << 16;
    return t3_8004d830(ordering, packet, color | 0x2C000000,
        left | top, right | top, left | bottom, right | bottom,
        0x7EDCA8E6, 0x000DA8EA, 0xC0EAC0E6);
}

uint32 t3_bns_03_800b2cd0(uint32 owner)
{
    MATRIX stretch = {0};
    MATRIX orientation = {0};
    MATRIX inverse = {0};
    MATRIX light;
    SVECTOR angles = {0};
    VECTOR scale;
    MATRIX *matrix;
    uint32 disabled;
    uint32 descriptor;
    uint32 exponent;
    uint32 deformation;
    uint32 ambient_red;
    uint32 ambient_green;
    uint32 ambient_blue;
    uint32 screen;
    uint32 depth;
    uint32 ordering;
    uint32 color_table = 0x800B6364;
    uint32 colored;
    uint32 phase;
    uint32 pass;
    uint32 count;
    uint32 vertex_count;
    uint32 packet_size;
    uint32 packet;
    uint32 table;
    uint32 bank;
    uint32 vertex;
    uint32 point[3];
    uint32 value;
    uint32 tag;
    uint32 link;
    uint32 index;
    uint32 source;
    uint32 first;
    uint32 second;
    uint32 saved_y;
    uint32 packet_base;
    uint32 ot_owner;
    sint32 face;

    FUNCTION_MARKER(0x800B2CD0, "SLUS_004.02-BNS-logical-03");
    disabled = r_u32(0x800ADCA4);
    if (disabled)
        return disabled;
    w_u32(owner + 92, r_u32(owner + 104));
    w_u32(owner + 96, r_u32(owner + 108));
    w_u32(owner + 100, r_u32(owner + 112));
    descriptor = 0x800B5F9C + r_u8(0x800AFAC9) * 24;
    exponent = r_u8(descriptor + 12);
    matrix = (MATRIX *)psx_addr(owner + 72, 32);
    t3_8003a490((const SVECTOR *)psx_addr(owner + 152, 8), matrix);
    t3_80039fbc(owner + 104);
    deformation = (uint32)r_s16(owner + 336);
    if (deformation) {
        stretch.m[0][0] = (sint16)(4096 - deformation);
        stretch.m[1][1] = (sint16)(deformation + 4096);
        stretch.m[2][2] = stretch.m[1][1];
        t3_8003a490((const SVECTOR *)psx_addr(owner + 328, 8), &orientation);
        TransposeMatrix(&orientation, &inverse);
        MulMatrix(&orientation, &stretch);
        MulMatrix(&orientation, &inverse);
        MulMatrix2(&orientation, matrix);
    }
    t3_80037814(owner + 72);
    ReadLightMatrix(&light);
    light.m[0][0] = (sint16)((uint32)(uint16)light.m[0][0] << 1);
    light.m[0][2] = (sint16)((uint32)(uint16)light.m[0][2] << 1);
    light.m[0][1] = (sint16)((uint32)(uint16)light.m[0][1] << 1);
    SetLightMatrix(&light);
    if (r_u8(owner + 326) == 4 && r_u32(0x80095494) == 0) {
        ambient_red = t3_8004dfe0(r_u32(0x800AE218) << 4);
        ambient_green = ambient_red;
        ambient_blue = ambient_red;
    } else {
        ambient_red = r_u8(descriptor + 13);
        ambient_green = r_u8(descriptor + 14);
        ambient_blue = r_u8(descriptor + 15);
    }
    SetBackColor((sint32)ambient_red, (sint32)ambient_green, (sint32)ambient_blue);
    t3_8003785c(0x800ADF70, owner + 72);
    w_u16(0x1F800100, 0);
    w_u16(0x1F800102, 0);
    w_u16(0x1F800104, 0);
    depth = t3_80036ad4((const SVECTOR *)psx_addr(0x1F800100, 8), &screen);
    if ((depth & 0xFFFF) >= 0x4000)
        return 10;
    phase = r_u8(owner + 326);
    if (phase == 10 || phase == 0)
        ordering = r_u32(0x800A9218);
    else
        ordering = r_u32(r_u32(0x800A8C54) + 4) + ((depth >> 1) & 0x1FFC);
    source = r_u32(owner + 180);
    t3_80036af8(source, (uint32 *)psx_addr(0x1F800000, 168),
        (uint16 *)psx_addr(0x1F800100, 84), 42);
    phase = r_u8(owner + 326);
    colored = 0;
    switch (phase) {
    case 0:
        for (vertex = 0; vertex < 3; ++vertex)
            w_u8(0x1F800200 + vertex, 0u - (r_u8(owner + 327) << 3));
        break;
    case 1:
        for (vertex = 0; vertex < 3; ++vertex)
            w_u8(0x1F800200 + vertex, (r_u8(owner + 327) << 3) - 1);
        break;
    case 2:
    case 5:
    case 8:
        w_u32(0x1F800200, 0xFFFFFFFF);
        break;
    default:
        color_table += r_u8(0x800AFAC9) * 64;
        colored = 1;
        break;
    }
    for (pass = 0; pass < 2; ++pass) {
        if (pass == 0) {
            count = 16;
            vertex_count = 3;
            packet_size = 28;
            table = 0x800B62C0;
            bank = r_u32(0x800ADEFC);
            w_u8(0x1F800203, 0x30);
            packet = owner + bank * 448 + 760;
        } else {
            count = 32;
            vertex_count = 4;
            packet_size = 36;
            table = 0x800B6340;
            if (colored)
                color_table += 32;
            bank = r_u32(0x800ADEFC);
            w_u8(0x1F800203, 0x38);
            packet = owner + bank * 1152 + 2352;
        }
        for (face = (sint32)count - 1; face >= 0; --face) {
            for (vertex = 0; vertex < vertex_count; ++vertex) {
                index = r_u8(table + vertex);
                value = r_u32(0x1F800000 + index * 4);
                w_u32(packet + 8 + vertex * 8, value);
                if (vertex < 3)
                    point[vertex] = value;
            }
            if (NormalClip((sint32)point[0], (sint32)point[1], (sint32)point[2]) >= 0) {
                phase = r_u8(owner + 326);
                if (phase < 3 || phase == 5 || phase == 8) {
                    for (vertex = 0; vertex < vertex_count; ++vertex)
                        w_u32(packet + 4 + vertex * 8, r_u32(0x1F800200));
                } else {
                    value = r_u32(color_table + ((uint32)face & 7) * 4);
                    w_u32(0x1F800200, value);
                    for (vertex = 0; vertex < vertex_count; ++vertex) {
                        index = r_u8(table + vertex);
                        ColorMatCol((SVECTOR *)psx_addr(0x800B6134 + index * 8, 8),
                            (CVECTOR *)psx_addr(0x1F800200, 4),
                            (CVECTOR *)psx_addr(packet + 4 + vertex * 8, 4), (sint32)exponent);
                    }
                }
                tag = r_u32(packet);
                link = r_u32(ordering);
                w_u32(packet, (tag & 0xFF000000) | (link & 0x00FFFFFF));
                link = r_u32(ordering);
                w_u32(ordering, (link & 0xFF000000) | (packet & 0x00FFFFFF));
            }
            packet -= packet_size;
            table -= 4;
        }
    }
    if (r_u8(owner + 326) == 2) {
        if (r_u32(0x80095494) == 0) {
            if (r_u8(owner + 327) == 5)
                t3_bns_03_800b42f0(owner + 104, owner + 120, r_s32(owner + 108) < -384);
            for (index = 0; index < 4; ++index) {
                bank = r_u32(0x800ADEFC);
                packet = owner + bank * 112 + 3756 + index * 28;
                w_u32(packet + 8, screen);
                value = (uint32)randPSX();
                value = (screen & 0xFFFF) + (uint32)((sint32)value >> 4) - 1024;
                w_u16(packet + 16, value);
                value += r_u8(owner + 327) << 3;
                w_u16(packet + 24, value);
                w_u16(packet + 26, index & 2 ? 0 : 480);
                value = r_u16(packet + 26);
                w_u16(packet + 18, value);
                ordering = r_u32(0x800A9218);
                tag = r_u32(packet);
                link = r_u32(ordering);
                w_u32(packet, (tag & 0xFF000000) | (link & 0x00FFFFFF));
                link = r_u32(ordering);
                w_u32(ordering, (link & 0xFF000000) | (packet & 0x00FFFFFF));
            }
        } else {
            for (index = 0; index < 4; ++index) {
                bank = r_u32(0x800ADEFC);
                packet = owner + bank * 112 + 3756 + index * 28;
                source = owner + (1 - bank) * 112 + index * 28;
                w_u32(packet + 8, screen);
                value = r_u32(source + 3772);
                w_u32(packet + 16, value);
                value = r_u32(source + 3780);
                w_u32(packet + 24, value);
                ordering = r_u32(0x800A9218);
                tag = r_u32(packet);
                link = r_u32(ordering);
                w_u32(packet, (tag & 0xFF000000) | (link & 0x00FFFFFF));
                link = r_u32(ordering);
                w_u32(ordering, (link & 0xFF000000) | (packet & 0x00FFFFFF));
            }
        }
        bank = r_u32(0x800ADEFC);
        packet = owner + bank * 12 + 3732;
        ordering = r_u32(0x800A9218);
        tag = r_u32(packet);
        link = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (link & 0x00FFFFFF));
        link = r_u32(ordering);
        w_u32(ordering, (link & 0xFF000000) | (packet & 0x00FFFFFF));
    }
    saved_y = r_u32(owner + 96);
    w_u32(owner + 96, 0);
    angles.vy = (sint16)r_u16(owner + 154);
    t3_8003a490(&angles, matrix);
    deformation = (uint32)r_s16(owner + 336);
    stretch.m[0][0] = (sint16)(4096 - deformation);
    stretch.m[1][1] = (sint16)(deformation + 4096);
    stretch.m[2][2] = stretch.m[1][1];
    angles.vy = (sint16)r_u16(owner + 330);
    t3_8003a490(&angles, &orientation);
    MulMatrix(&orientation, &stretch);
    MulMatrix2(&orientation, matrix);
    scale.vy = 4096;
    scale.vx = (sint32)((uint32)(r_s32(owner + 108) >> 1) + 4096);
    scale.vz = (sint32)((uint32)(r_s32(owner + 108) >> 1) + 4096);
    ScaleMatrix(matrix, &scale);
    t3_8003785c(0x800ADF70, owner + 72);
    for (index = 0; index < 4; ++index) {
        source = r_u32(owner + 180);
        bank = r_u32(0x800ADEFC);
        source += index * 8;
        first = r_u32(source + 144);
        second = r_u32(source + 148);
        w_u32(0x1F800100, first);
        w_u32(0x1F800104, second);
        packet_base = owner + bank * 96 + 3540;
        source = r_u32(owner + 180) + index * 8;
        first = r_u32(source + 152);
        second = r_u32(source + 156);
        w_u32(0x1F800108, first);
        w_u32(0x1F80010C, second);
        source = r_u32(owner + 180) + ((index + 5) & 7) * 8;
        first = r_u32(source + 144);
        second = r_u32(source + 148);
        w_u32(0x1F800110, first);
        w_u32(0x1F800114, second);
        source = r_u32(owner + 180) + ((index + 4) & 7) * 8;
        first = r_u32(source + 144);
        second = r_u32(source + 148);
        w_u32(0x1F800118, first);
        w_u32(0x1F80011C, second);
        w_u16(0x1F80011A, 0);
        w_u16(0x1F800112, 0);
        w_u16(0x1F80010A, 0);
        packet = packet_base + index * 24;
        w_u16(0x1F800102, 0);
        t3_80036af8(0x1F800100, (uint32 *)psx_addr(packet + 8, 16),
            (uint16 *)psx_addr(0x1F800100, 8), 4);
        ot_owner = r_u32(0x800A8C54);
        ordering = r_u32(ot_owner + 4) + 0xFBC;
        tag = r_u32(packet);
        link = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (link & 0x00FFFFFF));
        ordering = r_u32(ot_owner + 4) + 0xFBC;
        link = r_u32(ordering);
        w_u32(ordering, (link & 0xFF000000) | (packet & 0x00FFFFFF));
    }
    w_u32(owner + 96, saved_y);
    return 0;
}

uint32 t3_bns_03_800b42f0(uint32 position, uint32 previous, uint32 high)
{
    uint32 index;
    uint32 first;
    uint32 second;
    uint32 random;
    uint32 effect;
    uint32 value;
    uint32 z;

    FUNCTION_MARKER(0x800B42F0, "SLUS_004.02-BNS-logical-03");
    for (index = 0; index < 16; ++index) {
        first = (uint32)randPSX();
        second = (uint32)randPSX();
        random = (first << 16) + second;
        effect = t3_8006f4f4();
        if (effect == 0xFFFFFFFF)
            return 0xFFFFFFFF;
        w_u8(effect + 12, 1);
        value = r_u32(position);
        w_u32(effect + 40, value - 255 + (random >> 23));
        value = r_u32(position + 4);
        w_u32(effect + 44, value - 255 + ((random >> 14) & 0x1FF));
        z = r_u32(position + 8) - 255 + ((random >> 5) & 0x1FF);
        if (high == 0) {
            w_u16(effect + 16, 1024);
            w_u32(effect + 48, z);
            value = (uint32)randPSX();
            w_u16(effect + 18, (value - 2048) & 0x100);
        } else {
            w_u32(effect + 48, z);
            value = r_u16(previous);
            w_u16(effect + 16, value);
            value = r_u16(previous + 4);
            w_u16(effect + 18, value);
        }
    }
    return 0;
}

uint32 t3_main_800b0b18(void)
{
    FUNCTION_MARKER(0x800B0B18, "SLUS_004.02");
    CloseEvent(r_u32(0x800ADF40));
    CloseEvent(r_u32(0x800ADF44));
    CloseEvent(r_u32(0x800ADF48));
    CloseEvent(r_u32(0x800ADF4C));
    CloseEvent(r_u32(0x800ADF50));
    CloseEvent(r_u32(0x800ADF54));
    CloseEvent(r_u32(0x800ADF58));
    return (uint32)CloseEvent(r_u32(0x800ADF5C));
}

void t3_8005138c(uint32 row, uint32 character, uint32 costume, uint32 side, uint32 elapsed)
{
    uint32 entry = 0x800AFAC8 + row * 8;
    FUNCTION_MARKER(0x8005138C, "SLUS_004.02");
    w_u16(entry + 6, side);
    w_u8(entry + 4, character);
    w_u8(entry + 5, costume);
    w_u32(entry, elapsed);
}

uint32 t3_800511b0(uint32 side)
{
    uint32 count;
    FUNCTION_MARKER(0x800511B0, "SLUS_004.02");
    if (side == r_u8(0x800ADF3E)) {
        count = r_u16(0x800ADD50) + 1;
    } else {
        w_u8(0x800ADF3E, side);
        w_u8(0x800B053E, (side + 1) & 1);
        count = 1;
    }
    w_u16(0x800ADD50, count);
    return count;
}

uint32 t3_80056684(uint32 character)
{
    uint32 flags;
    uint32 value;
    uint32 mode;
    FUNCTION_MARKER(0x80056684, "SLUS_004.02");
    if (character != 17)
        return 17;
    flags = r_u32(0x80097EF0);
    value = flags & 0x20000;
    if (value)
        return value;
    mode = r_u32(0x800AFA88);
    if (mode != 0 && mode != 7)
        return 7;
    value = flags | 0x20000;
    w_u8(0x80097F12, character);
    w_u16(0x80097F1C, 0);
    w_u32(0x80097EF0, value);
    return value;
}

void t3_bns_00_800b0e84(uint32 state)
{
    uint32 type;
    uint32 winner;
    uint32 other;
    uint32 total;
    uint32 elapsed;
    uint32 character;
    uint32 costume;
    uint32 row;
    FUNCTION_MARKER(0x800B0E84, "SLUS_004.02-BNS-logical-00");
    type = r_u8(state + 0x1C);
    if (type == 1) {
        winner = r_u8(state + 0x22);
        other = r_u8(state + 0x1E);
        if (winner == other) {
            total = r_u32(state + 0x34);
            elapsed = r_u32(state + 0x2C);
            winner = r_u8(state + 0x22);
            w_u32(state + 0x30, 0);
            w_u32(state + 0x34, total + elapsed);
            elapsed = r_u32(state + 0x2C);
            character = r_u16(0x800ADD5C + winner * 2);
            costume = r_u16(0x800ADD98 + winner * 2);
            row = r_u32(state + 0x24);
            t3_8005138c(row, character, costume, winner, elapsed);
            other = r_u8(state + 0x1F);
            character = r_u16(0x800ADD5C + other * 2);
            t3_80056684(character);
            row = r_u32(state + 0x24) + 1;
            w_u32(state + 0x24, row);
            if (row < 10) {
                w_u16(0x800AE224, 2);
                return;
            }
            winner = r_u8(state + 0x22);
            costume = r_u16(0x800ADD98 + winner * 2);
            character = r_u16(0x800ADD5C + winner * 2);
            w_u32(state + 0x38, winner);
            w_u32(state + 0x3C, character * 4 + costume);
            character = r_u16(0x800ADD5C + winner * 2);
            costume = r_u16(0x800ADD98 + winner * 2);
            t3_bns_00_800b1e88(character, costume);
            winner = r_u8(state + 0x22);
            character = r_u16(0x800ADD5C + winner * 2);
            t3_800513b4(character);
            winner = r_u8(state + 0x22);
            w_u16(0x800AE200, 0);
            w_u16(0x800AE1F8 + winner * 2, 0);
            t3_8004ff20(19);
        } else {
            elapsed = r_u32(state + 0x2C);
            w_u32(state + 0x30, elapsed);
            other = r_u8(state + 0x23);
            w_u16(0x800AE1F8 + other * 2, 0);
            other = r_u8(state + 0x23);
            w_u16(0x800ADFBC + other * 2, 0);
            other = r_u8(state + 0x23);
            character = r_u16(0x800ADD5C + other * 2);
            t3_800513b4(character);
            w_u16(0x800AE224, 9);
        }
    } else if (type == 2) {
        row = r_u32(state + 0x28);
        winner = r_u8(state + 0x22);
        w_u32(state + 0x28, row + 1);
        other = r_u8(state + 0x23);
        character = r_u16(0x800ADD5C + winner * 2);
        costume = r_u16(0x800ADD5C + other * 2);
        t3_80051458(character, costume);
        other = r_u8(state + 0x23);
        w_u16(0x800AE1F8 + other * 2, 0);
        other = r_u8(state + 0x23);
        w_u16(0x800ADFBC + other * 2, 0);
        winner = r_u8(state + 0x22);
        t3_800511b0(winner);
        w_u16(0x800AE224, type);
    }
}

uint32 t3_bns_02_800b101c(void)
{
    FUNCTION_MARKER(0x800B101C, "SLUS_004.02-BNS-logical-02");
    return 0xA00;
}

void t3_bns_00_800b1e88(uint32 character, uint32 costume)
{
    uint32 first;
    uint32 second;
    uint32 count;
    uint32 mask;
    uint32 index;
    uint32 level = 0;
    FUNCTION_MARKER(0x800B1E88, "SLUS_004.02-BNS-logical-00");
    if (r_u32(0x800AFA88) != 0)
        return;
    first = r_u32(0x80097EF8);
    second = r_u32(0x80097EFC);
    w_u8(0x80097F12, 22);
    if (character == 8) {
        if ((sint32)costume < 2)
            first |= 0x100;
        else
            second |= 0x100;
    } else if (character == 11) {
        if (costume == 1)
            second |= 0x800;
        else
            first |= 0x800;
    } else if (character == 16) {
        if (first & 0x10000)
            second |= 0x10000;
        else
            first |= 0x10000;
    } else {
        first |= 1u << (character & 31);
    }
    first &= 0x1FFFFF;
    second &= 0x1FFFFF;
    w_u32(0x80097EF8, first);
    w_u32(0x80097EFC, second);
    count = t3_8004ccac(first | second);
    if (count) {
        mask = r_u32(0x800B49DC);
        index = 0;
        do {
            if (mask & (1u << (index & 31)))
                ++level;
            ++index;
        } while (index < count);
    }
    t3_800561e0(level);
}

uint32 t3_bns_00_800b1104(uint32 state)
{
    uint32 rounds;
    uint32 type;
    uint32 side;
    uint32 other;
    uint32 entry;
    uint32 value;
    uint32 character;
    uint32 opponent;
    FUNCTION_MARKER(0x800B1104, "SLUS_004.02-BNS-logical-00");
    rounds = r_u32(state + 0x28);
    type = r_u8(state + 0x21);
    w_u32(state + 0x28, rounds + 1);
    if (type == 1 || type == 2) {
        side = r_u8(state + 0x22);
        t3_800511b0(side);
        side = r_u8(state + 0x22);
        entry = state + side * 4 + 0x38;
        value = r_u16(entry);
        w_u16(entry, value + 1);
        side = r_u8(state + 0x22);
        other = r_u8(state + 0x23);
        character = r_u16(0x800ADD5C + side * 2);
        opponent = r_u16(0x800ADD5C + other * 2);
        t3_80051458(character, opponent);
        side = r_u8(state + 0x22);
        character = r_u16(0x800ADD5C + side * 2);
        t3_800513b4(character);
    } else if (type == 3) {
        side = r_u8(0x800ADF3E);
        t3_8005118c(side);
        value = r_u16(state + 0x40);
        w_u16(state + 0x40, value + 1);
        character = r_u16(0x800ADD5C);
        opponent = r_u16(0x800ADD5E);
        t3_80051550(character, opponent);
    }
    w_u16(0x800ADFBE, 0);
    w_u16(0x800ADFBC, 0);
    w_u16(0x800AE224, 0);
    return 0x800B0000;
}

void t3_bns_00_800b1228(uint32 state)
{
    uint32 history;
    uint32 result;
    uint32 type;
    uint32 side;
    uint32 other;
    uint32 human;
    uint32 entry;
    uint32 first;
    uint32 second;
    uint32 total;
    uint32 character;
    uint32 opponent;
    uint32 index;
    uint32 flags;
    uint32 continuing;
    FUNCTION_MARKER(0x800B1228, "SLUS_004.02-BNS-logical-00");
    history = r_u8(state + 0x58);
    result = r_u8(state + 0x21);
    w_u16(state + 0x38 + history * 2, result);
    result = r_u8(state + 0x21);
    if (result == 1 || result == 2) {
        side = r_u8(state + 0x22);
        t3_800511b0(side);
        other = r_u8(state + 0x23);
        entry = state + other * 13 + 0x62;
        first = r_u8(entry);
        w_u8(entry, first + 1);
        type = r_u8(state + 0x1C);
        if (type == 1) {
            other = r_u8(state + 0x23);
            human = r_u8(state + 0x1E);
            if (other == human) {
                character = r_u16(0x800ADD5C + human * 2);
                t3_800513b4(character);
            }
        } else if (type == 2) {
            side = r_u8(state + 0x22);
            other = r_u8(state + 0x23);
            character = r_u16(0x800ADD5C + side * 2);
            opponent = r_u16(0x800ADD5C + other * 2);
            t3_80051458(character, opponent);
        }
    } else if (result == 3) {
        first = r_u8(state + 0x6F);
        second = r_u8(state + 0x62);
        w_u8(state + 0x6F, first + 1);
        type = r_u8(state + 0x1C);
        w_u8(state + 0x62, second + 1);
        if (type == 1) {
            human = r_u8(state + 0x1E);
            character = r_u16(0x800ADD5C + human * 2);
            t3_800513b4(character);
        } else if (type == 2) {
            character = r_u16(0x800ADD5C);
            opponent = r_u16(0x800ADD5E);
            t3_80051550(character, opponent);
        }
    }
    history = r_u8(state + 0x58);
    first = r_u8(state + 0x62);
    total = r_u8(state + 0x65);
    w_u8(state + 0x58, history + 1);
    continuing = first < total;
    if (continuing) {
        second = r_u8(state + 0x6F);
        total = r_u8(state + 0x72);
        continuing = second < total;
    }
    if (continuing) {
        w_u16(0x800AE224, 2);
        return;
    }
    for (index = 0; index < 2; ++index) {
        entry = state + index * 13;
        first = r_u8(entry + 0x62);
        total = r_u8(entry + 0x65);
        if (first < total) {
            flags = r_u8(state + 0x1D);
            if ((flags >> index) & 1) {
                result = r_u8(state + 0x21);
                if (result != 3) {
                    character = r_u16(0x800ADD5C + index * 2);
                    t3_800513b4(character);
                }
            }
        }
        w_u16(0x800AE1F8 + index * 2, 0);
        w_u16(0x800ADFBC + index * 2, 0);
    }
    t3_8004ff20(12);
}

void t3_bns_00_800b14b4(uint32 state)
{
    uint32 winner;
    uint32 human;
    uint32 other;
    uint32 total;
    uint32 elapsed;
    uint32 row;
    uint32 character;
    uint32 costume;
    FUNCTION_MARKER(0x800B14B4, "SLUS_004.02-BNS-logical-00");
    winner = r_u8(state + 0x22);
    human = r_u8(state + 0x1E);
    if (winner == human) {
        total = r_u32(state + 0x34);
        elapsed = r_u32(state + 0x2C);
        winner = r_u8(state + 0x22);
        w_u32(state + 0x30, 0);
        w_u32(state + 0x34, total + elapsed);
        elapsed = r_u32(state + 0x2C);
        character = r_u16(0x800ADD5C + winner * 2);
        costume = r_u16(0x800ADD98 + winner * 2);
        row = r_u32(state + 0x24);
        t3_8005138c(row, character, costume, winner, elapsed);
        row = r_u32(state + 0x24) + 1;
        w_u32(state + 0x24, row);
        if (row < 10) {
            w_u16(0x800AE224, 2);
            return;
        }
        winner = r_u8(state + 0x22);
        character = r_u16(0x800ADD5C + winner * 2);
        costume = r_u16(0x800ADD98 + winner * 2);
        w_u32(state + 0x38, winner);
        w_u32(state + 0x3C, character * 4 + costume);
        t3_bns_00_800b46b8();
        winner = r_u8(state + 0x22);
        character = r_u16(0x800ADD5C + winner * 2);
        t3_800513b4(character);
        winner = r_u8(state + 0x22);
        w_u16(0x800AE1F8 + winner * 2, 0);
        t3_8004ff20(13);
    } else {
        elapsed = r_u32(state + 0x2C);
        w_u32(state + 0x30, elapsed);
        other = r_u8(state + 0x23);
        w_u16(0x800AE1F8 + other * 2, 0);
        other = r_u8(state + 0x23);
        w_u16(0x800ADFBC + other * 2, 0);
        other = r_u8(state + 0x23);
        character = r_u16(0x800ADD5C + other * 2);
        t3_800513b4(character);
        w_u16(0x800AE224, 9);
    }
}

uint32 t3_bns_00_800b46b8(void)
{
    uint32 side;
    uint32 character;
    uint32 record;
    uint32 index;
    uint32 total;
    uint32 destination;
    uint32 flags;
    FUNCTION_MARKER(0x800B46B8, "SLUS_004.02-BNS-logical-00");
    total = 0;
    side = r_u16(0x800AFB16);
    character = r_u8(0x800AFB14);
    w_u8(0x800980FC, 0);
    for (index = 0; index < 10; ++index) {
        record = 0x800AFAC8 + index * 8;
        if (r_u16(record + 6) != side)
            return 0;
        if (r_u8(record + 4) != character)
            return 0;
        total += r_u32(record);
    }
    destination = 0x80097F4C + character * 8;
    if (total >= r_u32(destination))
        return 0;
    flags = r_u8(0x800980FE);
    w_u8(0x800980FE, flags | 1);
    w_u32(0x80097F44, destination);
    w_u8(0x80098100, character);
    w_u8(0x800980FF, side);
    w_u32(destination, total);
    t3_8004cc08(destination + 4, 0x800B0790);
    return 1;
}

void t3_bns_00_800b1648(uint32 state)
{
    uint32 loser;
    uint32 human;
    uint32 winner;
    uint32 character;
    uint32 costume;
    uint32 row;
    uint32 count;
    uint32 total;
    uint32 index;
    uint32 score;
    FUNCTION_MARKER(0x800B1648, "SLUS_004.02-BNS-logical-00");
    loser = r_u8(state + 0x23);
    human = r_u8(state + 0x1E);
    winner = r_u8(state + 0x22);
    character = r_u16(0x800ADD5C + loser * 2);
    if (winner == human) {
        t3_800511b0(human);
        row = r_u32(state + 0x24);
        w_u32(state + 0x24, row + 1);
        count = r_u16(state + 0x60 + character * 2);
        w_u16(state + 0x60 + character * 2, count + 1);
        total = 0;
        for (index = 0; index < 22; ++index)
            total += r_u16(state + 0x60 + index * 2);
        w_u32(state + 0x44, total);
        w_u16(0x800AE224, 2);
    } else {
        w_u32(state + 0x38, human);
        costume = r_u16(0x800ADD98 + human * 2);
        w_u32(state + 0x3C, (character << 2) | costume);
        score = r_u16(0x800ADD50);
        t3_bns_00_800b47cc(human, character, score);
        w_u16(0x800AE1F8 + human * 2, 0);
        w_u16(0x800ADFBC + human * 2, 0);
        t3_800513b4(character);
        t3_8004ff20(14);
    }
}

uint32 t3_bns_00_800b47cc(uint32 side, uint32 character, uint32 score)
{
    uint32 pairs[22];
    uint16 characters[11];
    uint16 scores[11];
    uint8 names[11][4];
    uint32 index;
    uint32 record;
    uint32 label;
    uint32 flags;
    FUNCTION_MARKER(0x800B47CC, "SLUS_004.02-BNS-logical-00");
    w_u8(0x800980FC, 1);
    for (index = 0; index < 10; ++index) {
        if ((sint32)r_u16(0x80097FFE + index * 8) < (sint32)score)
            break;
    }
    if (index == 10)
        return 0;
    w_u8(0x800980FF, side);
    w_u8(0x80098100, character);
    for (index = 0; index < 10; ++index) {
        record = 0x80097FFC + index * 8;
        characters[index] = (uint16)r_u16(record);
        scores[index] = (uint16)r_u16(record + 2);
        t3_copy_guest_string_to_native(names[index], record + 4);
        pairs[index * 2] = index;
        pairs[index * 2 + 1] = scores[index];
    }
    scores[10] = (uint16)score;
    characters[10] = (uint16)character;
    t3_copy_guest_string_to_native(names[10], 0x800B0790);
    pairs[20] = 10;
    pairs[21] = score;
    t3_8004cd80(pairs, 11, 0);
    for (index = 0; index < 10; ++index) {
        label = pairs[index * 2];
        record = 0x80097FFC + index * 8;
        if (label == 10)
            w_u32(0x80097F48, record);
        w_u16(record, characters[label]);
        w_u16(record + 2, scores[label]);
        t3_copy_native_string_to_guest(record + 4, names[label]);
    }
    flags = r_u8(0x800980FE);
    w_u8(0x800980FE, flags | 2);
    return 1;
}

uint32 t3_bns_00_800b1764(void)
{
    uint32 packet;
    uint32 ordering;
    uint32 side;
    uint32 result;
    uint32 team;
    uint32 x;
    uint32 step;
    uint32 u;
    uint32 next_u;
    uint32 split;
    uint32 active;
    uint32 index;
    uint32 completed;
    uint32 texture;
    uint32 right;
    FUNCTION_MARKER(0x800B1764, "SLUS_004.02-BNS-logical-00");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    for (side = 0; side < 2; ++side) {
        result = r_u8(0x800AFAA9);
        team = 0x800AFAED + side * 13;
        if (side == 0) {
            x = 156;
            step = (uint32)-9;
            u = 64;
            next_u = 74;
            active = result - 2 < 2;
        } else {
            x = 202;
            step = 9;
            u = 73;
            next_u = 63;
            active = result == 1 || result == 3;
        }
        if (r_u8(team) == 0)
            continue;
        split = u | ((next_u | 0xF800) << 16) | 0xF800;
        index = 0;
        do {
            completed = r_u8(team - 3);
            if (index < completed || (index == completed && active)) {
                packet = t3_8004d9bc(ordering, packet, 0x65000000,
                    (x & 0xFFFF) | 0x430000, 0x15000A, 0x7ED1E04A);
                completed = r_u8(team - 3);
            }
            texture = 0x7ED0;
            if (index < completed)
                texture = 0x7ED1;
            else if (index == completed && active) {
                if (!(r_u32(0x800AE218) & 0x10))
                    texture = 0x7ED1;
            }
            right = (x + 10) & 0xFFFF;
            packet = t3_8004d830(ordering, packet, 0x2D000000,
                (x & 0xFFFF) | 0x400000, right | 0x400000,
                (x & 0xFFFF) | 0x580000, right | 0x580000,
                u | (texture << 16) | 0xE000, next_u | 0x1FE000, split);
            ++index;
            x += step;
        } while (index < r_u8(team));
    }
    packet = t3_8004daa8(ordering, packet, 31);
    w_u32(0x800ADF64, packet);
    return packet;
}

void t3_bns_02_800b2658(void)
{
    uint32 primary;
    uint32 slot;
    uint32 actor;
    uint32 previous_x;
    uint32 previous_z;
    uint32 index;
    uint32 column;
    uint32 table;
    uint32 character;
    uint32 costume;
    uint32 selection;
    uint32 current;
    uint32 value;
    uint32 stage;
    uint32 primary_model;
    uint32 actor_model;
    uint32 primary_character;
    uint32 actor_character;
    uint32 identity;
    uint32 model;
    uint32 packets;
    FUNCTION_MARKER(0x800B2658, "SLUS_004.02-BNS-logical-02");
    primary = t3_bns_02_800b297c();
    slot = t3_bns_02_800b29ac(0);
    index = (uint32)r_s16(primary + 0x14);
    column = r_u32(0x800B6C20);
    actor = r_u32(slot);
    previous_x = r_u32(primary);
    previous_z = r_u32(primary + 8);
    table = r_u32(0x800B6378 + index * 4) + (column & 3) * 4;
    costume = (uint32)r_s16(table + 2);
    selection = r_u16(primary + 0x14);
    character = r_u16(table);
    if (costume == 0xFFFFFFFF) {
        current = (uint32)r_s16(primary + 0x18);
        selection &= 3;
        costume = current == character ? selection == 0 : selection != 0;
    }
    t3_8004f274(actor, character, costume);
    value = t3_8002d1dc((uint32)r_s16(actor + 0x16));
    w_u16(actor + 0x16, value);
    t3_bns_02_800b3148(1, character, costume);
    column = r_u32(0x800B6C20);
    w_u8(0x800AFAC9 + (column & 3), (character << 2) + costume);
    t3_8007641c(primary, actor);
    if (r_s16(actor + 0x18) == 15)
        t3_8004f2dc(actor);
    actor_character = (uint32)r_s16(actor + 0x16);
    stage = r_u16(0x800ADC84);
    primary_model = (uint32)r_s16(primary + 0x1C);
    actor_model = (uint32)r_s16(actor + 0x1C);
    primary_character = (uint32)r_s16(primary + 0x16);
    t3_8006c618(stage, primary_model, actor_model, primary_character, actor_character);
    t3_80069aa8(0);
    t3_80069f74();
    t3_8006a158(primary);
    t3_80035f2c(r_u16(0x800ADC84));
    t3_80035dec();
    t3_80074f44();
    identity = r_u8(primary + 0x1E);
    t3_80036310(actor, &model, &packets);
    t3_80035ce0();
    t3_80035ce8(actor, model, packets);
    model = t3_80035ed0(identity);
    packets = t3_80035eec(identity);
    t3_80035ce8(primary, model, packets);
    t3_8006c63c(1, 1);
    t3_8006c63c(2, 1);
    table = t3_8004eff0(character, costume);
    value = r_u8(table + 0x0B);
    w_u16(0x800ADD08, value);
    t3_8006ae14(r_u16(0x800ADD08), 0);
    w_u16(slot + 0x0E, 4);
    w_u16(slot + 8, 1);
    w_u16(slot + 0x0A, 0);
    t3_8002d6e8(actor, 3);
    t3_bns_02_800b2a6c(actor, r_u32(0x800A85D4) + 0x1000, 0);
    w_u8(actor + 0xC3, 1);
    column = r_u32(0x800B6C20);
    index = (uint32)r_s16(slot + 0x0E);
    value = r_u32(0x800B0684 + column * 20 + index * 4);
    w_u8(actor + 0xC2, 0);
    w_u32(actor + 4, 0);
    w_u16(actor + 0x92, 0);
    w_u8(actor + 0xB8, 0);
    w_u32(actor + 0x3F8, value);
    w_u32(actor + 0x3F4, value);
    t3_8003f044(actor);
    t3_bns_02_800b2a6c(primary, previous_x, previous_z);
}

uint32 t3_bns_02_800b29ac(uint32 side)
{
    FUNCTION_MARKER(0x800B29AC, "SLUS_004.02-BNS-logical-02");
    return 0x800B6598 + (side & 1) * 24;
}

void t3_bns_02_800b2a6c(uint32 actor, uint32 x, uint32 z)
{
    FUNCTION_MARKER(0x800B2A6C, "SLUS_004.02-BNS-logical-02");
    w_u32(actor + 0xF68, x);
    w_u32(actor, x);
    w_u32(actor + 0xF70, z);
    w_u32(actor + 8, z);
}

uint32 t3_bns_02_800b3148(uint32 side, uint32 character, uint32 costume)
{
    uint32 primary;
    uint32 uv;
    uint32 table;
    uint32 palette;
    uint32 value;
    uint32 page;
    uint32 resource;
    FUNCTION_MARKER(0x800B3148, "SLUS_004.02-BNS-logical-02");
    side = (uint32)((sint32)side % 3);
    primary = t3_bns_02_800b297c();
    uv = r_u8(primary + 0x1E) ? 0 : 0x100;
    if (character == 21) {
        uv += costume << 4;
        table = 0x800B0A18;
    } else {
        table = 0x800B0A18 + costume * 4;
    }
    palette = r_u16(table);
    value = r_u16(table + 2);
    w_u16(0x800B6644 + side * 2, value);
    page = ((uv & 0x100) >> 4) | 7;
    uv = 0x40 | ((uv & 0xFF) << 8);
    resource = t3_8004eff0(character, costume);
    value = r_u8(resource + 5);
    w_u16(0x800B661C + side * 2, value);
    w_u32(0x800B662C + side * 4, uv | (palette << 16));
    w_u16(0x800B663C + side * 2, page);
    return 0x800B663C;
}

uint32 t3_bns_00_800b26d0(uint32 state)
{
    uint32 pairs[44];
    uint32 mask;
    uint32 previous;
    uint32 count;
    uint32 index;
    uint32 threshold;
    uint32 eligible;
    uint32 random;
    uint32 next;
    uint32 flags;
    uint32 selected;
    uint32 destination;
    sint32 round;
    FUNCTION_MARKER(0x800B26D0, "SLUS_004.02-BNS-logical-00");
    round = r_s32(state + 0x24);
    mask = round < 7 ? 0x3FF : round < 17 ? 0xFBFFF : 0x1FFFFF;
    mask &= 0x157FFF;
    for (index = 0; index < 4; ++index) {
        previous = r_u32(state + 0x50 + index * 4);
        if (previous != 22)
            mask &= ~((uint32)1 << (previous & 31));
    }
    count = 0;
    for (index = 0; index < 22; ++index) {
        if (mask & ((uint32)1 << index)) {
            pairs[count * 2] = index;
            pairs[count * 2 + 1] = r_u16(state + 0x60 + index * 2);
            ++count;
        }
    }
    t3_8004cd80(pairs, count, 1);
    previous = r_u32(state + 0x44);
    threshold = pairs[1];
    if (previous >= 20)
        threshold += 4;
    eligible = 0;
    while (eligible < count && pairs[eligible * 2 + 1] <= threshold)
        ++eligible;
    random = t3_8004ce54();
    next = r_u32(state + 0x4C) + 1;
    flags = r_u32(0x800AE218);
    selected = pairs[(random % eligible) * 2];
    w_u32(state + 0x48, (selected << 2) + (flags & 1));
    if ((sint32)next >= 4)
        next = 0;
    destination = state + next * 4;
    w_u32(state + 0x4C, next);
    w_u32(destination + 0x50, selected);
    return destination;
}

uint32 t3_bns_00_800b467c(uint32 score)
{
    uint32 index;
    FUNCTION_MARKER(0x800B467C, "SLUS_004.02-BNS-logical-00");
    for (index = 0; index < 10; ++index) {
        if ((sint32)r_u16(0x80097FFE + index * 8) < (sint32)score)
            return index + 1;
    }
    return 0;
}

uint32 t3_bns_00_800b0b84(uint32 state)
{
    uint32 value;
    uint32 side;
    uint32 packed;
    uint32 random;
    uint32 flags;
    sint32 score;
    FUNCTION_MARKER(0x800B0B84, "SLUS_004.02-BNS-logical-00");
    value = t3_bns_00_800b467c(r_u32(state + 0x44));
    w_u32(state + 0x40, value);
    t3_bns_00_800b26d0(state);
    side = r_u8(state + 0x1F);
    packed = r_u32(state + 0x48);
    w_u16(0x800ADD5C + side * 2, packed >> 2);
    w_u16(0x800ADD98 + side * 2, packed & 3);
    w_u16(0x800A9128 + side * 2, 1);
    t3_8004f0e4(side);
    t3_800517b0(side);
    score = r_s32(state + 0x44);
    w_u16(0x800AE208, 1);
    if (score < 5) {
        random = t3_8004ce54() & 0xFFFF;
        w_u8(0x800AE211, random % 3 + 1);
    } else if (score < 10) {
        random = t3_8004ce54() & 0xFFFF;
        w_u8(0x800AE211, random % 3 + 2);
    } else if (score < 20) {
        random = t3_8004ce54() & 0xFFFF;
        w_u8(0x800AE211, random % 5 + 3);
    } else if (score < 50) {
        random = t3_8004ce54() & 0xFFFF;
        flags = r_u32(0x800AE218);
        w_u8(0x800AE211, random % 5 + 3);
        if (flags & 0x10)
            w_u16(0x800AE208, 2);
    } else if (score % 10 == 0) {
        w_u8(0x800AE211, 0);
    } else if (score % 5 == 3) {
        random = t3_8004ce54() & 0xFFFF;
        w_u8(0x800AE211, random % 4 + 6);
        w_u16(0x800AE208, 2);
    } else {
        random = t3_8004ce54() & 0xFFFF;
        flags = r_u32(0x800AE218);
        w_u8(0x800AE211, random % 7 + 3);
        if (flags & 0x14)
            w_u16(0x800AE208, 2);
    }
    if (r_u8(0x800AE211) >= 10)
        w_u8(0x800AE211, 9);
    if (r_u16(0x800AE208) >= 3)
        w_u16(0x800AE208, 2);
    return 2;
}
