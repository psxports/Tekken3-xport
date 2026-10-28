#include "game_functions.h"

uint32 t3_80066148(uint32 first, uint32 second)
{
    uint32 target_x, origin_x, target_z, origin_z, angle;
    sint32 first_order, second_order;
    FUNCTION_MARKER(0x80066148, "SLUS_004.02");
    target_x = r_u32(0x800ADCB8);
    origin_x = r_u32(0x800ADCA8);
    first_order = r_s16(first + 0x1278);
    target_z = r_u32(0x800ADCC0);
    origin_z = r_u32(0x800ADCB0);
    second_order = r_s16(second + 0x1278);
    angle = t3_8004b39c(target_x - origin_x, target_z - origin_z);
    angle = (uint32)(sint32)(sint16)angle;
    if (first_order >= second_order)
        angle = (angle + 2048) & 0xFFF;
    return angle << 6;
}

uint32 t3_800628c4(uint32 object)
{
    uint32 context, frame;
    sint32 animation;
    FUNCTION_MARKER(0x800628C4, "SLUS_004.02");
    context = r_u32(0x800A867C);
    frame = r_u16(context + 0x58);
    animation = r_s16(context + 0xA0);
    if (animation == 0x2C7 && r_s32(object + 0x18) < 3) {
        if ((sint16)frame >= 16)
            return 0x8000;
        w_u16(object + 0x68, 0xFFFF);
    }
    return 0xFFFFFFFF;
}

uint32 t3_8006299c(uint32 object)
{
    uint32 actor, frame;
    FUNCTION_MARKER(0x8006299C, "SLUS_004.02");
    actor = r_u32(0x800AFA5C);
    if (r_s16(actor + 0xA0) != 0xDE3)
        return 0xFFFFFFFF;
    frame = r_u16(actor + 0x58);
    if (frame - 13 < 5 && r_s16(object + 0x20A) < 10)
        return 0xFFFFFFFF;
    return (randPSX() & 15) ? 0x2000 : 0xFFFFFFFF;
}

uint32 t3_8006307c(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x8006307C, "SLUS_004.02");
    t3_80062a10();
    t3_800690a0(first, second);
    t3_80065ed8(first, second);
    t3_800666e4();
    return t3_80069708();
}

uint32 t3_8006460c(uint32 index)
{
    uint32 source, first, second, third, fourth;
    FUNCTION_MARKER(0x8006460C, "SLUS_004.02");
    source = 0x800A02A8 + index * 24;
    first = r_u32(source);
    second = r_u32(source + 4);
    third = r_u32(source + 8);
    fourth = r_u32(source + 12);
    w_u32(0x800A0308, first);
    w_u32(0x800A030C, second);
    w_u32(0x800A0310, third);
    w_u32(0x800A0314, fourth);
    first = r_u32(source + 16);
    second = r_u32(source + 20);
    w_u32(0x800A0318, first);
    w_u32(0x800A031C, second);
    return source;
}

uint32 t3_8004c1cc(void)
{
    uint32 result, attempted = 0;
    FUNCTION_MARKER(0x8004C1CC, "SLUS_004.02");
    for (;;) {
        result = t3_8004c51c(0);
        if (result != 3 || attempted)
            return result;
        format((const char *)psx_addr(0x800219CC, 6));
        attempted = 1;
    }
}

uint32 t3_8004da5c(uint32 ordering, uint32 packet, uint32 first, uint32 second, uint32 third)
{
    uint32 tag;
    FUNCTION_MARKER(0x8004DA5C, "SLUS_004.02");
    w_u32(packet + 4, first);
    w_u32(packet + 8, second);
    w_u32(packet + 12, third);
    tag = r_u32(ordering);
    w_u32(packet, (tag & 0xFFFFFF) | 0x03000000);
    tag = r_u32(ordering);
    w_u32(ordering, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    return packet + 16;
}

uint32 t3_8004ff20(uint32 state)
{
    uint32 previous;
    FUNCTION_MARKER(0x8004FF20, "SLUS_004.02");
    t3_8002960c(1);
    previous = r_u8(0x800AE204);
    w_u16(0x800AE204, 18);
    w_u8(0x80097F38, state);
    w_u16(0x800AE224, 0);
    w_u8(0x80097F39, previous);
    return 0x800B0000;
}

void t3_8004ff74(void)
{
    uint32 state;
    FUNCTION_MARKER(0x8004FF74, "SLUS_004.02");
    t3_8002960c(0);
    t3_80028f5c();
    t3_8006c588(31);
    t3_800697d4(-1);
    state = r_u8(0x80097F38);
    t3_80052bb4(state == 19 ? 8 : 9);
    t3_80052c30(state, 0);
}

uint32 t3_80051660(uint32 selection)
{
    uint32 entry, first, second, third;
    sint32 remainder;
    FUNCTION_MARKER(0x80051660, "SLUS_004.02");
    remainder = (sint32)selection % 22;
    entry = 0x80097F44 + ((uint32)remainder << 3);
    first = r_u16(entry + 0x108);
    second = r_u16(entry + 0x10C);
    third = r_u16(entry + 0x10E);
    return first + second + third;
}

uint32 t3_800592fc(uint32 object)
{
    uint32 flags;
    FUNCTION_MARKER(0x800592FC, "SLUS_004.02");
    w_u16(object + 0x40, 40);
    flags = r_u16(object + 2);
    w_u16(object + 0x3A, 0);
    w_u16(object + 0x38, 0);
    w_u16(object + 0x48, 0xFFFF);
    return ((flags & 1) == 0) << 13;
}

uint32 t3_80059394(uint32 object)
{
    FUNCTION_MARKER(0x80059394, "SLUS_004.02");
    w_u32(object + 0x7C, 0x800613CC);
    return 0x800613CC;
}

uint32 t3_800593a4(uint32 object)
{
    FUNCTION_MARKER(0x800593A4, "SLUS_004.02");
    return t3_8006183c(object, 0x80022C46);
}

uint32 t3_80067f08(void)
{
    uint32 context;
    FUNCTION_MARKER(0x80067F08, "SLUS_004.02");
    context = r_u32(0x800A0438);
    w_u16(0x800A8CA4, 1);
    return (uint32)r_s16(context + 4);
}

uint32 t3_80067f28(void)
{
    uint32 context;
    FUNCTION_MARKER(0x80067F28, "SLUS_004.02");
    context = r_u32(0x800A0438);
    return (uint32)r_s16(context + 6);
}

uint32 t3_80067f40(void)
{
    uint32 context, first, second;
    FUNCTION_MARKER(0x80067F40, "SLUS_004.02");
    context = r_u32(0x800A0438);
    first = (uint32)r_s16(context + 2);
    second = (uint32)r_s16(context + 6);
    return first - second;
}

uint32 t3_8006df08(uint32 x, uint32 z)
{
    uint32 angle;
    FUNCTION_MARKER(0x8006DF08, "SLUS_004.02");
    angle = t3_8004b39c(x, z);
    return ((uint32)(sint32)(sint16)angle - 1024) & 0xFFF;
}

uint32 t3_8006e51c(uint32 side, uint32 index)
{
    uint32 base;
    FUNCTION_MARKER(0x8006E51C, "SLUS_004.02");
    base = r_u32(0x800ADC80);
    return base ? base + side * 1920 + 12800 + index * 24 : 0;
}

uint32 t3_800724a4(uint32 object)
{
    FUNCTION_MARKER(0x800724A4, "SLUS_004.02");
    w_u8(object + 14, 4);
    w_u32(object + 36, 0);
    w_u32(object + 32, 0);
    w_u32(object + 28, 0);
    w_u32(object + 72, 0xFFFFFFFF);
    return 0xFFFFFFFF;
}

uint32 t3_800484c8(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800484C8, "SLUS_004.02");
    value = r_u8(0x800AFAA0);
    if (!value) {
        value = 2;
        w_u32(0x8009E778, value);
    }
    return value;
}

uint32 t3_8004a678(void)
{
    FUNCTION_MARKER(0x8004A678, "SLUS_004.02");
    w_u32(0x8009E830, 0);
    return 0x800A0000;
}

uint32 t3_8004a684(uint32 count)
{
    uint32 current, boundary, position, result;
    FUNCTION_MARKER(0x8004A684, "SLUS_004.02");
    current = r_u32(0x8009E834);
    boundary = r_u32(0x8009E844);
    position = current - (count << 3);
    result = position - boundary;
    w_u32(0x8009E838, position);
    if ((sint32)result < 0) {
        result = position + 2400;
        w_u32(0x8009E838, result);
    }
    return result;
}

uint32 t3_8004c390(void)
{
    FUNCTION_MARKER(0x8004C390, "SLUS_004.02");
    t3_800564b8();
    w_u8(0x800ADF60, 1);
    return 1;
}

uint32 t3_8004f89c(void)
{
    FUNCTION_MARKER(0x8004F89C, "SLUS_004.02");
    return r_u8(0x80097F3E) != 3;
}

uint32 t3_8004ffe0(void)
{
    FUNCTION_MARKER(0x8004FFE0, "SLUS_004.02");
    w_u32(0x800AE214, 808);
    return 808;
}

uint32 t3_80050190(void)
{
    FUNCTION_MARKER(0x80050190, "SLUS_004.02");
    t3_8002960c(1);
    w_u16(0x800AE204, 16);
    w_u16(0x800AE224, 0);
    return 0x800B0000;
}

void t3_80052938(void)
{
    FUNCTION_MARKER(0x80052938, "SLUS_004.02");
}

uint32 t3_80052990(uint32 value, uint32 selection)
{
    FUNCTION_MARKER(0x80052990, "SLUS_004.02");
    return t3_8006c470(selection == 0xFFFFFFFF ? 73 : selection + 74, value);
}

void t3_800529c4(void)
{
    FUNCTION_MARKER(0x800529C4, "SLUS_004.02");
}

uint32 t3_800325bc(void)
{
    FUNCTION_MARKER(0x800325BC, "SLUS_004.02");
    return r_u32(0x8009BCA8) == 5;
}

uint32 t3_800325d4(void)
{
    FUNCTION_MARKER(0x800325D4, "SLUS_004.02");
    w_u32(0x8009BCC8, 1);
    return 1;
}

uint32 t3_80033788(void)
{
    FUNCTION_MARKER(0x80033788, "SLUS_004.02");
    return (r_u32(0x8009BCBC) ^ 1) & 1;
}

uint32 t3_8003478c(uint32 object, uint32 value)
{
    FUNCTION_MARKER(0x8003478C, "SLUS_004.02");
    return t3_800344a4(object, 3, 0, value);
}

uint32 t3_80037814(uint32 matrix)
{
    uint32 lighting;
    FUNCTION_MARKER(0x80037814, "SLUS_004.02");
    lighting = t3_80039e38();
    MulMatrix0((MATRIX *)psx_addr(lighting, 32), (MATRIX *)psx_addr(matrix, 32),
               (MATRIX *)psx_addr(0x1F800020, 32));
    SetLightMatrix((MATRIX *)psx_addr(0x1F800020, 32));
    return 0x1F800020;
}

void t3_8003785c_host(uint32 left, const MATRIX *right)
{
    MATRIX rotation;
    VECTOR input;
    VECTOR output;
    uint32 value, translation;
    SetRotMatrix((MATRIX *)psx_addr(left, 32));
    input.vx = right->t[0];
    input.vy = right->t[1];
    input.vz = right->t[2];
    ApplyRotMatrixLV(&input, &output);
    w_u32(0x1F800034, (uint32)output.vx);
    w_u32(0x1F800038, (uint32)output.vy);
    w_u32(0x1F80003C, (uint32)output.vz);
    value = r_u32(0x1F800034);
    translation = r_u32(left + 20);
    w_u32(0x1F800034, value + translation);
    value = r_u32(0x1F800038);
    translation = r_u32(left + 24);
    w_u32(0x1F800038, value + translation);
    value = r_u32(0x1F80003C);
    translation = r_u32(left + 28);
    w_u32(0x1F80003C, value + translation);
    SetTransMatrix((MATRIX *)psx_addr(0x1F800020, 32));
    MulRotMatrix0(right, &rotation);
    SetRotMatrix(&rotation);
}

uint32 t3_8003785c(uint32 left, uint32 right)
{
    FUNCTION_MARKER(0x8003785C, "SLUS_004.02");
    t3_8003785c_host(left, (const MATRIX *)psx_addr(right, 32));
    return right;
}

uint32 t3_80038cd8(uint32 record)
{
    FUNCTION_MARKER(0x80038CD8, "SLUS_004.02");
    return r_u16(record) & 0x3FF;
}

uint32 t3_80039ec4(uint32 index)
{
    FUNCTION_MARKER(0x80039EC4, "SLUS_004.02");
    return r_u32(0x80096C5C + (index + 1) * 28 + 24);
}

uint32 t3_8003a2c8(void)
{
    uint32 entry, red, green, blue;
    FUNCTION_MARKER(0x8003A2C8, "SLUS_004.02");
    entry = 0x8001A114 + (r_u32(0x800987FC) & 31) * 4;
    red = r_u8(entry);
    green = r_u8(entry + 1);
    blue = r_u8(entry + 2);
    SetBackColor((sint32)red, (sint32)green, (sint32)blue);
    return 0x80020000;
}

void t3_8003a310(void)
{
    FUNCTION_MARKER(0x8003A310, "SLUS_004.02");
    SetBackColor(0, 0, 0);
}

uint32 t3_80041b70(uint32 actor, uint32 opponent)
{
    uint32 identity, sound;
    FUNCTION_MARKER(0x80041B70, "SLUS_004.02");
    if (r_s16(actor + 0x5E) < 7 || r_s16(opponent + 0x5E) < 16)
        sound = 0x705D;
    else
        sound = (r_u32(0x800AE218) & 3) ? 0x7098 : 0x7099;
    identity = (uint32)r_s16(opponent + 0x12);
    t3_8003352c(3, identity, sound);
    return t3_80040df4(identity, sound);
}

uint32 t3_80041c40(uint32 actor, uint32 opponent)
{
    uint32 random, index, identity, sound;
    FUNCTION_MARKER(0x80041C40, "SLUS_004.02");
    (void)actor;
    random = (uint32)randPSX() & 255;
    index = (random * 5) >> 8;
    identity = (uint32)r_s16(opponent + 0x12);
    sound = (uint32)r_s16(0x8001A6F0 + index * 2);
    t3_8003352c(3, identity, sound);
    return t3_80040df4(identity, sound);
}

uint32 t3_80040ea0(uint32 actor, uint32 opponent)
{
    uint32 animation, events, metadata, health, identity, other_identity;
    uint32 voice, other_voice, descriptor, cooldown, selected, category, lower;
    uint32 table, random, product, sound, entry, word, owner, owner_voice;
    uint32 frame, value, active, event_index, index, base;
    sint32 count, opponent_kind, signed_sound;
    int voiced = 0, knockout = 0;
    FUNCTION_MARKER(0x80040EA0, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    identity = (uint32)r_s16(actor + 0x12);
    other_identity = (uint32)r_s16(opponent + 0x12);
    events = r_u32(animation + 0x1C);
    voice = (uint32)r_s16(actor + 0x1A);
    other_voice = (uint32)r_s16(opponent + 0x1A);
    w_u16(0x800971F0 + identity * 2, 0);
    t3_8004b720(actor);
    if (r_u8(actor + 0xDE)) {
        table = r_u32(0x8001AD54 + voice * 24);
        random = (uint32)randPSX();
        count = r_s16(table + 4);
        product = (random & 255) * (uint32)count;
        sound = (uint32)((sint32)product >> 8) | 0x4000;
        signed_sound = (sint16)sound;
        if (!t3_80075914(identity, (uint32)(signed_sound >> 12)))
            t3_8003352c(2, identity, (uint32)signed_sound);
        t3_80040c90(identity, voice, (uint32)signed_sound);
        voiced = 1;
    }
    animation = r_u32(actor + 0x54);
    health = r_u32(opponent + 0x3F4);
    metadata = r_u32(animation + 0x28);
    if (!health && r_u8(opponent + 0xD0)) {
        sound = r_u8(metadata) < 11 ? 0x7044 : 0x7048;
        t3_8003352c(3, identity, sound);
        t3_80040df4(identity, sound);
        knockout = 1;
    }
    cooldown = 0x800971E8 + identity * 2;
    if (events) {
        for (event_index = 0; event_index < 3; ++event_index, events += 2) {
            selected = r_u16(events);
            if (selected == 0xFFFF)
                break;
            category = selected >> 12;
            lower = selected & 0xFFF;
            switch (category) {
            case 1:
            case 7:
                animation = r_u32(actor + 0x54);
                active = r_u8(actor + 0xD1);
                metadata = r_u32(animation + 0x28);
                if (!active || knockout)
                    break;
                if (r_u8(opponent + 0xCF)) {
                    opponent_kind = r_s16(opponent + 0x18);
                    if (opponent_kind == 15 &&
                        r_u32(0x800954A8 + (uint32)r_s16(opponent + 0x12) * 4))
                        sound = r_u8(metadata) < 12 ? 0x707B : 0x707A;
                    else
                        sound = r_u8(metadata) < 12 ? 0x704D : 0x704E;
                    t3_8003352c(3, other_identity, sound);
                    t3_80040df4(other_identity, sound);
                    break;
                }
                t3_80041998(opponent);
                opponent_kind = r_s16(opponent + 0x18);
                if (opponent_kind == 16) {
                    t3_80041b70(actor, opponent);
                    break;
                }
                if (opponent_kind == 15 &&
                    r_u32(0x800954A8 + (uint32)r_s16(opponent + 0x12) * 4)) {
                    t3_80041c40(actor, opponent);
                    break;
                }
                if (selected == 0x7000 || selected == 0x7041 ||
                    selected == 0x7042 || selected == 0x7045) {
                    t3_80041cb4(actor);
                    break;
                }
                goto direct_event_sound;
            case 2:
            case 3:
                if (r_s16(actor + 0x58) == 1 && r_u32(actor + 0x3F4) &&
                    !r_s16(cooldown) && !voiced) {
                    signed_sound = (sint16)selected;
                    if (!t3_80075914(identity, (uint32)(signed_sound >> 12)))
                        t3_8003352c(2, identity, (uint32)signed_sound);
                    t3_80040c90(identity, voice, (uint32)signed_sound);
                    if (category == 2) {
                        value = r_u16(0x8001AD54 + voice * 24 + 12);
                        w_u16(cooldown, value);
                    } else {
                        voiced = 1;
                    }
                }
                break;
            case 4:
            case 5:
direct_event_sound:
                signed_sound = (sint16)selected;
                t3_8003352c(3, identity, (uint32)signed_sound);
                t3_80040df4(identity, (uint32)signed_sound);
                break;
            case 8:
                if (lower && r_s16(actor + 0x72) != (sint32)lower) {
                    frame = r_u16(actor + 0x58);
                    w_u16(actor + 0x70, lower);
                    w_u16(actor + 0x72, lower);
                    w_u16(actor + 0x11E, 0);
                    w_u16(actor + 0x120, frame - 1);
                }
                break;
            case 9:
                t3_800419b0(actor, opponent, lower);
                break;
            default:
                break;
            }
        }
    }
    if (r_u8(actor + 0xCE))
        t3_80041998(actor);
    if (r_s16(actor + 0x70) != -1 && t3_8002bb88()) {
        index = (uint32)r_s16(actor + 0x70);
        frame = r_u16(actor + 0x120);
        table = r_u32(0x800971F8 + index * 4);
        w_u16(actor + 0x120, frame + 1);
        for (;;) {
            index = (uint32)r_s16(actor + 0x11E);
            entry = table + index * 4;
            word = r_u32(entry);
            lower = word & 0xFFF;
            if (!lower) {
                t3_80041998(actor);
                goto decrement_timers;
            }
            frame = (uint32)r_s16(actor + 0x120);
            if ((sint32)frame < (sint32)lower)
                goto decrement_timers;
            if (frame == lower) {
                category = (word >> 12) & 15;
                if (!category) {
                    signed_sound = r_s16(entry + 2);
                    t3_8003352c(1, 0, (uint32)signed_sound);
                    t3_80040c00((uint32)signed_sound);
                } else if (category == 1 || category == 2) {
                    sound = r_u16(entry + 2);
                    owner = category == 1 ? identity : other_identity;
                    owner_voice = category == 1 ? voice : other_voice;
                    signed_sound = (sint16)sound;
                    if ((sound >> 12) - 2u < 4) {
                        if (!t3_80075914(owner, (uint32)(signed_sound >> 12)))
                            t3_8003352c(2, owner, (uint32)signed_sound);
                        t3_80040c90(owner, owner_voice, (uint32)signed_sound);
                    } else {
                        t3_8003352c(3, owner, (uint32)signed_sound);
                        t3_80040df4(owner, (uint32)signed_sound);
                    }
                } else if (category == 3) {
                    value = r_u16(entry + 2);
                    t3_800419b0(actor, opponent, value & 0xFFF);
                }
            }
            index = r_u16(actor + 0x11E);
            w_u16(actor + 0x11E, index + 1);
        }
    }
    if (!voiced && r_u32(actor + 0x3F4) && r_u8(actor + 0xD0)) {
        value = r_u32(0x800971DC + identity * 4);
        active = r_u32(0x800AFA4C);
        if (!value && (active & 1)) {
            opponent_kind = r_s16(opponent + 0x5E);
            table = r_u32(0x8001AD54 + voice * 24 + 8);
            base = 2;
            if (opponent_kind < 20) {
                count = r_s16(table);
            } else {
                base = (uint32)r_s16(table);
                count = r_s16(table + 2);
                base += 2;
            }
            voiced = 1;
            if (count) {
                random = (uint32)randPSX();
                product = (random & 255) * (uint32)count;
                index = base + (uint32)((sint32)product >> 8);
                sound = r_u16(table + index * 2);
                signed_sound = (sint16)sound;
                if (!t3_80075914(identity, (uint32)(signed_sound >> 12)))
                    t3_8003352c(2, identity, (uint32)signed_sound);
                t3_80040c90(identity, voice, (uint32)signed_sound);
                w_u32(0x800971DC + identity * 4, 40);
                voiced = 1;
            }
        }
    }
    if (!r_s16(cooldown) && !voiced && r_s16(actor + 0x5E) &&
        !r_s16(actor + 0x74) && r_u8(actor + 0xDA) && !(randPSX() & 3)) {
        descriptor = 0x8001AD54 + voice * 24;
        table = r_u32(descriptor + 4);
        random = (uint32)randPSX();
        count = r_s16(table);
        product = (uint32)count * (random & 255);
        index = (uint32)((sint32)product >> 8);
        sound = r_u16(table + index * 2 + 2);
        signed_sound = (sint16)sound;
        if (!t3_80075914(identity, (uint32)(signed_sound >> 12)))
            t3_8003352c(2, identity, (uint32)signed_sound);
        t3_80040c90(identity, voice, (uint32)signed_sound);
        value = r_u16(descriptor + 12);
        w_u16(cooldown, value);
    }
    if (!r_s16(0x800971F0 + identity * 2)) {
        animation = r_u32(actor + 0x54);
        frame = (uint32)r_s16(actor + 0x58);
        value = r_u8(animation + 0x19);
        if (frame == value && frame != 1) {
            t3_8003352c(3, identity, 0x1053);
            t3_80040df4(identity, 0x1053);
        }
        if (r_u8(actor + 0xDC) && !r_s16(0x800971F0 + identity * 2)) {
            t3_8003352c(3, identity, 0x1054);
            t3_80040df4(identity, 0x1054);
        }
        if (r_u8(actor + 0xDD) && !r_s16(0x800971F0 + identity * 2)) {
            t3_8003352c(3, identity, 0x104F);
            t3_80040df4(identity, 0x104F);
        }
    }
    animation = r_u32(actor + 0x54);
    value = r_u8(animation + 0x2D);
    frame = (uint32)r_s16(actor + 0x58);
    if (frame == value - 1)
        t3_80041b30(actor);
decrement_timers:
    count = r_s16(cooldown);
    value = r_u16(cooldown);
    if (count > 0)
        w_u16(cooldown, value - 1);
    value = r_u32(0x800971DC + identity * 4);
    if ((sint32)value > 0)
        w_u32(0x800971DC + identity * 4, value - 1);
    return value - 1;
}

uint32 t3_8002ae58(void)
{
    uint32 first, second, third, value, mode, opponent, frame0, frame1;
    const uint32 actor0 = 0x800A9228, actor1 = 0x800AAAB4, actor2 = 0x800AC340;
    FUNCTION_MARKER(0x8002AE58, "SLUS_004.02");
    if (r_u32(0x800AFA88) == 8) {
        t3_bns_02_800b59dc(&first, &second, &third);
    } else {
        if (!r_u8(actor0 + 197)) {
            first = actor0;
            second = actor1;
        } else {
            first = actor1;
            second = actor0;
        }
        third = actor2;
        value = r_u8(second + 30);
        w_u8(first + 32, (uint8)value);
        value = r_u8(first + 30);
        w_u8(second + 32, (uint8)value);
        value = r_u8(second + 30);
        w_u8(first + 31, (uint8)value);
        value = r_u8(first + 30);
        w_u8(second + 31, (uint8)value);
    }
    if (!r_u32(0x800ADCA4))
        t3_8002b798();
    value = r_u32(0x80095468);
    w_u32(0x8009549C, 1);
    if (value || r_u32(0x80095494)) {
        if (r_u32(0x800954A4)) {
            t3_80048240(actor0, actor1);
            t3_8002ba64(actor0, actor1, actor2);
            t3_8006d7cc();
            t3_8006f0f4(actor0, 0);
            t3_8006f0f4(actor1, 0);
            t3_8006f5c0(0);
            t3_8004a6bc(0);
        } else {
            w_u32(actor0 + 0x12D0, 1);
            w_u32(actor1 + 0x12D0, 1);
            t3_8003d6b0(actor0, actor1);
            t3_80048240(actor0, actor1);
            t3_8002ba64(actor0, actor1, actor2);
            t3_8006d7cc();
            t3_8006f5c0(0);
            mode = r_u32(0x800AFA88);
            if (mode == 8) {
                t3_bns_02_800b1294();
                mode = r_u32(0x800AFA88);
            }
            if (mode == 7)
                t3_bns_03_800b094c(actor0, actor1, 0);
            if (!r_u8(0x800AFA9B) && r_u32(0x800AFA88) != 5)
                t3_80078008(r_u32(0x80095494));
            t3_8004ac24(0);
            t3_8007745c(0);
            t3_8002bce8(actor0);
            t3_8002bce8(actor1);
            if (r_u32(0x800AFA88) == 8)
                t3_8002bce8(actor2);
            t3_80040ad0(actor0);
        }
        goto finish;
    }
    t3_8003ebfc();
    value = t3_80032178();
    w_u32(0x800954A4, value);
    t3_8003cb84();
    if (r_u32(0x800954A4)) {
        value = t3_80033788();
        w_u32(0x8009549C, value);
        if (r_u32(0x800AFA88) != 5 && (t3_8002b9d4() & 0x800)) {
            t3_800325d4();
            w_u32(0x80095480, 1);
        }
        if (r_u32(0x800AFA88) == 7)
            t3_800630d8_host(actor0, actor1, 0x8009546C, 1);
        else
            t3_80069414(actor0, actor1);
        t3_8004ad34();
        t3_80048240(actor0, actor1);
        t3_8002ba64(actor0, actor1, actor2);
        t3_8006d7cc();
        t3_8006f0f4(actor0, r_u32(0x8009549C));
        t3_8006f0f4(actor1, r_u32(0x8009549C));
        t3_8006f5c0(r_u32(0x8009549C));
        t3_800759d4();
        t3_8004a6bc(r_u32(0x8009549C));
        if (r_u32(0x800AFA88) == 7)
            t3_bns_03_800b094c(actor0, actor1, 0);
        goto finish;
    }
    t3_8003d6b0(actor0, actor1);
    t3_800630d8_host(actor0, actor1, 0x8009546C, 1);
    t3_80046234(first, second);
    t3_80046234(second, first);
    if (r_u32(0x800AFA88) == 8)
        t3_80046234(third, first);
    t3_8004ad34();
    t3_80048240(actor0, actor1);
    t3_80041f4c(actor0);
    t3_80041f4c(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_80041f4c(actor2);
    t3_8004559c(1);
    value = t3_80045878();
    w_u32(0x800954A0, value);
    if (r_u32(0x800AFA88) == 7) {
        t3_bns_03_800b068c(actor0);
        t3_bns_03_800b068c(actor1);
    }
    t3_80043684(r_u32(0x800AFA88) == 8 ? 3 : 2);
    t3_8003f098(actor0);
    t3_8003f098(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_8003f098(actor2);
    t3_80046d00(actor0);
    t3_80046d00(actor1);
    if (r_u32(0x800AFA88) - 7 < 2)
        t3_80046d00(actor2);
    t3_8002ba64(actor0, actor1, actor2);
    t3_800430fc(actor0);
    t3_800430fc(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_800430fc(actor2);
    t3_80042fc8();
    t3_80043018(actor0);
    t3_80043018(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_80043018(actor2);
    t3_80042f40();
    t3_8006d7cc();
    t3_8006f0f4(actor0, 1);
    t3_8006f0f4(actor1, 1);
    t3_8006f5c0(r_u32(0x8009549C));
    t3_800759d4();
    t3_80040a0c(actor0);
    t3_80040a0c(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_80040a0c(actor2);
    t3_80041f6c(actor0);
    t3_80041f6c(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_80041f6c(actor2);
    t3_8003ec64(actor0);
    t3_8003ec64(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_8003ec64(actor2);
    t3_80043ca8();
    if (r_s32(0x8009546C) < 3) {
        t3_80043d84(actor0, actor1);
        if (r_u32(0x800AFA88) == 8) {
            t3_80043d84(actor0, actor2);
            t3_80043d84(actor1, actor2);
        }
    }
    t3_80044008(actor0);
    t3_80044008(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_80044008(actor2);
    t3_8004406c(first, second);
    t3_8004406c(second, first);
    if (r_u32(0x800AFA88) == 8) {
        t3_8004406c(first, third);
        t3_8004406c(third, first);
        t3_8004406c(second, third);
        t3_8004406c(third, second);
    }
    t3_80040a60(actor0);
    t3_80040a60(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_80040a60(actor2);
    t3_80045104();
    if (r_u32(0x800AFA88) == 7)
        t3_bns_03_800b094c(actor0, actor1, 0);
    t3_800458c8(actor0);
    t3_800458c8(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_800458c8(actor2);
    t3_80044fc8(actor0);
    t3_80044fc8(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_80044fc8(actor2);
    t3_8004439c(actor0);
    t3_8004439c(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_8004439c(actor2);
    if (r_u8(actor0 + 195)) {
        opponent = t3_80045c18(actor0);
        t3_80040ea0(actor0, opponent);
    }
    if (r_u8(actor1 + 195)) {
        opponent = t3_80045c18(actor1);
        t3_80040ea0(actor1, opponent);
    }
    if (r_u32(0x800AFA88) == 8 && r_u8(actor2 + 195)) {
        opponent = t3_80045c18(actor2);
        t3_80040ea0(actor2, opponent);
    }
    if (!t3_8002bb58()) {
        t3_80077524();
        t3_8004acb4();
    }
    t3_8002bce8(actor0);
    t3_8002bce8(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_8002bce8(actor2);
    t3_80040ad0(actor0);
    t3_80040ad0(actor1);
    if (r_u32(0x800AFA88) == 8)
        t3_80040ad0(actor2);
    frame0 = r_u16(actor0 + 88);
    frame1 = r_u16(actor1 + 88);
    mode = r_u32(0x800AFA88);
    w_u16(actor0 + 90, (uint16)frame0);
    w_u16(actor1 + 90, (uint16)frame1);
    if (mode == 8) {
        value = r_u16(actor2 + 88);
        w_u16(actor2 + 90, (uint16)value);
    }
    t3_800451b8(0x800AAAB4);
    value = r_u32(0x80095460);
    mode = r_u32(0x800AFA88);
    w_u32(0x80095464, value);
    if (mode == 8 && t3_bns_02_800b1294())
        w_u32(0x80096F2C, 7);
finish:
    if (r_u32(0x800AFA88) == 5)
        w_u32(0x800954B8, t3_bns_01_800b2ba4());
    return (uint32)r_s16(0x80096F2C);
}

uint32 t3_80050428(void)
{
    uint32 previous, index, value, first, second, mode, state, saved[3];
    uint32 character0, enabled0, variant0, character1, enabled1, variant1;
    FUNCTION_MARKER(0x80050428, "SLUS_004.02");
    if (t3_8005101c())
        return 1;
    index = r_u32(0x800ADEFC);
    previous = r_u32(0x800ADF64);
    value = r_u32(0x800AE058 + index * 28);
    state = (uint32)r_s16(0x800AE224);
    w_u32(0x800ADF64, value);
    switch (state) {
    case 0:
        mode = r_u32(0x800AFA88);
        w_u16(0x800AE212, 0);
        if (mode == 6) {
            t3_8002a16c(0, 0);
            t3_8002a16c(1, 0);
            w_u16(0x800AE224, 1);
            goto initialize_mode;
        }
        t3_8002a16c(0, r_u8(0x80097F0C));
        t3_8002a16c(1, r_u8(0x80097F0D));
        w_u8(0x800AFA9C, 8);
        w_u8(0x800AFA9D, 1);
        if (t3_80050f1c()) {
            t3_8002960c(1);
            w_u16(0x800AE204, 10);
            goto restart;
        }
        t3_80052bb4(4);
        t3_80052c30(9, 0);
        break;
    case 1:
initialize_mode:
        switch (r_u32(0x800AFA88)) {
        case 0: case 3: t3_bns_00_800b0794(0x800AFA88); break;
        case 2: t3_bns_00_800b0808(0x800AFA88); break;
        case 7: t3_bns_03_800b4794(); break;
        case 8: t3_bns_02_800b5bc0(0x800AFA88); t3_bns_02_800b5418(); break;
        default: break;
        }
        w_u8(0x800AFA99, 1);
        w_u16(0x800AE224, 2);
        t3_800784b0();
        goto initialize_players;
    case 2:
initialize_players:
        first = r_u16(0x800AE1F8);
        second = r_u16(0x800AE1FA);
        first = first != 0;
        second = second != 0;
        w_u8(0x800AFAA4, (uint8)(first + second));
        w_u8(0x800AFAA5, (uint8)(first | (second << 1)));
        value = r_u8(0x800AFAA5);
        w_u16(0x800ADF10, (uint16)value);
        value = r_u8(0x800AFAA5);
        if (value == 1) {
            w_u8(0x800AFAA6, 0);
            w_u8(0x800AFAA7, 1);
        } else if (value == 2) {
            w_u8(0x800AFAA6, 1);
            w_u8(0x800AFAA7, 0);
        } else if (value == 0 || value == 3) {
            w_u8(0x800AFAA6, 0);
            w_u8(0x800AFAA7, 0);
        }
        value = r_u32(0x800AFAB8);
        mode = r_u32(0x800AFA88);
        w_u8(0x800AFAA9, 0);
        w_u8(0x800AFAAA, 0);
        w_u8(0x800AFAAB, 0);
        w_u32(0x800AFAB4, value);
        switch (mode) {
        case 0: case 3: t3_bns_00_800b08f8(0x800AFA88); break;
        case 2: t3_bns_00_800b09b0(0x800AFA88); break;
        case 4: t3_bns_00_800b0b84(0x800AFA88); break;
        case 7: t3_bns_03_800b47bc(0x800AFA88); break;
        case 8:
            t3_bns_02_800b5c2c(0x800AFA88);
            t3_bns_02_800b542c(r_u32(0x800B6BF4));
            w_u8(0x800AFAA5, 1);
            w_u16(0x800ADF10, 1);
            w_u8(0x800AFAA6, 0);
            w_u8(0x800AFAA7, 1);
            break;
        default: break;
        }
        w_u32(0x800AE214, r_u32(0x800AE218));
        t3_8004f3b0(0x800AFA88);
        w_u8(0x800AFA9C, 8);
        w_u8(0x800AFA9D, 3);
        t3_800524ec();
        break;
    case 3:
        character0 = r_u16(0x800ADD5C);
        enabled0 = r_u16(0x800A9128);
        variant0 = r_u16(0x800ADD98);
        enabled1 = r_u16(0x800A912A);
        character1 = r_u16(0x800ADD5E);
        variant1 = r_u16(0x800ADD9A);
        t3_8002a40c(character0, enabled0, variant0, character1, enabled1, variant1);
        w_u16(0x800AE224, 4);
        goto wait_initialization;
    case 4:
wait_initialization:
        first = r_u32(0x800AE218);
        second = r_u32(0x800AE214);
        if (first - second >= 60)
            w_u16(0x800AE224, 5);
        break;
    case 5:
        t3_8002960c(1);
        w_u16(0x800AE224, 6);
        break;
    case 6:
        t3_8002960c(0);
        w_u16(0x800AE224, 7);
        break;
    case 7:
        w_u8(0x800B040C, 0);
        t3_8002a914();
        w_u16(0x800AE200, (uint16)(r_u32(0x800AFA88) != 5));
        w_u16(0x800AE212, 0);
        w_u8(0x800AFA9B, 0);
        w_u32(0x800AE214, 8);
        w_u16(0x800AE224, 8);
        break;
    case 8:
        value = r_u32(0x800AE214);
        if (value) {
            w_u32(0x800AE214, value - 1);
        } else if (r_u8(0x800AFA8D)) {
            first = t3_80050f5c(0);
            second = t3_80050f5c(1);
            if (first | second) {
                w_u16(0x800AE224, 13);
                break;
            }
        }
        w_u32(0x800ADCA4, 0);
        for (;;) {
            value = t3_8002ae58();
            if (value != 1 || r_u32(0x800ADCA4) || !t3_800295f4())
                break;
            w_u32(0x800ADCA4, value);
            t3_80029dc0(1);
        }
        mode = r_u32(0x800AFA88);
        w_u32(0x800ADCA4, 0);
        if (mode == 6 && (sint32)value >= 3) {
            w_u16(0x800AE224, 24);
            break;
        }
        if (value == 7) {
            w_u16(0x800AE224, 21);
            break;
        }
        if ((sint32)value < 9)
            break;
        if (value == 10) {
            t3_8002960c(1);
            if (r_u32(0x800AFA88) == 3)
                w_u16(0x800AE224, 19);
            else
                w_u16(0x800AE224, 15);
            break;
        }
        if (value == 11) {
            w_u16(0x800AE224, 25);
            break;
        }
        mode = r_u32(0x800AFA88);
        if (mode == 5) {
            w_u16(0x800AE224, 0);
            index = r_u8(0x800B053E);
            w_u16(0x800AE1F8 + index * 2, 0);
            w_u16(0x800ADFBE, 0);
            w_u16(0x800ADFBC, 0);
            break;
        }
        if (!r_u8(0x800AFAA9)) {
            w_u16(0x800AE224, 5);
            break;
        }
        switch (mode) {
        case 0: t3_bns_00_800b0e84(0x800AFA88); break;
        case 1: t3_bns_00_800b1104(0x800AFA88); break;
        case 2: t3_bns_00_800b1228(0x800AFA88); break;
        case 3: t3_bns_00_800b14b4(0x800AFA88); break;
        case 4: t3_bns_00_800b1648(0x800AFA88); break;
        case 5: case 6: w_u16(0x800AE224, 24); break;
        case 7: t3_bns_03_800b4860(0x800AFA88); break;
        case 8: t3_bns_02_800b5cf0(0x800AFA88); break;
        default: break;
        }
        break;
    case 9:
        t3_80040c00(22);
        t3_8006bc98(60, 60);
        t3_8004ffe0();
        w_u8(0x800AFA9A, 1);
        w_u16(0x800AE224, 10);
        goto continue_screen;
    case 10:
continue_screen:
        t3_8002ae58();
        first = r_u8(0x800AFAAB);
        second = r_u8(0x800AFAAA);
        value = t3_8004fff0(first);
        if (value == 1 || value == 2) {
            w_u16(0x800AE224, value == 1 ? 2 : 0);
            first = r_u8(0x800AFAAA);
            w_u8(0x800AFA9A, 0);
            t3_8005118c(first);
        } else if (value) {
            w_u16(0x800AE224, 11);
            w_u8(0x800AFA9A, 0);
        }
        break;
    case 11:
        w_u32(0x800AE214, 0);
        index = r_u8(0x800AFAAB);
        w_u16(0x800AE1F8 + index * 2, 0);
        t3_80040c00(0x1017);
        t3_8006ae14(3, 0);
        w_u16(0x800AE224, 12);
        goto game_over;
    case 12:
game_over:
        t3_8004ce74(0x80022490, 1, 2, 85, 200);
        t3_8002ae58();
        value = r_u32(0x800AE214) + 1;
        w_u32(0x800AE214, value);
        if ((sint32)value >= 121)
            t3_80050190();
        break;
    case 13:
        t3_800753bc(0, 0x49A1, 0);
        t3_8006bc98(0, 30);
        w_u32(0x800AE214, 8);
        w_u8(0x800AFA9B, 1);
        if (t3_80050f1c())
            goto restart_challenger;
        w_u16(0x800AE224, 14);
        goto challenger;
    case 14:
challenger:
        value = t3_8002ae58();
        if ((sint32)value > 0 || r_s32(0x800AE214) >= 24)
            w_u32(0x80095468, 1);
        if (r_u32(0x800AE214) & 24)
            t3_8004ce74(0x800224A4, 7, 2, 8, 288);
        if (r_u32(0x800AE214) == 40)
            t3_80052a34(4);
        value = r_u32(0x800AE214) + 1;
        w_u32(0x800AE214, value);
        if ((sint32)value < 74)
            break;
restart_challenger:
        t3_8002960c(1);
        if (r_u8(0x800AFA8E)) {
            w_u16(0x800ADFBE, 0);
            w_u16(0x800ADFBC, 0);
        }
restart:
        w_u16(0x800AE224, 0);
        break;
    case 15:
        t3_8002960c(0);
        w_u16(0x800AE224, 16);
        break;
    case 16:
        t3_bns_00_800b3ca0();
        w_u16(0x800AE224, 17);
        break;
    case 17:
        if (t3_bns_00_800b4060()) {
            w_u16(0x800AE224, 18);
        } else {
            index = r_u8(0x800AFAA6);
            if (r_u16(0x800ADF08 + index * 2) & 0x800)
                w_u16(0x800AE224, 18);
        }
        break;
    case 18:
        t3_8002960c(1);
        w_u16(0x800AE224, 19);
        break;
    case 19:
        t3_8002960c(0);
        w_u16(0x800AE224, 20);
        break;
    case 20:
        t3_bns_00_800b2870(0x800AFB18);
        w_u8(0x800AFAA9, 0);
        w_u8(0x800AFAAA, 0);
        w_u8(0x800AFAAB, 0);
        w_u8(0x800AFAA1, 0);
        t3_bns_00_800b08f8(0x800AFA88);
        t3_8004f3b0(0x800AFA88);
        t3_8002a384(saved);
        character0 = r_u16(0x800ADD5C);
        enabled0 = r_u16(0x800A9128);
        variant0 = r_u16(0x800ADD98);
        enabled1 = r_u16(0x800A912A);
        character1 = r_u16(0x800ADD5E);
        variant1 = r_u16(0x800ADD9A);
        t3_8002a40c(character0, enabled0, variant0, character1, enabled1, variant1);
        t3_8002a3e0(saved);
        w_u16(0x800AE224, 5);
        break;
    case 21:
        t3_8002960c(1);
        w_u16(0x800AE224, 22);
        break;
    case 22:
        t3_8002960c(0);
        w_u16(0x800AE224, 23);
        break;
    case 23:
        t3_bns_02_800b2658();
        w_u16(0x800AE224, 8);
        break;
    case 24:
        t3_8004b61c(0);
        t3_80028f5c();
        if (t3_8004f89c())
            t3_80050190();
        else
            t3_8004f8f8(3);
        break;
    default:
        t3_8004b61c(1);
        t3_8004b5f4();
        t3_80028f5c();
        t3_8004f8f8(4);
        break;
    }
    w_u32(0x800ADF64, previous);
    return 0x800B0000;
}

uint32 t3_8003e800(uint32 actor, uint32 opponent)
{
    sint32 first, second;
    uint32 kind, winner, alternate, selector;
    FUNCTION_MARKER(0x8003E800, "SLUS_004.02");
    first = r_s16(actor + 68);
    second = r_s16(opponent + 68);
    if (second < first) {
        kind = 1;
        winner = 0;
        alternate = 1;
    } else if (first < second) {
        kind = 2;
        winner = 1;
        alternate = 0;
    } else {
        selector = r_u16(0x800ADF10);
        winner = selector == 1;
        if (selector == 3)
            winner = r_u8(0x800ADF3E);
        alternate = (winner + 1) & 1;
        kind = 3;
    }
    w_u8(0x800AFAA9, (uint8)kind);
    w_u8(0x800AFAAA, (uint8)winner);
    w_u8(0x800AFAAB, (uint8)alternate);
    return 0x800AFA88;
}

uint32 t3_8003e8c8(uint32 actor, uint32 opponent)
{
    uint32 variant, value;
    FUNCTION_MARKER(0x8003E8C8, "SLUS_004.02");
    variant = r_u32(0x800AFA4C) & 1;
    if (r_s16(actor + 70) > 0) {
        w_u16(actor + 72, (uint16)(variant + 1));
        t3_8003c258(actor);
        return t3_8002d45c(actor, opponent, variant);
    }
    value = (uint32)r_s16(opponent + 70);
    if ((sint32)value <= 0)
        return variant + 1;
    w_u16(opponent + 72, (uint16)(variant + 1));
    t3_8003c258(opponent);
    return t3_8002d45c(opponent, actor, variant);
}

uint32 t3_8003e9bc(uint32 actor, uint32 opponent)
{
    uint32 value;
    FUNCTION_MARKER(0x8003E9BC, "SLUS_004.02");
    if (r_s16(actor + 70) < 0) {
        if (r_u32(actor + 1012))
            t3_8002d5f0(actor, opponent);
        else
            t3_8002d66c(actor, opponent);
    }
    value = (uint32)r_s16(opponent + 70);
    if ((sint32)value >= 0)
        return value;
    if (r_u32(opponent + 1012))
        return t3_8002d5f0(opponent, actor);
    return t3_8002d66c(opponent, actor);
}

uint32 t3_8003ea5c(uint32 actor, uint32 opponent)
{
    uint32 mode, limit, value;
    sint32 wins;
    FUNCTION_MARKER(0x8003EA5C, "SLUS_004.02");
    mode = r_u32(0x800AFA88);
    if (mode == 6)
        goto inactive;
    wins = r_s16(actor + 68);
    limit = r_u16(0x800ADDFC);
    if (wins < (sint32)limit && r_s16(opponent + 68) < (sint32)limit)
        goto select_winner;
    if (r_u8(0x800AFA92) == 1)
        goto select_winner;
    if (!r_u8(actor + 197) && r_s16(actor + 72) > 0) {
        value = 3;
        goto first;
    }
    if (!r_u8(opponent + 197) && r_s16(opponent + 72) > 0)
        goto select_winner;
    if (r_s16(actor + 72) > 0) {
        value = r_u32(opponent + 1012);
        w_u32(0x800AE21C, 10000);
        t3_8006875c(opponent, value);
        value = 7;
        goto second;
    }
    if (r_s16(opponent + 72) > 0) {
        value = r_u32(actor + 1012);
        w_u32(0x800AE21C, 10000);
        t3_8006875c(actor, value);
        value = 6;
        goto first;
    }
    goto neither;
select_winner:
    if (r_s16(actor + 72) > 0) {
        value = 3;
        goto first;
    }
    if (r_s16(opponent + 72) > 0) {
        value = 4;
        goto second;
    }
neither:
    w_u32(0x8009546C, 2);
inactive:
    w_u8(opponent + 195, 1);
    w_u8(actor + 195, 1);
    return 1;
first:
    w_u32(0x8009546C, value);
    return t3_8002b98c(r_u8(actor + 30));
second:
    w_u32(0x8009546C, value);
    return t3_8002b98c(r_u8(opponent + 30));
}

uint32 t3_8003e52c(uint32 actor, uint32 opponent)
{
    uint32 mode, first, second, value, limit, priority, other, selector;
    sint32 first_wins, second_wins;
    FUNCTION_MARKER(0x8003E52C, "SLUS_004.02");
    mode = r_u32(0x800AFA88);
    if (mode == 8) {
        if (r_u8(actor + 197)) {
            second = r_u16(opponent + 1024);
            first = r_u16(actor + 1024);
            if (first < second && !(r_u16(0x800ADE78) & 1)) {
                w_u16(opponent + 70, 1);
                w_u16(actor + 70, 0xFFFF);
                value = r_u16(opponent + 68);
                w_u16(opponent + 68, (uint16)(value + 1));
                goto finish;
            }
            w_u16(opponent + 70, 0xFFFF);
            value = r_u16(actor + 68);
            w_u16(actor + 70, 1);
            w_u16(actor + 68, (uint16)(value + 1));
            goto finish;
        }
        first = r_u16(actor + 1024);
        second = r_u16(opponent + 1024);
        if (second < first && !(r_u16(0x800ADE78) & 1))
            goto first_winner;
        goto second_winner;
    }
    first = r_u16(actor + 1024);
    second = r_u16(opponent + 1024);
    if (second < first)
        goto first_winner;
    if (first < second)
        goto second_winner;
    value = r_u16(0x800ADF3C);
    first_wins = r_s16(actor + 68);
    second_wins = r_s16(opponent + 68);
    w_u16(0x800ADF3C, (uint16)(value - 1));
    priority = actor;
    other = opponent;
    if (first_wins < second_wins) {
        priority = opponent;
        other = actor;
    } else if (first_wins == second_wins) {
        selector = r_u16(0x800ADF10);
        value = selector == 1;
        if (selector == 3)
            value = r_u8(0x800ADF3E);
        if (value) {
            priority = opponent;
            other = actor;
        }
    }
    value = r_u16(priority + 68);
    w_u16(priority + 68, (uint16)(value + 1));
    value = r_u16(other + 68);
    w_u16(other + 68, (uint16)(value + 1));
    w_u16(priority + 70, 0xFFFF);
    w_u16(other + 70, 0xFFFF);
    if (r_u8(0x800AFA90)) {
        first_wins = r_s16(priority + 68);
        limit = r_u16(0x800ADDFC);
        if (first_wins >= (sint32)limit && r_s16(other + 68) < (sint32)limit)
            w_u16(priority + 70, 1);
    } else {
        first_wins = r_s16(priority + 68);
        limit = r_u16(0x800ADDFC);
        if (first_wins >= (sint32)limit)
            w_u16(priority + 70, 1);
        second_wins = r_s16(other + 68);
        limit = r_u16(0x800ADDFC);
        value = r_u16(other + 68);
        if (second_wins >= (sint32)limit)
            w_u16(other + 68, (uint16)(value - 1));
    }
    goto finish;
first_winner:
    w_u16(actor + 70, 1);
    w_u16(opponent + 70, 0xFFFF);
    value = r_u16(actor + 68);
    w_u16(actor + 68, (uint16)(value + 1));
    goto finish;
second_winner:
    w_u16(actor + 70, 0xFFFF);
    value = r_u16(opponent + 68);
    w_u16(opponent + 70, 1);
    w_u16(opponent + 68, (uint16)(value + 1));
finish:
    limit = r_u16(0x800ADF3C);
    value = r_u32(0x80095484);
    if ((sint32)value >= (sint32)limit)
        return t3_8003e800(actor, opponent);
    first_wins = r_s16(actor + 68);
    limit = r_u16(0x800ADDFC);
    if (first_wins >= (sint32)limit)
        return t3_8003e800(actor, opponent);
    second_wins = r_s16(opponent + 68);
    if (second_wins >= (sint32)limit)
        return t3_8003e800(actor, opponent);
    return 1;
}

uint32 t3_8003d554(uint32 actor)
{
    uint32 side;
    FUNCTION_MARKER(0x8003D554, "SLUS_004.02");
    if (!r_u32(actor + 1012) || r_s16(actor + 70) < 0) {
        side = r_u8(actor + 30);
        w_u32(0x80095470 + side * 4, 2);
        w_u16(actor + 1032, 0);
        return 2;
    }
    if (!(r_u16(0x800ADE78) & 1) && !r_u8(actor + 197)) {
        side = r_u8(actor + 30);
        w_u32(0x80095470 + side * 4, 1);
        return 1;
    }
    side = r_u8(actor + 30);
    w_u32(0x80095470 + side * 4, 2);
    return t3_8003d60c(actor);
}

uint32 t3_8003e438(uint32 actor, uint32 opponent)
{
    uint32 row, divisor, health, value;
    FUNCTION_MARKER(0x8003E438, "SLUS_004.02");
    for (row = 0; row < 2; ++row) {
        divisor = r_u32(actor + 1016);
        health = r_u32(actor + 1012);
        divisor = (uint32)((sint32)divisor >> 12);
        value = health + divisor - 1;
        value = divisor ? value / divisor : 0xFFFFFFFF;
        w_u16(actor + 1024, (uint16)value);
        actor = opponent;
    }
    return 0;
}

uint32 t3_8003e474(uint32 actor, uint32 opponent)
{
    uint32 timer, first, second, flags, winner;
    FUNCTION_MARKER(0x8003E474, "SLUS_004.02");
    timer = r_u32(0x800ADBCC);
    first = r_u16(actor + 1024);
    second = r_u16(opponent + 1024);
    flags = (sint32)timer < 1;
    if (first == second) {
        flags |= 32;
        if (!first)
            flags |= 6;
    } else {
        winner = actor;
        if (second < first)
            actor = opponent;
        else
            winner = opponent;
        if (!r_u16(actor + 1024)) {
            first = r_u16(winner + 1024);
            flags |= 2;
            if (first == 4096)
                flags |= 8;
            else if (first < 205)
                flags |= 16;
        }
    }
    w_u16(0x800ADE78, (uint16)flags);
    return 0x800B0000;
}

uint32 t3_8003e95c(uint32 actor, uint32 opponent)
{
    uint32 value, first, result;
    FUNCTION_MARKER(0x8003E95C, "SLUS_004.02");
    result = r_u16(0x800ADE78) & 1;
    if (!result)
        return result;
    first = (uint32)r_s16(actor + 70);
    result = (uint32)r_s16(opponent + 70);
    value = r_u16(actor + 70);
    if (first == result) {
        w_u16(actor + 72, 0);
        w_u16(opponent + 72, 0);
    } else {
        if ((sint32)first < 0) {
            w_u16(actor + 72, (uint16)value);
            result = (uint32)r_s16(opponent + 70);
        }
        value = r_u16(opponent + 70);
        if ((sint32)result < 0)
            w_u16(opponent + 72, (uint16)value);
    }
    return result;
}

uint32 t3_8003d60c(uint32 actor)
{
    uint32 animation, value;
    FUNCTION_MARKER(0x8003D60C, "SLUS_004.02");
    if (r_s16(actor + 22) == 18) {
        w_u16(actor + 1032, 0);
        return 0x800B0000;
    }
    if (r_u32(0x800AFA88) == 8 && t3_8002d254((uint32)r_s16(actor + 160))) {
        w_u16(actor + 1032, 0x8000);
        return 0x8000;
    }
    animation = r_u32(actor + 420);
    if (!animation) {
        animation = r_u32(actor + 84);
        value = (uint32)r_s16(animation + 16);
        animation = t3_8002d178(actor, value);
    }
    value = r_u16(animation + 4);
    w_u16(actor + 1032, value & 0x204 ? 0x1000 : 0);
    return 0x1000;
}

uint32 t3_8003cb84(void)
{
    uint32 third = 0, state, value, mode, first, second, actor, animation, length;
    sint32 frame, outcome;
    FUNCTION_MARKER(0x8003CB84, "SLUS_004.02");
    if (r_u32(0x800AFA88) == 8)
        third = 0x800AC340;
    state = r_u32(0x80096F2C);
    if ((sint32)state >= 2)
        w_u32(0x80096F30, r_u32(0x80096F30) + 1);
    switch (state) {
    case 0:
        value = r_u32(0x80095460) + 1;
        w_u32(0x80095460, value);
        if (r_u32(0x80095484) == 1) {
            if ((sint32)value < 151)
                return 0x800B0000;
        } else if ((sint32)value < 91) {
            return 0x800B0000;
        }
        if (r_u32(0x800AFA88) != 5)
            t3_8002bc00(1);
        w_u32(0x80096F2C, 1);
        return 1;
    case 1:
        w_u32(0x80095460, r_u32(0x80095460) + 1);
        value = t3_8003e370();
        mode = r_u32(0x800AFA88);
        w_u32(0x8009547C, value);
        if (mode == 8)
            w_u16(0x800AE212, 1);
        if (!value) {
            if (!r_u32(0x800954B0))
                w_u32(0x800ADBCC, r_u32(0x800ADBCC) - 1);
            value = r_u32(0x800AFAB4) + 1;
            w_u32(0x800AFAB4, value);
            return value;
        }
        t3_8003e438(0x800A9228, 0x800AAAB4);
        t3_8003e474(0x800A9228, 0x800AAAB4);
        t3_8003e52c(0x800A9228, 0x800AAAB4);
        t3_800517ec(0x800A9228, 0x800AAAB4);
        if (r_u8(0x800AFAA9) && !r_u32(0x800AFA88)) {
            first = r_u8(0x800AFAA5);
            second = r_u8(0x800AFAAB);
            if ((first >> (second & 31)) & 1)
                w_u16(0x800AE212, 1);
        }
        t3_8003e95c(0x800A9228, 0x800AAAB4);
        w_u32(0x8009E5F8, 0);
        value = r_u16(0x800ADE78);
        w_u32(0x8009E5FC, 0);
        if (value & 1) {
            w_u32(0x80096F38, 90);
            w_u32(0x80095478, 0);
            w_u32(0x80095474, 0);
            w_u32(0x80095470, 0);
        } else {
            w_u32(0x80096F38, 240);
        }
        w_u32(0x80096F2C, 2);
        w_u32(0x80096F3C, 0);
        return 0x80090000;
    case 2:
        t3_8003d554(0x800A9228);
        t3_8003d554(0x800AAAB4);
        if (r_u32(0x800AFA88) == 8)
            t3_8003d554(third);
        mode = r_u32(0x800AFA88);
        if (mode == 7) {
            if (r_u32(0x80096F30) == 16) {
                w_u32(0x800AFAC0, 1);
                outcome = r_s16(0x800A926E);
                w_u16(0x800AE200, 0);
                w_u32(0x80096F34, 1);
                if (outcome < 0) {
                    t3_8006875c(0x800A9228, r_u32(0x800A961C));
                    w_u32(0x8009546C, 6);
                    w_u8(0x800AFAC8, 0);
                } else {
                    t3_8006875c(0x800AAAB4, r_u32(0x800AAEA8));
                    w_u32(0x8009546C, mode);
                    w_u8(0x800AFAC8, 1);
                }
            } else if (r_s32(0x800AFAC0) > 0) {
                if (t3_800325bc()) {
                    t3_8007666c();
                    t3_8004a83c();
                    t3_80041998(0x800A9228);
                    t3_80041998(0x800AAAB4);
                    first = r_u32(0x800AFAC0);
                    second = r_u32(0x800AFAC4);
                    if ((sint32)first < (sint32)second) {
                        w_u32(0x80096F34, 1);
                        t3_8006f468();
                        w_u32(0x80096F30, r_u32(0x80096F30) - 1);
                        value = r_u32(0x800AFAC0);
                        outcome = r_s16(0x800A926E);
                        w_u32(0x800AFAC0, value + 1);
                        w_u32(0x8009546C, outcome < 0 ? 3 : 4);
                    } else {
                        w_u16(0x800AE200, 1);
                    }
                } else {
                    w_u32(0x80096F30, r_u32(0x80096F30) - 1);
                }
            }
        }
        if ((r_u16(0x800ADE78) & 31) && r_u32(0x80096F30) == 90)
            w_u32(0x80096F3C, 1);
        value = r_u32(0x80096F3C);
        if (!value)
            return value;
        w_u32(0x80095478, 0);
        w_u32(0x80095474, 0);
        w_u32(0x80095470, 0);
        t3_80041998(0x800A9228);
        t3_80041998(0x800AAAB4);
        t3_8004b770();
        mode = r_u32(0x800AFA88);
        if (mode == 8) {
            actor = t3_bns_02_800b297c();
            if (r_s16(actor + 70) == 1) {
                t3_8006b6c8(0);
                w_u32(0x80096F30, 0);
                w_u32(0x80096F2C, mode);
                return 0x80090000;
            }
            w_u32(0x80096F2C, 9);
            return 9;
        }
        w_u32(0x80096F2C, 3);
        goto prepare_next;
    case 3:
prepare_next:
        if (r_u32(0x800AFA88) != 7) {
            w_u32(0x80096F34, 1);
            w_u32(0x80096F2C, 4);
            return 4;
        }
        outcome = r_s16(0x800A926E);
        w_u32(0x8009546C, 2);
        if (outcome < 0 && r_u8(0x800AAB79))
            w_u32(0x800A961C, 1);
        if (r_s16(0x800AAAFA) < 0 && r_u8(0x800A92ED))
            w_u32(0x800AAEA8, 1);
        t3_8003e8c8(0x800A9228, 0x800AAAB4);
        t3_8003ea5c(0x800A9228, 0x800AAAB4);
        if (t3_8002bb58())
            t3_8003e9bc(0x800A9228, 0x800AAAB4);
        t3_8002bfe8();
        value = r_u32(0x800AFAC0);
        w_u32(0x80096F2C, 5);
        w_u32(0x800AFAC0, value + 1);
        goto reset_round;
    case 4:
        value = t3_800325bc();
        if (!value)
            return value;
        w_u32(0x800A8D6C, 2);
        t3_8007666c();
        t3_8004a83c();
        t3_80041998(0x800A9228);
        t3_80041998(0x800AAAB4);
        if (!r_u8(0x800AFAA9) && ((t3_8002b9d4() & 0x800) || r_u32(0x80095480))) {
            t3_8004b688(0);
            goto finish_round;
        }
        t3_8003e8c8(0x800A9228, 0x800AAAB4);
        if (!(r_u16(0x800ADE78) & 1)) {
            t3_8003a818(0x800A9228);
            t3_8003a818(0x800AAAB4);
        }
        t3_8003ea5c(0x800A9228, 0x800AAAB4);
        if (t3_8002bb58())
            t3_8003e9bc(0x800A9228, 0x800AAAB4);
        t3_8002bfe8();
        t3_8004b688(0);
        w_u32(0x80096F2C, r_u32(0x80096F2C) + 1);
        goto reset_round;
    case 5:
reset_round:
        w_u32(0x800A8D6C, 2);
        w_u32(0x80096F38, 300);
        w_u32(0x8009E5F8, 0);
        w_u32(0x8009E5FC, 0);
        w_u32(0x80096F30, 0);
        w_u32(0x80096F3C, 0);
        value = r_u32(0x80096F2C);
        first = r_u8(0x800AFA91);
        w_u32(0x80096F2C, value + 1);
        if (first) {
            first = r_u32(0x800A9624);
            second = r_u32(0x800AAEB0);
            w_u32(0x800A961C, first);
            w_u32(0x800AAEA8, second);
        }
        goto wait_round;
    case 6:
wait_round:
        value = r_u32(0x8009546C);
        w_u32(0x800A8D6C, 2);
        if (value == 2) {
            if (r_u32(0x80096F30) == 75)
                w_u32(0x80096F3C, 1);
        } else if (value - 6 < 2) {
            mode = r_u32(0x800AFA88);
            if (r_u32(0x80096F30) == (mode == 7 ? 240 : 60))
                w_u32(0x80096F3C, 1);
        } else if (value == 5) {
            for (actor = 0x800A9228; actor != 0x800AC340; actor += 6284) {
                outcome = r_s16(actor + 72);
                if (outcome > 0) {
                    animation = r_u32(actor + 84);
                    length = r_u8(animation + 24);
                    frame = r_s16(actor + 88);
                    if (frame < (sint32)(length - 1))
                        continue;
                }
                w_u32(0x8009E5F8 + (actor == 0x800A9228 ? 0 : 4), 1);
            }
            if (r_u32(0x8009E5F8) && r_u32(0x8009E5FC))
                w_u32(0x80096F3C, 1);
        }
        value = r_u32(0x80096F38) - 1;
        w_u32(0x80096F38, value);
        if ((sint32)value < 0)
            w_u32(0x80096F38, 0);
        if (t3_8002b9d4() & 0x800)
            t3_8004b688(0);
        if (!r_u32(0x80096F3C) && r_s32(0x80096F38) > 0 &&
            (r_u8(0x800AFAA9) || !r_u32(0x80095480) || r_s32(0x80096F30) < 2)) {
            value = t3_8002b9d4() & 0x800;
            if (!value)
                return value;
        }
finish_round:
        w_u32(0x80096F2C, 9);
        goto finish;
    case 7:
        w_u32(0x80096F2C, 1);
        return 1;
    case 8:
        t3_8004ce74(0x8001A33C, 2, 8, 41, 112);
        if (r_s32(0x80096F30) >= 300 || ((t3_8002b9d4() & 0x800) && r_u32(0x800B6BFC)))
            w_u32(0x80096F2C, 9);
        actor = t3_bns_02_800b297c();
        value = r_u8(actor + 30);
        w_u32(0x80095470 + value * 4, 2);
        return t3_8003d60c(actor);
    case 9:
finish:
        value = t3_80051204();
        if (value) {
            value = 10;
            w_u32(0x80096F2C, value);
        }
        return value;
    default:
        return 0x80020000;
    }
}

uint32 t3_8002d5f0(uint32 actor, uint32 opponent)
{
    uint32 value;
    FUNCTION_MARKER(0x8002D5F0, "SLUS_004.02");
    w_u16(actor + 416, 3436);
    value = t3_8002d178(actor, 3436);
    w_u32(actor + 84, value);
    w_u32(actor + 76, value);
    w_u32(actor + 420, value);
    w_u16(actor + 88, 1);
    w_u16(actor + 408, 1);
    value = t3_80030f68(actor, 6);
    w_u16(actor + 410, (uint16)value);
    w_u16(actor + 412, 0);
    w_u16(actor + 414, 0);
    return t3_8002f3ac(actor, opponent);
}

uint32 t3_8002d66c(uint32 actor, uint32 opponent)
{
    uint32 value;
    FUNCTION_MARKER(0x8002D66C, "SLUS_004.02");
    w_u16(actor + 416, 3429);
    value = t3_8002d178(actor, 3429);
    w_u32(actor + 84, value);
    w_u32(actor + 76, value);
    w_u32(actor + 420, value);
    w_u16(actor + 88, 1);
    w_u16(actor + 408, 1);
    value = t3_80030f68(actor, 6);
    w_u16(actor + 410, (uint16)value);
    w_u16(actor + 412, 0);
    w_u16(actor + 414, 0);
    return t3_8002f3ac(actor, opponent);
}

uint32 t3_8002d6e8(uint32 actor, uint32 animation)
{
    uint32 value;
    FUNCTION_MARKER(0x8002D6E8, "SLUS_004.02");
    w_u16(actor + 416, (uint16)animation);
    value = t3_8002d178(actor, (uint32)(sint32)(sint16)animation);
    w_u32(actor + 84, value);
    w_u32(actor + 76, value);
    w_u32(actor + 420, value);
    w_u16(actor + 88, 1);
    w_u16(actor + 408, 1);
    value = t3_80030f68(actor, 0);
    w_u16(actor + 410, (uint16)value);
    w_u16(actor + 412, 0);
    w_u16(actor + 414, 0);
    return t3_8002f3ac(actor, actor);
}

uint32 t3_80031ddc(uint32 actor)
{
    uint32 frame, value, result;
    FUNCTION_MARKER(0x80031DDC, "SLUS_004.02");
    frame = (uint32)r_s16(actor + 88);
    value = (r_u32(0x8009BCB8) - frame - 30) << 1;
    w_u32(0x8009BCC0, value);
    if ((sint32)value < 0) {
        value += 19200;
        w_u32(0x8009BCC0, value);
    }
    value = r_u32(0x8009BCC0);
    result = (uint32)((sint32)value / 600) * 600;
    w_u32(0x8009BCC0, value - result);
    return result;
}

uint32 t3_8002b9d4(void)
{
    uint32 mode, first, second;
    FUNCTION_MARKER(0x8002B9D4, "SLUS_004.02");
    if (r_u32(0x800AFA88) == 8)
        return r_u32(0x800B6BF4) ? r_u16(0x800ADF0A) : r_u16(0x800ADF08);
    mode = r_u16(0x800ADF10);
    if (mode == 1)
        return r_u16(0x800ADF08);
    if (mode == 2)
        return r_u16(0x800ADF0A);
    if (mode == 3) {
        first = r_u16(0x800ADF08);
        second = r_u16(0x800ADF0A);
        return first | second;
    }
    return 0;
}

uint32 t3_8002bc70(uint32 side, uint32 value)
{
    FUNCTION_MARKER(0x8002BC70, "SLUS_004.02");
    t3_8002c050(0x800A9228 + side * 6284);
    w_u32(0x80095470 + side * 4, value);
    return 0x80095470;
}

uint32 t3_8002d264(uint32 animation, uint32 kind)
{
    uint32 table, wanted, value;
    sint32 gate;
    FUNCTION_MARKER(0x8002D264, "SLUS_004.02");
    table = r_u32(0x80095694 + (uint32)((sint32)(kind << 16) >> 14));
    wanted = r_u16(animation - 2);
    gate = r_s16(table);
    value = r_u16(table);
    while (gate > 0) {
        table += 2;
        if ((sint16)wanted == (sint16)value)
            return 1;
        gate = r_s16(table);
        value = r_u16(table);
    }
    return 0;
}

uint32 t3_8003866c(uint32 actor, uint32 matrix, uint32 point)
{
    uint32 delta[3], local[3], first, second, squared, length, row, numerator;
    VECTOR position, transformed;
    FUNCTION_MARKER(0x8003866C, "SLUS_004.02");
    first = r_u32(actor + 0xD48);
    second = r_u32(actor + 0xC7C);
    delta[0] = first - second;
    first = r_u32(actor + 0xD4C);
    second = r_u32(actor + 0xC80);
    delta[1] = first - second;
    second = r_u32(actor + 0xC84);
    first = r_u32(actor + 0xD50);
    delta[2] = first - second;
    squared = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if ((sint32)squared < 0x4000)
        return 1;
    length = t3_8004aedc(squared);
    if (!length)
        length = 1;
    for (row = 3; row-- > 0;) {
        numerator = delta[row] << 7;
        first = (uint32)((sint64)(sint32)numerator / (sint32)length);
        second = r_u32(actor + 0xC7C + row * 4);
        local[row] = first + second;
    }
    local[2] += 64;
    for (row = 3; row-- > 0;) {
        first = r_u32(actor + 0xDD0 + row * 4);
        local[row] -= first;
    }
    position.vx = (sint32)local[0];
    position.vy = (sint32)local[1];
    position.vz = (sint32)local[2];
    ApplyMatrixLV((MATRIX *)psx_addr(matrix, 32), &position, &transformed);
    w_u32(point, (uint32)transformed.vx);
    w_u32(point + 4, (uint32)transformed.vy);
    w_u32(point + 8, (uint32)transformed.vz);
    return t3_8003a5bc(point, point);
}
