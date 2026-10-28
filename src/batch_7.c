#include "game_functions.h"
#include <string.h>

uint32 t3_8003ad10(uint32 pose, MATRIX *matrices, uint32 *written_mask)
{
    uint32 row, index, bone;
    uint32 written = 0, result;
    uint32 words[5];
    uint32 first, value;
    sint32 x, y, z;
    FUNCTION_MARKER(0x8003AD10, "SLUS_004.02");
    for (row = 0; row < 10; ++row) {
        x = r_s16(pose + 38 + row * 6);
        y = r_s16(pose + 40 + row * 6);
        z = r_s16(pose + 42 + row * 6);
        bone = r_u8(0x8001A330 + row);
        t3_8003a338(x, y, z, matrices + bone);
        written |= (uint32)1 << bone;
    }
    for (index = 0; index < 5; ++index)
        words[index] = r_u32(0x80096EAC + index * 4);
    for (index = 0; index < 5; ++index)
        w_u32(0x1F800300 + index * 4, words[index]);
    MulMatrix0(matrices + 2, (MATRIX *)psx_addr(0x1F800300, 32), matrices + 2);
    MulMatrix0(matrices + 3, (MATRIX *)psx_addr(0x1F800300, 32), matrices + 3);
    MulMatrix0(matrices + 7, (MATRIX *)psx_addr(0x1F800300, 32), matrices + 7);
    memcpy(&value, (const uint8 *)matrices + 4, sizeof(value));
    memcpy(&first, matrices, sizeof(first));
    w_u32(0x1F800304, value);
    memcpy(&value, (const uint8 *)matrices + 8, sizeof(value));
    w_u32(0x1F800308, value);
    memcpy(&value, (const uint8 *)matrices + 12, sizeof(value));
    w_u32(0x1F80030C, value);
    memcpy(&value, (const uint8 *)matrices + 16, sizeof(value));
    w_u32(0x1F800300, first);
    w_u32(0x1F800310, value);
    value = (uint32)r_s16(pose);
    w_u32(0x1F800314, value);
    value = (uint32)r_s16(pose + 2);
    w_u32(0x1F800318, value);
    value = (uint32)r_s16(pose + 4);
    w_u32(0x1F80031C, value);
    t3_8003b5b8(0x1F800300, matrices + 1, 0x1F800380, 0x80096ECC);
    t3_8003b5b8(0x1F800380, matrices + 3, 0x1F8003A0, 0x80096ED8);
    t3_8003b5b8(0x1F800380, matrices + 7, 0x1F8003C0, 0x80096EE4);
    t3_8003b5b8(0x1F800300, matrices + 11, 0x1F8003E0, 0x80096EF0);
    t3_8003b9b4(0x1F8003A0);
    t3_8003ba28(pose + 6, 0x1F8003B4, 0x80096EFC, 0x1F800340);
    t3_8003b9b4(0x1F8003C0);
    t3_8003ba28(pose + 12, 0x1F8003D4, 0x80096F08, 0x1F800350);
    t3_8003b9b4(0x1F8003E0);
    t3_8003ba28(pose + 18, 0x1F8003F4, 0x80096F14, 0x1F800360);
    t3_8003b9b4(0x1F8003E0);
    t3_8003ba28(pose + 24, 0x1F8003F4, 0x80096F20, 0x1F800370);
    value = (uint32)r_s16(pose + 30);
    result = t3_8003bca8(0x1F800340, value, matrices + 4, 1);
    if (result != 0xFFFFFFFF)
        written |= (uint32)1 << 4;
    if (result == 0)
        written |= (uint32)1 << 5;
    value = (uint32)r_s16(pose + 32);
    result = t3_8003bca8(0x1F800350, value, matrices + 8, 1);
    if (result != 0xFFFFFFFF)
        written |= (uint32)1 << 8;
    if (result == 0)
        written |= (uint32)1 << 9;
    value = (uint32)r_s16(pose + 34);
    result = t3_8003bca8(0x1F800360, value, matrices + 12, 0);
    if (result != 0xFFFFFFFF)
        written |= (uint32)1 << 12;
    if (result == 0)
        written |= (uint32)1 << 13;
    value = (uint32)r_s16(pose + 36);
    result = t3_8003bca8(0x1F800370, value, matrices + 15, 0);
    if (result != 0xFFFFFFFF)
        written |= (uint32)1 << 15;
    if (result == 0)
        written |= (uint32)1 << 16;
    if (written_mask)
        *written_mask = written;
    return result;
}

uint32 t3_8003c8f0(uint32 actor)
{
    uint32 previous;
    uint32 animation;
    uint32 length;
    uint32 start;
    sint32 frame;
    sint32 remaining;
    sint32 span;
    FUNCTION_MARKER(0x8003C8F0, "SLUS_004.02");
    previous = r_u32(actor + 0x1824);
    animation = r_u32(actor + 0x54);
    if (previous == animation && r_u8(actor + 0x8B) != 3)
        return 0;
    if (!r_u8(actor + 0xDA))
        return 0;
    length = r_u8(animation + 24);
    frame = r_s16(actor + 0x58);
    remaining = (sint32)length - frame;
    if (remaining <= 0)
        return 0;
    start = r_u8(animation + 45);
    if (start && frame < (sint32)start) {
        span = (sint32)start - frame;
        if (span >= 17)
            span = 16;
    } else {
        span = r_s16(actor + 0x74) < 0 ? 16 : 4;
    }
    if (remaining < span)
        span = remaining;
    return span < 2 ? 0 : (uint32)span;
}

uint32 t3_8003c9b4(uint32 actor)
{
    uint32 animation;
    FUNCTION_MARKER(0x8003C9B4, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    if ((r_u32(animation + 36) >> 8) & 2)
        return 1;
    if (r_u8(actor + 0xBB))
        return 1;
    if (r_u16(actor + 0x19E))
        return 1;
    if (r_s16(actor + 0x94))
        return 1;
    return r_u8(actor + 0xDB) && r_u8(actor + 0x87) ? 1 : 0;
}

uint32 t3_8003a72c(uint32 delta, uint32 base, uint32 scale, uint32 output)
{
    uint32 first_xy;
    uint32 first_z;
    uint32 second_x;
    uint32 second_yz;
    uint32 third_xy;
    uint32 third_z;
    uint32 reference[3];
    uint32 result[9];
    uint32 row;
    uint32 value;
    sint64 sum;
    FUNCTION_MARKER(0x8003A72C, "SLUS_004.02");
    first_xy = r_u32(delta);
    first_z = (uint32)r_s16(delta + 4);
    xport_gte_write_data(8, scale);
    xport_gte_write_data(9, first_xy);
    xport_gte_write_data(10, first_xy >> 16);
    xport_gte_write_data(11, first_z);
    second_x = (uint32)r_s16(delta + 6);
    xport_gte_execute(0x4B98003D);
    second_yz = r_u32(delta + 8);
    value = r_u32(base);
    reference[0] = value;
    reference[2] = (uint32)r_s16(base + 4);
    reference[1] = value >> 16;
    result[0] = xport_gte_read_data(9);
    result[1] = xport_gte_read_data(10);
    result[2] = xport_gte_read_data(11);
    for (row = 0; row < 3; ++row) {
        sum = (sint64)(sint32)result[row] + (sint32)reference[row];
        if (sum > 0x7FFFFFFF || sum < -2147483648LL) {
            xport_mips_overflow_exception(0x8003A770 + row * 4);
            return value;
        }
        result[row] = (uint32)sum;
    }
    xport_gte_write_data(9, second_x);
    xport_gte_write_data(10, second_yz);
    xport_gte_write_data(11, second_yz >> 16);
    third_xy = r_u32(delta + 12);
    xport_gte_execute(0x4B98003D);
    third_z = (uint32)r_s16(delta + 16);
    value = (uint32)r_s16(base + 6);
    reference[0] = value;
    reference[1] = r_u32(base + 8);
    reference[2] = reference[1] >> 16;
    result[3] = xport_gte_read_data(9);
    result[4] = xport_gte_read_data(10);
    result[5] = xport_gte_read_data(11);
    xport_gte_write_data(9, third_xy);
    xport_gte_write_data(10, third_xy >> 16);
    xport_gte_write_data(11, third_z);
    sum = (sint64)(sint32)result[3] + (sint32)reference[0];
    if (sum > 0x7FFFFFFF || sum < -2147483648LL) {
        xport_mips_overflow_exception(0x8003A7BC);
        return value;
    }
    result[3] = (uint32)sum;
    xport_gte_execute(0x4B98003D);
    for (row = 1; row < 3; ++row) {
        sum = (sint64)(sint32)result[3 + row] + (sint32)reference[row];
        if (sum > 0x7FFFFFFF || sum < -2147483648LL) {
            xport_mips_overflow_exception(0x8003A7C0 + row * 4);
            return value;
        }
        result[3 + row] = (uint32)sum;
    }
    value = r_u32(base + 12);
    reference[0] = value;
    reference[2] = (uint32)r_s16(base + 16);
    reference[1] = value >> 16;
    result[6] = xport_gte_read_data(9);
    result[7] = xport_gte_read_data(10);
    result[8] = xport_gte_read_data(11);
    for (row = 0; row < 3; ++row) {
        sum = (sint64)(sint32)result[6 + row] + (sint32)reference[row];
        if (sum > 0x7FFFFFFF || sum < -2147483648LL) {
            xport_mips_overflow_exception(0x8003A7E4 + row * 4);
            return value;
        }
        result[6 + row] = (uint32)sum;
    }
    for (row = 0; row < 9; ++row)
        w_u16(output + row * 2, (uint16)result[row]);
    return value;
}

uint32 t3_8003dcc8(void)
{
    uint32 mode;
    uint32 maximum;
    uint32 round;
    uint32 stage;
    sint32 timer;
    FUNCTION_MARKER(0x8003DCC8, "SLUS_004.02");
    mode = r_u32(0x800AFA88);
    if (mode == 2) {
        t3_8004ce74(0x8001A420, 8, 2, 63, 112);
    } else if (mode == 4) {
        t3_8004ce74(0x8001A434, 8, 2, 19, 112);
    } else if (mode == 5) {
        t3_8004ce74(0x8001A44C, 8, 2, 41, 112);
    } else if (mode == 8) {
        stage = r_u32(0x800AFAAC);
        if (stage < 4) {
            stage = r_u32(0x800AFAAC);
            t3_8004ce74(0x8001A464, 8, 2, 107, 112, stage + 1);
        } else {
            t3_8004ce74(0x8001A478, 8, 2, 63, 112);
        }
    } else {
        maximum = r_u16(0x800ADF3C);
        round = r_u32(0x80095484);
        if ((sint32)round >= (sint32)maximum) {
            t3_8004ce74(0x8001A40C, 8, 2, 63, 112);
        } else {
            timer = r_s32(0x80095460);
            if ((round == 1 && timer < 61) || (round != 1 && timer <= 0))
                goto ready;
            round = r_u32(0x80095484);
            t3_8004ce74(0x8001A3F8, 8, 2, 107, 112, round);
        }
    }
ready:
    round = r_u32(0x80095484);
    timer = r_s32(0x80095460);
    if (timer < (round == 1 ? 106 : 46))
        return 228;
    return t3_8004ce74(0x8001A48C, 9, 2, 118, 228);
}

uint32 t3_8003da8c(void)
{
    uint32 mode;
    uint32 round;
    uint32 maximum;
    uint32 sound;
    uint32 record;
    uint32 flag;
    uint32 threshold;
    sint32 timer;
    FUNCTION_MARKER(0x8003DA8C, "SLUS_004.02");
    mode = r_u32(0x800AFA88);
    if (mode == 5) {
        round = r_u32(0x80095484);
        timer = r_s32(0x80095460);
        if (timer == (round == 1 ? 61 : 1)) {
            flag = r_u16(0x800AE1F8);
            t3_800757a8(flag == 0);
        }
    } else if (mode == 8) {
        round = r_u32(0x80095484);
        timer = r_s32(0x80095460);
        if (timer == (round == 1 ? 61 : 1)) {
            record = t3_bns_02_800b297c();
            t3_800757a8((uint32)r_s16(record + 18));
        }
    } else if (mode != 2 && mode != 4) {
        round = r_u32(0x80095484);
        timer = r_s32(0x80095460);
        if (timer == (round == 1 ? 61 : 1)) {
            maximum = r_u16(0x800ADF3C);
            round = r_u32(0x80095484);
            sound = (sint32)round < (sint32)maximum ? 0x103B : 0x103A;
            t3_80040c00(sound);
        }
        round = r_u32(0x80095484);
        timer = r_s32(0x80095460);
        if (timer == (round == 1 ? 100 : 40)) {
            maximum = r_u16(0x800ADF3C);
            round = r_u32(0x80095484);
            if ((sint32)round < (sint32)maximum) {
                sound = r_u16(0x8001A3BA + round * 2);
                t3_80040c00(sound);
            }
        }
    }
    round = r_u32(0x80095484);
    timer = r_s32(0x80095460);
    threshold = round == 1 ? 150 : 90;
    if ((uint32)timer != threshold)
        return threshold;
    return t3_80040c00(0x103C);
}

uint32 t3_800333ac(uint32 actor, uint32 value)
{
    uint32 side;
    uint32 mode;
    uint32 base;
    uint32 frame;
    uint32 bank;
    uint32 record;
    FUNCTION_MARKER(0x800333AC, "SLUS_004.02");
    side = actor != 0x800A9228;
    mode = r_u32(0x8009BCA8);
    if (mode == 3) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 0x79E0 : 0;
        w_u8(record + 49, (uint8)value);
    }
    if (mode == 1 || mode == 3 || mode == 6) {
        frame = (uint32)(r_s32(0x8009BCBC) >> 1);
        base = r_u32(0x800ADE68);
        bank = r_u32(0x8009BCAC);
        record = base ? base + bank * 31200 + side * 15600 + frame * 52 : 0;
        return r_u8(record + 49);
    }
    if (mode == 4) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 0x79E0 : 0;
        return r_u8(record + 49);
    }
    if (mode == 7) {
        base = r_u32(0x800ADE68);
        frame = r_u32(0x8009BCB8);
        record = base ? base + side * 15600 + frame * 52 : 0;
        w_u8(record + 49, (uint8)value);
    }
    return value;
}

uint32 t3_8006f2f0(void)
{
    uint32 actor;
    uint32 destination;
    uint32 saved_animation;
    uint32 animation;
    uint32 commands;
    uint32 codes[4];
    uint32 row;
    uint32 index;
    uint32 bit;
    uint32 value;
    uint32 enabled;
    sint32 frame;
    uint32 end;
    FUNCTION_MARKER(0x8006F2F0, "SLUS_004.02");
    w_u32(0x800A8660, 0);
    w_u32(0x800A8664, 0);
    for (row = 0; row < 2; ++row) {
        actor = 0x800A9228 + row * 6284;
        destination = 0x800A8660 + row * 4;
        enabled = (uint32)r_s16(actor + 0x122);
        saved_animation = r_u32(actor + 0x54);
        if (!t3_800333ac(actor, enabled))
            continue;
        if (r_s16(actor + 24) == 17) {
            value = r_u32(destination);
            w_u32(destination, value | 0x440);
            continue;
        }
        animation = r_u32(actor + 0x54);
        commands = r_u32(animation + 40);
        codes[0] = r_u8(commands);
        codes[1] = r_u8(commands + 1);
        codes[2] = r_u8(commands + 2);
        codes[3] = r_u8(commands + 3);
        if (!(codes[0] + codes[1] + codes[2] + codes[3])) {
            value = r_u32(destination);
            w_u32(destination, value | 0x440);
            continue;
        }
        frame = r_s16(actor + 0x58);
        end = r_u8(saved_animation + 46);
        if (frame > (sint32)end)
            continue;
        codes[1] = r_u8(commands + 1);
        codes[2] = r_u8(commands + 2);
        codes[3] = r_u8(commands + 3);
        for (index = 0; index < 4; ++index) {
            bit = r_u32(0x800251C4 + codes[index] * 8);
            if (bit) {
                value = r_u32(destination);
                value |= (1u << (bit & 31)) | 0x80000000;
                w_u32(destination, value);
            }
        }
    }
    return 0;
}

uint32 t3_8007061c(uint32 event)
{
    uint32 count;
    uint32 flags;
    uint32 row;
    uint32 packet;
    uint32 value;
    FUNCTION_MARKER(0x8007061C, "SLUS_004.02");
    count = r_u8(event + 14);
    flags = r_u8(event + 13);
    w_u8(event + 14, (uint8)(count + 1));
    w_u8(event + 13, (uint8)(flags | 0x60));
    for (row = 0; row < 2; ++row) {
        packet = event + 80 + row * 40;
        SetPolyFT4((POLY_FT4 *)psx_addr(packet, 40));
        w_u8(packet + 4, 128);
        w_u8(packet + 5, 128);
        w_u8(packet + 6, 128);
        value = r_u16(0x800ADE70);
        w_u16(packet + 22, (uint16)value);
        value = r_u16(0x800ADD84);
        w_u16(packet + 14, (uint16)value);
        SetSemiTrans((POLY_FT4 *)psx_addr(packet, 40), 1);
    }
    for (row = 0; row < 2; ++row) {
        packet = event + 80 + row * 40;
        value = r_u8(0x800252E8);
        w_u8(packet + 12, (uint8)value);
        value = r_u8(0x800252E9);
        w_u8(packet + 13, (uint8)value);
        value = r_u8(0x800252E8);
        w_u8(packet + 20, (uint8)(value + 15));
        value = r_u8(0x800252E9);
        w_u8(packet + 21, (uint8)value);
        value = r_u8(0x800252E8);
        w_u8(packet + 28, (uint8)value);
        value = r_u8(0x800252E9);
        w_u8(packet + 29, (uint8)(value + 15));
        value = r_u8(0x800252E8);
        w_u8(packet + 36, (uint8)(value + 15));
        value = r_u8(0x800252E9);
        w_u8(packet + 37, (uint8)(value + 15));
    }
    w_u32(event + 8, 0x80025310);
    w_u32(event + 68, 0xFFFFFFFF);
    return 0xFFFFFFFF;
}

uint32 t3_80033084(uint32 actor, uint32 value, uint32 index)
{
    uint32 side;
    uint32 mode;
    uint32 base;
    uint32 frame;
    uint32 bank;
    uint32 record;
    FUNCTION_MARKER(0x80033084, "SLUS_004.02");
    side = actor != 0x800A9228;
    mode = r_u32(0x8009BCA8);
    if (mode == 3) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 0x79E0 : 0;
        w_u8(record + index + 42, (uint8)value);
    }
    if (mode == 1 || mode == 3 || mode == 6) {
        frame = (uint32)(r_s32(0x8009BCBC) >> 1);
        base = r_u32(0x800ADE68);
        bank = r_u32(0x8009BCAC);
        record = base ? base + bank * 31200 + side * 15600 + frame * 52 : 0;
        return r_u8(record + index + 42);
    }
    if (mode == 4) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 0x79E0 : 0;
        return r_u8(record + index + 42);
    }
    if (mode == 7) {
        base = r_u32(0x800ADE68);
        frame = r_u32(0x8009BCB8);
        record = base ? base + side * 15600 + frame * 52 : 0;
        w_u8(record + index + 42, (uint8)value);
    }
    return value;
}

uint32 t3_800329b8(uint32 actor, uint16 *angles)
{
    uint32 side = actor != 0x800A9228;
    uint32 mode = r_u32(0x8009BCA8);
    uint32 base, record, frame, bank, value;
    FUNCTION_MARKER(0x800329B8, "SLUS_004.02");
    if (mode - 1 >= 7)
        return 0;
    if (mode == 2 || mode == 5)
        return 0x80032B8C;
    if (mode == 3) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 31200 : 0;
        w_u16(record + 36, angles[0]);
        w_u16(record + 38, angles[1]);
        w_u16(record + 40, angles[2]);
    }
    if (mode == 1 || mode == 3 || mode == 6) {
        frame = (uint32)(r_s32(0x8009BCBC) >> 1);
        base = r_u32(0x800ADE68);
        bank = r_u32(0x8009BCAC);
        record = base ? base + bank * 31200 + side * 15600 + frame * 52 : 0;
    } else if (mode == 4) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 31200 : 0;
    } else {
        base = r_u32(0x800ADE68);
        frame = r_u32(0x8009BCB8);
        record = base ? base + side * 15600 + frame * 52 : 0;
        w_u16(record + 36, angles[0]);
        w_u16(record + 38, angles[1]);
        value = angles[2];
        w_u16(record + 40, value);
        return value;
    }
    angles[0] = r_u16(record + 36);
    angles[1] = r_u16(record + 38);
    value = r_u16(record + 40);
    angles[2] = (uint16)value;
    return value;
}

uint32 t3_8003b71c(uint32 left, uint32 right, uint32 output)
{
    uint32 rotation[5];
    uint32 vector_xy;
    uint32 vector_z;
    uint32 packed;
    uint32 first_column[3];
    uint32 second_column[3];
    uint32 third_x;
    uint32 third_y;
    uint32 translation[3];
    uint32 high[3];
    uint32 low[3];
    uint32 origin[3];
    uint32 index;
    uint32 magnitude;
    FUNCTION_MARKER(0x8003B71C, "SLUS_004.02");
    for (index = 0; index < 5; ++index)
        rotation[index] = r_u32(left + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, rotation[index]);
    vector_xy = r_u16(right);
    packed = r_u32(right + 4);
    vector_z = r_u32(right + 12);
    vector_xy |= packed & 0xFFFF0000;
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    vector_xy = r_u16(right + 2);
    packed = r_u32(right + 8);
    vector_z = (uint32)r_s16(right + 14);
    vector_xy |= packed << 16;
    first_column[0] = xport_gte_read_data(9);
    first_column[1] = xport_gte_read_data(10);
    first_column[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    vector_xy = r_u16(right + 4);
    packed = r_u32(right + 8);
    vector_z = r_u32(right + 16);
    vector_xy |= packed & 0xFFFF0000;
    second_column[0] = xport_gte_read_data(9);
    second_column[1] = xport_gte_read_data(10);
    second_column[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    w_u32(output, (second_column[0] << 16) | (first_column[0] & 0xFFFF));
    w_u32(output + 12, (second_column[2] << 16) | (first_column[2] & 0xFFFF));
    third_x = xport_gte_read_data(9);
    third_y = xport_gte_read_data(10);
    w_u32(output + 16, xport_gte_read_data(11));
    w_u32(output + 4, (first_column[1] << 16) | (third_x & 0xFFFF));
    w_u32(output + 8, (third_y << 16) | (second_column[1] & 0xFFFF));
    translation[0] = r_u32(right + 20);
    translation[1] = r_u32(right + 24);
    translation[2] = r_u32(right + 28);
    for (index = 0; index < 3; ++index) {
        if ((sint32)translation[index] < 0) {
            magnitude = 0 - translation[index];
            high[index] = 0 - (uint32)((sint32)magnitude >> 15);
            low[index] = 0 - (magnitude & 0x7FFF);
        } else {
            high[index] = (uint32)((sint32)translation[index] >> 15);
            low[index] = translation[index] & 0x7FFF;
        }
    }
    xport_gte_write_data(9, high[0]);
    xport_gte_write_data(10, high[1]);
    xport_gte_write_data(11, high[2]);
    xport_gte_execute(0x4A41E012);
    high[0] = xport_gte_read_data(25);
    high[1] = xport_gte_read_data(26);
    high[2] = xport_gte_read_data(27);
    xport_gte_write_data(9, low[0]);
    xport_gte_write_data(10, low[1]);
    xport_gte_write_data(11, low[2]);
    xport_gte_execute(0x4A49E012);
    high[0] <<= 3;
    high[1] <<= 3;
    high[2] <<= 3;
    translation[0] = xport_gte_read_data(25);
    translation[1] = xport_gte_read_data(26);
    translation[2] = xport_gte_read_data(27);
    translation[0] += high[0];
    translation[1] += high[1];
    translation[2] += high[2];
    origin[0] = r_u32(left + 20);
    origin[1] = r_u32(left + 24);
    origin[2] = r_u32(left + 28);
    w_u32(output + 20, translation[0] + origin[0]);
    w_u32(output + 24, translation[1] + origin[1]);
    w_u32(output + 28, translation[2] + origin[2]);
    return output;
}

uint32 t3_8003b5b8(uint32 left, const MATRIX *right, uint32 output, uint32 translation_point)
{
    uint32 rotation[5];
    uint32 vector_xy;
    uint32 vector_z;
    uint32 packed;
    uint32 first_column[3];
    uint32 second_column[3];
    uint32 third_x;
    uint32 third_y;
    uint32 translation[3];
    uint32 origin[3];
    uint32 index;
    sint64 sum;
    FUNCTION_MARKER(0x8003B5B8, "SLUS_004.02");
    for (index = 0; index < 5; ++index)
        rotation[index] = r_u32(left + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, rotation[index]);
    vector_xy = (uint16)right->m[0][0];
    packed = (uint16)right->m[0][2] | ((uint32)(uint16)right->m[1][0] << 16);
    vector_z = (uint16)right->m[2][0] | ((uint32)(uint16)right->m[2][1] << 16);
    vector_xy |= packed & 0xFFFF0000;
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    vector_xy = (uint16)right->m[0][1];
    packed = (uint16)right->m[1][1] | ((uint32)(uint16)right->m[1][2] << 16);
    vector_z = (uint32)(sint32)right->m[2][1];
    vector_xy |= packed << 16;
    first_column[0] = xport_gte_read_data(9);
    first_column[1] = xport_gte_read_data(10);
    first_column[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    vector_xy = (uint16)right->m[0][2];
    packed = (uint16)right->m[1][1] | ((uint32)(uint16)right->m[1][2] << 16);
    vector_z = (uint32)(sint32)right->m[2][2];
    vector_xy |= packed & 0xFFFF0000;
    second_column[0] = xport_gte_read_data(9);
    second_column[1] = xport_gte_read_data(10);
    second_column[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    w_u32(output, (second_column[0] << 16) | (first_column[0] & 0xFFFF));
    w_u32(output + 12, (second_column[2] << 16) | (first_column[2] & 0xFFFF));
    third_x = xport_gte_read_data(9);
    third_y = xport_gte_read_data(10);
    w_u32(output + 16, xport_gte_read_data(11));
    vector_xy = r_u16(translation_point);
    packed = r_u32(translation_point + 4);
    vector_z = r_u32(translation_point + 8);
    vector_xy |= packed << 16;
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    w_u32(output + 4, (first_column[1] << 16) | (third_x & 0xFFFF));
    w_u32(output + 8, (third_y << 16) | (second_column[1] & 0xFFFF));
    translation[0] = xport_gte_read_data(25);
    translation[1] = xport_gte_read_data(26);
    translation[2] = xport_gte_read_data(27);
    origin[0] = r_u32(left + 20);
    origin[1] = r_u32(left + 24);
    origin[2] = r_u32(left + 28);
    for (index = 0; index < 3; ++index) {
        sum = (sint64)(sint32)translation[index] + (sint32)origin[index];
        if (sum > 0x7FFFFFFF || sum < -2147483648LL) {
            xport_mips_overflow_exception(0x8003B6FC + index * 4);
            return output;
        }
        translation[index] = (uint32)sum;
    }
    w_u32(output + 20, translation[0]);
    w_u32(output + 24, translation[1]);
    w_u32(output + 28, translation[2]);
    return output;
}

uint32 t3_8003bca8(uint32 point, uint32 angle, MATRIX *output, uint32 mode)
{
    uint32 x, y, z, squared, length, scale;
    uint32 normalized_x, normalized_y, normalized_z, product_y;
    uint32 pole, factor, tangent, bias, extended;
    uint32 basis_y, basis_z, first_y, first_z, second_y, second_z, third_y, third_z;
    uint32 table_offset, cosine, sine, negative_sine;
    uint32 correction, span, bend_first, bend_second, bend_sine, bend_other;
    uint32 value;
    MATRIX *auxiliary;
    FUNCTION_MARKER(0x8003BCA8, "SLUS_004.02");
    x = r_u32(point);
    squared = x * x;
    y = r_u32(point + 4);
    squared += y * y;
    z = r_u32(point + 8);
    squared += z * z;
    if (!squared)
        return 0xFFFFFFFF;
    auxiliary = output + 1;
    length = SquareRoot0PSX(squared, 0x80098D08);
    scale = length ? (uint32)(0x400000 / (sint32)length) : 0xFFFFFFFF;
    normalized_x = (uint32)((sint32)(x * scale) >> 10);
    product_y = y * scale;
    normalized_y = (uint32)((sint32)product_y >> 10);
    normalized_z = (uint32)((sint32)(z * scale) >> 10);
    pole = normalized_x + 4096;
    if (!pole) {
        factor = (uint32)-2048;
        tangent = (uint32)((sint32)product_y >> 11);
        bias = (uint32)-4096;
    } else {
        if ((sint32)pole < 8192) {
            factor = (uint32)(-16777216 / (sint32)pole);
            tangent = (uint32)((sint32)(factor * normalized_y) >> 12);
        } else {
            factor = (uint32)-2048;
            tangent = (uint32)((sint32)(0 - normalized_y) >> 1);
        }
        bias = 4096;
    }
    basis_y = (uint32)((sint32)(tangent * normalized_y) >> 12) + bias;
    extended = bias + normalized_x;
    first_y = tangent * extended;
    factor = (uint32)((sint32)(factor * normalized_z) >> 12);
    first_z = factor * extended;
    second_z = factor * normalized_y;
    third_y = tangent * normalized_z;
    third_z = factor * normalized_z;
    table_offset = (angle >> 3) & 0x1FFE;
    output->m[0][0] = (sint16)normalized_x;
    output->m[1][0] = (sint16)normalized_y;
    output->m[2][0] = (sint16)normalized_z;
    cosine = (uint32)r_s16(0x8001F0C4 + table_offset);
    first_y = (uint32)((sint32)first_y >> 12);
    sine = (uint32)r_s16(0x8001E8C4 + table_offset);
    first_z = (uint32)((sint32)first_z >> 12);
    second_z = (uint32)((sint32)second_z >> 12);
    third_y = (uint32)((sint32)third_y >> 12);
    basis_z = (uint32)((sint32)third_z >> 12) + 4096;
    negative_sine = 0 - sine;
    second_y = cosine * basis_y + sine * second_z;
    value = cosine * first_y + sine * first_z;
    output->m[0][1] = (sint16)((sint32)value >> 12);
    output->m[1][1] = (sint16)((sint32)second_y >> 12);
    value = cosine * third_y + sine * basis_z;
    output->m[2][1] = (sint16)((sint32)value >> 12);
    value = negative_sine * first_y + cosine * first_z;
    output->m[0][2] = (sint16)((sint32)value >> 12);
    value = negative_sine * basis_y + cosine * second_z;
    output->m[1][2] = (sint16)((sint32)value >> 12);
    value = negative_sine * third_y + cosine * basis_z;
    output->m[2][2] = (sint16)((sint32)value >> 12);
    if ((sint32)length >= (mode ? 534 : 892)) {
        auxiliary->m[2][2] = 4096;
        auxiliary->m[1][1] = 4096;
        auxiliary->m[0][0] = 4096;
        auxiliary->m[1][2] = 0;
        auxiliary->m[2][1] = 0;
        auxiliary->m[2][0] = 0;
        auxiliary->m[0][2] = 0;
        auxiliary->m[0][1] = 0;
        auxiliary->m[1][0] = 0;
        return 0;
    }
    if ((sint32)length < (mode ? 71 : 3))
        return 1;
    span = length << 11;
    correction = (uint32)((sint32)(scale * (mode ? 37380 : 1784)) >> 11);
    bend_first = (uint32)((sint32)((span + correction) * (mode ? 217 : 146)) >> 16);
    bend_second = (uint32)((sint32)((span - correction) * (mode ? 282 : 147)) >> 16);
    bend_sine = 0;
    if ((sint32)bend_first > 4096)
        bend_first = 4096;
    else if ((sint32)bend_first < -4096)
        bend_first = (uint32)-4096;
    else {
        bend_sine = SquareRoot0PSX(0x1000000 - bend_first * bend_first, 0x80098D08);
        if (!mode)
            bend_sine = 0 - bend_sine;
    }
    t3_8003bb88(output, (uint32)(sint32)(sint16)bend_sine, bend_first);
    bend_other = 0;
    if ((sint32)bend_second > 4096)
        bend_second = 4096;
    else if ((sint32)bend_second < -4096)
        bend_second = (uint32)-4096;
    else
        bend_other = (uint32)((sint32)((0 - bend_sine) * (mode ? 5332 : 4114)) >> 12);
    value = bend_first * bend_second + bend_sine * bend_other;
    value = (uint32)((sint32)value / 4096);
    auxiliary->m[1][1] = (sint16)value;
    auxiliary->m[0][0] = (sint16)value;
    value = (0 - bend_sine) * bend_second + bend_first * bend_other;
    value = (uint32)((sint32)value / 4096);
    auxiliary->m[0][1] = (sint16)(0 - value);
    auxiliary->m[1][0] = (sint16)value;
    auxiliary->m[1][2] = 0;
    auxiliary->m[2][1] = 0;
    auxiliary->m[2][0] = 0;
    auxiliary->m[0][2] = 0;
    auxiliary->m[2][2] = 4096;
    return 0;
}

MATRIX *t3_8003a338(sint32 x, sint32 y, sint32 z, MATRIX *output)
{
    uint32 x_index = ((uint32)x >> 3) & 0x1FFE;
    uint32 y_index = ((uint32)y >> 3) & 0x1FFE;
    uint32 z_index = ((uint32)z >> 3) & 0x1FFE;
    uint32 cos_x, cos_y, cos_z, sin_x, sin_y, sin_z;
    uint32 product_xz, product_xs, mixed;
    uint32 first[3], second[3], third[3];
    uint32 row_first, row_second, row_third;
    FUNCTION_MARKER(0x8003A338, "SLUS_004.02");
    cos_z = (uint32)r_s16(0x8001F0C4 + z_index);
    cos_x = (uint32)r_s16(0x8001F0C4 + x_index);
    product_xz = cos_x * cos_z;
    cos_y = (uint32)r_s16(0x8001F0C4 + y_index);
    sin_x = (uint32)r_s16(0x8001E8C4 + x_index);
    sin_z = (uint32)r_s16(0x8001E8C4 + z_index);
    xport_gte_write_data(8, sin_x);
    xport_gte_write_data(9, cos_z);
    xport_gte_write_data(10, sin_z);
    xport_gte_write_data(11, cos_y);
    product_xs = cos_x * sin_z;
    product_xz = (uint32)((sint32)product_xz >> 12);
    xport_gte_execute(0x4B98003D);
    sin_y = (uint32)r_s16(0x8001E8C4 + y_index);
    first[0] = xport_gte_read_data(9);
    first[1] = xport_gte_read_data(10);
    first[2] = xport_gte_read_data(11);
    xport_gte_write_data(8, sin_y);
    xport_gte_write_data(9, first[0]);
    xport_gte_write_data(10, first[1]);
    xport_gte_write_data(11, product_xz);
    product_xs = (uint32)((sint32)product_xs >> 12);
    xport_gte_execute(0x4B98003D);
    mixed = product_xs * sin_y;
    output->m[2][1] = (sint16)first[2];
    output->m[2][0] = (sint16)(0 - sin_y);
    second[0] = xport_gte_read_data(9);
    second[1] = xport_gte_read_data(10);
    second[2] = xport_gte_read_data(11);
    xport_gte_write_data(8, cos_y);
    xport_gte_write_data(9, cos_z);
    xport_gte_write_data(10, sin_z);
    xport_gte_write_data(11, cos_x);
    row_first = second[0] - product_xs;
    row_second = product_xz + second[1];
    xport_gte_execute(0x4B98003D);
    row_third = first[1] + second[2];
    output->m[0][1] = (sint16)row_first;
    output->m[1][1] = (sint16)row_second;
    output->m[0][2] = (sint16)row_third;
    mixed = (uint32)((sint32)mixed >> 12) - first[0];
    third[0] = xport_gte_read_data(9);
    third[1] = xport_gte_read_data(10);
    third[2] = xport_gte_read_data(11);
    output->m[1][2] = (sint16)mixed;
    output->m[0][0] = (sint16)third[0];
    output->m[1][0] = (sint16)third[1];
    output->m[2][2] = (sint16)third[2];
    return output;
}

MATRIX *t3_8003a34c(const SVECTOR *angles, MATRIX *output)
{
    uint32 packed, x_index, y_index, z_index;
    sint32 z;
    uint32 cos_x, cos_y, cos_z, sin_x, sin_y, sin_z;
    uint32 product_xz, product_xs, mixed;
    uint32 first[3], second[3], third[3];
    uint32 row_first, row_second, row_third;
    FUNCTION_MARKER(0x8003A34C, "SLUS_004.02");
    z = angles->vz;
    packed = (uint16)angles->vx;
    packed |= (uint32)(uint16)angles->vy << 16;
    x_index = (packed >> 3) & 0x1FFE;
    y_index = (uint32)((sint32)packed >> 19) & 0x1FFE;
    z_index = ((uint32)z >> 3) & 0x1FFE;
    cos_z = (uint32)r_s16(0x8001F0C4 + z_index);
    cos_x = (uint32)r_s16(0x8001F0C4 + x_index);
    product_xz = cos_x * cos_z;
    cos_y = (uint32)r_s16(0x8001F0C4 + y_index);
    sin_x = (uint32)r_s16(0x8001E8C4 + x_index);
    sin_z = (uint32)r_s16(0x8001E8C4 + z_index);
    xport_gte_write_data(8, sin_x);
    xport_gte_write_data(9, cos_z);
    xport_gte_write_data(10, sin_z);
    xport_gte_write_data(11, cos_y);
    product_xs = cos_x * sin_z;
    product_xz = (uint32)((sint32)product_xz >> 12);
    xport_gte_execute(0x4B98003D);
    sin_y = (uint32)r_s16(0x8001E8C4 + y_index);
    first[0] = xport_gte_read_data(9);
    first[1] = xport_gte_read_data(10);
    first[2] = xport_gte_read_data(11);
    xport_gte_write_data(8, sin_y);
    xport_gte_write_data(9, first[0]);
    xport_gte_write_data(10, first[1]);
    xport_gte_write_data(11, product_xz);
    product_xs = (uint32)((sint32)product_xs >> 12);
    xport_gte_execute(0x4B98003D);
    mixed = product_xs * sin_y;
    output->m[2][1] = (sint16)first[2];
    output->m[2][0] = (sint16)(0 - sin_y);
    second[0] = xport_gte_read_data(9);
    second[1] = xport_gte_read_data(10);
    second[2] = xport_gte_read_data(11);
    xport_gte_write_data(8, cos_y);
    xport_gte_write_data(9, cos_z);
    xport_gte_write_data(10, sin_z);
    xport_gte_write_data(11, cos_x);
    row_first = second[0] - product_xs;
    row_second = product_xz + second[1];
    xport_gte_execute(0x4B98003D);
    row_third = first[1] + second[2];
    output->m[0][1] = (sint16)row_first;
    output->m[1][1] = (sint16)row_second;
    output->m[0][2] = (sint16)row_third;
    mixed = (uint32)((sint32)mixed >> 12) - first[0];
    third[0] = xport_gte_read_data(9);
    third[1] = xport_gte_read_data(10);
    third[2] = xport_gte_read_data(11);
    output->m[1][2] = (sint16)mixed;
    output->m[0][0] = (sint16)third[0];
    output->m[1][0] = (sint16)third[1];
    output->m[2][2] = (sint16)third[2];
    return output;
}

uint32 t3_8003afcc(uint32 actor)
{
    uint32 words[4];
    uint32 row, block, index, special, enabled, scale, source, destination;
    uint32 pose = actor + 0xF74;
    uint32 model_table, angle, x, y, opponent, local, parent, output;
    uint32 mode, count, projected = 0x1F800200;
    FUNCTION_MARKER(0x8003AFCC, "SLUS_004.02");
    special = r_s16(actor + 24) == 20;
    for (block = 0; block < 2; ++block) {
        for (index = 0; index < 4; ++index)
            words[index] = r_u32(actor + 0xF54 + block * 16 + index * 4);
        for (index = 0; index < 4; ++index)
            w_u32(actor + 0x8B0 + block * 16 + index * 4, words[index]);
    }
    if (!r_u32(0x8009548C) && !r_u32(0x80095468)) {
        t3_8003ad10(0x800A85E8, (MATRIX *)psx_addr(pose, 576), NULL);
        enabled = r_u32(actor + 0x181C);
        if (enabled)
            scale = (uint32)r_s16(actor + 0x186E);
        for (row = 1; row < 18; ++row) {
            source = actor + 0xF74 + row * 32;
            destination = actor + 0x1390 + row * 32;
            if (enabled) {
                t3_8003a72c(actor + 0x15D0 + row * 32, source, scale, 0x1F800000);
                t3_80075fc8(0x1F800000, source);
            }
            for (block = 0; block < 2; ++block) {
                for (index = 0; index < 4; ++index)
                    words[index] = r_u32(source + block * 16 + index * 4);
                for (index = 0; index < 4; ++index)
                    w_u32(destination + block * 16 + index * 4, words[index]);
            }
        }
        if (!r_u8(actor + 0xC9)) {
            if (r_s16(actor + 24) == 17) {
                opponent = t3_80045c18(actor);
                t3_800397fc(actor, opponent);
            }
            opponent = t3_80045c18(actor);
            t3_80039074(actor, opponent);
        }
    }
    model_table = r_u32(actor + 0x1274) + 20;
    if (special) {
        x = (uint32)r_s16(pose + 0xA0);
        y = (uint32)r_s16(pose + 0xA2);
        angle = (0 - t3_8004b39c(x, y)) << 2;
    }
    for (row = 0; row < 2; ++row) {
        source = row ? 0x800ADE48 : 0x800ADE20;
        destination = 0x1F800100 + row * 32;
        for (block = 0; block < 2; ++block) {
            for (index = 0; index < 3; ++index)
                words[index] = r_u32(source + block * 12 + index * 4);
            for (index = 0; index < 3; ++index)
                w_u32(destination + block * 12 + index * 4, words[index]);
        }
        words[0] = r_u32(source + 24);
        words[1] = r_u32(source + 28);
        w_u32(destination + 24, words[0]);
        w_u32(destination + 28, words[1]);
    }
    mode = r_u32(0x800AFA88);
    w_u32(actor + 0x12D4, 0);
    for (row = 0; row < 18; ++row) {
        if (!r_u32(actor + row * 40 + 0x4F4))
            continue;
        output = actor + 0x8F4 + row * 68;
        destination = output + 32;
        if (special)
            local = t3_80033a00(actor, row, angle);
        else
            local = pose + row * 32;
        parent = r_u32(destination + 32);
        t3_8003b5c0(parent, local, 0x1F800140);
        for (block = 0; block < 2; ++block) {
            for (index = 0; index < 4; ++index)
                words[index] = r_u32(0x1F800140 + block * 16 + index * 4);
            for (index = 0; index < 4; ++index)
                w_u32(output + block * 16 + index * 4, words[index]);
        }
        t3_8003b71c(0x1F800100, 0x1F800140, destination);
        if (mode != 8) {
            source = r_u32(model_table + row * 4);
            count = r_u32(source);
            if (count) {
                t3_8003b71c(0x1F800120, 0x1F800140, 0x1F800140);
                t3_8003b96c(0x1F800140);
                projected = t3_80037a9c(model_table + row * 4, projected, count);
            }
        }
    }
    for (row = 18; row < 22; ++row) {
        if (!r_u32(actor + row * 40 + 0x4F4))
            continue;
        t3_80037cbc(actor, row);
        output = actor + 0x8F4 + row * 68;
        destination = output + 32;
        if (special)
            local = t3_80033a00(actor, row, angle);
        else
            local = pose + row * 32;
        parent = r_u32(destination + 32);
        t3_8003b5c0(parent, local, 0x1F800140);
        for (block = 0; block < 2; ++block) {
            for (index = 0; index < 4; ++index)
                words[index] = r_u32(0x1F800140 + block * 16 + index * 4);
            for (index = 0; index < 4; ++index)
                w_u32(output + block * 16 + index * 4, words[index]);
        }
        t3_8003b71c(0x1F800100, 0x1F800140, destination);
    }
    return 0;
}

uint32 t3_80075fc8(uint32 input, uint32 output)
{
    uint32 value, first_y, second_y, third_y;
    uint32 axis_x, axis_y, axis_z, dot, projected_x, projected_y, projected_z;
    FUNCTION_MARKER(0x80075FC8, "SLUS_004.02");
    value = (uint32)r_s16(input);
    w_u32(0x1F800060, value);
    value = (uint32)r_s16(input + 6);
    w_u32(0x1F800064, value);
    value = (uint32)r_s16(input + 12);
    w_u32(0x1F800068, value);
    t3_8003a5bc(0x1F800060, 0x1F800040);
    value = r_u16(0x1F800040);
    w_u16(output, (uint16)value);
    value = r_u16(0x1F800044);
    w_u16(output + 6, (uint16)value);
    value = r_u16(0x1F800048);
    w_u16(output + 12, (uint16)value);
    first_y = (uint32)r_s16(input + 2);
    axis_x = r_u32(0x1F800040);
    dot = first_y * axis_x;
    second_y = (uint32)r_s16(input + 8);
    axis_y = r_u32(0x1F800044);
    dot += second_y * axis_y;
    axis_z = r_u32(0x1F800048);
    third_y = (uint32)r_s16(input + 14);
    dot += third_y * axis_z;
    dot = (uint32)((sint32)dot >> 12);
    projected_x = dot * axis_x;
    projected_y = dot * axis_y;
    projected_z = dot * axis_z;
    value = first_y - (uint32)((sint32)projected_x >> 12);
    w_u32(0x1F800060, value);
    value = (uint32)r_s16(input + 8);
    value -= (uint32)((sint32)projected_y >> 12);
    w_u32(0x1F800064, value);
    value = (uint32)r_s16(input + 14);
    value -= (uint32)((sint32)projected_z >> 12);
    w_u32(0x1F800068, value);
    t3_8003a5bc(0x1F800060, 0x1F800050);
    value = r_u16(0x1F800050);
    w_u16(output + 2, (uint16)value);
    value = r_u16(0x1F800054);
    w_u16(output + 8, (uint16)value);
    value = r_u16(0x1F800058);
    w_u16(output + 14, (uint16)value);
    t3_8003a6d8(0x1F800040, 0x1F800050, 0x1F800060);
    value = r_u16(0x1F800060);
    w_u16(output + 4, (uint16)value);
    value = r_u16(0x1F800064);
    w_u16(output + 10, (uint16)value);
    value = r_u16(0x1F800068);
    w_u16(output + 16, (uint16)value);
    return value;
}

uint32 t3_8003a618(uint32 x, uint32 y, uint32 z, uint32 *normalized)
{
    uint32 squares[3], products[3];
    uint32 squared, leading = 0, shift, index, factor;
    FUNCTION_MARKER(0x8003A618, "SLUS_004.02");
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    xport_gte_execute(0x4AA00428);
    squares[0] = xport_gte_read_data(25);
    squares[1] = xport_gte_read_data(26);
    squares[2] = xport_gte_read_data(27);
    squared = squares[0] + squares[1] + squares[2];
    xport_gte_write_data(30, squared);
    if ((sint32)squared >= 0)
        leading = xport_gte_read_data(31) & 30;
    shift = (uint32)((sint32)(31 - leading) >> 1);
    if ((sint32)(leading - 24) < 0)
        index = (uint32)((sint32)squared >> ((24 - leading) & 31));
    else
        index = squared << ((leading - 24) & 31);
    factor = (uint32)r_s16(0x8001A194 + (index - 64) * 2);
    xport_gte_write_data(8, factor);
    xport_gte_write_data(9, x);
    xport_gte_write_data(10, y);
    xport_gte_write_data(11, z);
    xport_gte_execute(0x4B90003D);
    products[0] = xport_gte_read_data(25);
    products[1] = xport_gte_read_data(26);
    products[2] = xport_gte_read_data(27);
    normalized[0] = (uint32)((sint32)products[0] >> (shift & 31));
    normalized[1] = (uint32)((sint32)products[1] >> (shift & 31));
    normalized[2] = (uint32)((sint32)products[2] >> (shift & 31));
    return squared;
}

uint32 t3_8003a5bc(uint32 input, uint32 output)
{
    uint32 x, y, z, result;
    uint32 normalized[3];
    FUNCTION_MARKER(0x8003A5BC, "SLUS_004.02");
    x = r_u32(input);
    y = r_u32(input + 4);
    z = r_u32(input + 8);
    result = t3_8003a618(x, y, z, normalized);
    w_u32(output, normalized[0]);
    w_u32(output + 4, normalized[1]);
    w_u32(output + 8, normalized[2]);
    return result;
}

uint32 t3_80033210(uint32 actor, uint32 value)
{
    uint32 side = actor != 0x800A9228;
    uint32 mode, base, record, frame, bank;
    FUNCTION_MARKER(0x80033210, "SLUS_004.02");
    mode = r_u32(0x8009BCA8);
    if (mode == 3) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 31200 : 0;
        w_u8(record + 50, (uint8)((sint32)value / 2));
    }
    if (mode == 1 || mode == 3 || mode == 6) {
        frame = (uint32)(r_s32(0x8009BCBC) >> 1);
        base = r_u32(0x800ADE68);
        bank = r_u32(0x8009BCAC);
        record = base ? base + bank * 31200 + side * 15600 + frame * 52 : 0;
        return (uint32)r_u8(record + 50) << 1;
    }
    if (mode == 4) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 31200 : 0;
        return (uint32)r_u8(record + 50) << 1;
    }
    if (mode == 7) {
        base = r_u32(0x800ADE68);
        frame = r_u32(0x8009BCB8);
        record = base ? base + side * 15600 + frame * 52 : 0;
        w_u8(record + 50, (uint8)((sint32)value / 2));
    }
    return value;
}

uint32 t3_80033a00(uint32 actor, uint32 bone, uint32 angle)
{
    sint32 phase = -1;
    uint32 selected = bone, output, value, progress, amplitude, offset;
    uint32 first_scale, other_scale, row, first, second, third;
    FUNCTION_MARKER(0x80033A00, "SLUS_004.02");
    if (bone == 6) {
        phase = 1;
        selected = 5;
    } else if (bone == 18) {
        phase = 2;
        selected = 19;
    } else if (bone == 19) {
        phase = 3;
        selected = 6;
    } else if (bone == 5) {
        if (!r_u32(0x8009548C) && !r_u32(0x80095468)) {
            value = r_u16(actor + 0x1294);
            w_u16(actor + 0x1294, (uint16)(value + 1));
        }
        w_u16(0x8009BCE8, 0);
        value = (uint32)r_s16(actor + 0x1294);
        w_u32(0x8009BCD8, (value << 5) & 0xFFF);
        value = (uint32)r_s16(actor + 0x1294);
        w_u32(0x8009BCDC, (value << 6) & 0xFFF);
        value = 0 - r_u32(actor + 0xAA4);
        value = (uint32)((sint32)value / 2);
        w_u32(0x8009BCE0, value);
        if ((sint32)value >= 257)
            w_u32(0x8009BCE0, 256);
        if (r_s32(0x8009BCE0) < 0)
            w_u32(0x8009BCE0, 0);
        phase = 0;
        selected = 18;
    }
    output = actor + (selected << 5) + 0xF74;
    if (r_u32(0x8009548C) || r_u32(0x80095468) || phase < 0)
        return output;
    value = (uint32)r_s16(actor + 0x1296);
    progress = t3_80033210(actor, value);
    value = r_u32(0x8009BCE0) * (256 - progress);
    amplitude = (uint32)((sint32)value / 256);
    offset = (uint32)phase << 10;
    value = r_u32(0x8009BCD8);
    value = (uint32)r_s16(0x8001E8C4 + ((value + offset) & 0xFFF) * 2);
    value *= amplitude;
    value = (uint32)((sint32)value / 512);
    first = r_u32(0x8009BCDC);
    w_u16(0x8009BCEA, (uint16)value);
    value = (uint32)r_s16(0x8001E8C4 + ((first + offset) & 0xFFF) * 2);
    value *= amplitude;
    value = (uint32)((sint32)value / 256) + angle;
    w_u16(0x8009BCEC, (uint16)value);
    t3_8003a34c((const SVECTOR *)psx_addr(0x8009BCE8, 8), (MATRIX *)psx_addr(output, 32));
    first_scale = (uint32)((sint32)progress / 4) + 256;
    other_scale = (uint32)((sint32)progress / 8) + 256;
    if (bone == 19)
        first_scale = 512 - first_scale;
    for (row = 0; row < 3; ++row) {
        first = (uint32)r_s16(output + row * 6) * first_scale;
        second = (uint32)r_s16(output + row * 6 + 2) * other_scale;
        w_u16(output + row * 6, (uint16)((sint32)first / 256));
        third = (uint32)r_s16(output + row * 6 + 4) * other_scale;
        w_u16(output + row * 6 + 2, (uint16)((sint32)second / 256));
        w_u16(output + row * 6 + 4, (uint16)((sint32)third / 256));
    }
    return output;
}

uint32 t3_800397fc(uint32 actor, uint32 opponent)
{
    uint32 words[4], block, index, x, y, z, origin_x, origin_y, origin_z;
    sint32 angle, adjustment;
    FUNCTION_MARKER(0x800397FC, "SLUS_004.02");
    for (block = 0; block < 2; ++block) {
        for (index = 0; index < 4; ++index)
            words[index] = r_u32(actor + 0x97C + block * 16 + index * 4);
        for (index = 0; index < 4; ++index)
            w_u32(0x1F800000 + block * 16 + index * 4, words[index]);
    }
    x = r_u32(opponent + 0x990);
    w_u32(0x1F800020, x);
    y = r_u32(opponent + 0x994);
    w_u32(0x1F800024, y);
    origin_x = r_u32(0x1F800014);
    z = r_u32(opponent + 0x998);
    w_u32(0x1F800020, x - origin_x);
    origin_y = r_u32(0x1F800018);
    origin_z = r_u32(0x1F80001C);
    w_u32(0x1F800028, z);
    w_u32(0x1F800024, y - origin_y);
    w_u32(0x1F800028, z - origin_z);
    t3_8003b9b4(0x1F800000);
    ApplyRotMatrixLVPSX(0x1F800020, 0x1F800030);
    x = r_u32(0x1F800030);
    y = r_u32(0x1F800034);
    angle = (sint16)t3_8004b39c(x, y);
    if (angle < 2048) {
        adjustment = -(angle / 60);
        if (adjustment < -17)
            adjustment = -17;
    } else {
        adjustment = -((angle - 3840) / 60);
        if (adjustment > 17)
            adjustment = 17;
    }
    return t3_80033ecc(actor, (uint32)adjustment);
}

uint32 t3_80033ecc(uint32 actor, uint32 adjustment)
{
    PSX_RECT rectangle;
    uint32 entry, dimensions, row, offsets[2], source, destination;
    uint32 value, source_y;
    sint32 destination_x, destination_y;
    FUNCTION_MARKER(0x80033ECC, "SLUS_004.02");
    if (r_s16(actor + 24) != 17)
        return 17;
    entry = 0x800958BC + (uint32)r_s16(actor + 28) * 6;
    dimensions = 0x8009586C + (uint32)r_s8(entry + 1) * 4;
    rectangle.w = (sint16)r_u16(dimensions);
    rectangle.h = (sint16)r_u16(dimensions + 2);
    offsets[0] = (sint32)adjustment < 0 ? 0 : adjustment;
    offsets[1] = (sint32)adjustment < 0 ? adjustment : 0;
    for (row = 0; row < 2; ++row) {
        value = (uint32)r_s8(actor + 0x1292 + row);
        if (value == offsets[row])
            continue;
        w_u8(actor + 0x1292 + row, (uint8)offsets[row]);
        source = 0x8009579C + (uint32)r_s8(entry + row + 4) * 4;
        destination = 0x8009579C + (uint32)r_s8(entry + row + 2) * 4;
        rectangle.x = (sint16)r_u16(source);
        destination_x = r_s16(destination);
        source_y = r_u16(source + 2) + offsets[row] + (row ? 21 : 8);
        rectangle.y = (sint16)source_y;
        destination_y = r_s16(destination + 2) + (row ? 13 : 17);
        if (r_s16(actor + 18)) {
            destination_y += 256;
            rectangle.y = (sint16)((uint16)rectangle.y + 256);
        }
        t3_800293bc(&rectangle, destination_x, destination_y);
    }
    return 0;
}

uint32 t3_800707f4(uint32 event)
{
    VECTOR rotated;
    uint32 selection, flags, value, bone, actor, matrix, vector, origin;
    uint32 index, counter, bank, packet, random_value, threshold;
    FUNCTION_MARKER(0x800707F4, "SLUS_004.02");
    selection = r_u32(event + 64);
    flags = r_u32((selection & 0x8000) ? 0x800A8664 : 0x800A8660);
    selection = r_u32(event + 64);
    if (!((flags >> (selection & 31)) & 1)) {
        value = r_u8(event + 13) & 0xBF;
        w_u8(event + 13, (uint8)value);
        return value;
    }
    selection = r_u32(event + 64);
    value = r_u8(event + 13);
    bone = selection & 0x7FFF;
    w_u8(event + 13, (uint8)(value | 0x40));
    actor = 0x800A9228 + ((selection & 0x8000) >> 15) * 6284;
    if (bone == 6 || bone == 10) {
        if (bone == 6)
            matrix = actor + (r_s16(actor + 24) == 20 ? 0xE00 : 0xA8C);
        else
            matrix = actor + 0xB9C;
        SetRotMatrix((MATRIX *)psx_addr(matrix, 32));
        value = (uint32)r_s16(actor + 24);
        vector = (bone == 6 ? 0x80025334 : 0x8002533C) + (value << 4);
        ApplyRotMatrix((SVECTOR *)psx_addr(vector, 8), &rotated);
        /* Original SDK stores saturated IR values */
        rotated.vx = (sint32)xport_gte_read_data(9);
        rotated.vy = (sint32)xport_gte_read_data(10);
        rotated.vz = (sint32)xport_gte_read_data(11);
        origin = matrix + 20;
        value = r_u32(origin);
        w_u32(event + 40, (uint32)rotated.vx + value);
        value = r_u32(origin + 4);
        w_u32(event + 44, (uint32)rotated.vy + value);
        value = (uint32)rotated.vz + r_u32(origin + 8);
    } else {
        origin = actor + bone * 68 + 0x908;
        value = r_u32(origin);
        w_u32(event + 40, value);
        value = r_u32(origin + 4);
        w_u32(event + 44, value);
        value = r_u32(origin + 8);
    }
    w_u32(event + 48, value);
    counter = r_u32(event + 68);
    bank = r_u32(0x800ADEFC);
    ++counter;
    w_u32(event + 68, counter);
    value = r_u16(0x800A8D00 + ((counter << 1) & 28));
    w_u16(event + bank * 40 + 94, (uint16)value);
    if ((sint32)flags < 0) {
        packet = t3_8006f4f4();
        if (packet != 0xFFFFFFFF) {
            w_u8(packet + 12, 4);
            for (index = 0; index < 3; ++index) {
                value = r_u32(event + 40 + index * 4);
                w_u32(packet + 40 + index * 4, value);
            }
        }
    }
    value = (uint32)r_s16(0x800A9194);
    threshold = (80 - value) * 50;
    threshold <<= 14;
    threshold = (uint32)((sint32)threshold / 80000) << 2;
    random_value = (uint32)randPSX();
    if ((sint32)random_value < (sint32)threshold) {
        packet = t3_8006f4f4();
        if (packet != 0xFFFFFFFF) {
            w_u8(packet + 12, 5);
            for (index = 0; index < 3; ++index) {
                value = r_u32(event + 40 + index * 4);
                w_u32(packet + 40 + index * 4, value);
            }
        }
    }
    value = random_value & 0x80;
    if (value)
        return t3_80039f0c((const uint32 *)psx_addr(event + 40, 12), 0x80025330);
    return value;
}

MATRIX *t3_8003a490(const SVECTOR *angles, MATRIX *output)
{
    uint32 x_index, y_index, z_index;
    sint32 z;
    uint32 cos_x, cos_y, cos_z, sin_x, sin_y, sin_z;
    uint32 product_xz, product_xs, mixed;
    uint32 first[3], second[3], third[3];
    uint32 row_first, row_second, row_third;
    FUNCTION_MARKER(0x8003A490, "SLUS_004.02");
    z = angles->vz;
    x_index = ((uint32)(uint16)angles->vx << 1) & 0x1FFE;
    y_index = ((uint32)(uint16)angles->vy << 1) & 0x1FFE;
    z_index = ((uint32)z << 1) & 0x1FFE;
    cos_z = (uint32)r_s16(0x8001F0C4 + z_index);
    cos_x = (uint32)r_s16(0x8001F0C4 + x_index);
    product_xz = cos_x * cos_z;
    cos_y = (uint32)r_s16(0x8001F0C4 + y_index);
    sin_x = (uint32)r_s16(0x8001E8C4 + x_index);
    sin_z = (uint32)r_s16(0x8001E8C4 + z_index);
    xport_gte_write_data(8, sin_x);
    xport_gte_write_data(9, cos_z);
    xport_gte_write_data(10, sin_z);
    xport_gte_write_data(11, cos_y);
    product_xs = cos_x * sin_z;
    product_xz = (uint32)((sint32)product_xz >> 12);
    xport_gte_execute(0x4B98003D);
    sin_y = (uint32)r_s16(0x8001E8C4 + y_index);
    first[0] = xport_gte_read_data(9);
    first[1] = xport_gte_read_data(10);
    first[2] = xport_gte_read_data(11);
    xport_gte_write_data(8, sin_y);
    xport_gte_write_data(9, first[0]);
    xport_gte_write_data(10, first[1]);
    xport_gte_write_data(11, product_xz);
    product_xs = (uint32)((sint32)product_xs >> 12);
    xport_gte_execute(0x4B98003D);
    mixed = product_xs * sin_y;
    output->m[1][2] = (sint16)(0 - first[2]);
    output->m[0][2] = (sint16)sin_y;
    second[0] = xport_gte_read_data(9);
    second[1] = xport_gte_read_data(10);
    second[2] = xport_gte_read_data(11);
    xport_gte_write_data(8, cos_y);
    xport_gte_write_data(9, cos_z);
    xport_gte_write_data(10, 0 - sin_z);
    xport_gte_write_data(11, cos_x);
    row_first = second[0] + product_xs;
    row_second = product_xz - second[1];
    xport_gte_execute(0x4B98003D);
    row_third = first[1] - second[2];
    output->m[1][0] = (sint16)row_first;
    output->m[1][1] = (sint16)row_second;
    output->m[2][0] = (sint16)row_third;
    mixed = (uint32)((sint32)mixed >> 12) + first[0];
    third[0] = xport_gte_read_data(9);
    third[1] = xport_gte_read_data(10);
    third[2] = xport_gte_read_data(11);
    output->m[2][1] = (sint16)mixed;
    output->m[0][0] = (sint16)third[0];
    output->m[0][1] = (sint16)third[1];
    output->m[2][2] = (sint16)third[2];
    return output;
}

void t3_8006e44c(const uint16 *depths, uint32 packet, uint32 ordering, uint32 event)
{
    uint32 screens[4], index, depth, flags, tag;
    FUNCTION_MARKER(0x8006E44C, "SLUS_004.02");
    for (index = 0; index < 4; ++index)
        screens[index] = r_u32(0x1F800000 + index * 4);
    if ((sint16)screens[0] > 368 || (sint16)screens[1] < 0 ||
        ((sint32)screens[0] >> 16) > 468 || ((sint32)screens[2] >> 16) < 20)
        return;
    depth = depths[0] | ((uint32)depths[1] << 16);
    flags = r_u32(event + 12);
    depth = (depth >> 21) & 0x3FF;
    if (flags & 0x2000) {
        depth -= 5;
        if ((sint32)depth < 0)
            return;
    }
    ordering += depth * 4;
    tag = (r_u32(ordering) & 0xFFFFFF) | 0x09000000;
    w_u32(packet + 8, screens[0]);
    w_u32(packet + 16, screens[1]);
    w_u32(packet + 24, screens[2]);
    w_u32(packet + 32, screens[3]);
    w_u32(packet, tag);
    w_u32(ordering, packet & 0xFFFFFF);
}

uint32 t3_8006f5c0(uint32 update)
{
    static const uint32 initial_bones[8] = {6, 10, 14, 17, 0x8006, 0x800A, 0x800E, 0x8011};
    static const uint32 packet_bytes[8] = {12, 13, 20, 21, 28, 29, 36, 37};
    MATRIX camera, projection;
    SVECTOR angles, relative;
    VECTOR translation;
    uint16 depths[4], interpolation[4], projection_flags[4];
    uint32 previous_bank, owner, ordering, count = 0, value, second;
    uint32 index, event, next, kind, flags, base, slot, output, source, bank;
    FUNCTION_MARKER(0x8006F5C0, "SLUS_004.02");
    w_u32(0x800ADEB0, 0);
    w_u32(0x800ADEAC, 0);
    previous_bank = r_u32(0x800ADEFC);
    owner = r_u32(0x800A8C54);
    previous_bank = (previous_bank + 1) & 1;
    ordering = r_u32(owner + 4);
    if (update) {
        value = r_u32(0x800987FC);
        w_u8(0x800ADBC8, 0);
        w_u8(0x800AC33C, 0);
        w_u8(0x800AAAB0, 0);
        w_u32(0x800987FC, value + 1);
        t3_8006f2f0();
    }
    if (!r_u8(0x800A0990) && r_u32(0x800954A4)) {
        t3_8006f468();
        for (index = 0; index < 8; ++index) {
            event = t3_8006f4f4();
            if (event != 0xFFFFFFFF) {
                w_u8(event + 12, 3);
                w_u32(event + 64, initial_bones[index]);
            }
        }
        value = r_u8(0x800A0990);
        w_u8(0x800ADED6, 0);
        w_u8(0x800A0990, (uint8)(value + 1));
    }
    if (!r_u8(0x800A0991) && r_u32(0x8009546C) - 3 < 5) {
        t3_8006f468();
        value = r_u8(0x800A0991);
        w_u8(0x800A0991, (uint8)(value + 1));
    }
    angles.vx = (sint16)(r_u16(0x800A85C0) & 0xFFF);
    value = r_u16(0x800A85C4);
    second = r_u16(0x800A85C8);
    angles.vy = (sint16)(value & 0xFFF);
    angles.vz = (sint16)((0 - second) & 0xFFF);
    t3_8003a490(&angles, &camera);
    relative.vx = 0;
    relative.vy = 0;
    relative.vz = 0;
    t3_8003a490(&relative, &projection);
    /* GTE ignores the high half of rotation control word four */
    ((uint8 *)&camera)[18] = 0;
    ((uint8 *)&camera)[19] = 0;
    ((uint8 *)&projection)[18] = 0;
    ((uint8 *)&projection)[19] = 0;
    next = r_u32(0x800A098C);
    for (index = 0; index < 3; ++index)
        camera.m[1][index] = (sint16)((sint32)camera.m[1][index] * 20 / 12);
    for (index = 0; index < 3; ++index)
        projection.m[1][index] = (sint16)((sint32)projection.m[1][index] * 20 / 12);
    while (next != 0x800A0988) {
        event = next;
        next = r_u32(event + 4);
        if (update) {
            kind = r_u8(event + 12);
            switch (kind) {
            case 0: t3_8006fd64(event); break;
            case 1: t3_8006fe04(event); break;
            case 2: t3_800701f0(event); break;
            case 3: t3_800705c8(event); break;
            case 4: t3_80070b74(event); break;
            case 5: t3_80070ea0(event); break;
            case 6: t3_800711ac(event); break;
            case 7: t3_80071a24(event); break;
            case 8: t3_80071e5c(event); break;
            case 9: t3_800725ec(event); break;
            case 10: t3_800729fc(event); break;
            case 11: t3_80072e74(event, event); break;
            case 12: t3_800734b4(event); break;
            case 13: t3_800738ac(event); break;
            case 15: t3_80073c90(event); break;
            case 16: t3_8007444c(event); break;
            case 17: t3_800744e0(event); break;
            case 18: t3_80074668(event); break;
            case 19: t3_800747ec(event); break;
            case 20: t3_80074a30(event); break;
            }
            flags = r_u8(event + 13);
            ++count;
            if (flags & 2) {
                base = r_u32(0x800ADC80);
                if (flags & 16) {
                    slot = r_u32(0x800ADEAC);
                    output = base ? base + 0x3200 + slot * 24 : 0;
                    value = r_u32(0x800ADEAC);
                    w_u32(0x800ADEAC, value + 1);
                } else {
                    slot = r_u32(0x800ADEB0);
                    output = base ? base + 0x3980 + slot * 24 : 0;
                    value = r_u32(0x800ADEB0);
                    w_u32(0x800ADEB0, value + 1);
                }
                for (index = 0; index < 3; ++index) {
                    value = r_u32(event + 52 + index * 4);
                    w_u32(output + index * 4, value);
                }
                for (index = 0; index < 3; ++index) {
                    value = r_u32(event + 40 + index * 4);
                    w_u32(output + 12 + index * 4, value);
                }
            }
        }
        if (!(r_u8(event + 13) & 0x40) || r_u32(0x800ADCA4))
            continue;
        if (!update) {
            source = event + previous_bank * 40 + 80;
            bank = r_u32(0x800ADEFC);
            output = event + bank * 40 + 80;
            value = r_u16(source + 22);
            w_u16(output + 22, (uint16)value);
            value = r_u16(source + 14);
            w_u16(output + 14, (uint16)value);
            for (index = 0; index < 8; ++index) {
                value = r_u8(source + packet_bytes[index]);
                w_u8(output + packet_bytes[index], (uint8)value);
            }
        }
        value = r_u16(event + 40);
        second = r_u16(0x800A85D4);
        relative.vx = (sint16)(value - second);
        value = r_u16(event + 44);
        second = r_u16(0x800A85D8);
        relative.vy = (sint16)(value - second);
        value = r_u16(event + 48);
        second = r_u16(0x800A85DC);
        relative.vz = (sint16)(value - second);
        ApplyMatrix(&camera, &relative, &translation);
        camera.t[0] = translation.vx;
        camera.t[1] = translation.vy;
        camera.t[2] = translation.vz;
        SetRotMatrix(&projection);
        SetTransMatrix(&camera);
        source = r_u32(event + 8);
        t3_800827f8(source, 0x1F800000, depths, interpolation, projection_flags, 4);
        bank = r_u32(0x800ADEFC);
        t3_8006e44c(depths, event + bank * 40 + 80, ordering, event);
    }
    w_u16(0x800A9194, (uint16)count);
    return 0x800B0000;
}

MATRIX *t3_8003a460(const uint16 *angles, MATRIX *output)
{
    uint32 x_index, y_index, z_index;
    sint32 z;
    uint32 cos_x, cos_y, cos_z, sin_x, sin_y, sin_z;
    uint32 product_xz, product_xs, mixed;
    uint32 first[3], second[3], third[3];
    uint32 row_first, row_second, row_third;
    FUNCTION_MARKER(0x8003A460, "SLUS_004.02");
    z = (sint16)angles[2];
    x_index = ((uint32)angles[0] >> 3) & 0x1FFE;
    y_index = ((uint32)angles[1] >> 3) & 0x1FFE;
    z_index = ((uint32)z >> 3) & 0x1FFE;
    cos_z = (uint32)r_s16(0x8001F0C4 + z_index);
    cos_x = (uint32)r_s16(0x8001F0C4 + x_index);
    product_xz = cos_x * cos_z;
    cos_y = (uint32)r_s16(0x8001F0C4 + y_index);
    sin_x = (uint32)r_s16(0x8001E8C4 + x_index);
    sin_z = (uint32)r_s16(0x8001E8C4 + z_index);
    xport_gte_write_data(8, sin_x);
    xport_gte_write_data(9, cos_z);
    xport_gte_write_data(10, sin_z);
    xport_gte_write_data(11, cos_y);
    product_xs = cos_x * sin_z;
    product_xz = (uint32)((sint32)product_xz >> 12);
    xport_gte_execute(0x4B98003D);
    sin_y = (uint32)r_s16(0x8001E8C4 + y_index);
    first[0] = xport_gte_read_data(9);
    first[1] = xport_gte_read_data(10);
    first[2] = xport_gte_read_data(11);
    xport_gte_write_data(8, sin_y);
    xport_gte_write_data(9, first[0]);
    xport_gte_write_data(10, first[1]);
    xport_gte_write_data(11, product_xz);
    product_xs = (uint32)((sint32)product_xs >> 12);
    xport_gte_execute(0x4B98003D);
    mixed = product_xs * sin_y;
    output->m[1][2] = (sint16)(0 - first[2]);
    output->m[0][2] = (sint16)sin_y;
    second[0] = xport_gte_read_data(9);
    second[1] = xport_gte_read_data(10);
    second[2] = xport_gte_read_data(11);
    xport_gte_write_data(8, cos_y);
    xport_gte_write_data(9, cos_z);
    xport_gte_write_data(10, 0 - sin_z);
    xport_gte_write_data(11, cos_x);
    row_first = second[0] + product_xs;
    row_second = product_xz - second[1];
    xport_gte_execute(0x4B98003D);
    row_third = first[1] - second[2];
    output->m[1][0] = (sint16)row_first;
    output->m[1][1] = (sint16)row_second;
    output->m[2][0] = (sint16)row_third;
    mixed = (uint32)((sint32)mixed >> 12) + first[0];
    third[0] = xport_gte_read_data(9);
    third[1] = xport_gte_read_data(10);
    third[2] = xport_gte_read_data(11);
    output->m[2][1] = (sint16)mixed;
    output->m[0][0] = (sint16)third[0];
    output->m[0][1] = (sint16)third[1];
    output->m[2][2] = (sint16)third[2];
    return output;
}

uint32 t3_8003aaf4(uint32 actor)
{
    uint16 angles[3];
    uint32 value, vertical, lateral, longitudinal, enabled, divisor, numerator;
    uint32 matrix = actor + 0xF54;
    uint32 heading, mode, offset, cosine, sine, product, origin;
    FUNCTION_MARKER(0x8003AAF4, "SLUS_004.02");
    value = r_u16(actor + 12);
    angles[0] = (uint16)(0 - value);
    value = r_u16(actor + 44);
    angles[1] = (uint16)(value - 0x8000);
    value = r_u16(actor + 16);
    angles[2] = (uint16)(0 - value);
    t3_800329b8(actor, angles);
    if (r_u32(0x800954A4))
        w_u16(actor + 44, (uint16)(angles[1] - 0x8000));
    t3_8003a460(angles, (MATRIX *)psx_addr(matrix, 32));
    vertical = r_u16(actor + 38);
    lateral = r_u16(actor + 36);
    enabled = r_u32(actor + 0x181C);
    longitudinal = r_u16(actor + 40);
    if (enabled && (divisor = r_u32(actor + 0x1870)) != 0) {
        numerator = r_u32(actor + 0x1878) << 12;
        value = (uint32)((sint64)(sint32)numerator / (sint32)divisor);
        w_u16(actor + 0x186E, (uint16)value);
        vertical = t3_80075f9c(actor, (uint32)(sint32)(sint16)vertical);
        w_u16(actor + 0x1812, (uint16)vertical);
    } else {
        w_u16(actor + 0x186E, 4096);
        w_u16(actor + 0x1812, (uint16)vertical);
    }
    origin = r_u32(actor);
    heading = (uint32)r_s16(actor + 14);
    w_u32(matrix + 20, origin);
    value = r_u32(actor + 4);
    w_u32(matrix + 24, value);
    value = r_u32(actor + 8);
    w_u32(matrix + 28, value);
    heading = (uint32)((sint32)(0x8000 - heading) >> 4);
    mode = r_u8(actor + 184);
    if (!mode) {
        offset = (heading & 0xFFF) * 2;
        cosine = (uint32)r_s16(0x8001F0C4 + offset);
        sine = (uint32)r_s16(0x8001E8C4 + offset);
        lateral = (uint32)(sint32)(sint16)lateral;
        longitudinal = (uint32)(sint32)(sint16)longitudinal;
        product = lateral * cosine - longitudinal * sine;
        origin = r_u32(matrix + 20);
        value = (uint32)((sint32)product / 4096);
        w_u32(matrix + 20, origin + value);
        origin = r_u32(matrix + 24);
        w_u32(matrix + 24, origin + (uint32)(sint32)(sint16)vertical);
        sine = (uint32)r_s16(0x8001E8C4 + offset);
        product = lateral * sine;
        cosine = (uint32)r_s16(0x8001F0C4 + offset);
        product += longitudinal * cosine;
        origin = r_u32(matrix + 28);
        value = (uint32)((sint32)product / 4096);
        w_u32(matrix + 28, origin + value);
    } else if (mode == 2) {
        origin = r_u32(matrix + 24);
        w_u32(matrix + 24, origin + (uint32)(sint32)(sint16)vertical);
    }
    return t3_800325e4(actor);
}

uint32 t3_800325e4(uint32 actor)
{
    uint32 side = actor != 0x800A9228;
    uint32 mode, base, bank, frame, next_frame, record, following, odd, index, value;
    FUNCTION_MARKER(0x800325E4, "SLUS_004.02");
    mode = r_u32(0x8009BCA8);
    if (mode - 1 >= 7)
        return 0;
    if (mode == 2 || mode == 5)
        return 0x800329A0;
    if (mode == 3) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 31200 : 0;
        for (index = 0; index < 3; ++index) {
            value = r_u32(actor + index * 4);
            w_u32(record + 24 + index * 4, value);
        }
    }
    if (mode == 1 || mode == 3 || mode == 6) {
        frame = (uint32)(r_s32(0x8009BCBC) >> 1);
        next_frame = frame + 1;
        if ((sint32)next_frame < 0)
            next_frame = frame + 19201;
        next_frame = (uint32)((sint32)next_frame % 300);
        base = r_u32(0x800ADE68);
        bank = r_u32(0x8009BCAC);
        record = base ? base + bank * 31200 + side * 15600 + frame * 52 : 0;
        bank = r_u32(0x8009BCAC);
        base = r_u32(0x800ADE68);
        following = base ? base + bank * 31200 + side * 15600 + next_frame * 52 : 0;
        odd = r_u32(0x8009BCBC) & 1;
        for (index = 0; index < 3; ++index) {
            value = r_u32(record + 12 + index * 4);
            if (odd) {
                value += r_u32(following + 12 + index * 4);
                value = (uint32)((sint32)value / 2);
            }
            w_u32(actor + 0xF68 + index * 4, value);
        }
        if (r_u32(0x8009BCA8) == 6 && r_u32(0x8009BCBC) == 2 && r_s16(actor + 18) == 1)
            t3_8006953c(0x800A9228, 0x800AAAB4);
    } else if (mode == 4) {
        base = r_u32(0x800ADE68);
        record = base ? base + side * 52 + 31200 : 0;
        for (index = 0; index < 3; ++index) {
            value = r_u32(record + 12 + index * 4);
            w_u32(actor + 0xF68 + index * 4, value);
        }
    } else {
        base = r_u32(0x800ADE68);
        frame = r_u32(0x8009BCB8);
        record = base ? base + side * 15600 + frame * 52 : 0;
        for (index = 0; index < 3; ++index) {
            value = r_u32(actor + 0xF68 + index * 4);
            w_u32(record + 12 + index * 4, value);
        }
        for (index = 0; index < 3; ++index) {
            value = r_u32(actor + index * 4);
            w_u32(record + 24 + index * 4, value);
        }
        return value;
    }
    for (index = 0; index < 3; ++index) {
        value = r_u32(record + 24 + index * 4);
        w_u32(actor + index * 4, value);
    }
    return value;
}

uint32 t3_8004aa90(uint32 update)
{
    uint32 row, record, counter, item, object, axis, offset, origin, bank, flags;
    FUNCTION_MARKER(0x8004AA90, "SLUS_004.02");
    update = (uint32)(sint32)(sint16)update;
    for (row = 0; row < 3; ++row) {
        record = 0x8009E850 + row * 880;
        if (r_s16(record) <= 0)
            continue;
        flags = 0;
        if (update) {
            counter = r_u16(record);
            if (!(counter & 1)) {
                item = (uint32)((sint16)counter / 2);
                if ((sint32)item < 6) {
                    object = record + (item << 6) + 16;
                    t3_800766b4(object, 3, 3);
                    w_u16(object + 2, 1);
                    for (axis = 0; axis < 3; ++axis) {
                        offset = (uint32)r_s16(0x8001E818 + item * 8 + axis * 2);
                        origin = r_u32(record + 4 + axis * 4);
                        w_u32(object + 36 + axis * 4, origin + (offset << 7));
                    }
                }
            }
            counter = r_u16(record);
            w_u16(record, (uint16)(counter + 1));
            flags = 1;
        }
        bank = r_u32(0x800ADEFC);
        if (t3_80076e70(6, record + 16, record + bank * 240 + 400, flags | 2) == 1)
            w_u16(record, 0);
    }
    return 0;
}

uint32 t3_80076e70(uint32 count, uint32 objects, uint32 packets, uint32 flags)
{
    SVECTOR vertex = {0, 0, 0, 0};
    uint32 screen, row = 0, finished = 1;
    uint32 object, packet, value, second, third, x, y, z, depth, divisor;
    uint32 width, height, left, top, right, bottom, page, clut, u, v, color;
    uint32 owner, ordering, tag, offset, velocity_x, velocity_y, velocity_z;
    uint32 counter, age, cursor, limit, step, base;
    FUNCTION_MARKER(0x80076E70, "SLUS_004.02");
    if ((sint32)count <= 0)
        return 1;
    do {
        object = objects + 2;
        packet = packets + 6;
        if (r_s16(object) > 0) {
            y = r_u32(object + 38);
            if ((sint32)y >= -31999)
                y = (uint32)-32000;
            x = r_u32(object + 34);
            w_u32(object + 38, y);
            z = r_u32(object + 42);
            t3_8004adc4((uint32)((sint32)x / 128), (uint32)((sint32)y / 128), (uint32)((sint32)z / 128));
            depth = t3_80036ad4(&vertex, &screen);
            if (depth) {
                divisor = depth << 2;
                value = (uint32)r_s16(object + 4) * 11787;
                width = (uint32)((sint64)(sint32)value / (sint32)divisor);
                value = (uint32)r_s16(object + 6) * 11787;
                value *= 1780;
                left = (uint16)screen;
                top = (uint32)(sint32)(sint16)(screen >> 16);
                value = (uint32)((sint32)value / 1024);
                height = (uint32)((sint64)(sint32)value / (sint32)divisor);
                if ((width & 0xFFFF) >= 369)
                    width = 368;
                if ((height & 0xFFFF) >= 481)
                    height = 480;
                left -= (width & 0xFFFF) >> 1;
                top -= (height & 0xFFFF) >> 1;
                right = left + width;
                w_u16(packet + 4, (uint16)top);
                w_u16(packet + 12, (uint16)top);
                bottom = top + height;
                w_u16(packet + 2, (uint16)left);
                w_u16(packet + 10, (uint16)right);
                w_u16(packet + 18, (uint16)left);
                w_u16(packet + 20, (uint16)bottom);
                w_u16(packet + 26, (uint16)right);
                w_u16(packet + 28, (uint16)bottom);
                page = r_u16(object + 60);
                value = r_u16(object + 24);
                second = r_u16(object + 22);
                page = ((page & 3) << 5) | ((value & 0x100) >> 4) | ((second & 0x3FF) >> 6);
                w_u16(packet + 16, (uint16)page);
                clut = r_u16(object + 32);
                value = r_u16(object + 30);
                clut = (clut << 6) | ((value >> 4) & 63);
                w_u16(packet + 8, (uint16)clut);
                value = (uint32)r_s16(object + 22);
                value = (uint32)((sint32)value % 64);
                u = (value << 16) >> 14;
                value = (uint32)r_s16(object + 24);
                v = (uint32)((sint32)value % 256);
                if (r_s16(object + 58)) {
                    value = r_u8(object + 4);
                    w_u8(packet + 7, (uint8)v);
                    w_u8(packet + 14, (uint8)u);
                    w_u8(packet + 15, (uint8)v);
                    w_u8(packet + 6, (uint8)(value + u - 1));
                    value = r_u8(object + 4);
                    w_u8(packet + 22, (uint8)(value + u - 1));
                    value = r_u8(object + 6);
                    w_u8(packet + 30, (uint8)u);
                    w_u8(packet + 23, (uint8)(value + v - 1));
                } else {
                    w_u8(packet + 6, (uint8)u);
                    w_u8(packet + 7, (uint8)v);
                    value = r_u8(object + 4);
                    w_u8(packet + 15, (uint8)v);
                    w_u8(packet + 22, (uint8)u);
                    w_u8(packet + 14, (uint8)(value + u - 1));
                    value = r_u8(object + 6);
                    w_u8(packet + 23, (uint8)(value + v - 1));
                    value = r_u8(object + 4);
                    w_u8(packet + 30, (uint8)(value + u - 1));
                }
                value = r_u8(object + 6);
                w_u8(packet + 31, (uint8)(value + v - 1));
                color = 128;
                if (r_s16(object + 60) == 1) {
                    value = (uint32)r_s16(object);
                    second = (uint32)r_s16(object + 2);
                    value = value - second + 10;
                    if ((sint32)value > 0) {
                        value *= 39321;
                        color = (uint32)-128 - (uint32)((sint32)value / 4096);
                    }
                }
                w_u8(packet - 2, (uint8)color);
                w_u8(packet - 1, (uint8)color);
                w_u8(packet, (uint8)color);
                offset = flags & 2 ? 1 : (uint32)((sint32)(depth - 110) >> 3);
                value = r_u32(0x800ADCA4);
                offset = (offset & 0x3FF) * 4;
                if (!value) {
                    owner = r_u32(0x800A8C54);
                    ordering = r_u32(owner + 4);
                    tag = r_u32(packets);
                    value = r_u32(ordering + offset);
                    w_u32(packets, (tag & 0xFF000000) | (value & 0xFFFFFF));
                    ordering = r_u32(owner + 4) + offset;
                    value = r_u32(ordering);
                    w_u32(ordering, (value & 0xFF000000) | (packets & 0xFFFFFF));
                }
            }
            if (flags & 1) {
                value = r_u32(object + 46);
                second = r_u32(object + 50);
                value = (uint32)((sint32)(value * 118) >> 7);
                w_u32(object + 46, value);
                velocity_x = r_u32(object + 46);
                third = r_u32(object + 54);
                second = (uint32)((sint32)(second * 118) >> 7);
                w_u32(object + 50, second);
                x = r_u32(object + 34);
                third = (uint32)((sint32)(third * 118) >> 7);
                w_u32(object + 54, third);
                y = r_u32(object + 38);
                velocity_z = r_u32(object + 54);
                velocity_y = r_u32(object + 50);
                w_u32(object + 34, x + velocity_x);
                z = r_u32(object + 42);
                w_u32(object + 38, y + velocity_y);
                w_u32(object + 42, z + velocity_z);
                counter = r_u16(objects) + 1;
                w_u16(objects, (uint16)counter);
                if ((sint32)(counter << 16) > 0) {
                    w_u16(objects, 0);
                    age = r_u16(object);
                    u = r_u16(object + 22);
                    step = r_u16(object + 8);
                    w_u16(object, (uint16)(age + 1));
                    cursor = r_u16(object + 12);
                    w_u16(object + 22, (uint16)(u + step));
                    limit = (uint32)r_s16(object + 10);
                    ++cursor;
                    w_u16(object + 12, (uint16)cursor);
                    if ((sint16)cursor >= (sint32)limit) {
                        v = r_u16(object + 24);
                        step = r_u16(object + 6);
                        cursor = r_u16(object + 16) + 1;
                        w_u16(object + 16, (uint16)cursor);
                        w_u16(object + 24, (uint16)(v + step));
                        limit = (uint32)r_s16(object + 14);
                        if ((sint16)cursor >= (sint32)limit) {
                            value = r_u16(object + 8);
                            second = r_u16(object + 10);
                            base = r_u16(object + 28);
                            third = r_u16(object + 26);
                            w_u16(object + 16, 0);
                            w_u16(object + 24, (uint16)base);
                            w_u16(object + 26, (uint16)(third + value * second));
                        }
                        value = r_u16(object + 26);
                        w_u16(object + 12, 0);
                        w_u16(object + 22, (uint16)value);
                    }
                }
                age = (uint32)r_s16(object);
                limit = (uint32)r_s16(object + 2);
                if ((sint32)limit < (sint32)age)
                    w_u16(object, 0);
            }
            if (r_s16(object) > 0)
                finished = 0;
        }
        ++row;
        objects += 64;
        packets += 40;
    } while ((sint32)(sint16)row < (sint32)count);
    return finished;
}
