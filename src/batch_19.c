#include "game_functions.h"
#include "t3_audio.h"
#include "t3_data.h"
#include "psx_spu.h"
#include <stdlib.h>

uint32 t3_80078550(uint32 side, uint32 controller_select)
{
    uint32 text_base;
    uint32 pressed;
    uint32 held;
    uint32 x_offset;
    uint32 selection;
    uint32 count;
    uint32 scroll;
    uint32 disabled;
    uint32 page_offset;
    uint32 index;
    uint32 y;
    uint32 text;
    uint32 rows;
    uint32 row;
    uint32 packed_y;
    uint8 first_line[24];

    FUNCTION_MARKER(0x80078550, "SLUS_004.02");
    side &= 255;
    text_base = 0x800A3628 + side * 416;
    pressed = r_u16(controller_select ? 0x800ADF0A : 0x800ADF08);
    held = r_u16(controller_select ? 0x800ADD6A : 0x800ADD68);
    x_offset = side ? 184 : 0;
    selection = 0x800A3618 + side * 8;
    disabled = r_u32(0x800ADCA4);
    count = r_u8(text_base);
    if (!disabled) {
        uint32 delta = pressed == 0x4000;
        uint32 selected;
        if (pressed == 0x1000)
            delta = (uint32)-1;
        if (!(held & 0x5000)) {
            if (pressed == 0x2000)
                delta = 5;
            else if (pressed == 0x8000)
                delta = (uint32)-5;
        }
        if (delta)
            t3_800753bc(0, 0x546C, 0);
        selected = r_u32(selection);
        if ((sint32)selected >= (sint32)count)
            selected = 0;
        selected += delta;
        if ((sint32)selected < 0)
            selected += count;
        if ((sint32)selected >= (sint32)count)
            selected -= count;
        scroll = r_u32(selection + 4);
        w_u32(selection, selected);
        w_u32(selection + 4, scroll + (delta << 6));
    }
    scroll = r_u32(selection + 4);
    if (scroll) {
        uint32 negative = (sint32)scroll < 0;
        sint32 reduction;
        if (negative)
            scroll = 0 - scroll;
        reduction = (sint32)(scroll + 3) / 3;
        scroll -= (uint32)reduction;
        if ((sint32)scroll <= 0)
            scroll = 0;
        if (negative)
            scroll = 0 - scroll;
    }
    disabled = r_u32(0x800ADCA4);
    w_u32(selection + 4, scroll);
    if (disabled)
        return disabled;
    page_offset = (uint32)((sint32)scroll / 64) + ((sint32)scroll > 0);
    index = r_u32(selection) - page_offset;
    y = scroll + 118 - (page_offset << 6);
    while ((sint32)index < 0)
        index += count;
    while ((sint32)index >= (sint32)count)
        index -= count;
    text = t3_80079144(text_base + 1, index << 1);
    rows = 6;
    if (r_s16(0x800A9228 + side * 6284 + 0x18) == 15) {
        y = 246;
        rows = 1;
    }
    t3_80077724(0x80027C04, (uint32)0x707070, side);
    packed_y = (y << 16) - 0x20000;
    for (row = 0; row < rows; ++row) {
        uint32 color0;
        uint32 color1;
        uint32 gradient = 1;
        uint32 packet;
        uint32 second_color;
        uint32 second_position;
        uint32 second_size;
        if ((sint32)t3_80079208(text) >= 29) {
            t3_80077724(0x80027C0C, x_offset + 6, y);
            text = t3_80079170(first_line, text, 28);
            t3_command_native_text(first_line);
            t3_80077724(0x80027C0C, x_offset + 6, y + 12);
        } else {
            t3_80077724(0x80027C0C, x_offset + 6, y + 6);
        }
        text = t3_80077724(text);
        t3_80077724(0x80027C0C, x_offset + 6, y + 39);
        text = t3_80077724(text);
        color0 = 0x602010;
        color1 = 0xB89246;
        if (r_s16(0x800A9228 + side * 6284 + 0x18) == 6) {
            if (index - 6 < 8) {
                color0 = 0x204000;
                color1 = 0x5FA255;
                if ((sint32)index < 7) {
                    color1 = 0x602010;
                    gradient = 0;
                }
            } else if ((sint32)index >= 14) {
                color0 = 0x103040;
                color1 = 0x2090A0;
                if ((sint32)index < 15) {
                    color1 = 0x602010;
                    gradient = 0;
                }
            }
        }
        if (gradient) {
            uint32 position = (x_offset | 3) | packed_y;
            uint32 bottom = (y + 24) << 16;
            packet = t3_8004d8b0(r_u32(0x800A9218) + 4, r_u32(0x800ADF64),
                color0 | 0x38000000, color0, color1, color1,
                position, (x_offset + 181) | packed_y,
                (x_offset | 3) | bottom, (x_offset + 181) | bottom);
            w_u32(0x800ADF64, packet);
            packet = t3_8004da10(r_u32(0x800A9218) + 4, packet,
                color0 | 0x60000000, position, 0x3F00B2);
            second_color = 0x60D0D0D0;
            second_position = (x_offset | 2) | ((y - 3) << 16);
            second_size = 0x4200B4;
        } else {
            packet = t3_8004da10(r_u32(0x800A9218) + 4, r_u32(0x800ADF64),
                color1 | 0x60000000, (x_offset | 3) | packed_y, 0x1A00B2);
            second_color = color0 | 0x60000000;
            second_position = (x_offset | 2) | packed_y;
            second_size = 0x3F00B4;
        }
        w_u32(0x800ADF64, packet);
        packet = t3_8004da10(r_u32(0x800A9218) + 4, packet,
            second_color, second_position, second_size);
        w_u32(0x800ADF64, packet);
        ++index;
        if ((sint32)index >= (sint32)count) {
            text = text_base + 1;
            index = 0;
        }
        packed_y += 0x400000;
        y += 64;
    }
    t3_80077724(0x80027C14, x_offset + 39, (uint32)97, (uint32)0);
    {
        uint32 packet;
        packet = t3_8004da10(r_u32(0x800A9218), r_u32(0x800ADF64),
            0x60E0E0E0, x_offset | 0x720002, 0x200B4);
        w_u32(0x800ADF64, packet);
        packet = t3_8004da10(r_u32(0x800A9218), packet,
            0x60E0E0E0, x_offset | 0x1B30002, 0x200B4);
        w_u32(0x800ADF64, packet);
        packet = t3_8004da10(r_u32(0x800A9218), packet,
            0x60602010, x_offset | 0x5C0001, 0x17000B6);
        w_u32(0x800ADF64, packet);
        return packet;
    }
}

void t3_bns_01_800b70c8(uint32 mode, uint32 x, uint32 y, uint32 color)
{
    uint8 text[256];
    uint32 output = 0;
    uint32 source;
    uint32 current_x = x;
    uint32 character;
    uint32 clut;

    FUNCTION_MARKER(0x800B70C8, "SLUS_004.02-BNS-logical-01");
    if (r_u32(0x800ADCA4))
        return;
    source = r_u32(0x800B8B40 + mode * 4);
    character = r_u8(source++);
    if (character) {
        clut = (((((uint32)((sint32)color >> 4) + 24) & 31) + 480) << 6)
            | (color & 15) | 16;
        clut <<= 16;
        do {
            if (character == 'u' || character == 'd') {
                uint32 packet;
                packet = t3_8004d9bc(r_u32(0x800A9218) + 28, r_u32(0x800ADF64),
                    0x65000000, (current_x & 0xFFFF) | (y << 16),
                    0x10000A, clut | (character == 'u' ? 0xE078 : 0xE082));
                w_u32(0x800ADF64, packet);
                packet = t3_8004daa8(r_u32(0x800A9218) + 28, packet, 6);
                w_u32(0x800ADF64, packet);
                character = 32;
            }
            text[output++] = (uint8)character;
            current_x += 9;
            character = r_u8(source++);
        } while (character);
    }
    text[output] = 0;
    t3_render_native_strings(0x800B0A50, color, (uint32)0, x, y, (const uint8 *)text);
}

void t3_pad_prepare_actuators(uint32 owner)
{
    psx_pad_prepare_actuators(owner, 0x8009B948);
}

void t3_8008ebd8(void)
{
    CdInit();
}

uint32 t3_bns_03_800b3e58(void)
{
    sint32 index;

    FUNCTION_MARKER(0x800B3E58, "SLUS_004.02-BNS-logical-03");
    for (index = 7; index >= 0; --index)
        w_u8(0x800B6690 + (uint32)index * 32 + 31, 0);
    w_u32(0x800B6688, 0);
    w_u32(0x800B6790, 0);
    return 0x800B0000;
}

uint32 t3_bns_03_800b496c(void)
{
    uint32 index;
    uint32 record;

    FUNCTION_MARKER(0x800B496C, "SLUS_004.02-BNS-logical-03");
    for (index = 0; index < 2; ++index) {
        record = 0x800B6424 + index * 16;
        w_u32(record, 0);
        w_u16(record + 4, 0);
        w_u16(record + 6, 0);
        w_u16(record + 8, 0);
    }
    return 0;
}

uint32 t3_bns_03_800b07c8(void)
{
    uint32 row;
    uint32 fourth;
    uint32 fifth;
    uint32 index;
    uint32 actor;

    FUNCTION_MARKER(0x800B07C8, "SLUS_004.02-BNS-logical-03");
    row = 0x800B5F9C + r_u8(0x800AFAC9) * 24;
    w_u16(0x800B6612, r_u16(row));
    w_u16(0x800B6610, r_u16(row + 4));
    w_u16(0x800B6614, r_u16(row + 8));
    fourth = r_u16(row + 16);
    fifth = r_u16(row + 20);
    w_u16(0x800B6618, fourth);
    w_u16(0x800B6616, fifth);
    t3_bns_03_800b0b14();
    for (index = 0; index < 2; ++index) {
        actor = 0x800A9228 + index * 6284;
        w_u32(actor, index ? 0x1400 : 0xFFFFEC00);
        w_u32(actor + 8, 0);
        w_u16(actor + 14, index ? 0xC000 : 0x4000);
        t3_8002bd78(actor, 0x800A9228 + (1 - index) * 6284, 0);
    }
    t3_bns_03_800b3e58();
    t3_bns_03_800b496c();
    w_u32(0x800AFAC0, 0);
    w_u32(0x800AFAC4, 2);
    w_u8(0x800B5F98, 0);
    w_u32(0x800B6620, 0);
    return 0x800B0000;
}

uint32 t3_bns_03_800b41cc(uint32 selector)
{
    uint32 index = selector & 255;
    uint32 mask = 1u << (index & 31);
    uint32 result;
    uint32 object;

    FUNCTION_MARKER(0x800B41CC, "SLUS_004.02-BNS-logical-03");
    result = r_u8(0x800B5F98) & mask;
    if (result)
        return result;
    object = t3_8006f4f4();
    if (object == 0xFFFFFFFF)
        return 15;
    w_u8(object + 12, 15);
    w_u32(object + 64, selector & 1);
    result = r_u8(0x800B5F98);
    w_u32(object + 76, index >> 1);
    result |= mask;
    w_u8(0x800B5F98, result);
    return result;
}

uint32 t3_bns_03_800b425c(uint32 variant, uint32 kind)
{
    uint32 selector = variant + (kind << 1);
    uint32 index = selector & 255;
    uint32 mask = 1u << (index & 31);
    uint32 result;
    uint32 object;

    FUNCTION_MARKER(0x800B425C, "SLUS_004.02-BNS-logical-03");
    result = r_u8(0x800B5F98) & mask;
    if (result)
        return result;
    object = t3_8006f4f4();
    if (object == 0xFFFFFFFF)
        return 15;
    w_u8(object + 12, 15);
    w_u32(object + 64, selector & 1);
    result = r_u8(0x800B5F98);
    w_u32(object + 76, index >> 1);
    result |= mask;
    w_u8(0x800B5F98, result);
    return result;
}

uint32 t3_80073c90(uint32 object)
{
    uint32 context;
    uint32 random;
    uint32 second_random;
    uint32 kind;
    uint32 phase;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 velocity;
    uint32 position;
    uint32 state;
    uint32 variant;
    uint32 shift;
    uint32 mask;
    uint32 flags;

    FUNCTION_MARKER(0x80073C90, "SLUS_004.02");
    context = t3_bns_03_800b07a8();
    random = (uint32)randPSX();
    second_random = (uint32)randPSX();
    random |= second_random << 16;
    kind = r_u32(object + 76);
    if (kind == 0) {
        phase = r_u8(object + 14);
        if (phase == 0)
            t3_80073ef8(object);
        if (phase == 0 || phase == 1)
            t3_80074104(object);
    } else if (kind == 1) {
        phase = r_u8(object + 14);
        if (phase == 0)
            t3_80073ef8(object);
        if (phase == 0 || phase == 1)
            t3_800742ac(object);
        else if (phase == 2)
            t3_800743a0(object);
    }
    x = r_u32(object + 40);
    y = r_u32(object + 44);
    z = r_u32(object + 48);
    w_u32(object + 60, z);
    w_u32(object + 52, x);
    w_u32(object + 56, y);
    velocity = r_u32(context + 120);
    position = r_u32(context + 104);
    w_u32(object + 40, position + (uint32)((sint32)velocity >> 8));
    velocity = r_u32(context + 124);
    position = r_u32(context + 108);
    w_u32(object + 44, position + (uint32)((sint32)velocity >> 8));
    velocity = r_u32(context + 128);
    position = r_u32(context + 112);
    w_u32(object + 48, position + (uint32)((sint32)velocity >> 8));
    state = r_u8(context + 326);
    if (state == 5)
        return 5;
    if (state == 8)
        return 8;
    kind = r_u32(object + 76);
    if (kind != 0 && kind != 1)
        return random & 15;
    if (kind == 0) {
        if (!(random & 15))
            t3_80039f0c((const uint32 *)psx_addr(object + 40, 12), 0x80025330);
        variant = r_u32(object + 64);
        state = r_u8(context + 326);
        if (variant == 0) {
            if (state == 6)
                return 6;
        } else if (state == 7) {
            return 7;
        }
    } else {
        if (!(random & 15))
            t3_80039f0c((const uint32 *)psx_addr(object + 40, 12), 0x8002517C);
        state = r_u8(context + 326);
        if (state == 9)
            return 9;
    }
    shift = r_u32(object + 76);
    variant = r_u32(object + 64);
    shift = (shift << 1) + variant;
    mask = (kind ? kind : 1u) << (shift & 31);
    flags = mask ? 0 : r_u8(0x800B5F98) & 1;
    w_u8(0x800B5F98, flags);
    return t3_8006f5ac(object);
}

uint32 t3_bns_03_800b114c(uint32 object)
{
    uint32 history = object + 0xD0;
    uint32 limit = object + 0x130;
    uint32 current = object + 0xDC;
    uint32 offsets = 0x800B6344;
    uint32 y;
    uint32 z;
    uint32 position;
    sint32 delta;
    FUNCTION_MARKER(0x800B114C, "SLUS_004.02-BNS-logical-03");
    while (history < limit) {
        w_u32(history, r_u32(current));
        y = r_u32(current + 4);
        z = r_u32(current + 8);
        w_u32(current - 8, y);
        w_u32(current - 4, z);
        delta = r_s16(offsets);
        position = r_u32(object + 0x68);
        history += 0x18;
        w_u32(current, position + (uint32)delta);
        delta = r_s16(offsets + 2);
        position = r_u32(object + 0x6C);
        w_u32(current + 4, position + (uint32)delta);
        delta = r_s16(offsets + 4);
        position = r_u32(object + 0x70);
        w_u32(current + 8, position + (uint32)delta);
        offsets += 8;
        current += 0x18;
    }
    return 0;
}

uint32 t3_bns_03_800b0b14(void)
{
    uint32 object = r_u32(0x800ADD74);
    uint32 mode = r_u32(0x80095484);
    uint32 player;
    uint32 opponent;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 half_width;
    uint32 colors[9];
    uint32 group;
    uint32 i;
    uint32 packet;
    uint32 quad;
    uint32 line;
    sint32 shade;
    sint32 index;
    FUNCTION_MARKER(0x800B0B14, "SLUS_004.02-BNS-logical-03");
    w_u16(object, 0);
    w_u16(object + 2, 0);
    w_u8(object + 0x147, mode == 1 ? 100 : 40);
    t3_8003c250(0, object + 4);
    t3_8003c200(object + 0x48);
    w_u32(object + 0x68, r_u8(0x800AFAC8) ? 0xD00 : 0xFFFFF300);
    player = r_u8(0x800AFAC8);
    w_u32(object + 0x130, 0x800A9228 + player * 6284);
    opponent = 1u - r_u8(0x800AFAC8);
    y = (uint32)r_s16(0x800B6614);
    w_u32(object + 0x6C, 0xFFFFFB00);
    w_u16(object + 0xA0, 32);
    w_u16(object + 0xA2, 16);
    w_u32(object + 0x70, 0);
    w_u32(object + 0xA8, 0);
    w_u32(object + 0xAC, 0);
    w_u16(object + 0x13C, 0);
    w_u16(object + 0x13E, 0);
    w_u16(object + 0x140, 0);
    w_u32(object + 0x7C, 0);
    w_u32(object + 0x80, 0);
    w_u32(object + 0x78, 0);
    w_u32(object + 0x90, 0);
    w_u32(object + 0x8C, 0);
    w_u32(object + 0x88, 0);
    w_u16(object + 0x9C, 0);
    w_u16(object + 0x9A, 0);
    w_u16(object + 0x98, 0);
    w_u16(object + 0xA4, 4);
    w_u16(object + 0xB0, 0);
    w_u16(object + 0xB2, 0);
    w_u16(object + 0x14C, 0);
    w_u16(object + 0x14A, 0);
    w_u16(object + 0x148, 0);
    w_u16(object + 0x150, 0);
    w_u8(object + 0xCC, 0);
    w_u32(object + 0x134, 0x800A9228 + opponent * 6284);
    w_u32(object + 0x8C, y);
    w_u8(object + 0x144, 0);
    w_u16(object + 0xC4, 0x260);
    x = r_u32(object + 0x68);
    y = r_u32(object + 0x6C);
    z = r_u32(object + 0x70);
    w_u32(object + 0xC8, 0x5A400);
    w_u32(object + 0xB4, 0x800B5FE4);
    w_u32(object + 0xB8, x);
    w_u32(object + 0xBC, y);
    w_u32(object + 0xC0, z);
    t3_bns_03_800b114c(object);
    t3_bns_03_800b114c(object);
    half_width = r_u16(0x800B6616);
    for (i = 0; i < 9; ++i) {
        z = (uint32)(-8000 + (sint32)i * 1536);
        w_u16(0x800B6480 + i * 16, 0xFF9C);
        w_u16(0x800B6488 + i * 16, 100);
        w_u16(0x800B6482 + i * 16, 0);
        w_u16(0x800B648A + i * 16, 0);
        w_u16(0x800B6484 + i * 16, z);
        w_u16(0x800B648C + i * 16, z);
        w_u16(0x800B6510 + i * 16, 0u - half_width);
        w_u16(0x800B6518 + i * 16, half_width);
        w_u16(0x800B6512 + i * 16, 0);
        w_u16(0x800B651A + i * 16, 0);
        w_u16(0x800B6514 + i * 16, z);
        w_u16(0x800B651C + i * 16, z);
    }
    for (index = 8; index >= 0; --index) {
        shade = index >= 4 ? 48 * (8 - index) : 192 - 48 * (4 - index);
        x = shade > 0 ? (uint32)shade & 255 : 0;
        colors[index] = 0x3A000000 | (x << 16) | (x << 8) | x;
    }
    for (group = 0; group < 2; ++group) {
        for (index = 7; index >= 0; --index) {
            quad = object + group * 0x120 + 0xFC - (uint32)(7 - index) * 0x24;
            line = object + group * 0x140 + 0x118 - (uint32)(7 - index) * 0x28;
            w_u32(quad + 0x1220, colors[index + 1]);
            w_u32(quad + 0x1228, colors[index + 1]);
            w_u32(quad + 0x1210, colors[index]);
            w_u32(quad + 0x1218, colors[index]);
            w_u8(quad + 0x120F, 8);
            w_u8(quad + 0x1213, 0x3A);
            w_u32(line + 0xFAC, colors[index + 1]);
            w_u32(line + 0xFA4, colors[index]);
            w_u8(line + 0xFA3, 4);
            w_u8(line + 0xFA7, 0x52);
            w_u32(line + 0xF90, colors[index]);
            w_u32(line + 0xF98, colors[index + 1]);
            w_u8(line + 0xF8F, 4);
            w_u8(line + 0xF93, 0x52);
        }
        SetDrawMode((DR_MODE *)psx_addr(object + 0x144C + group * 12, 12), 0, 1, 32, NULL);
    }
    for (group = 0; group < 2; ++group) {
        for (i = 0; i < 16; ++i) {
            packet = object + 0x154 + group * 0x1C0 + i * 0x1C;
            w_u8(packet + 4, 0x80);
            w_u8(packet + 5, 0x80);
            w_u8(packet + 6, 0x80);
            setPolyG3(psx_addr(packet, 28));
            setSemiTrans(psx_addr(packet, 28), 0);
        }
        for (i = 0; i < 32; ++i) {
            packet = object + 0x4D4 + group * 0x480 + i * 0x24;
            w_u8(packet + 4, 0x80);
            w_u8(packet + 5, 0x80);
            w_u8(packet + 6, 0x80);
            setPolyG4(psx_addr(packet, 36));
            setSemiTrans(psx_addr(packet, 36), 0);
        }
        for (i = 0; i < 4; ++i) {
            packet = object + 0xDD4 + group * 0x60 + i * 0x18;
            setPolyF4(psx_addr(packet, 24));
            setSemiTrans(psx_addr(packet, 24), 0);
            packet = object + 0xEAC + group * 0x70 + i * 0x1C;
            w_u8(packet + 4, 0xFF);
            w_u8(packet + 5, 0xFF);
            w_u8(packet + 6, 0xFF);
            w_u8(packet + 12, 0x30);
            w_u8(packet + 13, 0x30);
            w_u8(packet + 14, 0x30);
            w_u8(packet + 20, 0x30);
            w_u8(packet + 21, 0x30);
            w_u8(packet + 22, 0x30);
            setPolyG3(psx_addr(packet, 28));
            setSemiTrans(psx_addr(packet, 28), 1);
        }
        SetDrawMode((DR_MODE *)psx_addr(object + 0xE94 + group * 12, 12), 0, 1, 32, NULL);
    }
    for (group = 0; group < 2; ++group) {
        for (i = 0; i < 4; ++i) {
            x = t3_80039e9c(r_u16(0x800ADC84));
            w_u32(object + group * 0x60 + i * 0x18 + 0xDD8, x | 0x29000000);
        }
    }
    return 0;
}

void t3_8003c200(uint32 object)
{
    uint32 x;
    uint32 y;
    uint32 z;
    FUNCTION_MARKER(0x8003C200, "SLUS_004.02");
    x = r_u32(0x800ADF18);
    y = r_u32(0x800ADF1C);
    z = r_u32(0x800ADF20);
    w_u32(object, x);
    w_u32(object + 4, y);
    w_u32(object + 8, z);
    x = r_u32(0x800ADF24);
    y = r_u32(0x800ADF28);
    z = r_u32(0x800ADF2C);
    w_u32(object + 12, x);
    w_u32(object + 16, y);
    w_u32(object + 20, z);
    x = r_u32(0x800ADF30);
    y = r_u32(0x800ADF34);
    w_u32(object + 24, x);
    w_u32(object + 28, y);
}

static sint32 t3_ai_candidate_character_matches(uint32 row, uint32 actor, uint32 opponent)
{
    uint32 selector = r_u8(row + 2);
    if (!selector)
        return 1;
    if (selector < 24)
        return r_s16(actor + 0x16) == (sint32)selector - 1;
    if (selector < 47)
        return r_s16(actor + 0x16) != (sint32)selector - 24;
    if (selector < 70)
        return r_s16(opponent + 0x16) == (sint32)selector - 47;
    if (selector < 93)
        return r_s16(actor + 0x18) == (sint32)selector - 70;
    if (selector < 116)
        return r_s16(actor + 0x18) != (sint32)selector - 93;
    return 0;
}

static sint32 t3_ai_candidate_condition_matches(uint32 row, uint32 predicate_row, uint32 actor, uint32 opponent, uint32 buttons, sint32 continuation)
{
    uint32 condition = r_u8(row + 3);
    if (!condition)
        return 1;
    if (condition == 0x24)
        return (buttons & 1) != 0;
    if (condition == 0x25)
        return (buttons & 2) != 0;
    if (condition == 0x26)
        return continuation || (buttons & 4) != 0;
    if (condition == 0x30)
        return continuation || (buttons & 8) != 0;
    if (condition == 0x27)
        return continuation || (buttons & 16) != 0;
    if (continuation)
        opponent = r_u32(0x800A867C);
    return t3_8002e0bc(predicate_row, actor, opponent) != 0;
}

uint32 t3_80056764(uint32 state, uint32 mode)
{
    uint32 cursor = r_u32(r_u32(state + 0x58) + 12);
    uint32 opponent = r_u32(0x800A867C);
    uint32 lookup = r_u32(state + 12);
    uint32 actor = r_u32(0x800AFA5C);
    uint32 buttons = r_u32(state + 0x64);
    uint32 output = 0x8009F978;
    uint32 total = 0;
    uint32 overflow = 0;
    uint32 expanded;
    uint32 count;
    uint32 index;
    uint32 animation;
    uint32 callback;
    sint32 frame;
    FUNCTION_MARKER(0x80056764, "SLUS_004.02");
    while (r_u16(cursor) != 0xC000) {
        if (r_u16(cursor) == 0xC00C) {
            expanded = 0x80017C78 + r_u16(cursor + 6) * 12;
            count = r_u8(cursor + 11);
            for (index = 0; index < count; ++index, expanded += 12) {
                frame = r_s16(actor + 0x58);
                if (frame < r_u8(expanded + 9) || r_u8(expanded + 10) < frame)
                    continue;
                if (!t3_ai_candidate_character_matches(expanded, actor, r_u32(0x800A867C)))
                    continue;
                if (!t3_ai_candidate_condition_matches(expanded, expanded, actor, opponent, buttons, 0))
                    continue;
                if (total >= 180) {
                    overflow = 1;
                    break;
                }
                animation = r_u32(lookup + r_u16(expanded + 6) * 4);
                ++total;
                w_u32(output, animation);
                w_u16(output + 8, 0);
                w_u32(output + 4, expanded);
                output += 12;
            }
            if (overflow)
                goto first_stream_done;
        } else {
            frame = r_s16(actor + 0x58);
            if (frame >= r_u8(cursor + 9) && r_u8(cursor + 10) >= frame
                && t3_ai_candidate_character_matches(cursor, actor, r_u32(0x800A867C))
                && t3_ai_candidate_condition_matches(cursor, cursor, actor, opponent, buttons, 0)) {
                if (total >= 180)
                    goto first_stream_done;
                animation = r_u32(lookup + r_u16(cursor + 6) * 4);
                ++total;
                w_u16(output + 8, 0);
                w_u32(output + 4, cursor);
                w_u32(output, animation);
                output += 12;
            }
        }
        cursor += 12;
    }

first_stream_done:
    if (mode)
        return total;
    frame = r_s16(actor + 0x58);
    if (frame < r_u8(cursor + 9) || r_u8(cursor + 10) < frame)
        goto mark_candidates;
    animation = r_u32(lookup + r_u16(cursor + 6) * 4);
    if (animation == r_u32(actor + 0x54))
        goto mark_candidates;
    cursor = r_u32(animation + 12);
    expanded = cursor;
    while (r_u16(cursor) != 0xC000) {
        if (r_u16(cursor) == 0xC00C) {
            expanded = 0x80017C78 + r_u16(cursor + 6) * 12;
            count = r_u8(cursor + 11);
            for (index = 0; index < count; ++index, expanded += 12) {
                if (r_u8(expanded + 9) >= 2)
                    continue;
                frame = r_s16(actor + 0x58);
                if (r_u8(expanded + 10) < frame)
                    continue;
                if (!t3_ai_candidate_character_matches(expanded, actor, r_u32(0x800A867C)))
                    continue;
                if (!t3_ai_candidate_condition_matches(expanded, expanded, actor, 0, buttons, 1))
                    continue;
                if (total >= 180) {
                    overflow = 1;
                    break;
                }
                animation = r_u32(lookup + r_u16(expanded + 6) * 4);
                ++total;
                w_u32(output, animation);
                w_u16(output + 8, 0);
                w_u32(output + 4, expanded);
                output += 12;
            }
            if (overflow)
                goto mark_candidates;
        } else {
            if (r_u8(cursor + 9) < 2) {
                frame = r_s16(actor + 0x58);
                if (r_u8(cursor + 10) >= frame
                    && t3_ai_candidate_character_matches(cursor, actor, r_u32(0x800A867C))
                    && t3_ai_candidate_condition_matches(cursor, expanded, actor, 0, buttons, 1)) {
                    if (total >= 180)
                        goto mark_candidates;
                    animation = r_u32(lookup + r_u16(cursor + 6) * 4);
                    ++total;
                    w_u16(output + 8, 0);
                    w_u32(output + 4, cursor);
                    w_u32(output, animation);
                    output += 12;
                }
            }
        }
        cursor += 12;
    }

mark_candidates:
    for (index = 0, output = 0x8009F978; index < total; ++index, output += 12) {
        animation = r_u32(output);
        /* Remove the original comparison against undefined SP+1C */
        if (r_u32(animation + 4) & 0x800000)
            w_u16(output + 8, 0xFFFF);
    }
    callback = r_u32(state + 0x7C);
    if (callback) {
        w_u16(state + 0x70, total);
        psx_dispatch_guest_call(callback, 1, state, 0, 0);
    }
    return total;
}

static uint32 t3_lighting_signed_divide(uint32 numerator, uint32 denominator)
{
    if (!denominator)
        return (sint32)numerator < 0 ? 1 : 0xFFFFFFFF;
    return (uint32)((sint64)(sint32)numerator / (sint32)denominator);
}

static uint32 t3_lighting_signed_remainder(uint32 numerator, uint32 denominator)
{
    if (!denominator)
        return numerator;
    return (uint32)((sint64)(sint32)numerator % (sint32)denominator);
}

uint32 t3_80049848(uint32 actor, uint32 opponent)
{
    uint32 count = r_u32(0x8009E82C);
    uint32 uv_table = 0x1F800000 + (count + 2) * 4;
    uint32 cache = uv_table + 32;
    uint32 depths = cache + (uint32)((sint32)(count + 3) / 2) * 2;
    uint32 first_position = actor + 0xF68;
    uint32 second_position = opponent + 0xF68;
    uint32 has_third = r_u32(0x800AFA88) == 8;
    uint32 third_position = 0;
    uint32 x, z, spacing, half, tile_x, tile_z, origin_x, origin_z;
    uint32 divisor, cell_x, cell_z, data, vertices, vertex_count;
    uint32 first, second, index, ordering, indices, packet, buffer;
    uint32 first_x, first_z, second_x, second_z, scale = 0;
    uint32 row, column, packed, vertex[4], visible, descriptor;
    uint32 layer, texture, attributes, uv, flip, field, tag;
    uint32 cache_address, point, dx, dz, denominator, attenuation, sum;
    uint32 shade = actor;
    sint32 light_type, sine, screen_y;
    FUNCTION_MARKER(0x80049848, "SLUS_004.02");
    if (has_third)
        third_position = t3_bns_02_800b297c() + 0xF68;
    sine = r_s16(0x8001E8C4 + ((r_u32(0x800A85C0) + 239) & 0xFFF) * 2);
    w_u32(0x800ADFD0, sine > 0);
    if (!sine)
        return 0;
    if (r_u32(0x800AFA88) == 8) {
        spacing = r_u32(0x8009E780);
        half = (uint32)((sint32)spacing / 2);
        z = r_u32(third_position + 8);
        x = r_u32(third_position);
        z += half;
    } else {
        sum = r_u32(first_position);
        sum += r_u32(second_position);
        first = r_u32(first_position + 8);
        second = r_u32(second_position + 8);
        x = (uint32)((sint32)sum / 2);
        z = (uint32)((sint32)(first + second) / 2);
    }
    spacing = r_u32(0x8009E780);
    half = (uint32)((sint32)spacing / 2);
    x += half;
    z += half;
    tile_x = t3_lighting_signed_divide(x, spacing);
    if ((sint32)x < 0)
        --tile_x;
    origin_x = tile_x * spacing;
    tile_z = t3_lighting_signed_divide(z, spacing);
    if ((sint32)z < 0)
        --tile_z;
    divisor = r_u32(0x8009E824);
    cell_x = t3_lighting_signed_remainder(tile_x, divisor);
    origin_z = tile_z * spacing;
    if ((sint32)cell_x < 0)
        cell_x += divisor;
    divisor = r_u32(0x8009E828);
    cell_z = t3_lighting_signed_remainder(tile_z, divisor);
    if ((sint32)cell_z < 0)
        cell_z += divisor;
    cell_z = divisor - (cell_z + 1);
    for (index = 0; index < 4; ++index) {
        first = r_u32(0x8009E800 + index * 8);
        first = r_u32(0x8009E800 + index * 8);
        second = r_u32(0x8009E804 + index * 8);
        second = r_u32(0x8009E804 + index * 8);
        w_u32(uv_table + index * 8, first);
        w_u32(uv_table + index * 8, first);
        w_u32(uv_table + index * 8 + 4, second);
        w_u32(uv_table + index * 8 + 4, second);
    }
    t3_8004adc4(origin_x, 0, origin_z);
    data = r_u32(0x800ADD14);
    vertices = r_u32(data + 12);
    vertex_count = r_u32(data + 16);
    t3_8004a364(vertices, 0x1F800000, depths, vertex_count);
    count = (r_u32(0x8009E82C) + 3) >> 2;
    for (index = 0; index < count; ++index)
        w_u32(cache + index * 4, 0xFFFFFFFF);
    data = r_u32(0x800ADD14);
    ordering = r_u32(r_u32(0x800A8C54) + 4) + 4084;
    first_x = r_u32(first_position) - origin_x;
    first_z = r_u32(first_position + 8) - origin_z;
    second_x = r_u32(second_position) - origin_x;
    second_z = r_u32(second_position + 8) - origin_z;
    indices = r_u32(data + 28);
    buffer = r_u32(0x800ADEFC);
    packet = data ? data + buffer * 5200 + 1508 : 0;
    light_type = r_s16(0x8009E820);
    switch (light_type) {
    case 1: scale = 0x0F200000; break;
    case 2: scale = 0x2FAF0800; break;
    case 3: scale = 0x1E848000; break;
    case 4: scale = 0x18B82000; break;
    }
    first = r_u32(0x8009E828);
    if ((sint32)first <= 0)
        return first;
    row = 0;
    do {
        divisor = r_u32(0x8009E824);
        column = 0;
        x = cell_x;
        if ((sint32)divisor > 0) {
            do {
                data = r_u32(0x800ADD14);
                packed = r_u32(indices + (uint32)(sint32)(sint16)(column * 4 + row * 40));
                vertices = r_u32(data + 12);
                for (index = 0; index < 4; ++index)
                    vertex[index] = (packed >> (index * 8)) & 255;
                visible = 1;
                for (index = 0; index < 4; ++index) {
                    if (!r_s16(depths + vertex[index] * 2)) {
                        visible = 0;
                        break;
                    }
                }
                if (visible) {
                    visible = 0;
                    for (index = 0; index < 4; ++index) {
                        screen_y = r_s16(0x1F800002 + vertex[index] * 4);
                        if (screen_y >= 0 && screen_y < 480) {
                            visible = 1;
                            break;
                        }
                    }
                }
                if (visible) {
                    visible = 0;
                    for (index = 0; index < 4; ++index) {
                        if (r_u16(0x1F800000 + vertex[index] * 4) < 368) {
                            visible = 1;
                            break;
                        }
                    }
                }
                if (visible) {
                    for (index = 0; index < 4; ++index)
                        w_u32(packet + 8 + index * 12, r_u32(0x1F800000 + vertex[index] * 4));
                    descriptor = r_u32(0x8009E7F8);
                    layer = r_u8(0x8009E790 + cell_z * 10 + x);
                    descriptor += cell_z * 80 + 4 + x * 8;
                    texture = r_u32(descriptor + 4);
                    attributes = r_u32(descriptor);
                    uv = uv_table + (attributes >> 30) * 8;
                    attributes &= 0x3FFFFFFF;
                    for (index = 0; index < 4; ++index) {
                        cache_address = cache + vertex[index];
                        first = r_u8(cache_address);
                        if (first != 255) {
                            shade = first;
                        } else {
                            light_type = r_s16(0x8009E820);
                            point = vertices + vertex[index] * 4;
                            switch (light_type) {
                            case 1:
                                dx = first_x - (uint32)r_s16(point + 2);
                                dz = first_z - (uint32)r_s16(point);
                                denominator = dx * dx + dz * dz;
                                if (!denominator)
                                    denominator = 1;
                                dx = second_x - (uint32)r_s16(point + 2);
                                dz = second_z - (uint32)r_s16(point);
                                attenuation = t3_lighting_signed_divide(scale, denominator);
                                denominator = dx * dx + dz * dz;
                                if (!denominator)
                                    denominator = 1;
                                shade = attenuation + t3_lighting_signed_divide(scale, denominator);
                                if ((sint32)shade >= 255)
                                    shade = 254;
                                w_u8(cache_address, shade);
                                break;
                            case 2:
                                sum = r_u32(first_position) + r_u32(second_position);
                                dx = (uint32)((sint32)sum / 2) - (origin_x + (uint32)r_s16(point + 2));
                                sum = r_u32(first_position + 8) + r_u32(second_position + 8);
                                dz = (uint32)((sint32)sum / 2) - (origin_z + (uint32)r_s16(point));
                                denominator = dx * dx + dz * dz;
                                if (!denominator)
                                    denominator = 1;
                                attenuation = t3_lighting_signed_divide(scale, denominator);
                                shade = r_u32(0x800ADD94);
                                if ((sint32)shade >= (sint32)attenuation)
                                    shade = attenuation;
                                w_u8(cache_address, shade);
                                break;
                            case 3:
                                /* Keep the prior color when no third actor was acquired */
                                if (!has_third)
                                    break;
                                dx = r_u32(third_position) - (origin_x + (uint32)r_s16(point + 2));
                                dz = r_u32(third_position + 8) - (origin_z + (uint32)r_s16(point));
                                denominator = dx * dx + dz * dz;
                                if (!denominator)
                                    denominator = 1;
                                shade = t3_lighting_signed_divide(scale, denominator);
                                if ((sint32)shade >= 255)
                                    shade = 254;
                                w_u8(cache_address, shade);
                                break;
                            case 4:
                                dx = r_u32(0x800A85D4) - (origin_x + (uint32)r_s16(point + 2));
                                dz = (uint32)-2000 - (origin_z + (uint32)r_s16(point));
                                denominator = dx * dx + dz * dz;
                                if (!denominator)
                                    denominator = 1;
                                shade = t3_lighting_signed_divide(scale, denominator);
                                if ((sint32)shade >= 255)
                                    shade = 254;
                                w_u8(cache_address, shade);
                                break;
                            }
                        }
                        w_u8(packet + 6 + index * 12, shade);
                        w_u8(packet + 5 + index * 12, shade);
                        w_u8(packet + 4 + index * 12, shade);
                    }
                    flip = (attributes >> 16) & 0x600;
                    for (index = 0; index < 4; ++index) {
                        field = index ^ (flip >> 9);
                        first = (attributes & 0xFFFF) + r_u16(uv + field * 2);
                        if (index == 0)
                            first |= texture;
                        else if (index == 1)
                            first |= attributes & 0x39FF0000;
                        w_u32(packet + 12 + index * 12, first);
                    }
                    tag = ordering - layer * 4;
                    first = (r_u32(tag) & 0xFFFFFF) | 0x0C000000;
                    w_u32(packet, first);
                    first = r_u32(tag);
                    w_u32(tag, (first & 0xFF000000) | (packet & 0xFFFFFF));
                }
                divisor = r_u32(0x8009E824);
                ++x;
                packet += 52;
                if ((sint32)x >= (sint32)divisor)
                    x = 0;
                ++column;
            } while ((sint32)column < (sint32)divisor);
        }
        --cell_z;
        if ((sint32)cell_z < 0)
            cell_z = r_u32(0x8009E828) - 1;
        first = r_u32(0x8009E828);
        ++row;
    } while ((sint32)row < (sint32)first);
    return 0x800A0000;
}

uint32 t3_bns_02_800b2a80(uint32 stage, uint32 side)
{
    FUNCTION_MARKER(0x800B2A80, "SLUS_004.02-BNS-logical-02");
    return r_u8(0x800B06E8 + ((stage & 3) << 1) + (side & 1));
}

uint32 t3_bns_02_800b298c(uint32 index)
{
    FUNCTION_MARKER(0x800B298C, "SLUS_004.02-BNS-logical-02");
    return r_u32(0x800B65D0 + ((index & 1) << 2));
}

uint32 t3_bns_02_800b2fcc(uint32 side, uint32 character, uint32 variant)
{
    uint32 index;
    uint32 actor;
    uint32 row;
    uint32 flags;
    uint32 texture;
    uint32 palette;
    uint32 resource_character;
    uint32 resource_variant;
    uint32 descriptor;
    uint32 selected_y;
    uint32 other_y;
    uint32 y;

    FUNCTION_MARKER(0x800B2FCC, "SLUS_004.02-BNS-logical-02");
    side &= 1;
    selected_y = side ? 23 : 7;
    other_y = side ? 7 : 23;
    for (index = 0; index < 3; ++index) {
        if (!index) {
            flags = 0x40;
            resource_character = character;
            resource_variant = variant;
            y = selected_y;
            row = 0x800B0A18 + (variant << 2);
            texture = r_u16(row);
            palette = r_u16(row + 2);
            w_u16(0x800B6644, (uint16)palette);
        }
        else {
            actor = t3_bns_02_800b298c(index - 1);
            resource_character = 21;
            resource_variant = r_u16(actor + 0x14) & 3;
            y = other_y;
            flags = resource_variant << 12;
            if (side)
                flags &= 0xF000;
            flags |= 0x40;
            texture = r_u16(0x800B0A18);
            palette = r_u16(0x800B0A1A);
            w_u16(0x800B6644 + index * 2, (uint16)palette);
        }
        descriptor = t3_8004eff0(resource_character, resource_variant);
        w_u16(0x800B661C + index * 2, r_u8(descriptor + 5));
        w_u32(0x800B662C + index * 4, flags | (texture << 16));
        w_u16(0x800B663C + index * 2, (uint16)y);
    }
    return 0;
}

void t3_8006bdf4(uint8 status, uint8 *result)
{
    FUNCTION_MARKER(0x8006BDF4, "SLUS_004.02");
    (void)status;
    (void)result;
}

// Adapt extracted XAS resources to the shared fullscreen player
sint32 t3_fmv_play_xas(uint32 resource_index, uint32 final_frame, sint32 repeat, const XportFmvPresentation *presentation)
{
    static const char *const paths[] = {
        "DATA/TEKKEN3/MOVIE/XAS_001.STR",
        "DATA/TEKKEN3/MOVIE/XAS_002.STR",
        "DATA/TEKKEN3/MOVIE/XAS_003.STR",
        "DATA/TEKKEN3/MOVIE/XAS_004.STR",
        "DATA/TEKKEN3/MOVIE/XAS_005.STR",
        "DATA/TEKKEN3/MOVIE/XAS_006.STR",
        "DATA/TEKKEN3/MOVIE/XAS_007.STR",
        "DATA/TEKKEN3/MOVIE/XAS_008.STR",
        "DATA/TEKKEN3/MOVIE/XAS_009.STR",
        "DATA/TEKKEN3/MOVIE/XAS_010.STR",
        "DATA/TEKKEN3/MOVIE/XAS_011.STR",
        "DATA/TEKKEN3/MOVIE/XAS_012.STR",
        "DATA/TEKKEN3/MOVIE/XAS_013.STR",
        "DATA/TEKKEN3/MOVIE/XAS_014.STR",
        "DATA/TEKKEN3/MOVIE/XAS_015.STR",
        "DATA/TEKKEN3/MOVIE/XAS_016.STR",
        "DATA/TEKKEN3/MOVIE/XAS_017.STR",
        "DATA/TEKKEN3/MOVIE/XAS_018.STR",
        "DATA/TEKKEN3/MOVIE/XAS_019.STR",
        "DATA/TEKKEN3/MOVIE/XAS_020.STR",
        "DATA/TEKKEN3/MOVIE/XAS_021.STR",
        "DATA/TEKKEN3/MOVIE/XAS_022.STR"
    };
    sint32 result;
    uint32 vsync_callback;

    if (resource_index < 1 || resource_index > sizeof(paths) / sizeof(paths[0]))
        return -1;
    t3_file_audio_stop();
    vsync_callback = VSyncCallbackPSX(0);
    result = xport_fmv_play_presented(paths[resource_index - 1], final_frame, repeat, presentation);
    VSyncCallbackPSX(vsync_callback);
    if (result == 1)
        return 1;
    if (result == 2)
        return -2;
    return -1;
}

uint32 t3_8006ae14(uint32 mode, uint32 value)
{
    uint32 selector;
    uint32 entry;
    uint32 stream;
    uint32 row;
    uint32 start;
    sint32 channel;
    sint32 resumed = 0;
    const char *path;

    FUNCTION_MARKER(0x8006AE14, "SLUS_004.02");
    if (t3_8006c23c()) {
        if (value)
            return 0x80090000;
        if (!t3_bns_pump()) {
            xport_message_error("Tekken 3", "Cannot complete BNS resource read");
            return 0xFFFFFFFF;
        }
    }
    selector = r_u8(0x80097F05);
    if (selector >= 2) {
        selector = 0;
        if ((uint32)(r_u16(0x800AE204) - 8) < 10)
            selector = 0xFFFFFFFF;
    }
    if ((sint32)selector < 0)
        return selector;
    entry = 0x80024FD0 + mode * 4 + selector * 2;
    w_u16(0x800A0622, value ? mode : 0xFFFF);
    w_u16(0x800A05DE, 600);
    stream = r_u8(entry);
    if (mode == (uint32)r_s16(0x800A0620))
        return t3_8006bc98(127, -1);
    t3_file_audio_stop();
    t3_8006bc98(0, -1);
    if (!(stream & 0x80)) {
        row = 0x80024D78 + stream * 12;
        channel = r_s16(row + 8);
        start = r_u32(row);
        w_u32(0x800A0610, start);
        w_u32(0x800A0614, r_u32(row + 4));
        w_u16(0x800A061E, r_u8(entry + 1) & 0x80);
        w_u16(0x800A068A, r_u8(entry + 1) & 0x7F);
        if (r_u32(0x800A0630) == mode && (sint32)mode > 0) {
            uint32 saved_position = r_u32(0x800A062C);
            if ((sint32)saved_position > 0) {
                start = saved_position;
                resumed = 1;
                w_u32(0x800A0630, 0xFFFFFFFF);
                w_u32(0x800A062C, 0xFFFFFFFF);
            }
        }
        if (value) {
            w_u16(0x800A061C, 0);
            t3_8006bc98(0, -1);
        } else {
            w_u16(0x800A0620, mode);
            w_u16(0x800A061C, 0);
            t3_8006bc98(127, resumed ? 45 : -1);
            if (!t3_file_audio_play("DATA/TEKKEN3/TEKKEN3.XAS", start, r_u32(0x800A0614), channel,
                    r_s16(0x800A061E) ? (sint32)r_u32(0x800A0610) : -1)) {
                t3_8006b54c(0);
                xport_message_error("Tekken 3", "Cannot play XA audio from DATA/TEKKEN3/TEKKEN3.XAS");
                return 0xFFFFFFFF;
            }
            w_u16(0x800A05D8, 2);
            w_u32(0x800A866C, 0);
        }
        w_u32(0x800A0618, 0);
    } else {
        w_u16(0x800A061E, r_u8(entry + 1) & 0x80);
        w_u16(0x800A068A, r_u8(entry + 1) & 0x7F);
        if (!value) {
            t3_8006b54c(0);
            stream &= 0x7F;
            w_u32(0x800A0634, (uint32)r_s16(0x80025050 + stream * 2));
            path = stream == 2 ? "DATA/TEKKEN3/TEKKEN3.DA" :
                stream == 3 ? "DATA/TEKKEN3/TEKKEN3.DMY" : NULL;
            if (!t3_file_audio_play(path, 0, 0, -1, -1)) {
                t3_8006b54c(0);
                xport_message_error("Tekken 3", "Cannot play PCM audio from DATA/TEKKEN3");
                return 0xFFFFFFFF;
            }
            w_u16(0x800A0620, mode);
            w_u16(0x800A061C, 2);
            t3_8006bc98(127, -1);
            w_u16(0x800A05D8, 4);
            w_u32(0x800A866C, 0);
        }
    }
    w_u32(0x800A062C, 0xFFFFFFFF);
    w_u32(0x800A0630, 0xFFFFFFFF);
    return 0x800A05D8;
}

void t3_8006b6fc(void)
{
    uint32 position;
    sint32 status;
    sint32 state;

    FUNCTION_MARKER(0x8006B6FC, "SLUS_004.02");
    state = (sint16)(r_u16(0x800A05D8) - 1);
    switch (state) {
    case 0:
    case 7:
    case 8:
    case 9:
        return;
    case 1:
        t3_8006ba44();
        status = t3_file_audio_status(&position);
        w_u32(0x800A0618, position);
        if (status != 1) {
            if (status < 0)
                xport_message_error("Tekken 3", "Error reading XA audio file");
            t3_8006b54c(0);
            return;
        }
        if (r_s32(0x800A0618) < r_s32(0x800A0614))
            return;
        if (!r_s16(0x800A061E))
            t3_8006b54c(0);
        else
            w_u32(0x800A0618, r_u32(0x800A0610));
        return;
    case 2:
        t3_8006ba44();
        if (!r_s16(0x800A068C))
            t3_8006b54c(0);
        return;
    case 3:
        status = t3_file_audio_status(&position);
        w_u32(0x800A0618, position);
        if (status != 1) {
            if (status < 0)
                xport_message_error("Tekken 3", "Error reading PCM audio file");
            t3_8006b54c(0);
        }
        t3_8006ba44();
        return;
    case 4:
    case 5:
        t3_8006ba44();
        return;
    case 6:
        status = t3_file_audio_status(&position);
        if (status != 0)
            w_u32(0x800A0618, position);
        if (status != 1 || r_s32(0x800A0614) < r_s32(0x800A0618))
            t3_8006b54c(0);
        return;
    default:
        w_u16(0x800A05D8, 1);
        return;
    }
}

uint32 t3_bns_02_800b292c(void)
{
    FUNCTION_MARKER(0x800B292C, "SLUS_004.02-BNS-logical-02");
    if (!r_u8(0x800A92ED)) {
        w_u32(0x800B65C8, 0x800A9228);
        w_u32(0x800B65D0, 0x800AAAB4);
    } else {
        w_u32(0x800B65C8, 0x800AAAB4);
        w_u32(0x800B65D0, 0x800A9228);
    }
    w_u32(0x800B65D4, 0x800AC340);
    return 0x800AC340;
}

uint32 t3_bns_02_800b2ab4(void)
{
    FUNCTION_MARKER(0x800B2AB4, "SLUS_004.02-BNS-logical-02");
    if (r_u16(0x800ADC84) == 18) {
        w_u32(0x800B6604, (uint32)-375);
        w_u32(0x800B6608, (uint32)-2445);
        w_u32(0x800B660C, 333);
        w_u32(0x800B6610, 2175);
    } else {
        w_u32(0x800B6604, (uint32)-323);
        w_u32(0x800B6608, (uint32)-2553);
        w_u32(0x800B660C, 329);
        w_u32(0x800B6610, 1971);
    }
    w_u32(0x800B6614, (uint32)-259);
    w_u32(0x800B6618, (uint32)-2285);
    return 0xFFFFF713;
}

uint32 t3_bns_02_800b4584(void)
{
    FUNCTION_MARKER(0x800B4584, "SLUS_004.02-BNS-logical-02");
    return r_u32(0x800ADD0C);
}

uint32 t3_bns_02_800b4efc(uint32 mode)
{
    uint32 center;
    FUNCTION_MARKER(0x800B4EFC, "SLUS_004.02-BNS-logical-02");
    if (!mode)
        w_u32(0x800B6BE0, 1);
    else if (mode == 2) {
        center = r_u32(0x800B6BB8);
        w_u32(0x800B6BDC, (uint32)-100);
        w_u32(0x800B6BE0, 60);
        w_u32(0x800B6BE4, center - 5000);
        w_u32(0x800B6BE8, center + 5000);
    }
    w_u32(0x800B6BD0, mode);
    return 0x800B0000;
}

uint32 t3_bns_03_800b57e0(void)
{
    FUNCTION_MARKER(0x800B57E0, "SLUS_004.02-BNS-logical-03");
    return r_u32(0x800ADD0C);
}

uint32 t3_bns_02_800b1108(void)
{
    uint32 index;
    uint32 record;
    uint32 actor;
    uint32 resource;
    uint32 setting;

    FUNCTION_MARKER(0x800B1108, "SLUS_004.02-BNS-logical-02");
    t3_bns_02_800b292c();
    for (index = 0; index < 2; ++index) {
        record = 0x800B6598 + index * 24;
        w_u16(record + 8, 0);
        w_u16(record + 10, 0);
        w_u16(record + 12, 0);
        actor = t3_bns_02_800b298c(index);
        w_u32(record, actor);
        w_u16(record + 14, 0);
        w_u16(record + 16, index);
        w_u8(actor + 0xC3, 0);
        w_u32(actor + 0x3F4, 0);
        w_u8(actor + 0xC2, 1);
    }
    w_u32(0x800B6BF0, 0);
    w_u32(0x800B6BF8, 0);
    w_u32(0x800B658C, 0);
    t3_bns_02_800b4efc(0);
    w_u32(0x800B6C04, 0);
    resource = r_u32(0x800ADD74);
    setting = r_u32(0x800AFAAC);
    w_u32(0x800B6C20, setting);
    w_u32(0x800B65D8, resource);
    w_u32(0x800B6C18, 0);
    w_u32(0x800B6C14, 0);
    w_u32(0x800B6BFC, 0);
    w_u32(0x800B65DC, 0);
    w_u32(0x800B65E0, 0);
    w_u32(0x800B65E4, 0);
    w_u32(0x800B65E8, 0);
    w_u32(0x800B65EC, 0);
    w_u32(0x800B65F0, 0);
    w_u32(0x800B65F4, 0);
    w_u32(0x800B65F8, 0);
    w_u32(0x800B6600, 0);
    t3_bns_02_800b3488();
    t3_bns_02_800b3cb4();
    t3_bns_02_800b425c();
    t3_bns_02_800b2ab4();
    setting = r_u32(0x800AFAAC);
    w_u32(0x800B6C10, 0);
    if (setting >= 4) {
        w_u32(0x800B6C10, 1);
        w_u32(0x800B6BF8, 8);
        w_u32(0x800ADBCC, 3600);
        return 0;
    }
    return r_u32(0x800ADD74);
}

uint32 t3_bns_02_800b3488(void)
{
    PSX_RECT rect;
    uint32 palette[8];
    uint32 buffer;
    uint32 index;
    uint32 color;
    uint32 packet;
    uint32 x;
    uint32 source;

    FUNCTION_MARKER(0x800B3488, "SLUS_004.02-BNS-logical-02");
    rect.x = 0;
    rect.y = 486;
    rect.w = 16;
    rect.h = 1;
    for (buffer = 0; buffer < 4; ++buffer) {
        source = 0x800B6518 + buffer * 4;
        for (index = 0; index < 16; ++index) {
            color = r_u8(source) >> 3;
            color += (r_u8(source + 1) >> 3) << 5;
            color += (r_u8(source + 2) >> 3) << 10;
            xport_store_le16((uint16 *)palette + index, (uint16)(color | 0x8000));
        }
        LoadImagePSX(&rect, palette);
        DrawSync(0);
        rect.x += 16;
    }
    for (buffer = 0; buffer < 2; ++buffer) {
        x = 64;
        for (index = 0; index < 4; ++index) {
            packet = 0x800B664C + buffer * 200 + index * 40;
            setPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
            color = r_s32(0x800B6C20) < (sint32)index ? 0 : 128;
            w_u8(packet + 4, color);
            w_u8(packet + 5, color);
            w_u8(packet + 6, color);
            w_u16(packet + 22, index == r_u32(0x800B6C20) ? 40 : 8);
            w_u16(packet + 14, (index & 63) | 0x7980);
            w_u8(packet + 12, 0);
            w_u8(packet + 13, 0);
            w_u8(packet + 20, 1);
            w_u8(packet + 21, 0);
            w_u8(packet + 28, 0);
            w_u8(packet + 29, 1);
            w_u8(packet + 36, 1);
            w_u8(packet + 37, 1);
            setSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
            w_u16(packet + 8, x);
            w_u16(packet + 24, x);
            x += 60;
            w_u16(packet + 10, 436);
            w_u16(packet + 16, x);
            w_u16(packet + 18, 436);
            w_u16(packet + 26, 452);
            w_u16(packet + 32, x);
            w_u16(packet + 34, 452);
        }
        packet = 0x800B664C + buffer * 200 + 160;
        setPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u16(packet + 14, 0x7983);
        w_u8(packet + 12, 0);
        w_u8(packet + 13, 0);
        w_u8(packet + 20, 1);
        w_u8(packet + 21, 0);
        w_u8(packet + 28, 0);
        w_u8(packet + 29, 1);
        w_u8(packet + 36, 1);
        w_u8(packet + 37, 1);
        setSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
        if (r_s32(0x800B6C20) < 4) {
            w_u16(packet + 22, 8);
            w_u8(packet + 4, 0);
            w_u8(packet + 5, 0);
            w_u8(packet + 6, 0);
        } else {
            w_u16(packet + 22, 40);
            w_u8(packet + 4, 128);
            w_u8(packet + 5, 128);
            w_u8(packet + 6, 128);
            w_u16(packet + 8, x);
            w_u16(packet + 10, 436);
            w_u16(packet + 16, x + 12);
            w_u16(packet + 18, 436);
            w_u16(packet + 24, x);
            w_u16(packet + 26, 452);
            w_u16(packet + 32, x + 12);
            w_u16(packet + 34, 452);
        }
        for (index = 0; index < 5; ++index) {
            packet = 0x800B683C + buffer * 100 + index * 20;
            setLineG2((LINE_G2 *)psx_addr(packet, 20));
            w_u16(packet + 8, 63 + index * 60);
            w_u16(packet + 16, 63 + index * 60);
            w_u16(packet + 10, 436);
            w_u8(packet + 4, 255);
            w_u8(packet + 5, 255);
            w_u8(packet + 6, 255);
            w_u8(packet + 12, 128);
            w_u8(packet + 13, 128);
            w_u8(packet + 14, 128);
            w_u16(packet + 18, 452);
        }
        packet = 0x800B67DC + buffer * 16;
        setLineF2((LINE_F2 *)psx_addr(packet, 16));
        w_u8(packet + 4, 255);
        w_u8(packet + 5, 255);
        w_u8(packet + 6, 255);
        packet = 0x800B67FC + buffer * 28;
        setPolyG3((POLY_G3 *)psx_addr(packet, 28));
        w_u8(packet + 4, 0);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 255);
        w_u8(packet + 12, 0);
        w_u8(packet + 13, 128);
        w_u8(packet + 14, 255);
        w_u8(packet + 20, 255);
        w_u8(packet + 21, 255);
        w_u8(packet + 22, 255);
    }
    w_u32(0x800B6904, 0);
    w_u32(0x800B6908, 0);
    return 0x800B0000;
}

uint32 t3_bns_02_800b3cb4(void)
{
    uint32 buffer;
    uint32 index;
    uint32 base;
    uint32 packet;

    FUNCTION_MARKER(0x800B3CB4, "SLUS_004.02-BNS-logical-02");
    for (buffer = 0; buffer < 2; ++buffer) {
        base = 0x800B690C + buffer * 188;
        for (index = 0; index < 2; ++index) {
            packet = base + index * 40;
            setPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
            w_u16(packet + 14, 0x7801);
            w_u16(packet + 22, 8);
            w_u8(packet + 4, 128);
            w_u8(packet + 5, 128);
            w_u8(packet + 6, 128);
            w_u8(packet + 12, 0);
            w_u8(packet + 13, 64);
            w_u8(packet + 20, 63);
            w_u8(packet + 21, 64);
            w_u8(packet + 28, 0);
            w_u8(packet + 29, 191);
            w_u8(packet + 36, 63);
            w_u8(packet + 37, 191);
            setSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 0);
            setShadeTex((POLY_FT4 *)psx_addr(packet, 40), 0);
            packet = base + 80 + index * 40;
            setPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
            w_u8(packet + 4, 32);
            w_u8(packet + 5, 32);
            w_u8(packet + 6, 32);
            w_u16(packet + 22, 8);
            w_u16(packet + 14, 0x7800);
            w_u8(packet + 12, 0);
            w_u8(packet + 13, 0);
            w_u8(packet + 20, 63);
            w_u8(packet + 21, 0);
            w_u8(packet + 28, 0);
            w_u8(packet + 29, 63);
            w_u8(packet + 36, 63);
            w_u8(packet + 37, 63);
            setSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
            setShadeTex((POLY_FT4 *)psx_addr(packet, 40), 0);
        }
        w_u32(base + 160, 0);
        w_u32(base + 164, 0);
        w_u32(base + 168, 1056);
        w_u32(base + 172, 0);
        w_u32(base + 184, 0);
    }
    return 0;
}

uint32 t3_bns_02_800b425c(void)
{
    uint32 buffer;
    uint32 index;
    uint32 packet;
    uint32 image_y;
    uint32 palette_x;

    FUNCTION_MARKER(0x800B425C, "SLUS_004.02-BNS-logical-02");
    for (buffer = 0; buffer < 2; ++buffer) {
        packet = 0x800B6A8C + buffer * 8;
        w_u8(packet + 3, 1);
        w_u32(packet + 4, 0xE100021A);
        for (index = 0; index < 3; ++index) {
            packet = 0x800B6A9C + buffer * 60 + index * 20;
            setSprt((SPRT *)psx_addr(packet, 20));
            w_u16(packet + 16, 20);
            w_u16(packet + 18, 36);
            w_u8(packet + 4, 128);
            w_u8(packet + 5, 128);
            w_u8(packet + 6, 128);
            setShadeTex((SPRT *)psx_addr(packet, 20), 0);
            image_y = 320 + index * 36;
            palette_x = 128 + index * 16;
            w_u16(packet + 14, (486 << 6) | ((palette_x >> 4) & 63));
            w_u16(packet + 8, index * 20);
            w_u8(packet + 13, image_y % 256);
            w_u8(packet + 12, (688 % 64) * 4);
            w_u16(packet + 10, 420);
        }
    }
    return 0;
}

uint32 t3_bns_02_800b4660(void)
{
    uint32 configuration;
    uint32 stage;
    uint32 data;
    uint32 width;
    uint32 height;
    uint32 buffer;
    uint32 row;
    uint32 index;
    uint32 base;
    uint32 packet;

    FUNCTION_MARKER(0x800B4660, "SLUS_004.02-BNS-logical-02");
    configuration = r_u32(0x800ADD0C);
    stage = r_u16(0x800ADC84) - 15;
    w_u32(0x800B6BB0, stage);
    data = configuration + 4;
    width = r_u16(configuration);
    w_u32(0x800B6B9C, data);
    w_u32(0x800B6BA0, data);
    w_u32(0x800B6B94, width);
    height = r_u16(configuration + 2);
    w_u32(0x800B6B98, height);
    width = r_u16(configuration);
    height = r_u16(configuration + 2);
    w_u32(0x800B6BA4, 20);
    w_u32(0x800B6BA0, data + ((width * height) << 1));
    if (stage == 3) {
        w_u32(0x800B6BAC, 216);
        w_u32(0x800B6BA8, 1022);
    } else {
        w_u32(0x800B6BAC, 0);
        w_u32(0x800B6BA8, 1008);
    }
    if (r_u32(0x800B6C10))
        w_u32(0x800B6BA8, 1008);
    for (buffer = 0; buffer < 2; ++buffer) {
        base = r_u32(0x800ADD58);
        packet = base ? base + buffer * 840 : 0;
        if (r_s32(0x800B6B98) > 0) {
            row = 0;
            do {
                for (index = 0; index < 7; ++index) {
                    w_u8(packet + 4, 128);
                    w_u8(packet + 5, 128);
                    w_u8(packet + 6, 128);
                    w_u16(packet + 16, 64);
                    w_u16(packet + 18, 64);
                    setShadeTex((SPRT *)psx_addr(packet, 20), 1);
                    setSprt((SPRT *)psx_addr(packet, 20));
                    packet += 20;
                }
                ++row;
            } while ((sint32)row < r_s32(0x800B6B98));
        }
        if (r_u32(0x800B6BB0) == 3 || r_u32(0x800B6C10)) {
            packet = 0x800B6B14 + buffer * 16;
            if (!r_u32(0x800B6C10)) {
                w_u16(packet + 12, 272);
                w_u16(packet + 14, 100);
            }
            w_u16(packet + 8, 0);
            w_u16(packet + 10, 20);
            w_u8(packet + 4, 0);
            w_u8(packet + 5, 0);
            w_u8(packet + 6, 0);
            setTile((TILE *)psx_addr(packet, 16));
            for (index = 0; index < 3; ++index) {
                packet = 0x800B6B34 + buffer * 48 + index * 16;
                w_u8(packet + 4, 0);
                w_u8(packet + 5, 0);
                w_u8(packet + 6, 0);
                w_u16(packet + 12, 64);
                w_u16(packet + 14, 32);
                setTile((TILE *)psx_addr(packet, 16));
            }
        }
    }
    w_u32(0x800B6C0C, 0);
    return 0x800B0000;
}

uint32 t3_bns_02_800b45a4(uint32 archive)
{
    sint32 count;
    uint32 index;
    uint32 resource;
    T3_TIM_IMAGE image;

    FUNCTION_MARKER(0x800B45A4, "SLUS_004.02-BNS-logical-02");
    count = r_s32(archive);
    t3_80029638(1);
    index = 0;
    if (count > 0) {
        do {
            resource = t3_80077590(archive, (sint32)index);
            if (t3_8007efc8(resource) == 0xFFFFFFFF)
                break;
            t3_8007efd8(&image);
            ++index;
            t3_8007c650(image.image_rectangle, image.image_pixels);
            DrawSync(0);
            t3_8007c650(image.clut_rectangle, image.clut_pixels);
            DrawSync(0);
        } while ((sint32)index < count);
    }
    return t3_80029638(0);
}

uint32 t3_bns_03_800b5800(uint32 stage, uint32 configuration, uint32 archive)
{
    uint32 data;
    uint32 width;
    uint32 height;
    sint32 count;
    uint32 index;
    uint32 cursor;
    uint32 buffer;
    uint32 row;
    uint32 base;
    uint32 packet;
    uint32 value;
    T3_TIM_IMAGE image;

    FUNCTION_MARKER(0x800B5800, "SLUS_004.02-BNS-logical-03");
    data = configuration + 4;
    width = r_u16(configuration);
    w_u32(0x800B6668, data);
    w_u32(0x800B666C, data);
    w_u32(0x800B6660, width);
    height = r_u16(configuration + 2);
    w_u32(0x800B6664, height);
    width = r_u16(configuration);
    height = r_u16(configuration + 2);
    w_u32(0x800B6670, 96);
    w_u32(0x800B666C, data + ((width * height) << 1));
    count = r_s32(archive);
    t3_80029638(1);
    index = 0;
    if (count > 0) {
        cursor = archive;
        do {
            value = r_u32(cursor + 4);
            if (t3_8007efc8(archive + value) == 0xFFFFFFFF)
                break;
            t3_8007efd8(&image);
            cursor += 8;
            t3_8007c650(image.image_rectangle, image.image_pixels);
            DrawSync(0);
            ++index;
            t3_8007c650(image.clut_rectangle, image.clut_pixels);
            DrawSync(0);
        } while ((sint32)index < count);
    }
    t3_80029638(0);
    for (buffer = 0; buffer < 2; ++buffer) {
        base = r_u32(0x800ADD58);
        packet = base ? base + buffer * 2400 : 0;
        if (r_s32(0x800B6664) > 0) {
            row = 0;
            do {
                for (index = 0; index < 15; ++index) {
                    w_u8(packet + 4, 128);
                    w_u8(packet + 5, 128);
                    w_u8(packet + 6, 128);
                    w_u16(packet + 16, 32);
                    w_u16(packet + 18, 32);
                    setShadeTex((SPRT *)psx_addr(packet, 20), 1);
                    setSprt((SPRT *)psx_addr(packet, 20));
                    packet += 20;
                }
                ++row;
            } while ((sint32)row < r_s32(0x800B6664));
        }
        packet = 0x800B6640 + buffer * 16;
        w_u8(packet + 4, 33);
        w_u8(packet + 5, 58);
        w_u8(packet + 6, 148);
        value = r_u16(0x800AE230);
        w_u16(packet + 8, value);
        value = r_u16(0x800AE232);
        w_u16(packet + 10, value);
        setTile((TILE *)psx_addr(packet, 16));
    }
    if (r_u8(0x800AFAA0))
        return t3_800482b0(stage);
    return 0;
}

uint32 t3_bns_08_800ff964(uint32 source, uint32 destination, sint32 height, sint32 mode)
{
    uint32 palette;
    uint32 row;
    uint32 group;
    uint32 pixel;
    uint32 channel;
    uint32 packed;
    uint32 indices[4];
    uint32 color;
    uint32 target;
    uint32 value;

    FUNCTION_MARKER(0x800FF964, "SLUS_004.02-BNS-logical-08");
    switch (mode) {
    case 0: palette = 0x800B9804; break;
    case 2: palette = 0x800B9884; break;
    case 3: palette = 0x800B9904; break;
    default:
        // The immutable presentation table supplies only modes 0, 2 and 3
        abort();
    }
    if (height <= 0)
        return 0x800C0000;
    row = 0;
    do {
        for (group = 0; group < 4; ++group) {
            packed = r_u16(source);
            indices[3] = (packed >> 9) & 0x78;
            indices[2] = (packed >> 5) & 0x78;
            indices[1] = (packed >> 1) & 0x78;
            indices[0] = (packed << 3) & 0x78;
            source += 2;
            for (pixel = 0; pixel < 4; ++pixel) {
                if (!indices[pixel])
                    continue;
                color = palette + indices[pixel];
                target = destination + pixel * 3;
                if (!r_u8(color + 3)) {
                    if (!(pixel & 1)) {
                        value = r_u16(color);
                        w_u16(target, value);
                        value = r_u8(color + 2);
                        w_u8(target + 2, value);
                    } else {
                        for (channel = 0; channel < 3; ++channel) {
                            value = r_u8(color + channel);
                            w_u8(target + channel, value);
                        }
                    }
                } else {
                    for (channel = 0; channel < 3; ++channel) {
                        value = r_u8(target + channel);
                        value = ((value >> 1) + r_u8(color + channel)) >> 1;
                        w_u8(target + channel, value);
                    }
                }
            }
            destination += 12;
        }
        ++row;
        source += 120;
    } while ((sint32)row < height);
    return 0;
}

uint32 t3_bns_08_800ffc24(uint32 strip, uint32 destination)
{
    uint32 position;
    uint32 origin;
    uint32 edge;
    uint32 end;
    uint32 offset;
    uint32 bank;
    sint32 height;
    uint32 line;
    sint32 mode;

    FUNCTION_MARKER(0x800FFC24, "SLUS_004.02-BNS-logical-08");
    if (r_s16(0x80184CC0) < 0)
        return 0x80180000;
    position = (uint32)((sint32)(strip << 1) / 3);
    origin = (uint32)r_s16(0x80184CC8);
    edge = (uint32)r_s16(0x80184CCC);
    position -= origin;
    if ((sint32)position < (sint32)edge)
        return 1;
    end = edge + (uint32)r_s16(0x80184CCE);
    if ((sint32)position >= (sint32)end)
        return 0;
    offset = r_u32(0x80184CC4);
    bank = r_u32(0x8010E1E0);
    height = r_s16(0x80184CD0);
    bank += (offset >> 1) << 1;
    bank += (position >> 2) << 1;
    line = (uint32)r_s16(0x80184CCA);
    mode = r_s16(0x80184CD4);
    return t3_bns_08_800ff964(bank, destination + line * 48, height, mode);
}

uint32 t3_bns_08_800ff7a8(void)
{
    FUNCTION_MARKER(0x800FF7A8, "SLUS_004.02-BNS-logical-08");
    w_u16(0x80184CC0, 0xFFFF);
    w_u16(0x80184CC2, 0xFFFF);
    return 0x80184CC0;
}

uint32 t3_bns_08_800ff7c0(sint32 index)
{
    uint32 source;
    uint32 offset;
    uint32 fields[7];
    uint32 field;

    FUNCTION_MARKER(0x800FF7C0, "SLUS_004.02-BNS-logical-08");
    w_u16(0x80184CC0, index);
    if (index < 0)
        return 0x80180000;
    source = 0x800B96D8 + (uint32)index * 20;
    offset = r_u32(source);
    for (field = 0; field < 7; ++field)
        fields[field] = r_u16(source + 4 + field * 2);
    w_u32(0x80184CC4, offset);
    for (field = 0; field < 7; ++field)
        w_u16(0x80184CC8 + field * 2, fields[field]);
    return fields[6];
}

uint32 t3_bns_08_800ff830(sint32 ordinal)
{
    uint32 index;
    uint32 cursor;
    uint32 remaining;

    FUNCTION_MARKER(0x800FF830, "SLUS_004.02-BNS-logical-08");
    if (ordinal < 0) {
        w_u16(0x80184CC2, 0xFFFF);
        w_u16(0x80184CC0, 0xFFFF);
        return 0xFFFFFFFF;
    }
    index = 0;
    cursor = 0x800B978C;
    remaining = (uint32)ordinal;
    do {
        if (!r_u16(cursor)) {
            --remaining;
            if ((sint32)remaining < 0)
                break;
        }
        ++index;
        cursor += 4;
    } while (index < 30);
    w_u16(0x80184CC2, index);
    w_u16(0x80184CC0, index);
    if ((sint32)remaining >= 0)
        t3_bns_08_800ff7c0(r_s16(0x800B978E + index * 4));
    w_u16(0x80184CC0, 0xFFFF);
    return 0xFFFFFFFF;
}
