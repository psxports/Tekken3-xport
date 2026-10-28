#include "game_functions.h"

uint32 t3_bns_09_800ee250(uint32 state, uint32 selected, uint32 entries)
{
    uint32 history;
    uint32 ordering;
    uint32 first_consumed = 0;
    uint32 second_consumed = 0;
    uint32 first_style = 0;
    uint32 second_style = 0;
    uint32 background = 0x404040;
    uint32 row = 0;
    uint32 kind;
    uint32 first;
    uint32 second;
    uint32 x;
    uint32 y;
    uint32 packet;
    uint32 total;
    uint32 top;
    uint32 xy[4];
    uint32 corner;
    uint32 result;
    sint32 quotient;
    FUNCTION_MARKER(0x800EE250, "SLUS_004.02-BNS-logical-09");
    history = r_u8(state + 0x58);
    ordering = r_u32(0x800A9218);
    if ((sint32)history < (sint32)selected)
        selected = history;
    while ((sint32)row < (sint32)selected)
    {
        kind = r_u16(state + 0x38 + row * 2);
        if (kind == 1)
        {
            background = 0xFF;
            first_style = 2;
            second_style = 0x60;
        }
        else if (kind == 2)
        {
            background = 0xC0FF;
            first_style = 0x60;
            second_style = 1;
        }
        else if (kind == 3)
        {
            background = 0xC0C0C0;
            first_style = 0;
            second_style = 0;
        }
        first = r_u8(entries + first_consumed);
        second = r_u8(entries + second_consumed + 13);
        if (first >= 88)
            first = 88;
        if (second >= 88)
            second = 88;
        kind = r_u16(state + 0x38 + row * 2);
        if (kind == 2 || kind == 3)
            ++first_consumed;
        if (kind == 1 || kind == 3)
            ++second_consumed;
        quotient = (sint32)row / 3;
        x = (row - (uint32)quotient * 3) * 96;
        y = (uint32)quotient * 30;
        packet = r_u32(0x800ADF64);
        total = r_u8(state + 0x65);
        total += r_u8(state + 0x72);
        top = (5u - (total + 1) / 3) * 30;
        packet = t3_bns_09_800f0cfc(ordering, packet, x + 71, y + top + 294, first, first_style);
        w_u32(0x800ADF64, packet);
        total = r_u8(state + 0x65);
        total += r_u8(state + 0x72);
        top = (5u - (total + 1) / 3) * 30;
        packet = t3_bns_09_800f0cfc(ordering, packet, x + 111, y + top + 294, second, second_style);
        w_u32(0x800ADF64, packet);
        total = r_u8(state + 0x65);
        total += r_u8(state + 0x72);
        top = (5u - (total + 1) / 3) * 30;
        ++row;
        t3_8004ce74(0x800B8E74, 6, 0, x + 44, y + top + 306, row);
        for (corner = 0; corner < 4; ++corner)
        {
            total = r_u8(state + 0x65);
            total += r_u8(state + 0x72);
            top = (5u - (total + 1) / 3) * 30;
            xy[corner] = ((x + ((corner & 1) ? 135 : 40)) & 0xFFFF) |
                         ((y + top + (corner < 2 ? 309 : 323)) << 16);
        }
        packet = r_u32(0x800ADF64);
        packet = t3_8004d8b0(ordering, packet, 0x38000000, 0, background, background, xy[0], xy[1], xy[2], xy[3]);
        w_u32(0x800ADF64, packet);
        total = r_u8(state + 0x65);
        total += r_u8(state + 0x72);
        top = (5u - (total + 1) / 3) * 30;
        packet = t3_8004da10(ordering, packet, 0x60000000, ((x + 40) & 0xFFFF) | ((y + top + 294) << 16), 0x1D005F);
        w_u32(0x800ADF64, packet);
    }
    first = r_u8(state + 0x65);
    second = r_u8(state + 0x72);
    row = selected;
    result = (sint32)row < (sint32)(first + second - 1);
    while (result != 0)
    {
        quotient = (sint32)row / 3;
        x = (row - (uint32)quotient * 3) * 96;
        y = (uint32)quotient * 30;
        packet = r_u32(0x800ADF64);
        top = (5u - (first + second + 1) / 3) * 30;
        packet = t3_8004da10(ordering, packet, 0x60201010, ((x + 71) & 0xFFFF) | ((y + top + 294) << 16), 0x1D0010);
        w_u32(0x800ADF64, packet);
        total = r_u8(state + 0x65);
        total += r_u8(state + 0x72);
        top = (5u - (total + 1) / 3) * 30;
        packet = t3_8004da10(ordering, packet, 0x60201010, ((x + 111) & 0xFFFF) | ((y + top + 294) << 16), 0x1D0010);
        w_u32(0x800ADF64, packet);
        total = r_u8(state + 0x65);
        total += r_u8(state + 0x72);
        top = (5u - (total + 1) / 3) * 30;
        ++row;
        t3_8004ce74(0x800B8E74, 26, 0, x + 44, y + top + 306, row);
        for (corner = 0; corner < 4; ++corner)
        {
            total = r_u8(state + 0x65);
            total += r_u8(state + 0x72);
            top = (5u - (total + 1) / 3) * 30;
            xy[corner] = ((x + ((corner & 1) ? 135 : 40)) & 0xFFFF) |
                         ((y + top + (corner < 2 ? 309 : 323)) << 16);
        }
        packet = r_u32(0x800ADF64);
        packet = t3_8004d8b0(ordering, packet, 0x38000000, 0, 0x404040, 0x404040, xy[0], xy[1], xy[2], xy[3]);
        w_u32(0x800ADF64, packet);
        total = r_u8(state + 0x65);
        total += r_u8(state + 0x72);
        top = (5u - (total + 1) / 3) * 30;
        packet = t3_8004da10(ordering, packet, 0x60000000, ((x + 40) & 0xFFFF) | ((y + top + 294) << 16), 0x1D005F);
        w_u32(0x800ADF64, packet);
        first = r_u8(state + 0x65);
        second = r_u8(state + 0x72);
        result = (sint32)row < (sint32)(first + second - 1);
    }
    return result;
}

uint32 t3_bns_04_8010f880(uint32 state)
{
    uint32 ordering;
    uint32 packet;
    uint32 side;
    uint32 control;
    uint32 phase;
    uint32 layer;
    uint32 shade;
    uint32 row;
    uint32 x;
    uint32 y;
    uint32 offset;
    uint32 color;
    uint32 texture;
    uint32 index;
    uint32 entry;
    uint32 xy;
    uint32 dimensions;
    FUNCTION_MARKER(0x8010F880, "SLUS_004.02-BNS-logical-04");
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    for (side = 0; side < 2; ++side)
    {
        control = state + 44 + side * 124;
        phase = r_u32(control);
        if (phase == 0)
            continue;
        if (phase == 1 || phase == 3)
        {
            layer = 2;
            shade = t3_8004dfe0(r_u32(0x800AE218) << 4);
        }
        else
        {
            layer = 0;
            shade = 128;
        }
        row = 0x801296C8 + r_u32(control - 4) * 12;
        if (r_u8(row + 5) == 0)
            continue;
        x = (uint32)r_s16(row + 8);
        y = (uint32)r_s16(row + 10);
        offset = r_u32(state + 300);
        x -= 2;
        y += offset;
        color = t3_8004df6c(0xFFFFFF, shade);
        phase = r_u32(control);
        texture = r_u32(control + ((phase - 3) < 2 ? 104 : 100));
        xy = r_u16(control + 114);
        offset = r_u32(state + 316);
        xy = ((x + xy) & 0xFFFF) | ((y + offset) << 16);
        packet = t3_8004d9bc(ordering + 12, packet, color | 0x64000000, xy, 0x100014, texture);
        packet = t3_8004daa8(ordering + 12, packet, 14);
        phase = r_u32(state + 4);
        texture = r_u32(control + (phase != 0 ? 96 : 92));
        phase = r_u32(control);
        if (phase - 3 < 2)
            texture = (texture & 0xFFFF) | 0x7CA90000;
        y += r_u32(state + 312);
        for (index = 0; index < 2; ++index)
        {
            entry = ordering + layer * 4;
            if (index == side)
                entry += 4;
            xy = (x & 0xFFFF) | (y << 16);
            dimensions = (r_u32(state + 308) << 16) | 18;
            x += 18;
            packet = t3_8004d9bc(entry, packet, color | 0x64000000, xy, dimensions, texture);
            packet = t3_8004daa8(entry, packet, 25);
            texture = (texture & 0xFFFFFF00) | ((texture + 18) & 255);
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_bns_04_8010e7dc(uint32 state, uint32 label)
{
    uint32 ordering;
    uint32 side;
    uint32 control;
    uint32 x;
    uint32 kind;
    uint32 y;
    uint32 width;
    uint32 color;
    uint32 packet;
    uint32 texture;
    uint32 mode;
    uint32 end;
    FUNCTION_MARKER(0x8010E7DC, "SLUS_004.02-BNS-logical-04");
    ordering = r_u32(0x800A9218);
    for (side = 0; side < 2; ++side)
    {
        control = state + 76 + side * 124;
        x = r_u16(control + 46);
        kind = r_u32(control - 28);
        y = r_u32(state + 292);
        if (kind == 1 || kind == 2)
        {
            if (kind == 2)
            {
                color = r_u32(state + 328);
                t3_8004eee8(side, x, 34, color);
            }
            width = r_u32(control + 4);
            if (width != 0)
            {
                packet = r_u32(0x800ADF64);
                texture = r_u32(control);
                texture = ((texture << 3) & 0x80) | ((texture & 15) << 12) | 0x7CA50000;
                packet = t3_8004d9bc(ordering, packet, 0x65000000, ((x - (width >> 1)) & 0xFFFF) | (y << 16), (width & 0xFFFF) | 0x100000, texture);
                w_u32(0x800ADF64, packet);
                packet = t3_8004daa8(ordering, packet, 11);
                w_u32(0x800ADF64, packet);
            }
        }
        else if (kind == 3)
        {
            mode = r_u32(state);
            switch (mode)
            {
            case 0: case 1: case 2: case 6: case 7:
                label = 0x800B90DC;
                break;
            case 3:
                label = 0x800B90A4;
                break;
            case 4:
                label = 0x800B90B4;
                break;
            case 5:
                label = 0x800B90C0;
                break;
            case 8:
                label = 0x800B90CC;
                break;
            }
            width = t3_8007a418(label) * 13;
            x -= (uint32)((sint32)width >> 1);
            end = x + width;
            if ((sint32)x < 8)
            {
                x = 8;
                end = x + width;
            }
            if ((sint32)end >= 361)
                x = 360 - width;
            t3_8004ce74(0x800B90E0, (uint32)5, (uint32)1, x, y - 30, label);
        }
    }
    return 0;
}

void t3_bns_04_8010e40c(uint32 state)
{
    uint32 frame;
    uint32 side;
    uint32 player;
    uint32 previous;
    uint32 character;
    uint32 costume;
    uint32 descriptor;
    uint32 flags;
    uint32 first;
    uint32 second;
    uint32 flip;
    uint32 portrait;
    uint32 packed;
    uint32 clut_x;
    uint32 clut_y;
    uint32 count;
    uint32 image_x;
    uint32 image_y;
    uint32 source;
    uint32 destination = state + 1612;
    T3_TIM_IMAGE image;
    FUNCTION_MARKER(0x8010E40C, "SLUS_004.02-BNS-logical-04");
    frame = r_u32(state + 8) + 1;
    side = frame & 1;
    w_u32(state + 8, frame);
    player = state + 36 + side * 124;
    previous = r_u32(player + 20);
    character = r_u32(player + 28);
    if (previous == character)
    {
        previous = r_u32(player + 24);
        costume = r_u32(player + 32);
        if (previous == costume)
            return;
    }
    previous = r_u32(player + 28);
    costume = r_u32(player + 32);
    w_u32(player + 20, character);
    w_u32(player + 24, costume);
    descriptor = t3_8004b790((sint32)previous, (sint32)costume);
    character = r_u32(player + 28);
    costume = r_u32(player + 32);
    flags = r_u8(descriptor);
    descriptor = t3_8004eff0(character, costume);
    first = r_u8(descriptor + 5);
    w_u32(player + 44, first);
    first = r_u8(descriptor + 4);
    flip = r_u16(player + 96);
    w_u32(player + 40, first);
    if (((flags & 0x20) != 0) != (flip == 1))
        w_u32(player + 16, r_u32(player + 20) != 22);
    else
        w_u32(player + 16, 0);
    portrait = flags & 31;
    packed = r_u16(player + 92);
    clut_x = (packed & 63) << 4;
    clut_y = packed >> 6;
    t3_800524d4(side, portrait);
    count = r_u32(0x800B912C);
    image_x = r_u16(player + 88);
    image_y = r_u16(player + 90);
    if ((sint32)portrait >= (sint32)count)
        return;
    source = 0x800B912C + r_u32(0x800B9130 + portrait * 8);
    if ((sint32)t3_80031bfc(source, destination) > 33344)
        t3_8007ae58(1);
    if (t3_8007efc8(destination) == 0xFFFFFFFF)
        t3_8007ae58(1);
    t3_8007efd8(&image);
    w_u16(image.image_rectangle, (uint16)image_x);
    w_u16(image.image_rectangle + 2, (uint16)image_y);
    w_u16(image.clut_rectangle, (uint16)clut_x);
    w_u16(image.clut_rectangle + 2, (uint16)clut_y);
    first = r_u16(image.image_rectangle);
    second = r_u16(image.image_rectangle + 2);
    if ((first | second) != 0)
        t3_80029348(image.image_rectangle, image.image_pixels);
    if ((image.mode & 8) != 0)
    {
        first = r_u16(image.clut_rectangle);
        second = r_u16(image.clut_rectangle + 2);
        if ((first | second) != 0)
            t3_80029348(image.clut_rectangle, image.clut_pixels);
    }
}

uint32 t3_800524d4(uint32 side, uint32 portrait)
{
    FUNCTION_MARKER(0x800524D4, "SLUS_004.02");
    w_u8(0x80098106 + side, (uint8)(portrait & 31));
    return 0x80098104;
}

uint32 t3_80029348(uint32 rectangle, uint32 pixels)
{
    uint32 count;
    uint32 destination;
    uint32 component;
    FUNCTION_MARKER(0x80029348, "SLUS_004.02");
    count = r_u32(0x8009BC54);
    if ((sint32)count >= 32)
        return 1;
    destination = r_u32(0x8009BC50);
    w_u16(destination, 1);
    w_u32(destination + 4, pixels);
    component = r_u16(rectangle);
    w_u16(destination + 8, (uint16)component);
    component = r_u16(rectangle + 2);
    w_u16(destination + 10, (uint16)component);
    component = r_u16(rectangle + 4);
    w_u32(0x8009BC54, count + 1);
    w_u16(destination + 12, (uint16)component);
    component = r_u16(rectangle + 6);
    w_u32(0x8009BC50, destination + 16);
    w_u16(destination + 14, (uint16)component);
    return 0;
}

uint32 t3_bns_04_8010f140(uint32 state)
{
    uint32 packet;
    uint32 ordering;
    uint32 side;
    uint32 player;
    uint32 x;
    uint32 y;
    uint32 u;
    uint32 v;
    uint32 width;
    uint32 timer;
    uint32 scale;
    uint32 clut;
    uint32 character;
    uint32 costume;
    uint32 page;
    uint32 descriptor;
    uint32 previous;
    uint32 next;
    uint32 table;
    uint32 section;
    uint32 remaining;
    uint32 top_shade;
    uint32 bottom_shade;
    uint32 boundary;
    uint32 consumed;
    uint32 step;
    uint32 clut_step;
    uint32 top_color;
    uint32 bottom_color;
    uint32 xy[4];
    uint32 uv[3];
    uint32 bottom;
    FUNCTION_MARKER(0x8010F140, "SLUS_004.02-BNS-logical-04");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    for (side = 0; side < 2; ++side)
    {
        player = state + 60 + side * 124;
        x = r_u16(player + 60);
        y = r_u32(state + 288);
        u = r_u16(player + 64);
        v = r_u16(player + 66);
        u = (u << 1) & 127;
        width = 126;
        if (r_u32(player - 8) == 1)
        {
            x += 126;
            width = (uint32)-126;
        }
        timer = r_u32(player + 24);
        scale = 32;
        if (timer != 0)
        {
            scale -= timer;
            w_u32(player + 24, timer - 1);
        }
        boundary = 64;
        consumed = 0;
        clut = r_u16(player + 68);
        character = r_u32(player - 4);
        costume = r_u32(player);
        page = r_u16(player + 70);
        descriptor = t3_8004b790((sint32)character, (sint32)costume);
        previous = r_u32(0x801185AC);
        table = 0x8011857C + (r_u8(descriptor) >> 6) * 12;
        for (section = 0; section < 3; ++section)
        {
            top_shade = previous * scale;
            next = r_u32(0x801185B0 + section * 4);
            bottom_shade = next * scale;
            previous = next;
            top_shade = (uint32)((sint32)top_shade >> 5);
            remaining = r_u32(table + section * 4);
            bottom_shade = (uint32)((sint32)bottom_shade >> 5);
            while (remaining != 0)
            {
                clut_step = 0;
                step = remaining;
                if (boundary < consumed + remaining)
                {
                    clut_step = 4;
                    step = boundary - consumed;
                    boundary += 64;
                }
                if (step != 0)
                {
                    if (r_u8(descriptor) == 21)
                    {
                        top_color = t3_8004df6c(0x7F7F7F, top_shade);
                        bottom_color = t3_8004df6c(0x7F7F7F, bottom_shade);
                        xy[0] = (x & 0xFFFF) | (y << 16);
                        xy[1] = ((x + width) & 0xFFFF) | (y << 16);
                        xy[2] = (x & 0xFFFF) | ((y + step) << 16);
                        xy[3] = ((x + width) & 0xFFFF) | ((y + step) << 16);
                        uv[0] = u | ((v & 255) << 8) | (clut << 16);
                        uv[1] = (u + 125) | ((v & 255) << 8) | ((page | 0x40) << 16);
                        bottom = ((v + step) & 255) << 8;
                        uv[2] = u | bottom | (((u + 125) | bottom) << 16);
                        if (top_shade != bottom_shade)
                            packet = t3_8004d924(ordering, packet, top_color | 0x3E000000, top_color, bottom_color, bottom_color, xy[0], xy[1], xy[2], xy[3], uv[0], uv[1], uv[2]);
                        else if (bottom_shade != 0)
                            packet = t3_8004d830(ordering, packet, top_color | 0x2E000000, xy[0], xy[1], xy[2], xy[3], uv[0], uv[1], uv[2]);
                    }
                    else
                        packet = t3_bns_04_8010ed6c(ordering, packet, top_shade, bottom_shade, x, y, width, step, u, v, 125, step, clut, page);
                }
                y += step;
                v += step;
                remaining -= step;
                clut += clut_step;
                consumed += step;
            }
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_bns_04_8010ed6c(uint32 ordering, uint32 packet, uint32 top_shade, uint32 bottom_shade, uint32 x, uint32 y, uint32 width, uint32 height, uint32 u, uint32 v, uint32 texture_width, uint32 texture_height, uint32 clut, uint32 page)
{
    uint32 top_color;
    uint32 bottom_color;
    uint32 xy[4];
    uint32 uv_top;
    uint32 uv_right;
    uint32 uv_bottom;
    uint32 right;
    uint32 bottom;
    FUNCTION_MARKER(0x8010ED6C, "SLUS_004.02-BNS-logical-04");
    top_color = t3_8004df6c(0x7F7F7F, top_shade);
    bottom_color = t3_8004df6c(0x7F7F7F, bottom_shade);
    if (top_shade == bottom_shade && top_shade == 0)
        return packet;
    right = (x + width) & 0xFFFF;
    bottom = (y + height) << 16;
    xy[0] = (x & 0xFFFF) | (y << 16);
    xy[1] = right | (y << 16);
    xy[2] = (x & 0xFFFF) | bottom;
    xy[3] = right | bottom;
    uv_top = (u & 255) | ((v & 255) << 8);
    uv_right = ((u + texture_width) & 255) | ((v & 255) << 8);
    bottom = ((v + texture_height) & 255) << 8;
    uv_bottom = (u & 255) | bottom | ((((u + texture_width) & 255) | bottom) << 16);
    if (top_shade == bottom_shade)
    {
        if ((sint32)top_shade >= 256)
            return t3_8004d830(ordering, packet, 0x2D000000, xy[0], xy[1], xy[2], xy[3], uv_top | (clut << 16), uv_right | (page << 16), uv_bottom);
        packet = t3_8004d830(ordering, packet, top_color | 0x2E000000, xy[0], xy[1], xy[2], xy[3], uv_top | (clut << 16), uv_right | ((page | 0x20) << 16), uv_bottom);
        return t3_8004d830(ordering, packet, top_color | 0x2E000000, xy[0], xy[1], xy[2], xy[3], uv_top | 0x7FD00000, uv_right | ((page | 0x40) << 16), uv_bottom);
    }
    packet = t3_8004d924(ordering, packet, top_color | 0x3E000000, top_color, bottom_color, bottom_color, xy[0], xy[1], xy[2], xy[3], uv_top | (clut << 16), uv_right | ((page | 0x20) << 16), uv_bottom);
    return t3_8004d924(ordering, packet, top_color | 0x3E000000, top_color, bottom_color, bottom_color, xy[0], xy[1], xy[2], xy[3], uv_top | 0x7FD00000, uv_right | ((page | 0x40) << 16), uv_bottom);
}

uint32 t3_bns_04_8010ff4c(void)
{
    uint32 state = 0x80118628;
    sint32 phase;
    uint32 value;
    uint32 other;
    uint32 side;
    uint32 source;
    uint32 destination;
    uint32 offset;
    uint32 words[4];
    uint32 saved_packet;
    uint32 frame;
    uint32 ordering;
    uint32 packet;
    uint32 numerator;
    uint32 denominator;
    uint32 quotient;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x8010FF4C, "SLUS_004.02-BNS-logical-04");
    if (r_s16(0x800AE224) > 0 && r_s32(state + 8) >= 3 && t3_8005101c() != 0)
        return 1;
    phase = r_s16(0x800AE224);
    if (phase == 0)
    {
        t3_8006c588(20);
        t3_800697d4(1);
        t3_8004ca40(0x8011033C, state);
        w_u32(state, r_u32(0x800AFA88));
        t3_bns_04_8010d5e4(state);
        t3_8004b688(0);
        t3_8006ae14(1, 0);
        source = 0x801102DC + r_u32(state + 4) * 48;
        destination = state + 284;
        for (offset = 0; offset < 48; offset += 16)
        {
            words[0] = r_u32(source + offset);
            words[1] = r_u32(source + offset + 4);
            words[2] = r_u32(source + offset + 8);
            words[3] = r_u32(source + offset + 12);
            w_u32(destination + offset, words[0]);
            w_u32(destination + offset + 4, words[1]);
            w_u32(destination + offset + 8, words[2]);
            w_u32(destination + offset + 12, words[3]);
        }
        w_u32(state + 8, 0);
        w_u32(state + 12, 2);
        for (side = 0; side < 2; ++side)
        {
            w_u32(state + 36 + side * 124, 0);
            value = r_u8(0x80097F0E + side);
            w_u32(state + 96 + side * 124, value >> 2);
            w_u32(state + 100 + side * 124, value & 3);
            t3_bns_04_8010dc30(state, side);
        }
        for (offset = 0; offset < 1280; offset += 20)
        {
            w_u16(state + 332 + offset, 0);
            w_u16(state + 334 + offset, 0);
        }
        w_u16(0x800AE224, 1);
    }
    else if (phase == 1)
    {
        value = r_u32(state + 20);
        if ((sint32)value > 0)
            w_u32(state + 20, value - 1);
        value = t3_bns_04_8010dc30(state, 0) != 0;
        other = t3_bns_04_8010dc30(state, 1);
        if (value != 0 && other != 0)
            w_u16(0x800AE224, 2);
    }
    else if (phase == 2)
    {
        t3_bns_04_8010e328(0x800AFA88, state);
        value = r_u8(0x800AFA9C);
        other = r_u8(0x800AFA9D);
        w_u16(0x800AE204, (uint16)value);
        w_u16(0x800AE224, (uint16)other);
    }
    t3_bns_04_8010e40c(state);
    frame = r_u32(0x800ADEFC);
    saved_packet = r_u32(0x800ADF64);
    value = r_u32(state + 8);
    w_u32(0x800ADF64, state + frame * 17920 + 33940);
    if ((sint32)value >= 3)
    {
        t3_bns_04_8010eb94(state);
        t3_bns_04_8010f140(state);
        t3_bns_04_8010e67c(state);
        numerator = r_u32(state + 20);
        denominator = r_u32(state + 24);
        numerator += denominator - 1;
        if (denominator == 0)
            quotient = (sint32)numerator < 0 ? 1 : 0xFFFFFFFF;
        else if (numerator == 0x80000000 && denominator == 0xFFFFFFFF)
            quotient = numerator;
        else
            quotient = (uint32)((sint32)numerator / (sint32)denominator);
        value = r_u32(state + 320);
        other = r_u32(state + 324);
        t3_8004e128(quotient, 184, value, other);
        t3_bns_04_8010e7dc(state, saved_packet);
        t3_bns_04_8010f880(state);
        t3_bns_04_8010f744(state);
        t3_bns_04_8010fb34(state);
        value = r_u32(state + 12);
        if ((sint32)value > 0)
            w_u32(state + 12, value - 1);
        else
        {
            words[0] = r_u32(0x800AE230);
            words[1] = r_u32(0x800AE234);
            rectangle.x = (sint16)words[0];
            rectangle.y = (sint16)(words[0] >> 16);
            rectangle.w = (sint16)words[1];
            rectangle.h = (sint16)(words[1] >> 16);
            ordering = r_u32(0x800A9218);
            value = r_u16(state + 296);
            other = r_u16(0x800AE232);
            packet = r_u32(0x800ADF64);
            rectangle.h = (sint16)(value - other);
            packet = t3_8004db28(ordering, packet, &rectangle);
            w_u32(0x800ADF64, packet);
        }
        t3_bns_04_8010f5dc(state);
        t3_bns_04_8010fc80(state);
    }
    w_u32(0x800ADF64, saved_packet);
    return 0;
}

void t3_8010ff4c(void)
{
    t3_bns_04_8010ff4c();
}

uint32 t3_bns_04_8010fc80(uint32 state)
{
    uint32 first;
    uint32 second;
    uint32 ordering;
    uint32 packet;
    uint32 frame;
    uint32 y;
    uint32 scroll;
    uint32 shift;
    uint32 crop;
    uint32 height;
    uint32 tile_row;
    uint32 table;
    uint32 x;
    uint32 width;
    uint32 inset;
    uint32 index;
    uint32 texture;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x8010FC80, "SLUS_004.02-BNS-logical-04");
    first = r_u32(0x800AE230);
    second = r_u32(0x800AE234);
    rectangle.x = (sint16)first;
    rectangle.y = (sint16)(first >> 16);
    rectangle.w = (sint16)second;
    rectangle.h = (sint16)(second >> 16);
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    packet = t3_8004db28(ordering, packet, &rectangle);
    frame = r_u32(0x800AE218);
    y = (uint32)r_s16(0x800AE232);
    scroll = 0u - (frame << 7);
    shift = (uint32)((sint32)scroll >> 4);
    crop = (64 - shift) & 63;
    height = 64 - crop;
    tile_row = 7 - (uint32)((sint32)(shift - 1) >> 6);
    w_u32(0x80118624, scroll);
    while ((sint32)y < 341)
    {
        table = 0x801185BC + (tile_row & 7) * 8;
        x = 0;
        inset = 8;
        width = 40;
        for (index = 0; index < 8; ++index)
        {
            texture = r_u8(table++);
            texture = r_u32(0x801185FC + texture * 4);
            if (inset != 0)
                texture += inset;
            if (crop != 0)
                texture += crop << 8;
            packet = t3_8004d9bc(ordering, packet, 0x65000000, (x & 0xFFFF) | (y << 16), width | (height << 16), texture);
            x += width;
            width = 48;
            inset = 0;
        }
        y += height;
        height = 64;
        crop = 0;
        ++tile_row;
    }
    packet = t3_8004daa8(ordering, packet, 24);
    first = r_u16(0x800AE232);
    second = r_u16(state + 296);
    rectangle.h = (sint16)(second - first);
    packet = t3_8004db28(ordering, packet, &rectangle);
    packet = t3_8004db9c(ordering, packet, 128, 58, 110, 32, 0x24D, 0x112, 0x7CA4, 0);
    packet = t3_8004db9c(ordering, packet, 84, 98, 198, 14, 0x242, 0x13A, 0x7CA4, 0);
    packet = t3_8004db9c(ordering, packet, 128, 120, 112, 30, 0x24D, 0x150, 0x7CA4, 0);
    y = r_u32(state + 296);
    packet = t3_8004da10(ordering, packet, 0x60000000, 0, (y << 16) | 368);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_04_8010fb34(uint32 state)
{
    uint32 packet;
    uint32 alternate;
    uint32 ordering;
    uint32 row;
    uint32 values[8];
    uint32 y;
    uint32 color;
    FUNCTION_MARKER(0x8010FB34, "SLUS_004.02-BNS-logical-04");
    packet = r_u32(0x800ADF64);
    alternate = r_u32(state + 4);
    ordering = r_u32(0x800A9218);
    row = alternate != 0 ? 0x800B8F08 : 0x800B8DF8;
    while (r_u16(row) != 0xFFFF)
    {
        values[2] = r_u16(row + 6);
        values[0] = r_u16(row + 2);
        values[1] = r_u16(row + 4);
        values[3] = r_u16(row + 8);
        values[4] = r_u16(row + 10);
        values[5] = r_u16(row + 12);
        values[6] = r_u16(row + 14);
        values[7] = r_u16(row);
        row += 16;
        packet = t3_8004db9c(ordering, packet, values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7]);
    }
    y = r_u32(state + 296);
    alternate = r_u32(state + 4);
    y = (y + 16) << 16;
    color = alternate != 0 ? 208 : 0;
    packet = t3_8004d8b0(ordering, packet, 0x38000000, 0, color, color, y, y | 368, 0x01E00000, 0x01E00170);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_04_8010f744(uint32 state)
{
    uint32 packet;
    uint32 ordering;
    uint32 y_offset;
    uint32 alternate;
    uint32 index;
    uint32 row;
    uint32 character;
    uint32 y;
    uint32 x;
    uint32 texture;
    uint32 page;
    FUNCTION_MARKER(0x8010F744, "SLUS_004.02-BNS-logical-04");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    y_offset = r_u32(state + 300);
    alternate = r_u32(state + 4);
    for (index = 0; index < 22; ++index)
    {
        row = 0x801296C8 + index * 12;
        if (r_u8(row + 5) == 0)
            continue;
        character = (uint32)r_s16(row + 6);
        if ((sint32)character >= 22)
            continue;
        y = (uint32)r_s16(row + 10);
        x = (uint32)r_s16(row + 8);
        y += y_offset;
        if (alternate != 0)
            packet = t3_8004b928(ordering, packet, x, y, character << 2, 8);
        else
        {
            texture = r_u32(0x800B8DA8 + character * 8);
            page = r_u16(0x800B8DAC + character * 8);
            packet = t3_8004d9bc(ordering, packet, 0x65000000, (x & 0xFFFF) | (y << 16), 0x440020, texture);
            packet = t3_8004daa8(ordering, packet, page);
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_bns_04_8010f5dc(uint32 state)
{
    uint32 packet;
    uint32 ordering;
    uint32 side;
    uint32 player;
    uint32 character;
    uint32 costume;
    uint32 descriptor;
    uint32 x;
    uint32 right;
    uint32 color;
    uint32 y;
    uint32 top;
    uint32 middle;
    uint32 bottom;
    FUNCTION_MARKER(0x8010F5DC, "SLUS_004.02-BNS-logical-04");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    for (side = 0; side < 2; ++side)
    {
        player = state + 120 + side * 124;
        character = r_u32(player - 64);
        costume = r_u32(player - 60);
        descriptor = t3_8004b790((sint32)character, (sint32)costume);
        x = r_u16(player);
        color = r_u32(descriptor) >> 8;
        right = (x + 126) & 0xFFFF;
        y = r_u32(state + 288);
        top = (y - 20) << 16;
        middle = (y + 208) << 16;
        packet = t3_8004d8b0(ordering, packet, 0x3A000000, 0, color, color, x | top, right | top, x | middle, right | middle);
        bottom = (y + 280) << 16;
        packet = t3_8004d8b0(ordering, packet, color | 0x3A000000, color, 0xC0C0C0, 0xC0C0C0, x | middle, right | middle, x | bottom, right | bottom);
        packet = t3_8004daa8(ordering, packet, 32);
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_bns_04_8010eb94(uint32 state)
{
    uint32 packet;
    uint32 frame;
    uint32 ordering;
    uint32 quadrant;
    uint32 player;
    uint32 effect;
    uint32 index;
    uint32 value;
    uint32 x;
    uint32 y;
    uint32 first;
    uint32 second;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x8010EB94, "SLUS_004.02-BNS-logical-04");
    packet = r_u32(0x800ADF64);
    frame = r_u32(state + 8);
    ordering = r_u32(0x800A9218);
    if ((frame & 3) == 0)
    {
        quadrant = (frame >> 3) & 3;
        quadrant = ((quadrant << 1) | (quadrant >> 1)) & 3;
        player = state + 36 + ((frame >> 2) & 1) * 124;
        effect = 0;
        for (index = 0; index < 64; ++index)
        {
            value = state + 332 + index * 20;
            if (r_u16(value) == 0)
            {
                w_u16(value + 2, 0);
                effect = value;
                break;
            }
        }
        if (effect != 0)
        {
            value = t3_8004ce54();
            value = (quadrant << 10) | ((value >> 2) & 1023);
            w_u16(effect, 1);
            x = r_u16(player + 84);
            w_u16(effect + 4, (uint16)(x + ((value * 126) >> 12)));
            y = r_u16(state + 292);
            w_u16(effect + 8, (uint16)y);
            w_u16(effect + 6, (uint16)y);
        }
    }
    first = r_u32(0x800AE230);
    second = r_u32(0x800AE234);
    rectangle.x = (sint16)first;
    rectangle.y = (sint16)(first >> 16);
    rectangle.w = (sint16)second;
    rectangle.h = (sint16)(second >> 16);
    packet = t3_8004db28(ordering, packet, &rectangle);
    for (index = 0; index < 64; ++index)
    {
        effect = state + 332 + index * 20;
        value = r_u16(effect);
        if (value == 1)
            packet = t3_bns_04_8010ea00(ordering, packet, effect);
    }
    y = r_u16(state + 296);
    value = r_u16(0x800AE232);
    rectangle.h = (sint16)(y - 18 - value);
    packet = t3_8004db28(ordering, packet, &rectangle);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_04_8010ea00(uint32 ordering, uint32 packet, uint32 effect)
{
    uint32 phase;
    uint32 frame;
    uint32 level;
    uint32 speed;
    uint32 elapsed;
    uint32 y;
    uint32 x;
    uint32 color;
    uint32 high;
    uint32 points[4];
    FUNCTION_MARKER(0x8010EA00, "SLUS_004.02-BNS-logical-04");
    phase = r_u16(effect + 2);
    if (phase == 0)
    {
        frame = r_u16(0x800AE218);
        phase = r_u16(effect + 2);
        w_u32(effect + 12, 96);
        w_u16(effect + 10, (uint16)frame);
        w_u16(effect + 2, (uint16)(phase + 1));
        w_u32(effect + 16, (t3_8004ce54() & 3) + 4);
    }
    else if (phase != 1)
        return packet;
    level = r_u32(effect + 12);
    speed = r_u32(effect + 16);
    frame = r_u16(0x800AE218);
    elapsed = frame - r_u16(effect + 10);
    w_u32(effect + 12, level - speed);
    y = r_u16(effect + 8) - (elapsed << 4);
    w_u16(effect + 6, (uint16)y);
    if ((sint16)y + 160 < 0)
    {
        w_u16(effect, 0);
        return packet;
    }
    level = r_u32(effect + 12);
    if ((sint32)level <= 0)
    {
        w_u16(effect, 0);
        return packet;
    }
    level &= 255;
    high = level << 16;
    y = (uint32)r_s16(effect + 6);
    x = r_u16(effect + 4);
    points[0] = x | (y << 16);
    x = (uint32)r_s16(effect + 4);
    y = (uint32)r_s16(effect + 6);
    points[1] = ((x + 1) & 0xFFFF) | (y << 16);
    y = (uint32)r_s16(effect + 6);
    x = r_u16(effect + 4);
    points[2] = x | ((y + 160) << 16);
    color = level | (level << 8) | high;
    x = (uint32)r_s16(effect + 4);
    y = (uint32)r_s16(effect + 6);
    points[3] = ((x + 1) & 0xFFFF) | ((y + 160) << 16);
    packet = t3_8004d8b0(ordering, packet, color | 0x3A000000, color, high, high, points[0], points[1], points[2], points[3]);
    return t3_8004daa8(ordering, packet, 32);
}

uint32 t3_bns_04_8010e67c(uint32 state)
{
    uint32 packet;
    uint32 timer;
    uint32 ordering;
    uint32 y;
    uint32 frame;
    uint32 shade;
    uint32 motion;
    uint32 color;
    FUNCTION_MARKER(0x8010E67C, "SLUS_004.02-BNS-logical-04");
    packet = r_u32(0x800ADF64);
    timer = r_u32(state + 16);
    ordering = r_u32(0x800A9218);
    if (timer != 0)
    {
        y = r_u32(state + 296);
        w_u32(state + 16, timer - 1);
        y = (y - 160) << 16;
        if (r_s32(state + 36) < 4 || r_s32(state + 160) < 4)
        {
            frame = r_u32(0x800AE218);
            shade = (frame << 4) & 511;
            motion = frame & 31;
            if (shade >= 256)
                shade = 511 - shade;
            color = shade | (shade << 8) | (shade << 16) | 0x64000000;
            if (motion >= 17)
                motion = 31 - motion;
            motion >>= 1;
            packet = t3_8004d9bc(ordering, packet, color, ((12 - motion) & 0xFFFF) | y, 0x10000A, 0x7E1500E0);
            packet = t3_8004d9bc(ordering, packet, color, ((motion + 346) & 0xFFFF) | y, 0x10000A, 0x7E1500EA);
            packet = t3_8004daa8(ordering, packet, 25);
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_bns_07_800d302c(void)
{
    uint32 total;
    uint32 index;
    uint32 flag;
    uint32 mask;
    uint32 character;
    uint32 first_roster;
    uint32 count;
    uint32 first;
    uint32 second;
    uint32 phase;
    uint32 remainder;
    uint8 candidates[24];

    FUNCTION_MARKER(0x800D302C, "SLUS_004.02-BNS-logical-07");
    total = 0;
    for (index = 0; index < 22; ++index)
        total += t3_80051660(index);
    flag = (total & 0x200) != 0;
    mask = r_u32(0x80097EF0);
    count = 0;
    first_roster = r_u8(0x800D55BC);
    for (index = 0; index < 14; ++index)
    {
        character = index == 0 ? first_roster : r_u8(0x800D55BC + index);
        if ((mask & ((uint32)1 << (character & 31))) != 0)
        {
            candidates[count] = (uint8)character;
            ++count;
        }
    }
    if (count == 0)
    {
        candidates[0] = (uint8)first_roster;
        count = 1;
    }
    first = r_u8(0x80097F13) + 1;
    if ((sint32)first >= (sint32)count)
        first = 0;
    phase = r_u32(0x800AE218) & 0xFFF;
    remainder = count == 1 ? phase : phase % (count - 1);
    second = (remainder + first + 1) % count;
    w_u8(0x80097F13, first);
    character = candidates[first];
    w_u16(0x800ADD98, flag);
    w_u16(0x800A9128, 1);
    w_u16(0x800ADD5C, character);
    t3_8004f0e4(0);
    character = candidates[second];
    w_u16(0x800ADD9A, flag);
    w_u16(0x800A912A, 1);
    w_u16(0x800ADD5E, character);
    return t3_8004f0e4(1);
}

uint32 t3_80062928(void)
{
    FUNCTION_MARKER(0x80062928, "SLUS_004.02");
    return 0xFFFFFFFF;
}

uint32 t3_80062930(void)
{
    FUNCTION_MARKER(0x80062930, "SLUS_004.02");
    return 0xFFFFFFFF;
}

uint32 t3_80062938(void)
{
    FUNCTION_MARKER(0x80062938, "SLUS_004.02");
    return 0xFFFFFFFF;
}

uint32 t3_80062940(uint32 state)
{
    sint32 phase;

    FUNCTION_MARKER(0x80062940, "SLUS_004.02");
    if (r_s16(r_u32(0x800A867C) + 0xA0) != 0x137)
        return 0xFFFFFFFF;
    phase = r_s32(state + 0x18);
    if (phase < 2) {
        w_u16(state + 0x68, 0xFFFF);
        return 0xFFFFFFFE;
    }
    return phase < 4 ? 0xFFFFFFFF : 0x8000;
}

uint32 t3_80062994(void)
{
    FUNCTION_MARKER(0x80062994, "SLUS_004.02");
    return 0xFFFFFFFF;
}
