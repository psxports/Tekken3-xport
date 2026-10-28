#include "game_functions.h"

uint32 t3_80048ff8(const uint32 *position)
{
    uint32 spacing, x, z, tile_x, tile_z, count, uv_table, cache, depths, cell_x, cell_z, origin_x, origin_z;
    uint32 divisor, value, first, second, i, vertices, vertex_count, data, ordering, packed_indices, packet, buffer;
    uint32 distance, sum, dx, dz, row, column, quad, vertex[4], valid, table, descriptor, texture, layer, uv, flip, field;
    uint32 cached, amplitude_base, scale, product, tag, dimensions, result;
    sint32 screen_y, shade, offset;
    FUNCTION_MARKER(0x80048FF8, "SLUS_004.02");
    spacing = r_u32(0x8009E780);
    x = position[0];
    z = position[2];
    x += (sint32)spacing / 2;
    z += (sint32)spacing / 2;
    tile_x = spacing ? (uint32)((sint64)(sint32)x / (sint32)spacing) : ((sint32)x < 0 ? 1 : 0xFFFFFFFF);
    count = r_u32(0x8009E82C);
    uv_table = 0x1F800000 + (count + 2) * 4;
    cache = uv_table + 32;
    depths = cache + (uint32)((sint32)(count + 3) / 2) * 2;
    if ((sint32)x < 0)
        --tile_x;
    origin_x = tile_x * spacing;
    tile_z = spacing ? (uint32)((sint64)(sint32)z / (sint32)spacing) : ((sint32)z < 0 ? 1 : 0xFFFFFFFF);
    if ((sint32)z < 0)
        --tile_z;
    divisor = r_u32(0x8009E824);
    cell_x = divisor ? (uint32)((sint64)(sint32)tile_x % (sint32)divisor) : tile_x;
    origin_z = tile_z * spacing;
    if ((sint32)cell_x < 0)
        cell_x += divisor;
    divisor = r_u32(0x8009E828);
    cell_z = divisor ? (uint32)((sint64)(sint32)tile_z % (sint32)divisor) : tile_z;
    if ((sint32)cell_z < 0)
        cell_z += divisor;
    cell_z = divisor - (cell_z + 1);
    for (i = 0; i < 4; ++i)
    {
        first = r_u32(0x8009E800 + i * 8);
        first = r_u32(0x8009E800 + i * 8);
        second = r_u32(0x8009E804 + i * 8);
        second = r_u32(0x8009E804 + i * 8);
        w_u32(uv_table + i * 8, first);
        w_u32(uv_table + i * 8, first);
        w_u32(uv_table + i * 8 + 4, second);
        w_u32(uv_table + i * 8 + 4, second);
    }
    t3_8004adc4(origin_x, 0, origin_z);
    data = r_u32(0x800ADD14);
    vertices = r_u32(data + 12);
    vertex_count = r_u32(data + 16);
    t3_8004a364(vertices, 0x1F800000, depths, vertex_count);
    count = (r_u32(0x8009E82C) + 3) >> 2;
    for (i = 0; i < count; ++i)
        w_u32(cache + i * 4, 0xFFFFFFFF);
    data = r_u32(0x800ADD14);
    ordering = r_u32(r_u32(0x800A8C54) + 4) + 4084;
    packed_indices = r_u32(data + 28);
    buffer = r_u32(0x800ADEFC);
    packet = data ? data + buffer * 5200 + 1508 : 0;
    if (r_u32(0x800AFA88) == 8)
    {
        distance = r_u32(0x800A85DC);
        if ((sint32)distance < 0)
            distance = 0 - distance;
    }
    else
    {
        sum = r_u32(0x800AA190);
        sum += r_u32(0x800ABA1C);
        dx = r_u32(0x800A85D4) - (uint32)((sint32)sum / 2);
        sum = r_u32(0x800AA198);
        sum += r_u32(0x800ABA24);
        dz = r_u32(0x800A85DC) - (uint32)((sint32)sum / 2);
        distance = SquareRoot0((sint32)(dx * dx + dz * dz));
    }
    result = r_u32(0x8009E828);
    if ((sint32)result <= 0)
        return result;
    row = 0;
    do
    {
        dimensions = r_u32(0x8009E824);
        column = 0;
        x = cell_x;
        if ((sint32)dimensions > 0)
        {
            do
            {
                offset = (sint16)((column * 4 + row * 40) & 0xFFFF);
                quad = r_u32(packed_indices + (uint32)offset);
                for (i = 0; i < 4; ++i)
                    vertex[i] = (quad >> (i * 8)) & 255;
                valid = 1;
                for (i = 0; i < 4; ++i)
                    if (!r_u16(depths + vertex[i] * 2))
                    {
                        valid = 0;
                        break;
                    }
                if (valid)
                {
                    valid = 0;
                    for (i = 0; i < 4; ++i)
                    {
                        screen_y = (sint16)r_u16(0x1F800002 + vertex[i] * 4);
                        if (screen_y >= 0 && screen_y < 480)
                        {
                            valid = 1;
                            break;
                        }
                    }
                }
                if (valid)
                {
                    valid = 0;
                    for (i = 0; i < 4; ++i)
                        if (r_u16(0x1F800000 + vertex[i] * 4) < 368)
                        {
                            valid = 1;
                            break;
                        }
                }
                if (valid)
                {
                    for (i = 0; i < 3; ++i)
                        w_u32(packet + 8 + i * 12, r_u32(0x1F800000 + vertex[i] * 4));
                    value = r_u32(0x1F800000 + vertex[3] * 4);
                    data = r_u32(0x8009E7F8);
                    w_u32(packet + 44, value);
                    table = data + cell_z * 80 + 4 + x * 8;
                    descriptor = r_u32(table);
                    layer = r_u8(0x8009E790 + cell_z * 10 + x);
                    amplitude_base = r_s16(data + 806);
                    texture = r_u32(table + 4);
                    uv = uv_table + (descriptor >> 30) * 8;
                    descriptor &= 0x3FFFFFFF;
                    if (amplitude_base)
                    {
                        for (i = 0; i < 4; ++i)
                        {
                            cached = r_u8(cache + vertex[i]);
                            if (cached == 255)
                            {
                                value = r_u16(depths + vertex[i] * 2) - distance;
                                if ((sint32)value < 0)
                                    value = 0;
                                divisor = r_u32(0x800AE21C);
                                scale = divisor ? (uint32)((sint64)0x4000000 / (sint32)divisor) : 0xFFFFFFFF;
                                product = value * scale;
                                offset = (sint32)product >> 16;
                                if (offset >= 1024)
                                    offset = 1023;
                                amplitude_base = r_u32(0x8009E7F8);
                                value = r_u8(0x8001DF6C + (uint32)offset);
                                product = value * (uint32)r_s16(amplitude_base + 806);
                                shade = ((sint32)product >> 7) + 128;
                                if (shade < 0)
                                    shade = 0;
                                else if (shade >= 255)
                                    shade = 254;
                                cached = shade;
                                w_u8(cache + vertex[i], cached);
                            }
                            else
                                cached = r_u8(cache + vertex[i]);
                            w_u8(packet + 6 + i * 12, cached);
                            w_u8(packet + 5 + i * 12, cached);
                            w_u8(packet + 4 + i * 12, cached);
                        }
                    }
                    flip = (descriptor >> 25) & 3;
                    for (i = 0; i < 4; ++i)
                    {
                        field = (descriptor & 0xFFFF) + r_u16(uv + (i ^ flip) * 2);
                        if (i == 0)
                            field |= texture;
                        else if (i == 1)
                            field |= descriptor & 0x39FF0000;
                        w_u32(packet + 12 + i * 12, field);
                    }
                    table = ordering - layer * 4;
                    tag = (r_u32(table) & 0xFFFFFF) | 0x0C000000;
                    w_u32(packet, tag);
                    tag = r_u32(table);
                    w_u32(table, (tag & 0xFF000000) | (packet & 0xFFFFFF));
                }
                dimensions = r_u32(0x8009E824);
                ++x;
                packet += 52;
                if ((sint32)x >= (sint32)dimensions)
                    x = 0;
                ++column;
            } while ((sint32)column < (sint32)dimensions);
        }
        --cell_z;
        if ((sint32)cell_z < 0)
            cell_z = r_u32(0x8009E828) - 1;
        dimensions = r_u32(0x8009E828);
        ++row;
        result = 0x800A0000;
    } while ((sint32)row < (sint32)dimensions);
    return result;
}

uint32 t3_800489d4(uint32 *output)
{
    uint32 pitch, height, index, magnitude, product, numerator, horizontal, distance, extent_x, extent_z, radius;
    uint32 yaw, x, z, quadrant, parity, clip, limit_x, limit_z, offset, camera, result;
    sint32 cosine, sine, denominator;
    FUNCTION_MARKER(0x800489D4, "SLUS_004.02");
    pitch = r_u32(0x800A85C0);
    height = r_u32(0x800A85D8);
    index = ((pitch + 239) & 0xFFF) * 2;
    cosine = r_s16(0x8001F0C4 + index);
    magnitude = (sint32)height < 0 ? 0 - height : height;
    numerator = magnitude * (uint32)cosine;
    denominator = r_s16(0x8001E8C4 + index);
    horizontal = denominator ? (uint32)((sint64)(sint32)numerator / denominator) : ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
    distance = r_u32(0x8009E780);
    product = distance * r_u32(0x8009E824);
    extent_x = (sint32)product / 2;
    product = distance * r_u32(0x8009E828);
    extent_z = (sint32)product / 2;
    radius = t3_8004aedc(horizontal * horizontal + height * height) * 184;
    radius = (sint32)radius / 536;
    yaw = r_u32(0x800A85C4);
    index = ((yaw + 1024) & 0xFFF) * 2;
    cosine = r_s16(0x8001F0C4 + index);
    sine = r_s16(0x8001E8C4 + index);
    product = horizontal * (uint32)cosine;
    x = r_u32(0x800A85D4) + (sint32)product / 4096;
    product = horizontal * (uint32)sine;
    z = r_u32(0x800A85DC) + (sint32)product / 4096;
    quadrant = ((yaw + 1536) & 0xFFF) >> 10;
    parity = (((yaw + 1536) & 0xFFF) >> 9) & 1;
    product = r_u32(0x8009E780) << 12;
    clip = (sint32)product / 300;
    product = ((uint32)-1230 - height) * clip;
    clip = (sint32)product / 4096;
    limit_x = (sint32)extent_x < (sint32)clip ? extent_x : clip;
    limit_z = (sint32)extent_z < (sint32)clip ? extent_z : clip;
    if (quadrant == 0 || quadrant == 2)
    {
        product = radius * (uint32)sine;
        offset = (sint32)product / 4096;
        x = parity ? x - offset : x + offset;
        distance = (sint32)r_u32(0x8009E780) / 2;
        if (quadrant == 0)
        {
            x -= distance;
            camera = r_u32(0x800A85D4);
            x += limit_x;
        }
        else
        {
            x += distance;
            camera = r_u32(0x800A85D4);
            x -= limit_x;
        }
        numerator = (uint32)sine * (x - camera);
        offset = cosine ? (uint32)((sint64)(sint32)numerator / cosine) : ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
        result = r_u32(0x800A85DC);
        z = offset + result;
    }
    else
    {
        product = radius * (uint32)cosine;
        offset = (sint32)product / 4096;
        z = parity ? z + offset : z - offset;
        distance = (sint32)r_u32(0x8009E780) / 2;
        if (quadrant == 1)
        {
            z -= distance;
            camera = r_u32(0x800A85DC);
            z += limit_z;
        }
        else
        {
            z += distance;
            camera = r_u32(0x800A85DC);
            z -= limit_z;
        }
        numerator = (uint32)cosine * (z - camera);
        offset = sine ? (uint32)((sint64)(sint32)numerator / sine) : ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
        result = r_u32(0x800A85D4);
        x = offset + result;
    }
    output[0] = x;
    output[2] = z;
    return result;
}

uint32 t3_8004b928(uint32 order, uint32 packet, uint32 x, uint32 y, uint32 code, uint32 flags)
{
    uint32 colors[3], shade, table, i, xy, first, second, index, uv, page, size;
    FUNCTION_MARKER(0x8004B928, "SLUS_004.02");
    if (!(flags & 8))
    {
        shade = (flags & 16) ? t3_8004dfe0(r_u32(0x800AE218) << 4) : 128;
        table = 0x80021664 + (flags & 3) * 40;
        for (i = 0; i < 3; ++i)
            colors[i] = t3_8004df6c(r_u32(table + i * 4), shade << 1) | 0x40000000;
        xy = (x & 0xFFFF) | (y << 16);
        table = 0x800215EC;
        for (i = 0; i < 10; ++i, table += 12)
        {
            first = r_u32(table + 4);
            second = r_u32(table + 8);
            index = r_u32(table);
            packet = t3_8004da5c(order, packet, colors[index], xy + first, xy + second);
        }
        x += 2;
        y += 3;
    }
    if (flags & 0x40)
    {
        packet = t3_8004d9bc(order, packet, 0x65000000, (x & 0xFFFF) | ((y - 5) << 16), 0x200020, 0x7F1DE000);
        packet = t3_8004d9bc(order, packet, 0x65000000, (x & 0xFFFF) | ((y + 27) << 16), 0x200020, 0x7F1DE020);
        packet = t3_8004daa8(order, packet, 31);
    }
    if (flags & 0x80)
        return packet;
    index = (uint32)((sint32)code >> 2);
    if (code == 89)
        ++index;
    table = 0x8002152C + index * 8;
    uv = r_u32(table);
    page = r_u16(table + 4);
    if (flags & 0x20)
        uv = (uv & 0xFFFF) | 0x7D500000;
    xy = (x & 0xFFFF) | (y << 16);
    if (index == 20)
    {
        packet = t3_8004d9bc(order, packet, 0x65000000, xy, 0x180020, uv);
        uv += 32;
        packet = t3_8004d9bc(order, packet, 0x65000000, (x & 0xFFFF) | ((y + 24) << 16), 0x180020, uv);
        uv += 32;
        xy = (x & 0xFFFF) | ((y + 48) << 16);
        size = 0xA0020;
    }
    else if ((sint32)index < 22)
        size = 0x3A0020;
    else
    {
        packet = t3_8004d9bc(order, packet, 0x65000000, xy, 0xA0020, uv);
        uv &= 0xFFFF00FF;
        packet = t3_8004d9bc(order, packet, 0x65000000, (x & 0xFFFF) | ((y + 10) << 16), 0x180020, uv + 32);
        uv += 64;
        xy = (x & 0xFFFF) | ((y + 34) << 16);
        size = 0x180020;
    }
    packet = t3_8004d9bc(order, packet, 0x65000000, xy, size, uv);
    return t3_8004daa8(order, packet, page);
}

uint32 t3_800711ac(uint32 object)
{
    uint32 actor, animation, code, flags, owner, buffer, state, x, y, z, vx, vy, vz, active, random, factor, threshold, child, result;
    sint32 frame;
    FUNCTION_MARKER(0x800711AC, "SLUS_004.02");
    if (r_u32(object + 76))
    {
        actor = (r_u8(object + 13) & 0x10) ? 0x800A9228 : 0x800AAAB4;
        animation = r_u32(actor + 84);
        code = r_u8(r_u32(animation + 40));
        result = 1;
        if (code == 24)
        {
            frame = r_s16(actor + 88);
            result = frame < r_u8(animation + 45);
        }
        if (result)
            w_u32(object + 76, 0);
        owner = r_u8(object + 13) & 0x10;
        buffer = r_u32(0x800ADEFC);
        if (owner ? buffer != 0 : buffer == 0)
        {
            x = r_u32(object + 40);
            y = r_u32(object + 44);
            z = r_u32(object + 48);
            w_u32(object + 52, x);
            w_u32(object + 56, y);
            w_u32(object + 60, z);
        }
    }
    state = r_u8(object + 14);
    switch (state)
    {
    case 0:
        t3_80071540(object);
        t3_800716c0(object);
        break;
    case 1: t3_800716c0(object); break;
    case 2: t3_800717c4(object); break;
    case 3: t3_800718e0(object); break;
    default: break;
    }
    vx = r_s16(object + 30);
    x = r_u32(object + 40);
    vy = r_s16(object + 34);
    vz = r_s16(object + 38);
    w_u32(object + 40, x + vx);
    y = r_u32(object + 44);
    z = r_u32(object + 48);
    w_u32(object + 44, y + vy);
    w_u32(object + 48, z + vz);
    if (r_s32(object + 44) >= -215)
    {
        w_u32(object + 44, -216);
        w_u16(object + 16, 0);
        w_u32(object + 32, 0);
        t3_8006e0d0(object + 16);
    }
    flags = r_u8(object + 13);
    active = r_u32(object + 76);
    w_u8(object + 13, flags & 0xFD);
    if (active)
    {
        buffer = r_u32(0x800ADEFC);
        if ((flags & 0x10) ? buffer == 0 : buffer != 0)
            w_u8(object + 13, (flags & 0xFD) | 2);
    }
    random = (uint32)randPSX();
    factor = 80 - (uint32)r_s16(0x800A9194);
    threshold = (factor * 15) << 14;
    threshold = (uint32)((sint32)threshold / 80000) << 2;
    if ((sint32)random < (sint32)threshold)
    {
        child = t3_8006f4f4();
        if (child != 0xFFFFFFFF)
        {
            w_u8(child + 12, 7);
            w_u32(child + 40, r_u32(object + 40));
            w_u32(child + 44, r_u32(object + 44));
            w_u32(child + 48, r_u32(object + 48));
            w_u16(child + 16, r_u16(object + 16));
            w_u16(child + 18, r_u16(object + 18));
        }
    }
    else if (r_u32(object + 44) == (uint32)-216 && r_u32(object + 76))
    {
        threshold = (factor * 9) << 14;
        threshold = (uint32)((sint32)threshold / 80000) << 2;
        random = (uint32)randPSX();
        if ((sint32)random < (sint32)threshold)
        {
            child = t3_8006f4f4();
            if (child != 0xFFFFFFFF)
            {
                w_u8(child + 12, 9);
                w_u32(child + 40, r_u32(object + 40));
                w_u32(child + 48, r_u32(object + 48));
            }
        }
    }
    result = (uint32)r_u8(object + 14) - 1 < 2;
    if (result)
    {
        result = ((sint32)random >> 8) & 15;
        if (!result)
            return t3_80039f0c((const uint32 *)psx_addr(object + 40, 12), 0x8002517C);
    }
    return result;
}

uint32 t3_80061b88(uint32 state)
{
    uint32 opponent, distance, frame_bits, actor, code_bits, heading_bits, animation, choice;
    sint32 age, code, frame, heading, level;
    FUNCTION_MARKER(0x80061B88, "SLUS_004.02");
    opponent = r_u32(0x800A867C);
    distance = r_u32(state + 28);
    age = r_s16(state + 0x20A);
    frame_bits = r_u16(opponent + 0x58);
    code = r_s16(opponent + 0xA0);
    actor = r_u32(0x800AFA5C);
    code_bits = r_u16(opponent + 0xA0);
    heading_bits = r_u16(actor + 0x3E);
    frame = (sint16)frame_bits;
    heading = (sint16)heading_bits;
    if (code == 0x16F)
    {
        if (r_s32(state + 24) < 3)
            w_u16(state + 104, -1);
        return 0xFFFFFFFF;
    }
    if (code == 0x456)
    {
        if (r_s32(state + 24) >= 3)
            return 0x2000;
        w_u16(state + 140, 4);
        t3_80059394(state);
        w_u16(state + 104, -1);
        return 0xFFFFFFFF;
    }
    if (code_bits - 0x459 < 3)
    {
        level = r_s32(state + 20);
        if (level <= 0)
        {
            animation = r_u32(opponent + 84);
            if (frame >= r_u8(animation + 45) && r_u8(animation + 46) >= frame)
                return 0;
            t3_80059394(state);
            w_u16(state + 104, -1);
            w_u16(state + 140, 4);
            return 0xFFFFFFFE;
        }
        if ((uint32)r_u16(opponent + 48) - 0x2000 < 0x4001)
        {
            level = r_s32(state + 20);
            w_u16(state + 140, 20);
            if (level < 2)
            {
                t3_80059394(state);
                w_u16(state + 104, -1);
                return 0xFFFFFFFF;
            }
            return level > 1 ? 0x2000 : 0xFFFFFFFF;
        }
        if (level >= 3)
        {
            level = r_s32(state + 20);
            w_u16(state + 140, 10);
            return level > 2 ? 0x2000 : 0xFFFFFFFF;
        }
        if ((uint32)randPSX() & 7)
            return 0;
        if (r_u32(state + 0x224) & 0x400001)
            return 0;
        t3_80061a58(state, 0xFFFFFFFF);
        return 0xFFFFFFFE;
    }
    if (code == 0x472)
        return 0xFFFFFFFF;
    if (code == 0x16A || code == 0x43A)
    {
        animation = r_u32(opponent + 84);
        if (r_u8(animation + 45) && r_u8(animation + 46) < frame)
        {
            w_u16(state + 104, -1);
            return 0;
        }
        if (heading > 4096)
            return r_s32(state + 24) < 2 ? 0xFFFFFFFF : 0x2000;
        if (r_s32(state + 24) >= 3 && distance < 0xA8D)
        {
            t3_800593a4(state);
            return 0xFFFFFFFE;
        }
        if (r_s32(state + 24) < 2 && age >= 11)
            return 0xFFFFFFFF;
        if (r_s32(state + 24) >= 3 || age < 11)
            return 0;
        choice = (uint32)randPSX() & 1;
        t3_80061a58(state, choice);
        return 0xFFFFFFFE;
    }
    if (code != 0x458 && code != 0x49C && code != 0x49D && code != 0x49E)
        return 0xFFFFFFFF;
    if (heading >= 0x4000)
    {
        if (r_s32(state + 20) < 2)
            w_u16(state + 104, -1);
        return 0xFFFFFFFF;
    }
    level = r_s32(state + 20);
    if (level < 2)
    {
        w_u16(state + 104, 1);
        return 0;
    }
    if (level >= 3)
        return 0x2000;
    t3_80059394(state);
    w_u16(state + 104, -1);
    return 0xFFFFFFFF;
}

uint32 t3_8006251c(uint32 state)
{
    uint32 seed, actor, opponent, eligible, random;
    sint32 threshold, cooldown, posture;
    FUNCTION_MARKER(0x8006251C, "SLUS_004.02");
    seed = r_u32(0x800ADCA0);
    w_u32(0x800ADCA0, seed * 5 + 3);
    threshold = r_s16(state + 0x24E);
    actor = r_u32(0x800AFA5C);
    eligible = (sint32)(seed & 0xFFF) < threshold;
    cooldown = r_s16(state + 0x68);
    opponent = r_u32(0x800A867C);
    if (cooldown > 0 || !eligible)
        return 0xFFFFFFFF;
    if (r_s16(opponent + 22) != 19
        && (r_u32(state + 0x230) & 2)
        && r_s32(state + 20) < 3
        && !r_u8(opponent + 0xDB)
        && !r_u8(opponent + 0xDF)
        && !r_u8(r_u32(actor + 84) + 45)
        && r_s16(state + 0x20A) >= 9
        && !r_s16(actor + 50)
        && !r_s16(actor + 64)
        && !((uint32)randPSX() & 7)
        && t3_80058be8(21, state))
    {
        random = (uint32)randPSX() & 0xFFF;
        t3_8006183c(state, 0x800230DE + (random % 3) * 14);
        w_u16(state + 0xD8, 1);
        return 0xFFFFFFFE;
    }
    if ((r_u32(state + 0x230) & 1)
        && r_s32(state + 20) < 2
        && !r_u8(r_u32(0x800A867C) + 0xDF)
        && !r_u8(r_u32(actor + 84) + 45)
        && r_s16(state + 0x20A) >= 9
        && !r_s16(actor + 50)
        && !r_s16(actor + 64)
        && t3_80057018(state))
    {
        random = (uint32)randPSX() & 1;
        t3_8006183c(state, 0x800230C2 + random * 14);
        return 0xFFFFFFFE;
    }
    if (!r_s16(state + 0x208) || r_s32(state + 20) >= 2)
        return 0xFFFFFFFF;
    posture = r_s16(actor + 64);
    if (posture && posture != 2)
        return 0xFFFFFFFF;
    if (r_s16(actor + 50)
        || r_u8(r_u32(0x800A867C) + 0xDF)
        || r_u8(r_u32(actor + 84) + 45)
        || r_s16(state + 0x20A) < 9
        || r_s16(state + 0x88)
        || !t3_80057018(state))
        return 0xFFFFFFFF;
    random = (uint32)randPSX() & 1;
    t3_8006183c(state, 0x800230A6 + random * 14);
    return 0xFFFFFFFE;
}

uint32 t3_80071e5c(uint32 event)
{
    uint32 flags, actor, animation, code, state, x, y, z, vx, vy, vz, threshold, random, child, result;
    sint32 frame;
    FUNCTION_MARKER(0x80071E5C, "SLUS_004.02");
    flags = r_u8(event + 13);
    if (flags & 2)
    {
        if (r_u32(0x800954A4))
        {
            if (flags & 0x10)
            {
                if (r_u8(0x800ADED6) & 8)
                    return t3_8006f5ac(event);
            }
            else
            {
                if (r_u8(0x800ADED6) & 0x10)
                    return t3_8006f5ac(event);
                flags = r_u8(event + 13);
            }
        }
        else
        {
            actor = (r_u8(event + 13) & 0x10) ? 0x800A9228 : 0x800AAAB4;
            animation = r_u32(actor + 0x54);
            code = r_u8(r_u32(animation + 40));
            result = 1;
            if (code == 26)
            {
                frame = r_s16(actor + 0x58);
                result = frame < r_u8(animation + 45);
            }
            if (result)
                w_u8(event + 13, r_u8(event + 13) & 0xFD);
            flags = r_u8(event + 13);
        }
        if ((flags & 2) && (uint32)r_u8(event + 14) - 1 < 2)
        {
            actor = (flags & 0x10) ? 0x800AAAB4 : 0x800A9228;
            if (r_u8(actor + 0xD0))
                return t3_8006f5ac(event);
            if (r_u8(actor + 0xCF))
                w_u8(event + 14, 3);
        }
    }
    state = r_u8(event + 14);
    switch (state)
    {
    case 0:
        t3_80072194(event);
        t3_800722d4(event);
        break;
    case 1: t3_800722d4(event); break;
    case 2: t3_800723cc(event); break;
    case 3:
        t3_800724a4(event);
        t3_800724c4(event);
        break;
    case 4: t3_800724c4(event); break;
    default: break;
    }
    x = r_u32(event + 40);
    y = r_u32(event + 44);
    z = r_u32(event + 48);
    w_u32(event + 52, x);
    w_u32(event + 56, y);
    w_u32(event + 60, z);
    vx = r_s16(event + 30);
    x = r_u32(event + 40);
    vy = r_s16(event + 34);
    vz = r_s16(event + 38);
    w_u32(event + 40, x + vx);
    y = r_u32(event + 44);
    z = r_u32(event + 48);
    w_u32(event + 44, y + vy);
    w_u32(event + 48, z + vz);
    if (r_s32(event + 44) >= -215)
    {
        w_u32(event + 44, -216);
        w_u16(event + 16, 0);
        w_u32(event + 32, 0);
        t3_8006e0d0(event + 16);
    }
    threshold = ((80 - (uint32)r_s16(0x800A9194)) * 15) << 14;
    threshold = (uint32)((sint32)threshold / 80000) << 2;
    random = (uint32)randPSX();
    if ((sint32)random < (sint32)threshold)
    {
        child = t3_8006f4f4();
        if (child != 0xFFFFFFFF)
        {
            w_u8(child + 12, 7);
            w_u32(child + 40, r_u32(event + 40));
            w_u32(child + 44, r_u32(event + 44));
            w_u32(child + 48, r_u32(event + 48));
            w_u16(child + 16, r_u16(event + 16));
            w_u16(child + 18, r_u16(event + 18));
        }
    }
    result = ((sint32)random >> 8) & 15;
    if (!result)
        return t3_80039f0c((const uint32 *)psx_addr(event + 40, 12), 0x8002517C);
    return result;
}

uint32 t3_80031e48(uint32 value)
{
    uint32 available, index, actor, destination, current, cursor, distance;
    FUNCTION_MARKER(0x80031E48, "SLUS_004.02");
    available = r_u32(0x8009BCD0);
    if ((sint32)available < (sint32)value)
        value = available - 2;
    w_u32(0x8009BC98, r_u16(0x800ADC84));
    actor = 0x800A9228;
    destination = 0x8009BCA0;
    for (index = 0; index < 2; ++index, actor += 0x188C, destination += 3)
    {
        w_u8(destination, r_u8(actor + 20));
        w_u8(destination + 1, r_u8(actor + 22));
        w_u8(destination + 2, r_u8(actor + 26));
    }
    if (!value && r_s32(0x8009BCC0) < 0)
    {
        current = r_u32(0x8009BCB8);
        w_u32(0x8009BCA8, 5);
        cursor = current * 2 - 320;
        w_u32(0x8009BCC0, cursor);
        if ((sint32)cursor < 0)
        {
            cursor = current * 2 + 18880;
            w_u32(0x8009BCC0, cursor);
        }
        cursor = r_u32(0x8009BCC0);
        w_u32(0x8009BCC0, (sint32)cursor % 600);
        return t3_8004a678();
    }
    if ((sint32)value >= 299)
        value = 298;
    if (value)
    {
        cursor = r_u32(0x8009BCB8) - value;
        if ((sint32)cursor < 0)
            cursor += 19200;
        w_u32(0x8009BCC0, (uint32)((sint32)cursor % 300) * 2);
    }
    cursor = r_u32(0x8009BCC0);
    current = r_u32(0x8009BCB8);
    distance = current - ((sint32)cursor >> 1);
    if ((sint32)distance < 0)
        distance += 19200;
    distance = (sint32)distance % 300;
    if ((sint32)distance < 150 && r_s32(0x8009BCD0) >= 153)
        w_u32(0x8009BCC0, (current - 150) * 2);
    if (r_u32(0x800AFA88) == 7)
        w_u32(0x8009BCC0, (r_u32(0x8009BCB8) - 30) * 2);
    cursor = r_u32(0x8009BCC0);
    if ((sint32)cursor < 0)
    {
        cursor += 19200;
        w_u32(0x8009BCC0, cursor);
    }
    cursor = r_u32(0x8009BCC0);
    w_u32(0x8009BCA8, 2);
    w_u32(0x800A8D6C, 20);
    cursor = (sint32)cursor % 600;
    w_u32(0x8009BCBC, cursor);
    current = r_u32(0x8009BCB8);
    w_u32(0x8009BCC0, cursor);
    distance = current - ((sint32)cursor >> 1);
    if ((sint32)distance < 0)
        distance += 19200;
    distance = (sint32)distance % 300;
    t3_8004a83c();
    t3_8007666c();
    t3_8004a684(distance);
    return t3_8004a678();
}

uint32 t3_800561e0(uint32 level)
{
    uint32 index, record = 0x80098214, kind, bit_index, mask, flags, j;
    FUNCTION_MARKER(0x800561E0, "SLUS_004.02");
    if (level >= 15)
        level = 14;
    for (index = 0; index < level; ++index, record += 2)
    {
        kind = r_u8(record);
        bit_index = r_u8(record + 1);
        switch (kind)
        {
        case 0:
            w_u16(0x80097F1C, 0);
            break;
        case 1:
            flags = r_u32(0x80097EF0);
            mask = 1u << (bit_index & 31);
            if (!(flags & mask))
            {
                w_u8(0x80097F12, bit_index);
                w_u16(0x80097F1C, 0);
            }
            w_u32(0x80097EF0, flags | mask);
            break;
        case 2:
            flags = r_u32(0x80097EF4);
            mask = 1u << (bit_index & 31);
            if (!(flags & mask))
                w_u16(0x80097F1C, 0);
            w_u32(0x80097EF4, (flags | mask) & 0x50382);
            break;
        case 3:
            if ((r_u32(0x80097EF8) & 0x3FF) == 0x3FF && !r_u8(0x80097F27))
            {
                w_u8(0x80097F27, 1);
                w_u16(0x80097F1C, 0);
            }
            break;
        case 4:
            if (!r_u8(0x80097F26))
            {
                w_u8(0x80097F26, 1);
                w_u16(0x80097F1C, 0);
            }
            break;
        case 5:
            for (j = 0; j < 22; ++j)
            {
                mask = 1u << j;
                if ((0x1FFFFF >> j) & 1)
                {
                    flags = r_u32(0x80097EF0);
                    if (!(flags & mask))
                    {
                        w_u8(0x80097F12, j);
                        w_u16(0x80097F1C, 0);
                    }
                    w_u32(0x80097EF0, flags | mask);
                }
                if ((0x50382 >> j) & 1)
                {
                    flags = r_u32(0x80097EF4);
                    if (!(flags & mask))
                        w_u16(0x80097F1C, 0);
                    w_u32(0x80097EF4, (flags | mask) & 0x50382);
                }
            }
            if (!r_u8(0x80097F26))
            {
                w_u8(0x80097F26, 1);
                w_u16(0x80097F1C, 0);
            }
            if (!r_u8(0x80097F27))
            {
                w_u8(0x80097F27, 1);
                w_u16(0x80097F1C, 0);
            }
            flags = r_u32(0x80097EF0);
            if (flags & 0x80000)
                w_u8(0x80097F1F, 2);
            else
                w_u8(0x80097F1F, t3_8004ccac(flags) >= 15);
            break;
        default:
            break;
        }
    }
    if ((r_u32(0x80097EF8) & 0x3FF) == 0x3FF && !r_u8(0x80097F27))
    {
        w_u8(0x80097F27, 1);
        w_u16(0x80097F1C, 0);
    }
    flags = r_u32(0x80097EF0);
    if (flags & 0x80000)
    {
        w_u8(0x80097F1F, 2);
        return 2;
    }
    w_u8(0x80097F1F, t3_8004ccac(flags) >= 15);
    return 1;
}

uint32 t3_80038ce4(uint32 data, uint16 *output, uint32 sample)
{
    uint32 header, channels, length, channel, segments, segment, packed, cursor, target, position, mode, span, i, start, end, value;
    FUNCTION_MARKER(0x80038CE4, "SLUS_004.02");
    header = r_u16(data);
    cursor = data + 2;
    channels = header >> 10;
    length = header & 0x3FF;
    for (channel = 0; channel < channels; ++channel)
    {
        packed = r_u16(cursor);
        cursor += 2;
        segments = (packed >> 12) + 1;
        target = (channel & 1) ? length - (sample + 1) : sample;
        position = 0;
        for (segment = 0; segment < segments; ++segment)
        {
            if (segment)
            {
                packed = r_u16(cursor);
                cursor += 2;
            }
            mode = (packed >> 10) & 3;
            span = packed & 0x3FF;
            if ((sint32)target < (sint32)position)
            {
                if (mode != 2)
                    position += span;
                switch (mode)
                {
                case 0: cursor += 2; break;
                case 1: cursor += 4; break;
                case 2:
                    cursor += 2;
                    for (i = 1; i < span; ++i, cursor += 2)
                        position += r_u16(cursor) & 31;
                    break;
                case 3: cursor += span * 2; break;
                }
                continue;
            }
            end = position + span;
            switch (mode)
            {
            case 0:
                if ((sint32)target < (sint32)end)
                    *output++ = r_u16(cursor);
                cursor += 2;
                position = end;
                break;
            case 1:
                if ((sint32)target < (sint32)end)
                {
                    value = (uint32)r_s16(cursor + 2) * (target - position);
                    if (length >= 10)
                        value = (sint32)value / 32;
                    value += r_s16(cursor);
                    *output++ = (uint16)value;
                }
                cursor += 4;
                position = end;
                break;
            case 2:
                start = position;
                for (i = 1; i < span; ++i)
                    position += 1 + (r_u16(cursor + i * 2) & 31);
                if ((sint32)position >= (sint32)target)
                    *output++ = (uint16)t3_800387c8(cursor, target - start, position - start + 1, 4);
                cursor += span * 2;
                ++position;
                break;
            case 3:
                if ((sint32)target < (sint32)end)
                    *output++ = r_u16(cursor + (target - position) * 2);
                position = end;
                cursor += span * 2;
                break;
            }
        }
    }
    return length;
}

uint32 t3_80073900(uint32 object)
{
    uint32 state, flags, index, packet, value, first, second, random, x, y, z, angle_x, angle_y, size, result;
    FUNCTION_MARKER(0x80073900, "SLUS_004.02");
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
    for (index = 0; index < 2; ++index)
    {
        packet = object + 80 + index * 40;
        w_u8(packet + 12, r_u8(0x800252E8));
        w_u8(packet + 13, r_u8(0x800252E9));
        w_u8(packet + 20, r_u8(0x800252E8) + 15);
        w_u8(packet + 21, r_u8(0x800252E9));
        w_u8(packet + 28, r_u8(0x800252E8));
        w_u8(packet + 29, r_u8(0x800252E9) + 15);
        w_u8(packet + 36, r_u8(0x800252E8) + 15);
        w_u8(packet + 37, r_u8(0x800252E9) + 15);
    }
    w_u32(object + 8, 0x800256E0);
    first = (uint32)randPSX();
    second = (uint32)randPSX();
    random = first | (second << 16);
    size = 5 * (((sint32)random >> 12) & 2047) / 4;
    x = r_u32(object + 40);
    y = r_u32(object + 44);
    w_u32(object + 68, (((sint32)random >> 10) & 31) + 20);
    w_u32(object + 64, 0);
    w_u32(object + 40, x - 127 + (random & 255));
    z = r_u32(object + 48);
    w_u32(object + 44, y - 127 + (((sint32)random >> 2) & 255));
    angle_x = r_u16(object + 16) - 63 + (((sint32)random >> 6) & 127);
    w_u32(object + 48, z - 127 + (((sint32)random >> 4) & 255));
    angle_y = r_u16(object + 18) - 63 + (((sint32)random >> 8) & 127);
    w_u16(object + 16, angle_x);
    w_u16(object + 18, angle_y);
    w_u16(object + 24, size);
    t3_8006e120(object + 16);
    value = r_u32(object + 28);
    x = r_u32(object + 40);
    result = r_u32(object + 32);
    w_u32(object + 40, x + ((sint32)value >> 13));
    y = r_u32(object + 44);
    value = r_u32(object + 36);
    w_u32(object + 44, y + ((sint32)result >> 13));
    z = r_u32(object + 48);
    result = (sint32)value >> 13;
    w_u32(object + 48, z + result);
    return result;
}

uint32 t3_80057018(uint32 state)
{
    uint32 actor, opponent, entries, animations, buttons, kind, count, record, index, code, result = 0, accepted;
    sint32 frame;
    FUNCTION_MARKER(0x80057018, "SLUS_004.02");
    actor = r_u32(0x800AFA5C);
    entries = r_u32(r_u32(actor + 0x54) + 12);
    opponent = r_u32(0x800A867C);
    animations = r_u32(state + 12);
    kind = r_u16(entries);
    buttons = r_u32(state + 100);
    while (kind != 0xC000)
    {
        count = 1;
        if (kind == 0xC00C)
        {
            index = r_u16(entries + 6);
            count = r_u8(entries + 11);
            record = 0x80017C78 + index * 12;
        }
        else
            record = entries;
        for (index = 0; index < count; ++index, record += 12)
        {
            frame = r_s16(actor + 0x58);
            if (frame < r_u8(record + 9))
                continue;
            if (r_u8(record + 10) < frame)
                continue;
            code = r_u8(record + 2);
            if (code == 0)
                accepted = 1;
            else if (code < 24)
                accepted = (uint32)r_s16(actor + 22) == code - 1;
            else if (code < 47)
                accepted = (uint32)r_s16(actor + 22) != code - 24;
            else if (code < 70)
                accepted = (uint32)r_s16(opponent + 22) == code - 47;
            else if (code < 93)
                accepted = (uint32)r_s16(actor + 24) == code - 70;
            else if (code < 116)
                accepted = (uint32)r_s16(actor + 24) != code - 93;
            else
                accepted = 0;
            if (!accepted)
                continue;
            code = r_u8(record + 3);
            switch (code)
            {
            case 0: accepted = 1; break;
            case 0x24: accepted = buttons & 1; break;
            case 0x25: accepted = buttons & 2; break;
            case 0x26: accepted = buttons & 4; break;
            case 0x30: accepted = buttons & 8; break;
            case 0x27: accepted = buttons & 16; break;
            default: accepted = t3_8002e0bc(record, actor, opponent); break;
            }
            if (accepted && (r_u32(r_u32(animations + r_u16(record + 6) * 4) + 8) & 0x400000))
            {
                result = 1;
                break;
            }
        }
        entries += 12;
        kind = r_u16(entries);
    }
    return result;
}

uint32 t3_80046a20(uint32 actor)
{
    uint32 x, z, dx, dy, dz, squared_x, squared_z, distance, amount, divisor, product, step_x, step_z;
    uint32 position[3], camera[8], old, delta, value, index, y;
    sint32 velocity_x, velocity_z, bound;
    FUNCTION_MARKER(0x80046A20, "SLUS_004.02");
    velocity_x = r_s16(0x8009E6F0);
    x = r_u32(0x8009E6DC);
    velocity_z = r_s16(0x8009E6F4);
    z = r_u32(0x8009E6E4);
    x += velocity_x;
    z += velocity_z;
    w_u32(0x8009E6DC, x);
    w_u32(0x8009E6E4, z);
    dx = r_u32(actor + 0xF68) - x;
    dy = r_u32(actor + 0xF6C);
    dy -= r_u32(0x8009E6E0);
    squared_x = dx * dx;
    dz = r_u32(actor + 0xF70) - z;
    squared_z = dz * dz;
    distance = t3_8004aedc(squared_x + dy * dy + squared_z);
    amount = 2308 - distance;
    if ((sint32)amount > 0)
    {
        divisor = t3_8004aedc(squared_x + squared_z);
        product = amount * dx;
        step_x = divisor ? (uint32)((sint64)(sint32)product / (sint32)divisor) : ((sint32)product < 0 ? 1 : 0xFFFFFFFF);
        product = amount * dz;
        step_z = divisor ? (uint32)((sint64)(sint32)product / (sint32)divisor) : ((sint32)product < 0 ? 1 : 0xFFFFFFFF);
        x = r_u32(0x8009E6DC);
        w_u32(0x8009E6DC, x - (uint32)((sint32)step_x / 16));
        z = r_u32(0x8009E6E4);
        w_u32(0x8009E6E4, z - (uint32)((sint32)step_z / 16));
    }
    position[0] = r_u32(0x8009E6DC);
    position[1] = r_u32(0x8009E6E0);
    position[2] = r_u32(0x8009E6E4);
    t3_80046270(actor, position, camera);
    old = r_u32(0x8009E6C8);
    delta = camera[0] - old;
    value = (delta & 0x20000) ? (delta | 0xFFFC0000) + 31 : delta & 0x3FFFF;
    x = old + ((sint32)value >> 5);
    old = r_u32(0x8009E6CC);
    w_u32(0x8009E6C8, x);
    delta = camera[1] - old;
    value = (delta & 0x20000) ? (delta | 0xFFFC0000) + 15 : delta & 0x3FFFF;
    w_u32(0x8009E6CC, old + ((sint32)value >> 4));
    value = r_u32(0x8009E6C8);
    if ((sint32)value > 0)
    {
        index = ((sint32)value >> 5) & 0x1FFE;
        bound = -7500 * r_s16(0x8001E8C4 + index) / 4096;
        y = r_u32(0x8009E6E0);
        if (bound < (sint32)y)
            w_u32(0x8009E6E0, y + (uint32)((sint32)((uint32)bound - y) / 16));
    }
    return t3_8004610c(0x8009E6C8);
}

uint32 t3_8002e8d0(uint32 condition, uint32 first, uint32 second)
{
    uint32 kind, code;
    FUNCTION_MARKER(0x8002E8D0, "SLUS_004.02");
    kind = r_u8(condition + 3);
    switch (kind)
    {
    case 0x45:
        if (!r_u8(first + 0xCE))
            return 0;
        if (r_s16(first + 0x30) >= 0x2AAA)
            return 0;
        return r_s16(second + 0x30) < 0x2AAA;
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
        if ((r_u32(r_u32(second + 0x54) + 0x24) >> 8) & 8)
            return 0;
        if (r_s16(first + 0x30) >= 0x2AAA)
            return 0;
        if (r_s16(second + 0x30) >= 0x2AAA)
            return 0;
        code = r_u16(second + 0x64);
        if (kind == 0x46 && code != 0x412)
            return 0;
        if (kind == 0x47 && code != 0x217)
            return 0;
        if (kind == 0x48 && code != 0x10F)
            return 0;
        if (kind == 0x49 && code != 0x412 && code != 0x217)
            return 0;
        return t3_8002eb4c(second, r_s16(condition + 4));
    case 0x4A:
        code = r_u16(condition + 4);
        if (code != (uint32)r_s16(second + 0xA0))
            return 0;
        if (r_s16(first + 0x30) >= 0x2AAA)
            return 0;
        return r_s16(second + 0x30) < 0x2AAA;
    case 0x4B:
        if (r_s16(first + 0x30) >= 0x2AAA)
            return 0;
        if (r_s16(second + 0x30) >= 0x2AAA)
            return 0;
        code = r_u32(second + 0x54);
        kind = r_s16(condition + 4);
        return t3_8002d264(r_u32(code), kind);
    default:
        return 0;
    }
}

uint32 t3_800467a0(uint32 actor)
{
    uint32 seed, first_index, second_index, index, position[3], dx, dz, side_x, side_z, x;
    sint32 longitudinal, lateral, height, heading, sine, cosine;
    FUNCTION_MARKER(0x800467A0, "SLUS_004.02");
    seed = (r_u32(0x800A912C) + 1) * 69069;
    first_index = (((seed >> 16) * 3) >> 16) * 2;
    w_u32(0x800A912C, seed);
    seed = (seed + 1) * 69069;
    second_index = (((seed >> 16) * 3) >> 16) * 2;
    longitudinal = r_s16(0x8001DF54 + first_index);
    w_u32(0x800A912C, seed);
    lateral = r_s16(0x8001DF5C + second_index);
    seed = (seed + 1) * 69069;
    index = (((seed >> 16) * 3) >> 16) * 2;
    w_u32(0x800A912C, seed);
    height = r_s16(0x8001DF64 + index);
    if (height < -1949)
        lateral = 2200;
    heading = r_s16(actor + 14);
    index = (1024 - (uint32)(heading / 16)) & 0xFFF;
    cosine = r_s16(0x8001F0C4 + index * 2);
    sine = r_s16(0x8001E8C4 + index * 2);
    w_u16(0x8009E6F0, -cosine / 512);
    side_x = (uint32)lateral * (uint32)sine;
    side_z = (uint32)lateral * (uint32)-cosine;
    dx = (uint32)longitudinal * (uint32)cosine;
    w_u16(0x8009E6F4, -sine / 512);
    dz = (uint32)longitudinal * (uint32)sine;
    seed = (seed + 1) * 69069;
    w_u32(0x800A912C, seed);
    if ((seed >> 16) <= 0x7FFF)
    {
        side_x = 0 - side_x;
        side_z = 0 - side_z;
    }
    dx += side_x;
    dz += side_z;
    x = r_u32(actor + 0xF68);
    w_u32(0x8009E6E0, height);
    position[1] = height;
    x += (sint32)dx / 4096;
    w_u32(0x8009E6DC, x);
    position[0] = x;
    position[2] = r_u32(actor + 0xF70) + (sint32)dz / 4096;
    w_u32(0x8009E6E4, position[2]);
    return t3_80046270(actor, position, (uint32 *)psx_addr(0x8009E6C8, 32));
}

uint32 t3_80073508(uint32 object)
{
    uint32 state, flags, index, packet, value, first, second, random, x, y, result;
    sint32 delta_x, delta_y;
    FUNCTION_MARKER(0x80073508, "SLUS_004.02");
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
    for (index = 0; index < 2; ++index)
    {
        packet = object + 80 + index * 40;
        w_u8(packet + 12, r_u8(0x800252E8));
        w_u8(packet + 13, r_u8(0x800252E9));
        w_u8(packet + 20, r_u8(0x800252E8) + 15);
        w_u8(packet + 21, r_u8(0x800252E9));
        w_u8(packet + 28, r_u8(0x800252E8));
        w_u8(packet + 29, r_u8(0x800252E9) + 15);
        w_u8(packet + 36, r_u8(0x800252E8) + 15);
        w_u8(packet + 37, r_u8(0x800252E9) + 15);
    }
    w_u32(object + 8, 0x800256E0);
    first = (uint32)randPSX();
    second = (uint32)randPSX();
    random = first | (second << 16);
    delta_x = ((sint32)(random & 2047) - 1023) * 2 / 3;
    w_u32(object + 68, (((sint32)random >> 4) & 31) + 10);
    delta_y = ((((sint32)random >> 2) & 2047) - 1023) * 2 / 3;
    w_u32(object + 72, ((sint32)random >> 6) & 511);
    x = r_u16(object + 16);
    w_u32(object + 64, 0);
    w_u32(object + 76, (((sint32)random >> 8) & 511) + 1664);
    w_u16(object + 16, x + (uint32)delta_x);
    y = r_u16(object + 18);
    result = y + (uint32)delta_y;
    w_u16(object + 18, result);
    return result;
}

uint32 t3_80070bc8(uint32 object)
{
    uint32 state, flags, index, packet, value, first, second, random, x, y, z, result;
    FUNCTION_MARKER(0x80070BC8, "SLUS_004.02");
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
    w_u8(object + 13, r_u8(object + 13) | 0x20);
    for (index = 0; index < 2; ++index)
    {
        packet = object + 80 + index * 40;
        w_u8(packet + 12, r_u8(0x800252E8));
        w_u8(packet + 13, r_u8(0x800252E9));
        w_u8(packet + 20, r_u8(0x800252E8) + 15);
        w_u8(packet + 21, r_u8(0x800252E9));
        w_u8(packet + 28, r_u8(0x800252E8));
        w_u8(packet + 29, r_u8(0x800252E9) + 15);
        w_u8(packet + 36, r_u8(0x800252E8) + 15);
        w_u8(packet + 37, r_u8(0x800252E9) + 15);
    }
    w_u32(object + 8, 0x80025484);
    w_u32(object + 68, 0xFFFFFFFF);
    first = (uint32)randPSX();
    second = (uint32)randPSX();
    random = first | (second << 16);
    x = r_u32(object + 40);
    y = r_u32(object + 44);
    w_u32(object + 32, 0);
    result = ((sint32)random >> 2) & 127;
    x = x - 63 + (random & 127);
    y = y - 63 + result;
    w_u32(object + 40, x);
    z = r_u32(object + 48);
    w_u32(object + 44, y);
    w_u32(object + 48, z - 63 + (((sint32)random >> 4) & 127));
    return result;
}

uint32 t3_80073054(uint32 object)
{
    uint32 actor, age, flags, record, value, buffer, table, packet, threshold, child, z, state;
    FUNCTION_MARKER(0x80073054, "SLUS_004.02");
    age = r_u32(object + 68);
    flags = r_u8(object + 13);
    w_u32(object + 68, age + 1);
    if (flags & 0x1C) {
        if (flags & 0x10)
            actor = 0x800A9228;
        else if (flags & 8)
            actor = 0x800AAAB4;
        else if (flags & 4)
            actor = 0x800AC340;
        record = actor + r_u32(object + 76) * 68;
        w_u32(object + 40, r_u32(record + 0x908));
        value = r_u32(record + 0x90C);
        buffer = r_u32(0x800ADEFC);
        w_u32(object + 44, value);
        value = r_u32(record + 0x910);
        age = r_u32(object + 68);
        w_u32(object + 48, value);
    } else {
        buffer = r_u32(0x800ADEFC);
        age = r_u32(object + 68);
    }
    table = 0x800255CC + (age & 6);
    packet = object + buffer * 40 + 80;
    w_u8(packet + 12, r_u8(table));
    w_u8(packet + 13, r_u8(table + 1));
    w_u8(packet + 20, r_u8(table) + 31);
    w_u8(packet + 21, r_u8(table + 1));
    w_u8(packet + 28, r_u8(table));
    w_u8(packet + 29, r_u8(table + 1) + 31);
    w_u8(packet + 36, r_u8(table) + 31);
    w_u8(packet + 37, r_u8(table + 1) + 31);
    value = 80 - (uint32)r_s16(0x800A9194);
    value = (value * 15) << 14;
    threshold = (uint32)((sint32)value / 80000) << 2;
    if ((sint32)randPSX() < (sint32)threshold)
    {
        child = t3_8006f4f4();
        if (child != 0xFFFFFFFF)
        {
            w_u8(child + 12, 7);
            w_u32(child + 40, r_u32(object + 40));
            w_u32(child + 44, r_u32(object + 44));
            z = r_u32(object + 48);
            w_u16(child + 16, 1024);
            w_u16(child + 18, 0);
            w_u32(child + 48, z);
        }
    }
    value = r_u32(object + 64) - 1;
    w_u32(object + 64, value);
    if ((sint32)value < 0)
    {
        state = r_u8(object + 14);
        w_u32(object + 64, 0);
        w_u32(object + 32, 0);
        w_u8(object + 14, state + 1);
    }
    flags = r_u8(object + 13);
    if (flags & 0x10)
        w_u8(0x800AAAB0, 1);
    else if (flags & 8)
        w_u8(0x800AC33C, 1);
    else if (flags & 4)
        w_u8(0x800ADBC8, 1);
    else
        return 0;
    return 1;
}

uint32 t3_80048e58(uint32 first_actor, uint32 second_actor)
{
    uint32 pitch, index, result, first, second, position[3], yaw, distance, cosine_product, sine_product;
    sint32 sine, cosine, signed_pitch;
    FUNCTION_MARKER(0x80048E58, "SLUS_004.02");
    pitch = r_u32(0x800A85C0);
    index = ((pitch + 239) & 0xFFF) * 2;
    sine = r_s16(0x8001E8C4 + index);
    result = sine > 0;
    w_u32(0x800ADFD0, result);
    if (!result)
        return 0;
    if ((pitch & 0xFFF) != 0 && (pitch & 0x800) == 0 && r_s32(0x800A85D8) < -1720)
        t3_800489d4(position);
    else
    {
        first = r_u32(first_actor + 0xF68);
        second = r_u32(second_actor + 0xF68);
        position[0] = (uint32)((sint32)(first + second) / 2);
        first = r_u32(first_actor + 0xF70);
        second = r_u32(second_actor + 0xF70);
        pitch = r_u32(0x800A85C0);
        position[2] = (uint32)((sint32)(first + second) / 2);
        signed_pitch = pitch & 0xFFF;
        if (pitch & 0x800)
            signed_pitch -= 4096;
        if (signed_pitch < -37)
        {
            yaw = r_u32(0x800A85C4);
            distance = r_u32(0x8009E780);
            index = ((yaw + 1024) & 0xFFF) * 2;
            cosine = r_s16(0x8001F0C4 + index);
            cosine_product = distance * (uint32)cosine;
            sine = r_s16(0x8001E8C4 + index);
            sine_product = distance * (uint32)sine;
            position[0] -= (uint32)((sint32)cosine_product / 4096);
            position[2] -= (uint32)((sint32)sine_product / 4096);
        }
    }
    if (r_u32(0x800AFA88) == 8)
        position[2] = 325;
    return t3_80048ff8(position);
}

uint32 t3_80042538(uint32 actor)
{
    uint32 actor_x, camera_x, actor_z, camera_z, dx, dz, index, angle, step_x, step_z, x, z;
    sint32 cosine, sine;
    FUNCTION_MARKER(0x80042538, "SLUS_004.02");
    actor_x = r_u32(actor + 0xF68);
    camera_x = r_u32(0x8009E634);
    actor_z = r_u32(actor + 0xF70);
    camera_z = r_u32(0x8009E63C);
    dx = actor_x - camera_x;
    dz = actor_z - camera_z;
    angle = dx || dz ? t3_8002a224(dz, dx) : 0;
    index = (~angle >> 3) & 0x1FFE;
    cosine = r_s16(0x8001F0C4 + index);
    sine = r_s16(0x8001E8C4 + index);
    step_x = (uint32)(cosine * 70 / 4096);
    step_z = (uint32)(sine * 70 / 4096);
    if (r_u16(0x800ADD6A) & 0x2000)
    {
        x = r_u32(0x8009E634);
        z = r_u32(0x8009E63C);
        w_u32(0x8009E634, x + step_x);
        w_u32(0x8009E63C, z + step_z);
    }
    if (r_u16(0x800ADD6A) & 0x8000)
    {
        x = r_u32(0x8009E634);
        z = r_u32(0x8009E63C);
        w_u32(0x8009E634, x - step_x);
        w_u32(0x8009E63C, z - step_z);
    }
    if (r_u16(0x800ADD6A) & 0x1000)
    {
        x = r_u32(0x8009E634);
        z = r_u32(0x8009E63C);
        w_u32(0x8009E634, x - step_z);
        w_u32(0x8009E63C, z + step_x);
    }
    if (r_u16(0x800ADD6A) & 0x4000)
    {
        x = r_u32(0x8009E634);
        z = r_u32(0x8009E63C);
        w_u32(0x8009E634, x + step_z);
        w_u32(0x8009E63C, z - step_x);
    }
    if (r_u16(0x800ADD6A) & 0x80)
        w_u32(0x8009E638, r_u32(0x8009E638) - 70);
    if (r_u16(0x800ADD6A) & 0x40)
        w_u32(0x8009E638, r_u32(0x8009E638) + 70);
    (void)r_u32(0x8009E634);
    (void)r_u32(0x8009E638);
    (void)r_u32(0x8009E63C);
    (void)r_u32(actor + 0xF68);
    (void)r_u32(actor + 0xF6C);
    return r_u32(actor + 0xF70);
}

uint32 t3_80054d00(uint32 state)
{
    uint32 selected[2], resolved[2], ordering_entry, packet, side, record, row, column, code, mask, first, second, index, table, x, y, flags;
    FUNCTION_MARKER(0x80054D00, "SLUS_004.02");
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    ordering_entry += 4;
    for (side = 0; side < 2; ++side)
    {
        selected[side] = 89;
        resolved[side] = 89;
        record = state + side * 172;
        if (r_u32(record + 48) != 1)
            continue;
        row = r_u32(record + 36);
        column = r_u32(record + 32);
        selected[side] = r_u8(0x8002276C + (column + row * 7) * 6);
        row = r_u32(record + 36);
        column = r_u32(record + 32);
        code = r_u8(0x8002276C + (column + row * 7) * 6);
        mask = 1u << ((code >> 2) & 31);
        if ((r_u32(record + 60) & mask) == 0)
            code = r_u32(0x80097EF0) & mask ? 89 : 88;
        resolved[side] = code;
    }
    if (selected[0] == selected[1] && (resolved[0] != 89 || resolved[1] != 89))
    {
        selected[1] = 89;
        selected[0] = 89;
    }
    first = resolved[0] == 89 ? selected[0] : 89;
    second = resolved[1] == 89 ? selected[1] : 89;
    table = 0x80022768;
    for (index = 0; index < 21; ++index, table += 6)
    {
        code = r_u8(table + 4);
        x = r_u16(table);
        y = r_u16(table + 2);
        flags = code == first || code == second ? 32 : 0;
        mask = 1u << ((code >> 2) & 31);
        if ((r_u32(0x80097EF0) & mask) == 0)
            code = 89;
        packet = t3_8004b928(ordering_entry, packet, x, y, code, flags);
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_80070ef4(uint32 object)
{
    uint32 state, flags, index, packet, value, first, second, random, x, z;
    FUNCTION_MARKER(0x80070EF4, "SLUS_004.02");
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
    for (index = 0; index < 2; ++index)
    {
        packet = object + 80 + index * 40;
        w_u8(packet + 12, r_u8(0x800252E8));
        w_u8(packet + 13, r_u8(0x800252E9));
        w_u8(packet + 20, r_u8(0x800252E8) + 15);
        w_u8(packet + 21, r_u8(0x800252E9));
        w_u8(packet + 28, r_u8(0x800252E8));
        w_u8(packet + 29, r_u8(0x800252E9) + 15);
        w_u8(packet + 36, r_u8(0x800252E8) + 15);
        w_u8(packet + 37, r_u8(0x800252E9) + 15);
    }
    w_u32(object + 8, 0x800254A4);
    w_u32(object + 64, 0xFFFFFFFF);
    w_u32(object + 32, 0);
    first = (uint32)randPSX();
    second = (uint32)randPSX();
    random = first | (second << 16);
    x = r_u32(object + 40);
    z = r_u32(object + 48);
    x = x - 127 + (random & 255);
    z = z - 127 + (((sint32)random >> 4) & 255);
    w_u32(object + 40, x);
    w_u32(object + 48, z);
    return z;
}

uint32 t3_80052de4(uint32 state, uint32 selection, uint32 buttons)
{
    uint32 row, column, code, mask, flags, extra, chosen, slot, index, record, value, low, table, other, count, limit;
    FUNCTION_MARKER(0x80052DE4, "SLUS_004.02");
    row = r_u32(selection + 12);
    column = r_u32(selection + 8);
    code = r_u8(0x8002276C + (column + row * 7) * 6);
    flags = r_u32(selection + 36);
    mask = 1u << ((code >> 2) & 31);
    if ((flags & mask) == 0)
        code = r_u32(0x80097EF0) & mask ? 89 : 88;
    flags = r_u32(0x80097EF4);
    extra = flags & (1u << ((code >> 2) & 31)) ? 0x810 : 0;
    if (((extra | 0xF0) & buttons) == 0)
        return 1;
    chosen = code + ((extra & buttons) ? 2 : (buttons & 0x60) != 0);
    if ((sint32)code < 88)
    {
        slot = r_u32(selection + 44);
        w_u32(selection + 56 + slot * 4, chosen);
        if (r_u32(state) != 2)
        {
            for (index = 0; index < 2; ++index)
            {
                record = state + index * 172;
                value = r_u32(record + 80);
                w_u32(record + 128, value >> 2);
                w_u32(record + 132, value & 3);
                table = r_u32(record + 24) * 2 + 0x80022804;
                value = r_u8(table);
                w_u32(record + 136, value);
                value = r_u8(table + 1);
                w_u32(record + 140, value);
            }
            other = state + 24;
            if (other == selection)
                other = state + 196;
            t3_8004f04c(selection + 104, other + 104);
            for (index = 0; index < 2; ++index)
            {
                record = state + 80 + index * 172;
                value = r_u32(record + 48);
                low = r_u32(record + 52);
                w_u32(record, (value << 2) | (low & 3));
            }
            chosen = r_u32(selection + 56 + slot * 4);
        }
        mask = ~(1u << (((sint32)chosen >> 2) & 31));
        flags = r_u32(selection + 36);
        w_u32(selection + 100, chosen);
        count = r_u32(selection + 44);
        w_u32(selection + 44, count + 1);
        w_u32(selection + 36, flags & mask);
        t3_800753bc(0, 0x50F4, 0);
    }
    count = r_u32(selection + 44);
    limit = r_u32(selection + 40);
    return count < limit;
}

uint32 t3_80073ef8(uint32 object)
{
    uint32 state, flags, mode, value, clut, page, index, packet, first, second, random, x, y;
    FUNCTION_MARKER(0x80073EF8, "SLUS_004.02");
    state = r_u8(object + 14);
    flags = r_u8(object + 13);
    w_u8(object + 14, state + 1);
    w_u8(object + 13, flags | 0x40);
    SetPolyFT4((POLY_FT4 *)psx_addr(object + 80, 40));
    SetPolyFT4((POLY_FT4 *)psx_addr(object + 120, 40));
    w_u8(object + 84, 128);
    w_u8(object + 85, 128);
    w_u8(object + 86, 128);
    w_u8(object + 124, 128);
    w_u8(object + 125, 128);
    w_u8(object + 126, 128);
    SetSemiTrans((POLY_FT4 *)psx_addr(object + 80, 40), 1);
    SetSemiTrans((POLY_FT4 *)psx_addr(object + 120, 40), 1);
    mode = r_u32(object + 76);
    w_u32(object + 8, 0x80025700);
    w_u16(object + 24, 0);
    if (mode == 0)
    {
        value = r_u32(object + 64);
        w_u32(object + 72, 0xFFFFFFFF);
        clut = r_u32(object + 64);
        w_u16(object + 70, 0);
        value = (value << 7) + 368;
        clut = (clut & 63) | 0x7DC0;
        w_u16(object + 68, value);
        page = ((value & 0x3FF) >> 6) | 0x20;
        w_u16(object + 134, clut);
        w_u16(object + 94, clut);
        w_u16(object + 142, page);
        w_u16(object + 102, page);
    }
    else if (mode == 1)
    {
        w_u8(object + 13, r_u8(object + 13) | 0x40);
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
        flags = r_u8(object + 13);
        w_u32(object + 72, 0xFFFFFFFF);
        w_u8(object + 13, flags | 2);
    }
    first = (uint32)randPSX();
    second = (uint32)randPSX();
    random = first | (second << 16);
    x = r_u16(object + 16);
    y = r_u16(object + 18);
    w_u16(object + 24, 0);
    w_u16(object + 16, x - 63 + (((sint32)random >> 4) & 127));
    w_u16(object + 18, y - 31 + (((sint32)random >> 6) & 63));
    return t3_8006e120(object + 16);
}

uint32 t3_8006875c(uint32 actor, uint32 health)
{
    uint32 seed, index, value, x;
    sint32 heading, sine, cosine, kind, scale;
    FUNCTION_MARKER(0x8006875C, "SLUS_004.02");
    w_u16(0x800A8C78, 4096);
    w_u16(0x800A8C7A, 0);
    w_u16(0x800A8C7C, 0);
    if (!health)
    {
        w_u32(0x800A0508, 0);
        value = ((uint32)randPSX() & 127) - 64;
        w_u32(0x800A050C, value);
        return value;
    }
    seed = r_u32(0x800A912C);
    heading = r_s16(actor + 44);
    seed = (seed + 1) * 69069;
    w_u32(0x800A912C, seed);
    index = ((((seed >> 16) * 1025) >> 16) + 512 - (uint32)(heading / 16)) & 0xFFF;
    index *= 2;
    sine = r_s16(0x8001E8C4 + index);
    kind = r_s16(actor + 22);
    cosine = r_s16(0x8001F0C4 + index);
    scale = kind == 11 ? 6596 : 3978;
    value = r_u32(actor + 0xF68) + cosine * scale / 4096;
    w_u32(0x800A04E8, value);
    value = r_u32(actor + 0xF70) + sine * scale / 4096;
    w_u32(0x800A04F0, value);
    w_u32(0x800A04EC, (uint32)-1500);
    x = r_u32(actor + 0xF68);
    w_u32(0x800A04F8, (uint32)-1425);
    w_u32(0x800A04F4, x);
    value = r_u32(actor + 0xF70);
    w_u32(0x800A0500, 0);
    w_u32(0x800A0504, 0);
    w_u32(0x800A04FC, value);
    return 0x800A04E8;
}

uint32 t3_80053fac(uint32 destination, uint32 source)
{
    uint32 side, offset, record, output, mode, value, index;
    FUNCTION_MARKER(0x80053FAC, "SLUS_004.02");
    for (side = 0; side < 2; ++side)
    {
        offset = side * 2;
        record = source + 76 + side * 172;
        output = destination + side * 13;
        if (r_u32(destination) != 5 || r_u16(0x800AE1F8 + offset))
            w_u8(0x80097F0E + side, r_u8(record + 48));
        mode = r_u32(source);
        if (mode > 8)
            continue;
        if (mode == 5)
        {
            value = r_u32(record + 4);
            w_u16(0x800ADD5C + offset, (uint32)((sint32)value >> 2));
            w_u16(0x800ADD98 + offset, value & 3);
            w_u16(0x800A9128 + offset, 0);
            w_u16(0x800ADFBC + offset, 1);
        }
        else if (mode == 2)
        {
            w_u8(output + 98, 0);
            w_u8(output + 99, r_u8(record - 8));
            w_u8(output + 101, r_u8(record - 12));
            w_u8(output + 100, r_u8(record));
            for (index = 0; index < 8; ++index)
                w_u8(output + 89 + index, r_u32(source + 80 + side * 172 + index * 4));
            if (r_u16(0x800AE1F8 + offset))
            {
                w_u16(0x800ADFBC + offset, 1);
                w_u16(0x800A9128 + offset, 0);
            }
            else
                w_u16(0x800A9128 + offset, 1);
        }
        else
        {
            if (mode == 1)
                w_u16(destination + side * 4 + 58, r_u16(record + 36));
            if (r_u16(0x800AE1F8 + offset))
            {
                value = r_u32(record + 4);
                w_u16(0x800ADD5C + offset, (uint32)((sint32)value >> 2));
                w_u16(0x800ADD98 + offset, value & 3);
                w_u16(0x800ADFBC + offset, 1);
                w_u16(0x800A9128 + offset, 0);
            }
        }
    }
    return 0;
}

uint32 t3_80054194(uint32 state)
{
    uint32 ordering_entry, packet, record, side, index, x, y, code, mode, flags, row, column, mask;
    sint32 count;
    FUNCTION_MARKER(0x80054194, "SLUS_004.02");
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    ordering_entry += 4;
    for (side = 0; side < 2; ++side)
    {
        record = state + side * 172;
        count = r_s32(record + 64);
        x = r_u32(record + 40);
        y = r_u32(record + 44);
        for (index = 0; (sint32)index < count; ++index)
        {
            code = r_u32(record + 80 + index * 4);
            if ((sint32)code >= 88)
                code = 88;
            flags = 0;
            mode = r_u32(record + 56);
            if (mode == 1)
                flags = 132;
            else if (mode == 2)
            {
                if (index == r_u32(record + 68))
                    flags = 16;
            }
            else if (mode == 3)
            {
                if (index == r_u32(record + 68))
                {
                    row = r_u32(record + 36);
                    column = r_u32(record + 32);
                    code = r_u8(0x8002276C + (column + row * 7) * 6);
                    mask = 1u << ((code >> 2) & 31);
                    if ((r_u32(record + 60) & mask) == 0)
                        code = r_u32(0x80097EF0) & mask ? 89 : 88;
                    if ((sint32)code >= 88)
                        code = 88;
                }
            }
            packet = t3_8004b928(ordering_entry, packet, x + (index & 3) * 36, y + (index >> 2) * 64, code, flags);
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_800465bc(uint32 actor)
{
    uint32 dx, dy, dz, origin, length, step_x, step_y, step_z, value, y, z, position[3], camera[8], difference, change, heading;
    FUNCTION_MARKER(0x800465BC, "SLUS_004.02");
    value = r_u32(actor + 0xF68);
    origin = r_u32(0x8009E6B4);
    dx = value - origin;
    value = r_u32(actor + 0xF6C);
    origin = r_u32(0x8009E6B8);
    dy = value - origin;
    value = r_u32(actor + 0xF70);
    origin = r_u32(0x8009E6BC);
    dz = value - origin;
    length = t3_8004aedc(dx * dx + dy * dy + dz * dz);
    if ((sint32)(length - 2513) > 0)
    {
        step_x = (uint32)((sint64)(sint32)(dx << 3) / (sint32)length);
        step_y = (uint32)((sint64)(sint32)(dy << 3) / (sint32)length);
        step_z = (uint32)((sint64)(sint32)(dz << 3) / (sint32)length);
        value = r_u32(0x8009E6B4);
        w_u32(0x8009E6B4, value + step_x);
        y = r_u32(0x8009E6B8);
        z = r_u32(0x8009E6BC);
        w_u32(0x8009E6B8, y + step_y);
        w_u32(0x8009E6BC, z + step_z);
    }
    position[0] = r_u32(0x8009E6B4);
    position[1] = r_u32(0x8009E6B8);
    position[2] = r_u32(0x8009E6BC);
    t3_80046270(actor, position, camera);
    value = r_u32(0x8009E6A0);
    difference = camera[0] - value;
    change = difference & 0x20000 ? (difference | 0xFFFC0000) + 31 : difference & 0x3FFFF;
    value += (uint32)((sint32)change >> 5);
    heading = r_u32(0x8009E6A4);
    w_u32(0x8009E6A0, value);
    difference = camera[1] - heading;
    change = difference & 0x20000 ? (difference | 0xFFFC0000) + 31 : difference & 0x3FFFF;
    w_u32(0x8009E6A4, heading + (uint32)((sint32)change >> 5));
    return t3_8004610c(0x8009E6A0);
}

uint32 t3_80042b8c(void)
{
    uint32 first_x, second_x, first_z, second_z, sum, center_x, center_z, dx, dz, angle, index, end_x, end_z, first_health, second_health;
    sint32 first_height, second_height, sine, cosine;
    FUNCTION_MARKER(0x80042B8C, "SLUS_004.02");
    first_x = r_u32(0x800AA190);
    second_x = r_u32(0x800ABA1C);
    first_z = r_u32(0x800AA198);
    second_z = r_u32(0x800ABA24);
    second_height = r_s16(0x800ABD2C);
    sum = second_x + first_x;
    center_x = (uint32)((sint32)sum / 2);
    sum = second_z + first_z;
    center_z = (uint32)((sint32)sum / 2);
    first_height = r_s16(0x800AA4A0);
    if (first_height < second_height)
    {
        dx = second_x - first_x;
        dz = second_z - first_z;
    }
    else
    {
        dx = first_x - second_x;
        dz = first_z - second_z;
    }
    angle = t3_8004b39c(dx, dz);
    dx = r_u32(0x8009E634) - center_x;
    dz = r_u32(0x8009E63C) - center_z;
    angle = (uint32)(sint32)(sint16)angle;
    w_u32(0x8009E624, angle);
    t3_8002a324(dx * dx + dz * dz);
    index = ((6144 - angle) & 0xFFF) * 2;
    sine = r_s16(0x8001E8C4 + index);
    end_x = center_x + sine * 2250 / 4096;
    cosine = r_s16(0x8001F0C4 + index);
    end_z = center_z + cosine * 2250 / 4096;
    w_u32(0x8009E634, center_x + sine * 9000 / 4096);
    cosine = r_s16(0x8001F0C4 + index);
    w_u32(0x8009E63C, center_z + cosine * 9000 / 4096);
    t3_80042d70();
    t3_80042df8(center_x, (uint32)-1600, center_z, end_x, (uint32)-1600, end_z);
    first_health = r_u32(0x800A9620);
    second_health = r_u32(0x800AAEAC);
    w_u32(0x800A961C, first_health);
    w_u32(0x800AAEA8, second_health);
    return first_health;
}

uint32 t3_800579a0(uint32 state)
{
    sint32 count, row;
    uint32 entry, action, attributes, kind, mask, category, input, accepted, selected;
    FUNCTION_MARKER(0x800579A0, "SLUS_004.02");
    count = r_s16(state + 0x70);
    selected = 0;
    if (count <= 0)
        return 0;
    entry = 0x8009F978;
    for (row = 0; row < count; ++row, entry += 12)
    {
        if (r_s16(entry + 8) != 0)
            continue;
        action = r_u32(entry);
        if (r_u8(action + 45) == 0)
            continue;
        if (r_u8(action + 25) != 0)
            continue;
        attributes = r_u32(action + 8);
        kind = attributes & 0xFFFF;
        if (kind == 0x412 || (attributes & 0x400000))
            continue;
        if (r_u32(action + 4) & 0x20000)
            continue;
        mask = attributes & 0xFFFF0000;
        category = r_u32(state + 20);
        if ((mask & 0x1C0000) == 0)
            mask |= 0xC0000;
        if ((mask & r_u32(0x80098230 + category * 4)) == 0)
            continue;
        input = r_u32(state + 0x230);
        if ((input & 0xFFFF) == 0x4C02)
            accepted = 1;
        else if (input & 0x400)
            accepted = kind == 0x217 || kind == 0x31F;
        else if (input & 4)
            accepted = kind == 0x10F || kind == 0x51F;
        else if (input & 1)
            accepted = kind == 0x217 || kind == 0x10F;
        else if (input & 2)
            accepted = kind == 0x10F || kind == 0x217 || kind == 0x412;
        else
            accepted = 0;
        if (accepted)
        {
            w_u16(entry + 8, 1);
            selected += 1;
        }
    }
    w_u16(state + 0x72, r_u16(state + 0x72) + selected);
    return selected;
}

uint32 t3_800564b8(void)
{
    uint32 flags, value, threshold;
    FUNCTION_MARKER(0x800564B8, "SLUS_004.02");
    if ((sint32)t3_80051660(9) >= 50)
    {
        flags = r_u32(0x80097EF4);
        if (!(flags & 0x200))
            w_u16(0x80097F1C, 0);
        w_u32(0x80097EF4, (flags | 0x200) & 0x50382);
    }
    if ((sint32)t3_80051660(7) >= 50)
    {
        flags = r_u32(0x80097EF4);
        if (!(flags & 0x80))
            w_u16(0x80097F1C, 0);
        w_u32(0x80097EF4, (flags | 0x80) & 0x50382);
    }
    if ((sint32)t3_80051660(18) >= 25)
    {
        flags = r_u32(0x80097EF4);
        if (!(flags & 0x40000))
            w_u16(0x80097F1C, 0);
        w_u32(0x80097EF4, (flags | 0x40000) & 0x50382);
    }
    if ((sint32)t3_80051660(16) >= 10)
    {
        flags = r_u32(0x80097EF4);
        if (!(flags & 0x10000))
            w_u16(0x80097F1C, 0);
        w_u32(0x80097EF4, (flags | 0x10000) & 0x50382);
    }
    flags = r_u32(0x80097EF0);
    if (flags & 0x80000)
        w_u8(0x80097F1F, 2);
    else
        w_u8(0x80097F1F, t3_8004ccac(flags) >= 15);
    threshold = r_u8(0x80097F1B) ? 50 : 100;
    if (r_u16(0x80097F1C) >= threshold)
    {
        value = r_u8(0x80097F1A);
        w_u8(0x80097F1B, 1);
        value += 1;
        w_u16(0x80097F1C, 0);
        if (value >= 15)
            value = 14;
        w_u8(0x80097F1A, value);
        return t3_800561e0(value);
    }
    return 1;
}

