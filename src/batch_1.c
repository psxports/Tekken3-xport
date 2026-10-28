#include "game_functions.h"
#include "psx_spu.h"
#include "t3_sound.h"
#include "psx_gpu.h"
#include "t3_data.h"
#include "t3_audio.h"

void t3_80028ba0(void)
{
    FUNCTION_MARKER(0x80028BA0, "SLUS_004.02");
    // The host CRT replaces the empty original constructor list
    t3_800b0548();
    for (;;)
    {
        uint32 buffer_index;
        uint32 ordering_table;
        uint32 packet_head;
        uint32 packet_tail;

        t3_800296c4();
        t3_8006b6fc();
        w_u32(0x800AFA4C, r_u32(0x800AFA4C) + 1);
        if (t3_80029628() == 0)
            w_u32(0x800ADEFC, GsGetActiveBuff());
        buffer_index = r_u32(0x800ADEFC);
        ordering_table = 0x800A8590 + buffer_index * 20;
        w_u32(0x800A8C54, ordering_table);
        t3_80081c38(r_u32(0x800AE040 + buffer_index * 28));
        GsClearOt(0, 0, psx_addr(r_u32(0x800A8C54), 20));
        t3_8004c684(r_u32(0x800ADEFC));
        switch (r_s16(0x800AE204))
        {
        case 0: t3_800b0708(); break;
        case 1: t3_80052cc4(); break;
        case 2: t3_8004fa60(); break;
        case 3: t3_bns_05_800db1b8(); break;
        case 4: t3_bns_05_800db5b4(); break;
        case 5: t3_bns_05_800dde64(); break;
        case 6: t3_bns_07_800d3228(); break;
        case 7: t3_80050318(); break;
        case 8: t3_80050428(); break;
        case 9: t3_8010ff4c(); break;
        case 10: t3_80055590(); break;
        case 11: t3_80052520(); break;
        case 12: t3_800eee8c(); break;
        case 13: t3_800ef92c(); break;
        case 14: t3_800f0458(); break;
        case 15: t3_800f18e8(); break;
        case 16: t3_800501c0(); break;
        case 17: t3_800c2434(); break;
        case 18: t3_8004ff74(); break;
        case 19: t3_800ff0c4(); break;
        default: break;
        }
        packet_head = r_u32(0x800A9218);
        ordering_table = r_u32(0x800A8C54);
        packet_tail = packet_head + 32;
        AddPrims(psx_addr(r_u32(ordering_table + 4) + 32, 4), psx_addr(packet_head, 4), psx_addr(packet_tail, 4));
        packet_head = r_u32(0x800ADD54);
        ordering_table = r_u32(0x800A8C54);
        packet_tail = packet_head + 32;
        AddPrims(psx_addr(r_u32(ordering_table + 4) + 4024, 4), psx_addr(packet_head, 4), psx_addr(packet_tail, 4));
    }
}

uint32 t3_80079964(uint32 mode)
{
    FUNCTION_MARKER(0x80079964, "SLUS_004.02");
    return mode == 4 ? 0x28990 : 0x26900;
}

uint32 t3_800799a8(uint32 mode, uint32 work_base)
{
    uint32 table = 0x800AE040;
    uint32 buffer_index;
    uint32 item;
    uint32 draw_words;
    uint32 auxiliary_words;

    FUNCTION_MARKER(0x800799A8, "SLUS_004.02");
    if (mode == 3)
    {
        for (buffer_index = 0; buffer_index < 2; ++buffer_index)
        {
            for (item = 0; item < 2; ++item)
            {
                w_u32(table + item * 4, work_base);
                work_base += 0x7F48;
            }
            w_u32(table + 8, work_base - 0x3FA4);
            for (item = 0; item < 2; ++item)
            {
                w_u32(table + 12 + item * 4, work_base);
                work_base += 0x5B8;
            }
            w_u32(table + 24, work_base);
            w_u32(table + 20, work_base - 732);
            work_base += 0x2A80;
            table += 28;
        }
    }
    else
    {
        draw_words = mode == 4 ? 8669 : 8146;
        auxiliary_words = mode == 4 ? 364 : 366;
        for (buffer_index = 0; buffer_index < 2; ++buffer_index)
        {
            for (item = 0; item < 2; ++item)
            {
                w_u32(table + item * 4, work_base);
                work_base += draw_words * 4;
            }
            w_u32(table + 8, 0);
            for (item = 0; item < 2; ++item)
            {
                w_u32(table + 12 + item * 4, work_base);
                work_base += auxiliary_words * 4;
            }
            w_u32(table + 24, work_base);
            w_u32(table + 20, 0);
            work_base += 0x2A80;
            table += 28;
        }
    }
    return 0;
}

uint32 t3_8006ad28(uint32 mode)
{
    FUNCTION_MARKER(0x8006AD28, "SLUS_004.02");
    w_u16(0x800A05D8, (uint16)mode);
    w_u32(0x800A866C, mode - 5 < 2);
    return 0x800B0000;
}

uint32 t3_80028f5c(void)
{
    uint32 controller;
    uint32 field;

    FUNCTION_MARKER(0x80028F5C, "SLUS_004.02");
    for (controller = 0; controller < 2; ++controller)
    {
        for (field = 0; field < 4; ++field)
            w_u16(0x8009BA40 + controller * 8 + field * 2, 0);
        t3_8002a16c(controller, 0);
    }
    w_u8(0x8009BA38, 0);
    return 0x800A0000;
}

uint32 t3_8002a16c(uint32 controller, uint32 enabled)
{
    uint32 state = 0x800A9130 + controller * 42;

    FUNCTION_MARKER(0x8002A16C, "SLUS_004.02");
    w_u8(state + 41, (uint8)enabled);
    return state;
}

uint32 t3_800294ac(void)
{
    FUNCTION_MARKER(0x800294AC, "SLUS_004.02");
    w_u32(0x8009BC50, 0x8009BA50);
    w_u32(0x8009BC54, 0);
    return 0x800A0000;
}

uint32 t3_80029924(sint32 mode)
{
    uint32 previous;
    uint32 current;

    FUNCTION_MARKER(0x80029924, "SLUS_004.02");
    if (mode == 1)
    {
        current = r_u32(0x80095434);
        previous = r_u32(0x800AE218);
        w_u32(0x800AE218, current);
        w_u32(0x8009BC58, current - previous);
        return r_u32(0x800AE218);
    }
    if (mode >= 2)
    {
        if (mode == 2)
        {
            current = r_u32(0x80095434);
            return current - r_u32(0x800AE218);
        }
        if (mode == 3)
            return r_u32(0x8009BC58);
        return r_u32(0x800AE218);
    }
    if (mode != 0)
        return r_u32(0x800AE218);
    w_u32(0x8009BC58, 0);
    w_u32(0x800AE218, r_u32(0x80095434));
    return 0;
}

uint32 t3_80069708(void)
{
    FUNCTION_MARKER(0x80069708, "SLUS_004.02");
    w_u8(0x800A04CD, 0);
    return 0x800A0000;
}

void t3_80067258(void)
{
    FUNCTION_MARKER(0x80067258, "SLUS_004.02");
}

uint32 t3_80029674(void)
{
    uint32 counter;
    uint32 deadline;

    FUNCTION_MARKER(0x80029674, "SLUS_004.02");
    if (r_u32(0x80095424) == 0)
        return 0;
    counter = (uint32)GetRCnt(0xF2000002);
    deadline = r_u32(0x80095430) - 264;
    return counter >= deadline;
}

uint32 t3_80029628(void)
{
    FUNCTION_MARKER(0x80029628, "SLUS_004.02");
    return r_u32(0x8009542C) == 0;
}

uint32 t3_800296c4(void)
{
    uint32 seed;
    uint32 completed;

    FUNCTION_MARKER(0x800296C4, "SLUS_004.02");
    w_u32(0x80095428, 0);
    t3_80029924(1);
    if (r_u32(0x8009542C) != 0)
    {
        t3_80029a28();
        w_u32(0x8009BC60, r_u32(0x800A8C54));
    }
    w_u32(0x8009BC5C, 0);
    do
    {
        psx_native_poll_events();
        // Service timer events that became due while native presentation blocked
        psx_root_counter_poll();
        t3_8004ce54();
        seed = r_u32(0x800A912C);
        completed = r_u32(0x8009BC5C);
        w_u32(0x800A912C, (seed + 1) * 69069);
    } while (completed == 0);
    w_u32(0x800ADCA4, 0);
    return 0x800B0000;
}

void t3_80029a28(void)
{
    uint32 table;
    uint32 base;
    uint32 link;
    uint32 first_middle;
    uint32 second_middle;
    uint32 first_link;
    uint32 second_link;
    uint32 current;
    uint32 previous;
    uint32 pending = 0;
    uint32 packet;
    uint32 expected;
    sint32 remaining;

    FUNCTION_MARKER(0x80029A28, "SLUS_004.02");
    if (r_u32(0x800AFA88) - 7 >= 2)
    {
        table = r_u32(0x800A8C54);
        base = r_u32(table + 16) - 4092;
        link = r_u32(base + 3492);
        first_middle = r_u32(base + 2048);
        second_middle = r_u32(base + 2048);
        first_link = r_u32(base + 4);
        second_link = r_u32(base + 4);
        w_u32(base + 3492, (link & 0xFF000000) | (first_middle & 0xFFFFFF));
        w_u32(base + 4, (first_link & 0xFF000000) | (link & 0xFFFFFF));
        w_u32(base + 2048, (second_middle & 0xFF000000) | (second_link & 0xFFFFFF));
    }
    table = r_u32(0x800A8C54);
    base = r_u32(table + 16);
    remaining = (sint32)(1u << ((r_u32(table) - 1) & 31));
    current = base - 2048;
    previous = base - 2052;
    if (remaining > 0)
    {
        do
        {
            if (t3_80029674() != 0)
                goto timed_out;
            expected = previous & 0xFFFFFF;
            packet = r_u32(current);
            if ((packet & 0xFFFFFF) == expected)
            {
                if (pending == 0)
                    pending = current;
            }
            else if (pending != 0)
            {
                w_u32(pending, packet);
                pending = 0;
            }
            --remaining;
            previous -= 4;
            current -= 4;
        } while (remaining > 0);
    }
    table = r_u32(0x800A8C54);
    remaining = (sint32)(1u << ((r_u32(table) - 1) & 31));
    current = r_u32(table + 16);
    pending = 0;
    previous = current - 4;
    if (remaining <= 0)
        return;
    do
    {
        if (t3_80029674() != 0)
            goto timed_out;
        expected = previous & 0xFFFFFF;
        packet = r_u32(current);
        if ((packet & 0xFFFFFF) == expected)
        {
            if (pending == 0)
                pending = current;
        }
        else if (pending != 0)
        {
            w_u32(pending, packet);
            pending = 0;
        }
        --remaining;
        previous -= 4;
        current -= 4;
    } while (remaining > 0);
    return;

timed_out:
    if (pending != 0)
        w_u32(pending, r_u32(previous + 8));
}

uint32 t3_8006bc98(sint32 level, sint32 duration)
{
    sint32 scale;
    sint32 previous;
    sint32 master;
    sint32 current_volume;
    sint32 previous_volume;
    sint32 mode;
    sint32 narrowed;
    uint32 current_product;
    uint32 previous_product;
    CdlATV mix;

    FUNCTION_MARKER(0x8006BC98, "SLUS_004.02");
    scale = r_s16(0x800A068A);
    current_product = (uint32)level * (uint32)scale;
    previous = r_s16(0x800A068C);
    previous_product = (uint32)previous * (uint32)scale;
    master = r_s16(0x800A0688);
    current_product *= (uint32)master;
    previous_product *= (uint32)master;
    current_volume = (sint32)current_product / 16129;
    previous_volume = (sint32)previous_product / 16129;
    w_u16(0x800A0694, (uint16)duration);
    w_u16(0x800A0692, (uint16)duration);
    w_u16(0x800A0690, (uint16)current_volume);
    w_u16(0x800A068E, (uint16)previous_volume);
    if (duration > 0)
        return (uint32)previous_volume;
    w_u16(0x800A068E, (uint16)current_volume);
    mode = r_s16(0x800A061C);
    narrowed = (sint16)current_volume;
    w_u16(0x800A068C, (uint16)narrowed);
    if (mode == 0 || mode == 2)
    {
        mix.val0 = 0;
        mix.val1 = 0;
        mix.val2 = 0;
        mix.val3 = 0;
    }
    else if (r_u8(0x80097F04) != 0)
    {
        mix.val0 = (uint8)narrowed;
        mix.val1 = 0;
        mix.val2 = (uint8)narrowed;
        mix.val3 = 0;
    }
    else
    {
        uint8 volume = (uint8)(narrowed * 80 / 100);
        mix.val3 = volume;
        mix.val2 = volume;
        mix.val1 = volume;
        mix.val0 = volume;
    }
    return t3_80090a88(&mix);
}

uint32 t3_80090a88(CdlATV *volume)
{
    FUNCTION_MARKER(0x80090A88, "SLUS_004.02");
    CD_vol(volume);
    return 1;
}

uint32 t3_8006be80(void)
{
    FUNCTION_MARKER(0x8006BE80, "SLUS_004.02");
    w_u16(0x800A069C, 0);
    w_u8(0x800A069F, 0);
    w_u32(0x800A0698, 0x800A06B8);
    w_u8(0x800A069E, 0);
    return 0x800A06B8;
}

uint32 t3_8006ab64(void)
{
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;

    FUNCTION_MARKER(0x8006AB64, "SLUS_004.02");
    if (!t3_data_init()) {
        xport_message_error("Tekken 3", "Cannot open BNS/XAS data files in DATA/TEKKEN3");
        return t3_8007ae58(1);
    }
    t3_8006bc98(127, -1);
    if (r_u32(0x800987D4) != 0) {
        t3_8006b54c(0);
        return 1;
    }
    w_u32(0x800987D4, 1);
    w_u8(0x800ADC91, 3);
    w_u32(0x800A866C, 0);
    w_u16(0x800A0620, 0xFFFF);
    w_u16(0x800A0622, 0xFFFF);
    w_u32(0x800A0630, 0xFFFFFFFF);
    w_u32(0x800A062C, 0xFFFFFFFF);
    w_u32(0x800A0634, 0xFFFFFFFF);
    w_u16(0x800A05DE, 0xFFFF);
    t3_8006ad28(1);
    first = r_u32(0x800A0670);
    second = r_u32(0x800A0674);
    third = r_u32(0x800A0678);
    fourth = r_u32(0x800A067C);
    w_u32(0x800A05F8, first);
    w_u32(0x800A05FC, second);
    w_u32(0x800A0600, third);
    w_u32(0x800A0604, fourth);
    first = r_u32(0x800A0680);
    second = r_u32(0x800A0684);
    w_u32(0x800A0608, first);
    w_u32(0x800A060C, second);
    w_u16(0x800A063C, 1);
    w_u16(0x800A063A, 0);
    w_u16(0x800A0638, 0);
    w_u16(0x800A05DC, 3);
    w_u16(0x800A0688, 127);
    w_u16(0x800A068A, 127);
    t3_8006bc98(127, -1);
    w_u16(0x800A05DA, 0);
    return t3_8006be80();
}

void t3_8006acdc(uint32 mode)
{
    FUNCTION_MARKER(0x8006ACDC, "SLUS_004.02");
    w_u16(0x800A05DC, (uint16)mode);
}

uint32 t3_8004b61c(uint32 preserve_voice_three)
{
    sint32 voice;

    FUNCTION_MARKER(0x8004B61C, "SLUS_004.02");
    t3_8006b54c(0);
    for (voice = 0; voice < 24; ++voice)
    {
        if (preserve_voice_three == 0 || voice != 3)
            t3_80088938(voice);
    }
    return 0;
}

uint32 t3_8006c568(void)
{
    FUNCTION_MARKER(0x8006C568, "SLUS_004.02");
    return t3_8006c588(31);
}

uint32 t3_8006c588(uint32 flags)
{
    FUNCTION_MARKER(0x8006C588, "SLUS_004.02");
    if ((flags & 1) != 0)
        w_u16(0x800A0898, 0xFFFF);
    if ((flags & 2) != 0)
        w_u16(0x800A089A, 0xFFFF);
    if ((flags & 4) != 0)
        w_u16(0x800A089C, 0xFFFF);
    if ((flags & 8) != 0)
        w_u16(0x800A089E, 0xFFFF);
    if ((flags & 16) != 0)
        w_u16(0x800A08A0, 0xFFFF);
    if ((flags & 12) != 0)
        return t3_800697d4(3);
    return flags & 12;
}

uint32 t3_8004ce54(void)
{
    uint32 seed;

    FUNCTION_MARKER(0x8004CE54, "SLUS_004.02");
    seed = r_u32(0x8009F2A8) * 5 + 1;
    w_u32(0x8009F2A8, seed);
    return seed;
}

void t3_80074da4(void)
{
    FUNCTION_MARKER(0x80074DA4, "SLUS_004.02");
    t3_sound_bind();
    t3_80087b28();
    SsSetTickMode(4096);
    t3_8008a958();
    t3_80087e98();
    SsSetMVol(127, 127);
    t3_80074e10();
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, 128, 128);
}

uint32 t3_80074e10(void)
{
    FUNCTION_MARKER(0x80074E10, "SLUS_004.02");
    SsVabClose(0);
    SsVabClose(1);
    t3_80075970(0x80025850, 0x800D366C, 0);
    t3_80075970(0x80026A70, 0x8011001C, 1);
    w_u8(0x800A9227, 0xFF);
    w_u8(0x800A9226, 0xFF);
    w_u32(0x800AE010, 0xFFFFFFFF);
    w_u32(0x800AE014, 0x800A25E0);
    return 0x800A25E0;
}

uint32 t3_80075970(uint32 header, uint32 body, uint32 bank)
{
    sint32 signed_bank = (sint16)bank;
    uint32 sound_address;

    FUNCTION_MARKER(0x80075970, "SLUS_004.02");
    sound_address = r_u32(0x8002790C + bank * 4);
    t3_8008aa28(header, signed_bank, sound_address);
    t3_8008ae58(body, signed_bank);
    return (uint32)(sint32)SsVabTransCompleted(1);
}

uint32 t3_80029638(uint32 increment)
{
    uint32 count;

    FUNCTION_MARKER(0x80029638, "SLUS_004.02");
    count = r_u32(0x80095428);
    if (increment != 0)
        ++count;
    else
    {
        if (count == 0)
            return 0;
        count = r_u32(0x80095428) - 1;
    }
    w_u32(0x80095428, count);
    return count;
}

uint32 t3_8002960c(uint32 mode)
{
    FUNCTION_MARKER(0x8002960C, "SLUS_004.02");
    w_u32(0x8009542C, mode == 0);
    return 0x80090000;
}

uint32 t3_80052b7c(void)
{
    FUNCTION_MARKER(0x80052B7C, "SLUS_004.02");
    return 0x800B8D58;
}

uint32 t3_800529cc(uint32 destination, sint32 file)
{
    FUNCTION_MARKER(0x800529CC, "SLUS_004.02");
    if (file < 10)
        w_u32(0x80098118, 1);
    t3_8006bea8();
    t3_8006bf20(file, destination, 0);
    t3_8006c084();
    return 0;
}

uint32 t3_8006bea8(void)
{
    FUNCTION_MARKER(0x8006BEA8, "SLUS_004.02");
    if (r_u8(0x800A069F) != 0)
    {
        t3_8006c1fc();
        while (r_u8(0x800A069E) != 0)
        {
            t3_8006c2a0(1);
        }
    }
    w_u8(0x800A069F, 0);
    w_u16(0x800A069C, 0);
    w_u32(0x800A0698, 0);
    w_u8(0x800A069E, 0);
    return 0x800A0698;
}

uint32 t3_8006c23c(void)
{
    FUNCTION_MARKER(0x8006C23C, "SLUS_004.02");
    return r_u8(0x800A069F);
}

uint32 t3_8004da10(uint32 ordering_entry, uint32 packet, uint32 first, uint32 second, uint32 third)
{
    uint32 tag;

    FUNCTION_MARKER(0x8004DA10, "SLUS_004.02");
    w_u32(packet + 4, first);
    w_u32(packet + 8, second);
    w_u32(packet + 12, third);
    tag = r_u32(ordering_entry);
    w_u32(packet, (tag & 0xFFFFFF) | 0x03000000);
    tag = r_u32(ordering_entry);
    w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    return packet + 16;
}

uint32 t3_8004daa8(uint32 ordering_entry, uint32 packet, uint32 texture_page)
{
    uint32 tag;

    FUNCTION_MARKER(0x8004DAA8, "SLUS_004.02");
    SetDrawMode(psx_addr(packet, 12), 0, 1, texture_page, NULL);
    tag = r_u32(ordering_entry);
    w_u32(packet, (tag & 0xFFFFFF) | 0x02000000);
    tag = r_u32(ordering_entry);
    w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    return packet + 12;
}

void t3_8006c26c(uint32 result)
{
    FUNCTION_MARKER(0x8006C26C, "SLUS_004.02");
    if ((result & 0xFF) == 2 && !t3_bns_pump())
    {
        xport_message_error("Tekken 3", "Cannot read DATA/TEKKEN3/TEKKEN3.BNS");
        t3_8007ae58(1);
    }
}

uint32 t3_8004c370(void)
{
    FUNCTION_MARKER(0x8004C370, "SLUS_004.02");
    return t3_8004c240();
}

static uint32 t3_copy_guest_string_core(uint32 guest_destination, uint8 *native_destination, uint32 source, const uint8 *native_source)
{
    uint32 length = 0;
    uint32 value;
    for (;;)
    {
        value = native_source ? native_source[length] : r_u8(source++);
        if (native_destination)
            native_destination[length] = (uint8)value;
        else
            w_u8(guest_destination + length, (uint8)value);
        if (value == 0)
            return length;
        ++length;
    }
}

uint32 t3_8004cc08(uint32 destination, uint32 source)
{
    FUNCTION_MARKER(0x8004CC08, "SLUS_004.02");
    return destination + t3_copy_guest_string_core(destination, 0, source, 0);
}

void *t3_copy_guest_string_to_native(void *destination, uint32 source)
{
    return (uint8 *)destination + t3_copy_guest_string_core(0, destination, source, 0);
}

uint32 t3_copy_native_string_to_guest(uint32 destination, const uint8 *source)
{
    return destination + t3_copy_guest_string_core(destination, 0, 0, source);
}

uint32 t3_80052a70(void)
{
    FUNCTION_MARKER(0x80052A70, "SLUS_004.02");
    if (t3_8006c23c() != 0)
        return 1;
    if (r_u32(0x80098118) != 0)
    {
        EnterCriticalSectionPSX();
        FlushCache();
        ExitCriticalSection();
        w_u32(0x80098118, 0);
    }
    return 0;
}

uint32 t3_80050f38(void)
{
    uint32 first;
    uint32 second;

    FUNCTION_MARKER(0x80050F38, "SLUS_004.02");
    first = r_u16(0x800ADF08);
    second = r_u16(0x800ADF0A);
    return ((first | second) & 0x800) != 0;
}

uint32 t3_8006c084(void)
{
    sint32 count;
    uint32 node;
    uint32 next;
    uint32 size;
    sint32 rounded;
    uint32 sector;
    uint32 next_sector;
    uint32 callback_value;

    FUNCTION_MARKER(0x8006C084, "SLUS_004.02");
    count = r_s16(0x800A069C);
    w_u32(0x800A06B8 + (uint32)count * 28 + 24, 0);
    count = r_s16(0x800A069C);
    node = r_u32(0x800A0698);
    if (count >= 2 && r_u32(node + 24) != 0)
    {
        do
        {
            size = r_u32(node + 16);
            rounded = (sint32)(size + 2047);
            if (rounded < 0)
                rounded = (sint32)(size + 4094);
            rounded >>= 11;
            next = r_u32(node + 24);
            sector = r_u32(node + 12);
            next_sector = r_u32(next + 12);
            if ((uint32)rounded + sector == next_sector)
                w_u16(next + 2, 0);
            node = r_u32(node + 24);
        } while (r_u32(node + 24) != 0);
    }
    t3_8006b54c(0);
    count = r_s16(0x800A069C);
    if (count <= 0)
        return (uint32)count;
    if (t3_8006c23c() != 0)
        t3_8007ae58(1);
    node = r_u32(0x800A0698);
    w_u16(0x800A06A0, r_u16(node));
    w_u32(0x800A06A4, r_u32(node + 16));
    w_u32(0x800A06A8, r_u32(node + 8));
    callback_value = r_u32(node + 20);
    w_u8(0x800A069F, 1);
    w_u32(0x800A06AC, callback_value);
    w_u16(0x800A05D8, 8);
    w_u32(0x800A866C, 0);
    if (!t3_bns_pump()) {
        xport_message_error("Tekken 3", "Cannot read DATA/TEKKEN3/TEKKEN3.BNS");
        t3_8007ae58(1);
    }
    return 0x800B0000;
}

uint32 t3_8006c1fc(void)
{
    FUNCTION_MARKER(0x8006C1FC, "SLUS_004.02");
    if (t3_8006c23c() == 0)
        return 0;
    w_u8(0x800A069E, 1);
    return 1;
}

uint32 t3_80031d04(void)
{
    FUNCTION_MARKER(0x80031D04, "SLUS_004.02");
    return 0x8330;
}

uint32 t3_8004a404(void)
{
    FUNCTION_MARKER(0x8004A404, "SLUS_004.02");
    return 0x1060;
}

uint32 t3_8006e514(void)
{
    FUNCTION_MARKER(0x8006E514, "SLUS_004.02");
    return 0x4100;
}

uint32 t3_800484ec(void)
{
    FUNCTION_MARKER(0x800484EC, "SLUS_004.02");
    return 0x2E84;
}

uint32 t3_8006d00c(void)
{
    FUNCTION_MARKER(0x8006D00C, "SLUS_004.02");
    return 0x41A0;
}

uint32 t3_8006c7f8(void)
{
    FUNCTION_MARKER(0x8006C7F8, "SLUS_004.02");
    return 0x5F4C;
}

uint32 t3_80048074(void)
{
    FUNCTION_MARKER(0x80048074, "SLUS_004.02");
    return 0x328;
}

uint32 t3_800697b8(uint32 small)
{
    FUNCTION_MARKER(0x800697B8, "SLUS_004.02");
    return small != 0 ? 0x70800 : 0xEF808;
}

uint32 t3_8006ad54(uint32 channel)
{
    FUNCTION_MARKER(0x8006AD54, "SLUS_004.02");
    if (r_s16(0x800A0638 + channel * 2) != 0)
        return t3_80090c0c(psx_addr(0x800A0640 + channel * 24, 4));
    return t3_8007ae58(1);
}

uint32 t3_80090c0c(CdlLOC *position)
{
    FUNCTION_MARKER(0x80090C0C, "SLUS_004.02");
    return (uint32)CdPosToInt(position);
}

uint32 t3_8002a0e4(uint32 controller)
{
    FUNCTION_MARKER(0x8002A0E4, "SLUS_004.02");
    return (r_u8(0x800A9184) >> (controller & 31)) & 1;
}

uint32 t3_8004db28(uint32 ordering_entry, uint32 packet, const PSX_RECT *rectangle)
{
    uint32 tag;

    FUNCTION_MARKER(0x8004DB28, "SLUS_004.02");
    SetDrawArea(psx_addr(packet, 12), rectangle);
    tag = r_u32(ordering_entry);
    w_u32(packet, (tag & 0xFFFFFF) | 0x02000000);
    tag = r_u32(ordering_entry);
    w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    return packet + 12;
}

uint32 t3_8004d8b0(uint32 ordering_entry, uint32 packet, uint32 color0, uint32 color1, uint32 color2, uint32 color3, uint32 xy0, uint32 xy1, uint32 xy2, uint32 xy3)
{
    uint32 tag;

    FUNCTION_MARKER(0x8004D8B0, "SLUS_004.02");
    w_u32(packet + 4, color0);
    w_u32(packet + 12, color1);
    w_u32(packet + 20, color2);
    w_u32(packet + 28, color3);
    w_u32(packet + 8, xy0);
    w_u32(packet + 16, xy1);
    w_u32(packet + 24, xy2);
    w_u32(packet + 32, xy3);
    tag = r_u32(ordering_entry);
    w_u32(packet, (tag & 0xFFFFFF) | 0x08000000);
    tag = r_u32(ordering_entry);
    w_u32(ordering_entry, (packet & 0xFFFFFF) | (tag & 0xFF000000));
    return packet + 36;
}

uint32 t3_8004df6c(uint32 color, uint32 scale)
{
    uint32 red;
    uint32 green;
    uint32 blue;

    FUNCTION_MARKER(0x8004DF6C, "SLUS_004.02");
    red = ((color & 0xFF) * scale) >> 8;
    if (red >= 256)
        red = 255;
    green = (((color >> 8) & 0xFF) * scale) >> 8;
    if (green >= 256)
        green = 255;
    blue = (((color >> 16) & 0xFF) * scale) >> 8;
    if (blue >= 256)
        blue = 255;
    return red | (green << 8) | (blue << 16);
}

uint32 t3_8005118c(uint32 side)
{
    FUNCTION_MARKER(0x8005118C, "SLUS_004.02");
    w_u8(0x800ADF3E, (uint8)side);
    w_u8(0x800B053E, (uint8)((side + 1) & 1));
    w_u16(0x800ADD50, 0);
    return 0x800B0000;
}

uint32 t3_80052b88(uint32 file)
{
    FUNCTION_MARKER(0x80052B88, "SLUS_004.02");
    return t3_80052be0(0x800B0548, file, 0);
}

uint32 t3_80052be0(uint32 destination, uint32 file, uint32 flags)
{
    uint32 count;
    uint32 entry;

    FUNCTION_MARKER(0x80052BE0, "SLUS_004.02");
    count = r_u8(0x8009815C);
    if (count >= 8)
        return 0;
    count = r_u8(0x8009815C);
    entry = 0x8009811C + count * 8;
    w_u8(0x8009815C, (uint8)(count + 1));
    w_u16(entry, (uint16)flags);
    w_u16(entry + 2, (uint16)file);
    w_u32(entry + 4, destination);
    return 1;
}

uint32 t3_80052c30(uint32 state, uint32 substate)
{
    uint32 file;

    FUNCTION_MARKER(0x80052C30, "SLUS_004.02");
    if (r_u8(0x8009815C) == 0)
    {
        w_u16(0x800AE204, (uint16)state);
        w_u16(0x800AE224, (uint16)substate);
        return 0;
    }
    file = r_u16(0x8009811E);
    w_u8(0x8009815D, 0);
    t3_80052a34(file);
    t3_8002960c(1);
    w_u8(0x8009815E, (uint8)state);
    w_u8(0x8009815F, (uint8)substate);
    w_u16(0x800AE204, 1);
    w_u16(0x800AE224, 0);
    return 0x800B0000;
}

uint32 t3_80052a34(uint32 file)
{
    FUNCTION_MARKER(0x80052A34, "SLUS_004.02");
    if (t3_8006c23c() == 0)
        t3_8006c250();
    return 0;
}

uint32 t3_8006c250(void)
{
    FUNCTION_MARKER(0x8006C250, "SLUS_004.02");
    return (uint32)(sint32)r_s16(0x800A0620) >> 31;
}

uint32 t3_80050f1c(void)
{
    FUNCTION_MARKER(0x80050F1C, "SLUS_004.02");
    return (r_u32(0x800AFA94) & 0xFFFF00FF) != 0;
}

uint32 t3_80052bb4(uint32 file)
{
    FUNCTION_MARKER(0x80052BB4, "SLUS_004.02");
    return t3_80052be0(0x800B8D58, file, 0);
}

uint32 t3_8004ccac(uint32 value)
{
    uint32 count = 0;

    FUNCTION_MARKER(0x8004CCAC, "SLUS_004.02");
    while (value != 0)
    {
        if ((value & 1) != 0)
            ++count;
        value >>= 1;
    }
    return count;
}

uint32 t3_8004b688(uint32 preserve_reserved)
{
    uint32 voice;

    FUNCTION_MARKER(0x8004B688, "SLUS_004.02");
    for (voice = 0; voice < 24; ++voice)
    {
        if (voice != 3 && voice != 2 && (preserve_reserved == 0 || (voice - 6 >= 2 && voice != 1)))
            t3_80088938((sint32)voice);
    }
    return 0;
}

uint32 t3_80051114(uint32 player)
{
    uint32 selected;
    uint32 active;

    FUNCTION_MARKER(0x80051114, "SLUS_004.02");
    selected = r_u16(0x800AE1F8 + player * 2);
    active = r_u16(0x800ADFBC + player * 2);
    return ((selected != 0) << 1) | (active != 0);
}

uint32 t3_8004b790(sint32 character, sint32 variant)
{
    FUNCTION_MARKER(0x8004B790, "SLUS_004.02");
    if (character >= 22 || variant >= 4)
    {
        character = 22;
        variant = 0;
    }
    return r_u32(0x80097AB8 + (uint32)character * 16 + (uint32)variant * 4);
}

uint32 t3_8004eff0(uint32 character, uint32 variant)
{
    uint32 index = (character << 2) | variant;

    FUNCTION_MARKER(0x8004EFF0, "SLUS_004.02");
    if ((sint32)index >= 93)
        index = 88;
    return r_u32(0x80097D40 + index * 4);
}

uint32 t3_8004dfe0(uint32 phase)
{
    FUNCTION_MARKER(0x8004DFE0, "SLUS_004.02");
    phase &= 511;
    if (phase >= 256)
        return 511 - phase;
    return phase;
}

uint32 t3_8006bdfc(uint32 result)
{
    union
    {
        uint32 words[2];
        CdlLOC position;
    } sector;
    uint32 position;

    FUNCTION_MARKER(0x8006BDFC, "SLUS_004.02");
    if ((result & 0xFF) != 1)
        return 1;
    t3_80090ac8(&sector, 1);
    position = t3_80090c0c(&sector.position);
    w_u32(0x800A0618, position);
    return position;
}

uint32 t3_800524ec(void)
{
    FUNCTION_MARKER(0x800524EC, "SLUS_004.02");
    w_u16(0x800AE204, 11);
    w_u16(0x800AE224, 0);
    return t3_8002960c(1);
}

uint32 t3_8004efc8(sint32 index)
{
    FUNCTION_MARKER(0x8004EFC8, "SLUS_004.02");
    if (index >= 93)
        index = 88;
    return r_u32(0x80097D40 + (uint32)index * 4);
}

uint32 t3_8004ccd8(uint32 value)
{
    uint32 count = 0;

    FUNCTION_MARKER(0x8004CCD8, "SLUS_004.02");
    while (value != 0)
    {
        value >>= 4;
        ++count;
    }
    return count;
}

uint32 t3_8004807c(void)
{
    FUNCTION_MARKER(0x8004807C, "SLUS_004.02");
    return r_u32(0x800ADC88);
}

uint32 t3_8006c800(uint32 offset)
{
    uint32 base;

    FUNCTION_MARKER(0x8006C800, "SLUS_004.02");
    base = r_u32(0x800ADD0C);
    if (base != 0)
        return base + offset + 1296;
    return 0;
}

uint32 t3_80031cbc(sint32 index, uint32 table)
{
    sint32 count;
    uint32 offset;

    FUNCTION_MARKER(0x80031CBC, "SLUS_004.02");
    count = r_s32(table);
    offset = r_u32(table + (uint32)index * 8 + 4);
    if (index >= count)
        return 0;
    return table + offset;
}

uint32 t3_80031cf0(uint32 index, uint32 table)
{
    FUNCTION_MARKER(0x80031CF0, "SLUS_004.02");
    return r_u32(table + index * 8 + 8);
}

uint32 t3_80048230(void)
{
    FUNCTION_MARKER(0x80048230, "SLUS_004.02");
    return r_u32(0x800ADC88);
}

uint32 t3_80039e44(uint32 index, uint32 *first_output, uint32 *second_output)
{
    uint32 entry = 0x80096C78 + index * 28;
    uint32 value;

    FUNCTION_MARKER(0x80039E44, "SLUS_004.02");
    value = r_u16(entry + 22);
    *first_output = value;
    value = r_u16(entry + 20);
    *second_output = value;
    return value;
}

uint32 t3_80052958(uint32 destination, sint32 variant)
{
    uint32 file = 73;

    FUNCTION_MARKER(0x80052958, "SLUS_004.02");
    if (variant != -1)
        file = (uint32)variant + 74;
    return t3_8006bf20((sint32)file, destination, 0);
}

uint32 t3_80035ed0(uint32 index)
{
    FUNCTION_MARKER(0x80035ED0, "SLUS_004.02");
    return r_u32(0x800ADEC8 + index * 4);
}

uint32 t3_80075240(uint32 index)
{
    FUNCTION_MARKER(0x80075240, "SLUS_004.02");
    return r_u32(0x800ADFFC + index * 12);
}

uint32 t3_800561a0(void)
{
    FUNCTION_MARKER(0x800561A0, "SLUS_004.02");
    return r_u32(0x8009F2E4);
}

void t3_80035ce0(void)
{
    FUNCTION_MARKER(0x80035CE0, "SLUS_004.02");
}

void t3_8003c250(uint32 value, uint32 object)
{
    FUNCTION_MARKER(0x8003C250, "SLUS_004.02");
    w_u32(object + 64, value);
}

void t3_80033dbc(uint32 object)
{
    FUNCTION_MARKER(0x80033DBC, "SLUS_004.02");
    w_u16(object + 4750, 0);
    w_u8(object + 4752, 0);
}

uint32 t3_80040ab8(uint32 index)
{
    FUNCTION_MARKER(0x80040AB8, "SLUS_004.02");
    w_u16(0x80097048 + index * 2, 0);
    return 0x80097048;
}

uint32 t3_80063070(void)
{
    FUNCTION_MARKER(0x80063070, "SLUS_004.02");
    w_u16(0x800A8C72, 0);
    return 0x800B0000;
}

void t3_8003c258(uint32 object)
{
    FUNCTION_MARKER(0x8003C258, "SLUS_004.02");
    w_u32(object + 6172, 0);
    w_u32(object + 6176, 0);
    w_u32(0x800A8D6C, 20);
}

uint32 t3_8006c858(void)
{
    FUNCTION_MARKER(0x8006C858, "SLUS_004.02");
    w_u32(0x800A9188, 0);
    return 0x800B0000;
}

uint32 t3_8004ac64(void)
{
    FUNCTION_MARKER(0x8004AC64, "SLUS_004.02");
    return t3_8004a83c();
}

uint32 t3_8004acf0(void)
{
    FUNCTION_MARKER(0x8004ACF0, "SLUS_004.02");
    w_u32(0x80097AA0, 0);
    return 0x80090000;
}

uint32 t3_8003a120(void)
{
    FUNCTION_MARKER(0x8003A120, "SLUS_004.02");
    w_u8(0x8009E5F1, 0);
    w_u8(0x8009E5F0, 0);
    return 0x8009E5F0;
}

uint32 t3_80041998(uint32 object)
{
    FUNCTION_MARKER(0x80041998, "SLUS_004.02");
    w_u16(object + 112, 0xFFFF);
    w_u16(object + 114, 0);
    w_u16(object + 286, 0);
    w_u16(object + 288, 0);
    return 0xFFFFFFFF;
}

uint32 t3_8004e580(uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x8004E580, "SLUS_004.02");
    w_u8(0x80097D34, (uint8)first);
    w_u8(0x80097D35, (uint8)second);
    return 0x80097D34;
}

uint32 t3_80039eec(void)
{
    FUNCTION_MARKER(0x80039EEC, "SLUS_004.02");
    w_u32(0x8009E5EC, 0);
    w_u16(0x8009E5AA, 0);
    w_u16(0x8009E5A8, 0);
    w_u16(0x8009E5A6, 0);
    return 0x8009E5A0;
}

uint32 t3_8003ebf0(void)
{
    FUNCTION_MARKER(0x8003EBF0, "SLUS_004.02");
    w_u32(0x80096F34, 0);
    return 0x80090000;
}

uint32 t3_800666e4(void)
{
    FUNCTION_MARKER(0x800666E4, "SLUS_004.02");
    w_u32(0x800A8CB4, 0);
    return 0x800B0000;
}

void t3_80066138(void)
{
    FUNCTION_MARKER(0x80066138, "SLUS_004.02");
    w_u32(0x800A8CA0, 0xFFFFFFFF);
}

uint32 t3_800666cc(void)
{
    FUNCTION_MARKER(0x800666CC, "SLUS_004.02");
    w_u16(0x800A8CBA, 0xFFFF);
    w_u32(0x800A8CB4, 0);
    return 0x800A8C70;
}

uint32 t3_80041f4c(uint32 actor)
{
    FUNCTION_MARKER(0x80041F4C, "SLUS_004.02");
    return t3_8006aa9c(actor);
}

uint32 t3_8002bb58(void)
{
    FUNCTION_MARKER(0x8002BB58, "SLUS_004.02");
    return r_u32(0x8009546C) - 6 < 2;
}

uint32 t3_80031144(uint32 index)
{
    FUNCTION_MARKER(0x80031144, "SLUS_004.02");
    w_u32(0x80095764 + index * 4, 0);
    return 0x80095764;
}

uint32 t3_8003cb6c(void)
{
    FUNCTION_MARKER(0x8003CB6C, "SLUS_004.02");
    return r_u32(0x80096F2C) - 1 < 2;
}

uint32 t3_8002bb70(void)
{
    FUNCTION_MARKER(0x8002BB70, "SLUS_004.02");
    return r_u32(0x8009546C) == 2;
}

uint32 t3_800295f4(void)
{
    FUNCTION_MARKER(0x800295F4, "SLUS_004.02");
    return r_s32(0x8009BC5C) >= 2;
}

uint32 t3_80061a44(uint32 object)
{
    FUNCTION_MARKER(0x80061A44, "SLUS_004.02");
    return r_s16(object + 160) == 6;
}

uint32 t3_80039e38(void)
{
    FUNCTION_MARKER(0x80039E38, "SLUS_004.02");
    return 0x8009E5A0;
}

uint32 t3_800299e0(void)
{
    FUNCTION_MARKER(0x800299E0, "SLUS_004.02");
    SetRCnt(0xF2000002, (uint16)r_u32(0x80095430), 0x1000);
    StartRCnt(0xF2000002);
    w_u32(0x80095424, 1);
    return 1;
}

uint32 t3_8002a128(uint32 controller, uint32 first, uint32 second)
{
    uint32 state = 0x800A9130 + controller * 42;
    FUNCTION_MARKER(0x8002A128, "SLUS_004.02");
    if (!r_u8(state + 41)) {
        first = 0;
        second = 0;
    }
    w_u8(state + 36, (uint8)first);
    w_u8(state + 37, (uint8)second);
    return 0;
}

uint32 t3_8004d9bc(uint32 ordering_table, uint32 packet, uint32 first, uint32 second, uint32 fourth, uint32 third)
{
    FUNCTION_MARKER(0x8004D9BC, "SLUS_004.02");
    w_u32(packet + 4, first);
    w_u32(packet + 8, second);
    w_u32(packet + 16, fourth);
    w_u32(packet + 12, third);
    w_u32(packet, (r_u32(ordering_table) & 0xFFFFFF) | 0x04000000);
    w_u32(ordering_table, (packet & 0xFFFFFF) | (r_u32(ordering_table) & 0xFF000000));
    return packet + 20;
}

uint32 t3_800784b0(void)
{
    uint32 index;
    FUNCTION_MARKER(0x800784B0, "SLUS_004.02");
    w_u32(0x800A3968, 0);
    for (index = 0; index < 2; ++index) {
        w_u32(0x800A3618 + index * 8, 0);
        w_u32(0x800A361C + index * 8, 0);
    }
    return 0;
}

uint32 t3_800361a8(uint32 object)
{
    uint32 index;
    uint32 result;
    FUNCTION_MARKER(0x800361A8, "SLUS_004.02");
    index = r_u16(object + 20);
    result = index < 92 ? (uint32)r_s8(0x800958C4 + index) : 0xFFFFFFFF;
    w_u16(object + 28, (uint16)result);
    return result;
}

uint32 t3_80051304(void)
{
    uint32 index;
    FUNCTION_MARKER(0x80051304, "SLUS_004.02");
    w_u8(0x800AFAA0, 0);
    for (index = 0; index < 2; ++index) {
        if (r_s16(0x800A9240 + index * 6284) == 20)
            w_u8(0x800AFAA0, (uint8)(r_u8(0x800AFAA0) | (1 << index)));
    }
    return 0;
}

uint32 t3_8002d1dc(uint32 character)
{
    sint32 value = (sint16)character;
    FUNCTION_MARKER(0x8002D1DC, "SLUS_004.02");
    return value < 21 ? (uint32)value : 20;
}

uint32 t3_8006c618(uint32 mode, uint32 first, uint32 second, uint32 third, uint32 fourth)
{
    FUNCTION_MARKER(0x8006C618, "SLUS_004.02");
    w_u16(0x800A08B0, (uint16)mode);
    w_u16(0x800A08A8, (uint16)first);
    w_u16(0x800A08AA, (uint16)second);
    w_u16(0x800A08AC, (uint16)third);
    w_u16(0x800A08AE, (uint16)fourth);
    return 0x800A0000;
}

uint32 t3_8002d2c8(uint32 object)
{
    uint32 record;
    uint32 value;
    uint32 command;
    FUNCTION_MARKER(0x8002D2C8, "SLUS_004.02");
    record = r_u32(object + 32);
    command = r_u16(record);
    value = (uint32)r_s16(object + 20);
    while (command) {
        uint32 argument = r_u16(record + 2);
        if ((argument >> 8) == 11)
            value += argument & 255;
        record += 4;
        command = r_u16(record);
    }
    return (uint32)(sint32)(sint16)value;
}

uint32 t3_80075200(uint32 index)
{
    FUNCTION_MARKER(0x80075200, "SLUS_004.02");
    if (r_s16(0x800AE204) == 6)
        return 0xFFFFFFFF;
    return r_u32(0x800ADFF8 + index * 12);
}

uint32 t3_80035eec(uint32 index)
{
    FUNCTION_MARKER(0x80035EEC, "SLUS_004.02");
    if (r_s16(0x800AE204) == 6)
        return t3_bns_07_800d35c4(index);
    return t3_80079c54(index);
}

uint32 t3_80034db4(uint32 index)
{
    uint32 iteration;
    uint32 first = 0x8009BCF8 + index * 8;
    uint32 second = 0x800ADED8 + index * 8;
    uint32 source = 0x800AE040 + index * 4;
    FUNCTION_MARKER(0x80034DB4, "SLUS_004.02");
    for (iteration = 0; iteration < 2; ++iteration) {
        w_u32(first, r_u32(source));
        w_u32(second, r_u32(source + 12));
        source += 28;
        first += 4;
        second += 4;
    }
    return 0;
}

uint32 t3_80039e74(uint32 index)
{
    FUNCTION_MARKER(0x80039E74, "SLUS_004.02");
    return r_u32(0x80096C60 + (index + 1) * 28);
}

uint32 t3_80037c24(uint32 index, sint32 entry)
{
    FUNCTION_MARKER(0x80037C24, "SLUS_004.02");
    if (entry < 18)
        return 0;
    return r_u8(0x80095E10 + index * 6 + (uint32)entry - 18);
}

uint32 t3_80039e9c(uint32 index)
{
    FUNCTION_MARKER(0x80039E9C, "SLUS_004.02");
    return r_u32(0x80096C6C + (index + 1) * 28);
}

uint32 t3_80036294(uint32 object)
{
    uint32 index;
    FUNCTION_MARKER(0x80036294, "SLUS_004.02");
    index = (uint32)r_s16(object + 28);
    return index < 55 ? r_u8(0x80095950 + index) : 0;
}

uint32 t3_8004ea60(void)
{
    uint32 value;
    FUNCTION_MARKER(0x8004EA60, "SLUS_004.02");
    value = r_u16(0x800ADD50);
    w_u32(0x80097D38, value ? r_u8(0x800ADF3E) : 0);
    if (value < 1000) {
        w_u32(0x80097D3C, value);
        return 0x80090000;
    }
    w_u32(0x80097D3C, 999);
    return 999;
}

uint32 t3_8002a324(uint32 value)
{
    uint32 estimate = 1;
    uint32 scaled = value;
    uint32 previous;
    FUNCTION_MARKER(0x8002A324, "SLUS_004.02");
    if (!value)
        return 0;
    while (estimate < scaled) {
        estimate <<= 1;
        scaled >>= 1;
    }
    do {
        previous = estimate;
        estimate = (value / previous + previous) >> 1;
    } while (estimate < previous);
    return previous;
}

uint32 t3_8003cb48(void)
{
    FUNCTION_MARKER(0x8003CB48, "SLUS_004.02");
    w_u32(0x80096F2C, 0);
    w_u32(0x8009E5F8, 0);
    w_u32(0x8009E5FC, 0);
    w_u32(0x80096F30, 0);
    return 0x80090000;
}

uint32 t3_8006a370(uint32 object)
{
    uint32 state;
    uint32 index;
    uint32 destination;
    FUNCTION_MARKER(0x8006A370, "SLUS_004.02");
    state = r_u16(object + 24);
    index = r_u8(object + 30);
    destination = 0x800A05A8 + index * 4;
    if (state - 14 < 2) {
        w_u32(destination, 1);
        return 1;
    }
    w_u32(destination, 0);
    return destination;
}

uint32 t3_8006e958(uint32 value)
{
    uint32 object;
    FUNCTION_MARKER(0x8006E958, "SLUS_004.02");
    object = t3_8006f4f4();
    if (object == 0xFFFFFFFF)
        return 0xFFFFFFFF;
    w_u8(object + 12, 3);
    w_u32(object + 64, value);
    return 3;
}

uint32 t3_8006deac(const uint32 *table, uint32 origin_x, uint32 origin_y, uint32 x, uint32 y)
{
    uint32 delta_x = x - origin_x;
    uint32 delta_y;
    FUNCTION_MARKER(0x8006DEAC, "SLUS_004.02");
    if (!table[y * 6 + x])
        return 0x7FFF;
    delta_y = y - origin_y;
    if ((sint32)delta_x < 0)
        delta_x = 0 - delta_x;
    if ((sint32)delta_y < 0)
        delta_y = 0 - delta_y;
    return delta_x + delta_y;
}

uint32 t3_8006b54c(uint32 flags)
{
    sint32 current;
    uint32 begin;
    uint32 end;
    uint32 previous;
    uint32 saved_track;
    FUNCTION_MARKER(0x8006B54C, "SLUS_004.02");
    current = r_s16(0x800A0620);
    w_u16(0x800A061C, 4);
    w_u32(0x800A0634, 0xFFFFFFFF);
    if (current < 0) {
        if ((uint32)r_u16(0x800A05D8) - 5 >= 3)
            return 0;
        t3_8006bc98(0, -1);
        t3_file_audio_stop();
        w_u16(0x800A05D8, 1);
        w_u32(0x800A866C, 0);
        w_u16(0x800A061C, 4);
        return 0x800B0000;
    }
    t3_8006bc98(0, -1);
    if (flags & 1) {
        begin = r_u32(0x800A0610);
        end = r_u32(0x800A0614);
        saved_track = (uint32)r_s16(0x800A0620);
        previous = r_u32(0x800A0618);
        w_u32(0x800A0618, 0xFFFFFFFF);
        w_u32(0x800A0630, saved_track);
        w_u32(0x800A0624, begin);
        w_u32(0x800A0628, end);
        w_u32(0x800A062C, previous);
        t3_file_audio_status(&previous);
        w_u32(0x800A0618, previous);
        w_u32(0x800A062C, r_u32(0x800A0618));
        end = r_u32(0x800A0614);
        previous = r_u32(0x800A0618);
        if ((sint32)(end - 375) < (sint32)previous)
            w_u32(0x800A0630, 0xFFFFFFFF);
        else {
            previous = r_u32(0x800A0618);
            begin = r_u32(0x800A0610);
            if ((sint32)previous < (sint32)begin)
                w_u32(0x800A0630, 0xFFFFFFFF);
        }
    }
    else
        w_u32(0x800A0630, 0xFFFFFFFF);
    w_u16(0x800A0620, 0xFFFF);
    t3_file_audio_stop();
    w_u16(0x800A05D8, 1);
    w_u32(0x800A866C, 0);
    return 0x800B0000;
}

uint32 t3_800697d4(sint32 channel)
{
    uint32 index;
    FUNCTION_MARKER(0x800697D4, "SLUS_004.02");
    if ((((channel & 1) && r_s16(0x800A0510) == 14) ||
         ((channel & 2) && r_s16(0x800A0518) == 14)) &&
        (r_s16(0x800A9240) == 20 || r_s16(0x800AAACC) == 20))
        t3_8006c588(3);
    for (index = 0; index < 3; ++index) {
        if (((uint32)channel >> index) & 1) {
            uint32 state = 0x800A0510 + index * 8;
            w_u16(state, 0xFFFF);
            w_u16(state + 2, 0);
            w_u32(state + 4, 0);
        }
    }
    return 0;
}

uint32 t3_8004c684(uint32 buffer_index)
{
    uint32 buffer = 0x800A8D78 + buffer_index * 472;
    FUNCTION_MARKER(0x8004C684, "SLUS_004.02");
    w_u32(0x800ADF64, buffer + 72);
    w_u32(0x800A9218, buffer);
    w_u32(0x800ADD54, buffer + 36);
    t3_8007c7c8(buffer, 9);
    return t3_8007c7c8(r_u32(0x800ADD54), 9);
}

uint32 t3_80029894(void)
{
    uint32 event;
    FUNCTION_MARKER(0x80029894, "SLUS_004.02");
    if (!gpu_bind_soft_reset(0x80098CB0, 0x80098CB4))
        return t3_8007ae58(1);
    w_u32(0x8009BC5C, 0);
    w_u32(0x800ADCA4, 0);
    w_u32(0x80095428, 0);
    w_u32(0x8009542C, 1);
    t3_80079e58();
    event = t3_80079e18(0xF2000002, 2, 0x1000, 0x80029784);
    w_u32(0x8009BC64, event);
    t3_80079e48(event);
    t3_80079fcc(0xF2000002);
    t3_80079e68();
    return t3_80085c58(0x800299E0);
}

uint32 t3_80029784(void)
{
    uint32 frame;
    uint32 count;
    uint32 ticks;
    FUNCTION_MARKER(0x80029784, "SLUS_004.02");
    w_u32(0x80095424, 0);
    ticks = r_u32(0x80095434);
    frame = r_u32(0x8009BC5C);
    w_u32(0x80095434, ticks + 1);
    if (!frame) {
        ResetGraph(1);
        if (!r_u32(0x800A866C)) {
            GsSwapDispBuff();
            t3_800294c8();
        }
        t3_800817c8(r_u32(0x8009BC60));
    }
    else if (!r_u32(0x800A866C) && !r_u32(0x80095428)) {
        ResetGraph(1);
        t3_800817c8(r_u32(0x8009BC60));
    }
    SsSeqCalledTbyT();
    t3_800291d8();
    t3_80029dc0(r_u32(0x8009BC5C) ? 2 : 0);
    frame = r_u32(0x8009BC5C);
    count = r_u32(0x80097F00);
    w_u32(0x80097F00, count + 1);
    w_u32(0x8009BC5C, frame + 1);
    return SetRCnt(0xF2000002, 0xFFFF, 0x2000);
}

uint32 t3_800294c8(void)
{
    uint32 command = 0x8009BA50;
    uint32 payload = 0x8009BA54;
    uint32 kind;
    FUNCTION_MARKER(0x800294C8, "SLUS_004.02");
    w_u16(r_u32(0x8009BC50), 0);
    while ((kind = r_u16(command)) != 0) {
        if (kind == 1) {
            uint32 rectangle = command + 8;
            uint32 pixels = r_u32(payload);
            command += 16;
            payload += 16;
            t3_8007c650(rectangle, pixels);
        }
        else if (kind == 2) {
            uint32 rectangle = payload;
            uint32 x = r_u16(payload + 8);
            uint32 y = r_u16(payload + 10);
            command += 16;
            payload += 16;
            t3_8007c710(rectangle, x, y);
        }
        else if (kind == 3) {
            t3_8007c5b8(payload, 0, 0, 0);
            payload += 12;
            command += 12;
        }
    }
    w_u32(0x8009BC50, 0x8009BA50);
    w_u32(0x8009BC54, 0);
    return 0x800A0000;
}

void t3_800291d8(void)
{
    uint32 group;
    uint32 index;
    uint32 first;
    uint32 second;
    FUNCTION_MARKER(0x800291D8, "SLUS_004.02");
    if (!r_u8(0x8009BA38)) {
        for (group = 0; group < 2; ++group) {
            for (index = 0; index < 4; ++index) {
                uint32 record = 0x8009B9B0 + group * 32 + index * 8;
                w_u32(record, 0);
                w_u16(record + 6, 0);
                w_u8(record + 4, 0);
                w_u8(record + 5, 0);
            }
        }
        for (group = 0; group < 2; ++group) {
            for (index = 0; index < 4; ++index) {
                uint32 record = 0x8009B9F4 + group * 32 + index * 8;
                w_u32(record, 0);
                w_u16(record + 6, 0);
                w_u8(record + 4, 0);
                w_u8(record + 5, 0);
            }
        }
        w_u8(0x8009BA38, 1);
    }
    t3_80028fd4(0x8009BA40, 0x8009B9B0);
    t3_80028fd4(0x8009BA48, 0x8009B9F4);
    first = 0;
    second = 0;
    if (r_u16(0x800AE1F8)) {
        first = (uint32)r_s16(0x8009B9F0);
        second = (uint32)r_s16(0x8009B9F2);
    }
    t3_8002a128(0, first, second);
    first = 0;
    second = 0;
    if (r_u16(0x800AE1FA)) {
        first = (uint32)r_s16(0x8009BA34);
        second = (uint32)r_s16(0x8009BA36);
    }
    t3_8002a128(1, first, second);
}

uint32 t3_80028fd4(uint32 commands, uint32 state)
{
    uint32 index;
    uint32 cursor;
    uint32 offset;
    uint32 output;
    FUNCTION_MARKER(0x80028FD4, "SLUS_004.02");
    for (index = 0; index < 4; ++index) {
        uint32 command = r_u16(commands);
        if (command) {
            uint32 table = 0x80095334 + command * 8;
            uint32 sequence = r_u32(table);
            uint32 record = state + index * 8;
            if (r_u16(sequence) & 0xF000) {
                w_u32(record, sequence);
                w_u16(record + 6, 0);
            }
            sequence = r_u32(table + 4);
            if (r_u16(sequence) & 0xF000) {
                w_u32(record + 32, sequence);
                w_u16(record + 38, 0);
            }
            w_u16(commands, 0);
        }
        commands += 2;
    }
    cursor = state;
    offset = 0;
    do {
        uint32 sequence = r_u32(cursor);
        uint32 record;
        w_u8(cursor + 4, 0);
        if (sequence) {
            uint32 command = r_u16(sequence);
            uint32 frame = r_u32(0x80097F00);
            uint32 pattern = r_u8(0x80095414 + ((command & 0xF00) >> 8));
            uint32 count = r_u16(cursor + 6);
            w_u8(cursor + 4, 1);
            w_u16(cursor + 6, (uint16)(count + 1));
            count = r_u16(cursor + 6);
            w_u8(cursor + 5, (uint8)((pattern & (1 << (frame & 7))) != 0));
            if (count >= (command & 255)) {
                w_u16(cursor + 6, 0);
                w_u32(cursor, (command & 0xF000) == 0x1000 ? sequence + 2 : 0);
            }
        }
        record = state + offset + 32;
        sequence = r_u32(record);
        w_u8(record + 4, 0);
        if (sequence) {
            uint32 command = r_u16(sequence);
            uint32 count = r_u16(record + 6);
            w_u8(record + 4, 1);
            w_u16(record + 6, (uint16)(count + 1));
            count = r_u16(record + 6);
            w_u8(record + 5, (uint8)(((command & 0xF00) >> 4) + 15));
            if (count >= (command & 255)) {
                w_u16(record + 6, 0);
                w_u32(record, (command & 0xF000) == 0x1000 ? sequence + 2 : 0);
            }
        }
        cursor += 8;
        offset += 8;
    } while ((sint32)cursor < (sint32)(state + 32));
    output = 0;
    for (index = 0; index < 4; ++index) {
        uint32 record = state + index * 8;
        if (r_u8(record + 4))
            output = r_u8(record + 5);
    }
    w_u16(state + 64, (uint16)output);
    output = 0;
    for (index = 0; index < 4; ++index) {
        uint32 record = state + index * 8 + 32;
        if (r_u8(record + 4))
            output = r_u8(record + 5);
    }
    w_u16(state + 66, (uint16)output);
    return 0;
}

void t3_80029dc0(uint32 mode)
{
    uint32 index;
    uint32 buttons;
    uint32 mapped;
    uint32 previous;
    uint32 edges;
    uint32 second_buttons;
    uint32 second_previous;
    uint32 repeated;
    FUNCTION_MARKER(0x80029DC0, "SLUS_004.02");
    t3_80029c10(0x800A9130);
    t3_80029c10(0x800A915A);
    if ((sint32)mode >= 2)
        return;
    buttons = r_u8(0x800A9158);
    w_u8(0x800A9184, 0);
    if (buttons)
        w_u8(0x800A9184, 1);
    if (r_u8(0x800A9182))
        w_u8(0x800A9184, (uint8)(r_u8(0x800A9184) | 2));
    buttons = r_u16(0x800A9130);
    mapped = 0;
    for (index = 0; index < 16; ++index) {
        uint32 mapping = r_u16(0x80097EB0 + index * 2);
        if (buttons & 1)
            mapped |= mapping;
        buttons >>= 1;
    }
    if ((mapped & 0x5000) == 0x5000)
        mapped &= 0xAFFF;
    if ((mapped & 0xA000) == 0xA000)
        mapped &= 0x5FFF;
    previous = r_u16(0x800A8C48);
    w_u16(0x800A8C48, (uint16)mapped);
    buttons = r_u16(0x800A915A);
    w_u16(0x800ADC78, (uint16)((mapped ^ previous) & mapped));
    mapped = 0;
    for (index = 0; index < 16; ++index) {
        uint32 mapping = r_u16(0x80097ED0 + index * 2);
        if (buttons & 1)
            mapped |= mapping;
        buttons >>= 1;
    }
    if ((mapped & 0x5000) == 0x5000)
        mapped &= 0xAFFF;
    if ((mapped & 0xA000) == 0xA000)
        mapped &= 0x5FFF;
    previous = r_u16(0x800A8C4A);
    w_u16(0x800A8C4A, (uint16)mapped);
    w_u16(0x800ADC7A, (uint16)((mapped ^ previous) & mapped));
    if (mode)
        return;
    repeated = 0;
    buttons = r_u16(0x800A9130);
    previous = r_u16(0x800ADD68);
    edges = (previous ^ buttons) & buttons;
    w_u16(0x800ADD68, (uint16)buttons);
    w_u16(0x800ADF08, (uint16)edges);
    for (index = 0; index < 16; ++index) {
        uint32 counter = 0x8009BC68 + index;
        repeated <<= 1;
        if (!(buttons & 0x8000))
            w_u8(counter, 0);
        else if (edges & 0x8000) {
            w_u8(counter, 1);
            repeated |= 1;
        }
        else {
            uint32 old = r_u8(counter);
            uint32 next = old + 1;
            w_u8(counter, (uint8)next);
            if ((next & 255) >= 20) {
                repeated |= (next & 3) == 0;
                w_u8(counter, (uint8)(old - 3));
            }
        }
        buttons <<= 1;
        edges <<= 1;
    }
    second_buttons = r_u16(0x800A915A);
    second_previous = r_u16(0x800ADD6A);
    w_u16(0x800ADF00, (uint16)repeated);
    edges = (second_previous ^ second_buttons) & second_buttons;
    w_u16(0x800ADD6A, (uint16)second_buttons);
    w_u16(0x800ADF0A, (uint16)edges);
    repeated = 0;
    for (index = 0; index < 16; ++index) {
        uint32 counter = 0x8009BC78 + index;
        repeated <<= 1;
        if (!(second_buttons & 0x8000))
            w_u8(counter, 0);
        else if (edges & 0x8000) {
            w_u8(counter, 1);
            repeated |= 1;
        }
        else {
            uint32 old = r_u8(counter);
            uint32 next = old + 1;
            w_u8(counter, (uint8)next);
            if ((next & 255) >= 20) {
                repeated |= (next & 3) == 0;
                w_u8(counter, (uint8)(old - 3));
            }
        }
        second_buttons <<= 1;
        edges <<= 1;
    }
    w_u16(0x800ADF02, (uint16)repeated);
}
