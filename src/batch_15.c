#include "game_functions.h"

uint32 t3_800b0954(void)
{
    FUNCTION_MARKER(0x800B0954, "SLUS_004.02");
    VSync(0);
    t3_8007aed8(0);
    t3_8007af44();
    t3_80079e08();
    t3_800b09a0();
    w_u8(0x800ADF60, 0);
    w_u8(0x800ADF61, 0);
    return 0x800ADF40;
}

void t3_800b09a0(void)
{
    static const uint32 specifications[4] = {4, 0x8000, 0x100, 0x2000};
    uint32 index;
    uint32 handle;
    uint32 first_handle;
    FUNCTION_MARKER(0x800B09A0, "SLUS_004.02");
    EnterCriticalSectionPSX();
    for (index = 0; index < 7; ++index) {
        handle = OpenEventGuest(index < 4 ? 0xF4000001 : 0xF0000011, specifications[index & 3], 0x2000, 0);
        w_u32(0x800ADF40 + index * 4, handle);
    }
    handle = OpenEventGuest(0xF0000011, 0x2000, 0x2000, 0);
    first_handle = r_u32(0x800ADF40);
    w_u32(0x800ADF5C, handle);
    EnableEvent(first_handle);
    for (index = 1; index < 8; ++index)
        EnableEvent(r_u32(0x800ADF40 + index * 4));
    ExitCriticalSection();
}

void t3_800b094c(uint32 actor, uint32 opponent)
{
    FUNCTION_MARKER(0x800B094C, "SLUS_004.02");
    (void)actor;
    (void)opponent;
}

uint32 t3_800b0840(uint32 configuration)
{
    uint32 table = 0x800B0CC8 + configuration * 16;
    uint32 width;
    uint32 height;
    uint32 mode;
    uint32 first;
    uint32 second;
    uint32 x;
    uint32 y;
    FUNCTION_MARKER(0x800B0840, "SLUS_004.02");
    width = r_u16(table + 8);
    height = r_u16(table + 10);
    mode = r_u16(table + 12);
    GsInitGraph(width, height, mode, 1, 0);
    height = r_u16(table + 14);
    GsDefDispBuff(0, 0, 0, height);
    GsInit3D();
    t3_80080f38(table);
    width = r_u16(table + 8);
    height = r_u16(table + 10);
    SetGeomOffset((sint16)width >> 1, (sint16)height >> 1);
    t3_80081068(0, 0);
    w_u16(0x800ADE14, 258);
    x = r_u8(0x80097F24);
    w_u16(0x800ADE16, 240);
    y = r_u8(0x80097F25);
    w_u16(0x800ADE10, (sint8)x);
    w_u16(0x800ADE12, (sint8)y);
    first = r_u32(table);
    first = r_u32(table);
    second = r_u32(table + 4);
    second = r_u32(table + 4);
    w_u32(0x800AE230, first);
    w_u32(0x800AE230, first);
    w_u32(0x800AE234, second);
    w_u32(0x800AE234, second);
    SetGeomScreen(500);
    t3_80081088(1000);
    t3_80081098(10000);
    return 0x800B0000;
}

uint32 t3_800b0b9c(void)
{
    uint32 side;
    uint32 record;
    uint32 port;
    sint32 index;
    FUNCTION_MARKER(0x800B0B9C, "SLUS_004.02");
    w_u8(0x800A9184, 0);
    for (side = 0; side < 2; ++side) {
        record = 0x800A9130 + side * 42;
        port = r_u8(0x800B0CE8 + side);
        w_u8(record + 40, 0);
        w_u16(record, 0);
        w_u8(record + 38, 0);
        w_u8(record + 41, 0);
        w_u8(record + 39, port);
        for (index = 35; index >= 2; --index)
            w_u8(record + index, 0);
        w_u8(record + 37, 0);
        w_u8(record + 36, 0);
    }
    PadInitDirectPSX(0x800A9132, 0x800A915C);
    port = r_u8(0x800A9157);
    PadSetAct((sint32)port, (uint8 *)psx_addr(0x800A9154, 2), 2);
    port = r_u8(0x800A9181);
    PadSetAct((sint32)port, (uint8 *)psx_addr(0x800A917E, 2), 2);
    PadStartComPSX();
    w_u16(0x800ADD6A, 0);
    w_u16(0x800ADD68, 0);
    w_u16(0x800ADF0A, 0);
    w_u16(0x800ADF08, 0);
    w_u16(0x800ADF02, 0);
    w_u16(0x800ADF00, 0);
    w_u16(0x800A8C4A, 0);
    w_u16(0x800A8C48, 0);
    w_u16(0x800ADC7A, 0);
    w_u16(0x800ADC78, 0);
    return 0x800ADC78;
}

uint32 t3_bns_03_800b4484(void)
{
    FUNCTION_MARKER(0x800B4484, "SLUS_004.02-BNS-logical-03");
    return r_u32(0x800ADD74) + 104;
}

uint32 t3_bns_03_800b07a8(void)
{
    FUNCTION_MARKER(0x800B07A8, "SLUS_004.02-BNS-logical-03");
    return r_u32(0x800ADD74);
}

uint32 t3_bns_02_800b297c(void)
{
    FUNCTION_MARKER(0x800B297C, "SLUS_004.02-BNS-logical-02");
    return r_u32(0x800B65C8);
}

uint32 t3_camera_divide(uint32 numerator, uint32 denominator)
{
    if (!denominator)
        return (sint32)numerator < 0 ? 1 : 0xFFFFFFFF;
    return (uint32)((sint64)(sint32)numerator / (sint32)denominator);
}

uint32 t3_bns_03_800b4494(void)
{
    uint32 third;
    uint32 left_z;
    uint32 right_z;
    uint32 left_x;
    uint32 right_x;
    uint32 third_z;
    uint32 third_x;
    uint32 delta_x;
    uint32 delta_z;
    uint32 angle;
    uint32 cosine;
    uint32 sine;
    uint32 offset;
    uint32 first;
    uint32 second;
    uint32 third_forward;
    uint32 third_side;
    FUNCTION_MARKER(0x800B4494, "SLUS_004.02-BNS-logical-03");
    third = t3_bns_03_800b4484();
    left_z = r_u32(0x800AA198);
    right_z = r_u32(0x800ABA24);
    left_x = r_u32(0x800AA190);
    right_x = r_u32(0x800ABA1C);
    third_z = r_u32(third + 8);
    third_x = r_u32(third);
    delta_x = right_x - left_x;
    delta_z = right_z - left_z;
    angle = t3_8004b39c(delta_x, delta_z);
    offset = t3_camera_divide(delta_z * left_x, delta_x);
    angle = (0 - (uint32)(sint16)angle) & 0xFFF;
    cosine = r_s16(0x8001F0C4 + angle * 2);
    sine = r_s16(0x8001E8C4 + angle * 2);
    offset = left_z - offset;
    left_z -= offset;
    right_z -= offset;
    third_z -= offset;
    first = t3_camera_divide(left_x * cosine - left_z * sine, 4096);
    second = t3_camera_divide(right_x * cosine - right_z * sine, 4096);
    third_forward = t3_camera_divide(third_x * cosine - third_z * sine, 4096);
    third_side = t3_camera_divide(third_x * sine + third_z * cosine, 4096);
    w_u32(0x800B6628, first);
    w_u32(0x800B662C, second);
    w_u32(0x800B6630, third_forward);
    w_u32(0x800B6638, third_side);
    w_u32(0x800B6634, r_u32(third + 4));
    return 0x800B6628;
}

uint32 t3_bns_03_800b4644(uint32 actor)
{
    uint32 side;
    uint32 forward;
    uint32 difference;
    uint32 lateral;
    FUNCTION_MARKER(0x800B4644, "SLUS_004.02-BNS-logical-03");
    side = r_s16(actor + 0x12);
    forward = r_u32(0x800B6630);
    difference = r_u32(0x800B6628 + side * 4) - forward;
    lateral = r_u32(0x800B6638);
    return t3_8002a324(difference * difference + lateral * lateral);
}

uint32 t3_bns_03_800b46a0(uint32 state)
{
    uint32 side;
    uint32 third;
    uint32 boundary;
    uint32 outside;
    FUNCTION_MARKER(0x800B46A0, "SLUS_004.02-BNS-logical-03");
    side = r_u16(state);
    third = r_u32(0x800B6630);
    boundary = r_u32(side ? 0x800B662C : 0x800B6628);
    outside = side ? (sint32)boundary < (sint32)third : (sint32)third < (sint32)boundary;
    if (!outside)
        return 0;
    t3_800593a4(state);
    return 1;
}

uint32 t3_bns_03_800b4720(uint32 state)
{
    uint32 boundary;
    uint32 third;
    FUNCTION_MARKER(0x800B4720, "SLUS_004.02-BNS-logical-03");
    if (!r_u16(state)) {
        boundary = r_u32(0x800B6628);
        third = r_u32(0x800B6630);
        if ((sint32)boundary >= (sint32)third)
            return 0;
        return (sint32)(boundary + 2000) < (sint32)third;
    }
    boundary = r_u32(0x800B662C);
    third = r_u32(0x800B6630);
    if ((sint32)third >= (sint32)boundary)
        return 0;
    return (sint32)(boundary - 2000) < (sint32)third;
}

static void t3_camera_transform(uint32 x, uint32 y, uint32 z, uint32 cosine_yaw, uint32 sine_yaw, uint32 cosine_pitch, uint32 sine_pitch, uint32 *output)
{
    uint32 depth = t3_camera_divide(sine_yaw * x + cosine_yaw * z, 4096);
    output[0] = t3_camera_divide(cosine_yaw * x - sine_yaw * z, 4096);
    output[2] = t3_camera_divide(cosine_pitch * depth - sine_pitch * y, 4096);
    output[1] = t3_camera_divide(sine_pitch * depth + cosine_pitch * y, 4096);
}

uint32 t3_80064e10(uint32 first, uint32 second, uint32 pitch, uint32 yaw, uint32 reset, uint32 distance, uint32 camera_index)
{
    uint32 camera = 0x800A02A8 + camera_index * 24;
    uint32 state = 0x800A0328 + camera_index * 48;
    uint32 focal;
    uint32 saved[6];
    uint32 target[3];
    uint32 projected[2][3];
    uint32 depths[2];
    uint32 third_position[4];
    uint32 third_projected[3];
    uint32 adjustment[4] = {0, 0, 0, 0};
    uint32 index;
    uint32 angle;
    uint32 sine_pitch;
    uint32 cosine_pitch;
    uint32 sine_yaw;
    uint32 cosine_yaw;
    uint32 center_x;
    uint32 center_z;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 value;
    uint32 left;
    uint32 right;
    uint32 screen_left;
    uint32 screen_right;
    uint32 upper;
    uint32 lower;
    uint32 near_edge;
    uint32 far_edge;
    uint32 shift;
    uint32 smoothing;
    uint32 vertical;
    uint32 third;
    uint32 height;
    uint32 depth;
    uint32 ground_y;
    uint32 ground_depth;
    uint32 view_distance;
    uint32 limit;
    uint32 elevation;
    uint32 denominator;
    uint32 delta;
    uint32 velocity;
    uint32 final_x;
    uint32 final_y;
    uint32 final_z;
    uint32 delta_pitch;
    uint32 delta_yaw;
    FUNCTION_MARKER(0x80064E10, "SLUS_004.02");
    (void)first;
    (void)second;
    focal = (uint32)(sint32)r_s16(0x800A8CD4);
    if ((sint32)distance < 801)
        distance = 800;
    for (index = 0; index < 6; ++index)
        saved[index] = r_u32(camera + index * 4);
    angle = 0 - pitch;
    if ((sint32)angle < 0)
        angle += 63;
    angle = (uint32)((sint32)angle >> 5) & 0x1FFE;
    sine_pitch = (uint32)(sint32)r_s16(0x8001E8C4 + angle);
    cosine_pitch = (uint32)(sint32)r_s16(0x8001F0C4 + angle);
    angle = 0 - yaw;
    if ((sint32)angle < 0)
        angle += 63;
    angle = (uint32)((sint32)angle >> 5) & 0x1FFE;
    x = r_u32(0x800ADCA8);
    z = r_u32(0x800ADCB8);
    sine_yaw = (uint32)(sint32)r_s16(0x8001E8C4 + angle);
    cosine_yaw = (uint32)(sint32)r_s16(0x8001F0C4 + angle);
    center_x = t3_camera_divide(x + z, 2);
    x = r_u32(0x800ADCB0);
    z = r_u32(0x800ADCC0);
    center_z = t3_camera_divide(x + z, 2);
    if (reset) {
        angle = yaw + 196608;
        if ((sint32)angle < 0)
            angle += 63;
        angle = (uint32)((sint32)angle >> 5) & 0x1FFE;
        z = (uint32)(sint32)r_s16(0x8001F0C4 + angle);
        x = (uint32)(sint32)r_s16(0x8001E8C4 + angle);
        target[0] = center_x + t3_camera_divide(distance * z, 4096);
        target[2] = center_z + t3_camera_divide(distance * x, 4096);
    } else {
        angle = (yaw - saved[4]) & 0x3FFFF;
        if (angle & 0x20000)
            angle |= 0xFFFC0000;
        if ((sint32)angle < 0)
            angle += 63;
        angle = (uint32)((sint32)angle >> 5) & 0x1FFE;
        z = (uint32)(sint32)r_s16(0x8001F0C4 + angle);
        x = (uint32)(sint32)r_s16(0x8001E8C4 + angle);
        y = saved[0] - center_x;
        value = saved[2] - center_z;
        target[0] = center_x + t3_camera_divide(z * y - x * value, 4096);
        target[2] = center_z + t3_camera_divide(x * y + z * value, 4096);
    }
    target[1] = saved[1];
    for (index = 0; index < 2; ++index) {
        uint32 actor = 0x800ADCA8 + index * 16;
        x = r_u32(actor) - target[0];
        z = r_u32(actor + 8) - target[2];
        y = r_u32(actor + 4) - target[1];
        depths[index] = t3_camera_divide(sine_yaw * x + cosine_yaw * z, 4096);
        t3_camera_transform(x, y, z, cosine_yaw, sine_yaw, cosine_pitch, sine_pitch, projected[index]);
    }
    if (r_u32(0x800AFA88) == 7) {
        third = t3_bns_03_800b4484();
        for (index = 0; index < 4; ++index)
            third_position[index] = r_u32(third + index * 4);
        t3_camera_transform(third_position[0] - target[0], third_position[1] - target[1], third_position[2] - target[2], cosine_yaw, sine_yaw, cosine_pitch, sine_pitch, third_projected);
    }
    value = t3_camera_divide(depths[0] + depths[1], 2);
    ground_depth = t3_camera_divide(cosine_pitch * value - sine_pitch * (0 - 450 - target[1]), 4096);
    view_distance = distance;
    shift = distance - ground_depth;
    adjustment[2] -= shift;
    projected[1][2] += shift;
    projected[0][2] += shift;
    ground_y = t3_camera_divide(sine_pitch * value + cosine_pitch * (0 - 450 - target[1]), 4096);
    if (r_u32(0x800AFA88) == 7) {
        if ((sint32)projected[0][0] >= (sint32)projected[1][0]) {
            if ((sint32)third_projected[0] < (sint32)projected[1][0]) {
                projected[1][0] = third_projected[0] - 150;
                projected[1][2] = third_projected[2];
            } else if ((sint32)projected[0][0] < (sint32)third_projected[0]) {
                projected[0][0] = third_projected[0] + 150;
                projected[0][2] = third_projected[2];
            }
        } else if ((sint32)third_projected[0] < (sint32)projected[0][0]) {
            projected[0][0] = third_projected[0] - 150;
            projected[0][2] = third_projected[2];
        } else if ((sint32)projected[1][0] < (sint32)third_projected[0]) {
            projected[1][0] = third_projected[0] + 150;
            projected[1][2] = third_projected[2];
        }
    }
    if ((sint32)projected[0][0] >= (sint32)projected[1][0]) {
        left = 1;
        right = 0;
    } else {
        left = 0;
        right = 1;
    }
    screen_left = t3_camera_divide((projected[left][0] - 150) * focal, projected[left][2]);
    screen_right = t3_camera_divide((projected[right][0] + 150) * focal, projected[right][2]);
    projected[left][0] -= 150;
    projected[right][0] += 150;
    if ((sint32)(screen_right - screen_left) < 281) {
        near_edge = t3_camera_divide(48 * projected[right][2], focal);
        far_edge = t3_camera_divide((0 - 48) * projected[left][2], focal);
        if (reset || (sint32)(near_edge - (projected[right][0] - projected[left][0])) < (sint32)far_edge) {
            value = t3_camera_divide((projected[right][0] - projected[left][0]) * projected[left][2], projected[left][2] + projected[right][2]);
            adjustment[0] = t3_camera_divide(projected[left][0] + value, 8);
        } else if ((sint32)screen_left < -48) {
            adjustment[0] += t3_camera_divide(projected[left][0] - far_edge, 8);
        } else if ((sint32)screen_right >= 49) {
            adjustment[0] += t3_camera_divide(projected[right][0] - near_edge, 8);
        }
        if (reset) {
            w_u32(state + 44, 0);
        } else {
            value = r_u32(state + 44);
            smoothing = 0;
            if ((sint32)value < 0)
                smoothing = value - t3_camera_divide(value, 32);
            else if ((sint32)value > 0)
                smoothing = value - t3_camera_divide(value, 8);
            w_u32(state + 44, smoothing);
            adjustment[2] += smoothing;
            view_distance -= smoothing;
            projected[0][2] -= smoothing;
            projected[1][2] -= smoothing;
        }
    } else {
        value = focal * (projected[right][0] - projected[left][0]);
        depth = t3_camera_divide(value - 140 * (projected[right][2] - projected[left][2]), 280);
        shift = projected[left][2] - depth;
        if (!reset) {
            value = r_u32(state + 44);
            if ((sint32)value >= (sint32)shift)
                shift = value - t3_camera_divide(value - shift, 8);
            else
                shift = value + t3_camera_divide(shift - value, 32);
        }
        w_u32(state + 44, shift);
        adjustment[2] += shift;
        adjustment[0] = t3_camera_divide(projected[left][0] - t3_camera_divide((0 - 140) * depth, focal), 8);
        projected[0][2] -= shift;
        projected[1][2] -= shift;
        view_distance -= shift;
    }
    upper = t3_camera_divide(projected[0][1] * focal, projected[0][2]);
    lower = t3_camera_divide(projected[1][1] * focal, projected[1][2]);
    if ((sint32)upper >= (sint32)lower) {
        height = projected[1][1];
        depth = projected[1][2];
        value = r_u32(0x800ADCBC) * cosine_pitch;
    } else {
        height = projected[0][1];
        depth = projected[0][2];
        value = r_u32(0x800ADCAC) * cosine_pitch;
    }
    elevation = t3_camera_divide(value, 4096);
    if (r_u32(0x800AFA88) == 7 && (sint32)third_projected[1] < (sint32)height) {
        depth = third_projected[2];
        height = third_projected[1];
        elevation = t3_camera_divide(third_position[1] * cosine_pitch, 4096);
    }
    limit = 100;
    if (camera_index == 1 && (sint32)pitch < 0)
        limit = 128;
    vertical = 0;
    if (camera_index == 1 && (sint32)pitch > 0) {
        if ((sint32)elevation >= -1000)
            vertical = t3_camera_divide(368 * (elevation + 1000), 640);
        if (!reset) {
            value = r_u32(0x80098374);
            if ((sint32)value >= (sint32)vertical) {
                if ((sint32)vertical < (sint32)value && (sint32)(value - vertical) >= 29)
                    vertical = value - 28;
            } else if ((sint32)(vertical - value) >= 29) {
                vertical = value + 28;
            }
        }
        w_u32(0x80098374, vertical);
    }
    value = limit - t3_camera_divide(450 * focal, distance);
    denominator = t3_camera_divide(cosine_pitch * value, 4096);
    delta = t3_camera_divide((0 - 112) * (depth - view_distance) - focal * (height - ground_y), denominator + 112) - view_distance;
    if ((sint32)delta < 0)
        delta = 0;
    if (reset) {
        delta = 0;
        w_u32(state + 36, 0);
    } else {
        value = r_u32(state + 40);
        if ((sint32)delta > (sint32)value) {
            velocity = delta - value;
            if ((sint32)velocity >= 801)
                velocity = 800;
            velocity = t3_camera_divide(velocity, 8);
            value = r_u32(state + 40);
            w_u32(state + 36, velocity);
            delta = value + velocity;
        } else if ((sint32)delta < (sint32)value) {
            shift = value - delta;
            if ((sint32)shift >= 1201)
                shift = 1200;
            velocity = t3_camera_divide(8 * (16 * shift - shift), 128) - shift;
            value = r_u32(state + 40);
            w_u32(state + 36, velocity);
            delta = value + velocity;
        }
    }
    w_u32(state + 40, delta);
    if ((sint32)delta > 0) {
        adjustment[2] -= delta;
        projected[0][2] += delta;
        projected[1][2] += delta;
        view_distance += delta;
    }
    if (camera_index == 0) {
        value = t3_camera_divide(delta * cosine_pitch, 4096);
        value = t3_camera_divide(value * focal, distance);
        denominator += t3_camera_divide(value, 16);
    }
    adjustment[1] = ground_y - t3_camera_divide(denominator * view_distance, focal) + vertical;
    angle = pitch;
    if ((sint32)angle < 0)
        angle += 63;
    angle = (uint32)((sint32)angle >> 5) & 0x1FFE;
    sine_pitch = (uint32)(sint32)r_s16(0x8001E8C4 + angle);
    cosine_pitch = (uint32)(sint32)r_s16(0x8001F0C4 + angle);
    angle = yaw;
    if ((sint32)angle < 0)
        angle += 63;
    angle = (uint32)((sint32)angle >> 5) & 0x1FFE;
    sine_yaw = (uint32)(sint32)r_s16(0x8001E8C4 + angle);
    cosine_yaw = (uint32)(sint32)r_s16(0x8001F0C4 + angle);
    y = t3_camera_divide(adjustment[2] * sine_pitch + adjustment[1] * cosine_pitch, 4096);
    z = t3_camera_divide(adjustment[2] * cosine_pitch - adjustment[1] * sine_pitch, 4096);
    final_x = target[0] + t3_camera_divide(adjustment[0] * cosine_yaw - z * sine_yaw, 4096);
    final_y = target[1] + y;
    final_z = target[2] + t3_camera_divide(adjustment[0] * sine_yaw + z * cosine_yaw, 4096);
    if (reset) {
        saved[0] = final_x;
        saved[1] = final_y;
        saved[2] = final_z;
        saved[3] = pitch;
        saved[4] = yaw;
    } else {
        x = final_x - saved[0];
        y = final_y - saved[1];
        z = final_z - saved[2];
        delta_pitch = pitch - saved[3];
        delta_yaw = yaw - saved[4];
        delta_pitch = (delta_pitch & 0x20000) ? (delta_pitch | 0xFFFC0000) : (delta_pitch & 0x3FFFF);
        delta_yaw = (delta_yaw & 0x20000) ? (delta_yaw | 0xFFFC0000) : (delta_yaw & 0x3FFFF);
        if (camera_index == 0)
            w_u16(0x800A032A, (sint32)(x * x + z * z) >= 1001);
        saved[0] += x;
        saved[1] += y;
        saved[2] += z;
        saved[3] += delta_pitch;
        saved[4] += delta_yaw;
    }
    for (index = 0; index < 6; ++index)
        w_u32(camera + index * 4, saved[index]);
    return camera;
}

static uint32 t3_ai_condition(uint32 condition, uint32 actor, uint32 opponent)
{
    if (!condition)
        return 1;
    if (condition < 24)
        return r_s16(actor + 0x16) == (sint32)condition - 1;
    if (condition < 47)
        return r_s16(actor + 0x16) != (sint32)condition - 24;
    if (condition < 70)
        return r_s16(opponent + 0x16) == (sint32)condition - 47;
    if (condition < 93)
        return r_s16(actor + 0x18) == (sint32)condition - 70;
    if (condition < 116)
        return r_s16(actor + 0x18) != (sint32)condition - 93;
    return 0;
}

static uint32 t3_ai_command_condition(uint32 record, uint32 actor, uint32 opponent, uint32 flags)
{
    uint32 command = r_u8(record + 3);
    switch (command) {
    case 0:
        return 1;
    case 36:
        return flags & 1;
    case 37:
        return flags & 2;
    case 38:
        return flags & 4;
    case 48:
        return flags & 8;
    case 39:
        return flags & 16;
    default:
        return t3_8002e0bc(record, actor, opponent);
    }
}

static uint32 t3_ai_mark_command(uint32 state, uint32 command)
{
    sint32 limit = r_s16(state + 0x70);
    sint32 inspected = 1;
    uint32 added = 0;
    uint32 entry = 0x8009F978;
    uint32 flag;
    uint32 action;
    uint32 opcode;
    uint32 repeat;
    if (limit <= 0)
        return 0;
    do {
        flag = r_s16(entry + 8);
        action = r_u32(entry + 4);
        opcode = r_u16(action);
        if (!flag && opcode == command) {
            w_u16(entry + 8, 1);
            ++added;
        }
        repeat = inspected < limit;
        ++inspected;
        entry += 12;
    } while (repeat);
    w_u16(state + 0x72, r_u16(state + 0x72) + added);
    return added;
}

static uint32 t3_ai_mark_standing(uint32 state)
{
    sint32 limit = r_s16(state + 0x70);
    uint32 row;
    uint32 entry;
    uint32 action;
    uint32 flags;
    uint32 start;
    uint32 added = 0;
    if (limit <= 0)
        return 0;
    for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
        if (r_s16(entry + 8))
            continue;
        action = r_u32(entry);
        flags = r_u32(action + 4);
        start = r_u8(action + 0x2D);
        if ((flags & 1) == 1 && !start) {
            w_u16(entry + 8, 1);
            ++added;
        }
    }
    w_u16(state + 0x72, r_u16(state + 0x72) + added);
    return added;
}

static uint32 t3_ai_mark_normal(uint32 state)
{
    sint32 limit = r_s16(state + 0x70);
    uint32 row;
    uint32 entry;
    uint32 action;
    uint32 opcode;
    uint32 flags;
    uint32 accepted;
    uint32 added = 0;
    if (limit <= 0)
        return 0;
    for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
        if (r_s16(entry + 8))
            continue;
        action = r_u32(entry + 4);
        opcode = r_u16(action);
        action = r_u32(entry);
        accepted = 1;
        if (opcode != 0xC001) {
            flags = r_u32(action + 4);
            accepted = (flags & 0xA0000) == 0x80000 && !(flags & 0x600000);
        }
        if (accepted) {
            w_u16(entry + 8, 1);
            ++added;
        }
    }
    w_u16(state + 0x72, r_u16(state + 0x72) + added);
    return added;
}

uint32 t3_80059890(uint32 actor, uint16 *held_output, uint16 *pressed_output)
{
    uint32 state;
    uint32 opponent;
    uint32 animation;
    uint32 opponent_animation;
    uint32 own_start;
    uint32 own_end;
    sint32 own_frame;
    uint32 opponent_start;
    uint32 opponent_end;
    sint32 opponent_frame;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 flags;
    uint32 angle;
    uint32 random_value;
    uint32 random_result;
    uint32 distance;
    uint32 adjusted_distance;
    uint32 band;
    uint32 attack;
    uint32 special_attack;
    sint32 counter;
    uint32 special_actor;
    uint32 current_animation;
    uint32 stream;
    uint32 entry;
    uint32 opcode;
    uint32 condition;
    uint32 accepted;
    uint32 repeat_count;
    uint32 row;
    uint32 count;
    uint32 stop;
    uint32 candidates[6];
    uint32 added;
    uint32 current_actor;
    uint32 selection_mask;
    uint32 selected_mask;
    sint32 limit;
    uint32 handled;
    uint32 animation_table;
    uint32 category_mask;
    uint32 blocked;
    uint32 action;
    sint32 threshold;
    uint32 decision;
    uint32 action_count;
    uint32 output_cursor;
    sint32 attack_pressure;
    sint32 preference_threshold;
    uint32 filter_kind;

    FUNCTION_MARKER(0x80059890, "SLUS_004.02");
    state = 0x8009F318 + 0x330 * r_u8(actor + 0x1886);
    opponent = r_u32(state + 0x10);
    w_u32(0x800AFA5C, actor);
    w_u32(0x800A867C, opponent);
    if (r_u32(0x800AFA88) == 7)
        t3_bns_03_800b4494();
    actor = r_u32(0x800AFA5C);
    w_u32(state + 0x5C, 0);
    first = r_u16(actor + 0x58);
    counter = r_s16(actor + 0x58);
    second = r_s16(state + 0x34);
    w_u16(state + 0x34, first);
    if (counter != (sint32)second)
        w_u16(state + 2, r_u16(state + 2) + 1);

    actor = r_u32(0x800AFA5C);
    opponent = r_u32(0x800A867C);
    animation = r_u32(actor + 0x54);
    opponent_animation = r_u32(opponent + 0x54);
    first = r_u16(actor + 0x30);
    angle = r_u16(actor + 0x2E);
    w_u32(state + 0x224, r_u32(animation + 4));
    w_u32(state + 0x220, r_u32(animation + 8));
    w_u32(state + 0x228, r_u16(animation + 8));
    w_u32(state + 0x230, r_u32(opponent_animation + 4));
    w_u32(state + 0x22C, r_u32(opponent_animation + 8));
    w_u32(state + 0x234, r_u16(opponent_animation + 8));
    w_u32(state + 0x1C, r_u32(actor + 0xF8));
    flags = (sint32)(first << 16) > 0x40000000;
    second = r_u16(actor + 0x30);
    w_u16(state + 0x2E, second);
    if (angle - 0x4E38 < 0x31C8)
        flags |= 2;
    if ((uint16)(angle + 0x8000) < 0x31C7)
        flags |= 4;
    if (r_u16(opponent + 0x64) == 0x412) {
        counter = r_s16(opponent + 0x58);
        first = r_u8(opponent_animation + 0x2E);
        if ((sint32)first >= counter && r_u32(0x800AFA3C) < 0x700)
            flags |= 8;
    }
    if (!r_s16(actor + 0x32))
        flags |= 16;
    w_u8(state + 0x201, 0);
    w_u8(state + 0x200, 0);
    opponent = r_u32(0x800A867C);
    w_u32(state + 0x64, flags);
    counter = r_s16(opponent + 0x16);
    if (counter == 14 && r_u8(r_u32(opponent_animation + 0x28)) == 24) {
        w_u8(state + 0x200, 1);
    } else {
        opponent = r_u32(0x800A867C);
        if (r_s16(opponent + 0x16) == 19 && r_u8(r_u32(opponent_animation + 0x28)) == 26) {
            w_u8(state + 0x201, 1);
            w_u32(state + 0x22C, r_u32(state + 0x22C) | 0x10000);
        }
    }
    first = r_u32(state + 8);
    if (first) {
        second = r_u16(first);
        w_u32(state + 8, first + 2);
        if (!r_s16(first + 2))
            w_u32(state + 8, 0);
        w_u16(state + 4, t3_800617dc((uint32)(sint16)second));
        goto finish;
    }
    counter = r_s16(state + 0x21E);
    if (!counter) {
        w_u16(state + 4, 0);
        goto finish;
    }
    if (counter == 1) {
        w_u16(state + 4, (r_u32(state + 0x224) << 10) & 0x1000);
        goto finish;
    }
    if (r_u32(0x8009F2FC)) {
        first = r_u16(state);
        second = r_u32(0x800AFA4C);
        if (second & first & 1)
            goto hold_previous;
    }
    if (r_u32(0x800AFA88) == 8) {
        first = r_u16(state);
        if (r_s32(0x8009F2F8 + first * 4 + 16) > 0)
            goto hold_previous;
    }

    opponent = r_u32(0x800A867C);
    actor = r_u32(0x800AFA5C);
    opponent_animation = r_u32(opponent + 0x54);
    animation = r_u32(actor + 0x54);
    own_start = r_u8(animation + 0x2D);
    own_end = r_u8(animation + 0x2E);
    own_frame = r_s16(actor + 0x58);
    opponent_start = r_u8(opponent_animation + 0x2D);
    opponent_end = r_u8(opponent_animation + 0x2E);
    opponent_frame = r_s16(opponent + 0x58);
    w_u32(state + 0x7C, 0);
    if (!((r_u32(animation + 0x24) >> 8) & 0x20))
        w_u16(state + 0xD4, 0);
    first = r_u8(state + 0x8E);
    second = r_u8(state + 0x90);
    w_u8(state + 0x8E, 0);
    w_u8(state + 0x90, 0);
    w_u8(state + 0x8F, first);
    w_u8(state + 0x91, second);
    opponent = r_u32(0x800A867C);
    first = r_u32(opponent + 0x3F8);
    second = r_u32(opponent + 0x3F4);
    if ((sint32)first / 10 >= (sint32)second) {
        w_u16(state + 0x250, 0);
        w_u16(state + 0x252, 0);
        w_u16(state + 0x258, 0);
        w_u16(state + 0x25A, 0);
    }
    random_value = r_u32(0x800ADCA0);
    w_u32(0x800ADCA0, random_value * 5 + 3);
    if ((sint32)(random_value & 0xFFF) >= r_s16(state + 0x27E) &&
        r_s16(state + 0x2E) >= 0x2000 &&
        r_s16(r_u32(0x800AFA5C) + 0x3E) < 0x6001 &&
        r_s32(state + 0x18) < 3)
        w_u8(state + 0x8F, 1);
    first = r_u32(state + 0x58);
    second = animation != first || own_frame < r_s16(state + 0x4E);
    first = r_u32(state + 0x1D8);
    w_u16(state + 0x28, second);
    second = opponent_animation != first || opponent_frame < r_s16(state + 0x206);
    first = r_u16(state + 0x2C);
    w_u16(state + 0x204, second);
    third = r_u32(state + 0x58);
    w_u16(state + 0x2C, first + 1);
    if (third != animation) {
        w_u32(state + 0x58, animation);
        w_u16(state + 0x2C, 0);
    }
    first = r_u32(state + 0x224);
    w_u16(state + 0x4E, own_frame);
    if (first & 0x400000) {
        w_u16(state + 0x40, 40);
        w_u16(state + 0x3A, 0);
        w_u16(state + 0x38, 0);
        w_u16(state + 0x48, 0xFFFF);
    }
    if (own_start) {
        if ((sint32)own_start == own_frame)
            w_u16(state + 0x1D2, r_u16(state + 0x1D2) + 1);
    } else if (!(r_u32(state + 0x220) & 0x80000)) {
        opponent = r_u32(0x800A867C);
        if (!r_u8(opponent + 0x87))
            w_u16(state + 0x1D2, 0);
    }
    if (opponent_start) {
        if ((sint32)opponent_start == opponent_frame) {
            first = r_u16(state + 0x216) + 1;
            w_u16(state + 0x216, first);
            if ((sint16)first >= 11)
                w_u16(state + 0x216, 10);
        }
    } else if (!(r_u32(state + 0x22C) & 0x80000) && !r_u8(actor + 0x87)) {
        w_u16(state + 0x216, 0xFFFF);
    }
    distance = r_u32(state + 0x1C);
    first = r_u32(0x800AFA88);
    w_u32(0x8009F2F8, 0);
    if (first == 7) {
        first = t3_bns_03_800b4644(r_u32(0x800AFA5C));
        if ((sint32)first < (sint32)distance) {
            distance = first;
            w_u32(0x8009F2F8, 1);
        }
    }
    first = r_u32(state + 0x20);
    second = r_s16(state + 0x32A);
    w_u32(state + 0x20, distance);
    adjusted_distance = distance - second - 128;
    counter = r_s16(state + 0x314);
    w_u32(state + 0x24, distance - first);
    if ((sint32)adjusted_distance < counter) {
        first = r_u32(actor + 0x3E8);
        if (!first)
            first = r_u32(actor + 0x3EC);
        if (!first)
            first = r_u32(actor + 0x3F0);
        if (!first) {
            opponent = r_u32(0x800A867C);
            first = r_u32(opponent + 0x3E8);
            if (!first)
                first = r_u32(opponent + 0x3EC);
            if (!first)
                first = r_u32(opponent + 0x3F0);
        }
        if (first) {
            counter = r_s16(state + 0x50);
            w_u32(state + 0x14, 0);
            if (!counter) {
                w_u16(state + 0x68, r_u16(state + 0x68) - 1);
                if (!r_s16(state + 0x50))
                    w_u16(state + 0x68, r_u16(state + 0x68) - 1);
            }
        } else {
            w_u32(state + 0x14, 1);
            if (!r_s16(state + 0x50))
                w_u16(state + 0x68, r_u16(state + 0x68) - 1);
        }
        w_u16(state + 0x2A, 0);
    } else if ((sint32)adjusted_distance < r_s16(state + 0x316)) {
        w_u32(state + 0x14, 2);
        w_u16(state + 0x2A, 0);
    } else if ((sint32)adjusted_distance < r_s16(state + 0x318)) {
        w_u32(state + 0x14, 3);
        w_u16(state + 0x2A, 0);
    } else {
        band = (sint32)adjusted_distance < r_s16(state + 0x31A) ? 4 : 5;
        w_u32(state + 0x14, band);
        random_result = (uint32)randPSX();
        random_value = r_u32(0x800ADCA0);
        first = ((random_result + random_value) & 0x7FFF) * (band == 4 ? 2 : 3);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        second = r_u16(state + 0x2A);
        w_u16(state + 0x2A, second + band + (first >> 15));
    }
    if (!r_s16(state + 0x50)) {
        w_u16(state + 0x68, r_u16(state + 0x68) - 1);
    } else {
        opponent = r_u32(0x800A867C);
        if (r_u8(opponent + 0xDB))
            w_u16(state + 0x68, r_u16(state + 0x68) - 1);
    }
    third = r_s16(state + 0x31E);
    first = r_u32(state + 0x1C);
    second = r_u32(state + 0x24);
    adjusted_distance = first - third;
    if ((sint32)second < 0)
        adjusted_distance += second;
    if ((sint32)adjusted_distance < r_s16(state + 0x320) + 128)
        band = 1;
    else if ((sint32)adjusted_distance < r_s16(state + 0x322) + 256)
        band = 2;
    else if ((sint32)adjusted_distance < r_s16(state + 0x324) + 512)
        band = 3;
    else if ((sint32)adjusted_distance < r_s16(state + 0x326) + 640)
        band = 4;
    else
        band = 5;
    w_u32(state + 0x18, band);
    if (!opponent_start && (r_u32(state + 0x230) & 0x80000)) {
        first = r_u16(state + 0x20E);
        opponent = r_u32(0x800A867C);
        w_u16(state + 0x20E, first + 1);
        first = r_u32(opponent + 0x54);
        second = r_u16(first + 0x10);
        counter = r_s16(opponent + 0xA0);
        if (second == (uint32)counter)
            w_u16(state + 0x20E, 0);
    } else {
        w_u16(state + 0x20E, 0);
    }
    attack = r_s16(state + 0x20E) > 0 ||
        (opponent_start && (sint32)opponent_end + 1 >= opponent_frame);
    w_u16(state + 0x20C, attack);
    if (attack && opponent_frame < (sint32)opponent_start - 3)
        w_u32(state + 0x80, r_u32(state + 0x80) + 1);
    else
        w_u32(state + 0x80, 0);
    if (own_start && (sint32)opponent_end + 1 < opponent_frame)
        w_u32(state + 0x84, r_u32(state + 0x84) + 1);
    else
        w_u32(state + 0x84, 0);
    if (r_s16(state + 0x20C)) {
        counter = r_s16(state + 0x6C);
        first = r_u16(state + 0x6C);
        if (counter > 0)
            w_u16(state + 0x6C, first - 1);
    } else {
        w_u16(state + 0x6C, 0xFFFF);
    }
    if (!r_s16(state + 0x20C) && (r_u32(state + 0x230) & 0x200000))
        w_u16(state + 0x20C, 1);
    if (r_u32(state + 0x230) & 0x80000)
        w_u16(state + 0x210, r_u16(state + 0x210) + 1);
    else
        w_u16(state + 0x210, 0);
    special_attack = (r_u32(state + 0x22C) & 0x400000) &&
        (!opponent_start || (sint32)opponent_end >= opponent_frame);
    first = r_s16(state + 0x20C);
    w_u16(state + 0x214, special_attack);
    if (!first && !special_attack) {
        w_u16(state + 0x20A, 999);
        w_u8(state + 0x92, 0);
    } else if (opponent_start) {
        if ((sint32)opponent_end < opponent_frame) {
            w_u16(state + 0x214, 0);
            w_u16(state + 0x20C, 0);
            w_u16(state + 0x20A, 997);
        } else {
            first = opponent_start - (uint32)opponent_frame;
            w_u16(state + 0x20A, first);
            if ((sint16)first >= 101) {
                w_u16(state + 0x214, 0);
                w_u16(state + 0x20C, 0);
                w_u16(state + 0x20A, 998);
            }
        }
    } else {
        w_u16(state + 0x20A, 10);
        first = r_u32(opponent_animation + 0xC);
        first = r_u8(first + 0xA);
        w_u16(state + 0x20A, first - (uint32)opponent_frame + 1);
    }
    opponent = r_u32(0x800A867C);
    first = r_u32(opponent + 0x994);
    second = r_u32(0x800AFA5C);
    w_u16(state + 0x21A, (sint32)first < -3099);
    if (r_s16(second + 0x3E) >= 0x3001) {
        w_u8(state + 0x94, 0);
    } else if (!(r_u32(state + 0x230) & 0x200000)) {
        first = r_u32(state + 0x234);
        if (first != 0x607 && first != 0x706)
            w_u8(state + 0x94, 0);
    }
    if (r_s16(state + 0x54) >= 0 && r_s32(state + 0x18) < 2) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((sint32)(random_value & 0xFFF) >= r_s16(state + 0x27E))
            w_u8(state + 0x92, 1);
    }
    first = r_u32(state + 0x230);
    second = 0;
    if ((first & 0x204) && (uint16)first != 0x4C02) {
        opponent = r_u32(0x800A867C);
        second = !r_u8(opponent + 0xDB);
    }
    opponent = r_u32(0x800A867C);
    w_u16(state + 0x208, second);
    first = r_s16(opponent + 0x16) == 4 && r_s16(opponent + 0xA0) == 0x456;
    w_u16(state + 0x88, first);
    if (first)
        w_u16(state + 0x216, 0xFFFF);
    distance = r_u32(state + 0x1C);
    first = r_u16(state + 0x224);
    second = 0;
    if ((sint32)distance < 2951 && !r_s16(state + 0x88)) {
        if (r_s16(state + 0x214)) {
            if (r_u32(state + 0x234) != 0x217)
                second = first == 0x2021 || first == 0x2829;
        } else if (r_s16(state + 0x20C) && r_s16(state + 0x2E) < 0x3001) {
            third = r_u32(state + 0x234);
            switch (third) {
            case 0x412:
                second = first == 0x2021 || first == 0x2829 || first == 0x1052;
                break;
            case 0x217:
                second = first == 0x1052;
                break;
            case 0x10F:
                second = first == 0x2829;
                break;
            case 0x31F:
                second = first == 0x2829 || first == 0x1052;
                break;
            case 0x706:
                second = first == 0x2829 || first == 0x2021;
                break;
            }
        }
    }
    w_u16(state + 0x6A, second);
    if (!(r_u32(state + 0x224) & 1)) {
        w_u16(state + 0x4A, 0xFFFF);
    } else if (r_s16(state + 0x3C) > 0) {
        w_u16(state + 0x4A, r_u16(state + 0x4A) - 1);
    } else if (!((r_u32(animation + 0x24) >> 8) & 0x10)) {
        first = r_u32(0x800ADCA0);
        second = first * 5 + 3;
        third = second * 5 + 3;
        counter = (first & 7) + (second & 15) + 20 + (third & 7);
        w_u32(0x800ADCA0, third * 5 + 3);
        w_u16(state + 0x4A, counter);
    }
    w_u16(state + 0x50, own_start != 0);
    special_attack = 0;
    if ((r_u32(animation + 4) & 0x200000) && (r_u32(animation + 8) & 0x200000))
        special_attack = (sint32)own_end >= own_frame;
    special_actor = r_u32(0x800AFA5C);
    w_u16(state + 0x52, special_attack);
    if (!r_s16(special_actor + 0x74)) {
        w_u32(state + 0xA0, 0);
        w_u16(state + 0x96, 0);
        w_u16(state + 0x9C, 0xFFFF);
        w_u32(state + 0xBC, 0);
        goto ordinary_actions;
    }
    w_u16(state + 0x40, 40);
    w_u16(state + 0x3A, 0);
    w_u16(state + 0x38, 0);
    w_u16(state + 0x48, 0xFFFF);
    first = r_u32(state + 0xA0);
    current_animation = r_u32(special_actor + 0x54);
    counter = r_s16(special_actor + 0x74);
    if (counter > 0) {
        if (current_animation != first) {
            counter = r_s16(state + 0x96);
            if (counter >= 0 && counter < r_s16(state + 0x246) - 1) {
                random_value = r_u32(0x800ADCA0);
                w_u32(0x800ADCA0, random_value * 5 + 3);
                if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x24C)) {
                    count = 0;
                    first = r_u32(special_actor + 0x54);
                    stream = r_u32(first + 0xC);
                    actor = special_actor;
                    opcode = r_u16(stream);
                    stop = 0;
                    while (opcode != 0xC000) {
                        repeat_count = 1;
                        if (opcode == 0xC00C) {
                            first = r_u16(stream + 6);
                            repeat_count = r_u8(stream + 11);
                            entry = 0x80017C78 + 12 * first;
                        } else {
                            entry = stream;
                        }
                        for (row = 0; row < repeat_count; ++row, entry += 12) {
                            condition = r_u8(entry + 2);
                            opponent = r_u32(0x800A867C);
                            if (!condition)
                                accepted = 1;
                            else if (condition < 24)
                                accepted = r_s16(actor + 0x16) == (sint32)condition - 1;
                            else if (condition < 47)
                                accepted = r_s16(actor + 0x16) != (sint32)condition - 24;
                            else if (condition < 70)
                                accepted = r_s16(opponent + 0x16) == (sint32)condition - 47;
                            else if (condition < 93)
                                accepted = r_s16(actor + 0x18) == (sint32)condition - 70;
                            else if (condition < 116)
                                accepted = r_s16(actor + 0x18) != (sint32)condition - 93;
                            else
                                accepted = 0;
                            if (!accepted)
                                continue;
                            random_value = r_u32(0x800ADCA0);
                            w_u32(0x800ADCA0, random_value * 5 + 3);
                            random_value &= 0x70;
                            first = r_u16(entry + 6);
                            accepted = 1;
                            switch (first) {
                            case 0x3A7:
                                accepted = 0;
                                break;
                            case 0xD41:
                                first = r_u32(0x800AFA5C);
                                if (r_s16(first + 0x16) == 4)
                                    accepted = random_value != 0;
                                break;
                            case 0xD24:
                            case 0xD32:
                                first = r_u32(0x800AFA5C);
                                counter = r_s16(first + 0x16);
                                if (!counter || counter == 13)
                                    accepted = !random_value;
                                break;
                            case 0xDE2:
                                first = r_u32(0x800AFA5C);
                                if (!r_s16(first + 0x16) && random_value)
                                    accepted = 0;
                                break;
                            case 0xD25:
                            case 0xD34:
                                first = r_u32(0x800AFA5C);
                                counter = r_s16(first + 0x16);
                                if (!counter || counter == 13)
                                    accepted = 0;
                                break;
                            }
                            if (!accepted)
                                continue;
                            if (r_u8(entry + 3)) {
                                opponent = r_u32(0x800A867C);
                                if (!t3_8002e0bc(entry, actor, opponent))
                                    continue;
                            }
                            candidates[count++] = entry;
                            if (count >= 6) {
                                stop = 1;
                                break;
                            }
                        }
                        stream += 12;
                        opcode = r_u16(stream);
                        if (stop)
                            break;
                    }
                    if (count) {
                        first = r_u32(0x800AFA5C);
                        second = r_u16(state + 0x96);
                        third = r_u32(first + 0x54);
                        w_u16(state + 0x96, second + 1);
                        w_u32(state + 0xA0, third);
                        random_result = (uint32)randPSX();
                        random_value = r_u32(0x800ADCA0);
                        first = count * ((random_result + random_value) & 0x7FFF);
                        w_u32(0x800ADCA0, random_value * 5 + 3);
                        entry = candidates[first >> 15];
                        w_u16(state + 0x9C, r_u8(entry + 9) + 1);
                        w_u32(state + 0xBC, entry);
                    } else {
                        w_u16(state + 0x96, 0xFFFF);
                        w_u16(state + 0x9C, 0xFFFF);
                        w_u32(state + 0xBC, 0);
                    }
                }
            }
        }
        counter = r_s16(state + 0x9C);
        if (r_s16(special_actor + 0x58) != counter)
            goto no_input;
        first = r_u8(special_actor + 0x1886);
        entry = r_u32(state + 0xBC);
        opcode = r_u16(entry);
        second = r_u16(entry + 6);
        first = r_u32(0x8009F2E8 + first * 4);
        third = r_u32(first + second * 4);
        t3_800618c0(state, opcode, third);
        goto finish;
    }
    if (current_animation == first)
        goto no_input;
    w_u16(state + 0x96, r_u16(state + 0x96) - 1);
    opponent = r_u32(0x800A867C);
    w_u32(state + 0xA0, current_animation);
    counter = r_s16(opponent + 0xA0);
    first = r_s16(opponent + 0x16);
    accepted = counter == 0x189 || counter == 0x18E || counter == 0x17F || counter == 0x181 ||
        (first == 9 && counter == 0x172) || (!first && counter == 0x172) ||
        (first == 3 && counter == 0x152);
    if (!accepted) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((sint32)(random_value & 0xFFF) >= r_s16(state + 0x24A))
            goto finish;
    }
    first = r_u32(0x800AFA5C);
    first = r_u32(first + 0x54);
    count = 0;
    stream = r_u32(first + 0xC);
    opcode = r_u16(stream);
    stop = 0;
    while (opcode != 0xC000) {
        repeat_count = 1;
        if (opcode == 0xC00C) {
            first = r_u16(stream + 6);
            repeat_count = r_u8(stream + 11);
            entry = 0x80017C78 + first * 12;
        } else {
            entry = stream;
        }
        for (row = 0; row < repeat_count; ++row, entry += 12) {
            w_u32(0x800A01E8 + count * 4, entry);
            ++count;
            if (count >= 48) {
                stop = 1;
                break;
            }
        }
        stream += 12;
        opcode = r_u16(stream);
        if (stop)
            break;
    }
    opcode = 0;
    if (count) {
        random_result = (uint32)randPSX();
        random_value = r_u32(0x800ADCA0);
        first = count * ((random_result + random_value) & 0x7FFF);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        entry = r_u32(0x800A01E8 + (first >> 15) * 4);
        first = r_u8(entry + 3);
        if (first == 58)
            opcode = 1;
        else if (first == 59)
            opcode = 2;
        else if (first == 60)
            opcode = 1;
        else
            opcode = r_u16(entry);
    }
    if (!opcode)
        goto no_input;
    t3_800618c0(state, opcode, 0);
    goto finish;

ordinary_actions:
    if (r_u16(state + 0x224) == 0x4C02) {
        current_actor = r_u32(0x800AFA5C);
        first = r_u32(current_actor + 0x54);
        if (!r_u8(first + 0x2D)) {
            w_u16(state + 0x40, 40);
            w_u16(state + 0x3A, 0);
            w_u16(state + 0x38, 0);
            w_u16(state + 0x48, 0xFFFF);
            first = r_u32(current_actor + 0x54);
            w_u16(state + 0x72, 0);
            w_u32(state + 0x58, first);
            first = t3_80056764(state, 1);
            w_u16(state + 0x70, first);
            if ((sint16)first <= 0)
                goto no_input;
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((sint32)(random_value & 0xFFF) >= r_s16(state + 0x29A))
                goto no_input;
            added = 0;
            limit = r_s16(state + 0x70);
            if (limit > 0) {
                for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                    if (r_s16(entry + 8))
                        continue;
                    first = r_u32(entry + 4);
                    first = r_u8(first + 8);
                    if (first - 28 < 2) {
                        w_u16(entry + 8, 1);
                        ++added;
                    }
                }
                w_u16(state + 0x72, r_u16(state + 0x72) + added);
            }
            if (!added)
                goto no_input;
            t3_80058a84(state);
            goto finish;
        }
    }
    current_actor = r_u32(0x800AFA5C);
    if (r_u32(current_actor + 0x1A4))
        goto finish;
    if (r_u8(current_actor + 0xDB) || !(r_u32(state + 0x224) & 0x404)) {
        w_u16(state + 0x36, 0);
        goto ordinary_helpers;
    }
    first = r_u32(current_actor + 0x54);
    stream = r_u32(first + 0xC);
    opcode = r_u16(stream);
    stop = 0;
    while (opcode != 0xC000) {
        repeat_count = 1;
        if (opcode == 0xC00C) {
            first = r_u16(stream + 6);
            repeat_count = r_u8(stream + 11);
            entry = 0x80017C78 + first * 12;
        } else {
            entry = stream;
        }
        for (row = 0; row < repeat_count; ++row, entry += 12) {
            if (r_u8(entry + 3) == 49) {
                stop = 1;
                break;
            }
        }
        stream += 12;
        opcode = r_u16(stream);
    }
    if (stop) {
        first = r_u16(state + 0x6E);
        w_u16(state + 0x36, 1);
        first -= 1;
        w_u16(state + 0x6E, first);
        if ((sint16)first >= 0)
            goto no_input;
        random_value = r_u32(0x800ADCA0);
        first = state + (random_value & 3);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        first = r_u8(first + 0x238);
        w_u16(state + 4, 16);
        w_u16(state + 0x6E, first);
        goto finish;
    }
    current_actor = r_u32(0x800AFA5C);
    first = r_u32(current_actor + 0x54);
    w_u16(state + 0x72, 0);
    w_u32(state + 0x58, first);
    first = t3_80056764(state, 1);
    w_u16(state + 0x70, first);
    if ((sint16)first <= 0)
        goto no_input;
    second = r_u8(state + 0x201);
    w_u16(state + 0x36, 1);
    w_u16(state + 0x40, 40);
    w_u16(state + 0x48, 0xFFFF);
    w_u16(state + 0x3A, 0);
    w_u16(state + 0x38, 0);
    w_u16(state + 0x8A, 16);
    if (second) {
        current_actor = r_u32(0x800AFA5C);
        if (r_u32(current_actor + 0x60) & 0x200) {
            w_u16(state + 4, 0x8000);
            goto finish;
        }
    }
    accepted = 0;
    if (r_u32(state + 0x230) & 0x200000) {
        opponent = r_u32(0x800A867C);
        first = r_u32(opponent + 0x54);
        accepted = !r_u8(first + 0x2D);
    }
    if (!accepted && r_u32(state + 0x234) == 0x607 &&
        !(r_u32(state + 0x22C) & 0x10000) && r_s16(state + 0x20A) < 29)
        accepted = 1;
    if (accepted) {
        opponent = r_u32(0x800A867C);
        first = r_u32(opponent + 0x54);
        first = r_u8(first + 0x2E);
        if (r_s16(opponent + 0x58) < (sint32)first)
            goto no_input;
    }
    opponent = r_u32(0x800A867C);
    first = r_u32(opponent + 0x54);
    if (!r_u8(first + 0x2D)) {
        if (r_s32(state + 0x14) < 3) {
            selection_mask = 0;
            if (r_u16(state + 2) & 1) {
                counter = r_s16(state + 0x68);
                w_u16(state + 0x36, 0);
                if (counter >= 0) {
                    selection_mask = 7;
                } else {
                    random_value = r_u32(0x800ADCA0);
                    w_u32(0x800ADCA0, random_value * 5 + 3);
                    if (random_value & 0x3F) {
                        selection_mask = 7;
                    } else {
                        w_u16(state + 0x36, 0);
                        selection_mask = 0x38;
                    }
                }
            }
        } else {
            selection_mask = r_s32(state + 0x18) < 4 ? 0x3F : 7;
            w_u16(state + 0x36, 0);
        }
        goto select_input_mask;
    }
    if (r_s16(state + 0x20A) >= 12)
        goto distant_attack_mask;
    second = r_u8(first + 0x2E);
    counter = r_s16(opponent + 0x58);
    if ((sint32)second < counter)
        goto distant_attack_mask;
    if (r_u32(state + 0x234) == 0x10F || (r_u32(state + 0x22C) & 0x10000)) {
        if (!(r_u16(state + 2) & 1)) {
            selection_mask = 0;
            goto select_input_mask;
        }
        random_value = r_u32(0x800ADCA0);
        w_u16(state + 0x36, 0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if (random_value & 2) {
            selection_mask = 0x38;
            goto select_input_mask;
        }
        first = r_u32(opponent + 0x54);
        if (!r_u8(first + 0x19) && r_s16(state + 0x2E) < 0x4001 &&
            !(r_u32(state + 0x230) & 0x200000)) {
            selection_mask = 4;
            goto select_input_mask;
        }
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        selection_mask = (random_value & 0x3F) ? 2 : 4;
        goto select_input_mask;
    }
    band = r_u32(state + 0x18);
    if ((sint32)band < 3 && ((sint32)band < 2 || r_s32(state + 0x24) < 11)) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        selection_mask = (random_value & 15) ? 0x3F : 0;
    } else {
        selection_mask = 1;
    }
    w_u16(state + 0x36, 0);
    goto select_input_mask;

distant_attack_mask:
    selection_mask = 0;
    if (r_s32(state + 0x14) < 2) {
        selection_mask = r_u32(state + 0x230) & 2 ? 0x38 : 0x28;
        w_u16(state + 0x36, 0);
    }

select_input_mask:
    if (selection_mask == 0x80) {
        w_u16(state + 4, 0);
        goto ordinary_helpers;
    }
    if (!selection_mask)
        goto no_input;
    do {
        random_result = (uint32)randPSX();
        random_value = r_u32(0x800ADCA0);
        first = ((random_result + random_value) & 0x7FFF) * 6;
        w_u32(0x800ADCA0, random_value * 5 + 3);
        selected_mask = r_u16(0x80022E74 + (first >> 15) * 2);
    } while (!(selection_mask & selected_mask));
    switch (selected_mask) {
    case 1:
        w_u16(state + 4, 0x1000);
        goto finish;
    case 2:
        first = (r_u16(state + 2) & 7) ? 0x80 : 0x4080;
        w_u16(state + 4, first);
        goto finish;
    case 4:
        current_actor = r_u32(0x800AFA5C);
        first = r_s16(current_actor + 0x16) == 18 ? 0x2000 : 0x8000;
        w_u16(state + 4, first);
        goto finish;
    case 8:
        t3_800572a4(state);
        break;
    case 16:
        limit = r_s16(state + 0x70);
        if (limit > 0) {
            added = 0;
            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                if (r_s16(entry + 8))
                    continue;
                first = r_u32(entry);
                first = r_u32(first + 8);
                if ((first & 0x10000) || (uint16)first == 0x10F) {
                    w_u16(entry + 8, 1);
                    ++added;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
        }
        break;
    case 32:
        limit = r_s16(state + 0x70);
        if (limit > 0) {
            added = 0;
            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                if (r_s16(entry + 8))
                    continue;
                first = r_u32(entry + 4);
                opcode = r_u16(first);
                first = r_u32(entry);
                accepted = 1;
                if (opcode != 0xC001) {
                    first = r_u32(first + 4);
                    accepted = (first & 0xA0000) == 0x80000 && !(first & 0x600000);
                }
                if (accepted) {
                    w_u16(entry + 8, 1);
                    ++added;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
        }
        break;
    }
    if (r_s16(state + 0x72) > 0)
        t3_80058a84(state);
    goto finish;

ordinary_helpers:
    if (t3_800591dc(0, state))
        goto finish;
    if (t3_800591dc(1, state))
        goto finish;
    first = r_u16(0x800AE208);
    second = r_s16(state + 0x216);
    second *= first * 30;
    opponent = r_u32(0x800A867C);
    third = r_s16(state + 0x258);
    accepted = 0;
    first = r_u8(opponent + 0xD2);
    second += third;
    if (first) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((sint32)(random_value & 0xFFF) < (sint32)second) {
            w_u8(state + 0x8F, 1);
            accepted = 1;
        }
    }
    if (!accepted) {
        accepted = r_s16(state + 0x6C) >= 0;
        if (!accepted) {
            opponent = r_u32(0x800A867C);
            first = r_u32(opponent + 0x54);
            first = r_u8(first + 0x2D);
            accepted = !first;
            if (!accepted)
                accepted = (sint32)first - 30 < r_s16(opponent + 0x58);
        }
        if (accepted && r_s16(state + 0x6A))
            goto hold_previous;
    }
    counter = r_s16(state + 0x28);
    w_u16(state + 0x72, 0);
    w_u16(state + 0x70, 0);
    handled = 0;
    if (counter) {
        current_actor = r_u32(0x800AFA5C);
        counter = r_s16(state + 0xC0);
        first = r_u32(current_actor + 0x54);
        w_u32(state + 0x58, first);
        w_u16(state + 0x4E, r_u16(current_actor + 0x58));
        if (counter > 0) {
            first = r_u32(state + 0x58);
            stream = r_u32(first + 0xC);
            count = 0;
            opcode = r_u16(stream);
            stop = 0;
            while (opcode != 0xC000) {
                repeat_count = 1;
                if (opcode == 0xC00C) {
                    first = r_u16(stream + 6);
                    repeat_count = r_u8(stream + 11);
                    entry = 0x80017C78 + 12 * first;
                } else {
                    entry = stream;
                }
                for (row = 0; row < repeat_count; ++row, entry += 12) {
                    condition = r_u8(entry + 2);
                    current_actor = r_u32(0x800AFA5C);
                    opponent = r_u32(0x800A867C);
                    if (!t3_ai_condition(condition, current_actor, opponent))
                        continue;
                    current_actor = r_u32(0x800AFA5C);
                    first = r_u8(current_actor + 0x1886);
                    second = r_u16(entry + 6);
                    first = r_u32(0x8009F2E8 + first * 4);
                    action = r_u32(first + second * 4);
                    if (!((r_u32(action + 0x24) >> 8) & 0x20))
                        continue;
                    w_u32(state + 0xC4 + count * 4, entry);
                    ++count;
                    if (count >= 4) {
                        stop = 1;
                        break;
                    }
                }
                stream += 12;
                opcode = r_u16(stream);
                if (stop)
                    break;
            }
            w_u16(state + 0xC2, count);
            handled = 1;
            if (count) {
                w_u16(state + 4, r_u16(state + 6) & 0xF000);
            } else {
                w_u16(state + 0xC2, 0);
                w_u16(state + 0xC0, 0);
            }
        } else if (r_s16(state + 0xD8) > 0) {
            count = 0;
            first = r_u32(state + 0x58);
            random_value = r_u32(0x800ADCA0);
            animation_table = r_u32(state + 0xC);
            blocked = r_s16(state + 0x208);
            threshold = r_s16(state + 0x24E);
            stream = r_u32(first + 0xC);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            first = r_u32(state + 0x14);
            opcode = r_u16(stream);
            category_mask = r_u32(0x80098230 + first * 4);
            stop = 0;
            while (opcode != 0xC000) {
                repeat_count = 1;
                if (opcode == 0xC00C) {
                    first = r_u16(stream + 6);
                    repeat_count = r_u8(stream + 11);
                    entry = 0x80017C78 + first * 12;
                } else {
                    entry = stream;
                }
                for (row = 0; row < repeat_count; ++row, entry += 12) {
                    condition = r_u8(entry + 2);
                    current_actor = r_u32(0x800AFA5C);
                    opponent = r_u32(0x800A867C);
                    if (!t3_ai_condition(condition, current_actor, opponent))
                        continue;
                    first = r_u16(entry + 6);
                    action = r_u32(animation_table + first * 4);
                    if (blocked && (r_u32(action + 8) & 0x400000))
                        continue;
                    flags = r_u32(action + 4);
                    if (flags & 0x20000)
                        continue;
                    first = r_u32(action + 8);
                    if (!(first & 0x200000) && !(flags & 0x80000)) {
                        if (!(first & 0x400000))
                            continue;
                        random_value = r_u32(0x800ADCA0);
                        w_u32(0x800ADCA0, random_value * 5 + 3);
                        if (threshold >= (sint32)(random_value & 0x7FFF))
                            continue;
                        first = r_u32(action + 8);
                    }
                    first &= 0xFFFF0000;
                    if (!(first & 0x1C0000))
                        first |= 0xC0000;
                    if (!(first & category_mask))
                        continue;
                    w_u32(state + 0xDC + count * 4, entry);
                    ++count;
                    if (count >= 48) {
                        stop = 1;
                        break;
                    }
                }
                stream += 12;
                opcode = r_u16(stream);
                if (stop)
                    break;
            }
            w_u16(state + 0xDA, count);
            handled = 1;
            if (count) {
                w_u16(state + 4, r_u16(state + 6) & 0xF000);
            } else {
                w_u16(state + 0xDA, 0);
                w_u16(state + 0xD8, 0);
            }
        } else if (!r_s16(state + 0x52)) {
            first = r_u32(current_actor + 0x54);
            count = 0;
            if (!r_u8(first + 0x2D)) {
                w_u16(state + 0x1A2, 0);
                w_u16(state + 0xDA, 0);
                w_u16(state + 0xC2, 0);
                w_u16(state + 0x1A0, 0);
                w_u16(state + 0xD8, 0);
                w_u16(state + 0xC0, 0);
                w_u16(state + 0x98, 0);
            } else {
                first = r_s16(state + 0x1CC);
                threshold = r_s16(state + 0x27E);
                if (first) {
                    w_u16(state + 0x1CC, 0);
                } else {
                    first = r_u32(state + 0x58);
                    w_u16(state + 0x1D0, 0);
                    stream = r_u32(first + 0xC);
                    opcode = r_u16(stream);
                    stop = 0;
                    special_actor = current_actor;
                    while (opcode != 0xC000) {
                        repeat_count = 1;
                        if (opcode == 0xC00C) {
                            first = r_u16(stream + 6);
                            repeat_count = r_u8(stream + 11);
                            entry = 0x80017C78 + first * 12;
                        } else {
                            entry = stream;
                        }
                        for (row = 0; row < repeat_count; ++row, entry += 12) {
                            if (r_u8(entry + 9) != 1)
                                continue;
                            first = r_u8(entry + 10);
                            second = r_u8(entry + 11);
                            if (first != second)
                                continue;
                            condition = r_u8(entry + 2);
                            current_actor = r_u32(0x800AFA5C);
                            opponent = r_u32(0x800A867C);
                            if (!t3_ai_condition(condition, current_actor, opponent))
                                continue;
                            first = r_u8(special_actor + 0x1886);
                            second = r_u16(entry + 6);
                            first = r_u32(0x8009F2E8 + first * 4);
                            action = r_u32(first + second * 4);
                            if ((r_u32(action + 0x24) >> 8) & 0x20) {
                                random_value = r_u32(0x800ADCA0);
                                w_u32(0x800ADCA0, random_value * 5 + 3);
                                if ((sint32)(random_value & 0xFFF) < threshold)
                                    continue;
                            }
                            w_u32(state + 0x1A4 + count * 4, entry);
                            if (r_u8(entry + 10) < 60) {
                                counter = r_s16(state + 0x1D0);
                                first = r_u8(entry + 10);
                                if (counter < (sint32)first)
                                    w_u16(state + 0x1D0, r_u8(entry + 10));
                            }
                            ++count;
                            if (count >= 10) {
                                stop = 1;
                                break;
                            }
                        }
                        stream += 12;
                        opcode = r_u16(stream);
                        if (stop)
                            break;
                    }
                    w_u16(state + 0x1A2, count);
                }
                handled = 1;
                if (count) {
                    first = r_u16(state + 6);
                    second = r_u16(state + 0x1A0);
                    w_u16(state + 4, first & 0xF000);
                    w_u16(state + 0x1A0, second + 1);
                } else {
                    w_u16(state + 0x1A2, 0);
                    w_u16(state + 0x1A0, 0);
                }
            }
        }
    }
    current_actor = r_u32(0x800AFA5C);
    w_u16(state + 0x4E, r_u16(current_actor + 0x58));
    if (handled)
        goto finish;
    first = t3_80056764(state, 0);
    counter = r_s16(state + 0x2C);
    w_u16(state + 0x70, first);
    if (counter >= 221) {
        added = 0;
        limit = (sint16)first;
        for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
            if (!r_s16(entry + 8)) {
                w_u16(entry + 8, 1);
                ++added;
            }
        }
        w_u16(state + 0x72, r_u16(state + 0x72) + added);
        if (added) {
            t3_80058a84(state);
            goto finish;
        }
    }
    if (r_u32(0x800AFA88) == 7 && t3_bns_03_800b46a0(state))
        goto finish;
    if (r_s16(state + 0x52) && r_s16(state + 0x70) && !r_u8(state + 0x200)) {
        current_actor = r_u32(0x800AFA5C);
        first = r_u32(current_actor + 0x54);
        counter = r_s16(current_actor + 0x58);
        first = r_u8(first + 0x2D);
        second = r_s16(state + 0x20A);
        first -= (uint32)counter;
        if ((sint32)second < (sint32)first || r_s32(state + 0x14) >= 3 ||
            r_s16(current_actor + 0x32) == 2) {
            added = 0;
            limit = r_s16(state + 0x70);
            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                if (!r_s16(entry + 8)) {
                    w_u16(entry + 8, 1);
                    ++added;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
            if (added && (sint32)t3_80058a84(state) >= 0)
                w_u16(state + 0x68, 0xFFFF);
        }
        goto finish;
    }
    if (r_s16(state + 0x20C) && r_s16(state + 0x216) >= 0 && r_s32(state + 0x18) < 4) {
        accepted = r_u8(state + 0x8F) != 0;
        if (!accepted) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            accepted = (sint32)(random_value & 0xFFF) < r_s16(state + 0x25E);
        }
        if (accepted) {
            counter = r_s16(state + 0x20A);
            if (counter < 30 && counter >= 2) {
                first = counter > 0 ? t3_80057d58(state) : t3_80057538(state);
                if ((sint32)first > 0 && (sint32)t3_80058a84(state) >= 0)
                    goto finish;
            }
        }
        accepted = 0;
        if (!r_s16(state + 0x50)) {
            first = r_u16(state + 0x22C);
            accepted = first && first != 0x607;
            if (!accepted)
                accepted = r_s16(state + 0x20E) > 0;
            if (accepted) {
                accepted = r_u8(state + 0x92) != 0;
                if (!accepted) {
                    random_value = r_u32(0x800ADCA0);
                    w_u32(0x800ADCA0, random_value * 5 + 3);
                    accepted = (sint32)(random_value & 0xFFF) < r_s16(state + 0x262);
                }
            }
        }
        if (accepted) {
            current_actor = r_u32(0x800AFA5C);
            first = 0;
            if (!r_s16(current_actor + 0x32)) {
                second = r_u32(state + 0x234);
                w_u16(state + 0x40, 40);
                w_u16(state + 0x48, 0xFFFF);
                w_u16(state + 0x3A, 0);
                w_u16(state + 0x38, 0);
                first = second == 0x217 ? 0x8000 : 0xC000;
            }
            w_u16(state + 4, first);
            goto finish;
        }
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x25C)) {
            second = r_u16(state + 0x20A);
            second -= 9;
            if (second < 3 && r_s16(state + 0x2E) < 0x4000 &&
                !(r_u32(state + 0x224) & 0x400000) && r_s32(state + 0x18) >= 2) {
                t3_80061a58(state, 0xFFFFFFFF);
                w_u16(state + 0x68, r_u16(state + 0x68) - 10);
                goto finish;
            }
        }
    }
    handled = 0;
    action_count = 0;
    decision = 0;
    if (r_s16(state + 0x216) >= 0 || r_s16(state + 0x88))
        goto action_dispatch;
    if (!r_s16(state + 0x6C) && !r_s16(state + 0x50)) {
        decision = 2;
        goto action_dispatch;
    }
    if (!r_u8(state + 0x94)) {
        if (!r_s16(state + 0x20C) || r_s32(state + 0x18) >= 4)
            goto action_dispatch;
    }
    opponent = r_u32(0x800A867C);
    action = r_u32(opponent + 0x54);
    accepted = 0;
    if (r_u8(action + 0x2D)) {
        first = r_u8(action + 0x2E);
        counter = r_s16(opponent + 0x58);
        accepted = (sint32)first < counter;
    }
    if (!accepted) {
        current_actor = r_u32(0x800AFA5C);
        accepted = r_s16(current_actor + 0x40) == 2;
    }
    if (accepted)
        w_u8(state + 0x94, 2);
    if (!r_u8(state + 0x94)) {
        first = r_u32(state + 0x234);
        if (first == 0x607 || first == 0x706 || (r_u32(state + 0x230) & 0x200000)) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x272))
                w_u8(state + 0x94, 1);
        }
    }
    if (r_s16(state + 0x8C) < 0) {
        first = r_u8(state + 0x94);
        if (first != 2 && first) {
            decision = 3;
            goto action_dispatch;
        }
    }
    first = r_u16(state + 0x20A);
    first -= 8;
    if (first < 2 && !r_u32(state + 0x228)) {
        current_actor = r_u32(0x800AFA5C);
        if (!r_u8(current_actor + 0xDB)) {
            first = r_s16(current_actor + 0x16);
            if (r_s16(0x80022D14 + first * 16)) {
                opponent = r_u32(0x800A867C);
                action = r_u32(opponent + 0x54);
                if (!((r_u32(action + 0x24) >> 8) & 8) && !r_u8(opponent + 0xDB)) {
                    accepted = 0;
                    if (r_s16(state + 0x20C) && !r_s16(state + 0x6A) &&
                        r_s16(current_actor + 0x3E) < 0x5000 && r_s32(state + 0x1C) < 2951) {
                        first = r_u32(action + 8) & 0xFFFF0000;
                        second = r_u32(state + 0x18);
                        if (!(first & 0x1C0000))
                            first |= 0xC0000;
                        accepted = (first & r_u32(0x80098230 + second * 4)) != 0;
                    }
                    if (accepted) {
                        random_value = r_u32(0x800ADCA0);
                        w_u32(0x800ADCA0, random_value * 5 + 3);
                        if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x292)) {
                            decision = 5;
                            goto action_dispatch;
                        }
                    }
                }
            }
        }
    }
    current_actor = r_u32(0x800AFA5C);
    if (r_s16(current_actor + 0x16) == 9) {
        counter = r_s16(state + 0x20A);
        if (counter < 27 && counter >= 22 && !r_u32(state + 0x228) && !r_u8(current_actor + 0xDB)) {
            accepted = 0;
            if (r_s16(state + 0x20C) && !r_s16(state + 0x6A) &&
                r_s16(current_actor + 0x3E) < 0x5000 && r_s32(state + 0x1C) < 2951) {
                opponent = r_u32(0x800A867C);
                first = r_u32(opponent + 0x54);
                first = r_u32(first + 8) & 0xFFFF0000;
                second = r_u32(state + 0x18);
                if (!(first & 0x1C0000))
                    first |= 0xC0000;
                accepted = (first & r_u32(0x80098230 + second * 4)) != 0;
            }
            if (accepted) {
                random_value = r_u32(0x800ADCA0);
                w_u32(0x800ADCA0, random_value * 5 + 3);
                if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x292)) {
                    decision = 6;
                    goto action_dispatch;
                }
            }
        }
    }
    first = r_u16(state + 0x20A);
    first -= 4;
    if (first < 7 && !r_u32(state + 0x228)) {
        current_actor = r_u32(0x800AFA5C);
        if (!r_u8(current_actor + 0xDB)) {
            opponent = r_u32(0x800A867C);
            if (!r_u8(opponent + 0xDB)) {
                accepted = 0;
                if (r_s16(state + 0x20C) && !r_s16(state + 0x6A) &&
                    r_s16(current_actor + 0x3E) < 0x5000 && r_s32(state + 0x1C) < 2951) {
                    first = r_u32(opponent + 0x54);
                    first = r_u32(first + 8) & 0xFFFF0000;
                    second = r_u32(state + 0x18);
                    if (!(first & 0x1C0000))
                        first |= 0xC0000;
                    accepted = (first & r_u32(0x80098230 + second * 4)) != 0;
                }
                if (accepted) {
                    random_value = r_u32(0x800ADCA0);
                    w_u32(0x800ADCA0, random_value * 5 + 3);
                    if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x292)) {
                        first = r_u32(state + 0x234);
                        if (first == 0x412 || first == 0x217 || first == 0x607 || first == 0x706) {
                            current_actor = r_u32(0x800AFA5C);
                            first = r_s16(current_actor + 0x16);
                            if (r_u32(0x80022D14 + first * 16 + 4)) {
                                decision = 7;
                                goto action_dispatch;
                            }
                            first = r_u32(state + 0x234);
                        }
                        if (first == 0x10F) {
                            current_actor = r_u32(0x800AFA5C);
                            first = r_s16(current_actor + 0x16);
                            if (r_u32(0x80022D14 + first * 16 + 8)) {
                                decision = 8;
                                goto action_dispatch;
                            }
                        }
                    }
                }
            }
        }
    }
    if (r_u32(state + 0x22C) & 0x400000) {
        opponent = r_u32(0x800A867C);
        counter = r_s16(opponent + 0xA0);
        first = r_s16(opponent + 0x16);
        accepted = counter == 0x189 || counter == 0x18E || counter == 0x17F || counter == 0x181 ||
            (first == 9 && counter == 0x172) || (!first && counter == 0x172) ||
            (first == 3 && counter == 0x152);
        if (accepted) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x272)) {
                decision = 1;
                goto action_dispatch;
            }
        }
        if ((r_u32(state + 0x22C) & 0x400000) && r_s16(state + 0x20A) < 30) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x296)) {
                w_u8(state + 0x92, 2);
                decision = 2;
                goto action_dispatch;
            }
        }
    }
    first = r_u16(state + 0x22C);
    accepted = first && first != 0x607;
    if (!accepted)
        accepted = r_s16(state + 0x20E) > 0;
    if (accepted) {
        if (r_s16(state + 0x6A) || r_s16(state + 0x50))
            goto action_dispatch;
        if (r_u8(state + 0x92)) {
            decision = 2;
            goto action_dispatch;
        }
        random_value = r_u32(0x800ADCA0);
        first = random_value * 5 + 3;
        counter = r_s16(state + 0x254);
        w_u32(0x800ADCA0, first);
        accepted = (sint32)(random_value & 0xFFF) < counter;
        if (!accepted && r_s16(state + 0x20E) >= 31) {
            counter = r_s16(state + 0x256);
            w_u32(0x800ADCA0, first * 5 + 3);
            accepted = (sint32)(first & 0xFFF) < counter;
        }
        if (accepted) {
            w_u8(state + 0x92, 5);
            decision = 2;
            goto action_dispatch;
        }
    }
    if (r_s16(state + 0x6C) >= 9) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if (!(random_value & 31)) {
            decision = 1;
            goto action_dispatch;
        }
    }
    if (r_s16(state + 0x202)) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if (!(random_value & 15)) {
            decision = 1;
            goto action_dispatch;
        }
    }
    opponent = r_u32(0x800A867C);
    first = r_u32(opponent + 0x54);
    second = r_u32(state + 0x1DC);
    if (first == second) {
        if (r_s16(state + 0x88))
            goto sustained_attack_choice;
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x25A)) {
            decision = 1;
            goto action_dispatch;
        }
    }
    if (!r_s16(state + 0x88)) {
        random_value = r_u32(0x800ADCA0);
        first = random_value * 5 + 3;
        counter = r_s16(state + 0x258);
        w_u32(0x800ADCA0, first);
        if ((sint32)(random_value & 0xFFF) < counter)
            goto choose_standing_counter;
    }

sustained_attack_choice:
    if (r_s16(state + 0x20E) >= 31) {
        random_value = r_u32(0x800ADCA0);
        first = random_value * 5 + 3;
        counter = r_s16(state + 0x25A);
        w_u32(0x800ADCA0, first);
        if ((sint32)(random_value & 0xFFF) < counter)
            goto choose_standing_counter;
    }
    first = r_u16(state + 0x20A);
    first -= 9;
    if (first >= 3 || r_s16(state + 0x2E) >= 0x4000 || (r_u32(state + 0x224) & 0x400000))
        goto action_dispatch;
    if (r_u32(state + 0x18) != 2 || (r_u32(state + 0x224) & 1))
        goto action_dispatch;
    current_actor = r_u32(0x800AFA5C);
    if (r_u8(current_actor + 0xDB) || r_s16(state + 0x50) || r_u32(state + 0x234) != 0x412)
        goto action_dispatch;
    random_value = r_u32(0x800ADCA0);
    w_u32(0x800ADCA0, random_value * 5 + 3);
    if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x25C))
        decision = 4;
    goto action_dispatch;

choose_standing_counter:
    decision = 1;
    if (r_u32(state + 0x234) == 0x412) {
        w_u32(0x800ADCA0, first * 5 + 3);
        if (!(first & 0x3F))
            decision = 9;
    }

action_dispatch:
    switch (decision) {
    case 0:
        break;
    case 1:
        if (!r_s16(state + 0x20C)) {
            action_count = t3_80057538(state);
        } else {
            first = r_u32(state + 0x234);
            if (first == 0x607 || first == 0x706 || (r_u32(state + 0x230) & 0x200001))
                action_count = t3_80057e94(state);
            else
                action_count = t3_80057d58(state);
        }
        if (r_u32(state + 0x234) == 0x412) {
            added = 0;
            limit = r_s16(state + 0x70);
            if (limit > 0) {
                for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                    if (r_s16(entry + 8))
                        continue;
                    action = r_u32(entry);
                    first = r_u8(action + 0x2D);
                    flags = r_u32(action + 4);
                    if (!first || (flags & 0x20001) != 1)
                        continue;
                    first = r_u32(action + 8);
                    second = r_u32(state + 0x14);
                    first &= 0xFFFF0000;
                    if (!(first & 0x1C0000))
                        first |= 0xC0000;
                    if (!(first & r_u32(0x80098230 + second * 4)))
                        continue;
                    w_u16(entry + 8, 1);
                    ++added;
                }
                w_u16(state + 0x72, r_u16(state + 0x72) + added);
            }
            action_count += added;
        }
        if ((sint32)action_count > 0)
            handled = 1;
        break;
    case 2:
        action_count = t3_80057fb8(state);
        handled = 1;
        if ((sint32)action_count > 0)
            break;
        current_actor = r_u32(0x800AFA5C);
        first = 0;
        if (!r_s16(current_actor + 0x32)) {
            second = r_u32(state + 0x234);
            w_u16(state + 0x40, 40);
            w_u16(state + 0x48, 0xFFFF);
            w_u16(state + 0x3A, 0);
            w_u16(state + 0x38, 0);
            first = second == 0x217 ? 0x8000 : 0xC000;
        }
        w_u16(state + 4, first);
        goto finish;
    case 3:
        if (r_s16(state + 0x21A))
            goto action_decision_done;
        first = r_u8(state + 0x94);
        distance = r_u32(state + 0x1C);
        if (first == 1) {
            accepted = r_s32(state + 0x14) >= 3;
            if (!accepted) {
                random_value = r_u32(0x800ADCA0);
                w_u32(0x800ADCA0, random_value * 5 + 3);
                accepted = (sint32)(random_value & 0xFFF) < r_s16(state + 0x27E);
            }
            first = 3;
            if (accepted) {
                random_result = (uint32)randPSX();
                random_value = r_u32(0x800ADCA0);
                first = ((random_result + random_value) & 0x7FFF) * 7;
                w_u32(0x800ADCA0, random_value * 5 + 3);
                first = r_u8(0x80022E80 + (first >> 15) * 2);
            }
            w_u8(state + 0x94, first);
        }
        opponent = r_u32(0x800A867C);
        if (r_s16(opponent + 0x16) == 4)
            distance -= 300;
        first = r_u8(state + 0x94);
        first -= 1;
        if (first >= 2 && (sint32)distance >= 2951) {
            w_u8(state + 0x94, 0);
            w_u16(state + 4, 0);
            handled = 1;
            goto action_decision_done;
        }
        switch (r_u8(state + 0x94)) {
        case 3:
            w_u16(state + 0x40, 40);
            opponent = r_u32(0x800A867C);
            w_u16(state + 0x3A, 0);
            w_u16(state + 0x38, 0);
            w_u16(state + 0x48, 0xFFFF);
            action = r_u32(opponent + 0x54);
            if (r_u8(action + 0x2D)) {
                counter = r_s16(opponent + 0x58);
                first = r_u8(action + 0x2E);
                if ((sint32)first < counter) {
                    handled = 0;
                    break;
                }
            }
            counter = r_s16(state + 0x20A);
            if (counter >= 29) {
                if (r_s32(state + 0x14) >= 3) {
                    first = t3_800592fc(state);
                    random_value = r_u32(0x800ADCA0);
                    w_u16(state + 4, first);
                    w_u32(0x800ADCA0, random_value * 5 + 3);
                    w_u16(state + 0x68, random_value & 15);
                    handled = 1;
                } else {
                    handled = (sint32)t3_800573f4(state) <= 0;
                }
            } else if (counter >= 9) {
                if ((sint32)distance >= 2951) {
                    w_u16(state + 4, 0);
                    handled = 1;
                } else if (r_s32(state + 0x14) < 3) {
                    first = t3_800573f4(state);
                    handled = 1;
                    if ((sint32)first > 0) {
                        w_u16(state + 0x68, 0xFFFF);
                        handled = 0;
                    }
                } else {
                    current_actor = r_u32(0x800AFA5C);
                    handled = 0;
                    if (r_s16(current_actor + 0x3E) < 0x4000) {
                        w_u8(state + 0x94, 4);
                        w_u16(state + 4, 0x8000);
                        handled = 1;
                    }
                }
            } else {
                first = 0;
                if ((sint32)distance < 2951) {
                    current_actor = r_u32(0x800AFA5C);
                    if (r_s16(current_actor + 0x3E) < 0x4000)
                        first = 0x8000;
                }
                w_u16(state + 4, first);
                handled = 1;
            }
            break;
        case 4:
            w_u16(state + 0x40, 40);
            current_actor = r_u32(0x800AFA5C);
            w_u16(state + 0x3A, 0);
            w_u16(state + 0x38, 0);
            w_u16(state + 0x48, 0xFFFF);
            handled = 1;
            if (r_s16(current_actor + 0x3E) >= 0x4001 && r_s32(state + 0x18) >= 2) {
                w_u16(state + 4, 0x2000);
            } else if ((sint32)distance >= 2951) {
                w_u16(state + 4, 0);
            } else if (r_s16(state + 0x20A) >= 9 && r_s32(state + 0x14) < 2) {
                if ((sint32)t3_800573f4(state) > 0) {
                    w_u16(state + 0x68, 0xFFFF);
                    handled = 0;
                }
            } else {
                t3_8006183c(state, 0x80022C46);
            }
            break;
        case 5:
            accepted = (r_u32(state + 0x224) & 0x400000) != 0;
            if (!accepted) {
                random_value = r_u32(0x800ADCA0);
                w_u32(0x800ADCA0, random_value * 5 + 3);
                accepted = (random_value & 7) != 0;
            }
            if (!accepted)
                accepted = r_s16(state + 0x20A) < 7;
            if (accepted) {
                w_u16(state + 4, 0x2000);
                handled = 0;
            } else {
                current_actor = r_u32(0x800AFA5C);
                if (r_s16(current_actor + 0x3E) >= 0x4000)
                    w_u8(state + 0x94, 3);
                t3_80061a58(state, 0xFFFFFFFF);
                handled = 1;
            }
            break;
        case 6:
            w_u16(state + 0x40, 40);
            w_u16(state + 0x48, 0xFFFF);
            w_u16(state + 0x3A, 0);
            w_u16(state + 0x38, 0);
            handled = 1;
            if ((sint32)distance >= 2951) {
                w_u16(state + 4, 0);
            } else {
                counter = r_s16(state + 0x20A);
                if (counter >= 29) {
                    w_u16(state + 4, r_s32(state + 0x18) > 0 ? 0x8000 : 0);
                } else if (r_s32(state + 0x18) < 2 && counter >= 8) {
                    w_u16(state + 4, 0x3000);
                    w_u16(state + 0x48, 20);
                } else {
                    t3_8006183c(state, 0x80022C46);
                }
            }
            break;
        default:
            handled = 0;
            break;
        }
        goto action_decision_done;
    case 4:
        t3_80061a58(state, 0xFFFFFFFF);
        w_u16(state + 0x68, r_u16(state + 0x68) - 10);
        goto finish;
    case 5:
    case 6:
        added = 0;
        limit = r_s16(state + 0x70);
        if (limit > 0) {
            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                if (r_s16(entry + 8))
                    continue;
                first = r_u32(entry + 4);
                opcode = r_u16(first + 6);
                if (decision == 5 ? opcode == 0x1EE || opcode == 0x6A3 : opcode == 0x22C) {
                    w_u16(entry + 8, 1);
                    ++added;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
        }
        action_count = added;
        if ((sint32)action_count > 0)
            handled = 1;
        break;
    case 7:
    case 8:
        current_actor = r_u32(0x800AFA5C);
        first = r_s16(current_actor + 0x16);
        first = r_u32(0x80022D14 + first * 16 + (decision == 7 ? 4 : 8));
        t3_8006183c(state, first);
        goto finish;
    case 9:
        added = 0;
        limit = r_s16(state + 0x70);
        if (limit > 0) {
            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                if (r_s16(entry + 8))
                    continue;
                action = r_u32(entry);
                first = r_u32(action + 4);
                second = r_u8(action + 0x2D);
                if ((first & 1) == 1 && !second) {
                    w_u16(entry + 8, 1);
                    ++added;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
        }
        action_count = added;
        if ((sint32)action_count > 0) {
            handled = 1;
            w_u8(state + 0x8E, 1);
            w_u16(state + 0x68, 0xFFFF);
        }
        break;
    }
    if (handled && (sint32)action_count > 0) {
        if ((sint32)t3_80058a84(state) >= 0) {
            w_u16(state + 0x40, 40);
            w_u16(state + 0x3A, 0);
            w_u16(state + 0x38, 0);
            w_u16(state + 0x48, 0xFFFF);
        }
    }

action_decision_done:
    if (handled)
        goto finish;
    output_cursor = 0x800A01E8;
    current_actor = r_u32(0x800AFA5C);
    opponent = r_u32(0x800A867C);
    counter = r_s16(state + 0xC0);
    flags = r_u32(state + 0x64);
    if (counter > 0) {
        for (row = 0; (sint32)row < r_s16(state + 0xC2); ++row) {
            entry = r_u32(state + 0xC4 + row * 4);
            counter = r_s16(current_actor + 0x58);
            first = r_u8(entry + 9);
            if (counter < (sint32)first || (sint32)r_u8(entry + 10) < counter)
                continue;
            condition = r_u8(entry + 2);
            if (!t3_ai_condition(condition, current_actor, opponent))
                continue;
            if (t3_ai_command_condition(entry, current_actor, opponent, flags)) {
                w_u32(output_cursor, entry);
                output_cursor += 4;
            }
        }
        /* The original candidate count stays zero on every reachable loop path */
        w_u16(state + 0xC0, 0);
        w_u16(state + 0xC2, 0);
        w_u16(state + 0xD4, 1);
    }
    output_cursor = 0x800A01E8;
    current_actor = r_u32(0x800AFA5C);
    opponent = r_u32(0x800A867C);
    counter = r_s16(state + 0xD8);
    flags = r_u32(state + 0x64);
    if (counter > 0) {
        for (row = 0; (sint32)row < r_s16(state + 0xDA); ++row) {
            entry = r_u32(state + 0xDC + row * 4);
            counter = r_s16(current_actor + 0x58);
            first = r_u8(entry + 9);
            if (counter < (sint32)first || (sint32)r_u8(entry + 10) < counter)
                continue;
            condition = r_u8(entry + 2);
            if (!t3_ai_condition(condition, current_actor, opponent))
                continue;
            if (t3_ai_command_condition(entry, current_actor, opponent, flags)) {
                first = r_u32(state + 0xDC + row * 4);
                w_u32(output_cursor, first);
                output_cursor += 4;
            }
        }
        w_u16(state + 0xD8, 0);
        w_u16(state + 0xDA, 0);
    }
    current_actor = r_u32(0x800AFA5C);
    opponent = r_u32(0x800A867C);
    counter = r_s16(state + 0x1A0);
    flags = r_u32(state + 0x64);
    if (counter >= 0) {
        for (row = 0; (sint32)row < r_s16(state + 0x1A2); ++row) {
            entry = r_u32(state + 0x1A4 + row * 4);
            counter = r_s16(current_actor + 0x58);
            first = r_u8(entry + 9);
            if (counter < (sint32)first || (sint32)r_u8(entry + 10) < counter)
                continue;
            condition = r_u8(entry + 2);
            if (!t3_ai_condition(condition, current_actor, opponent))
                continue;
            if (t3_ai_command_condition(entry, current_actor, opponent, flags)) {
                first = r_u32(state + 0x1A4 + row * 4);
                w_u32(0x800A01E8, first);
            }
        }
        w_u16(state + 0x1A0, 0);
        w_u16(state + 0x1A2, 0);
    }
    if (!r_s16(state + 0x50)) {
        opponent = r_u32(0x800A867C);
        action = r_u32(opponent + 0x54);
        if (r_u8(action + 0x19) && r_s32(state + 0x18) < 2) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x280)) {
                if (r_s16(state + 0x2E) < 0x4000 && r_s32(state + 0x24) < -10) {
                    w_u16(state + 4, 0x6000);
                    goto finish;
                }
                opponent = r_u32(0x800A867C);
                first = r_u32(opponent + 0x54);
                counter = r_s16(opponent + 0x58);
                first = r_u8(first + 0x1A);
                if ((sint32)(first - (uint32)counter) >= 21 && r_s16(state + 0x2E) >= 0x4001) {
                    w_u16(state + 4, 0x2000);
                    goto finish;
                }
            }
        }
        if (r_s16(state + 0x2E) >= 0x6001 && r_s32(state + 0x18) < 2) {
            opponent = r_u32(0x800A867C);
            if (r_u8(opponent + 0xDB))
                goto no_input;
            if (r_s16(state + 0x208)) {
                first = r_u16(state + 2) & 1 ? 0x4040 : 0x4020;
                w_u16(state + 4, first);
                goto finish;
            }
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x27E)) {
                current_actor = r_u32(0x800AFA5C);
                first = 0x2000;
                if (r_s16(current_actor + 0x16) == 7)
                    first = r_s16(state + 0x2E) < 0x4000 ? 0x2000 : 0x8000;
                w_u16(state + 4, first);
                goto finish;
            }
            w_u16(state + 0x68, 0xFFFE);
            goto directional_defense_done;
        }
        if (r_s16(state + 0x2E) >= 0x1001) {
            random_value = r_u32(0x800ADCA0);
            first = random_value * 5 + 3;
            counter = r_s16(state + 0x2E);
            w_u32(0x800ADCA0, first);
            if ((sint32)(random_value & 0x7FFF) >= counter)
                goto directional_defense_done;
            threshold = r_s16(state + 0x27E);
            w_u32(0x800ADCA0, first * 5 + 3);
            if ((sint32)(first & 0xFFF) < threshold)
                goto directional_defense_done;
            w_u16(state + 0x38, 8);
            current_actor = r_u32(0x800AFA5C);
            w_u16(state + 0x3A, 0);
            w_u16(state + 0x40, 48);
            if (r_s16(current_actor + 0x16) == 7) {
                first = r_s16(state + 0x2E) < 0x4000 ? 0x2000 : 0x8000;
                w_u16(state + 4, first);
                goto finish;
            }
            if (r_s16(state + 0x20C)) {
                random_value = r_u32(0x800ADCA0);
                w_u32(0x800ADCA0, random_value * 5 + 3);
                if (!(random_value & 7)) {
                    current_actor = r_u32(0x800AFA5C);
                    first = 0;
                    if (!r_s16(current_actor + 0x32)) {
                        second = r_u32(state + 0x234);
                        w_u16(state + 0x40, 40);
                        w_u16(state + 0x48, 0xFFFF);
                        w_u16(state + 0x3A, 0);
                        w_u16(state + 0x38, 0);
                        first = second == 0x217 ? 0x8000 : 0xC000;
                    }
                    w_u16(state + 4, first);
                    goto finish;
                }
            }
            first = 0x2000;
            if (r_s32(state + 0x18) < 3 && (r_u16(state + 2) & 1))
                first = 0x8000;
            w_u16(state + 4, first);
            goto finish;
        }
    }

directional_defense_done:
    if (r_u32(0x800AFA88) == 8) {
        first = r_u16(state);
        if (r_s32(0x8009F2F8 + first * 4 + 8) > 0) {
            handled = 0;
            goto attack_selection_done;
        }
        first = t3_bns_02_800b297c();
        current_actor = r_u32(0x800AFA5C);
        second = r_u8(first + 0x1F);
        first = r_u8(current_actor + 0x1E);
        if (second != first) {
            random_value = r_u32(0x800ADCA0);
            first = r_u32(state + 0x32C);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((sint32)(random_value & 0xFFF) < r_s16(first + 4)) {
                handled = 0;
                goto attack_selection_done;
            }
        }
    }
    counter = r_s16(state + 0x4C);
    first = r_u16(state + 0x4C);
    if (counter > 0) {
        second = r_u16(state + 6);
        w_u16(state + 0x4C, first - 1);
        w_u16(state + 4, second & 0xF000);
        handled = 1;
        goto attack_selection_done;
    }
    if (r_s16(state + 0x44) >= 5) {
        w_u16(state + 4, r_u16(state + 6) & 0xF000);
        handled = 1;
        goto attack_selection_done;
    }
    band = r_u32(state + 0x14);
    if ((sint32)band >= 4 || r_s16(state + 0x21A)) {
        handled = 0;
        goto attack_selection_done;
    }
    counter = r_s16(state + 0x1D2);
    threshold = r_s16(state + 0x1D4);
    if (counter >= threshold) {
        handled = 0;
        goto attack_selection_done;
    }
    if ((sint32)band >= 3) {
        random_result = (uint32)randPSX();
        random_value = r_u32(0x800ADCA0);
        first = ((random_result + random_value) & 0x7FFF) * 12;
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if (first >> 15) {
            handled = 0;
            goto attack_selection_done;
        }
    }
    if (r_s32(state + 0x14) >= 2) {
        random_result = (uint32)randPSX();
        random_value = r_u32(0x800ADCA0);
        first = ((random_result + random_value) & 0x7FFF) * 8;
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if (!(first >> 15)) {
            handled = 0;
            goto attack_selection_done;
        }
    }
    if (r_s16(state + 0x3A)) {
        if (r_s16(state + 0x6C) > 0 || (r_s16(state + 0x20C) && r_s32(state + 0x18) < 4)) {
            w_u16(state + 4, 0x8000);
            handled = 1;
            goto attack_selection_done;
        }
        if (r_u32(state + 0x230) & 4) {
            random_result = (uint32)randPSX();
            random_value = r_u32(0x800ADCA0);
            first = ((random_result + random_value) & 0x7FFF) * 4096;
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((first >> 15) < 256) {
                handled = 0;
                goto attack_selection_done;
            }
        }
    }
    attack_pressure = r_s16(state + 0x68);
    first = r_s16(state + 0xD4);
    preference_threshold = r_s16(state + 0x248);
    action_count = 0;
    if (first) {
        added = 0;
        for (row = 0, entry = 0x8009F978; (sint32)row < r_s16(state + 0x70); ++row, entry += 12) {
            counter = r_s16(entry + 8);
            if (counter < 0)
                continue;
            action = r_u32(entry);
            if (!((r_u32(action + 0x24) >> 8) & 0x20))
                continue;
            if (counter > 0)
                ++added;
            w_u16(entry + 8, 0xFFFF);
        }
        w_u16(state + 0x72, r_u16(state + 0x72) - added);
    }
    accepted = 0;
    if (r_s32(state + 0x80) >= 26) {
        counter = r_s16(state + 0x6C);
        if (counter >= 0) {
            accepted = 1;
        } else if (!r_s16(state + 0x6A)) {
            random_value = r_u32(0x800ADCA0);
            counter = r_s16(state + 0x20A);
            counter -= (sint32)(random_value & 7) + 4;
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if (counter < 3)
                counter = 3;
            w_u16(state + 0x6C, counter);
        }
    }
    if (!accepted)
        counter = r_s16(state + 0x6C);
    if (counter > 0) {
        action_count = 0;
        goto attack_candidates_ready;
    }
    accepted = r_u8(state + 0x8F) != 0 || r_u8(state + 0x91) != 0;
    if (!accepted && r_s32(state + 0x80) >= 26) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        first = r_u16(state + 0x27E);
        threshold = (sint16)first / 2;
        accepted = (sint32)(random_value & 0xFFF) >= threshold;
    }
    if (!accepted) {
        first = r_u32(state + 0x84) - 1;
        if (first < 19) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            accepted = (sint32)(random_value & 0xFFF) >= r_s16(state + 0x27E);
        }
    }
    if (accepted)
        attack_pressure = -1;
    random_value = r_u32(0x800ADCA0);
    w_u32(0x800ADCA0, random_value * 5 + 3);
    if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x280)) {
        opponent = r_u32(0x800A867C);
        if (r_u8(opponent + 0xDB) && r_u16(state + 0x230) != 0x4C02) {
            action = r_u32(opponent + 0x54);
            first = r_u8(action + 0x1A);
            second = r_u8(action + 0x19);
            counter = r_s16(opponent + 0x58);
            threshold = (sint32)second + ((sint32)first - (sint32)second) / 2;
            if (counter >= threshold && (sint32)first >= counter &&
                r_s16(state + 0x2E) < 0x4001 && r_u32(state + 0x14) == 2)
                w_u16(state + 0x68, 0xFFFF);
        }
    }
    opponent = r_u32(0x800A867C);
    if (t3_80061a44(opponent) && r_s32(state + 0x14) < 3) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x258))
            w_u16(state + 0x68, r_u16(state + 0x68) - 2);
    }
    accepted = (r_u32(state + 0x230) & 0x605) != 0;
    if (!accepted) {
        opponent = r_u32(0x800A867C);
        accepted = r_u8(opponent + 0xDB) || r_u8(opponent + 0xDF) ||
            r_s16(opponent + 0x16) == 19 || r_s16(state + 0x20A) < 10;
    }
    if (accepted) {
        preference_threshold = 0;
        w_u8(state + 0x91, 0);
    }
    if (r_u32(0x800AFA88) == 7) {
        first = t3_bns_03_800b4720(state);
        if ((sint32)first > 0)
            goto select_default_attack;
    }
    if (r_s16(state + 0x208) && !r_s16(state + 0x88)) {
        if (r_s16(state + 0x50)) {
            action_count = 0;
            goto attack_candidates_ready;
        }
        random_value = r_u32(0x800ADCA0);
        first = random_value * 5 + 3;
        threshold = r_s16(state + 0x27E);
        w_u32(0x800ADCA0, first);
        if ((sint32)(random_value & 0xFFF) >= threshold)
            attack_pressure = -1;
        filter_kind = 0;
        if (r_s16(state + 0x2E) >= 0x4001) {
            w_u32(0x800ADCA0, first * 5 + 3);
            if (!(first & 15))
                filter_kind = 6;
        }
        if (!filter_kind) {
            if (attack_pressure >= 0) {
                action_count = 0;
                goto attack_candidates_ready;
            }
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            first = random_value & 0xFFF;
            if (first < 0x334) {
                filter_kind = 1;
            } else if (first < 0x3AF) {
                t3_80061a58(state, random_value & 1);
                action_count = 0xFFFFFFFF;
                goto attack_candidates_ready;
            } else if (first < 0x6E2) {
                filter_kind = 2;
            } else if (first < 0x75D && !(r_u32(state + 0x224) & 1)) {
                filter_kind = 3;
            } else if (first < 0x829) {
                filter_kind = 4;
            } else {
                second = r_u32(state + 0x14) - 2;
                if (second < 2 && first < 0x9C3) {
                    t3_8006183c(state, 0x80022C2A);
                    action_count = 0xFFFFFFFF;
                    goto attack_candidates_ready;
                }
                filter_kind = 5;
            }
        }
        added = 0;
        limit = r_s16(state + 0x70);
        if (limit > 0) {
            if (filter_kind == 4) {
                counter = 1;
                entry = 0x8009F978;
                do {
                    first = r_s16(entry + 8);
                    action = r_u32(entry + 4);
                    opcode = r_u16(action);
                    if (!first && opcode == 0xC002) {
                        w_u16(entry + 8, 1);
                        ++added;
                    }
                    accepted = counter < limit;
                    ++counter;
                    entry += 12;
                } while (accepted);
            } else {
                for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                    if (r_s16(entry + 8))
                        continue;
                    action = r_u32(entry);
                    accepted = 0;
                    switch (filter_kind) {
                    case 1:
                        first = r_u32(action + 0xC);
                        if (r_u8(first + 3) == 0x35)
                            accepted = !(r_u32(action + 8) & 0x400000);
                        break;
                    case 2:
                    case 5:
                        first = r_u8(action + 0x2D);
                        flags = r_u32(action + 8);
                        if (first < 81 && ((flags & 0x10000) || (uint16)flags == 0x51F ||
                            (filter_kind == 5 && (uint16)flags == 0x10F)))
                            accepted = !(flags & 0x400000);
                        break;
                    case 3:
                        flags = r_u32(action + 4);
                        first = r_u8(action + 0x2D);
                        accepted = (flags & 1) == 1 && !first;
                        break;
                    case 6:
                        accepted = (r_u32(action + 4) & 0x20000) != 0;
                        break;
                    }
                    if (accepted) {
                        w_u16(entry + 8, 1);
                        ++added;
                    }
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
        }
        action_count = added;
        goto attack_candidates_ready;
    }
    if (r_u16(state + 0x230) == 0x4C02) {
        random_value = r_u32(0x800ADCA0);
        first = random_value * 5 + 3;
        threshold = r_s16(state + 0x274);
        w_u32(0x800ADCA0, first);
        if ((sint32)(random_value & 0xFFF) < threshold) {
            band = r_u32(state + 0x14);
            action_count = 0;
            filter_kind = 0;
            if ((sint32)band >= 3) {
                accepted = 1;
                if (r_s32(state + 0x24) < 51) {
                    w_u32(0x800ADCA0, first * 5 + 3);
                    accepted = (first & 7) != 0;
                }
                filter_kind = accepted ? 7 : 8;
            } else if ((sint32)band >= 2) {
                w_u32(0x800ADCA0, first * 5 + 3);
                filter_kind = (first & 15) ? 8 : 7;
            }
            if (filter_kind == 7) {
                action_count = t3_ai_mark_command(state, 0xC001);
                if ((sint32)band >= 3 && (sint32)action_count > 0) {
                    w_u16(state + 0x3A, 2);
                    w_u16(state + 0x38, 250);
                    w_u16(state + 0x40, 310);
                }
            } else if (filter_kind == 8) {
                added = 0;
                limit = r_s16(state + 0x70);
                if (limit > 0) {
                    for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                        if (r_s16(entry + 8))
                            continue;
                        action = r_u32(entry);
                        if ((r_u32(action + 8) & 0x10000) && !r_u8(action + 0x2D) && !r_u8(action + 0x19)) {
                            w_u16(entry + 8, 1);
                            ++added;
                        }
                    }
                    w_u16(state + 0x72, r_u16(state + 0x72) + added);
                }
                action_count = added;
            }
            if (r_s32(state + 0x14) >= 4 || r_s16(state + 0x21A))
                goto attack_candidates_ready;
            random_value = r_u32(0x800ADCA0);
            first = random_value * 5 + 3;
            threshold = r_s16(state + 0x27E);
            w_u32(0x800ADCA0, first);
            if ((sint32)(random_value & 0xFFF) < threshold)
                goto attack_candidates_ready;
            opponent = r_u32(0x800A867C);
            w_u16(state + 0x68, 0xFFFF);
            accepted = 0;
            if (!r_u8(opponent + 0xDB)) {
                w_u32(0x800ADCA0, first * 5 + 3);
                if ((first & 0x3F) < 32 && !r_u32(state + 0x7C))
                    accepted = 1;
            }
            if (accepted)
                action_count += t3_800579a0(state);
            else
                action_count += t3_800577dc(state);
            goto attack_candidates_ready;
        }
    }
    if (r_s16(state + 0x46) >= 0) {
        added = 0;
        limit = r_s16(state + 0x70);
        if (limit > 0) {
            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                if (r_s16(entry + 8))
                    continue;
                action = r_u32(entry);
                if (!r_u8(action + 0x2D))
                    continue;
                flags = r_u32(action + 4);
                accepted = (flags & 0x80000) != 0;
                if (!accepted)
                    accepted = (r_u32(action + 8) & 0x200000) != 0;
                if (accepted && !(flags & 0x20000)) {
                    w_u16(entry + 8, 1);
                    ++added;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
        }
        action_count = added;
        if ((sint32)action_count > 0)
            w_u16(state + 0x46, 0xFFFF);
        goto attack_candidates_ready;
    }
    accepted = r_s16(state + 0x20E) != 0;
    if (!accepted && (r_s16(state + 0x20C) || r_s16(state + 0x214)))
        accepted = !r_s16(state + 0x88);
    if (accepted) {
        accepted = r_u8(state + 0x8F) != 0;
        if (!accepted) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            accepted = (sint32)(random_value & 0xFFF) < r_s16(state + 0x258);
        }
        if (accepted) {
            action_count = t3_800587d8(state);
            goto attack_candidates_ready;
        }
        if (attack_pressure > 0)
            goto attack_candidates_ready;
        if (r_u32(state + 0x230) & 1) {
            action_count = t3_800573f4(state);
            goto attack_candidates_ready;
        }
        goto select_default_attack;
    }
    if (attack_pressure > 0) {
        action_count = 0;
        if (r_s32(state + 0x14) >= 3 || (r_u32(state + 0x224) & 1))
            goto attack_candidates_ready;
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((random_value & 0xFFF) >= 4)
            goto attack_candidates_ready;
        added = 0;
        limit = r_s16(state + 0x70);
        if (limit > 0) {
            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                if (r_s16(entry + 8))
                    continue;
                action = r_u32(entry);
                flags = r_u32(action + 4);
                first = r_u8(action + 0x2D);
                if ((flags & 1) == 1 && !first) {
                    w_u16(entry + 8, 1);
                    ++added;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
        }
        action_count = added;
        if ((sint32)action_count > 0)
            goto attack_result_selected;
        goto attack_candidates_ready;
    }
    goto pressure_attack_choices;

select_default_attack:
    action_count = t3_800572a4(state);
    goto attack_candidates_ready;

pressure_attack_choices:
    if (r_s16(state + 0x44) >= 0) {
        added = 0;
        limit = r_s16(state + 0x70);
        if (limit > 0) {
            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                if (r_s16(entry + 8))
                    continue;
                action = r_u32(entry);
                if ((r_u32(action + 4) & 0xA0000) == 0x80000 && !r_u8(action + 0x19)) {
                    w_u16(entry + 8, 1);
                    ++added;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
        }
        action_count = added;
        w_u16(state + 0x44, 0xFFFF);
        goto attack_candidates_ready;
    }
    accepted = r_u8(state + 0x91) != 0;
    if (!accepted) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        accepted = (sint32)(random_value & 0xFFF) < preference_threshold;
    }
    if (accepted && r_s32(state + 0x14) < 2) {
        opponent = r_u32(0x800A867C);
        if (r_s16(opponent + 0x16) != 19) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            accepted = (sint32)(random_value & 0xFFF) < r_s16(state + 0x24E);
            action_count = 0;
            if (accepted) {
                added = 0;
                limit = r_s16(state + 0x70);
                if (limit > 0) {
                    for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                        if (r_s16(entry + 8))
                            continue;
                        action = r_u32(entry);
                        if (!(r_u32(action + 8) & 0x400000))
                            continue;
                        first = r_u32(entry + 4);
                        if ((r_u16(first) & 0xC200) != 0x200) {
                            w_u16(entry + 8, 1);
                            ++added;
                        }
                    }
                    w_u16(state + 0x72, r_u16(state + 0x72) + added);
                }
                action_count = added;
                if ((sint32)action_count > 0 || r_s32(state + 0x14) >= 2) {
                    w_u16(state + 0x98, 1);
                    goto preferred_attack_filtered;
                }
            } else if (r_s32(state + 0x14) >= 2) {
                goto preferred_attack_filtered;
            }
            added = 0;
            limit = r_s16(state + 0x70);
            if (limit > 0) {
                for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                    if (r_s16(entry + 8))
                        continue;
                    action = r_u32(entry);
                    if (!(r_u32(action + 8) & 0x400000))
                        continue;
                    first = r_u32(entry + 4);
                    if (!(r_u16(first) & 0xFFE0)) {
                        w_u16(entry + 8, 1);
                        ++added;
                    }
                }
                w_u16(state + 0x72, r_u16(state + 0x72) + added);
            }
            action_count = added;
preferred_attack_filtered:
            if ((sint32)action_count > 0)
                goto attack_candidates_ready;
        }
    }
    current_actor = r_u32(0x800AFA5C);
    first = r_s16(current_actor + 0x16);
    counter = r_s16(state + 0x42);
    stream = r_u32(0x80022D14 + first * 16 + 12);
    if (counter < 0 && stream && !r_s16(state + 0x50) && !(r_u32(state + 0x224) & 1)) {
        counter = r_s16(current_actor + 0xA0);
        threshold = r_s16(stream + 2);
        if (counter != threshold && r_s16(state + 0x2E) < 0x2001 &&
            !r_u8(current_actor + 0xDB) && r_s32(state + 0x14) < 3 && !r_s16(state + 0x208)) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            first = random_value & 0xFFF;
            threshold = r_s16(stream);
            accepted = 1;
            if (threshold < (sint32)first) {
                do {
                    stream += 8;
                    threshold = r_s16(stream);
                    if (threshold < 0) {
                        accepted = 0;
                        break;
                    }
                } while (threshold < (sint32)first);
            }
            if (accepted) {
                first = r_s16(stream + 2);
                if (t3_80058be8(first, state)) {
                    first = r_u32(stream + 4);
                    t3_8006183c(state, first + 2);
                    random_value = r_u32(0x800ADCA0);
                    w_u16(state + 0xD8, 1);
                    w_u32(0x800ADCA0, random_value * 5 + 3);
                    w_u16(state + 0x42, (random_value & 15) + 28);
                    action_count = 0xFFFFFFFF;
                    goto attack_candidates_ready;
                }
            }
        }
    }
    if (r_s32(state + 0x14) < 3) {
        current_actor = r_u32(0x800AFA5C);
        if (!r_u8(current_actor + 0xDB) && !(r_u32(state + 0x224) & 1)) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((random_value & 0x3F) < 14) {
                action_count = t3_ai_mark_standing(state);
                goto attack_candidates_ready;
            }
        }
    }
    random_value = r_u32(0x800ADCA0);
    first = random_value * 5 + 3;
    w_u32(0x800ADCA0, first);
    if ((random_value & 0x3F) < 32) {
        w_u32(0x800ADCA0, first * 5 + 3);
        if (first & 1)
            action_count = t3_ai_mark_normal(state);
        else
            action_count = t3_800577dc(state);
        goto attack_candidates_ready;
    }
    if (r_s16(state + 0x44) < 0 && r_s16(state + 0x46) < 0 && !(r_u32(state + 0x224) & 1)) {
        current_actor = r_u32(0x800AFA5C);
        if (!r_u8(current_actor + 0xDB)) {
            w_u32(0x800ADCA0, first * 5 + 3);
            if ((first & 0xFFF) < 512) {
                action_count = t3_ai_mark_standing(state);
                if ((sint32)action_count > 0) {
                    w_u16(state + 0x44, 18);
                    w_u16(state + 0x68, 0xFFFF);
                    goto attack_candidates_ready;
                }
            }
        }
    }
    action_count = t3_ai_mark_normal(state);
    random_value = r_u32(0x800ADCA0);
    first = random_value * 5 + 3;
    threshold = r_s16(state + 0x250);
    w_u32(0x800ADCA0, first);
    if ((sint32)(random_value & 0xFFF) < threshold) {
        action_count += t3_8005766c(state);
        goto attack_candidates_ready;
    }
    second = first * 5 + 3;
    threshold = r_s16(state + 0x252);
    w_u32(0x800ADCA0, second);
    if ((sint32)(first & 0xFFF) < threshold) {
        action_count += t3_80057538(state);
        goto attack_candidates_ready;
    }
    if (r_u32(state + 0x230) & 1) {
        threshold = r_s16(state + 0x27E);
        w_u32(0x800ADCA0, second * 5 + 3);
        if ((sint32)(second & 0xFFF) >= threshold) {
            action_count += t3_800573f4(state);
            goto attack_candidates_ready;
        }
    }
    random_value = r_u32(0x800ADCA0);
    w_u32(0x800ADCA0, random_value * 5 + 3);
    if ((random_value & 0x3F) < 10) {
        action_count += t3_80057b60(state);
        goto attack_candidates_ready;
    }
    current_actor = r_u32(0x800AFA5C);
    if (!r_u8(current_actor + 0xDB) && r_s16(state + 0x46) < 0) {
        random_result = (uint32)randPSX();
        random_value = r_u32(0x800ADCA0);
        first = ((random_result + random_value) & 0x7FFF) * 4096;
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((first >> 15) < 512) {
            added = 0;
            limit = r_s16(state + 0x70);
            if (limit > 0) {
                for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                    if (r_s16(entry + 8))
                        continue;
                    action = r_u32(entry);
                    if (!r_u8(action + 0x19))
                        continue;
                    flags = r_u32(action + 4);
                    if (flags & 0x20000)
                        continue;
                    accepted = (flags & 0x80000) != 0;
                    if (!accepted)
                        accepted = (r_u32(action + 8) & 0x200000) != 0;
                    if (accepted) {
                        w_u16(entry + 8, 1);
                        ++added;
                    }
                }
                w_u16(state + 0x72, r_u16(state + 0x72) + added);
            }
            action_count = added;
            if ((sint32)action_count > 0) {
                w_u16(state + 0x46, 4);
                count = 0;
                for (row = 0; row < 3; ++row) {
                    random_result = (uint32)randPSX();
                    random_value = r_u32(0x800ADCA0);
                    first = ((random_result + random_value) & 0x7FFF) * 4;
                    w_u32(0x800ADCA0, random_value * 5 + 3);
                    count += first >> 15;
                }
                w_u16(state + 0x46, count + 1);
                goto attack_candidates_ready;
            }
        }
    }
    random_result = (uint32)randPSX();
    random_value = r_u32(0x800ADCA0);
    first = ((random_result + random_value) & 0x7FFF) * 4096;
    w_u32(0x800ADCA0, random_value * 5 + 3);
    if ((first >> 15) < 10) {
        added = 0;
        limit = r_s16(state + 0x70);
        if (limit > 0) {
            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                if (r_s16(entry + 8))
                    continue;
                action = r_u32(entry);
                if (r_u8(action + 0x2D) || r_u8(action + 0x19) || (r_u32(action + 4) & 0x20000))
                    continue;
                first = r_u32(entry + 4);
                opcode = r_u16(first + 6);
                if (opcode != 0x1EE && opcode != 0x6A3) {
                    w_u16(entry + 8, 1);
                    ++added;
                }
            }
            w_u16(state + 0x72, r_u16(state + 0x72) + added);
        }
        action_count += added;
    } else {
        action_count += t3_800572a4(state);
    }

attack_candidates_ready:
    if (!action_count && r_s32(state + 0x18) >= 3) {
        current_actor = r_u32(0x800AFA5C);
        if (!r_s16(current_actor + 0x122)) {
            first = r_u32(current_actor + 0x3F8);
            second = r_u32(current_actor + 0x3F4);
            if ((sint32)first / 10 >= (sint32)second) {
                random_value = r_u32(0x800ADCA0);
                first = random_value * 5 + 3;
                w_u32(0x800ADCA0, first);
                if (!(random_value & 3)) {
                    threshold = r_s16(state + 0x258);
                    w_u32(0x800ADCA0, first * 5 + 3);
                    if ((sint32)(first & 0xFFF) < threshold) {
                        added = 0;
                        limit = r_s16(state + 0x70);
                        if (limit > 0) {
                            for (row = 0, entry = 0x8009F978; (sint32)row < limit; ++row, entry += 12) {
                                if (r_s16(entry + 8))
                                    continue;
                                action = r_u32(entry);
                                if ((r_u32(action + 0x24) >> 8) & 0x100) {
                                    w_u16(entry + 8, 1);
                                    ++added;
                                }
                            }
                            w_u16(state + 0x72, r_u16(state + 0x72) + added);
                            action_count = added;
                        }
                    }
                }
            }
        }
    }
    if ((sint32)action_count < 0) {
        w_u16(state + 0x40, 40);
        w_u16(state + 0x3A, 0);
        w_u16(state + 0x38, 0);
        w_u16(state + 0x48, 0xFFFF);
        handled = 1;
        goto attack_selection_done;
    }

attack_result_selected:
    first = t3_80058a84(state);
    if ((sint32)first < 0) {
        handled = 0;
        goto attack_selection_done;
    }
    w_u16(state + 0x40, 40);
    w_u16(state + 0x48, 0xFFFF);
    entry = 0x8009F978 + first * 12;
    second = r_u16(0x800ADED4);
    w_u16(state + 0x3A, 0);
    w_u16(state + 0x38, 0);
    action = r_u32(entry);
    if (!second && ((r_u32(action + 0x24) >> 8) & 0x20)) {
        w_u16(state + 0xC0, 1);
    } else {
        accepted = 0;
        if (r_u32(state + 0x224) & 0x80000) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            accepted = (random_value & 0x3F) < 32;
        }
        if (!accepted && (r_u32(action + 4) & 0x80000)) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            accepted = (random_value & 0x3F) < 48;
        }
        if (accepted)
            w_u16(state + 0xD8, 1);
    }
    first = r_u32(state + 0x5C);
    second = 0;
    if (first)
        second = r_u16(first + 4);
    if (second == 0x2021 || second == 0x2829)
        w_u16(state + 0x4C, 12);
    if (r_u8(action + 0x2D) || (r_u32(action + 4) & 0x80000))
        w_u32(state + 0x60, action);
    handled = 1;

attack_selection_done:
    if (handled)
        goto finish;

    handled = 0;
    if (r_s16(state + 0x8A) >= 0)
        goto direction_decision_done;
    if (r_s16(state + 0x38) >= 0) {
        if (r_u8(state + 0x200) && r_s32(state + 0x1C) < 7001)
            goto reset_direction_timers;
        if (r_s32(state + 0x14) <= 0)
            goto reset_direction_timers;
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((sint32)(random_value & 0xFFF) >= r_s16(state + 0x27E)) {
            accepted = 0;
            if (r_s16(state + 0x20C) && !r_s16(state + 0x6A) &&
                r_s16(r_u32(0x800AFA5C) + 0x3E) < 0x5000 && r_s32(state + 0x1C) < 2951) {
                first = r_u32(r_u32(r_u32(0x800A867C) + 0x54) + 8) & 0xFFFF0000;
                band = r_u32(state + 0x18);
                if (!(first & 0x1C0000))
                    first |= 0xC0000;
                accepted = (first & r_u32(0x80098248 + band * 4)) != 0;
            }
            if (accepted)
                goto reset_direction_timers;
        }
        if (r_s16(state + 0x3A) == 2) {
            accepted = r_u8(state + 0x200) && r_s32(state + 0x1C) < 7001;
            if (!accepted && r_s32(state + 0x18) < 3) {
                random_value = r_u32(0x800ADCA0);
                w_u32(0x800ADCA0, random_value * 5 + 3);
                accepted = (random_value & 0xFFF) < 32;
            }
            if (accepted) {
                w_u16(state + 0x40, 40);
                w_u16(state + 0x48, 0xFFFF);
                w_u16(state + 0x3A, 0);
                w_u16(state + 0x38, 0);
                w_u16(state + 4, 0x8000);
            } else {
                w_u16(state + 4, 0x2000);
            }
            handled = 1;
            goto direction_decision_done;
        }
        if (r_s16(state + 0x68) < 36 && r_u32(state + 0x18) == 2) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if (!(random_value & 0xF) && (sint32)t3_ai_mark_command(state, 0xC001) > 0 &&
                (sint32)t3_80058a84(state) >= 0) {
                w_u16(state + 0x3A, 0);
                w_u16(state + 0x38, 0);
                w_u16(state + 0x40, 40);
                handled = 1;
                goto direction_decision_done;
            }
        }
        w_u16(state + 4, r_u16(state + 6) & 0xF000);
        handled = 1;
        goto direction_decision_done;
    }
    if (r_s16(state + 0x50) || r_s16(state + 0x4A) >= 0 ||
        r_s16(state + 0x48) >= 0 || r_s16(state + 0x46) >= 0)
        goto direction_decision_done;
    goto choose_direction;

reset_direction_timers:
    w_u16(state + 0x40, 40);
    w_u16(state + 0x3A, 0);
    w_u16(state + 0x38, 0);
    w_u16(state + 0x48, 0xFFFF);

choose_direction:
    handled = 0;
    if (r_s16(state + 0x40) >= 0 || r_s16(state + 0x48) >= 0)
        goto direction_decision_done;
    decision = 0;
    random_result = (uint32)randPSX();
    random_value = r_u32(0x800ADCA0);
    first = ((random_result + random_value) & 0x7FFF) * 4096;
    w_u32(0x800ADCA0, random_value * 5 + 3);
    threshold = (sint32)first / 32768;
    if (!r_u8(state + 0x200)) {
        if (r_s16(state + 0x68) < 36) {
            switch (r_u32(state + 0x14)) {
            case 0:
            case 1:
                if (threshold < 20)
                    decision = 3;
                break;
            case 2:
                if (threshold < 122)
                    decision = 5;
                else if (threshold < 491)
                    decision = 1;
                else if (threshold < 532)
                    decision = 3;
                else if (threshold < r_s16(state + 0x25C) * 4096 / 1000)
                    decision = 8;
                break;
            case 3:
                if (threshold < 1638)
                    decision = 2;
                else if (threshold < 2867)
                    decision = 1;
                break;
            case 4:
                if (threshold < 204)
                    decision = 2;
                else if (threshold < 1433)
                    decision = 4;
                else
                    decision = 1;
                break;
            case 5:
                decision = 4;
                if (r_s16(state + 0x2A) < 721 && !r_s16(state + 0x208) && threshold >= 286)
                    decision = 1;
                break;
            default:
                break;
            }
        } else {
            switch (r_u32(state + 0x18)) {
            case 0:
            case 1:
                if (threshold < 409)
                    decision = 6;
                else if (threshold < 819)
                    decision = 3;
                break;
            case 2:
                if (threshold < 40)
                    decision = 2;
                else if (threshold < 368)
                    decision = 3;
                break;
            case 3:
                if (threshold < 491)
                    decision = 2;
                else if (threshold < 532)
                    decision = 5;
                else if (threshold < 536)
                    decision = 3;
                else if (threshold < 557)
                    decision = 1;
                else if (threshold < 577 && (r_u32(state + 0x224) & 2))
                    decision = 8;
                break;
            case 4:
                if (threshold < 128)
                    decision = 2;
                else if (threshold < 144)
                    decision = 5;
                else if (threshold < 192)
                    decision = 8;
                else if (threshold < 960)
                    decision = 1;
                break;
            case 5:
                if (r_s16(state + 0x2A) >= 721 || r_s16(state + 0x208) || threshold < 81)
                    decision = 4;
                else if (threshold < 204)
                    decision = 2;
                else if (threshold < 1024)
                    decision = 1;
                else if (threshold < 1515)
                    decision = 5;
                break;
            default:
                break;
            }
            if (decision == 3 || decision == 6) {
                random_value = r_u32(0x800ADCA0);
                w_u32(0x800ADCA0, random_value * 5 + 3);
                if ((sint32)(random_value & 0xFFF) >= r_s16(state + 0x284))
                    decision = 0;
            }
        }
        if ((decision == 3 || decision == 6) &&
            ((r_u32(state + 0x230) & 0x80000) || r_u32(state + 0x234))) {
            random_value = r_u32(0x800ADCA0);
            w_u32(0x800ADCA0, random_value * 5 + 3);
            if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x27E))
                decision = 0;
        }
    }
    switch (decision) {
    case 0:
        handled = 0;
        goto direction_decision_done;
    case 1:
        if ((sint32)t3_ai_mark_command(state, 0xC001) > 0) {
            w_u16(state + 0x3A, 0);
            w_u16(state + 0x38, 0);
            w_u16(state + 0x40, 40);
            goto select_direction_action;
        }
        /* Fall through */
    case 2:
        band = r_u32(state + 0x14);
        random_result = (uint32)randPSX();
        random_value = r_u32(0x800ADCA0);
        if ((sint32)band >= 0 && (sint32)band < 3) {
            first = ((random_result + random_value) & 0x7FFF) * 8 / 32768;
            second = first + 4;
            first += 44;
        } else if (band == 3) {
            first = ((random_result + random_value) & 0x7FFF) * 10 / 32768;
            second = first + 10;
            first += 50;
        } else {
            first = ((random_result + random_value) & 0x7FFF) * 15 / 32768;
            second = first + 15;
            first += 55;
        }
        w_u32(0x800ADCA0, random_value * 5 + 3);
        w_u16(state + 0x3A, 0);
        w_u16(state + 0x38, second);
        w_u16(state + 0x40, first);
        w_u16(state + 4, r_s16(state + 0x3C) > 0 ? 0x6000 : 0x2000);
        handled = 1;
        goto direction_decision_done;
    case 4:
        if ((sint32)t3_ai_mark_command(state, 0xC001) > 0) {
            w_u16(state + 0x3A, 2);
            w_u16(state + 0x38, 250);
            w_u16(state + 0x40, 310);
            goto select_direction_action;
        }
        /* Fall through */
    case 5:
        random_result = (uint32)randPSX();
        random_value = r_u32(0x800ADCA0);
        first = ((random_result + random_value) & 0x7FFF) * 20 / 32768;
        w_u32(0x800ADCA0, random_value * 5 + 3);
        w_u16(state + 0x38, first + 40);
        second = r_s16(state + 0x3C);
        w_u16(state + 0x3A, 0);
        w_u16(state + 0x40, first + 80);
        w_u16(state + 4, (sint32)second > 0 ? 0x6000 : 0x2000);
        handled = 1;
        goto direction_decision_done;
    case 6:
        if ((sint32)t3_ai_mark_command(state, 0xC002) > 0) {
            w_u16(state + 0x40, 60);
            goto select_direction_action;
        }
        /* Fall through */
    case 3:
        random_result = (uint32)randPSX();
        random_value = r_u32(0x800ADCA0);
        first = ((random_result + random_value) & 0x7FFF) * 5 / 32768;
        w_u32(0x800ADCA0, random_value * 5 + 3);
        w_u16(state + 0x38, first + 20);
        second = r_s16(state + 0x3C);
        w_u16(state + 0x3A, 0);
        w_u16(state + 0x40, first + 60);
        w_u16(state + 4, (sint32)second > 0 ? 0xC000 : 0x8000);
        handled = 1;
        goto direction_decision_done;
    case 8:
        if (r_s32(state + 0x18) >= 3 || r_s16(state + 0x20A) >= 11)
            t3_80061a58(state, 0xFFFFFFFF);
        goto select_direction_action;
    }

select_direction_action:
    handled = (sint32)t3_80058a84(state) >= 0;

direction_decision_done:
    if (handled)
        goto finish;
    if (r_s16(state + 0x4A) >= 0) {
        w_u16(state + 4, 0x4000);
        goto finish;
    }
    if (r_s16(state + 0x48) >= 0)
        goto hold_previous;
    goto no_input;

hold_previous:
    w_u16(state + 4, r_u16(state + 6) & 0xF000);
    goto finish;

no_input:
    w_u16(state + 4, 0);

finish:
    flags = r_u32(state + 0x224);
    action = r_u32(state + 0x5C);
    opponent = r_u32(0x800A867C);
    w_u16(state + 0x3C, (flags & 0x205) == 1);
    actor = r_u32(0x800AFA5C);
    if (action && r_s16(state + 0x48) < 0 && r_u8(action + 0x19))
        w_u16(state + 0x48, r_u8(action + 0x19));
    {
        const uint32 timer_offsets[8] = { 0x48, 0x38, 0x40, 0x54, 0x46, 0x44, 0x42, 0x8A };
        for (row = 0; row < 8; ++row) {
            counter = r_s16(state + timer_offsets[row]);
            if (counter >= 0)
                --counter;
            w_u16(state + timer_offsets[row], counter);
        }
    }
    counter = r_s16(state + 0x8C);
    if (counter >= 0)
        --counter;
    first = r_s16(state + 0x1FE);
    w_u16(state + 0x8C, counter);
    if (!first)
        t3_80061488(state, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
    counter = r_s16(state + 0x1FE);
    if (counter >= 0)
        --counter;
    w_u16(state + 0x1FE, counter);
    if (r_u32(0x800AFA88) == 8) {
        entry = 0x8009F2F8 + r_u16(state) * 4;
        first = r_u32(entry + 8);
        if ((sint32)first >= 0)
            --first;
        w_u32(entry + 8, first);
        entry = 0x8009F2F8 + r_u16(state) * 4;
        first = r_u32(entry + 0x10);
        if ((sint32)first >= 0)
            --first;
        w_u32(entry + 0x10, first);
    }
    first = r_u16(state + 0x20C);
    second = r_s16(state + 0x3E);
    flags &= 0xFFFF;
    w_u16(state + 0x212, first);
    accepted = flags == 0x1052 || flags == 0x2829;
    w_u16(state + 0x3E, accepted);
    if (!second || (!accepted && r_u8(state + 0x93)))
        w_u8(state + 0x93, 0);
    if (r_u8(actor + 0xCE)) {
        w_u16(state + 0x40, 40);
        w_u16(state + 0x3A, 0);
        w_u16(state + 0x38, 0);
        w_u16(state + 0x48, 0xFFFF);
        w_u16(state + 0xD8, 0);
        w_u16(state + 0xC0, 0);
        w_u16(state + 0x1A0, 0);
        w_u16(state + 0x98, 0);
    }
    if (r_u8(actor + 0xD2) && r_s32(state + 0x18) < 3)
        w_u16(state + 0x54, 30);
    if (r_u8(opponent + 0xD2) && r_s32(state + 0x14) < 2) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((sint32)(random_value & 0xFFF) < r_s16(state + 0x258))
            w_u16(state + 0x68, r_u16(state + 0x68) - 3);
    }
    first = r_u32(state + 0x30);
    second = r_u32(actor + 0x3F4);
    if ((sint32)second < (sint32)first) {
        random_value = r_u32(0x800ADCA0);
        w_u32(0x800ADCA0, random_value * 5 + 3);
        if ((sint32)(random_value & 0xFFF) >= r_s16(state + 0x27E)) {
            w_u16(state + 0x68, r_u16(state + 0x68) - 3);
            first = r_u32(0x8009F2FC);
            action = r_u32(opponent + 0x54);
            if (!first) {
                count = 0;
                w_u16(state + 0x202, 0);
                for (row = 0; row < 8; ++row) {
                    if (r_u32(state + 0x1DC + row * 4) == action)
                        ++count;
                }
                if (count >= 2) {
                    if (r_s16(state + 0x1FE) <= 0)
                        t3_80061488(state, 0, 9, 0xFFFFFFFF);
                    w_u16(state + 0x1FE, 600);
                    w_u16(state + 0x202, 1);
                }
                w_u32(state + 0x1DC + r_u16(state + 0x1FC) * 4, action);
                w_u16(state + 0x1FC, (r_u16(state + 0x1FC) + 1) & 7);
            }
        }
    }
    w_u32(state + 0x30, r_u32(actor + 0x3F4));
    if (r_u8(actor + 0xCE) && !r_u16(0x800ADED4)) {
        w_u16(state + 0x40, 40);
        first = r_u16(state + 0x254);
        w_u16(state + 0x48, 0xFFFF);
        threshold = r_s16(state + 0x256);
        w_u16(state + 0x3A, 0);
        w_u16(state + 0x38, 0);
        first += 10;
        w_u16(state + 0x254, first);
        second = r_u16(state + 0x258);
        counter = r_s16(state + 0x254);
        second += 10;
        w_u16(state + 0x258, second);
        third = r_u16(state + 0x256);
        if (threshold < counter)
            first = third;
        second = r_u16(state + 0x258);
        counter = r_s16(state + 0x258);
        threshold = r_s16(state + 0x25A);
        w_u16(state + 0x254, first);
        third = r_u16(state + 0x25A);
        if (threshold < counter)
            second = third;
        counter = r_s16(state + 0x216);
        w_u16(state + 0x258, second);
        if (counter > 0) {
            first = r_u16(state + 0x262);
            threshold = r_s16(state + 0x264);
            first += 40;
            w_u16(state + 0x262, first);
            second = r_u16(state + 0x25E);
            counter = r_s16(state + 0x262);
            second += 20;
            w_u16(state + 0x25E, second);
            third = r_u16(state + 0x264);
            if (threshold < counter)
                first = third;
            second = r_u16(state + 0x25E);
            counter = r_s16(state + 0x25E);
            threshold = r_s16(state + 0x260);
            w_u16(state + 0x262, first);
            third = r_u16(state + 0x260);
            if (threshold < counter)
                second = third;
            w_u16(state + 0x25E, second);
        }
    }
    first = r_u16(state + 4);
    second = r_u16(state + 6);
    second = first & (first ^ second);
    *pressed_output = (uint16)second;
    w_u16(state + 6, first);
    *held_output = (uint16)first;
    return second;
}
