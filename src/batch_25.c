#include "game_functions.h"

uint32 t3_8002c278(uint32 actor)
{
    uint32 mode, controller, opponent_index, mapping;
    uint32 held = 0;
    uint32 pressed = 0;
    uint32 input, edges, held_add, pressed_add;
    uint32 counter, timer, direction;
    uint16 ai_held, ai_pressed;

    FUNCTION_MARKER(0x8002C278, "SLUS_004.02");
    if (!r_u8(actor + 0xC5)) {
        mode = r_u32(0x800AFA88);
        controller = r_u8(actor + 0x1E);
        if (mode == 8) {
            opponent_index = r_u8(actor + 0x1F);
            controller = r_u32(0x800B6BF4);
        } else {
            opponent_index = ((uint32)r_s16(actor + 0x12) + 1) & 1;
        }
        t3_80046234(actor, 0x800A9228 + opponent_index * 6284);
        held = r_u16(0x800A8C48 + controller * 2);
        mapping = r_u8(actor + 0xC6);
        pressed = r_u16(0x800ADC78 + controller * 2);
        if (r_u32(0x800AFA88) == 5 && t3_bns_01_800b42e4()) {
            controller = (controller + 1) & 1;
            held = r_u16(0x800A8C48 + controller * 2);
            pressed = r_u16(0x800ADC78 + controller * 2);
        }
        held_add = 0;
        pressed_add = 0;
        counter = 0x8009567C + controller * 8;
        edges = r_u16(0x800ADC78 + controller * 2);
        input = r_u16(0x800A8C48 + controller * 2);
        if (edges & 1) {
            pressed_add = 0x1000;
            w_u32(counter, 0);
            w_u32(counter + 4, 35);
        } else if (edges & 4) {
            pressed_add = 0x4000;
            w_u32(counter, 1);
            w_u32(counter + 4, 35);
        }
        timer = r_u32(counter + 4);
        if ((sint32)timer > 0) {
            --timer;
            w_u32(counter + 4, timer);
            if ((sint32)timer >= 34)
                held_add = r_u32(counter) ? 0x4000 : 0x1000;
            direction = r_u32(counter);
            if (((!direction && (input & 1)) || (direction == 1 && (input & 4)))
                && !(held & 0xF0)) {
                held &= 0xFFFF0FFF;
                pressed &= 0xFFFF0FFF;
            }
        }
        held |= held_add;
        pressed |= pressed_add;
    } else {
        mapping = 0;
        if (r_u32(actor + 0x3F4)) {
            t3_80059890(actor, &ai_held, &ai_pressed);
            // The AI writes only the low half of each original word
            held = (held & 0xFFFF0000) | ai_held;
            pressed = (pressed & 0xFFFF0000) | ai_pressed;
        }
    }
    if (r_u32(0x800AFA88) == 5)
        t3_bns_01_800b74b0(actor, held);
    return t3_8002c580(actor, mapping, held, pressed);
}

uint32 t3_8003c270(uint32 actor)
{
    uint32 duration = 0, value, mode, animation, frame, data, index, bone;
    uint32 previous, current, limit, heading;
    uint16 left, right;
    uint32 target_written, first_written, second_written;
    MATRIX target[18] = {0};
    MATRIX first[18] = {0};
    MATRIX second[18] = {0};

    FUNCTION_MARKER(0x8003C270, "SLUS_004.02");
    if (r_u32(0x80095468) || r_u32(0x80095494))
        goto finish;
    value = r_u32(0x800A8D6C);
    if (value) {
        w_u32(0x800A8D6C, value - 1);
        w_u32(actor + 0x181C, 0);
        goto age_if_unchanged;
    }
    if (!r_u32(0x800954A4)) {
        w_u8(actor + 0xE5, 0);
        if (t3_8003c87c(actor)) {
            if (r_u32(actor + 0x181C) != 1)
                goto select_other_type;
            w_u32(actor + 0x181C, 0);
        } else {
            if (t3_8003c8cc(actor))
                w_u32(actor + 0x181C, 0);
            duration = t3_8003c8f0(actor);
            if (duration) {
                w_u8(actor + 0xE5, 1);
                w_u32(actor + 0x181C, 1);
            }
        }
        if (r_u32(actor + 0x181C) == 1)
            goto dispatch;
select_other_type:
        if (t3_8003c9b4(actor)) {
            if (r_u32(actor + 0x181C) - 2 < 2)
                w_u32(actor + 0x181C, 0);
        } else {
            if (t3_8003ca30(actor))
                w_u32(actor + 0x181C, 0);
            if (t3_8003ca5c(actor))
                w_u32(actor + 0x181C, 0);
            if (!r_u32(actor + 0x181C)) {
                duration = t3_8003ca9c(actor);
                value = 2;
                if (!duration) {
                    duration = t3_8003caf0(actor);
                    value = 3;
                }
                if (duration) {
                    w_u32(actor + 0x181C, value);
                    w_u8(actor + 0xE5, 1);
                }
            }
        }
    }
dispatch:
    if (!r_u8(actor + 0xE5))
        goto age;
    mode = r_u32(actor + 0x181C);
    if (mode == 1) {
        if (r_u32(0x800954A4)) {
            w_u32(actor + 0x1878, r_u32(actor + 0x1870) - 1);
        } else {
            previous = r_u32(actor + 0x1824);
            w_u32(actor + 0x1878, duration - 1);
            frame = r_u8(actor + 0x5A);
            animation = r_u32(actor + 0x54);
            w_u32(actor + 0x1870, duration);
            w_u32(actor + 0x187C, previous);
            value = r_u8(actor + 0x58);
            w_u8(actor + 0x1884, frame - 1);
            w_u32(actor + 0x1880, animation);
            w_u8(actor + 0x1885, value - 1);
        }
    } else if (mode == 2) {
        if (!r_u32(0x800954A4)) {
            value = (uint32)r_s16(actor + 0x1A0);
            animation = r_u32(actor + 0x54);
            frame = r_u8(actor + 0x198);
            w_u32(actor + 0x187C, animation);
            w_u8(actor + 0x1884, frame);
            data = t3_8002d178(actor, value);
            w_u32(actor + 0x1880, data);
            value = t3_800310cc(actor);
            w_u8(actor + 0x1885, value);
            w_u32(actor + 0x1870, duration);
        }
        w_u32(actor + 0x1878, 0);
    } else if (mode == 3) {
        if (!r_u32(0x800954A4)) {
            animation = r_u32(actor + 0x54);
            value = (uint32)r_s16(actor + 0x6E);
            frame = r_u8(animation + 0x18);
            w_u32(actor + 0x187C, animation);
            w_u8(actor + 0x1884, (sint32)value <= 0 ? 0 : frame);
            animation = r_u32(actor + 0x54);
            value = (uint32)r_s16(animation + 0x10);
            data = t3_8002d178(actor, value);
            w_u32(actor + 0x1880, data);
            w_u8(actor + 0x1885, 0);
            w_u32(actor + 0x1870, duration);
        }
        w_u32(actor + 0x1878, 0);
    }
    mode = r_u32(actor + 0x181C);
    if (mode == 1) {
        animation = r_u32(actor + 0x1880);
        frame = r_u8(actor + 0x1885);
        data = r_u32(animation);
        t3_800389c0(data, (uint16 *)psx_addr(0x1F800072, 6), (sint32)frame);
        t3_80038970((uint16 *)psx_addr(0x1F800072, 2), (uint16 *)psx_addr(0x1F800074, 2), (uint16 *)psx_addr(0x1F800076, 2), actor);
        for (index = 0; index < 3; ++index) {
            right = r_u16(0x1F800072 + index * 2);
            left = r_u16(actor + 0x1810 + index * 2);
            w_u16(actor + 0x1868 + index * 2, left - right);
        }
        animation = r_u32(actor + 0x1880);
        frame = r_u8(actor + 0x1885);
        data = r_u32(animation);
        t3_80038b4c(data, 0x1F800072, (sint32)frame);
        t3_8003ad10(0x1F800072, target, &target_written);
        for (bone = 1; bone < 18; ++bone) {
            for (index = 0; index < 9; ++index) {
                left = r_u16(actor + 0x1390 + bone * 32 + index * 2);
                right = (target_written & ((uint32)1 << bone)) != 0
                    ? (uint16)target[bone].m[index / 3][index % 3] : left;
                w_u16(actor + 0x15D0 + bone * 32 + index * 2, left - right);
            }
        }
    } else if ((sint32)mode > 0 && (sint32)mode < 4) {
        animation = r_u32(actor + 0x187C);
        frame = r_u8(actor + 0x1884);
        data = r_u32(animation);
        t3_800389c0(data, (uint16 *)psx_addr(0x1F800000, 6), (sint32)frame);
        t3_80038970((uint16 *)psx_addr(0x1F800000, 2), (uint16 *)psx_addr(0x1F800002, 2), (uint16 *)psx_addr(0x1F800004, 2), actor);
        animation = r_u32(actor + 0x1880);
        frame = r_u8(actor + 0x1885);
        data = r_u32(animation);
        t3_800389c0(data, (uint16 *)psx_addr(0x1F800072, 6), (sint32)frame);
        t3_80038970((uint16 *)psx_addr(0x1F800072, 2), (uint16 *)psx_addr(0x1F800074, 2), (uint16 *)psx_addr(0x1F800076, 2), actor);
        for (index = 0; index < 3; ++index) {
            left = r_u16(0x1F800000 + index * 2);
            right = r_u16(0x1F800072 + index * 2);
            w_u16(actor + 0x1868 + index * 2, right - left);
        }
        animation = r_u32(actor + 0x187C);
        frame = r_u8(actor + 0x1884);
        data = r_u32(animation);
        t3_80038b4c(data, 0x1F800000, (sint32)frame);
        animation = r_u32(actor + 0x1880);
        frame = r_u8(actor + 0x1885);
        data = r_u32(animation);
        t3_80038b4c(data, 0x1F800072, (sint32)frame);
        t3_8003ad10(0x1F800000, first, &first_written);
        t3_8003ad10(0x1F800072, second, &second_written);
        for (bone = 1; bone < 18; ++bone) {
            for (index = 0; index < 9; ++index) {
                if ((first_written & second_written & ((uint32)1 << bone)) != 0) {
                    left = (uint16)second[bone].m[index / 3][index % 3];
                    right = (uint16)first[bone].m[index / 3][index % 3];
                } else {
                    left = 0;
                    right = 0;
                }
                w_u16(actor + 0x15D0 + bone * 32 + index * 2, left - right);
            }
        }
    }
age_if_unchanged:
    if (r_u8(actor + 0xE5))
        goto finish;
age:
    mode = r_u32(actor + 0x181C);
    if (mode == 1) {
        current = r_u32(actor + 0x1878);
        if ((sint32)current > 0)
            w_u32(actor + 0x1878, current - 1);
        else if ((sint32)current < 0)
            w_u32(actor + 0x1878, 0);
    } else if ((sint32)mode >= 2 && (sint32)mode < 4) {
        current = r_u32(actor + 0x1878);
        limit = r_u32(actor + 0x1870);
        if ((sint32)current < (sint32)limit)
            w_u32(actor + 0x1878, current + 1);
        else if ((sint32)limit < (sint32)current)
            w_u32(actor + 0x1878, limit);
    } else if (!mode) {
        w_u32(actor + 0x1878, 0);
        w_u32(actor + 0x1870, 0);
    }
finish:
    animation = r_u32(actor + 0x54);
    frame = (uint32)r_s16(actor + 0x58);
    w_u32(actor + 0x1824, animation);
    heading = r_u16(0x800A858A);
    w_u32(actor + 0x182C, frame);
    w_u16(actor + 0x1812, heading);
    return 0;
}

uint32 t3_800676a0(uint32 actor, uint32 opponent, uint32 owner, uint32 direction)
{
    uint32 chosen = owner ? opponent : actor;
    uint32 sequence;
    uint32 selection;
    uint32 animation;
    uint32 category;
    sint32 kind;
    sint32 duration;

    FUNCTION_MARKER(0x800676A0, "SLUS_004.02");
    if (r_u32(0x800A8CA0) != 0xFFFFFFFF) {
        t3_80062a10();
        return 0xFFFFFFFF;
    }
    animation = r_u32(chosen + 0x54);
    category = r_u8(animation + 0x24);
    selection = r_u16(0x800A8CA6);
    sequence = t3_80067260(category, direction, owner, &selection);
    w_u32(0x800A8CA0, sequence);
    w_u16(0x800A8CA6, selection);
    if (sequence == 0xFFFFFFFF)
        return 0xFFFFFFFF;
    if (sequence < 30)
        kind = r_s16(0x80024170 + sequence * 2);
    else
        kind = (sint32)sequence < 0 && sequence <= 0x87FFFFFF ? -1 : 0;
    w_u16(0x800A8CA4, (uint32)kind);
    if (kind < 0) {
        animation = r_u32(chosen + 0x54);
        w_u16(0x800A8C9A, 0);
        w_u32(0x800A8CA8, animation);
        return 0;
    }
    if (!kind)
        return 0xFFFFFFFF;
    sequence = r_u32(0x800A8CA0);
    if (sequence < 30)
        kind = r_s16(0x80024170 + sequence * 2) - 1;
    else
        kind = (sint32)sequence < 0 && sequence <= 0x87FFFFFF ? -2 : -1;
    duration = r_s16(0x80023F18 + (uint32)kind * 20);
    animation = r_u32(chosen + 0x54);
    w_u32(0x800A8CA8, animation);
    w_u16(0x800A8C9A, duration < 2);
    return (uint32)duration;
}

uint32 t3_8006782c(uint32 actor, uint32 opponent, uint32 state)
{
    uint32 animation;
    uint32 category;
    uint32 sequence;
    uint32 selection;
    uint32 owner;
    sint32 kind;
    sint32 duration;

    FUNCTION_MARKER(0x8006782C, "SLUS_004.02");
    (void)opponent;
    if (r_u32(0x800A8CA0) == 0xFFFFFFFF) {
        t3_80062a10();
        return 0xFFFFFFFF;
    }
    animation = r_u32(actor + 0x54);
    owner = (uint32)r_s16(actor + 0x12);
    category = r_u8(animation + 0x24);
    selection = r_u16(0x800A8CA6);
    sequence = t3_80067260(category, state, owner, &selection);
    w_u32(0x800A8CA0, sequence);
    w_u16(0x800A8CA6, selection);
    if (sequence == 0xFFFFFFFF)
        return 0xFFFFFFFF;
    if (sequence < 30)
        kind = r_s16(0x80024170 + sequence * 2);
    else
        kind = (sint32)sequence < 0 && sequence <= 0x87FFFFFF ? -1 : 0;
    w_u16(0x800A8CA4, (uint32)kind);
    if (kind < 0) {
        animation = r_u32(actor + 0x54);
        w_u16(0x800A8C9A, 0);
        w_u32(0x800A8CA8, animation);
        return 0;
    }
    if (!kind)
        return 0xFFFFFFFF;
    sequence = r_u32(0x800A8CA0);
    if (sequence < 30)
        kind = r_s16(0x80024170 + sequence * 2) - 1;
    else
        kind = (sint32)sequence < 0 && sequence <= 0x87FFFFFF ? -2 : -1;
    duration = r_s16(0x80023F18 + (uint32)kind * 20);
    w_u16(0x800A8C9A, duration < 2);
    return (uint32)duration;
}
