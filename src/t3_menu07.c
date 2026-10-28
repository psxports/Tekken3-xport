#include "game_functions.h"

uint32 t3_8006994c(uint32 base)
{
    uint32 owner, offset, value;
    FUNCTION_MARKER(0x8006994C, "SLUS_004.02");
    w_u32(0x800ADC20, base);
    w_u32(0x800ADC24, base);
    value = r_u32(base + 4);
    w_u32(base + 4, value + base);
    for (offset = 8; offset <= 52; offset += 4) {
        owner = r_u32(0x800ADC20);
        value = r_u32(owner + offset);
        w_u32(owner + offset, value + base);
    }
    w_u16(0x800A0510, 0xFFFE);
    w_u16(0x800A0518, 0xFFFE);
    return base;
}

uint32 t3_bns_07_800d3748(void)
{
    uint32 phase, value, second_value, selection, index, actor, control, script, command;
    uint32 model, action, saved_flag, paused, first_resource, second_resource, third_resource;
    uint32 other_first, other_second, other_third, result = 0;
    uint32 packet, ordering, level, timer;
    sint32 next_time;
    FUNCTION_MARKER(0x800D3748, "SLUS_004.02-BNS-logical-07");
    t3_8004a678();
    phase = r_u32(0x8010BDFC);
    switch (phase) {
    case 0:
        t3_80029638(1);
        t3_8003c258(0x800A9228);
        t3_8003c258(0x800AAAB4);
        t3_8002bd78(0x800A9228, 0x800AAAB4, 1);
        t3_8002bd78(0x800AAAB4, 0x800A9228, 1);
        selection = (uint32)r_s8(0x800DD810);
        w_u16(0x800ADC84, 4);
        w_u32(0x8010BDF8, 0x800B8E3C + selection * 450);
        for (index = 0; index < 2; ++index) {
            control = 0x8010BE20 + index * 10;
            w_u16(control, 0);
            w_u16(control + 8, 0);
            if (selection == 0)
                w_u16(control + 2, index ? 0 : 12);
            else if (selection == 1)
                w_u16(control + 2, index ? 1 : 13);
            else if (selection == 2)
                w_u16(control + 2, index ? 30 : 23);
            w_u16(control + 4, 0);
        }
        t3_800361ec(0x800A9228, (uint32)r_s16(0x8010BE22));
        t3_800361ec(0x800AAAB4, (uint32)r_s16(0x8010BE2C));
        value = (uint32)(r_s16(0x800A923C) / 4);
        second_value = (uint32)(r_s16(0x800AAAC8) / 4);
        w_u16(0x800A9240, value);
        w_u16(0x800AAACC, second_value);
        t3_80035f2c(r_u16(0x800ADC84));
        t3_8006c588(31);
        t3_8006c618(r_u16(0x800ADC84), 0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE, 0xFFFFFFFE);
        value = t3_8006c800(0);
        t3_8006c95c(r_u16(0x800ADC84), value);
        t3_8004808c((uint32)r_s16(0x800ADC84));
        t3_8006c63c(16, 1);
        value = t3_80077590(0x800CB0F4, 0);
        t3_8007619c(0, 9, value);
        value = t3_80077590(0x800CB0F4, 1);
        t3_8007619c(1, 4, value);
        t3_8007641c(0x800A9228, 0x800AAAB4);
        t3_80035b28(0x800A9228, &first_resource, &second_resource, &third_resource);
        t3_80035bc0(0x800A9228, first_resource, second_resource, third_resource);
        t3_80035ce0();
        t3_80035b28(0x800AAAB4, &other_first, &other_second, &other_third);
        t3_80035bc0(0x800AAAB4, other_first, other_second, other_third);
        t3_80035ce0();
        t3_80035ce8(0x800A9228, second_resource, third_resource);
        t3_80035ce8(0x800AAAB4, other_second, other_third);
        value = t3_8002d178(0x800A9228, (uint32)r_s16(0x8010BE24));
        selection = (uint32)r_s16(0x8010BE2E);
        w_u32(0x800A927C, value);
        w_u32(0x800A9274, value);
        value = t3_8002d178(0x800AAAB4, selection);
        w_u16(0x800A9236, 0x4000);
        w_u16(0x800AAAC2, 0x4000);
        first_resource = r_u16(0x800A9236);
        second_resource = r_u16(0x800AAAC2);
        w_u32(0x800AAB08, value);
        w_u32(0x800AAB00, value);
        w_u16(0x800A9234, 0);
        w_u32(0x800A9228, 0);
        w_u32(0x800A922C, 0);
        w_u16(0x800A9238, 0);
        w_u32(0x800A9230, 0);
        w_u16(0x800AAAC0, 0);
        w_u32(0x800AAAB4, 0);
        w_u32(0x800AAAB8, 0);
        w_u16(0x800AAAC4, 0);
        w_u32(0x800AAABC, 0);
        w_u16(0x800A9254, first_resource);
        w_u16(0x800AAAE0, second_resource);
        t3_8006c858();
        w_u32(0x8009546C, 0);
        t3_bns_07_800d41a4(0x800A9228, 0x800AAAB4, 2);
        t3_80062a10();
        t3_800690a0(0x800A9228, 0x800AAAB4);
        t3_80065ed8(0x800A9228, 0x800AAAB4);
        // Mode 1 replaces both incoming selectors before use
        t3_800670fc(1, 43, 0);
        w_u32(0x8010BE34, 0);
        t3_8007666c();
        t3_8004a83c();
        value = r_u16(0x800ADC84);
        w_u8(0x800AAB77, 1);
        w_u8(0x800A92EB, 1);
        t3_80039960(value);
        t3_80031d0c(0);
        t3_80031d0c(1);
        t3_80031d0c(2);
        t3_8006ae14(0, 0);
        value = r_u32(0x8010BDFC);
        w_u32(0x8010BE00, 4);
        w_u32(0x8010BDFC, value + 1);
        t3_80029638(0);
        return 0;
    case 1:
        if (!t3_8006b9e0()) {
            value = r_u32(0x8010BE00) - 1;
            w_u32(0x8010BE00, value);
            if ((sint32)value < 0) {
                // Mode 0 never consumes the incoming selectors
                value = t3_800670fc(0, 0, 0);
                phase = r_u32(0x8010BDFC);
                w_u32(0x8010BE34, value);
                w_u32(0x8010BE00, 0);
                w_u32(0x8010BDFC, phase + 1);
            }
        }
        return 0;
    case 2:
        break;
    default:
        return 1;
    }
    script = r_u32(0x8010BDF8);
    next_time = r_s16(script);
    timer = r_u32(0x8010BE00);
    if ((sint32)timer >= next_time) {
        for (;;) {
            script = r_u32(0x8010BDF8);
            command = (uint32)r_s16(script + 2);
            if (command == 0xFFFFFFFF) {
                result = 1;
                break;
            }
            switch (command) {
            case 0:
            case 1:
                control = command ? 0x8010BE2A : 0x8010BE20;
                actor = command ? 0x800AAAB4 : 0x800A9228;
                script = r_u32(0x8010BDF8);
                value = (uint32)r_s16(control + 2);
                model = (uint32)r_s16(script + 4);
                if (value != model) {
                    t3_800361ec(actor, model);
                    w_u16(actor + 0x18, r_s16(actor + 0x14) / 4);
                    w_u16(control + 8, 2);
                    w_u16(control, 2);
                    script = r_u32(0x8010BDF8);
                }
                w_u16(control + 2, r_u16(script + 4));
                break;
            case 2:
            case 3:
                control = command == 3 ? 0x8010BE2A : 0x8010BE20;
                actor = command == 3 ? 0x800AAAB4 : 0x800A9228;
                script = r_u32(0x8010BDF8);
                value = (uint32)r_s16(control + 4);
                action = (uint32)r_s16(script + 4);
                if (value != action) {
                    value = t3_8002d178(actor, action);
                    w_u32(actor + 0x54, value);
                    w_u32(actor + 0x4C, value);
                }
                script = r_u32(0x8010BDF8);
                w_u16(control + 4, r_u16(script + 4));
                w_u16(control + 8, r_u16(script + 6));
                value = r_u16(script + 8);
                w_u16(control, 1);
                w_u16(control + 6, value);
                break;
            case 5:
                script = r_u32(0x8010BDF8);
                w_u32(0x8010BE0C, 0);
                w_u32(0x8010BE08, 1);
                w_u32(0x8010BE10, (uint32)r_s16(script + 4));
                break;
            default:
                break;
            }
            script = r_u32(0x8010BDF8);
            timer = r_u32(0x8010BE00);
            next_time = r_s16(script + 10);
            w_u32(0x8010BDF8, script + 10);
            if ((sint32)timer < next_time)
                break;
        }
    }
    t3_80048240(0x800A9228, 0x800AAAB4);
    t3_8006d7cc();
    t3_80039ddc(r_u16(0x800ADC84));
    t3_8003ec64(0x800A9228);
    t3_80039f84();
    t3_80036000();
    t3_800378f8();
    for (index = 0; index < 2; ++index) {
        actor = 0x800A9228 + index * 6284;
        control = 0x8010BE20 + index * 10;
        switch ((uint32)r_s16(control)) {
        case 1:
            value = r_u16(control + 8);
            action = r_u32(actor + 0x54);
            w_u16(actor + 0x58, value + 1);
            w_u16(actor + 0x50, value + 1);
            saved_flag = r_u32(action + 36);
            w_u32(action + 36, saved_flag & 0xBFFFFFFF);
            t3_800458c8(0x800A9228);
            t3_800458c8(0x800AAAB4);
            w_u32(0x80096EA8, 1);
            t3_8003a818(actor);
            action = r_u32(actor + 0x54);
            paused = r_u32(0x8010BE04) & 1;
            value = r_u32(action + 36);
            w_u32(action + 36, (value & 0xBFFFFFFF) | (saved_flag & 0x40000000));
            if (!paused)
                w_u16(control + 8, r_u16(control + 8) + 1);
            value = (uint32)r_s16(control + 8);
            if (r_s16(control + 6) < (sint32)value)
                w_u16(control, 0);
            break;
        case 2:
            value = r_u16(control + 8) - 1;
            w_u16(control + 8, value);
            if (value & 0x8000)
                w_u16(control, 3);
            break;
        case 3:
            w_u16(control, 4);
            break;
        case 4:
            t3_8002960c(1);
            t3_80029638(1);
            t3_80035b28(actor, (uint32 *)psx_addr(0x8010BDE0 + index * 4, 4),
                (uint32 *)psx_addr(0x8010BDE8 + index * 4, 4),
                (uint32 *)psx_addr(0x8010BDF0 + index * 4, 4));
            t3_80029638(0);
            t3_8002960c(0);
            w_u16(control, 5);
            break;
        case 5:
            t3_8002960c(1);
            t3_80029638(1);
            first_resource = r_u32(0x8010BDE0 + index * 4);
            second_resource = r_u32(0x8010BDE8 + index * 4);
            third_resource = r_u32(0x8010BDF0 + index * 4);
            t3_80035bc0(actor, first_resource, second_resource, third_resource);
            t3_80035ce0();
            t3_80029638(0);
            t3_8002960c(0);
            if (!(r_u32(0x8010BE04) & 1))
                w_u16(control, 6);
            break;
        case 6:
            t3_8002960c(1);
            t3_80029638(1);
            second_resource = r_u32(0x8010BDE8 + index * 4);
            third_resource = r_u32(0x8010BDF0 + index * 4);
            t3_80035ce8(actor, second_resource, third_resource);
            t3_80029638(0);
            t3_8002960c(0);
            w_u16(control, 0);
            break;
        default:
            break;
        }
    }
    t3_8007745c(1);
    t3_8004ac24(1);
    if (r_u32(0x8010BE34)) {
        t3_bns_07_800d41a4(0x800A9228, 0x800AAAB4, 2);
    } else if (!(r_u32(0x8010BE04) & 1)) {
        value = t3_800670fc(0, 0, 0);
        w_u32(0x8010BE34, value);
    }
    if (r_u32(0x8010BE08)) {
        if (r_s32(0x8010BE0C) >= 257)
            w_u32(0x8010BE0C, 256);
        packet = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        level = r_u32(0x8010BE0C);
        packet = t3_8004e000(ordering + 4, packet, level + 256);
        paused = r_u32(0x8010BE04) & 1;
        w_u32(0x800ADF64, packet);
        if (!paused) {
            level = r_u32(0x8010BE0C);
            value = r_u32(0x8010BE10);
            w_u32(0x8010BE0C, level + value);
        }
    }
    w_u32(0x8010BE04, 0);
    timer = r_u32(0x8010BE00);
    value = r_u16(0x800A9280);
    second_value = r_u16(0x800AAB0C);
    w_u32(0x8010BE00, timer + 1);
    w_u16(0x800A9282, value);
    w_u16(0x800AAB0E, second_value);
    return result;
}

void t3_bns_07_800d41a4(uint32 actor, uint32 opponent, uint32 incoming_result)
{
    FUNCTION_MARKER(0x800D41A4, "SLUS_004.02-BNS-logical-07");
    t3_800630d8_host(actor, opponent, 0x8009546C, incoming_result);
}

uint32 t3_bns_07_800d4adc(uint32 x, uint32 z)
{
    uint32 shifted_x, shifted_z, quotient_x, quotient_z, high;
    uint32 tile_x, tile_z, source, destination, first, second, index;
    uint32 scene, vertices, count, ordering, grid, buffer, packet, tail;
    uint32 pitch, height, threshold, row, column, map_x, split_count, split_packets;
    uint32 corners, p0, p1, p2, p3, tiles, tile, texture, palette, rotation, key, link;
    uint32 small, small_tail, quarter, u, v, page, temporary;
    uint32 top_x, top_y, left_x, left_y, bottom_x, bottom_y, right_x, right_y;
    uint32 center_x, center_y;
    uint32 uv0, uv1, uv2, uv3;
    sint32 sine, screen_y, x0, x1, x2, x3, y0, y1, y2, y3;
    FUNCTION_MARKER(0x800D4ADC, "SLUS_004.02-BNS-logical-07");
    shifted_x = x + 900;
    shifted_z = z + 900;
    high = (uint32)(((sint64)(sint32)shifted_x * (sint32)0x91A2B3C5) >> 32);
    quotient_x = (uint32)((sint32)(high + shifted_x) >> 10) - (uint32)((sint32)shifted_x >> 31);
    if ((sint32)shifted_x < 0)
        --quotient_x;
    high = (uint32)(((sint64)(sint32)shifted_z * (sint32)0x91A2B3C5) >> 32);
    quotient_z = (uint32)((sint32)(high + shifted_z) >> 10) - (uint32)((sint32)shifted_z >> 31);
    if ((sint32)shifted_z < 0)
        --quotient_z;
    tile_x = (uint32)((sint32)quotient_x % 10);
    if ((sint32)tile_x < 0)
        tile_x += 10;
    tile_z = (uint32)((sint32)quotient_z % 10);
    if ((sint32)tile_z < 0)
        tile_z += 10;
    tile_z = 9 - tile_z;
    source = 0x801BFEB4;
    destination = 0x1F8001EC;
    for (index = 0; index < 4; ++index) {
        first = r_u32(source);
        second = r_u32(source + 4);
        w_u32(destination, first);
        w_u32(destination + 4, second);
        source += 8;
        destination += 8;
    }
    t3_8004adc4(quotient_x * 1800, 0, quotient_z * 1800);
    scene = r_u32(0x800ADD14);
    vertices = r_u32(scene + 12);
    count = r_u32(scene + 16);
    t3_8004a364(vertices, 0x1F800000, 0x1F800288, count);
    ordering = r_u32(0x800A8C54);
    scene = r_u32(0x800ADD14);
    ordering = r_u32(ordering + 4) + 4084;
    split_packets = 0;
    split_count = 0;
    grid = r_u32(scene + 28);
    buffer = r_u32(0x800ADEFC);
    packet = scene ? scene + buffer * 4000 + 1508 : 0;
    pitch = r_u32(0x800A85C0);
    sine = r_s16(0x8001E8C4 + ((pitch + 239) & 0xFFF) * 2);
    if (sine) {
        height = r_u32(0x800A85D8);
        if ((sint32)height < 0)
            height = 0 - height;
        threshold = t3_camera_divide(height << 12, (uint32)sine);
    } else {
        threshold = 0;
    }
    threshold += r_u32(0x80104090);
    for (row = 0; row < 10; ++row) {
        map_x = tile_x;
        tail = packet + 36;
        for (column = 0; column < 10; ++column) {
            corners = r_u32(grid + (uint32)(sint32)(sint16)(column * 4 + row * 40));
            p0 = corners & 255;
            p1 = (corners >> 8) & 255;
            p2 = (corners >> 16) & 255;
            p3 = corners >> 24;
            if (!r_u16(0x1F800288 + p0 * 2) || !r_u16(0x1F800288 + p1 * 2) ||
                !r_u16(0x1F800288 + p2 * 2) || !r_u16(0x1F800288 + p3 * 2))
                goto next_tile;
            screen_y = r_s16(0x1F800002 + p0 * 4);
            if (screen_y < 0 || screen_y >= 480) {
                screen_y = r_s16(0x1F800002 + p1 * 4);
                if (screen_y < 0 || screen_y >= 480) {
                    screen_y = r_s16(0x1F800002 + p2 * 4);
                    if (screen_y < 0 || screen_y >= 480) {
                        screen_y = r_s16(0x1F800002 + p3 * 4);
                        if (screen_y < 0 || screen_y >= 480)
                            goto next_tile;
                    }
                }
            }
            if (r_u16(0x1F800000 + p0 * 4) >= 368 && r_u16(0x1F800000 + p1 * 4) >= 368 &&
                r_u16(0x1F800000 + p2 * 4) >= 368 && r_u16(0x1F800000 + p3 * 4) >= 368)
                goto next_tile;
            first = r_u32(0x1F800000 + p0 * 4);
            tiles = r_u32(0x801BFEAC);
            w_u32(tail - 28, first);
            w_u32(tail - 20, r_u32(0x1F800000 + p1 * 4));
            w_u32(tail - 12, r_u32(0x1F800000 + p2 * 4));
            w_u32(tail - 4, r_u32(0x1F800000 + p3 * 4));
            tile = tiles + tile_z * 80 + 4 + map_x * 8;
            texture = r_u32(tile);
            palette = r_u32(tile + 4);
            rotation = 0x1F8001EC + (texture >> 30) * 8;
            texture &= 0x3FFFFFFF;
            key = r_u8(0x801BFE44 + tile_z * 10 + map_x);
            if ((sint32)split_count >= 11 ||
                ((sint32)r_u16(0x1F800288 + p0 * 2) >= (sint32)threshold &&
                 (sint32)r_u16(0x1F800288 + p1 * 2) >= (sint32)threshold &&
                 (sint32)r_u16(0x1F800288 + p2 * 2) >= (sint32)threshold &&
                 (sint32)r_u16(0x1F800288 + p3 * 2) >= (sint32)threshold &&
                 r_u32(0x800A85C0) - 1024 >= 513)) {
                uv0 = r_u16(rotation);
                uv1 = r_u16(rotation + 2);
                uv2 = r_u16(rotation + 4);
                uv3 = r_u16(rotation + 6);
                u = texture & 0xFFFF;
                w_u32(tail - 24, palette | (u + uv0));
                w_u32(tail - 16, (texture & 0x39FF0000) | (u + uv1));
                w_u32(tail - 8, u + uv2);
                w_u32(tail, u + uv3);
                link = ordering - key * 4;
                first = r_u32(link);
                w_u32(packet, (first & 0xFFFFFF) | 0x09000000);
                first = r_u32(link);
                w_u32(link, (first & 0xFF000000) | (packet & 0xFFFFFF));
            } else {
                scene = r_u32(0x800ADD14);
                buffer = r_u32(0x800ADEFC);
                small = scene ? scene + buffer * 1760 + 9508 + split_packets * 40 : 0;
                u = texture;
                v = (texture >> 8) & 255;
                page = (texture >> 16) & 0x39FF;
                small_tail = small + 14;
                ++split_count;
                link = ordering - key * 4;
                x0 = r_s16(tail - 28);
                x1 = r_s16(tail - 20);
                y0 = r_s16(tail - 26);
                y1 = r_s16(tail - 18);
                top_x = (uint32)((x0 + x1) / 2);
                top_y = (uint32)((y0 + y1) / 2);
                x2 = r_s16(tail - 12);
                y2 = r_s16(tail - 10);
                left_x = (uint32)((x0 + x2) / 2);
                left_y = (uint32)((y0 + y2) / 2);
                x3 = r_s16(tail - 4);
                y3 = r_s16(tail - 2);
                bottom_x = (uint32)((x2 + x3) / 2);
                bottom_y = (uint32)((y2 + y3) / 2);
                right_x = (uint32)((x1 + x3) / 2);
                right_y = (uint32)((y1 + y3) / 2);
                center_x = (uint32)(((sint16)left_x + (sint16)right_x) / 2);
                center_y = (uint32)(((sint16)top_y + (sint16)bottom_y) / 2);
                for (quarter = 0; quarter < 4; ++quarter) {
                    switch (quarter) {
                    case 0:
                        temporary = r_u16(tail - 28);
                        w_u16(small_tail - 6, temporary);
                        temporary = r_u16(tail - 26);
                        w_u16(small_tail + 2, top_x);
                        w_u16(small_tail + 4, top_y);
                        w_u16(small_tail + 12, left_y);
                        w_u16(small_tail + 18, center_x);
                        w_u16(small_tail + 20, center_y);
                        w_u8(small_tail - 2, u);
                        w_u8(small_tail - 1, v);
                        w_u8(small_tail + 6, u + 31);
                        w_u8(small_tail + 7, v);
                        w_u8(small_tail + 14, u);
                        w_u8(small_tail + 15, v + 31);
                        w_u8(small_tail + 22, u + 31);
                        w_u8(small_tail + 23, v + 31);
                        w_u16(small_tail - 4, temporary);
                        w_u16(small_tail + 10, left_x);
                        break;
                    case 1:
                        w_u16(small_tail - 6, top_x);
                        w_u16(small_tail - 4, top_y);
                        temporary = r_u16(tail - 20);
                        w_u16(small_tail + 2, temporary);
                        temporary = r_u16(tail - 18);
                        w_u16(small_tail + 10, center_x);
                        w_u16(small_tail + 12, center_y);
                        w_u16(small_tail + 18, right_x);
                        w_u16(small_tail + 20, right_y);
                        w_u8(small_tail - 2, u + 32);
                        w_u8(small_tail - 1, v);
                        w_u8(small_tail + 6, u + 63);
                        w_u8(small_tail + 7, v);
                        w_u8(small_tail + 14, u + 32);
                        w_u8(small_tail + 15, v + 31);
                        w_u8(small_tail + 22, u + 63);
                        w_u8(small_tail + 23, v + 31);
                        w_u16(small_tail + 4, temporary);
                        break;
                    case 2:
                        w_u16(small_tail - 4, left_y);
                        w_u16(small_tail + 2, center_x);
                        w_u16(small_tail + 4, center_y);
                        w_u16(small_tail - 6, left_x);
                        temporary = r_u16(tail - 12);
                        w_u16(small_tail + 10, temporary);
                        temporary = r_u16(tail - 10);
                        w_u16(small_tail + 20, bottom_y);
                        w_u8(small_tail - 2, u);
                        w_u8(small_tail - 1, v + 32);
                        w_u8(small_tail + 6, u + 31);
                        w_u8(small_tail + 7, v + 32);
                        w_u8(small_tail + 14, u);
                        w_u8(small_tail + 15, v + 63);
                        w_u8(small_tail + 22, u + 31);
                        w_u8(small_tail + 23, v + 63);
                        w_u16(small_tail + 18, bottom_x);
                        w_u16(small_tail + 12, temporary);
                        break;
                    case 3:
                        w_u16(small_tail - 6, center_x);
                        w_u16(small_tail - 4, center_y);
                        w_u16(small_tail + 2, right_x);
                        w_u16(small_tail + 4, right_y);
                        w_u16(small_tail + 12, bottom_y);
                        w_u16(small_tail + 10, bottom_x);
                        temporary = r_u16(tail - 4);
                        w_u16(small_tail + 18, temporary);
                        temporary = r_u16(tail - 2);
                        w_u8(small_tail - 2, u + 32);
                        w_u8(small_tail - 1, v + 32);
                        w_u8(small_tail + 6, u + 63);
                        w_u8(small_tail + 7, v + 32);
                        w_u8(small_tail + 14, u + 32);
                        w_u8(small_tail + 15, v + 63);
                        w_u8(small_tail + 22, u + 63);
                        w_u8(small_tail + 23, v + 63);
                        w_u16(small_tail + 20, temporary);
                        break;
                    }
                    w_u16(small_tail + 8, page);
                    w_u16(small_tail, palette >> 16);
                    small_tail += 40;
                    first = r_u32(link);
                    w_u32(small, (first & 0xFFFFFF) | 0x09000000);
                    first = r_u32(link);
                    w_u32(link, (first & 0xFF000000) | (small & 0xFFFFFF));
                    small += 40;
                }
                split_packets += 4;
            }
next_tile:
            tail += 40;
            packet += 40;
            if (++map_x >= 10)
                map_x = 0;
        }
        if ((sint32)--tile_z < 0)
            tile_z = 9;
    }
    return 0;
}

uint32 t3_bns_07_800d494c(uint32 actor, uint32 opponent)
{
    uint32 point[3];
    uint32 pitch, index, first, second, angle, cosine, sine;
    sint32 facing;
    FUNCTION_MARKER(0x800D494C, "SLUS_004.02-BNS-logical-07");
    pitch = r_u32(0x800A85C0);
    index = ((pitch + 239) & 0xFFF) * 2;
    facing = r_s16(0x8001E8C4 + index);
    w_u32(0x800ADFD0, facing > 0);
    if (facing <= 0)
        return 0;
    if ((pitch & 0xFFF) && !(pitch & 0x800) && r_s32(0x800A85D8) < -1720) {
        t3_bns_07_800d45c0(point);
    } else {
        first = r_u32(actor + 0xF68);
        second = r_u32(opponent + 0xF68);
        point[0] = t3_camera_divide(first + second, 2);
        first = r_u32(actor + 0xF70);
        second = r_u32(opponent + 0xF70);
        pitch = r_u32(0x800A85C0);
        point[2] = t3_camera_divide(first + second, 2);
        angle = pitch & 0xFFF;
        if (pitch & 0x800)
            angle -= 4096;
        if ((sint32)angle < -37) {
            index = ((r_u32(0x800A85C4) + 1024) & 0xFFF) * 2;
            cosine = (uint32)r_s16(0x8001F0C4 + index);
            sine = (uint32)r_s16(0x8001E8C4 + index);
            point[0] -= t3_camera_divide(cosine * 1800, 4096);
            point[2] -= t3_camera_divide(sine * 1800, 4096);
        }
    }
    return t3_bns_07_800d4adc(point[0], point[2]);
}

uint32 t3_bns_07_800d45c0(uint32 *point)
{
    uint32 pitch, height, magnitude, index, cosine, sine, product, radial, length;
    uint32 angle, scaled, x, z, sector, quadrant, flip, offset, step, origin, result;
    FUNCTION_MARKER(0x800D45C0, "SLUS_004.02-BNS-logical-07");
    pitch = r_u32(0x800A85C0);
    height = r_u32(0x800A85D8);
    index = ((pitch + 239) & 0xFFF) * 2;
    cosine = (uint32)r_s16(0x8001F0C4 + index);
    magnitude = (sint32)height < 0 ? 0 - height : height;
    product = magnitude * cosine;
    sine = (uint32)r_s16(0x8001E8C4 + index);
    radial = t3_camera_divide(product, sine);
    length = t3_8004aedc(radial * radial + height * height);
    product = length * 184;
    angle = r_u32(0x800A85C4);
    index = ((angle + 1024) & 0xFFF) * 2;
    scaled = (uint32)(((sint64)(sint32)product * 0x07A44C6B) >> 36);
    cosine = (uint32)r_s16(0x8001F0C4 + index);
    sine = (uint32)r_s16(0x8001E8C4 + index);
    scaled -= (uint32)((sint32)product >> 31);
    origin = r_u32(0x800A85D4);
    x = origin + t3_camera_divide(radial * cosine, 4096);
    sector = (angle + 1536) & 0xFFF;
    origin = r_u32(0x800A85DC);
    z = origin + t3_camera_divide(radial * sine, 4096);
    quadrant = sector >> 10;
    flip = (sector >> 9) & 1;
    offset = ((uint32)-1230 - height) * 6;
    if ((sint32)offset >= 9001)
        offset = 9000;
    if (quadrant == 0 || quadrant == 2) {
        step = t3_camera_divide(scaled * sine, 4096);
        if (flip)
            x -= step;
        else
            x += step;
        origin = r_u32(0x800A85D4);
        if (quadrant == 0)
            x = x - 900 + offset;
        else
            x = x + 900 - offset;
        step = t3_camera_divide(sine * (x - origin), cosine);
        result = r_u32(0x800A85DC);
        z = step + result;
    } else {
        step = t3_camera_divide(scaled * cosine, 4096);
        if (flip)
            z += step;
        else
            z -= step;
        origin = r_u32(0x800A85DC);
        if (quadrant == 1)
            z = z - 900 + offset;
        else
            z = z + 900 - offset;
        step = t3_camera_divide(cosine * (z - origin), sine);
        result = r_u32(0x800A85D4);
        x = step + result;
    }
    point[0] = x;
    point[2] = z;
    return result;
}

uint32 t3_bns_07_800d4440(void)
{
    uint32 keys[40];
    uint32 count = 0;
    uint32 row, column, scene, key, index;
    FUNCTION_MARKER(0x800D4440, "SLUS_004.02-BNS-logical-07");
    t3_bns_07_800d4220();
    scene = t3_80048230();
    w_u32(0x801BFEAC, scene);
    for (row = 0; row < 10; ++row) {
        for (column = 0; column < 10; ++column) {
            scene = r_u32(0x801BFEAC);
            key = r_u32(scene + row * 80 + column * 8 + 4) & 0x39FFFFFF;
            for (index = 0; index < count; ++index) {
                if (keys[index] == key)
                    break;
            }
            if (index < count) {
                w_u8(0x801BFE44 + row * 10 + column, index);
            } else {
                w_u8(0x801BFE44 + row * 10 + column, count);
                keys[count] = key;
                ++count;
            }
        }
    }
    w_u16(0x801BFEA8, count);
    return (sint16)count < 41 ? 0 : 0xFFFFFFFF;
}

uint32 t3_bns_07_800d4220(void)
{
    static const uint16 coordinates[16] = {
        0, 63, 0x3F00, 0x3F3F, 0x3F00, 0, 0x3F3F, 63,
        0x3F3F, 0x3F00, 63, 0, 63, 0x3F3F, 0, 0x3F00
    };
    uint32 base, group, buffer, index, packet, count;
    POLY_FT4 *polygon;
    FUNCTION_MARKER(0x800D4220, "SLUS_004.02-BNS-logical-07");
    DrawSync(0);
    base = r_u32(0x800ADD14);
    t3_80031bfc(0x8001E36C, base);
    GsMapModelingData(base + 4);
    for (group = 0; group < 2; ++group) {
        for (buffer = 0; buffer < 2; ++buffer) {
            base = r_u32(0x800ADD14);
            packet = base ? base + (group ? 9508 + buffer * 1760 : 1508 + buffer * 4000) : 0;
            count = group ? 44 : 100;
            for (index = 0; index < count; ++index) {
                polygon = (POLY_FT4 *)psx_addr(packet, 40);
                SetPolyFT4(polygon);
                w_u8(packet + 4, 103);
                w_u8(packet + 5, 103);
                w_u8(packet + 6, 103);
                SetSemiTrans(polygon, 0);
                SetShadeTex(polygon, 0);
                packet += 40;
            }
        }
    }
    for (index = 0; index < 16; ++index)
        w_u16(0x801BFEB4 + index * 2, coordinates[index]);
    return 0x801BFEB4;
}

uint32 t3_bns_07_800d2a0c(uint32 mode, uint32 option)
{
    uint32 state = 0x800AFA88;
    uint32 index, first, second, third, selected, value;
    FUNCTION_MARKER(0x800D2A0C, "SLUS_004.02-BNS-logical-07");
    t3_8006c588(31);
    t3_800697d4(-1);
    w_u16(0x800AAAC6, 1);
    w_u16(0x800AC352, 1);
    w_u8(0x800AAAD2, 1);
    w_u16(0x800A923A, 0);
    w_u8(0x800A9246, 0);
    w_u8(0x800AC35E, 2);
    w_u32(state, mode);
    w_u8(state + 0x19, 0);
    w_u8(state + 0x0F, 0);
    w_u8(state + 0x11, 0);
    w_u8(state + 0x12, 0);
    w_u8(state + 0x13, 0);
    w_u32(0x800954B0, 0);
    w_u32(0x800954B4, 0);
    for (index = 0x24; index <= 0x34; index += 4)
        w_u32(state + index, 0);
    w_u16(0x800AE212, 0);
    first = r_u8(0x80097F0A);
    second = r_u8(0x80097F10);
    third = r_u8(0x80097F07);
    w_u16(0x800ADED4, 0);
    w_u8(state + 0x0D, first);
    w_u8(state + 0x0E, second);
    w_u8(state + 0x20, third);
    w_u16(0x800AE208, r_u8(0x80097F06));
    w_u16(0x800ADEF8, r_u8(0x80097F08));
    w_u16(0x800AFA58, r_u8(0x80097F09));
    w_u8(0x800AE211, 0);
    for (index = 0; index < 2; ++index) {
        t3_8002a16c(index, r_u8(0x80097F0C + index));
        t3_80050ee4(index);
        t3_800517b0(index);
        if (!r_u8(0x80097F19))
            w_u8(0x80097F0E + index, 88);
        w_u16(0x800ADD5C + index * 2, 22);
        w_u16(0x800ADD98 + index * 2, 0);
        w_u16(0x800ADFBC + index * 2, 0);
        w_u16(0x800AE1F8 + index * 2, 0);
    }
    selected = (option ^ 1) & 1;
    t3_8005118c(selected);
    w_u16(0x800AE1F8 + selected * 2, 1);
    t3_80031d0c(0);
    switch (mode) {
    case 0:
        w_u8(state + 4, 1);
        w_u8(state + 5, 1);
        w_u8(state + 6, 0);
        w_u8(state + 7, 1);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
        w_u8(state + 10, 0);
        w_u8(state + 11, 1);
        w_u8(state + 12, 0);
        t3_8005135c();
        break;
    case 1:
        w_u8(state + 4, 1);
        w_u8(state + 7, 1);
        w_u8(state + 8, 1);
        w_u8(state + 10, 1);
        w_u8(state + 11, 1);
        w_u8(state + 12, 1);
        w_u8(state + 5, 0);
        w_u8(state + 6, 0);
        w_u8(state + 9, 0);
        w_u16(0x800AE1FA, 1);
        w_u16(0x800AE1F8, 1);
        goto team_state;
    case 2:
        w_u8(state + 4, 1);
        w_u8(state + 5, 1);
        w_u8(state + 6, 1);
        w_u8(state + 8, 1);
        w_u8(state + 9, 1);
        w_u8(state + 10, 1);
        w_u8(state + 11, 1);
        w_u8(state + 12, 1);
        w_u8(state + 7, 0);
        w_u8(state + 0x20, 0);
        w_u8(state + 0x65, 4);
        w_u8(state + 0x72, 4);
        for (index = 0; index < 22; ++index)
            w_u16(state + 0x9E - index * 2, 0);
        break;
    case 3:
        w_u8(state + 11, 1);
        for (index = 4; index <= 10; ++index)
            w_u8(state + index, 0);
        w_u8(state + 12, 0);
        w_u8(state + 13, 0);
        w_u16(0x800AE208, 1);
        w_u8(state + 0x20, 1);
        w_u16(0x800ADEF8, 2);
        w_u16(0x800AFA58, 0);
        t3_8005135c();
        break;
    case 4:
        w_u8(state + 7, 1);
        w_u8(state + 9, 1);
        w_u8(state + 10, 1);
        w_u8(state + 11, 1);
        w_u8(state + 4, 0);
        w_u8(state + 5, 0);
        w_u8(state + 6, 0);
        w_u8(state + 8, 0);
        w_u8(state + 12, 0);
        w_u16(0x800AE208, 1);
        w_u8(state + 0x20, 0);
        w_u16(0x800ADEF8, 2);
        w_u16(0x800AFA58, 0);
        w_u32(state + 0x40, 0);
        w_u32(state + 0x44, 0);
        w_u32(state + 0x48, 0);
        for (index = 0; index < 22; ++index)
            w_u16(state + 0x8A - index * 2, 0);
        w_u32(state + 0x4C, 0);
        for (index = 0; index < 4; ++index)
            w_u32(state + 0x5C - index * 4, 22);
        break;
    case 5:
        w_u8(state + 4, 1);
        w_u8(state + 11, 1);
        for (index = 5; index <= 10; ++index)
            w_u8(state + index, 0);
        w_u8(state + 12, 0);
        w_u8(state + 0x20, 0);
        w_u16(0x800ADEF8, 5);
        w_u32(0x800954B4, 1);
        w_u16(0x800AFA58, 0);
        goto finish;
    case 6:
        for (index = 4; index <= 9; ++index)
            w_u8(state + index, 0);
        w_u8(state + 10, 1);
        w_u8(state + 11, 1);
        w_u8(state + 12, 0);
        t3_8002a16c(0, 0);
        t3_8002a16c(1, 0);
        w_u16(0x800ADEF8, 2);
        w_u16(0x800ADED4, 1);
        t3_bns_07_800d302c();
        goto finish;
    case 7:
        w_u8(state + 4, 1);
        w_u8(state + 5, 1);
        w_u8(state + 8, 1);
        w_u8(state + 12, 1);
        w_u8(state + 6, 0);
        w_u8(state + 7, 0);
        w_u8(state + 9, 0);
        w_u8(state + 10, 0);
        w_u8(state + 11, 0);
        w_u16(0x800ADEF8, 5);
        w_u16(0x800AFA58, 0);
team_state:
        w_u16(state + 0x3A, 3);
        w_u16(state + 0x3E, 3);
        w_u16(state + 0x40, 0);
        w_u16(state + 0x38, 0);
        w_u16(state + 0x3C, 0);
        w_u32(state + 0x48, 0xFFFFFFFF);
        w_u32(state + 0x4C, 0xFFFFFFFF);
        w_u32(state + 0x44, 0xFFFFFFFF);
        break;
    case 8:
        w_u8(state + 4, 1);
        for (index = 5; index <= 12; ++index)
            w_u8(state + index, 0);
        w_u16(0x800ADEF8, 4);
        w_u8(state + 0x20, 0);
        w_u16(0x800AFA58, 0);
        break;
    default:
        goto finish;
    }
    t3_800516bc();
finish:
    if (r_u16(0x800ADEF8) >= 5)
        w_u32(0x800954B0, 1);
    value = t3_bns_07_800d2fc4(mode);
    w_u8(state + 0x16, value);
    w_u16(0x800AE204, 7);
    w_u16(0x800AE224, 0);
    return 0x800B0000;
}

uint32 t3_bns_07_800d2614(uint32 flags)
{
    uint32 ordering, packet, clut, index, column, row, width, height;
    FUNCTION_MARKER(0x800D2614, "SLUS_004.02-BNS-logical-07");
    switch (flags) {
    case 0:
        ordering = r_u32(0x800A9218);
        packet = r_u32(0x800ADF64);
        packet = t3_8004da10(ordering, packet, 0x60000000, 0, 0x01E00170);
        w_u32(0x800ADF64, packet);
        return packet;
    case 1:
    case 7:
        // The jump-table target remains in V0 through the epilogue
        return 0x800D29D8;
    case 2:
        t3_8004ee14();
        goto logo;
    case 3:
logo:
        packet = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        packet = t3_8004db9c(ordering + 28, packet, 151, 388, 72, 16, 800, 0, 0x7820, 0);
        w_u32(0x800ADF64, packet);
        goto captions;
    case 4:
captions:
        t3_8004ce74(0x800B8D58, 6, 36, 412);
        t3_8004ce74(0x800B8D84, 6, 99, 432);
        goto first_palette;
    case 5:
first_palette:
        clut = 0x7EA0;
        break;
    case 6:
        clut = 0x7D20;
        break;
    default:
        ordering = r_u32(0x800A9218);
        packet = r_u32(0x800ADF64);
        packet = t3_8004e000(ordering + 28, packet, 0);
        w_u32(0x800ADF64, packet);
        return packet;
    }
    for (index = 0; index < 6; ++index) {
        column = index % 3;
        row = index / 3;
        width = column == 2 ? 112 : 128;
        height = row ? 224 : 256;
        ordering = r_u32(0x800A9218);
        if (!index)
            packet = r_u32(0x800ADF64);
        else
            w_u32(0x800ADF64, packet);
        packet = t3_8004db9c(ordering, packet, column * 128, row * 256,
            width, height, 512 + column * 64, row * 256, clut + index * 64, 128);
    }
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_07_800d2fc4(uint32 mode)
{
    FUNCTION_MARKER(0x800D2FC4, "SLUS_004.02-BNS-logical-07");
    switch (mode) {
    case 0: case 1: case 2: case 3: case 4: case 6:
        return 0;
    case 5:
        return 1;
    case 7:
        return 3;
    case 8:
        return 2;
    default:
        return t3_8007ae58(1);
    }
}

uint32 t3_bns_07_800d3554(uint32 index)
{
    uint32 table;
    FUNCTION_MARKER(0x800D3554, "SLUS_004.02-BNS-logical-07");
    table = r_u32(0x801BFE40);
    return (uint32)r_s8(table + index);
}

uint32 t3_bns_07_800d3570(uint32 index)
{
    FUNCTION_MARKER(0x800D3570, "SLUS_004.02-BNS-logical-07");
    return t3_80077590(0x8010BE40, (sint32)(index << 1));
}

uint32 t3_bns_07_800d3598(uint32 index)
{
    FUNCTION_MARKER(0x800D3598, "SLUS_004.02-BNS-logical-07");
    return t3_80077590(0x8010BE40, (sint32)((index << 1) | 1));
}

uint32 t3_bns_07_800d4218(void)
{
    FUNCTION_MARKER(0x800D4218, "SLUS_004.02-BNS-logical-07");
    return 13028;
}

uint32 t3_bns_07_800d31a0(void)
{
    sint16 rectangle[4];
    uint32 top, width, bottom, remaining;
    FUNCTION_MARKER(0x800D31A0, "SLUS_004.02-BNS-logical-07");
    top = (uint32)r_s16(0x800AE232);
    width = (uint32)r_s16(0x800AE234);
    if (top) {
        rectangle[0] = 0;
        rectangle[1] = 0;
        rectangle[2] = (sint16)width;
        rectangle[3] = (sint16)top;
        t3_8002943c(rectangle);
        top = (uint32)r_s16(0x800AE232);
    }
    bottom = top + (uint32)r_s16(0x800AE236);
    remaining = 480 - bottom;
    if (!remaining)
        return remaining;
    rectangle[0] = 0;
    rectangle[1] = (sint16)bottom;
    rectangle[2] = (sint16)width;
    rectangle[3] = (sint16)remaining;
    return t3_8002943c(rectangle);
}

uint32 t3_bns_07_800d3228(void)
{
    uint32 phase, first, second, timer, packet, ordering, level, flags, index, saved, result;
    FUNCTION_MARKER(0x800D3228, "SLUS_004.02-BNS-logical-07");
    if (r_s16(0x800AE224) > 0 && t3_80050f38()) {
        t3_8004f870();
        t3_8004b61c(1);
        t3_8004b5f4();
        t3_80028f5c();
        t3_8004f8f8(4);
        return 1;
    }
    phase = (uint32)r_s16(0x800AE224);
    switch (phase) {
    case 0:
        t3_80055884(4);
        w_u16(0x800ADE14, 258);
        first = r_u8(0x80097F24);
        w_u16(0x800ADE16, 240);
        second = r_u8(0x80097F25);
        w_u16(0x800ADE10, (uint32)(sint32)(sint8)first);
        w_u16(0x800ADE12, (uint32)(sint32)(sint8)second);
        t3_800295b8();
        t3_bns_07_800d31a0();
        w_u32(0x800AFA88, 6);
        w_u8(0x800AFAA0, 0);
        w_u16(0x800ADED4, 1);
        t3_8002a16c(0, 0);
        t3_8002a16c(1, 0);
        t3_8006c588(31);
        w_u8(0x800B040C, 0);
        w_u16(0x800AE224, 1);
        w_u32(0x800DD80C, 8);
        w_u32(0x800AE214, 0);
        break;
    case 1:
        w_u32(0x800DD80C, 0);
        t3_bns_07_800d36b0();
        w_u16(0x800AE224, 2);
        break;
    case 2:
        w_u32(0x800DD80C, 7);
        if (t3_bns_07_800d3748())
            w_u16(0x800AE224, 3);
        break;
    case 3:
        w_u32(0x800DD80C, 7);
        t3_8004ca40(0x800D55CC, 0x800B9840);
        w_u16(0x800AE224, 4);
        break;
    case 4:
        w_u32(0x800DD80C, 5);
        w_u32(0x800AE214, 256);
        w_u16(0x800AE224, 5);
        goto fade_in;
    case 5:
fade_in:
        timer = r_u32(0x800AE214);
        if ((sint32)timer > 0) {
            level = timer + 253;
            packet = r_u32(0x800ADF64);
            ordering = r_u32(0x800A9218);
            w_u32(0x800AE214, timer - 3);
            w_u32(0x800ADF64, t3_8004e000(ordering + 12, packet, level));
        } else {
            w_u32(0x800AE214, 300);
            w_u16(0x800AE224, 6);
        }
        break;
    case 6:
        timer = r_u32(0x800AE214);
        w_u32(0x800DD80C, 2);
        --timer;
        w_u32(0x800AE214, timer);
        if (!timer) {
            w_u32(0x800AE214, 256);
            w_u16(0x800AE224, 7);
        }
        break;
    case 7:
        timer = r_u32(0x800AE214);
        if ((sint32)timer > 0) {
            packet = r_u32(0x800ADF64);
            ordering = r_u32(0x800A9218);
            level = timer - 8;
            w_u32(0x800AE214, level);
            w_u32(0x800ADF64, t3_8004e000(ordering + 28, packet, level));
        } else {
            w_u32(0x800DD80C, 8);
            w_u16(0x800AE224, 8);
        }
        break;
    default:
        w_u32(0x800AE214, 256);
        t3_8004f870();
        w_u32(0x800DD80C, 8);
        t3_bns_07_800d2a0c(6, 0);
        break;
    }
    flags = r_u32(0x800DD80C);
    index = r_u32(0x800ADEFC);
    saved = r_u32(0x800ADF64);
    w_u32(0x800ADF64, 0x80109FE0 + index * 3840);
    result = t3_bns_07_800d2614(flags);
    w_u32(0x800ADF64, saved);
    return result;
}

uint32 t3_bns_07_800d41c8(void)
{
    FUNCTION_MARKER(0x800D41C8, "SLUS_004.02-BNS-logical-07");
    t3_800697d4(-1);
    t3_8006994c(0x800DD818);
    t3_8002ce14(0);
    return t3_8002ce14(1);
}

uint32 t3_bns_07_800d36b0(void)
{
    FUNCTION_MARKER(0x800D36B0, "SLUS_004.02-BNS-logical-07");
    w_u32(0x8010BDFC, 0);
    w_u16(0x800ADED4, 1);
    w_u32(0x80095468, 0);
    w_u32(0x8009548C, 0);
    w_u32(0x80095494, 0);
    w_u32(0x80095490, 0);
    w_u32(0x8010BE04, 0);
    w_u32(0x8010BE08, 0);
    w_u16(0x800A923E, 0xFFFE);
    w_u16(0x800AAACA, 0xFFFE);
    t3_bns_07_800d41c8();
    w_u8(0x800AAAB0, 0);
    w_u8(0x800AC33C, 0);
    w_u8(0x800ADBC8, 0);
    w_u8(0x800AFAA1, 1);
    return 0x800B0000;
}

uint32 t3_bns_07_800d35c4(uint32 index)
{
    uint32 offset;
    FUNCTION_MARKER(0x800D35C4, "SLUS_004.02-BNS-logical-07");
    offset = r_u8(0x800DD814 + (uint32)r_s8(0x800DD810));
    return t3_80077590(0x8010BE40, (sint32)(offset + index));
}

void t3_bns_07_800d3604(void)
{
    uint32 selection;
    FUNCTION_MARKER(0x800D3604, "SLUS_004.02-BNS-logical-07");
    t3_80029638(1);
    t3_8006e37c(0x80109FE0);
    t3_80029638(0);
    selection = r_u8(0x80097F20);
    w_u8(0x800DD810, selection);
    selection = (uint32)(sint32)(sint8)selection;
    w_u32(0x801BFE40, 0x800B9384 + selection * 45);
    t3_800529cc(0x8010BE40, (sint32)(selection + 69));
}

void t3_bns_07_800d368c(void)
{
    FUNCTION_MARKER(0x800D368C, "SLUS_004.02-BNS-logical-07");
    t3_8006ae14(0, 1);
}
