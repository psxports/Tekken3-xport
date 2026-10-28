#include "game_functions.h"
#include "t3_audio.h"

uint32 t3_8003c8cc(uint32 object)
{
    FUNCTION_MARKER(0x8003C8CC, "SLUS_004.02");
    if (r_u32(object + 6172) == 1)
        return r_u32(object + 6264) == 0;
    return 0;
}

uint32 t3_8003ca30(uint32 object)
{
    FUNCTION_MARKER(0x8003CA30, "SLUS_004.02");
    if (r_u32(object + 6172) == 2) {
        uint32 first = r_u32(object + 6264);
        uint32 second = r_u32(object + 6256);
        return first == second;
    }
    return 0;
}

uint32 t3_8003ca5c(uint32 object)
{
    FUNCTION_MARKER(0x8003CA5C, "SLUS_004.02");
    if (r_u32(object + 6172) == 3) {
        uint32 first = r_u32(object + 6264);
        uint32 second = r_u32(object + 6256);
        if (first == second || r_u8(object + 218))
            return 1;
    }
    return 0;
}

uint32 t3_8003ca9c(uint32 object)
{
    FUNCTION_MARKER(0x8003CA9C, "SLUS_004.02");
    if (r_u32(object + 420)) {
        sint32 first = r_s16(object + 88);
        sint32 second = r_s16(object + 408);
        if (first < second && r_u8(object + 139) != 3) {
            sint32 result = second - first + 1;
            return result < 0 ? 0 : (uint32)result;
        }
    }
    return 0;
}

uint32 t3_8003caf0(uint32 object)
{
    uint32 pointer;
    uint32 state;
    uint32 limit;
    FUNCTION_MARKER(0x8003CAF0, "SLUS_004.02");
    pointer = r_u32(object + 84);
    state = r_u32(object + 420);
    limit = r_u8(pointer + 24);
    if (!state) {
        sint32 current = r_s16(object + 88);
        sint32 remaining = (sint32)limit - current;
        if (remaining < 16) {
            sint32 result = remaining + 1;
            if (r_s16(object + 110) <= 0)
                result = current;
            return result < 2 ? 0 : (uint32)result;
        }
    }
    return 0;
}

uint32 t3_80040c00(uint32 value)
{
    FUNCTION_MARKER(0x80040C00, "SLUS_004.02");
    return t3_80075600(value & 0xFFF);
}

uint32 t3_80075f9c(uint32 object, uint32 offset)
{
    uint32 first;
    uint32 second;
    uint32 product;
    FUNCTION_MARKER(0x80075F9C, "SLUS_004.02");
    first = (uint32)r_s16(object + 6254);
    second = (uint32)r_s16(object + 6250);
    product = second * (first << 1);
    return (uint32)((sint32)product / 8192) + offset;
}

uint32 t3_8002a194(uint32 angle)
{
    sint32 value;
    FUNCTION_MARKER(0x8002A194, "SLUS_004.02");
    value = r_s16(0x8001E8C4 + ((angle >> 3) & 0x1FFE));
    if (value >= 4096)
        value = 4095;
    if (value < -4095)
        value = -4095;
    return (uint32)(value * 8);
}

uint32 t3_8002a1dc(uint32 angle)
{
    sint32 value;
    FUNCTION_MARKER(0x8002A1DC, "SLUS_004.02");
    value = r_s16(0x8001F0C4 + ((angle >> 3) & 0x1FFE));
    if (value >= 4096)
        value = 4095;
    if (value < -4095)
        value = -4095;
    return (uint32)(value * 8);
}

uint32 t3_800310cc(uint32 object)
{
    sint32 first;
    sint32 second;
    uint32 result;
    FUNCTION_MARKER(0x800310CC, "SLUS_004.02");
    first = r_s16(object + 410);
    second = r_s16(object + 408);
    result = t3_800310fc(first, second);
    return (uint32)(sint32)(sint16)result;
}

void t3_8003a6d8(uint32 left, uint32 right, uint32 output)
{
    VECTOR first;
    VECTOR second;
    VECTOR result;
    FUNCTION_MARKER(0x8003A6D8, "SLUS_004.02");
    first.vx = (sint32)r_u32(left);
    first.vy = (sint32)r_u32(left + 4);
    first.vz = (sint32)r_u32(left + 8);
    first.pad = 0;
    second.vz = (sint32)r_u32(right + 8);
    second.vx = (sint32)r_u32(right);
    second.vy = (sint32)r_u32(right + 4);
    second.pad = 0;
    OuterProduct12(&first, &second, &result);
    w_u32(output, (uint32)result.vx);
    w_u32(output + 4, (uint32)result.vy);
    w_u32(output + 8, (uint32)result.vz);
}

uint32 t3_800310fc(sint32 first, sint32 second)
{
    FUNCTION_MARKER(0x800310FC, "SLUS_004.02");
    switch ((sint16)first) {
    case 2: case 18: case 19: case 20: case 21:
    case 22: case 25: case 26: case 29: case 44:
        return (uint32)(sint32)(sint16)second;
    default:
        return 0;
    }
}

uint32 t3_800408c0(uint32 value, uint32 bound)
{
    sint32 input = (sint16)value;
    FUNCTION_MARKER(0x800408C0, "SLUS_004.02");
    if (input < 0) {
        bound = 0 - bound;
        if (input < (sint32)bound)
            return (uint32)(sint32)(sint16)bound;
    }
    else if ((sint32)bound < input)
        return (uint32)(sint32)(sint16)bound;
    return (uint32)input;
}

uint32 t3_80041b30(uint32 object)
{
    sint32 index;
    FUNCTION_MARKER(0x80041B30, "SLUS_004.02");
    index = r_s16(object + 18);
    t3_8003352c(3, index, 24650);
    return t3_80040df4(index, 24650);
}

uint32 t3_80069714(uint32 source, uint32 *destination)
{
    uint32 left;
    uint32 right;
    uint32 packed;
    uint32 second;
    uint32 y;
    FUNCTION_MARKER(0x80069714, "SLUS_004.02");
    left = r_u32((source + 3) & ~3);
    right = r_u32(source & ~3);
    packed = (left << ((3 - ((source + 3) & 3)) * 8)) | (right >> ((source & 3) * 8));
    if (source > 0x7FFFFFFA && source <= 0x7FFFFFFF)
        xport_mips_overflow_exception(0x8006971C);
    y = ((packed >> 13) & 0x1FFF) - 4096;
    destination[0] = (packed & 0x1FFF) - 4096;
    left = r_u32((source + 4) & ~3);
    right = r_u32((source + 1) & ~3);
    second = (left << ((3 - ((source + 4) & 3)) * 8)) | (right >> (((source + 1) & 3) * 8));
    destination[1] = y;
    destination[2] = ((second >> 18) & 0x3FFF) - 8192;
    return source + 5;
}

uint32 t3_80044fe4(uint32 destination, uint32 source)
{
    sint32 magnitude;
    uint32 result;
    FUNCTION_MARKER(0x80044FE4, "SLUS_004.02");
    magnitude = r_s16(source + 30);
    if (magnitude < 0)
        magnitude = -magnitude;
    w_u16(destination + 232, (uint16)magnitude);
    w_u8(destination + 212, r_u8(source + 38));
    w_u8(destination + 207, r_u8(source + 34));
    w_u8(destination + 208, r_u8(source + 36));
    w_u8(destination + 211, r_u8(source + 37));
    result = r_u8(source + 32);
    w_u8(destination + 34, (uint8)result);
    return result;
}

uint32 t3_80035dec(void)
{
    uint32 result;
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x80035DEC, "SLUS_004.02");
    result = t3_80079c38(0);
    w_u32(0x800ADEC8, result);
    result = t3_80079c38(1);
    w_u32(0x800ADECC, result);
    result = t3_80079c38(2);
    w_u32(0x800ADED0, result);
    first = r_u32(0x800ADEC8);
    second = r_u32(0x800ADECC);
    w_u32(0x8009BD30, result);
    w_u32(0x8009BD28, first);
    w_u32(0x8009BD2C, second);
    return result;
}

uint32 t3_80069e2c(sint32 value, uint32 *output)
{
    FUNCTION_MARKER(0x80069E2C, "SLUS_004.02");
    if (r_s16(0x800A0512) && r_s16(0x800A0510) == value) {
        *output = r_u32(0x800A0514);
        return 0;
    }
    if (r_s16(0x800A051A) && r_s16(0x800A0518) == value) {
        *output = r_u32(0x800A051C);
        return 1;
    }
    *output = 0;
    return 0xFFFFFFFF;
}

uint32 t3_800784e4(uint32 source, uint32 index)
{
    uint32 destination = 0x800A3628 + index * 416;
    uint32 offset;
    FUNCTION_MARKER(0x800784E4, "SLUS_004.02");
    for (offset = 0; offset < 416; ++offset)
        w_u8(destination + offset, r_u8(source + offset));
    w_u8(destination + 415, 0);
    return destination;
}

uint32 t3_80035080(uint32 object, uint32 count)
{
    uint32 index;
    uint32 value;
    uint32 result = 0xFF000000;
    FUNCTION_MARKER(0x80035080, "SLUS_004.02");
    for (index = 0; index < count; ++index) {
        uint32 address = object + 20 + index * 4;
        value = r_u32(address);
        if (value && !(value & 0xFF000000))
            w_u32(address, value + object);
    }
    value = r_u32(object + 8);
    if (value) {
        result = value + object;
        if (!(value & 0xFF000000))
            w_u32(object + 8, result);
    }
    return result;
}

uint32 t3_80037c58(uint32 index, uint32 object, uint32 source)
{
    uint32 group;
    uint32 result;
    uint32 destination;
    FUNCTION_MARKER(0x80037C58, "SLUS_004.02");
    index -= 18;
    group = (uint32)r_s16(object + 28);
    result = r_u32(0x8009623C + group * 24 + index * 4);
    w_u32(object + index * 4 + 0x136C, result);
    destination = object + index * 8;
    w_u16(destination + 0x129E, r_u16(source + 36));
    w_u16(destination + 0x12A0, r_u16(source + 40));
    result = r_u16(source + 44);
    w_u16(destination + 0x12A2, (uint16)result);
    w_u16(object + index * 2 + 0x1360, 0);
    return result;
}

uint32 t3_8002bc00(uint32 value)
{
    uint32 index;
    FUNCTION_MARKER(0x8002BC00, "SLUS_004.02");
    for (index = 0; index < 3; ++index) {
        t3_8002c050(0x800A9228 + index * 6284);
        w_u32(0x80095470 + index * 4, value);
    }
    return 0;
}

uint32 t3_8006f4f4(void)
{
    uint32 object;
    uint32 next;
    uint32 previous;
    FUNCTION_MARKER(0x8006F4F4, "SLUS_004.02");
    object = r_u32(0x800A0984);
    if (object == 0x800A0980)
        return 0xFFFFFFFF;
    next = r_u32(object + 4);
    w_u32(0x800A0984, next);
    w_u32(next, 0x800A0980);
    w_u32(object + 4, 0x800A0988);
    previous = r_u32(0x800A0988);
    w_u32(object, previous);
    w_u32(0x800A0988, object);
    previous = r_u32(object);
    w_u32(previous + 4, object);
    w_u8(object + 13, 128);
    w_u8(object + 14, 0);
    return object;
}

uint32 t3_80062b04(void)
{
    FUNCTION_MARKER(0x80062B04, "SLUS_004.02");
    if (r_s16(0x800A923E) == 4 && r_u8(r_u32(0x800A927C) + 36) == 83)
        return 0;
    if (r_s16(0x800AAACA) == 4 && r_u8(r_u32(0x800AAB08) + 36) == 83)
        return 1;
    return 0xFFFFFFFF;
}

uint32 t3_80044008(uint32 object)
{
    uint32 index;
    FUNCTION_MARKER(0x80044008, "SLUS_004.02");
    w_u8(object + 209, 0);
    w_u8(object + 210, 0);
    w_u8(object + 206, 0);
    w_u16(object + 238, 0);
    w_u8(object + 231, 0);
    for (index = 0; index < 2; ++index) {
        uint32 destination = object + 316 + index * 44;
        uint32 offset;
        w_u16(destination + 28, 0);
        w_u16(destination + 30, 0);
        for (offset = 32; offset <= 40; ++offset)
            w_u8(destination + offset, 0);
    }
    return 0;
}

uint32 t3_8003ebfc(void)
{
    FUNCTION_MARKER(0x8003EBFC, "SLUS_004.02");
    if (r_u32(0x80096F34)) {
        t3_80031e48(0);
        t3_8007666c();
        t3_8004a83c();
        if (r_u32(0x80096F34) == 2)
            t3_80032170();
    }
    w_u32(0x80096F34, 0);
    return 0x80090000;
}

uint32 t3_8004096c(uint32 object)
{
    uint32 mode;
    uint32 pointer;
    uint32 result;
    sint32 current;
    uint32 limit;
    FUNCTION_MARKER(0x8004096C, "SLUS_004.02");
    mode = r_u8(object + 105);
    if (mode - 1 >= 13)
        return 0x80020000;
    if (mode != 9)
        return 0x800409E0;
    pointer = r_u32(object + 84);
    result = r_u8(pointer + 25);
    if (!result)
        return result;
    current = r_s16(object + 88);
    limit = r_u8(pointer + 25);
    if (current < (sint32)limit)
        return 1;
    result = r_u32(object + 96) | 4;
    w_u32(object + 96, result);
    return result;
}

uint32 t3_80040a94(uint32 destination, uint32 source)
{
    uint32 result;
    FUNCTION_MARKER(0x80040A94, "SLUS_004.02");
    w_u32(destination + 988, r_u32(source + 976));
    w_u32(destination + 992, r_u32(source + 980));
    result = r_u32(source + 984);
    w_u32(destination + 996, result);
    return result;
}

uint32 t3_8004a5a8(uint32 first, uint32 second, const uint32 *third)
{
    FUNCTION_MARKER(0x8004A5A8, "SLUS_004.02");
    return t3_8004a468(first, second, third, 0);
}

uint32 t3_8004b770(void)
{
    FUNCTION_MARKER(0x8004B770, "SLUS_004.02");
    return t3_80088938(0);
}

uint32 t3_80036ad4(const SVECTOR *source, uint32 *output)
{
    sint32 screen;
    sint32 depth;
    FUNCTION_MARKER(0x80036AD4, "SLUS_004.02");
    depth = gte_project_full_depth(source, &screen, 0);
    *output = (uint32)screen;
    return (uint32)(depth >> 2);
}

uint32 t3_80034744(uint32 object, uint32 value)
{
    uint32 index;
    FUNCTION_MARKER(0x80034744, "SLUS_004.02");
    index = t3_800362c4(object);
    return t3_800344a4(object, 3, index, value);
}

uint32 t3_800362c4(uint32 object)
{
    uint32 index;
    FUNCTION_MARKER(0x800362C4, "SLUS_004.02");
    index = (uint32)r_s16(object + 28);
    return index < 55 ? r_u8(0x80095984 + index) : 0;
}

uint32 t3_80075914(uint32 index, uint32 mode)
{
    FUNCTION_MARKER(0x80075914, "SLUS_004.02");
    if (!r_u8(0x800A92EC + index * 6284))
        return 0;
    return (sint16)mode == 4;
}

uint32 t3_800701f0(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x800701F0, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        t3_80070244(object);
    else if (state != 1)
        return 1;
    return t3_80070508(object);
}

uint32 t3_8006fe04(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x8006FE04, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        t3_8006fe58(object);
    else if (state != 1)
        return 1;
    return t3_800700bc(object);
}

uint32 t3_8004ac84(uint32 object)
{
    FUNCTION_MARKER(0x8004AC84, "SLUS_004.02");
    t3_8004a990(object);
    return t3_8004a5c8(object);
}

uint32 t3_8004acfc(uint32 index)
{
    uint32 selected;
    FUNCTION_MARKER(0x8004ACFC, "SLUS_004.02");
    selected = r_u32(0x80097AA4 + index * 4);
    w_u32(0x80097AA0, selected);
    return t3_80075d90(index);
}

uint32 t3_80075d90(uint32 index)
{
    uint32 result;
    FUNCTION_MARKER(0x80075D90, "SLUS_004.02");
    result = r_u32(0x800954A4);
    if (result)
        return result;
    if (r_u32(0x800AFA88) == 7) {
        result = r_u32(0x800AFAC0);
        if (result)
            return result;
    }
    return t3_80028ec0(2, index + 1);
}

uint32 t3_8006f5ac(uint32 object)
{
    FUNCTION_MARKER(0x8006F5AC, "SLUS_004.02");
    w_u8(object + 12, 0);
    w_u8(object + 14, 0);
    w_u8(object + 13, 128);
    return 128;
}

uint32 t3_8006fd64(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x8006FD64, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        t3_8006fdb8(object);
    else if (state != 1)
        return 1;
    return t3_8006fdd0(object);
}

uint32 t3_8006fdb8(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x8006FDB8, "SLUS_004.02");
    state = r_u8(object + 14);
    w_u32(object + 64, 2);
    w_u8(object + 14, (uint8)(state + 1));
    return state + 1;
}

uint32 t3_8006fdd0(uint32 object)
{
    uint32 count;
    FUNCTION_MARKER(0x8006FDD0, "SLUS_004.02");
    count = r_u32(object + 64) - 1;
    w_u32(object + 64, count);
    if ((sint32)count <= 0)
        return t3_8006f558(object);
    return count;
}

uint32 t3_8006f558(uint32 object)
{
    uint32 flags;
    uint32 previous;
    uint32 next;
    uint32 head;
    FUNCTION_MARKER(0x8006F558, "SLUS_004.02");
    flags = r_u8(object + 13) & 128;
    if (!flags)
        return flags;
    previous = r_u32(object);
    next = r_u32(object + 4);
    head = r_u32(0x800A0980);
    w_u32(previous + 4, next);
    previous = r_u32(object);
    w_u32(next, previous);
    w_u32(head + 4, object);
    w_u32(object, head);
    w_u32(object + 4, 0x800A0980);
    w_u32(0x800A0980, object);
    w_u8(object + 13, 0);
    return 0x800A0980;
}

uint32 t3_80031028(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x80031028, "SLUS_004.02");
    state = r_u32(object + 420);
    if (r_u8(state + 45)) {
        sint32 current = r_s16(object + 408);
        uint32 limit = r_u8(state + 46);
        if (current + 1 < (sint32)limit)
            return r_u8(state + 24) >= limit;
    }
    return 0;
}

uint32 t3_8004a468(uint32 first, uint32 second, const uint32 *third, uint32 fourth)
{
    uint32 result;
    uint32 count;
    uint32 list;
    uint32 node;
    uint32 head;
    uint32 base;
    uint32 encoded;
    FUNCTION_MARKER(0x8004A468, "SLUS_004.02");
    result = r_u32(0x8009E830);
    if (!result)
        return result;
    count = r_u32(0x8009E83C) + 1;
    w_u32(0x8009E83C, count);
    if ((sint32)count >= 65)
        return 0x800A0000;
    list = r_u32(0x8009E834);
    count = r_u32(list + 4);
    node = r_u32(0x8009E840);
    w_u32(list + 4, count + 1);
    w_u16(node + 22, (uint16)fourth);
    if (fourth == 0) {
        uint32 tail;
        if (!third && (uint16)second == 0x82) {
            /* This replay operation consumes only the actor slot */
            w_u16(node + 24, (uint16)first);
            w_u16(node + 26, (uint16)second);
        } else {
            w_u32(node, third[0]);
            w_u32(node + 4, third[1]);
            w_u32(node + 8, third[2]);
            w_u16(node + 16, ((const uint16 *)third)[8]);
            w_u16(node + 18, ((const uint16 *)third)[9]);
            tail = ((const uint16 *)third)[10];
            w_u16(node + 24, (uint16)first);
            w_u16(node + 26, (uint16)second);
            w_u16(node + 20, (uint16)tail);
        }
    }
    else if (fourth == 1) {
        w_u32(node, third[0]);
        w_u32(node + 4, third[1]);
        w_u32(node + 8, third[2]);
    }
    head = r_u32(0x8009E840);
    base = r_u32(0x8009E848);
    head += 28;
    w_u32(0x8009E840, head);
    encoded = (head - base) * 0xB6DB6DB7;
    result = ((sint32)encoded >> 2) < 64;
    if (!result)
        w_u32(0x8009E840, base);
    return result;
}

uint32 t3_800753bc(uint32 first, uint32 second, uint32 third)
{
    uint32 index;
    uint32 result = 15;
    uint32 bank;
    uint32 voice;
    uint32 volume;
    FUNCTION_MARKER(0x800753BC, "SLUS_004.02");
    if ((sint32)first >= 2)
        first = 1;
    index = (uint32)r_s16(0x800A923A + first * 6284);
    if (r_s16(0x800A9240 + index * 6284) == 15) {
        result = t3_80075548((sint32)index, second);
        second = result;
    }
    bank = second & 0xC000;
    if (!bank)
        return result;
    if (bank == 0xC000)
        bank = (uint32)r_s8(0x800A8C38 + index);
    else
        bank = (bank >> 14) - 1;
    voice = (second >> 10) & 15;
    if (third & 128)
        voice += 16;
    else if (voice >= 6)
        voice += index;
    voice = (uint32)(sint32)(sint16)voice;
    volume = r_u8(0x800278FC + ((second >> 6) & 15)) >> (third & 31);
    t3_80088938((sint32)voice);
    return t3_800885d8((sint32)voice, (sint16)bank, (second & 48) >> 4, second & 15, (second & 15) + 60, 0, volume, volume);
}

uint32 t3_80031d0c(sint32 mode)
{
    FUNCTION_MARKER(0x80031D0C, "SLUS_004.02");
    if (mode == 1)
        w_u32(0x8009BCB4, 0);
    else if (mode == 0)
        w_u32(0x8009BC98, 23);
    else if (mode == 2) {
        w_u32(0x8009BCA8, 7);
        w_u32(0x8009BCB8, 0);
        w_u32(0x8009BCBC, 0);
        w_u32(0x8009BCC0, 0xFFFFFFFE);
        w_u32(0x8009BCC4, 0);
        w_u32(0x8009BCAC, 0);
        w_u32(0x8009BCC8, 0);
        w_u32(0x8009BCD0, 0);
    }
    if (r_s16(0x800AE204) == 6)
        w_u32(0x8009BCA8, 0);
    return 0x800A0000;
}

uint32 t3_80050f5c(uint32 index)
{
    uint32 offset = index * 2;
    uint32 flags;
    FUNCTION_MARKER(0x80050F5C, "SLUS_004.02");
    if (!(r_u16(0x800ADF08 + offset) & 0x800))
        return 0;
    if (r_u16(0x800AE212))
        return 0;
    if (r_u16(0x800AE1F8 + offset))
        return 0;
    flags = r_u16(0x800ADD68 + offset);
    if (flags & 0x800)
        w_u8(0x800AFA97, flags == 0x80C);
    w_u16(0x800AE1F8 + offset, 1);
    w_u16(0x800ADFBC + offset, 0);
    t3_800511f8(index);
    return 1;
}

uint32 t3_80052114(uint32 side, uint32 resource, uint32 source)
{
    uint32 slot = 0;
    uint32 index;
    uint32 pointer;
    uint32 flags;
    uint32 result;
    uint32 table;
    FUNCTION_MARKER(0x80052114, "SLUS_004.02");
    for (index = 0; index < 64; ++index) {
        uint32 candidate = 0x800B8D60 + index * 40;
        if (!r_u16(candidate)) {
            slot = candidate;
            break;
        }
    }
    if (!slot)
        return 4;
    w_u16(slot, 4);
    pointer = t3_8004b790((sint32)(resource >> 2), resource & 3);
    w_u32(slot + 20, r_u32(pointer) >> 8);
    flags = r_u8(pointer);
    result = flags >> 5;
    if (side)
        result ^= 1;
    w_u16(slot + 36, (uint16)(result & 1));
    table = 0x80098108 + side * 8;
    w_u8(0x800B8D5A + side, (uint8)(flags & 31));
    w_u16(slot + 12, r_u16(table));
    w_u16(slot + 14, r_u16(table + 2));
    w_u16(slot + 16, r_u16(table + 4));
    w_u16(slot + 18, r_u16(table + 6));
    w_u16(slot + 4, r_u16(source));
    w_u16(slot + 6, r_u16(source + 2));
    w_u16(slot + 8, r_u16(source + 4));
    result = r_u16(source + 6);
    w_u16(slot + 10, (uint16)result);
    return result;
}

uint32 t3_80052ad0(uint32 destination, sint32 file)
{
    FUNCTION_MARKER(0x80052AD0, "SLUS_004.02");
    if (file < 10)
        w_u32(0x80098118, 1);
    t3_8006bea8();
    t3_8006bf20(file, destination, 0);
    t3_8006c084();
    while (t3_8006c23c()) {
    }
    if (r_u32(0x80098118)) {
        t3_80079e58();
        FlushCache();
        t3_80079e68();
        w_u32(0x80098118, 0);
    }
    return 0;
}

uint32 t3_800511f8(uint32 index)
{
    FUNCTION_MARKER(0x800511F8, "SLUS_004.02");
    w_u8(0x800B053E, (uint8)index);
    return 0x800B0000;
}

uint32 t3_80052368(sint32 resource, uint32 source)
{
    uint32 slot = 0;
    uint32 index;
    uint32 text;
    uint32 tail;
    uint32 width;
    uint32 x;
    uint32 right;
    uint32 result;
    sint32 direction;
    FUNCTION_MARKER(0x80052368, "SLUS_004.02");
    for (index = 0; index < 64; ++index) {
        uint32 candidate = 0x800B8D60 + index * 40;
        if (!r_u16(candidate)) {
            slot = candidate;
            break;
        }
    }
    if (!slot)
        return 0;
    w_u16(slot, 8);
    text = r_u32(t3_8004efc8(resource));
    w_u32(slot + 28, text);
    w_u16(slot + 4, r_u16(source));
    w_u16(slot + 6, r_u16(source + 2));
    w_u16(slot + 8, r_u16(source + 4));
    tail = r_u16(source + 6);
    w_u32(slot + 24, 0x800226E0);
    w_u16(slot + 16, 8);
    w_u16(slot + 18, 3);
    w_u16(slot + 10, (uint16)tail);
    width = t3_8007a418(text) * 21;
    x = (uint32)r_s16(slot + 4);
    if ((sint32)x < 0) {
        right = 0 - x;
        x = right - width;
    }
    else
        right = x + width;
    if ((sint32)x < 12)
        x = 12;
    if ((sint32)right >= 357)
        x = x + 356 - right;
    direction = r_s16(slot + 8);
    w_u16(slot + 4, (uint16)x);
    result = (uint32)direction;
    if (direction) {
        result = direction > 0 ? 368 - x : 0 - (x + width);
        w_u16(slot + 8, (uint16)result);
    }
    return result;
}

uint32 t3_800519b4(void)
{
    uint32 packet;
    uint32 scale;
    uint32 ordering;
    FUNCTION_MARKER(0x800519B4, "SLUS_004.02");
    packet = r_u32(0x800ADF64);
    scale = r_u32(0x800B8D5C);
    ordering = r_u32(0x800A9218);
    if (!scale) {
        uint32 shade = 0;
        uint32 top_color = r_u32(0x80022668);
        uint32 row;
        for (row = 1; row < 8; ++row) {
            uint32 bottom_color = r_u32(0x80022668 + row * 4);
            uint32 current = r_u16(0x80022658);
            uint32 top_y = ((row - 1) * 32 & 255) << 8;
            uint32 bottom_y = ((row * 32 - 1) & 255) << 8;
            uint32 column;
            for (column = 1; column < 7; ++column) {
                uint32 next = r_u16(0x80022658 + column * 2);
                uint32 left_x = ((column - 1) * 32) & 255;
                uint32 right_x = (column * 32 - 1) & 255;
                uint32 texture = (((shade & 48) << 2) | 0x7D00 | (shade & 15)) << 16;
                uint32 first_xy = left_x | top_y | texture;
                uint32 second_xy = right_x | top_y | 0x000D0000;
                uint32 split = left_x | bottom_y | ((right_x | bottom_y) << 16);
                ++shade;
                packet = t3_8004d830(ordering, packet, 0x2D000000, current | top_color, next | top_color, current | bottom_color, next | bottom_color, first_xy, second_xy, split);
                current = next;
            }
            top_color = bottom_color;
        }
    }
    else
        packet = t3_8004da10(ordering, packet, 0x60000000, 0, 0x01E00170);
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_8006c63c(uint32 mode, uint32 update)
{
    uint32 offset;
    sint32 previous;
    sint32 current;
    uint32 replacement;
    uint32 result;
    FUNCTION_MARKER(0x8006C63C, "SLUS_004.02");
    switch (mode) {
    case 1: offset = 0; break;
    case 2: offset = 2; break;
    case 4: offset = 4; break;
    case 8: offset = 6; break;
    case 16: offset = 8; break;
    default: return 1;
    }
    previous = r_s16(0x800A0898 + offset);
    current = r_s16(0x800A08A8 + offset);
    replacement = r_u16(0x800A08A8 + offset);
    result = previous != current;
    if (update)
        w_u16(0x800A0898 + offset, (uint16)replacement);
    return result;
}

uint32 t3_8006e37c(uint32 source)
{
    uint32 count = 0;
    T3_TIM_IMAGE image;
    FUNCTION_MARKER(0x8006E37C, "SLUS_004.02");
    while (r_u32(source) == 16) {
        if (t3_8007efc8(source) != 0xFFFFFFFF) {
            t3_8007efd8(&image);
            if (image.mode & 8) {
                source += 8;
                t3_8007c650(image.clut_rectangle, image.clut_pixels);
                DrawSync(0);
                t3_8007c650(image.image_rectangle, image.image_pixels);
                DrawSync(0);
                source += r_u32(source) & 0xFFFFFFFC;
                source += r_u32(source) & 0xFFFFFFFC;
            }
        }
        ++count;
    }
    return count;
}

uint32 t3_8006cb18(uint32 archive, uint32 first, uint32 second, uint32 third, uint32 index)
{
    uint32 base = archive + 12;
    uint32 descriptor = base + index * 28;
    uint32 result;
    uint32 count;
    uint32 object;
    uint32 sequence;
    uint32 data;
    uint32 header;
    uint32 other_count;
    FUNCTION_MARKER(0x8006CB18, "SLUS_004.02");
    w_u32(0x800A08B8, base);
    w_u32(0x800A08BC, base);
    w_u32(0x800A08BC, base + r_u32(descriptor));
    result = r_u32(descriptor + 16);
    w_u32(0x800A08B8, base + result);
    count = r_u32(descriptor + 20);
    if ((sint32)count <= 0)
        return result;
    object = r_u32(0x800ADD0C);
    sequence = r_u32(0x800A08C0);
    if (object)
        object += sequence * 36;
    data = r_u32(0x800A08B8);
    header = r_u32(0x800A08BC);
    w_u32(object, index);
    w_u32(object + 4, 0);
    w_u16(object + 28, (uint16)first);
    w_u16(object + 30, (uint16)second);
    w_u16(object + 32, (uint16)third);
    w_u16(object + 24, 0);
    w_u16(object + 22, 0);
    w_u16(object + 20, 0);
    w_u32(object + 8, data);
    w_u32(object + 12, header);
    data = r_u32(descriptor + 4);
    w_u32(object + 4, count);
    w_u32(object + 16, data);
    result = (sint32)data < 257;
    if (!result || (sint32)data < 0)
        return result;
    sequence = r_u32(0x800A08C0);
    other_count = r_u32(0x800ADEC4);
    result = sequence + 1;
    w_u32(0x800A08C0, result);
    w_u32(0x800ADEC4, other_count + 1);
    return result;
}

uint32 t3_800482b0(uint32 mode)
{
    uint32 index;
    FUNCTION_MARKER(0x800482B0, "SLUS_004.02");
    for (index = 0; index < 2; ++index) {
        uint32 packet = 0x8009E738 + index * 16;
        if (mode == 11 && !r_u8(0x800AFAA0)) {
            w_u8(packet + 4, 72);
            w_u8(packet + 5, 104);
            w_u8(packet + 6, 200);
        }
        else {
            w_u8(packet + 4, 0);
            w_u8(packet + 5, 0);
            w_u8(packet + 6, 0);
        }
        w_u16(packet + 12, 368);
        w_u16(packet + 14, 480);
        w_u16(packet + 8, 0);
        w_u16(packet + 10, 0);
        setTile((TILE *)psx_addr(packet, 16));
    }
    return 11;
}

uint32 t3_800483b0(sint32 mode)
{
    uint32 color = 0;
    uint32 index;
    FUNCTION_MARKER(0x800483B0, "SLUS_004.02");
    w_u32(0x8009E778, 0);
    if (mode < 15)
        color = r_u32(0x80097A64 + (uint32)mode * 4);
    color |= 0x60000000;
    for (index = 0; index < 2; ++index) {
        uint32 packet = 0x8009E758 + index * 16;
        w_u32(packet + 4, color);
        w_u32(packet + 8, 0x01180000);
        w_u32(packet + 12, 0x00500170);
        setTile((TILE *)psx_addr(packet, 16));
    }
    return 0;
}

uint32 t3_8006c95c(uint32 mode, uint32 configuration)
{
    uint32 value;
    uint32 step;
    uint32 negative;
    uint32 scaled;
    uint32 half;
    uint32 depth;
    uint32 index;
    uint32 row;
    uint32 column;
    sint32 count;
    sint32 high;
    sint64 product;
    FUNCTION_MARKER(0x8006C95C, "SLUS_004.02");
    w_u32(0x800B0460, mode);
    w_u32(0x800A9188, 0);
    w_u32(0x800ADEC4, 0);
    w_u32(0x800A08C0, 0);
    value = r_u32(0x800A85C4) & 0xFFF;
    w_u32(0x800A9190, value);
    w_u32(0x800A919C, value);
    value = r_u32(configuration + 4);
    step = value * 10;
    negative = 0 - step;
    scaled = value * 140;
    w_u32(0x800ADE00, value);
    w_u32(0x800A8C44, negative * 3);
    product = (sint64)(sint32)scaled * 0x66666667;
    w_u32(0x800A8C50, negative * 3);
    w_u32(0x800A8CF4, value * 20);
    w_u32(0x800A8CF8, value * 30);
    count = r_s32(configuration + 8);
    w_u32(0x800ADE00, step);
    high = (sint32)(product >> 32);
    w_u32(0x800A8D70, (uint32)(high >> 3) - (uint32)((sint32)scaled >> 31));
    if (count >= 36) {
        half = (uint32)((sint32)(negative + (negative >> 31)) >> 1);
        depth = half * 5;
        index = 0;
        for (row = 0; row < 6; ++row) {
            uint32 x;
            step = r_u32(0x800ADE00);
            half = (uint32)((sint32)(step + (step >> 31)) >> 1);
            x = 0 - half * 5;
            for (column = 0; column < 6; ++column) {
                uint32 archive = r_u32(0x800ADD0C);
                if (archive)
                    archive += 1296;
                t3_8006cb18(archive, x, 0, depth, index);
                ++index;
                step = r_u32(0x800ADE00);
                x += step;
            }
            depth += step;
        }
    }
    return t3_800482b0(r_u32(0x800B0460));
}

uint32 t3_80031070(uint32 object)
{
    uint32 mode;
    uint32 state;
    uint32 threshold;
    uint32 limit;
    sint32 current;
    FUNCTION_MARKER(0x80031070, "SLUS_004.02");
    mode = r_u8(object + 139);
    state = r_u32(object + 420);
    if (mode == 3)
        return 0;
    threshold = r_u8(state + 45);
    if (!threshold)
        return 1;
    if (r_u8(state + 24) < threshold)
        return 0;
    limit = r_u8(state + 46);
    current = r_s16(object + 88);
    return (sint32)limit >= current;
}

uint32 t3_80056748(uint32 object)
{
    FUNCTION_MARKER(0x80056748, "SLUS_004.02");
    w_u16(object + 64, 40);
    w_u16(object + 58, 0);
    w_u16(object + 56, 0);
    w_u16(object + 72, 0xFFFF);
    return 0xFFFFFFFF;
}

uint32 t3_800617dc(uint32 value)
{
    uint32 result = (value & 0x100) ? 128 : 0;
    FUNCTION_MARKER(0x800617DC, "SLUS_004.02");
    if (value & 0x200)
        result |= 16;
    if (value & 0x400)
        result |= 64;
    if (value & 0x800)
        result |= 32;
    return (result | r_u16(0x8002301C + (value & 15) * 2)) & 0xFFFF;
}

uint32 t3_80076bd0(uint32 first, const uint32 *second)
{
    FUNCTION_MARKER(0x80076BD0, "SLUS_004.02");
    t3_80076884(3, first, second, 0);
    return t3_8004a5a8(3, first, second);
}

uint32 t3_8002bb88(void)
{
    uint32 mode;
    FUNCTION_MARKER(0x8002BB88, "SLUS_004.02");
    mode = r_u32(0x8009546C);
    return mode == 2 || mode == 5 || mode == 0 || mode == 1;
}

uint32 t3_8002c0e8(uint32 object)
{
    uint32 result;
    FUNCTION_MARKER(0x8002C0E8, "SLUS_004.02");
    result = r_u32(object + 1040);
    if ((sint32)result < 0) {
        w_u32(object + 1040, 0);
        w_u32(object + 1044, 0);
        w_u32(object + 1048, 0);
    }
    return result;
}

uint32 t3_8004b5f4(void)
{
    FUNCTION_MARKER(0x8004B5F4, "SLUS_004.02");
    return t3_800753bc(0, 19648, 0);
}

uint32 t3_8002a384(uint32 *output)
{
    uint32 result;
    FUNCTION_MARKER(0x8002A384, "SLUS_004.02");
    if (r_u16(0x800ADF3C) < 2) {
        w_u32(0x80095484, 0);
        w_u16(0x800A926C, 0);
        w_u16(0x800AAAF8, 0);
    }
    output[0] = r_u32(0x80095484);
    output[1] = (uint32)r_s16(0x800A926C);
    result = (uint32)r_s16(0x800AAAF8);
    output[2] = result;
    return result;
}

uint32 t3_8002a3e0(const uint32 *source)
{
    FUNCTION_MARKER(0x8002A3E0, "SLUS_004.02");
    w_u32(0x80095484, source[0]);
    w_u16(0x800A926C, (uint16)source[1]);
    w_u16(0x800AAAF8, (uint16)source[2]);
    return 0x800A9228;
}

uint32 t3_8002b98c(uint32 index)
{
    uint32 object = 0x800A9228 + index * 6284;
    FUNCTION_MARKER(0x8002B98C, "SLUS_004.02");
    w_u8(0x800AC403, 0);
    w_u8(0x800AAB77, 0);
    w_u8(0x800A92EB, 0);
    w_u8(object + 195, 1);
    return object;
}

uint32 t3_8002bfe8(void)
{
    uint32 count;
    uint32 index;
    FUNCTION_MARKER(0x8002BFE8, "SLUS_004.02");
    count = r_u32(0x800AFA88) == 8 ? 3 : 2;
    for (index = 0; index < count; ++index) {
        uint32 object = 0x800A9228 + index * 6284;
        w_u32(object + 0x1870, 0);
        w_u32(object + 0x1878, 0);
        w_u16(object + 0x186E, 0);
        w_u16(object + 0x1868, 0);
        w_u16(object + 0x186A, 0);
        w_u16(object + 0x186C, 0);
    }
    return 0;
}

uint32 t3_8002c044(uint32 object)
{
    FUNCTION_MARKER(0x8002C044, "SLUS_004.02");
    w_u8(object + 223, 1);
    return 1;
}

uint32 t3_8002c524(uint32 object)
{
    uint32 index;
    uint32 current;
    uint32 previous;
    uint32 result;
    FUNCTION_MARKER(0x8002C524, "SLUS_004.02");
    index = r_u8(object + 30);
    current = r_u16(object + 1032);
    previous = r_u32(0x80095670 + index * 4);
    result = t3_8002c580(object, 0, current, current & (current ^ previous));
    w_u32(0x80095670 + index * 4, current);
    return result;
}

uint32 t3_8002d204(void)
{
    FUNCTION_MARKER(0x8002D204, "SLUS_004.02");
    return 3;
}

uint32 t3_8002d20c(void)
{
    FUNCTION_MARKER(0x8002D20C, "SLUS_004.02");
    return 40;
}

uint32 t3_8002d254(uint32 value)
{
    FUNCTION_MARKER(0x8002D254, "SLUS_004.02");
    return ((value - 36) & 0xFFFF) < 4;
}

uint32 t3_8002eb4c(uint32 object, uint32 value)
{
    uint32 pointer;
    uint32 index;
    sint32 key = (sint16)value;
    FUNCTION_MARKER(0x8002EB4C, "SLUS_004.02");
    pointer = r_u32(object + 84);
    pointer = r_u32(pointer + 40);
    if (!key)
        return 1;
    for (index = 0; index < 4; ++index)
        if ((sint32)r_u8(pointer + index) == key)
            return 1;
    return 0;
}

void t3_80032170(void)
{
    FUNCTION_MARKER(0x80032170, "SLUS_004.02");
}

uint32 t3_8006ba44(void)
{
    sint32 state;
    sint32 remaining;
    uint32 value;
    uint32 result;
    uint32 scaled;
    sint64 product;
    sint32 high;
    CdlATV mix;
    FUNCTION_MARKER(0x8006BA44, "SLUS_004.02");
    state = r_s16(0x800A061C);
    if (!state) {
        if (t3_file_audio_status(NULL) == 1 || xport_fmv_ready()) {
            w_u16(0x800A061C, 1);
            value = (uint32)r_s16(0x800A0690);
            (void)r_u8(0x80097F05);
            w_u16(0x800A068C, (uint16)value);
            if (r_u8(0x80097F04)) {
                mix.val0 = (uint8)value;
                mix.val1 = 0;
                mix.val2 = (uint8)value;
                mix.val3 = 0;
            }
            else {
                scaled = value * 80;
                product = (sint64)(sint32)scaled * 0x51EB851F;
                high = (sint32)(product >> 32);
                value = (uint32)(high >> 5) - (uint32)((sint32)scaled >> 31);
                mix.val3 = (uint8)value;
                mix.val2 = (uint8)value;
                mix.val1 = (uint8)value;
                mix.val0 = (uint8)value;
            }
            t3_80090a88(&mix);
        }
    }
    else if (state == 2) {
        w_u16(0x800A061C, 3);
        value = (uint32)r_s16(0x800A0690);
        w_u16(0x800A068C, (uint16)value);
        if (r_u8(0x80097F04)) {
            mix.val0 = (uint8)value;
            mix.val1 = 0;
            mix.val2 = (uint8)value;
            mix.val3 = 0;
        }
        else {
            scaled = value * 80;
            product = (sint64)(sint32)scaled * 0x51EB851F;
            high = (sint32)(product >> 32);
            value = (uint32)(high >> 5) - (uint32)((sint32)scaled >> 31);
            mix.val3 = (uint8)value;
            mix.val2 = (uint8)value;
            mix.val1 = (uint8)value;
            mix.val0 = (uint8)value;
        }
        t3_80090a88(&mix);
    }
    remaining = r_s16(0x800A0694);
    result = r_u16(0x800A0694) - 1;
    if (remaining <= 0)
        return result;
    w_u16(0x800A0694, (uint16)result);
    if ((sint16)result < 0)
        w_u16(0x800A0694, 0);
    {
        uint32 target = (uint32)r_s16(0x800A068E);
        uint32 start = (uint32)r_s16(0x800A0690);
        uint32 count = (uint32)r_s16(0x800A0694);
        uint32 numerator = (target - start) * count;
        sint32 denominator = r_s16(0x800A0692);
        uint32 quotient;
        if (!denominator)
            quotient = (sint32)numerator < 0 ? 1 : 0xFFFFFFFF;
        else if (numerator == 0x80000000 && denominator == -1)
            quotient = 0x80000000;
        else
            quotient = (uint32)((sint32)numerator / denominator);
        value = quotient + start;
    }
    state = r_s16(0x800A061C);
    w_u16(0x800A068C, (uint16)value);
    if (state == 0 || state == 2) {
        mix.val0 = 0;
        mix.val1 = 0;
        mix.val2 = 0;
        mix.val3 = 0;
    }
    else if (r_u8(0x80097F04)) {
        mix.val0 = (uint8)value;
        mix.val1 = 0;
        mix.val2 = (uint8)value;
        mix.val3 = 0;
    }
    else {
        scaled = value * 80;
        product = (sint64)(sint32)scaled * 0x51EB851F;
        high = (sint32)(product >> 32);
        value = (uint32)(high >> 5) - (uint32)((sint32)scaled >> 31);
        mix.val3 = (uint8)value;
        mix.val2 = (uint8)value;
        mix.val1 = (uint8)value;
        mix.val0 = (uint8)value;
    }
    return t3_80090a88(&mix);
}

uint32 t3_8004db9c(uint32 ordering, uint32 packet, uint32 x, uint32 y, uint32 width, uint32 height, uint32 texture_x, uint32 texture_y, uint32 clut, uint32 mode)
{
    uint32 format;
    uint32 table;
    uint32 boundary_x;
    uint32 boundary_y;
    uint32 remaining_y;
    FUNCTION_MARKER(0x8004DB9C, "SLUS_004.02");
    w_u32(0x1F800300, 0x65000000);
    w_u32(0x1F800304, clut << 16);
    format = (mode & 0x180) >> 7;
    table = 0x80021EDC + format * 4;
    w_u16(0x1F800318, (uint16)(mode & 0xFFFFFFE0));
    boundary_x = r_u16(table);
    w_u16(0x1F80031A, (uint16)format);
    w_u16(0x1F80031C, (uint16)(2 - format));
    w_u16(0x1F80031E, (uint16)boundary_x);
    boundary_y = r_u16(table + 2);
    w_u16(0x1F80030A, (uint16)y);
    w_u16(0x1F80030E, (uint16)texture_y);
    w_u16(0x1F800316, (uint16)height);
    w_u16(0x1F800312, 256);
    w_u16(0x1F800322, (uint16)(boundary_x - 1));
    w_u16(0x1F800320, (uint16)(boundary_y << 8));
    w_u16(0x1F800324, (uint16)((boundary_y - 1) << 8));
    if (texture_y & 255)
        w_u16(0x1F800312, (uint16)(256 - (texture_y & 255)));
    if (!(height & 0xFFFF))
        return packet;
    do {
        uint32 available = r_u16(0x1F800316);
        uint32 extent = r_u16(0x1F800312);
        uint32 shifted;
        uint32 remaining_x;
        if (available < extent)
            w_u16(0x1F800312, (uint16)available);
        shifted = texture_x << (r_u16(0x1F80031C) & 31);
        w_u16(0x1F80030C, (uint16)shifted);
        w_u16(0x1F800314, (uint16)width);
        w_u16(0x1F800310, 256);
        w_u16(0x1F800308, (uint16)x);
        if (shifted & 255)
            w_u16(0x1F800310, (uint16)(256 - (shifted & 255)));
        remaining_x = r_u16(0x1F800314);
        if (remaining_x) {
            do {
                uint32 rows;
                uint32 span;
                uint32 screen_y;
                uint32 tex_y;
                uint32 mask;
                uint32 page;
                uint32 shift;
                uint32 cursor;
                uint32 step;
                uint32 tex_x;
                uint32 screen_x;
                extent = r_u16(0x1F800310);
                if (remaining_x < extent)
                    w_u16(0x1F800310, (uint16)remaining_x);
                rows = r_u16(0x1F800312);
                span = r_u16(0x1F800320);
                screen_y = r_u16(0x1F80030A) << 16;
                tex_y = (r_u16(0x1F80030E) & 255) << 8;
                mask = r_u16(0x1F800324);
                if (tex_y & mask)
                    span -= tex_y & mask;
                while (rows & 0xFFFF) {
                    uint32 columns;
                    uint32 limit;
                    uint32 horizontal_mask;
                    uint32 vertical = (rows & 0xFFFF) << 8;
                    if (vertical < span)
                        span = vertical;
                    screen_x = r_u16(0x1F800308);
                    columns = r_u16(0x1F800310);
                    limit = r_u16(0x1F80031E);
                    tex_x = r_u16(0x1F80030C) & 255;
                    horizontal_mask = r_u16(0x1F800322);
                    if (tex_x & horizontal_mask)
                        limit -= tex_x & horizontal_mask;
                    if (columns & 0xFFFF) {
                        do {
                            uint32 uv = tex_x | tex_y;
                            uint32 xy;
                            uint32 size;
                            uint32 command;
                            uint32 tag;
                            if ((columns & 0xFFFF) < limit)
                                limit = columns & 0xFFFF;
                            tex_x += limit;
                            xy = screen_x | screen_y;
                            screen_x = (screen_x + limit) & 0xFFFF;
                            size = limit | (span << 8);
                            command = r_u32(0x1F800300);
                            columns -= limit;
                            w_u32(packet + 8, xy);
                            w_u32(packet + 16, size);
                            w_u32(packet + 4, command);
                            uv |= r_u32(0x1F800304);
                            w_u32(packet + 12, uv);
                            tag = r_u32(ordering);
                            w_u32(packet, (tag & 0xFFFFFF) | 0x04000000);
                            tag = r_u32(ordering);
                            w_u32(ordering, (packet & 0xFFFFFF) | (tag & 0xFF000000));
                            limit = r_u16(0x1F80031E);
                            packet += 20;
                        } while (columns & 0xFFFF);
                    }
                    tex_y += span;
                    screen_y += span << 8;
                    rows = (((rows & 0xFFFF) << 8) - span) >> 8;
                    span = r_u16(0x1F800320);
                }
                tex_x = r_u16(0x1F80030C);
                shift = r_u16(0x1F80031C);
                tex_y = r_u16(0x1F80030E);
                page = (((tex_x & 0xFF00) >> (shift & 31)) & 0x3FF) >> 6;
                page |= (tex_y & 0x100) >> 4;
                page |= r_u16(0x1F800318);
                SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, page, 0);
                cursor = r_u32(ordering);
                w_u32(packet, (cursor & 0xFFFFFF) | 0x02000000);
                cursor = r_u32(ordering);
                w_u32(ordering, (packet & 0xFFFFFF) | (cursor & 0xFF000000));
                packet += 12;
                step = r_u16(0x1F800310);
                remaining_x = r_u16(0x1F800314) - step;
                w_u16(0x1F800314, (uint16)remaining_x);
                tex_x = r_u16(0x1F80030C);
                screen_x = r_u16(0x1F800308);
                w_u16(0x1F800310, 256);
                w_u16(0x1F80030C, (uint16)(tex_x + step));
                w_u16(0x1F800308, (uint16)(screen_x + step));
            } while (remaining_x);
        }
        {
            uint32 step = r_u16(0x1F800312);
            uint32 tex_y = r_u16(0x1F80030E);
            uint32 remaining = r_u16(0x1F800316);
            uint32 screen_y;
            w_u16(0x1F80030E, (uint16)(tex_y + step));
            screen_y = r_u16(0x1F80030A);
            remaining_y = remaining - step;
            w_u16(0x1F800312, 256);
            w_u16(0x1F800316, (uint16)remaining_y);
            w_u16(0x1F80030A, (uint16)(screen_y + step));
        }
    } while (remaining_y);
    return packet;
}

uint32 t3_80051db8(uint32 object, uint32 x, uint32 y)
{
    uint32 packet;
    uint32 ordering;
    uint32 texture_y;
    uint32 texture_x;
    uint32 left;
    uint32 right;
    uint32 screen_y = y + 6;
    uint32 clut;
    uint32 page;
    uint32 remaining = 212;
    uint32 progress = 20;
    uint32 boundary = 64;
    uint32 index;
    uint32 color;
    uint32 top;
    uint32 middle;
    uint32 bottom;
    uint32 middle_left;
    uint32 middle_right;
    FUNCTION_MARKER(0x80051DB8, "SLUS_004.02");
    packet = r_u32(0x800ADF64);
    texture_y = (r_u16(object + 14) + 20) & 255;
    texture_x = (r_u16(object + 12) * 2) & 127;
    ordering = r_u32(0x800A9218);
    if (r_u16(object + 36)) {
        left = (x + 128) & 0xFFFF;
        right = (x + 2) & 0xFFFF;
    }
    else {
        left = (x + 2) & 0xFFFF;
        right = (x + 128) & 0xFFFF;
    }
    clut = r_u16(object + 18);
    page = r_u16(object + 16);
    do {
        uint32 size;
        uint32 first_y = screen_y << 16;
        uint32 second_y;
        uint32 first_uv_y = (texture_y & 255) << 8;
        uint32 second_uv_y;
        if (boundary < progress + remaining) {
            size = boundary - progress;
            boundary += 64;
        }
        else
            size = remaining;
        screen_y += size;
        second_y = screen_y << 16;
        texture_y += size;
        second_uv_y = (texture_y & 255) << 8;
        packet = t3_8004d830(ordering, packet, 0x2D000000, left | first_y, right | first_y, left | second_y, right | second_y, texture_x | first_uv_y | (page << 16), (texture_x + 125) | first_uv_y | (clut << 16), texture_x | second_uv_y | (((texture_x + 125) | second_uv_y) << 16));
        page += 4;
        remaining -= size;
        progress += size;
    } while (remaining);
    for (index = 0; index < 4; ++index) {
        uint32 table = 0x800226C0 + index * 8;
        uint32 offset_x = (uint32)r_s16(table);
        uint32 offset_y = (uint32)r_s16(table + 2);
        uint32 height = (uint32)r_s16(table + 6);
        uint32 width = r_u16(table + 4);
        uint32 xy = ((offset_x + x) & 0xFFFF) | ((offset_y + y) << 16);
        packet = t3_8004da10(ordering, packet, 0x60E8E8E8, xy, width | (height << 16));
    }
    left = (x + 2) & 0xFFFF;
    top = (y + 6) << 16;
    color = r_u32(object + 20);
    right = (x + 128) & 0xFFFF;
    middle = (y + 112) << 16;
    middle_left = left | middle;
    middle_right = right | middle;
    packet = t3_8004d8b0(ordering, packet, 0x3A000000, 0, color, color, left | top, right | top, middle_left, middle_right);
    bottom = (y + 218) << 16;
    packet = t3_8004d8b0(ordering, packet, color | 0x3A000000, color, 0xC0C0C0, 0xC0C0C0, middle_left, middle_right, left | bottom, right | bottom);
    packet = t3_8004daa8(ordering, packet, 32);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_8004808c(uint32 mode)
{
    sint32 selected = (sint16)mode;
    sint32 kind;
    uint32 active;
    FUNCTION_MARKER(0x8004808C, "SLUS_004.02");
    if (r_s16(0x800AE204) == 6) {
        w_u32(0x8009E728, 0x800D494C);
        t3_bns_07_800d4440();
        return t3_800483b0(selected);
    }
    kind = r_s16(t3_80048230() + 804);
    if (kind == 0) {
        if (r_u32(0x800AFA88) == 8)
            active = r_u32(0x800AFAAC) == 4;
        else
            active = r_u8(0x800AFAA0) != 0;
        if (active) {
            w_u32(0x8009E728, 0x80049848);
            w_u32(0x800ADD94, selected == 13 || selected == 19 ? 128 : 192);
        }
        else
            w_u32(0x8009E728, 0x80048E58);
    }
    else if (kind == 1 || kind == 2 || kind == 3) {
        if (kind == 2)
            w_u32(0x800ADD94, 192);
        w_u32(0x8009E728, 0x80049848);
    }
    else
        return t3_800483b0(selected);
    t3_8004870c(selected);
    return t3_800483b0(selected);
}

uint32 t3_800484f4(void)
{
    uint32 source;
    uint32 destination;
    uint32 row;
    uint32 index;
    static const uint16 coordinates[16] = {
        0, 63, 0x3F00, 0x3F3F, 0x3F00, 0, 0x3F3F, 63,
        0x3F3F, 0x3F00, 63, 0, 63, 0x3F3F, 0, 0x3F00
    };
    FUNCTION_MARKER(0x800484F4, "SLUS_004.02");
    DrawSync(0);
    source = r_u32(0x800AFA88) == 8 ? 0x8001E6C4 : 0x8001E36C;
    destination = r_u32(0x800ADD14);
    t3_80031bfc(source, destination);
    GsMapModelingData(destination + 4);
    for (row = 0; row < 2; ++row) {
        uint32 object = r_u32(0x800ADD14);
        if (object)
            object += row * 5200 + 1508;
        for (index = 0; index < 100; ++index) {
            POLY_GT4 *packet = (POLY_GT4 *)psx_addr(object, 52);
            uint32 vertex;
            setPolyGT4(packet);
            setSemiTrans(packet, 0);
            setShadeTex(packet, 0);
            for (vertex = 0; vertex < 4; ++vertex) {
                uint32 color = object + 4 + vertex * 12;
                w_u8(color, 128);
                w_u8(color + 1, 128);
                w_u8(color + 2, 128);
            }
            object += 52;
        }
    }
    if (r_u32(0x800AFA88) == 8) {
        w_u32(0x8009E780, 2500);
        w_u32(0x8009E824, 10);
        w_u32(0x8009E828, 3);
        w_u32(0x8009E82C, 44);
    }
    else {
        w_u32(0x8009E780, 1800);
        w_u32(0x8009E824, 10);
        w_u32(0x8009E828, 10);
        w_u32(0x8009E82C, 121);
    }
    for (index = 0; index < 16; ++index)
        w_u16(0x8009E800 + index * 2, coordinates[index]);
    return 0x8009E800;
}

uint32 t3_80069aa8(uint32 mode)
{
    uint32 mapping[2];
    sint32 actors[2] = { -1, -1 };
    uint32 dirty[2] = { 0, 0 };
    sint32 keys[2];
    uint32 index;
    uint32 base;
    FUNCTION_MARKER(0x80069AA8, "SLUS_004.02");
    if (r_s16(0x800A0512) || r_s16(0x800A051A)) {
        w_u16(0x800A0510, 0xFFFF);
        w_u16(0x800A0518, 0xFFFF);
        w_u16(0x800A0520, 0xFFFF);
    }
    if (r_s16(0x800A0520) != 99 && !mode) {
        base = r_u32(0x800ADE80);
        w_u16(0x800A0522, 1);
        w_u32(0x800A0524, base ? base + 0xB4C94 : 0);
    }
    keys[0] = r_s16(0x800A0510);
    keys[1] = r_s16(0x800A0518);
    for (index = 0; index < 2; ++index) {
        uint32 object = 0x800A9228 + index * 6284;
        sint32 key = r_s16(object + 22);
        uint32 slot;
        if (key == keys[0])
            slot = 0;
        else if (key == keys[1])
            slot = 1;
        else {
            sint32 other = r_s16(0x800A923E + (1 - index) * 6284);
            if (other == keys[0])
                slot = 1;
            else if (other == keys[1])
                slot = 0;
            else
                slot = index;
        }
        mapping[index] = slot;
        actors[slot] = (sint32)index;
        keys[slot] = r_s16(object + 22);
    }
    for (index = 0; index < 2; ++index) {
        sint32 key = r_s16(0x800A923E + index * 6284);
        sint32 previous = r_s16(0x800A0510 + mapping[index] * 8);
        if (key != previous)
            dirty[mapping[index]] = 1;
    }
    if (actors[0] >= 0 && actors[0] < 2) {
        w_u16(0x800A0512, (uint16)dirty[0]);
        if (dirty[0])
            w_u16(0x800A0510, r_u16(0x800A923E + (uint32)actors[0] * 6284));
        w_u32(0x800A0514, r_u32(0x800ADE80));
    }
    if (actors[1] >= 0 && actors[1] < 2) {
        uint32 offset;
        w_u16(0x800A051A, (uint16)dirty[1]);
        if (dirty[1])
            w_u16(0x800A0518, r_u16(0x800A923E + (uint32)actors[1] * 6284));
        offset = r_s16(0x800A0518) == 3 ? 0x5233C : 0x62958;
        base = r_u32(0x800ADE80);
        w_u32(0x800A051C, base ? base + offset : 0);
    }
    if (dirty[0])
        return r_u32(0x800ADE80);
    if (!dirty[1])
        return 0;
    base = r_u32(0x800ADE80);
    return base ? base + 0x62958 : 0;
}

uint32 t3_80036600(uint32 archive)
{
    uint32 primary;
    uint32 secondary;
    uint32 extra;
    uint32 mode;
    uint32 source;
    uint32 size;
    uint32 offset;
    FUNCTION_MARKER(0x80036600, "SLUS_004.02");
    primary = t3_8004807c();
    mode = r_u32(0x800AFA88);
    if (mode == 7) {
        secondary = t3_bns_03_800b57e0();
        archive = t3_800561a0();
        t3_80052ad0(archive, r_u16(0x800ADC84) + 34);
        source = t3_80031cbc(1, archive);
        size = t3_80031cf0(1, archive);
        for (offset = 0; (sint32)offset < (sint32)size; offset += 4)
            w_u32(primary + offset, r_u32(source + offset));
        source = t3_80031cbc(2, archive);
        size = t3_80031cf0(2, archive);
        for (offset = 0; (sint32)offset < (sint32)size; offset += 4)
            w_u32(secondary + offset, r_u32(source + offset));
        source = t3_80031cbc(0, archive);
        t3_bns_03_800b5800(r_u16(0x800ADC84), secondary, source);
    }
    else if (mode == 8) {
        extra = t3_bns_02_800b1108();
        secondary = t3_bns_02_800b4584();
        if (!archive)
            archive = t3_80069df4();
        t3_80052ad0(archive, r_u16(0x800ADC84) + 34);
        source = t3_80031cbc(1, archive);
        size = t3_80031cf0(1, archive);
        for (offset = 0; (sint32)offset < (sint32)size; offset += 4)
            w_u32(primary + offset, r_u32(source + offset));
        source = t3_80031cbc(2, archive);
        size = t3_80031cf0(2, archive);
        for (offset = 0; (sint32)offset < (sint32)size; offset += 4)
            w_u32(secondary + offset, r_u32(source + offset));
        if (extra) {
            source = t3_80031cbc(3, archive);
            size = t3_80031cf0(3, archive);
            for (offset = 0; (sint32)offset < (sint32)size; offset += 4)
                w_u32(extra + offset, r_u32(source + offset));
        }
        source = t3_80031cbc(0, archive);
        t3_bns_02_800b45a4(source);
        t3_bns_02_800b4660();
    }
    else {
        secondary = t3_8006c800(0);
        if (t3_8006c63c(16, 0) && r_s16(0x800AE204) != 6) {
            if (!archive)
                archive = t3_80069df4();
            t3_80052ad0(archive, r_u16(0x800ADC84) + 34);
            source = t3_80031cbc(0, archive);
            t3_8006e37c(source);
            source = t3_80031cbc(1, archive);
            size = t3_80031cf0(1, archive);
            for (offset = 0; (sint32)offset < (sint32)size; offset += 4)
                w_u32(primary + offset, r_u32(source + offset));
            t3_80052ad0(secondary, r_u16(0x800ADC84) + 54);
        }
        t3_8006c95c(r_u16(0x800ADC84), secondary);
        t3_8006c63c(16, 1);
    }
    return t3_8004808c((uint32)r_s16(0x800ADC84));
}

uint32 t3_8007641c(uint32 first, uint32 second)
{
    uint32 index;
    uint32 row;
    uint32 key;
    FUNCTION_MARKER(0x8007641C, "SLUS_004.02");
    if (r_s16(0x800AE204) == 6) {
        w_u32(0x800A35C8, 0x80027986);
        w_u32(0x800A35D8, 0x80027968);
    }
    else {
        key = (uint32)r_s16(first + 24);
        w_u32(0x800A35C8, 0x80027950 + key * 6);
        key = (uint32)r_s16(second + 24);
        w_u32(0x800A35D8, 0x80027950 + key * 6);
    }
    if (r_u32(0x800AFA88) == 8) {
        uint32 object = t3_bns_02_800b298c(0);
        key = (uint32)r_s16(object + 24);
        w_u32(0x800A35E8, 0x80027950 + key * 6);
    }
    else
        w_u32(0x800A35E8, r_u32(0x800A35D8));
    for (index = 0; index < 4; ++index)
        w_u32(0x800A35F8 + index * 4, 0x800279D4 + index * 6);
    w_u32(0x800A3600, 0x800279DA);
    w_u16(0x800A35B8, 0);
    for (row = 0; row < 2; ++row) {
        for (index = 0; index < 3; ++index) {
            uint32 address = 0x800A34C8 + row * 120 + index * 40;
            POLY_FT4 *packet;
            w_u16(0x800A340A + index * 64, 0);
            packet = (POLY_FT4 *)psx_addr(address, 40);
            setPolyFT4(packet);
            setSemiTrans(packet, 1);
            setShadeTex(packet, 0);
            w_u8(address + 4, 128);
            w_u8(address + 5, 128);
            w_u8(address + 6, 128);
        }
    }
    return 0;
}

uint32 t3_80074f44(void)
{
    SpuReverbAttr reverb;
    uint32 key;
    uint32 first_byte;
    uint32 second_key;
    sint32 first;
    sint32 second;
    sint32 previous;
    uint32 missing = 0;
    uint32 toggle;
    uint32 bank;
    uint32 buffer;
    FUNCTION_MARKER(0x80074F44, "SLUS_004.02");
    if (t3_8008c818() == 1) {
        reverb.mask = 7;
        reverb.mode = 1;
        reverb.depth.left = 0;
        reverb.depth.right = 0;
        SpuSetReverb(0);
        SpuSetReverbDepth(&reverb);
        SpuReserveReverbWorkArea(0);
    }
    key = (uint32)r_s16(0x800A9242);
    first_byte = r_u8(0x80027890 + key);
    second_key = (uint32)r_s16(0x800AAACE);
    w_u8(0x800ADC70, (uint8)first_byte);
    w_u8(0x800ADC71, r_u8(0x80027890 + second_key));
    first = (sint8)first_byte;
    if (r_s8(0x800A9226) == first)
        w_u8(0x800A8C38, 2);
    else if (r_s8(0x800A9227) == first)
        w_u8(0x800A8C38, 3);
    else
        missing = 1;
    previous = r_s8(0x800A9226);
    second = r_s8(0x800ADC71);
    if (previous == second)
        w_u8(0x800A8C39, 2);
    else if (r_s8(0x800A9227) == second)
        w_u8(0x800A8C39, 3);
    else
        missing |= 2;
    switch (missing) {
    case 0:
        w_u32(0x800ADFF8, 0xFFFFFFFF);
        w_u32(0x800AE004, 0xFFFFFFFF);
        w_u32(0x800ADFFC, 0x800A25E0);
        w_u32(0x800AE008, 0x800A25E0);
        return 0x800ADFF8;
    case 1:
        toggle = (uint32)r_s8(0x800A8C39);
        w_u32(0x800AE008, 0x800A25E0);
        first = r_s8(0x800ADC70);
        toggle = (toggle + 1) & 1;
        bank = toggle + 2;
        w_u32(0x800ADFF8, (uint32)first);
        buffer = 0x800A09A0 + toggle * 3616;
        w_u8(0x800A8C38, (uint8)bank);
        w_u32(0x800ADFFC, buffer);
        w_u32(0x800AE000, bank);
        return buffer;
    case 2:
        toggle = (uint32)r_s8(0x800A8C38);
        w_u32(0x800ADFFC, 0x800A25E0);
        second = r_s8(0x800ADC71);
        toggle = (toggle + 1) & 1;
        bank = toggle + 2;
        w_u8(0x800A8C39, (uint8)bank);
        w_u32(0x800AE004, (uint32)second);
        buffer = 0x800A09A0 + toggle * 3616;
        w_u32(0x800AE008, buffer);
        w_u32(0x800AE00C, bank);
        return buffer;
    default:
        first = r_s8(0x800ADC70);
        w_u8(0x800A8C38, 2);
        w_u32(0x800ADFFC, 0x800A09A0);
        w_u32(0x800AE000, 2);
        w_u32(0x800ADFF8, (uint32)first);
        second = r_s8(0x800ADC71);
        if (first == second) {
            w_u8(0x800A8C39, 2);
            w_u32(0x800AE008, 0x800A25E0);
            return 0x800A25E0;
        }
        w_u8(0x800A8C39, 3);
        w_u32(0x800AE004, (uint32)second);
        w_u32(0x800AE008, 0x800A17C0);
        w_u32(0x800AE00C, 3);
        return 0x800A17C0;
    }
}

uint32 t3_8006e560(void)
{
    uint32 index;
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x8006E560, "SLUS_004.02");
    w_u32(0x800ADE70, 63);
    w_u32(0x800ADE74, 31);
    for (index = 0; index < 16; ++index) {
        uint32 value = (16 + index) & 63;
        w_u32(0x800A8D00 + index * 4, value | 0x7F40);
        w_u32(0x800B04A8 + index * 4, value | 0x7F80);
    }
    w_u32(0x800ADD80, 0x7DC6);
    w_u32(0x800ADD84, 0x7DC7);
    w_u32(0x800ADD88, 0x7DC8);
    w_u32(0x800ADD8C, 0x7DC9);
    first = r_u32(0x800A8D3C);
    second = r_u32(0x800B04E4);
    w_u32(0x800ADD90, 0x7DCA);
    w_u32(0x800A8D40, first);
    w_u32(0x800B04E8, second);
    return 0x7DCA;
}
