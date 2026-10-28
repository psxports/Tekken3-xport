#include "game_functions.h"

uint32 t3_8003368c(void)
{
    uint32 frame;
    uint32 base;
    uint32 bank;
    uint32 record;
    uint32 kind;
    FUNCTION_MARKER(0x8003368C, "SLUS_004.02");
    frame = r_u32(0x8009BCBC);
    if (frame & 1)
        return 1;
    frame = (uint32)((sint32)frame >> 1);
    base = r_u32(0x800ADE68);
    bank = r_u32(0x8009BCAC);
    record = base ? base + bank * 1200 + 0x7A48 + frame * 4 : 0;
    kind = r_u8(record + 2);
    if (kind == 1)
        return t3_80040bc0(r_u16(record));
    if (kind == 2) {
        uint32 actor = r_u8(record + 3);
        uint32 value = r_u16(record);
        sint32 character = r_s16(0x800A9242 + actor * 6284);
        return t3_80040c20(actor, (uint32)character, value);
    }
    if (kind == 3) {
        uint32 actor = r_u8(record + 3);
        uint32 value = r_u16(record);
        return t3_80040da0(actor, value);
    }
    return 3;
}

uint32 t3_800566e8(uint32 limit)
{
    uint32 random;
    uint32 sequence;
    uint32 product;
    FUNCTION_MARKER(0x800566E8, "SLUS_004.02");
    random = (uint32)randPSX();
    sequence = r_u32(0x800ADCA0);
    product = limit * ((random + sequence) & 0x7FFF);
    w_u32(0x800ADCA0, sequence * 5 + 3);
    return (uint32)((sint32)product / 32768);
}

uint32 t3_8006953c(uint32 first, uint32 second)
{
    uint32 flag;
    uint32 second_x;
    uint32 first_x;
    uint32 second_z;
    uint32 first_z;
    uint32 angle;
    uint32 random;
    uint32 product;
    uint32 pitch;
    uint32 yaw;
    FUNCTION_MARKER(0x8006953C, "SLUS_004.02");
    t3_80062a10();
    t3_80065ed8(first, second);
    w_u16(0x800A0490, 0xFFFF);
    w_u8(0x800A04CD, 1);
    w_u16(0x800A8C78, 0);
    w_u16(0x800A8C7A, 4096);
    w_u16(0x800A8C7C, 0);
    flag = r_u8(0x800A04CC);
    w_u8(0x800A04CE, 1);
    w_u32(0x800A04C8, 11034);
    w_u16(0x800A04D2, 0);
    second_x = r_u32(0x800ADCB8);
    first_x = r_u32(0x800ADCA8);
    second_z = r_u32(0x800ADCC0);
    first_z = r_u32(0x800ADCB0);
    w_u8(0x800A04CC, (uint8)((flag + 1) & 1));
    angle = t3_8004b39c(second_x - first_x, second_z - first_z);
    w_u16(0x800A04D0, (uint16)((uint32)(sint32)(sint16)angle << 6));
    random = (uint32)randPSX();
    if (random & 1)
        r_u16(0x800A04D0);
    if (!r_u8(0x800A04CC)) {
        random = t3_800566e8(0xE38E);
        angle = r_u16(0x800A04D0);
        w_u16(0x800A04D0, (uint16)(angle - 29127 + random));
        random = t3_800566e8(30);
        product = (random + 25) << 18;
        pitch = (uint32)((sint32)product / 360);
        w_u16(0x800A04D6, (uint16)pitch);
        random = t3_800566e8(30);
        product = (random + 25) << 18;
        yaw = 0 - (uint32)((sint32)product / 360);
        w_u16(0x800A04D8, (uint16)yaw);
        if (randPSX() & 1) {
            pitch = r_u16(0x800A04D6);
            yaw = r_u16(0x800A04D8);
            w_u16(0x800A04D6, (uint16)(0 - pitch));
            w_u16(0x800A04D8, (uint16)(0 - yaw));
        }
    } else {
        random = t3_800566e8(0x15555);
        angle = r_u16(0x800A04D0);
        w_u16(0x800A04D8, 0);
        w_u16(0x800A04D6, 0);
        w_u16(0x800A04D0, (uint16)(angle + 21846 + random));
    }
    return t3_80069414(first, second);
}

uint32 t3_80040bc0(uint32 value)
{
    uint32 sound = (uint32)(sint32)(sint16)value;
    FUNCTION_MARKER(0x80040BC0, "SLUS_004.02");
    t3_8003352c(1, 0, sound);
    return t3_80040c00(sound);
}

uint32 t3_80040c20(uint32 actor, uint32 character, uint32 value)
{
    sint32 sound = (sint16)value;
    FUNCTION_MARKER(0x80040C20, "SLUS_004.02");
    if (!t3_80075914(actor, (uint32)(sound >> 12)))
        t3_8003352c(2, actor, (uint32)sound);
    return t3_80040c90(actor, character, (uint32)sound);
}

uint32 t3_80040da0(uint32 actor, uint32 value)
{
    uint32 signed_actor = (uint32)(sint32)(sint16)actor;
    uint32 signed_value = (uint32)(sint32)(sint16)value;
    FUNCTION_MARKER(0x80040DA0, "SLUS_004.02");
    t3_8003352c(3, signed_actor, signed_value);
    return t3_80040df4(signed_actor, signed_value);
}

uint32 t3_8003352c(uint32 kind, uint32 actor, uint32 value)
{
    uint32 frame;
    uint32 count;
    uint32 slot;
    uint32 base;
    uint32 destination = 0;
    FUNCTION_MARKER(0x8003352C, "SLUS_004.02");
    if (r_u32(0x8009BCA8) != 7)
        return 7;
    frame = r_u32(0x8009BCB8);
    count = r_u32(0x8009BCC4);
    slot = frame + count;
    if ((sint32)slot < 0)
        slot += 0x4B00;
    w_u32(0x8009BCC4, count + 1);
    slot = (uint32)((sint32)slot % 300);
    base = r_u32(0x800ADE68);
    if (base) {
        slot <<= 2;
        destination = base + 0x7A48 + slot;
    }
    w_u8(destination + 2, (uint8)kind);
    w_u8(destination + 3, (uint8)actor);
    w_u16(destination, (uint16)value);
    return slot;
}

uint32 t3_80040c90(uint32 actor, uint32 character, uint32 value)
{
    uint32 table;
    uint32 kind;
    uint32 group;
    uint32 sound;
    uint32 offset = 0;
    uint32 index;
    uint32 entry;
    uint32 flag;
    FUNCTION_MARKER(0x80040C90, "SLUS_004.02");
    table = r_u32(0x8001AD54 + character * 24);
    kind = (value & 0xFFFF) >> 12;
    group = kind - 2;
    if (group >= 4)
        return 0;
    sound = value & 0xFFF;
    if (r_s16(table + group * 2) - 1 < (sint32)sound)
        return 1;
    for (index = 0; index < group; ++index)
        offset += (uint32)r_s16(table + index * 2);
    entry = table + (sound + offset) * 2;
    t3_800753bc(actor, (uint32)r_s16(entry + 8), 0);
    if (kind == 4) {
        t3_8006f208(actor, (uint32)r_s16(entry + 8));
        return t3_8004b770();
    }
    flag = r_u32(0x800954A4);
    if (flag)
        return t3_8006f29c(actor, (uint32)r_s16(entry + 8));
    return flag;
}

uint32 t3_80040df4(uint32 actor, uint32 value)
{
    uint32 kind = (uint32)((sint32)(value << 16) >> 28);
    uint32 sound = value & 0xFFF;
    uint32 entry;
    uint32 flag;
    FUNCTION_MARKER(0x80040DF4, "SLUS_004.02");
    if (!sound)
        return kind;
    entry = 0x80097050 + sound * 2;
    actor = (uint32)(sint32)(sint16)actor;
    if (!kind)
        return t3_800755a4(actor, r_u16(entry));
    t3_800753bc(actor, r_u16(entry), 0);
    flag = r_u32(0x800954A4);
    if (flag)
        return t3_8006f29c(actor, r_u16(entry));
    return flag;
}

uint32 t3_800755a4(uint32 actor, uint32 sound)
{
    uint32 voice;
    FUNCTION_MARKER(0x800755A4, "SLUS_004.02");
    if ((sint32)actor >= 2)
        actor = 1;
    if (!(sound & 0xC000))
        return 0;
    voice = (sound & 0x3C00) >> 10;
    if (voice >= 6)
        voice += actor;
    return t3_80088938((sint16)voice);
}

uint32 t3_80075600(uint32 value)
{
    uint32 offset = (uint32)((sint32)(value << 16) >> 15);
    uint32 sound;
    uint32 bank;
    uint32 voice;
    uint32 volume;
    FUNCTION_MARKER(0x80075600, "SLUS_004.02");
    sound = r_u16(0x80097050 + offset);
    bank = sound & 0xC000;
    if (!bank)
        return 0;
    bank = (bank >> 14) - 1;
    voice = (sound >> 10) & 15;
    volume = r_u8(0x800278FC + ((sound >> 6) & 15));
    t3_80088938((sint32)voice);
    return t3_800885d8((sint32)voice, (sint16)bank, (sound & 48) >> 4, sound & 15, (sound & 15) + 60, 0, volume, volume);
}

uint32 t3_8006f208(uint32 actor, uint32 sound)
{
    uint32 flag;
    uint32 event;
    FUNCTION_MARKER(0x8006F208, "SLUS_004.02");
    flag = r_u8(0x800A92EC + actor * 6284);
    if (flag)
        return flag;
    event = t3_8006f4f4();
    if (event == 0xFFFFFFFF)
        return 17;
    w_u8(event + 12, 17);
    sound &= 0xFFFF;
    w_u32(event + 64, actor);
    w_u32(event + 68, sound);
    return sound;
}

uint32 t3_8006f29c(uint32 actor, uint32 sound)
{
    uint32 event;
    FUNCTION_MARKER(0x8006F29C, "SLUS_004.02");
    event = t3_8006f4f4();
    if (event == 0xFFFFFFFF)
        return 18;
    w_u8(event + 12, 18);
    sound &= 0xFFFF;
    w_u32(event + 64, actor);
    w_u32(event + 68, sound);
    return sound;
}

uint32 t3_8003b4f8(uint32 actor)
{
    uint32 owner;
    uint32 packet;
    uint32 model;
    uint32 ordering;
    FUNCTION_MARKER(0x8003B4F8, "SLUS_004.02");
    owner = r_u32(0x800A8C54);
    owner = r_u32(owner + 4);
    packet = r_u32(owner + 0xFBC) & 0xFFFFFF;
    if (r_u32(0x800AFA88) == 8) {
        ordering = r_u32(0x800ADEFC);
        packet = t3_800793b8(actor, ordering, packet);
    } else {
        model = r_u32(actor + 0x1274);
        ordering = r_u32(0x800ADEFC);
        packet = t3_80037b28(model, ordering, packet);
    }
    t3_80037924(actor);
    if (r_u32(0x800ADFD0)) {
        owner = r_u32(0x800A8C54);
        owner = r_u32(owner + 4);
        w_u32(owner + 0xFBC, packet);
    }
    return t3_8003172c(actor);
}

uint32 t3_8003b9b4(uint32 matrix)
{
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    uint32 fifth;
    FUNCTION_MARKER(0x8003B9B4, "SLUS_004.02");
    first = r_u32(matrix);
    second = r_u32(matrix + 4);
    third = r_u32(matrix + 8);
    fourth = r_u32(matrix + 12);
    fifth = r_u32(matrix + 16);
    xport_gte_write_control(0, (first & 0xFFFF) | (second & 0xFFFF0000));
    xport_gte_write_control(1, (fourth & 0xFFFF) | (first & 0xFFFF0000));
    xport_gte_write_control(2, (third & 0xFFFF) | (fourth & 0xFFFF0000));
    xport_gte_write_control(3, (second & 0xFFFF) | (third & 0xFFFF0000));
    xport_gte_write_control(4, fifth);
    return matrix;
}

uint32 t3_80037a9c(uint32 model, uint32 output, uint32 count)
{
    uint32 source;
    uint32 next_x0;
    uint32 next_z0;
    uint32 next_x1;
    uint32 next_z1;
    uint32 next_x2;
    uint32 next_z2;
    FUNCTION_MARKER(0x80037A9C, "SLUS_004.02");
    source = r_u32(model);
    xport_gte_write_data(0, r_u32(source + 4));
    xport_gte_write_data(1, r_u32(source + 8));
    xport_gte_write_data(2, r_u32(source + 12));
    xport_gte_write_data(3, r_u32(source + 16));
    xport_gte_write_data(4, r_u32(source + 20));
    xport_gte_write_data(5, r_u32(source + 24));
    xport_gte_execute(0x4A280030);
    source += 28;
    for (;;) {
        if ((sint64)(sint32)count - 3 < -2147483648LL) {
            xport_mips_overflow_exception(0x80037AE4);
            return output;
        }
        count -= 3;
        next_x0 = r_u32(source);
        next_z0 = r_u32(source + 4);
        next_x1 = r_u32(source + 8);
        if ((sint64)(sint32)output + 12 > 0x7FFFFFFF) {
            xport_mips_overflow_exception(0x80037AF8);
            return output;
        }
        output += 12;
        if ((sint32)count > 0) {
            next_z1 = r_u32(source + 12);
            next_x2 = r_u32(source + 16);
            next_z2 = r_u32(source + 20);
        }
        if ((sint64)(sint32)source + 24 > 0x7FFFFFFF) {
            xport_mips_overflow_exception(0x80037B08);
            return output;
        }
        source += 24;
        w_u32(output - 12, xport_gte_read_data(12));
        w_u32(output - 8, xport_gte_read_data(13));
        w_u32(output - 4, xport_gte_read_data(14));
        if ((sint32)count <= 0)
            return output;
        xport_gte_write_data(0, next_x0);
        xport_gte_write_data(1, next_z0);
        xport_gte_write_data(2, next_x1);
        xport_gte_write_data(3, next_z1);
        xport_gte_write_data(4, next_x2);
        xport_gte_write_data(5, next_z2);
        xport_gte_execute(0x4A280030);
    }
}

uint32 t3_80037b28(uint32 model, uint32 ordering, uint32 packet)
{
    uint32 source;
    uint32 destination;
    uint32 count;
    uint32 indices;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    sint64 table_address;
    FUNCTION_MARKER(0x80037B28, "SLUS_004.02");
    source = r_u32(model + 8);
    ordering <<= 2;
    table_address = (sint64)(sint32)ordering + (sint32)model;
    if (table_address > 0x7FFFFFFF || table_address < -2147483648LL) {
        xport_mips_overflow_exception(0x80037B30);
        return packet;
    }
    destination = r_u32((uint32)table_address);
    count = r_u32(source);
    source += 4;
    w_u32(destination, packet | 0x04000000);
    do {
        indices = r_u32(source);
        --count;
        first = r_u32(0x1F800200 + (indices & 0x3FC));
        second = r_u32(0x1F800200 + ((indices >> 8) & 0x3FC));
        third = r_u32(0x1F800200 + ((indices >> 18) & 0x3FC));
        w_u32(destination + 8, first);
        w_u32(destination + 12, second);
        w_u32(destination + 16, third);
        destination += 20;
        source += 4;
    } while (count);
    count = r_u32(source);
    source += 4;
    do {
        indices = r_u32(source);
        --count;
        first = r_u32(0x1F800200 + ((indices << 2) & 0x3FC));
        second = r_u32(0x1F800200 + ((indices >> 6) & 0x3FC));
        third = r_u32(0x1F800200 + ((indices >> 14) & 0x3FC));
        fourth = r_u32(0x1F800200 + ((indices >> 22) & 0x3FC));
        w_u32(destination + 8, first);
        w_u32(destination + 12, second);
        w_u32(destination + 16, third);
        w_u32(destination + 20, fourth);
        destination += 24;
        source += 4;
    } while (count);
    return (destination - 24) & 0xFFFFFF;
}

uint32 t3_80037924(uint32 actor)
{
    MATRIX lighting;
    uint32 phase;
    uint32 side;
    uint32 row;
    uint32 record = actor;
    uint32 bone;
    uint32 matrix;
    uint32 light;
    uint32 ordering;
    uint32 geometry;
    uint32 chain;
    uint32 output;
    uint32 result;
    FUNCTION_MARKER(0x80037924, "SLUS_004.02");
    phase = r_u8(0x8009E550);
    side = (uint32)r_s16(actor + 18);
    phase = (phase ^ (phase >> 1) ^ side) & 1;
    for (row = 0; row < 22; ++row, record += 40) {
        result = r_u32(record + 0x4F0);
        bone = r_u8(0x8001A074 + row);
        if (!result)
            continue;
        light = t3_80039e38();
        matrix = actor + bone * 68 + 0x8F4;
        MulMatrix0((MATRIX *)psx_addr(light, 32), (MATRIX *)psx_addr(matrix, 32), &lighting);
        SetLightMatrix(&lighting);
        matrix += 32;
        SetRotMatrix((MATRIX *)psx_addr(matrix, 32));
        SetTransMatrix((MATRIX *)psx_addr(matrix, 32));
        ordering = r_u32(0x800ADEFC);
        geometry = r_u32(record + 0x4F8);
        chain = r_u32(0x800A8C54);
        output = r_u32(record + ordering * 4 + 0x508);
        chain = r_u32(chain + 4);
        t3_80036cac(geometry, output, chain, phase);
        geometry = r_u32(record + 0x4FC);
        if (geometry) {
            ordering = r_u32(0x800ADEFC);
            chain = r_u32(0x800A8C54);
            output = r_u32(record + ordering * 4 + 0x510);
            chain = r_u32(chain + 4);
            t3_80036cac(geometry, output, chain, phase);
        }
        if (bone == 11) {
            result = t3_80036aa8(0, 0, 0, actor + 0x1278);
            w_u16(actor + 0x127A, (uint16)result);
        }
    }
    return 0;
}

uint32 t3_800293bc(const PSX_RECT *rectangle, sint32 x, sint32 y)
{
    uint32 count;
    uint32 destination;
    uint16 component;
    FUNCTION_MARKER(0x800293BC, "SLUS_004.02");
    count = r_u32(0x8009BC54);
    if ((sint32)count >= 32)
        return 1;
    destination = r_u32(0x8009BC50);
    w_u16(destination, 2);
    component = (uint16)rectangle->x;
    w_u16(destination + 4, component);
    component = (uint16)rectangle->y;
    w_u16(destination + 6, component);
    component = (uint16)rectangle->w;
    w_u32(0x8009BC54, count + 1);
    w_u16(destination + 8, component);
    component = (uint16)rectangle->h;
    w_u32(0x8009BC50, destination + 16);
    w_u16(destination + 12, (uint16)x);
    w_u16(destination + 14, (uint16)y);
    w_u16(destination + 10, component);
    return 0;
}

uint32 t3_80034844(uint32 animation, uint32 selection, uint32 actor)
{
    PSX_RECT rectangle;
    uint32 table;
    uint32 value;
    uint32 index;
    uint32 entry;
    uint32 dimensions;
    uint32 destination;
    uint32 source;
    uint32 old_selection;
    sint32 destination_y;
    sint32 destination_x;
    sint32 side;
    uint32 source_y;
    FUNCTION_MARKER(0x80034844, "SLUS_004.02");
    table = r_u32(animation + 4);
    if (table) {
        value = r_u32(table + selection * 4);
        if (value)
            w_u32(animation, value);
    }
    if (r_u8(0x800B040C))
        return 17;
    if (r_s16(actor + 24) != 17)
        return 17;
    index = (uint32)r_s16(actor + 28);
    old_selection = r_u8(actor + 0x1291);
    index <<= 2;
    entry = 0x8009591C + index;
    if (old_selection == selection)
        return index;
    w_u8(actor + 0x1291, (uint8)selection);
    dimensions = 0x8009586C + (uint32)r_s8(entry) * 4;
    rectangle.w = (sint16)r_u16(dimensions);
    rectangle.h = (sint16)r_u16(dimensions + 2);
    destination = 0x8009579C + (uint32)r_s8(entry + 1) * 4;
    source = 0x8009579C + (uint32)r_s8(entry + ((selection - 4 < 2) ? 3 : 2)) * 4;
    rectangle.x = (sint16)r_u16(source);
    source_y = r_u16(source + 2);
    rectangle.y = (sint16)source_y;
    destination_y = r_s16(destination + 2);
    side = r_s16(actor + 18);
    destination_x = r_s16(destination);
    if (side) {
        rectangle.y = (sint16)(source_y + 4);
        destination_y += 4;
    }
    return t3_800293bc(&rectangle, destination_x, destination_y);
}

uint32 t3_8003ba28(uint32 point, uint32 origin, uint32 translation, uint32 output)
{
    uint32 delta[3];
    uint32 origin_x;
    uint32 origin_y;
    uint32 origin_z;
    uint32 high[3];
    uint32 low[3];
    uint32 translation_x;
    uint32 translation_y;
    uint32 translation_z;
    uint32 index;
    uint32 magnitude;
    uint32 result_x;
    uint32 result_y;
    uint32 result_z;
    FUNCTION_MARKER(0x8003BA28, "SLUS_004.02");
    delta[0] = (uint32)r_s16(point);
    delta[1] = (uint32)r_s16(point + 2);
    delta[2] = (uint32)r_s16(point + 4);
    origin_x = r_u32(origin);
    origin_y = r_u32(origin + 4);
    origin_z = r_u32(origin + 8);
    delta[0] -= origin_x;
    delta[1] -= origin_y;
    delta[2] -= origin_z;
    for (index = 0; index < 3; ++index) {
        if ((sint32)delta[index] < 0) {
            magnitude = 0 - delta[index];
            high[index] = 0 - (uint32)((sint32)magnitude >> 15);
            low[index] = 0 - (magnitude & 0x7FFF);
        } else {
            high[index] = (uint32)((sint32)delta[index] >> 15);
            low[index] = delta[index] & 0x7FFF;
        }
    }
    xport_gte_write_data(9, high[0]);
    xport_gte_write_data(10, high[1]);
    xport_gte_write_data(11, high[2]);
    translation_x = r_u32(translation);
    xport_gte_execute(0x4A41E012);
    translation_y = r_u32(translation + 4);
    translation_z = r_u32(translation + 8);
    high[0] = xport_gte_read_data(25);
    high[1] = xport_gte_read_data(26);
    high[2] = xport_gte_read_data(27);
    xport_gte_write_data(9, low[0]);
    xport_gte_write_data(10, low[1]);
    xport_gte_write_data(11, low[2]);
    xport_gte_execute(0x4A49E012);
    high[0] <<= 3;
    high[1] <<= 3;
    high[2] <<= 3;
    result_x = xport_gte_read_data(25);
    result_y = xport_gte_read_data(26);
    result_z = xport_gte_read_data(27);
    result_x += high[0] + translation_x;
    result_y += high[1] + translation_y;
    result_z += high[2] + translation_z;
    w_u32(output, result_x);
    w_u32(output + 4, result_y);
    w_u32(output + 8, result_z);
    return origin;
}

uint32 t3_8003bb88(MATRIX *matrix, uint32 sine, uint32 cosine)
{
    uint32 first[3];
    uint32 second[3];
    uint32 rotated_first[3];
    uint32 rotated_second[3];
    uint32 row;
    uint32 product;
    FUNCTION_MARKER(0x8003BB88, "SLUS_004.02");
    sine = (uint32)(sint32)(sint16)sine;
    cosine = (uint32)(sint32)(sint16)cosine;
    for (row = 0; row < 3; ++row) {
        first[row] = (uint32)(sint32)matrix->m[row][0];
        second[row] = (uint32)(sint32)matrix->m[row][1];
        product = first[row] * cosine + second[row] * sine;
        rotated_first[row] = (uint32)((sint32)product >> 12);
        product = (0 - first[row]) * sine + second[row] * cosine;
        rotated_second[row] = (uint32)((sint32)product >> 12);
    }
    matrix->m[0][0] = (sint16)rotated_first[0];
    matrix->m[1][0] = (sint16)rotated_first[1];
    matrix->m[0][1] = (sint16)rotated_second[0];
    matrix->m[1][1] = (sint16)rotated_second[1];
    matrix->m[2][0] = (sint16)rotated_first[2];
    matrix->m[2][1] = (sint16)rotated_second[2];
    return rotated_second[2];
}

uint32 t3_8003b5c0(uint32 left, uint32 right, uint32 output)
{
    uint32 rotation[5];
    uint32 vector_xy;
    uint32 vector_z;
    uint32 packed;
    uint32 first_column[3];
    uint32 second_column[3];
    uint32 third_x;
    uint32 third_y;
    uint32 translation[3];
    uint32 origin[3];
    uint32 index;
    sint64 sum;
    FUNCTION_MARKER(0x8003B5C0, "SLUS_004.02");
    for (index = 0; index < 5; ++index)
        rotation[index] = r_u32(left + index * 4);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, rotation[index]);
    vector_xy = r_u16(right);
    packed = r_u32(right + 4);
    vector_z = r_u32(right + 12);
    vector_xy |= packed & 0xFFFF0000;
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    vector_xy = r_u16(right + 2);
    packed = r_u32(right + 8);
    vector_z = (uint32)r_s16(right + 14);
    vector_xy |= packed << 16;
    first_column[0] = xport_gte_read_data(9);
    first_column[1] = xport_gte_read_data(10);
    first_column[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    vector_xy = r_u16(right + 4);
    packed = r_u32(right + 8);
    vector_z = r_u32(right + 16);
    vector_xy |= packed & 0xFFFF0000;
    second_column[0] = xport_gte_read_data(9);
    second_column[1] = xport_gte_read_data(10);
    second_column[2] = xport_gte_read_data(11);
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    w_u32(output, (second_column[0] << 16) | (first_column[0] & 0xFFFF));
    w_u32(output + 12, (second_column[2] << 16) | (first_column[2] & 0xFFFF));
    third_x = xport_gte_read_data(9);
    third_y = xport_gte_read_data(10);
    w_u32(output + 16, xport_gte_read_data(11));
    vector_xy = r_u16(right + 20);
    packed = r_u32(right + 24);
    vector_z = r_u32(right + 28);
    vector_xy |= packed << 16;
    xport_gte_write_data(0, vector_xy);
    xport_gte_write_data(1, vector_z);
    xport_gte_execute(0x4A486012);
    w_u32(output + 4, (first_column[1] << 16) | (third_x & 0xFFFF));
    w_u32(output + 8, (third_y << 16) | (second_column[1] & 0xFFFF));
    translation[0] = xport_gte_read_data(25);
    translation[1] = xport_gte_read_data(26);
    translation[2] = xport_gte_read_data(27);
    origin[0] = r_u32(left + 20);
    origin[1] = r_u32(left + 24);
    origin[2] = r_u32(left + 28);
    for (index = 0; index < 3; ++index) {
        sum = (sint64)(sint32)translation[index] + (sint32)origin[index];
        if (sum > 0x7FFFFFFF || sum < -2147483648LL) {
            xport_mips_overflow_exception(0x8003B6FC + index * 4);
            return output;
        }
        translation[index] = (uint32)sum;
    }
    w_u32(output + 20, translation[0]);
    w_u32(output + 24, translation[1]);
    w_u32(output + 28, translation[2]);
    return output;
}

uint32 t3_80043018(uint32 actor)
{
    uint32 opponent;
    uint32 mode;
    uint32 flag;
    uint32 first;
    uint32 second;
    uint32 value;
    FUNCTION_MARKER(0x80043018, "SLUS_004.02");
    opponent = t3_80045c18(actor);
    mode = r_u32(0x800AFA88);
    if (mode - 7 >= 2)
        return 0;
    flag = r_u8(actor + 196);
    if (!flag)
        return flag;
    flag = r_u8(opponent + 196);
    if (!flag)
        return flag;
    if (r_s16(actor + 116) >= 0)
        return 8;
    if (mode == 8 && r_s16(actor + 92) >= 19) {
        first = r_u8(actor + 30);
        second = r_u8(opponent + 30);
        if (t3_800439f0(first, second) >= 0x579)
            w_u32(0x80097A5C, 0);
    }
    flag = r_u32(0x80097A5C);
    if (!flag)
        return flag;
    value = r_u32(opponent);
    w_u32(actor, value);
    value = r_u32(opponent + 8);
    w_u32(actor + 8, value);
    return value;
}

uint32 t3_80042f40(void)
{
    uint32 index;
    uint32 value;
    uint32 step;
    uint32 destination;
    uint32 speed;
    FUNCTION_MARKER(0x80042F40, "SLUS_004.02");
    for (index = 0; index < 3; ++index) {
        destination = 0x80097A44 + index * 4;
        speed = 0x80097A50 + index * 4;
        value = r_u32(destination);
        if ((sint32)value < 2000) {
            step = r_u32(speed);
            w_u32(destination, value + step);
            step = r_u32(speed);
            w_u32(speed, step + 2);
        }
        value = r_u32(destination);
        if ((sint32)value >= 2001) {
            w_u32(destination, 2000);
            w_u32(speed, 2);
        }
    }
    return 0;
}

uint32 t3_80075ef0(uint32 actor)
{
    uint32 side;
    uint32 controller;
    FUNCTION_MARKER(0x80075EF0, "SLUS_004.02");
    if (r_u32(0x800AFA88) != 8)
        return (uint32)r_s16(0x800A923A + actor * 6284);
    side = r_u8(0x800A9246 + actor * 6284);
    controller = r_u32(0x800B6BF4);
    return side ? (controller + 1) & 1 : controller;
}

uint32 t3_800439f0(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x800439F0, "SLUS_004.02");
    if ((sint32)first >= 2)
        first = 1u << ((first - 1) & 31);
    if ((sint32)second >= 2)
        second = 1u << ((second - 1) & 31);
    return r_u32(0x8009E660 + ((first + second) << 4));
}

uint32 t3_80043ca8(void)
{
    uint32 count;
    uint32 row;
    uint32 actor;
    uint32 opponent;
    FUNCTION_MARKER(0x80043CA8, "SLUS_004.02");
    count = r_u32(0x800AFA88) - 7 < 2 ? 3 : 2;
    for (row = 0; row < count; ++row) {
        actor = 0x800A9228 + row * 6284;
        w_u8(actor + 0xD5, 0);
        w_u8(actor + 0xD9, 0);
        w_u8(actor + 0xD8, 0);
        w_u8(actor + 0xD7, 0);
        w_u8(actor + 0xD6, 0);
    }
    for (row = 0; row < count; ++row) {
        actor = 0x800A9228 + row * 6284;
        if (r_s16(actor + 0x74)) {
            opponent = r_u8(actor + 0x21);
            w_u8(actor + 0xD9, 1);
            w_u8(0x800A9301 + opponent * 6284, 1);
        }
    }
    return 0;
}

uint32 t3_80045104(void)
{
    uint32 first;
    uint32 second;
    uint32 opponent;
    FUNCTION_MARKER(0x80045104, "SLUS_004.02");
    if (r_u32(0x800AFA4C) & 1) {
        first = 0x800A9228;
        second = 0x800AAAB4;
    } else {
        first = 0x800AAAB4;
        second = 0x800A9228;
    }
    opponent = t3_80045c18(first);
    t3_8002d8b4(first, opponent);
    opponent = t3_80045c18(second);
    t3_8002d8b4(second, opponent);
    if (r_u32(0x800AFA88) != 8)
        return 8;
    opponent = t3_80045c18(0x800AC340);
    return t3_8002d8b4(0x800AC340, opponent);
}

uint32 t3_800451b8(uint32 initial_return_sequence)
{
    uint32 first;
    uint32 second;
    uint32 flag;
    FUNCTION_MARKER(0x800451B8, "SLUS_004.02");
    if (r_u32(0x800AFA4C) & 1) {
        first = 0x800A9228;
        second = 0x800AAAB4;
    } else {
        first = 0x800AAAB4;
        second = 0x800A9228;
    }
    t3_80045248(first, initial_return_sequence);
    t3_80045248(second, initial_return_sequence);
    if (r_u32(0x800AFA88) != 8)
        return 0x800B0000;
    flag = r_u32(0x800B6C04);
    if (flag)
        return flag;
    return t3_80045248(0x800AC340, initial_return_sequence);
}

uint32 t3_800449b4(uint32 actor)
{
    uint32 kind;
    uint32 value;
    uint32 result = 0;
    sint32 active;
    FUNCTION_MARKER(0x800449B4, "SLUS_004.02");
    kind = r_u8(actor + 0xE4);
    if (kind == 2) {
        active = r_s16(actor + 0xEC);
        value = r_u16(actor + 0xEC);
        if (!active)
            value = r_u16(actor + 0x5E);
        result = (uint32)(sint32)(sint16)value;
        kind = r_u8(actor + 0xE4);
    }
    if (kind == 3) {
        active = r_s16(actor + 0xEC);
        value = r_u16(actor + 0xEC);
        if (!active)
            value = r_u16(actor + 0x5E);
        result -= (uint32)(sint32)(sint16)value;
    }
    if (r_s16(actor + 0x74) < 0) {
        actor = t3_80045c18(actor);
        kind = r_u8(actor + 0xE4);
        if (kind == 1) {
            active = r_s16(actor + 0xEC);
            value = r_u16(actor + 0xEC);
            if (!active)
                value = r_u16(actor + 0x5E);
            result += (uint32)(sint32)(sint16)value;
        }
    }
    return result;
}

uint32 t3_8002d8b4(uint32 actor, uint32 opponent)
{
    uint32 value;
    uint32 animation;
    uint32 command;
    uint32 frame;
    uint32 sequence;
    uint32 duration;
    FUNCTION_MARKER(0x8002D8B4, "SLUS_004.02");
    if (r_s16(opponent + 24) == 17)
        return 17;
    value = r_u8(actor + 0xCE);
    w_u16(actor + 0xEA, 0);
    if (!value)
        return value;
    animation = r_u32(actor + 0x54);
    frame = (uint32)r_s16(actor + 0x58);
    sequence = r_u32(animation + 12);
    command = t3_8002e708(sequence, frame, actor, opponent, opponent);
    value = r_u16(command);
    if (value == 0xC000)
        return 0xC000;
    value = r_u16(command + 6);
    w_u16(actor + 0x1A0, (uint16)value);
    value = t3_8002d178(actor, (uint32)(sint32)(sint16)value);
    w_u32(actor + 0x1A4, value);
    value = r_u8(command + 11);
    frame = r_u16(actor + 0x58);
    w_u16(actor + 0x198, (uint16)value);
    value = r_u8(command + 10);
    w_u8(actor + 0x87, 0);
    w_u8(actor + 0xCE, 0);
    w_u8(actor + 0xCF, 0);
    w_u8(actor + 0xD0, 0);
    w_u16(actor + 0x104, 0);
    w_u16(actor + 0x6C, (uint16)(value - frame));
    w_u8(opponent + 0xD1, 0);
    w_u8(opponent + 0x83, 0);
    w_u8(opponent + 0x84, 0);
    w_u8(opponent + 0x85, 0);
    w_u8(opponent + 0x88, 0);
    value = r_u8(command + 8);
    value = t3_80030f68(actor, value & 0x3F);
    w_u16(actor + 0x19A, (uint16)value);
    w_u16(actor + 0x19C, 0);
    w_u16(actor + 0x19E, 0);
    w_u8(actor + 0x189, 0);
    w_u8(actor + 0x15D, 0);
    duration = r_u16(opponent + 0x5E);
    value = (uint32)((sint32)(sint16)duration / 2 + 25);
    w_u16(actor + 0xEA, (uint16)value);
    return value;
}

uint32 t3_80045248(uint32 actor, uint32 initial_return_sequence)
{
    uint32 opponent;
    uint32 first;
    uint32 second;
    uint32 distance;
    uint32 special = 0;
    uint32 kind;
    uint32 active;
    uint32 record;
    uint32 key;
    uint32 expected;
    uint32 matched;
    sint32 character;
    sint32 height;
    FUNCTION_MARKER(0x80045248, "SLUS_004.02");
    opponent = t3_80045c18(actor);
    t3_80043a94(actor, opponent);
    first = r_u8(actor + 30);
    second = r_u8(opponent + 30);
    if (first >= 2)
        first = 1u << ((first - 1) & 31);
    if (second >= 2)
        second = 1u << ((second - 1) & 31);
    distance = r_u32(0x8009E660 + ((first + second) << 4));
    character = r_s16(actor + 24);
    w_u32(actor + 0xF8, distance);
    if (character == 11 || character == 20)
        special = 1;
    character = r_s16(opponent + 24);
    if (character == 11 || character == 20)
        ++special;
    active = r_u8(actor + 0xCE);
    w_u32(actor + 0xF4, distance - special * 40);
    if (active) {
        kind = r_u8(opponent + 0xB7);
        if (kind == 0 || (kind == 2 && !r_u8(actor + 0xDB) && !(r_u32(actor + 0x60) & 4))) {
            t3_8002eba4(actor, opponent);
            height = r_s16(actor + 0x74);
            w_u8(actor + 0x8B, 1);
            if (height > 0)
                w_u8(actor + 0xB7, 1);
        }
    }
    if (r_u32(0x800AFA88) == 7) {
        first = r_u8(actor + 30);
        second = r_u32(0x800B6684);
        if (second == first + 1) {
            t3_8002d178(actor, (uint32)r_s16(actor + 0x1A0));
            w_u8(actor + 0xDB, 1);
            w_u8(actor + 0xCF, 0);
            w_u8(actor + 0xD0, 1);
            t3_80043a94(actor, 0x800AC340);
            t3_8002eba4(actor, opponent);
            w_u8(actor + 0xB7, 1);
        }
    }
    if (!r_u32(actor + 0x1A4))
        t3_8002d9e8(actor, opponent, initial_return_sequence);
    kind = r_u8(0x800AFA93);
    if (!r_u32(actor + 0x3F4) && r_u8(actor + 0xD0)) {
        record = t3_8002d178(actor, (uint32)r_s16(actor + 0x1A0));
        key = r_u16(record + 16);
        expected = t3_8002d204();
        if (!kind) {
            matched = (key << 16) == (expected << 16);
            if (!matched) {
                expected = t3_8002d20c();
                matched = (sint16)key == (sint16)expected;
            }
            if (matched) {
                w_u8(actor + 0xDB, 1);
                w_u8(actor + 0xCF, 0);
                w_u8(actor + 0xD0, 1);
                w_u16(actor + 0x66, 0);
                t3_8002eba4(actor, opponent);
                w_u8(actor + 0xB7, 1);
            }
        } else {
            matched = key == (uint32)(sint32)(sint16)expected;
            if (!matched) {
                key = r_u16(record + 16);
                expected = t3_8002d20c();
                matched = key == (uint32)(sint32)(sint16)expected;
            }
            if (matched || (!(r_u32(record + 4) & 4) && !r_u8(record + 25))) {
                t3_8002d82c(actor, opponent);
                w_u8(actor + 0xB7, 1);
            }
        }
    }
    if (r_s16(actor + 0x19A) != 33)
        return 33;
    first = r_u8(opponent + 30);
    second = r_u8(actor + 30);
    w_u8(actor + 33, (uint8)first);
    w_u8(opponent + 33, (uint8)second);
    w_u8(opponent + 196, 1);
    w_u8(actor + 196, 1);
    return 1;
}

uint32 t3_8002d82c(uint32 actor, uint32 opponent)
{
    uint32 value;
    FUNCTION_MARKER(0x8002D82C, "SLUS_004.02");
    w_u16(actor + 0x1A0, 3430);
    value = t3_8002d178(actor, 3430);
    w_u32(actor + 0x1A4, value);
    w_u16(actor + 0x58, 1);
    w_u16(actor + 0x198, 1);
    value = t3_80030f68(actor, 6);
    w_u16(actor + 0x19A, (uint16)value);
    w_u16(actor + 0x19C, 0);
    w_u16(actor + 0x19E, 0);
    w_u8(actor + 0xCF, 0);
    w_u8(actor + 0xD0, 1);
    w_u16(actor + 0x66, 0);
    return t3_8002f3ac(actor, opponent);
}

uint32 t3_8002d9e8(uint32 actor, uint32 opponent, uint32 initial_return_sequence)
{
    uint32 value;
    uint32 animation;
    uint32 sequence;
    uint32 command;
    uint32 following;
    uint32 record;
    uint32 frame_bits;
    uint32 flags;
    uint32 saved_angle;
    uint32 angle_delta;
    uint32 start;
    uint32 limit;
    uint32 result;
    sint32 frame;
    sint32 direction;
    sint32 next_frame;
    FUNCTION_MARKER(0x8002D9E8, "SLUS_004.02");
    value = r_u32(actor + 0x1A4);
    w_u16(actor + 0x6C, 0xFFFF);
    if (value)
        return 0xFFFFFFFF;
    if (r_s16(actor + 0x128) >= 26)
        return 0;
    animation = r_u32(actor + 0x54);
    frame = r_s16(actor + 0x58);
    sequence = r_u32(animation + 12);
    command = t3_8002de24(sequence, (uint32)frame, actor, opponent, initial_return_sequence);
    if (r_u16(command) != 0xC000) {
        value = r_u16(command + 6);
        w_u16(actor + 0x1A0, (uint16)value);
        value = t3_8002d178(actor, (uint32)(sint32)(sint16)value);
        w_u32(actor + 0x1A4, value);
        value = r_u8(command + 10);
        frame_bits = r_u16(actor + 0x58);
        w_u16(actor + 0x6C, (uint16)(value - frame_bits));
        value = r_u8(command + 8);
        value = t3_80030f68(actor, value & 0x3F);
        w_u16(actor + 0x19A, (uint16)value);
        flags = r_u8(command + 8);
        w_u16(actor + 0x19C, (uint16)(flags >> 7));
        flags = r_u8(command + 8);
        direction = r_s16(actor + 0x6E);
        w_u16(actor + 0x19E, (uint16)((flags >> 6) & 1));
        value = r_u8(command + (direction < 0 ? 10 : 11));
        w_u16(actor + 0x198, (uint16)value);
        if (r_s16(actor + 0x198) == 255) {
            animation = r_u32(actor + 0x54);
            value = r_u8(animation + 24);
            w_u16(actor + 0x198, (uint16)value);
        }
        w_u8(actor + 0x8B, 2);
        return 2;
    }
    start = r_u8(command + 9);
    frame = r_s16(actor + 0x58);
    if (frame >= (sint32)start - 5) {
        if (frame >= (sint32)start)
            goto check_window;
        if (r_s16(actor + 0x6E) > 0) {
            value = r_u32(actor + 0x40C);
            if (r_u8(actor + value + 0x41C) & 0xF0)
                goto try_transition;
        }
    }
    frame = r_s16(actor + 0x58);
    start = r_u8(command + 9);
    if (frame < (sint32)start)
        goto check_mode;
    frame = r_s16(actor + 0x58);
check_window:
    limit = r_u8(command + 10);
    if ((sint32)limit >= frame && r_s16(actor + 0x6E) > 0)
        goto try_transition;
check_mode:
    if (r_u32(0x800AFA88) != 7)
        goto fallback;
    record = 0x800B6678 + (uint32)r_s16(actor + 18) * 6;
    value = r_u8(record + 5);
    if (!value)
        goto fallback;
    frame = r_s16(actor + 0x58);
    value = r_u8(record + 5);
    if (frame < (sint32)value || r_s16(actor + 0x6E) <= 0)
        goto fallback;
try_transition:
    record = t3_8002d178(actor, (uint32)r_s16(command + 6));
    animation = r_u32(actor + 0x54);
    saved_angle = r_u16(actor + 0x2C);
    angle_delta = r_u16(animation + 18);
    sequence = r_u32(record + 12);
    w_u16(actor + 0x2C, (uint16)(saved_angle + angle_delta));
    t3_80043a94(actor, opponent);
    following = t3_8002de24(sequence, 1, actor, opponent, sequence);
    w_u16(actor + 0x2C, (uint16)saved_angle);
    t3_80043a94(actor, opponent);
    if (r_u16(following) != 0xC000) {
        value = r_u16(following + 6);
        w_u16(actor + 0x1A0, (uint16)value);
        value = t3_8002d178(actor, (uint32)(sint32)(sint16)value);
        w_u32(actor + 0x1A4, value);
        value = r_u8(command + 9);
        w_u16(actor + 0x198, (uint16)value);
        flags = r_u8(command + 8);
        frame_bits = r_u16(actor + 0x58);
        w_u16(actor + 0x19E, (uint16)((flags >> 6) & 1));
        value = r_u8(following + 10);
        w_u16(actor + 0x6C, (uint16)(value - frame_bits));
        value = r_u8(following + 8);
        value = t3_80030f68(actor, value & 0x3F);
        w_u16(actor + 0x19A, (uint16)value);
        flags = r_u8(following + 8);
        w_u8(actor + 0xC1, 1);
        w_u8(actor + 0x8B, 3);
        w_u16(actor + 0x19C, (uint16)(flags >> 7));
        return 3;
    }
fallback:
    frame = r_s16(actor + 0x58);
    direction = r_s16(actor + 0x6E);
    animation = r_u32(actor + 0x54);
    next_frame = frame + direction;
    limit = r_u8(animation + 24);
    frame_bits = r_u16(actor + 0x58);
    result = (sint32)limit < next_frame;
    if (!result && next_frame > 0)
        return result;
    w_u16(actor + 0x198, (uint16)frame_bits);
    value = r_u16(command + 6);
    w_u16(actor + 0x1A0, (uint16)value);
    value = t3_8002d178(actor, (uint32)(sint32)(sint16)value);
    w_u32(actor + 0x1A4, value);
    value = r_u8(command + 8);
    value = t3_80030f68(actor, value & 0x3F);
    w_u16(actor + 0x19A, (uint16)value);
    flags = r_u8(command + 8);
    w_u16(actor + 0x19C, (uint16)(flags >> 7));
    flags = r_u8(command + 8);
    w_u8(actor + 0xC1, 1);
    w_u8(actor + 0x8B, 4);
    w_u16(actor + 0x19E, (uint16)((flags >> 6) & 1));
    return 4;
}

uint32 t3_8004a614(void)
{
    uint32 cursor;
    uint32 base;
    uint32 value;
    FUNCTION_MARKER(0x8004A614, "SLUS_004.02");
    value = r_u32(0x8009E830);
    if (!value)
        return value;
    cursor = r_u32(0x8009E834);
    base = r_u32(0x8009E844);
    cursor += 8;
    w_u32(0x8009E834, cursor);
    if (((sint32)(cursor - base) >> 3) >= 300) {
        w_u32(0x8009E834, base);
        w_u32(0x8009E83C, 0);
    }
    cursor = r_u32(0x8009E834);
    value = r_u32(0x8009E840);
    w_u32(cursor + 4, 0);
    w_u32(cursor, value);
    return value;
}

uint32 t3_8002eba4(uint32 actor, uint32 opponent)
{
    uint32 action;
    uint32 offset;
    uint32 value;
    uint32 flags;
    uint32 table;
    uint32 frame;
    sint32 quadrant;
    FUNCTION_MARKER(0x8002EBA4, "SLUS_004.02");
    if (r_u8(actor + 0xCF)) {
        action = 35;
        offset = 20;
        goto select_pair;
    }
    value = r_u8(actor + 0xDB);
    if (!value) {
        table = r_u32(actor + 0x54);
        value = r_u32(table + 4) & 0x400;
    }
    if (value) {
        value = t3_80030f68(actor, 36);
        w_u16(actor + 0x19A, (uint16)value);
        t3_8002ee94(actor, opponent);
        goto finalize;
    }
    flags = r_u32(actor + 0x60);
    if (flags & 0x200) {
        value = t3_80030f68(actor, 37);
        table = r_u32(actor + 0x1A8);
        w_u16(actor + 0x19A, (uint16)value);
        value = r_u16(table + 26);
        w_u16(actor + 0x1A0, (uint16)value);
        goto finalize;
    }
    if (flags & 4) {
        value = t3_80030f68(actor, 37);
        table = r_u32(actor + 0x1A8);
        w_u16(actor + 0x19A, (uint16)value);
        value = r_u16(table + 24);
        w_u16(actor + 0x1A0, (uint16)value);
        goto finalize;
    }
    if (r_u8(actor + 0xD3)) {
        action = 38;
        offset = 4;
        goto select_pair;
    }
    quadrant = r_s16(actor + 50);
    if (quadrant == 0) {
        action = 42;
        offset = 0;
    } else if (quadrant == 1) {
        action = 39;
        offset = 8;
    } else if (quadrant == 2) {
        action = 43;
        offset = 16;
    } else if (quadrant == 3) {
        action = 40;
        offset = 12;
    } else {
        goto finalize;
    }
select_pair:
    value = t3_80030f68(actor, action);
    flags = r_u32(actor + 0x60);
    w_u16(actor + 0x19A, (uint16)value);
    table = r_u32(actor + 0x1A8);
    value = r_u16(table + offset + ((flags & 2) ? 0 : 2));
    w_u16(actor + 0x1A0, (uint16)value);
finalize:
    action = (uint32)r_s16(actor + 0x1A0);
    frame = r_u16(actor + 0x58);
    w_u16(actor + 0x19C, 0);
    w_u16(actor + 0x19E, 0);
    w_u16(actor + 0x198, (uint16)frame);
    value = t3_8002d178(actor, action);
    w_u32(actor + 0x1A4, value);
    return value;
}

uint32 t3_8002ee94(uint32 actor, uint32 opponent)
{
    sint32 action;
    sint32 character;
    sint32 quadrant;
    sint32 speed;
    uint32 active;
    uint32 reaction = 0;
    uint32 pose;
    uint32 first_index;
    uint32 second_index;
    uint32 command;
    uint32 value;
    FUNCTION_MARKER(0x8002EE94, "SLUS_004.02");
    action = r_s16(actor + 0xA0);
    active = action != 0xD62;
    if (action == 0xD63 && r_s16(actor + 0x58) >= 19)
        active = 0;
    action = r_s16(actor + 0xA0);
    if (action == 0xD64 && r_s16(actor + 0x58) >= 19)
        active = 0;
    if (active) {
        character = r_s16(opponent + 22);
        if (character == 9)
            reaction = r_s16(opponent + 0xA0) == 0x241;
        if (character == 16 && r_s16(opponent + 0xA0) == 0x14A) {
            value = r_u32(opponent + 0x1A4);
            reaction = 1;
            if (value && r_u16(opponent + 0x1A0) - 0x848u < 2)
                reaction = 2;
        }
        pose = 1;
        if (reaction != 1 && (reaction == 2 || r_s32(actor + 0x3E0) < 110)) {
            if (r_s16(actor + 0xA0) == 0xD58) {
                pose = 0;
            } else {
                quadrant = r_s16(actor + 50);
                pose = quadrant == 1 ? 4 : (quadrant == 3 ? 3 : 0);
            }
        }
    } else {
        action = r_s16(actor + 0xA0);
        pose = 2;
        if (action != 0xD62) {
            speed = r_s32(actor + 0xF6C);
            if (speed >= 0) {
                speed = r_s16(actor + 0xF6C);
            } else {
                value = r_u16(actor + 0xF6C);
                speed = (sint16)(0u - value);
            }
            if (speed >= 1024)
                pose = 0;
        }
    }
    w_u16(actor + 0x90, (uint16)pose);
    first_index = (uint32)r_s16(actor + 0x90);
    second_index = (uint32)r_s16(actor + 0x90);
    command = r_u16(0x8001A632 + first_index * 4);
    value = r_u16(0x8001A630 + second_index * 4);
    w_u16(actor + 0x1A0, (uint16)command);
    w_u16(actor + 0x96, (uint16)value);
    return value;
}

uint32 t3_800448e0(uint32 actor, uint32 event, uint32 value)
{
    uint32 flag;
    uint32 minimum;
    uint32 side;
    sint32 signed_minimum;
    FUNCTION_MARKER(0x800448E0, "SLUS_004.02");
    flag = r_u8(event + 36);
    if (!flag)
        flag = r_u8(actor + 0xDE);
    if (flag) {
        signed_minimum = r_s16(event + 28);
        minimum = r_u16(event + 28);
        if ((sint16)value < signed_minimum)
            value = minimum;
        if (r_u32(0x800AFA88) == 7) {
            side = r_u8(actor + 30);
            return t3_80076c28((uint32)(sint32)(sint16)value, actor, 1 - side, (const uint32 *)psx_addr(event, 22));
        }
        side = r_u8(event + 32);
        return t3_80076c28((uint32)(sint32)(sint16)value, actor, side, (const uint32 *)psx_addr(event, 22));
    }
    if (r_u8(event + 34))
        return t3_80076bd0(2, (const uint32 *)psx_addr(event, 22));
    flag = r_u8(event + 40);
    if (flag)
        return t3_80076bd0(1, (const uint32 *)psx_addr(event, 22));
    return flag;
}

uint32 t3_80044a8c(uint32 actor, uint32 opponent, uint32 flags)
{
    uint32 animation;
    uint32 index;
    uint32 value;
    uint32 first;
    uint32 second;
    uint32 active = 0;
    sint32 limit;
    sint32 frame;
    FUNCTION_MARKER(0x80044A8C, "SLUS_004.02");
    if (!r_s16(opponent + 0x5E)) {
        w_u8(flags + 40, 1);
        return 1;
    }
    animation = r_u32(opponent + 0x54);
    index = r_u16(animation + 52);
    if (index) {
        limit = r_s16(0x800177DE + index * 4);
        value = r_u32(0x800AFA3C);
        if (value < (uint32)limit)
            w_u8(flags + 38, 1);
    }
    first = r_u16(actor + 0x66);
    second = r_u16(opponent + 0x64);
    if ((first & second) && r_u16(actor + 0x30) < 0x4000) {
        w_u8(flags + 34, 1);
        value = r_u16(0x800AFA58);
        if (!value)
            value = (uint32)r_s16(opponent + 0x122);
        if (value)
            w_u8(flags + 35, 1);
        return value;
    }
    w_u8(flags + 36, 1);
    if (r_s16(actor + 0x5E)) {
        animation = r_u32(actor + 0x54);
        frame = r_s16(actor + 0x58);
        value = r_u8(animation + 45);
        active = frame < (sint32)value;
    }
    if (!active)
        active = r_s16(actor + 0x122) != 0;
    if (!active)
        active = r_s16(opponent + 0x122) != 0;
    if (active)
        w_u8(flags + 37, 1);
    value = r_u8(actor + 0xDB);
    if (!value) {
        animation = r_u32(actor + 0x54);
        value = r_u32(animation + 4) & 0x400;
    }
    if (value)
        w_u8(flags + 39, 1);
    return 1;
}

uint32 t3_80031348(uint32 actor)
{
    uint32 action;
    uint32 record;
    uint32 value;
    uint32 duration;
    FUNCTION_MARKER(0x80031348, "SLUS_004.02");
    action = r_u16(actor + 0x19A);
    if (action - 39 >= 2 && (sint16)action != 43)
        return (uint32)(sint32)(sint16)action;
    if (r_s16(actor + 0x126)) {
        if (r_s16(actor + 0x13A) == (sint16)action) {
            value = r_u16(actor + 0x124);
            w_u16(actor + 0x124, (uint16)(value + 1));
        }
    } else {
        w_u16(actor + 0x124, 0);
    }
    record = r_u32(actor + 0x1A4);
    action = r_u16(actor + 0x19A);
    duration = r_u8(record + 24);
    w_u16(actor + 0x13A, (uint16)action);
    duration += 10;
    w_u16(actor + 0x126, (uint16)duration);
    return duration;
}

uint32 t3_80028ec0(uint32 side, uint32 value)
{
    uint32 index;
    uint32 destination;
    FUNCTION_MARKER(0x80028EC0, "SLUS_004.02");
    index = r_u8(0x80010050 + value);
    if ((sint32)side >= 2) {
        t3_80028e30(0, value, index);
        t3_80028e30(1, value, index);
        return 0;
    }
    if (index >= 4)
        return 0x800A0000;
    destination = 0x8009BA40 + side * 8 + index * 2;
    w_u16(destination, (uint16)value);
    return destination;
}

uint32 t3_80028e30(uint32 side, uint32 value, uint32 index)
{
    uint32 destination;
    FUNCTION_MARKER(0x80028E30, "SLUS_004.02");
    if ((sint32)side >= 2) {
        t3_80028e30(0, value, index);
        t3_80028e30(1, value, index);
        return 0;
    }
    if ((sint32)index >= 4)
        return 0x800A0000;
    destination = 0x8009BA40 + side * 8 + index * 2;
    w_u16(destination, (uint16)value);
    return destination;
}

uint32 t3_80039f0c(const uint32 *position, uint32 color)
{
    uint32 value;
    FUNCTION_MARKER(0x80039F0C, "SLUS_004.02");
    value = position[0];
    w_u32(0x8009E5E0, value);
    value = position[1];
    w_u32(0x8009E5E4, value);
    value = position[2];
    w_u32(0x8009E5E8, value);
    value = r_u8(color);
    w_u16(0x8009E5C2, (uint16)(value << 4));
    value = r_u8(color + 1);
    w_u16(0x8009E5C8, (uint16)(value << 4));
    value = r_u8(color + 2);
    w_u16(0x8009E5CE, (uint16)(value << 4));
    SetColorMatrix((MATRIX *)psx_addr(0x8009E5C0, 32));
    w_u32(0x8009E5EC, 48);
    return 48;
}

uint32 t3_80040904(uint32 first, uint32 second, uint32 limit)
{
    sint32 x = (sint16)first;
    sint32 y = (sint16)second;
    sint32 magnitude;
    uint32 result;
    FUNCTION_MARKER(0x80040904, "SLUS_004.02");
    if (y < 0)
        y = -y;
    magnitude = x < 0 ? -x : x;
    if ((sint32)limit >= y + magnitude)
        return 0;
    result = (uint32)(sint32)(sint16)(limit - (uint32)y);
    return x < 0 ? 0 - result : result;
}

uint32 t3_800572a4(uint32 state)
{
    sint32 count;
    sint32 index;
    uint32 selected = 0;
    uint32 entry;
    uint32 action;
    uint32 command;
    uint32 flags;
    uint32 attributes;
    uint32 kind;
    uint32 high;
    uint32 selector;
    uint32 mask;
    uint32 eligible;
    uint32 total;
    FUNCTION_MARKER(0x800572A4, "SLUS_004.02");
    count = r_s16(state + 0x70);
    if (count <= 0)
        return 0;
    for (index = 0; index < count; ++index) {
        entry = 0x8009F978 + (uint32)index * 12;
        if (r_s16(entry + 8))
            continue;
        action = r_u32(entry);
        command = r_u32(entry + 4);
        flags = r_u32(action + 4);
        kind = r_u8(action + 25);
        attributes = r_u32(action + 8);
        eligible = 0;
        if (!kind || (flags & 0x80000) || (attributes & 0x200000)) {
            if (r_u16(command) == 0xC001 || (flags & 0x80000)) {
                eligible = 1;
            } else if (r_u8(action + 45) && !(attributes & 0x400000) && !(flags & 0x20000)) {
                high = r_u32(action + 8);
                selector = r_u32(state + 20);
                high &= 0xFFFF0000;
                if (!(high & 0x1C0000))
                    high |= 0xC0000;
                mask = r_u32(0x80098230 + selector * 4);
                eligible = (high & mask) != 0 || (flags & 0x10000) != 0;
            }
        }
        if (eligible) {
            w_u16(entry + 8, 1);
            ++selected;
        }
    }
    total = r_u16(state + 0x72);
    w_u16(state + 0x72, (uint16)(total + selected));
    return selected;
}

uint32 t3_80058958(uint32 state, uint32 command)
{
    uint32 actor;
    uint32 owner;
    uint32 action;
    uint32 index;
    uint32 table;
    uint32 value;
    uint32 sequence;
    uint32 base;
    uint32 adjustment;
    uint32 parameters;
    uint32 record;
    uint32 side;
    sint32 previous;
    sint32 direction;
    FUNCTION_MARKER(0x80058958, "SLUS_004.02");
    actor = r_u32(0x800AFA5C);
    owner = r_u8(actor + 0x1886);
    index = r_u16(command + 6);
    table = r_u32(0x8009F2E8 + owner * 4);
    action = r_u32(table + index * 4);
    if (r_u32(action + 8) & 0x400000) {
        value = r_u16(state + 0x96);
        w_u16(state + 0x96, (uint16)(value + 1));
    }
    value = r_u8(action + 45);
    if (!value)
        return value;
    sequence = r_u32(0x800ADCA0);
    base = (uint32)r_s16(state + 0x23C);
    adjustment = (uint32)r_s16(state + 0x23E + (sequence & 3) * 2);
    w_u32(0x800ADCA0, sequence * 5 + 3);
    w_u16(state + 0x68, (uint16)(base + adjustment));
    if (r_u32(0x800AFA88) == 8) {
        actor = r_u32(0x800AFA5C);
        parameters = r_u32(state + 0x32C);
        side = r_u8(actor + 30);
        record = 0x8009F2F8 + (2 - side) * 4;
        value = (uint32)r_s16(parameters);
        previous = r_s32(record + 16);
        w_u32(record + 8, value);
        if (previous < 0) {
            parameters = r_u32(state + 0x32C);
            value = (uint32)r_s16(parameters + 2);
            w_u32(record + 16, value);
        }
    }
    direction = r_s16(state + 0x46);
    w_u16(state + 0x44, 0xFFFF);
    if (direction > 0)
        w_u16(state + 0x46, 0xFFFF);
    w_u16(state + 0x3A, 0);
    w_u16(state + 0x38, 0);
    w_u16(state + 0x40, 40);
    w_u16(state + 0x48, 0xFFFF);
    return 40;
}

uint32 t3_8006a230(uint32 output, const uint32 *delta, const uint32 *origin)
{
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 dx;
    uint32 dy;
    uint32 dz;
    uint32 enabled;
    uint32 pivot_x;
    uint32 pivot_z;
    uint32 sine;
    uint32 cosine;
    uint32 offset_x;
    uint32 offset_z;
    uint32 product;
    uint32 rotated_x;
    uint32 rotated_z;
    uint32 first;
    uint32 second;
    uint32 result;
    FUNCTION_MARKER(0x8006A230, "SLUS_004.02");
    x = origin[0];
    z = origin[2];
    dx = delta[0];
    dy = delta[1];
    dz = delta[2];
    enabled = r_u32(0x800A05C8);
    y = origin[1];
    if (enabled) {
        pivot_x = r_u32(0x800A05CC);
        cosine = r_u32(0x800A05C4);
        offset_x = pivot_x - x;
        pivot_z = r_u32(0x800A05D4);
        sine = r_u32(0x800A05C0);
        offset_z = pivot_z - z;
        product = offset_x * cosine - offset_z * sine;
        rotated_x = (uint32)((sint32)product / 4096);
        product = offset_x * sine + offset_z * cosine;
        rotated_z = (uint32)((sint32)product / 4096);
        x = pivot_x - rotated_x;
        z = pivot_z - rotated_z;
    }
    cosine = r_u32(0x800A05BC);
    product = dx * cosine;
    first = (uint32)((sint32)product / 4096);
    sine = r_u32(0x800A05B8);
    product = dz * sine;
    second = (uint32)((sint32)product / 4096);
    w_u32(output, x + first - second);
    w_u32(output + 4, y + dy);
    sine = r_u32(0x800A05B8);
    product = dx * sine;
    first = (uint32)((sint32)product / 4096);
    cosine = r_u32(0x800A05BC);
    product = dz * cosine;
    second = (uint32)((sint32)product / 4096);
    result = z + first + second;
    w_u32(output + 8, result);
    return result;
}

uint32 t3_8006183c(uint32 output, uint32 data)
{
    uint32 header;
    uint32 flags;
    uint32 value;
    FUNCTION_MARKER(0x8006183C, "SLUS_004.02");
    header = r_u16(data);
    data += 2;
    if (r_s16(data))
        w_u32(output + 8, data);
    else
        w_u32(output + 8, 0);
    flags = header & 0x100 ? 128 : 0;
    if (header & 0x200)
        flags |= 16;
    if (header & 0x400)
        flags |= 64;
    if (header & 0x800)
        flags |= 32;
    value = r_u16(0x8002301C + (header & 15) * 2) | flags;
    w_u16(output + 4, (uint16)value);
    return value;
}

uint32 t3_80067afc(uint32 index, uint32 *distance, uint32 *angle)
{
    uint32 random;
    uint32 table;
    uint32 value;
    sint32 mode;
    FUNCTION_MARKER(0x80067AFC, "SLUS_004.02");
    random = (uint32)randPSX();
    table = 0x80023F18 + index * 20;
    value = (uint32)r_s16(table + (random & 3) * 2 + 4) << 5;
    *distance = value;
    random = (uint32)randPSX();
    value = (uint32)r_s16(table + (random & 3) * 2 + 12) << 6;
    *angle = value;
    if ((sint32)*distance < 0 && r_s16(0x800A8CDC) == 2)
        *distance = 0;
    mode = r_s16(0x800A8CDC);
    if (!mode)
        return 0;
    value = (uint32)((sint32)*distance / 2);
    *distance = value;
    return value;
}

uint32 t3_800661b8(uint32 first, uint32 second)
{
    uint32 second_x;
    uint32 first_x;
    uint32 second_z;
    uint32 first_z;
    sint32 first_height;
    sint32 second_height;
    uint32 angle;
    uint32 distance;
    uint32 adjustment;
    uint32 index;
    FUNCTION_MARKER(0x800661B8, "SLUS_004.02");
    second_x = r_u32(0x800ADCB8);
    first_x = r_u32(0x800ADCA8);
    first_height = r_s16(first + 0x1278);
    second_z = r_u32(0x800ADCC0);
    first_z = r_u32(0x800ADCB0);
    second_height = r_s16(second + 0x1278);
    angle = (uint32)(sint32)(sint16)t3_8004b39c(second_x - first_x, second_z - first_z);
    if (first_height >= second_height)
        angle = (angle + 2048) & 0xFFF;
    angle <<= 6;
    index = (uint32)r_s16(0x800A8CA4) - 1;
    t3_80067afc(index, &distance, &adjustment);
    if (r_s16(0x800A8C90))
        angle -= adjustment;
    else
        angle += adjustment;
    w_u32(0x800A8CB0, angle);
    w_u32(0x800A8CAC, distance);
    return 0x800A8C70;
}

uint32 t3_800573f4(uint32 state)
{
    sint32 count;
    sint32 index;
    uint32 selected = 0;
    uint32 entry;
    uint32 action;
    uint32 flags;
    uint32 attributes;
    uint32 kind;
    uint32 high;
    uint32 selector;
    uint32 mask;
    uint32 eligible;
    uint32 total;
    FUNCTION_MARKER(0x800573F4, "SLUS_004.02");
    count = r_s16(state + 0x70);
    if (count <= 0)
        return 0;
    for (index = 0; index < count; ++index) {
        entry = 0x8009F978 + (uint32)index * 12;
        if (r_s16(entry + 8))
            continue;
        action = r_u32(entry);
        flags = r_u32(action + 4);
        kind = r_u8(action + 25);
        attributes = r_u32(action + 8);
        eligible = 0;
        if (!kind || (flags & 0x80000) || (attributes & 0x200000)) {
            if (r_u8(action + 45) && (attributes & 0xFFFF) != 0x412 && (attributes & 0xFFFF) != 0x706 && !(attributes & 0x400000) && !(flags & 0x20000)) {
                high = r_u32(action + 8);
                selector = r_u32(state + 20);
                high &= 0xFFFF0000;
                if (!(high & 0x1C0000))
                    high |= 0xC0000;
                mask = r_u32(0x80098230 + selector * 4);
                eligible = (high & mask) != 0 || (flags & 0x10000) != 0;
            }
        }
        if (eligible) {
            w_u16(entry + 8, 1);
            ++selected;
        }
    }
    total = r_u16(state + 0x72);
    w_u16(state + 0x72, (uint16)(total + selected));
    return selected;
}

uint32 t3_80057b60(uint32 state)
{
    sint32 count;
    sint32 index;
    uint32 selected = 0;
    uint32 entry;
    uint32 action;
    uint32 command;
    uint32 attributes;
    uint32 high;
    uint32 selector;
    uint32 mask;
    uint32 total;
    FUNCTION_MARKER(0x80057B60, "SLUS_004.02");
    count = r_s16(state + 0x70);
    if (count <= 0)
        return 0;
    for (index = 0; index < count; ++index) {
        entry = 0x8009F978 + (uint32)index * 12;
        if (r_s16(entry + 8))
            continue;
        action = r_u32(entry);
        if (!r_u8(action + 45))
            continue;
        command = r_u32(entry + 4);
        if (r_u16(command) & 0x3FE0)
            continue;
        if (r_u32(action + 4) & 0x20000)
            continue;
        if (r_s16(action + 20) < 0)
            continue;
        attributes = r_u32(action + 8);
        if (attributes & 0x400000)
            continue;
        if (r_u8(action + 25))
            continue;
        high = attributes & 0xFFFF0000;
        selector = r_u32(state + 20);
        if (!(high & 0x1C0000))
            high |= 0xC0000;
        mask = r_u32(0x80098230 + selector * 4);
        if (high & mask) {
            w_u16(entry + 8, 1);
            ++selected;
        }
    }
    total = r_u16(state + 0x72);
    w_u16(state + 0x72, (uint16)(total + selected));
    return selected;
}

uint32 t3_80061a58(uint32 output, uint32 selection)
{
    uint32 actor;
    uint32 opponent;
    uint32 direction;
    uint32 table;
    uint32 header;
    uint32 flags;
    uint32 value;
    sint32 first_height;
    sint32 second_height;
    FUNCTION_MARKER(0x80061A58, "SLUS_004.02");
    if (selection == 0xFFFFFFFF) {
        actor = r_u32(0x800AFA5C);
        opponent = r_u32(0x800A867C);
        first_height = r_s16(actor + 0x1278);
        second_height = r_s16(opponent + 0x1278);
        direction = r_u16(actor + 60);
        selection = 0;
        if (direction - 257 >= 0x7EFF) {
            selection = 1;
            if (((direction + 0x7FFF) & 0xFFFF) >= 0x7EFE)
                selection = ((uint32)randPSX() >> 4) & 1;
        }
        if (second_height < first_height)
            selection = 1 - selection;
    }
    table = 0x8002306E + selection * 24;
    header = r_u16(table);
    if (r_s16(table + 2))
        w_u32(output + 8, table + 2);
    else
        w_u32(output + 8, 0);
    flags = header & 0x100 ? 128 : 0;
    if (header & 0x200)
        flags |= 16;
    if (header & 0x400)
        flags |= 64;
    if (header & 0x800)
        flags |= 32;
    value = r_u16(0x8002301C + (header & 15) * 2) | flags;
    w_u16(output + 4, (uint16)value);
    return t3_80056748(output);
}
