#include "game_functions.h"
#include <stdarg.h>
#include <string.h>

uint32 t3_80029c10(uint32 state)
{
    uint32 result;
    uint32 port;
    uint32 high;
    uint32 low;
    uint32 buttons;
    uint32 controller_state;
    uint32 phase;
    uint32 kind;
    uint32 index;
    uint8 axes[4];
    FUNCTION_MARKER(0x80029C10, "SLUS_004.02");
    result = r_u8(state + 2);
    w_u8(state + 40, 0);
    w_u16(state, 0);
    if (result)
        return result;
    port = r_u8(state + 39);
    high = r_u8(state + 4);
    low = r_u8(state + 5);
    buttons = ~(low | (high << 8)) & 0xF9FF;
    controller_state = PadGetStatePSX(port);
    if (controller_state < 2)
        w_u8(state + 38, 0);
    result = (uint32)PadInfoMode((sint32)r_u8(state + 39), 2, 0);
    if (result) {
        phase = r_u8(state + 38);
        w_u8(state + 40, 9);
        if (phase) {
            if (phase == 1)
                w_u16(state, (uint16)buttons);
            return 1;
        }
        if (controller_state != 6)
            return 6;
        result = PadSetActAlignPSX(r_u8(state + 39), 0x80095458);
        if (result) {
            result = r_u8(state + 38) + 1;
            w_u8(state + 38, (uint8)result);
        }
        return result;
    }
    result = (uint32)PadInfoMode((sint32)r_u8(state + 39), 1, 0);
    w_u8(state + 40, (uint8)result);
    kind = r_u8(state + 40);
    if (kind == 4) {
        w_u16(state, (uint16)buttons);
        return 1;
    }
    if (kind >= 5) {
        if (kind == 7)
            w_u16(state, (uint16)buttons);
        else
            w_u8(state + 40, 0);
        return 7;
    }
    if (kind != 2) {
        w_u8(state + 40, 0);
        return 2;
    }
    for (index = 0; index < 4; ++index)
        axes[index] = r_u8(state + index + 6) >= 129;
    low = buttons & 0xF838;
    if (axes[1])
        low |= 0x40;
    if (axes[2])
        low |= 0x80;
    result = buttons & 8;
    if (axes[3]) {
        low |= 4;
        if (result)
            low |= 0x100;
    }
    w_u16(state, (uint16)low);
    return result;
}

uint32 t3_8006bf20(sint32 file, uint32 destination, uint32 offset)
{
    sint32 count;
    uint32 node;
    uint32 rank;
    uint32 start;
    uint32 length;
    uint32 previous;
    uint32 current;
    FUNCTION_MARKER(0x8006BF20, "SLUS_004.02");
    count = r_s16(0x800A069C);
    node = 0x800A06B8 + (uint32)count * 28;
    rank = r_u16(0x80098574 + (uint32)file * 2);
    if (count >= 8)
        t3_8007ae58(1);
    start = r_u32(0x80024400 + rank * 8);
    length = r_u32(0x80024404 + rank * 8);
    w_u16(node, (uint16)rank);
    w_u32(node + 8, destination);
    w_u32(node + 20, offset);
    w_u16(node + 2, 1);
    w_u32(node + 12, start);
    w_u32(node + 16, length);
    if ((sint32)length <= 0)
        return 1;
    // Native reads use the archive-relative sector at node + 12
    previous = 0;
    if (r_s16(0x800A069C) <= 0) {
        w_u32(node + 24, 0);
        w_u32(0x800A0698, node);
    }
    else {
        current = r_u32(0x800A0698);
        while ((sint32)rank >= r_s16(current)) {
            previous = current;
            current = r_u32(previous + 24);
            if (!current)
                break;
        }
        w_u32(node + 24, current);
        if (previous)
            w_u32(previous + 24, node);
        else
            w_u32(0x800A0698, node);
    }
    count = r_u16(0x800A069C) + 1;
    w_u16(0x800A069C, (uint16)count);
    return (uint32)count;
}

uint32 t3_80067be4(void)
{
    uint32 table;
    FUNCTION_MARKER(0x80067BE4, "SLUS_004.02");
    if (r_u32(0x80098504))
        return 0x80020000;
    for (table = 0x8002428C; table < 0x800242A4; table += 8) {
        uint32 sum = 0;
        uint32 cumulative = 0;
        uint32 count = r_u32(table + 4);
        uint32 record = r_u32(table);
        uint32 index;
        if ((sint32)count > 0) {
            uint32 limit = r_u32(table + 4);
            index = 0;
            do {
                uint32 signed_weight = (uint32)r_s16(record);
                uint32 weight = r_u16(record);
                sum += signed_weight;
                weight += cumulative;
                cumulative = (uint32)(sint32)(sint16)weight;
                w_u16(record, (uint16)weight);
                ++index;
                record += 20;
            } while ((sint32)index < (sint32)limit);
            count = r_u32(table + 4);
            record = r_u32(table);
        }
        if ((sint32)count > 0) {
            uint32 limit = r_u32(table + 4);
            index = 0;
            do {
                uint32 numerator = (uint32)r_s16(record) << 12;
                uint32 quotient;
                if (!sum)
                    quotient = (sint32)numerator < 0 ? 1 : 0xFFFFFFFF;
                else
                    quotient = (uint32)((sint32)numerator / (sint32)sum);
                w_u16(record, (uint16)quotient);
                ++index;
                record += 20;
            } while ((sint32)index < (sint32)limit);
        }
    }
    w_u32(0x80098504, 1);
    return 1;
}

uint32 t3_80031bfc(uint32 source, uint32 destination)
{
    uint32 start = destination;
    uint32 control;
    FUNCTION_MARKER(0x80031BFC, "SLUS_004.02");
    control = r_u8(source++);
    while (control) {
        if (control < 2) {
            control = r_u8(source++);
            continue;
        }
        if (control & 1) {
            uint32 byte = r_u8(source++);
            w_u8(destination++, (uint8)byte);
        }
        else {
            uint32 pair = r_u8(source++);
            uint32 distance;
            uint32 length;
            uint32 copy;
            uint32 index;
            pair = (pair << 8) + r_u8(source++);
            distance = pair & 0x7FF;
            if (!distance)
                distance = 2048;
            copy = destination - distance;
            length = (pair >> 11) & 31;
            if (!length)
                length = 32;
            for (index = 0; index < length; ++index) {
                uint32 byte = r_u8(copy++);
                w_u8(destination++, (uint8)byte);
            }
        }
        control >>= 1;
    }
    return destination - start;
}

uint32 t3_8004f8f8(uint32 mode)
{
    uint32 transitioned = 0;
    uint32 previous;
    FUNCTION_MARKER(0x8004F8F8, "SLUS_004.02");
    t3_8002960c(1);
    if (mode == 3) {
        uint32 buttons = r_u16(0x800ADD68);
        uint32 selection = r_u8(0x80097F20);
        if (buttons & 0xFFF3)
            buttons = 0;
        if (r_u8(0x80097F1F) && buttons) {
            if (buttons == 8)
                selection = 0;
            else if (buttons == 4)
                selection = 1;
            else if (buttons == 12)
                selection = 2;
            if (r_u8(0x80097F1F) >= selection)
                w_u8(0x80097F3E, 1);
            else
                selection = r_u8(0x80097F20);
        }
        else if (r_u8(0x80097F3E) == 1)
            ++selection;
        if (r_u8(0x80097F1F) < selection)
            selection = 0;
        w_u8(0x80097F20, (uint8)selection);
        transitioned = 1;
        if (r_u8(0x80097F3E) == 1)
            mode = 6;
        else
            transitioned = 0;
    }
    w_u8(0x80097F38, (uint8)mode);
    w_u8(0x80097F3A, (uint8)transitioned);
    previous = r_u8(0x800AE204);
    w_u16(0x800AE204, 2);
    w_u16(0x800AE224, 0);
    w_u8(0x80097F39, (uint8)previous);
    return 0x800B0000;
}

void t3_8004f750(sint32 mode, sint32 fade)
{
    uint32 packet;
    uint32 ordering_entry;
    FUNCTION_MARKER(0x8004F750, "SLUS_004.02");
    if (mode != 1 && mode != 2 && mode != 0)
        return;
    if (mode == 0 && fade < 256) {
        uint32 shade = 255 - (uint32)fade;
        uint32 color = shade | (shade << 8) | (shade << 16) | 0x62000000;
        packet = r_u32(0x800ADF64);
        ordering_entry = r_u32(0x800A9218) + 28;
        packet = t3_8004da10(ordering_entry, packet, color, 0x00F00075, 0x00120087);
        ordering_entry = r_u32(0x800A9218) + 28;
        w_u32(0x800ADF64, packet);
        w_u32(0x800ADF64, t3_8004daa8(ordering_entry, packet, 0x40));
    }
    if (mode != 2)
        t3_8004ce74(0x8002240C, 2, 0, 117, 240, 6);
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    w_u32(0x800ADF64, t3_8004da10(ordering_entry, packet, 0x60000000, 0, 0x01E00170));
}

uint32 t3_8004c470(void)
{
    uint32 remaining;
    uint32 flags;
    uint32 color = 2;
    uint32 packet;
    uint32 ordering_entry;
    FUNCTION_MARKER(0x8004C470, "SLUS_004.02");
    remaining = r_u8(0x800ADF61);
    if (!remaining)
        return 0;
    flags = r_u32(0x800AE218);
    w_u8(0x800ADF61, (uint8)(remaining - 1));
    if (!(flags & 2))
        color = 10;
    t3_8004ce74(0x800219DC, color, 1, 80, 240);
    packet = r_u32(0x800ADF64);
    ordering_entry = r_u32(0x800A9218) + 28;
    w_u32(0x800ADF64, t3_8004da10(ordering_entry, packet, 0x60000000, 0, 0x01E00170));
    return 1;
}

uint32 t3_8004e000(uint32 ordering_entry, uint32 packet, sint32 level)
{
    uint32 shade, blend;
    FUNCTION_MARKER(0x8004E000, "SLUS_004.02");
    if (level >= 512)
        level = 511;
    if (level == 256)
        return packet;
    if (level < 256) {
        shade = 255 - (uint32)level;
        blend = 2;
    }
    else {
        shade = (uint32)level - 256;
        blend = 1;
    }
    w_u32(packet + 4, shade | (shade << 8) | (shade << 16) | 0x62000000);
    w_u32(packet + 8, 0);
    w_u32(packet + 12, 0x01E00170);
    w_u32(packet, (r_u32(ordering_entry) & 0xFFFFFF) | 0x03000000);
    w_u32(ordering_entry, (packet & 0xFFFFFF) | (r_u32(ordering_entry) & 0xFF000000));
    packet += 16;
    SetDrawMode((DR_MODE *)psx_addr(packet, 12), 0, 1, (blend & 3) << 5, 0);
    w_u32(packet, (r_u32(ordering_entry) & 0xFFFFFF) | 0x02000000);
    w_u32(ordering_entry, (packet & 0xFFFFFF) | (r_u32(ordering_entry) & 0xFF000000));
    return packet + 12;
}

uint32 t3_8006c828(uint32 index)
{
    uint32 base;
    FUNCTION_MARKER(0x8006C828, "SLUS_004.02");
    base = r_u32(0x800ADD0C);
    return base ? base + index * 36 : 0;
}

uint32 t3_80046cd4(void)
{
    uint32 index;
    FUNCTION_MARKER(0x80046CD4, "SLUS_004.02");
    for (index = 0; index < 3; ++index)
        w_u16(0x8009E718 - index * 16, 100);
    return 0x8009E6E8;
}

uint32 t3_800774c4(void)
{
    uint32 index;
    FUNCTION_MARKER(0x800774C4, "SLUS_004.02");
    w_u16(0x800A35B8, 0);
    for (index = 0; index < 3; ++index)
        w_u16(0x800A340A + index * 64, 0);
    return t3_8004a40c();
}

uint32 t3_8004a40c(void)
{
    uint32 base;
    uint32 end;
    FUNCTION_MARKER(0x8004A40C, "SLUS_004.02");
    base = r_u32(0x800ADD10);
    w_u32(0x8009E844, base);
    end = base ? base + 2400 : 0;
    w_u32(0x8009E848, end);
    w_u32(0x8009E830, 1);
    w_u32(0x8009E834, base);
    w_u32(0x8009E840, end);
    w_u32(0x8009E83C, 0);
    w_u32(base + 4, 0);
    w_u32(base, end);
    return 0x800A0000;
}

uint32 t3_80042f1c(uint32 object)
{
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x80042F1C, "SLUS_004.02");
    first = r_u32(object);
    second = r_u32(object + 8);
    w_u32(object + 256, 1000);
    w_u32(object + 252, 1000);
    w_u32(object + 248, 1000);
    w_u32(object + 300, first);
    w_u32(object + 304, second);
    return 1000;
}

uint32 t3_8002d178(uint32 object, uint32 index)
{
    uint32 owner;
    uint32 table;
    FUNCTION_MARKER(0x8002D178, "SLUS_004.02");
    owner = r_u32(0x800ADC20 + (uint32)r_s16(object + 18) * 4);
    table = r_u32(owner + 12);
    return r_u32(table + (uint32)(sint32)(sint16)index * 4);
}

uint32 t3_8003f044(uint32 object)
{
    uint32 table;
    uint32 index;
    FUNCTION_MARKER(0x8003F044, "SLUS_004.02");
    table = r_u32(0x80096FF0 + (uint32)r_s16(object + 24) * 4);
    for (index = 0; index < 14; ++index) {
        sint32 value = r_s16(table);
        table += 2;
        w_u16(object + 536, (uint16)value);
        w_u32(object + 540, (uint32)(value * value));
        object += 20;
    }
    return 0;
}

uint32 t3_800346fc(uint32 object, uint32 value)
{
    uint32 index;
    FUNCTION_MARKER(0x800346FC, "SLUS_004.02");
    index = t3_80036294(object) & 255;
    return t3_800344a4(object, 3, index, value);
}

uint32 t3_800409e8(uint32 object)
{
    uint32 result;
    FUNCTION_MARKER(0x800409E8, "SLUS_004.02");
    w_u32(object + 952, r_u32(object + 3944));
    w_u32(object + 956, r_u32(object + 3948));
    result = r_u32(object + 3952);
    w_u32(object + 960, result);
    return result;
}

uint32 t3_800439a8(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x800439A8, "SLUS_004.02");
    if ((sint32)first >= 2)
        first = 1u << ((first - 1) & 31);
    if ((sint32)second >= 2)
        second = 1u << ((second - 1) & 31);
    return 0x8009E660 + (first + second) * 16;
}

uint32 t3_8004b348(uint32 value)
{
    sint32 input = (sint16)value;
    sint32 index = input / 4;
    uint32 result;
    FUNCTION_MARKER(0x8004B348, "SLUS_004.02");
    if (index < 0)
        index = -index;
    result = r_u8(0x800210C8 + (uint32)index) + (uint32)(index >> 1);
    if (input < 0)
        result = 0 - result;
    return (uint32)(sint32)(sint16)result;
}

uint32 t3_80045bb8(uint32 object)
{
    FUNCTION_MARKER(0x80045BB8, "SLUS_004.02");
    if (r_u8(object + 196))
        return r_u8(object + 33);
    if (r_u8(object + 135))
        return r_u8(object + 34);
    if (r_u8(object + 209))
        return r_u8(object + 35);
    return r_u8(object + 32);
}

uint32 t3_80043a40(uint32 first, uint32 second, uint32 value)
{
    sint32 character;
    uint32 count = 0;
    FUNCTION_MARKER(0x80043A40, "SLUS_004.02");
    character = r_s16(first + 24);
    if (character == 11 || character == 20)
        count = 1;
    character = r_s16(second + 24);
    if (character == 11 || character == 20)
        ++count;
    return value - count * 40;
}

void t3_80063cbc(void)
{
    FUNCTION_MARKER(0x80063CBC, "SLUS_004.02");
    w_u16(0x800A8CD4, 500);
    w_u16(0x800A8CD6, 500);
    w_u16(0x800A8CD8, 500);
    w_u16(0x800A8CDA, 1);
    w_u16(0x800A8CDC, 0);
    SetGeomScreen(500);
}

uint32 t3_80063fd4(uint32 *destination, uint32 index)
{
    uint32 source = 0x800A02A8 + index * 24;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    FUNCTION_MARKER(0x80063FD4, "SLUS_004.02");
    first = r_u32(source);
    second = r_u32(source + 4);
    third = r_u32(source + 8);
    fourth = r_u32(source + 12);
    destination[0] = first;
    destination[1] = second;
    destination[2] = third;
    destination[3] = fourth;
    first = r_u32(source + 16);
    second = r_u32(source + 20);
    destination[4] = first;
    destination[5] = second;
    return source;
}

uint32 t3_8002d154(uint32 index)
{
    FUNCTION_MARKER(0x8002D154, "SLUS_004.02");
    return r_u32(r_u32(0x800ADC20 + index * 4) + 12);
}

uint32 t3_80062868(uint32 alternate, uint32 key)
{
    uint32 index;
    FUNCTION_MARKER(0x80062868, "SLUS_004.02");
    for (index = 0; index < 10; ++index) {
        uint32 record = 0x80023134 + index * 12;
        if (key == r_u16(record))
            return r_u32(record + (alternate ? 4 : 8));
    }
    return 0;
}

uint32 t3_8004e238(void)
{
    uint32 index;
    FUNCTION_MARKER(0x8004E238, "SLUS_004.02");
    for (index = 0; index < 2; ++index) {
        uint32 record = 0x80097D14 + index * 16;
        w_u32(record, 0);
        w_u16(record + 4, 0);
        w_u16(record + 6, 0);
        w_u16(record + 8, 0);
    }
    return 0;
}

uint32 t3_80039034(void)
{
    uint32 index;
    FUNCTION_MARKER(0x80039034, "SLUS_004.02");
    for (index = 0; index < 2; ++index) {
        uint32 record = 0x8009E578 + index * 20;
        w_u16(record, 0);
        w_u16(record + 2, 910);
        w_u16(record + 4, 0);
        w_u8(record + 16, 0);
        w_u8(record + 17, 1);
    }
    return 0;
}

uint32 t3_80067cc4(void)
{
    FUNCTION_MARKER(0x80067CC4, "SLUS_004.02");
    w_u32(0x800A0458, 0x800A93BC);
    w_u32(0x800A045C, 0x800AAC48);
    w_u32(0x800A0460, 0x800AC4D4);
    return 0x800A0458;
}

uint32 t3_800690a0(uint32 first, uint32 second)
{
    sint32 remainder;
    uint32 value;
    FUNCTION_MARKER(0x800690A0, "SLUS_004.02");
    remainder = r_s16(0x800A04C4) % 4;
    w_u16(0x800A04C4, (uint16)remainder);
    value = r_u16(0x800242BC + (uint32)remainder * 2);
    w_u16(0x800A04C6, (uint16)value);
    return t3_80065ed8(first, second);
}

uint32 t3_80064020(uint32 first, uint32 second, uint32 third, uint32 fourth, uint32 fifth, uint32 sixth, uint32 index)
{
    uint32 record = 0x800A02A8 + index * 24;
    FUNCTION_MARKER(0x80064020, "SLUS_004.02");
    w_u32(record, first);
    w_u32(record + 4, second);
    w_u32(record + 8, third);
    w_u32(record + 12, fourth);
    w_u32(record + 16, fifth);
    w_u32(record + 20, sixth);
    return record;
}

void t3_80063c64(sint32 distance)
{
    sint32 minimum;
    FUNCTION_MARKER(0x80063C64, "SLUS_004.02");
    minimum = r_s16(0x800A8CD6);
    if (distance < minimum)
        distance = minimum;
    else {
        sint32 maximum = r_s16(0x800A8CD8);
        if (maximum < distance)
            distance = maximum;
    }
    SetGeomScreen(distance);
}

uint32 t3_80046234(uint32 first, uint32 second)
{
    uint32 result;
    FUNCTION_MARKER(0x80046234, "SLUS_004.02");
    if (r_u8(first + 199))
        result = r_u8(first + 200);
    else {
        sint32 left = r_s16(first + 4728);
        sint32 right = r_s16(second + 4728);
        result = left >= right;
    }
    w_u8(first + 198, (uint8)result);
    return result;
}

uint32 t3_8004cb8c(uint32 value)
{
    uint32 packed = 0;
    uint32 shift = 28;
    uint32 table = 0x80021BB8;
    uint32 divisor;
    FUNCTION_MARKER(0x8004CB8C, "SLUS_004.02");
    if (value > 100000000)
        return 0x99999999;
    divisor = r_u32(table);
    while (divisor) {
        divisor = r_u32(table);
        if (value >= divisor) {
            uint32 quotient = divisor ? value / divisor : 0xFFFFFFFF;
            value = divisor ? value % divisor : value;
            packed |= quotient << (shift & 31);
        }
        table += 4;
        divisor = r_u32(table);
        shift -= 4;
    }
    return packed + value;
}

static uint32 t3_render_text_core(uint32 text, const uint8 *native_text, uint32 native_strings, va_list *arguments)
{
    uint32 packet;
    uint32 ordering_entry;
    uint32 format = 0;
    uint32 zero_pad = 0;
    uint32 width = 0;
    uint32 character;
    uint32 color;
    uint32 font_offset;
    uint32 origin_x;
    uint32 origin_y;
    uint32 position_x;
    uint32 position_y;
    uint32 font;
    uint32 advance_x;
    uint32 advance_y;
    uint32 size_x;
    uint32 size_y;
    uint32 texture_page;
    uint32 combined;
    uint32 clut;
    uint32 disabled;
    disabled = r_u32(0x800ADCA4);
    if (disabled)
        return disabled;
    packet = r_u32(0x800ADF64);
    color = r_u8(0x8009F2B8);
    font_offset = r_u8(0x80021D72);
    w_u8(0x1F800392, (uint8)color);
    w_u8(0x1F800393, (uint8)font_offset);
    ordering_entry = r_u32(0x800A9218) + 28;
    origin_x = r_u16(0x8009F2B0);
    origin_y = r_u16(0x8009F2B2);
    position_x = r_u16(0x8009F2B4);
    position_y = r_u16(0x8009F2B6);
    font = r_u32(0x80021D64);
    advance_x = r_u16(0x80021D68);
    advance_y = r_u16(0x80021D6A);
    size_x = r_u16(0x80021D6C);
    size_y = r_u16(0x80021D6E);
    texture_page = r_u16(0x80021D70);
    color = r_u8(0x1F800392);
    font_offset = r_u8(0x1F800393);
    w_u8(0x1F800392, (uint8)color);
    combined = color + font_offset;
    clut = (((((combined >> 4) + 24) | 0x1E0) << 6) | (combined & 15) | 16) << 16;
    w_u16(0x1F800398, (uint16)origin_x);
    w_u16(0x1F80039A, (uint16)origin_y);
    w_u16(0x1F800384, (uint16)position_x);
    w_u16(0x1F800386, (uint16)position_y);
    w_u32(0x1F800380, font);
    w_u16(0x1F800394, (uint16)advance_x);
    w_u16(0x1F800396, (uint16)advance_y);
    w_u16(0x1F800388, (uint16)size_x);
    w_u16(0x1F80038A, (uint16)size_y);
    w_u16(0x1F800390, (uint16)texture_page);
    w_u32(0x1F80038C, clut);
    character = native_text ? *native_text++ : r_u8(text++);
    while (character) {
        if (character == 32) {
            position_x = r_u16(0x1F800384);
            advance_x = r_u16(0x1F800394);
            w_u16(0x1F800384, (uint16)(position_x + advance_x));
            goto next_character;
        }
        if (character == 10) {
            origin_y = r_u16(0x1F80039A);
            advance_y = r_u16(0x1F800396);
            origin_x = r_u16(0x1F800398);
            origin_y += advance_y;
            w_u16(0x1F800384, (uint16)origin_x);
            w_u16(0x1F80039A, (uint16)origin_y);
            w_u16(0x1F800386, (uint16)origin_y);
            goto next_character;
        }
        if (character == '%') {
            if (format != 1) {
                width = 0;
                format = 1;
                zero_pad = 0;
                goto next_character;
            }
            format = 0;
        }
        if (format) {
            format = 0;
            switch (character) {
            case '0':
                zero_pad = 1;
                format = 1;
                break;
            case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8':
                width = character - '0';
                format = 1;
                break;
            case 'D': case 'd': case 'b': case 'x':
            {
                uint32 value = va_arg(*arguments, uint32);
                uint32 negative = 0;
                uint32 digits;
                uint32 shift;
                uint32 step;
                if (character == 'd' && (sint32)value < 0) {
                    value = 0 - value;
                    negative = 1;
                }
                if (character == 'D' || character == 'd')
                    value = t3_8004cb8c(value);
                digits = t3_8004ccd8(value);
                if (!digits)
                    digits = 1;
                if (!width)
                    width = digits;
                else {
                    if (negative)
                        --width;
                    if ((sint32)width < (sint32)digits || zero_pad)
                        digits = width;
                }
                if ((sint32)digits < (sint32)width) {
                    uint32 padding = width - digits;
                    uint32 advance;
                    if (character == 'b')
                        padding <<= 2;
                    advance = (uint32)r_s16(0x1F800394);
                    position_x = r_u16(0x1F800384);
                    w_u16(0x1F800384, (uint16)(position_x + padding * advance));
                }
                if (negative) {
                    uint32 size;
                    uint32 glyph;
                    uint32 tag;
                    position_x = r_u32(0x1F800384);
                    w_u32(packet + 4, 0x65000000);
                    w_u32(packet + 8, position_x);
                    advance_x = r_u16(0x1F800394);
                    size = r_u32(0x1F800388);
                    w_u16(0x1F800384, (uint16)(position_x + advance_x));
                    w_u32(packet + 16, size);
                    font = r_u32(0x1F800380);
                    clut = r_u32(0x1F80038C);
                    glyph = r_u16(font + 90);
                    w_u32(packet + 12, glyph | clut);
                    tag = r_u32(ordering_entry);
                    w_u32(packet, (tag & 0xFFFFFF) | 0x04000000);
                    tag = r_u32(ordering_entry);
                    w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
                    packet += 20;
                }
                step = character == 'b' ? 1 : 4;
                shift = character == 'b' ? digits * 4 - 1 : (digits - 1) * 4;
                while ((sint32)shift >= 0) {
                    uint32 digit = (uint32)((sint32)value >> (shift & 31));
                    uint32 size;
                    uint32 glyph;
                    uint32 tag;
                    digit &= character == 'b' ? 1 : 15;
                    digit = r_u8(0x80021DA4 + digit);
                    position_x = r_u32(0x1F800384);
                    shift -= step;
                    w_u32(packet + 4, 0x65000000);
                    w_u32(packet + 8, position_x);
                    advance_x = r_u16(0x1F800394);
                    size = r_u32(0x1F800388);
                    w_u16(0x1F800384, (uint16)(position_x + advance_x));
                    w_u32(packet + 16, size);
                    font = r_u32(0x1F800380);
                    glyph = r_u16(font + digit * 2);
                    clut = r_u32(0x1F80038C);
                    w_u32(packet + 12, glyph | clut);
                    tag = r_u32(ordering_entry);
                    w_u32(packet, (tag & 0xFFFFFF) | 0x04000000);
                    tag = r_u32(ordering_entry);
                    w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
                    packet += 20;
                }
                break;
            }
            case 's':
            {
                uint32 string = 0;
                const uint8 *native_string = 0;
                uint32 byte;
                if (native_strings)
                    native_string = va_arg(*arguments, const uint8 *);
                else
                    string = va_arg(*arguments, uint32);
                byte = native_strings ? *native_string++ : r_u8(string++);
                while (byte) {
                    if (byte == 32) {
                        position_x = r_u16(0x1F800384);
                        advance_x = r_u16(0x1F800394);
                        w_u16(0x1F800384, (uint16)(position_x + advance_x));
                    }
                    else {
                        uint32 size;
                        uint32 glyph;
                        uint32 tag;
                        position_x = r_u32(0x1F800384);
                        w_u32(packet + 4, 0x65000000);
                        w_u32(packet + 8, position_x);
                        advance_x = r_u16(0x1F800394);
                        size = r_u32(0x1F800388);
                        w_u16(0x1F800384, (uint16)(position_x + advance_x));
                        w_u32(packet + 16, size);
                        font = r_u32(0x1F800380);
                        glyph = r_u16(font + byte * 2);
                        clut = r_u32(0x1F80038C);
                        w_u32(packet + 12, glyph | clut);
                        tag = r_u32(ordering_entry);
                        w_u32(packet, (tag & 0xFFFFFF) | 0x04000000);
                        tag = r_u32(ordering_entry);
                        w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
                        packet += 20;
                    }
                    byte = native_strings ? *native_string++ : r_u8(string++);
                }
                break;
            }
            case 'H': case 'h':
            {
                uint32 value;
                if (character == 'h') {
                    uint32 advance = (uint32)r_s16(0x1F800394);
                    value = va_arg(*arguments, uint32) * advance;
                }
                else
                    value = va_arg(*arguments, uint32);
                w_u16(0x1F800384, (uint16)value);
                w_u16(0x1F800398, (uint16)value);
                break;
            }
            case 'V': case 'v':
            {
                uint32 value;
                if (character == 'v') {
                    uint32 advance = (uint32)r_s16(0x1F800396);
                    value = va_arg(*arguments, uint32) * advance;
                }
                else
                    value = va_arg(*arguments, uint32);
                w_u16(0x1F800386, (uint16)value);
                w_u16(0x1F80039A, (uint16)value);
                break;
            }
            case 'c':
            {
                uint32 value = va_arg(*arguments, uint32);
                combined = value + r_u8(0x1F800393);
                clut = ((((((uint32)((sint32)combined >> 4) + 24) & 31) + 480) << 6) | (combined & 15) | 16) << 16;
                w_u8(0x1F800392, (uint8)value);
                w_u32(0x1F80038C, clut);
                break;
            }
            case 'p':
                packet = t3_8004daa8(ordering_entry, packet, r_u16(0x1F800390));
                ordering_entry = r_u32(0x800A9218) + va_arg(*arguments, uint32) * 4;
                break;
            case 'f':
            {
                uint32 selected = va_arg(*arguments, uint32);
                packet = t3_8004daa8(ordering_entry, packet, r_u16(0x1F800390));
                selected = 0x80021D64 + selected * 16;
                w_u32(0x1F800380, r_u32(selected));
                w_u16(0x1F800394, r_u16(selected + 4));
                w_u16(0x1F800396, r_u16(selected + 6));
                w_u16(0x1F800388, r_u16(selected + 8));
                w_u16(0x1F80038A, r_u16(selected + 10));
                w_u16(0x1F800390, r_u16(selected + 12));
                w_u8(0x1F800393, r_u8(selected + 14));
                color = r_u8(0x1F800392);
                font_offset = r_u8(0x1F800393);
                w_u8(0x1F800392, (uint8)color);
                combined = color + font_offset;
                clut = (((((combined >> 4) + 24) | 0x1E0) << 6) | (combined & 15) | 16) << 16;
                w_u32(0x1F80038C, clut);
                break;
            }
            case 'C':
                character = va_arg(*arguments, uint32) & 255;
                if (character == 32) {
                    position_x = r_u16(0x1F800384);
                    advance_x = r_u16(0x1F800394);
                    w_u16(0x1F800384, (uint16)(position_x + advance_x));
                    break;
                }
                goto draw_character;
            default:
                t3_8007ae58(0);
                goto draw_character;
            }
        }
        else {
draw_character:
            {
                uint32 size;
                uint32 glyph;
                uint32 tag;
                position_x = r_u32(0x1F800384);
                w_u32(packet + 4, 0x65000000);
                w_u32(packet + 8, position_x);
                advance_x = r_u16(0x1F800394);
                size = r_u32(0x1F800388);
                w_u16(0x1F800384, (uint16)(position_x + advance_x));
                w_u32(packet + 16, size);
                font = r_u32(0x1F800380);
                glyph = r_u16(font + character * 2);
                clut = r_u32(0x1F80038C);
                w_u32(packet + 12, glyph | clut);
                tag = r_u32(ordering_entry);
                w_u32(packet, (tag & 0xFFFFFF) | 0x04000000);
                tag = r_u32(ordering_entry);
                w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
                packet += 20;
            }
        }
next_character:
        character = native_text ? *native_text++ : r_u8(text++);
    }
    w_u32(0x800ADF64, t3_8004daa8(ordering_entry, packet, r_u16(0x1F800390)));
    w_u16(0x8009F2B0, r_u16(0x1F800398));
    w_u16(0x8009F2B2, r_u16(0x1F80039A));
    w_u16(0x8009F2B4, r_u16(0x1F800384));
    w_u16(0x8009F2B6, r_u16(0x1F800386));
    w_u32(0x8009F2B8, r_u8(0x1F800392));
    return 0x800A0000;
}

uint32 t3_8004ce74(uint32 text, ...)
{
    va_list arguments;
    uint32 result;
    FUNCTION_MARKER(0x8004CE74, "SLUS_004.02");
    va_start(arguments, text);
    result = t3_render_text_core(text, 0, 0, &arguments);
    va_end(arguments);
    return result;
}

uint32 t3_render_native_text(const uint8 *text, ...)
{
    va_list arguments;
    uint32 result;
    va_start(arguments, text);
    result = t3_render_text_core(0, text, 0, &arguments);
    va_end(arguments);
    return result;
}

uint32 t3_render_native_strings(uint32 text, ...)
{
    va_list arguments;
    uint32 result;
    va_start(arguments, text);
    result = t3_render_text_core(text, 0, 1, &arguments);
    va_end(arguments);
    return result;
}

void t3_8004fa60(void)
{
    uint32 phase;
    uint32 draw_mode = 3;
    uint32 fade = 0;
    uint32 value;
    uint32 old;
    FUNCTION_MARKER(0x8004FA60, "SLUS_004.02");
    if (r_u8(0x80097F3D)) {
        sint32 current = r_s16(0x800AE224);
        if (current >= 6 && current < 11 && t3_80050f38()) {
            t3_800753bc(0, 19648, 0);
            t3_8004f8f8(4);
            return;
        }
    }
    phase = (uint32)r_s16(0x800AE224);
    switch (phase) {
    case 0:
        t3_80055884(0);
        t3_8002960c(0);
        value = r_u8(0x80097F39);
        w_u16(0x800ADED4, 1);
        if (value - 2 >= 4 || r_u8(0x80097F3C) >= 4)
            w_u8(0x80097F3C, 0);
        value = r_u8(0x80097F3A);
        old = r_u8(0x80097F3B);
        if (value != old) {
            value = r_u8(0x80097F3A);
            w_u8(0x80097F3C, 0);
            w_u8(0x80097F3B, (uint8)value);
        }
        w_u16(0x800AE224, 5);
        value = r_u8(0x80097F38);
        if (value != 6 && (value != 3 || r_u8(0x80097F3E) == 3))
            w_u16(0x800AE224, 1);
        break;
    case 1:
        if (!r_u8(0x80097F3C)) {
            sint32 file = r_u8(0x80097F3A) ? 7 : 5;
            t3_800529cc(t3_80052b7c(), file);
            w_u8(0x80097F3C, 0);
        }
        w_u16(0x800AE224, 2);
        goto phase_two;
    case 2:
phase_two:
        t3_8004c3b8();
        w_u16(0x800AE224, 3);
        goto phase_three;
    case 3:
phase_three:
        if (!r_u8(0x80097F3C)) {
            if (t3_80052a70())
                break;
            if (r_u8(0x80097F3A))
                t3_bns_07_800d3604();
            else
                t3_bns_05_800db12c();
            w_u8(0x80097F3C, 2);
        }
        w_u16(0x800AE224, 4);
        goto phase_four;
    case 4:
phase_four:
        if (r_u8(0x80097F3C) < 3) {
            if (r_u8(0x80097F3A)) {
                if (t3_80052a70())
                    break;
                t3_bns_07_800d368c();
            }
            w_u8(0x80097F3C, 3);
        }
        w_u16(0x800AE224, 12);
        break;
    case 5:
        if (!r_u8(0x80097F3C)) {
            sint32 file = r_u8(0x80097F3A) ? 7 : 5;
            t3_800529cc(t3_80052b7c(), file);
            w_u8(0x80097F3C, 0);
        }
        w_u32(0x800AE214, 0);
        w_u16(0x800AE224, 6);
        goto phase_six;
    case 6:
phase_six:
        old = r_u32(0x800AE214);
        fade = old << 3;
        value = old + 1;
        w_u32(0x800AE214, value);
        draw_mode = 0;
        if ((sint32)value >= 33) {
            draw_mode = 1;
            value = r_u32(0x800AE218);
            w_u16(0x800AE224, 7);
            w_u32(0x800AE214, value);
        }
        break;
    case 7:
        if (!r_u8(0x80097F3D)) {
            t3_8004c370();
            w_u8(0x80097F3D, 1);
        }
        else
            t3_8004c3b8();
        w_u16(0x800AE224, 8);
        goto phase_eight;
    case 8:
phase_eight:
        draw_mode = 1;
        if (!r_u8(0x80097F3C)) {
            if (t3_80052a70())
                break;
            if (r_u8(0x80097F3A))
                t3_bns_07_800d3604();
            else
                t3_bns_05_800db12c();
            w_u8(0x80097F3C, 2);
        }
        w_u16(0x800AE224, 9);
        goto phase_nine;
    case 9:
phase_nine:
        draw_mode = 1;
        if (r_u8(0x80097F3C) < 3) {
            if (r_u8(0x80097F3A)) {
                if (t3_80052a70())
                    break;
                t3_bns_07_800d368c();
            }
            w_u8(0x80097F3C, 3);
        }
        value = r_u32(0x800AE218);
        old = r_u32(0x800AE214);
        if (value - old >= 91) {
            w_u32(0x800AE214, 32);
            w_u16(0x800AE224, 10);
        }
        break;
    case 10:
        old = r_u32(0x800AE214);
        fade = old << 3;
        value = old - 1;
        w_u32(0x800AE214, value);
        draw_mode = 0;
        if ((sint32)value <= 0) {
            draw_mode = 2;
            w_u32(0x800AE214, 0);
            w_u16(0x800AE224, 11);
        }
        break;
    case 11:
        draw_mode = 2;
        w_u16(0x800AE224, 12);
        goto phase_twelve;
    case 12:
phase_twelve:
        if (t3_8004c470())
            return;
        w_u16(0x800AE224, 0);
        w_u16(0x800AE204, r_u8(0x80097F38));
        break;
    default:
        break;
    }
    t3_8004f750((sint32)draw_mode, (sint32)fade);
}

void t3_8004c3b8(void)
{
    char path[40];
    uint8 header[512];
    uint8 data[512];
    DIRENTRY record;
    uint32 result;
    FUNCTION_MARKER(0x8004C3B8, "SLUS_004.02");
    if (r_u8(0x80097F11) && r_u8(0x800ADF60)) {
        if (firstfile((const char *)psx_addr(0x800219D4, 7), &record) || !(result = t3_8004c51c(0))) {
            uint32 length = 0;
            uint32 part;
            for (part = 0; part < 2; ++part) {
                uint32 source = part ? 0x8002172C : 0x800219CC;
                uint32 byte;
                do {
                    byte = r_u8(source++);
                    path[length] = (char)byte;
                    if (byte)
                        ++length;
                } while (byte);
            }
            memset(data + 505, 0, 7);
            result = t3_8004bec0(path, header, 512, data, 512, 0);
        }
        w_u8(0x800ADF61, result ? 120 : 0);
    }
    w_u8(0x800ADF60, 0);
}

void t3_80050318(void)
{
    sint32 phase;
    FUNCTION_MARKER(0x80050318, "SLUS_004.02");
    phase = r_s16(0x800AE224);
    if (phase == 0) {
        t3_8002960c(1);
        w_u16(0x800AE224, 1);
    }
    else if (phase == 1) {
        t3_8002960c(0);
        w_u16(0x800AE224, 2);
    }
    else if (phase == 2) {
        uint32 file = r_u8(0x800AFA9E);
        uint32 previous = r_u8(0x800AFA9F);
        if (file != previous) {
            w_u8(0x800AFA9F, (uint8)file);
            t3_80052b88(file);
            t3_80052c30(7, 2);
        }
        w_u16(0x800AE224, 3);
    }
    else if (phase == 3) {
        uint32 state = r_u32(0x800AFA88);
        uint32 mode = state == 7 ? 2 : state == 8 ? 3 : 1;
        t3_80055884(mode);
        w_u16(0x800AE204, 8);
        w_u16(0x800AE224, 0);
    }
}

void t3_80052cc4(void)
{
    uint32 index;
    uint32 record;
    uint32 phase;
    uint32 remaining;
    FUNCTION_MARKER(0x80052CC4, "SLUS_004.02");
    index = r_u8(0x8009815D);
    phase = (uint32)r_s16(0x800AE224);
    record = 0x8009811C + index * 8;
    switch (phase) {
    case 0:
        t3_8002960c(0);
        w_u16(0x800AE224, 1);
        return;
    case 1:
    {
        uint32 destination = r_u32(record + 4);
        uint32 file = r_u16(record + 2);
        t3_800529cc(destination, (sint32)file);
        w_u16(0x800AE224, 2);
        goto phase_two;
    }
    case 2:
phase_two:
        if (t3_80052a70())
            return;
        remaining = r_u8(0x8009815C) - 1;
        w_u8(0x8009815C, (uint8)remaining);
        if (remaining & 255) {
            index = r_u8(0x8009815D) + 1;
            w_u8(0x8009815D, (uint8)index);
            t3_80052a34(r_u16(record + 10));
        }
        w_u16(0x800AE224, 3);
        goto phase_three;
    case 3:
phase_three:
        if (r_u8(0x8009815C)) {
            w_u16(0x800AE224, 1);
            return;
        }
        goto phase_four;
    case 4:
phase_four:
        w_u16(0x800AE204, r_u8(0x8009815E));
        w_u16(0x800AE224, r_u8(0x8009815F));
        return;
    default:
        return;
    }
}

uint32 t3_8004c51c(uint32 port)
{
    uint32 index;
    FUNCTION_MARKER(0x8004C51C, "SLUS_004.02");
    for (index = 0; index < 4; ++index)
        TestEvent((sint32)r_u32(0x800ADF50 + index * 4));
    while (!_card_clear(port)) {
    }
    while (TestEvent((sint32)r_u32(0x800ADF50)) != 1 &&
           TestEvent((sint32)r_u32(0x800ADF54)) != 1 &&
           TestEvent((sint32)r_u32(0x800ADF58)) != 1 &&
           TestEvent((sint32)r_u32(0x800ADF5C)) != 1) {
    }
    for (index = 0; index < 4; ++index)
        TestEvent((sint32)r_u32(0x800ADF40 + index * 4));
    psx_bios_card_load_guest(port);
    for (;;) {
        if (TestEvent((sint32)r_u32(0x800ADF40)) == 1)
            return 0;
        if (TestEvent((sint32)r_u32(0x800ADF44)) == 1)
            return 1;
        if (TestEvent((sint32)r_u32(0x800ADF48)) == 1)
            return 2;
        if (TestEvent((sint32)r_u32(0x800ADF4C)) == 1)
            return 3;
    }
}

uint32 t3_8004c240(void)
{
    char path[40];
    uint8 header[512];
    uint8 data[512];
    uint32 length = 0;
    uint32 part;
    uint32 result;
    sint32 descriptor;
    sint32 read_bytes;
    uint32 previous;
    uint32 checksum;
    uint32 index;
    FUNCTION_MARKER(0x8004C240, "SLUS_004.02");
    for (part = 0; part < 2; ++part) {
        uint32 source = part ? 0x8002172C : 0x800219CC;
        uint32 byte;
        do {
            byte = r_u8(source++);
            path[length] = (char)byte;
            if (byte)
                ++length;
        } while (byte);
    }
    descriptor = openPSX(path, 1);
    if (descriptor < 0) {
        result = t3_8004c51c(0);
        if (result)
            return result;
    }
    read_bytes = readPSX(descriptor, header, 512);
    if (read_bytes != 512 || readPSX(descriptor, data, 512) != read_bytes) {
        closePSX(descriptor);
        return 5;
    }
    closePSX(descriptor);
    previous = data[0];
    checksum = previous;
    for (index = 1; index < 512; ++index) {
        uint32 encoded = data[index];
        uint32 decoded = encoded - (previous * 5 + 1);
        previous = encoded;
        checksum += decoded;
        data[index] = (uint8)decoded;
    }
    if (!(checksum & 255)) {
        for (index = 0; index < 72; ++index)
            w_u8(0x80097EF0 + index, data[index + 1]);
        for (index = 0; index < 432; ++index)
            w_u8(0x80097F4C + index, data[index + 73]);
    }
    return 0;
}

uint32 t3_8004ccf4(uint32 destination, uint32 source, sint32 count)
{
    uint32 index = 0;
    FUNCTION_MARKER(0x8004CCF4, "SLUS_004.02");
    if (count > 0) {
        do {
            uint32 byte = r_u8(source++);
            ++index;
            w_u8(destination++, (uint8)byte);
        } while ((sint32)index < count);
    }
    return destination;
}

void t3_8004bcc0(char *title)
{
    uint32 unlocked;
    uint32 first;
    uint32 second;
    uint32 completed;
    uint32 suffix;
    uint32 remaining;
    uint32 character;
    char *end;
    FUNCTION_MARKER(0x8004BCC0, "SLUS_004.02");
    unlocked = r_u32(0x80097EF0) & 0x1FFFFF;
    first = r_u32(0x80097EF8);
    second = r_u32(0x80097EFC);
    completed = (first | second) & 0x1FFFFF;
    end = t3_copy_guest_string_to_native(title, 0x80021744);
    if (unlocked == 1023) {
        suffix = 0x80021760;
        goto append_suffix;
    }
    if (unlocked == 0x1FFFFF && completed == unlocked) {
        suffix = 0x80021788;
        goto append_suffix;
    }
    if (!(unlocked & 0x20000)) {
        uint32 mode = r_u8(0x80097F26);
        if (mode) {
            suffix = mode == 1 ? 0x800217D8 : 0x80021804;
            goto append_suffix;
        }
    }
    if (r_u8(0x80097F12) < 22) {
        remaining = t3_8004ccac((unlocked ^ 0x1FFFFF) & 0x1FFFFF);
        if (remaining - 1 >= 14)
            return;
        end = t3_copy_guest_string_to_native(end, r_u32(0x80097C80 + remaining * 4));
        suffix = remaining == 1 ? 0x80097CC0 : 0x80097CE4;
        goto append_suffix;
    }
    if ((unlocked & 0x1DFFFF) == 0x15FFFF) {
        suffix = 0x800217B0;
        goto append_suffix;
    }
    remaining = (completed ^ unlocked) & unlocked;
    character = 0;
    if (remaining) {
        while (!(remaining & (1 << character))) {
            ++character;
            if (character >= 22)
                break;
        }
        end = t3_copy_guest_string_to_native(end, 0x800219BC);
        end = t3_copy_guest_string_to_native(end, r_u32(0x80097C28 + character * 4));
        suffix = 0x800219C8;
    }
    else
        suffix = 0x80021788;
append_suffix:
    t3_copy_guest_string_to_native(end, suffix);
}

uint32 t3_80048470(void)
{
    uint32 count;
    uint32 index;
    uint32 buffer;
    uint32 first;
    FUNCTION_MARKER(0x80048470, "SLUS_004.02");
    count = r_u32(0x8009E778);
    if (!count)
        return 0;
    w_u32(0x8009E778, count - 1);
    index = r_u32(0x800ADEFC);
    buffer = r_u32(0x800A8C54);
    first = r_u32(buffer + 4) + 4092;
    return t3_8007ba74(first, 0x8009E758 + index * 16);
}

uint32 t3_8002c10c(uint32 object)
{
    FUNCTION_MARKER(0x8002C10C, "SLUS_004.02");
    if (r_s32(object + 1040) >= 0) {
        w_u32(object + 1040, 0xFFFFFFFF);
        w_u32(object + 1044, 0xFFFFFFFF);
        w_u32(object + 1048, 0xFFFFFFFF);
    }
    return 0xFFFFFFFF;
}

uint32 t3_8002c130(uint32 object)
{
    sint32 limit;
    uint32 current;
    uint32 next;
    FUNCTION_MARKER(0x8002C130, "SLUS_004.02");
    limit = r_s32(object + 1040);
    current = r_u32(object + 1044);
    next = current + 1;
    if ((sint32)current < limit)
        w_u32(object + 1044, next);
    return next;
}

uint32 t3_80045878(void)
{
    uint32 first;
    uint32 second;
    uint32 count;
    FUNCTION_MARKER(0x80045878, "SLUS_004.02");
    first = r_u8(0x800A92EC);
    second = r_u8(0x800AAB78);
    count = first != 0;
    if (second)
        ++count;
    if (r_u32(0x800AFA88) == 8 && r_u8(0x800AC404))
        ++count;
    return count;
}

uint32 t3_80046d00(uint32 object)
{
    uint32 index;
    uint32 value;
    uint32 record;
    FUNCTION_MARKER(0x80046D00, "SLUS_004.02");
    index = r_u8(object + 30);
    value = r_u32(object + 3944);
    record = 0x8009E6F8 + index * 16;
    w_u32(record + 4, value);
    value = r_u32(object + 3948);
    w_u32(record + 8, value);
    value = r_u32(object + 3952);
    w_u32(record + 12, value);
    return value;
}

uint32 t3_80039ddc(uint32 index)
{
    uint32 value;
    FUNCTION_MARKER(0x80039DDC, "SLUS_004.02");
    if (r_u32(0x800AFA88) == 8 && r_u32(0x800B6C10))
        index = 12;
    value = r_u32(0x80096C7C + index * 28);
    w_u32(0x800ADE1C, value);
    return 0x800B0000;
}

uint32 t3_80039f84(void)
{
    uint32 count;
    uint32 next;
    FUNCTION_MARKER(0x80039F84, "SLUS_004.02");
    count = r_u32(0x8009E5EC);
    next = count - 1;
    if ((sint32)count > 0) {
        w_u32(0x8009E5EC, next);
        if ((sint32)next <= 0) {
            w_u16(0x8009E5AA, 0);
            w_u16(0x8009E5A8, 0);
            w_u16(0x8009E5A6, 0);
            return 0x8009E5A0;
        }
    }
    return next;
}

uint32 t3_800378f8(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800378F8, "SLUS_004.02");
    result = r_u32(0x800ADCA4);
    if (!result) {
        result = r_u8(0x8009E550) + 1;
        w_u8(0x8009E550, (uint8)result);
    }
    return result;
}

uint32 t3_8003615c(uint32 object)
{
    uint32 result = 15;
    FUNCTION_MARKER(0x8003615C, "SLUS_004.02");
    if (r_s16(object + 24) == 15) {
        result = r_s16(object + 22) == 4;
        w_u32(object + 1984, result);
    }
    return result;
}

uint32 t3_80038970(uint16 *first, uint16 *second, uint16 *third, uint32 object)
{
    sint32 factor;
    sint32 value;
    FUNCTION_MARKER(0x80038970, "SLUS_004.02");
    factor = r_s16(object + 1262);
    value = ((sint16)*first * factor) >> 12;
    *first = (uint16)value;
    value = ((sint16)*second * factor) >> 12;
    *second = (uint16)value;
    value = ((sint16)*third * factor) >> 12;
    *third = (uint16)value;
    return (uint32)value;
}

void t3_8003b96c(uint32 matrix)
{
    union {
        uint32 words[8];
        MATRIX matrix;
    } snapshot;
    uint32 index;
    FUNCTION_MARKER(0x8003B96C, "SLUS_004.02");
    for (index = 0; index < 8; ++index)
        snapshot.words[index] = r_u32(matrix + index * 4);
    SetRotMatrix(&snapshot.matrix);
    SetTransMatrix(&snapshot.matrix);
}

uint32 t3_80036aa8(uint32 x, uint32 y, uint32 z, uint32 output)
{
    SVECTOR vertex;
    sint32 screen;
    sint32 depth;
    FUNCTION_MARKER(0x80036AA8, "SLUS_004.02");
    vertex.vx = (sint16)x;
    vertex.vy = (sint16)y;
    vertex.vz = (sint16)z;
    vertex.pad = 0;
    depth = gte_project_full_depth(&vertex, &screen, 0);
    w_u32(output, (uint32)screen);
    return (uint32)depth;
}

uint32 t3_80042fc8(void)
{
    uint32 current;
    FUNCTION_MARKER(0x80042FC8, "SLUS_004.02");
    current = r_u32(0x800954A0);
    if ((sint32)current < 2)
        w_u32(0x80097A5C, 0);
    else if (r_u32(0x80097A60) != current)
        w_u32(0x80097A5C, 1);
    w_u32(0x80097A60, r_u32(0x800954A0));
    return 0x80090000;
}

uint32 t3_800705c8(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x800705C8, "SLUS_004.02");
    state = r_u8(object + 14);
    if (!state)
        t3_8007061c(object);
    else if (state != 1)
        return 1;
    return t3_800707f4(object);
}

uint32 t3_80040a0c(uint32 object)
{
    uint32 current;
    uint32 previous;
    uint32 first;
    FUNCTION_MARKER(0x80040A0C, "SLUS_004.02");
    current = r_u32(object + 3944);
    previous = r_u32(object + 952);
    w_u32(object + 964, current - previous);
    current = r_u32(object + 3948);
    previous = r_u32(object + 956);
    first = r_u32(object + 3944);
    w_u32(object + 968, current - previous);
    current = r_u32(object + 3952);
    previous = r_u32(object + 960);
    w_u32(object + 952, first);
    w_u32(object + 972, current - previous);
    w_u32(object + 956, r_u32(object + 3948));
    current = r_u32(object + 3952);
    w_u32(object + 960, current);
    return current;
}

uint32 t3_80040a60(uint32 object)
{
    uint32 current_x;
    uint32 previous_x;
    uint32 previous_y;
    uint32 previous_z;
    uint32 current_y;
    uint32 current_z;
    uint32 result;
    FUNCTION_MARKER(0x80040A60, "SLUS_004.02");
    current_x = r_u32(object + 440);
    previous_x = r_u32(object + 428);
    previous_y = r_u32(object + 432);
    previous_z = r_u32(object + 436);
    w_u32(object + 976, current_x - previous_x);
    current_y = r_u32(object + 444);
    current_z = r_u32(object + 448);
    result = current_y - previous_y;
    w_u32(object + 980, result);
    w_u32(object + 984, current_z - previous_z);
    return result;
}

void t3_80044fc8(uint32 object)
{
    FUNCTION_MARKER(0x80044FC8, "SLUS_004.02");
    w_u16(object + 232, 0);
    w_u8(object + 212, 0);
    w_u8(object + 207, 0);
    w_u8(object + 208, 0);
    w_u8(object + 211, 0);
    w_u8(object + 222, 0);
}

uint32 t3_8004b720(uint32 object)
{
    FUNCTION_MARKER(0x8004B720, "SLUS_004.02");
    if (r_u8(object + 208))
        return t3_8004b770();
    if (r_s16(object + 116) < 0 && r_s16(object + 92) == 1)
        return t3_8004b770();
    return 1;
}

uint32 t3_8004d830(uint32 ordering_entry, uint32 packet, uint32 first, uint32 second, uint32 fourth, uint32 sixth, uint32 eighth, uint32 third, uint32 fifth, uint32 split)
{
    FUNCTION_MARKER(0x8004D830, "SLUS_004.02");
    w_u32(packet + 4, first);
    w_u32(packet + 8, second);
    w_u32(packet + 16, fourth);
    w_u32(packet + 24, sixth);
    w_u32(packet + 32, eighth);
    w_u32(packet + 12, third);
    w_u32(packet + 20, fifth);
    w_u32(packet + 28, split & 0xFFFF);
    w_u32(packet + 36, split >> 16);
    w_u32(packet, (r_u32(ordering_entry) & 0xFFFFFF) | 0x09000000);
    w_u32(ordering_entry, (packet & 0xFFFFFF) | (r_u32(ordering_entry) & 0xFF000000));
    return packet + 40;
}

uint32 t3_8004e128(uint32 number, uint32 x, uint32 y, uint32 palette)
{
    uint32 packet;
    uint32 ordering_entry;
    uint32 packed;
    uint32 index;
    FUNCTION_MARKER(0x8004E128, "SLUS_004.02");
    packet = r_u32(0x800ADF64);
    ordering_entry = r_u32(0x800A9218);
    packed = number == 0xFFFFFFFF ? 0xAB : t3_8004cb8c(number);
    palette <<= 16;
    for (index = 0; index < 2; ++index) {
        uint32 digit = packed & 15;
        uint32 texture;
        packed >>= 4;
        texture = r_u32(0x80021EE8 + digit * 4) | palette;
        packet = t3_8004d9bc(ordering_entry, packet, 0x65000000, (x & 0xFFFF) | (y << 16), 0x002E0010, texture);
        x -= 15;
    }
    packet = t3_8004daa8(ordering_entry, packet, 14);
    w_u32(0x800ADF64, packet);
    return packet;
}

void t3_8004eee8(uint32 controller, uint32 x, uint32 y, uint32 color)
{
    uint32 flags;
    uint32 index;
    uint32 table;
    uint32 offset;
    uint32 string;
    FUNCTION_MARKER(0x8004EEE8, "SLUS_004.02");
    flags = r_u32(0x800AE218);
    if (!(flags & 0x30))
        return;
    if (flags & 0x40) {
        index = t3_8002a0e4(controller) == 0;
        table = 0x8002216C;
    }
    else {
        uint32 active = t3_8002a0e4(controller);
        index = 0;
        if ((sint32)active > 0)
            index = controller ? 2 : 1;
        table = 0x80022134;
    }
    offset = r_u32(table + index * 8 + 4);
    string = r_u32(table + index * 8);
    t3_8004ce74(0x80022194, x - offset, y, color, string);
}

uint32 t3_8004f04c(uint32 first, uint32 second)
{
    uint32 result;
    uint32 left;
    uint32 value;
    FUNCTION_MARKER(0x8004F04C, "SLUS_004.02");
    result = r_u32(second + 8);
    if (!result)
        return result;
    left = r_u32(first);
    result = r_u32(second);
    if (left != result)
        return result;
    left = r_u32(first + 4);
    result = r_u32(second + 4);
    if (left != result)
        return result;
    result = r_u32(second + 12);
    if (!result) {
        result = r_u32(first + 12);
        if (result) {
            result = left ^ 1;
            w_u32(second + 4, (sint32)left < 2 ? result : 0);
            return result;
        }
    }
    value = r_u32(second + 4);
    result = value ^ 1;
    w_u32(first + 4, (sint32)value < 2 ? result : 0);
    return result;
}

uint32 t3_8004f274(uint32 object, uint32 character, uint32 variant)
{
    uint32 packed = character * 4 + (variant & 3);
    uint32 index = (sint32)packed < 93 ? packed : 88;
    uint32 record;
    FUNCTION_MARKER(0x8004F274, "SLUS_004.02");
    record = r_u32(0x80097D40 + index * 4);
    w_u16(object + 24, (uint16)character);
    w_u16(object + 20, (uint16)packed);
    w_u16(object + 22, r_u8(record + 9));
    w_u16(object + 26, r_u8(record + 8));
    return t3_800361a8(object);
}

uint32 t3_8005101c(void)
{
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x8005101C, "SLUS_004.02");
    if (r_u32(0x800AFA88) == 6) {
        first = r_u16(0x800ADF08);
        second = r_u16(0x800ADF0A);
        if (!((first | second) & 0x800))
            return 0;
    }
    else {
        first = r_u16(0x800ADD68) & 0xFFF3;
        second = r_u16(0x800ADD6A) & 0xFFF3;
        if (first == 0x900 && (r_u16(0x800ADF08) & 0x100))
            goto reset;
        if (second == 0x900 && (r_u16(0x800ADF0A) & 0x100))
            goto reset;
        if (r_u8(0x80098A41) != 3)
            return 0;
    }
reset:
    w_u8(0x80098A41, 0);
    t3_8006ab64();
    t3_8004b61c(1);
    t3_80028f5c();
    t3_8004b5f4();
    t3_8004f8f8(4);
    return 1;
}

uint32 t3_800520ac(uint32 object)
{
    uint32 x;
    uint32 y;
    uint32 scale;
    FUNCTION_MARKER(0x800520AC, "SLUS_004.02");
    x = (uint32)r_s16(object + 4);
    scale = r_u32(0x800B8D5C);
    y = (uint32)r_s16(object + 6);
    if (scale) {
        uint32 delta_x = scale * (uint32)r_s16(object + 8);
        uint32 delta_y = scale * (uint32)r_s16(object + 10);
        x += (uint32)((sint32)delta_x >> 3);
        y += (uint32)((sint32)delta_y >> 3);
    }
    return t3_80051db8(object, x, y);
}

void t3_80052250(uint32 object)
{
    uint32 x;
    uint32 y;
    uint32 scale;
    uint32 text;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    uint32 kind;
    FUNCTION_MARKER(0x80052250, "SLUS_004.02");
    x = (uint32)r_s16(object + 4);
    scale = r_u32(0x800B8D5C);
    y = (uint32)r_s16(object + 6);
    if (scale) {
        uint32 delta_x = scale * (uint32)r_s16(object + 8);
        uint32 delta_y = scale * (uint32)r_s16(object + 10);
        x += (uint32)((sint32)delta_x >> 3);
        y += (uint32)((sint32)delta_y >> 3);
    }
    text = r_u32(object + 24);
    first = r_u16(object + 16);
    second = r_u16(object + 18);
    third = r_u32(object + 28);
    kind = r_u16(object);
    fourth = r_u32(object + 32);
    if (kind == 8 || !scale)
        t3_8004ce74(text, first, second, x, y, third, fourth);
}

uint32 t3_80051b8c(uint32 object)
{
    uint32 packet;
    uint32 scale;
    uint32 ordering_entry;
    uint32 x;
    uint32 y;
    uint32 extent;
    uint32 color;
    uint32 top;
    uint32 bottom;
    uint32 left;
    uint32 right;
    uint32 index;
    FUNCTION_MARKER(0x80051B8C, "SLUS_004.02");
    packet = r_u32(0x800ADF64);
    scale = r_u32(0x800B8D5C);
    ordering_entry = r_u32(0x800A9218);
    if (!scale) {
        x = (uint32)r_s16(object + 4);
        color = r_u32(object + 20);
        y = (uint32)r_s16(object + 6);
        extent = (uint32)r_s16(object + 8) - 110;
        top = y << 16;
        packet = t3_8004da10(ordering_entry, packet, color | 0x60000000, (x & 0xFFFF) | top, (extent & 0xFFFF) | 0x00240000);
        x += extent;
        right = (x + 110) & 0xFFFF;
        left = x & 0xFFFF;
        bottom = (y + 36) << 16;
        for (index = 0; index < 2; ++index) {
            uint32 texture_page;
            packet = t3_8004d8b0(ordering_entry, packet, color | 0x3A000000, 0, color, 0, left | top, right | top, left | bottom, right | bottom);
            texture_page = r_u32(0x80022688 + index * 4);
            packet = t3_8004daa8(ordering_entry, packet, texture_page);
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_8004ca40(uint32 destination, uint32 archive)
{
    sint32 count;
    uint32 index;
    uint32 entry = archive;
    FUNCTION_MARKER(0x8004CA40, "SLUS_004.02");
    t3_80029638(1);
    count = r_s32(archive);
    for (index = 0; (sint32)index < count; ++index) {
        T3_TIM_IMAGE image;
        uint32 source = r_u32(entry + 4) + archive;
        uint32 first;
        uint32 second;
        if ((sint32)t3_80031bfc(source, destination) > 33344)
            t3_8007ae58(1);
        if (t3_8007efc8(destination) == 0xFFFFFFFF)
            t3_8007ae58(1);
        t3_8007efd8(&image);
        first = r_u16(image.image_rectangle);
        second = r_u16(image.image_rectangle + 2);
        if (first | second)
            t3_8007c650(image.image_rectangle, image.image_pixels);
        if (image.mode & 8) {
            first = r_u16(image.clut_rectangle);
            second = r_u16(image.clut_rectangle + 2);
            if (first | second)
                t3_8007c650(image.clut_rectangle, image.clut_pixels);
        }
        DrawSync(0);
        entry += 8;
    }
    return t3_80029638(0);
}

uint32 t3_8004c7e4(uint32 destination, uint32 source, uint32 clut_x, uint32 clut_y, uint32 image_x, uint32 image_y)
{
    T3_TIM_IMAGE image;
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x8004C7E4, "SLUS_004.02");
    if ((sint32)t3_80031bfc(source, destination) > 33344)
        t3_8007ae58(1);
    if (t3_8007efc8(destination) == 0xFFFFFFFF)
        t3_8007ae58(1);
    t3_8007efd8(&image);
    w_u16(image.image_rectangle, (uint16)image_x);
    w_u16(image.image_rectangle + 2, (uint16)image_y);
    w_u16(image.clut_rectangle, (uint16)clut_x);
    w_u16(image.clut_rectangle + 2, (uint16)clut_y);
    first = r_u16(image.image_rectangle);
    second = r_u16(image.image_rectangle + 2);
    if (first | second)
        t3_8007c650(image.image_rectangle, image.image_pixels);
    if (image.mode & 8) {
        first = r_u16(image.clut_rectangle);
        second = r_u16(image.clut_rectangle + 2);
        if (first | second)
            t3_8007c650(image.clut_rectangle, image.clut_pixels);
    }
    return (uint32)DrawSync(0);
}

uint32 t3_8003c87c(uint32 object)
{
    uint32 state;
    FUNCTION_MARKER(0x8003C87C, "SLUS_004.02");
    state = r_u32(object + 84);
    if ((r_u32(state + 36) & 0x100) || r_u8(object + 186) || (r_u32(object + 136) & 0xFFFF00))
        return 1;
    return 0;
}
