#include "game_functions.h"

void t3_8004a364(uint32 vertices, uint32 screen, uint32 depths, uint32 count)
{
    uint32 first;
    uint32 second;
    uint32 third;
    FUNCTION_MARKER(0x8004A364, "SLUS_004.02");
    if ((sint32)count <= 0)
        return;
    first = r_u32(vertices);
    second = r_u32(vertices + 4);
    third = r_u32(vertices + 8);
    xport_gte_write_data(1, first);
    xport_gte_write_data(3, second);
    xport_gte_write_data(5, third);
    first >>= 16;
    second >>= 16;
    third >>= 16;
    do {
        uint32 first_depth;
        uint32 second_depth;
        uint32 third_depth;
        xport_gte_write_data(0, first);
        xport_gte_write_data(2, second);
        xport_gte_write_data(4, third);
        vertices += 12;
        xport_gte_execute(0x4A280030);
        first = r_u32(vertices);
        second = r_u32(vertices + 4);
        third = r_u32(vertices + 8);
        xport_gte_write_data(1, first);
        xport_gte_write_data(3, second);
        xport_gte_write_data(5, third);
        first >>= 16;
        second >>= 16;
        third >>= 16;
        w_u32(screen, xport_gte_read_data(12));
        w_u32(screen + 4, xport_gte_read_data(13));
        w_u32(screen + 8, xport_gte_read_data(14));
        first_depth = xport_gte_read_data(17);
        second_depth = xport_gte_read_data(18);
        third_depth = xport_gte_read_data(19);
        w_u16(depths, first_depth);
        w_u16(depths + 2, second_depth);
        w_u16(depths + 4, third_depth);
        count -= 3;
        screen += 12;
        depths += 6;
    } while ((sint32)count > 0);
}

uint32 t3_8004adc4(uint32 x, uint32 y, uint32 z)
{
    uint32 rotated_x;
    uint32 rotated_y;
    uint32 rotated_z;
    uint32 translation_x;
    uint32 translation_y;
    uint32 translation_z;
    uint32 result;
    MATRIX translation;
    FUNCTION_MARKER(0x8004ADC4, "SLUS_004.02");
    result = t3_80081858(0x800ADF70, x, y, z, &rotated_x, &rotated_y, &rotated_z);
    translation_x = r_u32(0x800ADF84);
    translation.t[0] = (sint32)(rotated_x + translation_x);
    translation_y = r_u32(0x800ADF88);
    translation_z = r_u32(0x800ADF8C);
    translation.t[1] = (sint32)(rotated_y + translation_y);
    translation.t[2] = (sint32)(rotated_z + translation_z);
    SetRotMatrix((MATRIX *)psx_addr(0x800ADF70, 32));
    SetTransMatrix(&translation);
    return result;
}

uint32 t3_80064810(uint32 first, uint32 second, uint32 *output, uint32 reset, uint32 camera_index)
{
    uint32 camera_snapshot[6];
    uint32 index;
    uint32 state = 0x800A0328 + camera_index * 48;
    uint32 second_x;
    uint32 first_x;
    uint32 second_z;
    uint32 first_z;
    sint32 first_side;
    sint32 second_side;
    uint32 angle;
    uint32 difference;
    uint32 magnitude;
    sint32 direction = 0;
    sint32 adjustment = 0;
    sint32 phase;
    FUNCTION_MARKER(0x80064810, "SLUS_004.02");
    for (index = 0; index < 6; ++index)
        camera_snapshot[index] = r_u32(0x800A02A8 + camera_index * 24 + index * 4);
    second_x = r_u32(0x800ADCB8);
    first_x = r_u32(0x800ADCA8);
    first_side = r_s16(first + 0x1278);
    second_z = r_u32(0x800ADCC0);
    first_z = r_u32(0x800ADCB0);
    second_side = r_s16(second + 0x1278);
    angle = (uint32)(sint32)(sint16)t3_8004b39c(second_x - first_x, second_z - first_z);
    if (first_side >= second_side)
        angle = (angle + 2048) & 0xFFF;
    angle <<= 6;
    difference = angle - camera_snapshot[4];
    difference = difference & 0x20000 ? difference | 0xFFFC0000 : difference & 0x3FFFF;
    magnitude = difference;
    if ((sint32)difference > 0)
        direction = 1;
    else if ((sint32)difference < 0) {
        magnitude = 0 - difference;
        direction = -1;
    }
    if (reset) {
        w_u32(state + 12, 0);
        w_u32(state + 16, 0);
        w_u16(state + 4, 0);
        w_u16(state + 6, 0);
        w_u32(state + 24, 0);
        w_u32(state + 20, 0);
        w_u32(state + 28, 0);
        *output = angle;
        return reset;
    }
    if (camera_index || (sint32)magnitude > 109226 || r_u32(0x800AFA3C) < 200) {
        w_u16(state + 4, 0);
        w_u16(state + 6, 0);
        *output = camera_snapshot[4];
        return 0;
    }
    phase = r_s16(state);
    if (!phase) {
        if ((sint32)magnitude < 3640)
            w_u16(state + 8, 0);
        else
            w_u16(state + 8, r_u16(state + 8) + 1);
        if (r_s16(state + 8) > 0) {
            uint32 velocity;
            w_u16(state, 1);
            velocity = r_u32(state + 16);
            w_u16(state + 8, 0);
            w_u32(state + 16, velocity + (uint32)direction * 1536);
        }
    } else if (phase == 1) {
        uint32 position;
        uint32 velocity;
        uint32 first_resource;
        uint32 second_resource;
        uint32 first_kind;
        uint32 second_kind;
        sint32 factor = -1;
        if ((sint32)magnitude < 14564) {
            uint32 decay;
            position = r_u32(state + 12);
            velocity = r_u32(state + 16);
            decay = position * 14;
            w_u32(state + 16, velocity - (uint32)((sint32)velocity / 2));
            w_u32(state + 12, position - (uint32)((sint32)decay / 128));
            velocity = r_u32(state + 16) + (uint32)direction * 768;
            w_u32(state + 16, velocity);
            if ((sint32)magnitude < 3641) {
                uint32 timer = r_u16(state + 8);
                w_u32(state + 16, velocity - (uint32)((sint32)velocity / 2));
                ++timer;
                w_u16(state + 8, timer);
                if ((sint16)timer >= 32) {
                    w_u32(state + 12, 0);
                    w_u16(state, 0);
                    w_u16(state + 8, 0);
                }
            } else {
                w_u16(state + 8, 0);
            }
        } else if (direction > 0) {
            w_u32(state + 16, r_u32(state + 16) + 1536);
        } else if (direction < 0) {
            w_u32(state + 16, r_u32(state + 16) - 1536);
        }
        position = r_u32(state + 12);
        velocity = r_u32(state + 16);
        w_u32(state + 12, position + velocity);
        first_resource = r_u32(0x800A927C);
        second_resource = r_u32(0x800AAB08);
        first_kind = r_u8(first_resource + 36);
        second_kind = r_u8(second_resource + 36);
        if (first_kind == 39 || second_kind == 39)
            factor = 512;
        else if (first_kind == 40 || second_kind == 40)
            factor = 384;
        else if (first_kind == 41 || second_kind == 41)
            factor = 128;
        else if (first_kind == 42 || second_kind == 42)
            factor = 64;
        if (factor > 0) {
            sint32 limit = (56784 * factor) / 256;
            position = r_u32(state + 12);
            if ((sint32)position > limit) {
                velocity = r_u32(state + 16);
                w_u32(state + 16, velocity - 1536);
                w_u32(state + 12, (uint32)limit);
            } else if ((sint32)position < -limit) {
                velocity = r_u32(state + 16);
                w_u32(state + 16, velocity + 1536);
                w_u32(state + 12, (uint32)-limit);
            }
        } else {
            position = r_u32(state + 12);
            if ((sint32)position > 56784) {
                velocity = r_u32(state + 16);
                w_u32(state + 12, 56784);
                w_u32(state + 16, velocity - 1536);
            } else if ((sint32)position < -56784) {
                velocity = r_u32(state + 16);
                w_u32(state + 12, 0xFFFF2230);
                w_u32(state + 16, velocity + 1536);
            }
        }
        adjustment = r_s32(state + 12) / 64;
    } else {
        t3_8007ae58(1);
    }
    w_u32(state + 32, magnitude);
    if (!camera_index) {
        if ((sint32)magnitude < 14563) {
            w_u32(0x800A0340, 0);
        } else {
            uint32 numerator = (magnitude - 14563) << 12;
            sint32 coordinate = (sint32)numerator / 50973;
            uint32 lookup;
            uint32 scaled;
            if (coordinate < 0)
                coordinate += 255;
            lookup = (uint32)(coordinate >> 7) & 0x1FFE;
            scaled = (uint32)r_s16(0x8001E8C4 + lookup) * 279616;
            w_u32(0x800A0340, (uint32)((sint32)scaled / 4096));
        }
        if (r_s16(0x800A032A)) {
            uint32 current = r_u32(0x800A033C);
            uint32 target = r_u32(0x800A0340);
            sint32 step = (sint32)(target - current) / 16;
            if (step > 256)
                step = 256;
            else if (step < -256)
                step = -256;
            current = r_u32(0x800A033C);
            w_u32(0x800A0344, (uint32)step);
            w_u32(0x800A033C, current + (uint32)step);
        } else {
            w_u32(0x800A0344, 0);
        }
    }
    *output = camera_snapshot[4] + (uint32)adjustment;
    return 0;
}

uint32 t3_80066084(uint32 first, uint32 second, uint32 mode, uint32 distance)
{
    uint32 angle;
    uint32 result;
    uint32 offset;
    FUNCTION_MARKER(0x80066084, "SLUS_004.02");
    result = t3_80064810(first, second, &angle, mode, 0);
    if (r_u32(0x800AFA88) == 7) {
        offset = 0;
        angle = 0;
    } else {
        offset = r_u32(0x800A033C);
    }
    return t3_80064e10(first, second, offset, angle, result, distance, 0);
}

uint32 t3_80063eec(uint32 *camera, uint32 x, uint32 y, uint32 z, uint32 target_x, uint32 target_y, uint32 target_z, sint32 distance)
{
    uint32 delta_x;
    uint32 delta_y;
    uint32 delta_z;
    uint32 horizontal;
    uint32 angle;
    uint32 result;
    FUNCTION_MARKER(0x80063EEC, "SLUS_004.02");
    camera[0] = x;
    camera[1] = y;
    camera[2] = z;
    delta_x = target_x - x;
    delta_y = target_y - y;
    delta_z = target_z - z;
    horizontal = t3_8002a324(delta_x * delta_x + delta_z * delta_z);
    angle = t3_8004b39c(horizontal, delta_y);
    camera[3] = (uint32)(sint32)(sint16)angle << 6;
    angle = t3_8004b39c(delta_z, 0 - delta_x);
    result = (uint32)(sint32)(sint16)angle << 6;
    camera[4] = result;
    if (distance > 0)
        camera[5] = (uint32)distance;
    return result;
}

void t3_80064080(uint32 index)
{
    uint32 camera = 0x800A02A8 + index * 24;
    uint32 x;
    uint32 y;
    uint32 z;
    sint32 pitch;
    sint32 yaw;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    FUNCTION_MARKER(0x80064080, "SLUS_004.02");
    x = r_u32(camera);
    pitch = r_s32(camera + 12);
    y = r_u32(camera + 4);
    yaw = r_s32(camera + 16);
    z = r_u32(camera + 8);
    first = r_u32(camera);
    second = r_u32(camera + 4);
    third = r_u32(camera + 8);
    fourth = r_u32(camera + 12);
    w_u32(0x800A02F0, first);
    w_u32(0x800A02F4, second);
    w_u32(0x800A02F8, third);
    w_u32(0x800A02FC, fourth);
    first = r_u32(camera + 16);
    second = r_u32(camera + 20);
    w_u32(0x800A0300, first);
    w_u32(0x800A0304, second);
    w_u32(0x800A85D4, x);
    w_u32(0x800A85D8, y);
    w_u32(0x800A85DC, z);
    w_u32(0x800A85C0, (uint32)(pitch / 64));
    w_u32(0x800A85C4, (uint32)(yaw / 64));
    t3_80063c64(r_s32(camera + 20));
}

uint32 t3_800335e4(void)
{
    uint32 countdown;
    uint32 value;
    uint32 base;
    uint32 destination = 0;
    FUNCTION_MARKER(0x800335E4, "SLUS_004.02");
    countdown = r_u32(0x8009BCC4);
    if ((sint32)countdown > 0)
        w_u32(0x8009BCC4, countdown - 1);
    value = r_u32(0x8009BCB8);
    value += r_u32(0x8009BCC4);
    if ((sint32)value < 0)
        value += 0x4B00;
    value = (uint32)((sint32)value % 300);
    base = r_u32(0x800ADE68);
    if (base) {
        value <<= 2;
        destination = base + 0x7A48 + value;
    }
    w_u8(destination + 2, 0);
    return value;
}

uint32 t3_8003a818(uint32 object)
{
    FUNCTION_MARKER(0x8003A818, "SLUS_004.02");
    t3_8003615c(object);
    t3_80039fbc(object + 0xF68);
    if (r_u8(object + 0x1888))
        t3_8003a310();
    else
        t3_8003a164(object, r_u16(0x800ADC84));
    return t3_8003a87c(object);
}

uint32 t3_8002ba64(uint32 first, uint32 second, uint32 third)
{
    uint32 result;
    FUNCTION_MARKER(0x8002BA64, "SLUS_004.02");
    t3_80039ddc(r_u16(0x800ADC84));
    t3_80039f84();
    t3_80036000();
    t3_800378f8();
    if (r_u8(first + 195))
        t3_8003a818(first);
    if (r_u8(second + 195))
        t3_8003a818(second);
    result = 8;
    if (r_u32(0x800AFA88) == 8) {
        result = r_u8(third + 195);
        if (result)
            result = t3_8003a818(third);
    }
    return result;
}

uint32 t3_80036000(void)
{
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 result;
    uint32 offset;
    FUNCTION_MARKER(0x80036000, "SLUS_004.02");
    if (!r_u8(0x800B040C)) {
        for (offset = 0; offset < 32; offset += 8) {
            first = r_u32(0x800ADF70 + offset);
            second = r_u32(0x800ADF74 + offset);
            w_u32(0x800ADE20 + offset, first);
            w_u32(0x800ADE24 + offset, second);
        }
        MulMatrix0((MATRIX *)psx_addr(0x800ADF70, 32),
            (MATRIX *)psx_addr(0x800AFB50, 32), (MATRIX *)psx_addr(0x800ADE48, 32));
        result = r_u32(0x800ADF84);
        second = r_u32(0x800ADF88);
        third = r_u32(0x800ADF8C);
        w_u32(0x800ADE5C, result);
        w_u32(0x800ADE60, second);
        w_u32(0x800ADE64, third);
    } else {
        first = r_u32(0x800ADF84);
        second = r_u32(0x800ADF88);
        third = r_u32(0x800ADF8C);
        w_u16(0x800ADE20, 4096);
        w_u16(0x800ADE22, 0);
        w_u16(0x800ADE24, 0);
        w_u16(0x800ADE26, 0);
        w_u16(0x800ADE28, 4096);
        w_u16(0x800ADE2A, 0);
        w_u16(0x800ADE2C, 0);
        w_u16(0x800ADE2E, 0);
        w_u16(0x800ADE30, 4096);
        w_u32(0x800ADE34, first);
        w_u32(0x800ADE38, second);
        w_u32(0x800ADE3C, third - 2618);
        MulMatrix0((MATRIX *)psx_addr(0x800ADF70, 32),
            (MATRIX *)psx_addr(0x800ADE20, 32), (MATRIX *)psx_addr(0x800ADE20, 32));
        MulMatrix0((MATRIX *)psx_addr(0x800ADE20, 32),
            (MATRIX *)psx_addr(0x800AFB50, 32), (MATRIX *)psx_addr(0x800ADE48, 32));
        first = r_u32(0x800ADF84);
        result = r_u32(0x800ADF8C);
        second = r_u32(0x800ADF88);
        result -= 1868;
        w_u32(0x800ADE5C, first);
        w_u32(0x800ADE60, second);
        w_u32(0x800ADE64, result);
    }
    return result;
}

uint32 t3_8004aedc(uint32 value)
{
    uint32 leading;
    sint32 shift;
    uint32 index;
    uint32 root;
    FUNCTION_MARKER(0x8004AEDC, "SLUS_004.02");
    xport_gte_write_data(30, value);
    leading = xport_gte_read_data(31) & 0xFFFE;
    if (!value)
        return 0;
    shift = 24 - (sint32)leading;
    if (shift >= 0)
        index = (uint32)((sint32)value >> ((uint32)shift & 31));
    else
        index = value << ((leading - 24) & 31);
    index = (index - 64) << 1;
    root = (uint32)(sint32)r_s16(0x8004AF40 + index);
    shift = (sint32)(31 - leading) >> 1;
    return (root << ((uint32)shift & 31)) >> 12;
}

uint32 t3_80039fbc(uint32 position)
{
    uint32 strength;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 origin;
    uint32 length;
    uint32 factor;
    uint32 result;
    FUNCTION_MARKER(0x80039FBC, "SLUS_004.02");
    strength = r_u32(0x8009E5EC);
    if ((sint32)strength <= 0)
        return strength;
    x = r_u32(position);
    x -= r_u32(0x8009E5E0);
    y = r_u32(position + 4);
    y -= r_u32(0x8009E5E4);
    origin = r_u32(0x8009E5E8);
    z = r_u32(position + 8) - origin;
    length = t3_8004aedc(x * x + y * y + z * z);
    if (length) {
        factor = (uint32)(0x04000000 / (sint32)length);
        factor *= r_u32(0x8009E5EC);
        factor = (uint32)((sint32)factor / 48);
        x = (uint32)((sint32)(x * factor) / 16384);
        y = (uint32)((sint32)(y * factor) / 16384);
        z = (uint32)((sint32)(z * factor) / 16384);
    }
    w_u16(0x8009E5A6, (uint16)(0 - (uint16)x));
    result = 0 - (uint32)(uint16)y;
    w_u16(0x8009E5A8, (uint16)result);
    w_u16(0x8009E5AA, (uint16)(0 - (uint16)z));
    return result;
}

uint32 t3_8003a164(uint32 object, uint32 selection)
{
    uint32 table = 0x80096C5C + (selection + 1) * 28;
    uint32 mode;
    uint32 brightness;
    uint32 entry;
    uint32 phase;
    uint32 product;
    uint32 intensity;
    uint32 blue;
    uint32 red;
    uint32 green;
    uint32 result;
    FUNCTION_MARKER(0x8003A164, "SLUS_004.02");
    mode = r_u32(0x800AFA88);
    brightness = r_u16(table);
    if (mode == 5) {
        entry = 0x800ADF68 + (uint32)r_s16(object + 18) * 4;
        if (r_u8(entry)) {
            red = r_u8(entry + 1);
            green = r_u8(entry + 2);
            blue = r_u8(entry + 3);
            SetBackColor((sint32)red, (sint32)green, (sint32)blue);
            return 0x800A0000;
        }
    }
    entry = 0x8009E5F0 + (uint32)r_s16(object + 18);
    phase = r_u8(entry);
    if (!phase) {
        intensity = brightness >> 4;
        SetBackColor((sint32)intensity, (sint32)intensity, (sint32)intensity);
        return 0;
    }
    product = (0x2000 - brightness) * ((33 - phase) << 7);
    intensity = (uint32)((sint32)product / 4096) + brightness;
    intensity = (uint32)((sint32)intensity >> 4);
    blue = (phase & 1) ? intensity : brightness >> 4;
    SetBackColor((sint32)intensity, (sint32)intensity, (sint32)blue);
    entry = 0x8009E5F0 + (uint32)r_s16(object + 18);
    phase = r_u8(entry);
    w_u8(entry, (uint8)(phase + 1));
    entry = 0x8009E5F0 + (uint32)r_s16(object + 18);
    result = r_u8(entry) < 33;
    if (!result)
        w_u8(entry, 0);
    return result;
}

uint32 t3_8003a87c(uint32 object)
{
    uint32 animation;
    sint32 frame;
    uint32 data;
    uint32 result;
    FUNCTION_MARKER(0x8003A87C, "SLUS_004.02");
    t3_80032c74(object);
    t3_8003890c(object);
    animation = r_u32(object + 76);
    frame = r_s16(object + 80);
    data = r_u32(animation);
    t3_800389c0(data, (uint16 *)psx_addr(0x800A8588, 6), frame - 1);
    w_u16(object + 36, r_u16(0x800A8588));
    w_u16(object + 38, r_u16(0x800A858A));
    w_u16(object + 40, r_u16(0x800A858C));
    t3_80038970((uint16 *)psx_addr(object + 36, 2), (uint16 *)psx_addr(object + 38, 2), (uint16 *)psx_addr(object + 40, 2), object);
    animation = r_u32(object + 84);
    frame = r_s16(object + 88);
    data = r_u32(animation);
    t3_80038b4c(data, 0x800A85E8, frame - 1);
    t3_8003a9c8(object);
    if (r_u32(0x8009549C)) {
        if (!r_u32(0x80096EA8))
            t3_8003c270(object);
    }
    w_u32(0x80096EA8, 0);
    t3_80032b94(object);
    t3_8003aaf4(object);
    t3_80034970(object);
    if (r_s16(0x800AE204) != 6)
        t3_80034100(object);
    t3_8003afcc(object);
    result = r_u32(0x800ADCA4);
    if (!result)
        result = t3_8003b4f8(object);
    return result;
}

uint32 t3_8003890c(uint32 object)
{
    uint32 result;
    FUNCTION_MARKER(0x8003890C, "SLUS_004.02");
    if (r_s16(object + 116) < 0)
        result = r_u16(0x800A9714 + r_u8(object + 33) * 6284);
    else
        result = r_u16(object + 1260);
    w_u16(object + 1262, (uint16)result);
    return result;
}

uint32 t3_800389c0(uint32 data, uint16 *output, sint32 frame)
{
    uint32 header;
    uint32 count;
    uint32 encoding;
    uint32 types = data + 2;
    uint32 values = data + 50;
    uint32 component;
    FUNCTION_MARKER(0x800389C0, "SLUS_004.02");
    header = r_u16(data);
    count = header & 255;
    if (frame < 0)
        frame = 0;
    if (frame >= (sint32)count)
        frame = (sint32)count - 1;
    encoding = header >> 8;
    for (component = 0; component < 3; ++component) {
        uint32 sample = (component & 1) ? count - ((uint32)frame + 1) : (uint32)frame;
        uint32 value;
        if (!encoding) {
            *output = 0;
        } else {
            if (encoding == 1) {
                value = r_u16(values);
            } else if (encoding == 2) {
                uint32 increment = (uint32)r_s16(values + 2) * sample;
                if (count >= 10)
                    increment = (uint32)((sint32)increment >> 5);
                value = r_u16(values) + increment;
            } else if (encoding == count) {
                value = r_u16(values + sample * 2);
            } else {
                uint32 precision = r_u8(0x80096C24 + component);
                value = t3_800387c8(values, sample, count, precision);
            }
            *output = (uint16)(value << 1);
        }
        values += encoding * 2;
        encoding = r_u8(types++);
        ++output;
    }
    return 0;
}

uint32 t3_80038b4c(uint32 data, uint32 output, sint32 frame)
{
    uint32 header;
    uint32 count;
    uint32 encoding;
    uint32 types = data + 2;
    uint32 values = data + 50;
    uint32 component;
    FUNCTION_MARKER(0x80038B4C, "SLUS_004.02");
    header = r_u16(data);
    count = header & 255;
    if (frame < 0)
        frame = 0;
    if (frame >= (sint32)count)
        frame = (sint32)count - 1;
    encoding = header >> 8;
    for (component = 0; component < 49; ++component) {
        uint32 sample = (component & 1) ? count - ((uint32)frame + 1) : (uint32)frame;
        uint32 value;
        if (!encoding) {
            w_u16(output, 0);
        } else {
            if (encoding == 1) {
                value = r_u16(values);
            } else if (encoding == 2) {
                uint32 increment = (uint32)r_s16(values + 2) * sample;
                if (count >= 10)
                    increment = (uint32)((sint32)increment >> 5);
                value = r_u16(values) + increment;
            } else if (encoding == count) {
                value = r_u16(values + sample * 2);
            } else {
                uint32 precision = r_u8(0x80096C24 + component);
                value = t3_800387c8(values, sample, count, precision);
            }
            w_u16(output, (uint16)(value << 1));
        }
        values += encoding * 2;
        encoding = r_u8(types++);
        output += 2;
    }
    return 0;
}

uint32 t3_800387c8(uint32 data, uint32 sample, uint32 count, uint32 precision)
{
    uint32 last = count - 1;
    uint32 start = 0;
    uint32 end = 0;
    uint32 previous;
    uint32 current;
    uint32 next;
    uint32 beyond;
    uint32 encoded;
    uint32 run;
    uint32 offset;
    uint32 span;
    uint32 coefficient;
    uint32 product;
    uint32 mixing;
    FUNCTION_MARKER(0x800387C8, "SLUS_004.02");
    previous = (uint32)r_s16(data);
    encoded = (uint32)r_s16(data + 2);
    current = previous;
    data += 4;
    beyond = current + ((uint32)((sint32)encoded >> 5) << (precision & 31));
    run = encoded & 31;
    for (;;) {
        end += run + 1;
        next = beyond;
        if (end == last)
            break;
        encoded = (uint32)r_s16(data);
        data += 2;
        beyond += (uint32)((sint32)encoded >> 5) << (precision & 31);
        run = encoded & 31;
        if ((sint32)(sample - end) <= 0)
            break;
        start = end;
        previous = current;
        current = next;
    }
    offset = sample - start;
    span = end - start;
    if (offset) {
        if ((sint32)(span - 16) > 0)
            offset = r_u8(0x8009671C - (span << 5) - offset + 0x420);
        else
            offset = r_u8(0x8009671C + (span << 5) + offset - 0x21);
        offset <<= 1;
    }
    if (!start) {
        coefficient = r_u16(0x80096A1C - offset);
        product = coefficient * (beyond - current);
        mixing = r_u16(0x80096C20 - offset);
    } else {
        uint32 previous_product;
        coefficient = r_u16(0x8009691C + offset);
        previous_product = coefficient * (previous - current);
        coefficient = r_u16(0x80096A1C - offset);
        if (end == last) {
            mixing = r_u16(0x80096C20 - offset);
            mixing += coefficient + 0x2AAB;
            product = previous_product;
        } else {
            product = coefficient * (beyond - current);
            mixing = r_u16(0x80096B1E - offset);
            product += previous_product;
        }
    }
    product += mixing * (next - current);
    return (uint32)((sint32)product >> 16) + current;
}

uint32 t3_80032b94(uint32 object)
{
    uint32 side = object != 0x800A9228;
    uint32 base;
    uint32 frame;
    uint32 destination;
    uint32 high;
    uint32 result;
    FUNCTION_MARKER(0x80032B94, "SLUS_004.02");
    if (r_u32(0x8009BCA8) != 7)
        return 7;
    base = r_u32(0x800ADE68);
    frame = r_u32(0x8009BCB8);
    destination = base ? base + side * 15600 + frame * 52 : 0;
    w_u32(destination, r_u32(object + 84));
    w_u32(destination + 4, r_u32(object + 0x187C));
    w_u32(destination + 8, r_u32(object + 0x1880));
    w_u8(destination + 44, r_u8(object + 0x1884));
    w_u8(destination + 45, r_u8(object + 0x1885));
    w_u8(destination + 46, r_u8(object + 88));
    w_u8(destination + 47, r_u8(object + 0x1870));
    high = r_u8(object + 0x181C);
    result = r_u8(object + 229) & 15;
    w_u8(destination + 51, (uint8)((high << 4) + result));
    return result;
}

uint32 t3_80032c74(uint32 object)
{
    uint32 side = object != 0x800A9228;
    sint32 mode;
    uint32 base;
    uint32 record;
    uint32 high;
    uint32 value;
    uint32 frame;
    uint32 bank;
    FUNCTION_MARKER(0x80032C74, "SLUS_004.02");
    mode = r_s32(0x8009BCA8);
    if (mode == 3) {
        base = r_u32(0x800ADE68);
        record = base ? base + 0x79E0 + side * 52 : 0;
        w_u32(record, r_u32(object + 84));
        w_u32(record + 4, r_u32(object + 0x187C));
        w_u32(record + 8, r_u32(object + 0x1880));
        w_u8(record + 44, r_u8(object + 0x1884));
        w_u8(record + 45, r_u8(object + 0x1885));
        w_u8(record + 46, r_u8(object + 88));
        w_u8(record + 47, r_u8(object + 0x1870));
        high = r_u8(object + 0x181C);
        value = r_u8(object + 229) & 15;
        w_u8(record + 51, (uint8)((high << 4) + value));
    } else if (mode != 1 && mode != 4 && mode != 6) {
        return 0x800A0000;
    }
    if (mode == 4) {
        base = r_u32(0x800ADE68);
        record = base ? base + 0x79E0 + side * 52 : 0;
    } else {
        frame = (uint32)(r_s32(0x8009BCBC) >> 1);
        base = r_u32(0x800ADE68);
        bank = r_u32(0x8009BCAC);
        record = base ? base + bank * 31200 + side * 15600 + frame * 52 : 0;
    }
    value = r_u32(record);
    w_u32(object + 76, value);
    w_u32(object + 84, value);
    w_u32(object + 0x187C, r_u32(record + 4));
    w_u32(object + 0x1880, r_u32(record + 8));
    w_u8(object + 0x1884, r_u8(record + 44));
    w_u8(object + 0x1885, r_u8(record + 45));
    w_u16(object + 88, r_u8(record + 46));
    w_u32(object + 0x1870, r_u8(record + 47));
    value = r_u8(record + 51);
    w_u32(object + 0x181C, value >> 4);
    value = r_u8(record + 51) & 15;
    w_u8(object + 229, (uint8)value);
    return value;
}

uint32 t3_8002b798(void)
{
    uint32 first = 0;
    uint32 second = 0;
    uint32 controller;
    uint32 current;
    uint32 previous;
    uint32 result;
    FUNCTION_MARKER(0x8002B798, "SLUS_004.02");
    if (r_u32(0x800AFA88) == 5) {
        if (!r_u32(0x800954A4)) {
            if (r_u32(0x800954BC)) {
                controller = r_u8(0x800ADF3E);
                first = controller == 0;
                second = controller == 1;
            } else {
                controller = r_u8(0x800ADF3E);
                if (!controller)
                    first = (r_u16(0x800ADF08) & 0x800) != 0;
                if (controller == 1)
                    second = (r_u16(0x800ADF0A) & 0x800) != 0;
            }
        }
    } else {
        if (r_u16(0x800AE1F8))
            first = (r_u16(0x800ADF08) & 0x800) != 0;
        if (r_u16(0x800AE1FA))
            second = (r_u16(0x800ADF0A) & 0x800) != 0;
    }
    if (r_u8(0x800AFA8C) && r_u32(0x80096F2C) == 1) {
        if (!r_u32(0x8009548C)) {
            if (first)
                w_u32(0x8009548C, 1);
            if (second)
                w_u32(0x8009548C, 2);
        }
    } else {
        w_u32(0x8009548C, 0);
    }
    if (r_u8(0x80098A41) == 1) {
        w_u32(0x8009548C, 0);
        w_u8(0x80098A41, 0);
    }
    if (r_u32(0x80095488)) {
        w_u32(0x8009548C, 0);
        w_u32(0x80095488, 0);
    }
    current = r_u32(0x8009548C);
    previous = r_u32(0x80095490);
    if (current != previous) {
        if (current) {
            w_u8(0x80098A40, 255);
            t3_8006bc98(30, 20);
        } else {
            w_u8(0x80098A41, 0);
            t3_8006bc98(127, 20);
        }
    }
    current = r_u32(0x8009548C);
    w_u32(0x80095490, current);
    result = r_u32(0x80095460);
    w_u32(0x80095494, current);
    if (!result)
        w_u32(0x80095494, 0);
    return result;
}

uint32 t3_800344a4(uint32 object, uint32 mode, uint32 index, uint32 value)
{
    uint32 result;
    FUNCTION_MARKER(0x800344A4, "SLUS_004.02");
    if ((sint32)index >= 10) {
        uint32 table;
        uint32 position;
        uint32 dimensions;
        uint32 destination;
        sint32 source_index;
        sint32 destination_index;
        sint32 dimensions_index;
        sint32 x;
        sint32 y;
        sint32 side;
        PSX_RECT rectangle;
        index -= 10;
        if ((sint32)index >= 2)
            return 0;
        result = r_u8(object + 0x1290);
        w_u16(object + 0x128E, (uint16)value);
        if (result == index)
            return result;
        table = 0x800959CC + (uint32)r_s16(object + 28) * 4;
        w_u8(object + 0x1290, (uint8)index);
        source_index = r_s8(table + index + 2);
        if (source_index == -1)
            return 0xFFFFFFFF;
        position = 0x8009579C + (uint32)source_index * 4;
        destination_index = r_s8(table);
        dimensions_index = r_s8(table + 1);
        rectangle.x = (sint16)r_u16(position);
        rectangle.y = (sint16)r_u16(position + 2);
        dimensions = 0x8009586C + (uint32)dimensions_index * 4;
        rectangle.w = (sint16)r_u16(dimensions);
        rectangle.h = (sint16)r_u16(dimensions + 2);
        destination = 0x8009579C + (uint32)destination_index * 4;
        y = r_s16(destination + 2);
        side = r_s16(object + 18);
        x = r_s16(destination);
        if (side) {
            rectangle.y = (sint16)((uint16)rectangle.y + 256);
            y += 256;
        }
        return t3_800293bc(&rectangle, x, y);
    }
    if ((sint32)index >= 8)
        return 20;
    if (r_s16(object + 24) == 20) {
        uint32 divisor;
        uint32 step;
        uint32 target;
        sint32 current;
        if (!value)
            value = 1;
        divisor = (sint32)value < 0 ? 1 - value : value + 1;
        step = (uint32)(256 / (sint32)divisor);
        w_u16(object + 0x129A, 0);
        if (index == 2 || index == 3) {
            target = index == 2 ? 256 : 0;
            current = r_s16(object + 0x1296);
            result = current < 256;
            w_u16(object + 0x1298, 256);
            if (result)
                w_u16(object + 0x129A, (uint16)step);
        } else {
            target = 0;
            current = r_s16(object + 0x1296);
            result = (uint32)current;
            w_u16(object + 0x1298, 0);
            if (current > 0)
                w_u16(object + 0x129A, (uint16)step);
        }
        w_u16(object + 0x1280, (uint16)target);
        w_u16(object + 0x1284, (uint16)step);
        return result;
    } else {
        uint32 row;
        uint32 positive_divisor = value + 1;
        uint32 negative_divisor = 1 - value;
        for (row = 0; row < 2; ++row) {
            if ((row + 1) & mode) {
                uint32 target = !index ? 0 : index == 1 ? 512 : index + 511;
                sint32 current = r_s16(object + 0x127C);
                if (current >= 513 || (sint32)target >= 513) {
                    w_u16(object + 0x127C, (uint16)target);
                    w_u16(object + 0x1284, 0);
                } else if (value) {
                    uint32 divisor = (sint32)value < 0 ? negative_divisor : positive_divisor;
                    uint32 step = (uint32)(512 / (sint32)divisor);
                    w_u16(object + 0x1284, (uint16)step);
                }
                w_u16(object + 0x1280, (uint16)target);
            }
            object += 2;
        }
        return 0;
    }
}

uint32 t3_80059654(void)
{
    sint32 owners[3];
    sint32 opponents[3];
    uint32 active;
    uint32 row;
    uint32 object = 0x800A9228;
    FUNCTION_MARKER(0x80059654, "SLUS_004.02");
    active = r_u16(0x800ADED4) != 0;
    if (!active)
        active = r_u32(0x800AFA88) == 8;
    w_u32(0x8009F2FC, active);
    w_u32(0x8009F304, 0xFFFFFFFF);
    w_u32(0x8009F300, 0xFFFFFFFF);
    w_u32(0x8009F30C, 0xFFFFFFFF);
    w_u32(0x8009F308, 0xFFFFFFFF);
    if (r_u32(0x800AFA88) != 8) {
        owners[0] = 0; owners[1] = 1; owners[2] = -1;
        opponents[0] = 1; opponents[1] = 0; opponents[2] = -1;
    } else {
        uint32 controller = r_u16(0x800ADF10);
        if (controller == 1) {
            owners[0] = -1; owners[1] = 0; owners[2] = 1;
            opponents[0] = -1; opponents[1] = 0; opponents[2] = 0;
        } else if (controller == 2) {
            owners[0] = 0; owners[1] = -1; owners[2] = 1;
            opponents[0] = 1; opponents[1] = -1; opponents[2] = 1;
        } else {
            return 2;
        }
    }
    for (row = 0; row < 3; ++row) {
        w_u8(object + 0x1886, (uint8)owners[row]);
        w_u8(object + 0x1887, (uint8)opponents[row]);
        if (owners[row] >= 0) {
            uint32 owner = (uint32)owners[row];
            uint32 record = 0x8009F318 + owner * 816;
            uint32 opponent = 0x800A9228 + (uint32)opponents[row] * 6284;
            uint32 resource;
            w_u16(record + 2, (uint16)owner);
            w_u16(record, (uint16)owner);
            w_u32(record + 16, opponent);
            w_u32(0x800AFA5C, object);
            w_u32(0x800A867C, opponent);
            resource = t3_8002d154(owner);
            w_u32(record + 12, resource);
            resource = t3_8002d154(owner);
            w_u32(0x8009F2E8 + owner * 4, resource);
            w_u16(record + 6, 0);
            w_u16(record + 4, 0);
            t3_800593c8(object, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
        }
        object += 6284;
    }
    return 0;
}

uint32 t3_800593c8(uint32 object, uint32 first, uint32 second, uint32 third)
{
    uint32 state;
    uint32 health;
    uint32 resource;
    uint32 actor;
    uint32 value;
    uint32 scale;
    uint32 offset;
    uint32 random;
    uint32 sequence;
    sint32 a;
    sint32 b;
    sint32 c;
    sint32 d;
    sint32 e;
    FUNCTION_MARKER(0x800593C8, "SLUS_004.02");
    state = 0x8009F318 + r_u8(object + 0x1886) * 816;
    health = r_u32(r_u32(0x800AFA5C) + 0x3F4);
    w_u16(state + 0x34, 0xFFFF);
    w_u16(state + 0x36, 0);
    w_u16(state + 0x3C, 0);
    w_u16(state + 0x4A, 0xFFFF);
    w_u16(state + 0x4C, 0);
    w_u16(state + 0x8A, 0xFFFF);
    w_u16(state + 0x48, 0xFFFF);
    w_u16(state + 0x38, 0xFFFF);
    w_u16(state + 0x3A, 0);
    w_u32(state + 0x58, 0);
    w_u32(state + 0xA0, 0);
    w_u16(state + 0x54, 0xFFFF);
    w_u16(state + 0x3E, 0);
    w_u16(state + 0x46, 0xFFFF);
    w_u16(state + 0x44, 0xFFFF);
    w_u16(state + 0x4E, 0xFFFF);
    w_u16(state + 0x2C, 0);
    w_u16(state + 0x2A, 0);
    w_u16(state + 0x42, 0xFFFF);
    w_u32(state + 0x60, 0);
    w_u16(state + 0x8C, 0xFFFF);
    w_u16(state + 0x1FE, 0xFFFF);
    w_u16(state + 0x6C, 0xFFFF);
    w_u16(state + 0x6A, 0);
    w_u16(state + 0x72, 0);
    w_u16(state + 0x70, 0);
    w_u16(state + 0xC0, 0);
    w_u16(state + 0xC2, 0);
    w_u32(state + 0x30, health);
    w_u16(state + 0xD4, 0);
    w_u16(state + 0xD8, 0);
    w_u16(state + 0xDA, 0);
    w_u16(state + 0x19C, 0);
    w_u16(state + 0x1CC, 0);
    w_u16(state + 0x1A0, 0);
    w_u16(state + 0x1A2, 0);
    w_u16(state + 0x1CE, 0);
    w_u16(state + 0x1D2, 0);
    w_u16(state + 0x96, 0);
    w_u16(state + 0x98, 0);
    w_u16(state + 0x9C, 0xFFFF);
    w_u32(state + 0xBC, 0);
    w_u16(state + 0x6E, 0);
    w_u32(state + 8, 0);
    w_u16(state + 0x216, 0xFFFF);
    w_u16(state + 0x1FC, 0);
    for (offset = 0; offset < 8; ++offset)
        w_u32(state + 0x1F8 - offset * 4, 0);
    w_u8(state + 0x8F, 0);
    w_u8(state + 0x8E, 0);
    w_u8(state + 0x91, 0);
    w_u8(state + 0x90, 0);
    w_u8(state + 0x92, 0);
    w_u8(state + 0x93, 0);
    w_u8(state + 0x94, 0);
    t3_80061488(state, first, second, third);
    resource = r_u32(0x800A867C);
    value = t3_80062868(1, (uint32)r_s16(resource + 22));
    actor = r_u32(0x800AFA5C);
    w_u32(state + 0x74, value);
    value = t3_80062868(0, (uint32)r_s16(actor + 22));
    scale = r_u16(0x800ADED4);
    w_u32(state + 0x78, value);
    if (scale) {
        a = r_s16(state + 0x23E);
        e = r_s16(state + 0x254);
        b = r_s16(state + 0x240);
        w_u16(state + 0x23E, (uint16)(a / 2));
        c = r_s16(state + 0x242);
        w_u16(state + 0x240, (uint16)(b / 2));
        d = r_s16(state + 0x244);
        w_u16(state + 0x242, (uint16)(c / 2));
        w_u16(state + 0x244, (uint16)(d / 2));
        w_u16(state + 0x254, (uint16)(e / 4));
    }
    w_u16(state + 0x68, r_u16(state + 0x23E));
    random = (uint32)randPSX();
    sequence = r_u32(0x800ADCA0);
    random = ((random + sequence) & 0x7FFF) * 30;
    w_u32(0x800ADCA0, sequence * 5 + 3);
    w_u16(state + 0x40, (uint16)((random >> 15) + 30));
    value = 0x800A01DC;
    for (offset = 0; offset < 180; ++offset) {
        w_u16(value + 8, 0xFFFF);
        value -= 12;
    }
    return value;
}

uint32 t3_8006aa9c(uint32 object)
{
    uint32 offset;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    uint32 fifth;
    uint32 sixth;
    FUNCTION_MARKER(0x8006AA9C, "SLUS_004.02");
    offset = r_u8(object + 30) * 12;
    w_u32(0x800A0578 + offset, r_u32(object));
    w_u32(0x800A057C + offset, r_u32(object + 4));
    w_u32(0x800A0580 + offset, r_u32(object + 8));
    w_u32(0x800A0528 + offset, r_u32(object + 0x1B8));
    w_u32(0x800A052C + offset, r_u32(object + 0x1BC));
    w_u32(0x800A0530 + offset, r_u32(object + 0x1C0));
    w_u32(0x800A054C + offset, r_u32(object + 0x1D0));
    w_u32(0x800A0550 + offset, r_u32(object + 0x1D4));
    w_u32(0x800A0554 + offset, r_u32(object + 0x1D8));
    first = r_u32(object + 0x1B8);
    second = r_u32(object + 0x1BC);
    third = r_u32(object + 0x1C0);
    fourth = r_u32(object + 0x1D0);
    fifth = r_u32(object + 0x1D4);
    sixth = r_u32(object + 0x1D8);
    w_u32(object + 0x1AC, first);
    w_u32(object + 0x1B0, second);
    w_u32(object + 0x1B4, third);
    w_u32(object + 0x1C4, fourth);
    w_u32(object + 0x1C8, fifth);
    w_u32(object + 0x1CC, sixth);
    return first;
}

uint32 t3_80061488(uint32 state, uint32 first, uint32 second, uint32 third)
{
    uint32 scaled;
    uint32 template_source;
    uint32 block;
    uint32 opponent = 0;
    uint32 values[4];
    uint32 source;
    uint32 destination;
    uint32 result;
    sint32 left;
    sint32 right;
    uint32 left_bits;
    uint32 right_bits;
    FUNCTION_MARKER(0x80061488, "SLUS_004.02");
    if ((sint32)first <= 0)
        first = r_u16(0x800AE208);
    if ((sint32)second <= 0)
        second = r_u8(0x800AE211);
    scaled = r_u16(0x800ADED4);
    w_u16(state + 0x21E, (uint16)third);
    if (scaled)
        second = 4;
    if (first >= 3)
        first = 2;
    if (second >= 10)
        second = 9;
    template_source = 0x800231AC + first * 1100 + second * 110;
    for (block = 0; block < 2; ++block) {
        uint32 chunk;
        uint32 unaligned;
        source = block ? 0x80023E22 : template_source;
        destination = state + 0x238 + block * 110;
        unaligned = (source | destination) & 3;
        for (chunk = 0; chunk < 7; ++chunk) {
            uint32 words = chunk < 6 ? 4 : 3;
            uint32 word;
            uint16 tail = 0;
            for (word = 0; word < words; ++word) {
                uint32 address = source + word * 4;
                if (unaligned || chunk == 6) {
                    uint32 upper = r_u32((address + 3) & ~3u);
                    uint32 lower = r_u32(address & ~3u);
                    uint32 shift = (address & 3) * 8;
                    values[word] = shift ? (upper << (32 - shift)) | (lower >> shift) : lower;
                } else {
                    values[word] = r_u32(address);
                }
            }
            if (chunk == 6)
                tail = r_u16(source + 12);
            for (word = 0; word < words; ++word) {
                uint32 address = destination + word * 4;
                if (unaligned || chunk == 6) {
                    uint32 shift = (address & 3) * 8;
                    uint32 upper_address = (address + 3) & ~3u;
                    uint32 lower_address = address & ~3u;
                    if (shift) {
                        uint32 old = r_u32(upper_address);
                        uint32 mask = 0xFFFFFFFF >> (32 - shift);
                        w_u32(upper_address, (old & ~mask) | (values[word] >> (32 - shift)));
                        old = r_u32(lower_address);
                        mask = 0xFFFFFFFF << shift;
                        w_u32(lower_address, (old & ~mask) | (values[word] << shift));
                    } else {
                        w_u32(upper_address, values[word]);
                        w_u32(lower_address, values[word]);
                    }
                } else {
                    w_u32(address, values[word]);
                }
            }
            if (chunk == 6)
                w_u16(destination + 12, tail);
            source += 16;
            destination += 16;
        }
    }
    for (block = 0; block < 2; ++block) {
        uint32 word;
        if (!block) {
            uint32 actor = r_u32(0x800AFA5C);
            source = 0x80098260 + (uint32)r_s16(actor + 24) * 12;
            opponent = r_u32(0x800A867C);
        } else {
            source = 0x80098260 + (uint32)r_s16(opponent + 24) * 12;
        }
        for (word = 0; word < 3; ++word) {
            uint32 address = source + word * 4;
            uint32 upper = r_u32((address + 3) & ~3u);
            uint32 lower = r_u32(address & ~3u);
            uint32 shift = (address & 3) * 8;
            values[word] = shift ? (upper << (32 - shift)) | (lower >> shift) : lower;
        }
        destination = state + 0x314 + block * 12;
        for (word = 0; word < 3; ++word) {
            uint32 address = destination + word * 4;
            uint32 shift = (address & 3) * 8;
            uint32 upper_address = (address + 3) & ~3u;
            uint32 lower_address = address & ~3u;
            if (shift) {
                uint32 old = r_u32(upper_address);
                uint32 mask = 0xFFFFFFFF >> (32 - shift);
                w_u32(upper_address, (old & ~mask) | (values[word] >> (32 - shift)));
                old = r_u32(lower_address);
                mask = 0xFFFFFFFF << shift;
                w_u32(lower_address, (old & ~mask) | (values[word] << shift));
            } else {
                w_u32(upper_address, values[word]);
                w_u32(lower_address, values[word]);
            }
        }
    }
    left = r_s16(state + 0x26E);
    right = r_s16(state + 0x26A);
    left_bits = r_u16(state + 0x26E);
    right_bits = r_u16(state + 0x26A);
    w_u16(state + 0x1D4, (uint16)(left < right ? right_bits : left_bits));
    scaled = r_u32(0x800AFA88);
    result = (sint32)second < 3;
    if (scaled == 8) {
        if ((sint32)second >= 3)
            second = 3;
        result = 0x80022FA4 + second * 6;
        w_u32(state + 0x32C, result + (first & 3) * 30);
    }
    return result;
}

uint32 t3_8004e8b4(void)
{
    uint32 packet;
    uint32 ordering;
    uint32 row;
    FUNCTION_MARKER(0x8004E8B4, "SLUS_004.02");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    for (row = 0; row < 2; ++row) {
        uint32 x = r_u16(0x80021F34 + row * 8);
        uint32 y = r_u16(0x80021F36 + row * 8);
        uint32 texture = r_u32(0x8009F2C8 + row * 4);
        sint32 width = r_s16(0x8009F2C0 + row * 2);
        uint32 left;
        uint32 right;
        uint32 top;
        uint32 bottom;
        uint32 palette;
        uint32 variant;
        if (row)
            x -= (uint32)width;
        packet = t3_8004d9bc(ordering, packet, 0x65000000,
            (x & 0xFFFF) | (y << 16), ((uint32)width & 0xFFFF) | 0x00100000, texture);
        packet = t3_8004daa8(ordering, packet, r_u16(0x8009F2D0 + row * 2));
        left = (x - 4) & 0xFFFF;
        top = (y + 9) << 16;
        right = (x - 4 + (uint32)width + 8) & 0xFFFF;
        variant = r_u16(0x8009F2D4 + row * 2);
        palette = r_u16(0x80021F2C + variant * 2);
        bottom = (y + 19) << 16;
        packet = t3_8004d830(ordering, packet, 0x2D000000,
            left | top, right | top, left | bottom, right | bottom,
            (palette << 16) | 0xA8DC, 0x000DA8DE, 0xB2DEB2DC);
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_8004e594(uint32 first, uint32 second)
{
    uint32 packet;
    uint32 ordering;
    uint32 row;
    FUNCTION_MARKER(0x8004E594, "SLUS_004.02");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    for (row = 0; row < 2; ++row) {
        uint32 x = row ? 202 : 151;
        uint32 step = row ? 14 : (uint32)-14;
        uint32 wins = row ? second : first;
        uint32 slot = 0;
        if (r_u16(0x800ADDFC)) {
            do {
                uint32 empty;
                uint32 texture;
                if ((sint32)slot >= (sint32)wins) {
                    empty = 1;
                } else {
                    empty = 0;
                    if (slot == wins - 1 && r_u8(0x80097D34 + row) != wins)
                        empty = (r_u32(0x800AE218) & 0x10) != 0;
                }
                texture = r_u32(0x80021F24 + empty * 4);
                packet = t3_8004d9bc(ordering, packet, 0x65000000,
                    (x & 0xFFFF) | 0x00460000, 0x0012000F, texture);
                x += step;
                ++slot;
            } while ((sint32)slot < (sint32)r_u16(0x800ADDFC));
        }
    }
    packet = t3_8004daa8(ordering, packet, 13);
    w_u32(0x800ADF64, packet);
    return packet;
}

void t3_8004eb54(uint32 context, uint32 actor, uint32 value)
{
    sint32 x;
    uint32 format;
    FUNCTION_MARKER(0x8004EB54, "SLUS_004.02");
    x = r_s16(0x80021FBC + actor * 2);
    switch (value) {
    case 1:
        t3_8004eee8(actor, (uint32)r_s16(0x80021FC0 + actor * 2), 24, 5);
        break;
    case 2:
        t3_8004ce74(0x80021FC8, (uint32)2, (uint32)0, (uint32)(x + 32), (uint32)24);
        break;
    case 3: {
        uint32 elapsed = r_u32(context + 44);
        uint32 round = r_u32(context + 36);
        uint32 adjustment = r_u32(context + 52);
        t3_bns_00_800b19b0(round + 1, elapsed + adjustment, (uint32)x, 24);
        break;
    }
    case 4:
    case 5:
    case 6:
    case 9:
        format = value == 4 ? 0x80021FDC : value == 5 ? 0x80021FF4 : value == 6 ? 0x8002200C : 0x80022024;
        t3_8004ce74(format, (uint32)6, (uint32)0, (uint32)x, (uint32)24);
        break;
    case 8:
        t3_bns_00_800b1c14(r_u32(context + 64), (uint32)x, 24);
        t3_bns_00_800b1b3c(r_u32(context + 68), (uint32)(x + 48), 24);
        break;
    case 10: {
        uint32 palette;
        uint32 score;
        if (!actor) {
            format = 0x8002203C;
            palette = 5;
            score = r_u32(0x800B6C08);
        } else {
            format = 0x80022050;
            palette = 2;
            score = r_u32(0x800B6C1C);
        }
        t3_8004ce74(format, palette, (uint32)0, (uint32)x, (uint32)24, (uint32)6, score);
        break;
    }
    default:
        break;
    }
}

uint32 t3_80045ca8(uint32 camera, uint32 matrix)
{
    MATRIX rotation = {0};
    uint32 index;
    uint32 sine;
    uint32 cosine;
    sint32 first;
    sint32 second;
    sint32 third;
    uint32 x;
    uint32 y;
    uint32 z;
    FUNCTION_MARKER(0x80045CA8, "SLUS_004.02");
    index = ((0 - r_u32(camera + 8)) & 0xFFF) * 2;
    sine = r_u16(0x8001E8C4 + index);
    cosine = r_u16(0x8001F0C4 + index);
    rotation.m[0][0] = (sint16)cosine;
    rotation.m[0][1] = (sint16)(0 - sine);
    rotation.m[1][0] = (sint16)sine;
    rotation.m[1][1] = (sint16)cosine;
    rotation.m[2][2] = 4096;
    index = ((0 - r_u32(camera + 4)) & 0xFFF) * 2;
    sine = r_u16(0x8001E8C4 + index);
    cosine = r_u16(0x8001F0C4 + index);
    w_u16(matrix + 2, 0);
    w_u16(matrix + 6, 0);
    w_u16(matrix + 8, 4096);
    w_u16(matrix + 10, 0);
    w_u16(matrix + 14, 0);
    w_u16(matrix, (uint16)cosine);
    w_u16(matrix + 4, (uint16)(0 - sine));
    w_u16(matrix + 12, (uint16)sine);
    w_u16(matrix + 16, (uint16)cosine);
    MulMatrix0((MATRIX *)psx_addr(matrix, 32), &rotation, (MATRIX *)psx_addr(matrix, 32));
    index = ((0 - r_u32(camera)) & 0xFFF) * 2;
    sine = r_u16(0x8001E8C4 + index);
    cosine = r_u16(0x8001F0C4 + index);
    rotation.m[0][0] = 4096;
    rotation.m[0][1] = 0;
    rotation.m[0][2] = 0;
    rotation.m[1][0] = 0;
    rotation.m[2][0] = 0;
    rotation.m[1][1] = (sint16)cosine;
    rotation.m[1][2] = (sint16)sine;
    rotation.m[2][1] = (sint16)(0 - sine);
    rotation.m[2][2] = (sint16)cosine;
    MulMatrix0(&rotation, (MATRIX *)psx_addr(matrix, 32), (MATRIX *)psx_addr(matrix, 32));
    first = r_s16(matrix + 6);
    second = r_s16(matrix + 8);
    w_u16(matrix + 6, (uint16)((first * 7122) / 4096));
    third = r_s16(matrix + 10);
    w_u16(matrix + 8, (uint16)((second * 7122) / 4096));
    w_u16(matrix + 10, (uint16)((third * 7122) / 4096));
    w_u32(matrix + 28, 0);
    w_u32(matrix + 24, 0);
    w_u32(matrix + 20, 0);
    x = r_u32(camera + 20);
    y = r_u32(camera + 24);
    z = r_u32(camera + 28);
    return t3_80081858(matrix, 0 - x, 0 - y, 0 - z,
        (uint32 *)psx_addr(matrix + 20, 4), (uint32 *)psx_addr(matrix + 24, 4),
        (uint32 *)psx_addr(matrix + 28, 4));
}

uint32 t3_8006cc28(uint32 source, uint32 packet, uint32 ordering, uint32 control)
{
    uint32 count;
    uint32 remaining;
    uint32 bytes = 0;
    FUNCTION_MARKER(0x8006CC28, "SLUS_004.02");
    count = r_u32(control);
    remaining = r_u32(control + 4);
    do {
        uint32 opcode = r_u32(source) & 0xFF000000;
        uint32 advance = 0;
        uint32 advance_pc = 0;
        if (opcode == 0x2C000000 || opcode == 0x24000000 || opcode == 0x28000000) {
            uint32 packed = r_u32(source + (opcode == 0x28000000 ? 4 : 16));
            uint32 vertices[4];
            uint32 coordinates[4];
            sint32 x[4];
            uint32 vertex;
            uint32 vertices_count = opcode == 0x24000000 ? 3 : 4;
            uint32 visible = 1;
            uint32 all_negative = 1;
            uint32 all_right = 1;
            for (vertex = 0; vertex < vertices_count; ++vertex)
                vertices[vertex] = 0x1F800000 + ((packed >> (vertex * 8)) & 255) * 4;
            if (vertices_count == 4) {
                for (vertex = 0; vertex < 4; ++vertex)
                    coordinates[vertex] = r_u32(vertices[vertex]);
                for (vertex = 0; vertex < 4; ++vertex) {
                    if ((sint32)coordinates[vertex] >= 0)
                        all_negative = 0;
                }
                if (all_negative) {
                    visible = 0;
                    advance_pc = opcode == 0x2C000000 ? 0x8006CD08 : 0x8006CF48;
                }
            }
            if (visible) {
                all_negative = 1;
                for (vertex = 0; vertex < vertices_count; ++vertex)
                    x[vertex] = r_s16(vertices[vertex]);
                for (vertex = 0; vertex < vertices_count; ++vertex) {
                    if (x[vertex] >= 0)
                        all_negative = 0;
                    if (x[vertex] < 368)
                        all_right = 0;
                }
                if (all_negative) {
                    visible = 0;
                    advance_pc = opcode == 0x2C000000 ? 0x8006CD3C : opcode == 0x28000000 ? 0x8006CF7C : 0x8006CECC;
                } else if (all_right) {
                    visible = 0;
                    advance_pc = opcode == 0x2C000000 ? 0x8006CD68 : opcode == 0x28000000 ? 0x8006CFA8 : 0x8006CECC;
                }
            }
            advance = opcode == 0x28000000 ? 8 : 20;
            if (visible) {
                uint32 length;
                uint32 decrement_pc;
                uint32 bytes_pc;
                uint32 packet_pc;
                uint32 tag;
                for (vertex = 0; vertex < vertices_count; ++vertex)
                    coordinates[vertex] = r_u32(vertices[vertex]);
                if (opcode == 0x28000000) {
                    uint32 command;
                    w_u32(packet + 8, coordinates[0]);
                    w_u32(packet + 12, coordinates[1]);
                    w_u32(packet + 16, coordinates[2]);
                    w_u32(packet + 20, coordinates[3]);
                    command = r_u32(source);
                    tag = r_u32(ordering);
                    w_u32(packet, 0x05000000);
                    w_u32(packet + 4, command);
                    w_u32(packet, (tag & 0xFFFFFF) | 0x05000000);
                    length = 24;
                    decrement_pc = 0x8006CFF8;
                    bytes_pc = 0x8006CFFC;
                    packet_pc = 0x8006D000;
                    advance_pc = 0x8006D008;
                } else {
                    uint32 command = r_u32(source);
                    uint32 uv_first = r_u32(source + 4);
                    uint32 uv_second = r_u32(source + 8);
                    uint32 uv_third;
                    uint32 uv_fourth = 0;
                    length = opcode == 0x2C000000 ? 40 : 32;
                    w_u32(packet, opcode == 0x2C000000 ? 0x09000000 : 0x07000000);
                    w_u32(packet + 4, command);
                    w_u32(packet + 8, coordinates[0]);
                    w_u32(packet + 12, uv_first);
                    w_u32(packet + 16, coordinates[1]);
                    w_u32(packet + 20, uv_second);
                    if (opcode == 0x2C000000) {
                        w_u32(packet + 24, coordinates[2]);
                        w_u32(packet + 32, coordinates[3]);
                    }
                    uv_third = r_u16(source + 12);
                    if (opcode == 0x2C000000)
                        uv_fourth = r_u16(source + 14);
                    tag = r_u32(ordering);
                    if (opcode == 0x2C000000) {
                        w_u32(packet + 28, uv_third);
                        w_u32(packet + 36, uv_fourth);
                    } else {
                        w_u32(packet + 24, coordinates[2]);
                        w_u32(packet + 28, uv_third);
                    }
                    w_u32(packet, (tag & 0xFFFFFF) | (opcode == 0x2C000000 ? 0x09000000 : 0x07000000));
                    decrement_pc = opcode == 0x2C000000 ? 0x8006CDD8 : 0x8006CEC0;
                    bytes_pc = opcode == 0x2C000000 ? 0x8006CDDC : 0x8006CEC4;
                    packet_pc = opcode == 0x2C000000 ? 0x8006CDE0 : 0x8006CEC8;
                    advance_pc = opcode == 0x2C000000 ? 0x8006CDE8 : 0x8006CECC;
                }
                w_u32(ordering, packet & 0xFFFFFF);
                if (remaining == 0x80000000) {
                    xport_mips_overflow_exception(decrement_pc);
                    return remaining;
                }
                --remaining;
                if ((sint64)(sint32)bytes + length > 0x7FFFFFFF) {
                    xport_mips_overflow_exception(bytes_pc);
                    return remaining;
                }
                bytes += length;
                if ((sint64)(sint32)packet + length > 0x7FFFFFFF) {
                    xport_mips_overflow_exception(packet_pc);
                    return remaining;
                }
                packet += length;
            }
            if ((sint64)(sint32)source + advance > 0x7FFFFFFF) {
                xport_mips_overflow_exception(advance_pc);
                return remaining;
            }
            source += advance;
        }
        if (count == 0x80000000) {
            xport_mips_overflow_exception(0x8006CC74);
            return remaining;
        }
        --count;
    } while ((sint32)remaining > 0 && (sint32)count > 0);
    w_u32(control, bytes);
    w_u32(control + 4, remaining);
    return remaining;
}

uint32 t3_80039960(uint32 index)
{
    uint32 record;
    uint32 azimuth;
    uint32 elevation;
    sint32 first;
    sint32 second;
    uint32 direction[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, (uint32)-100, 0}};
    uint32 color[3];
    uint32 half_color[3];
    uint32 row;
    uint32 component;
    uint32 brightness;
    uint32 result;
    FUNCTION_MARKER(0x80039960, "SLUS_004.02");
    if (r_u32(0x800AFA88) == 8 && r_u32(0x800B6C10))
        index = 12;
    record = 0x80096C78 + index * 28;
    azimuth = (r_u16(record + 14) - 2048) & 0xFFF;
    elevation = r_u16(record + 12) & 0xFFF;
    first = r_s16(0x8001F0C4 + azimuth * 2);
    second = r_s16(0x8001F0C4 + elevation * 2);
    direction[0][0] = (uint32)((first * second) / 4096);
    elevation = r_u16(record + 12) & 0xFFF;
    direction[0][1] = (uint32)r_s16(0x8001E8C4 + elevation * 2);
    azimuth = (r_u16(record + 14) - 2048) & 0xFFF;
    elevation = r_u16(record + 12) & 0xFFF;
    first = r_s16(0x8001E8C4 + azimuth * 2);
    second = r_s16(0x8001F0C4 + elevation * 2);
    direction[0][2] = (uint32)((first * second) / 4096);
    color[0] = r_u8(record + 10);
    color[1] = r_u8(record + 9);
    color[2] = r_u8(record + 8);
    if (index == 17) {
        half_color[0] = 0;
        half_color[1] = 0;
        half_color[2] = 0;
    } else {
        half_color[0] = r_u8(record + 10) >> 1;
        half_color[1] = r_u8(record + 9) >> 1;
        half_color[2] = r_u8(record + 8) >> 1;
    }
    for (row = 0; row < 3; ++row) {
        uint32 x = direction[row][0];
        uint32 y = direction[row][1];
        uint32 z = direction[row][2];
        uint32 length = t3_8002a324(x * x + y * y + z * z);
        if (length) {
            for (component = 0; component < 3; ++component)
                direction[row][component] = (uint32)((sint32)(direction[row][component] << 12) / (sint32)length);
        }
    }
    w_u16(0x8009E5A0, (uint16)(0 - (uint16)direction[0][0]));
    w_u16(0x8009E5A4, (uint16)(0 - (uint16)direction[0][2]));
    w_u16(0x8009E5A2, (uint16)(0 - (uint16)direction[0][1]));
    w_u16(0x8009E5A6, (uint16)(0 - (uint16)direction[1][0]));
    w_u16(0x8009E5A8, (uint16)(0 - (uint16)direction[1][1]));
    w_u16(0x8009E5AA, (uint16)(0 - (uint16)direction[1][2]));
    w_u16(0x8009E5AC, (uint16)(0 - (uint16)direction[2][0]));
    w_u16(0x8009E5AE, (uint16)(0 - (uint16)direction[2][1]));
    w_u16(0x8009E5B0, (uint16)(0 - (uint16)direction[2][2]));
    w_u16(0x8009E5C0, (uint16)(color[0] << 4));
    w_u16(0x8009E5C2, 0);
    w_u16(0x8009E5C4, (uint16)(half_color[0] << 4));
    w_u16(0x8009E5C6, (uint16)(color[1] << 4));
    w_u16(0x8009E5C8, 0);
    w_u16(0x8009E5CA, (uint16)(half_color[1] << 4));
    w_u16(0x8009E5CC, (uint16)(color[2] << 4));
    w_u16(0x8009E5CE, 0);
    result = half_color[2] << 4;
    w_u16(0x8009E5D0, (uint16)result);
    SetColorMatrix((MATRIX *)psx_addr(0x8009E5C0, 32));
    brightness = r_u16(record) >> 4;
    SetBackColor((sint32)brightness, (sint32)brightness, (sint32)brightness);
    return result;
}

uint32 t3_8004e274(uint32 first_value, uint32 first_maximum, uint32 second_value, uint32 second_maximum)
{
    uint32 packet;
    uint32 ordering;
    uint32 row;
    FUNCTION_MARKER(0x8004E274, "SLUS_004.02");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    for (row = 0; row < 2; ++row) {
        uint32 record = 0x80097D14 + row * 16;
        uint32 value = row ? second_value : first_value;
        uint32 maximum = row ? second_maximum : first_maximum;
        sint32 target = r_s16(record + 4);
        sint32 current = r_s16(record + 6);
        uint32 cached = r_u32(record);
        sint32 trailing = r_s16(record + 8);
        uint32 segments[3];
        uint32 x;
        uint32 y;
        uint32 cursor;
        uint32 top;
        uint32 bottom;
        uint32 segment;
        if (cached != value) {
            sint32 divisor = (sint32)maximum / 152;
            uint32 numerator;
            w_u32(record, value);
            numerator = value - 1 + (uint32)divisor;
            if (!divisor)
                target = (sint32)numerator < 0 ? 1 : -1;
            else if (numerator == 0x80000000 && divisor == -1)
                target = (sint32)0x80000000;
            else
                target = (sint32)numerator / divisor;
        }
        if (target >= 153)
            target = 152;
        if (current < trailing)
            trailing -= (trailing - current + 15) >> 4;
        else
            trailing = current;
        if (current < target) {
            current += 3;
            if (target < current)
                current = target;
        } else {
            current = target;
        }
        if (trailing < current)
            trailing = current;
        w_u16(record + 4, (uint16)target);
        w_u16(record + 6, (uint16)current);
        w_u16(record + 8, (uint16)trailing);
        segments[0] = (uint32)current;
        segments[1] = (uint32)trailing - (uint32)current;
        segments[2] = 152 - (uint32)trailing;
        x = r_u16(record + 12);
        y = r_u16(record + 14);
        packet = t3_8004d9bc(ordering, packet, 0x65000000,
            x | (y << 16), 0x00180006, 0x7EDEA8E0);
        packet = t3_8004d9bc(ordering, packet, 0x65000000,
            ((x + 150) & 0xFFFF) | (y << 16), 0x00180006, 0x7EDEA8EA);
        if (r_u16(record + 10) == 1)
            cursor = r_u16(record + 12) + 2;
        else
            cursor = r_u16(record + 12) + 154;
        y = r_u16(record + 14);
        top = y << 16;
        bottom = (y + 24) << 16;
        for (segment = 0; segment < 3; ++segment) {
            uint32 length = segments[segment];
            if (length) {
                uint32 left;
                uint32 right;
                uint32 palette;
                if (r_u16(record + 10) == 1) {
                    left = cursor;
                    cursor += length;
                } else {
                    cursor -= length;
                    left = cursor;
                }
                right = (left + length) & 0xFFFF;
                left &= 0xFFFF;
                palette = segment == 0 ? 0x7EDC : segment == 1 ? 0x7EDD : 0x7EDE;
                packet = t3_8004d830(ordering, packet, 0x2D000000,
                    left | top, right | top, left | bottom, right | bottom,
                    (palette << 16) | 0xA8E8, 0x000DA8E9, 0xC0E9C0E8);
            }
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_80043a94(uint32 object, uint32 opponent)
{
    uint32 angle;
    uint32 other_angle;
    uint32 difference;
    uint32 quadrant;
    uint32 result;
    FUNCTION_MARKER(0x80043A94, "SLUS_004.02");
    if (r_u8(object + 199)) {
        angle = r_u8(object + 200) ? (uint32)-16384 : 16384;
        w_u16(object + 56, (uint16)(angle - 32768));
        w_u16(object + 58, (uint16)(angle - 32768));
        w_u16(object + 42, (uint16)angle);
        difference = 0;
    } else {
        uint32 other_x = r_u32(opponent + 0xF68);
        uint32 other_z = r_u32(opponent + 0xF70);
        uint32 own_z = r_u32(object + 0xF70);
        uint32 own_x = r_u32(object + 0xF68);
        angle = t3_8002a224(other_z - own_z, other_x - own_x);
        w_u16(object + 42, (uint16)angle);
        w_u16(object + 56, (uint16)(angle - 32768));
        other_angle = r_u16(opponent + 44);
        angle = r_u16(object + 56);
        w_u16(object + 58, (uint16)other_angle);
        difference = other_angle - angle;
    }
    w_u16(object + 60, (uint16)difference);
    angle = r_u16(object + 44);
    other_angle = r_u16(object + 42);
    difference = angle - other_angle;
    quadrant = ((difference + 8192) & 0xFFFF) >> 14;
    w_u16(object + 46, (uint16)difference);
    w_u16(object + 50, (uint16)quadrant);
    angle = r_u16(object + 58);
    other_angle = r_u16(object + 56);
    difference = angle - other_angle;
    quadrant = ((difference + 8192) & 0xFFFF) >> 14;
    w_u16(object + 60, (uint16)difference);
    w_u16(object + 64, (uint16)quadrant);
    difference = r_u16(object + 46);
    w_u16(object + 48, (uint16)((difference & 0x8000) ? ~difference : difference));
    difference = r_u16(object + 60);
    w_u16(object + 62, (uint16)((difference & 0x8000) ? ~difference : difference));
    result = r_u32(object + 248) < 513;
    if (!result) {
        uint32 magnitude = r_u16(object + 48);
        angle = r_u16(object + 42);
        if (magnitude >= 16384)
            angle -= 32768;
        w_u16(object + 52, (uint16)angle);
        magnitude = r_u16(object + 62);
        result = r_u16(object + 56);
        if (magnitude >= 16384)
            result -= 32768;
        w_u16(object + 66, (uint16)result);
    }
    return result;
}

uint32 t3_80032178(void)
{
    uint32 doubled;
    uint32 boundary;
    uint32 mode;
    uint32 state;
    uint32 value;
    FUNCTION_MARKER(0x80032178, "SLUS_004.02");
    doubled = r_u32(0x8009BCB8) << 1;
    mode = r_u32(0x800AFA88);
    boundary = doubled - 2;
    if (mode == 7) {
        if (r_u32(0x800AFAC0) == 3 && r_s32(0x800AFAC4) >= 4)
            boundary = doubled - 32;
        if ((sint32)boundary < 0)
            boundary += 600;
    }
    if ((sint32)boundary < 0)
        boundary = 598;
    state = r_u32(0x8009BCA8);
    switch (state) {
    case 1: {
        uint32 old;
        if (r_u32(0x800AFA4C) & 0x10)
            t3_8004ce74(0x80019F58, (uint32)7, (uint32)1, (uint32)244, (uint32)30);
        old = r_u32(0x8009BCBC);
        w_u32(0x8009BCBC, old + 1);
        if (r_u32(0x800AFA88) == 7) {
            sint32 current = r_s32(0x800AFAC0);
            sint32 total = r_s32(0x800AFAC4);
            if (current < total)
                w_u32(0x8009BCBC, old + 2);
        }
        value = r_u32(0x8009BCBC);
        if ((sint32)value < 0)
            w_u32(0x8009BCBC, value + 0x4B00);
        value = (uint32)(r_s32(0x8009BCBC) % 600);
        w_u32(0x8009BCBC, value);
        if (value == boundary || r_u32(0x8009BCC8))
            w_u32(0x8009BCA8, 4);
        t3_8003368c();
        break;
    }
    case 2: {
        uint32 snapshot_base = r_u32(0x800ADE68);
        uint32 frame = (uint32)(r_s32(0x8009BCBC) >> 1);
        uint32 actor;
        w_u32(0x8009BCA8, 3);
        for (actor = 0; actor < 2; ++actor) {
            uint32 component;
            uint32 object = 0x800A9228 + actor * 6284;
            for (component = 0; component < 3; ++component) {
                uint32 destination = snapshot_base ? snapshot_base + 0x79E0 + actor * 52 : 0;
                uint32 base = r_u32(0x800ADE68);
                uint32 bank = r_u32(0x8009BCAC);
                uint32 source = base ? base + bank * 31200 + actor * 15600 + frame * 52 : 0;
                value = r_u32(object + 0xF68 + component * 4);
                w_u32(destination + component * 4 + 12, value);
                value = r_u32(source + component * 4 + 12);
                w_u32(object + 0xF68 + component * 4, value);
            }
        }
        t3_8006953c(0x800A9228, 0x800AAAB4);
        t3_800347b4(0x800A9228);
        t3_800347b4(0x800AAAB4);
        break;
    }
    case 3:
        w_u32(0x8009BCA8, 1);
        w_u32(0x8009BCC8, 0);
        break;
    case 4:
        w_u32(0x8009BCA8, 5);
        w_u32(0x800A8D6C, 20);
        t3_8002bfe8();
        return 0;
    case 5:
    case 8:
        return 0;
    case 6: {
        uint32 old = r_u32(0x8009BCBC);
        value = old + 2;
        w_u32(0x8009BCBC, value);
        if ((sint32)value < 0)
            w_u32(0x8009BCBC, old + 0x4B02);
        value = (uint32)(r_s32(0x8009BCBC) % 600);
        w_u32(0x8009BCBC, value);
        if (value == boundary)
            w_u32(0x8009BCCC, 0);
        t3_8003368c();
        break;
    }
    case 7: {
        uint32 old;
        value = r_u32(0x8009BCD0);
        if ((sint32)value <= 0x6FFFFFFF)
            w_u32(0x8009BCD0, value + 1);
        old = r_u32(0x8009BCB8);
        value = old + 1;
        w_u32(0x8009BCB8, value);
        if ((sint32)value < 0)
            w_u32(0x8009BCB8, old + 0x4B01);
        value = (uint32)(r_s32(0x8009BCB8) % 300);
        w_u32(0x8009BCB8, value);
        t3_800335e4();
        return 0;
    }
    default:
        break;
    }
    return r_u32(0x8009BCA8);
}
