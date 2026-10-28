#include "game_functions.h"

uint32 t3_8006989c(void)
{
    uint32 base;
    uint32 pointer;
    FUNCTION_MARKER(0x8006989C, "SLUS_004.02");
    base = r_u32(0x800ADE80);
    if (base)
        base += 0xB4C94;
    w_u32(0x800ADC28, base);
    w_u32(base + 4, r_u32(base + 4) + base);
    pointer = r_u32(0x800ADC28);
    w_u32(pointer + 8, r_u32(pointer + 8) + base);
    pointer = r_u32(0x800ADC28);
    w_u32(pointer + 16, r_u32(pointer + 16) + base);
    pointer = r_u32(0x800ADC28);
    w_u32(pointer + 20, r_u32(pointer + 20) + base);
    pointer = r_u32(0x800ADC28);
    w_u32(pointer + 28, r_u32(pointer + 28) + base);
    w_u16(0x800A0520, 99);
    return base;
}

uint32 t3_80069ea4(uint32 index)
{
    uint32 slot = 0x800A0510 + index * 8;
    uint32 base;
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x80069EA4, "SLUS_004.02");
    if (!r_s16(slot + 2))
        return 0;
    base = r_u32(slot + 4);
    first = r_u32(base + 4);
    second = r_u32(base + 12);
    w_u32(base + 4, first + base);
    first = r_u32(base + 8);
    w_u32(base + 12, second + base);
    second = r_u32(base + 20);
    w_u32(base + 8, first + base);
    first = r_u32(base + 16);
    w_u32(base + 20, second + base);
    second = r_u32(base + 28);
    w_u32(base + 16, first + base);
    first = r_u32(base + 24);
    w_u32(base + 28, second + base);
    second = r_u32(base + 36);
    w_u32(base + 24, first + base);
    first = r_u32(base + 32);
    w_u32(base + 36, second + base);
    second = r_u32(base + 44);
    w_u32(base + 32, first + base);
    first = r_u32(base + 40);
    w_u32(base + 44, second + base);
    second = r_u32(base + 52);
    w_u32(base + 40, first + base);
    first = r_u32(base + 48);
    w_u32(base + 52, second + base);
    w_u32(base + 48, first + base);
    w_u16(slot + 2, 0);
    return base;
}

uint32 t3_80069f74(void)
{
    uint32 index;
    FUNCTION_MARKER(0x80069F74, "SLUS_004.02");
    if (r_s16(0x800A0512) || r_s16(0x800A051A) || r_s16(0x800A0522)) {
        t3_8006b54c(1);
        t3_8006bea8();
        if (r_s16(0x800A0522))
            t3_80052958(r_u32(0x800A0524), -1);
        for (index = 0; index < 2; ++index) {
            uint32 slot = 0x800A0510 + index * 8;
            if (r_s16(slot + 2)) {
                uint32 destination = r_u32(slot + 4);
                sint32 key = r_s16(slot);
                t3_80052958(destination, key);
            }
        }
        t3_8006c084();
        while (t3_8006c23c()) {
        }
        if (r_s16(0x800A0522)) {
            uint32 resource = t3_8006989c();
            if (resource)
                t3_8002ce14(-1);
            w_u16(0x800A0522, 0);
        }
    }
    for (index = 0; index < 2; ++index) {
        uint32 slot = 0x800A0510 + index * 8;
        if (r_s16(slot + 2)) {
            uint32 base = r_u32(slot + 4);
            uint32 first = r_u32(base + 4);
            uint32 second = r_u32(base + 12);
            w_u32(base + 4, first + base);
            first = r_u32(base + 8);
            w_u32(base + 12, second + base);
            second = r_u32(base + 20);
            w_u32(base + 8, first + base);
            first = r_u32(base + 16);
            w_u32(base + 20, second + base);
            second = r_u32(base + 28);
            w_u32(base + 16, first + base);
            first = r_u32(base + 24);
            w_u32(base + 28, second + base);
            second = r_u32(base + 36);
            w_u32(base + 24, first + base);
            first = r_u32(base + 32);
            w_u32(base + 36, second + base);
            second = r_u32(base + 44);
            w_u32(base + 32, first + base);
            first = r_u32(base + 40);
            w_u32(base + 44, second + base);
            second = r_u32(base + 52);
            w_u32(base + 40, first + base);
            first = r_u32(base + 48);
            w_u32(base + 52, second + base);
            w_u32(base + 48, first + base);
            w_u16(slot + 2, 0);
        }
    }
    return 0;
}

uint32 t3_8002d07c(void)
{
    uint32 first_base;
    uint32 second_base;
    uint32 first_data;
    uint32 second_data;
    uint32 first_masks;
    uint32 second_masks;
    uint32 first_bits;
    uint32 second_bits;
    uint32 first_fill;
    uint32 second_fill;
    uint32 index;
    FUNCTION_MARKER(0x8002D07C, "SLUS_004.02");
    first_base = r_u32(0x800ADC20);
    second_base = r_u32(0x800ADC24);
    first_data = r_u32(first_base + 12);
    first_masks = r_u32(first_base + 24);
    second_data = r_u32(second_base + 12);
    second_masks = r_u32(second_base + 24);
    first_bits = r_u16(first_masks);
    first_masks += 2;
    second_bits = r_u16(second_masks);
    second_masks += 2;
    first_fill = r_u32(first_data + 0x3714);
    second_fill = r_u32(second_data + 0x3714);
    first_bits |= 0x10000;
    second_bits |= 0x10000;
    for (index = 0; index < 4023; ++index) {
        if (first_bits & 1) {
            if (second_bits & 1)
                w_u32(first_data, first_fill);
            else
                w_u32(first_data, r_u32(second_data));
        }
        if (second_bits & 1) {
            if (first_bits & 1)
                w_u32(second_data, second_fill);
            else
                w_u32(second_data, r_u32(first_data));
        }
        first_bits >>= 1;
        second_bits >>= 1;
        if (first_bits == 1) {
            first_bits = r_u16(first_masks) | 0x10000;
            first_masks += 2;
            second_bits = r_u16(second_masks) | 0x10000;
            second_masks += 2;
        }
        first_data += 4;
        second_data += 4;
    }
    return 0;
}

uint32 t3_80077590(uint32 archive, sint32 index)
{
    FUNCTION_MARKER(0x80077590, "SLUS_004.02");
    if (index < r_s32(archive))
        return archive + r_u32(archive + (uint32)index * 8 + 4);
    return 0;
}

uint32 t3_80035bc0(uint32 object, uint32 archive, uint32 model, uint32 extra)
{
    sint32 character;
    uint32 actor;
    uint32 value;
    uint32 index;
    FUNCTION_MARKER(0x80035BC0, "SLUS_004.02");
    character = r_s16(object + 28);
    actor = r_u8(object + 30);
    if ((uint32)(character - 47) < 4) {
        for (index = 47; index < 51; ++index) {
            uint32 resource = t3_80077590(archive, (sint32)(index - 47));
            t3_80035744((sint32)actor, (sint32)index, resource, archive);
        }
    } else {
        t3_80035744((sint32)actor, character, archive, archive);
    }
    DrawSync(0);
    value = r_u32(model + 4) << 12;
    value = (uint32)(((uint64)value * 0x51EB851F) >> 32) >> 5;
    w_u16(object + 1262, value);
    w_u16(object + 1260, value);
    t3_80034ee4(model, 27, object);
    if (r_u32(0x800AFA88) != 8)
        t3_80035080(extra, 18);
    t3_80033d0c(object);
    t3_8003404c(object);
    return t3_80033dc8(object);
}

uint32 t3_80035744(sint32 actor, sint32 character, uint32 source, uint32 incoming_s3)
{
    uint32 character_offset = (uint32)character - 47;
    FUNCTION_MARKER(0x80035744, "SLUS_004.02");
    if (actor >= 2)
        return 0;
    for (;;) {
        T3_TIM_IMAGE tim;
        PSX_RECT clut;
        PSX_RECT image;
        uint32 words[4];
        uint32 index;
        if (r_u32(source) != 16)
            return 16;
        if (t3_8007efc8(source) == 0xFFFFFFFF)
            return 0xFFFFFFFF;
        t3_8007efd8(&tim);
        for (index = 0; index < 4; ++index) {
            uint32 address = index < 2 ? tim.clut_rectangle + index * 4 : tim.image_rectangle + (index - 2) * 4;
            uint32 upper = r_u32((address + 3) & 0xFFFFFFFC);
            uint32 lower = r_u32(address & 0xFFFFFFFC);
            uint32 shift = (address & 3) * 8;
            words[index] = shift ? (lower >> shift) | (upper << (32 - shift)) : lower;
        }
        clut.x = (sint16)words[0];
        clut.y = (sint16)(words[0] >> 16);
        clut.w = (sint16)words[1];
        clut.h = (sint16)(words[1] >> 16);
        image.x = (sint16)words[2];
        image.y = (sint16)(words[2] >> 16);
        image.w = (sint16)words[3];
        image.h = (sint16)(words[3] >> 16);
        clut.y = (sint16)((uint16)clut.y + 504);
        image.x = (sint16)((uint16)image.x + 384);
        if (character_offset < 4) {
            clut.y = (sint16)((uint16)clut.y + character_offset);
            image.x = (sint16)((uint16)image.x + character_offset * 16);
        }
        if (tim.mode != 128 || (sint32)incoming_s3 >= 16) {
            uint32 span;
            if (actor)
                image.y = (sint16)((uint16)image.y + 256);
            DrawSync(0);
            span = image.w > 0 && image.h > 0 ? (uint32)image.w * (uint32)image.h * 2 : 0;
            LoadImagePSX(&image, (uint32 *)psx_addr(tim.image_pixels, span));
        }
        if (tim.mode & 8) {
            uint32 span;
            if (actor)
                clut.y = (sint16)((uint16)clut.y + 4);
            span = clut.w > 0 && clut.h > 0 ? (uint32)clut.w * (uint32)clut.h * 2 : 0;
            LoadImagePSX(&clut, (uint32 *)psx_addr(tim.clut_pixels, span));
        }
        source += 8;
        if (tim.mode & 8)
            source += r_u32(source) & 0xFFFFFFFC;
        source += r_u32(source) & 0xFFFFFFFC;
    }
}

void t3_80034ee4(uint32 model, uint32 count, uint32 object)
{
    uint32 index;
    uint32 record = model;
    FUNCTION_MARKER(0x80034EE4, "SLUS_004.02");
    for (index = 0; index < count; ++index, record += 56) {
        uint32 offset;
        uint32 base = model;
        uint32 table;
        uint32 value;
        for (offset = 16; offset <= 32; offset += 4) {
            value = r_u32(record + offset);
            if (value && !(value & 0xFF000000))
                w_u32(record + offset, value + model);
        }
        if (index == 2 && r_s16(object + 24) == 20)
            base = t3_8006975c(object);
        table = r_u32(record + 20);
        if (!table)
            continue;
        value = r_u32(table);
        if (value && !(value & 0xFF000000))
            w_u32(table, value + model);
        for (offset = 4; offset <= 160; offset += 4) {
            value = r_u32(table + offset);
            if (value && !(value & 0xFF000000))
                w_u32(table + offset, value + base);
        }
    }
}

uint32 t3_80033d0c(uint32 object)
{
    uint32 record;
    sint32 first;
    sint32 second;
    sint32 third;
    uint32 position;
    uint32 size;
    uint32 destination;
    sint32 x;
    sint32 y;
    sint32 side;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x80033D0C, "SLUS_004.02");
    record = 0x800959CC + (uint32)r_s16(object + 28) * 4;
    first = r_s8(record);
    if (first == -1)
        return 0xFFFFFFFC;
    position = 0x8009579C + (uint32)first * 4;
    rectangle.x = (sint16)r_u16(position);
    second = r_s8(record + 1);
    third = r_s8(record + 2);
    rectangle.y = (sint16)r_u16(position + 2);
    size = 0x8009586C + (uint32)second * 4;
    rectangle.w = (sint16)r_u16(size);
    rectangle.h = (sint16)r_u16(size + 2);
    destination = 0x8009579C + (uint32)third * 4;
    y = r_s16(destination + 2);
    side = r_s16(object + 18);
    x = r_s16(destination);
    if (side) {
        rectangle.y = (sint16)((uint16)rectangle.y + 256);
        y += 256;
    }
    return (uint32)MoveImage(&rectangle, x, y);
}

uint32 t3_8003404c(uint32 object)
{
    uint32 record;
    uint32 size;
    uint32 position;
    uint32 destination;
    sint32 second;
    sint32 third;
    sint32 x;
    sint32 y;
    sint32 side;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x8003404C, "SLUS_004.02");
    if (r_s16(object + 24) != 17)
        return 0x80090000;
    record = 0x8009591C + (uint32)r_s16(object + 28) * 4;
    size = 0x8009586C + (uint32)r_s8(record) * 4;
    rectangle.w = (sint16)r_u16(size);
    rectangle.h = (sint16)r_u16(size + 2);
    second = r_s8(record + 1);
    third = r_s8(record + 2);
    position = 0x8009579C + (uint32)second * 4;
    rectangle.x = (sint16)r_u16(position);
    rectangle.y = (sint16)r_u16(position + 2);
    destination = 0x8009579C + (uint32)third * 4;
    y = r_s16(destination + 2);
    side = r_s16(object + 18);
    x = r_s16(destination);
    if (side) {
        rectangle.y = (sint16)((uint16)rectangle.y + 4);
        y += 4;
    }
    return (uint32)MoveImage(&rectangle, x, y);
}

uint32 t3_80033dc8(uint32 object)
{
    uint32 record;
    uint32 size;
    uint32 index;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x80033DC8, "SLUS_004.02");
    if (r_s16(object + 24) != 17)
        return 17;
    record = 0x800958BC + (uint32)r_s16(object + 28) * 6;
    size = 0x8009586C + (uint32)r_s8(record) * 4;
    rectangle.w = (sint16)r_u16(size);
    rectangle.h = (sint16)r_u16(size + 2);
    for (index = 0; index < 2; ++index) {
        sint32 first;
        sint32 second;
        uint32 position;
        uint32 destination;
        sint32 x;
        sint32 y;
        sint32 side;
        w_u8(object + index + 0x1292, 0);
        first = r_s8(record + index + 2);
        second = r_s8(record + index + 4);
        position = 0x8009579C + (uint32)first * 4;
        rectangle.x = (sint16)r_u16(position);
        rectangle.y = (sint16)r_u16(position + 2);
        destination = 0x8009579C + (uint32)second * 4;
        y = r_s16(destination + 2);
        side = r_s16(object + 18);
        x = r_s16(destination);
        if (side) {
            rectangle.y = (sint16)((uint16)rectangle.y + 256);
            y += 256;
        }
        MoveImage(&rectangle, x, y);
    }
    return object + 2;
}


uint32 t3_8002ce14(sint32 actor)
{
    uint32 base;
    uint32 record;
    sint32 count;
    uint32 index;
    FUNCTION_MARKER(0x8002CE14, "SLUS_004.02");
    if (actor < 0) {
        actor = 2;
    } else {
        uint32 loaded = r_u8(r_u32(0x800ADC20 + (uint32)actor * 4));
        if (loaded)
            return loaded;
    }
    base = r_u32(0x800ADC20 + (uint32)actor * 4);
    count = r_s16(base + 2);
    record = r_u32(base + 8);
    if (count > 0) {
        index = 0;
        do {
            uint32 value = r_u32(record);
            uint32 pointer;
            uint32 reference;
            uint32 entry;
            uint32 byte;
            if (value <= 0x3FFFFFFF) {
                pointer = value + r_u32(base + 28);
            } else {
                pointer = value + r_u32(r_u32(0x800ADC28) + 28) + 0xC0000000;
            }
            w_u32(record, pointer);
            byte = r_u8(pointer);
            reference = r_u32(record + 40);
            entry = r_u32(record + 12);
            w_u8(record + 24, byte);
            pointer = r_u32(base + 4) + entry * 12;
            w_u32(record + 12, pointer);
            if ((sint32)reference >= 0) {
                uint32 row;
                uint32 replacement = 0;
                for (row = 0; row < 13; ++row) {
                    uint32 table = 0x80017C10 + row * 8;
                    if (reference == r_u16(table)) {
                        replacement = r_u32(table + 4);
                        break;
                    }
                }
                if (!replacement)
                    replacement = reference - 4 + r_u32(base + 40);
                w_u32(record + 40, replacement);
            }
            entry = r_u32(record + 32);
            if (entry == 0xFFFFFFFF) {
                w_u32(record + 32, 0);
            } else {
                pointer = r_u32(base + 20) + entry * 4;
                w_u32(record + 32, pointer);
                w_u16(record + 20, t3_8002d2c8(record));
            }
            entry = r_u32(record + 28);
            if (entry == 0xFFFFFFFF)
                w_u32(record + 28, 0);
            else
                w_u32(record + 28, r_u32(base + 16) + entry * 2);
            ++index;
            count = r_s16(base + 2);
            record += 56;
        } while ((sint32)index < count);
    }
    if (actor < 2) {
        uint32 table = r_u32(base + 12);
        for (index = 0; index < 4023; ++index, table += 4) {
            uint32 value = r_u32(table);
            uint32 owner;
            if (value <= 0x3FFFFFFF) {
                owner = r_u32(base + 8);
            } else {
                value += 0xC0000000;
                owner = r_u32(r_u32(0x800ADC28) + 8);
            }
            w_u32(table, owner + value * 56);
        }
    }
    w_u8(base, 1);
    return 1;
}

uint32 t3_800350e8(uint32 object, uint32 color, uint32 actor)
{
    uint32 table = 0x800ADED8 + actor * 8;
    uint32 first_count;
    uint32 second_count;
    uint32 row;
    FUNCTION_MARKER(0x800350E8, "SLUS_004.02");
    first_count = r_u32(object + 12);
    second_count = r_u32(object + 16);
    for (row = 0; row < 2; ++row) {
        uint32 previous = 0;
        uint32 packet = r_u32(table);
        uint32 count = first_count;
        w_u32(object, packet);
        do {
            uint32 tag = previous | 0x04000000;
            previous = packet & 0x00FFFFFF;
            w_u32(packet, tag);
            w_u32(packet + 4, color | 0x21000000);
            --count;
            packet += 20;
        } while ((sint32)count > 0);
        count = second_count;
        do {
            uint32 tag = previous | 0x05000000;
            previous = packet & 0x00FFFFFF;
            w_u32(packet, tag);
            w_u32(packet + 4, color | 0x29000000);
            --count;
            packet += 24;
        } while ((sint32)count > 0);
        object += 4;
        table += 4;
    }
    return 0;
}

uint32 t3_80035908(uint32 object, uint32 actor, sint32 index, uint32 model)
{
    uint32 slot = object + (uint32)index * 40 + 0x4F0;
    uint32 position_index;
    uint32 model_index;
    uint32 destination;
    uint32 model_record;
    sint32 parent;
    FUNCTION_MARKER(0x80035908, "SLUS_004.02");
    position_index = r_u8(0x8001A074 + (uint32)index);
    model_index = r_u8(0x8001A05C + (uint32)index);
    w_u32(slot, 0);
    if (r_s16(0x8001A08C + (uint32)index * 2) >= 0) {
        uint32 color = t3_80039e74(r_u16(0x800ADC84));
        sint32 character = r_s16(object + 28);
        t3_80035190(model, slot, index, color, actor, 0, character);
        color = t3_80039e74(r_u16(0x800ADC84));
        character = r_s16(object + 28);
        t3_80035190(model, slot, index, color, actor, 1, character);
        w_u32(slot, 2);
    }
    if (index >= 18 && !(t3_80037c24(r_s16(object + 28), index) & 255))
        w_u32(slot, 0);
    destination = object + position_index * 68 + 0x8F4;
    model_record = model + model_index * 56;
    w_u32(slot + 4, destination);
    parent = r_s32(model_record + 48);
    if (parent >= 0) {
        uint32 source = r_u32(object + (uint32)parent * 40 + 0x4F4);
        t3_8003c250(source, destination);
    } else if (index >= 18 && parent == -1) {
        w_u32(slot + 4, 0);
    } else {
        destination = r_u32(slot + 4);
        t3_8003c250(object + 0x8B0, destination);
    }
    destination = object + position_index * 32;
    w_u32(destination + 0xF88, r_u32(model_record + 36));
    w_u32(destination + 0xF8C, r_u32(model_record + 40));
    w_u32(destination + 0xF90, 0 - r_u32(model_record + 44));
    if (index >= 18)
        return t3_80037c58((uint32)index, object, model_record + 16);
    return 1;
}

uint32 t3_80035ce8(uint32 object, uint32 model, uint32 packets)
{
    uint32 actor;
    uint32 index;
    FUNCTION_MARKER(0x80035CE8, "SLUS_004.02");
    actor = r_u8(object + 30);
    w_u32(object + 0x1274, packets);
    t3_80034db4(actor);
    t3_8003c250(0, object + 0x8B0);
    for (index = 0; index < 22; ++index)
        t3_80035908(object, actor, (sint32)index, model);
    if (r_u32(0x800AFA88) == 8) {
        uint32 color = t3_80039ec4(r_u16(0x800ADC84));
        t3_80079290(packets, color, actor);
    } else {
        uint32 color = t3_80039e9c(r_u16(0x800ADC84));
        t3_800350e8(packets, color, actor);
    }
    w_u8(object + 0x128B, 0);
    w_u8(object + 0x128A, 0);
    t3_80033dbc(object);
    t3_800347b4(object);
    return t3_800313c8(object);
}

uint32 t3_800347b4(uint32 object)
{
    uint32 index;
    sint32 side;
    FUNCTION_MARKER(0x800347B4, "SLUS_004.02");
    for (index = 0; index < 2; ++index) {
        uint32 value = t3_80036294(object) & 255;
        w_u16(object + index * 2 + 0x1280, value);
        w_u16(object + index * 2 + 0x127C, value);
        w_u16(object + index * 2 + 0x1284, 0);
    }
    side = r_s16(object + 18);
    w_u8(object + 0x128A, 0);
    w_u8(object + 0x128B, 0);
    w_u8(object + 0x128C, 0);
    w_u16(object + 0x1296, 0);
    w_u16(object + 0x1298, 0);
    w_u16(object + 0x129A, 0);
    w_u8(0x8009BCF0 + (uint32)side, 0);
    return 0x8009BCF0;
}

uint32 t3_80034e14(uint32 actor, uint32 first_output, uint32 second_output)
{
    uint32 table = 0x8009BCF8 + actor * 8;
    uint32 value;
    FUNCTION_MARKER(0x80034E14, "SLUS_004.02");
    value = r_u32(table);
    w_u32(first_output, value);
    value = r_u32(table + 4);
    w_u32(second_output, value);
    return value;
}

uint32 t3_80034e3c(uint32 actor, uint32 first, uint32 second)
{
    uint32 table = 0x8009BCF8 + actor * 8;
    FUNCTION_MARKER(0x80034E3C, "SLUS_004.02");
    w_u32(table, first);
    w_u32(table + 4, second);
    return 0x8009BCF8;
}

uint32 t3_800313c8(uint32 object)
{
    sint32 kind;
    uint32 actor;
    uint32 table;
    uint32 row;
    uint32 outputs[2];
    FUNCTION_MARKER(0x800313C8, "SLUS_004.02");
    kind = r_s16(object + 24);
    actor = (uint32)r_s16(object + 18);
    if (kind != 4 && kind != 15)
        return 15;
    table = 0x8009BC88 + actor * 8;
    t3_80034e14(actor, table, table + 4);
    for (row = 0; row < 2; ++row) {
        uint32 packet = r_u32(table + row * 4);
        uint8 colors[18];
        uint8 selectors[10];
        uint8 black[3] = {0, 0, 0};
        const uint8 *palette[2];
        uint32 index;
        uint32 color_offset;
        uint32 page;
        sint32 character;
        for (index = 0; index < 4; ++index) {
            uint32 address = 0x80019F04 + index * 4;
            uint32 upper = r_u32(address);
            uint32 value = r_u32(address);
            colors[index * 4] = (uint8)value;
            colors[index * 4 + 1] = (uint8)(value >> 8);
            colors[index * 4 + 2] = (uint8)(value >> 16);
            colors[index * 4 + 3] = (uint8)(value >> 24);
            (void)upper;
        }
        colors[16] = r_u8(0x80019F14);
        colors[17] = r_u8(0x80019F15);
        for (index = 0; index < 2; ++index) {
            uint32 address = 0x80019F18 + index * 4;
            uint32 upper = r_u32(address);
            uint32 value = r_u32(address);
            selectors[index * 4] = (uint8)value;
            selectors[index * 4 + 1] = (uint8)(value >> 8);
            selectors[index * 4 + 2] = (uint8)(value >> 16);
            selectors[index * 4 + 3] = (uint8)(value >> 24);
            (void)upper;
        }
        selectors[8] = r_u8(0x80019F20);
        selectors[9] = r_u8(0x80019F21);
        character = r_s16(object + 28);
        color_offset = row * 3 + (character == 8 ? 0 : character == 9 ? 6 : 12);
        palette[0] = black;
        palette[1] = colors + color_offset;
        page = getTPage(0, 1, 512, 0) & 0xFFFF;
        for (index = 0; index < 10; ++index) {
            uint32 selector = selectors[index];
            uint32 vertex;
            uint32 opcode;
            setPolyGT4((POLY_GT4 *)psx_addr(packet, 52));
            for (vertex = 0; vertex < 4; ++vertex) {
                const uint8 *color = palette[(selector >> vertex) & 1];
                w_u8(packet + vertex * 12 + 4, color[0]);
                w_u8(packet + vertex * 12 + 5, color[1]);
                w_u8(packet + vertex * 12 + 6, color[2]);
            }
            opcode = r_u8(packet + 7);
            w_u8(packet + 12, 0);
            w_u8(packet + 13, 0);
            w_u8(packet + 24, 0);
            w_u8(packet + 25, 0);
            w_u8(packet + 36, 0);
            w_u8(packet + 37, 0);
            w_u8(packet + 48, 0);
            w_u8(packet + 49, 0);
            w_u16(packet + 26, page);
            w_u16(packet + 14, 0x7FD1);
            w_u8(packet + 7, opcode | 2);
            packet += 52;
        }
        outputs[row] = packet;
    }
    return t3_80034e3c(actor, outputs[0], outputs[1]);
}

uint32 t3_80075288(uint32 actor, uint32 resource, uint32 header, uint32 body)
{
    sint32 kind;
    uint32 bank;
    uint32 address;
    sint32 short_bank;
    FUNCTION_MARKER(0x80075288, "SLUS_004.02");
    kind = r_s16(0x800A9228 + actor * 6284 + 26);
    bank = r_u32(0x800ADFF8 + actor * 12 + 8);
    if (kind == 22)
        return 22;
    if (header == 0x800A25E0)
        return bank << 16;
    short_bank = (sint16)bank;
    SsVabClose((sint16)short_bank);
    if (bank == 3)
        address = 0x80000 - r_u32(0x800278A8 + resource * 4);
    else
        address = r_u32(0x8002790C + bank * 4);
    t3_8008aa28(header, short_bank, address);
    t3_8008ae58(body, (sint16)bank);
    SsVabTransCompleted(1);
    w_u8(0x800A9224 + bank, resource);
    return 0x800A9224 + bank;
}

uint32 t3_8007619c(sint32 actor, sint32 kind, uint32 source)
{
    uint32 count_table = 0x80027950 + (uint32)kind * 6;
    uint32 configuration;
    uint32 x;
    uint32 y;
    sint32 stride_x;
    sint32 stride_y;
    sint32 quotient_x;
    sint32 quotient_y;
    sint32 remainder_x;
    sint32 remainder_y;
    uint32 base_x;
    uint16 base_y;
    uint32 column;
    uint32 row;
    uint32 index = 0;
    sint32 count;
    FUNCTION_MARKER(0x8007619C, "SLUS_004.02");
    if (actor >= 2)
        return 0;
    configuration = 0x800279EC + ((uint32)actor & 1) * 48;
    t3_80029638(1);
    x = r_u16(configuration + 4);
    stride_x = r_s16(configuration + 8);
    quotient_x = stride_x ? (sint32)x / stride_x : -1;
    remainder_x = stride_x ? (sint32)x % stride_x : (sint32)x;
    y = r_u16(configuration + 6);
    stride_y = r_s16(configuration + 10);
    quotient_y = stride_y ? (sint32)y / stride_y : -1;
    remainder_y = stride_y ? (sint32)y % stride_y : (sint32)y;
    base_x = (uint32)quotient_x * (uint32)stride_x;
    base_y = (uint16)((uint32)quotient_y * (uint32)stride_y);
    column = (uint32)(remainder_x < 0 ? remainder_x + 7 : remainder_x) >> 3;
    row = (uint32)(remainder_y < 0 ? remainder_y + 31 : remainder_y) >> 5;
    count = r_s16(count_table);
    while ((sint32)index < count) {
        T3_TIM_IMAGE tim;
        uint32 width;
        uint32 step;
        uint32 length;
        t3_8007efc8(source);
        t3_8007efd8(&tim);
        if (!index) {
            w_u16(tim.clut_rectangle, r_u16(configuration));
            w_u16(tim.clut_rectangle + 2, r_u16(configuration + 2));
            t3_8007c650(tim.clut_rectangle, tim.clut_pixels);
            DrawSync(0);
        }
        w_u16(tim.image_rectangle, x);
        w_u16(tim.image_rectangle + 2, y);
        t3_8007c650(tim.image_rectangle, tim.image_pixels);
        DrawSync(0);
        ++column;
        stride_x = r_s16(configuration + 8);
        width = r_u16(tim.image_rectangle + 4);
        step = r_u16(configuration + 8);
        x += width;
        if ((sint16)column >= (stride_x < 0 ? stride_x + 7 : stride_x) >> 3) {
            uint32 height;
            ++row;
            height = r_u16(tim.image_rectangle + 6);
            stride_y = r_s16(configuration + 10);
            y += height;
            column = 0;
            if ((sint16)row >= (stride_y < 0 ? stride_y + 31 : stride_y) >> 5) {
                row = 0;
                y = base_y;
                base_x += step;
            }
            x = base_x;
        }
        source += 8;
        ++index;
        source += r_u32(source) & 0xFFFFFFFC;
        length = r_u32(source) & 0xFFFFFFFC;
        count = r_s16(count_table);
        source += length;
    }
    return t3_80029638(0);
}

uint32 t3_80035190(uint32 model, uint32 slot, sint32 index, uint32 color, uint32 actor, uint32 buffer_index, sint32 character)
{
    uint32 model_index;
    uint32 first_uv;
    uint32 second_uv;
    uint32 uv_adjustment = 0;
    uint32 character_offset = (uint32)character - 47;
    uint32 record_offset;
    uint32 value;
    uint32 stream;
    uint32 palette;
    uint32 coordinates;
    uint32 first_table;
    uint32 second_table;
    uint32 first_packet;
    uint32 second_packet;
    uint32 result;
    uint32 group;
    FUNCTION_MARKER(0x80035190, "SLUS_004.02");
    model_index = r_u8(0x8001A05C + (uint32)index) + buffer_index;
    first_uv = actor ? 0x7F000000 : 0x7E000000;
    second_uv = actor ? 0x00160000 : 0x00060000;
    if (character_offset < 4) {
        first_uv += character_offset * 0x00400040;
        uv_adjustment = character_offset << 6;
        second_uv += uv_adjustment;
    }
    record_offset = model_index * 56;
    value = r_u32(model + record_offset + 16);
    if (!value) {
        w_u32(slot + 12, 0);
        return 0;
    }
    if (buffer_index) {
        uint32 eligible = r_u8(0x8001A044 + (uint32)index);
        if (eligible != 1) {
            w_u32(slot + 12, 0);
            return record_offset + 16;
        }
        w_u32(slot + 12, model + record_offset + 16);
    } else {
        w_u32(slot + 8, model + record_offset + 16);
    }
    stream = r_u32(model + model_index * 56 + 32);
    first_table = 0x8009BCF8 + actor * 8;
    second_table = first_table + 4;
    if (buffer_index) {
        w_u32(slot + 32, r_u32(first_table));
        w_u32(slot + 36, r_u32(second_table));
    } else {
        w_u32(slot + 24, r_u32(first_table));
        w_u32(slot + 28, r_u32(second_table));
    }
    palette = stream + 2;
    first_packet = r_u32(first_table);
    value = r_u16(stream);
    second_packet = r_u32(second_table);
    stream += value;
    result = r_u16(stream);
    coordinates = stream + 2;
    stream += result;
    for (group = 0; group < 4; ++group) {
        uint32 count = r_u8(stream++);
        uint32 item;
        uint32 gouraud = group >= 2;
        uint32 quad = group & 1;
        uint32 stride = gouraud ? (quad ? 52 : 40) : (quad ? 40 : 32);
        uint32 uv_stride = gouraud ? 12 : 8;
        uint32 command = color | (gouraud ? (quad ? 0x3C000000 : 0x34000000) : (quad ? 0x2C000000 : 0x24000000));
        for (item = 0; item < count; ++item) {
            uint32 palette_index = r_u8(stream++);
            uint32 packed_color = r_u16(palette + palette_index * 2);
            uint32 coordinate_index;
            uint32 offset;
            uint32 last_index;
            uint32 total;
            uint32 group_total;
            uint32 group_counter = 0x8009BD14 + group * 4;
            w_u32(second_packet + 4, command);
            w_u32(first_packet + 4, command);
            if (gouraud) {
                for (offset = 16; offset <= (quad ? 40u : 28u); offset += 12) {
                    w_u32(second_packet + offset, color);
                    w_u32(first_packet + offset, color);
                }
            }
            coordinate_index = r_u8(stream++);
            value = ((packed_color & 255) << 16) + r_u16(coordinates + coordinate_index * 2) + first_uv;
            w_u32(second_packet + 12, value);
            w_u32(first_packet + 12, value);
            coordinate_index = r_u8(stream++);
            value = ((packed_color & 0xFF00) << 8) + r_u16(coordinates + coordinate_index * 2) + second_uv;
            w_u32(second_packet + 12 + uv_stride, value);
            w_u32(first_packet + 12 + uv_stride, value);
            if (quad) {
                coordinate_index = r_u8(stream++);
                value = r_u16(coordinates + coordinate_index * 2) + uv_adjustment;
                w_u32(second_packet + 12 + uv_stride * 2, value);
                w_u32(first_packet + 12 + uv_stride * 2, value);
            }
            last_index = r_u8(stream++);
            total = r_u32(0x8009BD10);
            w_u32(0x8009BD10, total + 1);
            group_total = r_u32(group_counter);
            value = r_u16(coordinates + last_index * 2) + uv_adjustment;
            offset = 12 + uv_stride * (quad ? 3 : 2);
            w_u32(second_packet + offset, value);
            w_u32(first_packet + offset, value);
            w_u32(group_counter, group_total + 1);
            first_packet += stride;
            second_packet += stride;
            result = 0;
        }
    }
    w_u32(first_table, first_packet);
    w_u32(second_table, second_packet);
    return result;
}

uint32 t3_8004e79c(uint32 first_character, uint32 first_variant, uint32 second_character, uint32 second_variant)
{
    uint32 row;
    FUNCTION_MARKER(0x8004E79C, "SLUS_004.02");
    for (row = 0; row < 2; ++row) {
        uint32 character = row ? second_character : first_character;
        uint32 variant = row ? second_variant : first_variant;
        uint32 resource = t3_8004eff0(character, variant);
        uint32 descriptor = r_u32(0x80021F38 + row * 8);
        uint32 value = r_u8(resource + 5);
        w_u16(0x8009F2C0 + row * 2, value);
        value = r_u16(0x80021F44 + variant * 2);
        w_u32(0x8009F2C8 + row * 4, descriptor | (value << 16));
        w_u16(0x8009F2D4 + row * 2, variant);
        w_u16(0x8009F2D0 + row * 2, row ? 23 : 7);
    }
    return 0;
}

uint32 t3_800516cc(uint32 object)
{
    uint32 mode;
    sint32 side;
    uint32 value;
    uint32 index;
    FUNCTION_MARKER(0x800516CC, "SLUS_004.02");
    mode = r_u32(0x800AFA88);
    side = r_s16(object + 18);
    if (mode < 9) {
        uint32 target = r_u32(0x80022614 + mode * 4);
        (void)target;
    }
    switch (mode) {
    case 0:
        index = r_u8(0x800AFAA4) == 2 ? 3 : 8;
        value = r_u32(0x800225EC + index * 4);
        break;
    case 1:
        index = r_u16(0x800AFAC2 + (uint32)side * 4);
        value = r_u32(0x800225EC + index * 4);
        break;
    case 2:
        value = r_u32(0x80022600);
        break;
    case 4:
        value = r_u32(0x800225F8);
        break;
    case 5:
        value = r_u32(0x80022608);
        break;
    case 6:
        value = r_u32(0x800225F4);
        break;
    case 8:
        value = r_u8(object + 197) ? 0 : r_u32(0x8002260C);
        break;
    default:
        value = r_u32(0x8002260C);
        break;
    }
    w_u32(object + 1016, value);
    return value;
}

uint32 t3_8002c050(uint32 object)
{
    uint32 index;
    uint32 actor;
    FUNCTION_MARKER(0x8002C050, "SLUS_004.02");
    for (index = 0; index < 60; ++index) {
        w_u8(object + 0x41C + index, 5);
        w_u16(object + 0x462 + index * 2, 0);
    }
    w_u16(object + 0x402, 5);
    w_u32(object + 0x410, 0xFFFFFFFF);
    w_u32(object + 0x414, 0xFFFFFFFF);
    actor = r_u8(object + 30);
    w_u32(object + 0x40C, 0);
    w_u16(object + 0x406, 0);
    w_u16(object + 0x404, 0);
    w_u32(object + 0x4DC, 0);
    w_u32(object + 0x4E0, 0);
    w_u32(object + 0x4E4, 0);
    w_u32(object + 0x4E8, 0);
    w_u16(object + 0x408, 0);
    w_u32(0x80095670 + actor * 4, 0);
    return t3_80040ab8(r_u8(object + 30));
}

uint32 t3_8006f468(void)
{
    uint32 base;
    uint32 index;
    FUNCTION_MARKER(0x8006F468, "SLUS_004.02");
    base = r_u32(0x800ADC80);
    w_u32(0x800A0984, base);
    w_u32(0x800A0980, base ? base + 0x3160 : 0);
    w_u32(0x800A098C, 0x800A0988);
    w_u32(0x800A0988, 0x800A0988);
    for (index = 0; index < 80; ++index) {
        uint32 node = base + index * 160;
        w_u32(node, node - 160);
        w_u32(node + 4, node + 160);
    }
    w_u32(base, 0x800A0980);
    w_u32(base + 0x3164, 0x800A0980);
    return 0x800A0980;
}

uint32 t3_8006e614(void)
{
    FUNCTION_MARKER(0x8006E614, "SLUS_004.02");
    t3_8006f468();
    w_u8(0x800A0990, 0);
    w_u8(0x800A0991, 0);
    w_u32(0x800ADEAC, 0);
    w_u32(0x800ADEB0, 0);
    w_u8(0x800AAAB0, 0);
    w_u8(0x800AC33C, 0);
    w_u8(0x800ADBC8, 0);
    w_u32(0x800A8660, 0);
    w_u32(0x800A8664, 0);
    t3_8006e958(6);
    t3_8006e958(10);
    t3_8006e958(14);
    t3_8006e958(17);
    t3_8006e958(0x8006);
    t3_8006e958(0x800A);
    t3_8006e958(0x800E);
    return t3_8006e958(0x8011);
}

uint32 t3_80048374(uint32 index)
{
    uint32 owner;
    uint32 ordering;
    FUNCTION_MARKER(0x80048374, "SLUS_004.02");
    owner = r_u32(0x800A8C54);
    ordering = r_u32(owner + 4);
    return t3_8007ba74(ordering + 0xFFC, 0x8009E738 + index * 16);
}

uint32 t3_8006d7cc(void)
{
    uint32 status;
    uint32 mode;
    uint32 index;
    uint32 owner;
    uint32 ordering;
    uint32 packet;
    uint32 packet_tag;
    uint32 ordering_tag;
    uint32 offset;
    uint32 result;
    uint32 special;
    FUNCTION_MARKER(0x8006D7CC, "SLUS_004.02");
    status = r_u32(0x800ADCA4);
    if (status)
        return status;
    mode = r_u32(0x800AFA88);
    if (mode == 8)
        return t3_bns_02_800b4914();
    if (r_u8(0x800AFAA0))
        return t3_80048374(r_u32(0x800ADEFC));
    if (mode == 7)
        return t3_bns_03_800b5a48();
    index = r_u32(0x800ADEFC);
    t3_8007c890(0x800A08C8 + index * 8, 2);
    t3_8006d014();
    special = r_u16(0x800ADC84) == 11;
    offset = special ? 0xFF4 : 0xFC8;
    index = r_u32(0x800ADEFC);
    owner = r_u32(0x800A8C54);
    packet = 0x800A08C8 + index * 8;
    ordering = r_u32(owner + 4);
    packet_tag = r_u32(packet);
    ordering_tag = r_u32(ordering + offset);
    w_u32(packet, (packet_tag & 0xFF000000) | (ordering_tag & 0x00FFFFFF));
    ordering = r_u32(owner + 4);
    ordering_tag = r_u32(ordering + offset);
    result = (ordering_tag & 0xFF000000) | ((0x800A08CC + index * 8) & 0x00FFFFFF);
    w_u32(ordering + offset, result);
    if (special)
        return t3_80048374(index);
    return result;
}

uint32 t3_8006e1b8(void)
{
    uint32 x;
    uint32 z;
    uint32 center_x;
    uint32 center_z;
    uint32 angle;
    uint32 desired;
    uint32 previous;
    uint32 lookup;
    uint32 sine;
    uint32 cosine;
    uint32 scaled_current;
    uint32 difference;
    uint32 magnitude;
    sint32 turn;
    uint32 amount;
    FUNCTION_MARKER(0x8006E1B8, "SLUS_004.02");
    x = r_u32(0x800ADC10);
    z = r_u32(0x800ADC18);
    center_x = r_u32(0x800A85D4);
    center_z = r_u32(0x800A85DC);
    w_u32(0x800A91A8, x);
    w_u32(0x800A91B0, z);
    desired = t3_8004b39c(x - center_x, z - center_z);
    angle = r_u32(0x800A85C4) & 0xFFF;
    lookup = ((0 - angle) & 0xFFF) * 2;
    previous = r_u32(0x800A919C) & 0xFFF;
    sine = (uint32)r_s16(0x8001E8C4 + lookup);
    cosine = (uint32)r_s16(0x8001F0C4 + lookup);
    center_x = r_u32(0x800A85D4);
    w_u32(0x800A9190, previous);
    center_z = r_u32(0x800A85DC);
    w_u32(0x800A919C, angle);
    w_u32(0x800ADC10, sine);
    w_u32(0x800ADC18, cosine);
    w_u32(0x800ADC10, sine + center_x);
    w_u32(0x800ADC18, cosine + center_z);
    desired = ((uint32)(sint32)(sint16)desired - 1024) & 0xFFF;
    if (!r_u32(0x80096F2C))
        return 0;
    previous = r_u16(0x800A9190);
    scaled_current = (uint32)r_u16(0x800A919C) << 4;
    difference = (previous << 4) - scaled_current;
    magnitude = (sint16)difference < 0 ? 0 - difference : difference;
    if ((sint16)magnitude >= 17)
        return 0;
    turn = (sint16)((desired << 4) - scaled_current);
    if (turn >= 0) {
        amount = (uint32)turn / 24;
        if ((sint16)amount >= 171)
            amount = 170;
        return amount & 0xFFF;
    }
    amount = (uint16)((angle - desired) << 4) / 24;
    if (amount >= 171)
        amount = 170;
    return (0 - amount) & 0xFFF;
}

uint32 t3_8004b39c(uint32 x, uint32 z)
{
    uint32 numerator;
    uint32 denominator;
    uint32 bias;
    uint32 subtract;
    uint32 quotient;
    uint32 value;
    FUNCTION_MARKER(0x8004B39C, "SLUS_004.02");
    if (!x && !z)
        return 0;
    if ((sint32)x >= 0) {
        if ((sint32)z >= 0) {
            if ((sint32)x < (sint32)z) {
                numerator = x << 10;
                denominator = z;
                bias = 1024;
                subtract = 1;
            } else {
                numerator = z << 10;
                denominator = x;
                bias = 0;
                subtract = 0;
            }
        } else {
            uint32 magnitude = 0 - z;
            if ((sint32)x < (sint32)magnitude) {
                numerator = x << 10;
                denominator = magnitude;
                bias = 3072;
                subtract = 0;
            } else {
                numerator = magnitude << 10;
                denominator = x;
                bias = 4096;
                subtract = 1;
            }
        }
    } else {
        uint32 magnitude = 0 - x;
        if ((sint32)z >= 0) {
            if ((sint32)magnitude < (sint32)z) {
                numerator = magnitude << 10;
                denominator = z;
                bias = 1024;
                subtract = 0;
            } else {
                numerator = z << 10;
                denominator = magnitude;
                bias = 2048;
                subtract = 1;
            }
        } else if ((sint32)magnitude < (sint32)(0 - z)) {
            numerator = x << 10;
            denominator = z;
            bias = 3072;
            subtract = 1;
        } else {
            numerator = z << 10;
            denominator = x;
            bias = 2048;
            subtract = 0;
        }
    }
    if (!denominator)
        quotient = (sint32)numerator < 0 ? 1 : 0xFFFFFFFF;
    else if (numerator == 0x80000000 && denominator == 0xFFFFFFFF)
        quotient = 0x80000000;
    else
        quotient = (uint32)((sint32)numerator / (sint32)denominator);
    value = r_u8(0x800210C8 + quotient);
    value += (uint32)((sint32)quotient >> 1);
    value = subtract ? bias - value : bias + value;
    return (uint32)(sint32)(sint16)value;
}

void t3_80036af8(uint32 vertices, uint32 *screen, uint16 *depths, uint32 count)
{
    sint32 remaining;
    FUNCTION_MARKER(0x80036AF8, "SLUS_004.02");
    if ((sint32)count < -2147483645) {
        xport_mips_overflow_exception(0x80036AF8);
        return;
    }
    remaining = (sint32)(count - 3);
    xport_gte_write_data(0, r_u32(vertices));
    if (remaining >= 0) {
        xport_gte_write_data(1, r_u32(vertices + 4));
        xport_gte_write_data(2, r_u32(vertices + 8));
        xport_gte_write_data(3, r_u32(vertices + 12));
        xport_gte_write_data(4, r_u32(vertices + 16));
        xport_gte_write_data(5, r_u32(vertices + 20));
        if ((sint32)vertices > 0x7FFFFFE7) {
            xport_mips_overflow_exception(0x80036B18);
            return;
        }
        vertices += 24;
        xport_gte_execute(0x4A280030);
        remaining -= 3;
        for (;;) {
            uint32 first_depth;
            uint32 second_depth;
            uint32 third_depth;
            screen += 3;
            depths += 3;
            screen[-3] = xport_gte_read_data(12);
            if (remaining < 0)
                break;
            {
                uint32 first_xy = r_u32(vertices);
                uint32 first_z = r_u32(vertices + 4);
                uint32 second_xy = r_u32(vertices + 8);
                uint32 second_z = r_u32(vertices + 12);
                uint32 third_xy = r_u32(vertices + 16);
                uint32 third_z = r_u32(vertices + 20);
                screen[-3] = xport_gte_read_data(12);
                screen[-2] = xport_gte_read_data(13);
                screen[-1] = xport_gte_read_data(14);
                xport_gte_write_data(0, first_xy);
                xport_gte_write_data(1, first_z);
                xport_gte_write_data(2, second_xy);
                xport_gte_write_data(3, second_z);
                xport_gte_write_data(4, third_xy);
                xport_gte_write_data(5, third_z);
            }
            first_depth = xport_gte_read_data(17);
            second_depth = xport_gte_read_data(18);
            third_depth = xport_gte_read_data(19);
            xport_gte_execute(0x4A280030);
            depths[-3] = (uint32)((sint32)first_depth >> 5);
            depths[-2] = (uint32)((sint32)second_depth >> 5);
            depths[-1] = (uint32)((sint32)third_depth >> 5);
            if ((sint32)vertices > 0x7FFFFFE7) {
                xport_mips_overflow_exception(0x80036B8C);
                return;
            }
            vertices += 24;
            remaining -= 3;
        }
        screen[-2] = xport_gte_read_data(13);
        screen[-1] = xport_gte_read_data(14);
        {
            uint32 first_depth = xport_gte_read_data(17);
            uint32 second_depth = xport_gte_read_data(18);
            uint32 third_depth = xport_gte_read_data(19);
            depths[-3] = (uint32)((sint32)first_depth >> 5);
            depths[-2] = (uint32)((sint32)second_depth >> 5);
            depths[-1] = (uint32)((sint32)third_depth >> 5);
        }
    }
    remaining += 2;
    if (remaining < 0)
        return;
    xport_gte_write_data(0, r_u32(vertices));
    xport_gte_write_data(1, r_u32(vertices + 4));
    if (remaining) {
        uint32 first_depth;
        uint32 second_depth;
        xport_gte_write_data(2, r_u32(vertices + 8));
        xport_gte_write_data(3, r_u32(vertices + 12));
        if ((sint32)vertices > 0x7FFFFFEF) {
            xport_mips_overflow_exception(0x80036BF4);
            return;
        }
        xport_gte_execute(0x4A280030);
        screen[0] = xport_gte_read_data(12);
        screen[1] = xport_gte_read_data(13);
        first_depth = xport_gte_read_data(17);
        second_depth = xport_gte_read_data(18);
        depths[0] = (uint32)((sint32)first_depth >> 5);
        depths[1] = (uint32)((sint32)second_depth >> 5);
    } else {
        if ((sint32)vertices > 0x7FFFFFF7) {
            xport_mips_overflow_exception(0x80036C2C);
            return;
        }
        xport_gte_execute(0x4A180001);
        screen[0] = xport_gte_read_data(14);
        depths[0] = (uint32)((sint32)xport_gte_read_data(19) >> 5);
    }
}

uint32 t3_8004a83c(void)
{
    uint32 group;
    FUNCTION_MARKER(0x8004A83C, "SLUS_004.02");
    w_u16(0x8009F2A0, 0);
    for (group = 0; group < 3; ++group) {
        uint32 base = 0x8009E850 + group * 880;
        uint32 index;
        uint32 row;
        for (index = 0; index < 6; ++index) {
            uint32 record = base + index * 64;
            t3_800766b4(record + 16, 3, 3);
            w_u16(record + 18, 0);
            w_u32(record + 72, 0);
            w_u32(record + 68, 0);
            w_u32(record + 64, 0);
            w_u16(record + 76, 0);
        }
        w_u16(base, 0);
        w_u16(base + 2, r_u16(0x8009E878));
        for (row = 0; row < 2; ++row) {
            for (index = 0; index < 6; ++index) {
                POLY_FT4 *packet = (POLY_FT4 *)psx_addr(base + 400 + row * 240 + index * 40, 40);
                setPolyFT4(packet);
                setSemiTrans(packet, 1);
                setShadeTex(packet, 1);
            }
        }
    }
    return 0;
}

uint32 t3_80030f68(uint32 object, uint32 mode)
{
    uint32 index = (uint32)(sint32)(sint16)(mode - 2);
    FUNCTION_MARKER(0x80030F68, "SLUS_004.02");
    if (index < 24) {
        uint32 target = r_u32(0x80019DF0 + index * 4);
        (void)target;
        switch (index) {
        case 0:
            if (t3_80031028(object))
                mode = 20;
            break;
        case 10:
        case 11:
        case 12:
        case 13:
            if (!r_u8(r_u32(object + 420) + 45))
                mode = 11;
            break;
        case 14:
            mode = 16;
            break;
        case 18:
        case 19:
            if (!t3_80031028(object))
                mode = 11;
            break;
        case 23:
            if (t3_80031028(object))
                mode = 22;
            break;
        default:
            break;
        }
    }
    return (uint32)(sint32)(sint16)mode;
}

uint32 t3_800766b4(uint32 object, uint32 bank, uint32 variant)
{
    uint32 selected = bank;
    uint32 resource;
    uint32 configuration;
    uint32 value;
    sint32 size;
    sint32 divisor;
    sint32 numerator;
    sint32 remainder;
    sint32 quotient;
    sint32 x;
    sint32 y;
    uint32 product;
    FUNCTION_MARKER(0x800766B4, "SLUS_004.02");
    if (bank == 2)
        selected = r_u16(0x800A9128) == 0;
    else if ((sint32)bank >= 3)
        selected = bank - 1;
    resource = r_u32(0x800A35C8 + bank * 16 + variant * 4);
    w_u16(object, 0);
    value = r_u16(resource);
    w_u16(object + 6, 32);
    w_u16(object + 8, 32);
    size = r_s16(object + 6);
    w_u16(object + 4, value);
    configuration = 0x800279EC + selected * 48 + variant * 12;
    divisor = (size < 0 ? size + 3 : size) >> 2;
    w_u16(object + 10, (uint32)divisor);
    numerator = r_s16(configuration + 8);
    quotient = divisor ? numerator / divisor : numerator < 0 ? 1 : -1;
    w_u16(object + 12, (uint32)quotient);
    numerator = r_s16(configuration + 4);
    divisor = r_s16(configuration + 8);
    remainder = divisor ? numerator % divisor : numerator;
    w_u16(object + 14, (uint32)((remainder < 0 ? remainder + 7 : remainder) >> 3));
    size = r_s16(configuration + 10);
    w_u16(object + 16, (uint32)((size < 0 ? size + 31 : size) >> 5));
    numerator = r_s16(configuration + 6);
    divisor = r_s16(configuration + 10);
    remainder = divisor ? numerator % divisor : numerator;
    w_u16(object + 18, (uint32)((remainder < 0 ? remainder + 31 : remainder) >> 5));
    w_u16(object + 62, 1);
    if (bank == 3 && variant == 3)
        w_u16(object + 62, 0);
    value = r_u16(configuration + 4);
    w_u16(object + 20, value);
    w_u16(object + 24, value);
    value = r_u16(configuration + 6);
    x = r_s16(object + 24);
    w_u16(object + 22, value);
    w_u16(object + 26, value);
    divisor = r_s16(configuration + 8);
    quotient = divisor ? x / divisor : x < 0 ? 1 : -1;
    product = (uint32)quotient * (uint32)divisor;
    y = r_s16(object + 26);
    w_u16(object + 28, product);
    divisor = r_s16(configuration + 10);
    quotient = divisor ? y / divisor : y < 0 ? 1 : -1;
    w_u16(object + 30, (uint32)quotient * (uint32)divisor);
    value = r_u16(configuration);
    w_u16(object + 32, value);
    value = r_u16(configuration + 2);
    w_u16(object + 34, value);
    return value;
}

uint32 t3_8002d31c(uint32 object, uint32 mode)
{
    uint32 flags;
    uint32 selection = 1;
    uint32 resource;
    uint32 duration = 1;
    uint32 result;
    FUNCTION_MARKER(0x8002D31C, "SLUS_004.02");
    if (r_u8(object + 30))
        flags = r_u16(0x800ADD6A);
    else
        flags = r_u16(0x800ADD68);
    if (r_u32(0x80095484) == 1) {
        if (r_u32(0x800AFA88) != 7) {
            if (flags & 0x90)
                selection = 0;
            else if (flags & 0x44)
                selection = 2;
            else
                selection = (randPSX() & 1) ? 0 : 2;
        }
    }
    w_u16(object + 416, selection);
    resource = t3_8002d178(object, (uint32)r_s16(object + 416));
    w_u32(object + 84, resource);
    w_u32(object + 76, resource);
    w_u32(object + 420, resource);
    if (r_u32(0x80095484) == 1) {
        uint32 frames = r_u8(r_u32(object + 84) + 24);
        if (frames >= 151)
            duration = frames - 149;
    }
    w_u16(object + 88, 1);
    w_u16(object + 408, 1);
    w_u16(object + 410, t3_80030f68(object, 0));
    w_u16(object + 412, 0);
    w_u16(object + 414, 0);
    result = t3_8002f3ac(object, mode);
    w_u16(object + 80, duration);
    w_u16(object + 88, duration);
    return result;
}

uint32 t3_80045c18(uint32 object)
{
    uint32 actor;
    FUNCTION_MARKER(0x80045C18, "SLUS_004.02");
    if (r_u8(object + 196))
        actor = r_u8(object + 33);
    else if (r_u8(object + 135))
        actor = r_u8(object + 34);
    else if (r_u8(object + 209))
        actor = r_u8(object + 35);
    else
        actor = r_u8(object + 32);
    return 0x800A9228 + actor * 6284;
}

uint32 t3_8002a224(uint32 x, uint32 z)
{
    uint32 magnitude_x;
    uint32 magnitude_z;
    uint32 angle;
    uint32 scaled;
    FUNCTION_MARKER(0x8002A224, "SLUS_004.02");
    magnitude_x = (sint16)x < 0 ? (0 - (x & 0xFFFF)) & 0xFFFF : x & 0xFFFF;
    magnitude_z = (sint16)z < 0 ? (0 - (z & 0xFFFF)) & 0xFFFF : z & 0xFFFF;
    if (magnitude_x == magnitude_z)
        angle = 512;
    else if (magnitude_z < magnitude_x)
        angle = t3_8004b348((magnitude_z << 12) / magnitude_x);
    else
        angle = 1024 - t3_8004b348((magnitude_x << 12) / magnitude_z);
    scaled = (angle & 0xFFFF) << 4;
    if ((sint16)x < 0) {
        if ((sint16)z < 0)
            return (scaled + 0x8000) & 0xFFF0;
        return (0x7FFF - scaled) & 0xFFFF;
    }
    if ((sint16)z < 0)
        return ~scaled & 0xFFFF;
    return scaled & 0xFFF0;
}

uint32 t3_8006465c(uint32 first, uint32 second, uint32 force)
{
    uint32 index;
    uint32 x;
    uint32 second_x;
    uint32 z;
    uint32 second_z;
    FUNCTION_MARKER(0x8006465C, "SLUS_004.02");
    if (r_s16(first + 116) || r_s16(second + 116))
        force = 1;
    for (index = 0; index < 2; ++index) {
        uint32 actor = index ? second : first;
        uint32 position = 0x800ADCA8 + index * 16;
        uint32 old_x = r_u32(position);
        uint32 old_y = r_u32(position + 4);
        uint32 old_z = r_u32(position + 8);
        uint32 old_angle = r_u32(position + 12);
        w_u32(position + 48, old_x);
        w_u32(position + 52, old_y);
        w_u32(position + 56, old_z);
        w_u32(position + 60, old_angle);
        if (r_s16(0x800A8CE0) != (sint32)index || (r_u16(actor + 88) & 1)) {
            if ((r_u32(r_u32(actor + 84) + 36) & 0x20000000) || r_s16(actor + 116) || force) {
                w_u32(position, r_u32(actor + 0xF68));
                w_u32(position + 8, r_u32(actor + 0xF70));
            } else {
                w_u32(position, r_u32(actor));
                w_u32(position + 8, r_u32(actor + 8));
            }
            w_u32(position + 12, (uint32)r_s16(actor + 0x1278));
            w_u32(position + 4, r_u32(actor + 0x994) - 290);
            if (r_s16(actor + 160) == 2 && r_s16(actor + 22) == 4)
                w_u32(position + 4, 0xFFFFF948);
        }
    }
    x = r_u32(0x800ADCA8);
    second_x = r_u32(0x800ADCB8);
    if (x != second_x)
        return second_x;
    z = r_u32(0x800ADCB0);
    second_z = r_u32(0x800ADCC0);
    if (z != second_z)
        return x - 1;
    w_u32(0x800ADCA8, x - 1);
    w_u32(0x800ADCB0, z - 1);
    w_u32(0x800ADCB8, x + 1);
    w_u32(0x800ADCC0, z + 1);
    return z + 1;
}

uint32 t3_80065ed8(uint32 first, uint32 second)
{
    uint32 index;
    uint32 scale;
    FUNCTION_MARKER(0x80065ED8, "SLUS_004.02");
    t3_80063cbc();
    t3_8006465c(first, second, 0);
    t3_8006465c(first, second, 0);
    for (index = 0; index < 5; ++index) {
        uint32 state = 0x800A0328 + index * 48;
        w_u16(state + 4, 0);
        w_u16(state + 6, 0);
        w_u32(state + 36, 0);
        w_u32(state + 44, 0);
        w_u32(state + 40, 0);
        w_u16(state + 2, 0);
        w_u32(state + 16, 0);
        w_u32(state + 12, 0);
        w_u16(state, 0);
        w_u32(state + 32, 0);
        w_u32(state + 24, 0);
        w_u32(state + 20, 0);
        w_u32(state + 28, 0);
        w_u16(state + 8, 0);
    }
    scale = (uint32)r_s16(0x800A8CD4);
    for (index = 0; index < 2; ++index) {
        uint32 camera = 0x800A02A8 + index * 24;
        w_u32(camera, 0);
        w_u32(camera + 4, 0xFFFFFA56);
        w_u32(camera + 8, 0xFFFFE0C0);
        w_u32(camera + 12, 0);
        w_u32(camera + 16, 0);
        w_u32(camera + 20, scale);
    }
    return 0x800A02A8;
}

uint32 t3_8002bd78(uint32 object, uint32 opponent, uint32 reset_position)
{
    uint32 opponent_index;
    uint32 actor;
    uint32 x;
    uint32 z = 0;
    uint32 angle;
    FUNCTION_MARKER(0x8002BD78, "SLUS_004.02");
    opponent_index = r_u8(opponent + 30);
    actor = r_u8(object + 30);
    w_u8(object + 31, opponent_index);
    if (reset_position) {
        x = actor ? 1000 : 0xFFFFFC18;
        angle = actor ? 0xFFFFC000 : 0x4000;
    } else {
        x = r_u32(object);
        z = r_u32(object + 8);
        angle = r_u16(object + 14);
    }
    t3_80042f1c(object);
    t3_8002d31c(object, opponent);
    w_u16(object + 14, angle);
    w_u32(object, x);
    w_u32(object + 4, 0);
    w_u32(object + 8, z);
    w_u16(object + 12, 0);
    w_u16(object + 16, 0);
    w_u16(object + 44, angle);
    w_u16(object + 48, angle);
    w_u16(object + 0x1278, actor ? 100 : 0xFFFFFF9C);
    w_u32(object + 0x994, 0xFFFFFA6F);
    w_u32(object + 0x950, 0xFFFFFBE5);
    w_u32(object + 0xF68, x);
    w_u32(object + 0xF70, z);
    w_u32(object + 0x990, x);
    w_u32(object + 0x998, z);
    w_u32(object + 0x94C, x);
    w_u32(object + 0x954, z);
    w_u32(object + 1000, 0);
    w_u32(object + 1004, 0);
    w_u32(object + 1008, 0);
    if (r_u8(0x800AFA91)) {
        uint32 saved = r_u32(object + 1020);
        w_u32(object + 1012, saved);
        if (!saved || r_s32(object + 1016) < (sint32)saved)
            w_u32(object + 1012, r_u32(object + 1016));
    } else {
        w_u32(object + 1012, r_u32(object + 1016));
    }
    w_u16(object + 106, 0xFFFF);
    w_u16(object + 1024, 0);
    w_u16(object + 118, 0);
    w_u8(object + 130, 0);
    w_u16(object + 146, 0);
    w_u16(object + 148, 0);
    w_u16(object + 154, 0);
    w_u16(object + 156, 0);
    w_u16(object + 158, 0);
    w_u8(object + 181, 0);
    w_u8(object + 135, 0);
    w_u8(object + 137, 0);
    w_u8(object + 138, 0);
    w_u8(object + 183, 0);
    w_u8(object + 140, 0);
    w_u8(object + 186, 0);
    w_u8(object + 187, 0);
    w_u8(object + 189, 0);
    w_u8(object + 184, 0);
    w_u16(object + 70, 0);
    w_u16(object + 72, 0);
    w_u16(object + 260, 0);
    w_u16(object + 262, 0);
    w_u16(object + 266, 0);
    w_u16(object + 276, 0);
    w_u16(object + 278, 0);
    w_u16(object + 280, 0);
    w_u16(object + 282, 0);
    w_u16(object + 292, 0);
    w_u16(object + 294, 0);
    w_u16(object + 290, 0);
    w_u16(object + 296, 0);
    w_u16(object + 204, 0);
    w_u8(object + 211, 0);
    w_u8(object + 212, 0);
    w_u8(object + 207, 0);
    w_u8(object + 208, 0);
    w_u8(object + 222, 0);
    w_u16(object + 232, 0);
    w_u8(object + 219, 0);
    w_u16(object + 236, 0);
    w_u8(object + 190, 0);
    w_u8(object + 191, 0);
    w_u8(object + 192, 0);
    w_u8(object + 193, 0);
    w_u8(object + 194, 0);
    w_u8(object + 195, 1);
    w_u8(object + 196, 0);
    w_u8(object + 201, 0);
    w_u8(object + 199, 0);
    w_u32(object + 6256, 0);
    w_u32(object + 6264, 0);
    w_u16(object + 6254, 0);
    w_u16(object + 6248, 0);
    w_u16(object + 6250, 0);
    w_u16(object + 6252, 0);
    t3_80044008(object);
    w_u32(object + 404, object + 316);
    t3_80041998(object);
    t3_8003f044(object);
    t3_800346fc(object, 10);
    return t3_800409e8(object);
}

uint32 t3_80068394(uint32 advance, uint32 first, uint32 second)
{
    uint32 value;
    sint32 remainder;
    uint32 duration;
    FUNCTION_MARKER(0x80068394, "SLUS_004.02");
    if (advance) {
        sint32 restricted = r_s16(0x800A8CDC);
        do {
            value = r_u32(0x8009854C) + 1;
            w_u32(0x8009854C, value);
            remainder = (sint32)value % 10;
            w_u16(0x800A04C2, (uint32)remainder);
        } while (restricted && (sint16)remainder >= 8);
    } else {
        value = r_u32(0x8009854C);
        remainder = (sint32)value % 10;
        w_u16(0x800A04C2, (uint32)remainder);
    }
    duration = r_u32(0x80095484) == 1 ? 120 : 53;
    w_u16(0x800A04C0, duration);
    w_u16(0x800A8C76, duration);
    t3_80066084(first, second, 1, 5517);
    return t3_80063fd4((uint32 *)psx_addr(0x800A04A8, 24), 0);
}

uint32 t3_8004eab4(void)
{
    sint32 wins;
    uint32 side;
    uint32 descriptor;
    uint32 y;
    uint32 format;
    FUNCTION_MARKER(0x8004EAB4, "SLUS_004.02");
    wins = r_s32(0x80097D3C);
    if (!wins)
        return 0x80090000;
    side = r_u32(0x80097D38) != 0;
    if (wins < 2)
        descriptor = 0x80021F4C;
    else if (wins < 10)
        descriptor = 0x80021F54;
    else if (wins < 100)
        descriptor = 0x80021F5C;
    else
        descriptor = 0x80021F64;
    y = r_u16(descriptor + side * 2 + 4);
    format = r_u32(descriptor);
    return t3_8004ce74(format, (uint32)1, (uint32)8, y, (uint32)432, (uint32)wins);
}

uint32 t3_8004ed10(void)
{
    uint32 actor;
    FUNCTION_MARKER(0x8004ED10, "SLUS_004.02");
    for (actor = 0; actor < 2; ++actor) {
        uint32 mode = r_u32(0x800AFA88);
        uint32 value = 0;
        if (mode < 9) {
            uint32 target = r_u32(0x80022098 + mode * 4);
            (void)target;
        }
        switch (mode) {
        case 0:
            value = t3_bns_00_800b1ca8(0x800AFA88, actor);
            break;
        case 2:
            value = t3_bns_00_800b1d8c(0x800AFA88, actor);
            break;
        case 3:
            value = t3_bns_00_800b1e08(0x800AFA88, actor);
            break;
        case 4:
            value = t3_bns_00_800b1e48(0x800AFA88, actor);
            break;
        case 6:
            value = 1;
            break;
        case 7:
            value = t3_bns_03_800b4d4c(0x800AFA88, actor);
            break;
        case 8:
            value = 10;
            break;
        default:
            break;
        }
        t3_8004eb54(0x800AFA88, actor, value);
    }
    return 0;
}

uint32 t3_80062a10(void)
{
    uint32 index;
    uint32 mode;
    FUNCTION_MARKER(0x80062A10, "SLUS_004.02");
    w_u32(0x800A85C0, 0);
    w_u32(0x800A85C4, 0);
    w_u32(0x800A85C8, 0);
    w_u16(0x800A85D0, 0);
    w_u16(0x800A85CE, 0);
    w_u16(0x800A85CC, 0);
    w_u32(0x800A85D4, 0);
    w_u32(0x800A85D8, 0xFFFFFB50);
    w_u32(0x800A85DC, 0);
    w_u32(0x800A85E0, 0);
    for (index = 0; index < 5; ++index) {
        w_u16(0x800A8C82 + index * 2, 0);
        w_u16(0x800A8C78 + index * 2, 0);
    }
    w_u16(0x800A8C78, 4096);
    w_u16(0x800A8C70, 1);
    w_u16(0x800A8C92, 0xFFFF);
    w_u16(0x800A8C98, 0xFFFF);
    w_u16(0x800A8C96, 0xFFFF);
    w_u16(0x800A8C94, 0xFFFF);
    w_u16(0x800A8C9E, 0xFFFF);
    w_u16(0x800A8C9C, 0xFFFF);
    w_u16(0x800A8CE0, 0xFFFF);
    w_u16(0x800A8CDE, 0);
    w_u16(0x800A8C74, 1);
    w_u16(0x800A8C76, 0);
    t3_80067cc4();
    mode = r_u32(0x800AFA88);
    w_u16(0x800A8C8C, mode < 6 || mode >= 9);
    return mode < 9;
}

uint32 t3_8004ad34(void)
{
    uint32 stream;
    sint32 delta;
    uint32 offset;
    uint32 value;
    FUNCTION_MARKER(0x8004AD34, "SLUS_004.02");
    stream = r_u32(0x80097AA0);
    if (!stream)
        return 0;
    delta = r_s8(stream);
    w_u32(0x80097AA0, stream + 1);
    if (delta == -128) {
        w_u32(0x80097AA0, 0);
        delta = 0;
    }
    w_u32(0x800A85C0, r_u32(0x800A85C0) + (uint32)delta);
    for (offset = 0; offset < 32; offset += 16) {
        uint32 first = r_u32(0x800A85C0 + offset);
        uint32 second = r_u32(0x800A85C4 + offset);
        uint32 third = r_u32(0x800A85C8 + offset);
        uint32 fourth = r_u32(0x800A85CC + offset);
        w_u32(0x800A85C0 + offset, first);
        w_u32(0x800A85C4 + offset, second);
        w_u32(0x800A85C8 + offset, third);
        w_u32(0x800A85CC + offset, fourth);
    }
    value = r_u32(0x800A85E0);
    w_u32(0x800A85E0, value);
    return value;
}
