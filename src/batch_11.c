#include "game_functions.h"

uint32 t3_80057e94(uint32 state)
{
    sint32 count, index;
    uint32 entry, action, attributes, required, kind, selected = 0, total;
    FUNCTION_MARKER(0x80057E94, "SLUS_004.02");
    count = r_s16(state + 0x70);
    if (count <= 0)
        return 0;
    for (index = 0; index < count; ++index) {
        entry = 0x8009F978 + (uint32)index * 12;
        if (r_s16(entry + 8))
            continue;
        action = r_u32(entry);
        attributes = r_u32(action + 8);
        kind = r_u8(action + 0x2D);
        if ((attributes & 0x400000) || r_s16(action + 0x14) <= 0 || !kind)
            continue;
        if ((sint32)kind >= r_s16(state + 0x20A) || (attributes & 0xFFFF) == 0x412)
            continue;
        if (r_u32(action + 4) & 0x20000)
            continue;
        required = attributes & 0xFFFF0000;
        kind = r_u32(state + 0x14);
        if (!(required & 0x1C0000))
            required |= 0xC0000;
        if (required & r_u32(0x80098230 + kind * 4)) {
            w_u16(entry + 8, 1);
            ++selected;
        }
    }
    total = r_u16(state + 0x72);
    w_u16(state + 0x72, total + selected);
    return selected;
}

uint32 t3_800590ec(uint32 state, uint32 action)
{
    uint32 seed, next, command;
    sint32 threshold, mode;
    FUNCTION_MARKER(0x800590EC, "SLUS_004.02");
    seed = r_u32(0x800ADCA0);
    next = seed * 5 + 3;
    threshold = r_s16(state + 0x27E);
    w_u32(0x800ADCA0, next);
    if ((sint32)(seed & 0xFFF) < threshold)
        return 0;
    w_u32(0x800ADCA0, next * 5 + 3);
    if (!(next & 31))
        return 0;
    mode = r_s32(state + 0x14);
    if (mode < 2 && r_s16(state + 0x54) >= 0)
        return 1;
    if (mode >= 4)
        return 1;
    if ((r_u32(state + 0x230) & 4) && !r_u8(r_u32(0x800A867C) + 0xDB) && !(r_u32(action + 8) & 0x10000)) {
        command = r_u16(action + 4);
        if (command != 0x10F && command != 0x51F)
            return 1;
    }
    return r_s16(state + 0x2E) >= 0x3801;
}

uint32 t3_800613cc(uint32 state)
{
    uint32 entry = 0x8009F978, removed = 0, action, attributes, total;
    sint32 index = 0;
    FUNCTION_MARKER(0x800613CC, "SLUS_004.02");
    if (r_s16(state + 0x70) > 0) {
        do {
            if (r_s16(entry + 8) >= 0) {
                action = r_u32(entry);
                attributes = r_u32(action + 8);
                if ((attributes & 0x10000) || (attributes & 0xFFFF) == 0x10F) {
                    if (r_s16(entry + 8) > 0)
                        ++removed;
                    w_u16(entry + 8, 0xFFFF);
                }
            }
            ++index;
            entry += 12;
        } while (index < r_s16(state + 0x70));
    }
    total = r_u16(state + 0x72);
    w_u16(state + 0x72, total - removed);
    return removed;
}

uint32 t3_80062e44(uint32 state, uint32 actor, uint32 opponent)
{
    uint32 previous, result;
    FUNCTION_MARKER(0x80062E44, "SLUS_004.02");
    previous = r_u16(0x800A8C8E);
    w_u16(0x800A8C90, state);
    w_u16(0x800A8C9E, 0xFFFF);
    w_u16(0x800A8C98, previous);
    result = t3_8006782c(actor, opponent, state);
    if ((sint32)result < 0) {
        w_u32(0x800A8CA0, 0xFFFFFFFF);
    } else if (r_s16(0x800A8CA4) < 0) {
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

uint32 t3_8004cd88(void)
{
    FUNCTION_MARKER(0x8004CD88, "SLUS_004.02");
    return 1;
}

uint32 t3_8004e704(uint32 ordering, uint32 packet, uint32 x, uint32 y, uint32 width, uint32 palette)
{
    uint32 top, bottom, right, left, texture;
    FUNCTION_MARKER(0x8004E704, "SLUS_004.02");
    top = y << 16;
    bottom = (y + 10) << 16;
    texture = (r_u16(0x80021F2C + palette * 2) << 16) | 0xA8DC;
    right = (x + width) & 0xFFFF;
    left = x & 0xFFFF;
    return t3_8004d830(ordering, packet, 0x2D000000, left | top, right | top,
        left | bottom, right | bottom, texture, 0xDA8DE, 0xB2DEB2DC);
}

uint32 t3_8004ee14(void)
{
    sint32 first, second, maximum, minimum;
    uint32 flags, selection, table, offset, text;
    FUNCTION_MARKER(0x8004EE14, "SLUS_004.02");
    first = t3_8002a0e4(0);
    second = t3_8002a0e4(1);
    flags = r_u32(0x800AE218) & 0x30;
    if (!flags)
        return flags;
    maximum = first >= second ? first : second;
    minimum = first >= second ? second : first;
    if (maximum < 1)
        selection = 0;
    else if (minimum < 1)
        selection = second < 1 ? 1 : 2;
    else
        selection = 3;
    table = 0x800220BC + selection * 8;
    offset = r_u32(table + 4);
    text = r_u32(table);
    return t3_8004ce74(0x80022194, (uint32)(184 - offset), (uint32)364, (uint32)5, text);
}

uint32 t3_8004f2dc(uint32 actor)
{
    uint32 mask, count, random, selected, ordinal, index, pointer, result;
    FUNCTION_MARKER(0x8004F2DC, "SLUS_004.02");
    mask = r_u32(0x80097EF0) & 0x13FFF;
    count = t3_8004ccac(mask);
    random = t3_8004ce54() & 0xFFF;
    selected = count ? (uint32)((sint32)random % (sint32)count) : random;
    ordinal = 0;
    index = 0;
    for (result = 0; result < 22; ++result) {
        if (mask & ((uint32)1 << result)) {
            if (ordinal == selected) {
                index = result;
                break;
            }
            ++ordinal;
        }
    }
    index *= 4;
    if ((sint32)index >= 93)
        index = 88;
    pointer = r_u32(0x80097D40 + index * 4);
    result = r_u8(pointer + 9);
    w_u16(actor + 0x16, result);
    return result;
}

uint32 t3_8004b570(uint32 x, uint32 y)
{
    uint32 magnitude_x, magnitude_y;
    FUNCTION_MARKER(0x8004B570, "SLUS_004.02");
    magnitude_x = (sint32)x < 0 ? 0 - x : x;
    magnitude_y = (sint32)y < 0 ? 0 - y : y;
    while ((sint32)magnitude_x > 0x1FFFFF || (sint32)magnitude_y > 0x1FFFFF) {
        magnitude_x = (sint32)magnitude_x >> 1;
        magnitude_y = (sint32)magnitude_y >> 1;
    }
    if ((sint32)x < 0)
        magnitude_x = 0 - magnitude_x;
    if ((sint32)y < 0)
        magnitude_y = 0 - magnitude_y;
    return (uint32)(sint16)t3_8004b39c(magnitude_x, magnitude_y);
}

uint32 t3_8004c6fc(uint32 destination, uint32 source)
{
    T3_TIM_IMAGE image;
    uint32 first, second;
    FUNCTION_MARKER(0x8004C6FC, "SLUS_004.02");
    if ((sint32)t3_80031bfc(source, destination) > 33344)
        t3_8007ae58(1);
    if (t3_8007efc8(destination) == 0xFFFFFFFF)
        t3_8007ae58(1);
    t3_8007efd8(&image);
    first = r_u16(image.image_rectangle);
    second = r_u16(image.image_rectangle + 2);
    if (first | second)
        t3_8007c650(image.image_rectangle, image.image_pixels);
    if (image.mode & 8) {
        first = r_u16(image.clut_rectangle);
        second = r_u16(image.clut_rectangle + 2);
        if (first | second)
            t3_8007c650(image.clut_rectangle, image.clut_pixels);
    }
    return (uint32)DrawSync(0);
}

uint32 t3_8004c91c(uint32 destination, uint32 archive, uint32 index)
{
    T3_TIM_IMAGE image;
    uint32 first, second, source;
    FUNCTION_MARKER(0x8004C91C, "SLUS_004.02");
    t3_80029638(1);
    if ((sint32)index < r_s32(archive)) {
        source = archive + r_u32(archive + (index << 3) + 4);
        if ((sint32)t3_80031bfc(source, destination) > 33344)
            t3_8007ae58(1);
        if (t3_8007efc8(destination) == 0xFFFFFFFF)
            t3_8007ae58(1);
        t3_8007efd8(&image);
        first = r_u16(image.image_rectangle);
        second = r_u16(image.image_rectangle + 2);
        if (first | second)
            t3_8007c650(image.image_rectangle, image.image_pixels);
        if (image.mode & 8) {
            first = r_u16(image.clut_rectangle);
            second = r_u16(image.clut_rectangle + 2);
            if (first | second)
                t3_8007c650(image.clut_rectangle, image.clut_pixels);
        }
        DrawSync(0);
    }
    return t3_80029638(0);
}

uint32 t3_80045ee8(uint32 input, uint32 *output)
{
    uint32 dx, dy, dz, value, other, root, angle;
    FUNCTION_MARKER(0x80045EE8, "SLUS_004.02");
    value = r_u32(input + 12);
    other = r_u32(input);
    dx = value - other;
    value = r_u32(input + 20);
    other = r_u32(input + 8);
    dz = value - other;
    value = r_u32(input + 16);
    other = r_u32(input + 4);
    dy = value - other;
    root = t3_8002a324(dx * dx + dz * dz);
    angle = (uint32)(sint16)t3_8004b39c(root, dy);
    output[0] = angle;
    value = angle & 0xFFF;
    if (angle & 0x800)
        value -= 4096;
    output[0] = value;
    angle = (uint32)(sint16)t3_8004b39c(dx, dz) - 1024;
    output[1] = angle;
    output[2] = r_u32(input + 24);
    output[5] = r_u32(input);
    output[6] = r_u32(input + 4);
    value = r_u32(input + 8);
    output[7] = value;
    return value;
}

uint32 t3_80045fec(const uint32 *input, uint32 *output)
{
    uint32 dx, dy, dz, value, root, angle, cached_roll, position_z;
    FUNCTION_MARKER(0x80045FEC, "SLUS_004.02");
    dx = input[3] - input[0];
    dz = input[5] - input[2];
    dy = input[4] - input[1];
    root = t3_8002a324(dx * dx + dz * dz);
    angle = (uint32)(sint16)t3_8004b39c(root, dy);
    output[0] = angle;
    value = angle & 0xFFF;
    if (angle & 0x800)
        value -= 4096;
    output[0] = value;
    output[1] = (uint32)(sint16)t3_8004b39c(dx, dz) - 1024;
    output[2] = input[6];
    value = input[0];
    cached_roll = output[2];
    output[5] = value;
    output[6] = input[1];
    value = output[0];
    position_z = input[2];
    output[0] = value << 6;
    value = output[1];
    output[2] = cached_roll << 6;
    output[7] = position_z;
    value <<= 6;
    output[1] = value;
    return value;
}

uint32 t3_8004610c(uint32 camera)
{
    uint32 offset, first, second, third, fourth;
    sint32 x, y, z;
    FUNCTION_MARKER(0x8004610C, "SLUS_004.02");
    for (offset = 0; offset != 32; offset += 16) {
        first = r_u32(camera + offset);
        second = r_u32(camera + offset + 4);
        third = r_u32(camera + offset + 8);
        fourth = r_u32(camera + offset + 12);
        w_u32(0x800A85C0 + offset, first);
        w_u32(0x800A85C4 + offset, second);
        w_u32(0x800A85C8 + offset, third);
        w_u32(0x800A85CC + offset, fourth);
    }
    w_u32(0x800A85E0, r_u32(camera + 32));
    x = r_s32(0x800A85C0);
    y = r_s32(0x800A85C4);
    w_u32(0x800A85C0, x / 64);
    z = r_s32(0x800A85C8);
    w_u32(0x800A85C4, y / 64);
    w_u32(0x800A85C8, z / 64);
    return t3_80045ca8(0x800A85C0, 0x800ADF70);
}

uint32 t3_8003f098(uint32 actor)
{
    uint32 opponent, state, animation, value, other, x, z, vx, vz, table;
    uint32 product, magnitude, side, accumulator_x, accumulator_z, old_x, old_z;
    uint32 count, raw_count, angle, root, pointer, step, result, limit, heading;
    sint32 frame, start, distance, vertical, mode, identity, movement, duration;
    sint32 signed_count, difference, speed, roll, category, extra;
    FUNCTION_MARKER(0x8003F098, "SLUS_004.02");
    opponent = t3_80045c18(actor);
    w_u8(actor + 0xDC, 0);
    w_u8(actor + 0xDD, 0);
    w_u8(actor + 0xDF, 0);
    t3_80043a94(actor, opponent);
    state = r_u8(actor + 0x82);
    if (state == 1) {
        animation = r_u32(actor + 0x4C);
        frame = r_s16(actor + 0x58);
        start = r_u8(animation + 0x19);
        if (frame >= start) {
            duration = r_u8(animation + 0x1A) - frame;
            if (!duration)
                duration = 1;
            w_u8(actor + 0x82, 2);
            value = r_u32(0x800AFA3C);
            if (value >= 3073)
                value = 3072;
            value -= 500;
            magnitude = (uint32)(sint16)((sint32)value / duration);
            heading = r_s16(actor + 0x2A);
            product = magnitude * t3_8002a194(heading);
            heading = r_s16(actor + 0x2A);
            w_u32(actor + 0x78, (sint32)product / 32767);
            product = magnitude * t3_8002a1dc(heading);
            w_u8(actor + 0xB8, 2);
            w_u32(actor + 0x7C, (sint32)product / 32767);
        }
    } else if (state == 2) {
        x = r_u32(actor);
        z = r_u32(actor + 8);
        vz = r_u32(actor + 0x7C);
        vx = r_u32(actor + 0x78);
        w_u32(actor + 8, z + vz);
        animation = r_u32(actor + 0x4C);
        w_u32(actor, x + vx);
        frame = r_s16(actor + 0x58);
        if (frame >= r_u8(animation + 0x1A)) {
            x = r_u32(actor + 0xF68);
            z = r_u32(actor + 0xF70);
            w_u8(actor + 0x82, 0);
            w_u32(actor + 0x78, 0);
            w_u32(actor + 0x7C, 0);
            w_u32(actor, x);
            w_u32(actor + 8, z);
        }
    }
    if (r_s16(actor + 0x92) && (!r_s16(actor + 0x76) || r_s16(opponent + 0x16) != 14)) {
        heading = r_s16(actor + 0x2A);
        count = r_u16(actor + 0x9C);
        value = r_u32(actor + 4);
        vx = r_s16(actor + 0x9A);
        vz = r_s16(actor + 0x9E);
        count += 6;
        w_u16(actor + 0x9C, count);
        x = r_u32(actor);
        w_u32(actor + 4, value + (sint16)count);
        z = r_u32(actor + 8);
        w_u32(actor, x + vx);
        w_u32(actor + 8, z + vz);
        result = t3_8002a194(heading + 0x8000);
        magnitude = r_s16(actor + 0x98);
        product = magnitude * result;
        heading = r_s16(actor + 0x2A);
        w_u16(actor + 0x9A, (sint32)product / 32767);
        result = t3_8002a1dc(heading + 0x8000);
        magnitude = r_s16(actor + 0x98);
        product = magnitude * result;
        vertical = r_s16(actor + 0x9C);
        w_u16(actor + 0x9E, (sint32)product / 32767);
        if (vertical > 0) {
            distance = r_s16(actor + 0x96);
            value = r_u32(actor + 4);
            if ((sint32)value >= -distance) {
                if (r_s16(actor + 0x90) == 1)
                    t3_8004acfc(1);
                z = r_u32(actor + 0xF70);
                x = r_u32(actor + 0xF68);
                w_u32(actor + 8, z);
                mode = r_s16(actor + 0x90);
                w_u32(actor + 4, 0);
                w_u32(actor, x);
                w_u8(actor + (mode == 5 ? 0xDC : 0xDD), 1);
                w_u16(actor + 0x92, 0);
                w_u8(actor + 0xB8, 2);
            }
        }
    } else {
        animation = r_u32(actor + 0x54);
        start = r_u8(animation + 0x19);
        if (start) {
            frame = r_s16(actor + 0x58);
            if (frame >= start - 1 && r_u8(animation + 0x1A) >= frame) {
                distance = r_s16(animation + 0x16);
                side = r_u8(actor + 0x1E);
                if (distance) {
                    heading = r_s16(actor + 0xE);
                    old_x = r_u32(actor);
                    old_z = r_u32(actor + 8);
                    product = (uint32)distance * t3_8002a194(heading);
                    accumulator_x = 0x8009E600 + side * 4;
                    value = r_u32(accumulator_x);
                    w_u32(accumulator_x, value + (sint32)product / 32767);
                    heading = r_s16(actor + 0xE);
                    product = (uint32)distance * t3_8002a1dc(heading);
                    accumulator_z = 0x8009E610 + side * 4;
                    value = r_u32(accumulator_z);
                    w_u32(accumulator_z, value + (sint32)product / 32767);
                    value = r_u32(accumulator_x);
                    x = r_u32(actor);
                    w_u32(actor, x - (uint32)((sint32)value >> 8));
                    value = r_u32(accumulator_z);
                    z = r_u32(actor + 8);
                    x = r_u32(actor);
                    w_u32(actor + 8, z - (uint32)((sint32)value >> 8));
                    value = r_u32(accumulator_x);
                    w_u32(accumulator_x, value + ((x - old_x) << 8));
                    z = r_u32(actor + 8);
                    value = r_u32(accumulator_z);
                    w_u32(accumulator_z, value + ((z - old_z) << 8));
                }
            }
        }
    }
    if (r_s16(actor + 0x18) == 17 && r_u8(actor + 0x8A) && !r_s16(actor + 0x94)) {
        if (r_u8(actor + 0xDB)) {
            value = r_u32(actor + 0xF6C) + 128;
            if ((sint32)value < 0)
                w_u32(actor + 4, (sint32)value >> 1);
        } else {
            w_u32(actor + 4, 0);
        }
    }
    mode = r_s32(0x800AFA88);
    identity = r_s16(actor + 0x12);
    if (mode == 7) {
        table = 0x800B6678 + (uint32)identity * 6;
        if (r_u8(table + 2)) {
            frame = r_s16(actor + 0x58);
            start = r_u8(table + 3);
            if (frame >= start && r_u8(table + 4) >= frame) {
                heading = r_s16(actor + 0x2A);
                angle = r_s16(table);
                result = t3_8002a194(heading + angle);
                magnitude = r_u8(table + 2);
                product = magnitude * result;
                x = r_u32(actor);
                w_u32(actor, x + (sint32)product / 32767);
                heading = r_s16(actor + 0x2A);
                angle = r_s16(table);
                result = t3_8002a1dc(heading + angle);
                magnitude = r_u8(table + 2);
                product = magnitude * result;
                z = r_u32(actor + 8);
                w_u32(actor + 8, z + (sint32)product / 32767);
            }
        }
    }
    if (!r_u8(actor + 0xDB))
        w_u16(actor + 0x8E, 0);
    signed_count = r_s16(actor + 0x114);
    count = r_u16(actor + 0x114);
    if (signed_count) {
        angle = r_u16(actor + 0x2C);
        value = r_u16(actor + 0x116);
        w_u16(actor + 0x114, count - 1);
        angle += value;
        mode = r_s16(actor + 0x74);
        w_u16(actor + 0x2C, angle);
        if (mode == 1)
            w_u16(actor + 0xE, angle);
    }
    if (r_u8(actor + 0xBF) == 1) {
        z = r_u32(actor + 0xF70);
        value = r_u32(actor + 0xA8);
        x = r_u32(actor + 0xF68);
        w_u8(actor + 0xBF, 0);
        z -= value;
        x -= r_u32(actor + 0xA4);
        angle = t3_8002a224(z, x);
        product = x * x + z * z;
        w_u16(actor + 0x10C, angle);
        w_u16(actor + 0x106, 16);
        root = t3_8002a324(product);
        duration = r_s16(actor + 0x106);
        value = duration ? (uint32)((sint64)(sint32)root / duration) :
            ((sint32)root < 0 ? 1 : 0xFFFFFFFF);
        heading = r_s16(actor + 0x10C);
        w_u16(actor + 0x108, value);
        product = root * t3_8002a194(heading + 0x8000);
        heading = r_s16(actor + 0x10C);
        x = r_u32(actor + 0xF68);
        w_u32(actor, x - (sint32)product / 32767);
        product = root * t3_8002a1dc(heading + 0x8000);
        z = r_u32(actor + 0xF70);
        w_u32(actor + 8, z - (sint32)product / 32767);
    }
    signed_count = r_s16(actor + 0x106);
    count = r_u16(actor + 0x106);
    movement = 0;
    if (signed_count > 0) {
        movement = r_s16(actor + 0x108);
        w_u16(actor + 0x106, count - 1);
    }
    signed_count = r_s16(actor + 0x10A);
    count = r_u16(actor + 0x10A);
    if (signed_count > 0) {
        pointer = r_u32(actor + 0x110);
        extra = r_s16(actor + 0x124);
        w_u16(actor + 0x10A, count - 1);
        distance = r_s16(pointer);
        w_u32(actor + 0x110, pointer + 2);
        movement = (sint32)((uint32)movement + (uint32)distance);
        if (extra > 0)
            movement = (sint32)((uint32)movement + extra * 40 + 10);
    }
    if (movement) {
        heading = r_s16(actor + 0x10C);
        product = (uint32)movement * t3_8002a194(heading);
        heading = r_s16(actor + 0x10C);
        x = r_u32(actor);
        w_u32(actor, x + (sint32)product / 32767);
        product = (uint32)movement * t3_8002a1dc(heading);
        z = r_u32(actor + 8);
        w_u32(actor + 8, z + (sint32)product / 32767);
    }
    state = r_u8(actor + 0xB9);
    switch (state) {
    case 0:
        w_u16(actor + 0x2C, r_u16(actor + 0xE));
        break;
    case 1:
        animation = r_u32(actor + 0x54);
        count = r_u8(animation + 0x2D);
        raw_count = r_u16(actor + 0x58);
        duration = (sint16)(count + 1 - raw_count);
        if (duration <= 0)
            duration = 1;
        angle = r_u16(actor + 0x34);
        value = r_u16(actor + 0x2C);
        difference = (sint16)(angle - value);
        step = t3_800408c0((sint16)(difference / duration), 5461);
        result = t3_80040904((sint16)step, r_s16(actor + 0x36), 32767);
        if ((sint16)result) {
            w_u8(actor + 0xB9, 6);
            step = result;
        }
        angle = r_u16(actor + 0x2C);
        value = r_u16(actor + 0x36);
        angle += step;
        w_u16(actor + 0x2C, angle);
        w_u16(actor + 0xE, angle);
        goto finish_turn;
    case 2:
    case 3:
        if (r_s16(opponent + 0x11C) && r_u8(actor + 0xB4) && r_s16(actor + 0x5C) < 9)
            goto locked_turn;
        animation = r_u32(actor + 0x54);
        count = r_u8(animation + 0x2D);
        raw_count = r_u16(actor + 0x58);
        duration = (sint16)(count + 1 - raw_count);
        if (duration <= 0)
            duration = 1;
        speed = r_u8(actor + 0xDB) ? 1 : (r_s16(actor + 0x5C) >= 8 ? 14 : 3);
        limit = speed * 65535 / 360;
        if (r_u8(actor + 0xB9) == 3)
            limit = (sint16)limit >> 1;
        angle = r_u16(actor + 0x2A);
        value = r_u16(actor + 0x2C);
        difference = (sint16)(angle - value);
        step = t3_800408c0((sint16)(difference / duration), (sint16)limit);
        result = t3_80040904((sint16)step, r_s16(actor + 0x36), 21845);
        goto update_turn;
    case 4:
        if (r_s16(actor + 0x114) || (r_s16(opponent + 0x11C) && r_u8(actor + 0xB4)))
            break;
        animation = r_u32(actor + 0x54);
        count = r_u8(animation + 0x2D);
        raw_count = r_u16(actor + 0x58);
        duration = (sint16)(count + 1 - raw_count);
        if (duration <= 0)
            duration = 1;
        angle = r_u16(actor + 0x2A);
        value = r_u16(actor + 0x2C);
        difference = (sint16)(angle - value);
        value = difference / duration;
        speed = r_u8(actor + 0xDB) ? 1 : (r_s16(actor + 0x5C) >= 8 ? 3 : 2);
        limit = speed * 65535 / 360;
        step = t3_800408c0((sint16)value, (sint16)limit);
        result = t3_80040904((sint16)step, r_s16(actor + 0x36), 3640);
        goto update_turn;
    case 5:
        if (r_s16(opponent + 0x11C) && r_u8(actor + 0xB4) && r_s16(actor + 0x5C) < 9) {
locked_turn:
            animation = r_u32(actor + 0x54);
            count = r_u8(animation + 0x2D);
            raw_count = r_u16(actor + 0x58);
            value = count - raw_count;
            w_u16(actor + 0x114, value);
            if ((sint16)value <= 0)
                w_u16(actor + 0x114, 1);
            angle = r_u16(actor + 0x2A);
            value = r_u16(actor + 0x2C);
            duration = r_s16(actor + 0x114);
            difference = (sint16)(angle - value);
            value = duration ? (uint32)(difference / duration) : (difference < 0 ? 1 : 0xFFFFFFFF);
            w_u16(actor + 0x116, value);
            result = t3_800408c0((sint16)value, 2548);
            w_u16(actor + 0x116, result);
            w_u8(actor + 0xB9, 12);
            break;
        }
        animation = r_u32(actor + 0x54);
        count = r_u8(animation + 0x2D);
        raw_count = r_u16(actor + 0x58);
        duration = (sint16)(count + 1 - raw_count);
        if (duration <= 0)
            duration = 1;
        speed = r_u8(actor + 0xDB) ? 4 : (r_s16(actor + 0x5C) >= 8 ? 100 : 3);
        angle = r_u16(actor + 0x2A);
        value = r_u16(actor + 0x2C);
        difference = (sint16)(angle - value);
        limit = speed * 65535 / 360;
        step = t3_800408c0((sint16)(difference / duration), (sint16)limit);
        result = t3_80040904((sint16)step, r_s16(actor + 0x36), 38228);
update_turn:
        if ((sint16)result) {
            w_u8(actor + 0xB9, 6);
            step = result;
        }
        angle = r_u16(actor + 0x2C);
        value = r_u16(actor + 0x36);
        w_u16(actor + 0x2C, angle + step);
finish_turn:
        animation = r_u32(actor + 0x54);
        w_u16(actor + 0x36, value + step);
        frame = r_s16(actor + 0x58);
        if (frame >= r_u8(animation + 0x2D))
            w_u8(actor + 0xB9, 6);
        break;
    case 6:
        if (r_s16(actor + 0x5E)) {
            animation = r_u32(actor + 0x54);
            if (r_u8(animation + 0x19) && !r_u8(actor + 0xDB)) {
                frame = r_s16(actor + 0x58);
                if (frame >= r_u8(animation + 0x2E)) {
                    w_u16(actor + 0x114, 0);
                    w_u16(actor + 0x116, 0);
                    break;
                }
            }
        }
        animation = r_u32(actor + 0x54);
        frame = r_s16(actor + 0x58);
        start = r_u8(animation + 0x2D);
        raw_count = r_u16(actor + 0x58);
        if (start < frame && (frame - start) % 5 == 0 && r_s16(actor + 0x30) < 21846) {
            duration = r_u8(animation + 0x18) - frame;
            value = duration < 6 ? 5 : r_u8(animation + 0x18) - raw_count;
            w_u16(actor + 0x114, value);
            duration = r_s16(actor + 0x114);
            if (duration < 0) {
                w_u16(actor + 0x114, 0);
            } else {
                angle = r_u16(actor + 0x34);
                value = r_u16(actor + 0x2C);
                difference = (sint16)(angle - value);
                value = duration ? (uint32)(difference / duration) : (difference < 0 ? 1 : 0xFFFFFFFF);
                w_u16(actor + 0x116, value);
                result = t3_800408c0((sint16)value, 546);
                w_u16(actor + 0x116, result);
            }
        }
        break;
    case 7:
        animation = r_u32(actor + 0x54);
        count = r_u8(animation + 0x1A);
        if (!count)
            count = r_u8(animation + 0x18);
        frame = r_s16(actor + 0x58);
        if ((sint32)count >= frame) {
            angle = r_u16(actor + 0x34);
            value = r_u16(actor + 0x2C);
            if (!count)
                count = 1;
            product = (uint32)(sint16)(angle - value) * (uint32)frame;
            step = t3_800408c0((sint16)((sint32)product / (sint32)count), 910);
            angle = r_u16(actor + 0x2C);
            w_u16(actor + 0x2C, angle + step);
        }
        break;
    case 8:
        signed_count = r_s16(actor + 0x5C);
        if (signed_count > 8)
            signed_count = 8;
        angle = r_u16(actor + 0x34);
        value = r_u16(actor + 0x2C);
        product = (uint32)(sint16)(angle - value) * (uint32)signed_count;
        step = t3_800408c0((sint16)((sint32)product / 8), 1820);
        angle = r_u16(actor + 0x2C);
        w_u16(actor + 0x2C, angle + step);
        break;
    case 9:
        if (r_u32(0x800AFA3C) < 1200)
            break;
        x = r_u32(actor + 0xAC);
        z = r_u32(actor + 0xB0);
        vz = r_u32(actor + 0xF70);
        vx = r_u32(actor + 0xF68);
        angle = t3_8002a224(z - vz, x - vx);
        heading = r_u16(actor + 0xE);
        value = heading - angle;
        if (value & 0x8000)
            value = ~value;
        if ((value & 0xFFFF) >= 0x4000)
            angle -= 0x8000;
        heading = r_u16(actor + 0xE);
        frame = r_s16(actor + 0x58);
        product = (uint32)(sint16)(angle - heading) * (uint32)frame;
        animation = r_u32(actor + 0x54);
        count = r_u8(animation + 0x18);
        value = count ? (uint32)((sint32)product / (sint32)count) : ((sint32)product < 0 ? 1 : 0xFFFFFFFF);
        step = t3_800408c0((sint16)value, 1456);
        angle = r_u16(actor + 0xE) + step;
        w_u16(actor + 0xE, angle);
        w_u8(actor + 0xBE, 1);
        w_u16(actor + 0x2C, angle);
        break;
    case 10:
        animation = r_u32(actor + 0x54);
        frame = r_s16(actor + 0x58);
        if (r_u8(animation + 0x18) < frame)
            break;
        roll = r_s16(actor + 0x24);
        category = r_s16(actor + 0x18);
        if (category == 7 && r_s16(actor + 0x32) == 2)
            difference = roll >= 0 ? -1820 : 1820;
        else
            difference = roll >= 0 ? 4551 : -4551;
        product = (uint32)difference * (uint32)frame;
        animation = r_u32(actor + 0x54);
        count = r_u8(animation + 0x18);
        value = count ? (uint32)((sint32)product / (sint32)count) : ((sint32)product < 0 ? 1 : 0xFFFFFFFF);
        angle = r_u16(actor + 0x34);
        w_u8(actor + 0xBE, 1);
        angle += value;
        w_u16(actor + 0xE, angle);
        w_u16(actor + 0x2C, angle);
        break;
    }
    signed_count = r_s16(actor + 0x11C);
    count = r_u16(actor + 0x11C);
    if (signed_count > 0)
        w_u16(actor + 0x11C, count - 1);
    signed_count = r_s16(actor + 0x104);
    count = r_u16(actor + 0x104);
    if (signed_count > 0)
        w_u16(actor + 0x104, count - 1);
    animation = r_u32(actor + 0x54);
    frame = r_s16(actor + 0x58);
    start = r_u8(animation + 0x2D);
    if (frame < start || r_u8(animation + 0x2E) < frame)
        w_u8(actor + 0xE3, 0);
    else
        w_u8(actor + 0xE3, r_u8(r_u32(animation + 0x28) + 2) ? 2 : 1);
    w_u8(actor + 0xE6, 0);
    if (r_u32(0x800AFA88) == 8) {
        if (r_s16(actor + 0x74) < 0) {
            if (!r_u8(actor + 0xC5))
                goto mode_eight_default;
            frame = r_s16(actor + 0x58);
            if (frame >= 11 && frame < r_u8(r_u32(actor + 0x54) + 0x18)) {
                x = r_u32(actor + 0x3C4);
                product = x * x;
                x = r_u32(actor + 0x3C8);
                product += x * x;
                x = r_u32(actor + 0x3CC);
                product += x * x;
                result = t3_8002a324(product);
                if ((sint32)result >= 90) {
                    w_u8(actor + 0xE6, 1);
                    w_u8(actor + 0x68, 6);
                    w_u16(actor + 0x64, 0x607);
                    w_u16(actor + 0x5E, ((sint32)result >> 4) + 10);
                }
            }
        }
        if (!r_u8(actor + 0xC5)) {
mode_eight_default:
            animation = r_u32(actor + 0x54);
            if ((r_u32(animation + 0x24) >> 8) & 0x100) {
                state = r_u8(actor + 0xDA);
                w_u8(actor + 0xE6, 1);
                w_u8(actor + 0x68, 6);
                w_u16(actor + 0x64, 0x607);
                w_u16(actor + 0x5E, 10);
                if (state) {
                    value = r_u32(actor + 0x3F4);
                    if ((sint32)value > 655360)
                        w_u32(actor + 0x3F4, value - 655360);
                }
            }
        }
    }
    if (r_s16(actor + 0x94)) {
        w_u8(actor + 0xDB, 1);
    } else {
        animation = r_u32(actor + 0x54);
        frame = r_s16(actor + 0x58);
        start = r_u8(animation + 0x19);
        w_u8(actor + 0xDB, frame >= start && r_u8(animation + 0x1A) >= frame);
    }
    animation = r_u32(actor + 0x54);
    frame = r_s16(actor + 0x58);
    if (frame == r_u8(animation + 0x1A) && r_s16(actor + 0x6A) == -1 && frame != 1 && r_u8(actor + 0xB8) != 2) {
        if (r_u8(actor + 0x87) && r_s16(actor + 0x90) != 5 && (r_u8(actor + 0x69) == 9 || (r_u32(actor + 0x60) & 4)))
            w_u8(actor + 0xDD, 1);
        else
            w_u8(actor + 0xDC, 1);
    }
    signed_count = r_s16(actor + 0x11A);
    count = r_u16(actor + 0x11A);
    if (signed_count) {
        value = count - 1;
        w_u16(actor + 0x11A, value);
        if ((sint16)value < 0)
            w_u16(actor + 0x11A, 0);
        if (r_u8(actor + 0xD0))
            w_u16(actor + 0x11A, 0);
    }
    if (r_s16(actor + 0x11A) < 31) {
        signed_count = r_s16(actor + 0x118);
        count = r_u16(actor + 0x118);
        if (signed_count) {
            pointer = r_u32(actor + 0x40C);
            w_u16(actor + 0x118, count - 1);
            value = r_u8(actor + pointer + 0x41C);
            extra = value & 0x20 ? 3 : (value & 0xF0 ? 1 : 0);
            value = r_u16(actor + 0x118) - extra;
            w_u16(actor + 0x118, value);
            if ((sint16)value < 0)
                w_u16(actor + 0x118, 0);
        }
    }
    if (r_s16(actor + 0x5E)) {
        animation = r_u32(actor + 0x54);
        start = r_u8(animation + 0x2D);
        duration = start - 3;
        if (duration < 0)
            duration = 1;
        frame = r_s16(actor + 0x58);
        if (frame >= duration && frame <= start)
            w_u8(actor + 0xDF, 1);
    }
    animation = r_u32(actor + 0x54);
    if ((r_u32(animation + 0x24) >> 8) & 4)
        w_u8(actor + 0xDF, 1);
    if (r_s16(actor + 0x18) == 17 && r_s16(opponent + 0x18) != 17)
        w_u8(actor + 0xDF, 1);
    animation = r_u32(actor + 0x54);
    if ((r_u32(animation + 0x24) >> 8) & 0x40) {
        pointer = r_u32(actor + 0x40C);
        value = r_u8(actor + pointer + 0x41C);
        state = r_u8(actor + 0xCA);
        value &= 15;
        if (state ? value == 4 || value == 5 : value == 4) {
            w_u32(actor + 0x60, 0x1052);
            w_u16(actor + 0x66, r_u16(actor + 0x60) & 0x18);
        } else if (state ? value == 1 || value == 2 : value == 1) {
            w_u32(actor + 0x60, 0x2829);
            w_u16(actor + 0x66, r_u16(actor + 0x60) & 0x18);
        }
    } else if (r_u8(actor + 0xB6)) {
        pointer = r_u32(actor + 0x40C);
        value = r_u8(actor + pointer + 0x41C) & 15;
        if (value == 4 || value == 1) {
            w_u32(actor + 0x60, value == 4 ? 0x1052 : 0x2829);
            w_u16(actor + 0x66, r_u16(actor + 0x60) & 0x18);
        } else {
            value = r_s16(animation + 4);
            w_u32(actor + 0x60, value);
            other = r_u16(actor + 0x60);
            value = r_u32(actor + 0x60);
            w_u16(actor + 0x66, other & 0x18);
            if ((value & 0x100) && !r_u8(actor + 0xCA)) {
                other = r_u16(actor + 0x66);
                w_u32(actor + 0x60, value & 0xFFFFFFE7);
                w_u16(actor + 0x66, other & 0xFFE7);
            }
        }
    }
    if (r_u8(actor + 0xDB) || r_s16(actor + 0x30) >= 0x4000)
        w_u16(actor + 0x66, 0);
    else
        w_u16(actor + 0x66, r_u16(actor + 0x60) & 0x18);
    t3_8004096c(actor);
    if (r_u8(actor + 0xD0)) {
        t3_80034744(actor, 10);
    } else if (!r_u8(actor + 0xBC)) {
        if (r_u8(actor + 0xDA)) {
            if (r_s16(actor + 0x5E))
                goto animation_threshold;
            if (!r_u8(actor + 0x8A) && t3_8003cb6c())
                t3_800346fc(actor, 10);
        }
        if (r_s16(actor + 0x5E)) {
animation_threshold:
            animation = r_u32(actor + 0x54);
            frame = r_s16(actor + 0x58);
            if (r_u8(animation + 0x2D) - frame < 8)
                t3_800346fc(actor, 10);
        }
    }
    w_u32(actor + 0x12D0, r_s16(actor + 0x76) && r_s16(opponent + 0x16) == 14);
    w_u8(actor + 0x128A, !r_u8(actor + 0x8A) && r_u8(actor + 0xDB));
    if (r_u8(actor + 0xD0) || r_u8(opponent + 0xD0)) {
        w_u16(actor + 0x122, 0);
    } else if (r_s16(actor + 0x122)) {
        other = r_u16(actor + 0x66) & 0xFFE7;
        value = r_u16(actor + 0x122) - 1;
        w_u16(actor + 0x122, value);
        w_u16(actor + 0x66, other);
        if ((sint16)value < 0)
            w_u16(actor + 0x122, 0);
    }
    if (r_u8(actor + 0xDB))
        w_u16(actor + 0x126, 0);
    signed_count = r_s16(actor + 0x126);
    count = r_u16(actor + 0x126);
    w_u16(actor + 0x126, signed_count > 0 ? count - 1 : 0);
    if (!r_s16(actor + 0x126))
        w_u16(actor + 0x124, 0);
    if (r_u32(0x800AFA88) == 8)
        w_u8(actor + 0xC9, r_u8(actor + 0xC5) || r_u8(actor + 0xC7));
    result = r_u8(actor + 0xC3);
    if (result) {
        result = r_u8(actor + 0xDD);
        if (result) {
            t3_8004ac84(actor + 0xF68);
            result = t3_8004acfc(0);
        }
    }
    return result;
}

uint32 t3_80051204(void)
{
    uint32 mode, active, first, second, limit;
    FUNCTION_MARKER(0x80051204, "SLUS_004.02");
    mode = r_u32(0x800AFA88);
    if (mode != 0 && mode != 3)
        return 0;
    if (r_u32(0x800AFAAC) != 9)
        return 0;
    active = r_u8(0x800AFAA4);
    if (active != 1 || r_u32(0x800AFB44) || r_u16(0x800AE212))
        return 0;
    first = r_u8(0x800AFAA6);
    second = r_u8(0x800AFAA7);
    limit = r_u16(0x800ADDFC);
    if (r_s16(0x800A9228 + second * 6284 + 68) >= (sint32)limit)
        return 0;
    if (r_s16(0x800A9228 + first * 6284 + 68) <= 0)
        return 0;
    w_u32(0x800AFB44, active);
    return 1;
}

uint32 t3_80074e94(void)
{
    SpuReverbAttr reverb;
    FUNCTION_MARKER(0x80074E94, "SLUS_004.02");
    SsVabClose(2);
    SsVabClose(3);
    t3_80075970(0x800E051C, 0x800E113C, 2);
    w_u8(0x800A8C38, 2);
    w_u8(0x800A9227, 255);
    w_u8(0x800A9226, 255);
    reverb.mask = 7;
    reverb.mode = 1;
    reverb.depth.left = 0x1FFF;
    reverb.depth.right = 0x1FFF;
    SpuSetReverbModeParam(&reverb);
    SpuReserveReverbWorkArea(1);
    SpuClearReverbWorkArea(1);
    SpuSetReverb(1);
    SpuSetReverbDepth(&reverb);
    return SpuSetReverbVoice(1, 2);
}

uint32 t3_80074750(uint32 object)
{
    uint32 timer, accelerated, result, sequence, channel, sound, state;
    FUNCTION_MARKER(0x80074750, "SLUS_004.02");
    timer = r_u32(object + 72);
    accelerated = r_u32(0x800954A4);
    w_u32(object + 72, timer - 1);
    if (accelerated)
        w_u32(object + 72, timer - 2);
    result = r_u32(object + 72);
    if ((sint32)result >= 0)
        return result;
    sequence = r_u32(0x800A0998);
    channel = r_u32(object + 64);
    sequence = (sequence + 1) & 7;
    w_u32(0x800A0998, sequence);
    sound = r_u32(object + 68);
    state = r_u8(object + 14);
    sound = (sound & 0xFFFFC3FF) | (sequence << 10);
    t3_800753bc(channel, sound, state | 0x80);
    result = r_u8(object + 14);
    w_u32(object + 72, 15);
    result += 1;
    w_u8(object + 14, result);
    return result;
}

uint32 t3_800747ec(uint32 object)
{
    uint32 actor, animation, metadata, state, end;
    sint32 frame;
    FUNCTION_MARKER(0x800747EC, "SLUS_004.02");
    actor = (r_u8(object + 13) & 0x10) ? 0x800A9228 : 0x800AAAB4;
    animation = r_u32(actor + 0x54);
    metadata = r_u32(animation + 0x28);
    if (r_u8(metadata) != 27)
        return t3_8006f5ac(object);
    frame = r_s16(actor + 0x58);
    end = r_u8(animation + 0x2D);
    if (frame < (sint32)end)
        return t3_8006f5ac(object);
    state = r_u8(object + 14);
    if (!state)
        t3_800748b8(object);
    else if (state != 1)
        return 1;
    return t3_800748d4(object);
}

uint32 t3_800743a0(uint32 object)
{
    uint32 buffer, frame, table, packet, value;
    FUNCTION_MARKER(0x800743A0, "SLUS_004.02");
    buffer = r_u32(0x800ADEFC);
    frame = r_u32(object + 72) + 1;
    packet = object + buffer * 40 + 80;
    w_u32(object + 72, frame);
    table = 0x80025514 + (frame & 7) * 2;
    value = r_u8(table);
    w_u8(packet + 12, value);
    value = r_u8(table + 1);
    w_u8(packet + 13, value);
    value = r_u8(table);
    w_u8(packet + 20, value + 31);
    value = r_u8(table + 1);
    w_u8(packet + 21, value);
    value = r_u8(table);
    w_u8(packet + 28, value);
    value = r_u8(table + 1);
    w_u8(packet + 29, value + 31);
    value = r_u8(table);
    w_u8(packet + 36, value + 31);
    value = r_u8(table + 1) + 31;
    w_u8(packet + 37, value);
    return value;
}

uint32 t3_80075864(void)
{
    sint32 state;
    uint32 result;
    FUNCTION_MARKER(0x80075864, "SLUS_004.02");
    state = r_s8(0x800A3400);
    result = r_u8(0x800A3400);
    if (state == 1)
        goto increment_state;
    if (state < 2)
        return 2;
    if (state == 2) {
        result = (uint32)SpuGetKeyStatus(2);
        if (result == 1)
            return result;
        result = r_u8(0x800A3400);
increment_state:
        result += 1;
        w_u8(0x800A3400, result);
        return result;
    }
    if (state != 3)
        return 3;
    result = r_u8(0x800A3401) - 1;
    w_u8(0x800A3401, result);
    result <<= 24;
    if ((sint32)result <= 0) {
        result = t3_800753bc(0, 0x86DA, 0);
        w_u8(0x800A3400, 0);
    }
    return result;
}

uint32 t3_80070e08(uint32 object)
{
    uint32 frame, buffer, value, velocity, position;
    FUNCTION_MARKER(0x80070E08, "SLUS_004.02");
    frame = r_u32(object + 68) + 1;
    w_u32(object + 68, frame);
    if ((sint32)frame >= 16)
        return t3_8006f5ac(object);
    buffer = r_u32(0x800ADEFC);
    value = r_u16(0x800A8D00 + frame * 4);
    w_u16(object + buffer * 40 + 94, value);
    velocity = r_u32(object + 32);
    position = r_u32(object + 44);
    velocity += 0x12000;
    w_u32(object + 32, velocity);
    value = position - (uint32)((sint32)velocity >> 16);
    w_u32(object + 44, value);
    return value;
}

uint32 t3_80071110(uint32 object)
{
    uint32 frame, buffer, value, velocity, position;
    FUNCTION_MARKER(0x80071110, "SLUS_004.02");
    frame = r_u32(object + 64) + 1;
    w_u32(object + 64, frame);
    if ((sint32)frame >= 32)
        return t3_8006f5ac(object);
    frame = (uint32)((sint32)frame >> 1);
    buffer = r_u32(0x800ADEFC);
    value = r_u16(0x800A8D00 + frame * 4);
    w_u16(object + buffer * 40 + 94, value);
    velocity = r_u32(object + 32);
    position = r_u32(object + 44);
    velocity += 0x12000;
    w_u32(object + 32, velocity);
    value = position - (uint32)((sint32)velocity >> 16);
    w_u32(object + 44, value);
    return value;
}

uint32 t3_80073bd0(uint32 object)
{
    uint32 frame, duration, numerator, index, buffer, value;
    uint32 dx, dy, dz, x, y, z;
    FUNCTION_MARKER(0x80073BD0, "SLUS_004.02");
    frame = r_u32(object + 64);
    duration = r_u32(object + 68);
    frame += 1;
    w_u32(object + 64, frame);
    if ((sint32)frame >= (sint32)duration)
        return t3_8006f5ac(object);
    numerator = frame << 4;
    index = duration ? (uint32)((sint64)(sint32)numerator / (sint32)duration) :
        ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
    buffer = r_u32(0x800ADEFC);
    value = r_u16(0x800A8D00 + index * 4);
    w_u16(object + buffer * 40 + 94, value);
    dx = (uint32)r_s16(object + 30);
    x = r_u32(object + 40);
    dy = (uint32)r_s16(object + 34);
    dz = (uint32)r_s16(object + 38);
    w_u32(object + 40, x + dx);
    y = r_u32(object + 44);
    z = r_u32(object + 48);
    w_u32(object + 44, y + dy);
    w_u32(object + 48, z + dz);
    value = r_u32(object + 44);
    if ((sint32)value >= 0)
        return t3_8006f5ac(object);
    return value;
}

uint32 t3_800744e0(uint32 object)
{
    uint32 timer, accelerated, state;
    FUNCTION_MARKER(0x800744E0, "SLUS_004.02");
    timer = r_u32(object + 72);
    accelerated = r_u32(0x800954A4);
    w_u32(object + 72, timer - 1);
    if (accelerated)
        w_u32(object + 72, timer - 2);
    state = r_u8(object + 14);
    if (!state)
        return t3_8007457c(object);
    if (state == 1)
        return t3_80074594(object);
    if (state == 2)
        return t3_80074604(object);
    return 2;
}

uint32 t3_80074668(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x80074668, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        return t3_800746ec(object);
    if (state == 1 || state == 2)
        return t3_80074750(object);
    if (state == 3)
        return t3_8006f5ac(object);
    return 3;
}

uint32 t3_80074604(uint32 object)
{
    uint32 result, channel, sound;
    FUNCTION_MARKER(0x80074604, "SLUS_004.02");
    result = r_u32(object + 72);
    if ((sint32)result >= 0)
        return result;
    channel = r_u32(object + 64);
    sound = r_u32(object + 68);
    sound = (sound & 0xFFFFC3FF) | ((channel << 10) + 0x3800);
    w_u32(object + 68, sound);
    t3_800753bc(channel, sound, 2);
    return t3_8006f5ac(object);
}

uint32 t3_800746ec(uint32 object)
{
    uint32 channel, sound, state;
    FUNCTION_MARKER(0x800746EC, "SLUS_004.02");
    channel = r_u32(object + 64);
    sound = r_u32(object + 68);
    sound = t3_80075548((sint32)channel, sound);
    w_u32(object + 68, sound);
    if (!(sound & 0x3C00))
        return t3_8006f5ac(object);
    state = r_u8(object + 14);
    w_u32(object + 72, 15);
    w_u8(object + 14, state + 1);
    return state + 1;
}

uint32 t3_800748b8(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x800748B8, "SLUS_004.02");
    state = r_u8(object + 14);
    w_u32(object + 64, 0xFFFFFFFF);
    w_u32(object + 68, 0);
    w_u8(object + 14, state + 1);
    return 0xFFFFFFFF;
}

uint32 t3_80075548(sint32 index, uint32 sound)
{
    uint32 entry = 0x8001AF7C, key, value;
    FUNCTION_MARKER(0x80075548, "SLUS_004.02");
    (void)index;
    key = (uint32)r_s16(entry);
    for (;;) {
        value = r_u16(0x80097050 + key * 2);
        if (sound == value)
            return (uint32)r_s16(entry + 2);
        entry += 4;
        key = (uint32)r_s16(entry);
        if (key == 0xFFFFFFFF)
            return sound;
    }
}

void t3_80076174(void)
{
    FUNCTION_MARKER(0x80076174, "SLUS_004.02");
    t3_80052938();
}

void t3_8007666c(void)
{
    uint32 index;
    FUNCTION_MARKER(0x8007666C, "SLUS_004.02");
    w_u16(0x800A35B8, 0);
    for (index = 0; index < 3; ++index)
        w_u16(0x800A340A + index * 64, 0);
}

uint32 t3_800734b4(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x800734B4, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        t3_80073508(object);
    else if (state != 1)
        return 1;
    return t3_80073778(object);
}

uint32 t3_800738ac(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x800738AC, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        t3_80073900(object);
    else if (state != 1)
        return 1;
    return t3_80073bd0(object);
}

uint32 t3_8007444c(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x8007444C, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        w_u8(object + 14, 1);
    else if (state == 1)
        return t3_80074494(object);
    return 1;
}

uint32 t3_80074494(uint32 object)
{
    uint32 result;
    FUNCTION_MARKER(0x80074494, "SLUS_004.02");
    result = (uint32)SpuGetKeyStatus(2);
    if (result == 1)
        return result;
    t3_800753bc(0, 0x86DA, 0);
    return t3_8006f5ac(object);
}

uint32 t3_8007457c(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x8007457C, "SLUS_004.02");
    state = r_u8(object + 14);
    w_u32(object + 72, 15);
    w_u8(object + 14, state + 1);
    return state + 1;
}

uint32 t3_80074594(uint32 object)
{
    uint32 result, selection, packed, channel;
    FUNCTION_MARKER(0x80074594, "SLUS_004.02");
    result = r_u32(object + 72);
    if ((sint32)result >= 0)
        return result;
    selection = r_u32(object + 64);
    packed = r_u32(object + 68);
    channel = r_u32(object + 64);
    packed = (packed & 0xFFFFC3FF) | ((selection << 10) + 0x3000);
    w_u32(object + 68, packed);
    t3_800753bc(channel, packed, 1);
    result = r_u8(object + 14);
    w_u32(object + 72, 15);
    result += 1;
    w_u8(object + 14, result);
    return result;
}

uint32 t3_8006975c(uint32 actor)
{
    uint32 variant, identity, model;
    FUNCTION_MARKER(0x8006975C, "SLUS_004.02");
    variant = r_u16(actor + 0x14) & 1;
    identity = (uint32)r_s16(actor + 0x12);
    model = r_u32(0x800ADC20 + identity * 4);
    return r_u32(model + (variant ? 0x34 : 0x30));
}

uint32 t3_80069df4(void)
{
    uint32 value;
    FUNCTION_MARKER(0x80069DF4, "SLUS_004.02");
    if (!r_s16(0x800A0512)) {
        value = r_u32(0x800ADE80);
        w_u16(0x800A0512, 1);
        w_u32(0x800A0514, value);
    }
    return r_u32(0x800ADE80);
}

uint32 t3_8006b9e0(void)
{
    sint32 timer, mode;
    uint32 value;
    FUNCTION_MARKER(0x8006B9E0, "SLUS_004.02");
    timer = r_s16(0x800A05DE);
    value = r_u16(0x800A05DE);
    if (timer < 0) {
        t3_8006ab64();
        return 0;
    }
    mode = r_s16(0x800A061C);
    w_u16(0x800A05DE, value - 1);
    return mode == 0 || mode == 2;
}

uint32 t3_8006be40(uint32 status, const uint8 *response)
{
    uint32 sector;
    FUNCTION_MARKER(0x8006BE40, "SLUS_004.02");
    if ((status & 255) != 2)
        return 2;
    sector = t3_80090c0c((CdlLOC *)(response + 5));
    if ((sint32)sector <= 0)
        return 0x800A0000;
    w_u32(0x800A0618, sector);
    return 0x800A05D8;
}

uint32 t3_8006e0d0(uint32 object)
{
    uint32 angle, magnitude, sine, cosine;
    FUNCTION_MARKER(0x8006E0D0, "SLUS_004.02");
    angle = (uint32)r_s16(object + 2);
    magnitude = (uint32)r_s16(object + 8);
    angle = ((0 - angle) & 0xFFF) * 2;
    sine = (uint32)r_s16(0x8001E8C4 + angle);
    w_u32(object + 12, magnitude * sine);
    cosine = (uint32)r_s16(0x8001F0C4 + angle);
    w_u32(object + 20, magnitude * cosine);
    return cosine;
}

uint32 t3_80070b74(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x80070B74, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        t3_80070bc8(object);
    else if (state != 1)
        return 1;
    return t3_80070e08(object);
}

uint32 t3_80070ea0(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x80070EA0, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        t3_80070ef4(object);
    else if (state != 1)
        return 1;
    return t3_80071110(object);
}

uint32 t3_80078008(uint32 paused)
{
    uint32 value;
    FUNCTION_MARKER(0x80078008, "SLUS_004.02");
    value = r_u32(0x800ADCA4);
    if (value)
        return value;
    if (r_u8(0x80098A40) == 255) {
        w_u8(0x80098A41, 0);
        w_u8(0x80098A40, 0);
        w_u8(0x800A8672, 2);
        t3_8004b688(0);
        t3_800753bc(0, 0x50F4, 0);
    }
    value = r_u8(0x80098A41);
    if (value == 2)
        return t3_80078df8(paused);
    if (value == 4)
        return t3_bns_03_800b3bc0(paused);
    return t3_800780bc(paused);
}

uint32 t3_8003a9c8(uint32 actor)
{
    uint16 position[3];
    uint32 result, animation, data, index, cosine, sine, x, z, product, origin;
    sint32 last, elapsed;
    FUNCTION_MARKER(0x8003A9C8, "SLUS_004.02");
    result = r_u8(actor + 0xBE);
    if (!result)
        return result;
    last = r_s16(actor + 0x50);
    elapsed = r_s16(actor + 0x6E);
    elapsed = last - elapsed - 1;
    w_u8(actor + 0xBE, 0);
    if (elapsed < 0)
        elapsed = 0;
    animation = r_u32(actor + 0x4C);
    data = r_u32(animation);
    t3_800389c0(data, position, elapsed);
    t3_80038970(position, position + 1, position + 2, actor);
    index = (uint32)r_s16(actor + 0xE);
    x = (uint32)(sint32)(sint16)position[0];
    index = ((uint32)((sint32)(0x8000 - index) >> 3)) & 0x1FFE;
    cosine = (uint32)r_s16(0x8001F0C4 + index);
    product = x * cosine;
    z = (uint32)(sint32)(sint16)position[2];
    sine = (uint32)r_s16(0x8001E8C4 + index);
    product -= z * sine;
    if ((sint32)product < 0)
        product += 4095;
    origin = r_u32(actor + 0xF68);
    result = origin - (uint32)((sint32)product >> 12);
    w_u32(actor, result);
    sine = (uint32)r_s16(0x8001E8C4 + index);
    product = x * sine;
    cosine = (uint32)r_s16(0x8001F0C4 + index);
    product += z * cosine;
    if ((sint32)product < 0)
        product += 4095;
    origin = r_u32(actor + 0xF70);
    result = origin - (uint32)((sint32)product >> 12);
    w_u32(actor + 8, result);
    return result;
}

uint32 t3_80035f2c(uint32 index)
{
    uint32 yaw, pitch, numerator_x, numerator_z, quotient_x, quotient_z;
    sint32 denominator;
    SVECTOR vector = {0};
    MATRIX matrix = {0};
    VECTOR transformed;
    FUNCTION_MARKER(0x80035F2C, "SLUS_004.02");
    t3_80039e44(index, &yaw, &pitch);
    vector.vx = 0;
    vector.vy = (sint16)(0 - (yaw & 0xFFFF));
    vector.vz = (sint16)(0 - (pitch & 0xFFFF));
    t3_8003a490(&vector, &matrix);
    vector.vx = 4096;
    vector.vy = 0;
    vector.vz = 0;
    ApplyMatrix(&matrix, &vector, &transformed);
    numerator_x = (uint32)transformed.vx << 12;
    denominator = transformed.vy;
    quotient_x = denominator ? (uint32)((sint64)(sint32)numerator_x / denominator) :
        ((sint32)numerator_x < 0 ? 1 : 0xFFFFFFFF);
    numerator_z = (uint32)transformed.vz << 12;
    quotient_z = denominator ? (uint32)((sint64)(sint32)numerator_z / denominator) :
        ((sint32)numerator_z < 0 ? 1 : 0xFFFFFFFF);
    w_u16(0x800AFB50, 4096);
    w_u16(0x800AFB54, 0);
    w_u16(0x800AFB56, 0);
    w_u16(0x800AFB58, 0);
    w_u16(0x800AFB5A, 0);
    w_u16(0x800AFB5C, 0);
    w_u16(0x800AFB60, 4096);
    w_u16(0x800AFB52, 0 - quotient_x);
    w_u16(0x800AFB5E, 0 - quotient_z);
    return 0x800AFB50;
}

void t3_800630d8_host(uint32 actor, uint32 opponent, uint32 state, uint32 incoming_result)
{
    uint32 mode, animation, value, phase, selected, first, second, direction;
    uint32 duration, numerator, quotient, elapsed, reset, chosen, blend;
    sint32 owner, first_status, denominator, counter;
    uint32 current_result = incoming_result;
    FUNCTION_MARKER(0x800630D8, "SLUS_004.02");
    mode = r_u32(0x800AFA88);
    owner = -1;
    if (r_s16(0x800A923E) == 4) {
        animation = r_u32(0x800A927C);
        if (r_u8(animation + 0x24) == 83)
            owner = 0;
    }
    if (owner != 0 && r_s16(0x800AAACA) == 4) {
        animation = r_u32(0x800AAB08);
        if (r_u8(animation + 0x24) == 83)
            owner = 1;
    }
    value = r_u16(0x800ADED4);
    w_u16(0x800A8CE0, (uint32)owner);
    if (value)
        mode = 6;
    if (mode > 8)
        return;
    if (mode == 6) {
        counter = r_s16(0x800A8C72);
        if (!counter) {
            t3_80062a10();
            t3_800690a0(actor, opponent);
            t3_80065ed8(actor, opponent);
            t3_800666e4();
            t3_80069708();
            w_u16(0x800A8C72, 1);
        } else if (counter != 1) {
            w_u16(0x800A8C72, 0);
            return;
        }
        t3_80069100(actor, opponent);
        return;
    }
    if (mode == 8) {
        phase = r_u32(state);
        if (!phase) {
            t3_80062a10();
            t3_800690a0(actor, opponent);
            t3_80065ed8(actor, opponent);
            t3_800666e4();
            t3_80069708();
            t3_bns_02_800b4f78();
            w_u32(state, 2);
        } else if (phase != 2) {
            return;
        }
        t3_bns_02_800b5020();
        return;
    }
    phase = r_u32(state);
    switch (phase) {
    case 0:
        t3_80062a10();
        t3_800690a0(actor, opponent);
        t3_80065ed8(actor, opponent);
        t3_800666e4();
        t3_80069708();
        t3_80068394(1, actor, opponent);
        t3_80066138();
        t3_800666cc();
        w_u32(state, 1);
        goto initial_blend;
    case 1:
initial_blend:
        if (r_s16(0x800A8C74)) {
            t3_8006465c(actor, opponent, 0);
            w_u32(0x800A8CA0, 0xFFFFFFFF);
            t3_800666cc();
            value = t3_800684b4();
            blend = 4096 - value;
            w_u16(0x800A8C78, blend);
            w_u16(0x800A8C7C, value);
            if ((sint32)(blend << 16) > 0)
                t3_80066084(actor, opponent, r_s16(0x800A8C82) < 1, 5517);
            t3_80062b74();
            if (r_s16(0x800A8C7C) <= 0) {
                w_u16(0x800A8C70, 1);
                w_u32(state, 2);
            }
        }
        return;
    case 2:
        value = (uint32)r_s16(0x800A8C74);
        if (!value)
            return;
        t3_8006465c(actor, opponent, 0);
        owner = r_s16(0x800A8C98);
        if ((!owner && !r_s16(actor + 0x74)) ||
            (owner == 1 && !r_s16(opponent + 0x74)))
            w_u16(0x800A8C98, 0xFFFF);
        selected = r_u16(0x800A8C70);
        switch ((sint16)(selected - 1)) {
        case 0:
            counter = r_s16(0x800A8C8C);
            w_u32(0x800A8CA0, 0xFFFFFFFF);
            w_u16(0x800A8C94, 0xFFFF);
            w_u16(0x800A8C9E, 0xFFFF);
            if (!counter)
                break;
            if (r_s16(0x800A8C98) < 0) {
                first_status = r_s16(actor + 0x74);
                if (first_status > 0 || r_s16(opponent + 0x74) > 0) {
                    selected = first_status < 1;
                    w_u16(0x800A8C8E, selected);
                    w_u16(0x800A8C98, selected);
                    first = r_u32(0x800ADCB4);
                    second = r_u32(0x800ADCC4);
                    if (first == second) {
                        first = r_u32(0x800ADCD4);
                        second = r_u32(0x800ADCE4);
                    }
                    direction = !((sint32)first < (sint32)second);
                    if (selected)
                        direction = 1 - direction;
                    current_result = 0x800B0000;
                    owner = r_s16(0x800A8C8E);
                    w_u16(0x800A8C90, direction);
                    w_u16(0x800A8C9E, 0xFFFF);
                    duration = t3_800676a0(actor, opponent, (uint32)owner, direction);
                    if ((sint32)duration < 0) {
                        w_u32(0x800A8CA0, 0xFFFFFFFF);
                    } else {
                        if (r_s16(0x800A8CA4) >= 0) {
                            t3_800661b8(actor, opponent);
                            w_u16(0x800A8C92, 16);
                            selected = 3;
                        } else {
                            selected = 6;
                            w_u16(0x800A8C92, 0xFFFF);
                        }
                        w_u16(0x800A8C70, selected);
                        w_u16(0x800A8C96, duration);
                        w_u16(0x800A8C94, duration);
                    }
                    break;
                }
            }
            selected = t3_80067cec(actor, opponent);
            if ((sint32)selected < 0)
                break;
            first = r_u32(0x800ADCB4);
            second = r_u32(0x800ADCC4);
            w_u16(0x800A8C8E, selected);
            if (first == second) {
                first = r_u32(0x800ADCD4);
                second = r_u32(0x800ADCE4);
            }
            direction = !((sint32)first < (sint32)second);
            if (selected)
                direction = 1 - direction;
            w_u16(0x800A8C90, direction);
            duration = t3_80067f08();
            if ((sint32)duration >= 0 && r_s16(0x800A8CA4) >= 0) {
                w_u16(0x800A8C70, 8);
                w_u16(0x800A8C96, duration);
                w_u16(0x800A8C94, duration);
                value = t3_80067f40();
                w_u16(0x800A8C9E, value);
                w_u16(0x800A8C9C, value);
            }
            goto save_duration;
        case 1:
            owner = r_s16(0x800A8C8E);
            w_u16(0x800A8C7A, 4096);
            w_u16(0x800A8C7C, 0);
            w_u16(0x800A8C78, 0);
            if (!owner && t3_80067530(actor, opponent))
                goto start_return_blend;
            if (r_s16(0x800A8C8E) != 1 || !t3_80067530(opponent, actor))
                break;
start_return_blend:
            duration = t3_800679a4();
            w_u16(0x800A8C70, 4);
save_duration:
            w_u16(0x800A8C96, duration);
            w_u16(0x800A8C94, duration);
            break;
        case 2:
            counter = r_s16(0x800A8C92);
            value = r_u16(0x800A8C92);
            if (counter >= 0) {
                w_u16(0x800A8C92, value - 1);
                animation = r_u32(actor + 0x54);
                if (r_u8(animation + 0x24) != 9) {
                    animation = r_u32(opponent + 0x54);
                    if (r_u8(animation + 0x24) != 9)
                        break;
                }
                w_u32(0x800A8CA0, 0xFFFFFFFF);
                w_u16(0x800A8C92, 0xFFFF);
                w_u16(0x800A8C70, 1);
                break;
            }
            numerator = (uint32)r_s16(0x800A8C94);
            denominator = r_s16(0x800A8C96);
            numerator <<= 12;
            quotient = denominator ? (uint32)((sint32)numerator / denominator) :
                ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
            owner = r_s16(0x800A8C8E);
            w_u16(0x800A8C78, quotient);
            w_u16(0x800A8C7A, 4096 - quotient);
            if (!owner && t3_80067530(actor, opponent))
                goto interrupt_blend;
            if (r_s16(0x800A8C8E) == 1 && t3_80067530(opponent, actor)) {
interrupt_blend:
                duration = t3_800679a4();
                counter = r_s16(0x800A8C7A);
                w_u16(0x800A8C96, duration);
                w_u16(0x800A8C94, duration);
                if (!counter) {
                    w_u16(0x800A8C70, 1);
                } else if (!r_s16(0x800A8C78)) {
                    w_u16(0x800A8C70, 4);
                } else {
                    w_u16(0x800A8C80, 4096);
                    w_u16(0x800A8C7A, 0);
                    w_u16(0x800A8C78, 0);
                    w_u16(0x800A8C70, 5);
                    t3_8006460c(3);
                }
            } else {
                value = r_u16(0x800A8C94) - 1;
                w_u16(0x800A8C94, value);
                if ((sint32)(value << 16) < 0)
                    w_u16(0x800A8C70, 2);
            }
            break;
        case 3:
        case 4:
            numerator = (uint32)r_s16(0x800A8C94);
            denominator = r_s16(0x800A8C96);
            numerator <<= 12;
            quotient = denominator ? (uint32)((sint32)numerator / denominator) :
                ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
            elapsed = r_u16(0x800A8C94) - 1;
            w_u16(0x800A8C94, elapsed);
            if ((sint16)(selected - 1) == 3) {
                w_u16(0x800A8C78, 4096 - quotient);
                w_u16(0x800A8C7A, quotient);
                if ((sint32)(elapsed << 16) < 0) {
                    w_u16(0x800A8C96, 0xFFFF);
                    w_u16(0x800A8C94, 0xFFFF);
                    w_u16(0x800A8C70, 1);
                }
            } else {
                w_u16(0x800A8C80, quotient);
                w_u16(0x800A8C78, 4096 - quotient);
                if ((sint32)(elapsed << 16) < 0)
                    w_u16(0x800A8C70, 1);
            }
            break;
        case 5:
            reset = 0;
            if (r_s16(0x800A8CDE)) {
                w_u16(0x800A8C92, 0xFFFD);
                w_u16(0x800A8CDE, 0);
            } else if (r_s16(0x800A8C92) == -1) {
                reset = 1;
                w_u16(0x800A8C92, 0xFFFE);
            }
            owner = r_s16(0x800A8C8E);
            duration = t3_80066280(actor, opponent, reset, 5517, (uint32)owner, current_result);
            if ((sint32)duration < 0)
                return;
            current_result = 4096;
            numerator = duration << 12;
            w_u16(0x800A8C7A, 0);
            quotient = (uint32)((sint32)numerator / 320);
            w_u16(0x800A8C78, quotient);
            w_u16(0x800A8C7C, 4096 - quotient);
            animation = r_u32(actor + 0x54);
            if (r_u8(animation + 0x24) != 9) {
                animation = r_u32(opponent + 0x54);
                if (r_u8(animation + 0x24) != 9)
                    goto check_owner_end;
            }
            t3_800679a4();
            w_u16(0x800A8C96, 8);
            w_u16(0x800A8C94, 8);
            if (duration == 320) {
                w_u16(0x800A8C70, 1);
            } else {
                w_u16(0x800A8C80, 4096);
                w_u16(0x800A8C7C, 0);
                w_u16(0x800A8C78, 0);
                w_u16(0x800A8C70, 5);
                t3_8006460c(3);
            }
            goto clear_camera_phase;
check_owner_end:
            owner = r_s16(0x800A8C8E);
            if ((!owner && r_s16(actor + 0x74) < 0) ||
                (owner == 1 && r_s16(opponent + 0x74) < 0)) {
                w_u16(0x800A8C96, 35);
                w_u16(0x800A8C94, 35);
                w_u16(0x800A8C80, 4096);
                w_u16(0x800A8C7C, 0);
                w_u16(0x800A8C78, 0);
                w_u16(0x800A8C70, 5);
                t3_8006460c(3);
clear_camera_phase:
                w_u16(0x800A8C92, 0xFFFF);
                w_u32(0x800A8CA0, 0xFFFFFFFF);
            } else {
                counter = r_s16(0x800A8C92);
                current_result = 0x800B0000;
                if (counter == -2 && !duration) {
                    w_u16(0x800A8C92, 0xFFFD);
                } else if (r_s16(0x800A8C92) == -3 && duration == 320) {
                    duration = t3_800679a4();
                    w_u16(0x800A8C96, duration);
                    w_u16(0x800A8C94, duration);
                    w_u16(0x800A8C70, 1);
                    w_u16(0x800A8C92, 0xFFFF);
                    t3_800666e4();
                }
            }
            break;
        case 7:
            elapsed = r_u16(0x800A8C9C);
            w_u16(0x800A8C7A, 4096);
            w_u16(0x800A8C7C, 0);
            w_u16(0x800A8C78, 0);
            elapsed -= 1;
            w_u16(0x800A8C9C, elapsed);
            if ((sint32)(elapsed << 16) <= 0) {
                t3_80067f28();
                w_u16(0x800A8C9E, 0xFFFF);
                w_u16(0x800A8C70, 1);
                w_u16(0x800A8C78, 4096);
                w_u16(0x800A8C7C, 0);
                w_u16(0x800A8C7A, 0);
                w_u16(0x800A8C96, 0xFFFF);
                w_u16(0x800A8C94, 0xFFFF);
            }
            break;
        default:
            t3_80062a10();
            break;
        }
        if (r_s16(0x800A8C7A) > 0) {
            reset = r_s16(0x800A8C84) < 1;
            counter = r_s16(0x800A8C9E);
            owner = r_s16(0x800A8C8E);
            if (counter < 0)
                t3_80066280(actor, opponent, reset, 5517, (uint32)owner, current_result);
            else
                t3_80067f5c(actor, opponent, reset, 5517);
        }
        if (r_s16(0x800A8C78) > 0)
            t3_80066084(actor, opponent, r_s16(0x800A8C82) < 1, 5517);
        t3_80062b74();
        return;
    case 3:
    case 4:
        t3_8004acf0();
        chosen = r_u32(state) == 3 ? actor : opponent;
        t3_800686b0(chosen);
        w_u32(state, 5);
        t3_8006870c();
        return;
    case 5:
        t3_8006870c();
        return;
    case 6:
    case 7:
        t3_8004acf0();
        chosen = r_u32(state) == 6 ? actor : opponent;
        value = r_u32(chosen + 0x3F4);
        t3_8006895c(chosen, value == 0);
        return;
    case 8:
        return;
    default:
        return;
    }
}

uint32 t3_80066dc4(uint32 data)
{
    FUNCTION_MARKER(0x80066DC4, "SLUS_004.02");
    t3_800663f8(data, 0, 0);
    return t3_800663f8(0, 0x800A0428, 0);
}

uint32 t3_800674e0(uint32 index)
{
    FUNCTION_MARKER(0x800674E0, "SLUS_004.02");
    if (index < 30)
        return (uint32)r_s16(0x80024170 + index * 2);
    if ((sint32)index < 0 && index <= 0x87FFFFFF)
        return 0xFFFFFFFF;
    return 0;
}

uint32 t3_800686b0(uint32 actor)
{
    uint32 toggle;
    FUNCTION_MARKER(0x800686B0, "SLUS_004.02");
    toggle = r_u16(0x800A049C);
    w_u16(0x800A8C74, 0);
    w_u32(0x800A0498, actor);
    toggle = (toggle + 1) & 1;
    w_u16(0x800A049C, toggle);
    return toggle ? t3_800467a0(actor) : t3_80046460(actor);
}

uint32 t3_8006870c(void)
{
    sint32 toggle;
    uint32 actor;
    FUNCTION_MARKER(0x8006870C, "SLUS_004.02");
    toggle = r_s16(0x800A049C);
    w_u16(0x800A8C74, 0);
    actor = r_u32(0x800A0498);
    return toggle ? t3_80046a20(actor) : t3_800465bc(actor);
}
