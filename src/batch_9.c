#include "game_functions.h"

uint32 t3_80038430(uint32 actor, uint32 bone, uint32 matrix, uint32 point, uint32 target, uint32 *changed)
{
    uint32 shape, source, row, value, table, first, second, angle, length;
    VECTOR position;
    FUNCTION_MARKER(0x80038430, "SLUS_004.02");
    shape = r_u32(actor + (bone - 18) * 4 + 0x136C);
    position.vz = r_s16(shape + 20);
    position.vy = r_s16(shape + 18);
    position.vx = r_s16(shape + 16);
    source = actor + (uint32)(sint32)(sint16)target * 68 + 0x8F4;
    ApplyMatrixLV((MATRIX *)psx_addr(source, 32), &position, &position);
    value = r_u32(source + 28);
    position.vz = (sint32)((uint32)position.vz + value);
    value = r_u32(source + 24);
    position.vy = (sint32)((uint32)position.vy + value);
    value = r_u32(source + 20);
    position.vx = (sint32)((uint32)position.vx + value);
    value = r_u32(point + 8);
    position.vz = (sint32)((uint32)position.vz - value);
    value = r_u32(point + 4);
    position.vy = (sint32)((uint32)position.vy - value);
    value = r_u32(point);
    position.vx = (sint32)((uint32)position.vx - value);
    ApplyMatrixLV((MATRIX *)psx_addr(matrix, 32), &position, &position);
    value = (uint32)r_s16(actor + 28);
    row = r_u16(shape);
    if (value == 38) {
        *changed = 0;
        if (position.vx < 0) {
            row = 8;
            *changed = 1;
        }
    }
    table = 0x80095D20 + (uint32)(sint32)(sint16)row * 24;
    first = r_u32(table) * (uint32)position.vz;
    second = r_u32(table + 4) * (uint32)position.vx;
    angle = (uint32)ratan2((sint32)first, (sint32)second);
    w_u32(actor + 0x1388, angle);
    first = (uint32)position.vx;
    second = (uint32)position.vz;
    length = t3_8004aedc(first * first + second * second);
    first = r_u32(table + 8) * (uint32)position.vy;
    second = r_u32(table + 12) * length;
    angle = (uint32)ratan2((sint32)first, (sint32)second);
    w_u32(actor + 0x138C, angle);
    value = r_u32(table + 20);
    w_u32(actor + 0x138C, angle * value);
    first = r_u32(actor + 0x1388);
    value = r_u32(table + 16);
    w_u32(actor + 0x1388, first * value);
    return value;
}

uint32 t3_80037cbc(uint32 actor, uint32 bone)
{
    uint32 part = bone - 18, shape, angles0, angles1, value, result;
    uint32 matrix, parent, row, axis, first, second, product, sum;
    uint32 negative = 1, changed = 0, mode, table, limits = 0;
    uint32 yaw, pitch, previous_yaw, previous_pitch, coefficient_yaw, coefficient_pitch;
    sint32 character, identity, bound;
    VECTOR transformed;
    FUNCTION_MARKER(0x80037CBC, "SLUS_004.02");
    character = r_s16(actor + 28);
    angles0 = r_u32(actor + 0x129E + part * 8);
    angles1 = r_u32(actor + 0x12A2 + part * 8);
    result = r_u32(0x80095494);
    shape = r_u32(actor + 0x136C + part * 4);
    if (result)
        return result;
    result = r_u32(0x80095468);
    if (result)
        return result;
    identity = r_s16(actor + 28);
    value = r_u32(0x8009623C + (uint32)identity * 24 + part * 4);
    result = (uint32)character - 32;
    if (value == 0x80095F48)
        return result;
    value = result < 2;
    result = (sint32)part < 2;
    if (value && result)
        return result;
    result = (uint32)r_s16(actor + 0x1360 + part * 2);
    if (!result) {
        w_u16(actor + 0x1360 + part * 2, 1);
        t3_8003a338((sint16)angles0, (sint16)(angles0 >> 16), (sint16)angles1, (MATRIX *)psx_addr(actor + bone * 32 + 0xF74, 32));
        return actor + bone * 32 + 0xF74;
    }
    t3_8003a338((sint16)angles0, (sint16)(angles0 >> 16), (sint16)angles1, (MATRIX *)psx_addr(0x1F800000, 32));
    parent = r_u32(actor + bone * 68 + 0x934);
    MulMatrix0((MATRIX *)psx_addr(parent, 32), (MATRIX *)psx_addr(0x1F800000, 32), (MATRIX *)psx_addr(0x1F800020, 32));
    TransposeMatrix((MATRIX *)psx_addr(0x1F800020, 32), (MATRIX *)psx_addr(0x1F800090, 32));
    matrix = actor + bone * 68 + 0x8F4;
    w_u32(0x1F800048, 0);
    w_u32(0x1F800044, 0);
    value = (uint32)r_s16(shape + 8);
    w_u32(0x1F800040, value);
    ApplyMatrix((MATRIX *)psx_addr(matrix, 32), (SVECTOR *)psx_addr(0x1F800040, 8), &transformed);
    w_u32(0x1F800060, (uint32)transformed.vx);
    w_u32(0x1F800064, (uint32)transformed.vy);
    w_u32(0x1F800068, (uint32)transformed.vz);
    for (row = 3; row-- > 0;) {
        first = r_u32(matrix + 20 + row * 4);
        second = r_u32(0x1F800060 + row * 4);
        w_u32(0x1F800060 + row * 4, second + first);
    }
    value = r_u16(actor + bone * 32 + 0xF88);
    w_u16(0x1F800040, (uint16)value);
    value = r_u16(actor + bone * 32 + 0xF8C);
    w_u16(0x1F800042, (uint16)value);
    value = r_u16(actor + bone * 32 + 0xF90);
    w_u16(0x1F800044, (uint16)value);
    parent = r_u32(actor + bone * 68 + 0x934);
    ApplyMatrix((MATRIX *)psx_addr(parent, 32), (SVECTOR *)psx_addr(0x1F800040, 8), &transformed);
    w_u32(0x1F800050, (uint32)transformed.vx);
    w_u32(0x1F800054, (uint32)transformed.vy);
    w_u32(0x1F800058, (uint32)transformed.vz);
    parent = r_u32(actor + bone * 68 + 0x934);
    for (row = 3; row-- > 0;) {
        first = r_u32(parent + 20 + row * 4);
        second = r_u32(0x1F800050 + row * 4);
        w_u32(0x1F800050 + row * 4, second + first);
    }
    if (r_s32(0x1F800064) >= 0) {
        negative = 0;
        w_u32(0x1F800064, 0);
    }
    for (row = 3; row-- > 0;) {
        first = r_u32(0x1F800050 + row * 4);
        second = r_u32(0x1F800060 + row * 4);
        w_u32(0x1F800060 + row * 4, second - first);
    }
    SetRotMatrix((MATRIX *)psx_addr(0x1F800090, 32));
    ApplyRotMatrixLVPSX(0x1F800060, 0x1F800060);
    t3_8003a5bc(0x1F800060, 0x1F800070);
    w_u32(0x1F800068, 0);
    w_u32(0x1F800064, 0);
    w_u32(0x1F800060, 0);
    if (negative) {
        value = (uint32)r_s16(shape + 10);
        w_u16(0x1F800082, (uint16)((256 - value) << 4));
        w_u16(0x1F800080, (uint16)((sint32)value >> 4));
        w_u16(0x1F800084, (uint16)((sint32)value >> 4));
        ApplyRotMatrix((SVECTOR *)psx_addr(0x1F800080, 8), &transformed);
        for (axis = 0; axis < 3; ++axis)
            w_u32(0x1F800060 + axis * 4, xport_gte_read_data(9 + axis));
    }
    for (row = 3; row-- > 0;) {
        first = (uint32)r_s16(shape + 4);
        second = r_u32(0x1F800070 + row * 4);
        product = first * second;
        w_u32(0x1F800040 + row * 4, product);
        first = (uint32)r_s16(shape + 6);
        second = r_u32(0x1F800060 + row * 4);
        sum = product + first * second;
        w_u32(0x1F800040 + row * 4, sum);
        if ((sint32)sum < 0)
            sum += 255;
        w_u32(0x1F800040 + row * 4, (uint32)((sint32)sum >> 8));
    }
    first = (uint32)r_s16(shape + 4);
    second = (uint32)r_s16(shape + 6);
    value = r_u32(0x1F800040);
    w_u32(0x1F800040, value + ((256 - first - second) << 4));
    mode = r_u16(shape + 12);
    if (mode == 1) {
        value = (uint32)r_s16(shape + 14);
        t3_80038430(actor, bone, 0x1F800090, 0x1F800050, value, &changed);
    } else if ((character == 36 || character == 41) && !part) {
        t3_8003866c(actor, 0x1F800090, 0x1F800040);
    }
    value = r_u16(shape);
    if (character == 38 && changed)
        value = 2;
    table = 0x80095D20 + (uint32)(sint32)(sint16)value * 24;
    first = r_u32(table);
    second = r_u32(0x1F800048);
    first *= second;
    second = r_u32(0x1F800040);
    value = r_u32(table + 4);
    yaw = (uint32)ratan2((sint32)first, (sint32)(second * value));
    first = r_u32(0x1F800040);
    second = r_u32(0x1F800048);
    result = SquareRoot0PSX(first * first + second * second, 0x80098D08);
    first = r_u32(table + 8);
    second = r_u32(0x1F800044);
    value = r_u32(table + 12);
    pitch = (uint32)ratan2((sint32)(first * second), (sint32)(value * result));
    yaw *= r_u32(table + 16);
    pitch *= r_u32(table + 20);
    if (mode == 1) {
        identity = r_s16(shape + 22);
        table = 0x80095D20 + (uint32)identity * 24;
        coefficient_yaw = (uint32)r_s16(table + 20);
        coefficient_pitch = (uint32)r_s16(table + 16);
        previous_yaw = r_u32(actor + 0x1388);
        previous_pitch = r_u32(actor + 0x138C);
        if ((sint32)(coefficient_yaw * previous_yaw + coefficient_pitch * previous_pitch) <
            (sint32)(coefficient_yaw * yaw + coefficient_pitch * pitch)) {
            yaw = previous_yaw;
            pitch = previous_pitch;
        }
    }
    yaw <<= 4;
    pitch <<= 4;
    if ((character == 14 && !part) || (character == 15 && part - 1 < 2) || (character == 27 && part == 2))
        yaw = 0;
    if (character == 38) {
        if (!part && (sint32)pitch < 2048)
            pitch = 2048;
        else if (part == 3)
            yaw = 0;
    }
    if (character == 26)
        limits = 0x80095D10;
    else if ((character == 36 || character == 41) && (sint32)part < 4)
        limits = 0x80095D14;
    if (limits) {
        bound = r_s16(limits);
        if (bound < (sint16)yaw)
            yaw = (uint32)bound;
        else {
            bound = r_s16(limits + 2);
            if ((sint16)yaw < bound)
                yaw = (uint32)bound;
        }
        bound = r_s16(limits);
        if (bound < (sint16)pitch)
            pitch = (uint32)bound;
        else {
            bound = r_s16(limits + 2);
            if ((sint16)pitch < bound)
                pitch = (uint32)bound;
        }
    }
    t3_8003a338(0, (sint16)yaw, (sint16)pitch, (MATRIX *)psx_addr(0x1F800020, 32));
    result = actor + bone * 32 + 0xF74;
    MulMatrix0((MATRIX *)psx_addr(0x1F800000, 32), (MATRIX *)psx_addr(0x1F800020, 32), (MATRIX *)psx_addr(result, 32));
    return result;
}

uint32 t3_80044bd8(uint32 actor, uint32 opponent, uint32 record)
{
    uint32 damage, guarded, value, animation, divisor, numerator, result;
    sint32 power, extra, frame;
    FUNCTION_MARKER(0x80044BD8, "SLUS_004.02");
    damage = r_u16(opponent + 94);
    guarded = r_u8(record + 34);
    w_u16(record + 28, (uint16)damage);
    if (guarded) {
        damage = r_u8(record + 35) ? (uint32)((sint16)damage / 10 + 2) : 0;
    } else {
        if (r_u8(record + 38)) {
            damage += (uint32)((sint16)damage / 2);
        } else {
            value = (uint32)r_s16(record + 24);
            damage = t3_80045038((uint32)(sint32)(sint16)damage, value);
        }
        if (r_u8(record + 37)) {
            power = r_s16(actor + 94);
            if (power < 0)
                power = -power;
            extra = (3 * (sint16)power) / 10;
            if (!r_s16(actor + 290) && !r_s16(opponent + 290)) {
                animation = r_u32(actor + 84);
                divisor = r_u8(animation + 45);
                if (!divisor || (sint32)divisor < r_s16(actor + 88))
                    divisor = r_u16(actor + 88);
                power = (3 * (sint16)extra) / 10;
                frame = r_s16(actor + 88);
                numerator = (uint32)power * (uint32)frame;
                divisor = (uint32)(sint32)(sint16)divisor;
                if (!divisor)
                    extra = (sint32)numerator < 0 ? 1 : -1;
                else
                    extra = (sint32)((sint64)(sint32)numerator / (sint64)(sint32)divisor);
            }
            damage = (uint32)extra + (uint32)((12 * (sint16)damage) / 10);
        }
        if (r_u8(record + 39)) {
            if (!r_s16(actor + 148)) {
                damage = (uint32)((80 * (sint16)damage) / 100);
            } else {
                value = (uint32)(sint32)(sint16)damage;
                damage = (value + (value >> 31)) >> 1;
            }
        }
    }
    result = r_u16(record + 30) + damage;
    w_u16(record + 30, (uint16)result);
    return result;
}

uint32 t3_8004439c(uint32 actor)
{
    uint32 opponent, bonus, total = 0, row, record, value, side, selected, animation, index, code, raw, resource, gate, health, maximum, score = 0, result;
    FUNCTION_MARKER(0x8004439C, "SLUS_004.02");
    opponent = 0x800A9228 + t3_80045bb8(actor) * 6284;
    bonus = t3_800449b4(actor);
    for (row = 0; row < 2; ++row) {
        record = actor + 0x13C + row * 44;
        if (r_u8(record + 33)) {
            side = r_u8(record + 32);
            w_u8(actor + 199, 0);
            opponent = 0x800A9228 + side * 6284;
            t3_80043a94(actor, opponent);
            t3_80044a8c(actor, opponent, record);
            t3_80044bd8(actor, opponent, record);
            total += (uint32)r_s16(record + 30);
        }
    }
    if (r_u8(actor + 206)) {
        selected = t3_80044e34(actor);
        w_u32(actor + 404, selected);
        t3_80044fe4(actor, selected);
        if (r_u32(0x800AFA88) == 5) {
            side = r_u8(actor + 30);
            if (r_u32(0x800954C0 + side * 4) && !r_u8(actor + 138) && r_u8(actor + 208))
                w_u8(actor + 211, 1);
        }
        t3_80040a94(actor, opponent);
        if (r_u32(0x800AFA88) == 7 && r_u8(0x800B668C)) {
            animation = r_u32(0x800AC394);
            code = r_u16(animation + 50);
            resource = code & 0xF000 ? 0x800174C4 + (code & 0xFFF) * 2 : 0x80010CE8 + r_u16(animation + 50) * 42;
        } else if (r_u8(actor + 231)) {
            resource = 0x8001DEC0;
        } else if (r_u8(actor + 212)) {
            animation = r_u32(opponent + 84);
            index = r_u16(animation + 52);
            index = 0x800177DC + index * 4;
            code = (uint32)r_s16(index);
            raw = r_u16(index);
            resource = code & 0xF000 ? 0x800174C4 + (raw & 0xFFF) * 2 : 0x80010CE8 + code * 42;
        } else {
            animation = r_u32(opponent + 84);
            code = r_u16(animation + 50);
            resource = code & 0xF000 ? 0x800174C4 + (code & 0xFFF) * 2 : 0x80010CE8 + r_u16(animation + 50) * 42;
        }
        w_u32(actor + 424, resource);
    }
    gate = r_u32(0x8009547C);
    health = r_u32(actor + 1012);
    if (!gate && !r_u32(0x800954B4))
        w_u32(actor + 1012, health - ((bonus + total) << 16));
    if (r_u32(0x800AFA88) == 7 && r_u8(opponent + 30) != 2)
        w_u32(actor + 1012, health);
    value = r_u32(actor + 1012);
    if ((sint32)value < 0) {
        w_u32(actor + 1012, 0);
        value = r_u32(actor + 1012);
    }
    maximum = r_u32(actor + 1016);
    if ((sint32)maximum < (sint32)value)
        w_u32(actor + 1012, maximum);
    if (!r_u32(actor + 1012) && health)
        w_u8(actor + 222, 1);
    if (r_u8(actor + 222)) {
        selected = r_u32(actor + 404);
        if (r_u8(selected + 35)) {
            w_u8(actor + 207, 0);
            w_u8(actor + 208, 1);
        }
    }
    if (r_u8(actor + 206)) {
        selected = r_u32(actor + 404);
        t3_800448e0(actor, selected, (uint32)(sint32)(sint16)total);
    }
    if (r_u8(actor + 222) && !r_u32(0x80095498)) {
        if (r_u32(0x800AFA88) == 8)
            goto mode_eight;
        t3_80031ddc(opponent);
        w_u32(0x80095498, 1);
    }
    if (r_u32(0x800AFA88) != 8) {
        result = 8;
        goto finish;
    }
mode_eight:
    result = r_u8(actor + 197);
    if (!result)
        goto finish;
    if (r_u8(actor + 206)) {
        for (row = 0; row < 2; ++row) {
            record = actor + 0x13C + row * 44;
            if (r_u8(record + 33)) {
                side = r_u8(record + 32);
                opponent = 0x800A9228 + side * 6284;
                value = t3_bns_02_800b297c();
                if (opponent == value || r_u8(opponent + 230))
                    score += (uint32)r_s16(record + 28);
            }
        }
    }
    if ((sint32)bonus > 0)
        score += bonus * 2;
    result = 0x800B0000;
    if (score) {
        value = r_u32(0x800B6C08);
        w_u32(0x800B6C08, value + score * 10);
    }
finish:
    w_u16(actor + 204, (uint16)bonus);
    return result;
}

uint32 t3_8002e0bc(uint32 entry, uint32 actor, uint32 opponent)
{
    uint32 type, side, value, limit, classification, animation;
    sint32 frame;
    FUNCTION_MARKER(0x8002E0BC, "SLUS_004.02");
    type = r_u8(entry + 3);
    if (type >= 2 && type <= 23) {
        side = r_u8(actor + 30);
        if (!r_u32(0x80095764 + side * 4))
            t3_8003115c(side, actor, opponent);
        limit = r_u16(entry + 4);
        value = r_u32(actor + 244);
        if (limit < value)
            return 0;
        side = r_u8(actor + 30);
        if (type < 20) {
            type = r_u8(entry + 3);
            classification = r_u32(0x80095764 + side * 4);
            return type == classification;
        }
        classification = r_u32(0x80095764 + side * 4);
        return classification == type - 18 || classification == type - 14;
    }
    switch (type) {
    case 0: return 1;
    case 1: return r_u8(actor + 209) != 0;
    case 24:
        limit = r_u16(entry + 4);
        value = r_u32(actor + 248);
        return limit >= value;
    case 25:
        limit = r_u16(entry + 4);
        value = r_u32(actor + 248);
        return value >= limit;
    case 26:
        if (!r_u8(actor + 209)) return 0;
        value = r_u16(actor + 100);
        limit = r_u16(opponent + 102);
        return !(value & limit);
    case 27: return r_u8(opponent + 207);
    case 28: return r_u8(actor + 210);
    case 29: return r_s16(opponent + 410) != 33;
    case 30: return r_s16(opponent + 94) != 0;
    case 31:
        if (!r_s16(opponent + 94)) return 0;
        return (r_u16(opponent + 96) & 7) == 2;
    case 32:
        if (!r_s16(opponent + 94)) return 0;
        return (r_u16(opponent + 96) & 7) == 1;
    case 33: return r_s16(opponent + 94) == 0;
    case 34: return (r_u32(opponent + 96) & 0x407) == 2;
    case 35: return (r_u32(opponent + 96) & 0x407) == 1;
    case 36: return r_s16(actor + 48) >= 16385;
    case 37: return (uint32)(r_u16(actor + 46) - 20024) < 0x31C8;
    case 38:
        value = r_u16(actor + 46);
        return value > 0x7FFF && value <= 0xB1C6;
    case 39: return r_s16(actor + 50) == 0;
    case 40: return r_s16(actor + 50) == 1;
    case 41: return r_s16(actor + 50) == 3;
    case 42: return r_s16(actor + 50) == 2;
    case 43: return r_s16(actor + 64) == 0;
    case 44: return r_s16(actor + 64) == 1;
    case 45: return r_s16(actor + 64) == 3;
    case 46: return r_s16(actor + 64) == 2;
    case 47: return 0;
    case 48:
        if (r_u16(opponent + 100) != 0x412) return 0;
        animation = r_u32(opponent + 84);
        frame = r_s16(opponent + 88);
        value = r_u8(animation + 46);
        if ((sint32)value < frame) return 0;
        return r_u32(actor + 248) < 0x700;
    case 49: return r_u32(actor + 280) == 0;
    case 50: return r_u32(actor + 280) != 0;
    case 51: return r_u8(actor + 213);
    case 52:
        if (!r_u8(actor + 213) || r_u8(opponent + 219)) return 0;
        return !(r_u32(opponent + 96) & 4);
    case 53: return r_u32(opponent + 96) & 4;
    case 54: return ((r_u32(opponent + 96) >> 2) ^ 1) & 1;
    case 55:
        if (!(r_u32(opponent + 96) & 4)) return 0;
        animation = r_u32(opponent + 84);
        if (r_u8(animation + 25)) return 0;
        limit = r_u16(entry + 4);
        value = r_u32(actor + 248);
        return limit >= value;
    case 56:
        if (!(r_u32(opponent + 96) & 4)) return 0;
        return r_s16(actor + 48) >= 16384;
    case 57:
        if (!r_u8(opponent + 211)) return 0;
        return r_u8(opponent + 219) == 0;
    case 58: case 59: case 60:
        value = r_u8(actor + 224 + type - 58);
        w_u8(actor + 140, 1);
        return value;
    case 61: return r_u32(actor + 1012) == 0;
    case 62: return r_u8(actor + 198) == 0;
    case 63: return r_u8(actor + 198) != 0;
    case 64:
        if (r_u8(actor + 198)) return 0;
        return r_s16(actor + 48) >= 16385;
    case 65:
        if (!r_u8(actor + 198)) return 0;
        return r_s16(actor + 48) >= 16385;
    case 66:
        if (r_s16(actor + 142)) return 0;
        return r_u32(actor + 1012) != 0;
    case 67: return r_s16(actor + 290) != 0;
    case 68: return r_s16(actor + 290) == 0;
    default: return 0;
    }
}

uint32 t3_80058be8(uint32 command, uint32 context)
{
    uint32 actor, opponent, animation, outer, marker, flags, entry, count, row, phase, found = 0, index, bank, candidate, type, accepted, value;
    sint32 wanted, frame;
    FUNCTION_MARKER(0x80058BE8, "SLUS_004.02");
    actor = r_u32(0x800AFA5C);
    animation = r_u32(actor + 0x54);
    outer = r_u32(animation + 12);
    opponent = r_u32(0x800A867C);
    marker = r_u16(outer);
    flags = r_u32(context + 100);
    entry = outer;
    wanted = (sint16)command;
    for (phase = 0; phase < 2; ++phase) {
        while (marker != 0xC000) {
            count = 1;
            if (marker == 0xC00C) {
                index = r_u16(outer + 6);
                count = r_u8(outer + 11);
                entry = 0x80017C78 + index * 12;
            } else {
                entry = outer;
            }
            for (row = 0; row < count; ++row, entry += 12) {
                if ((sint32)r_u16(entry + 6) != wanted)
                    continue;
                if (phase && r_u8(entry + 9) != 1)
                    continue;
                frame = r_s16(actor + 0x58);
                value = r_u8(entry + 9);
                if (frame < (sint32)value)
                    continue;
                value = r_u8(entry + 10);
                if ((sint32)value < frame)
                    continue;
                type = r_u8(entry + 2);
                if (!type)
                    accepted = 1;
                else if (type < 24)
                    accepted = r_s16(actor + 22) == (sint32)type - 1;
                else if (type < 47)
                    accepted = r_s16(actor + 22) != (sint32)type - 24;
                else if (type < 70)
                    accepted = r_s16(opponent + 22) == (sint32)type - 47;
                else if (type < 93)
                    accepted = r_s16(actor + 24) == (sint32)type - 70;
                else if (type < 116)
                    accepted = r_s16(actor + 24) != (sint32)type - 93;
                else
                    accepted = 0;
                if (!accepted)
                    continue;
                type = r_u8(entry + 3);
                switch (type) {
                case 0: accepted = 1; break;
                case 0x24: accepted = flags & 1; break;
                case 0x25: accepted = flags & 2; break;
                case 0x26: accepted = flags & 4; break;
                case 0x30: accepted = flags & 8; break;
                case 0x27: accepted = flags & 16; break;
                default: accepted = t3_8002e0bc(entry, actor, opponent); break;
                }
                if (accepted) {
                    found = 1;
                    break;
                }
            }
            outer += 12;
            marker = r_u16(outer);
        }
        if (phase || found)
            return found;
        bank = r_u8(actor + 0x1886);
        index = r_u16(entry + 6);
        bank = r_u32(0x8009F2E8 + bank * 4);
        candidate = r_u32(bank + index * 4);
        animation = r_u32(actor + 0x54);
        if (candidate == animation)
            return 0;
        value = r_u8(entry + 9);
        frame = r_s16(actor + 0x58);
        if (frame < (sint32)value)
            return 0;
        if ((sint32)r_u8(entry + 10) < frame)
            return 0;
        outer = r_u32(candidate + 12);
        marker = r_u16(outer);
    }
    return found;
}

uint32 t3_80034100(uint32 actor)
{
    PSX_RECT rectangle;
    uint32 animation, flags, special, identity, table, size, source, destination, value, source_y, force, transition;
    sint32 character, index, source_index, size_index, destination_index, x, y, side;
    FUNCTION_MARKER(0x80034100, "SLUS_004.02");
    character = r_s16(actor + 24);
    animation = r_u32(actor + 0x54);
    identity = (uint32)r_s16(actor + 18);
    flags = r_u32(animation + 36);
    special = character == 17;
    force = 0;
    if (flags & 0x40000) {
        force = 1;
        if (r_u8(0x800B040C) && special) {
            t3_800344a4(actor, 1, 3, 1);
            index = r_s16(actor + 28);
            value = r_u8(actor + 0x1291);
            table = 0x8009591C + (uint32)index * 4;
            if (value != 4) {
                w_u8(actor + 0x1291, 4);
                size_index = r_s8(table);
                size = 0x8009586C + (uint32)size_index * 4;
                rectangle.w = (sint16)r_u16(size);
                rectangle.h = (sint16)r_u16(size + 2);
                source_index = r_s8(table + 3);
                destination_index = r_s8(table + 1);
                source = 0x8009579C + (uint32)source_index * 4;
                destination = 0x8009579C + (uint32)destination_index * 4;
                rectangle.x = (sint16)r_u16(source);
                source_y = r_u16(source + 2);
                rectangle.y = (sint16)source_y;
                y = r_s16(destination + 2);
                side = r_s16(actor + 18);
                x = r_s16(destination);
                if (side) {
                    rectangle.y = (sint16)(source_y + 4);
                    y += 4;
                }
                t3_800293bc(&rectangle, x, y);
            }
        }
    } else if ((r_u32(animation + 4) & 4) && !(flags & 0x40000000)) {
        force = 1;
    }
    if (force) {
        w_u16(actor + 0x128E, 2);
        if (r_u8(actor + 0x1290) != 1) {
            w_u8(actor + 0x1290, 1);
            index = r_s16(actor + 28);
            table = 0x800959CC + (uint32)index * 4;
            source_index = r_s8(table + 3);
            if (source_index != -1) {
                source = 0x8009579C + (uint32)source_index * 4;
                destination_index = r_s8(table);
                size_index = r_s8(table + 1);
                rectangle.x = (sint16)r_u16(source);
                source_y = r_u16(source + 2);
                rectangle.y = (sint16)source_y;
                size = 0x8009586C + (uint32)size_index * 4;
                rectangle.w = (sint16)r_u16(size);
                rectangle.h = (sint16)r_u16(size + 2);
                destination = 0x8009579C + (uint32)destination_index * 4;
                y = r_s16(destination + 2);
                side = r_s16(actor + 18);
                x = r_s16(destination);
                if (side) {
                    rectangle.y = (sint16)(source_y + 256);
                    y += 256;
                }
                t3_800293bc(&rectangle, x, y);
            }
        }
    }
    if (!r_s16(actor + 0x128E)) {
        transition = r_u8(actor + 0x1290) != 0;
        if (special && !r_u8(0x8009BCF0 + identity) && !r_s16(actor + 0x127E)) {
            t3_800344a4(actor, 1, transition ? 0 : 3, 1);
            w_u8(0x8009BCF0 + identity, 0);
        }
        if (transition) {
            value = ((uint32)randPSX() & 0xFE) + 1;
            transition = r_u8(actor + 0x1290);
            w_u16(actor + 0x128E, (uint16)value);
            if (transition) {
                index = r_s16(actor + 28);
                w_u8(actor + 0x1290, 0);
                table = 0x800959CC + (uint32)index * 4;
                source_index = r_s8(table + 2);
            } else {
                source_index = -1;
            }
        } else {
            w_u16(actor + 0x128E, 4);
            if (r_u8(actor + 0x1290) != 1) {
                w_u8(actor + 0x1290, 1);
                index = r_s16(actor + 28);
                table = 0x800959CC + (uint32)index * 4;
                source_index = r_s8(table + 3);
            } else {
                source_index = -1;
            }
        }
        if (source_index != -1) {
            source = 0x8009579C + (uint32)source_index * 4;
            destination_index = r_s8(table);
            size_index = r_s8(table + 1);
            rectangle.x = (sint16)r_u16(source);
            source_y = r_u16(source + 2);
            rectangle.y = (sint16)source_y;
            size = 0x8009586C + (uint32)size_index * 4;
            rectangle.w = (sint16)r_u16(size);
            rectangle.h = (sint16)r_u16(size + 2);
            destination = 0x8009579C + (uint32)destination_index * 4;
            y = r_s16(destination + 2);
            side = r_s16(actor + 18);
            x = r_s16(destination);
            if (side) {
                rectangle.y = (sint16)(source_y + 256);
                y += 256;
            }
            t3_800293bc(&rectangle, x, y);
        }
    }
    value = r_u16(actor + 0x128E) - 1;
    w_u16(actor + 0x128E, (uint16)value);
    return value;
}

uint32 t3_80034970(uint32 actor)
{
    uint32 gate, state, enabled, increment, index, table, value, row, current_address, target_address, step_address, raw, step, slot, animation, selection;
    sint32 character, target, current, next, frame = 0;
    FUNCTION_MARKER(0x80034970, "SLUS_004.02");
    gate = r_u32(0x80095494);
    character = r_s16(actor + 24);
    if (gate)
        return 0x80090000;
    if (r_u32(0x80095468))
        return 20;
    if (character == 20) {
        state = r_u8(actor + 0x128C);
        if (!state) {
            if (r_u8(actor + 0x128A)) {
                value = r_u8(actor + 0x128B);
                w_u8(actor + 0x128B, (uint8)(value + 2));
                index = r_u8(actor + 0x128B);
                if (r_u8(0x80095888 + index) == 255) {
                    w_u8(actor + 0x128C, 1);
                    w_u8(actor + 0x128B, 0);
                    w_u8(actor + 0x128D, 1);
                }
            } else {
                value = r_u8(actor + 0x128B);
                if (value)
                    w_u8(actor + 0x128B, (uint8)(value - 1));
            }
        } else if (state == 1) {
            increment = 2;
            if (r_u8(actor + 0x128D))
                w_u8(actor + 0x128D, 0);
            else
                increment = 1;
            enabled = r_u8(actor + 0x128A);
            value = r_u8(actor + 0x128B);
            w_u8(actor + 0x128B, (uint8)(value + increment));
            index = r_u8(actor + 0x128B);
            if (r_u8(0x8009589C + index) == 255) {
                if (enabled) {
                    w_u8(actor + 0x128B, 0);
                } else {
                    w_u8(actor + 0x128B, 16);
                    w_u8(actor + 0x128C, 0);
                }
            }
        }
        table = r_u8(actor + 0x128C) ? 0x8009589C : 0x80095888;
        index = r_u8(actor + 0x128B);
        value = r_u8(table + index);
        selection = t3_80032f04(actor, value);
        animation = r_u32(actor + 0x524);
        t3_80034844(animation, selection, actor);
    }
    for (row = 0; row < 2; ++row) {
        if (character == 20 && row == 1) {
            current_address = actor + 0x1296;
            target_address = actor + 0x1298;
            step_address = actor + 0x129A;
        } else {
            current_address = actor + 0x127C + row * 2;
            target_address = current_address + 4;
            step_address = current_address + 8;
        }
        target = r_s16(target_address);
        current = r_s16(current_address);
        raw = r_u16(current_address);
        if (current != target) {
            step = r_u16(step_address);
            value = current < target ? raw + step : raw - step;
            w_u16(current_address, (uint16)value);
            next = (sint16)value;
            if ((current < target && next >= target) || (target < current && next <= target)) {
                w_u16(step_address, 0);
                w_u16(current_address, (uint16)target);
            }
        }
        if (character == 20)
            continue;
        value = r_u16(current_address);
        current = (sint16)value;
        frame = current - 509;
        if (current < 513) {
            frame = current >> 7;
            if (frame)
                --frame;
        }
        slot = 12;
        if (!row)
            slot = character == 17 || character == 11 ? 17 : 16;
        selection = t3_80033084(actor, (uint32)frame, row);
        slot = actor + slot * 40;
        animation = r_u32(slot + 0x4F8);
        t3_80034844(animation, selection, actor);
        if (r_u32(slot + 0x4FC)) {
            selection = t3_80033084(actor, (uint32)frame, row);
            animation = r_u32(slot + 0x4FC);
            t3_80034844(animation, selection, actor);
        }
    }
    if (character == 20) {
        value = r_u16(actor + 0x127C);
        frame = (sint16)value >> 7;
        if (frame)
            --frame;
        selection = t3_80033084(actor, (uint32)frame, 0);
        animation = r_u32(actor + 0x7F0);
        return t3_80034844(animation, selection, actor);
    }
    value = (uint32)r_s16(actor + 18);
    w_u8(0x8009BCF0 + value, (uint8)frame);
    return 0x8009BCF0;
}

uint32 t3_800430fc(uint32 actor)
{
    uint32 correction[3] = {0, 0, 0};
    uint32 side, opponent, mode, value, magnitude, difference, x, y, z, other, relative, left, right, distance, product, owner, base, row;
    FUNCTION_MARKER(0x800430FC, "SLUS_004.02");
    side = r_u8(actor + 0x1E);
    opponent = t3_80045c18(actor);
    mode = r_u32(0x800AFA88);
    if (mode == 7) {
        value = r_u32(actor + 0xF70);
        magnitude = (sint32)value < 0 ? 0 - value : value;
        difference = magnitude - 256;
        if ((sint32)difference > 0)
            correction[2] = (sint32)value > 0 ? 0 - difference : difference;
        value = r_u32(actor + 0xF68);
        magnitude = (sint32)value < 0 ? 0 - value : value;
        difference = magnitude - 5120;
        if ((sint32)difference > 0)
            correction[0] = (sint32)value > 0 ? 0 - difference : difference;
        other = r_u8(actor + 0x1E);
        if (!other) {
            value = r_u32(actor + 0xF68);
            left = (sint32)value >= 0;
        } else if (other == 1) {
            value = r_u32(actor + 0xF68);
            left = (sint32)value <= 0;
        } else {
            left = 0;
        }
        if (left) {
            correction[0] = 0 - value;
            magnitude = (sint32)correction[0] < 0 ? 0 - correction[0] : correction[0];
            if ((sint32)magnitude >= 1001) {
                w_u32(actor + 0x60, 0x5000);
                w_u8(actor + 0x84, 0);
                w_u8(actor + 0x83, 0);
                w_u8(actor + 0xD9, 1);
            }
        }
        difference = (uint32)-4608 - r_u32(actor + 0xF6C);
        if ((sint32)difference > 0)
            correction[1] = difference;
    } else if (mode == 8 && r_s16(0x800AE204) != 6 && r_u32(0x80096F2C)) {
        z = r_u32(actor + 0xF70);
        difference = z - 2300;
        if ((sint32)difference > 0)
            correction[2] = 0 - difference;
        value = r_u16(0x800ADC84);
        difference = z + 2200;
        if (value == 18) {
            x = r_u32(actor + 0xF68);
            other = r_u32(0x800B6C0C);
            value = t3_bns_02_800b2bb8(x - other);
            z = r_u32(actor + 0xF70);
            difference = z - value;
        }
        if ((sint32)difference < 0)
            correction[2] = 0 - difference;
        if (!r_u8(actor + 0xC5)) {
            x = r_u32(actor + 0xF68);
            other = r_u32(0x800B6C0C);
            z = r_u32(actor + 0xF70);
            relative = x - other;
            left = t3_bns_02_800b2b3c(0, z);
            z = r_u32(actor + 0xF70);
            right = t3_bns_02_800b2b3c(1, z);
            difference = relative - left;
            if ((sint32)difference < 0)
                correction[0] = 0 - difference;
            difference = relative - right;
        } else {
            x = r_u32(actor + 0xF68);
            other = r_u32(0x800B6C0C);
            difference = x + 4200 - other;
            if ((sint32)difference < 0)
                correction[0] = 0 - difference;
            difference = x - 4200 - other;
        }
        if ((sint32)difference > 0)
            correction[0] = 0 - difference;
    } else {
        value = r_u32(actor + 0xF68);
        magnitude = (sint32)value < 0 ? 0 - value : value;
        difference = magnitude - 300000;
        if ((sint32)difference > 0)
            correction[0] = (sint32)value > 0 ? 0 - difference : difference;
        value = r_u32(actor + 0xF70);
        magnitude = (sint32)value < 0 ? 0 - value : value;
        difference = magnitude - 300000;
        if ((sint32)difference > 0)
            correction[2] = (sint32)value > 0 ? 0 - difference : difference;
        if (r_u32(actor + 0xF8) >= 0x1D00) {
            left = r_u32(opponent + 0x12C);
            right = r_u32(actor + 0xF68);
            x = left - right;
            left = r_u32(opponent + 0x130);
            right = r_u32(actor + 0xF70);
            z = left - right;
            distance = t3_8004aedc(x * x + z * z);
            if ((sint32)distance >= 7681) {
                value = (uint32)r_s16(actor + 0x2A);
                distance -= 7680;
                product = distance * t3_8002a194(value);
                correction[0] = (uint32)((sint32)product / 32767);
                value = (uint32)r_s16(actor + 0x2A);
                product = distance * t3_8002a1dc(value);
                correction[2] = (uint32)((sint32)product / 32767);
            } else if (distance >= r_u32(actor + 0xF8)) {
                left = r_u32(actor + 0x12C);
                right = r_u32(actor + 0xF68);
                x = r_u32(actor + 0x130);
                z = r_u32(actor + 0xF70);
                correction[0] = left - right;
                correction[2] = x - z;
            }
        }
    }
    if (r_u32(0x800AFA88) == 7 && r_s16(actor + 0x74) < 0) {
        w_u32(0x80097A44 + side * 4, 0);
        w_u32(0x80097A50 + side * 4, 0);
    }
    for (row = 0; row < 3; ++row) {
        value = r_u32(0x80097A44 + side * 4);
        magnitude = (sint32)correction[row] < 0 ? 0 - correction[row] : correction[row];
        if ((sint32)value < (sint32)magnitude)
            correction[row] = (sint32)correction[row] > 0 ? value : 0 - value;
    }
    for (row = 0; row < 2; ++row) {
        base = actor + (row ? 0xF68 : 0);
        x = r_u32(base);
        z = r_u32(base + 8);
        w_u32(base, x + correction[0]);
        y = r_u32(base + 4);
        w_u32(base + 8, z + correction[2]);
        w_u32(base + 4, y + correction[1]);
    }
    value = r_u8(actor + 0xC4);
    if (!value)
        return value;
    value = r_u8(opponent + 0xC4);
    if (!value)
        return 0x80090000;
    value = r_u32(0x80097A5C);
    if (!value)
        return value;
    owner = opponent;
    for (row = 0; row < 2; ++row) {
        base = owner + (row ? 0xF68 : 0);
        x = r_u32(base);
        z = r_u32(base + 8);
        w_u32(base, x + correction[0]);
        y = r_u32(base + 4);
        w_u32(base + 8, z + correction[2]);
        value = y + correction[1];
        w_u32(base + 4, value);
    }
    return value;
}

uint32 t3_8006a3bc(uint32 actor)
{
    uint32 origin[3], previous_origin[3], first_delta[3], second_delta[3];
    uint32 animation, side, sequence, value, other, offset, header, selector, mode, frame, cursor, first, second, x, y, z, row;
    sint32 angle;
    FUNCTION_MARKER(0x8006A3BC, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    side = r_u8(actor + 0x1E);
    sequence = r_u32(animation + 0x28);
    first = actor + 0x1AC;
    second = actor + 0x1C4;
    value = r_u8(actor + 0xB4);
    if (!value)
        return value;
    if (r_u8(sequence) >= 24)
        return 0;
    value = (uint32)r_s16(actor + 44);
    other = (uint32)r_s16(actor + 14);
    angle = (sint32)(value - other);
    w_u32(0x800A05C8, (uint32)angle);
    if (angle) {
        for (row = 0; row < 3; ++row) {
            value = r_u32(actor + 0xF68 + row * 4);
            w_u32(0x800A05CC + row * 4, value);
        }
        angle = -angle;
        if (angle < 0)
            angle += 15;
        offset = ((uint32)(angle >> 3)) & 0x1FFE;
        value = (uint32)r_s16(0x8001E8C4 + offset);
        w_u32(0x800A05C0, value);
        value = (uint32)r_s16(0x8001F0C4 + offset);
        w_u32(0x800A05C4, value);
    }
    angle = -r_s16(actor + 44);
    if (angle < 0)
        angle += 15;
    offset = ((uint32)(angle >> 3)) & 0x1FFE;
    value = (uint32)r_s16(0x8001E8C4 + offset);
    w_u32(0x800A05B8, value);
    value = (uint32)r_s16(0x8001F0C4 + offset);
    w_u32(0x800A05BC, value);
    value = r_u8(sequence + 4);
    other = r_u8(sequence + 5);
    header = (value << 8) | other;
    selector = r_u32(0x800A05A8 + side * 4);
    offset = 0;
    if (selector == 1)
        offset = (header & 0x7FFF) >> 4;
    else if (selector == 2) {
        if (header & 0x8000)
            offset = (header & 0x7FFE) >> 3;
    } else if (selector) {
        return header & 0x8000;
    }
    animation = r_u32(actor + 0x54);
    frame = (uint32)r_s16(actor + 0x58);
    value = r_u8(animation + 45);
    mode = header & 15;
    frame -= value;
    if (mode >= 15)
        return 0;
    cursor = sequence + offset + 6;
    switch (mode) {
    case 0: cursor += frame * 10; break;
    case 1: cursor += frame * 5 + 5; break;
    case 2: cursor += frame * 10 + 10; break;
    case 3: case 4: cursor += frame * 15 + 5; break;
    case 5: cursor += frame * 20; break;
    case 6: case 7: case 9: cursor += frame * 10 + 5; break;
    case 10: case 12: case 13: case 14: cursor += frame * 15; break;
    default: return 0x8006AA70;
    }
    origin[0] = r_u32(actor);
    origin[1] = r_u32(actor + 4);
    origin[2] = r_u32(actor + 8);
    previous_origin[0] = r_u32(0x800A0578 + side * 12);
    previous_origin[1] = r_u32(0x800A057C + side * 12);
    previous_origin[2] = r_u32(0x800A0580 + side * 12);
    if (mode == 1 || mode == 2 || mode == 4 || mode == 6 || mode == 7) {
        if (!frame) {
            t3_80069714(cursor - (mode == 2 ? 10 : 5), first_delta);
            t3_8006a230(first, first_delta, previous_origin);
        } else {
            for (row = 0; row < 3; ++row) {
                value = r_u32(0x800A0528 + side * 12 + row * 4);
                w_u32(first + row * 4, value);
            }
        }
        cursor = t3_80069714(cursor, second_delta);
    } else {
        cursor = t3_80069714(cursor, first_delta);
        t3_8006a230(first, first_delta, origin);
        cursor = t3_80069714(cursor, second_delta);
    }
    t3_8006a230(first + 12, second_delta, origin);
    switch (mode) {
    case 2: case 3:
        if (!frame) {
            t3_80069714(cursor - (mode == 2 ? 10 : 15), first_delta);
            t3_8006a230(second, first_delta, previous_origin);
        } else {
            for (row = 0; row < 3; ++row) {
                value = r_u32(0x800A054C + side * 12 + row * 4);
                w_u32(second + row * 4, value);
            }
        }
        t3_80069714(cursor, second_delta);
        return t3_8006a230(second + 12, second_delta, origin);
    case 4: case 5:
        cursor = t3_80069714(cursor, first_delta);
        t3_8006a230(second, first_delta, origin);
        t3_80069714(cursor, second_delta);
        return t3_8006a230(second + 12, second_delta, origin);
    case 6: case 10:
        t3_80069714(cursor, first_delta);
        t3_8006a230(second, first_delta, origin);
        x = r_u32(first + 12);
        y = r_u32(first + 16);
        z = r_u32(first + 20);
        w_u32(second + 12, x);
        w_u32(second + 16, y);
        w_u32(second + 20, z);
        return first;
    case 7: case 11:
        t3_80069714(cursor, second_delta);
        x = r_u32(first + 12);
        y = r_u32(first + 16);
        z = r_u32(first + 20);
        w_u32(second, x);
        w_u32(second + 4, y);
        w_u32(second + 8, z);
        return t3_8006a230(second + 12, second_delta, origin);
    case 9:
        if (!frame) {
            t3_80069714(cursor - 5, first_delta);
            t3_8006a230(second, first_delta, previous_origin);
        } else {
            for (row = 0; row < 3; ++row) {
                value = r_u32(0x800A054C + side * 12 + row * 4);
                w_u32(second + row * 4, value);
            }
        }
        break;
    case 12:
        t3_80069714(cursor, first_delta);
        t3_8006a230(second, first_delta, origin);
        break;
    case 13:
        t3_80069714(cursor, second_delta);
        x = r_u32(first);
        y = r_u32(first + 4);
        z = r_u32(first + 8);
        w_u32(second, x);
        w_u32(second + 4, y);
        w_u32(second + 8, z);
        return t3_8006a230(second + 12, second_delta, origin);
    case 14:
        t3_80069714(cursor, second_delta);
        t3_8006a230(second + 12, second_delta, origin);
        x = r_u32(second + 12);
        y = r_u32(second + 16);
        z = r_u32(second + 20);
        w_u32(second, x);
        w_u32(second + 4, y);
        w_u32(second + 8, z);
        return second;
    default: return 0x8006AA70;
    }
    x = r_u32(first);
    y = r_u32(first + 4);
    z = r_u32(first + 8);
    w_u32(second + 12, x);
    w_u32(second + 16, y);
    w_u32(second + 20, z);
    return first;
}

uint32 t3_8003ec64(uint32 actor)
{
    uint32 character, animation, flags, table, mode, row, value, active = 0, start;
    sint32 frame;
    FUNCTION_MARKER(0x8003EC64, "SLUS_004.02");
    character = (uint32)r_s16(actor + 24);
    animation = r_u32(actor + 0x54);
    flags = r_u16(animation + 48);
    table = r_u32(0x80096F60 + character * 4);
    mode = flags & 15;
    if (!mode) {
        if (!(flags & 0x1000)) {
            active = 1;
        } else {
            start = r_u8(animation + 45);
            if (!start) {
                active = 1;
            } else {
                frame = r_s16(actor + 0x58);
                if ((sint32)start - 8 < frame)
                    active = frame < r_u8(animation + 46);
            }
        }
        for (row = 0; row < 8; ++row) {
            if (active && (((sint32)(sint16)flags >> (row + 4)) & 1))
                value = 0;
            else
                value = r_u16(table + row * 2);
            w_u16(actor + 0x330 + row * 16, (uint16)value);
        }
    } else if (mode == 2 || mode == 4) {
        for (row = 8; row; --row)
            w_u16(actor + 0x330 + (row - 1) * 16, (uint16)(mode == 2));
    } else {
        for (row = 0; row < 8; ++row) {
            if (mode == 1 || mode == 5) {
                if (!row || row == 3) {
                    value = r_u16(table + row * 2);
                    if (mode == 5)
                        value = (uint32)((sint16)value >> 1);
                } else {
                    value = 1;
                }
            } else if (mode == 6) {
                if (!row)
                    value = r_u16(table) + 60;
                else if (row >= 4)
                    value = 1;
                else
                    value = r_u16(table + row * 2);
            } else {
                value = r_u16(table + row * 2);
                if (mode == 3)
                    value = (uint32)((sint16)value >> 1);
                else if (mode == 7)
                    value += 100;
            }
            w_u16(actor + 0x330 + row * 16, (uint16)value);
        }
    }
    for (row = 0; row < 8; ++row) {
        animation = r_u32(actor + 0x54);
        if (r_u8(animation + 45)) {
            if (row - 1 < 2)
                w_u16(actor + 0x330 + row * 16, 0);
        } else if (r_u8(actor + 0xDB) && r_u8(actor + 0x87)) {
            if (row - 6 < 2)
                w_u16(actor + 0x330 + row * 16, 0);
        } else {
            animation = r_u32(actor + 0x54);
            value = r_u32(animation + 36);
            if ((value & 0x20000) && (row - 6 < 2 || row == 4 || row == 5))
                w_u16(actor + 0x330 + row * 16, 0);
        }
    }
    return 0;
}

uint32 t3_8004acb4(void)
{
    FUNCTION_MARKER(0x8004ACB4, "SLUS_004.02");
    return t3_8004aa90(1);
}

uint32 t3_8004ac24(uint32 update)
{
    FUNCTION_MARKER(0x8004AC24, "SLUS_004.02");
    return t3_8004aa90((uint32)(sint32)(sint16)update);
}

uint32 t3_80041f24(void)
{
    FUNCTION_MARKER(0x80041F24, "SLUS_004.02");
    return t3_800753bc(0, 0x4951, 0);
}

uint32 t3_8004406c(uint32 actor, uint32 opponent)
{
    uint32 record = 0, row, side, animation, value, other, kind, attack, hit;
    sint32 frame;
    FUNCTION_MARKER(0x8004406C, "SLUS_004.02");
    for (row = 0; row < 2; ++row) {
        if (!r_u8(opponent + 0x15D + row * 44))
            record = opponent + 0x13C + row * 44;
    }
    if (!record || r_u8(actor + 0xC2) || r_u8(opponent + 0xC2) || !r_u8(actor + 0xC3) || !r_u8(opponent + 0xC3))
        goto animation_end;
    side = r_u8(opponent + 0x1E);
    if (r_u8(actor + side + 0x83))
        goto animation_end;
    if (r_u16(actor + 0x64) == 0x10F && r_u8(opponent + 0x69) == 12) {
        animation = r_u32(opponent + 0x54);
        frame = r_s16(opponent + 0x58);
        if (frame >= r_u8(animation + 25) && frame <= (sint32)r_u8(animation + 26) - 5)
            goto animation_end;
        if (r_u8(opponent + 0xDB))
            goto animation_end;
    }
    if (r_u32(0x800AFA88) - 7 < 2 && r_u8(opponent + 0xC4))
        goto animation_end;
    kind = r_u8(opponent + 0xB6);
    attack = r_u16(actor + 0x64);
    if (kind == 2)
        attack |= 1;
    if (r_u8(actor + 0xE6) && r_u8(opponent + 0xC5) && r_u8(opponent + r_u8(actor + 0x1E) + 0xD6)) {
        for (row = 0; row < 3; ++row) {
            value = r_u32(opponent + 0xF68 + row * 4);
            w_u32(record + row * 4, value);
        }
        for (row = 0; row < 3; ++row) {
            value = r_u32(actor + 0xF68 + row * 4);
            other = r_u32(opponent + 0xF68 + row * 4);
            w_u16(record + 16 + row * 2, (uint16)((sint32)(value - other) >> 1));
        }
        hit = 1;
        t3_80041f24();
    } else {
        hit = t3_80047d5c(actor, opponent, record, actor, attack);
    }
    if (!hit)
        goto animation_end;
    value = r_u16(opponent + 0x60);
    if (!(attack & value & 7))
        goto animation_end;
    side = r_u8(opponent + 0x1E);
    w_u8(actor + side + 0x83, 1);
    w_u8(actor + 0x88, 1);
    w_u8(actor + 0xD1, 1);
    side = r_u8(opponent + 0x1E);
    w_u8(actor + 0x23, (uint8)side);
    w_u8(opponent + 0x87, 1);
    w_u8(opponent + 0xCE, 1);
    animation = r_u32(actor + 0x54);
    value = r_u8(animation + 44);
    w_u16(opponent + 0xEE, (uint16)value);
    w_u8(record + 0x21, 1);
    value = r_u8(actor + 0x1E);
    w_u8(record + 0x20, (uint8)value);
    value = r_u8(actor + 0xE6);
    w_u8(opponent + 0xE7, (uint8)value);
    if (r_s16(actor + 0xA0) == 2200)
        w_u16(opponent + 0xEE, 30);
    if (r_s16(actor + 0x5E) || r_u8(opponent + 0xDB))
        w_u16(opponent + 0x104, 4);
animation_end:
    animation = r_u32(actor + 0x54);
    value = r_u8(animation + 46);
    frame = r_s16(actor + 0x58);
    if (value == (uint32)frame) {
        value = r_u8(actor + 0x88);
        if (!value)
            w_u8(actor + 0xD2, 1);
        return 1;
    }
    return (uint32)frame;
}

uint32 t3_8002c580(uint32 actor, uint32 side, uint32 held, uint32 edges)
{
    uint32 masks[4] = {128, 16, 64, 32};
    uint32 index, direction = 5, row, table, mapped_held, mapped_edges = 0, value, previous, destination, result;
    FUNCTION_MARKER(0x8002C580, "SLUS_004.02");
    index = r_u32(actor + 0x40C);
    w_u32(actor + 0x40C, index < 59 ? index + 1 : 0);
    if (side < 2) {
        table = 0x80010738 + side * 18;
        for (row = 1; row < 10; ++row) {
            if (r_u16(table) == (held & 0xF000)) {
                direction = row;
                break;
            }
            table += 2;
        }
    }
    mapped_held = (held >> 3) & 16;
    if (held & 16)
        mapped_held |= 32;
    if (held & 64)
        mapped_held |= 64;
    if (held & 32)
        mapped_held |= 128;
    w_u16(actor + 0x406, (uint16)(mapped_held >> 4));
    for (row = 0; row < 4; ++row) {
        if (edges & masks[row]) {
            value = r_u32(actor + 0x4DC + row * 4);
            mapped_edges |= 16 << row;
            w_u32(actor + 0x4DC + row * 4, value + 1);
        }
    }
    index = r_u32(actor + 0x40C);
    w_u8(actor + index + 0x41C, (uint8)(mapped_edges | direction));
    w_u16(actor + 0x404, (uint16)(mapped_edges >> 4));
    index = r_u32(actor + 0x40C);
    w_u16(actor + 0x402, (uint16)(1 << ((direction & 15) + 4)));
    previous = r_u16(actor + index * 2 + 0x462);
    for (row = 0; row < 4; ++row) {
        if (previous & masks[row]) {
            value = r_u32(actor + 0x4DC + row * 4);
            w_u32(actor + 0x4DC + row * 4, value - 1);
        }
    }
    index = r_u32(actor + 0x40C);
    w_u16(actor + index * 2 + 0x462, (uint16)edges);
    if (r_u32(actor + 0x410) >= 10)
        return 0;
    value = r_u32(actor + 0x418);
    result = value - 1;
    if ((sint32)value > 0)
        w_u32(actor + 0x418, result);
    if (!mapped_edges)
        return result;
    if (r_s32(actor + 0x418) > 0) {
        index = r_u32(actor + 0x410);
        destination = actor + index + 0x458;
        value = r_u8(destination) & 0xF0;
        if ((mapped_held & ~mapped_edges) == value) {
            w_u8(destination, (uint8)(mapped_held + direction));
            w_u32(actor + 0x418, 3);
            return 3;
        }
    }
    index = r_u32(actor + 0x410);
    result = index + 1;
    if ((sint32)index >= 9)
        return result;
    w_u32(actor + 0x410, result);
    w_u8(actor + result + 0x458, (uint8)(mapped_held + direction));
    w_u32(actor + 0x418, 3);
    return 3;
}

uint32 t3_8004770c(uint32 segment, uint32 sphere)
{
    uint32 order[3] = {0, 8, 4};
    uint32 axis, offset, first, last, difference, extent, center, radius, point, x0, z0, x1, z1, dx, dy, dz, y0, y1, lower, upper, ratio, numerator, product_x, product_z, endpoint, pass, clip, value, norm, projection, relative_x, relative_z, sphere_x, sphere_z, distance;
    FUNCTION_MARKER(0x8004770C, "SLUS_004.02");
    for (axis = 0; axis < 3; ++axis) {
        offset = order[axis];
        last = r_u32(segment + offset + 12);
        first = r_u32(segment + offset);
        difference = last - first;
        if ((sint32)difference < 0)
            difference = first - last;
        extent = (uint32)((sint32)difference / 2);
        last = r_u32(segment + offset + 12);
        first = r_u32(segment + offset);
        center = (uint32)((sint32)(last + first) / 2);
        radius = (uint32)r_s16(sphere + 12);
        point = r_u32(sphere + offset);
        extent += radius;
        difference = point - center;
        if ((sint32)difference < 0)
            difference = center - point;
        if ((sint32)extent < (sint32)difference)
            return 0;
    }
    first = r_u32(segment);
    last = r_u32(segment + 12);
    dy = r_u32(segment + 16);
    y0 = r_u32(segment + 4);
    dz = r_u32(segment + 20);
    value = r_u32(segment + 8);
    x0 = first;
    z0 = r_u32(segment + 8);
    x1 = r_u32(segment + 12);
    dx = last - first;
    z1 = r_u32(segment + 20);
    dy -= y0;
    y0 = r_u32(segment + 4);
    y1 = r_u32(segment + 16);
    dz -= value;
    if (y0 != y1) {
        radius = (uint32)r_s16(sphere + 12);
        point = r_u32(sphere + 4);
        lower = point - radius;
        upper = point + radius;
        for (pass = 0; pass < 2; ++pass) {
            if (!pass) {
                clip = (sint32)upper < (sint32)((sint32)y1 < (sint32)y0 ? y0 : y1);
                numerator = (upper - y0) << 12;
                endpoint = (sint32)y1 < (sint32)y0 ? 0 : 1;
            } else if ((sint32)y1 < (sint32)y0) {
                value = r_u32(segment + 16);
                clip = (sint32)value < (sint32)lower;
                if (clip)
                    y0 = r_u32(segment + 4);
                numerator = (lower - y0) << 12;
                endpoint = 1;
            } else {
                value = r_u32(segment + 4);
                clip = (sint32)value < (sint32)lower;
                numerator = (lower - value) << 12;
                endpoint = 0;
            }
            if (clip) {
                ratio = (uint32)((sint64)(sint32)numerator / (sint64)(sint32)dy);
                product_x = ratio * dx;
                product_z = ratio * dz;
                first = r_u32(segment);
                first += (uint32)((sint32)product_x / 4096);
                last = r_u32(segment + 8);
                last += (uint32)((sint32)product_z / 4096);
                if (!endpoint) {
                    x0 = first;
                    z0 = last;
                } else {
                    x1 = first;
                    z1 = last;
                }
            }
        }
    }
    dx = x1 - x0;
    dz = z1 - z0;
    sphere_x = r_u32(sphere);
    relative_x = x0 - sphere_x;
    sphere_z = r_u32(sphere + 8);
    relative_z = z0 - sphere_z;
    norm = dx * dx + dz * dz;
    projection = 0 - (dx * relative_x + dz * relative_z);
    if (!norm || (sint32)projection < 0) {
        distance = relative_x * relative_x + relative_z * relative_z;
    } else if ((sint32)norm < (sint32)projection) {
        relative_x = x1 - sphere_x;
        relative_z = z1 - sphere_z;
        distance = relative_x * relative_x + relative_z * relative_z;
    } else {
        numerator = dx * projection;
        relative_x += (uint32)((sint64)(sint32)numerator / (sint64)(sint32)norm);
        numerator = dz * projection;
        relative_z += (uint32)((sint64)(sint32)numerator / (sint64)(sint32)norm);
        distance = relative_x * relative_x + relative_z * relative_z;
    }
    return r_s32(sphere + 16) >= (sint32)distance;
}

uint32 t3_800684b4(void)
{
    uint32 camera[6], table, numerator, denominator, quotient, angle, factor, product, x, y, z, target_x, target_y, value;
    sint32 counter;
    FUNCTION_MARKER(0x800684B4, "SLUS_004.02");
    table = 0x80098508 + (uint32)r_s16(0x800A04C2) * 6;
    if (r_u32(0x800AFA88) == 7)
        table = 0x80098544;
    t3_80063fd4(camera, 0);
    numerator = (uint32)r_s16(0x800A8C76) << 10;
    denominator = (uint32)r_s16(0x800A04C0);
    if (!denominator)
        quotient = (sint32)numerator < 0 ? 1 : 0xFFFFFFFF;
    else
        quotient = (uint32)((sint64)(sint32)numerator / (sint64)(sint32)denominator);
    angle = (quotient + 3072) & 0xFFF;
    factor = (uint32)(r_s16(0x8001E8C4 + angle * 2) + 4096);
    product = (uint32)r_s16(table) * factor;
    target_x = r_u32(0x800A04A8);
    x = target_x + (uint32)((sint32)product / 4096);
    product = (uint32)r_s16(table + 2) * factor;
    target_y = r_u32(0x800A04AC);
    y = target_y + (uint32)((sint32)product / 4096);
    product = (uint32)r_s16(table + 4) * factor;
    z = r_u32(0x800A04B0) + (uint32)((sint32)product / 4096);
    t3_80063eec(camera, x, y, z, target_x, target_y, 0, (sint32)camera[5]);
    t3_80064020(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], 2);
    value = r_u16(0x800A8C76) - 1;
    w_u16(0x800A8C76, (uint16)value);
    counter = (sint16)value;
    return counter >= 17 ? 4096 : (uint32)counter << 8;
}

uint32 t3_8002f074(uint32 actor)
{
    uint32 animation, count, world_height, target_height, initial_height, left, right, sum, radicand, duration, start, difference, next, acceleration, value, result;
    sint32 amount, level, velocity, state;
    FUNCTION_MARKER(0x8002F074, "SLUS_004.02");
    animation = r_u32(actor + 0x1A4);
    w_u16(actor + 0x198, 1);
    count = r_u8(animation + 27);
    if (!count) {
        animation = r_u32(actor + 0x54);
        count = r_u8(animation + 24);
    }
    level = r_s16(actor + 0x94);
    world_height = r_u32(actor + 0xF6C);
    target_height = (uint32)r_s16(actor + 0x96);
    amount = r_s16(actor + 0xE8) + 7 * (level + 1) + 10;
    state = r_s16(actor + 0x90);
    initial_height = 0 - world_height;
    if (state == 5)
        amount += 10;
    if (amount < 0)
        amount = 0;
    if (amount > 100)
        amount = 100;
    left = r_u32(actor + 0x3C8);
    right = r_u32(actor + 0x3E0);
    level = r_s16(actor + 0x94);
    w_u16(actor + 0x98, (uint16)amount);
    sum = left + right;
    value = ((uint32)r_s16(actor + 0xE8) << 2) - (uint32)((sint32)sum >> 3) - ((uint32)level * 7 - 40);
    velocity = (sint32)value;
    if (velocity > 100)
        velocity = 100;
    if (velocity < -100)
        velocity = -100;
    w_u16(actor + 0x9C, (uint16)(0 - (uint32)velocity));
    radicand = (uint32)(velocity * velocity) + (target_height - initial_height) * 12;
    if ((sint32)radicand < 0)
        radicand = 0 - radicand;
    duration = ((uint32)velocity + t3_8002a324(radicand)) / 6;
    if ((sint32)duration <= 0)
        duration = 1;
    start = count - duration + 1;
    if ((sint32)start <= 0)
        start = 1;
    if (!(r_u32(actor + 0x60) & 4) && start != 1) {
        duration = duration - 1 + start;
        start = 1;
    } else if (r_s16(actor + 0x94) == 0) {
        duration += 10;
        start = count - duration + 1;
        if ((sint32)start <= 0)
            start = 1;
    } else {
        left = r_u32(actor + 0x54);
        right = r_u32(actor + 0x1A4);
        left = r_u32(left);
        right = r_u32(right);
        if (left == right) {
            value = (uint32)r_s16(actor + 0x58);
            difference = value - start;
            if ((sint32)difference < 0)
                difference = start - value;
            if ((sint32)difference < 10) {
                next = (uint32)r_s16(actor + 0x58) - 10;
                if ((sint32)next <= 0)
                    next = 1;
                duration += start - next;
                start = next;
            }
        }
    }
    ++duration;
    acceleration = (uint32)((sint32)(duration * (duration * 6)) >> 1);
    value = (uint32)velocity * duration - acceleration + initial_height;
    if ((sint32)value < (sint32)target_height) {
        value = target_height - initial_height + acceleration;
        if (!duration)
            value = (sint32)value < 0 ? 1 : 0xFFFFFFFF;
        else
            value = (uint32)((sint64)(sint32)value / (sint64)(sint32)duration);
        w_u16(actor + 0x9C, (uint16)(0 - value));
    }
    w_u32(actor + 0x110, 0x80019D2C);
    w_u16(actor + 0x10A, 8);
    value = r_u16(actor + 0x2A);
    state = r_s16(actor + 0x90);
    w_u16(actor + 0x198, (uint16)start);
    w_u16(actor + 0x10C, (uint16)(value - 0x8000));
    if (state == 1) {
        animation = r_u32(actor + 0x1A4);
        w_u16(actor + 0x9C, 400);
        w_u16(actor + 0x198, 6);
        amount = (sint32)r_u8(animation + 24) - 6;
        w_u16(actor + 0x106, (uint16)amount);
        if (amount < 0)
            w_u16(actor + 0x106, 0);
        value = r_u16(actor + 0x2A);
        w_u16(actor + 0x108, 40);
        w_u16(actor + 0x10C, (uint16)(value - 0x8000));
    }
    result = 0x80095770 + r_u8(actor + 0x1E) * 4;
    w_u32(result, duration);
    return result;
}

uint32 t3_80070244(uint32 event)
{
    uint32 row, packet, count, flags, value, random_value;
    uint32 x, y, z, angle_x, angle_y, velocity;
    FUNCTION_MARKER(0x80070244, "SLUS_004.02");
    count = r_u8(event + 14);
    flags = r_u8(event + 13);
    w_u8(event + 14, (uint8)(count + 1));
    w_u8(event + 13, (uint8)(flags | 64));
    for (row = 0; row < 2; ++row) {
        packet = event + 80 + row * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE70);
        w_u16(packet + 22, (uint16)value);
        value = r_u16(0x800ADD80);
        w_u16(packet + 14, (uint16)value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    w_u32(event + 8, 0x800252F0);
    for (row = 0; row < 2; ++row) {
        packet = event + 80 + row * 40;
        value = r_u8(0x800252EC);
        w_u8(packet + 12, (uint8)value);
        value = r_u8(0x800252ED);
        w_u8(packet + 13, (uint8)value);
        value = r_u8(0x800252EC);
        w_u8(packet + 20, (uint8)(value + 15));
        value = r_u8(0x800252ED);
        w_u8(packet + 21, (uint8)value);
        value = r_u8(0x800252EC);
        w_u8(packet + 28, (uint8)value);
        value = r_u8(0x800252ED);
        w_u8(packet + 29, (uint8)(value + 15));
        value = r_u8(0x800252EC);
        w_u8(packet + 36, (uint8)(value + 15));
        value = r_u8(0x800252ED);
        w_u8(packet + 37, (uint8)(value + 15));
    }
    random_value = (uint32)randPSX();
    random_value |= (uint32)randPSX() << 16;
    x = r_u32(event + 40);
    y = r_u32(event + 44);
    w_u32(event + 68, ((random_value >> 10) & 31) + 20);
    w_u32(event + 64, 0);
    w_u32(event + 40, x - 127 + (random_value & 0xFF));
    z = r_u32(event + 48);
    w_u32(event + 44, y - 127 + ((random_value >> 2) & 0xFF));
    angle_x = r_u16(event + 16) - 63 + ((random_value >> 6) & 127);
    w_u32(event + 48, z - 127 + ((random_value >> 4) & 0xFF));
    angle_y = r_u16(event + 18);
    w_u16(event + 16, (uint16)angle_x);
    w_u16(event + 18, (uint16)(angle_y - 63 + ((random_value >> 8) & 127)));
    w_u16(event + 24, (uint16)((5 * ((random_value >> 12) & 0x7FF)) / 4));
    t3_8006e120(event + 16);
    velocity = r_u32(event + 28);
    x = r_u32(event + 40);
    x += (uint32)((sint32)velocity >> 13);
    velocity = r_u32(event + 32);
    w_u32(event + 40, x);
    y = r_u32(event + 44);
    y += (uint32)((sint32)velocity >> 13);
    velocity = r_u32(event + 36);
    w_u32(event + 44, y);
    z = r_u32(event + 48);
    value = (uint32)((sint32)velocity >> 13);
    w_u32(event + 48, z + value);
    return value;
}

uint32 t3_80041cb4(uint32 actor)
{
    uint32 animation, sequence, table, random_value, sound, character;
    sint32 type, amount;
    FUNCTION_MARKER(0x80041CB4, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    sequence = r_u32(animation + 0x28);
    if (r_u8(sequence) >= 12) {
        amount = r_s16(actor + 0x5E);
        sound = amount < 13 ? 0x7045 : (amount < 23 ? 0x7046 : (amount < 33 ? 0x7047 : 0x7048));
        character = (uint32)r_s16(actor + 0x12);
    } else {
        type = r_s16(actor + 0x18);
        table = type == 5 || type == 7 || type == 10 || type == 18 ? 0x8001A6FC : 0x8001A710;
        amount = r_s16(actor + 0x5E);
        random_value = (uint32)randPSX();
        character = (uint32)r_s16(actor + 0x12);
        if (amount < 13)
            sound = (uint32)r_s16(table + ((random_value >> 6) & 2));
        else
            sound = (uint32)r_s16(table + (amount < 23 ? 8 : (amount < 33 ? 12 : 16)));
    }
    t3_8003352c(3, character, sound);
    return t3_80040df4(character, sound);
}

uint32 t3_8006fe58(uint32 event)
{
    uint32 row, packet, count, flags, value, random_value;
    sint32 delta_x, delta_y;
    FUNCTION_MARKER(0x8006FE58, "SLUS_004.02");
    count = r_u8(event + 14);
    flags = r_u8(event + 13);
    w_u8(event + 14, (uint8)(count + 1));
    w_u8(event + 13, (uint8)(flags | 64));
    for (row = 0; row < 2; ++row) {
        packet = event + 80 + row * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE70);
        w_u16(packet + 22, (uint16)value);
        value = r_u16(0x800ADD80);
        w_u16(packet + 14, (uint16)value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    for (row = 0; row < 2; ++row) {
        packet = event + 80 + row * 40;
        value = r_u8(0x800252EC);
        w_u8(packet + 12, (uint8)value);
        value = r_u8(0x800252ED);
        w_u8(packet + 13, (uint8)value);
        value = r_u8(0x800252EC);
        w_u8(packet + 20, (uint8)(value + 15));
        value = r_u8(0x800252ED);
        w_u8(packet + 21, (uint8)value);
        value = r_u8(0x800252EC);
        w_u8(packet + 28, (uint8)value);
        value = r_u8(0x800252ED);
        w_u8(packet + 29, (uint8)(value + 15));
        value = r_u8(0x800252EC);
        w_u8(packet + 36, (uint8)(value + 15));
        value = r_u8(0x800252ED);
        w_u8(packet + 37, (uint8)(value + 15));
    }
    w_u32(event + 8, 0x800252F0);
    random_value = (uint32)randPSX();
    random_value |= (uint32)randPSX() << 16;
    delta_x = ((sint32)(random_value & 0x7FF) - 1023) * 2;
    w_u32(event + 68, ((random_value >> 4) & 31) + 10);
    delta_y = ((sint32)((random_value >> 2) & 0x7FF) - 1023) * 2;
    w_u32(event + 72, (random_value >> 6) & 0x1FF);
    value = r_u16(event + 16);
    w_u32(event + 64, 0);
    w_u32(event + 76, ((random_value >> 8) & 0x1FF) + 1664);
    w_u16(event + 16, (uint16)(value + (uint32)(delta_x / 3)));
    value = r_u16(event + 18) + (uint32)(delta_y / 3);
    w_u16(event + 18, (uint16)value);
    return value;
}

uint32 t3_80062b74(void)
{
    uint32 first[6], second[6], row, mask = 0, other;
    sint32 weight, angle;
    FUNCTION_MARKER(0x80062B74, "SLUS_004.02");
    for (row = 0; row < 5; ++row) {
        if (r_s16(0x800A8C78 + row * 2) > 0)
            mask |= 1 << row;
    }
    if (mask == 1)
        t3_80064080(0);
    else if (mask == 2)
        t3_80064080(1);
    else if (mask == 4)
        t3_80064080(2);
    else if (mask == 16)
        t3_80064080(4);
    else if (mask == 3 || mask == 5 || mask == 17) {
        other = mask == 3 ? 1 : (mask == 5 ? 2 : 4);
        t3_80063fd4(first, 0);
        t3_80063fd4(second, other);
        if (mask == 17) {
            angle = (sint16)r_u16(0x800A8C78);
            angle = (angle / 2 - 1024) & 0xFFF;
            weight = (sint16)r_u16(0x8001E8C4 + (uint32)angle * 2);
            weight = weight / 2 + 2048;
        } else {
            weight = r_s16(0x800A8C78);
        }
        t3_80064170(first[0], first[1], first[2], first[3], first[4], first[5], second[0], second[1], second[2], second[3], second[4], second[5], (uint32)weight);
    } else {
        t3_80062a10();
    }
    for (row = 0; row < 5; ++row) {
        other = r_u16(0x800A8C78 + row * 2);
        w_u16(0x800A8C82 + row * 2, (uint16)other);
    }
    return 0;
}

uint32 t3_80041f6c(uint32 actor)
{
    union { uint32 words[2]; SVECTOR vector; } vectors[6];
    VECTOR transformed;
    uint32 animation, row, axis, matrix, output, translation, value, sequence, count, bone, source, destination;
    sint32 frame, type;
    FUNCTION_MARKER(0x80041F6C, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    frame = r_s16(actor + 0x58);
    if (frame >= r_u8(animation + 0x2D) && frame <= r_u8(animation + 0x2E)) {
        t3_8006a3bc(actor);
    } else {
        for (row = 0; row < 6; ++row) {
            vectors[row].words[0] = r_u32(0x8001DC28 + row * 8);
            vectors[row].words[1] = r_u32(0x8001DC2C + row * 8);
        }
        type = r_s16(actor + 0x16);
        if (type == 4) {
            for (row = 0; row < 2; ++row) {
                matrix = actor + 0xB9C;
                output = actor + (row ? 0xE14 : 0xDD0);
                ApplyMatrix((MATRIX *)psx_addr(matrix, 32), &vectors[row + 0].vector, &transformed);
                w_u32(output, (uint32)transformed.vx);
                w_u32(output + 4, (uint32)transformed.vy);
                w_u32(output + 8, (uint32)transformed.vz);
                for (axis = 0; axis < 3; ++axis) {
                    translation = r_u32(matrix + 20 + axis * 4);
                    value = r_u32(output + axis * 4);
                    w_u32(output + axis * 4, value + translation);
                }
            }
            type = r_s16(actor + 0x16);
        }
        if (type == 14) {
            if (r_s16(actor + 0x18) == 20) {
                for (row = 0; row < 2; ++row) {
                    matrix = actor + (row ? 0x97C : 0xBE0);
                    output = actor + (row ? 0xF24 : 0xEE0);
                    ApplyMatrix((MATRIX *)psx_addr(matrix, 32), &vectors[row + 2].vector, &transformed);
                    w_u32(output, (uint32)transformed.vx);
                    w_u32(output + 4, (uint32)transformed.vy);
                    w_u32(output + 8, (uint32)transformed.vz);
                    for (axis = 0; axis < 3; ++axis) {
                        translation = r_u32(matrix + 20 + axis * 4);
                        value = r_u32(output + axis * 4);
                        w_u32(output + axis * 4, value + translation);
                    }
                }
            }
            type = r_s16(actor + 0x16);
        }
        if (type == 19) {
            for (row = 0; row < 2; ++row) {
                matrix = actor + (row ? 0x97C : 0xBE0);
                output = actor + (row ? 0xF24 : 0xE9C);
                ApplyMatrix((MATRIX *)psx_addr(matrix, 32), &vectors[row + 4].vector, &transformed);
                w_u32(output, (uint32)transformed.vx);
                w_u32(output + 4, (uint32)transformed.vy);
                w_u32(output + 8, (uint32)transformed.vz);
                for (axis = 0; axis < 3; ++axis) {
                    translation = r_u32(matrix + 20 + axis * 4);
                    value = r_u32(output + axis * 4);
                    w_u32(output + axis * 4, value + translation);
                }
            }
        }
        animation = r_u32(actor + 0x54);
        count = r_u8(actor + 0xB4);
        sequence = r_u32(animation + 0x28);
        if (count) {
            row = 0;
            do {
                destination = actor + 0x1AC + row * 24;
                bone = r_u8(sequence);
                if (bone == 19 && r_s16(actor + 0x18) == 14)
                    bone = 6;
                if (bone < 24) {
                    source = actor + bone * 68 + 0x908;
                    value = r_u32(source);
                    w_u32(destination + 12, value);
                    value = r_u32(source + 4);
                    w_u32(destination + 16, value);
                    value = r_u32(source + 8);
                    w_u32(destination + 20, value);
                }
                bone = r_u8(sequence + 1);
                if (bone) {
                    if (bone == 19 && r_s16(actor + 0x18) == 14)
                        bone = 6;
                    if (bone < 24) {
                        source = actor + bone * 68 + 0x908;
                        value = r_u32(source);
                        w_u32(destination, value);
                        value = r_u32(source + 4);
                        w_u32(destination + 4, value);
                        value = r_u32(source + 8);
                        w_u32(destination + 8, value);
                    }
                }
                count = r_u8(actor + 0xB4);
                ++row;
                sequence += 2;
            } while (row < count);
        }
    }
    for (row = 0; row < 14; ++row) {
        bone = r_u32(0x80096FB8 + row * 4);
        source = actor + bone * 68 + 0x908;
        destination = actor + 0x20C + row * 20;
        value = r_u32(source);
        w_u32(destination, value);
        value = r_u32(source + 4);
        w_u32(destination + 4, value);
        value = r_u32(source + 8);
        w_u32(destination + 8, value);
        if (row == 8) {
            value = r_u32(destination + 4);
            w_u32(destination + 4, value - 120);
        }
        if (row == 11) {
            value = r_u32(destination + 4);
            w_u32(destination + 4, value - 60);
        }
    }
    for (row = 0; row < 8; ++row) {
        bone = r_u32(0x80096F40 + row * 4);
        source = actor + bone * 68 + 0x908;
        destination = actor + 0x324 + row * 16;
        value = r_u32(source);
        w_u32(destination, value);
        value = r_u32(source + 4);
        w_u32(destination + 4, value);
        value = r_u32(source + 8);
        w_u32(destination + 8, value);
    }
    value = r_u32(actor + 0x20C);
    w_u32(actor + 0x3A4, value);
    value = r_u32(actor + 0x210);
    w_u32(actor + 0x3A8, value);
    value = r_u32(actor + 0x214);
    w_u32(actor + 0x3AC, value);
    return value;
}
