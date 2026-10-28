#include "game_functions.h"

uint32 t3_80075dec(uint32 character, uint32 sound)
{
    uint32 value, actor;
    FUNCTION_MARKER(0x80075DEC, "SLUS_004.02");
    value = r_u32(0x800954A4);
    if (value)
        return value;
    if (r_u32(0x800AFA88) == 7) {
        value = r_u32(0x800AFAC0);
        if (value)
            return value;
    }
    actor = t3_80075ef0(character & 0xFF);
    switch (sound & 0xFF) {
    case 0:
        t3_80028ec0(actor, 23);
        return t3_80028ec0((actor + 1) & 1, 10);
    case 1:
        t3_80028ec0(actor, 15);
        return t3_80028ec0((actor + 1) & 1, 27);
    case 2: return t3_80028ec0(actor, 25);
    case 3: return t3_80028ec0(actor, 12);
    case 4: return t3_80028ec0(actor, 13);
    case 5: return t3_80028ec0(actor, 5);
    default: return 0x80020000;
    }
}

uint32 t3_8004a5c8(uint32 source)
{
    uint32 snapshot[4];
    FUNCTION_MARKER(0x8004A5C8, "SLUS_004.02");
    snapshot[0] = r_u32(source);
    snapshot[1] = r_u32(source + 4);
    snapshot[2] = r_u32(source + 8);
    snapshot[3] = r_u32(source + 12);
    return t3_8004a468(0, 0, snapshot, 1);
}

uint32 t3_8006ef60(uint32 mode)
{
    uint32 actor, opponent, count = 10, row, event, flags, value, side_flag;
    FUNCTION_MARKER(0x8006EF60, "SLUS_004.02");
    t3_80075dec(mode, 0);
    actor = 0x800A9228 + mode * 6284;
    opponent = r_u8(actor + 0x22);
    if (r_s16(0x800A9228 + opponent * 6284 + 24) == 17) {
        count = 1;
        flags = r_u8(0x800ADED6);
        side_flag = 16 >> (mode & 31);
        w_u8(0x800ADED6, (uint8)(flags | side_flag));
    }
    for (row = 0; row < count; ++row) {
        event = t3_8006f4f4();
        if (event == 0xFFFFFFFF)
            return event;
        opponent = r_u8(actor + 0x22);
        if (r_s16(0x800A9228 + opponent * 6284 + 24) == 17) {
            flags = r_u8(event + 13);
            w_u8(event + 13, (uint8)(flags | 32));
        }
        w_u8(event + 12, 11);
        w_u32(event + 72, row);
        value = r_u32(0x80025180 + row * 4);
        flags = r_u8(event + 13);
        w_u32(event + 76, value);
        side_flag = 16 >> (mode & 31);
        w_u8(event + 13, (uint8)(flags | side_flag));
    }
    return 0;
}

uint32 t3_8006e8b0(const uint32 *payload)
{
    uint32 row, event, value;
    FUNCTION_MARKER(0x8006E8B0, "SLUS_004.02");
    for (row = 0; row < 20; ++row) {
        event = t3_8006f4f4();
        if (event == 0xFFFFFFFF)
            return event;
        w_u8(event + 12, 13);
        value = payload[0];
        w_u32(event + 40, value);
        value = payload[1];
        w_u32(event + 44, value);
        value = payload[2];
        w_u32(event + 48, value);
        value = ((const uint16 *)payload)[8];
        w_u16(event + 16, (uint16)value);
        value = ((const uint16 *)payload)[9];
        w_u16(event + 18, (uint16)value);
    }
    return 0;
}


uint32 t3_8006e808(const uint32 *payload)
{
    uint32 row, event, value;
    FUNCTION_MARKER(0x8006E808, "SLUS_004.02");
    for (row = 0; row < 6; ++row) {
        event = t3_8006f4f4();
        if (event == 0xFFFFFFFF)
            return event;
        w_u8(event + 12, 12);
        value = payload[0];
        w_u32(event + 40, value);
        value = payload[1];
        w_u32(event + 44, value);
        value = payload[2];
        w_u32(event + 48, value);
        value = ((const uint16 *)payload)[8];
        w_u16(event + 16, (uint16)value);
        value = ((const uint16 *)payload)[9];
        w_u16(event + 18, (uint16)value);
    }
    return 0;
}


uint32 t3_800458c8(uint32 actor)
{
    uint32 animation, first, second, script, active, time, command, kind, argument, bone, group, index;
    sint32 frame;
    union { uint32 words[6]; uint16 halves[12]; } payload;
    FUNCTION_MARKER(0x800458C8, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    first = r_u8(actor + 30);
    second = r_u8(actor + 31);
    script = r_u32(animation + 32);
    active = r_u8(actor + 0xC3);
    w_u8(actor + 0xE4, 0);
    if (!active || !script)
        return active;
    if (!r_u16(script))
        return 0x80020000;
    do {
        frame = r_s16(actor + 0x58);
        time = r_u16(script);
        if (frame >= (sint32)time && r_s16(actor + 0x5A) < (sint32)time) {
            command = r_u16(script + 2);
            argument = command & 0xFF;
            kind = command >> 8;
            if (kind - 6 < 5) {
                bone = actor + argument * 68;
                payload.words[0] = r_u32(bone + 0x908);
                payload.words[1] = r_u32(bone + 0x90C);
                command = r_u32(bone + 0x910);
                payload.halves[8] = 0;
                payload.halves[9] = 0;
                payload.halves[10] = 0;
                payload.words[2] = command;
                switch (kind) {
                case 6:
                    t3_80076b70(first, payload.words, 0);
                    t3_80075dec(first, 1);
                    break;
                case 7:
                    t3_80076b70(second, payload.words, 0);
                    break;
                case 8: t3_80076bd0(0, payload.words); break;
                case 9: t3_80076bd0(1, payload.words); break;
                default: t3_80076bd0(3, payload.words); break;
                }
            } else if (kind - 14 < 48) {
                w_u8(actor + 0xBC, 1);
                group = (kind - 14) >> 4;
                if (!group)
                    group = 3;
                index = (kind - 14) & 7;
                if (kind == 20)
                    t3_800344a4(actor, group, 0, argument);
                else if (kind == 21) {
                    index = t3_80036294(actor) & 0xFF;
                    t3_800344a4(actor, group, index, argument);
                } else {
                    argument = r_u8(script + 2);
                    t3_800344a4(actor, group, index, argument);
                }
            } else {
                switch (kind) {
                case 1: t3_8004acfc(0); break;
                case 2: t3_8004acfc(1); break;
                case 3: t3_8004acfc(2); break;
                case 4:
                    t3_8004ac84(actor + 0xF68);
                    t3_80075dec((uint32)(sint32)r_s16(actor + 18), 2);
                    break;
                case 11:
                case 12:
                case 13:
                    w_u8(actor + 0xE4, (uint8)(kind - 10));
                    w_u16(actor + 0xEC, (uint16)argument);
                    break;
                default: break;
                }
            }
        }
        script += 4;
    } while (r_u16(script));
    return 0;
}

uint32 t3_80076884(uint32 mode, uint32 kind, const uint32 *payload, uint32 flags)
{
    uint32 index, counter, object, value, x, y, z, length = 0, color, origin_x, origin_z, first, second;
    const sint16 *halves = (const sint16 *)payload;
    FUNCTION_MARKER(0x80076884, "SLUS_004.02");
    if (kind & 0x80) {
        index = kind & 0x7F;
        if (index >= 16)
            return 0x80020000;
        switch (index) {
        case 0: return t3_8006e6b8(payload);
        case 1: return t3_8006e760(payload);
        case 2: return t3_8006ef60(mode);
        case 3: return t3_8006e808(payload);
        case 4: return t3_8006e8b0(payload);
        case 15: return t3_bns_03_800b41cc(mode);
        default: return 0x80020000;
        }
    }
    counter = r_u16(0x800A35B8);
    index = (uint32)(sint32)r_s16(0x800A35B8);
    ++counter;
    w_u16(0x800A35B8, (uint16)counter);
    object = 0x800A3408 + (index << 6);
    if ((sint16)counter >= 3)
        w_u16(0x800A35B8, 0);
    t3_800766b4(object, mode, kind);
    w_u16(object + 2, 1);
    if ((sint32)mode < 3) {
        color = r_u32(0x800A35C8 + (mode << 4) + (kind << 2));
        t3_80039f0c(payload, color + 2);
    }
    value = payload[0];
    w_u32(object + 36, value << 7);
    value = payload[1];
    w_u32(object + 40, value << 7);
    value = payload[2];
    w_u32(object + 44, value << 7);
    if (!flags) {
        x = (uint32)(sint32)halves[8];
        y = (uint32)(sint32)halves[9];
        z = (uint32)(sint32)halves[10];
        length = t3_8004aedc(x * x + y * y + z * z);
    }
    if (length) {
        value = (uint32)(sint32)halves[8] * 6400;
        w_u32(object + 48, (uint32)((sint64)(sint32)value / (sint32)length));
        value = (uint32)(sint32)halves[9] * 6400;
        w_u32(object + 52, (uint32)((sint64)(sint32)value / (sint32)length));
        value = (uint32)(sint32)halves[10] * 6400;
        w_u32(object + 56, (uint32)((sint64)(sint32)value / (sint32)length));
    } else {
        w_u32(object + 56, 0);
        w_u32(object + 52, 0);
        w_u32(object + 48, 0);
    }
    z = (uint32)(sint32)halves[10];
    value = payload[2];
    x = payload[0];
    origin_z = r_u32(0x800A85DC);
    origin_x = r_u32(0x800A85D4);
    first = (value - z - origin_z) * (x - origin_x);
    z = value - origin_z;
    y = (uint32)(sint32)halves[8];
    second = (origin_x - (x - y)) * z;
    value = (first + second) >> 31;
    w_u16(object + 60, (uint16)value);
    return value;
}

uint32 t3_80076b70(uint32 mode, const uint32 *payload, uint32 flags)
{
    FUNCTION_MARKER(0x80076B70, "SLUS_004.02");
    t3_80076884(mode, 0, payload, flags);
    return t3_8004a5a8(mode, 0, payload);
}

uint32 t3_8002bce8(uint32 actor)
{
    uint32 index, state;
    FUNCTION_MARKER(0x8002BCE8, "SLUS_004.02");
    index = r_u8(actor + 30);
    state = r_u32(0x80095470 + (index << 2));
    if (state == 1)
        return t3_8002c278(actor);
    if ((sint32)state < 2) {
        if (!state)
            return t3_8002c050(actor);
        return 1;
    }
    if (state == 2)
        return t3_8002c524(actor);
    return 2;
}

uint32 t3_800577dc(uint32 actor)
{
    sint32 count, row;
    uint32 added = 0, entry, action, attributes, flags, mask, category, input, accepted;
    FUNCTION_MARKER(0x800577DC, "SLUS_004.02");
    count = r_s16(actor + 0x70);
    if (count <= 0)
        return 0;
    entry = 0x8009F978;
    for (row = 0; row < count; ++row, entry += 12) {
        if (r_s16(entry + 8))
            continue;
        action = r_u32(entry);
        if (!r_u8(action + 0x2D))
            continue;
        if (r_u8(action + 0x19))
            continue;
        attributes = r_u32(action + 8);
        if (attributes & 0x400000)
            continue;
        if (r_u32(action + 4) & 0x20000)
            continue;
        mask = attributes & 0xFFFF0000;
        category = r_u32(actor + 20);
        if (!(mask & 0x1C0000))
            mask |= 0xC0000;
        flags = r_u32(0x80098230 + (category << 2));
        if (!(flags & mask))
            continue;
        input = r_u32(actor + 0x230);
        flags = attributes & 0xFFFF;
        if ((input & 0xFFFF) == 0x4C02)
            accepted = 1;
        else if (input & 0x400)
            accepted = flags == 0x412 || flags == 0x217 || flags == 0x31F;
        else if (input & 4)
            accepted = flags == 0x10F || flags == 0x51F;
        else if (input & 1)
            accepted = flags == 0x217 || flags == 0x10F;
        else if (input & 2)
            accepted = flags == 0x10F || flags == 0x217 || flags == 0x412;
        else
            accepted = 0;
        if (accepted) {
            w_u16(entry + 8, 1);
            ++added;
        }
    }
    flags = r_u16(actor + 0x72);
    w_u16(actor + 0x72, (uint16)(flags + added));
    return added;
}

uint32 t3_800419b0(uint32 actor, uint32 state, uint32 mode)
{
    uint32 character, action, packed, result, destination, random_value;
    FUNCTION_MARKER(0x800419B0, "SLUS_004.02");
    mode = (uint32)(sint32)(sint16)mode;
    if (mode == 1) {
        if (r_s16(state + 0x70) == -1)
            return 0x80020000;
        action = (uint32)(sint32)r_s16(actor + 26);
        character = (uint32)(sint32)r_s16(actor + 18);
        packed = r_u16(0x8001AD54 + action * 24 + 18);
        packed = (uint32)(sint32)(sint16)packed;
        result = t3_80075914(character, (uint32)((sint32)packed >> 12));
        if (!result)
            t3_8003352c(2, character, packed);
        result = t3_80040c90(character, action, packed);
        w_u16(state + 0x70, 0xFFFF);
        w_u16(state + 0x72, 0);
        w_u16(state + 0x11E, 0);
        w_u16(state + 0x120, 0);
        return result;
    }
    if (!mode || mode == 2) {
        character = (uint32)(sint32)r_s16(actor + 18);
        destination = (mode ? 0x800971E8 : 0x800971F0) + (character << 1);
        w_u16(destination, 1);
        return destination;
    }
    if ((sint32)mode < 2)
        return 1;
    if (mode != 3)
        return 3;
    random_value = (uint32)randPSX() & 0xFF;
    character = (uint32)(sint32)r_s16(state + 18);
    action = (uint32)(sint32)r_s16(0x8001AD4C + ((random_value * 3) >> 8) * 2);
    t3_8003352c(3, character, action);
    return t3_80040df4(character, action);
}

uint32 t3_800679a4(void)
{
    uint32 camera, index, value, x, own_x, z, own_z, angle, difference, snapshot[6];
    sint32 result, first_position, second_position;
    FUNCTION_MARKER(0x800679A4, "SLUS_004.02");
    camera = r_u32(0x800A8CA0);
    if (camera < 30)
        index = (uint32)(sint32)r_s16(0x80024170 + camera * 2) - 1;
    else
        index = (sint32)camera < 0 && camera <= 0x87FFFFFF ? 0xFFFFFFFE : 0xFFFFFFFF;
    result = r_s16(0x80023F18 + index * 20 + 2);
    if (result >= 2)
        return (uint32)result;
    first_position = r_s16(0x800AA4A0);
    second_position = r_s16(0x800ABD2C);
    t3_80063fd4(snapshot, 1);
    if (snapshot[3] + 3639 >= 7279)
        return (uint32)result;
    x = r_u32(0x800ADCB8);
    own_x = r_u32(0x800ADCA8);
    z = r_u32(0x800ADCC0);
    own_z = r_u32(0x800ADCB0);
    angle = t3_8004b39c(x - own_x, z - own_z);
    if (first_position >= second_position)
        angle = (angle + 2048) & 0xFFF;
    difference = snapshot[4] - (angle << 6);
    value = difference & 0x20000 ? difference | 0xFFFC0000 : difference & 0x3FFFF;
    return value + 3639 < 7279 ? 5 : (uint32)result;
}

uint32 t3_8003115c(uint32 index, uint32 actor, uint32 opponent)
{
    uint32 result = 0xFFFFFFFF, flags, base, destination;
    sint32 angle, state;
    FUNCTION_MARKER(0x8003115C, "SLUS_004.02");
    if (r_u8(actor + 0xD1) && !r_u8(opponent + 0xDB) && !r_u8(opponent + 0xDF)) {
        angle = r_s16(actor + 0x30);
        if (angle < 0x2AAA) {
            flags = r_u32(opponent + 0x60);
            base = 0;
            if (flags & 0x80)
                base = flags & 0x200 ? 14 : 10;
            else if (flags & 0x40)
                base = 2;
            else if (flags & 0x20)
                base = 6;
            if (base) {
                state = r_s16(actor + 0x40);
                if (!state)
                    result = base;
                else if (state == 1)
                    result = base + 2;
                else if (state == 3)
                    result = base + 3;
                else if (state == 2)
                    result = base + 1;
            }
        } else if (angle >= 0x5556 && (r_u32(opponent + 0x60) & 2)) {
            state = r_s16(actor + 0x40);
            if (!state)
                result = 18;
            else if (state == 2)
                result = 19;
        }
    }
    destination = 0x80095764 + (index << 2);
    w_u32(destination, result);
    return destination;
}

uint32 t3_80066280(uint32 first, uint32 second, uint32 mode, uint32 distance, uint32 swap, uint32 incoming_result)
{
    uint32 actor = swap ? second : first, opponent = swap ? first : second;
    uint32 animation, previous, category, offset, angle, value;
    sint32 kind;
    FUNCTION_MARKER(0x80066280, "SLUS_004.02");
    kind = r_s16(0x800A8CA4);
    if (kind < 0) {
        value = mode ? r_u32(0x800A8CA0) : 0;
        return t3_80066dfc_host(value, actor, opponent, actor, incoming_result);
    }
    if (!kind) {
        t3_80062a10();
        return 0;
    }
    distance = (uint32)((sint32)(distance * 114) / 128);
    offset = r_u32(0x800A8CAC);
    angle = r_u32(0x800A8CB0);
    t3_80064e10(first, second, offset, angle, mode, distance, 1);
    if (mode)
        return 0;
    animation = r_u32(actor + 0x54);
    previous = r_u32(0x800A8CA8);
    if (animation == previous)
        return 0;
    if (r_s16(actor + 0x74) <= 0)
        return 0;
    if (r_u8(animation + 0x24) == 37)
        return 0;
    w_u32(0x800A8CA8, animation);
    animation = r_u32(actor + 0x54);
    kind = r_s16(0x800A8CDC);
    category = r_u8(animation + 0x24);
    if (kind != 2 && !r_s16(actor + 22) && category == 99)
        t3_80062f24((uint32)(sint32)r_s16(0x800A8C90), actor, opponent);
    return 0;
}

uint32 t3_80067530(uint32 first, uint32 second)
{
    uint32 first_animation, second_animation, first_script, second_script, command;
    sint32 first_frame, second_frame, first_remaining = -1, second_remaining = -1;
    FUNCTION_MARKER(0x80067530, "SLUS_004.02");
    if (r_s16(first + 0x74) <= 0)
        return 1;
    if (r_s16(second + 0x74) >= 0)
        return 1;
    first_animation = r_u32(first + 0x54);
    if (r_u8(first_animation + 0x24) == 9)
        return 1;
    if (r_s16(0x800A8C9A) == 1)
        return 0;
    second_animation = r_u32(second + 0x54);
    first_script = r_u32(first_animation + 0x20);
    first_frame = r_s16(first + 0x58);
    second_frame = r_s16(second + 0x58);
    second_script = r_u32(second_animation + 0x20);
    if (r_u32(second_animation + 4) & 0x400000)
        return 1;
    if (first_script && r_u16(first_script)) {
        do {
            command = r_u16(first_script + 2);
            if ((command & 0xFF00) == 0xB00)
                first_remaining = (sint32)r_u16(first_script) - first_frame;
            first_script += 4;
        } while (r_u16(first_script));
    }
    if (second_script && r_u16(second_script)) {
        do {
            command = r_u16(second_script + 2);
            if ((command & 0xFF00) == 0xC00)
                second_remaining = (sint32)r_u16(second_script) - second_frame;
            second_script += 4;
        } while (r_u16(second_script));
    }
    if (first_remaining < 0 && second_remaining < 0)
        return 0;
    if (second_remaining < first_remaining) {
        if (first_remaining)
            return 0;
    } else if (second_remaining) {
        return 0;
    }
    return r_s16(first + 0x19A) != 33;
}

uint32 t3_80077524(void)
{
    uint32 bank;
    FUNCTION_MARKER(0x80077524, "SLUS_004.02");
    bank = r_u32(0x800ADEFC);
    t3_80076e70(3, 0x800A3408, 0x800A34C8 + bank * 120, 1);
    return t3_8004a614();
}

uint32 t3_8007745c(uint32 update)
{
    uint32 bank;
    FUNCTION_MARKER(0x8007745C, "SLUS_004.02");
    bank = r_u32(0x800ADEFC);
    return t3_80076e70(3, 0x800A3408, 0x800A34C8 + bank * 120, update != 0);
}

uint32 t3_80067260(uint32 category, uint32 alternate, uint32 side, uint32 *selected)
{
    uint32 random_value, entry, index = 0, type, result, first_threshold, next_threshold;
    FUNCTION_MARKER(0x80067260, "SLUS_004.02");
    random_value = (uint32)randPSX() & 0xFFF;
    if ((sint32)category <= 0)
        category = 1;
    if (r_u32(0x800AFA88) == 5 || category - 25 < 18)
        return 0xFFFFFFFF;
    if ((sint32)category < 43)
        entry = 0x80098370 + (category << 3);
    else {
        entry = r_u32(0x800ADC20 + (side << 2));
        entry = r_u32(entry + 32) + ((category - 43) << 3);
    }
    entry += 8;
    first_threshold = r_u16(entry + 2);
    while (first_threshold < random_value) {
        entry += 8;
        first_threshold = r_u16(entry + 2);
        ++index;
    }
    *selected = index;
    w_u16(0x800A8CBC, 0);
    type = r_u16(entry);
    result = r_u32(entry + 4);
    if (type != 1 && alternate) {
        type = r_u16(entry + 8);
        if (!type)
            w_u16(0x800A8CBC, 1);
        else {
            first_threshold = r_u16(entry + 2);
            next_threshold = r_u16(entry + 10);
            if (first_threshold < next_threshold)
                w_u16(0x800A8CBC, 1);
            else
                result = r_u32(entry + 12);
        }
    }
    if ((sint32)result < 0 && result != 0xFFFFFFFF) {
        entry = r_u32(0x800ADC20 + (side << 2));
        entry = r_u32(entry + 36);
        result = (result & 0xFFFF) + entry;
    }
    return result;
}

uint32 t3_80073778(uint32 event)
{
    uint32 counter, limit, numerator, quotient, bank, color, factor, start, end;
    uint32 x, y, z, dx, dy, dz;
    FUNCTION_MARKER(0x80073778, "SLUS_004.02");
    counter = r_u32(event + 0x40) + 1;
    limit = r_u32(event + 0x44);
    w_u32(event + 0x40, counter);
    if ((sint32)counter >= (sint32)limit)
        return t3_8006f5ac(event);
    numerator = counter << 4;
    quotient = limit ? (uint32)((sint64)(sint32)numerator / (sint32)limit) : ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
    bank = r_u32(0x800ADEFC);
    color = r_u16(0x800A8D00 + (quotient << 2));
    w_u16(event + bank * 40 + 0x5E, (uint16)color);
    counter = r_u32(event + 0x40);
    limit = r_u32(event + 0x44);
    numerator = (counter << 10) - counter;
    quotient = limit ? (uint32)((sint64)(sint32)numerator / (sint32)limit) : ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
    factor = (uint32)(sint32)r_s16(0x8001F0C4 + (quotient & 0x3FF) * 2);
    end = r_u32(event + 0x4C);
    start = r_u32(event + 0x48);
    factor = (factor << 2) + 0x2000;
    factor = (end - start) * factor;
    start = r_u16(event + 0x48);
    w_u16(event + 0x18, (uint16)(start + (uint32)((sint32)factor >> 15)));
    t3_8006e120(event + 16);
    dx = (uint32)(sint32)r_s16(event + 0x1E);
    x = r_u32(event + 0x28);
    dy = (uint32)(sint32)r_s16(event + 0x22);
    dz = (uint32)(sint32)r_s16(event + 0x26);
    w_u32(event + 0x28, x + dx);
    y = r_u32(event + 0x2C);
    z = r_u32(event + 0x30);
    w_u32(event + 0x2C, y + dy);
    w_u32(event + 0x30, z + dz);
    y = r_u32(event + 0x2C);
    if ((sint32)y >= 0)
        return t3_8006f5ac(event);
    return y;
}


uint32 t3_800700bc(uint32 event)
{
    uint32 counter, limit, numerator, quotient, bank, color, factor, start, end;
    uint32 x, y, z, dx, dy, dz;
    FUNCTION_MARKER(0x800700BC, "SLUS_004.02");
    counter = r_u32(event + 0x40) + 1;
    limit = r_u32(event + 0x44);
    w_u32(event + 0x40, counter);
    if ((sint32)counter >= (sint32)limit)
        return t3_8006f5ac(event);
    numerator = counter << 4;
    quotient = limit ? (uint32)((sint64)(sint32)numerator / (sint32)limit) : ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
    bank = r_u32(0x800ADEFC);
    color = r_u16(0x800B04A8 + (quotient << 2));
    w_u16(event + bank * 40 + 0x5E, (uint16)color);
    counter = r_u32(event + 0x40);
    limit = r_u32(event + 0x44);
    numerator = (counter << 10) - counter;
    quotient = limit ? (uint32)((sint64)(sint32)numerator / (sint32)limit) : ((sint32)numerator < 0 ? 1 : 0xFFFFFFFF);
    factor = (uint32)(sint32)r_s16(0x8001F0C4 + (quotient & 0x3FF) * 2);
    end = r_u32(event + 0x4C);
    start = r_u32(event + 0x48);
    factor = (factor << 2) + 0x2000;
    factor = (end - start) * factor;
    start = r_u16(event + 0x48);
    w_u16(event + 0x18, (uint16)(start + (uint32)((sint32)factor >> 15)));
    t3_8006e120(event + 16);
    dx = (uint32)(sint32)r_s16(event + 0x1E);
    x = r_u32(event + 0x28);
    dy = (uint32)(sint32)r_s16(event + 0x22);
    dz = (uint32)(sint32)r_s16(event + 0x26);
    w_u32(event + 0x28, x + dx);
    y = r_u32(event + 0x2C);
    z = r_u32(event + 0x30);
    w_u32(event + 0x2C, y + dy);
    w_u32(event + 0x30, z + dz);
    y = r_u32(event + 0x2C);
    if ((sint32)y >= 0)
        return t3_8006f5ac(event);
    return y;
}


uint32 t3_8006eb28(uint32 actor)
{
    uint32 animation, frame, row, event, flag, positions[2][3];
    VECTOR transformed;
    FUNCTION_MARKER(0x8006EB28, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    frame = (uint32)(sint32)r_s16(actor + 0x58);
    if (frame == r_u8(animation + 0x2D))
        t3_80075dec((uint32)(sint32)r_s16(actor + 18), 4);
    SetRotMatrix((MATRIX *)psx_addr(actor + 0x97C, 32));
    for (row = 0; row < 2; ++row) {
        ApplyRotMatrix((SVECTOR *)psx_addr(0x80098820 + row * 8, 8), &transformed);
        positions[row][0] = xport_gte_read_data(9);
        positions[row][1] = xport_gte_read_data(10);
        positions[row][2] = xport_gte_read_data(11);
        positions[row][0] += r_u32(actor + 0x990);
        positions[row][1] += r_u32(actor + 0x994);
        positions[row][2] += r_u32(actor + 0x998);
    }
    event = t3_8006f4f4();
    if (event != 0xFFFFFFFF) {
        w_u8(event + 12, 8);
        frame = (uint32)(sint32)r_s16(actor + 18);
        flag = r_u8(event + 13);
        w_u8(event + 13, (uint8)(flag | (frame ? 8 : 16)));
        w_u32(event + 40, positions[0][0]);
        w_u32(event + 44, positions[0][1]);
        w_u32(event + 48, positions[0][2]);
        t3_8006df34(positions[1][0] - positions[0][0], positions[1][1] - positions[0][1], positions[1][2] - positions[0][2], (uint16 *)psx_addr(event + 16, 4));
    }
    return t3_80039f0c(positions[0], 0x8002517C);
}

uint32 t3_8006eca4(uint32 actor, uint32 mode)
{
    uint32 row, event, character, flag, result, positions[2][3];
    VECTOR transformed;
    FUNCTION_MARKER(0x8006ECA4, "SLUS_004.02");
    SetRotMatrix((MATRIX *)psx_addr(actor + 0x97C, 32));
    for (row = 0; row < 2; ++row) {
        ApplyRotMatrix((SVECTOR *)psx_addr(0x80098830 + row * 8, 8), &transformed);
        positions[row][0] = xport_gte_read_data(9);
        positions[row][1] = xport_gte_read_data(10);
        positions[row][2] = xport_gte_read_data(11);
        positions[row][0] += r_u32(actor + 0x990);
        positions[row][1] += r_u32(actor + 0x994);
        positions[row][2] += r_u32(actor + 0x998);
    }
    event = t3_8006f4f4();
    if (event == 0xFFFFFFFF)
        return event;
    w_u8(event + 12, 10);
    character = (uint32)(sint32)r_s16(actor + 18);
    flag = r_u8(event + 13);
    w_u8(event + 13, (uint8)(flag | (character ? 8 : 16)));
    w_u32(event + 40, positions[0][0]);
    w_u32(event + 44, positions[0][1]);
    w_u32(event + 48, positions[0][2]);
    result = t3_8006df34(positions[1][0] - positions[0][0], positions[1][1] - positions[0][1], positions[1][2] - positions[0][2], (uint16 *)psx_addr(event + 16, 4));
    w_u32(event + 76, mode);
    return result;
}

uint32 t3_8006edfc(uint32 actor)
{
    uint32 animation, frame, row, event, character, flag, result, positions[2][3];
    VECTOR transformed;
    FUNCTION_MARKER(0x8006EDFC, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    frame = (uint32)(sint32)r_s16(actor + 0x58);
    result = r_u8(animation + 0x2D);
    if (frame != result)
        return result;
    SetRotMatrix((MATRIX *)psx_addr(actor + 0xBE0, 32));
    for (row = 0; row < 2; ++row) {
        ApplyRotMatrix((SVECTOR *)psx_addr(0x80098840 + row * 8, 8), &transformed);
        positions[row][0] = xport_gte_read_data(9);
        positions[row][1] = xport_gte_read_data(10);
        positions[row][2] = xport_gte_read_data(11);
        positions[row][0] += r_u32(actor + 0xBF4);
        positions[row][1] += r_u32(actor + 0xBF8);
        positions[row][2] += r_u32(actor + 0xBFC);
    }
    event = t3_8006f4f4();
    if (event == 0xFFFFFFFF)
        return event;
    w_u8(event + 12, 19);
    character = (uint32)(sint32)r_s16(actor + 18);
    flag = r_u8(event + 13);
    w_u8(event + 13, (uint8)(flag | (character ? 8 : 16)));
    w_u32(event + 40, positions[0][0]);
    w_u32(event + 44, positions[0][1]);
    w_u32(event + 48, positions[0][2]);
    return t3_8006df34(positions[1][0] - positions[0][0], positions[1][1] - positions[0][1], positions[1][2] - positions[0][2], (uint16 *)psx_addr(event + 16, 4));
}

uint32 t3_8006e99c(uint32 actor)
{
    uint32 animation, frame, row, input, event, flag, positions[2][3];
    VECTOR transformed;
    FUNCTION_MARKER(0x8006E99C, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    frame = (uint32)(sint32)r_s16(actor + 0x58);
    if (frame == r_u8(animation + 0x2D))
        t3_80075dec((uint32)(sint32)r_s16(actor + 18), 3);
    SetRotMatrix((MATRIX *)psx_addr(actor + 0x97C, 32));
    for (row = 0; row < 2; ++row) {
        input = (uint32)(sint32)r_s16(actor + 18);
        input = 0x80098800 + ((input * 2 + row) << 3);
        ApplyRotMatrix((SVECTOR *)psx_addr(input, 8), &transformed);
        positions[row][0] = xport_gte_read_data(9);
        positions[row][1] = xport_gte_read_data(10);
        positions[row][2] = xport_gte_read_data(11);
        positions[row][0] += r_u32(actor + 0x990);
        positions[row][1] += r_u32(actor + 0x994);
        positions[row][2] += r_u32(actor + 0x998);
    }
    event = t3_8006f4f4();
    if (event != 0xFFFFFFFF) {
        w_u8(event + 12, 6);
        frame = (uint32)(sint32)r_s16(actor + 18);
        flag = r_u8(event + 13);
        w_u8(event + 13, (uint8)(flag | (frame ? 8 : 16)));
        w_u32(event + 40, positions[0][0]);
        w_u32(event + 44, positions[0][1]);
        w_u32(event + 48, positions[0][2]);
        t3_8006df34(positions[1][0] - positions[0][0], positions[1][1] - positions[0][1], positions[1][2] - positions[0][2], (uint16 *)psx_addr(event + 16, 4));
    }
    return t3_80039f0c(positions[0], 0x8002517C);
}

uint32 t3_8002d75c(uint32 actor, uint32 opponent)
{
    uint32 animation, flags, table, motion, result;
    FUNCTION_MARKER(0x8002D75C, "SLUS_004.02");
    animation = r_u32(opponent + 0x1A4);
    flags = r_u16(animation + 0x32);
    if (flags & 0xF000)
        table = 0x800174C4 + (flags & 0xFFF) * 2;
    else
        table = 0x80010CE8 + r_u16(animation + 0x32) * 42;
    motion = r_u16(table);
    w_u16(actor + 0x1A0, (uint16)motion);
    result = t3_8002d178(actor, (uint32)(sint32)(sint16)motion);
    w_u32(actor + 0x1A4, result);
    w_u16(actor + 0x58, 1);
    w_u16(actor + 0x198, 1);
    result = t3_80030f68(actor, 34);
    w_u16(actor + 0x19A, (uint16)result);
    result = r_u16(opponent + 0x19C);
    w_u16(actor + 0x19C, (uint16)result);
    w_u16(opponent + 0x19C, 0);
    w_u16(actor + 0x19E, 0);
    return result;
}

uint32 t3_80044e34(uint32 actor)
{
    static const uint32 offsets[7] = {40, 34, 35, 38, 36, 37, 39};
    uint32 best = actor + 0x13C, candidate = actor + 0x168, offset = 0x13C, pass, item;
    FUNCTION_MARKER(0x80044E34, "SLUS_004.02");
    for (pass = 0; pass < 2; ++pass) {
        if (r_u8(candidate + 33)) {
            uint32 best_score = 0, candidate_score = 0;
            for (item = 0; item < 7; ++item)
                if (r_u8(best + offsets[item]))
                    best_score += item + 1;
            for (item = 0; item < 7; ++item)
                if (r_u8(candidate + offsets[item]))
                    candidate_score += item + 1;
            if (best_score < candidate_score)
                best = candidate;
            else if (best_score == candidate_score) {
                sint32 other = r_s16(candidate + 30);
                sint32 current = r_s16(best + 30);
                if (current < other)
                    best = candidate;
            }
        }
        offset += 44;
        candidate = actor + offset;
    }
    return best;
}

uint32 t3_8006e120(uint32 motion)
{
    uint32 angle, scale, sine, cosine, product, horizontal, next_angle;
    FUNCTION_MARKER(0x8006E120, "SLUS_004.02");
    angle = (uint32)(sint32)r_s16(motion);
    scale = (uint32)(sint32)r_s16(motion + 8);
    angle = ((0 - angle) & 0xFFF) * 2;
    sine = (uint32)(sint32)r_s16(0x8001E8C4 + angle);
    product = scale * sine;
    scale = (uint32)(sint32)r_s16(motion + 8);
    w_u32(motion + 16, product);
    cosine = (uint32)(sint32)r_s16(0x8001F0C4 + angle);
    product = scale * cosine;
    next_angle = (uint32)(sint32)r_s16(motion + 2);
    next_angle = ((0 - next_angle) & 0xFFF) * 2;
    sine = (uint32)(sint32)r_s16(0x8001E8C4 + next_angle);
    horizontal = (uint32)((sint32)product >> 12);
    w_u32(motion + 12, horizontal * sine);
    cosine = (uint32)(sint32)r_s16(0x8001F0C4 + next_angle);
    w_u32(motion + 20, horizontal * cosine);
    return cosine;
}

uint32 t3_8004a990(uint32 position)
{
    uint32 counter, index, record, object, x, z, offset, origin;
    FUNCTION_MARKER(0x8004A990, "SLUS_004.02");
    counter = r_u16(0x8009F2A0);
    index = (uint32)(sint32)r_s16(0x8009F2A0);
    ++counter;
    record = 0x8009E850 + index * 880;
    w_u16(0x8009F2A0, (uint16)counter);
    if ((sint16)counter >= 3)
        w_u16(0x8009F2A0, 0);
    object = record + 16;
    w_u16(record, 1);
    x = r_u32(position);
    w_u32(record + 8, 0xFFFF8300);
    w_u32(record + 4, x << 7);
    z = r_u32(position + 8);
    w_u32(record + 12, z << 7);
    t3_800766b4(object, 3, 3);
    w_u16(object + 2, 1);
    offset = (uint32)(sint32)r_s16(0x8001E818);
    origin = r_u32(record + 4);
    w_u32(object + 36, origin + (offset << 7));
    offset = (uint32)(sint32)r_s16(0x8001E81A);
    origin = r_u32(record + 8);
    w_u32(object + 40, origin + (offset << 7));
    offset = (uint32)(sint32)r_s16(0x8001E81C);
    origin = r_u32(record + 12);
    offset <<= 7;
    w_u32(object + 44, origin + offset);
    return offset;
}

uint32 t3_8006dfc4(uint32 x, uint32 z, uint32 angle)
{
    uint32 magnitude_x, magnitude_z, factor, numerator, quotient;
    FUNCTION_MARKER(0x8006DFC4, "SLUS_004.02");
    magnitude_x = (sint32)x < 0 ? 0 - x : x;
    magnitude_z = (sint32)z < 0 ? 0 - z : z;
    angle = (angle & 0xFFF) * 2;
    if ((sint32)magnitude_z < (sint32)magnitude_x) {
        factor = r_u16(0x8001E8C4 + angle) << 3;
        numerator = x << 16;
    } else {
        factor = r_u16(0x8001F0C4 + angle) << 3;
        numerator = z << 16;
    }
    if (!(uint16)factor)
        factor = 1;
    quotient = (uint32)((sint64)(sint32)numerator / (sint16)factor);
    quotient = (uint32)((sint32)quotient >> 1);
    return (sint32)quotient < 0 ? 0 - quotient : quotient;
}

uint32 t3_8006df34(uint32 x, uint32 y, uint32 z, uint16 *angles)
{
    uint32 angle, length;
    FUNCTION_MARKER(0x8006DF34, "SLUS_004.02");
    angle = ((uint32)(sint32)(sint16)t3_8004b39c(x, z) - 1024) & 0xFFF;
    angles[1] = (uint16)angle;
    length = t3_8006dfc4(x, z, angle);
    angle = ((uint32)(sint32)(sint16)t3_8004b39c(y, length) - 1024) & 0xFFF;
    angles[0] = (uint16)angle;
    return angle;
}

uint32 t3_8006e760(const uint32 *payload)
{
    uint32 row, event, value;
    FUNCTION_MARKER(0x8006E760, "SLUS_004.02");
    for (row = 0; row < 20; ++row) {
        event = t3_8006f4f4();
        if (event == 0xFFFFFFFF)
            return event;
        w_u8(event + 12, 2);
        value = payload[0];
        w_u32(event + 40, value);
        value = payload[1];
        w_u32(event + 44, value);
        value = payload[2];
        w_u32(event + 48, value);
        value = ((const uint16 *)payload)[8];
        w_u16(event + 16, (uint16)value);
        value = ((const uint16 *)payload)[9];
        w_u16(event + 18, (uint16)value);
    }
    return 0;
}

uint32 t3_8006e6b8(const uint32 *payload)
{
    uint32 row, event, value;
    FUNCTION_MARKER(0x8006E6B8, "SLUS_004.02");
    for (row = 0; row < 6; ++row) {
        event = t3_8006f4f4();
        if (event == 0xFFFFFFFF)
            return event;
        w_u8(event + 12, 1);
        value = payload[0];
        w_u32(event + 40, value);
        value = payload[1];
        w_u32(event + 44, value);
        value = payload[2];
        w_u32(event + 48, value);
        value = ((const uint16 *)payload)[8];
        w_u16(event + 16, (uint16)value);
        value = ((const uint16 *)payload)[9];
        w_u16(event + 18, (uint16)value);
    }
    return 0;
}

uint32 t3_8002cd7c(uint32 command)
{
    uint32 index, table;
    FUNCTION_MARKER(0x8002CD7C, "SLUS_004.02");
    command &= 0xFFFF;
    if (command <= 0xC00D) {
        if (command == 0xC001)
            return 0x8001006C;
        if (command == 0xC002)
            return 0x80010078;
        return 0;
    }
    if (command <= 0xC7FE) {
        index = command - 0xC00E;
        if (index >= 63)
            return 0;
        table = 0x800954D0;
    } else {
        index = command - 0xC7FF;
        if (index >= 41)
            return 0;
        table = 0x800955CC;
    }
    return r_u32(table + index * 4);
}

uint32 t3_8006f0f4(uint32 actor, uint32 enabled)
{
    uint32 animation, sequence, category, flag;
    sint32 frame;
    FUNCTION_MARKER(0x8006F0F4, "SLUS_004.02");
    animation = r_u32(actor + 0x54);
    sequence = r_u32(animation + 0x28);
    if (!enabled)
        return 0x800A0000;
    flag = r_u8(0x800A0991);
    if (flag)
        return flag;
    category = r_u8(sequence) - 24;
    if (category >= 5)
        return 0;
    frame = r_s16(actor + 0x58);
    if (frame < r_u8(animation + 0x2D))
        return 1;
    if (r_u8(animation + 0x2E) < frame)
        return 0x80020000;
    switch (category) {
    case 0: return t3_8006e99c(actor);
    case 1: return t3_8006eca4(actor, 0);
    case 2: return t3_8006eb28(actor);
    case 3: return t3_8006edfc(actor);
    default: return t3_8006eca4(actor, 1);
    }
}

uint32 t3_80058a84(uint32 actor)
{
    sint32 total, count, selected, index, wanted;
    uint32 random_value, seed, flags, entry, sequence, command;
    FUNCTION_MARKER(0x80058A84, "SLUS_004.02");
    total = r_s16(actor + 0x72);
    if (total <= 0)
        return 0xFFFFFFFF;
    random_value = (uint32)randPSX();
    seed = r_u32(0x800ADCA0);
    wanted = (sint32)((uint32)total * ((random_value + seed) & 0x7FFF)) / 32768;
    w_u32(0x800ADCA0, seed * 5 + 3);
    flags = r_u16(actor + 2);
    count = r_s16(actor + 0x70);
    if (flags & 1) {
        entry = 0x8009F978;
        index = 0;
        while (index < count) {
            sint32 enabled = r_s16(entry + 8);
            ++index;
            entry += 12;
            if (enabled > 0 && --wanted <= 0)
                break;
        }
        selected = index - 1;
    } else {
        entry = 0x8009F96C + (uint32)count * 12;
        index = count - 1;
        while (index >= 0) {
            sint32 enabled = r_s16(entry + 8);
            --index;
            entry -= 12;
            if (enabled > 0 && --wanted < 0)
                break;
        }
        selected = index + 1;
    }
    if (selected < 0)
        return 0xFFFFFFFF;
    entry = 0x8009F978 + (uint32)selected * 12;
    sequence = r_u32(entry + 4);
    command = r_u32(entry);
    flags = r_u16(sequence);
    t3_800618c0(actor, flags, command);
    sequence = r_u32(entry + 4);
    t3_80058958(actor, sequence);
    return (uint32)selected;
}

uint32 t3_800618c0(uint32 actor, uint32 command, uint32 context)
{
    uint32 category = command & 0xC000, sequence, present, flags, bits, result, row;
    FUNCTION_MARKER(0x800618C0, "SLUS_004.02");
    if (category) {
        sequence = t3_8002cd7c(command & 0xFFFF);
        if (!sequence || category != 0xC000)
            return 0;
        present = (uint32)(sint32)r_s16(sequence + 4);
        flags = r_u16(sequence + 2);
        w_u32(actor + 8, present ? sequence + 4 : 0);
        bits = (flags & 0x100) ? 128 : 0;
        if (flags & 0x200)
            bits |= 16;
        if (flags & 0x400)
            bits |= 64;
        if (flags & 0x800)
            bits |= 32;
        bits |= r_u16(0x8002301C + (flags & 15) * 2);
        w_u16(actor + 4, (uint16)bits);
        result = 0xFFFFFFFF;
    } else {
        flags = (command >> 5) & 0x1FF;
        row = 9;
        sequence = 0x8002304E;
        do {
            if (r_u16(sequence) & flags)
                break;
            --row;
            sequence -= 2;
        } while (row > 0);
        bits = r_u16(0x80023050 + row * 2);
        flags = 0;
        for (row = 0; row < 4; ++row) {
            if (command & 1)
                flags = (flags & 0xFFFF) | r_u16(0x80023064 + row * 2);
            else
                flags &= 0xFFFF;
            command = (command & 0xFFFF) >> 1;
        }
        w_u16(actor + 4, (uint16)(bits | flags));
        result = r_u16(actor + 4);
    }
    w_u32(actor + 0x5C, context);
    return result;
}

uint32 t3_80070508(uint32 event)
{
    uint32 counter, limit, quotient, bank, color, x, y, z, dx, dy, dz;
    FUNCTION_MARKER(0x80070508, "SLUS_004.02");
    counter = r_u32(event + 0x40);
    limit = r_u32(event + 0x44);
    ++counter;
    w_u32(event + 0x40, counter);
    if ((sint32)counter >= (sint32)limit)
        return t3_8006f5ac(event);
    counter <<= 4;
    if (!limit)
        quotient = (sint32)counter < 0 ? 1 : 0xFFFFFFFF;
    else
        quotient = (uint32)((sint64)(sint32)counter / (sint32)limit);
    bank = r_u32(0x800ADEFC);
    color = r_u16(0x800B04A8 + (quotient << 2));
    w_u16(event + bank * 40 + 0x5E, (uint16)color);
    dx = (uint32)(sint32)r_s16(event + 0x1E);
    x = r_u32(event + 0x28);
    dy = (uint32)(sint32)r_s16(event + 0x22);
    dz = (uint32)(sint32)r_s16(event + 0x26);
    w_u32(event + 0x28, x + dx);
    y = r_u32(event + 0x2C);
    z = r_u32(event + 0x30);
    w_u32(event + 0x2C, y + dy);
    w_u32(event + 0x30, z + dz);
    y = r_u32(event + 0x2C);
    if ((sint32)y >= 0)
        return t3_8006f5ac(event);
    return y;
}

uint32 t3_8004b218(uint32 basis, uint32 *first_output, const SVECTOR *vector, uint32 *output)
{
    uint32 xy, z, first[3], second[3], third[3], result;
    FUNCTION_MARKER(0x8004B218, "SLUS_004.02");
    xport_gte_write_data(0, r_u32(basis));
    xport_gte_write_data(1, r_u32(basis + 4));
    xport_gte_execute(0x4A486012);
    xy = r_u32(basis + 8);
    z = r_u32(basis + 12);
    first[0] = xport_gte_read_data(9);
    first[1] = xport_gte_read_data(10);
    first[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, xy);
    xport_gte_write_data(1, z);
    xport_gte_execute(0x4A486012);
    xy = r_u32(basis + 16);
    z = r_u32(basis + 20);
    second[0] = xport_gte_read_data(9);
    second[1] = xport_gte_read_data(10);
    second[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, xy);
    xport_gte_write_data(1, z);
    xport_gte_execute(0x4A486012);
    third[0] = xport_gte_read_data(9);
    third[1] = xport_gte_read_data(10);
    third[2] = xport_gte_read_data(11);
    first_output[0] = first[0];
    first_output[1] = first[1];
    first_output[2] = first[2];
    xport_gte_write_control(0, (first[1] << 16) | (first[0] & 0xFFFF));
    xport_gte_write_control(1, (second[0] << 16) | (first[2] & 0xFFFF));
    xport_gte_write_control(2, (second[2] << 16) | (second[1] & 0xFFFF));
    result = (third[1] << 16) | (third[0] & 0xFFFF);
    xport_gte_write_control(3, result);
    xport_gte_write_control(4, third[2]);
    xy = (uint16)vector->vx | ((uint32)(uint16)vector->vy << 16);
    z = (uint16)vector->vz;
    xport_gte_write_data(0, xy);
    xport_gte_write_data(1, z);
    xport_gte_execute(0x4A486012);
    output[0] = xport_gte_read_data(9);
    output[1] = xport_gte_read_data(10);
    output[2] = xport_gte_read_data(11);
    return result;
}

uint32 t3_8003e370(void)
{
    uint32 actor;
    FUNCTION_MARKER(0x8003E370, "SLUS_004.02");
    if (r_u32(0x800AFA88) == 8) {
        if (!r_u32(0x800B6C04)) {
            actor = t3_bns_02_800b297c();
            if (r_s32(actor + 0x3F4) <= 0)
                return 1;
        } else {
            if (r_s32(0x800A961C) <= 0)
                return 1;
            if (r_s32(0x800AAEA8) <= 0)
                return 1;
        }
    } else {
        if (!r_u32(0x800A961C))
            return 1;
        if (!r_u32(0x800AAEA8))
            return 1;
    }
    return r_s32(0x800ADBCC) <= 0;
}

uint32 t3_80063d00(uint32 x, uint32 y, uint32 z, uint32 pitch, uint32 yaw, uint32 *output)
{
    uint32 index, pitch_sine, pitch_cosine, yaw_sine, yaw_cosine;
    uint32 dx, dy, dz, first_squared, second_squared, first_distance, distance, component;
    FUNCTION_MARKER(0x80063D00, "SLUS_004.02");
    yaw += 0x10000;
    if ((sint32)pitch < 0)
        pitch += 63;
    index = ((uint32)((sint32)pitch >> 5)) & 0x1FFE;
    pitch_sine = (uint32)(sint32)r_s16(0x8001E8C4 + index);
    pitch_cosine = (uint32)(sint32)r_s16(0x8001F0C4 + index);
    if ((sint32)yaw < 0)
        yaw += 63;
    dx = r_u32(0x800ADCA8) - x;
    dy = r_u32(0x800ADCAC) - y;
    dz = r_u32(0x800ADCB0) - z;
    first_squared = dx * dx + dy * dy + dz * dz;
    dx = r_u32(0x800ADCB8) - x;
    dy = r_u32(0x800ADCBC) - y;
    second_squared = dx * dx + dy * dy + dz * dz;
    index = ((uint32)((sint32)yaw >> 5)) & 0x1FFE;
    yaw_sine = (uint32)(sint32)r_s16(0x8001E8C4 + index);
    yaw_cosine = (uint32)(sint32)r_s16(0x8001F0C4 + index);
    first_distance = t3_8002a324(first_squared);
    distance = (first_distance + t3_8002a324(second_squared)) >> 1;
    component = (uint32)((sint32)(distance * yaw_cosine) / 4096);
    component = (uint32)((sint32)(component * pitch_cosine) / 4096);
    output[0] = x + component;
    output[1] = y + (uint32)((sint32)(distance * pitch_sine) / 4096);
    component = (uint32)((sint32)(distance * yaw_sine) / 4096);
    component = (uint32)((sint32)(component * pitch_cosine) / 4096);
    output[2] = z + component;
    return distance;
}

uint32 t3_80067cec(uint32 first, uint32 second)
{
    uint32 actor = first, opponent = second, side, kind, active, entry, row, table, value, random_value;
    FUNCTION_MARKER(0x80067CEC, "SLUS_004.02");
    if (r_u8(first + 0xD0) && r_u8(second + 0xD0))
        return 0xFFFFFFFF;
    for (side = 0; side < 2; ++side) {
        if (r_u8(actor + 0xD0)) {
            active = 0;
            if (r_u8(actor + 0xDE))
                active = r_s16(actor + 0xE8) >= 28;
            kind = 3;
            if (!active) {
                kind = 2;
                if (!r_u8(actor + 0xD3))
                    kind = r_u8(actor + 0xD4) != 0;
            }
            entry = 0x800242A4;
            for (row = 0; row < 4; ++row, entry += 6) {
                if (kind != 3) {
                    sint32 actual = r_s16(opponent + 22), expected = r_s16(entry);
                    if (actual != expected)
                        continue;
                    actual = r_s16(opponent + 0xA0);
                    expected = r_s16(entry + 2);
                    if (actual != expected)
                        continue;
                }
                value = (uint32)(r_s16(entry + 4) - 300);
                random_value = (uint32)randPSX() & 0xFFF;
                table = r_u32(0x8002428C + (value << 3));
                value = (uint32)(sint32)r_s16(table);
                if ((sint32)value < (sint32)random_value) {
                    table += 20;
                    for (;;) {
                        value = (uint32)(sint32)r_s16(table);
                        if ((sint32)value >= 4097)
                            return 0xFFFFFFFF;
                        table += 20;
                        if ((sint32)value >= (sint32)random_value) {
                            table -= 20;
                            break;
                        }
                    }
                }
                value = r_u32(table + kind * 4 + 4);
                if (r_s16(value) == -1)
                    return 0xFFFFFFFF;
                w_u32(0x800A0438, value);
                table = r_u32(0x800A0458 + side * 4);
                table = r_u32(table);
                {
                    uint32 a = r_u32(table), b = r_u32(table + 4), c = r_u32(table + 8);
                    w_u32(0x800A0448, a);
                    w_u32(0x800A044C, b);
                    w_u32(0x800A0450, c);
                }
                value = r_u32(table + 12);
                w_u32(0x800A0454, value);
                return side;
            }
        }
        actor = second;
        opponent = first;
    }
    w_u32(0x800A0438, 0);
    return 0xFFFFFFFF;
}

uint32 t3_8002ca84(uint32 actor, uint32 pattern)
{
    uint32 index, cursor, command, buttons, edges, flags, low, history, matched;
    sint32 remaining;
    FUNCTION_MARKER(0x8002CA84, "SLUS_004.02");
    index = r_u32(actor + 0x40C);
    command = r_u16(pattern + 2);
    remaining = r_s16(pattern);
    cursor = pattern + 2;
    if (command) {
        cursor += 2;
        do {
            command = r_u16(cursor);
            cursor += 2;
        } while (command);
        cursor -= 2;
    }
    cursor -= 2;
    command = r_u16(cursor);
    low = command & 15;
    if (low == command) {
        index = index < 59 ? index + 1 : 0;
    } else {
        history = r_u8(actor + index + 0x41C);
        if (low != (history & 15))
            return 0;
        flags = command >> 8;
        edges = r_u16(actor + 0x406);
        buttons = r_u16(actor + 0x404);
        if ((flags & 31) == 16)
            matched = 1;
        else if (flags & 16)
            matched = (buttons & flags & 15) != 0;
        else {
            low = flags & 15;
            if (!low && !buttons)
                matched = 1;
            else
                matched = (buttons & low) && low == (edges & low);
        }
        if (!matched)
            return 0;
        cursor -= 2;
        if (cursor == pattern)
            return 1;
        command = r_u16(cursor);
    }
    while (remaining > 0) {
        index = (sint32)index > 0 ? index - 1 : 59;
        history = r_u8(actor + index + 0x41C);
        matched = 0;
        if ((command & 15) == (history & 15)) {
            flags = command >> 8;
            buttons = history >> 4;
            if ((flags & 31) == 16)
                matched = 1;
            else if (flags & 16)
                matched = flags & buttons;
            else
                matched = (flags & 15) == buttons;
        }
        if (matched) {
            cursor -= 2;
            if (cursor == pattern)
                return 1;
            command = r_u16(cursor);
        } else if (history >> 4) {
            return 0;
        }
        --remaining;
    }
    return 0;
}

uint32 t3_8002cc28(uint32 actor, uint32 condition, uint32 mask, uint32 buttons)
{
    uint32 command, edges, low, index, table;
    FUNCTION_MARKER(0x8002CC28, "SLUS_004.02");
    command = r_u16(condition);
    if (command < 0xC000) {
        if (!(command & mask) && (command & 0x3FE0))
            return 0;
        edges = r_u16(actor + 0x406);
        if ((command & 31) == 16)
            return 1;
        buttons &= 0xFFFF;
        low = command & 15;
        if (command & 16)
            return (low & buttons) != 0;
        if (!low && !buttons)
            return 1;
        return (low & buttons) && low == (edges & low);
    }
    if (command <= 0xC00D) {
        if (command == 0xC001)
            return t3_8002c8b4(actor);
        if (command == 0xC002)
            return t3_8002c99c(actor);
        return 0;
    }
    if (command <= 0xC7FE) {
        index = command - 0xC00E;
        if (index >= 63)
            return 0;
        table = 0x800954D0;
    } else {
        index = command - 0xC7FF;
        if (index >= 41)
            return 0;
        table = 0x800955CC;
    }
    return t3_8002ca84(actor, r_u32(table + index * 4));
}

uint32 t3_8002c8b4(uint32 actor)
{
    uint32 index = r_u32(actor + 0x40C), value;
    sint32 remaining = 19;
    FUNCTION_MARKER(0x8002C8B4, "SLUS_004.02");
    if ((r_u8(actor + index + 0x41C) & 15) != 6)
        return 0;
    index = (sint32)index > 0 ? index - 1 : 59;
    if ((r_u8(actor + index + 0x41C) & 15) != 5)
        return 0;
    do {
        index = (sint32)index > 0 ? index - 1 : 59;
        value = r_u8(actor + index + 0x41C) & 15;
        if (value != 5) {
            if (value != 6)
                return 0;
            break;
        }
        --remaining;
    } while (remaining > 0);
    if (remaining <= 0)
        return 0;
    do {
        index = (sint32)index > 0 ? index - 1 : 59;
        if ((r_u8(actor + index + 0x41C) & 15) != 6)
            break;
        --remaining;
    } while (remaining > 0);
    return remaining > 0;
}

uint32 t3_8002c99c(uint32 actor)
{
    uint32 index = r_u32(actor + 0x40C), value;
    sint32 remaining = 19;
    FUNCTION_MARKER(0x8002C99C, "SLUS_004.02");
    if ((r_u8(actor + index + 0x41C) & 15) != 4)
        return 0;
    index = (sint32)index > 0 ? index - 1 : 59;
    if ((r_u8(actor + index + 0x41C) & 15) != 5)
        return 0;
    do {
        index = (sint32)index > 0 ? index - 1 : 59;
        value = r_u8(actor + index + 0x41C) & 15;
        if (value != 5) {
            if (value != 4)
                return 0;
            break;
        }
        --remaining;
    } while (remaining > 0);
    if (remaining <= 0)
        return 0;
    do {
        index = (sint32)index > 0 ? index - 1 : 59;
        if ((r_u8(actor + index + 0x41C) & 15) != 4)
            break;
        --remaining;
    } while (remaining > 0);
    return remaining > 0;
}

uint32 t3_80046d34(uint32 first, uint32 second)
{
    union { uint32 words[32]; uint16 halves[64]; } snapshot;
    uint32 first_origin = first + 0xF68, second_origin = second + 0xF68;
    uint32 first_reference, second_reference, row, shape, pair;
    uint32 penetration = 0, crossing = 0, x, y, z, radius, distance, candidate;
    uint32 first_x, first_y, first_z, second_x, second_y, second_z;
    uint32 first_length, second_length, heading, first_heading, second_heading, difference;
    uint32 first_cosine, first_sine, second_cosine, second_sine, first_share, second_share, total;
    uint32 adjusted_first_x, adjusted_first_z, adjusted_second_x, adjusted_second_z, result;
    FUNCTION_MARKER(0x80046D34, "SLUS_004.02");
    first_reference = r_u8(first + 0x1E);
    second_reference = r_u8(second + 0x1E);
    w_u32(first + 0x3E8, 0);
    w_u32(first + 0x3EC, 0);
    w_u32(first + 0x3F0, 0);
    w_u32(second + 0x3E8, 0);
    w_u32(second + 0x3EC, 0);
    w_u32(second + 0x3F0, 0);
    first_reference = 0x8009E6F8 + first_reference * 16;
    second_reference = 0x8009E6F8 + second_reference * 16;
    for (row = 0; row < 8; ++row) {
        uint32 source = second + 0x324 + row * 16;
        uint32 a = r_u32(source), b = r_u32(source + 4);
        uint32 c = r_u32(source + 8), d = r_u32(source + 12);
        snapshot.words[row * 4] = a;
        snapshot.words[row * 4 + 1] = b;
        snapshot.words[row * 4 + 2] = c;
        snapshot.words[row * 4 + 3] = d;
    }
    for (shape = first + 0x324; shape < first + 0x3A4; shape += 16) {
        if (!r_s16(shape + 12))
            continue;
        for (pair = 0; pair < 8; ++pair) {
            uint16 *other = snapshot.halves + pair * 8;
            uint32 other_y, own_y, other_z, own_z, own_radius, own_x;
            if (!(sint16)other[6])
                continue;
            other_y = other[2];
            own_y = r_u16(shape + 4);
            other_z = other[4];
            own_z = r_u16(shape + 8);
            own_radius = r_u16(shape + 12);
            own_x = r_u16(shape);
            radius = own_radius + other[6];
            x = (uint32)(sint32)(sint16)(other[0] - own_x);
            y = (uint32)(sint32)(sint16)(other_y - own_y);
            z = (uint32)(sint32)(sint16)(other_z - own_z);
            if ((sint32)((sint32)x < 0 ? 0 - x : x) >= (sint32)(uint16)radius ||
                (sint32)((sint32)y < 0 ? 0 - y : y) >= (sint32)(uint16)radius ||
                (sint32)((sint32)z < 0 ? 0 - z : z) >= (sint32)(uint16)radius)
                continue;
            distance = t3_8004aedc(x * x + y * y + z * z);
            candidate = radius - distance;
            if ((sint32)(penetration << 16) < (sint32)(candidate << 16))
                penetration = (uint16)candidate;
        }
    }
    if (r_u8(r_u32(first + 0x54) + 0x2D) && r_u8(r_u32(second + 0x54) + 0x2D)) {
        uint32 close_y = (sint16)penetration > 0;
        if (!close_y) {
            y = r_u32(first_origin + 4);
            z = r_u32(second_origin + 4);
            difference = y - z;
            close_y = (sint32)((sint32)difference < 0 ? z - y : difference) < 300;
        }
        if (close_y) {
            sint32 angle = r_s16(first + 14);
            uint32 delta_x = r_u32(second);
            uint32 own_x = r_u32(first);
            uint32 delta_z = r_u32(second + 8);
            uint32 own_z = r_u32(first + 8);
            uint32 cosine, sine, first_point_x, first_point_z, second_point_x, second_point_z;
            delta_x -= own_x;
            delta_z -= own_z;
            heading = (1024 - angle / 16) & 0xFFF;
            cosine = (uint32)(sint32)r_s16(0x8001F0C4 + heading * 2) * 200;
            sine = (uint32)(sint32)r_s16(0x8001E8C4 + heading * 2) * 200;
            first_point_x = r_u32(first_origin) + (uint32)((sint32)cosine / 4096);
            y = r_u32(first_origin + 8);
            angle = r_s16(second + 14);
            first_point_z = y + (uint32)((sint32)sine / 4096);
            heading = (1024 - angle / 16) & 0xFFF;
            cosine = (uint32)(sint32)r_s16(0x8001F0C4 + heading * 2) * 200;
            sine = (uint32)(sint32)r_s16(0x8001E8C4 + heading * 2) * 200;
            second_point_x = r_u32(second_origin) + (uint32)((sint32)cosine / 4096);
            second_point_z = r_u32(second_origin + 8) + (uint32)((sint32)sine / 4096);
            x = second_point_x - first_point_x;
            z = second_point_z - first_point_z;
            if ((sint32)((sint32)x < 0 ? 0 - x : x) < 500 &&
                (sint32)((sint32)z < 0 ? 0 - z : z) < 500 &&
                (sint32)(delta_x * x + delta_z * z) < 0) {
                crossing = 1;
                if ((sint16)penetration < 400)
                    penetration = 400;
            }
        }
    }
    result = penetration << 16;
    if ((sint32)result <= 0)
        return result;
    first_x = r_u32(first_origin);
    x = (uint32)(sint32)(sint16)(first_x - r_u16(first_reference + 4));
    first_y = r_u32(first_origin + 4);
    y = (uint32)(sint32)(sint16)(first_y - r_u16(first_reference + 8));
    first_z = r_u32(first_origin + 8);
    z = (uint32)(sint32)(sint16)(first_z - r_u16(first_reference + 12));
    second_x = r_u32(second_origin);
    second_z = r_u32(second_origin + 8);
    second_y = r_u32(second_origin + 4);
    first_length = (uint16)t3_8004aedc(x * x + y * y + z * z);
    x = (uint32)(sint32)(sint16)(second_x - r_u16(second_reference + 4));
    y = (uint32)(sint32)(sint16)(second_y - r_u16(second_reference + 8));
    z = (uint32)(sint32)(sint16)(second_z - r_u16(second_reference + 12));
    second_length = t3_8004aedc(x * x + y * y + z * z);
    first_heading = 1024 - r_s16(first + 14) / 16;
    x = r_u32(second_origin);
    y = r_u32(first_origin);
    z = r_u32(second_origin + 8);
    radius = r_u32(first_origin + 8);
    heading = t3_8004b39c(x - y, z - radius);
    difference = (first_heading - (uint32)(sint32)(sint16)heading) & 0xFFF;
    if ((difference < 2048 ? difference : 4096 - difference) > 1024)
        first_heading += 2048;
    second_heading = 1024 - r_s16(second + 14) / 16;
    difference = (second_heading - (uint32)(sint32)(sint16)(heading + 2048)) & 0xFFF;
    if ((difference < 2048 ? difference : 4096 - difference) > 1024)
        second_heading += 2048;
    second_heading = (second_heading & 0xFFF) * 2;
    first_heading = (first_heading & 0xFFF) * 2;
    second_cosine = (uint32)(sint32)r_s16(0x8001F0C4 + second_heading);
    first_sine = (uint32)r_u16(0x8001E8C4 + first_heading);
    first_cosine = (uint32)r_u16(0x8001F0C4 + first_heading);
    second_sine = (uint32)(sint32)r_s16(0x8001E8C4 + second_heading);
    first_sine = (uint32)(sint32)(sint16)first_sine;
    first_cosine = (uint32)(sint32)(sint16)first_cosine;
    adjusted_first_x = first_x + (uint32)((sint32)(second_cosine * (uint16)second_length) / 4096);
    adjusted_first_z = first_z + (uint32)((sint32)(second_sine * (uint16)second_length) / 4096);
    adjusted_second_x = second_x + (uint32)((sint32)(first_cosine * first_length) / 4096);
    adjusted_second_z = second_z + (uint32)((sint32)(first_sine * first_length) / 4096);
    second_length = (uint16)second_length;
    if (!(first_length | second_length))
        first_length = second_length = 1;
    total = first_length + second_length;
    if (!total)
        total = 1;
    first_share = (uint32)((sint32)((uint32)(sint32)(sint16)penetration * first_length) / (sint32)total);
    second_share = (uint32)((sint32)((uint32)(sint32)(sint16)penetration * second_length) / (sint32)total);
    adjusted_first_x -= (uint32)((sint32)(first_cosine * first_share) / 4096);
    adjusted_first_z -= (uint32)((sint32)(first_sine * first_share) / 4096);
    adjusted_second_x -= (uint32)((sint32)(second_cosine * second_share) / 4096);
    adjusted_second_z -= (uint32)((sint32)(second_sine * second_share) / 4096);
    result = adjusted_first_x - r_u32(first_origin);
    w_u32(first + 0x3E8, crossing ? result : (uint32)((sint32)result >> 1));
    result = first_y - r_u32(first_origin + 4);
    w_u32(first + 0x3EC, crossing ? result : (uint32)((sint32)result >> 1));
    result = adjusted_first_z - r_u32(first_origin + 8);
    w_u32(first + 0x3F0, crossing ? result : (uint32)((sint32)result >> 1));
    result = adjusted_second_x - r_u32(second_origin);
    w_u32(second + 0x3E8, crossing ? result : (uint32)((sint32)result >> 1));
    result = second_y - r_u32(second_origin + 4);
    w_u32(second + 0x3EC, crossing ? result : (uint32)((sint32)result >> 1));
    result = adjusted_second_z - r_u32(second_origin + 8);
    if (!crossing)
        result = (uint32)((sint32)result >> 1);
    w_u32(second + 0x3F0, result);
    return result;
}

uint32 t3_800476d0(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x800476D0, "SLUS_004.02");
    return t3_80046d34(first, second);
}

uint32 t3_8003d6b0(uint32 first, uint32 second)
{
    uint32 value, state, mode, palette, flag, left, right, timer, record, player0, player1;
    uint32 health0, maximum0, health1, maximum1;
    FUNCTION_MARKER(0x8003D6B0, "SLUS_004.02");
    value = r_u32(0x800ADCA4);
    if (value)
        return value;
    if (!r_u16(0x800ADED4) && !r_u8(0x800AFA9B)) {
        state = r_u32(0x80096F2C);
        if (!state) {
            if (r_u32(0x80095460)) {
                t3_8003dcc8();
                t3_8003da8c();
            }
        } else if (state == 2) {
            if (r_u32(0x800AFA88) != 8 || (r_u16(0x800ADE78) & 1)) {
                t3_8003df68();
                if (!r_u32(0x8009548C))
                    t3_8003dec0();
            }
        } else if (state == 6 || state == 9) {
            mode = r_u32(0x800AFA88);
            if (mode != 8 && mode != 5) {
                t3_8003e170();
                if (r_u8(0x800AFAA9) || !r_u32(0x80095480) || r_u32(0x8009548C))
                    t3_8003e060();
            }
        }
    }
    if (!r_u16(0x800AE200))
        return 0;
    flag = r_u8(0x800AFA8F);
    palette = r_u32(0x800A9218);
    if (flag)
        t3_8004eab4();
    mode = r_u32(0x800AFA88);
    if (mode != 6) {
        value = r_u32(0x800ADD54);
        w_u32(0x800A9218, value);
    }
    if (mode == 8) {
        t3_bns_02_800b32ac();
        t3_8004ed10();
        timer = 0xFFFFFFFF;
        if (!r_u32(0x800954B0)) {
            value = r_u32(0x800ADBCC) + 59;
            timer = (uint32)((sint32)value / 60);
        }
        t3_8004e128(timer, 184, 22, 0x7F1C);
        record = t3_bns_02_800b297c();
        player0 = t3_bns_02_800b29ac(0);
        player1 = t3_bns_02_800b29ac(1);
        t3_bns_02_800b2c34(record, player0, player1);
    } else {
        if (mode == 6)
            t3_8004ce74(0x8001A37C, 2, 1, 126, 260);
        t3_8004e8b4();
        if (mode == 2) {
            if (r_u8(0x80098A41) != 2 || r_u8(0x800A8672))
                t3_bns_00_800b1764();
        } else {
            left = (uint32)r_s16(first + 68);
            right = (uint32)r_s16(second + 68);
            t3_8004e594(left, right);
        }
        t3_8004ed10();
        timer = 0xFFFFFFFF;
        if (!r_u32(0x800954B0)) {
            value = r_u32(0x800ADBCC) + 59;
            timer = (uint32)((sint32)value / 60);
        }
        t3_8004e128(timer, 184, 22, 0x7F1C);
        health0 = r_u32(first + 0x3F4);
        maximum0 = r_u32(first + 0x3F8);
        health1 = r_u32(second + 0x3F4);
        maximum1 = r_u32(second + 0x3F8);
        t3_8004e274(health0, maximum0, health1, maximum1);
    }
    w_u32(0x800A9218, palette);
    return 0x800B0000;
}

uint32 t3_8003dec0(void)
{
    uint32 flags, sound;
    FUNCTION_MARKER(0x8003DEC0, "SLUS_004.02");
    if (r_u32(0x80096F30) != 1)
        return 1;
    flags = r_u16(0x800ADE78);
    if (flags & 1)
        sound = r_u16(0x80097076);
    else if (flags & 8)
        sound = r_u16(0x80097082);
    else if (flags & 4)
        sound = r_u16(0x8009707A);
    else if (flags & 16)
        sound = r_u16(0x80097080);
    else
        sound = r_u16(0x80097084);
    return t3_800753bc(0, sound, 0);
}

uint32 t3_8003df68(void)
{
    uint32 mode, maximum, round, flags;
    FUNCTION_MARKER(0x8003DF68, "SLUS_004.02");
    mode = r_u32(0x800AFA88);
    if (mode == 7) {
        maximum = r_u32(0x800AFAC4);
        round = r_u32(0x800AFAC0);
        --maximum;
        if ((sint32)round < (sint32)maximum)
            return maximum;
    }
    if (r_u32(0x80096F34))
        return 0x800B0000;
    flags = r_u16(0x800ADE78);
    if (flags & 1)
        return t3_8004ce74(0x8001A4C0, 3, 2, 107, 228);
    if (flags & 8)
        return t3_8004ce74(0x8001A4D0, 2, 2, 96, 228);
    if (flags & 4)
        return t3_8004ce74(0x8001A4E4, 1, 2, 63, 228);
    return t3_8004ce74(0x8001A4F8, 1, 2, 140, 228);
}

uint32 t3_8003e060(void)
{
    uint32 notify, side, flags, result;
    sint32 first, second;
    FUNCTION_MARKER(0x8003E060, "SLUS_004.02");
    if (r_u32(0x80096F30) != 1)
        return 1;
    notify = r_u8(0x800AFAA9) != 0;
    first = r_s16(0x800A926E);
    second = r_s16(0x800AAAFA);
    if (first == second)
        result = t3_80040c00(0x1014);
    else {
        side = second >= first;
        flags = r_u16(0x800ADF10);
        if (flags == 3 || r_u32(0x800AFA88) == 2) {
            result = t3_800757a8(side);
            if (!result)
                result = t3_8006f1d4();
            else
                result = t3_80040c00(0x1011);
        } else {
            flags = r_u16(0x800ADF10);
            if ((flags >> side) & 1)
                result = t3_80040c00(0x1011);
            else {
                t3_80040c00(0x1012);
                result = r_u8(0x800AFA92);
                if (!result)
                    notify = 0;
            }
        }
    }
    if (notify)
        return t3_8006b6c8(0);
    return result;
}

uint32 t3_8003e170(void)
{
    sint32 first, second, character;
    uint32 winner, loser, flags, record, name, length, position, value;
    FUNCTION_MARKER(0x8003E170, "SLUS_004.02");
    first = r_s16(0x800A926E);
    second = r_s16(0x800AAAFA);
    if (first == second)
        return t3_8004ce74(0x8001A508, 6, 2, 140, 368);
    winner = second >= first;
    loser = 1 - winner;
    flags = r_u16(0x800ADF10);
    if (flags == 3 || r_u32(0x800AFA88) == 2) {
        character = r_s16(0x800A923C + winner * 6284);
        record = t3_8004efc8(character);
        name = r_u32(record);
        length = t3_8007a418(name);
        position = (uint32)(sint32)(sint16)(184 - (length * 11 + 66));
        name = r_u32(record);
        t3_8004ce74(0x8001A518, 4, 2, position, 368, name);
        value = r_u8(0x800AFAA9);
        if (!value)
            return value;
        value = r_u32(0x800AFA88);
        if (value)
            return value;
        t3_8004ce74(0x8001A52C, 9, 1, winner * 196 + 47, 104);
        return t3_8004ce74(0x8001A53C, 9, 1, loser * 195 + 54, 104);
    }
    flags = r_u16(0x800ADF10);
    if ((flags >> winner) & 1)
        return t3_8004ce74(0x8001A54C, 4, 2, 96, 368);
    return t3_8004ce74(0x8001A560, 5, 2, 96, 368);
}

uint32 t3_8006f1d4(void)
{
    uint32 event;
    FUNCTION_MARKER(0x8006F1D4, "SLUS_004.02");
    event = t3_8006f4f4();
    if (event != 0xFFFFFFFF)
        w_u8(event + 12, 16);
    return 16;
}

uint32 t3_8006b6c8(uint32 level)
{
    FUNCTION_MARKER(0x8006B6C8, "SLUS_004.02");
    t3_8006bc98((sint32)level, 180);
    w_u16(0x800A05D8, 3);
    w_u32(0x800A866C, 0);
    return 0x800B0000;
}

uint32 t3_80043d84(uint32 first, uint32 second)
{
    uint32 first_side, second_side, value, row, actor, other, x, y, z, dx, dy, dz;
    uint32 matrix_x, matrix_y, matrix_z;
    FUNCTION_MARKER(0x80043D84, "SLUS_004.02");
    first_side = r_u8(first + 30);
    second_side = r_u8(second + 30);
    if (first_side >= 2)
        first_side = 1u << ((first_side - 1) & 31);
    if (second_side >= 2)
        second_side = 1u << ((second_side - 1) & 31);
    value = r_u32(0x8009E660 + ((first_side + second_side) << 4));
    if (value >= 3001)
        return 0;
    if (r_u8(first + 217)) {
        value = r_u8(second + 217);
        if (value)
            return value;
    }
    value = r_u8(first + 194);
    if (value == 1)
        return value;
    value = r_u8(second + 194);
    if (value == 1)
        return value;
    if (!r_u8(first + 195) || !r_u8(second + 195))
        return 0;
    t3_800476d0(first, second);
    for (row = 0; row < 2; ++row) {
        actor = row ? second : first;
        other = row ? first : second;
        if (!r_u32(actor + 0x3E8) && !r_u32(actor + 0x3EC) && !r_u32(actor + 0x3F0))
            continue;
        w_u8(actor + 213, 1);
        value = r_u8(other + 30);
        w_u8(actor + 214 + value, 1);
        if (r_u32(0x800AFA88) == 8 && r_s32(0x800954A0) >= 2) {
            if (r_u8(actor + 217)) {
                w_u32(actor + 0x3F0, 0);
                w_u32(actor + 0x3EC, 0);
                w_u32(actor + 0x3E8, 0);
            } else {
                dx = r_u32(actor + 0x3E8);
                dz = r_u32(actor + 0x3F0);
                w_u32(actor + 0x3E8, dx << 1);
                dy = r_u32(actor + 0x3EC);
                w_u32(actor + 0x3F0, dz << 1);
                w_u32(actor + 0x3EC, dy << 1);
            }
        }
        x = r_u32(actor);
        dx = r_u32(actor + 0x3E8);
        y = r_u32(actor + 4);
        dy = r_u32(actor + 0x3EC);
        w_u32(actor, x + dx);
        z = r_u32(actor + 8);
        dz = r_u32(actor + 0x3F0);
        w_u32(actor + 4, y + dy);
        matrix_x = r_u32(actor + 0xF68);
        dx = r_u32(actor + 0x3E8);
        w_u32(actor + 8, z + dz);
        matrix_y = r_u32(actor + 0xF6C);
        dy = r_u32(actor + 0x3EC);
        w_u32(actor + 0xF68, matrix_x + dx);
        matrix_z = r_u32(actor + 0xF70);
        dz = r_u32(actor + 0x3F0);
        w_u32(actor + 0xF6C, matrix_y + dy);
        w_u32(actor + 0xF70, matrix_z + dz);
    }
    return 0;
}
