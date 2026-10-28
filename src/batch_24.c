#include "game_functions.h"
#include <string.h>

uint32 t3_80035e5c(uint32 side, uint32 resource)
{
    uint32 destination;
    uint32 source;
    uint32 value;
    FUNCTION_MARKER(0x80035E5C, "SLUS_004.02");
    side = (uint32)((sint32)side % 3);
    resource = (uint32)((sint32)resource % 3);
    destination = 0x800ADEC8 + (side << 2);
    source = 0x8009BD28 + (resource << 2);
    value = r_u32(source);
    w_u32(destination, value);
    return value;
}

uint32 t3_bns_03_800b2c50(uint32 angles, const VECTOR *vector)
{
    uint32 x;
    uint32 other;
    uint32 value;
    FUNCTION_MARKER(0x800B2C50, "SLUS_004.02-BNS-logical-03");
    w_u16(angles, 0);
    x = (uint32)vector->vx;
    other = (uint32)vector->vz;
    value = t3_8004b39c(x, other);
    w_u16(angles + 2, value);
    x = (uint32)vector->vx;
    other = (uint32)vector->vy;
    if ((sint32)x > 0) {
        value = t3_8004b39c(x, other);
    } else {
        value = 0 - t3_8004b39c(x, other);
    }
    w_u16(angles + 4, value);
    return value;
}

uint32 t3_bns_03_800b2a4c(uint32 owner)
{
    uint32 decay;
    uint32 value;
    uint32 other;
    uint32 selected;
    sint32 state;
    sint32 amount;
    sint32 velocity;
    FUNCTION_MARKER(0x800B2A4C, "SLUS_004.02-BNS-logical-03");
    decay = r_s16(owner + 0xB2) == 0;
    if (!decay) {
        if (r_s16(owner + 0x150) < 128) {
            w_u8(owner + 0xCD, 0);
            t3_bns_03_800b2c50(owner + 0x148, (const VECTOR *)psx_addr(owner + 0x78, 12));
            amount = r_s16(owner + 0x150) * 2;
            velocity = r_s32(owner + 0xA8);
            value = r_u16(owner + 0x14C);
            w_u16(owner + 0x14C, value + 1024);
            value = (uint32)(amount / 3) + (uint32)((velocity >> 6) / 3);
            w_u16(owner + 0x150, value);
        } else {
            decay = r_u8(owner + 0xCD) != 0;
        }
    }
    if (decay) {
        amount = r_s16(owner + 0x150);
        value = r_u16(owner + 0x150);
        value -= (uint32)((amount + 16) >> 4);
        w_u16(owner + 0x150, value);
    }
    state = r_s16(owner);
    if (state == 6) {
        w_u8(owner + 0x146, 1);
        return 1;
    }
    if (state == 1 || state == 8) {
        w_u8(owner + 0x146, 0);
        return (uint32)state;
    }
    if (state == 7) {
        w_u8(owner + 0x146, 2);
        return 2;
    }
    if (r_s16(owner + 0x140) == 0) {
        if (r_u8(owner + 0xCC)) {
            w_u8(owner + 0x146, 5);
            return 5;
        }
        state = r_s16(owner);
        w_u8(owner + 0x146, state == 3 ? 4 : 3);
        return 3;
    }
    value = r_u32(owner + 0x130);
    selected = r_u8(value + 0x1E);
    amount = r_s16(owner + 0x140);
    w_u8(owner + 0x147, selected);
    if (amount < 100) {
        if (r_u8(owner + 0xCC) == 5)
            t3_bns_03_800b425c(r_u8(owner + 0x147), 0);
        value = r_u8(owner + 0x147);
        w_u8(owner + 0x146, value ? 7 : 6);
    } else {
        if (r_u8(owner + 0xCC) == 5)
            t3_bns_03_800b425c(r_u8(owner + 0x147), 1);
        w_u8(owner + 0x146, 9);
    }
    value = r_u8(owner + 0xCC);
    if (!value)
        return 0;
    other = r_u8(owner + 0x145);
    value = other ? 8 : 5;
    w_u8(owner + 0x146, value);
    return value;
}

uint32 t3_bns_03_800b3de0(uint32 input, uint8 *output)
{
    uint32 packed;
    uint32 result;
    uint32 shift;
    FUNCTION_MARKER(0x800B3DE0, "SLUS_004.02-BNS-logical-03");
    packed = t3_8004cb8c(input);
    result = t3_8004ccd8(packed);
    result = result ? result - 1 : 0;
    shift = result << 2;
    while ((sint32)shift >= 0) {
        result = ((uint32)((sint32)packed >> (shift & 31)) & 15) + 48;
        *output = (uint8)result;
        shift -= 4;
        ++output;
    }
    *output = 0;
    return result;
}

uint32 t3_bns_03_800b38dc(uint32 first, uint32 second, uint32 owner)
{
    SVECTOR angles;
    uint32 value;
    uint32 bank;
    uint32 base;
    uint32 packet;
    uint32 paired;
    uint32 xy;
    uint32 paired_xy;
    uint32 screen;
    uint32 ordering_owner;
    uint32 ordering;
    uint32 tag;
    uint32 link;
    uint32 packet_link;
    uint32 index;
    FUNCTION_MARKER(0x800B38DC, "SLUS_004.02-BNS-logical-03");
    value = r_u32(0x800ADCA4);
    if (value)
        return value;
    value = first + second;
    w_u32(0x800B660C, (uint32)((sint32)value >> 1));
    angles.vz = 0;
    angles.vy = 0;
    angles.vx = 0;
    t3_8003a490(&angles, (MATRIX *)psx_addr(0x800B65F0, 32));
    t3_8003785c(0x800ADF70, 0x800B65F0);
    t3_80036af8(0x800B6480, (uint32 *)psx_addr(0x1F800000, 72),
        (uint16 *)psx_addr(0x1F800100, 36), 18);
    screen = 0x1F800044;
    bank = r_u32(0x800ADEFC);
    base = owner + bank * 288 + 0x120C;
    packet = base + 0xFC;
    xy = base + 0x104;
    for (index = 0; index < 8; ++index) {
        value = r_u32(screen);
        screen -= 4;
        w_u32(xy + 24, value);
        value = r_u32(screen);
        screen -= 4;
        w_u32(xy + 16, value);
        value = r_u32(screen);
        screen -= 4;
        w_u32(xy + 8, value);
        value = r_u32(screen);
        screen += 4;
        w_u32(xy, value);
        ordering_owner = r_u32(0x800A8C54);
        ordering = r_u32(ordering_owner + 4) + 0xFD8;
        tag = r_u32(packet);
        link = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (link & 0x00FFFFFF));
        ordering = r_u32(ordering_owner + 4) + 0xFD8;
        link = r_u32(ordering);
        value = (link & 0xFF000000) | (packet & 0x00FFFFFF);
        packet -= 36;
        xy -= 36;
        w_u32(ordering, value);
    }
    t3_80036af8(0x800B6510, (uint32 *)psx_addr(0x1F800000, 72),
        (uint16 *)psx_addr(0x1F800100, 36), 18);
    screen = 0x1F800044;
    bank = r_u32(0x800ADEFC);
    base = owner + bank * 320 + 0xF8C;
    packet = base + 0x12C;
    paired = base + 0x118;
    xy = base + 0x120;
    paired_xy = base + 0x134;
    for (index = 0; index < 8; ++index) {
        value = r_u32(screen);
        screen -= 4;
        packet_link = packet & 0x00FFFFFF;
        w_u32(xy + 8, value);
        value = r_u32(screen);
        screen -= 4;
        w_u32(paired_xy + 8, value);
        value = r_u32(screen);
        screen -= 4;
        w_u32(xy, value);
        value = r_u32(screen);
        screen += 4;
        w_u32(paired_xy, value);
        ordering_owner = r_u32(0x800A8C54);
        ordering = r_u32(ordering_owner + 4) + 0xFD8;
        tag = r_u32(packet);
        link = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (link & 0x00FFFFFF));
        ordering = r_u32(ordering_owner + 4) + 0xFD8;
        link = r_u32(ordering);
        value = (link & 0xFF000000) | packet_link;
        packet -= 40;
        w_u32(ordering, value);
        ordering = r_u32(ordering_owner + 4) + 0xFD8;
        tag = r_u32(paired);
        link = r_u32(ordering);
        w_u32(paired, (tag & 0xFF000000) | (link & 0x00FFFFFF));
        ordering = r_u32(ordering_owner + 4) + 0xFD8;
        link = r_u32(ordering);
        value = (link & 0xFF000000) | (paired & 0x00FFFFFF);
        paired -= 40;
        xy -= 40;
        paired_xy -= 40;
        w_u32(ordering, value);
    }
    bank = r_u32(0x800ADEFC);
    packet = owner + bank * 12 + 0x144C;
    ordering_owner = r_u32(0x800A8C54);
    ordering = r_u32(ordering_owner + 4) + 0xFD8;
    tag = r_u32(packet);
    link = r_u32(ordering);
    w_u32(packet, (tag & 0xFF000000) | (link & 0x00FFFFFF));
    ordering = r_u32(ordering_owner + 4) + 0xFD8;
    link = r_u32(ordering);
    value = (link & 0xFF000000) | (packet & 0x00FFFFFF);
    w_u32(ordering, value);
    return value;
}

uint32 t3_bns_03_800b3bc0(uint32 side)
{
    PSX_RECT rectangle;
    uint32 value;
    uint32 packet;
    uint32 ordering_owner;
    uint32 ordering;
    uint32 x;
    uint32 y;
    uint32 width;
    FUNCTION_MARKER(0x800B3BC0, "SLUS_004.02-BNS-logical-03");
    value = r_u32(0x800ADCA4);
    if (value)
        return value;
    value = r_u8(0x800A8672);
    if (value) {
        --value;
        w_u8(0x800A8672, value);
        return value;
    }
    value = r_u16(side == 1 ? 0x800ADF08 : 0x800ADF0A) & 0x8F0;
    if (value) {
        w_u8(0x80098A40, 0);
        w_u8(0x80098A41, 0);
        w_u8(0x800A8672, 3);
        t3_800753bc(0, 0x50F4, 0);
    }
    x = r_u16(0x800AE230);
    y = r_u16(0x800AE232);
    rectangle.x = (sint16)x;
    ordering_owner = r_u32(0x800A8C54);
    rectangle.w = 0;
    rectangle.h = 0;
    rectangle.y = (sint16)y;
    ordering = r_u32(ordering_owner + 4);
    packet = r_u32(0x800ADF64);
    packet = t3_8004db28(ordering + 8, packet, &rectangle);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004d9bc(ordering + 28, packet, 0x65000000,
        0x00800038, 0x00CC0100, 0x7C040000);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering + 28, packet, 0x60FFFFFF,
        0x00780034, 0x00DC0108);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering + 28, packet, 0x60000000,
        0x0084003C, 0x00DC0108);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004daa8(ordering + 28, packet, 26);
    ordering = r_u32(0x800A9218);
    rectangle.x = 52;
    rectangle.y = 120;
    rectangle.w = 278;
    w_u32(0x800ADF64, packet);
    rectangle.h = 232;
    packet = t3_8004db28(ordering + 28, packet, &rectangle);
    x = r_u16(0x800AE230);
    y = r_u16(0x800AE232);
    width = r_u16(0x800AE234);
    ordering_owner = r_u32(0x800A8C54);
    rectangle.h = 64;
    w_u32(0x800ADF64, packet);
    rectangle.x = (sint16)x;
    rectangle.y = (sint16)y;
    rectangle.w = (sint16)width;
    ordering = r_u32(ordering_owner + 16);
    packet = t3_8004db28(ordering, packet, &rectangle);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_03_800b4174(uint32 node)
{
    uint32 previous;
    uint32 next;
    uint32 result;
    FUNCTION_MARKER(0x800B4174, "SLUS_004.02-BNS-logical-03");
    previous = r_u32(node + 4);
    w_u8(node + 31, 0);
    if (!previous) {
        next = r_u32(node);
        w_u32(0x800B6790, next);
        if (next)
            w_u32(next + 4, 0);
        return 0x800B0000;
    }
    result = r_u32(node);
    w_u32(previous, result);
    next = r_u32(node);
    if (next) {
        result = r_u32(node + 4);
        w_u32(next + 4, result);
    }
    return result;
}

uint32 t3_bns_03_800b3e8c(uint32 preserve)
{
    uint32 node;
    uint32 result = 0x800B0000;
    uint32 first;
    uint32 second;
    uint32 remaining;
    sint32 x;
    sint32 y;
    FUNCTION_MARKER(0x800B3E8C, "SLUS_004.02-BNS-logical-03");
    node = r_u32(0x800B6790);
    while (node) {
        if (!preserve && !r_u8(node + 30)) {
            result = t3_bns_03_800b4174(node);
        } else {
            first = r_u8(node + 28);
            second = r_u8(node + 29);
            x = r_s16(node + 24);
            y = r_s16(node + 26);
            result = t3_8004ce74(node + 8, 0, first, second, x, y);
            if (!preserve) {
                result = r_u16(node + 26);
                remaining = r_u8(node + 30);
                --result;
                --remaining;
                w_u16(node + 26, result);
                w_u8(node + 30, remaining);
            }
        }
        node = r_u32(node);
    }
    return result;
}

uint32 t3_bns_03_800b5a48(void)
{
    VECTOR input;
    VECTOR transformed;
    MATRIX matrix;
    SVECTOR angles;
    sint32 screen;
    sint32 projection;
    sint32 flags;
    uint32 state;
    uint32 sine;
    uint32 cosine;
    uint32 value;
    uint32 other;
    uint32 divisor;
    uint32 quotient;
    uint32 offset;
    uint32 column_start;
    uint32 bank;
    uint32 clip;
    uint32 width;
    uint32 tiles;
    uint32 palettes;
    uint32 allocation;
    uint32 mode_packet;
    uint32 sprite;
    uint32 ordering;
    uint32 row;
    uint32 column;
    uint32 x_start;
    uint32 x;
    uint32 y;
    uint32 tile;
    uint32 palette;
    uint32 texture_page;
    uint32 culled;
    FUNCTION_MARKER(0x800B5A48, "SLUS_004.02-BNS-logical-03");
    state = r_u32(0x800AFAC0);
    if (state == 1)
        return 1;
    value = r_u32(0x800B647C);
    if (state != value) {
        value = r_u32(0x800A85C4);
        w_u32(0x800B6674, value);
    }
    value = r_u32(0x800A85C4);
    input.vz = (sint32)r_u32(0x800A85DC);
    value = (value & 0xFFF) << 1;
    cosine = r_u16(0x8001F0C4 + value);
    sine = r_u16(0x8001E8C4 + value);
    input.vx = (sint32)r_u32(0x800A85D4);
    input.vy = (sint32)r_u32(0x800A85D8);
    w_u32(0x800B647C, state);
    bank = r_u32(0x800ADEFC);
    ApplyMatrixLV((MATRIX *)psx_addr(0x800ADF70, 32), &input, &transformed);
    value = (uint32)transformed.vx * (uint32)(sint32)(sint16)cosine;
    other = (uint32)transformed.vz * (uint32)(sint32)(sint16)sine;
    divisor = r_u32(0x800B6670);
    value = t3_camera_divide(value + other, divisor);
    other = r_u32(0x800B6674);
    other -= r_u32(0x800A85C4);
    other = (uint32)((sint32)other >> 1);
    offset = (uint32)((sint32)value >> 12) + other;
    divisor = r_u32(0x800B6660) << 5;
    quotient = t3_camera_divide(offset, divisor);
    offset -= quotient * divisor;
    w_u32(0x800B6794, (uint32)transformed.vx);
    if ((sint32)offset < 0)
        offset += divisor;
    value = (uint32)((sint32)offset / 32);
    column_start = value + 2;
    other = r_u32(0x80096F2C);
    if (other == 2 && r_u32(0x800AFAC0)) {
        column_start = value + 16;
    } else if (r_s32(0x80096F2C) >= 3) {
        column_start += 8;
    }
    angles.vz = 0;
    angles.vy = 0;
    angles.vx = 0;
    t3_8003a490(&angles, &matrix);
    matrix.t[1] = 256;
    value = (uint32)(sint32)(sint16)sine * 6656;
    other = r_u32(0x800A85D4);
    matrix.t[0] = (sint32)(other - value) >> 12;
    value = (uint32)(sint32)(sint16)cosine * 6656;
    other = r_u32(0x800A85DC);
    matrix.t[2] = (sint32)(other + value) >> 12;
    t3_8003785c_host(0x800ADF70, &matrix);
    RotTransPers(&angles, &screen, &projection, &flags);
    value = r_u32(0x800B6664);
    y = (uint32)((sint32)(sint16)((uint32)screen >> 16)) - (value << 5);
    other = (uint32)r_s16(0x800AE236);
    clip = 0x800B6640 + (bank << 4);
    width = r_u16(0x800AE234);
    if ((sint32)y < (sint32)other) {
        w_u16(clip + 14, y);
        w_u16(clip + 12, width);
    } else {
        w_u16(clip + 12, width);
        other = r_u16(0x800AE236);
        w_u16(clip + 14, other);
    }
    tiles = r_u32(0x800B6668);
    allocation = r_u32(0x800ADD58);
    palettes = r_u32(0x800B666C);
    mode_packet = allocation ? allocation + bank * 960 + 0x12C0 : 0;
    allocation = r_u32(0x800ADD58);
    sprite = allocation ? allocation + bank * 2400 : 0;
    value = (uint32)((sint32)offset / 32);
    x_start = ((value << 5) - offset) << 12;
    y <<= 12;
    ordering = r_u32(0x800A8C54);
    ordering = r_u32(ordering + 4);
    other = r_u32(0x800B6664);
    ordering += 0xFC0;
    row = 0;
    if ((sint32)other > 0) {
        do {
            culled = (sint32)y < -131072 || (sint32)y > 1966080;
            x = x_start;
            for (column = 0; column < 15; ++column) {
                if (!culled) {
                    divisor = r_u32(0x800B6660);
                    value = column_start + column;
                    quotient = t3_camera_divide(value, divisor);
                    value -= quotient * divisor;
                    value = (value + row * divisor) << 1;
                    tile = r_u16(tiles + value);
                    palette = r_u16(palettes + value);
                    w_u16(sprite + 14, palette);
                    w_u8(sprite + 12, (tile & 7) << 5);
                    w_u8(sprite + 13, (tile << 2) & 0xE0);
                    texture_page = (tile >> 8) & 0x1F;
                    w_u16(sprite + 8, (uint32)((sint32)x / 4096));
                    w_u16(sprite + 10, (uint32)((sint32)y / 4096));
                    AddPrim(psx_addr(ordering, 4), psx_addr(sprite, 4));
                    sprite += 20;
                    w_u8(mode_packet + 3, 1);
                    w_u32(mode_packet + 4, texture_page | 0xE1000200);
                    AddPrim(psx_addr(ordering, 4), psx_addr(mode_packet, 4));
                    mode_packet += 8;
                }
                x += 0x20000;
            }
            ++row;
            other = r_u32(0x800B6664);
            y += 0x20000;
        } while ((sint32)row < (sint32)other);
    }
    ordering = r_u32(0x800A8C54);
    ordering = r_u32(ordering + 4) + 0xFF8;
    AddPrim(psx_addr(ordering, 4), psx_addr(clip, 4));
    return r_u32(ordering);
}

uint32 t3_bns_03_800b4794(void)
{
    FUNCTION_MARKER(0x800B4794, "SLUS_004.02-BNS-logical-03");
    t3_bns_03_800b065c(0);
    return t3_bns_03_800b065c(1);
}

uint32 t3_bns_03_800b47bc(uint32 owner)
{
    uint32 value;
    uint32 side;
    uint32 selected;
    FUNCTION_MARKER(0x800B47BC, "SLUS_004.02-BNS-logical-03");
    value = r_u16(0x800AE208);
    w_u8(0x800AE211, (value << 2) + 1);
    selected = r_u8(owner + 28);
    if (selected != 1)
        return 1;
    t3_bns_03_800b4dc8(owner);
    side = r_u8(owner + 31);
    value = r_u32(owner + 112);
    w_u16(0x800ADD5C + (side << 1), (uint32)((sint32)value >> 2));
    w_u16(0x800ADD98 + (side << 1), value & 3);
    w_u16(0x800A9128 + (side << 1), selected);
    return t3_8004f0e4(side);
}

uint32 t3_bns_03_800b4860(uint32 owner)
{
    uint32 state;
    uint32 side;
    uint32 other;
    FUNCTION_MARKER(0x800B4860, "SLUS_004.02-BNS-logical-03");
    state = r_u8(owner + 28);
    if (state == 1) {
        side = r_u8(owner + 30);
        t3_800513b4(r_u16(0x800ADD5C + (side << 1)));
        other = r_u8(owner + 34);
        side = r_u8(owner + 30);
        if (other == side) {
            side = r_u8(owner + 31);
            t3_80056684(r_u16(0x800ADD5C + (side << 1)));
        }
    } else if (state == 2) {
        t3_800513b4(r_u16(0x800ADD5C));
        t3_800513b4(r_u16(0x800ADD5E));
        side = r_u8(owner + 35);
        w_u16(0x800AE1F8 + (side << 1), 0);
    }
    w_u16(0x800ADFBE, 0);
    w_u16(0x800ADFBC, 0);
    w_u16(0x800AE224, 0);
    return 0x800B0000;
}

uint32 t3_bns_03_800b57d0(void)
{
    FUNCTION_MARKER(0x800B57D0, "SLUS_004.02-BNS-logical-03");
    return 0x1A40;
}

uint32 t3_bns_03_800b57d8(void)
{
    FUNCTION_MARKER(0x800B57D8, "SLUS_004.02-BNS-logical-03");
    return 0x404;
}

uint32 t3_bns_03_800b49a8(uint32 first, uint32 maximum, uint32 second, uint32 highlighted)
{
    uint32 packet;
    uint32 ordering;
    uint32 side;
    uint32 owner;
    uint32 selected;
    uint32 target;
    uint32 current;
    uint32 trailing;
    uint32 cached;
    uint32 lengths[3];
    uint32 index;
    uint32 x;
    uint32 y;
    uint32 start;
    uint32 end;
    uint32 top;
    uint32 bottom;
    uint32 color;
    uint32 value;
    uint32 direction;
    uint32 code;
    uint32 palette;
    FUNCTION_MARKER(0x800B49A8, "SLUS_004.02-BNS-logical-03");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    for (side = 0; side < 2; ++side) {
        owner = 0x800B6424 + side * 16;
        selected = side ? second : first;
        code = side ? 61 : 62;
        target = (uint32)r_s16(owner + 4);
        current = (uint32)r_s16(owner + 6);
        cached = r_u32(owner);
        trailing = (uint32)r_s16(owner + 8);
        if (cached != selected) {
            w_u32(owner, selected);
            target = t3_camera_divide(selected * 76, maximum);
        }
        if ((sint32)target >= 77)
            target = 76;
        if ((sint32)current < (sint32)trailing) {
            value = trailing - current + 15;
            trailing -= (uint32)((sint32)value >> 4);
        } else {
            trailing = current;
        }
        if ((sint32)current < (sint32)target) {
            current += 3;
            if ((sint32)target < (sint32)current)
                current = target;
        } else {
            current = target;
        }
        if ((sint32)trailing < (sint32)current)
            trailing = current;
        w_u16(owner + 4, target);
        w_u16(owner + 6, current);
        w_u16(owner + 8, trailing);
        lengths[1] = trailing - current;
        lengths[2] = 76 - trailing;
        lengths[0] = current;
        x = r_u16(owner + 12);
        y = r_u16(owner + 14);
        color = 0x00808080;
        if (highlighted == (1u << side) && !r_u32(0x8009548C)) {
            value = t3_8004dfe0(r_u32(0x800AE218) << 4);
            color = t3_8004df6c(0x00FFFFFF, value);
        }
        value = color | 0x64000000;
        packet = t3_8004d9bc(ordering, packet, value, x | (y << 16),
            0x00180006, 0x7EDEA8E0);
        packet = t3_8004d9bc(ordering, packet, value,
            ((x + 74) & 0xFFFF) | (y << 16), 0x00180006, 0x7EDEA8EA);
        direction = r_u16(owner + 10);
        x = r_u16(owner + 12);
        x += direction == 1 ? 2 : 78;
        y = r_u16(owner + 14);
        for (index = 0; index < 3; ++index) {
            value = lengths[index];
            if (!value)
                continue;
            direction = r_u16(owner + 10);
            if (direction == 1) {
                start = x;
                x += value;
            } else {
                start = x - value;
                x = start;
            }
            if (index == 0)
                palette = (((code >> 4) | 0x1F8) << 6) | (code & 15) | 0x10;
            else
                palette = index == 1 ? 0x7EDC : 0x7EDF;
            top = y << 16;
            end = (start + value) & 0xFFFF;
            start &= 0xFFFF;
            bottom = (y + 24) << 16;
            packet = t3_8004d830(ordering, packet, color | 0x2C000000,
                start | top, end | top, start | bottom, end | bottom,
                (palette << 16) | 0xA8E8, 0x000DA8E9, 0xC0E9C0E8);
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_bns_03_800b07a0(void)
{
    FUNCTION_MARKER(0x800B07A0, "SLUS_004.02-BNS-logical-03");
    return 0x1464;
}

uint32 t3_bns_03_800b4dc8(uint32 owner)
{
    uint32 pairs[44];
    uint32 mask;
    uint32 count;
    uint32 eligible;
    uint32 index;
    uint32 threshold;
    uint32 random;
    uint32 selected;
    uint32 packed;
    uint32 result;
    FUNCTION_MARKER(0x800B4DC8, "SLUS_004.02-BNS-logical-03");
    mask = r_u32(0x80097EF0) & 0x53FFF;
    count = 0;
    for (index = 0; index < 22; ++index) {
        if (mask & (1 << index)) {
            pairs[count * 2] = index;
            pairs[count * 2 + 1] = r_u16(owner + 0x44 + index * 2);
            ++count;
        }
    }
    eligible = 0;
    if (count) {
        t3_8004cd80(pairs, count, 0);
        threshold = pairs[1] + 4;
        while (eligible < count && pairs[eligible * 2 + 1] <= threshold)
            ++eligible;
    }
    if (!r_u8(0x80097F1E)) {
        w_u8(0x80097F1E, 1);
        // Count the character selected by the first-use branch
        selected = 17;
        packed = 0x44;
    } else {
        random = t3_8004ce54();
        // Keep the first-use character when the candidate pool is empty
        selected = eligible ? pairs[(random % eligible) * 2] : 17;
        packed = selected * 4 + (r_u32(0x800AE218) & 1);
    }
    w_u32(owner + 0x70, packed);
    result = r_u16(owner + 0x44 + selected * 2) + 1;
    w_u16(owner + 0x44 + selected * 2, result);
    return result;
}

uint32 t3_bns_03_800b3f98(const uint8 *text, uint32 position, uint32 style, uint32 font, uint32 lifetime)
{
    MATRIX matrix = psx_identity_matrix;
    SVECTOR origin = {0, 0, 0, 0};
    uint32 next;
    uint32 slot;
    uint32 destination;
    uint32 length;
    uint32 byte;
    uint32 screen;
    uint32 x;
    uint32 y;
    uint32 previous;
    FUNCTION_MARKER(0x800B3F98, "SLUS_004.02-BNS-logical-03");
    next = r_u32(0x800B6688) + 1;
    next = (uint32)((sint32)next % 8);
    w_u32(0x800B6688, next);
    slot = 0x800B6690 + (next << 5);
    byte = r_u8(slot + 31);
    if (byte)
        return byte;
    destination = t3_8004cc08(slot + 8, 0x800B05C4);
    length = 0;
    do {
        byte = *text++;
        w_u8(destination++, byte);
        if (byte)
            ++length;
    } while (byte);
    // Define the message transform before composing its position
    matrix.t[0] = (sint32)r_u32(position);
    matrix.t[1] = (sint32)r_u32(position + 4);
    matrix.t[2] = (sint32)r_u32(position + 8);
    t3_8003785c_host(0x800ADF70, &matrix);
    t3_80036ad4(&origin, &screen);
    w_u32(slot + 24, screen);
    switch (font & 255) {
    case 0:
        x = r_u16(slot + 24) - length * 5;
        y = r_u16(slot + 26) - 8;
        break;
    case 1:
        x = r_u16(slot + 24) - length * 7;
        y = r_u16(slot + 26) - 12;
        break;
    case 2:
        x = r_u16(slot + 24) - length * 12;
        y = r_u16(slot + 26) - 20;
        break;
    default:
        x = 0;
        y = 0;
        break;
    }
    if ((font & 255) <= 2) {
        w_u16(slot + 24, x);
        w_u16(slot + 26, y);
    }
    w_u8(slot + 29, font);
    w_u8(slot + 28, style);
    w_u8(slot + 30, lifetime);
    w_u8(slot + 31, 1);
    previous = r_u32(0x800B6790);
    w_u32(slot + 4, 0);
    w_u32(slot, previous);
    if (previous)
        w_u32(previous + 4, slot);
    w_u32(0x800B6790, slot);
    return previous;
}

static void t3_ball_hit_score(uint32 owner, uint32 actor, uint8 *text)
{
    uint32 power = (uint32)r_s16(actor + 0x5E);
    uint32 scale = (uint32)r_s16(0x800B6618);
    uint32 product = power * scale;
    uint32 slot = owner + ((uint32)r_s16(actor + 0x12) << 1);
    uint32 value = r_u16(slot + 0x13C);
    uint32 score = (uint32)((sint32)product / 100);
    uint32 previous;
    uint32 style;
    value += score;
    w_u16(slot + 0x13C, value);
    w_u16(owner + 0x140, value);
    t3_bns_03_800b3de0(score, text);
    previous = r_u32(owner + 0x130);
    w_u16(owner + 0xB2, 1);
    style = r_u8(previous + 0x1E) ? 5 : 2;
    t3_bns_03_800b3f98(text, owner + 0x68, style, 1, 30);
}

static void t3_ball_history(uint32 owner)
{
    uint32 cursor = owner + 0xD0;
    uint32 limit = owner + 0x130;
    uint32 source = owner + 0xE4;
    uint32 offsets = 0x800B6344;
    uint32 offset_pair = offsets + 4;
    uint32 value;
    uint32 other;
    uint32 delta;
    while (cursor < limit) {
        value = r_u32(source - 8);
        w_u32(cursor, value);
        value = r_u32(source - 4);
        other = r_u32(source);
        w_u32(source - 16, value);
        w_u32(source - 12, other);
        delta = (uint32)r_s16(offsets);
        value = r_u32(owner + 0x68);
        cursor += 24;
        w_u32(source - 8, value + delta);
        delta = (uint32)r_s16(offset_pair - 2);
        value = r_u32(owner + 0x6C);
        offsets += 8;
        w_u32(source - 4, value + delta);
        delta = (uint32)r_s16(offset_pair);
        value = r_u32(owner + 0x70);
        offset_pair += 8;
        w_u32(source, value + delta);
        source += 24;
    }
}

uint32 t3_bns_03_800b11dc(uint32 owner, uint32 first_actor, uint32 second_actor,
    const uint32 *vertices, uint32 initial_collision, uint32 initial_count)
{
    VECTOR collision_vector;
    uint8 number_text[16];
    uint32 collision = initial_collision;
    uint32 captured_mode;
    uint32 value;
    uint32 other;
    uint32 third;
    uint32 fourth;
    uint32 result;
    uint32 x;
    uint32 z;
    uint32 left;
    uint32 right;
    uint32 delta_x;
    uint32 delta_z;
    uint32 angle;
    uint32 speed;
    uint32 target_x;
    uint32 target_z;
    uint32 target;
    uint32 product;
    uint32 frames;
    uint32 vertical;
    uint32 height;
    uint32 hit_actor;
    uint32 other_actor;
    uint32 source;
    uint32 destination;
    uint32 limit;
    uint32 strike_power;
    uint32 special_hit;
    sint32 state;

    FUNCTION_MARKER(0x800B11DC, "SLUS_004.02-BNS-logical-03");
    value = r_u8(owner + 0xCC);
    captured_mode = r_u8(0x800AFAC9);
    other = value ? r_u8(owner + 0x145) : 0;
    state = r_s16(owner);
    w_u8(owner + 0x145, other);
    w_u8(0x800B668C, 0);
    w_u32(0x800B6684, 0);
    switch (state) {
    case 0:
        value = r_u8(owner + 0x147) - 1;
        w_u8(owner + 0x147, value);
        result = value & 255;
        if (result)
            return result;
        w_u16(owner, 8);
        w_u8(owner + 0x147, 32);
        return t3_800753bc(0, 0x4CE8, 0);
    case 1:
    case 8:
        value = r_u8(owner + 0x147) - 1;
        w_u8(owner + 0x147, value);
        if (value & 255)
            return 8;
        state = r_s16(owner);
        w_u16(owner, state == 8 ? 9 : 2);
        w_u32(owner + 0xA8, 0);
        w_u8(owner + 0xCC, 0);
        w_u16(owner + 0xB0, 0);
        w_u16(owner + 0xB2, 0);
        left = vertices[0];
        right = vertices[1];
        x = r_u32(left);
        other = r_u32(right);
        if ((sint32)x > 0)
            x = 0;
        if ((sint32)other < 0)
            other = 0;
        value = r_u32(right + 8);
        z = r_u32(left + 8);
        delta_x = other - x;
        delta_z = value - z;
        if (delta_x) {
            value = r_u32(owner + 0x68);
            product = (value - x) * delta_z;
            result = z + t3_camera_divide(product, delta_x);
            w_u32(owner + 0x70, result);
        } else {
            result = x + t3_camera_divide(delta_x, delta_z);
            w_u32(owner + 0x68, result);
        }
        return result;
    case 6:
        value = r_u8(owner + 0x147);
        w_u16(owner + 0x140, 0);
        w_u8(owner + 0x144, 0);
        --value;
        w_u8(owner + 0x147, value);
        result = value & 255;
        if (result)
            return result;
        w_u16(owner, 1);
        w_u8(owner + 0x147, 32);
        w_u32(owner + 0x68, 0);
        w_u32(owner + 0x6C, (uint32)-0xE40);
        left = vertices[0];
        right = vertices[1];
        value = r_u32(left + 8);
        other = r_u32(right + 8);
        third = r_u16(0x800B6614);
        w_u32(owner + 0x90, 0);
        w_u32(owner + 0x88, 0);
        w_u32(owner + 0x80, 0);
        w_u32(owner + 0x7C, 0);
        w_u32(owner + 0x78, 0);
        w_u16(owner + 0xA4, 0);
        w_u16(owner + 0xA2, 0);
        w_u16(owner + 0xA0, 0);
        third = (uint32)((sint32)(sint16)third >> 1);
        w_u32(owner + 0x8C, third);
        w_u32(owner + 0x70, (uint32)((sint32)(value + other) >> 1));
        t3_ball_history(owner);
        t3_ball_history(owner);
        result = r_u32(0x8009547C);
        if (result)
            return result;
        return t3_800753bc(0, 0x4CE9, 0);
    case 7:
        value = r_u8(owner + 0x147);
        w_u16(owner + 0x140, 0);
        w_u8(owner + 0x144, 0);
        --value;
        w_u8(owner + 0x147, value);
        if (value & 255)
            return 6;
        w_u16(owner, 6);
        w_u8(owner + 0x147, 16);
        return 16;
    default:
        state = r_s16(owner);
        if (state == 4) {
            if (r_s32(owner + 0x6C) >= -0x1400)
                goto contact_and_floor;
            value = (uint32)r_s16(owner + 0x142) << 1;
            w_u32(owner + 0x6C, (uint32)-0x1400);
            other = (uint32)r_s16(0x800B6614);
            third = r_u32(owner + 0x138);
            w_u16(owner, 2);
            fourth = (uint32)r_s16(owner + 0x142);
            w_u32(owner + 0x8C, other);
            w_u32(owner + 0x134, third);
            w_u32(owner + 0xA8, fourth);
            w_u32(owner + 0x7C, (uint32)((sint32)value / 3) + 0x1000);
            goto motion;
        }
        if (r_s32(owner + 0x6C) >= -0x2000)
            goto contact_and_floor;
        other = (uint32)r_s16(0x800B6614);
        w_u32(owner + 0x6C, (uint32)-0x2000);
        w_u32(owner + 0x7C, 0);
        w_u32(owner + 0x8C, other);
        goto motion;
    }

contact_and_floor:
    value = r_u32(owner + 0x6C);
    if ((sint32)value < -0x180) {
        value = r_u32(owner + 0x68);
        if ((sint32)value < 0)
            value = 0 - value;
        if ((sint32)value <= 0x2800)
            goto settle_and_contact;
    }
    other = (uint32)r_s16(0x800B6614);
    value = r_u32(owner + 0x7C);
    w_u32(owner + 0x6C, (uint32)-0x180);
    w_u32(owner + 0x8C, other);
    if ((sint32)value > 0xD00)
        t3_800753bc(0, 0x4E6E, 0);
    if (captured_mode == 2) {
        value = r_u32(owner + 0x7C);
        if ((sint32)value > 0x3000)
            t3_8004acfc(1);
        else if ((sint32)value > 0x1800)
            t3_8004acfc(0);
    }
    value = r_u32(owner + 0x68);
    other = (uint32)r_s16(0x800B6616);
    if ((sint32)value < 0)
        value = 0 - value;
    if ((sint32)other < (sint32)value) {
        value = r_u32(0x8009547C);
        w_u32(owner + 0x7C, 0);
        w_u32(owner + 0xA8, 0);
        if (value)
            goto reset_wait;
        if (r_s16(owner) == 5)
            goto reset_wait;
        value = r_u32(owner + 0x68);
        target = (sint32)value < 0 ? owner + 0x13C : owner + 0x13E;
        w_u16(target, 0);
        value = r_u32(owner + 0x68);
        target = (sint32)value < 0 ? first_actor : second_actor;
        value = r_u32(target + 0x3F4);
        other = r_u8(target + 0x1E);
        value += 0xFFE20000;
        w_u32(target + 0x3F4, value);
        w_u8(target + 0x22, (other + 1) & 1);
        if ((sint32)value < 0)
            w_u32(target + 0x3F4, 0);
        w_u16(owner, 7);
        w_u8(owner + 0x147, 5);
        value = r_u32(0x800954A0);
        other = value ? 0 : r_u8(target + 0x1E) + 1;
        value = r_u32(owner + 0x68);
        w_u32(0x800AD2A8, value);
        value = r_u32(owner + 0x70);
        w_u32(0x800B6684, other);
        w_u32(0x800AD2B0, value);
        goto motion;
    }
    value = r_u32(owner + 0x7C);
    w_u16(owner, 2);
    other = r_u16(owner + 0xA0);
    third = r_u16(owner + 0xA2);
    value = (uint32)((sint32)(0 - value) >> 1);
    w_u32(owner + 0x7C, value);
    fourth = r_u16(owner + 0xA4);
    w_u16(owner + 0xA0, (uint32)((sint32)(sint16)other >> 1));
    w_u16(owner + 0xA2, (uint32)((sint32)(sint16)third >> 1));
    w_u16(owner + 0xA4, (uint32)((sint32)(sint16)fourth >> 1));

settle_and_contact:
    if (r_s32(owner + 0x6C) < -0x189) {
        w_u32(0x800B661C, 0);
    } else {
        value = r_u32(0x800B661C) + 1;
        w_u32(0x800B661C, value);
        if ((sint32)value >= 600)
            goto reset_wait;
    }
    if (!r_s16(owner + 0xB2)) {
        value = r_u32(owner + 0xA8) - 0x20;
        w_u32(owner + 0xA8, value);
        if ((sint32)value < 0)
            w_u32(owner + 0xA8, 0);
    }
    value = r_u8(owner + 0xCC);
    if (value) {
        w_u8(owner + 0xCC, value - 1);
        goto motion;
    }
    value = r_u32(owner + 0x68);
    other = r_u32(owner + 0x6C);
    third = r_u32(owner + 0x70);
    w_u32(owner + 0xB8, value);
    w_u32(owner + 0xBC, other);
    w_u32(owner + 0xC0, third);
    collision = t3_80047d98(first_actor, second_actor, owner + 0xB8,
        &collision_vector, owner, initial_count);
    if (!(collision & 7))
        goto missed_contact;
    other_actor = (collision & 2) ? second_actor : first_actor;
    hit_actor = (collision & 2) ? first_actor : second_actor;
    if (r_u8(hit_actor + 0x85))
        goto motion;
    t3_80075dec((uint32)r_s16(hit_actor + 0x12), 5);
    source = hit_actor;
    destination = 0x800AC340;
    limit = hit_actor + 0x1880;
    w_u32(owner + 0xAC, 0);
    w_u32(owner + 0x130, hit_actor);
    w_u32(owner + 0x134, other_actor);
    w_u16(owner + 0xB2, 0);
    do {
        value = r_u32(source);
        other = r_u32(source + 4);
        third = r_u32(source + 8);
        fourth = r_u32(source + 12);
        w_u32(destination, value);
        w_u32(destination + 4, other);
        w_u32(destination + 8, third);
        w_u32(destination + 12, fourth);
        source += 16;
        destination += 16;
    } while (source != limit);
    value = r_u32(source);
    other = r_u32(source + 4);
    third = r_u32(source + 8);
    w_u32(destination, value);
    w_u32(destination + 4, other);
    w_u32(destination + 8, third);
    w_u8(0x800AC35E, 2);
    w_u16(0x800AC462, 1);
    w_u16(0x800AC3A4, 0xFFFF);
    w_u8(0x800AC41F, 1);
    w_u8(hit_actor + 0x85, 1);
    w_u8(hit_actor + 0x88, 1);
    w_u8(hit_actor + 0xD1, 1);
    w_u8(hit_actor + 0x23, 2);
    w_u8(owner + 0xCC, 5);
    value = r_u8(other_actor + 0x1E);
    x = r_u32(owner + 0x68);
    target = vertices[value];
    z = r_u32(owner + 0x70);
    target_x = r_u32(target);
    target_z = r_u32(target + 8);
    result = t3_8004b39c(target_x - x, target_z - z);
    w_u16(owner + 0xB0, result);
    t3_bns_03_800b2c50(owner + 0x148, &collision_vector);
    value = r_u16(hit_actor + 0x5E);
    w_u8(owner + 0xCD, 1);
    w_u16(owner + 0x150, (value << 3) + 0x140);
    if ((collision & 3) == 3) {
        value = (uint32)r_s16(0x800B6614);
        w_u16(owner + 0x140, 0);
        w_u32(owner + 0xA8, 0);
        w_u16(owner, 2);
        w_u32(owner + 0x7C, (0 - value) * 37);
        w_u32(owner + 0x8C, value);
        goto motion;
    }
    if (r_s16(hit_actor + 0x16) == 4) {
        value = r_u16(hit_actor + 0x64);
        if ((value == 0x607 || value == 0x706) && !(collision & 8)
            && r_s16(hit_actor + 0xA0) != 0x181) {
            value = r_u8(other_actor + 0x1E);
            value = (uint32)r_s16(owner + 0x13C + value * 2) << 16;
            other = r_u32(hit_actor + 0x3F4) - value;
            w_u32(hit_actor + 0x3F4, other);
            if ((sint32)other < 0)
                w_u32(hit_actor + 0x3F4, 0);
            w_u16(owner + 0x13E, 0);
            w_u16(owner + 0x13C, 0);
            w_u16(owner, 7);
            w_u8(owner + 0x147, 5);
            value = r_u8(hit_actor + 0x1E);
            w_u32(0x800B6684, value + 1);
            goto motion;
        }
    }
    value = (uint32)r_s16(hit_actor + 0x5E);
    other = r_u16(hit_actor + 0x5E);
    if (!value) {
        if (r_s16(owner + 0x140) > 0) {
            value = r_u16(0x800B6612);
            other = (uint32)r_s16(0x800B6614);
            product = value * 5;
            w_u16(owner, 3);
            value = r_u16(owner + 0xA8);
            third = r_u32(owner + 0xA8);
            w_u32(owner + 0x7C, (0 - other) * 37);
            w_u32(owner + 0x8C, other);
            speed = (uint32)((sint32)third >> 2);
            w_u16(owner + 0x142, value + product);
            value = (uint32)r_s16(hit_actor + 0x5E);
            other = (uint32)r_s16(0x800B6610);
            product = (value + 8) * other;
            x = r_u32(owner + 0x68);
            value = (sint32)x < 0 ? product - x : product + x;
            value -= speed;
            w_u32(owner + 0xA8, value);
            if ((sint32)value < 0)
                w_u32(owner + 0xA8, 0);
            value = r_u8(owner + 0xCC);
            w_u16(owner + 0x140, 0);
            w_u8(owner + 0xCC, value + 2);
            t3_bns_03_800b3f98((const uint8 *)psx_addr(0x800B0570, 2),
                owner + 0x68, 1, 1, 30);
            t3_800753bc(0, 0x7082, 0);
            goto motion;
        }
        other = (uint32)r_s16(0x800B6614);
        value = r_u32(owner + 0xA8);
        w_u32(owner + 0x8C, other);
        w_u32(owner + 0x7C, (0 - other) * 37);
        other = (uint32)r_s16(hit_actor + 0x5E);
        speed = (uint32)((sint32)value >> 2);
        third = (uint32)r_s16(0x800B6610);
        product = (other + 10) * third;
        x = r_u32(owner + 0x68);
        value = (sint32)x < 0 ? product - x : product + x;
        value -= speed;
        w_u32(owner + 0xA8, value);
        if ((sint32)value < 0)
            w_u32(owner + 0xA8, 0);
        w_u16(owner + 0x140, 0);
        w_u16(owner, 2);
        goto motion;
    }
    value = r_u16(hit_actor + 0x64);
    if (value == 0x607 || value == 0x706) {
        w_u16(owner + 0xB2, 1);
        value = (uint32)r_s16(hit_actor + 0x12);
        other = (uint32)r_s16(0x800B6612);
        height = r_u32(owner + 0x6C);
        third = r_u16(owner + 0x13C + (value << 1));
        fourth = r_u16(0x800B6614);
        w_u32(owner + 0xA8, other * 60);
        w_u32(owner + 0x8C, (uint32)((sint32)(sint16)fourth >> 2));
        w_u16(owner + 0x140, third);
        value = r_u8(other_actor + 0x1E);
        target = vertices[value];
        value = r_u32(target + 8);
        w_u16(owner, 2);
        other = r_u32(0x800AFA3C);
        w_u32(owner + 0x7C, value - (height << 1) - other);
        t3_bns_03_800b3f98((const uint8 *)psx_addr(0x800B0570, 2),
            owner + 0x68, 2, 1, 30);
        goto motion;
    }
    special_hit = r_u8(hit_actor + 0xDB);
    if (special_hit) {
        value = r_s16(owner + 0x140);
        strike_power = other + 15;
        if (value) {
            value = r_u8(owner + 0x144);
            w_u8(owner + 0x145, 1);
            w_u8(owner + 0x144, value + 1);
        }
        t3_ball_hit_score(owner, hit_actor, number_text);
        goto powered_response;
    }
    value = r_u16(hit_actor + 0x64);
    if (value == 0x31F || value == 0x217) {
        other = r_u16(hit_actor + 0x5E);
        value = r_s16(owner + 0x140);
        strike_power = other + 18;
        if (value) {
            value = r_u8(owner + 0x144);
            w_u8(owner + 0x145, 1);
            w_u8(owner + 0x144, value + 1);
        }
        t3_ball_hit_score(owner, hit_actor, number_text);
        goto powered_response;
    }
    if (value == 0x10F || value == 0x51F) {
        other = (uint32)r_s16(0x800B6614);
        w_u32(owner + 0x7C, (0 - other) * 50);
        w_u32(owner + 0x8C, other);
        value = r_u16(hit_actor + 0x5E);
        other = (uint32)r_s16(0x800B6610);
        product = (uint32)((sint32)(sint16)value >> 2) * other;
        x = r_u32(owner + 0x68);
        value = (sint32)x < 0 ? product - x : product + x;
        w_u32(owner + 0xA8, value);
        w_u16(owner + 0x140, 0);
        w_u16(owner, 2);
        goto motion;
    }
    if (value != 0x412)
        goto motion;
    if (r_s16(owner) == 3) {
        other = (uint32)r_s16(0x800B6614);
        w_u16(owner, 4);
        w_u32(owner + 0x138, other_actor);
        third = (uint32)r_s16(0x800B6610);
        w_u32(owner + 0x7C, (0 - other) * 72);
        w_u32(owner + 0x8C, other);
        w_u32(owner + 0xA8, third * 10);
        t3_800753bc(0, 0x4DAF, 0);
        if (r_s16(owner + 0x140)) {
            value = r_u8(owner + 0x144);
            w_u8(owner + 0x145, 1);
            w_u8(owner + 0x144, value + 1);
        }
        t3_ball_hit_score(owner, hit_actor, number_text);
        goto motion;
    }
    other = (uint32)r_s16(0x800B6614);
    value = r_u32(owner + 0xA8);
    w_u32(owner + 0x8C, other);
    w_u32(owner + 0x7C, (0 - other) * 37);
    other = (uint32)r_s16(hit_actor + 0x5E);
    speed = (uint32)((sint32)value >> 2);
    third = (uint32)r_s16(0x800B6610);
    product = (other + 10) * third;
    x = r_u32(owner + 0x68);
    value = (sint32)x < 0 ? product - x : product + x;
    value -= speed;
    w_u32(owner + 0xA8, value);
    if ((sint32)value < 0)
        w_u32(owner + 0xA8, 0);
    w_u16(owner + 0x140, 0);
    w_u16(owner, 2);
    goto motion;

powered_response:
    value = (uint32)(sint32)(sint16)strike_power;
    if ((sint32)value < 20)
        value = 20;
    else if ((sint32)value > (special_hit ? 40 : 46))
        value = special_hit ? 40 : 46;
    other = r_u8(owner + 0x144);
    value += other;
    other = (uint32)r_s16(0x800B6612);
    product = value * other;
    value = r_u8(owner + 0x145);
    if (value) {
        value = r_u32(owner + 0xA8);
        product += (uint32)((sint32)value >> 4);
    }
    if (special_hit) {
        w_u32(owner + 0xA8, product);
        value = r_u8(other_actor + 0x1E);
        other = r_u16(0x800B6614);
        target = vertices[value];
        value = r_u32(target + 8);
        w_u16(owner, 2);
        w_u32(owner + 0x8C, (uint32)((sint32)(sint16)other >> 1));
        other = r_u32(owner + 0x6C);
        third = r_u32(0x800AFA3C);
        w_u32(owner + 0x7C, value - other - third);
    } else {
        value = r_u16(0x800B6614);
        w_u32(owner + 0xA8, product);
        w_u32(owner + 0x8C, (uint32)((sint32)(sint16)value >> 2));
        value = r_u8(other_actor + 0x1E);
        height = r_u32(owner + 0x6C);
        target = vertices[value];
        value = r_u32(target + 8);
        w_u16(owner, 2);
        other = r_u32(0x800AFA3C);
        w_u32(owner + 0x7C, value - (height << 1) - other);
    }
    goto motion;

reset_wait:
    w_u16(owner, 6);
    w_u8(owner + 0x147, 16);
    goto motion;

missed_contact:
    value = r_u8(first_actor + 0xC4);
    collision = 0;
    if (!value && !r_u8(second_actor + 0xC4))
        collision = t3_80047f14(first_actor, second_actor, owner + 0xD0);
    if (r_s32(0x80096F2C) >= 6) {
        if (!r_u8(second_actor + 0xC3))
            collision &= 2;
        else if (!r_u8(first_actor + 0xC3))
            collision &= 1;
    }
    if (!collision)
        goto motion;
    other_actor = (collision & 2) ? second_actor : first_actor;
    hit_actor = (collision & 2) ? first_actor : second_actor;
    value = r_u32(owner + 0x130);
    if (hit_actor == value && r_s16(owner + 0x140))
        goto motion;
    value = r_u8(other_actor + 0x1E);
    target = vertices[value];
    state = r_s16(owner);
    target_x = r_u32(target);
    target_z = r_u32(target + 8);
    w_u32(owner + 0xAC, 0);
    w_u16(owner + 0xB2, 0);
    if (state != 3)
        w_u16(owner, 2);
    x = r_u32(owner + 0x68);
    z = r_u32(owner + 0x70);
    result = t3_8004b39c(target_x - x, target_z - z);
    speed = r_u32(0x800AFA3C);
    other = (uint32)r_s16(0x800B6614);
    frames = (speed >> 7) + 12;
    product = (0 - other) * frames;
    w_u16(owner + 0xB0, result);
    height = r_u32(owner + 0x6C) + 0x1000;
    value = (uint32)((sint32)product >> 1) - height;
    w_u32(owner + 0x8C, other);
    w_u32(owner + 0x7C, value);
    value = speed > 0x800 ? speed - 0x800 : 0x400;
    result = t3_camera_divide(value << 8, frames);
    other = (uint32)r_s16(owner + 0x140);
    w_u32(owner + 0xA8, result);
    if (!other)
        goto repeated_contact;
    value = r_u32(owner + 0x130);
    if (value == hit_actor)
        goto repeated_contact;
    value = r_u16(owner + 0x140);
    w_u32(owner + 0x130, hit_actor);
    w_u32(owner + 0x134, other_actor);
    w_u16(owner, 2);
    w_u8(0x800B668C, 1);
    w_u16(0x800AC39E, value);
    w_u16(owner + 0x140, 0);
    value = r_u8(other_actor + 0x1E);
    w_u8(0x800AC3C3 + value, 1);
    w_u8(0x800AC3C8, 1);
    w_u8(0x800AC411, 1);
    value = r_u8(other_actor + 0x1E);
    w_u8(0x800AC363, value);
    w_u8(hit_actor + 0x87, 1);
    w_u8(hit_actor + 0xCE, 1);
    value = r_u32(0x800AC394);
    value = r_u8(value + 0x2C);
    w_u16(hit_actor + 0xEE, value);
    w_u8(hit_actor + 0x15D, 1);
    w_u8(hit_actor + 0x15C, 2);
    if (r_s16(0x800AC39E) || r_u8(hit_actor + 0xDB))
        w_u16(hit_actor + 0x104, 4);
    value = r_u16(hit_actor + 0x66);
    other = r_u16(0x800AC3A4);
    if (!(value & other)) {
        value = (uint32)r_s16(0x800AC39E);
        if ((sint32)value >= 100 || r_u8(owner + 0x146) == 9) {
            t3_8006ef60(r_u8(hit_actor + 0x1E));
            t3_8004a5a8(r_u8(hit_actor + 0x1E), 0x82, 0);
            if (captured_mode == 2)
                w_u32(0x800AFAC4, 6);
        }
        value = r_u16(0x800B6614);
        w_u32(owner + 0xA8, 0);
        w_u8(owner + 0x144, 0);
        w_u16(owner + 0x13C, 0);
        w_u16(owner + 0x13E, 0);
        w_u16(owner, 5);
        w_u32(owner + 0x130, hit_actor);
        w_u32(owner + 0x134, hit_actor);
        w_u8(owner + 0xCC, 5);
        other = (uint32)(sint32)(sint16)value;
        w_u32(owner + 0x7C, (0 - other) * 37);
        w_u32(owner + 0x8C, (uint32)((sint32)other >> 1));
        value = (uint32)r_s16(0x800AC39E);
        t3_800753bc(0, (sint32)value < 50 ? 0x4E6D : 0x4DAF, 0);
        goto motion;
    }
    t3_800753bc(0, 0x7043, 0);
    value = (uint32)r_s16(0x800AC39E);
    if ((sint32)value >= 100 || r_u8(owner + 0x146) == 9) {
        other = r_u32(hit_actor + 0x3F4);
        if ((sint32)(value << 13) >= (sint32)other) {
            t3_8006ef60(r_u8(hit_actor + 0x1E));
            t3_8004a5a8(r_u8(hit_actor + 0x1E), 0x82, 0);
        }
    }
    goto motion;

repeated_contact:
    if (!r_u32(0x800B6620)) {
        t3_800753bc(0, 0x704E, 0);
        w_u32(0x800B6620, 10);
    }
    w_u32(owner + 0x130, hit_actor);
    w_u32(owner + 0x134, other_actor);
    goto motion;

motion:
    angle = r_u16(owner + 0xB0) & 0xFFF;
    other = (uint32)r_s16(0x8001F0C4 + angle * 2);
    speed = r_u32(owner + 0xA8);
    value = other * speed;
    angle = r_u16(owner + 0xB0) & 0xFFF;
    w_u32(owner + 0x78, (uint32)((sint32)value >> 12));
    other = (uint32)r_s16(0x8001E8C4 + angle * 2);
    speed = r_u32(owner + 0xA8);
    state = r_s16(owner);
    value = other * speed;
    w_u32(owner + 0x80, (uint32)((sint32)value >> 12));
    if (state == 9) {
        value = r_u8(owner + 0x147);
        other = r_u8(owner + 0x147);
        value = r_u16(0x8001E8C4 + ((value << 6) & 0x1FC0));
        w_u8(owner + 0x147, other + 1);
        other = r_u32(0x80095460);
        value = (uint32)((sint32)(sint16)value >> 4) - 0x500;
        w_u32(owner + 0x6C, value);
        if ((sint32)other >= 600) {
            w_u16(owner, 2);
            w_u32(owner + 0x7C, 0);
        }
    } else {
        value = r_u32(owner + 0x78);
        other = r_u32(owner + 0x68);
        third = other + (uint32)((sint32)value >> 8);
        value = r_u32(owner + 0x7C);
        w_u32(owner + 0x68, third);
        other = r_u32(owner + 0x6C);
        third = other + (uint32)((sint32)value >> 8);
        value = r_u32(owner + 0x80);
        w_u32(owner + 0x6C, third);
        other = r_u32(owner + 0x70);
        w_u32(owner + 0x70, other + (uint32)((sint32)value >> 8));
    }
    t3_ball_history(owner);
    value = r_u32(owner + 0x7C);
    other = r_u32(owner + 0x8C);
    w_u32(owner + 0x7C, value + other);
    if (collision && r_s16(owner) != 3) {
        if (!r_s16(owner + 0xB2)) {
            angle = r_u16(owner + 0xB0) & 0xFFF;
            other = (uint32)r_s16(0x8001F0C4 + angle * 2);
            speed = r_u32(owner + 0xA8);
            product = other * speed;
            value = (uint32)((sint32)product >> 19);
            angle = r_u16(owner + 0xB0);
        } else {
            angle = r_u16(owner + 0xB0) & 0xFFF;
            other = (uint32)r_s16(0x8001F0C4 + angle * 2);
            speed = r_u32(owner + 0xA8);
            product = other * speed;
            value = (uint32)((sint32)product >> 17);
            angle = r_u16(owner + 0xB0);
        }
        speed = r_u32(owner + 0xA8);
        w_u16(owner + 0xA4, value);
        other = (uint32)r_s16(0x8001E8C4 + (angle & 0xFFF) * 2);
        value = r_u16(owner + 0xA4);
        product = other * speed;
        w_u16(owner + 0xA0, (uint32)((sint32)(sint16)value >> 2));
        w_u16(owner + 0xA2, (uint32)((sint32)product >> 19));
    }
    value = r_u16(owner + 0x98);
    other = r_u16(owner + 0xA0);
    third = r_u16(owner + 0xA2);
    fourth = r_u16(owner + 0xA4);
    w_u16(owner + 0x98, value + other);
    value = r_u16(owner + 0x9A);
    other = r_u16(owner + 0x9C);
    w_u16(owner + 0x9A, value + third);
    w_u16(owner + 0x9C, other + fourth);
    left = vertices[0];
    right = vertices[1];
    value = r_u32(left + 8);
    other = r_u32(right + 8);
    z = r_u32(owner + 0x70);
    other = (uint32)((sint32)(value + other) >> 1);
    if ((sint32)(z - other) >= 129)
        w_u32(owner + 0x70, other + 128);
    value = r_u32(owner + 0x70);
    if ((sint32)(value - other) < -128)
        w_u32(owner + 0x70, other - 128);
    value = r_u16(owner + 0x70);
    third = r_u16(owner + 0x98);
    value -= z;
    target = r_u32(owner + 0x134);
    w_u16(owner + 0x98, third + value);
    value = r_u8(target + 0x1E);
    target = vertices[value];
    value = (uint32)r_s16(owner + 0xB0);
    target_x = r_u32(target);
    target_z = r_u32(target + 8);
    other = r_u16(owner + 0xB0);
    if ((sint32)value < -0x800)
        w_u16(owner + 0xB0, other + 0x1000);
    value = (uint32)r_s16(owner + 0xB0);
    other = r_u16(owner + 0xB0);
    if ((sint32)value > 0x800)
        w_u16(owner + 0xB0, other - 0x1000);
    result = (uint32)r_s16(owner + 0xB2);
    if (!result)
        return result;
    x = r_u32(owner + 0x68);
    z = r_u32(owner + 0x70);
    result = t3_8004b39c(target_x - x, target_z - z);
    other = (uint32)r_s16(owner + 0xB2);
    w_u16(owner + 0xB0, result);
    if (!other)
        return result;
    speed = r_u32(owner + 0xA8);
    if ((sint32)speed <= 0x1000)
        return 1;
    value = r_u32(owner + 0x68) - target_x;
    product = value * value;
    value = r_u32(owner + 0x70) - target_z;
    frames = t3_8002a324(product + value * value);
    speed = r_u32(owner + 0xA8);
    frames = t3_camera_divide(frames, (uint32)((sint32)speed >> 8));
    value = r_u32(owner + 0x8C);
    product = frames * value;
    target = r_u32(owner + 0x134);
    vertical = r_u32(owner + 0x7C);
    height = r_u32(target + 0x950);
    value = r_u32(owner + 0x6C);
    value += (uint32)((sint32)(vertical + product) >> 8);
    value -= height;
    value <<= 8;
    result = t3_camera_divide(vertical - value, frames);
    result = (vertical * 13 + result * 3) << 8;
    result = (uint32)((sint32)result >> 12);
    w_u32(owner + 0x7C, result);
    return result;
}

uint32 t3_bns_03_800b094c(uint32 actor, uint32 opponent, uint32 incoming_collision)
{
    uint32 vertices[2];
    uint32 value;
    uint32 owner;
    uint32 first;
    uint32 second;
    uint32 highlighted;
    uint32 preserve;
    FUNCTION_MARKER(0x800B094C, "SLUS_004.02-BNS-logical-03");
    vertices[0] = actor + 0xF68;
    vertices[1] = opponent + 0xF68;
    value = r_u32(0x80096F2C);
    owner = r_u32(0x800ADD74);
    if (value == 1)
        w_u32(0x800AFAC4, 2);
    value = r_u32(0x800B6620);
    if ((sint32)value > 0)
        w_u32(0x800B6620, value - 1);
    if (!r_u32(0x80095468) && !r_u32(0x80095494)) {
        t3_800337a0(owner);
        if (!r_u32(0x800954A4)) {
            t3_bns_03_800b11dc(owner, actor, opponent, vertices,
                incoming_collision, opponent);
            t3_bns_03_800b2a4c(owner);
        } else if ((r_u32(owner + 0x144) & 0xFFFF0000) == 0x05020000) {
            t3_800753bc(0, 0x4E6E, 0);
        }
    }
    first = vertices[0];
    second = vertices[1];
    first = r_u32(first + 8);
    second = r_u32(second + 8);
    t3_bns_03_800b38dc(first, second, owner);
    if (r_s16(owner))
        t3_bns_03_800b2cd0(owner);
    if (r_u8(0x80098A41) == 2 && r_u8(0x800AFAA4) != 1)
        return 1;
    highlighted = 0;
    if (r_s16(owner + 0x140)) {
        value = r_u32(owner + 0x130);
        value = r_u8(value + 0x1E);
        highlighted = (uint32)1 << (value & 31);
    }
    first = (uint32)r_s16(owner + 0x13C);
    second = (uint32)r_s16(owner + 0x13E);
    t3_bns_03_800b49a8(first, 100, second, highlighted);
    preserve = 0;
    if (r_u32(0x80095494) || r_u32(0x80095468))
        preserve = 1;
    return t3_bns_03_800b3e8c(preserve);
}

uint32 t3_80066dfc_host(uint32 camera, uint32 actor, uint32 opponent,
    uint32 incoming_heading, uint32 incoming_result)
{
    uint32 kind;
    uint32 value;
    uint32 other;
    uint32 animation;
    uint32 heading = incoming_heading;
    uint32 first_point;
    uint32 middle_point;
    uint32 last_point;
    uint32 x;
    uint32 z;
    uint32 first;
    uint32 second;
    uint32 result;
    sint32 angle;
    FUNCTION_MARKER(0x80066DFC, "SLUS_004.02");
    if (camera) {
        w_u16(0x800A8CBA, 0);
        kind = t3_800663f8(camera, 0, 0);
        t3_800663f8(0, 0x800A0428, 0);
        angle = r_s16(opponent + 0x2C);
        angle /= 16;
        switch (kind) {
        case 0:
        case 4:
            heading = 0x400 - (uint32)angle;
            break;
        case 1:
            heading = 0x800 - (uint32)angle;
            break;
        case 2:
            heading = 0 - (uint32)angle;
            break;
        case 3:
        case 5:
            heading = 0 - ((uint32)angle + 0x400);
            break;
        }
        w_u32(0x800A8CB0, heading & 0xFFF);
        value = r_u32(opponent);
        w_u32(0x800A8CC4, value);
        value = r_u32(opponent + 4);
        w_u32(0x800A8CC8, value);
        value = r_u32(opponent + 8);
        w_u32(0x800A8CCC, value);
    } else {
        animation = r_u32(actor + 0x54);
        value = r_u32(0x800A8CA8);
        if (animation != value && r_s16(actor + 0x74) > 0
            && r_u8(animation + 0x24) != 37) {
            w_u32(0x800A8CA8, animation);
            animation = r_u32(actor + 0x54);
            value = r_u8(animation + 0x24);
            if (value < 43) {
                first = r_u16(actor + 0xF68);
                second = r_u16(actor + 0xF70);
                first_point = first | (second << 16);
                first = r_u32(actor + 0xF68);
                second = r_u32(opponent + 0xF68);
                value = first + second;
                x = (uint32)((sint32)value / 2);
                z = r_u16(0x800A042C);
                first = r_u32(actor + 0xF70);
                second = r_u32(opponent + 0xF70);
                other = r_u16(0x800A0428);
                value = first + second;
                last_point = other | (z << 16);
                z = (uint32)((sint32)value / 2);
                middle_point = (x & 0xFFFF) | (z << 16);
                // Pass packed coordinates to the existing NormalClip ABI
                result = (uint32)NormalClip((sint32)first_point,
                    (sint32)middle_point, (sint32)last_point);
                t3_80062e44(result >> 31, actor, opponent);
                return 0xFFFFFFFF;
            }
            value = t3_800673e4(actor);
            w_u32(0x800A8CA0, value);
            result = t3_800674e0(value);
            value = r_u32(0x800A8CA0);
            w_u16(0x800A8CA4, result);
            t3_800663f8(value, 0, 0);
            t3_800663f8(0, 0x800A0428, 0);
            w_u16(0x800A8CBA, 0);
        }
    }
    if (r_s16(0x800A8CA4) < 0)
        return t3_800666f0();
    t3_80062a10();
    return incoming_result;
}

uint32 t3_8006895c(uint32 actor, uint32 defeated)
{
    uint32 camera[9];
    uint32 first;
    uint32 second;
    uint32 dx;
    uint32 dz;
    uint32 distance;
    uint32 threshold;
    uint32 height;
    uint32 correction;
    uint32 y;
    uint32 index;
    sint32 character;

    FUNCTION_MARKER(0x8006895C, "SLUS_004.02");
    w_u16(0x800A8C74, 0);
    if (!(defeated & 1)) {
        first = r_u32(0x800A04F4);
        second = r_u32(0x800A04E8);
        dx = first - second;
        first = r_u32(0x800A04FC);
        second = r_u32(0x800A04F0);
        dz = first - second;
        distance = t3_8004aedc(dx * dx + dz * dz);
        character = r_s16(actor + 0x16);
        height = r_u32(actor + 0x994);
        threshold = distance - (character == 11 ? 6282 : 3664);
        if ((sint32)threshold > 0) {
            dx = (uint32)((sint64)(sint32)(dx << 1) / (sint32)distance);
            dz = (uint32)((sint64)(sint32)(dz << 1) / (sint32)distance);
            first = r_u32(0x800A04E8);
            second = r_u32(0x800A04F0);
            w_u32(0x800A04E8, first + dx);
            w_u32(0x800A04F0, second + dz);
        }
        first = r_u32(actor + 0xF68);
        second = r_u32(0x800A04F4);
        correction = (first - second) * 6;
        if ((sint32)correction < 0)
            correction += 127;
        y = r_u32(0x800A04F8);
        w_u32(0x800A04F4, second + (uint32)((sint32)correction >> 7));
        correction = (height - y) * 6;
        if ((sint32)correction < 0)
            correction += 127;
        y += (uint32)((sint32)correction >> 7);
        w_u32(0x800A04F8, y);
        first = r_u32(actor + 0xF70);
        second = r_u32(0x800A04FC);
        correction = (first - second) * 6;
        if ((sint32)correction < 0)
            correction += 127;
        w_u32(0x800A04FC, second + (uint32)((sint32)correction >> 7));
        if (r_s16(0x800A8CDC) == 2) {
            first = r_u32(0x800A04EC);
            if ((sint32)(first - y) < -400)
                w_u32(0x800A04F8, first + 400);
        }
        t3_80045ee8(0x800A04E8, camera);
    } else {
        if (r_u32(0x800AFA88) != 7)
            w_u8(0x800B040C, 1);
        first = (uint32)(r_s16(actor + 0x2C) / 16);
        camera[0] = 1024;
        second = r_u32(0x800A050C);
        camera[2] = 0;
        camera[1] = (3072 - first) + (second - 1024);
        second = r_u32(0x800A0508);
        first = r_u32(actor + 0xF68);
        camera[6] = (uint32)-7853 - second;
        camera[5] = first;
        camera[7] = r_u32(actor + 0xF70);
    }
    // Preserve prior camera words instead of undefined stack data
    camera[3] = r_u32(0x800A85CC);
    camera[4] = r_u32(0x800A85D0);
    camera[8] = r_u32(0x800A85E0);
    // Adapt the complete 461C0 publication to native local storage
    for (index = 0; index < 9; ++index)
        w_u32(0x800A85C0 + index * 4, camera[index]);
    return t3_80045ca8(0x800A85C0, 0x800ADF70);
}

uint32 t3_8004bec0(const char *path, void *header_buffer, uint32 header_bytes, void *data_buffer, uint32 data_bytes, uint32 mode)
{
    uint8 *header = (uint8 *)header_buffer;
    uint8 *data = (uint8 *)data_buffer;
    char title[136];
    uint32 index;
    uint32 frame;
    uint32 row;
    uint32 word;
    uint32 value;
    uint32 source;
    uint32 destination;
    uint32 checksum = 0;
    uint32 previous;
    sint32 descriptor;

    FUNCTION_MARKER(0x8004BEC0, "SLUS_004.02");
    for (index = 0; index < 512; ++index)
        header[index] = 0;
    t3_8004bcc0(title);
    header[0] = 0x53;
    header[1] = 0x43;
    header[2] = 0x13;
    header[3] = 1;
    strcpy((char *)header + 4, title);
    for (index = 0; index < 8; ++index) {
        value = r_u32(0x80021A0C + index * 4);
        destination = 96 + index * 4;
        header[destination] = (uint8)value;
        header[destination + 1] = (uint8)(value >> 8);
        header[destination + 2] = (uint8)(value >> 16);
        header[destination + 3] = (uint8)(value >> 24);
    }
    destination = 128;
    for (frame = 0; frame < 3; ++frame) {
        source = 0x80021A38 + frame * 8;
        for (row = 0; row < 16; ++row) {
            for (word = 0; word < 2; ++word) {
                value = r_u32(source);
                source += 4;
                header[destination] = (uint8)value;
                header[destination + 1] = (uint8)(value >> 8);
                header[destination + 2] = (uint8)(value >> 16);
                header[destination + 3] = (uint8)(value >> 24);
                destination += 4;
            }
            source += 16;
        }
    }
    for (index = 0; index < 72; ++index)
        data[index + 1] = r_u8(0x80097EF0 + index);
    for (index = 0; index < 432; ++index)
        data[index + 73] = r_u8(0x80097F4C + index);
    // The final seven payload bytes belong to the caller
    for (index = 1; index < 512; ++index)
        checksum += data[index];
    previous = 0 - checksum;
    data[0] = (uint8)previous;
    for (index = 1; index < 512; ++index) {
        previous = data[index] + (previous & 255) * 5 + 1;
        data[index] = (uint8)previous;
    }
    descriptor = openPSX(path, 2);
    if (mode == 0xFFFFFFFF) {
        if (descriptor < 0)
            return 5;
    } else {
        if (descriptor < 0) {
            if (!mode)
                return 5;
            descriptor = openPSX(path, 0x10202);
            if (descriptor < 0)
                return 4;
        }
        if ((uint32)writePSX(descriptor, header, header_bytes) != header_bytes ||
            (uint32)writePSX(descriptor, data, data_bytes) != data_bytes) {
            closePSX(descriptor);
            return 1;
        }
    }
    closePSX(descriptor);
    return 0;
}

uint32 t3_8004c138(uint32 ignored, uint32 mode)
{
    char path[40];
    uint8 header[512];
    uint8 data[512];
    DIRENTRY record;
    uint32 result;
    char *end;

    FUNCTION_MARKER(0x8004C138, "SLUS_004.02");
    (void)ignored;
    if (!firstfile((const char *)psx_addr(0x800219D4, 7), &record)) {
        result = t3_8004c51c(0);
        if (result)
            return result;
    }
    end = t3_copy_guest_string_to_native(path, 0x800219CC);
    t3_copy_guest_string_to_native(end, 0x8002172C);
    // Initialize only the payload bytes not filled by the writer
    memset(data + 505, 0, 7);
    return t3_8004bec0(path, header, 512, data, 512, mode);
}
