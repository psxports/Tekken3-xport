#include "game_functions.h"

uint32 t3_80051970(void)
{
    FUNCTION_MARKER(0x80051970, "SLUS_004.02");
    return 0x800B8D58;
}

uint32 t3_bns_00_800b3240(void)
{
    uint32 packet;
    uint32 ordering;
    uint32 control;
    uint32 value;
    FUNCTION_MARKER(0x800B3240, "SLUS_004.02-BNS-logical-00");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    control = t3_80051970();
    value = r_u32(control + 4);
    if (value)
        return value;
    packet = t3_8004d8b0(ordering, packet, 0x3AE0E0E0, 0, 0x707070, 0,
        0x60008E, 0x600170, 0xB8008E, 0xB80170);
    packet = t3_8004d8b0(ordering, packet, 0x3A000000, 0x707070, 0, 0xE0E0E0,
        0x15E0000, 0x15E00E2, 0x1B60000, 0x1B600E2);
    packet = t3_8004daa8(ordering, packet, 32);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_00_800b3350(uint32 state)
{
    uint32 control;
    uint32 x;
    uint32 y;
    uint32 code;
    uint32 motion;
    uint32 flags;
    uint32 product_x;
    uint32 product_y;
    uint32 ordering;
    uint32 packet;
    FUNCTION_MARKER(0x800B3350, "SLUS_004.02-BNS-logical-00");
    control = t3_80051970();
    x = (uint32)r_s16(state + 4);
    y = (uint32)r_s16(state + 6);
    code = r_u32(state + 0x1C);
    motion = r_u32(control + 4);
    flags = r_u32(state + 0x20);
    if (motion) {
        product_x = motion * (uint32)r_s16(state + 8);
        product_y = motion * (uint32)r_s16(state + 10);
        x += (sint32)product_x >> 3;
        y += (sint32)product_y >> 3;
    }
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    packet = t3_8004b928(ordering, packet, x, y, code, flags);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_00_800b33e8(uint32 state)
{
    uint32 control;
    uint32 value;
    uint32 height;
    uint32 y;
    uint32 width;
    uint32 x;
    uint32 packet;
    uint32 ordering;
    FUNCTION_MARKER(0x800B33E8, "SLUS_004.02-BNS-logical-00");
    control = t3_80051970();
    value = r_u32(control + 4);
    if (value)
        return value;
    height = (uint32)r_s16(state + 10);
    y = (uint32)r_s16(state + 6);
    width = r_u16(state + 8);
    x = r_u16(state + 4);
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    packet = t3_8004da10(ordering, packet, 0x60E0E0E0,
        x | (y << 16), width | (height << 16));
    w_u32(0x800ADF64, packet);
    return packet;
}

void t3_bns_01_800b4388(uint32 control)
{
    sint32 mode;
    uint32 index;
    uint32 value;
    FUNCTION_MARKER(0x800B4388, "SLUS_004.02-BNS-logical-01");
    mode = r_s8(control + 0x77);
    if (mode == 0) {
        index = r_u8(control + 0x84);
        value = r_u8(control + 0x84);
        w_u8(control + 0x9D, 1);
        w_u8(control + 0x87, 0);
        w_u8(control + 0x9F, 0);
        w_u8(control + 0x86, 0);
        w_u8(control + 0x7C, 0);
        w_u8(control + 0x7B, value);
        t3_bns_01_800b7c24(index);
        t3_bns_01_800b744c(r_u8(control + 0x84));
        w_u8(control + 0xA3, 1);
    } else if (mode == 1) {
        index = r_u8(control + 0x84);
        value = r_u8(control + 0x84);
        w_u8(control + 0x9E, 0);
        w_u8(control + 0x81, 0);
        w_u8(control + 0x87, 0);
        w_u8(control + 0x9F, 0);
        w_u8(control + 0x86, 0);
        w_u8(control + 0x7C, 0);
        w_u8(control + 0x7B, value);
        t3_bns_01_800b7c24(index);
        t3_bns_01_800b744c(r_u8(control + 0x84));
        w_u8(control + 0xA3, 1);
    } else if (mode == 2) {
        value = r_u8(control + 0x84);
        w_u8(control + 0x7B, value);
        index = (uint32)r_s8(control + 0x7B);
        w_u8(control + 0x9E, 0);
        w_u8(control + 0x81, 0);
        w_u8(control + 0xA0, 1);
        w_u8(control + 0x87, 0);
        w_u8(control + 0x9F, 0);
        w_u8(control + 0x86, 0);
        w_u8(control + 0x7D, 0);
        w_u8(control + 0x7C, 0);
        t3_bns_01_800b7c24(index);
        t3_bns_01_800b744c((uint32)r_s8(control + 0x7B));
        w_u8(control + 0xA3, 2);
    }
}

uint32 t3_bns_01_800b7284(uint32 code, uint32 y, uint32 x)
{
    uint32 packet;
    uint32 suppressed;
    uint32 ordering;
    uint32 palette;
    FUNCTION_MARKER(0x800B7284, "SLUS_004.02-BNS-logical-01");
    packet = r_u32(0x800ADF64);
    suppressed = r_u32(0x800ADCA4);
    ordering = r_u32(0x800A9218);
    if (suppressed)
        return 0x800B0000;
    palette = ((((uint32)((sint32)code >> 4) + 24) & 31) + 480) << 6;
    palette |= (code & 15) | 16;
    packet = t3_8004d9bc(ordering, packet, 0x65000000,
        (x & 0xFFFF) | (y << 16), 0x10000A, (palette << 16) | 0xF0AA);
    packet = t3_8004daa8(ordering, packet, 31);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_01_800b7b3c(uint32 character, uint32 index)
{
    uint32 table;
    uint32 record;
    uint32 value;
    FUNCTION_MARKER(0x800B7B3C, "SLUS_004.02-BNS-logical-01");
    table = 0x800B25F8 + (character << 3);
    if (!r_u32(table + 4))
        return 0;
    record = r_u32(table) + index * 12;
    r_u32(record);
    r_u32(record + 4);
    value = r_u32(record + 8);
    return (uint32)(sint32)(sint16)value;
}

uint32 t3_bns_02_800b29cc(void)
{
    uint32 index;
    uint32 record = 0x800B6598;
    FUNCTION_MARKER(0x800B29CC, "SLUS_004.02-BNS-logical-02");
    for (index = 0; index < 2; ++index) {
        if (r_s16(record + 8) == 1)
            return record;
        record += 24;
    }
    return 0;
}

uint32 t3_bns_02_800b2a08(uint32 side)
{
    uint32 record = 0x800B6598 + (side & 1) * 24;
    FUNCTION_MARKER(0x800B2A08, "SLUS_004.02-BNS-logical-02");
    if (r_s16(record + 8) != 1)
        return 0;
    return r_u32(0x800B6C04) == 0;
}

uint32 t3_bns_02_800b2a4c(uint32 record)
{
    uint32 actor;
    FUNCTION_MARKER(0x800B2A4C, "SLUS_004.02-BNS-logical-02");
    actor = r_u32(record);
    w_u32(actor + 0x3F4, 0);
    w_u8(actor + 0xC3, 0);
    w_u8(actor + 0xC2, 1);
    w_u16(record + 8, 1);
    return 1;
}

uint32 t3_bns_02_800b2b3c(uint32 side, uint32 value)
{
    uint32 table = 0x800B6604 + (side & 1) * 8;
    sint32 product;
    uint32 bias;
    FUNCTION_MARKER(0x800B2B3C, "SLUS_004.02-BNS-logical-02");
    product = (sint32)(r_u32(table) * value);
    bias = r_u32(table + 4);
    return (uint32)(product / 1000) + bias;
}

uint32 t3_bns_02_800b2bb8(uint32 value)
{
    sint32 product;
    uint32 bias;
    FUNCTION_MARKER(0x800B2BB8, "SLUS_004.02-BNS-logical-02");
    product = (sint32)(r_u32(0x800B6614) * value);
    bias = r_u32(0x800B6618);
    return (uint32)(product / 1000) + bias;
}

uint32 t3_bns_02_800b2bf8(void)
{
    uint32 index;
    uint32 record = 0x800B64E8;
    FUNCTION_MARKER(0x800B2BF8, "SLUS_004.02-BNS-logical-02");
    for (index = 0; index < 3; ++index) {
        w_u32(record, 0);
        w_u16(record + 4, 0);
        w_u16(record + 6, 0);
        w_u16(record + 8, 0);
        record += 16;
    }
    return 0;
}

uint32 t3_bns_02_800b2c34(uint32 primary, uint32 second_record, uint32 third_record)
{
    uint32 packet;
    uint32 ordering;
    uint32 index;
    FUNCTION_MARKER(0x800B2C34, "SLUS_004.02-BNS-logical-02");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    for (index = 0; index < 3; ++index) {
        uint32 row = 0x800B64E8 + index * 16;
        uint32 actor;
        uint32 width;
        uint32 target;
        uint32 display;
        uint32 trailing;
        uint32 health;
        uint32 cached;
        uint32 maximum;
        uint32 limit;
        uint32 segments[3];
        uint32 x;
        uint32 y;
        uint32 right;
        uint32 top;
        uint32 bottom;
        uint32 cursor;
        uint32 part;
        if (index == 0) {
            actor = primary;
            width = 156;
        } else {
            uint32 record = index == 1 ? second_record : third_record;
            uint32 type = (uint32)r_s16(record + 14);
            actor = r_u32(record);
            width = r_u16(0x800B0670 + type * 4);
        }
        if (index > 0) {
            if (r_u8(actor + 0xC2))
                continue;
            if (!r_u32(0x80096F2C))
                continue;
        }
        target = (uint32)r_s16(row + 4);
        display = (uint32)r_s16(row + 6);
        trailing = (uint32)r_s16(row + 8);
        health = r_u32(actor + 0x3F4);
        cached = r_u32(row);
        maximum = r_u32(actor + 0x3F8);
        limit = width - 4;
        if (cached != health) {
            uint32 step;
            uint32 numerator;
            if (!limit)
                step = (sint32)maximum < 0 ? 1 : 0xFFFFFFFF;
            else
                step = (uint32)((sint64)(sint32)maximum / (sint32)limit);
            w_u32(row, health);
            numerator = health - 1 + step;
            if (!step)
                target = (sint32)numerator < 0 ? 1 : 0xFFFFFFFF;
            else
                target = (uint32)((sint64)(sint32)numerator / (sint32)step);
        }
        if ((sint32)limit < (sint32)target)
            target = limit;
        if ((sint32)display < (sint32)trailing) {
            uint32 decay = (uint32)((sint32)(trailing - display + 15) >> 4);
            trailing -= decay;
        } else {
            trailing = display;
        }
        if ((sint32)display < (sint32)target) {
            display += 3;
            if ((sint32)target < (sint32)display)
                display = target;
        } else {
            display = target;
        }
        if ((sint32)trailing < (sint32)display)
            trailing = display;
        w_u16(row + 4, target);
        w_u16(row + 6, display);
        w_u16(row + 8, trailing);
        segments[0] = display;
        segments[1] = trailing - display;
        segments[2] = limit - trailing;
        x = r_u16(row + 12);
        if (index > 0)
            x -= width - 156;
        right = x + width - 6;
        y = r_u16(row + 14) << 16;
        packet = t3_8004d9bc(ordering, packet, 0x65000000,
            (x & 0xFFFF) | y, 0x180006, 0x7EDEA8E0);
        packet = t3_8004d9bc(ordering, packet, 0x65000000,
            (right & 0xFFFF) | y, 0x180006, 0x7EDEA8EA);
        x = r_u16(row + 12);
        y = r_u16(row + 14);
        cursor = x - (width - 156) + limit + 2;
        top = y << 16;
        bottom = (y + 24) << 16;
        for (part = 0; part < 3; ++part) {
            uint32 length = segments[part];
            uint32 start;
            uint32 end;
            uint32 palette;
            if (!length)
                continue;
            start = cursor - length;
            if (r_u16(row + 10) == 1) {
                start = cursor;
                cursor = start + length;
            } else {
                cursor = start;
                length = segments[part];
            }
            if (!length)
                continue;
            palette = part == 0 ? 0x7EDC : (part == 1 ? 0x7EDD : 0x7EDE);
            end = start + length;
            packet = t3_8004d830(ordering, packet, 0x2D000000,
                (start & 0xFFFF) | top, (end & 0xFFFF) | top,
                (start & 0xFFFF) | bottom, (end & 0xFFFF) | bottom,
                (palette << 16) | 0xA8E8, 0xDA8E9, 0xC0E9C0E8);
        }
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_bns_02_800b32ac(void)
{
    uint32 packet;
    uint32 ordering;
    uint32 index;
    FUNCTION_MARKER(0x800B32AC, "SLUS_004.02-BNS-logical-02");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    for (index = 0; index < 3; ++index) {
        uint32 x;
        uint32 y;
        uint32 texture;
        uint32 width;
        uint32 page;
        uint32 left;
        uint32 right;
        uint32 top;
        uint32 bottom;
        uint32 palette;
        if (index > 0) {
            uint32 actor = t3_bns_02_800b298c(index - 1);
            if (r_u8(actor + 0xC2))
                continue;
            if (!r_u32(0x80096F2C))
                continue;
        }
        x = r_u16(0x800B0A00 + index * 8);
        y = r_u16(0x800B0A02 + index * 8);
        texture = r_u32(0x800B662C + index * 4);
        width = (uint32)r_s16(0x800B661C + index * 2);
        if (index)
            x -= width;
        packet = t3_8004d9bc(ordering, packet, 0x65000000,
            (x & 0xFFFF) | (y << 16), (width & 0xFFFF) | 0x100000, texture);
        page = r_u16(0x800B663C + index * 2);
        packet = t3_8004daa8(ordering, packet, page);
        top = (y + 9) << 16;
        right = (x + width + 4) & 0xFFFF;
        left = (x - 4) & 0xFFFF;
        bottom = (y + 19) << 16;
        palette = r_u16(0x800B6644 + index * 2);
        packet = t3_8004d830(ordering, packet, 0x2D000000,
            left | top, right | top, left | bottom, right | bottom,
            (palette << 16) | 0xA8DC, 0xDA8DE, 0xB2DEB2DC);
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_bns_02_800b3850(void)
{
    uint32 suppressed;
    uint32 buffer;
    uint32 packet;
    uint32 owner;
    uint32 ordering;
    uint32 index;
    uint32 mode;
    uint32 offset;
    uint32 left;
    uint32 right;
    uint32 selected;
    uint32 phase;
    uint32 scaled;
    sint32 wave;
    sint32 shade;
    sint32 quotient;
    sint32 half;
    FUNCTION_MARKER(0x800B3850, "SLUS_004.02-BNS-logical-02");
    if (!r_u32(0x80095494)) {
        phase = r_u32(0x800B6904);
        w_u32(0x800B6904, phase + 64);
    }
    suppressed = r_u32(0x800ADCA4);
    if (suppressed)
        return suppressed;
    buffer = r_u32(0x800ADEFC);
    packet = 0x800B683C + buffer * 100;
    for (index = 0; index < 5; ++index) {
        owner = r_u32(0x800A8C54);
        ordering = r_u32(owner + 4) + 12;
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        packet += 20;
    }
    mode = r_u32(0x800B6C20);
    offset = mode * 60;
    left = offset + 64;
    if (!r_u32(0x800B6908)) {
        uint32 elapsed = r_u32(0x800A85D4);
        uint32 start = r_u32(0x800B6C00);
        scaled = (elapsed - start) * 60;
        right = left + (uint32)((sint32)scaled / 32768);
        if ((sint32)(offset + 124) < (sint32)right)
            right = offset + 124;
        if (r_u32(0x800B6C04))
            w_u32(0x800B6908, 1);
    } else {
        right = offset + 124;
    }
    if (r_u32(0x800B6C20) == 4)
        right = left + 12;
    buffer = r_u32(0x800ADEFC);
    selected = 0x800B664C + buffer * 200;
    for (index = 0; index < 4; ++index) {
        if (index == r_u32(0x800B6C20)) {
            phase = r_u32(0x800B6904);
            wave = r_s16(0x8001E8C4 + (phase & 0xFFF) * 2);
            shade = (wave + 4096) / 64 + 128;
            if (shade >= 256)
                shade = 255;
            w_u8(selected + 4, shade);
            w_u8(selected + 5, shade);
            w_u8(selected + 6, shade);
            buffer = r_u32(0x800ADEFC);
            w_u16(selected + 8, left);
            w_u16(selected + 10, 436);
            w_u16(selected + 16, right);
            w_u16(selected + 18, 436);
            w_u16(selected + 24, left);
            w_u16(selected + 26, 452);
            w_u16(selected + 32, right);
            w_u16(selected + 34, 452);
            packet = 0x800B66EC + buffer * 200;
            owner = r_u32(0x800A8C54);
            w_u16(packet + 8, right);
            w_u16(packet + 10, 436);
            w_u16(packet + 16, left + 60);
            w_u16(packet + 18, 436);
            w_u16(packet + 24, right);
            w_u16(packet + 26, 452);
            w_u16(packet + 32, left + 60);
            w_u16(packet + 34, 452);
            ordering = r_u32(owner + 4) + 12;
            AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        }
        owner = r_u32(0x800A8C54);
        ordering = r_u32(owner + 4) + 12;
        AddPrim(psx_addr(ordering, 4), psx_addr(selected, 4));
        selected += 40;
    }
    if (r_u32(0x800B6C20) == 4) {
        buffer = r_u32(0x800ADEFC);
        phase = r_u32(0x800B6904);
        wave = r_s16(0x8001E8C4 + (phase & 0xFFF) * 2);
        packet = 0x800B66EC + buffer * 200;
        shade = (wave + 4096) / 64 + 128;
        if (shade >= 256)
            shade = 255;
        w_u8(packet + 4, shade);
        w_u8(packet + 5, shade);
        w_u8(packet + 6, shade);
        owner = r_u32(0x800A8C54);
        ordering = r_u32(owner + 4) + 12;
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
    }
    buffer = r_u32(0x800ADEFC);
    owner = r_u32(0x800A8C54);
    packet = 0x800B67DC + (buffer << 4);
    w_u16(packet + 8, right);
    w_u16(packet + 10, 436);
    w_u16(packet + 12, right);
    w_u16(packet + 14, 452);
    ordering = r_u32(owner + 4) + 8;
    AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
    phase = r_u32(0x800B6904);
    wave = r_s16(0x8001E8C4 + (phase & 0xFFF) * 2);
    scaled = (uint32)wave * 12;
    if ((sint32)scaled < 0)
        scaled += 4095;
    quotient = (sint32)scaled >> 12;
    buffer = r_u32(0x800ADEFC);
    packet = 0x800B67FC + buffer * 28;
    half = (sint32)((uint32)quotient + (scaled >> 31)) >> 1;
    w_u16(packet + 8, right - (uint32)half);
    owner = r_u32(0x800A8C54);
    w_u16(packet + 10, 424);
    w_u16(packet + 16, right + (uint32)half);
    w_u16(packet + 18, 424);
    w_u16(packet + 24, right);
    w_u16(packet + 26, 436);
    ordering = r_u32(owner + 4) + 12;
    AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
    return r_u32(ordering);
}

uint32 t3_bns_02_800b3e44(uint32 first, uint32 second)
{
    uint32 index;
    uint32 slot = 0x800B69AC;
    FUNCTION_MARKER(0x800B3E44, "SLUS_004.02-BNS-logical-02");
    for (index = 0; index < 2; ++index) {
        if (!r_u32(slot)) {
            w_u32(slot + 4, first);
            w_u32(slot + 12, second);
            w_u32(slot, 1);
            return 0;
        }
        slot += 188;
    }
    return 0;
}

uint32 t3_8003a134(uint32 actor)
{
    FUNCTION_MARKER(0x8003A134, "SLUS_004.02");
    if (r_u32(0x800AFA88) == 8) {
        uint32 side = (uint32)r_s16(actor + 0x12);
        w_u8(0x8009E5F0 + side, 1);
        return 1;
    }
    return 0x800A0000;
}

uint32 t3_bns_02_800b3e90(void)
{
    uint32 actor;
    uint32 index;
    FUNCTION_MARKER(0x800B3E90, "SLUS_004.02-BNS-logical-02");
    actor = t3_bns_02_800b297c();
    for (index = 0; index < 2; ++index) {
        uint32 base = 0x800B690C + index * 188;
        uint32 slot = base + 160;
        uint32 buffer = r_u32(0x800ADEFC);
        uint32 first_packet = base + buffer * 40;
        uint32 second_packet = first_packet + 80;
        uint32 state = r_u32(slot);
        if (state == 1) {
            uint32 now = r_u32(0x800A85D4);
            uint32 position = r_u32(slot + 4);
            uint32 actor_x;
            uint32 actor_z;
            uint32 item_x;
            uint32 item_z;
            uint32 distance_x;
            uint32 distance_z;
            uint32 health;
            if ((sint32)position < (sint32)(now - 5000)) {
                w_u32(slot, 0);
                continue;
            }
            if (!r_u32(0x800ADCA4)) {
                SVECTOR origin = { 0 };
                uint32 screen;
                uint32 x = r_u32(slot + 4);
                uint32 y = r_u32(slot + 8);
                uint32 z = r_u32(slot + 12);
                uint32 depth;
                uint32 bucket;
                uint32 width;
                uint32 height;
                uint32 left;
                uint32 right;
                uint32 top;
                uint32 bottom;
                uint32 owner;
                uint32 ordering;
                t3_8004adc4(x, y, z);
                depth = t3_80036ad4(&origin, &screen);
                bucket = (((depth & 0x3FFF) >> 3) - 1) << 2;
                if (!depth)
                    depth = 1;
                width = (uint32)((sint64)89600 / (sint32)depth);
                height = (uint32)((sint64)179200 / (sint32)depth);
                left = (screen & 0xFFFF) - (uint32)((sint32)width / 2);
                bottom = (uint32)((sint32)screen >> 16) - height;
                top = bottom - height;
                owner = r_u32(0x800A8C54);
                right = left + width;
                w_u16(first_packet + 8, left);
                w_u16(first_packet + 10, top);
                w_u16(first_packet + 16, right);
                w_u16(first_packet + 18, top);
                w_u16(first_packet + 24, left);
                w_u16(first_packet + 26, bottom);
                w_u16(first_packet + 32, right);
                w_u16(first_packet + 34, bottom);
                ordering = r_u32(owner + 4) + bucket;
                AddPrim(psx_addr(ordering, 4), psx_addr(first_packet, 4));
                owner = r_u32(0x800A8C54);
                top = bottom - width;
                w_u16(second_packet + 8, left);
                w_u16(second_packet + 10, top);
                w_u16(second_packet + 16, right);
                w_u16(second_packet + 18, top);
                w_u16(second_packet + 24, left);
                w_u16(second_packet + 26, bottom);
                w_u16(second_packet + 32, right);
                w_u16(second_packet + 34, bottom);
                ordering = r_u32(owner + 4) + bucket + 4;
                AddPrim(psx_addr(ordering, 4), psx_addr(second_packet, 4));
            }
            actor_x = r_u32(actor + 0xF68);
            item_x = r_u32(slot + 4);
            distance_x = actor_x - item_x;
            if ((sint32)distance_x < 0)
                distance_x = item_x - actor_x;
            actor_z = r_u32(actor + 0xF70);
            item_z = r_u32(slot + 12);
            distance_z = actor_z - item_z;
            if ((sint32)distance_z < 0)
                distance_z = item_z - actor_z;
            if ((sint32)distance_x >= 501 || (sint32)distance_z >= 501)
                continue;
            health = r_u32(actor + 0x3F4);
            if ((sint32)health <= 0)
                continue;
            if (r_u8(actor + 0xDB))
                continue;
            w_u32(actor + 0x3F4, health + 0x320000);
            t3_800753bc(0, 0x4E6A, 0);
            {
                uint32 x = r_u32(actor + 0x990);
                uint32 y = r_u32(actor + 0x994);
                uint32 z = r_u32(actor + 0x998);
                SVECTOR origin = { 0 };
                uint32 screen;
                uint32 timer;
                uint32 screen_x;
                uint32 screen_y;
                uint32 value;
                t3_8004adc4(x, y - 300, z);
                t3_80036ad4(&origin, &screen);
                screen_x = screen & 0xFFFF;
                screen_y = (uint32)((sint32)screen >> 16);
                timer = r_u32(0x800B6BF0);
                w_u32(slot, 2);
                w_u16(slot + 20, screen_x);
                w_u16(slot + 22, screen_y);
                w_u32(slot + 24, timer + 60);
                value = (uint32)r_s16(slot + 22);
                w_u16(slot + 20, screen_x - 36);
                w_u16(slot + 22, value << 4);
                t3_8003a134(actor);
            }
        } else if (state == 2) {
            uint32 color = (r_u32(0x800B6BF0) & 2) ? 2 : 5;
            uint32 y;
            uint32 x;
            uint32 end;
            uint32 now;
            if (!r_u32(0x80095494)) {
                y = r_u16(slot + 22);
                w_u16(slot + 22, y - 8);
            }
            y = r_u16(slot + 22);
            x = (uint32)r_s16(slot + 20);
            t3_8004ce74(0x800B0A28, 0, color, x,
                (uint32)((sint32)(y << 16) >> 20));
            end = r_u32(slot + 24);
            now = r_u32(0x800B6BF0);
            if (now == end - 15) {
                t3_800753bc(0, 0x87C0, 0);
                now = r_u32(0x800B6BF0);
            }
            end = r_u32(slot + 24);
            if ((sint32)end < (sint32)now)
                w_u32(slot, 0);
        }
    }
    return 0;
}

uint32 t3_bns_02_800b4440(void)
{
    uint32 buffer;
    uint32 packet;
    uint32 command;
    uint32 ordering;
    sint32 count;
    FUNCTION_MARKER(0x800B4440, "SLUS_004.02-BNS-logical-02");
    buffer = r_u32(0x800ADEFC);
    packet = 0x800B6A9C + buffer * 60;
    if (r_u32(0x800ADCA4))
        return 0x800B6A8C;
    command = 0x800B6A8C + (buffer << 3);
    count = r_s32(0x800B6BEC);
    if (count > 0) {
        ordering = r_u32(0x800A9218) + 28;
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        packet += 20;
        count = r_s32(0x800B6BEC);
    }
    if (count >= 2) {
        ordering = r_u32(0x800A9218) + 28;
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        packet += 20;
    }
    count = r_s32(0x800B6BEC);
    if (count >= 3) {
        ordering = r_u32(0x800A9218) + 28;
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
    }
    ordering = r_u32(0x800A9218) + 28;
    AddPrim(psx_addr(ordering, 4), psx_addr(command, 4));
    return r_u32(ordering);
}

uint32 t3_bns_02_800b4538(uint32 side)
{
    FUNCTION_MARKER(0x800B4538, "SLUS_004.02-BNS-logical-02");
    return t3_800753bc(0, (side & 1) ? 0x4CEB : 0x4C6C, 0);
}

uint32 t3_bns_02_800b4574(void)
{
    FUNCTION_MARKER(0x800B4574, "SLUS_004.02-BNS-logical-02");
    return 2352;
}

uint32 t3_bns_02_800b457c(void)
{
    FUNCTION_MARKER(0x800B457C, "SLUS_004.02-BNS-logical-02");
    return 330;
}

uint32 t3_bns_02_800b4914(void)
{
    VECTOR input;
    VECTOR rotated;
    SVECTOR origin = { 0 };
    MATRIX matrix;
    sint32 screen;
    sint32 projection;
    sint32 flags;
    uint32 buffer;
    uint32 numerator;
    uint32 divisor;
    uint32 quotient;
    uint32 span;
    uint32 offset;
    uint32 columns;
    uint32 base_y;
    uint32 first_column;
    uint32 packet;
    uint32 owner;
    uint32 bucket;
    uint32 ordering;
    uint32 tiles;
    uint32 palettes;
    uint32 command_cursor;
    uint32 sprite_cursor;
    uint32 arena;
    uint32 angle;
    uint32 cosine;
    uint32 sine;
    uint32 slope;
    uint32 x_start;
    uint32 row_y;
    uint32 row;
    uint32 bottom_count = 0;
    sint32 height;
    FUNCTION_MARKER(0x800B4914, "SLUS_004.02-BNS-logical-02");
    input.vx = r_s32(0x800A85D4);
    input.vy = r_s32(0x800A85D8);
    input.vz = r_s32(0x800A85DC);
    buffer = r_u32(0x800ADEFC);
    ApplyMatrixLV((MATRIX *)psx_addr(0x800ADF70, 32), &input, &rotated);
    numerator = (uint32)rotated.vx;
    divisor = r_u32(0x800B6BA4);
    quotient = t3_camera_divide(numerator, divisor);
    columns = r_u32(0x800B6B94);
    span = columns << 6;
    offset = quotient - t3_camera_divide(quotient, span) * span;
    w_u32(0x800B6C0C, numerator);
    if ((sint32)offset < 0)
        offset += span;
    t3_8003a490(&origin, &matrix);
    matrix.t[1] = 0;
    numerator = r_u32(0x800A85D4);
    matrix.t[2] = 2500;
    matrix.t[0] = (sint32)numerator;
    t3_8003785c_host(0x800ADF70, &matrix);
    RotTransPers(&origin, &screen, &projection, &flags);
    height = r_s32(0x800B6B98);
    base_y = (uint32)((sint32)screen >> 16) - ((uint32)height << 6);
    first_column = (uint32)((sint32)offset / 64);
    if (r_u32(0x800B6BB0) == 3 || r_u32(0x800B6C10)) {
        if (r_u32(0x800B6C10)) {
            packet = 0x800B6B14 + (r_u32(0x800ADEFC) << 4);
            w_u16(packet + 12, 368);
            w_u16(packet + 14, (uint32)((sint32)screen >> 16) - 16);
        }
        packet = 0x800B6B14 + (r_u32(0x800ADEFC) << 4);
        owner = r_u32(0x800A8C54);
        bucket = r_u32(0x800B6BA8);
        ordering = r_u32(owner + 4) + (bucket << 2) + 4;
        AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
        if (r_u32(0x800B6C10))
            return 0x800B0000;
    }
    tiles = r_u32(0x800B6B9C);
    arena = r_u32(0x800ADD58);
    palettes = r_u32(0x800B6BA0);
    command_cursor = arena ? arena + buffer * 336 + 1680 : 0;
    arena = r_u32(0x800ADD58);
    sprite_cursor = arena ? arena + buffer * 840 : 0;
    angle = (r_u32(0x800B6BAC) & 0xFFF) << 1;
    cosine = (uint32)r_s16(0x8001F0C4 + angle);
    quotient = t3_camera_divide(0xC0000, cosine);
    sine = (uint32)r_s16(0x8001E8C4 + angle);
    slope = quotient * sine;
    quotient = (uint32)((sint32)offset / 192);
    numerator = 0xB8000 - ((quotient * 192 - offset) << 12);
    quotient = t3_camera_divide(numerator, cosine);
    x_start = ((first_column << 6) - offset) << 12;
    height = r_s32(0x800B6B98);
    row_y = (base_y << 12) + quotient * sine;
    if (height <= 0)
        return (uint32)height;
    row = 0;
    do {
        uint32 column = first_column;
        uint32 x = x_start;
        uint32 y = row_y;
        uint32 cell;
        uint32 sprite = sprite_cursor;
        uint32 command = command_cursor;
        for (cell = 0; cell < 7; ++cell) {
            if ((sint32)y >= -262144 && (sint32)y <= 1966080) {
                uint32 remainder;
                uint32 tile_index;
                uint32 tile;
                uint32 palette;
                uint32 page;
                columns = r_u32(0x800B6B94);
                remainder = column - t3_camera_divide(column, columns) * columns;
                tile_index = (remainder + row * columns) << 1;
                tile = r_u16(tiles + tile_index);
                palette = r_u16(palettes + tile_index);
                w_u16(sprite + 14, palette);
                w_u8(sprite + 12, (tile & 3) << 6);
                w_u8(sprite + 13, (tile << 4) & 0xC0);
                page = ((tile >> 8) & 16) | ((tile >> 8) & 15);
                w_u16(sprite + 8, (uint32)((sint32)x / 4096));
                w_u16(sprite + 10, (uint32)((sint32)y / 4096));
                sprite_cursor += 20;
                command_cursor += 8;
                packet = sprite;
                sprite += 20;
                owner = r_u32(0x800A8C54);
                bucket = r_u32(0x800B6BA8);
                ordering = r_u32(owner + 4) + (bucket << 2);
                AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
                w_u8(command + 3, 1);
                w_u32(command + 4, page | 0xE1000200);
                packet = command;
                command += 8;
                owner = r_u32(0x800A8C54);
                bucket = r_u32(0x800B6BA8);
                ordering = r_u32(owner + 4) + (bucket << 2);
                AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
            }
            if (r_u32(0x800B6BB0) == 3 && row == r_u32(0x800B6B98) - 1) {
                uint32 remainder;
                columns = r_u32(0x800B6B94);
                remainder = column - t3_camera_divide(column, columns) * columns;
                if (!remainder) {
                    buffer = r_u32(0x800ADEFC);
                    packet = 0x800B6B34 + buffer * 48 + (bottom_count << 4);
                    w_u16(packet + 8, (uint32)((sint32)x / 4096));
                    ++bottom_count;
                    w_u16(packet + 10, (uint32)((sint32)y / 4096) + 64);
                    owner = r_u32(0x800A8C54);
                    bucket = r_u32(0x800B6BA8);
                    ordering = r_u32(owner + 4) + (bucket << 2);
                    AddPrim(psx_addr(ordering, 4), psx_addr(packet, 4));
                }
            }
            ++column;
            x += 0x40000;
            if ((sint32)column % 3 == 0)
                y -= slope;
        }
        ++row;
        height = r_s32(0x800B6B98);
        row_y += 0x40000;
    } while ((sint32)row < height);
    return 0;
}

uint32 t3_bns_02_800b4f78(void)
{
    uint32 actor;
    uint32 value;
    FUNCTION_MARKER(0x800B4F78, "SLUS_004.02-BNS-logical-02");
    w_u32(0x800A85C8, r_u32(0x800AFAAC) == 3 ? 130 : 0);
    w_u32(0x800B6BE0, 1);
    w_u32(0x800B6BBC, 0xFFFFF254);
    w_u32(0x800B6BC0, 0xFFFFE4A8);
    w_u32(0x800B6BD0, 0);
    w_u32(0x800B6BB8, 0);
    w_u32(0x800B6BC4, 0x2AAA);
    w_u32(0x800B6BC8, 0);
    w_u32(0x800B6BCC, 500);
    w_u32(0x800B6BD8, 0x2AAA);
    w_u32(0x800B6BD4, 0);
    actor = t3_bns_02_800b297c();
    w_u32(0x800B6BB4, actor);
    value = r_u32(0x800B6BB8);
    w_u32(0x800B6C00, value);
    return 0x800B0000;
}

void t3_bns_02_800b5020(void)
{
    uint32 pitch;
    uint32 yaw;
    uint32 actor;
    uint32 previous_x;
    uint32 actor_x;
    uint32 delta_x;
    uint32 cosine;
    uint32 sine;
    uint32 base_y;
    uint32 negative_y;
    uint32 base_z;
    uint32 horizontal;
    uint32 vertical;
    uint32 delta_z;
    uint32 pitch_sine;
    uint32 pitch_cosine;
    uint32 depth;
    uint32 elevation;
    uint32 quotient;
    uint32 mode;
    uint32 displacement;
    sint32 scale;
    uint32 limit;
    uint32 current;
    uint32 target;
    uint32 x, y, z, zoom, roll, distance;
    FUNCTION_MARKER(0x800B5020, "SLUS_004.02-BNS-logical-02");
    pitch = 0 - r_u32(0x800B6BC4);
    if ((sint32)pitch < 0)
        pitch += 63;
    yaw = r_u32(0x800A85C8);
    actor = r_u32(0x800B6BB4);
    previous_x = r_u32(0x800B6BB8);
    yaw = ((0 - yaw) & 0xFFF) << 1;
    actor_x = r_u32(actor + 0xF68);
    cosine = (uint32)r_s16(0x8001F0C4 + yaw);
    delta_x = actor_x - previous_x;
    base_y = r_u32(0x800B6BBC);
    sine = (uint32)r_s16(0x8001E8C4 + yaw);
    negative_y = 0 - base_y;
    base_z = r_u32(0x800B6BC0);
    horizontal = cosine * delta_x - sine * negative_y;
    pitch = (uint32)((sint32)pitch >> 5) & 0x1FFE;
    delta_z = r_u32(actor + 0xF70);
    pitch_sine = (uint32)r_s16(0x8001E8C4 + pitch);
    pitch_cosine = (uint32)r_s16(0x8001F0C4 + pitch);
    delta_z -= base_z;
    horizontal = (uint32)((sint32)horizontal / 4096);
    vertical = sine * delta_x + cosine * negative_y;
    vertical = (uint32)((sint32)vertical / 4096);
    depth = pitch_cosine * delta_z - pitch_sine * vertical;
    elevation = pitch_sine * delta_z + pitch_cosine * vertical;
    depth = (uint32)((sint32)depth / 4096);
    elevation = (uint32)((sint32)elevation / 4096);
    quotient = t3_camera_divide(elevation * 2500, depth);
    mode = r_u32(0x800B6BD0);
    scale = (sint32)quotient / 3;
    displacement = t3_camera_divide(horizontal * 500, depth);
    w_u32(0x800B6BD4, previous_x);
    if (mode == 0 || mode == 2) {
        if (mode == 2) {
            limit = r_u32(0x800B6BDC);
            if ((sint32)displacement < (sint32)limit)
                w_u32(0x800B6BD4, previous_x - (limit - displacement) * 5);
        }
        limit = r_u32(0x800B6BE0);
        if ((sint32)limit < (sint32)displacement) {
            current = r_u32(0x800B6BB8);
            w_u32(0x800B6BD4, current + (displacement - limit) * 5);
        }
    }
    if ((sint32)displacement > 0) {
        current = r_u32(0x800B6BB8);
        target = r_u32(0x800B6BD4);
        if ((sint32)current < (sint32)target)
            w_u32(0x800B6BB8, target);
    } else if ((sint32)displacement < 0) {
        current = r_u32(0x800B6BB8);
        target = r_u32(0x800B6BD4);
        if ((sint32)target < (sint32)current)
            w_u32(0x800B6BB8, target);
    }
    if (r_u32(0x800B6BD0) == 2) {
        current = r_u32(0x800B6BB8);
        limit = r_u32(0x800B6BE4);
        if ((sint32)current < (sint32)limit)
            w_u32(0x800B6BB8, limit);
        current = r_u32(0x800B6BB8);
        limit = r_u32(0x800B6BE8);
        if ((sint32)limit < (sint32)current)
            w_u32(0x800B6BB8, limit);
    }
    if (scale >= 213) {
        current = r_u32(0x800B6BC4);
        w_u32(0x800B6BD8, current + 728);
    } else if (scale < 188) {
        current = r_u32(0x800B6BC4);
        w_u32(0x800B6BD8, current - 728);
    }
    current = r_u32(0x800B6BC4);
    target = r_u32(0x800B6BD8);
    if (current != target) {
        uint32 difference = current - target;
        uint32 step = (uint32)((sint32)difference / 8);
        w_u32(0x800B6BC4, current - step);
    }
    current = r_u32(0x800B6BC4);
    if ((sint32)current < 0x2AAA)
        w_u32(0x800B6BC4, 0x2AAA);
    else if ((sint32)current > 0x10000)
        w_u32(0x800B6BC4, 0x10000);
    x = r_u32(0x800B6BB8);
    y = r_u32(0x800B6BBC);
    z = r_u32(0x800B6BC0);
    zoom = r_u32(0x800B6BC4);
    roll = r_u32(0x800B6BC8);
    distance = r_u32(0x800B6BCC);
    t3_80064020(x, y, z, zoom, roll, distance, 0);
    t3_80064080(0);
}

uint32 t3_bns_02_800b53bc(uint32 first, uint32 second, uint32 third)
{
    uint32 value;
    FUNCTION_MARKER(0x800B53BC, "SLUS_004.02-BNS-logical-02");
    value = t3_80045bb8(first);
    w_u8(first + 31, value);
    value = t3_80045bb8(second);
    w_u8(second + 31, value);
    value = t3_80045bb8(third);
    w_u8(third + 31, value);
    return value;
}

uint32 t3_bns_02_800b5418(void)
{
    FUNCTION_MARKER(0x800B5418, "SLUS_004.02-BNS-logical-02");
    w_u32(0x800B6534, 0);
    w_u32(0x800B6530, 0);
    return 0x800B0000;
}

uint32 t3_bns_02_800b542c(uint32 side)
{
    uint32 buttons;
    uint32 value;
    FUNCTION_MARKER(0x800B542C, "SLUS_004.02-BNS-logical-02");
    buttons = r_u16(0x800ADD68 + (side << 1));
    if ((buttons & 0xF0) != 0xF0)
        return buttons & 4;
    if (buttons & 4)
        value = 4;
    else if (buttons & 1)
        value = 1;
    else if (buttons & 8)
        value = 8;
    else if (buttons & 2)
        value = 2;
    else
        value = 1;
    w_u32(0x800B6534, value);
    w_u32(0x800B6530, 1);
    return 1;
}

uint32 t3_bns_02_800b54a8(uint32 actor, uint32 first, uint32 second)
{
    uint32 selected = first;
    uint32 cooldown;
    uint32 old_id;
    uint32 actor_id;
    uint32 other_id;
    uint32 first_distance;
    uint32 second_distance;
    uint32 difference;
    uint32 magnitude;
    FUNCTION_MARKER(0x800B54A8, "SLUS_004.02-BNS-logical-02");
    cooldown = r_u32(0x800B653C);
    if ((sint32)cooldown > 0)
        w_u32(0x800B653C, cooldown - 1);
    cooldown = r_u32(0x800B653C);
    if ((sint32)cooldown < 0)
        w_u32(0x800B653C, 0);
    if (r_u8(first + 0xC2)) {
        if (!r_u8(second + 0xC2))
            selected = second;
        goto finish;
    }
    if (r_u8(second + 0xC2))
        goto finish;
    if (!r_u32(0x800B6528))
        goto secondary_check;
    if (!r_u32(0x800B652C))
        goto availability_recheck;
    if (r_u32(0x800B653C))
        goto availability_recheck;
    old_id = r_u8(first + 0x1E);
    actor_id = r_u8(actor + 0x1E);
    first_distance = t3_800439f0(actor_id, old_id);
    actor_id = r_u8(actor + 0x1E);
    other_id = r_u8(second + 0x1E);
    second_distance = t3_800439f0(actor_id, other_id);
    difference = first_distance - second_distance;
    if ((sint32)first_distance < 2500 || (sint32)second_distance < 2500) {
        magnitude = difference;
        if ((sint32)magnitude < 0)
            magnitude = 0 - magnitude;
        if ((sint32)magnitude >= 400) {
            selected = (sint32)difference > 0 ? second : first;
        } else {
            uint16 saved[12];
            uint16 first_state[12];
            uint16 second_state[12];
            uint32 index;
            sint32 first_category;
            sint32 second_category;
            for (index = 0; index < 12; ++index)
                saved[index] = r_u16(actor + 0x2A + (index % 6) * 2);
            t3_80043a94(actor, first);
            for (index = 0; index < 12; ++index)
                first_state[index] = r_u16(actor + 0x2A + (index % 6) * 2);
            t3_80043a94(actor, second);
            for (index = 0; index < 12; ++index)
                second_state[index] = r_u16(actor + 0x2A + (index % 6) * 2);
            first_category = (sint16)first_state[4];
            second_category = (sint16)second_state[4];
            if (first_category == second_category) {
                sint32 threshold = first_category == 0 ? 0x1000 :
                    (first_category == 2 ? 0x2000 : 0x3000);
                sint32 first_angle = (sint16)first_state[3];
                sint32 second_angle = (sint16)second_state[3];
                sint32 angle_difference = first_angle - second_angle;
                if (angle_difference < 0)
                    angle_difference = -angle_difference;
                if (angle_difference >= threshold)
                    selected = first_angle < second_angle ? first : second;
            } else {
                uint32 priority[4];
                uint32 first_priority;
                uint32 second_priority;
                priority[0] = r_u32(0x800B0A38);
                priority[1] = r_u32(0x800B0A3C);
                priority[2] = r_u32(0x800B0A40);
                priority[3] = r_u32(0x800B0A44);
                first_priority = priority[first_category];
                second_priority = priority[second_category];
                selected = (sint32)second_priority < (sint32)first_priority ? first : second;
            }
            for (index = 0; index < 6; ++index)
                w_u16(actor + 0x2A + index * 2, saved[index]);
            for (index = 0; index < 6; ++index)
                w_u16(actor + 0x38 + index * 2, saved[index]);
        }
    }
    if (r_u32(0x800B6530)) {
        uint32 buttons;
        uint32 mask;
        uint32 toggle;
        if (!r_u16(0x800A9128))
            buttons = r_u16(0x800ADF08);
        else
            buttons = r_u16(0x800ADF0A);
        mask = r_u32(0x800B6534);
        if ((uint32)(sint32)(sint16)buttons & mask) {
            toggle = r_u32(0x800B6538);
            w_u32(0x800B6538, (toggle + 1) & 1);
        }
        toggle = r_u32(0x800B6538);
        selected = toggle ? second : first;
    }
    if (old_id != r_u8(selected + 0x1E))
        w_u32(0x800B653C, 120);
    goto finish;
availability_recheck:
    if (r_u32(0x800B6528)) {
        selected = first;
        goto finish;
    }
secondary_check:
    if (r_u32(0x800B652C))
        selected = second;
finish:
    if (r_u32(0x800B6530))
        w_u32(0x800B6538, selected != first);
    return selected;
}

uint32 t3_bns_02_800b5e74(void)
{
    FUNCTION_MARKER(0x800B5E74, "SLUS_004.02-BNS-logical-02");
    return (r_u32(0x80097EF0) >> 19) & 1;
}

uint32 t3_bns_02_800b5e8c(void)
{
    FUNCTION_MARKER(0x800B5E8C, "SLUS_004.02-BNS-logical-02");
    if ((r_u32(0x80097EF0) >> 19) & 1)
        return 1;
    t3_800561b0(19);
    return 0;
}

uint32 t3_bns_02_800b5ecc(void)
{
    FUNCTION_MARKER(0x800B5ECC, "SLUS_004.02-BNS-logical-02");
    return r_u8(0x80097F18);
}

uint32 t3_bns_02_800b5edc(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800B5EDC, "SLUS_004.02-BNS-logical-02");
    value = r_u8(0x80097F18) + 1;
    if (value >= 256)
        value = 255;
    w_u8(0x80097F18, value);
    return t3_8004c390();
}

uint32 t3_bns_02_800b5f20(void)
{
    FUNCTION_MARKER(0x800B5F20, "SLUS_004.02-BNS-logical-02");
    return r_u32(0x80097F14);
}

uint32 t3_bns_02_800b5f30(uint32 value)
{
    FUNCTION_MARKER(0x800B5F30, "SLUS_004.02-BNS-logical-02");
    w_u32(0x80097F14, value);
    return t3_8004c390();
}

uint32 t3_bns_02_800b59dc(uint32 *output_primary, uint32 *output_selected, uint32 *output_other)
{
    uint32 primary;
    uint32 first;
    uint32 second;
    uint32 selected;
    uint32 source;
    uint32 destination;
    uint32 value;
    FUNCTION_MARKER(0x800B59DC, "SLUS_004.02-BNS-logical-02");
    if (r_u8(0x800A92ED) == 0) {
        primary = 0x800A9228;
        first = 0x800AAAB4;
        second = 0x800AC340;
    } else {
        primary = 0x800AAAB4;
        first = 0x800A9228;
        second = 0x800AC340;
    }
    if (r_u32(0x800AFA88) == 8) {
        if (r_u8(primary + 0x1E) == 0) {
            if (r_u8(primary + 0x20) == 1) {
                first = 0x800AAAB4;
                second = 0x800AC340;
            } else {
                first = 0x800AC340;
                second = 0x800AAAB4;
            }
        } else {
            if (r_u8(primary + 0x20) == 0) {
                first = 0x800A9228;
                second = 0x800AAAB4;
            } else {
                first = 0x800AAAB4;
                second = 0x800A9228;
            }
        }
    }
    w_u32(0x800B6528, (r_u16(first + 0x1278) - (uint32)1) < 399);
    w_u32(0x800B652C, (r_u16(second + 0x1278) - (uint32)1) < 399);
    *output_primary = primary;
    selected = t3_bns_02_800b54a8(primary, first, second);
    *output_selected = selected;
    *output_other = selected == first ? second : first;
    value = 0;
    if (r_u8(first + 0xC2) && r_u8(second + 0xC2))
        value = 1;
    w_u8(primary + 0xC7, value);
    w_u8(primary + 0xC8, 0);
    source = *output_selected;
    destination = *output_primary;
    value = r_u8(source + 0x1E);
    w_u8(destination + 0x20, value);
    source = *output_primary;
    destination = *output_selected;
    value = r_u8(source + 0x1E);
    w_u8(destination + 0x20, value);
    source = *output_primary;
    destination = *output_other;
    value = r_u8(source + 0x1E);
    w_u8(destination + 0x20, value);
    first = *output_primary;
    second = *output_selected;
    primary = *output_other;
    value = t3_80045bb8(first);
    w_u8(first + 0x1F, value);
    value = t3_80045bb8(second);
    w_u8(second + 0x1F, value);
    value = t3_80045bb8(primary);
    w_u8(primary + 0x1F, value);
    return value;
}

uint32 t3_bns_02_800b5bc0(uint32 control)
{
    sint32 index;
    FUNCTION_MARKER(0x800B5BC0, "SLUS_004.02-BNS-logical-02");
    t3_bns_02_800b1024();
    w_u32(control + 0x38, 0);
    w_u8(control + 0x3F, 0);
    w_u16(control + 0x3C, 0);
    w_u8(control + 0x40, 0);
    if (t3_bns_02_800b5e74())
        w_u8(control + 0x40, r_u8(control + 0x40) | 0x10);
    for (index = 3; index >= 0; --index)
        w_u8(control + 0x41 + (uint32)index, 0x58);
    return 0xFFFFFFFF;
}

uint32 t3_bns_02_800b5c2c(uint32 control)
{
    uint32 state;
    uint32 effect;
    uint32 offset;
    uint32 value;
    FUNCTION_MARKER(0x800B5C2C, "SLUS_004.02-BNS-logical-02");
    state = r_u32(control + 0x24);
    switch (state) {
    case 1: effect = 3; break;
    case 2: effect = 4; break;
    case 3:
    case 4: effect = 5; break;
    default: effect = 0; break;
    }
    w_u8(0x800AE211, effect);
    offset = r_u8(control + 0x1F) << 1;
    w_u16(0x800ADD5C + offset, 21);
    w_u16(0x800ADD98 + offset, 0);
    w_u16(0x800A9128 + offset, 1);
    value = r_u8(control + 0x1E);
    w_u32(0x800B6BF4, value);
    value = r_u8(control + 0x1E);
    w_u8(control + 0x3E, value);
    return value;
}

uint32 t3_bns_02_800b5cf0(uint32 control)
{
    uint32 actor;
    uint32 round;
    uint32 saved;
    uint32 value;
    uint32 first_id;
    uint32 current_id;
    FUNCTION_MARKER(0x800B5CF0, "SLUS_004.02-BNS-logical-02");
    actor = t3_bns_02_800b297c();
    if (r_u32(0x800B6C04)) {
        first_id = r_u8(control + 0x22);
        current_id = r_u8(control + 0x1E);
        if (first_id == current_id) {
            round = r_u32(control + 0x24) + 1;
            w_u32(control + 0x24, round);
            if (round < 4) {
                w_u16(0x800AE224, 2);
                return 2;
            }
            if (round == 4) {
                if (r_u32(0x800B6C14)) {
                    w_u16(0x800AE224, 2);
                    return 2;
                }
                t3_800513b4((uint32)r_s16(actor + 0x18));
                saved = r_u32(0x800B6C1C);
                w_u16(0x800AE1FA, 0);
                w_u16(0x800AE1F8, 0);
                w_u16(0x800ADFBE, 0);
                w_u16(0x800ADFBC, 0);
                w_u16(0x800AE200, 0);
            } else {
                t3_800513b4((uint32)r_s16(actor + 0x18));
                w_u16(0x800AE1FA, 0);
                w_u16(0x800AE1F8, 0);
                w_u16(0x800ADFBE, 0);
                w_u16(0x800ADFBC, 0);
                w_u16(0x800AE200, 0);
                t3_bns_02_800b5e8c();
                saved = r_u32(0x800B6C1C);
            }
            t3_bns_02_800b5f30(saved);
            value = r_u32(0x800B6C08);
            w_u32(control + 0x38, value);
            return t3_8004ff20(15);
        }
    }
    w_u16(0x800AE1FA, 0);
    w_u16(0x800AE1F8, 0);
    w_u16(0x800ADFBE, 0);
    w_u16(0x800ADFBC, 0);
    t3_800513b4((uint32)r_s16(actor + 0x18));
    saved = r_u32(0x800B6C1C);
    t3_bns_02_800b5f30(saved);
    w_u16(0x800AE224, 11);
    return 11;
}

uint32 t3_bns_02_800b1024(void)
{
    uint32 value;
    uint32 index;
    sint32 count;
    uint32 resource;
    T3_TIM_IMAGE image;
    FUNCTION_MARKER(0x800B1024, "SLUS_004.02-BNS-logical-02");
    value = t3_bns_02_800b5ecc();
    w_u32(0x800B6BEC, value);
    w_u32(0x800B6C08, 0);
    value = t3_bns_02_800b5f20();
    w_u32(0x800B6C1C, value);
    t3_80029638(1);
    count = r_s32(0x800B0AFC);
    index = 0;
    if (count > 0) {
        do {
            resource = t3_80077590(0x800B0AFC, (sint32)index);
            if (t3_8007efc8(resource) == 0xFFFFFFFF)
                break;
            t3_8007efd8(&image);
            ++index;
            t3_8007c650(image.image_rectangle, image.image_pixels);
            DrawSync(0);
            t3_8007c650(image.clut_rectangle, image.clut_pixels);
            DrawSync(0);
        } while ((sint32)index < count);
    }
    return t3_80029638(0);
}

uint32 t3_bns_02_800b6030(uint32 stage)
{
    uint32 position;
    uint32 scale;
    uint32 color;
    uint32 strings;
    uint32 slot;
    uint32 value;
    uint32 text;
    uint32 index;
    uint16 x;
    uint16 y;
    FUNCTION_MARKER(0x800B6030, "SLUS_004.02-BNS-logical-02");
    position = r_u32(0x800B6550);
    scale = r_u32(0x800B6554);
    x = (uint16)position;
    y = (uint16)(position >> 16);
    color = 0x800B6580 + stage;
    strings = 0x800B6558 + (stage << 3);
    for (index = 0; index < 2; ++index) {
        slot = t3_8005197c();
        if (!slot)
            return 8;
        w_u16(slot, 8);
        w_u32(slot + 0x18, 0x800B0AB0);
        value = r_u8(color);
        w_u16(slot + 0x12, 3);
        w_u16(slot + 0x10, value);
        w_u16(slot + 4, x);
        w_u16(slot + 6, y);
        w_u16(slot + 8, (uint16)scale);
        w_u16(slot + 10, (uint16)(scale >> 16));
        text = r_u32(strings);
        w_u32(slot + 0x1C, text);
        value = t3_8007a418(text);
        value = r_u16(slot + 4) - value * 21;
        w_u16(slot + 4, value);
        y = (uint16)(y + 46);
        strings += 4;
    }
    return 0;
}

uint32 t3_bns_02_800b1294(void)
{
    uint32 primary;
    uint32 elapsed;
    uint32 result;
    uint32 value;
    uint32 other;
    uint32 stage;
    uint32 phase;
    uint32 record;
    uint32 script;
    uint32 actor;
    uint32 character;
    uint32 costume;
    uint32 model;
    uint32 packets;
    uint32 stop;
    uint32 index;
    uint32 timer;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 projected;
    uint32 spent;
    uint32 remaining;
    uint32 minutes;
    uint32 seconds;
    uint32 hundredths;
    uint32 spent_hundredths;
    uint32 color;
    uint32 screen_y;
    FUNCTION_MARKER(0x800B1294, "SLUS_004.02-BNS-logical-02");
    primary = t3_bns_02_800b297c();
    value = r_u32(0x800A85D4);
    other = r_u32(0x800B6C00);
    phase = r_u32(0x80095494);
    result = 0;
    elapsed = value - other;
    if (!phase)
        w_u32(0x800B6BF0, r_u32(0x800B6BF0) + 1);
    if (!r_u32(0x800ADCA4))
        w_u32(0x800B6600, r_u32(0x800B6600) + 1);
    phase = r_u32(0x800B6BF8);
    switch (phase) {
    case 0:
        if (r_u32(0x80096F2C) != 1)
            break;
        script = r_u32(0x800B65D8);
        if ((sint32)elapsed < r_s32(script))
            break;
        stop = 0;
        for (;;) {
            if (stop)
                break;
            script = r_u32(0x800B65D8);
            switch (r_s16(script + 4)) {
            case 0:
                script = r_u32(0x800B65D8);
                value = (uint32)r_s16(script + 14);
                w_u32(0x800B658C, value);
                t3_bns_02_800b4efc(value);
                break;
            case 1:
                record = t3_bns_02_800b29cc();
                if (record) {
                    script = r_u32(0x800B65D8);
                    value = (uint32)r_s16(record + 16);
                    other = (uint32)r_s16(script + 14);
                    actor = r_u32(record);
                    if (value == other) {
                        w_u16(record + 8, 4);
                    } else {
                        w_u16(record + 8, 2);
                        w_u16(record + 18, r_u16(script + 14));
                    }
                    script = r_u32(0x800B65D8);
                    value = (uint32)((sint32)r_s16(script + 16) % 5);
                    w_u16(record + 14, value);
                    value = (uint32)r_s16(script + 6);
                    other = r_u32(0x800A85D4);
                    z = (uint32)r_s16(script + 10);
                    t3_bns_02_800b2a6c(actor, other + value, z);
                }
                break;
            case 2:
                value = r_u32(0x800B65E4);
                script = r_u32(0x800B65D8);
                w_u32(0x800B65E4, value + 1);
                value = r_u16(script + 16);
                other = r_u16(script + 14);
                w_u32(0x800B65E8, value);
                if (other < 49)
                    w_u32(0x800B65EC, 0x800B0548 + other * 6);
                break;
            case 3:
                stop = 1;
                script = r_u32(0x800B65D8);
                w_u32(0x800B65DC, 0);
                w_u32(0x800B6BF8, stop);
                value = (uint32)r_s16(script + 14);
                w_u32(0x800B65E0, value);
                break;
            case 4:
                script = r_u32(0x800B65D8);
                value = r_u32(0x800A85D4);
                other = (uint32)r_s16(script + 6);
                z = (uint32)r_s16(script + 10);
                t3_bns_02_800b3e44(value + other, z);
                break;
            case 6:
                record = 0x800B6598;
                for (index = 0; index < 2; ++index, record += 24)
                    t3_bns_02_800b2a4c(record);
                t3_8006bc98(0, 30);
                stop = 1;
                w_u8(0x800AFA8C, 0);
                w_u32(0x800B65E4, 0);
                w_u32(0x800B6BF8, 3);
                value = r_u32(0x800B6BF0) + 60;
                w_u32(0x800B6588, value);
                break;
            default:
                break;
            }
            script = r_u32(0x800B65D8);
            value = r_u32(script + 20);
            w_u32(0x800B65D8, script + 20);
            if ((sint32)elapsed < (sint32)value)
                break;
        }
        break;
    case 1:
        value = r_u32(0x800B65E0);
        if ((sint32)value < 0) {
            if (r_s16(0x800B65A0) == 1 && r_s16(0x800B65B8) == 1)
                w_u32(0x800B6BF8, 0);
        } else if (r_s32(0x800B65DC) >= (sint32)value) {
            w_u32(0x800B6BF8, 0);
        }
        break;
    case 3:
        value = r_u32(0x800B6BF0);
        other = r_u32(0x800B6588);
        if ((sint32)other < (sint32)value && !r_u32(0x80095494)) {
            w_u32(0x800B6588, value + 2);
            w_u32(0x800B6BF8, 4);
        }
        break;
    case 4:
        t3_8004ce74(0x800B088C, (uint32)0, (uint32)6, (uint32)242, (uint32)422);
        value = r_u32(0x800B6BF0);
        other = r_u32(0x800B6588);
        if ((sint32)other < (sint32)value && !r_u32(0x80095494)) {
            result = 1;
            w_u32(0x800B6BF8, 5);
        }
        break;
    case 5:
        w_u32(0x800B6C04, 1);
        w_u8(0x800AFA8C, 1);
        w_u32(0x800B6BF8, 6);
        value = r_u32(0x800ADBCC);
        w_u32(0x800B65F0, value);
        break;
    case 6:
        actor = t3_bns_02_800b298c(0);
        value = r_u32(0x800B65F0);
        other = r_u32(0x800ADBCC);
        spent = value - other;
        stage = r_u32(0x800B6C20);
        remaining = r_u32(0x800B06F4 + (stage << 2)) - spent;
        w_u32(0x800B65F4, spent);
        w_u32(0x800B65F8, remaining);
        if ((sint32)spent < 0)
            w_u32(0x800B65F4, 0);
        if ((sint32)remaining < 0)
            w_u32(0x800B65F8, 0);
        if (r_s32(0x800B65F8) > 0 && r_s32(actor + 0x3F4) > 0) {
            SVECTOR origin = {0, 0, 0, 0};
            x = r_u32(actor + 0x990);
            y = r_u32(actor + 0x994) - 500;
            z = r_u32(actor + 0x998);
            t3_8004adc4(x, y, z);
            t3_80036ad4(&origin, &projected);
            value = r_u32(0x800B65F8);
            seconds = (uint32)(((sint32)value / 60) % 60);
            other = value * 100;
            hundredths = (uint32)(((sint32)other / 60) % 100);
            t3_8004ce74(0x800B08A4, (uint32)0, (uint32)5,
                (projected & 0xFFFF) - 22, (uint32)((sint32)projected >> 16), seconds, hundredths);
        }
        if (r_u32(0x80096F2C) == 8) {
            value = r_u32(0x800A961C);
            value = (uint32)((sint32)value / 65536) * 100;
            w_u32(0x800B65FC, value);
            if (r_u32(0x800B6C20) == 3) {
                if (r_s32(0x800B6BEC) < 3 && !t3_bns_02_800b5e74()) {
                    w_u32(0x800B6C18, 1);
                    t3_bns_02_800b5edc();
                    value = r_u32(0x800B6BEC);
                    if (value == 1)
                        w_u8(0x800AFAC8, r_u8(0x800AFAC8) | 3);
                    else if ((sint32)value < 2) {
                        if (!value)
                            w_u8(0x800AFAC8, r_u8(0x800AFAC8) | 1);
                    } else if (value == 2) {
                        w_u8(0x800AFAC8, r_u8(0x800AFAC8) | 7);
                    }
                } else if (r_u32(0x800B6BEC) == 3 && !t3_bns_02_800b5e74()) {
                    w_u32(0x800B6C14, 1);
                }
            }
            w_u32(0x800B6588, 0);
            w_u32(0x800B6BF8, 7);
        }
        break;
    case 7:
        value = r_u32(0x800B6588) + 1;
        w_u32(0x800B6588, value);
        screen_y = 180;
        if ((sint32)value >= 30) {
            if (value == 30) {
                stage = r_u32(0x800B6C20);
                value = r_u32(0x800B6C08);
                other = r_u32(0x800B0708 + (stage << 2));
                w_u32(0x800B6C08, value + other);
                t3_bns_02_800b4538(0);
            }
            stage = r_u32(0x800B6C20);
            value = r_u32(0x800B0708 + (stage << 2));
            t3_8004ce74(0x800B08B8, (uint32)1, (uint32)4, (uint32)47, screen_y, value);
        }
        value = r_u32(0x800B6588);
        screen_y += 52;
        if ((sint32)value >= 60) {
            if (value == 60) {
                value = r_u32(0x800B6C08);
                other = r_u32(0x800B65FC);
                w_u32(0x800B6C08, value + other);
                t3_bns_02_800b4538(0);
            }
            value = r_u32(0x800B65FC);
            t3_8004ce74(0x800B08D4, (uint32)1, (uint32)1, (uint32)47, screen_y, value);
        }
        value = r_u32(0x800B6588);
        screen_y += 52;
        if ((sint32)value >= 90) {
            if (value == 90)
                t3_bns_02_800b4538(0);
            value = r_u32(0x800B65F4);
            minutes = (uint32)((sint32)value / 3600);
            seconds = (uint32)((sint32)value / 60) - minutes * 60;
            minutes = (uint32)((sint32)minutes % 60);
            other = value * 100;
            spent_hundredths = (uint32)(((sint32)other / 60) % 100);
            t3_8004ce74(0x800B08F0, (uint32)1, (uint32)5, (uint32)47,
                screen_y, minutes, seconds, spent_hundredths);
        }
        value = r_u32(0x800B6588);
        screen_y += 52;
        if ((sint32)value >= 120) {
            stage = r_u32(0x800B6C20);
            w_u32(0x800B6BFC, 1);
            if ((sint32)stage < 3 && (value = r_u32(0x800B65F8)) != 0) {
                minutes = (uint32)((sint32)value / 3600);
                seconds = (uint32)((sint32)value / 60) - minutes * 60;
                minutes = (uint32)((sint32)minutes % 60);
                other = value * 100;
                hundredths = (uint32)(((sint32)other / 60) % 100);
                if ((sint32)(spent_hundredths + hundredths) < 100)
                    hundredths = 100 - spent_hundredths;
                t3_8004ce74(0x800B0914, (uint32)1, (uint32)6, (uint32)47,
                    screen_y, minutes, seconds, hundredths);
                if (r_u32(0x800B6588) == 120) {
                    value = r_u32(0x800ADBCC);
                    other = r_u32(0x800B65F8);
                    w_u32(0x800ADBCC, value + other);
                    t3_bns_02_800b4538(0);
                }
            } else if (r_u32(0x800B6C18)) {
                color = (r_u32(0x800B6588) & 2) ? 6 : 2;
                t3_8004ce74(0x800B0938, (uint32)1, color, (uint32)86, screen_y);
                if (r_u32(0x800B6588) == 120) {
                    w_u32(0x800B6BEC, r_u32(0x800B6BEC) + 1);
                    t3_bns_02_800b4538(1);
                }
            } else if (r_u32(0x800B6C14)) {
                color = (r_u32(0x800B6588) & 4) ? 6 : 2;
                t3_8004ce74(0x800B0950, (uint32)1, color, (uint32)34, screen_y);
                if (r_u32(0x800B6588) == 120)
                    t3_bns_02_800b4538(1);
            }
        }
        break;
    case 8:
        record = t3_bns_02_800b29ac(0);
        actor = r_u32(record);
        w_u16(record + 14, 4);
        w_u16(record + 8, 1);
        w_u16(record + 10, 0);
        t3_8002d6e8(actor, 3);
        w_u8(actor + 0xC3, 1);
        stage = r_u32(0x800B6C20);
        value = (uint32)r_s16(record + 14);
        value = r_u32(0x800B0684 + stage * 20 + (value << 2));
        w_u8(actor + 0xC2, 0);
        w_u32(actor + 4, 0);
        w_u16(actor + 0x92, 0);
        w_u8(actor + 0xB8, 0);
        w_u32(actor + 0x3F8, value);
        w_u32(actor + 0x3F4, value);
        t3_bns_02_800b3148(1, 19, 0);
        w_u32(0x800B658C, 2);
        t3_bns_02_800b4efc(2);
        w_u32(0x800B6C04, 1);
        w_u32(0x800B6BF8, 9);
        value = r_u32(0x800ADBCC);
        w_u32(0x800B65F0, value);
        break;
    case 9:
        value = r_u32(0x800B65F0);
        other = r_u32(0x800ADBCC);
        value -= other;
        w_u32(0x800B65F4, value);
        if ((sint32)value < 0)
            w_u32(0x800B65F4, 0);
        if (r_u32(0x80096F2C) == 8) {
            w_u32(0x800B6588, 0);
            value = r_u8(0x800AFAC8);
            w_u32(0x800B6BF8, 10);
            w_u8(0x800AFAC8, value | 8);
        }
        break;
    case 10:
        value = r_u32(0x800B6588);
        w_u32(0x800B6588, value + 1);
        if ((sint32)value >= 181)
            w_u32(0x800B6BFC, 1);
        break;
    default:
        break;
    }
    value = r_u32(0x800B65E4);
    if (value && value == 1) {
        if ((sint32)elapsed < r_s32(0x800B65E8)) {
            script = r_u32(0x800B65EC);
            if (r_u16(script) != 99) {
                record = t3_bns_02_800b29cc();
                if (record) {
                    script = r_u32(0x800B65EC);
                    value = (uint32)r_s16(record + 16);
                    other = r_u16(script);
                    actor = r_u32(record);
                    if (value == other) {
                        w_u16(record + 8, 4);
                    } else {
                        w_u16(record + 8, 2);
                        w_u16(record + 18, r_u16(script));
                    }
                    script = r_u32(0x800B65EC);
                    value = (uint32)((sint32)r_s16(script + 2) % 5);
                    w_u16(record + 14, value);
                    value = r_u32(0x800A85D4);
                    t3_bns_02_800b2a6c(actor, value + 5000, 0);
                    w_u32(0x800B65EC, r_u32(0x800B65EC) + 6);
                }
            }
        } else {
            w_u32(0x800B65E4, 0);
        }
    }
    for (index = 0, record = 0x800B6598; index < 2; ++index, record += 24) {
        phase = (uint32)r_s16(record + 8);
        actor = r_u32(record);
        switch (phase) {
        case 0:
            w_u8(actor + 0xC3, 0);
            w_u32(actor + 0x3F4, 0);
            w_u8(actor + 0xC2, 1);
            w_u16(record + 8, 1);
            break;
        case 2:
            w_u8(actor + 0xC3, 0);
            value = r_u16(0x800B6600);
            w_u16(record + 8, 3);
            w_u16(record + 12, value + 2);
            break;
        case 3:
            value = (uint32)r_s16(record + 12);
            other = r_u32(0x800B6600);
            if ((sint32)value < (sint32)other && !r_u32(0x800ADCA4)) {
                value = r_u16(record + 18);
                stage = r_u32(0x800B6C20);
                w_u16(record + 16, value);
                costume = t3_bns_02_800b2a80(stage, (uint32)(sint32)(sint16)value);
                w_u16(actor + 20, costume + 84);
                t3_800361a8(actor);
                character = r_u8(actor + 0x1E);
                value = (uint32)r_s16(record + 16) + 1;
                t3_80035e5c(character, value);
                model = t3_80035ed0(character);
                packets = t3_80035eec(character);
                t3_80035ce8(actor, model, packets);
                t3_bns_02_800b3148(character, 21, costume);
                w_u16(record + 8, 4);
            }
            break;
        case 4:
            w_u8(actor + 0xC3, 1);
            stage = r_u32(0x800B6C20);
            value = (uint32)r_s16(record + 14);
            value = r_u32(0x800B0684 + stage * 20 + (value << 2));
            w_u8(actor + 0xC2, 0);
            w_u32(actor + 0x3F8, value);
            w_u32(actor + 0x3F4, value);
            w_u16(record + 8, 5);
            t3_8002d6e8(actor, 3);
            w_u32(actor + 4, 0);
            w_u16(actor + 0x92, 0);
            w_u8(actor + 0xB8, 0);
            break;
        case 5:
            if (r_s32(actor + 0x3F4) <= 0) {
                value = r_u16(0x800B6BF0);
                w_u16(record + 8, 6);
                w_u16(record + 12, value + 60);
                value = r_u8(actor + 0x22);
                other = r_u8(primary + 0x1E);
                if (value == other || r_u8(actor + 0xE7)) {
                    SVECTOR origin = {0, 0, 0, 0};
                    timer = r_u32(0x800ADBCC);
                    x = r_u32(actor + 0x990);
                    z = r_u32(actor + 0x998);
                    y = r_u32(actor + 0x994) - 300;
                    value = (uint32)r_s16(record + 14);
                    other = r_u16(0x800B0672 + (value << 2));
                    value = timer - (uint32)((sint32)timer % 60) + 59 + other * 60;
                    w_u32(0x800ADBCC, value);
                    t3_8004adc4(x, y, z);
                    t3_80036ad4(&origin, &projected);
                    value = r_u16(0x800B6BF0);
                    w_u16(record + 4, (projected & 0xFFFF) - 27);
                    w_u16(record + 6, ((uint32)((sint32)projected >> 16)) << 4);
                    w_u16(record + 20, value + 60);
                    w_u16(0x800AFAC4, r_u16(0x800AFAC4) + 1);
                }
            }
            break;
        case 6:
            value = (uint32)r_s16(record + 20);
            other = r_u32(0x800B6BF0);
            if ((sint32)other < (sint32)value) {
                x = (uint32)r_s16(record + 4);
                y = (uint32)((sint32)((uint32)r_u16(record + 6) << 16) >> 20);
                value = (uint32)r_s16(record + 14);
                other = r_u16(0x800B0672 + (value << 2));
                t3_8004ce74(0x800B0970, (uint32)0, (uint32)5, x, y + 4);
                t3_8004ce74(0x800B097C, (uint32)1, y, other);
                t3_8004ce74(0x800B0984, (uint32)0, (uint32)6, y + 6);
                if (!r_u32(0x80095494))
                    w_u16(record + 6, r_u16(record + 6) - 8);
            }
            value = (uint32)r_s16(record + 12);
            other = r_u32(0x800B6BF0);
            if ((sint32)value < (sint32)other && !r_u8(actor + 0xC4)) {
                value = r_u16(0x800B6BF0);
                w_u16(record + 8, 7);
                w_u16(record + 12, value + 40);
                w_u8(actor + 0xC2, 1);
            } else if (r_u8(actor + 0xCE)) {
                value = r_u8(actor + 0x22);
                other = r_u8(primary + 0x1E);
                if (value == other)
                    w_u16(record + 12, r_u16(0x800B6BF0) + 60);
            }
            break;
        case 7:
            w_u8(actor + 0xC3, r_u8(0x800B6BF0) & 1);
            value = (uint32)r_s16(record + 12);
            other = r_u32(0x800B6BF0);
            if ((sint32)value < (sint32)other) {
                w_u32(0x800B65DC, r_u32(0x800B65DC) + 1);
                w_u8(actor + 0xC3, 0);
                w_u16(record + 8, 1);
            }
            break;
        default:
            break;
        }
    }
    t3_bns_02_800b3850();
    t3_bns_02_800b3e90();
    t3_bns_02_800b4440();
    if (r_u32(0x800B6C08) > 99999990)
        w_u32(0x800B6C08, 99999990);
    value = r_u32(0x800B6C08);
    other = r_u32(0x800B6C1C);
    if (other < value) {
        w_u8(0x800AFAC7, 1);
        w_u32(0x800B6C1C, value);
    }
    if (r_s32(0x800ADBCC) >= 5941)
        w_u32(0x800ADBCC, 5940);
    if (!r_u32(0x80095494) && r_u32(0x80096F2C) == 1) {
        value = r_u32(0x800ADBCC);
        switch (value) {
        case 300: t3_800753bc(0, r_u16(0x80097062), 0); break;
        case 240: t3_800753bc(0, r_u16(0x80097060), 0); break;
        case 180: t3_800753bc(0, r_u16(0x8009705E), 0); break;
        case 120: t3_800753bc(0, r_u16(0x8009705C), 0); break;
        case 60: t3_800753bc(0, r_u16(0x8009705A), 0); break;
        default: break;
        }
    }
    return result;
}
