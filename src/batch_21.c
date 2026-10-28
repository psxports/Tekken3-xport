#include "game_functions.h"

uint32 t3_80042760(void)
{
    uint32 mode;
    uint32 timer;
    uint32 first;
    uint32 second;
    uint32 x;
    uint32 z;
    uint32 delta_x;
    uint32 delta_z;
    uint32 magnitude;
    uint32 angle;
    uint32 index;
    uint32 sine;
    uint32 cosine;
    uint32 buttons;
    uint32 value;

    FUNCTION_MARKER(0x80042760, "SLUS_004.02");
    if (r_u16(0x800ADF0A) & 1) {
        mode = r_u32(0x80097950) + 1;
        w_u32(0x80097950, mode);
        if ((sint32)mode >= 4)
            w_u32(0x80097950, 0);
        w_u32(0x80097954, 64);
    }
    mode = r_u32(0x80097950);
    if (mode == 2) {
        w_u32(0x8009E638, 0 - 12800);
        w_u32(0x8009E634, 0);
        w_u32(0x8009E63C, 0);
        w_u32(0x8009E620, 1024);
        w_u32(0x8009E624, 0);
        w_u32(0x8009E628, 0);
        mode = r_u32(0x80097950);
    }
    if (mode == 3) {
        first = r_u32(0x800AA190);
        second = r_u32(0x800ABA1C);
        x = r_u32(0x8009E634);
        delta_x = t3_camera_divide(first + second, 2) - x;
        first = r_u32(0x800AA198);
        second = r_u32(0x800ABA24);
        z = r_u32(0x8009E63C);
        delta_z = t3_camera_divide(first + second, 2) - z;
        magnitude = (sint32)delta_x < 0 ? 0 - delta_x : delta_x;
        if ((sint32)magnitude >= 257)
            delta_x = (sint32)delta_x < 0 ? 0 - 256 : 256;
        magnitude = (sint32)delta_z < 0 ? 0 - delta_z : delta_z;
        if ((sint32)magnitude >= 257)
            delta_z = (sint32)delta_z < 0 ? 0 - 256 : 256;
        w_u32(0x8009E638, 0 - 12800);
        w_u32(0x8009E620, 1024);
        x = r_u32(0x8009E634);
        w_u32(0x8009E624, 0);
        w_u32(0x8009E628, 0);
        w_u32(0x8009E634, x + (uint32)((sint32)delta_x >> 1));
        z = r_u32(0x8009E63C);
        w_u32(0x8009E63C, z + (uint32)((sint32)delta_z >> 1));
    }
    timer = r_u32(0x80097954);
    if (timer) {
        timer -= 1;
        w_u32(0x80097954, timer);
        if ((sint32)timer < 0)
            w_u32(0x80097954, 0);
        if (r_u32(0x80097954) & 16) {
            if (!r_u32(0x80097950))
                goto manual_camera;
        }
    }
    if (r_u32(0x80097950))
        goto publish_camera;
manual_camera:
    angle = r_u32(0x8009E624);
    index = (angle & 0xFFF) * 2;
    cosine = (uint32)r_s16(0x8001F0C4 + index);
    sine = (uint32)r_s16(0x8001E8C4 + index);
    cosine = t3_camera_divide(cosine * 70, 4096);
    sine = t3_camera_divide(sine * 70, 4096);
    buttons = r_u16(0x800ADD6A);
    if ((buttons & 0xC0) == 0xC0 || !(buttons & 0xC0)) {
        if (buttons & 0x1000) {
            x = r_u32(0x8009E634);
            z = r_u32(0x8009E63C);
            w_u32(0x8009E634, x - sine);
            w_u32(0x8009E63C, z + cosine);
        }
        if (buttons & 0x4000) {
            x = r_u32(0x8009E634);
            z = r_u32(0x8009E63C);
            w_u32(0x8009E634, x + sine);
            w_u32(0x8009E63C, z - cosine);
        }
        if ((buttons & 0xC0) == 0xC0) {
            if (buttons & 0x8000)
                w_u32(0x8009E624, angle + 5);
            if (buttons & 0x2000) {
                value = r_u32(0x8009E624);
                w_u32(0x8009E624, value - 5);
            }
            goto publish_camera;
        }
    } else if (buttons & 0x80) {
        if (buttons & 0x1000) {
            value = r_u32(0x8009E638);
            w_u32(0x8009E638, value - 70);
        }
        if (buttons & 0x4000) {
            value = r_u32(0x8009E638);
            w_u32(0x8009E638, value + 70);
        }
    } else {
        if (buttons & 0x1000) {
            value = r_u32(0x8009E620);
            w_u32(0x8009E620, value + 5);
        }
        if (buttons & 0x4000) {
            value = r_u32(0x8009E620);
            w_u32(0x8009E620, value - 5);
        }
        if (buttons & 0x2000)
            w_u32(0x8009E624, angle + 5);
        if (buttons & 0x8000) {
            value = r_u32(0x8009E624);
            w_u32(0x8009E624, value - 5);
        }
        goto publish_camera;
    }
    if (buttons & 0x2000) {
        x = r_u32(0x8009E634);
        z = r_u32(0x8009E63C);
        w_u32(0x8009E634, x + cosine);
        w_u32(0x8009E63C, z + sine);
    }
    if (buttons & 0x8000) {
        x = r_u32(0x8009E634);
        z = r_u32(0x8009E63C);
        w_u32(0x8009E634, x - cosine);
        w_u32(0x8009E63C, z - sine);
    }
publish_camera:
    return t3_800461c0(0x8009E620);
}

uint32 t3_80047d98(uint32 first_actor, uint32 second_actor, uint32 sphere, VECTOR *output, uint32 initial_segment, uint32 initial_count)
{
    uint32 actor = first_actor;
    uint32 segment = initial_segment;
    uint32 count = initial_count;
    uint32 mask = 0;
    uint32 bit = 2;
    uint32 pass;
    uint32 part;
    uint32 animation;
    uint32 model;
    uint32 side;
    uint32 end;
    uint32 start;

    FUNCTION_MARKER(0x80047D98, "SLUS_004.02");
    for (pass = 0; pass < 2; ++pass) {
        animation = r_u32(actor + 84);
        model = r_u32(animation + 40);
        if (r_u8(model) < 24) {
            count = r_u8(actor + 0xE3);
            segment = actor + 0x1AC;
            if (count >= 5)
                count = 4;
        } else {
            side = r_u8(actor + 30);
            mask |= 8;
            if (side < 3) {
                count = r_u32(side ? 0x800ADEB0 : 0x800ADEAC);
                segment = t3_8006e51c(side != 0, 0);
            }
        }
        for (part = 0; (sint32)part < (sint32)count; ++part, segment += 24) {
            if (!t3_8004770c(segment, sphere))
                continue;
            end = r_u32(segment + 12);
            start = r_u32(segment);
            output->vx = (sint32)(end - start);
            end = r_u32(segment + 16);
            start = r_u32(segment + 4);
            output->vy = (sint32)(end - start);
            end = r_u32(segment + 20);
            start = r_u32(segment + 8);
            mask |= bit;
            output->vz = (sint32)(end - start);
            break;
        }
        actor = second_actor;
        bit = 1;
    }
    return mask;
}

uint32 t3_80048208(uint32 actor, uint32 opponent)
{
    uint32 callback;

    FUNCTION_MARKER(0x80048208, "SLUS_004.02");
    callback = r_u32(0x8009E728);
    return psx_dispatch_guest_call(callback, 2, actor, opponent, 0);
}

void t3_800501c0(void)
{
    uint32 ordering;
    uint32 packet;

    FUNCTION_MARKER(0x800501C0, "SLUS_004.02");
    switch (r_s16(0x800AE224)) {
    case 0:
        t3_80055884(5);
        t3_8002960c(0);
        t3_8006c588(16);
        w_u16(0x800AE224, 1);
        return;
    case 1:
        t3_8004b61c(0);
        t3_80028f5c();
        if (!r_u8(0x800980FE))
            t3_8004c3b8();
        t3_80052bb4(6);
        t3_80052c30(16, 2);
        return;
    case 2:
    case 3:
        w_u16(0x800AE224, r_u16(0x800AE224) + 1);
        break;
    case 4:
        t3_bns_06_800c2dbc();
        w_u16(0x800AE224, r_u16(0x800AE224) + 1);
        goto wait_for_card;
    case 5:
wait_for_card:
        if (t3_8004c470())
            return;
        w_u16(0x800AE204, 17);
        w_u16(0x800AE224, 0);
        break;
    default:
        return;
    }
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    packet = t3_8004da10(ordering, packet, 0x60000000, 0, 0x01E00170);
    w_u32(0x800ADF64, packet);
}

uint32 t3_80035b28(uint32 actor, uint32 *first_output, uint32 *second_output, uint32 *third_output)
{
    uint32 character;
    uint32 side;
    uint32 table;
    uint32 resource;
    uint32 value;
    uint32 selector;
    uint32 index;

    FUNCTION_MARKER(0x80035B28, "SLUS_004.02");
    character = (uint32)r_s16(actor + 0x1C);
    side = r_u8(actor + 0x1E);
    // Logical 07 resource getters D3554, D3598 and D3570
    table = r_u32(0x801BFE40);
    resource = (uint32)r_s8(table + character);
    value = t3_80077590(0x8010BE40, (sint32)((resource << 1) | 1));
    *first_output = value;
    value = t3_80077590(0x8010BE40, (sint32)(resource << 1));
    *second_output = value;
    w_u32(0x800ADEC8 + side * 4, value);
    // Logical 07 D35C4 consumes the cached actor side in A0
    selector = (uint32)r_s8(0x800DD810);
    index = r_u8(0x800DD814 + selector) + side;
    value = t3_80077590(0x8010BE40, (sint32)index);
    *third_output = value;
    return value;
}

uint32 t3_bns_06_800c35b0(uint32 candidate)
{
    uint32 index;

    FUNCTION_MARKER(0x800C35B0, "SLUS_004.02-BNS-logical-06");
    for (index = 0; index < 4; ++index) {
        if (r_u32(0x800CB868 + index * 4) == candidate)
            return 0;
    }
    return 1;
}

uint32 t3_bns_06_800c331c(void)
{
    uint32 candidate;
    sint32 x;
    sint32 z;
    sint32 sample;

    FUNCTION_MARKER(0x800C331C, "SLUS_004.02-BNS-logical-06");
    w_u32(0x800AFA88, 6);
    w_u8(0x800AFAA0, 0);
    w_u16(0x800ADED4, 1);
    for (;;) {
        candidate = r_u8(0x800980FD) % 13;
        w_u8(0x800980FD, candidate);
        if (t3_bns_06_800c35b0(r_u8(0x800980FD)))
            break;
        w_u8(0x800980FD, r_u8(0x800980FD) + 1);
    }
    candidate = r_u8(0x800980FD);
    w_u8(0x800980FD, candidate + 1);
    w_u16(0x800ADC84, candidate);
    t3_80036974();
    sample = r_s16(0x800208C4);
    w_u16(0x800D3140, 3072);
    x = sample * 5000 / 4096;
    sample = r_s16(0x800200C4);
    w_u16(0x800D313C, x);
    z = sample * 5000 / 4096;
    w_u16(0x800D313E, z);
    w_u32(0x800AA190, 0 - 1000);
    w_u32(0x800ABA1C, 1000);
    w_u32(0x800AA198, 0);
    w_u32(0x800ABA24, 0);
    w_u32(0x800D312C, (uint32)(sint32)(sint16)x);
    w_u32(0x800D3130, 0 - 1400);
    w_u32(0x800D3134, (uint32)(sint32)(sint16)z);
    w_u32(0x800D3118, 0);
    w_u32(0x800D311C, 0);
    w_u32(0x800D3120, 0);
    return 0 - 1400;
}

uint32 t3_bns_06_800c2dbc(void)
{
    FUNCTION_MARKER(0x800C2DBC, "SLUS_004.02-BNS-logical-06");
    t3_80029638(1);
    t3_bns_06_800c331c();
    return t3_80029638(0);
}

uint32 t3_80072e74(uint32 event, uint32 retained_event)
{
    uint32 state;
    uint32 value;

    FUNCTION_MARKER(0x80072E74, "SLUS_004.02");
    state = r_u8(event + 14);
    switch (state) {
    case 0:
        t3_80072f38(event);
        goto update_attached_effect;
    case 1:
update_attached_effect:
        t3_80073054(event);
        break;
    case 2:
        t3_800732a4(event);
        break;
    case 3:
        t3_80073394(event);
        break;
    default:
        break;
    }
    state = r_u8(event + 14);
    if (state != 1)
        return 1;
    value = retained_event & 15;
    if (value)
        return value;
    return t3_80039f0c((const uint32 *)psx_addr(event + 40, 12), 0x8002517C);
}

uint32 t3_bns_06_800c2e48(uint32 output, uint32 first, uint32 second)
{
    uint32 result;

    FUNCTION_MARKER(0x800C2E48, "SLUS_004.02-BNS-logical-06");
    w_u16(output + 12, first);
    w_u16(output + 10, second);
    result = t3_8004cc08(output, 0x800B908C);
    w_u16(output + 4, 0);
    w_u16(output + 8, 0);
    w_u8(output + 6, 0);
    w_u8(output + 7, 0);
    return result;
}

uint32 t3_bns_06_800c34bc(void)
{
    uint32 yaw;
    uint32 angle;
    sint32 sample;
    sint32 x;
    sint32 z;

    FUNCTION_MARKER(0x800C34BC, "SLUS_004.02-BNS-logical-06");
    yaw = r_u32(0x800D311C);
    angle = (r_u16(0x800D3140) - 3) & 0xFFF;
    w_u32(0x800D311C, yaw - 3);
    sample = r_s16(0x8001F0C4 + angle * 2);
    w_u16(0x800D3140, angle);
    x = sample * 5000 / 4096;
    sample = r_s16(0x8001E8C4 + angle * 2);
    w_u16(0x800D313C, x);
    z = sample * 5000 / 4096;
    w_u16(0x800D313E, z);
    w_u32(0x800D312C, (uint32)(sint32)(sint16)x);
    w_u32(0x800D3134, (uint32)(sint32)(sint16)z);
    t3_800461c0(0x800D3118);
    t3_80048240(0x800A9228, 0x800AAAB4);
    return t3_8006d7cc();
}

void t3_800c2434(void)
{
    uint32 control = 0x800CB878;
    uint32 data = 0x80097F44;
    uint32 flags;
    uint32 mode;
    uint32 value;
    uint32 count;
    uint32 result;
    uint32 index;
    uint32 packet;
    uint32 ordering;
    uint32 saved_packet;
    uint32 bank;
    uint32 target_word;
    sint32 target;
    sint32 step;

    FUNCTION_MARKER(0x800C2434, "SLUS_004.02-BNS-logical-06");
    if (r_s16(0x800AE224) >= 2 && t3_80050f38()) {
        if (r_u8(data + 0x1BA)) {
            t3_bns_06_800c2e90(0x800D3108, 2);
            if (r_u8(data + 0x1BA) & 1)
                t3_8004cc08(r_u32(data) + 4, 0x800D3108);
            if (r_u8(data + 0x1BA) & 2)
                t3_8004cc08(r_u32(data + 4) + 4, 0x800D3108);
            w_u8(data + 0x1BA, 0);
            t3_8004c390();
        }
        t3_8004b61c(1);
        t3_8004b5f4();
        t3_8004f8f8(4);
        return;
    }
    switch (r_s16(0x800AE224)) {
    case 0:
        if (!r_u8(data + 0x1BA)) {
            value = r_u16(0x800ADD6A);
            mode = r_u8(data + 0x1B8);
            if (value & 0x2000)
                mode = 0;
            else if (value & 0x8000)
                mode = 1;
            else if (value & 0x1000)
                mode = 2;
            w_u8(data + 0x1B8, mode);
        }
        mode = r_u8(data + 0x1B8);
        t3_8004c91c(0x800C35E8, 0x800B9094, mode);
        w_u16(0x800AE224, 1);
        goto phase_one;
    case 1:
phase_one:
        if (!r_u8(data + 0x1BA)) {
            mode = r_u8(data + 0x1B8);
            if (mode == 0)
                t3_bns_06_800c172c(control, data);
            else if (mode == 1) {
                result = t3_bns_06_800c189c(data + 0xB8, r_u32(data + 4));
                if (result != 0xFFFFFFFF)
                    w_u32(data + 4, data + 0xB8 + result * 8);
                w_u16(control + 12, 10);
                for (index = 0; index < 10; ++index)
                    w_u16(control + 32 - index * 2, 9 - index);
                w_u16(control + 8, 150);
            } else if (mode == 2)
                t3_bns_06_800c19b8(control);
            w_u16(control + 0x88, 0xFFFF);
        } else {
            t3_8006ae14(2, 0);
            flags = r_u8(data + 0x1BA);
            if (flags & 1) {
                w_u8(data + 0x1B8, 0);
                result = t3_bns_06_800c172c(control, data);
                w_u16(control + 0x88, result);
            } else if (flags & 2) {
                value = r_u32(data + 4);
                w_u8(data + 0x1B8, 1);
                result = t3_bns_06_800c189c(data + 0xB8, value);
                if (result != 0xFFFFFFFF)
                    w_u32(data + 4, data + 0xB8 + result * 8);
                w_u16(control + 12, 10);
                for (index = 0; index < 10; ++index)
                    w_u16(control + 32 - index * 2, 9 - index);
                w_u16(control + 8, 150);
                w_u16(control + 0x88, result);
            } else {
                w_u8(data + 0x1B8, 2);
                w_u8(data + 0x1BA, 0);
                break;
            }
            value = r_u8(data + 0x1BB);
            count = r_u8(data + 0x1BC);
            t3_bns_06_800c2e48(0x800D3108, value, count);
        }
        w_u16(control + 6, 0);
        w_u16(control, 0);
        w_u16(0x800AE224, 2);
        break;
    case 2:
        w_u16(control, 1);
        w_u32(0x800AE214, 0);
        w_u16(0x800AE224, 3);
        goto phase_three;
    case 3:
phase_three:
        packet = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        value = r_u32(0x800AE214);
        packet = t3_8004e000(ordering + 28, packet, (sint32)value);
        value = r_u32(0x800AE214);
        w_u32(0x800ADF64, packet);
        value += 8;
        w_u32(0x800AE214, value);
        if ((sint32)value >= 256)
            w_u16(0x800AE224, r_u8(data + 0x1BA) ? 10 : 4);
        break;
    case 4:
        count = r_u16(control + 12);
        value = count % 5;
        w_u32(control + 0x54, count);
        if (!value)
            value = 5;
        w_u32(control + 0x50, count - value);
        w_u16(control, 2);
        value = r_u32(control + 0x54);
        w_u16(0x800AE224, 5);
        w_u16(control + 2, 168 - value * 72);
        break;
    case 5:
        value = r_u32(control + 0x50);
        count = r_u16(control + 2);
        w_u16(control + 4, 168 - value * 72);
        target = r_s16(control + 4);
        value = count + 12;
        w_u16(control + 2, value);
        target_word = r_u16(control + 4);
        if ((sint16)value >= target) {
            value = r_u32(control + 0x50);
            w_u16(control + 2, target_word);
            w_u16(control + 6, 0);
            w_u16(control + 10, 0);
            w_u16(0x800AE224, 6);
            w_u32(control + 0x54, value);
        }
        break;
    case 6:
        value = r_u16(control + 6);
        w_u16(control + 10, value < 64 ? value : 64);
        value = r_u16(control + 6);
        count = r_u16(control + 8);
        value += 1;
        w_u16(control + 6, value);
        if ((value & 0xFFFF) < count)
            break;
        value = r_u32(control + 0x50);
        if ((sint32)value > 0) {
            w_u32(control + 0x50, value - 5);
            w_u16(0x800AE224, 5);
        } else {
            w_u32(0x800AE214, 0);
            w_u16(0x800AE224, 7);
        }
        break;
    case 7:
        value = r_u32(0x800AE214) + 1;
        w_u32(0x800AE214, value);
        if ((sint32)value >= 60)
            w_u16(0x800AE224, 8);
        break;
    case 8:
        w_u32(0x800AE214, 256);
        w_u16(0x800AE224, 9);
        goto phase_nine;
    case 9:
phase_nine:
        packet = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        value = r_u32(0x800AE214);
        packet = t3_8004e000(ordering + 28, packet, (sint32)value);
        value = r_u32(0x800AE214);
        w_u32(0x800ADF64, packet);
        value -= 8;
        w_u32(0x800AE214, value);
        if ((sint32)value <= 0) {
            w_u16(control, 0);
            w_u16(0x800AE224, 14);
        }
        break;
    case 10:
        value = r_u16(control + 0x88);
        count = r_u16(control + 12);
        if (value < 2) {
            w_u32(control + 0x50, 0);
            w_u16(control + 0x86, value);
        } else if ((sint32)(count - 3) < (sint32)value) {
            w_u32(control + 0x50, count - 5);
            w_u16(control + 0x86, 5 - (count - value));
        } else {
            w_u32(control + 0x50, value - 2);
            w_u16(control + 0x86, 2);
        }
        w_u16(control, 2);
        count = r_u16(control + 12);
        w_u16(control + 10, 64);
        w_u16(0x800AE224, 11);
        w_u32(control + 0x54, 0);
        w_u16(control + 2, 168 - count * 72);
        break;
    case 11:
        value = r_u32(control + 0x50);
        target = r_s16(control + 2);
        value = 168 - value * 72;
        step = ((sint32)(sint16)value - target + 15) >> 4;
        w_u16(control + 4, value);
        if (!step)
            step = 1;
        else if (step >= 17)
            step = 16;
        value = r_u16(control + 2);
        target = r_s16(control + 4);
        value += (uint32)step;
        w_u16(control + 2, value);
        target_word = r_u16(control + 4);
        if ((sint16)value >= target) {
            w_u16(control + 6, 3000);
            w_u16(control + 2, target_word);
            w_u16(control, 3);
            w_u16(0x800AE224, 12);
        }
        break;
    case 12:
        value = r_u16(control + 6) - 1;
        w_u16(control + 6, value);
        result = t3_bns_06_800c2e90(0x800D3108, (value & 0xFFFF) == 0);
        if (r_u8(data + 0x1BA) & 1)
            t3_8004cc08(r_u32(data) + 4, 0x800D3108);
        if (r_u8(data + 0x1BA) & 2)
            t3_8004cc08(r_u32(data + 4) + 4, 0x800D3108);
        if (r_u8(0x800D310E))
            t3_800753bc(0, 0x50F4, 0);
        if (r_u8(0x800D310F))
            t3_800753bc(0, 0x55F5, 0);
        if (result) {
            w_u8(data + 0x1BA, 0);
            t3_8004c390();
            w_u16(control, 4);
            w_u32(0x800AE214, 0);
            w_u16(0x800AE224, 13);
        }
        break;
    case 13:
        value = r_u32(0x800AE214) + 1;
        w_u32(0x800AE214, value);
        if ((sint32)value >= 120) {
            t3_8006bc98(0, 60);
            w_u16(0x800AE224, 8);
        }
        break;
    case 14:
        value = (r_u8(data + 0x1B8) + 1) % 3;
        w_u16(control, 0);
        w_u8(data + 0x1B8, value);
        t3_8004f8f8(3);
        break;
    default:
        break;
    }
    bank = r_u32(0x800ADEFC);
    saved_packet = r_u32(0x800ADF64);
    mode = r_u16(control);
    w_u32(0x800ADF64, control + 144 + bank * 15360);
    switch (mode) {
    case 0:
        ordering = r_u32(0x800A9218);
        packet = r_u32(0x800ADF64);
        packet = t3_8004da10(ordering, packet, 0x60000000, 0, 0x01E00170);
        w_u32(0x800ADF64, packet);
        break;
    case 1:
        t3_bns_06_800c1b18(r_u8(data + 0x1B8));
        goto draw_scene;
    case 2:
        t3_bns_06_800c1b18(r_u8(data + 0x1B8));
        t3_bns_06_800c1de8(r_u8(data + 0x1B8));
        goto draw_scene;
    case 3:
        ordering = r_u32(0x800A9218);
        packet = r_u32(0x800ADF64);
        value = (r_u16(control + 0x86) * 72 + 104) << 16;
        packet = t3_8004da10(ordering, packet, 0x62100000, value, 0x00480170);
        packet = t3_8004daa8(ordering, packet, 0);
        w_u32(0x800ADF64, packet);
        goto countdown_text;
    case 4:
countdown_text:
        value = r_u16(control + 6);
        if (value)
            value = (value + 59) / 60;
        t3_8004ce74(0x800B8FEC, 1, 6, 1, 316, 48, value);
        t3_bns_06_800c1b18(r_u8(data + 0x1B8));
        t3_bns_06_800c22a8(r_u8(data + 0x1B8));
draw_scene:
        t3_800484c8();
        t3_bns_06_800c34bc();
        break;
    default:
        break;
    }
    w_u32(0x800ADF64, saved_packet);
}

static uint32 t3_compare_guest_string_core(uint32 first, uint32 second, const uint8 *native_second)
{
    uint32 left;
    uint32 right;

    right = native_second ? *native_second : r_u8(second);
    while (right) {
        left = r_u8(first);
        if (left != right)
            return 0xFFFFFFFF;
        if (native_second)
            ++native_second;
        else
            ++second;
        right = native_second ? *native_second : r_u8(second);
        ++first;
    }
    left = r_u8(first);
    right = native_second ? *native_second : r_u8(second);
    return (left ^ right) != 0;
}

uint32 t3_8004cc30(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x8004CC30, "SLUS_004.02");
    return t3_compare_guest_string_core(first, second, 0);
}

uint32 t3_compare_guest_native_string(uint32 first, const uint8 *second)
{
    return t3_compare_guest_string_core(first, 0, second);
}

uint32 t3_8004cc80(sint32 minimum, uint32 value, sint32 maximum)
{
    FUNCTION_MARKER(0x8004CC80, "SLUS_004.02");
    if ((sint32)value < minimum)
        return (uint32)maximum;
    if (maximum < (sint32)value)
        return (uint32)minimum;
    return value;
}

uint32 t3_800561b0(uint32 kind)
{
    uint32 flags;
    uint32 mask;
    uint32 present;

    FUNCTION_MARKER(0x800561B0, "SLUS_004.02");
    flags = r_u32(0x80097EF0);
    mask = (uint32)1 << (kind & 31);
    present = flags & mask;
    if (!present) {
        w_u8(0x80097F12, kind);
        w_u16(0x80097F1C, 0);
    }
    w_u32(0x80097EF0, flags | mask);
    return present;
}

uint32 t3_bns_06_800c2e90(uint32 output, uint32 mode)
{
    uint8 candidate[4];
    uint32 controller;
    uint32 selected;
    uint32 held;
    uint32 pressed;
    uint32 position;
    uint32 character;
    uint32 value;
    uint32 source;
    uint32 index;
    sint32 direction;

    FUNCTION_MARKER(0x800C2E90, "SLUS_004.02-BNS-logical-06");
    w_u8(output + 6, 0);
    w_u8(output + 7, 0);
    if (mode == 1)
        goto complete_name;
    if (mode == 2)
        goto restore_name;
    if (mode != 0)
        return 0;
    controller = r_u16(output + 12);
    selected = r_u16(output + 8);
    held = r_u16(0x800ADF00 + controller * 2);
    pressed = r_u16(0x800ADF08 + controller * 2);
    direction = (sint32)((held >> 13) & 1) - (sint32)(held >> 15);
    if (direction)
        w_u8(output + 7, 1);
    selected = t3_8004cc80(0, selected + (uint32)direction, 39);
    position = r_u16(output + 4);
    w_u16(output + 8, selected);
    character = 0;
    if (position < 3)
        character = r_u8(0x800B9054 + r_u16(output + 8));
    position = r_u16(output + 4);
    w_u8(output + position, character);
    if (!(pressed & 0xF0))
        return 0;
    w_u8(output + 6, 1);
    if (character == '<') {
        position = r_u16(output + 4);
        if (position) {
            w_u8(output + position, ' ');
            position = r_u16(output + 4);
            w_u16(output + 4, position - 1);
        }
        return 0;
    }
    if (character == '=')
        goto complete_name;
    position = r_u16(output + 4);
    w_u8(output + position, character);
    position = r_u16(output + 4) + 1;
    w_u16(output + 4, position);
    if ((position & 0xFFFF) < 3)
        return 0;
    if (!(r_u32(0x80097EF0) & 0x20000)) {
        for (index = 0; index < 4; ++index)
            candidate[index] = (uint8)r_u8(output + index);
        position = r_u16(output + 4);
        while (position < 3)
            candidate[position++] = ' ';
        candidate[3] = 0;
        if (!t3_compare_guest_native_string(0x800B9090, candidate))
            t3_800561b0(17);
    }
complete_name:
    position = r_u16(output + 4);
    while (position < 3) {
        w_u8(output + position, ' ');
        ++position;
    }
    w_u8(output + 3, 0);
    w_u16(output + 4, 3);
    for (index = 0; index < 5; ++index) {
        source = r_u32(0x800CB854 + index * 4);
        if (!t3_8004cc30(source, output))
            goto restore_name;
    }
    return 1;
restore_name:
    source = t3_8004eff0(r_u16(output + 10), 0);
    source = r_u32(source);
    for (index = 0; index < 3; ++index) {
        value = r_u8(source);
        ++source;
        w_u8(output + index, value);
    }
    w_u8(output + 3, 0);
    if (mode == 2)
        w_u16(output + 4, 3);
    return 1;
}

uint32 t3_8004cd20(uint32 numerator, uint32 denominator)
{
    FUNCTION_MARKER(0x8004CD20, "SLUS_004.02");
    if (denominator < numerator)
        return 1000;
    while (denominator > 0x3FFFFF) {
        denominator >>= 1;
        numerator >>= 1;
    }
    if (!denominator)
        return 0;
    return numerator * 1000 / denominator;
}

uint32 t3_bns_06_800c172c(uint32 control, uint32 data)
{
    uint32 pairs[44];
    uint32 mask = 0x3FF;
    uint32 count;
    uint32 index;
    uint32 position = 0;
    uint32 selected;
    uint32 label;

    FUNCTION_MARKER(0x800C172C, "SLUS_004.02-BNS-logical-06");
    w_u16(control + 8, 150);
    for (index = 0; index < 22; ++index) {
        if (r_u32(data + 8 + index * 8) <= 0x57E3E)
            mask |= (uint32)1 << index;
    }
    mask &= 0x1FFFFF;
    count = t3_8004ccac(mask);
    w_u16(control + 12, count);
    for (index = 0; index < 22; ++index) {
        if (mask & ((uint32)1 << index)) {
            pairs[position * 2] = index;
            pairs[position * 2 + 1] = r_u32(data + 8 + index * 8);
            ++position;
        }
    }
    t3_8004cd80(pairs, count, 1);
    for (index = 0; index < count; ++index)
        w_u16(control + 14 + index * 2, pairs[index * 2]);
    if (count) {
        selected = r_u32(data);
        for (index = 0; index < count; ++index) {
            label = r_u16(control + 14 + index * 2);
            if (data + 8 + label * 8 == selected)
                return index;
        }
    }
    return 0xFFFFFFFF;
}

uint32 t3_bns_06_800c189c(uint32 table, uint32 entry)
{
    uint32 pairs[20];
    uint16 first[10];
    uint16 second[10];
    uint8 names[10][4];
    uint32 found = 0xFFFFFFFF;
    uint32 index;
    uint32 record;
    uint32 label;

    FUNCTION_MARKER(0x800C189C, "SLUS_004.02-BNS-logical-06");
    for (index = 0; index < 10; ++index) {
        record = table + index * 8;
        first[index] = (uint16)r_u16(record);
        second[index] = (uint16)r_u16(record + 2);
        t3_copy_guest_string_to_native(names[index], record + 4);
        if (record == entry)
            found = index;
        pairs[index * 2] = index;
        pairs[index * 2 + 1] = second[index];
    }
    t3_8004cd80(pairs, 10, 0);
    for (index = 0; index < 10; ++index) {
        record = table + index * 8;
        label = pairs[index * 2];
        w_u16(record, first[label]);
        w_u16(record + 2, second[label]);
        t3_copy_native_string_to_guest(record + 4, names[label]);
    }
    return found;
}

uint32 t3_bns_06_800c19b8(uint32 control)
{
    uint32 pairs[44];
    uint32 mask = 0x3FF;
    uint32 total = 0;
    uint32 count;
    uint32 index;
    uint32 position = 0;
    uint32 value;

    FUNCTION_MARKER(0x800C19B8, "SLUS_004.02-BNS-logical-06");
    for (index = 0; index < 22; ++index) {
        value = t3_80051660(index);
        if (value) {
            mask |= (uint32)1 << index;
            total += value;
        }
    }
    mask &= 0x1FFFFF;
    count = t3_8004ccac(mask);
    w_u16(control + 12, count);
    for (index = 0; index < 22; ++index) {
        if (mask & ((uint32)1 << index)) {
            pairs[position * 2] = index;
            value = t3_80051660(index);
            pairs[position * 2 + 1] = value;
            ++position;
            value = t3_8004cd20(value, total);
            w_u16(control + 0x58 + index * 2, value);
        }
    }
    t3_8004cd80(pairs, count, 0);
    for (index = 0; index < count; ++index)
        w_u16(control + 14 + index * 2, pairs[index * 2]);
    w_u16(control + 8, 150);
    value = r_u16(control + 0x58 + pairs[0] * 2);
    w_u16(control + 0x84, value);
    return value;
}

uint32 t3_8004d924(uint32 ordering, uint32 packet, uint32 color0, uint32 color1, uint32 color2, uint32 color3, uint32 xy0, uint32 xy1, uint32 xy2, uint32 xy3, uint32 uv0, uint32 uv1, uint32 uv23)
{
    uint32 tag;

    FUNCTION_MARKER(0x8004D924, "SLUS_004.02");
    w_u32(packet + 4, color0);
    w_u32(packet + 16, color1);
    w_u32(packet + 28, color2);
    w_u32(packet + 40, color3);
    w_u32(packet + 8, xy0);
    w_u32(packet + 20, xy1);
    w_u32(packet + 32, xy2);
    w_u32(packet + 44, xy3);
    w_u32(packet + 12, uv0);
    w_u32(packet + 24, uv1);
    w_u32(packet + 36, uv23 & 0xFFFF);
    w_u32(packet + 48, uv23 >> 16);
    tag = r_u32(ordering);
    w_u32(packet, (tag & 0xFFFFFF) | 0x0C000000);
    tag = r_u32(ordering);
    w_u32(ordering, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    return packet + 52;
}

uint32 t3_bns_06_800c1b18(uint32 unused_mode)
{
    uint32 ordering;
    uint32 packet;

    FUNCTION_MARKER(0x800C1B18, "SLUS_004.02-BNS-logical-06");
    (void)unused_mode;
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    packet = t3_8004db9c(ordering, packet, 0, 36, 128, 48, 384, 0, 0x7F50, 128);
    packet = t3_8004db9c(ordering, packet, 128, 36, 112, 48, 384, 48, 0x7F50, 128);
    packet = t3_8004d924(ordering, packet, 0x3E808080, 0x00606060, 0x00808080, 0x00606060,
        0x002400F0, 0x00240100, 0x005400F0, 0x00540100, 0x7F503070, 0x00A63080, 0x60806070);
    packet = t3_8004d924(ordering, packet, 0x3E808080, 0x00606060, 0x00808080, 0x00606060,
        0x002400F0, 0x00240100, 0x005400F0, 0x00540100, 0x7FD03070, 0x00C63080, 0x60806070);
    packet = t3_8004d924(ordering, packet, 0x3E606060, 0x00101010, 0x00606060, 0x00101010,
        0x00240100, 0x00240170, 0x00540100, 0x00540170, 0x7F506000, 0x00A66070, 0x90709000);
    packet = t3_8004d924(ordering, packet, 0x3E606060, 0x00101010, 0x00606060, 0x00101010,
        0x00240100, 0x00240170, 0x00540100, 0x00540170, 0x7FD06000, 0x00C66070, 0x90709000);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_06_800c1de8(uint32 mode)
{
    uint32 ordering;
    uint32 packet;
    uint32 index;
    uint32 label;
    uint32 ratio;
    uint32 top_ratio;
    uint32 flags;
    uint32 y;
    sint32 revealed;
    PSX_RECT rectangle;

    FUNCTION_MARKER(0x800C1DE8, "SLUS_004.02-BNS-logical-06");
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    packet = t3_8004db28(ordering, packet, (const PSX_RECT *)psx_addr(0x800AE230, 8));
    w_u32(0x800ADF64, packet);
    if (mode == 0) {
        if (r_u16(0x800CB884)) {
            index = 0;
            do {
                label = r_u16(0x800CB886 + index * 2);
                y = (uint32)r_s16(0x800CB87A) + index * 72;
                t3_bns_06_800c0d1c(index, label, 0x80097F4C + label * 8, 0, y, 0);
                ++index;
            } while (index < r_u16(0x800CB884));
        }
    } else if (mode == 1) {
        if (r_u16(0x800CB884)) {
            index = 0;
            do {
                label = r_u16(0x800CB886 + index * 2);
                y = (uint32)r_s16(0x800CB87A) + index * 72;
                t3_bns_06_800c10b4(index, 0x80097FFC + label * 8, 0, y, 0);
                ++index;
            } while (index < r_u16(0x800CB884));
        }
    } else if (mode == 2) {
        if (r_u16(0x800CB884)) {
            index = 0;
            do {
                label = r_u16(0x800CB886 + index * 2);
                revealed = r_s32(0x800CB8CC);
                ratio = r_u16(0x800CB8D0 + label * 2);
                flags = 0;
                if ((sint32)index >= revealed)
                    flags = r_u16(0x800CB882);
                y = (uint32)r_s16(0x800CB87A) + index * 72;
                top_ratio = r_u16(0x800CB8FC);
                t3_bns_06_800c13c4(index, label, ratio, top_ratio, flags, 0, y, 0);
                ++index;
            } while (index < r_u16(0x800CB884));
        }
    }
    packet = r_u32(0x800ADF64);
    rectangle.x = 0;
    rectangle.y = 96;
    rectangle.w = 368;
    rectangle.h = 360;
    packet = t3_8004db28(ordering, packet, &rectangle);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_06_800c2034(uint32 control, uint32 data)
{
    uint32 index;
    uint32 label;
    uint32 selected;
    uint32 blink;
    uint32 y;
    uint32 ordering;
    uint32 position;
    uint32 packet;
    uint32 row;
    uint32 xy;

    FUNCTION_MARKER(0x800C2034, "SLUS_004.02-BNS-logical-06");
    if (r_u16(control + 12)) {
        index = 0;
        do {
            label = r_u16(control + 14 + index * 2);
            selected = r_u16(control + 0x88);
            blink = 0;
            if (selected == index)
                blink = (r_u32(0x800AE218) & 3) != 0;
            y = (uint32)r_s16(control + 2) + index * 72;
            t3_bns_06_800c0d1c(index, label, data + 8 + label * 8, 0, y, blink);
            ++index;
        } while (index < r_u16(control + 12));
    }
    ordering = r_u32(0x800A9218) + 8;
    position = r_u16(0x800D310C);
    if (position >= 3)
        return 0x40000;
    packet = r_u32(0x800ADF64);
    row = r_u16(control + 0x86);
    xy = ((position * 13 + 0x129) & 0xFFFF) | ((row * 72 + 0xA0) << 16);
    packet = t3_8004da10(ordering, packet, 0x600000FF, xy, 0x00040012);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_06_800c2170(uint32 control, uint32 data)
{
    uint32 index;
    uint32 label;
    uint32 selected;
    uint32 blink;
    uint32 y;
    uint32 ordering;
    uint32 position;
    uint32 packet;
    uint32 row;
    uint32 xy;

    FUNCTION_MARKER(0x800C2170, "SLUS_004.02-BNS-logical-06");
    if (r_u16(control + 12)) {
        index = 0;
        do {
            label = r_u16(control + 14 + index * 2);
            selected = r_u16(control + 0x88);
            blink = 0;
            if (selected == index)
                blink = (r_u32(0x800AE218) & 3) != 0;
            y = (uint32)r_s16(control + 2) + index * 72;
            t3_bns_06_800c10b4(index, data + 0xB8 + label * 8, 0, y, blink);
            ++index;
        } while (index < r_u16(control + 12));
    }
    ordering = r_u32(0x800A9218) + 8;
    position = r_u16(0x800D310C);
    if (position >= 3)
        return 0x40000;
    packet = r_u32(0x800ADF64);
    row = r_u16(control + 0x86);
    xy = ((position * 13 + 0x129) & 0xFFFF) | ((row * 72 + 0xA0) << 16);
    packet = t3_8004da10(ordering, packet, 0x600000FF, xy, 0x00040012);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_06_800c22a8(uint32 mode)
{
    uint32 ordering;
    uint32 packet;
    uint32 index;
    uint32 label;
    uint32 ratio;
    uint32 top_ratio;
    uint32 flags;
    uint32 y;
    sint32 revealed;
    PSX_RECT rectangle;

    FUNCTION_MARKER(0x800C22A8, "SLUS_004.02-BNS-logical-06");
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    packet = t3_8004db28(ordering, packet, (const PSX_RECT *)psx_addr(0x800AE230, 8));
    w_u32(0x800ADF64, packet);
    if (mode == 0)
        t3_bns_06_800c2034(0x800CB878, 0x80097F44);
    else if (mode == 1)
        t3_bns_06_800c2170(0x800CB878, 0x80097F44);
    else if (mode == 2) {
        if (r_u16(0x800CB884)) {
            index = 0;
            do {
                label = r_u16(0x800CB886 + index * 2);
                revealed = r_s32(0x800CB8CC);
                ratio = r_u16(0x800CB8D0 + label * 2);
                flags = 0;
                if ((sint32)index >= revealed)
                    flags = r_u16(0x800CB882);
                y = (uint32)r_s16(0x800CB87A) + index * 72;
                top_ratio = r_u16(0x800CB8FC);
                t3_bns_06_800c13c4(index, label, ratio, top_ratio, flags, 0, y, 0);
                ++index;
            } while (index < r_u16(0x800CB884));
        }
    }
    packet = r_u32(0x800ADF64);
    rectangle.x = 0;
    rectangle.y = 96;
    rectangle.w = 368;
    rectangle.h = 368;
    packet = t3_8004db28(ordering, packet, &rectangle);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_06_800c0d1c(uint32 index, uint32 label, uint32 entry, uint32 x, uint32 y, uint32 blink)
{
    uint32 palette;
    uint32 color;
    uint32 suffix;
    uint32 number;
    uint32 ordering;
    uint32 packet;
    uint32 name;
    uint32 start;
    uint32 xy;
    sint32 frames;
    sint32 seconds;
    sint32 minutes;
    sint32 hundredths;

    FUNCTION_MARKER(0x800C0D1C, "SLUS_004.02-BNS-logical-06");
    if (y >= 552)
        return 0;
    palette = 0x800CB838 + blink * 5;
    color = r_u8(palette++);
    suffix = (uint32)((sint32)index % 10);
    if ((sint32)suffix >= 4 || index - 10 < 10)
        suffix = 3;
    number = index + 1;
    if ((sint32)number >= 101)
        number = 99;
    suffix = r_u32(0x800CB828 + suffix * 4);
    t3_8004ce74(0x800B8F20, 1, color, 1, x + 12, y - 34, number, suffix);
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    packet = t3_8004b928(ordering, packet, x + 76, y - 58, label << 2, 8);
    w_u32(0x800ADF64, packet);
    color = r_u8(palette++);
    name = t3_8004eff0(label, 0);
    name = r_u32(name);
    t3_8004ce74(0x800B8F30, 1, color, 0, x + 111, y - 56, name);
    color = r_u8(palette++);
    frames = r_s32(entry);
    if (frames > 359999)
        frames = 359999;
    seconds = frames / 60;
    hundredths = (frames % 60) * 100 / 60;
    minutes = seconds / 60;
    seconds %= 60;
    t3_8004ce74(0x800B8F40, 1, color, 1, y - 38, x + 182, x + 217);
    t3_8004ce74(0x800B8F50, 1, color, 1, y - 34, x + 158,
        (uint32)minutes, x + 191, (uint32)seconds, x + 227, (uint32)hundredths);
    color = r_u8(palette);
    t3_8004ce74(0x800B8F30, 1, color, 1, x + 299, y - 34, entry + 4);
    start = x + 8;
    xy = (start & 0xFFFF) | ((y - 14) << 16);
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    packet = t3_8004da10(ordering, packet, 0x60909090, xy, 0x0008003E);
    xy = ((x + 70) & 0xFFFF) | ((y - 8) << 16);
    packet = t3_8004da10(ordering, packet, 0x60909090, xy, ((306 - start) & 0xFFFF) | 0x00020000);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_06_800c10b4(uint32 index, uint32 entry, uint32 x, uint32 y, uint32 blink)
{
    uint32 wins;
    uint32 palette;
    uint32 color;
    uint32 suffix;
    uint32 number;
    uint32 label;
    uint32 ordering;
    uint32 packet;
    uint32 name;
    uint32 start;
    uint32 xy;

    FUNCTION_MARKER(0x800C10B4, "SLUS_004.02-BNS-logical-06");
    if (y >= 552)
        return 0;
    wins = r_u16(entry + 2);
    if (wins >= 1000)
        wins = 999;
    palette = 0x800CB844 + blink * 4;
    color = r_u8(palette++);
    suffix = (uint32)((sint32)index % 10);
    if ((sint32)suffix >= 4 || index - 10 < 10)
        suffix = 3;
    number = index + 1;
    if ((sint32)number >= 101)
        number = 99;
    suffix = r_u32(0x800CB828 + suffix * 4);
    t3_8004ce74(0x800B8F20, 1, color, 1, x + 12, y - 34, number, suffix);
    label = r_u16(entry);
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    packet = t3_8004b928(ordering, packet, x + 76, y - 58, label << 2, 8);
    w_u32(0x800ADF64, packet);
    color = r_u8(palette++);
    label = r_u16(entry);
    name = t3_8004eff0(label, 0);
    name = r_u32(name);
    t3_8004ce74(0x800B8F30, 1, color, 0, x + 111, y - 56, name);
    color = r_u8(palette++);
    if (wins < 2) {
        t3_8004ce74(0x800B8FBC, 1, color, 1, x + 171, y - 34, wins);
        t3_8004ce74(0x800B8FCC, 1, color, 1, x + 217, y - 34);
    } else {
        t3_8004ce74(0x800B8FBC, 1, color, 1, x + 158, y - 34, wins);
        t3_8004ce74(0x800B8FDC, 1, color, 1, x + 204, y - 34);
    }
    color = r_u8(palette);
    t3_8004ce74(0x800B8F30, 1, color, 1, x + 299, y - 34, entry + 4);
    start = x + 8;
    xy = (start & 0xFFFF) | ((y - 14) << 16);
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    packet = t3_8004da10(ordering, packet, 0x60909090, xy, 0x0008003E);
    xy = ((x + 70) & 0xFFFF) | ((y - 8) << 16);
    packet = t3_8004da10(ordering, packet, 0x60909090, xy, ((306 - start) & 0xFFFF) | 0x00020000);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_06_800c13c4(uint32 index, uint32 label, uint32 ratio, uint32 top_ratio, uint32 flags, uint32 x, uint32 y, uint32 blink)
{
    uint32 product;
    uint32 scaled;
    uint32 quotient;
    uint32 bar;
    uint32 display;
    uint32 palette;
    uint32 color;
    uint32 suffix;
    uint32 number;
    uint32 ordering;
    uint32 packet;
    uint32 name;
    uint32 tens;
    uint32 units;
    uint32 start;
    uint32 xy;

    FUNCTION_MARKER(0x800C13C4, "SLUS_004.02-BNS-logical-06");
    if (y >= 552)
        return 0;
    if (top_ratio) {
        product = ratio * flags;
        scaled = product * 1000;
        if (scaled == 0x80000000 && top_ratio == 0xFFFFFFFF)
            quotient = 0x80000000;
        else
            quotient = (uint32)((sint32)scaled / (sint32)top_ratio);
        display = (uint32)((sint32)product >> 6);
        bar = (uint32)((sint32)quotient >> 6);
    } else {
        bar = 0;
        display = 0;
    }
    palette = 0x800CB84C + blink * 3;
    color = r_u8(palette++);
    suffix = (uint32)((sint32)index % 10);
    if ((sint32)suffix >= 4 || index - 10 < 10)
        suffix = 3;
    number = index + 1;
    if ((sint32)number >= 101)
        number = 99;
    suffix = r_u32(0x800CB828 + suffix * 4);
    t3_8004ce74(0x800B8F20, 1, color, 1, x + 12, y - 34, number, suffix);
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    packet = t3_8004b928(ordering, packet, x + 76, y - 58, label << 2, 8);
    w_u32(0x800ADF64, packet);
    color = r_u8(palette++);
    name = t3_8004eff0(label, 0);
    name = r_u32(name);
    t3_8004ce74(0x800B8F30, 1, color, 0, x + 111, y - 56, name);
    color = r_u8(palette);
    if ((sint32)display > 1000)
        display = 1000;
    if (display == 1000 || display == 0) {
        t3_8004ce74(display ? 0x800B8F6C : 0x800B8F84, 1, color, 1,
            x + 283, y - 34, 0, x + 325, y - 30);
    } else {
        tens = (uint32)((sint32)display / 10);
        units = display - tens * 10;
        t3_8004ce74(0x800B8F9C, 1, color, 1, x + 281, y - 34,
            tens, 0, x + 308, y - 30, x + 315, units);
    }
    t3_bns_06_800c0b34(label, bar, x + 273, y - 34);
    start = x + 8;
    xy = (start & 0xFFFF) | ((y - 14) << 16);
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    packet = t3_8004da10(ordering, packet, 0x60909090, xy, 0x0008003E);
    xy = ((x + 70) & 0xFFFF) | ((y - 8) << 16);
    packet = t3_8004da10(ordering, packet, 0x60909090, xy, ((306 - start) & 0xFFFF) | 0x00020000);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_8004d7d4(uint32 ordering, uint32 packet, uint32 color, uint32 xy0, uint32 xy1, uint32 xy2, uint32 xy3)
{
    uint32 tag;

    FUNCTION_MARKER(0x8004D7D4, "SLUS_004.02");
    w_u32(packet + 4, color);
    w_u32(packet + 8, xy0);
    w_u32(packet + 12, xy1);
    w_u32(packet + 16, xy2);
    w_u32(packet + 20, xy3);
    tag = r_u32(ordering);
    w_u32(packet, (tag & 0xFFFFFF) | 0x05000000);
    tag = r_u32(ordering);
    w_u32(ordering, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    return packet + 24;
}

uint32 t3_bns_06_800c0b34(uint32 label, uint32 level, uint32 x, uint32 y)
{
    uint32 ordering;
    uint32 packet;
    uint32 table;
    uint32 width;
    uint32 left;
    uint32 left_top;
    uint32 right_top;
    uint32 left_bottom;
    uint32 right_bottom;
    uint32 outer_top;
    uint32 outer_bottom;
    uint32 first;
    uint32 second;
    uint32 product;

    FUNCTION_MARKER(0x800C0B34, "SLUS_004.02-BNS-logical-06");
    ordering = r_u32(0x800A9218) + 4;
    packet = r_u32(0x800ADF64);
    if ((sint32)level >= 1001)
        level = 1000;
    product = level * 129;
    width = (uint32)(sint32)(sint16)((sint32)product / 1000);
    if (!width)
        return 0;
    table = 0x800B8D58 + label * 20;
    first = r_u32(table + 4);
    second = r_u32(table + 8);
    left = x - width;
    left_top = (left & 0xFFFF) | (y << 16);
    right_top = (x & 0xFFFF) | (y << 16);
    left_bottom = (left & 0xFFFF) | ((y + 24) << 16);
    right_bottom = (x & 0xFFFF) | ((y + 24) << 16);
    packet = t3_8004d8b0(ordering, packet, first | 0x38000000, second, first, second,
        left_top, right_top, left_bottom, right_bottom);
    first = r_u32(table);
    outer_top = ((left - 6) & 0xFFFF) | ((y - 8) << 16);
    outer_bottom = ((left - 6) & 0xFFFF) | ((y + 16) << 16);
    packet = t3_8004d7d4(ordering, packet, first | 0x28000000,
        outer_top, left_top, outer_bottom, left_bottom);
    first = r_u32(table + 12);
    second = r_u32(table + 16);
    packet = t3_8004d8b0(ordering, packet, first | 0x38000000, second, first, second,
        outer_top, ((x - 8) & 0xFFFF) | ((y - 8) << 16), left_top, right_top);
    packet = t3_8004daa8(ordering, packet, 0);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_80053000(uint32 context, uint32 state, uint32 side)
{
    uint32 opposite = (side + 1) & 1;
    uint32 selection = state + 24 + side * 172;
    uint32 other = state + 24 + opposite * 172;
    uint32 complete = 0;
    uint32 mode;
    uint32 phase;
    uint32 value;
    uint32 code;
    uint32 table;
    uint32 column;
    uint32 row;
    uint32 held;
    uint32 buttons;
    uint32 delta;
    uint32 candidate;
    uint32 count;
    uint32 slot;
    uint32 mask;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    FUNCTION_MARKER(0x80053000, "SLUS_004.02");
    phase = r_u32(selection);
    switch (phase) {
    case 0:
        t3_8004ccf4(selection + 120, 0x80098160 + side * 52, 52);
        value = r_u32(0x80097EF0);
        w_u32(selection + 36, value);
        mode = r_u32(state);
        switch (mode) {
        case 0:
        case 6:
            value = t3_80051114(side);
            if (value == 2)
                w_u32(selection, 3);
            else if (value == 3)
                w_u32(selection, 11);
            else if ((sint32)value >= 0 && (sint32)value < 3)
                w_u32(selection, 8);
            goto initialize_single;
        case 1:
            w_u32(selection, 3);
            goto initialize_single;
        case 2:
            value = r_u16(0x800AE1F8 + side * 2);
            w_u32(selection, value ? 1 : 9);
            w_u32(selection + 44, 0);
            w_u32(selection + 40, 4);
            w_u32(selection + 48, 0);
            w_u32(selection + 52, 0);
            count = 8;
            do {
                --count;
                w_u32(selection + 56 + count * 4, 88);
            } while (count);
            code = r_u32(selection + 56);
            value = r_u8(context + side * 13 + 101);
            w_u32(selection + 40, value);
            goto find_initial_character;
        case 3:
        case 4:
        case 8:
            value = r_u16(0x800AE1F8 + side * 2);
            w_u32(selection, value ? 3 : 10);
            goto initialize_single;
        case 5:
            value = r_u16(0x800AE1F8 + side * 2);
            w_u32(selection, value ? 3 : 6);
            goto initialize_single;
        case 7:
            value = t3_80051114(side);
            if (value == 2)
                w_u32(selection, 3);
            else if (value == 3)
                w_u32(selection, 14);
            else if ((sint32)value >= 0 && (sint32)value < 3)
                w_u32(selection, 8);
            goto initialize_single;
        default:
            goto publish_selection;
        }
initialize_single:
        w_u32(selection + 44, 0);
        w_u32(selection + 40, 1);
        w_u32(selection + 48, 1);
        if (r_u16(0x800ADFBC + side * 2)) {
            w_u32(selection + 44, 1);
            first = r_u16(0x800ADD5C + side * 2);
            second = r_u16(0x800ADD98 + side * 2);
            code = first * 4 + second;
            w_u32(selection + 56, code);
        } else {
            code = r_u32(selection + 100);
            w_u32(selection + 56, 88);
        }
find_initial_character:
        if ((sint32)code < 88)
            code &= 0xFFFFFFFC;
        else
            code = r_u32(selection + 132);
        table = 0x80022768;
        for (row = 0; row < 3; ++row) {
            for (column = 0; column < 7; ++column) {
                if (code == r_u8(table + 4)) {
                    w_u32(selection + 8, column);
                    w_u32(selection + 12, row);
                    goto initial_character_found;
                }
                table += 6;
            }
        }
initial_character_found:
        if (mode == 2) {
            first = r_u16(selection + 168);
            second = r_u16(selection + 170);
            w_u32(selection + 16, first);
            w_u32(selection + 20, second);
        } else if (mode == 1) {
            first = r_u16(selection + 144);
            second = r_u16(selection + 146);
            third = r_u16(selection + 148);
            w_u32(selection + 16, first);
            w_u32(selection + 20, second);
            fourth = r_u16(context + side * 4 + 58);
            first = r_u16(selection + 150);
            second = r_u16(selection + 156);
            w_u32(selection + 16, third);
            w_u32(selection + 20, first);
            w_u32(selection + 92, second);
            w_u32(selection + 96, second);
            w_u32(selection + 88, fourth);
        } else if (mode == 7) {
            first = r_u16(selection + 144);
            second = r_u16(selection + 146);
            third = r_u16(selection + 156);
            w_u32(selection + 16, first);
            w_u32(selection + 20, second);
            fourth = r_u16(context + side * 4 + 58);
            first = r_u16(selection + 148);
            second = r_u16(selection + 150);
            w_u32(selection + 92, third);
            w_u32(selection + 96, third);
            w_u32(selection + 16, first);
            w_u32(selection + 20, second);
            w_u32(selection + 88, fourth);
            t3_bns_03_800b5788(context, selection, side);
        } else {
            first = r_u16(selection + 144);
            second = r_u16(selection + 146);
            w_u32(selection + 16, first);
            w_u32(selection + 20, second);
        }
        break;
    case 1:
        held = r_u16(0x800ADF00 + side * 2);
        value = r_u32(selection + 40);
        delta = ((held >> 13) & 1) - (held >> 15);
        candidate = value + delta;
        if (delta && candidate < 9)
            value = candidate;
        if (value >= 9)
            value = 8;
        if (!value)
            value = 1;
        if (r_u32(selection + 40) != value)
            t3_800753bc(0, 0x55F5, 0);
        w_u32(selection + 40, value);
        buttons = r_u16(0x800ADF08 + side * 2);
        if (buttons & 0x8F0) {
            w_u32(selection + 52, (buttons & 0x60) != 0);
            t3_800753bc(0, 0x50F4, 0);
            w_u32(selection, 2);
        }
        break;
    case 2:
    case 3:
        column = r_u32(selection + 8);
        held = r_u16(0x800ADF00 + side * 2);
        row = r_u32(selection + 12);
        if (column >= 7)
            column = 0;
        if (row >= 3)
            row = 0;
        delta = ((held >> 13) & 1) - (held >> 15);
        candidate = column + delta;
        if (delta)
            column = candidate < 7 ? candidate : 6 - column;
        else {
            candidate = row + ((held >> 14) & 1) - ((held >> 12) & 1);
            if (candidate < 3)
                row = candidate;
        }
        if (r_u32(selection + 8) != column || r_u32(selection + 12) != row)
            t3_800753bc(0, 0x55F5, 0);
        phase = r_u32(selection);
        w_u32(selection + 8, column);
        w_u32(selection + 12, row);
        if (phase == 2) {
            buttons = r_u16(0x800ADF08 + side * 2);
            if (buttons & 0x800) {
                t3_800753bc(0, 0x50F4, 0);
                goto character_confirmed;
            }
            if (buttons & 0x100) {
                count = r_u32(selection + 44);
                if ((sint32)count <= 0) {
                    w_u32(selection, 1);
                    break;
                }
                --count;
                slot = selection + 56 + count * 4;
                code = r_u32(slot);
                w_u32(selection + 100, code);
                w_u32(slot, 88);
                mask = r_u32(selection + 36);
                w_u32(selection + 44, count);
                w_u32(selection + 36, mask | ((uint32)1 << (((sint32)code >> 2) & 31)));
                t3_800753bc(0, 0x50F4, 0);
            }
        }
        buttons = r_u16(0x800ADF08 + side * 2);
        if (t3_80052de4(state, selection, buttons))
            break;
character_confirmed:
        switch (r_u32(state)) {
        case 0:
        case 3:
        case 4:
        case 5:
        case 6:
        case 8:
            w_u32(selection, 11);
            break;
        case 1:
            w_u32(selection, 4);
            break;
        case 2:
            w_u32(selection, 10);
            break;
        case 7:
            if (r_u8(0x800AFACB))
                w_u32(selection, 14);
            else {
                w_u8(0x800AFACB, (uint32)1 << (side & 31));
                w_u8(0x800AFAC8, side);
                w_u32(selection, 15);
            }
            break;
        default:
            break;
        }
        break;
    case 4:
        value = r_u16(selection + 154);
        w_u32(selection + 92, value);
        held = r_u16(0x800ADF00 + side * 2);
        value = r_u32(selection + 88);
        delta = ((held >> 13) & 1) - (held >> 15);
        if (r_u32(0x800AFA88) == 7)
            delta <<= 1;
        if (delta) {
            if (!r_u16(selection + 152))
                delta = 0 - delta;
            candidate = value + delta;
            if (candidate < 8)
                value = candidate;
        }
        if (r_u32(selection + 88) != value)
            t3_800753bc(0, 0x55F5, 0);
        w_u32(selection + 88, value);
        buttons = r_u16(0x800ADF08 + side * 2);
        if (buttons & 0x100) {
            value = r_u16(selection + 156);
            count = r_u32(selection + 44);
            w_u32(selection + 92, value);
            if ((sint32)count > 0) {
                --count;
                slot = selection + 56 + count * 4;
                code = r_u32(slot);
                w_u32(selection + 100, code);
                w_u32(slot, 88);
                mask = r_u32(selection + 36);
                w_u32(selection + 44, count);
                w_u32(selection + 36, mask | ((uint32)1 << (((sint32)code >> 2) & 31)));
                t3_800753bc(0, 0x50F4, 0);
            }
            w_u32(selection, 3);
        } else if (buttons & 0xF0) {
            t3_800753bc(0, 0x50F4, 0);
            w_u32(selection, 12);
        }
        break;
    case 6:
        if (r_u32(other) == 11) {
            value = r_u32(other + 8);
            w_u32(selection + 8, value);
            value = r_u32(other + 12);
            w_u32(selection, 7);
            w_u32(selection + 12, value);
        }
        break;
    case 7:
        column = r_u32(selection + 8);
        held = r_u16(0x800ADF00 + opposite * 2);
        row = r_u32(selection + 12);
        if (column >= 7)
            column = 0;
        if (row >= 3)
            row = 0;
        delta = ((held >> 13) & 1) - (held >> 15);
        candidate = column + delta;
        if (delta)
            column = candidate < 7 ? candidate : 6 - column;
        else {
            candidate = row + ((held >> 14) & 1) - ((held >> 12) & 1);
            if (candidate < 3)
                row = candidate;
        }
        if (r_u32(selection + 8) != column || r_u32(selection + 12) != row)
            t3_800753bc(0, 0x55F5, 0);
        w_u32(selection + 8, column);
        w_u32(selection + 12, row);
        buttons = r_u16(0x800ADF08 + opposite * 2);
        if (buttons & 0x100) {
            count = r_u32(other + 44);
            if ((sint32)count > 0) {
                --count;
                slot = other + 56 + count * 4;
                code = r_u32(slot);
                w_u32(other + 100, code);
                w_u32(slot, 88);
                mask = r_u32(other + 36);
                w_u32(other + 44, count);
                w_u32(other + 36, mask | ((uint32)1 << (((sint32)code >> 2) & 31)));
                t3_800753bc(0, 0x50F4, 0);
            }
            w_u32(selection, 6);
            value = r_u8(0x8002282D);
            w_u32(other, 3);
            w_u32(other + 24, value);
            value = r_u8(0x8002282E);
            w_u32(other + 28, value);
            value = r_u8(0x8002282F);
            w_u32(other + 32, value);
        } else {
            buttons = r_u16(0x800ADF08 + opposite * 2);
            if (!t3_80052de4(state, selection, buttons))
                w_u32(selection, 13);
        }
        break;
    case 9:
        value = r_u32(other + 40);
        w_u32(selection + 40, value);
        goto check_join;
    case 8:
check_join:
        if (r_u8(0x800AFA8D) && t3_80050f5c(side)) {
            w_u32(selection, 0);
            t3_800753bc(0, 0x49A1, 0);
        } else
            complete = 1;
        break;
    case 10:
    case 11:
    case 12:
    case 13:
        complete = 1;
        break;
    case 14:
        if (r_u16(0x800ADF08 + side * 2) & 0x100) {
            count = r_u32(selection + 44);
            if ((sint32)count > 0) {
                --count;
                slot = selection + 56 + count * 4;
                code = r_u32(slot);
                w_u32(selection + 100, code);
                w_u32(slot, 88);
                mask = r_u32(selection + 36);
                w_u32(selection + 44, count);
                w_u32(selection + 36, mask | ((uint32)1 << (((sint32)code >> 2) & 31)));
                t3_800753bc(0, 0x50F4, 0);
            }
            w_u32(selection, 3);
            value = r_u8(0x800AFACB);
            if (value == ((uint32)1 << (side & 31)))
                w_u8(0x800AFACB, 0);
        } else
            complete = 1;
        break;
    case 15:
        value = t3_bns_03_800b51d8();
        if (value == 1)
            w_u32(selection, 14);
        else if (value == 0xFFFFFFFF) {
            count = r_u32(selection + 44);
            if ((sint32)count > 0) {
                --count;
                slot = selection + 56 + count * 4;
                code = r_u32(slot);
                w_u32(selection + 100, code);
                w_u32(slot, 88);
                mask = r_u32(selection + 36);
                w_u32(selection + 44, count);
                w_u32(selection + 36, mask | ((uint32)1 << (((sint32)code >> 2) & 31)));
                t3_800753bc(0, 0x50F4, 0);
            }
            w_u32(selection, 3);
            w_u8(0x800AFACB, 0);
        }
        break;
    default:
        break;
    }
publish_selection:
    table = 0x80022824 + r_u32(selection) * 3;
    value = r_u8(table);
    w_u32(selection + 24, value);
    value = r_u8(table + 1);
    w_u32(selection + 28, value);
    value = r_u8(table + 2);
    w_u32(selection + 32, value);
    return complete;
}

uint32 t3_bns_03_800b5788(uint32 context, uint32 selection, uint32 side)
{
    uint32 x;
    uint32 y;
    uint32 extent;
    uint32 offset;

    FUNCTION_MARKER(0x800B5788, "SLUS_004.02-BNS-logical-03");
    (void)context;
    x = r_u16(selection + 148);
    y = r_u16(selection + 150);
    extent = r_u16(selection + 156);
    w_u32(selection + 88, 6);
    offset = r_u16(selection + 154);
    w_u32(selection + 16, x);
    w_u32(selection + 20, y);
    w_u32(selection + 92, extent);
    w_u32(selection + 96, extent);
    offset += side ? 40 : (uint32)-40;
    w_u16(selection + 154, offset);
    return offset;
}

uint32 t3_8004d71c(uint32 ordering, uint32 packet, uint32 color, uint32 xy0, uint32 xy1, uint32 xy2)
{
    uint32 tag;

    FUNCTION_MARKER(0x8004D71C, "SLUS_004.02");
    w_u32(packet + 4, color);
    w_u32(packet + 8, xy0);
    w_u32(packet + 12, xy1);
    w_u32(packet + 16, xy2);
    tag = r_u32(ordering);
    w_u32(packet, (tag & 0xFFFFFF) | 0x04000000);
    tag = r_u32(ordering);
    w_u32(ordering, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    return packet + 20;
}

uint32 t3_bns_03_800b51d8(void)
{
    uint32 side;
    uint32 owner;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 target;
    uint32 current;
    uint32 value;
    uint32 difference;
    uint32 input;
    uint32 result = 0;
    uint32 ordering;
    uint32 packet;

    FUNCTION_MARKER(0x800B51D8, "SLUS_004.02-BNS-logical-03");
    side = r_u8(0x800AFAC8);
    owner = t3_bns_03_800b07a8();
    w_u8(0x800AFACB, 3);
    first = r_u16(owner + 152);
    second = r_u16(owner + 154);
    target = r_u32(owner + 120);
    w_u16(owner + 154, second + 8);
    third = r_u16(owner + 156);
    w_u16(owner + 152, first + 1);
    current = r_u32(owner + 104);
    difference = target - current;
    value = current + (uint32)((sint32)difference >> 3);
    w_u32(owner + 104, value);
    w_u16(owner + 156, third + 2);
    if ((sint32)value < -2047 && r_s16(owner + 176) == -1) {
        value = r_u8(0x800AFAC9) + 1;
        w_u8(0x800AFAC9, value);
        if ((value & 0xFF) >= 3)
            w_u8(0x800AFAC9, 0);
        w_u32(owner + 104, 2048);
        w_u32(owner + 120, 0);
    }
    if (r_s32(owner + 104) >= 2048 && r_s16(owner + 176) == 1) {
        value = r_u8(0x800AFAC9);
        value = value ? value - 1 : 2;
        w_u8(0x800AFAC9, value);
        w_u32(owner + 104, (uint32)-2048);
        w_u32(owner + 120, 0);
    }
    input = 0x800ADF08 + side * 2;
    if (r_u16(input) & 0x8000) {
        w_u32(owner + 120, (uint32)-4096);
        w_u16(owner + 176, 0xFFFF);
        t3_800753bc(0, 0x55F5, 0);
    }
    if (r_u16(input) & 0x2000) {
        w_u32(owner + 120, 4096);
        w_u16(owner + 176, 1);
        t3_800753bc(0, 0x55F5, 0);
    }
    current = r_u32(owner + 104);
    target = r_u32(owner + 120);
    difference = current - target;
    if ((sint32)difference < 0) {
        if ((sint32)(target - current) >= 64)
            return 0;
    } else if ((sint32)difference >= 64)
        return 0;
    if (r_u16(input) & 0x100)
        result = 0xFFFFFFFF;
    if (r_u32(0x800AE218) & 0x20) {
        ordering = r_u32(0x800A9218);
        packet = r_u32(0x800ADF64);
        packet = t3_8004d71c(ordering, packet, 0x2000FFFF, 0x014E00FC, 0x015E00FC, 0x01560104);
        ordering = r_u32(0x800A9218);
        w_u32(0x800ADF64, packet);
        packet = t3_8004d71c(ordering, packet, 0x2000FFFF, 0x014E0074, 0x015E0074, 0x0156006C);
        w_u32(0x800ADF64, packet);
    }
    if (r_u16(input) & 0xF0) {
        t3_800753bc(0, 0x50F4, 0);
        result = 1;
    }
    return result;
}
