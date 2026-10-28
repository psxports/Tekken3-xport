#include "game_functions.h"

void t3_bns_00_800b0794(uint32 state)
{
    uint32 selection;
    uint32 character;
    uint32 costume;

    FUNCTION_MARKER(0x800B0794, "SLUS_004.02-BNS-logical-00");
    if (r_u8(state + 0x11))
        return;
    selection = (r_u16(0x800AE1F8) == 0) * 2;
    character = r_u16(0x800ADD5C + selection);
    costume = r_u16(0x800ADD98 + selection);
    t3_bns_00_800b1fcc(state + 0x90, character * 4 + costume);
    w_u32(state + 0xBC, 0);
}

void t3_bns_00_800b08f8(uint32 state)
{
    uint32 row;
    uint32 controller;
    uint32 offset;
    uint32 character_slot;
    uint32 costume_slot;

    FUNCTION_MARKER(0x800B08F8, "SLUS_004.02-BNS-logical-00");
    row = state + r_u32(state + 0x24) * 4 + 0x90;
    w_u8(0x800AE211, r_u8(row + 2));
    if (r_u8(state + 0x1C) != 1)
        return;
    controller = r_u8(state + 0x1F);
    offset = controller * 2;
    character_slot = 0x800ADD5C + offset;
    w_u16(character_slot, r_u8(row));
    costume_slot = 0x800ADD98 + offset;
    w_u16(costume_slot, r_u8(row + 1));
    w_u16(0x800A9128 + offset, 1);
    t3_8004f0e4(controller);
    w_u8(row, r_u8(character_slot));
    w_u8(row + 1, r_u8(costume_slot));
}


uint32 t3_bns_00_800b0808(uint32 state)
{
    uint32 index;
    uint32 team;
    uint32 other;
    uint32 flags;
    uint32 value;

    FUNCTION_MARKER(0x800B0808, "SLUS_004.02-BNS-logical-00");
    w_u32(state + 0x24, 0);
    w_u32(state + 0x28, 0);
    w_u8(state + 0x58, 0);
    for (index = 0; index < 16; ++index)
        w_u16(state + 0x56 - index * 2, 0);
    for (index = 0; index < 2; ++index) {
        team = state + 0x59 + index * 13;
        other = state + 0x59 + ((index + 1) & 1) * 13;
        flags = 0x800AE1F8 + index * 2;
        w_u8(team + 8, 0xFF);
        w_u8(team + 9, 0);
        if (r_u16(flags) == 0) {
            value = r_u8(other + 12);
            w_u8(team + 10, 0);
            w_u8(team + 12, value);
            w_u8(team + 11, r_u8(other + 11) == 0);
        }
        t3_bns_00_800b225c(r_u16(flags), team, other);
    }
    return 0;
}

uint32 t3_800517b0(uint32 controller)
{
    uint32 actor;

    FUNCTION_MARKER(0x800517B0, "SLUS_004.02");
    actor = 0x800A9228 + controller * 6284;
    w_u32(actor + 0x3F4, 0);
    w_u32(actor + 0x3FC, 0);
    return actor;
}

uint32 t3_bns_00_800b09b0(uint32 state)
{
    uint32 controller;
    uint32 team;
    uint32 row;
    uint32 packed;
    uint32 character;
    uint32 costume;
    uint32 offset;
    uint32 difficulty;
    uint32 variation;
    uint32 next;
    uint32 total;

    FUNCTION_MARKER(0x800B09B0, "SLUS_004.02-BNS-logical-00");
    for (controller = 0; controller < 2; ++controller) {
        team = state + 0x59 + controller * 13;
        row = r_u8(team + 9);
        if (row != r_u8(team + 8)) {
            w_u8(team + 8, row);
            t3_800517b0(controller);
            packed = r_u8(team + row);
            offset = controller * 2;
            w_u16(0x800ADD5C + offset, packed >> 2);
            w_u16(0x800ADD98 + offset, packed & 3);
            w_u16(0x800A9128 + offset, r_u16(0x800AE1F8 + offset) == 0);
            t3_8004f0e4(controller);
            character = r_u16(0x800ADD5C + offset);
            costume = r_u16(0x800ADD98 + offset);
            w_u8(team + row, (character << 2) | costume);
        }
    }
    difficulty = 0;
    if (r_u8(state + 0x1C) == 1) {
        controller = r_u8(state + 0x1F);
        team = state + 0x59 + controller * 13;
        difficulty = r_u16(0x800AE208);
        variation = r_u32(0x800AE218);
        next = r_u8(team + 9);
        total = r_u8(team + 12);
        difficulty += 3 + (variation & 3);
        if (next + 2 == total)
            ++difficulty;
        else if (next + 1 == total)
            difficulty += 3;
        if ((sint32)difficulty >= 10)
            difficulty = 9;
    }
    w_u8(0x800AE211, difficulty);
    return 0x800B0000;
}

uint32 t3_8004cd80(uint32 *pairs, uint32 count, uint32 direction)
{
    uint32 index;
    uint32 position;
    uint32 left;
    uint32 right;
    uint32 key;
    uint32 label;

    FUNCTION_MARKER(0x8004CD80, "SLUS_004.02");
    if ((sint32)count < 2)
        return 1;
    for (index = 0; index < count - 1; ++index) {
        position = index + 1;
        while (position) {
            left = (position - 1) * 2;
            right = position * 2;
            if (direction ? pairs[right + 1] >= pairs[left + 1]
                          : pairs[left + 1] >= pairs[right + 1])
                break;
            key = pairs[left + 1];
            label = pairs[left];
            pairs[left + 1] = pairs[right + 1];
            pairs[left] = pairs[right];
            pairs[right + 1] = key;
            pairs[right] = label;
            --position;
        }
    }
    return 0;
}

uint32 t3_bns_00_800b225c(uint32 human, uint32 team, uint32 other)
{
    uint32 pairs[44];
    uint8 pool[22];
    uint32 mask;
    uint32 chosen;
    uint32 reserved;
    uint32 selected;
    uint32 total;
    uint32 count;
    uint32 index;
    uint32 label;
    uint32 needed;
    uint32 delta;
    uint32 threshold;
    uint32 eligible;
    uint32 missing;
    uint32 random;
    uint32 choice;
    uint32 last;
    uint32 costume;
    uint32 packed;
    uint32 other_count;
    uint32 scan;
    uint32 collision;
    uint32 frequency;

    FUNCTION_MARKER(0x800B225C, "SLUS_004.02-BNS-logical-00");
    mask = r_u32(0x80097EF0);
    selected = r_u8(team + 10);
    mask &= 0x1F7FFF;
    for (index = 0; index < selected; ++index) {
        label = r_u8(team + index) >> 2;
        mask &= ~(1u << (label & 31));
    }
    reserved = 0;
    chosen = 0;
    if (human && r_u8(team + 12) >= 3) {
        count = 0;
        for (index = 0; index < 22; ++index) {
            if (mask & (1 << index)) {
                pairs[count * 2 + 1] = t3_80051660(index);
                pairs[count * 2] = index;
                ++count;
            }
        }
        if (count)
            t3_8004cd80(pairs, count, 1);
        if (count >= 2) {
            reserved = 1;
            random = t3_8004ce54();
            label = pairs[(random & 1) * 2];
            chosen |= 1 << (label & 31);
            mask &= ~(1 << (label & 31));
        }
    }
    count = 0;
    for (index = 0; index < 22; ++index) {
        if (mask & (1 << index)) {
            frequency = r_u16(0x800AFAFC + index * 2);
            pairs[count * 2] = index;
            pairs[count * 2 + 1] = frequency;
            ++count;
        }
    }
    if (count)
        t3_8004cd80(pairs, count, 1);
    selected = r_u8(team + 10);
    total = r_u8(team + 12);
    needed = total - (selected + reserved);
    delta = 0;
    do {
        threshold = pairs[1] + delta;
        eligible = 0;
        while (eligible < count && threshold >= pairs[eligible * 2 + 1])
            ++eligible;
        ++delta;
    } while ((sint32)eligible < (sint32)needed);
    mask = 0;
    for (index = 0; index < eligible; ++index)
        mask |= 1 << (pairs[index * 2] & 31);
    count = 0;
    for (index = 0; index < 22; ++index) {
        if (mask & (1 << index))
            pool[count++] = (uint8)index;
    }
    w_u32(0x800B4A48, count);
    total = r_u8(team + 12);
    selected = r_u8(team + 10);
    missing = total - selected;
    for (index = reserved; (sint32)index < (sint32)missing; ++index) {
        random = t3_8004ce54();
        count = r_u32(0x800B4A48);
        choice = random % count;
        --count;
        w_u32(0x800B4A48, count);
        last = pool[count];
        label = pool[choice];
        chosen |= 1 << (label & 31);
        pool[choice] = (uint8)last;
    }
    count = 0;
    for (index = 0; index < 22; ++index) {
        if (chosen & (1 << index))
            pool[count++] = (uint8)index;
    }
    w_u32(0x800B4A48, count);
    total = r_u8(team + 12);
    selected = r_u8(team + 10);
    costume = r_u8(team + 11);
    if (selected < total) {
        for (index = selected; index < total; ++index) {
            random = t3_8004ce54();
            count = r_u32(0x800B4A48);
            choice = random % count;
            --count;
            w_u32(0x800B4A48, count);
            last = pool[count];
            label = pool[choice];
            pool[choice] = (uint8)last;
            other_count = r_u8(other + 10);
            packed = label * 4 + costume;
            collision = 0;
            for (scan = 0; scan < other_count; ++scan) {
                if (r_u8(other + scan) == packed) {
                    collision = 1;
                    break;
                }
            }
            if (collision)
                packed = label * 4 + (costume ^ 1);
            w_u8(team + index, packed);
        }
        total = r_u8(team + 12);
    }
    for (index = 0; index < total; ++index) {
        label = r_u8(team + index) >> 2;
        frequency = r_u16(0x800AFAFC + label * 2);
        w_u16(0x800AFAFC + label * 2, frequency + 1);
    }
    return total ? team + total : 0;
}

uint32 t3_bns_00_800b1fcc(uint32 destination, uint32 packed_selection)
{
    uint8 pool[22];
    uint32 alternate;
    uint32 boss;
    uint32 player;
    uint32 mask;
    uint32 first_mask;
    uint32 count;
    uint32 index;
    uint32 row;
    uint32 random;
    uint32 choice;
    uint32 last;
    uint32 character;

    FUNCTION_MARKER(0x800B1FCC, "SLUS_004.02-BNS-logical-00");
    alternate = (packed_selection & 3) != 0;
    boss = 13;
    player = (uint32)((sint32)packed_selection >> 2);
    if (player == 13)
        boss = 9;
    mask = r_u32(0x80097EF0);
    if (r_u32(0x800AFA88) == 3) {
        mask &= 0x157FFF;
    } else {
        mask &= 0x1FFFFF;
        if (r_u8(0x80097F1E))
            mask |= 0x20000;
    }
    mask &= 0xFFEF9FFF;
    mask &= ~(1u << (player & 31));
    mask &= ~(1 << boss);
    first_mask = mask & 0x3FF;
    count = 0;
    for (index = 0; index < 22; ++index) {
        if (first_mask & (1 << index))
            pool[count++] = (uint8)index;
    }
    w_u32(0x800B4A48, count);
    for (row = 0; row < 4; ++row) {
        random = t3_8004ce54();
        count = r_u32(0x800B4A48);
        choice = random % count;
        --count;
        w_u32(0x800B4A48, count);
        last = pool[count];
        character = pool[choice];
        pool[choice] = (uint8)last;
        mask &= ~(1 << (character & 31));
        w_u8(destination + row * 4, character);
    }
    count = 0;
    for (index = 0; index < 22; ++index) {
        if (mask & (1 << index))
            pool[count++] = (uint8)index;
    }
    w_u32(0x800B4A48, count);
    for (; row < 8; ++row) {
        random = t3_8004ce54();
        count = r_u32(0x800B4A48);
        choice = random % count;
        --count;
        w_u32(0x800B4A48, count);
        last = pool[count];
        character = pool[choice];
        pool[choice] = (uint8)last;
        w_u8(destination + row * 4, character);
    }
    w_u8(destination + 32, boss);
    w_u8(destination + 36, 14);
    for (row = 0; row < 10; ++row) {
        w_u8(destination + row * 4 + 2, row);
        character = r_u8(destination + row * 4);
        w_u8(destination + row * 4 + 1,
             character == 20 || character == 14 ? 0 : alternate);
    }
    return 0;
}

uint32 t3_bns_00_800b2870(uint32 rows)
{
    FUNCTION_MARKER(0x800B2870, "SLUS_004.02-BNS-logical-00");
    w_u8(rows + 0x24, 20);
    return 20;
}

uint32 t3_bns_03_800b065c(uint32 index)
{
    uint32 slot;

    FUNCTION_MARKER(0x800B065C, "SLUS_004.02-BNS-logical-03");
    slot = 0x800B6678 + index * 6;
    w_u16(slot, 0);
    w_u8(slot + 2, 0);
    w_u8(slot + 3, 0);
    w_u8(slot + 4, 0);
    w_u8(slot + 5, 0);
    return slot;
}

uint32 t3_bns_03_800b068c(uint32 actor)
{
    uint32 index;
    uint32 slot;

    FUNCTION_MARKER(0x800B068C, "SLUS_004.02-BNS-logical-03");
    index = (uint32)r_s16(actor + 0x12);
    slot = 0x800B6678 + index * 6;
    w_u16(slot, 0);
    w_u8(slot + 2, 0);
    w_u8(slot + 3, 0);
    w_u8(slot + 4, 0);
    w_u8(slot + 5, 0);
    switch (r_u16(actor + 0xA0)) {
    case 0x18:
    case 0x1A:
        w_u16(slot, 0x8000);
        w_u8(slot + 2, 30);
        w_u8(slot + 3, 3);
        w_u8(slot + 4, 9);
        return slot;
    case 0x1E:
    case 0x20:
        w_u16(slot, 0x8000);
        w_u8(slot + 2, 150);
        w_u8(slot + 3, 5);
        w_u8(slot + 4, 12);
        w_u8(slot + 5, 18);
        return slot;
    case 0x1F:
    case 0x21:
        w_u8(slot + 5, 18);
        return 18;
    case 0x19:
    case 0x1B:
    case 0x1C:
    case 0x1D:
        return 0x800B0798;
    default:
        return 0x800B0000;
    }
}

uint32 t3_80076c28(uint32 value, uint32 actor, uint32 side, const uint32 *event)
{
    uint32 selected, character, slot, kind;
    uint16 angles[2];
    union { uint32 words[6]; uint16 halves[12]; } payload;
    FUNCTION_MARKER(0x80076C28, "SLUS_004.02");
    t3_80076884(3, 0, event, 0);
    t3_8004a5a8(3, 0, event);
    if ((sint32)value >= 21) {
        t3_80076884(side, 0, event, 1);
        t3_8004a5a8(side, 0, event);
    }
    selected = 0x800A9228 + side * 6284;
    character = r_u8(r_u32(r_u32(selected + 84) + 40));
    if (character == 25)
        return 27;
    if (character == 27)
        return 28;
    if (character == 28)
        return 24;
    if (character == 24 || character == 26) {
        t3_8006ef60(r_u8(actor + 30));
        slot = r_u8(actor + 30);
        return t3_8004a5a8(slot, 0x82, 0);
    }
    if ((sint32)value < 21)
        return 26;
    selected = side ? 0x800AAAB4 : 0x800A9228;
    t3_8006df34(r_u32(selected + 0x1B8) - r_u32(selected + 0x1AC), r_u32(selected + 0x1BC) - r_u32(selected + 0x1B0), r_u32(selected + 0x1C0) - r_u32(selected + 0x1B4), angles);
    payload.words[0] = event[0];
    payload.words[1] = event[1];
    payload.words[2] = event[2];
    payload.halves[8] = angles[0];
    payload.halves[10] = (uint16)value;
    payload.halves[9] = angles[1];
    if (r_s16(selected + 0x122)) {
        t3_8006e8b0(payload.words);
        t3_8004a5a8(side, 0x84, payload.words);
        t3_8006e808(payload.words);
        kind = 0x83;
    } else {
        t3_8006e760(payload.words);
        t3_8004a5a8(side, 0x81, payload.words);
        t3_8006e6b8(payload.words);
        kind = 0x80;
    }
    return t3_8004a5a8(side, kind, payload.words);
}

void t3_80064170(uint32 x0, uint32 y0, uint32 z0, uint32 pitch0, uint32 yaw0, uint32 focal0, uint32 x1, uint32 y1, uint32 z1, uint32 pitch1, uint32 yaw1, uint32 focal1, uint32 weight)
{
    uint32 first[3], second[3], target[3], position[3], delta[3];
    uint32 length0, length1, length, other, product, difference, pitch, yaw;
    uint32 index, sine_pitch, cosine_pitch, sine_yaw, cosine_yaw, offset, focal;
    uint32 output_pitch, output_yaw, row;
    FUNCTION_MARKER(0x80064170, "SLUS_004.02");
    other = 4096 - weight;
    length0 = t3_80063d00(x0, y0, z0, pitch0, yaw0, first);
    length1 = t3_80063d00(x1, y1, z1, pitch1, yaw1, second);
    product = length0 * weight + length1 * other;
    length = (uint32)((sint32)product / 4096);
    for (row = 0; row < 3; ++row) {
        product = first[row] * weight + second[row] * other;
        target[row] = (uint32)((sint32)product / 4096);
    }
    difference = pitch1 - pitch0;
    difference = (difference & 0x20000) ? (difference | 0xFFFC0000) : (difference & 0x3FFFF);
    product = difference * other;
    pitch = pitch0 + (uint32)((sint32)product / 4096);
    difference = yaw1 - yaw0;
    difference = (difference & 0x20000) ? (difference | 0xFFFC0000) : (difference & 0x3FFFF);
    product = difference * other;
    yaw = yaw0 + (uint32)((sint32)product / 4096);
    index = pitch;
    if ((sint32)index < 0)
        index += 63;
    index = (uint32)((sint32)index >> 5) & 0x1FFE;
    sine_pitch = (uint32)(sint32)r_s16(0x8001E8C4 + index);
    cosine_pitch = (uint32)(sint32)r_s16(0x8001F0C4 + index);
    index = yaw;
    if ((sint32)index < 0)
        index += 63;
    index = (((uint32)((sint32)index >> 6) + 1024) & 0xFFF) * 2;
    cosine_yaw = (uint32)(sint32)r_s16(0x8001F0C4 + index);
    sine_yaw = (uint32)(sint32)r_s16(0x8001E8C4 + index);
    product = length * cosine_yaw;
    offset = (uint32)((sint32)product / 4096);
    product = offset * cosine_pitch;
    position[0] = target[0] - (uint32)((sint32)product / 4096);
    product = length * sine_pitch;
    position[1] = target[1] - (uint32)((sint32)product / 4096);
    product = length * sine_yaw;
    offset = (uint32)((sint32)product / 4096);
    product = offset * cosine_pitch;
    position[2] = target[2] - (uint32)((sint32)product / 4096);
    product = focal0 * weight + focal1 * other;
    focal = (uint32)((sint32)product / 4096);
    for (row = 0; row < 3; ++row)
        delta[row] = target[row] - position[row];
    product = delta[0] * delta[0] + delta[2] * delta[2];
    length = t3_8002a324(product);
    output_pitch = (uint32)((sint32)(sint16)t3_8004b39c(length, delta[1]) * 64);
    output_yaw = (uint32)((sint32)(sint16)t3_8004b39c(delta[2], 0 - delta[0]) * 64);
    w_u32(0x800A02F0, position[0]);
    w_u32(0x800A02F4, position[1]);
    w_u32(0x800A02F8, position[2]);
    w_u32(0x800A02FC, output_pitch);
    w_u32(0x800A0300, output_yaw);
    w_u32(0x800A0304, focal);
    t3_80064080(3);
    t3_80063c64((sint32)focal);
}

uint32 t3_80040ad0(uint32 actor)
{
    uint32 frame, history, cursor, sample, pressed, slot;
    FUNCTION_MARKER(0x80040AD0, "SLUS_004.02");
    frame = r_u32(0x80095464);
    history = r_u32(0x80095460);
    if (frame == history)
        return history;
    if (r_u8(actor + 0x8C) != 1 || !(r_u32(actor + 0xE0) & 0xFFFFFF)) {
        cursor = r_u32(actor + 0x40C);
        slot = r_u8(actor + 0x1E);
        sample = r_u8(actor + cursor + 0x41C);
        history = r_u16(0x80097048 + slot * 2);
        pressed = sample & (sample ^ history);
        if ((sample & 0x30) == 0x30 && (pressed & 0x30)) {
            w_u8(actor + 0xE2, 1);
        } else if (pressed & 0x10) {
            w_u8(actor + 0xE0, 1);
        } else if (pressed & 0x20) {
            w_u8(actor + 0xE1, 1);
        } else {
            w_u8(actor + 0xE2, 0);
            w_u8(actor + 0xE0, 0);
            w_u8(actor + 0xE1, 0);
        }
    } else {
        slot = r_u8(actor + 0x1E);
        sample = r_u16(0x80097048 + slot * 2);
    }
    slot = r_u8(actor + 0x1E);
    history = 0x80097048 + slot * 2;
    w_u16(history, (uint16)sample);
    return history;
}

static uint32 t3_grid_divide(uint32 numerator, uint32 denominator)
{
    if (!denominator)
        return (sint32)numerator < 0 ? 1 : 0xFFFFFFFF;
    return (uint32)((sint64)(sint32)numerator / (sint32)denominator);
}

uint32 t3_8006d95c(uint32 field_of_view, uint32 yaw, uint32 x, uint32 z, uint32 cell_width, uint32 *selection)
{
    uint32 index;
    uint32 negative;
    uint32 lower;
    uint32 upper;
    uint32 angle;
    uint32 sine;
    uint32 cosine;
    uint32 stride;
    uint32 x_offset;
    uint32 z_offset;
    uint32 x_step;
    uint32 z_step;
    uint32 cell_x;
    uint32 cell_z;
    /* Valid stage bounds guarantee both endpoint assignments */
    uint32 endpoint_x[2];
    uint32 endpoint_z[2];
    uint32 ray;
    uint32 row;
    uint32 column;
    uint32 end;
    uint32 fill;
    uint32 origin_x;
    uint32 origin_z;
    uint32 minimum;
    uint32 value;
    FUNCTION_MARKER(0x8006D95C, "SLUS_004.02");
    for (index = 36; index > 0; --index)
        selection[index - 1] = 0;
    negative = 0 - cell_width;
    lower = negative * 2;
    upper = cell_width * 2;
    if ((sint32)x < (sint32)lower)
        x = lower + negative;
    if ((sint32)x >= (sint32)upper)
        x = upper + cell_width - 1;
    if ((sint32)z < (sint32)lower)
        z = lower + negative;
    if ((sint32)z >= (sint32)upper)
        z = upper + cell_width - 1;
    for (ray = 0; ray < 2; ++ray) {
        uint32 half_angle = (uint32)((sint32)field_of_view >> 1);
        angle = 0 - yaw;
        angle = (ray ? angle + half_angle : angle - half_angle) & 4095;
        sine = (uint32)(sint32)r_s16(0x8001E8C4 + angle * 2);
        stride = r_u32(0x800A8D70);
        cosine = (uint32)(sint32)r_s16(0x8001F0C4 + angle * 2);
        z_offset = z - r_u32(0x800A8C50);
        x_offset = x - r_u32(0x800A8C44);
        x_step = (uint32)((sint32)(stride * sine) >> 15);
        z_step = (uint32)((sint32)(stride * cosine) >> 15);
        for (;;) {
            cell_x = t3_grid_divide(x_offset, cell_width);
            cell_z = t3_grid_divide(z_offset, cell_width);
            if (cell_x >= 6)
                break;
            x_offset += x_step;
            if (cell_z >= 6)
                break;
            endpoint_x[ray] = cell_x;
            endpoint_z[ray] = cell_z;
            index = (uint32)(sint32)r_s16(0x80025804 + (cell_z * 6 + cell_x) * 2);
            z_offset += z_step;
            selection[index] = 1;
        }
    }
    cell_x = endpoint_x[0];
    cell_z = endpoint_z[0];
    for (;;) {
        if (cell_z == 5 && (sint32)cell_x < 5) {
            ++cell_x;
            selection[cell_x + 30] = 1;
            if (cell_x == endpoint_x[1] && endpoint_z[1] == 5)
                break;
        }
        if (cell_x == 5 && (sint32)cell_z > 0) {
            --cell_z;
            selection[cell_z * 6 + 5] = 1;
            if (cell_z == endpoint_z[1] && endpoint_x[1] == 5)
                break;
        }
        if (cell_z == 0 && (sint32)cell_x > 0) {
            --cell_x;
            selection[cell_x] = 1;
            if (cell_x == endpoint_x[1] && endpoint_z[1] == 0)
                break;
        }
        if (cell_x == 0 && (sint32)cell_z < 5) {
            ++cell_z;
            selection[cell_z * 6] = 1;
            if (cell_z == endpoint_z[1] && endpoint_x[1] == 0)
                break;
        }
    }
    for (row = 0; row < 6; ++row) {
        for (column = 0; column < 6; ++column) {
            if (selection[row * 6 + column] && column < 5) {
                for (end = 5; end > column; --end) {
                    if (selection[row * 6 + end]) {
                        for (fill = column; fill < end; ++fill)
                            selection[row * 6 + fill] = 1;
                    }
                }
            }
        }
    }
    origin_x = t3_grid_divide(x - r_u32(0x800A8C44), cell_width);
    origin_z = t3_grid_divide(z - r_u32(0x800A8C50), cell_width);
    selection[origin_z * 6 + origin_x] = 0xFFFFFFFE;
    minimum = 32767;
    value = t3_8006deac(selection, origin_x, origin_z, 2, 2);
    if ((sint32)value < (sint32)minimum)
        minimum = value;
    value = t3_8006deac(selection, origin_x, origin_z, 2, 3);
    if ((sint32)value < (sint32)minimum)
        minimum = value;
    value = t3_8006deac(selection, origin_x, origin_z, 3, 2);
    if ((sint32)value < (sint32)minimum)
        minimum = value;
    value = t3_8006deac(selection, origin_x, origin_z, 3, 3);
    if ((sint32)value < (sint32)minimum)
        minimum = value;
    for (row = 0; row < 6; ++row) {
        for (column = 0; column < 6; ++column) {
            if (selection[row * 6 + column]) {
                value = t3_8006deac(selection, origin_x, origin_z, column, row);
                if ((sint32)minimum >= (sint32)value)
                    selection[row * 6 + column] = 0;
            }
        }
    }
    return 0;
}

uint32 t3_bns_00_800b4060(void)
{
    uint32 phase, tile, suppressed, owner, ordering, packet, result, camera;
    uint32 script, actor, slot, index, animation, flags, saved, paused;
    uint32 value, frame, last, level, step;
    sint32 time, ticks, opcode, previous, active, end;
    FUNCTION_MARKER(0x800B4060, "SLUS_004.02-BNS-logical-00");
    phase = r_u32(0x800B4A50);
    if (!phase) {
        t3_80062a10();
        t3_800690a0(0x800A9228, 0x800AAAB4);
        t3_80065ed8(0x800A9228, 0x800AAAB4);
        /* The preceding camera reset returns with its side carrier zero */
        t3_800670fc(2, 0, 0);
        w_u32(0x800B4A80, 0);
        tile = 0x800B4A8C + (r_u32(0x800ADEFC) << 4);
        w_u16(tile + 12, 368);
        suppressed = r_u32(0x800ADCA4);
        w_u16(tile + 14, 480);
        if (!suppressed) {
            owner = r_u32(0x800A8C54);
            ordering = r_u32(owner + 4) + 4092;
            AddPrim(psx_addr(ordering, 4), psx_addr(tile, 4));
        }
        phase = r_u32(0x800B4A50);
        w_u32(0x800B4A54, 0);
        w_u32(0x800B4A50, phase + 1);
        return 0;
    }
    if (phase == 2) {
        ordering = r_u32(0x800A9218);
        packet = r_u32(0x800ADF64);
        result = t3_8004e000(ordering + 28, packet, 512);
        camera = r_u32(0x800B4A80);
        w_u32(0x800ADF64, result);
        if (!camera)
            w_u32(0x800B4A80, t3_800670fc(0, 0, 0));
        else
            t3_800630d8_host(0x800A9228, 0x800AAAB4, 0x8009546C, 1);
        value = r_u32(0x800B4A54) - 1;
        w_u32(0x800B4A54, value);
        return (sint32)value < 0;
    }
    if (phase != 1)
        return 1;
    if (r_s32(0x800B4A54) >= 5) {
        tile = 0x800B4A8C + (r_u32(0x800ADEFC) << 4);
        w_u16(tile + 12, 368);
        w_u16(tile + 14, 100);
    }
    script = r_u32(0x800B4A4C);
    time = r_s16(script);
    ticks = r_s32(0x800B4A54);
    if (ticks >= time) {
        for (;;) {
            script = r_u32(0x800B4A4C);
            opcode = r_s16(script + 2);
            if (opcode == -1) {
                phase = r_u32(0x800B4A50);
                w_u32(0x800B4A54, 2);
                w_u32(0x800B4A50, phase + 1);
                break;
            }
            switch (opcode) {
            case 2:
            case 3:
                slot = opcode == 2 ? 0x800B4A6C : 0x800B4A76;
                actor = opcode == 2 ? 0x800A9228 : 0x800AAAB4;
                previous = r_s16(slot + 4);
                value = (uint32)r_s16(script + 4);
                if (previous != (sint32)value) {
                    animation = t3_8002d178(actor, value);
                    w_u32(actor + 84, animation);
                    w_u32(actor + 76, animation);
                }
                script = r_u32(0x800B4A4C);
                w_u16(slot + 4, r_u16(script + 4));
                w_u16(slot + 8, r_u16(script + 6));
                value = r_u16(script + 8);
                w_u16(slot, 1);
                w_u16(slot + 6, value);
                break;
            case 5:
                value = (uint32)r_s16(script + 4);
                w_u32(0x800B4A60, 0);
                w_u32(0x800B4A5C, 1);
                w_u32(0x800B4A64, value);
                break;
            case 6:
                t3_8006ae14(5, 0);
                break;
            default:
                break;
            }
            script = r_u32(0x800B4A4C);
            ticks = r_s32(0x800B4A54);
            time = r_s16(script + 10);
            w_u32(0x800B4A4C, script + 10);
            if (ticks < time)
                break;
        }
    }
    for (index = 0; index < 2; ++index) {
        slot = 0x800B4A6C + index * 10;
        actor = 0x800A9228 + index * 6284;
        active = r_s16(slot);
        if (active == 1) {
            frame = r_u16(slot + 8);
            animation = r_u32(actor + 84);
            ++frame;
            w_u16(actor + 88, frame);
            w_u16(actor + 80, frame);
            flags = r_u32(animation + 36);
            saved = flags & 0x40000000;
            w_u32(animation + 36, flags & 0xBFFFFFFF);
            /* Both original iterations update the fixed first actor here */
            t3_800458c8(0x800A9228);
            w_u32(0x80096EA8, (uint32)active);
            t3_8003a818(actor);
            animation = r_u32(actor + 84);
            paused = r_u32(0x800B4A58);
            flags = r_u32(animation + 36);
            w_u32(animation + 36, (flags & 0xBFFFFFFF) | saved);
            if (!(paused & 1))
                w_u16(slot + 8, r_u16(slot + 8) + 1);
            frame = (uint32)r_s16(slot + 8);
            end = r_s16(slot + 6);
            last = r_u16(slot + 6);
            if (end < (sint32)frame)
                w_u16(slot + 8, last);
        }
    }
    if (r_u32(0x800B4A80))
        t3_800630d8_host(0x800A9228, 0x800AAAB4, 0x8009546C, 0x800AC340);
    else if (!(r_u32(0x800B4A58) & 1))
        w_u32(0x800B4A80, t3_800670fc(0, 0, 0));
    t3_80048240(0x800A9228, 0x800AAAB4);
    if (!r_u32(0x800B4A88)) {
        t3_8006d7cc();
        if (!r_u32(0x800ADCA4)) {
            index = r_u32(0x800ADEFC);
            owner = r_u32(0x800A8C54);
            ordering = r_u32(owner + 4) + 4092;
            tile = 0x800B4A8C + (index << 4);
            AddPrim(psx_addr(ordering, 4), psx_addr(tile, 4));
        }
    } else {
        t3_80048374(r_u32(0x800ADEFC));
    }
    t3_80039ddc(r_u16(0x800ADC84));
    t3_8003ec64(0x800A9228);
    t3_80039f84();
    t3_80036000();
    t3_800378f8();
    if (r_u32(0x800B4A5C)) {
        level = r_u32(0x800B4A60);
        if ((sint32)level >= 208)
            w_u32(0x800B4A88, 1);
        if ((sint32)level >= 257)
            w_u32(0x800B4A60, 256);
        packet = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        level = r_u32(0x800B4A60);
        result = t3_8004e000(ordering + 8, packet, (sint32)(level + 256));
        paused = r_u32(0x800B4A58);
        w_u32(0x800ADF64, result);
        if (!(paused & 1)) {
            level = r_u32(0x800B4A60);
            step = r_u32(0x800B4A64);
            w_u32(0x800B4A60, level + step);
        }
    }
    w_u32(0x800B4A54, r_u32(0x800B4A54) + 1);
    return 0;
}

uint32 t3_bns_00_800b19b0(uint32 round, uint32 elapsed, uint32 x, uint32 y)
{
    uint32 total_seconds, minutes, seconds, centiseconds, clock_x;
    FUNCTION_MARKER(0x800B19B0, "SLUS_004.02-BNS-logical-00");
    if (elapsed > 359999)
        elapsed = 359999;
    total_seconds = elapsed / 60;
    minutes = total_seconds / 60;
    seconds = total_seconds - minutes * 60;
    centiseconds = ((elapsed - total_seconds * 60) * 100) / 60;
    clock_x = x + 75;
    if ((sint32)round < 10 || (r_u32(0x800AE218) & 0x20))
        t3_8004ce74(0x800B0550, (uint32)7, (uint32)0, x, y, round);
    else
        t3_8004ce74(0x800B0564, (uint32)7, (uint32)0, x, y);
    t3_8004ce74(0x800B0574, (uint32)5, (uint32)0, clock_x + 17, y - 2, clock_x + 42);
    return t3_8004ce74(0x800B0584, (uint32)5, (uint32)0, clock_x, y,
        minutes, clock_x + 24, seconds, clock_x + 50, centiseconds);
}

uint32 t3_bns_00_800b1ca8(uint32 context, uint32 actor_index)
{
    uint32 state, phase;
    FUNCTION_MARKER(0x800B1CA8, "SLUS_004.02-BNS-logical-00");
    state = r_u8(context + 28);
    if (state == 1) {
        if (actor_index == r_u8(context + 30))
            return r_u16(0x800AE1F8 + (actor_index << 1)) ? 3 : 1;
        if (r_u8(context + 18))
            return 2;
        if (r_u16(0x800AE212))
            return 2;
        return r_u8(context + 19) == 0;
    }
    if (state != 2)
        return 0;
    phase = r_u8(context + 33);
    if (!phase || phase == 3)
        return 0;
    if (r_u8(context + 35) != actor_index)
        return 0;
    return (r_u32(0x800AE218) & 0x18) ? 2 : 0;
}

uint32 t3_bns_00_800b1d8c(uint32 context, uint32 actor_index)
{
    uint32 state;
    FUNCTION_MARKER(0x800B1D8C, "SLUS_004.02-BNS-logical-00");
    state = r_u8(context + 28);
    if (state == 1) {
        if (actor_index != r_u8(context + 30))
            return 1;
        return r_u16(0x800AE1F8 + (actor_index << 1)) ? 4 : 1;
    }
    if (state == 2 && !actor_index)
        return 4;
    return 0;
}

uint32 t3_bns_00_800b1e08(uint32 context, uint32 actor_index)
{
    FUNCTION_MARKER(0x800B1E08, "SLUS_004.02-BNS-logical-00");
    if (actor_index != r_u8(context + 30))
        return 5;
    return r_u16(0x800AE1F8 + (actor_index << 1)) ? 3 : 1;
}

uint32 t3_bns_00_800b1e48(uint32 context, uint32 actor_index)
{
    FUNCTION_MARKER(0x800B1E48, "SLUS_004.02-BNS-logical-00");
    if (actor_index != r_u8(context + 30))
        return 6;
    return r_u16(0x800AE1F8 + (actor_index << 1)) ? 8 : 2;
}

uint32 t3_bns_03_800b4d4c(uint32 context, uint32 actor_index)
{
    uint32 state;
    FUNCTION_MARKER(0x800B4D4C, "SLUS_004.02-BNS-logical-03");
    state = r_u8(context + 28);
    if (state == 1) {
        if (actor_index != r_u8(context + 30))
            return 1;
        return r_u16(0x800AE1F8 + (actor_index << 1)) ? 9 : 1;
    }
    if (state == 2 && !actor_index)
        return 9;
    return 0;
}

uint32 t3_bns_00_800b1b3c(uint32 wins, uint32 x, uint32 y)
{
    uint32 variant, tail, text;
    FUNCTION_MARKER(0x800B1B3C, "SLUS_004.02-BNS-logical-00");
    if ((sint32)wins >= 10000)
        wins = 9999;
    if ((sint32)wins < 2)
        variant = 0;
    else if ((sint32)wins < 10)
        variant = 1;
    else if ((sint32)wins < 100)
        variant = 2;
    else if ((sint32)wins < 1000)
        variant = 3;
    else
        variant = 4;
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    text = r_u32(0x800B499C + variant * 4);
    t3_8004cc08(tail, text);
    return t3_8004ce74(0x1F800000, (uint32)1, (uint32)0, x, y, wins);
}

void t3_bns_00_800b1c14(uint32 count, uint32 x, uint32 y)
{
    uint32 tail, text;
    FUNCTION_MARKER(0x800B1C14, "SLUS_004.02-BNS-logical-00");
    if (!count)
        return;
    if ((sint32)count >= 11)
        count = 10;
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    text = r_u32(0x800B49B0 + (count << 2));
    t3_8004cc08(tail, text);
    t3_8004ce74(0x1F800000, (uint32)5, (uint32)0, x, y);
}

uint32 t3_800522f4(uint32 text, uint32 palette, uint32 font, uint32 x, uint32 y, uint32 value)
{
    uint32 index, slot = 0;
    FUNCTION_MARKER(0x800522F4, "SLUS_004.02");
    for (index = 0; index < 64; ++index) {
        uint32 candidate = 0x800B8D60 + index * 40;
        if (!r_u16(candidate)) {
            slot = candidate;
            break;
        }
    }
    if (!slot)
        return 0;
    w_u32(slot + 24, text);
    w_u16(slot + 16, palette);
    w_u16(slot + 18, font);
    w_u16(slot + 4, x);
    w_u16(slot + 8, 0);
    w_u16(slot + 10, 0);
    w_u16(slot + 6, y);
    w_u16(slot, 7);
    w_u32(slot + 28, value);
    return value;
}

uint32 t3_80051cdc(uint32 background, uint32 style)
{
    uint32 index, slot = 0, value;
    FUNCTION_MARKER(0x80051CDC, "SLUS_004.02");
    for (index = 0; index < 64; ++index) {
        uint32 candidate = 0x800B8D60 + index * 40;
        if (!r_u16(candidate)) {
            slot = candidate;
            break;
        }
    }
    if (!slot)
        return 3;
    w_u16(slot, 3);
    value = r_u32(0x80022694 + (style << 2));
    w_u16(slot + 6, 36);
    w_u16(slot + 4, 0);
    w_u32(slot + 20, value);
    value = r_u16(0x800226B0 + (style << 1));
    w_u16(slot + 8, value);
    slot = 0;
    for (index = 0; index < 64; ++index) {
        uint32 candidate = 0x800B8D60 + index * 40;
        if (!r_u16(candidate)) {
            slot = candidate;
            break;
        }
    }
    if (!slot)
        return 1;
    w_u16(slot, 1);
    value = r_u16(0x80022690 + (background << 1));
    w_u16(0x800B8D58, value);
    return value;
}

uint32 t3_bns_00_800b37a8(uint32 *selection)
{
    uint32 index, resource, round, background;
    FUNCTION_MARKER(0x800B37A8, "SLUS_004.02-BNS-logical-00");
    for (index = 0; index < 2; ++index) {
        resource = selection[index];
        t3_80052114(index, resource, 0x800B4A08 + index * 8);
        resource = selection[index + 2];
        t3_80052368((sint32)resource, 0x800B4A18 + index * 8);
    }
    t3_800522f4(0x800B06E4, 0, 2, 162, 250, 0);
    round = r_u32(0x800AFAAC);
    t3_800522f4(0x800B06F0, 6, 1, 28, 42, round + 1);
    background = selection[4];
    return t3_80051cdc(background, background);
}

uint32 t3_8005197c(void)
{
    uint32 index;
    FUNCTION_MARKER(0x8005197C, "SLUS_004.02");
    for (index = 0; index < 64; ++index) {
        uint32 slot = 0x800B8D60 + index * 40;
        if (!r_u16(slot))
            return slot;
    }
    return 0;
}

uint32 t3_bns_00_800b38a8(uint32 *selection)
{
    uint32 index, resource, round;
    FUNCTION_MARKER(0x800B38A8, "SLUS_004.02-BNS-logical-00");
    for (index = 0; index < 2; ++index) {
        resource = selection[index];
        t3_80052114(index, resource, 0x800B4A08 + index * 8);
        resource = selection[index + 2];
        t3_80052368((sint32)resource, 0x800B4A18 + index * 8);
    }
    t3_800522f4(0x800B06E4, 0, 2, 162, 250, 0);
    round = r_u32(0x800AFAB0);
    t3_800522f4(0x800B0704, 6, 1, 28, 42, round + 1);
    return t3_80051cdc(1, 1);
}

uint32 t3_bns_00_800b399c(uint32 *selection)
{
    uint32 index, resource, round, slot, background;
    FUNCTION_MARKER(0x800B399C, "SLUS_004.02-BNS-logical-00");
    for (index = 0; index < 2; ++index) {
        t3_bns_00_800b3460(index, 0x800AFAE1 + index * 13);
        resource = selection[index];
        t3_80052114(index, resource, 0x800B4A28 + index * 8);
        resource = selection[index + 2];
        t3_80052368((sint32)resource, 0x800B4A38 + index * 8);
    }
    t3_800522f4(0x800B06E4, 0, 2, 162, 250, 0);
    round = r_u8(0x800AFAE0);
    t3_800522f4(0x800B0704, 6, 1, 28, 42, round + 1);
    slot = t3_8005197c();
    if (slot)
        w_u16(slot, 2);
    background = selection[4];
    return t3_80051cdc(background, 2);
}

uint32 t3_bns_00_800b3ad0(uint32 *selection)
{
    uint32 index, resource, round;
    FUNCTION_MARKER(0x800B3AD0, "SLUS_004.02-BNS-logical-00");
    for (index = 0; index < 2; ++index) {
        resource = selection[index];
        t3_80052114(index, resource, 0x800B4A08 + index * 8);
        resource = selection[index + 2];
        t3_80052368((sint32)resource, 0x800B4A18 + index * 8);
    }
    t3_800522f4(0x800B06E4, 0, 2, 162, 250, 0);
    round = r_u32(0x800AFACC);
    t3_800522f4(0x800B0704, 6, 1, 28, 42, round + 1);
    return t3_80051cdc(0, 3);
}

uint32 t3_bns_00_800b3bc4(uint32 *selection)
{
    uint32 index, resource;
    FUNCTION_MARKER(0x800B3BC4, "SLUS_004.02-BNS-logical-00");
    for (index = 0; index < 2; ++index) {
        resource = selection[index];
        t3_80052114(index, resource, 0x800B4A08 + index * 8);
        resource = selection[index + 2];
        t3_80052368((sint32)resource, 0x800B4A18 + index * 8);
    }
    t3_800522f4(0x800B06E4, 0, 2, 162, 250, 0);
    t3_800522f4(0x800B0718, 6, 1, 28, 42, 0);
    return t3_80051cdc(1, 1);
}

uint32 t3_bns_00_800b3460(uint32 side, uint32 team)
{
    uint32 count, table, x, y, step, width, frame_x, frame_y, frame_width;
    uint32 visible, selected, index, character, palette, current, slot;
    FUNCTION_MARKER(0x800B3460, "SLUS_004.02-BNS-logical-00");
    count = r_u8(team + 12);
    if (count >= 8)
        count = 8;
    table = 0x800B06D4 + (side << 3);
    x = r_u16(table);
    y = r_u16(table + 2);
    step = r_u16(table + 4);
    if (count == 8)
        x = r_u16(table + 6);
    width = (step << 3) & 0xFFFF;
    frame_width = ((count - 1) * 33 + 1) & 0xFFFF;
    frame_x = x - 1;
    frame_y = (y - 2) & 0xFFFF;
    if (side)
        frame_x += (count - 2) * step;
    frame_x &= 0xFFFF;
    visible = r_u8(team + 10);
    selected = r_u8(team + 9);
    if (selected >= visible)
        visible = selected + 1;
    for (index = selected; index < count; ++index) {
        character = r_u8(team + index);
        if (character >= 88)
            character = 88;
        palette = 8;
        if (index >= visible) {
            character = 88;
        } else {
            current = r_u8(team + 9);
            if (index < current)
                palette = 104;
            else if (index == current)
                continue;
        }
        slot = t3_8005197c();
        if (slot) {
            w_u16(slot, 5);
            w_u32(slot + 28, character);
            w_u32(slot + 32, palette);
            w_u16(slot + 4, x);
            w_u16(slot + 6, y);
            w_u16(slot + 8, width);
            w_u16(slot + 10, 0);
        }
        x = (x + step) & 0xFFFF;
    }
    selected = r_u8(team + 9);
    for (index = 0; index < selected; ++index) {
        character = r_u8(team + index);
        if (character >= 88)
            character = 88;
        palette = 8;
        if (index >= visible) {
            character = 88;
        } else {
            current = r_u8(team + 9);
            if (index < current)
                palette = 104;
            else if (index == current)
                continue;
        }
        slot = t3_8005197c();
        if (slot) {
            w_u16(slot, 5);
            w_u32(slot + 28, character);
            w_u32(slot + 32, palette);
            w_u16(slot + 4, x);
            w_u16(slot + 6, y);
            w_u16(slot + 8, width);
            w_u16(slot + 10, 0);
        }
        x = (x + step) & 0xFFFF;
    }
    if (count < 2)
        return 1;
    slot = t3_8005197c();
    if (!slot)
        return 6;
    w_u16(slot, 6);
    w_u16(slot + 4, frame_x);
    w_u16(slot + 6, frame_y);
    w_u16(slot + 8, frame_width);
    w_u16(slot + 10, 62);
    return 62;
}

uint32 t3_800524ac(uint32 side)
{
    FUNCTION_MARKER(0x800524AC, "SLUS_004.02");
    w_u8(0x80098106 + side, 38);
    w_u8(0x800B8D5A + side, 38);
    return 0x800B8D58;
}

uint32 t3_bns_01_800b6b00(uint32 *selection)
{
    uint32 index, resource;
    FUNCTION_MARKER(0x800B6B00, "SLUS_004.02-BNS-logical-01");
    selection[5] = 0;
    for (index = 0; index < 2; ++index) {
        resource = selection[index];
        t3_80052114(index, resource, 0x800B8B14 + index * 8);
        resource = selection[index + 2];
        t3_80052368((sint32)resource, 0x800B8B24 + index * 8);
    }
    t3_800522f4(0x800B09D0, 0, 2, 162, 250, 0);
    t3_800522f4(0x800B09DC, 6, 1, 28, 42, 0);
    return t3_80051cdc(0, 4);
}

uint32 t3_bns_03_800b50f0(uint32 *selection)
{
    uint32 index, resource, background;
    FUNCTION_MARKER(0x800B50F0, "SLUS_004.02-BNS-logical-03");
    for (index = 0; index < 2; ++index) {
        resource = selection[index];
        t3_80052114(index, resource, 0x800B6444 + index * 8);
        resource = selection[index + 2];
        t3_80052368((sint32)resource, 0x800B6454 + index * 8);
    }
    t3_800522f4(0x800B05DC, 0, 2, 162, 250, 0);
    t3_800522f4(0x800B05E8, 6, 1, 28, 42, 0);
    background = selection[4];
    return t3_80051cdc(background, 5);
}

uint32 t3_bns_02_800b6164(uint32 *selection)
{
    uint32 round, side, resource, packed_xy, packed_size, x, y, width, height;
    uint32 index, slot, palette, name, length;
    FUNCTION_MARKER(0x800B6164, "SLUS_004.02-BNS-logical-02");
    round = r_u32(0x800AFAAC);
    selection[5] = round + 1;
    side = r_u8(0x800AFAC6) != 0;
    resource = selection[side];
    t3_80052114(side, resource, 0x800B6540);
    t3_800524ac(side ^ 1);
    resource = selection[side + 2];
    t3_80052368((sint32)resource, 0x800B6548);
    t3_800522f4(0x800B0ABC, 6, 1, 28, 42, 0);
    round = r_u32(0x800AFAAC);
    if ((sint32)round < 4)
        t3_800522f4(0x800B0AD4, 6, 1, 32, 84, round + 1);
    else
        t3_800522f4(0x800B0AE8, 6, 1, 32, 84, 0);
    /* Each original aligned ULW pair reads its source word twice */
    (void)r_u32(0x800B6550);
    packed_xy = r_u32(0x800B6550);
    (void)r_u32(0x800B6554);
    packed_size = r_u32(0x800B6554);
    x = packed_xy & 0xFFFF;
    y = packed_xy >> 16;
    width = packed_size & 0xFFFF;
    height = packed_size >> 16;
    for (index = 0; index < 2; ++index) {
        slot = t3_8005197c();
        if (!slot)
            break;
        w_u16(slot, 8);
        w_u32(slot + 24, 0x800B0AB0);
        palette = r_u8(0x800B6580 + round);
        w_u16(slot + 18, 3);
        w_u16(slot + 16, palette);
        w_u16(slot + 4, x);
        w_u16(slot + 6, y);
        w_u16(slot + 8, width);
        w_u16(slot + 10, height);
        name = r_u32(0x800B6558 + (round << 3) + index * 4);
        w_u32(slot + 28, name);
        length = t3_8007a418(name);
        w_u16(slot + 4, r_u16(slot + 4) - length * 21);
        y = (y + 46) & 0xFFFF;
    }
    return t3_80051cdc(0, 6);
}

uint32 t3_bns_01_800b8730(uint32 actor, uint32 opponent, uint32 control)
{
    uint32 row;
    uint32 previous;
    uint32 value;
    uint32 elapsed;
    uint32 length;
    uint32 flags;
    uint32 animation;
    sint32 ready = 0;

    FUNCTION_MARKER(0x800B8730, "SLUS_004.02-BNS-logical-01");
    row = control + (uint32)r_s16(actor + 0x12) * 32 + 20;
    if (!r_u8(row + 20)) {
        previous = r_u8(row + 24);
        w_u8(row + 25, previous);
    } else {
        w_u8(row + 25, 0);
    }
    w_u8(row + 24, r_u8(row + 20));
    if (t3_800325bc()) {
        t3_80031d0c(2);
        t3_8006e614();
        t3_8004a40c();
        t3_8007666c();
        t3_8004a83c();
        t3_80041998(actor);
        t3_80041998(opponent);
        w_u16(control + 16, 0);
        t3_8006307c(actor, opponent);
        w_u16(control + 16, 0);
    }
    value = r_u32(0x800954A4);
    if (value)
        return value;
    if (r_u8(control + 138)) {
        w_u16(control + 18, 0);
    } else {
        if (r_s8(control + 129) == 4) {
            w_u16(control + 18, 0);
        } else if (r_u8(row + 25) || r_u8(row + 20) ||
                   !r_u8(control + (uint32)r_s16(opponent + 0x12) + 170)) {
            w_u16(control + 18, r_u16(control + 18) + 1);
        } else {
            animation = r_u32(opponent + 0x54);
            w_u16(control + 18, r_u8(animation + 0x18));
        }
        elapsed = (r_u16(control + 16) + 1) & 0xFFFF;
        w_u16(control + 16, elapsed);
        if (elapsed >= 301)
            w_u16(control + 16, 300);
    }
    if (r_u16(control + 16) < 90)
        return 1;
    switch (r_s8(control + 129)) {
    case 1:
        ready = r_s8(row + 22) >= 4;
        break;
    case 2:
        ready = r_s8(row + 22) >= 5;
        break;
    case 3:
        ready = r_s8(row + 22) >= 6;
        break;
    case 4:
        if (r_u16(control + 2) & 0x100) {
            if (r_u16(control + 16) >= 281)
                w_u16(control + 16, 280);
            ready = r_u16(control + 16) >= 90;
        }
        break;
    default:
        break;
    }
    if (!ready)
        return 4;
    length = r_u16(control + 16);
    if (r_s8(control + 129) != 4) {
        value = r_u16(control + 18);
        if (value)
            length = value + 30;
    }
    elapsed = r_u16(control + 16);
    if (elapsed < length)
        length = elapsed;
    if (length >= 300)
        length = 300;
    else if (length < 90)
        length = 90;
    flags = r_u16(control + 2);
    w_u16(control + 16, 0);
    w_u16(control + 18, 0);
    w_u16(control + 2, flags & 0xFEFF);
    t3_80031e48(length - 2);
    t3_8004b770();
    t3_8007666c();
    t3_8004a83c();
    t3_80041998(actor);
    return t3_80041998(opponent);
}

uint32 t3_bns_01_800b86b8(uint32 actor, uint32 opponent, uint32 control)
{
    uint32 result;

    FUNCTION_MARKER(0x800B86B8, "SLUS_004.02-BNS-logical-01");
    t3_80031d0c(2);
    t3_8006e614();
    t3_8004a40c();
    t3_8007666c();
    t3_8004a83c();
    t3_80041998(actor);
    result = t3_80041998(opponent);
    w_u16(control + 16, 0);
    return result;
}

uint32 t3_bns_01_800b2ba4(void)
{
    uint32 first_index, second_index, actor, opponent, control;
    uint32 index, value, frozen, paused, previous_pause, other_control;
    uint32 cursor, row;

    FUNCTION_MARKER(0x800B2BA4, "SLUS_004.02-BNS-logical-01");
    first_index = r_u8(0x800ADF3E);
    second_index = r_u8(0x800B053E);
    control = r_u32(0x800B8A2C);
    w_u16(0x800AE200, 0);
    w_u8(control + 0x84, first_index);
    control = r_u32(0x800B8A2C);
    value = r_u8(0x800B053E);
    w_u8(control + 0x85, value);
    actor = 0x800A9228 + first_index * 6284;
    opponent = 0x800A9228 + second_index * 6284;
    if (!r_u32(0x800ADCA4)) {
        index = r_u8(0x800ADF3E) * 2;
        control = r_u32(0x800B8A2C);
        w_u16(control, r_u16(0x800ADD68 + index));
        w_u16(control + 2, r_u16(0x800ADF08 + index));
        w_u16(control + 4, r_u16(0x800ADF00 + index));
    } else {
        control = r_u32(0x800B8A2C);
        w_u16(control, 0);
        w_u16(control + 2, 0);
        w_u16(control + 4, 0);
    }
    index = r_u8(0x800ADF3E) * 2;
    control = r_u32(0x800B8A2C);
    w_u16(control + 6, r_u16(0x800A8C48 + index));
    value = r_u16(0x800ADC78 + index);
    frozen = r_u32(0x800954A4);
    w_u16(control + 8, value);
    if (!frozen) {
        w_u8(control + 0x8A, r_u8(0x8009548C));
        if (r_u32(0x800954BC)) {
            control = r_u32(0x800B8A2C);
            value = r_u32(0x8009548C);
            w_u16(control, 0);
            w_u16(control + 2, 0);
            w_u16(control + 4, 0);
            if (value) {
                w_u8(control + 0x76, r_u8(control + 0x76) - 1);
                control = r_u32(0x800B8A2C);
                if (r_s8(control + 0x76) < 0) {
                    w_u8(control + 0x76, 0);
                    w_u32(0x800954BC, 0);
                    t3_8002bc00(1);
                    t3_bns_01_800b744c(0);
                    t3_bns_01_800b744c(1);
                }
            }
        }
        control = r_u32(0x800B8A2C);
        paused = r_u8(control + 0x8A);
        previous_pause = r_u8(control + 0x8B);
        if (paused != previous_pause) {
            if (paused) {
                value = r_u16(control + 2);
                w_u8(control + 0x9F, 0);
                other_control = r_u32(0x800B8A2C);
                w_u16(control + 2, value & 0xF7FF);
                w_u8(other_control + 0x82, 0);
                t3_bns_01_800b8144(r_u32(0x800B8A2C));
                t3_8002bc00(1);
                for (row = 0; row < 2; ++row) {
                    cursor = 0x800ADF68 + row * 4;
                    w_u8(cursor, 0);
                    w_u8(cursor + 3, 0);
                    w_u8(cursor + 2, 0);
                    w_u8(cursor + 1, 0);
                }
                cursor = r_u32(0x800B8A2C) + 0x54;
                for (row = 0; row < 4; ++row) {
                    w_u16(cursor + 2, 0);
                    cursor += 8;
                }
                w_u8(r_u32(0x800B8A2C) + 0x74, 0);
                w_u8(r_u32(0x800B8A2C) + 0xA4, 0);
                w_u8(r_u32(0x800B8A2C) + 0xA3, 0);
                w_u8(r_u32(0x800B8A2C) + 0xA7, 1);
                w_u8(r_u32(0x800B8A2C) + 0x86, 0);
                w_u8(r_u32(0x800B8A2C) + 0xA9, 3);
                if (r_u8(r_u32(0x800B8A2C) + 0xAC))
                    t3_8004b688(0);
            } else {
                t3_bns_01_800b26e4(actor, opponent);
            }
        }
        control = r_u32(0x800B8A2C);
        w_u8(control + 0x8B, r_u8(control + 0x8A));
        control = r_u32(0x800B8A2C);
        value = r_u8(control + 0xA9);
        if (value) {
            w_u8(control + 0xA9, value - 1);
            control = r_u32(0x800B8A2C);
        }
        if (!r_u8(control + 0x8A)) {
            t3_bns_01_800b3328(actor, opponent);
        } else if (!r_u8(control + 0xA9) && !r_u32(0x800ADCA4)) {
            if (!r_u8(control + 0x9F))
                t3_bns_01_800b6764();
            if (!r_u32(0x800ADCA4)) {
                if (!r_u8(r_u32(0x800B8A2C) + 0x9F))
                    t3_bns_01_800b5290();
                if (!r_u32(0x800ADCA4)) {
                    if (!r_u8(r_u32(0x800B8A2C) + 0x9F))
                        t3_bns_01_800b44a8();
                    if (!r_u32(0x800ADCA4))
                        t3_bns_01_800b4728();
                }
            }
        }
        if (!r_u32(0x800954A4))
            t3_bns_01_800b6248();
    }
    t3_bns_01_800b40e4(actor, opponent);
    t3_bns_01_800b66b8();
    t3_bns_01_800b8730(opponent, actor, r_u32(0x800B8A2C));
    return r_u8(r_u32(0x800B8A2C) + 0x8A);
}

uint32 t3_bns_01_800b744c(uint32 index)
{
    uint32 row, cursor;

    FUNCTION_MARKER(0x800B744C, "SLUS_004.02-BNS-logical-01");
    cursor = 0x800B8C10;
    for (row = 0; row < 50; ++row) {
        w_u8(cursor, 0);
        w_u8(cursor + 1, 0);
        w_u32(cursor, r_u32(cursor) & 0x8000FFFF);
        cursor += 4;
    }
    w_u32(0x800B8D48 + index * 4, 0);
    w_u32(0x800B8B4C + index * 4, 30);
    return 30;
}

uint32 t3_bns_01_800b8144(uint32 control)
{
    uint32 first, second, destination;

    FUNCTION_MARKER(0x800B8144, "SLUS_004.02-BNS-logical-01");
    first = (uint32)r_s8(control + 0x7B);
    second = (uint32)r_s8(control + 0x7C);
    w_u8(control + 0xA6, 1);
    w_u8(control + 0xA5, 0);
    t3_bns_01_800b7f80(first, second);
    destination = 0x800B8D50 + (uint32)r_s8(control + 0x7B) * 4;
    w_u32(destination, 0);
    return destination;
}

uint32 t3_bns_01_800b66b8(void)
{
    uint32 result, control, cursor, row;

    FUNCTION_MARKER(0x800B66B8, "SLUS_004.02-BNS-logical-01");
    result = r_u32(0x800954BC);
    if (result)
        return result;
    control = r_u32(0x800B8A2C);
    if (r_u8(control + 0x8A))
        return 0x800B0000;
    cursor = 0x800ADF68;
    for (row = 0; row < 2; ++row) {
        control = r_u32(0x800B8A2C);
        if (!r_s8(control + 0x83)) {
            w_u8(cursor, 0);
            w_u8(cursor + 1, 0);
            w_u8(cursor + 2, 0);
        } else if (!r_u8(control + row + 0x88)) {
            w_u8(cursor, 1);
            w_u8(cursor + 1, 250);
            w_u8(cursor + 2, 0);
        } else {
            w_u8(cursor, 1);
            w_u8(cursor + 1, 0);
            w_u8(cursor + 2, 250);
        }
        w_u8(cursor + 3, 0);
        cursor += 4;
    }
    return 0;
}

uint32 t3_bns_01_800b2894(void)
{
    uint32 control, first, second, cursor, row;

    FUNCTION_MARKER(0x800B2894, "SLUS_004.02-BNS-logical-01");
    w_u16(r_u32(0x800B8A2C) + 0x10, 0);
    w_u8(r_u32(0x800B8A2C) + 0x8A, 0);
    w_u8(r_u32(0x800B8A2C) + 0x8B, 0);
    w_u8(r_u32(0x800B8A2C) + 0x75, 1);
    w_u8(r_u32(0x800B8A2C) + 0x77, 0);
    control = r_u32(0x800B8A2C);
    w_u32(0x800954BC, 1);
    w_u8(control + 0x76, 30);
    w_u8(r_u32(0x800B8A2C) + 0x78, 0);
    w_u8(r_u32(0x800B8A2C) + 0x79, 0);
    w_u8(r_u32(0x800B8A2C) + 0x7A, 0);
    w_u8(r_u32(0x800B8A2C) + 0x83, 0);
    w_u8(r_u32(0x800B8A2C) + 0x9D, 1);
    w_u8(r_u32(0x800B8A2C) + 0x9E, 0);
    w_u8(r_u32(0x800B8A2C) + 0x81, 0);
    w_u8(r_u32(0x800B8A2C) + 0x86, 0);
    w_u8(r_u32(0x800B8A2C) + 0x80, 1);
    w_u8(r_u32(0x800B8A2C) + 0x7E, 1);
    w_u8(r_u32(0x800B8A2C) + 0x7F, 2);
    w_u8(r_u32(0x800B8A2C) + 0xA0, 1);
    w_u8(r_u32(0x800B8A2C) + 0xA1, 15);
    w_u8(r_u32(0x800B8A2C) + 0x87, 0);
    w_u8(r_u32(0x800B8A2C) + 0xA2, 0);
    w_u8(r_u32(0x800B8A2C) + 0x7D, 0);
    control = r_u32(0x800B8A2C);
    w_u8(control + 0x7B, r_u8(0x800ADF3E));
    w_u8(r_u32(0x800B8A2C) + 0x7C, 0);
    w_u8(r_u32(0x800B8A2C) + 0xAC, 0);
    for (row = 0; row < 2; ++row) {
        w_u8(r_u32(0x800B8A2C) + row + 0xAA, 0);
        w_u8(r_u32(0x800B8A2C) + row + 0x88, 0);
    }
    t3_bns_01_800b744c(0);
    t3_bns_01_800b744c(1);
    w_u8(r_u32(0x800B8A2C) + 0xA9, 0);
    w_u8(r_u32(0x800B8A2C) + 0x9F, 0);
    w_u8(r_u32(0x800B8A2C) + 0x82, 0);
    t3_bns_01_800b8144(r_u32(0x800B8A2C));
    t3_8002bc00(1);
    for (row = 0; row < 2; ++row) {
        cursor = 0x800ADF68 + row * 4;
        w_u8(cursor, 0);
        w_u8(cursor + 3, 0);
        w_u8(cursor + 2, 0);
        w_u8(cursor + 1, 0);
    }
    cursor = r_u32(0x800B8A2C) + 0x54;
    for (row = 0; row < 4; ++row) {
        w_u16(cursor + 2, 0);
        cursor += 8;
    }
    w_u8(r_u32(0x800B8A2C) + 0x74, 0);
    w_u8(r_u32(0x800B8A2C) + 0xA4, 0);
    w_u8(r_u32(0x800B8A2C) + 0xA3, 0);
    w_u8(r_u32(0x800B8A2C) + 0xA7, 1);
    w_u8(r_u32(0x800B8A2C) + 0x86, 0);
    control = r_u32(0x800B8A2C);
    first = (uint32)r_s8(control + 0x7E);
    second = (uint32)r_s8(control + 0x7F);
    t3_800593c8(0x800AAAB4, first, second, 0xFFFFFFFF);
    t3_8002bc00(0);
    control = r_u32(0x800B8A2C);
    w_u32(0x800954C0, 0);
    w_u32(0x800954C4, 0);
    w_u32(0x800954C8, 0);
    cursor = control + 0x14;
    for (row = 0; row < 2; ++row) {
        w_u32(cursor, 0);
        w_u32(cursor + 4, 0);
        w_u32(cursor + 8, 0);
        w_u16(cursor + 0xC, 0);
        w_u16(cursor + 0xE, 0);
        w_u8(cursor + 0x14, 0);
        w_u8(cursor + 0x15, 0);
        w_u8(cursor + 0x16, 0);
        w_u8(cursor + 0x17, 0);
        w_u8(cursor + 0x18, 0);
        w_u8(cursor + 0x19, 0);
        w_u16(cursor + 0x12, 0);
        w_u8(cursor + 0x1A, 0);
        w_u8(cursor + 0x1B, 0);
        w_u8(cursor + 0x1C, 0);
        w_u8(cursor + 0x1D, 0);
        w_u16(cursor + 0x10, 0);
        w_u8(cursor + 0x1E, 0);
        w_u8(cursor + 0x1F, 0);
        cursor += 32;
    }
    return 0;
}

uint32 t3_bns_01_800b26e4(uint32 actor, uint32 opponent)
{
    uint32 control, first, second;
    sint32 mode;

    FUNCTION_MARKER(0x800B26E4, "SLUS_004.02-BNS-logical-01");
    t3_bns_01_800b4168(actor, opponent);
    control = r_u32(0x800B8A2C);
    if (r_s8(control + 0x80) == 2) {
        first = (uint32)r_s8(control + 0x7E);
        second = (uint32)r_s8(control + 0x7F);
        t3_800593c8(opponent, first, second, 0xFFFFFFFF);
    }
    w_u8(r_u32(0x800B8A2C) + 0xA8, 0);
    control = r_u32(0x800B8A2C);
    mode = r_s8(control + 0x77);
    switch (mode) {
    case 0:
        if ((r_u16(control) & 0x2F) == 0x2F && r_s8(control + 0x81) != 4)
            w_u8(control + 0x87, 1);
        else
            w_u8(r_u32(0x800B8A2C) + 0x87, 0);
        t3_bns_01_800b744c(r_u8(r_u32(0x800B8A2C) + 0x84));
        t3_bns_01_800b7c24(r_u8(r_u32(0x800B8A2C) + 0x84));
        break;
    case 1:
        w_u8(control + 0x9E, 0);
        w_u8(r_u32(0x800B8A2C) + 0x87, 0);
        t3_bns_01_800b744c(r_u8(r_u32(0x800B8A2C) + 0x84));
        break;
    case 2:
        w_u8(control + 0x9E, 0);
        w_u8(r_u32(0x800B8A2C) + 0x87, 1);
        w_u8(r_u32(0x800B8A2C) + 0xA0, 1);
        t3_bns_01_800b744c((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
        t3_bns_01_800b7c24((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B));
        t3_bns_01_800b7994((uint32)r_s8(r_u32(0x800B8A2C) + 0x7B), 0);
        break;
    default:
        break;
    }
    return t3_800484c8();
}

void t3_bns_01_800b4168(uint32 actor, uint32 opponent)
{
    uint32 side, control;
    sint32 mode, behavior;

    FUNCTION_MARKER(0x800B4168, "SLUS_004.02-BNS-logical-01");
    w_u8(actor + 0xCA, 1);
    side = r_u8(actor + 0x1E);
    w_u8(actor + 0xC5, 0);
    control = r_u32(0x800B8A2C);
    w_u32(0x80095470 + side * 4, 1);
    mode = r_s8(control + 0x77);
    if (mode == 1) {
        w_u8(control + 0x80, 2);
    } else if (mode == 0) {
        w_u8(control + 0x80, r_s8(control + 0x7D) == 8 ? 0 : 1);
    } else if (mode == 2) {
        w_u8(control + 0x80, 1);
        w_u8(r_u32(0x800B8A2C) + 0x7D, 0);
    }
    control = r_u32(0x800B8A2C);
    mode = r_s8(control + 0x80);
    switch (mode) {
    case 0:
        side = r_u8(opponent + 0x1E);
        w_u8(opponent + 0xCA, 1);
        w_u8(opponent + 0xC5, 0);
        w_u32(0x80095470 + side * 4, 1);
        break;
    case 1:
        side = r_u8(opponent + 0x1E);
        w_u8(opponent + 0xCA, 0);
        w_u8(opponent + 0xC5, 0);
        w_u32(0x80095470 + side * 4, 2);
        control = r_u32(0x800B8A2C);
        behavior = r_s8(control + 0x7D);
        t3_bns_01_800b81a8(opponent, actor, (uint32)behavior);
        break;
    case 2:
        side = r_u8(opponent + 0x1E);
        w_u8(opponent + 0xCA, 0);
        w_u8(opponent + 0xC5, 1);
        w_u32(0x80095470 + side * 4, 1);
        break;
    default:
        break;
    }
}

uint32 t3_bns_01_800b42e4(void)
{
    FUNCTION_MARKER(0x800B42E4, "SLUS_004.02-BNS-logical-01");
    return r_u8(r_u32(0x800B8A2C) + 0x86);
}

uint32 t3_bns_01_800b74b0(uint32 actor, uint32 held)
{
    uint32 destination;

    FUNCTION_MARKER(0x800B74B0, "SLUS_004.02-BNS-logical-01");
    destination = 0x800B8B54 + (uint32)r_s16(actor + 0x12) * 2;
    w_u16(destination, held);
    return destination;
}

uint32 t3_bns_01_800b81a8(uint32 actor, uint32 opponent, uint32 behavior)
{
    uint32 active, result;

    FUNCTION_MARKER(0x800B81A8, "SLUS_004.02-BNS-logical-01");
    if (r_s16(actor + 0x16) == 18)
        return 18;
    if (behavior == 6 || behavior == 7) {
        active = r_u8(actor + 0x8A);
        w_u8(actor + 0xCA, 0);
        if (active) {
            if (behavior == 6)
                return t3_bns_01_800b848c(actor);
            return t3_bns_01_800b84ac(actor);
        }
        return t3_bns_01_800b82e4(actor);
    }
    result = t3_bns_01_800b82e4(actor);
    if (!result)
        return behavior < 8;
    if (behavior >= 8) {
        w_u8(actor + 0xCA, 0);
        return 0x800B0000;
    }
    switch (behavior) {
    case 1:
        result = t3_bns_01_800b8358(actor);
        break;
    case 2:
        result = t3_bns_01_800b8628(actor, opponent);
        break;
    case 3:
        result = t3_bns_01_800b866c(actor, opponent);
        break;
    case 4:
        result = t3_bns_01_800b85cc(actor, opponent);
        break;
    case 5:
        w_u8(actor + 0xCA, 1);
        return 1;
    default:
        result = 0x800B82C4;
        break;
    }
    w_u8(actor + 0xCA, 0);
    return result;
}

uint32 t3_bns_01_800b82e4(uint32 actor)
{
    uint32 resource, animation, flags;

    FUNCTION_MARKER(0x800B82E4, "SLUS_004.02-BNS-logical-01");
    resource = r_u32(actor + 0x1A4);
    if (!resource) {
        animation = r_u32(actor + 0x54);
        resource = t3_8002d178(actor, (uint32)r_s16(animation + 0x10));
    }
    flags = r_u16(resource + 4);
    w_u16(actor + 0x408, flags & 0x204 ? 0x1000 : 0);
    return r_u16(actor + 0x408) == 0;
}

uint32 t3_bns_01_800b8358(uint32 actor)
{
    uint32 animation, frame;

    FUNCTION_MARKER(0x800B8358, "SLUS_004.02-BNS-logical-01");
    animation = r_u32(actor + 0x54);
    w_u16(actor + 0x408, 0x4000);
    frame = (uint32)r_s16(actor + 0xA0);
    return frame == r_u16(animation + 0x10);
}

uint32 t3_bns_01_800b8628(uint32 actor, uint32 opponent)
{
    uint32 animation;
    sint32 frame;

    FUNCTION_MARKER(0x800B8628, "SLUS_004.02-BNS-logical-01");
    if (!(r_u16(opponent + 0x64) & 0x10))
        return 0;
    animation = r_u32(opponent + 0x54);
    frame = r_s16(opponent + 0x58);
    if ((sint32)r_u8(animation + 0x2E) < frame)
        return 0;
    w_u16(actor + 0x408, 0x8000);
    return 1;
}

uint32 t3_bns_01_800b866c(uint32 actor, uint32 opponent)
{
    uint32 animation;
    sint32 frame;

    FUNCTION_MARKER(0x800B866C, "SLUS_004.02-BNS-logical-01");
    w_u16(actor + 0x408, 0x4000);
    if (!(r_u16(opponent + 0x64) & 8))
        return 0;
    animation = r_u32(opponent + 0x54);
    frame = r_s16(opponent + 0x58);
    if ((sint32)r_u8(animation + 0x2E) < frame)
        return 0;
    w_u16(actor + 0x408, 0xC000);
    return 1;
}
