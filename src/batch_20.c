#include "game_functions.h"
#include "t3_audio.h"
#include "t3_data.h"
#include "psx_gpu.h"
#include <string.h>

uint32 t3_bns_08_801004a8(uint32 descriptor)
{
    uint32 mask;
    uint32 kind;

    FUNCTION_MARKER(0x801004A8, "SLUS_004.02-BNS-logical-08");
    mask = r_u32(descriptor + 8);
    if (mask == 0)
        return (uint32)r_s16(descriptor);
    kind = r_u32(descriptor + 12);
    switch (kind) {
    case 0:
        if (r_u32(0x80097EF8) & mask)
            return (uint32)r_s16(descriptor);
        return 0xFFFFFFFF;
    case 1:
        if (r_u32(0x80097EFC) & 0x800)
            return (uint32)r_s16(descriptor + 2);
        if (r_u32(0x80097EF8) & 0x800)
            return (uint32)r_s16(descriptor);
        return 0xFFFFFFFF;
    case 2:
        if (r_u32(0x80097EFC) & 0x10000)
            return (uint32)r_s16(descriptor + 2);
        if (r_u32(0x80097EF8) & 0x10000)
            return (uint32)r_s16(descriptor);
        return 0xFFFFFFFF;
    case 3:
        if (r_u32(0x80097EFC) & 0x100)
            return (uint32)r_s16(descriptor);
        return 0xFFFFFFFF;
    default:
        return t3_8007ae58(1);
    }
}

uint32 t3_bns_08_80100f58(void)
{
    uint32 index;

    FUNCTION_MARKER(0x80100F58, "SLUS_004.02-BNS-logical-08");
    for (index = 0; index < 26; ++index) {
        if (t3_bns_08_801004a8(0x800BA160 + index * 28) == 0xFFFFFFFF)
            return 0;
    }
    return 1;
}

uint32 t3_bns_08_8010215c(void)
{
    FUNCTION_MARKER(0x8010215C, "SLUS_004.02-BNS-logical-08");
    w_u8(0x800ADC96, 0);
    w_u8(0x800ADC97, 0);
    w_u16(0x800ADC98, 0);
    w_u16(0x800ADC9A, 0);
    w_u16(0x800ADC9E, 0);
    return 0x800ADC90;
}

uint32 t3_bns_08_80102740(void)
{
    uint32 original_selection;
    uint32 selection;
    uint32 pressed_first;
    uint32 pressed;
    uint32 held_first;
    uint32 held;
    uint32 count;
    uint32 scroll;
    uint32 kind;
    uint32 table;
    sint32 original_mode;
    sint32 mode;

    FUNCTION_MARKER(0x80102740, "SLUS_004.02-BNS-logical-08");
    original_selection = r_u8(0x800ADC96);
    pressed_first = r_u16(0x800ADF08);
    pressed = pressed_first | r_u16(0x800ADF0A);
    held_first = r_u16(0x800ADF00);
    held = held_first | r_u16(0x800ADF02);
    original_mode = r_s16(0x800ADC9A);
    selection = original_selection;
    count = r_u8(0x800ADC93);
    if (original_mode == 0) {
        if (held & 0x2000) {
            w_u16(0x800ADC9A, 2);
        } else if (held & 0x1000) {
            if (original_selection != 0) {
                scroll = r_u8(0x800ADC97);
                selection = original_selection - 1;
                if (selection < scroll)
                    w_u8(0x800ADC97, scroll - 1);
            }
        } else if ((held & 0x4000) && (sint32)original_selection < (sint32)count - 1) {
            scroll = r_u8(0x800ADC97);
            selection = original_selection + 1;
            if (selection >= scroll + 14)
                w_u8(0x800ADC97, scroll + 1);
        }
    } else if (held & 0x8000) {
        w_u16(0x800ADC9A, 0);
    } else if (held & 0x1000) {
        if (original_mode == 1)
            w_u16(0x800ADC9A, 3);
        else if (original_mode == 3)
            w_u16(0x800ADC9A, 4);
        else if (original_mode == 4)
            w_u16(0x800ADC9A, 2);
    } else if (held & 0x4000) {
        if (original_mode == 2)
            w_u16(0x800ADC9A, 4);
        else if (original_mode == 3)
            w_u16(0x800ADC9A, 1);
        else if (original_mode == 4)
            w_u16(0x800ADC9A, 3);
    }
    if (r_s16(0x800ADC9C) == 0 && r_s16(0x800ADC9A) == 3)
        w_u16(0x800ADC9A, (uint32)original_mode);
    w_u8(0x800ADC96, selection);
    if (selection != original_selection || r_s16(0x800ADC9A) != original_mode)
        t3_800753bc(0, 0x546C, 0);
    if ((pressed & 0x8F0) == 0) {
        if ((pressed & 0x100) == 0)
            return 0;
        t3_800753bc(0, 0x4CEB, 0);
        return 4;
    }
    mode = r_s16(0x800ADC9A);
    if (mode == 0) {
        kind = r_u8(0x800ADC91);
        if (kind == 1 || kind == 2 || kind == 3) {
            if (kind == 1)
                table = 0x800BB55C;
            else if (kind == 2)
                table = 0x800BB19C;
            else
                table = 0x800BADAC;
            selection = r_u8(0x800ADC96);
            w_u8(0x800ADC95, r_u8(table + selection * 28));
        }
        if (r_u8(0x800ADC90) == 3)
            w_u8(0x800ADC95, r_u8(0x800ADC95) | 1);
        t3_8006b54c(0);
        return 1;
    }
    switch (mode) {
    case 1:
        t3_800753bc(0, 0x4CEB, 0);
        return 4;
    case 2:
        t3_8006b54c(0);
        t3_800753bc(0, 0x4CEB, 0);
        w_u8(0x800ADC96, 0);
        w_u8(0x800ADC97, 0);
        w_u16(0x800ADC98, 0);
        w_u16(0x800ADC9A, 0);
        w_u16(0x800ADC9E, 0);
        w_u8(0x800ADC90, 1);
        return 0;
    case 3:
        t3_800753bc(0, 0x4CEB, 0);
        return 5;
    case 4:
        t3_8006b54c(0);
        t3_800753bc(0, 0x4CEB, 0);
        w_u8(0x800ADC90, r_u8(0x800ADC90) ^ 1);
        return 0;
    default:
        return 0;
    }
}

uint32 t3_bns_08_801021bc(uint32 incoming_table)
{
    uint32 pressed;
    uint32 held;
    uint32 first;
    uint32 count;
    uint32 previous;
    uint32 kind;
    uint32 table;
    uint32 selection;
    uint32 column;
    uint32 row;
    uint32 scroll;
    uint32 saved_kind;
    sint32 previous_mode;
    sint32 mode;
    sint32 movie;

    FUNCTION_MARKER(0x801021BC, "SLUS_004.02-BNS-logical-08");
    first = r_u16(0x800ADF08);
    pressed = first | r_u16(0x800ADF0A);
    first = r_u16(0x800ADF00);
    held = first | r_u16(0x800ADF02);
    count = r_u8(0x800ADC92);
    previous = r_u8(0x800ADC96);
    kind = r_u8(0x800ADC91);
    previous_mode = r_s16(0x800ADC9A);
    table = incoming_table;
    if (kind == 1)
        table = 0x800BABAC;
    else if (kind == 2)
        table = 0x800BA648;
    else if (kind == 3)
        table = 0x800BA160;
    selection = r_u8(0x800ADC96);
    column = selection % 6;
    row = selection / 6;
    mode = r_s16(0x800ADC9A);
    if (mode == 0) {
        if (held & 0x8000) {
            if (column != 0 && (sint32)t3_bns_08_801004a8(table + (column + row * 6 - 1) * 28) >= -1)
                --column;
        } else if ((held & 0x2000) && column < 5) {
            if ((sint32)t3_bns_08_801004a8(table + (column + row * 6 + 1) * 28) >= -1)
                ++column;
        }
        if (held & 0x1000) {
            if ((sint32)row > 0)
                --row;
            scroll = r_u8(0x800ADC97);
            if ((sint32)row < (sint32)scroll)
                w_u8(0x800ADC97, scroll - 1);
        } else if (held & 0x4000) {
            ++row;
            if ((sint32)row < (sint32)(count / 6)) {
                if ((sint32)t3_bns_08_801004a8(table + (column + row * 6) * 28) < -1) {
                    if ((sint32)column < 3) {
                        while ((sint32)t3_bns_08_801004a8(table + (column + row * 6) * 28) < -1)
                            ++column;
                    } else {
                        while ((sint32)t3_bns_08_801004a8(table + (column + row * 6) * 28) < -1)
                            --column;
                    }
                }
            } else {
                w_u16(0x800ADC9A, r_s16(0x800ADC9C) != 0 ? 2 : 1);
                --row;
            }
            scroll = r_u8(0x800ADC97);
            if ((sint32)row >= (sint32)scroll + 4)
                w_u8(0x800ADC97, scroll + 1);
        }
    } else {
        if (r_s16(0x800ADC9C) != 0) {
            if (held & 0x8000) {
                if (mode == 2)
                    w_u16(0x800ADC9A, 3);
                else if (mode == 1)
                    w_u16(0x800ADC9A, 2);
            } else if (held & 0x2000) {
                if (mode == 2)
                    w_u16(0x800ADC9A, 1);
                else if (mode == 3)
                    w_u16(0x800ADC9A, 2);
            }
        }
        if (held & 0x1000)
            w_u16(0x800ADC9A, 0);
    }
    selection = column + row * 6;
    if ((sint32)selection < 0)
        selection = count - 1;
    else if ((sint32)selection >= (sint32)count)
        selection = 0;
    w_u8(0x800ADC96, selection);
    if (selection != previous || r_s16(0x800ADC9A) != previous_mode)
        t3_800753bc(0, 0x546C, 0);
    if ((pressed & 0x8F0) == 0) {
        if ((pressed & 0x100) == 0)
            return 0;
        t3_800753bc(0, 0x4CEB, 0);
        return 4;
    }
    mode = r_s16(0x800ADC9A);
    if (mode == 0) {
        kind = r_u8(0x800ADC91);
        movie = -1;
        if (kind == 1 || kind == 2 || kind == 3) {
            if (kind == 1)
                table = 0x800BABAC;
            else if (kind == 2)
                table = 0x800BA648;
            else
                table = 0x800BA160;
            selection = r_u8(0x800ADC96);
            movie = (sint32)t3_bns_08_801004a8(table + selection * 28);
        }
        if (movie < 0)
            return 0;
        t3_8006b54c(0);
        w_u8(0x800ADC94, (uint32)movie);
        return 1;
    }
    switch (mode) {
    case 1:
        t3_800753bc(0, 0x4CEB, 0);
        return 4;
    case 2:
        t3_800753bc(0, 0x4CEB, 0);
        saved_kind = r_u8(0x800ADC91);
        w_u8(0x800ADC96, 0);
        w_u8(0x800ADC97, 0);
        w_u16(0x800ADC98, 0);
        w_u16(0x800ADC9A, 0);
        w_u16(0x800ADC9E, 0);
        w_u8(0x800ADC90, 2);
        t3_bns_08_80102ae4(saved_kind * 2 - 2);
        t3_bns_08_80102ae4(r_u8(0x800ADC91) * 2u - 1);
        t3_bns_08_80102ae4(6);
        t3_bns_08_80102ae4(7);
        return 0;
    case 3:
        t3_800753bc(0, 0x4CEB, 0);
        return 5;
    default:
        return 0;
    }
}

uint32 t3_bns_08_8010217c(uint32 incoming_table)
{
    FUNCTION_MARKER(0x8010217C, "SLUS_004.02-BNS-logical-08");
    if (r_u8(0x800ADC90) == 1)
        return t3_bns_08_801021bc(incoming_table);
    return t3_bns_08_80102740();
}

uint32 t3_bns_08_80102ae4(uint32 index)
{
    uint32 descriptor;
    uint32 y;
    sint32 kind;
    sint32 width;
    T3_TIM_IMAGE image;

    FUNCTION_MARKER(0x80102AE4, "SLUS_004.02-BNS-logical-08");
    descriptor = 0x800B9C6C + index * 14;
    t3_80029638(1);
    if (t3_8007efc8(r_u32(0x8010E204 + index * 4)) == 0xFFFFFFFF)
        t3_8007ae58(1);
    t3_8007efd8(&image);
    w_u32(0x80184CD8, image.mode);
    w_u32(0x80184CDC, image.clut_rectangle);
    w_u32(0x80184CE0, image.clut_pixels);
    w_u32(0x80184CE4, image.image_rectangle);
    w_u32(0x80184CE8, image.image_pixels);
    kind = r_s16(descriptor + 12);
    if (kind == 0 || kind == 1 || kind == 2) {
        w_u16(0x80184CF8, r_u16(descriptor));
        y = r_u16(descriptor + 2);
        w_u16(0x80184CFC, kind == 1 ? 256 : 64);
        w_u16(0x80184CFE, 1);
        w_u16(0x80184CFA, y);
        w_u16(0x80184D00, r_u16(descriptor + 4));
        w_u16(0x80184D02, r_u16(descriptor + 6));
        width = r_s16(descriptor + 8);
        w_u16(0x80184D04, (uint32)(width / (kind == 1 ? 2 : 4)));
        w_u16(0x80184D06, r_u16(descriptor + 10));
    }
    t3_8007c650(0x80184CF8, r_u32(0x80184CE0));
    DrawSync(0);
    t3_8007c650(0x80184D00, r_u32(0x80184CE8));
    DrawSync(0);
    return t3_80029638(0);
}

uint32 t3_bns_08_80102ca8(void)
{
    uint32 first;
    uint32 phase;
    uint32 center;
    uint32 offset;
    uint32 text;
    uint32 mode;
    uint32 kind;
    uint32 color;
    uint32 length;
    uint32 x;

    FUNCTION_MARKER(0x80102CA8, "SLUS_004.02-BNS-logical-08");
    first = r_u16(0x800ADD68);
    if ((first | r_u16(0x800ADD6A)) != 0)
        w_u16(0x800ADC9E, 0);
    else
        w_u16(0x800ADC9E, r_u16(0x800ADC9E) + 1);
    phase = 0;
    if (r_u16(0x800ADC9E) >= 480)
        phase = r_u16(0x800ADC9E) - 480;
    center = (phase & 0x100) != 0 ? 552 : 184;
    if ((phase & 0xE0) == 0xE0) {
        offset = ((phase & 31) * 23) >> 1;
        if (phase & 0x100)
            center -= offset;
        else
            center += offset;
    }
    text = t3_8004cc08(0x80184D08, 0x80097D08);
    mode = r_u8(0x800ADC90);
    if (mode == 1 || mode == 2 || mode == 3) {
        kind = r_u8(0x800ADC91);
        offset = mode == 1 ? 0x8010E21C : mode == 2 ? 0x8010E220 : 0x8010E224;
        color = mode == 2 ? 1 : 6;
        t3_8004cc08(text, r_u32(offset + kind * 12));
    }
    length = t3_8007a418(text);
    x = center - (uint32)((sint32)(length * 13) >> 1);
    if (mode == 1 || mode == 2 || mode == 3)
        t3_8004ce74(0x80184D08, 6u, 1u, x, 34u, color);
    else
        t3_8004ce74(0x80184D08, 6u, 1u, x, 34u);
    t3_8004cc08(text, r_u32(0x8010E224));
    length = t3_8007a418(text);
    x = center - (uint32)((sint32)(length * 9) >> 1) - 368;
    return t3_8004ce74(0x80184D08, 6u, 0u, x, 34u, 5u, 6u, 5u);
}


uint32 t3_bns_08_800ff8e0(sint32 frame)
{
    sint32 start;
    uint32 index;
    uint32 cursor;

    FUNCTION_MARKER(0x800FF8E0, "SLUS_004.02-BNS-logical-08");
    start = r_s16(0x80184CC2);
    if (frame <= 0)
        return 0x80180000;
    if (start < 0)
        return 0x800C0000;
    index = (uint32)start;
    cursor = 0x800B978C + index * 4;
    while ((sint32)r_u16(cursor) < frame) {
        cursor += 4;
        ++index;
    }
    return t3_bns_08_800ff7c0(r_s16(0x800B978C + (index - 1) * 4 + 2));
}

uint32 t3_bns_08_801005d4(uint32 kind, uint32 *entry, uint32 *table, sint32 track, uint32 variation)
{
    uint32 selection;
    uint32 data;

    FUNCTION_MARKER(0x801005D4, "SLUS_004.02-BNS-logical-08");
    if (kind == 1) {
        selection = 0x800B9FF0;
        data = 0x800B9F7C;
    } else if (kind == 2) {
        selection = 0x800B9F7C;
        data = 0x800B9CDC;
    } else {
        return 2;
    }
    *entry = selection + (variation << 1) + ((uint32)track << 2);
    *table = data;
    return data;
}

uint32 t3_8006b194(sint32 selection, uint32 prepare)
{
    uint32 busy;
    uint32 variation;
    sint32 track;
    uint32 source_kind;
    uint32 entry;
    uint32 table;
    uint32 stream;
    uint32 row;
    uint32 begin;
    sint32 channel;
    const char *path;
    char pcm_path[64];

    FUNCTION_MARKER(0x8006B194, "SLUS_004.02");
    busy = t3_8006c23c();
    variation = (uint32)selection & 1;
    if (busy) {
        if (prepare)
            return busy;
        if (!t3_bns_pump()) {
            xport_message_error("Tekken 3", "Cannot complete BNS resource read");
            return 0xFFFFFFFF;
        }
    }
    track = selection / 2;
    source_kind = r_u8(0x800ADC91);
    if (source_kind == 1 || source_kind == 2) {
        t3_bns_08_801005d4(source_kind, &entry, &table, track, variation);
    } else if (source_kind == 3) {
        entry = 0x80024FD0 + ((uint32)track << 2) + (variation << 1);
        table = 0x80024D78;
    } else {
        return t3_8007ae58(1);
    }
    w_u16(0x800A0622, prepare ? track : 0xFFFF);
    w_u16(0x800A05DE, 600);
    stream = r_u8(entry);
    t3_file_audio_stop();
    t3_8006bc98(0, -1);
    if (!(stream & 0x80)) {
        row = table + stream * 12;
        source_kind = r_u8(0x800ADC91);
        channel = r_s16(row + 8) & 255;
        begin = r_u32(row);
        w_u32(0x800A0610, begin);
        w_u16(0x800A061E, 0);
        w_u32(0x800A0614, r_u32(row + 4));
        w_u16(0x800A068A, r_u8(entry + 1) & 0x7F);
        path = source_kind == 2 ? "DATA/TEKKEN2/TEKKEN2.XAS" :
            source_kind == 3 ? "DATA/TEKKEN3/TEKKEN3.XAS" : NULL;
        if (prepare) {
            w_u16(0x800A061C, 0);
            t3_8006bc98(0, -1);
            if (!t3_file_audio_prepare(path, r_u32(0x800A0610), r_u32(0x800A0614))) {
                t3_8006b54c(0);
                xport_message_error("Tekken 3", "Cannot prepare XA audio file from DATA");
                return 0xFFFFFFFF;
            }
        } else {
            w_u16(0x800A0620, track);
            w_u16(0x800A061C, 0);
            t3_8006bc98(127, -1);
            if (!t3_file_audio_play(path, r_u32(0x800A0610), r_u32(0x800A0614), channel, -1)) {
                t3_8006b54c(0);
                xport_message_error("Tekken 3", "Cannot play XA audio file from DATA");
                return 0xFFFFFFFF;
            }
            w_u16(0x800A05D8, 2);
            w_u32(0x800A866C, 0);
        }
        w_u32(0x800A0618, 0);
    } else {
        w_u16(0x800A061E, 0);
        w_u16(0x800A068A, r_u8(entry + 1) & 0x7F);
        if (!prepare) {
            t3_8006b54c(0);
            stream &= 0x7F;
            w_u32(0x800A0634, (uint32)r_s16(0x80025050 + stream * 2));
            if (source_kind == 1 || source_kind == 2) {
                snprintf(pcm_path, sizeof(pcm_path), "DATA/TEKKEN%u/TRACK_%02u.PCM", source_kind, stream);
                path = pcm_path;
            } else {
                path = stream == 2 ? "DATA/TEKKEN3/TEKKEN3.DA" :
                    stream == 3 ? "DATA/TEKKEN3/TEKKEN3.DMY" : NULL;
            }
            if (!t3_file_audio_play(path, 0, 0, -1, -1)) {
                t3_8006b54c(0);
                xport_message_error("Tekken 3", "Cannot play extracted PCM track from DATA");
                return 0xFFFFFFFF;
            }
            w_u16(0x800A0620, track);
            w_u16(0x800A061C, 2);
            t3_8006bc98(127, -1);
            w_u16(0x800A05D8, 4);
            w_u32(0x800A866C, 0);
        }
    }
    w_u32(0x800A062C, 0xFFFFFFFF);
    w_u32(0x800A0630, 0xFFFFFFFF);
    return 0x800A05D8;
}

uint32 t3_8006b524(uint32 begin, uint32 end)
{
    FUNCTION_MARKER(0x8006B524, "SLUS_004.02");
    w_u32(0x800A0618, begin);
    w_u32(0x800A0610, begin);
    w_u32(0x800A0614, end);
    w_u16(0x800A061E, 0);
    return 0x800A05D8;
}

uint32 t3_bns_08_800fefd4(void)
{
    uint32 controller;
    uint32 actor;
    sint32 character;
    uint32 variation;
    uint32 movie;

    FUNCTION_MARKER(0x800FEFD4, "SLUS_004.02-BNS-logical-08");
    controller = r_u16(0x800ADF10);
    w_u8(0x800ADC90, 0);
    if (!controller || controller >= 3) {
        w_u8(0x800ADC90, 1);
        return 0;
    }
    actor = 0x800A799C + controller * 6284;
    character = r_s16(actor + 20) / 4;
    variation = r_u16(actor + 20) & 3;
    if ((uint32)character >= 21) {
        variation = 0;
        character = 0;
    }
    movie = r_u8(0x800B95DC + (uint32)character * 4 + variation);
    if (character == 16 && (r_u32(0x80097EFC) & 0x10000))
        movie = 24;
    return movie;
}

void t3_bns_08_80105340(uint32 source, uint32 destination)
{
    uint32 count = 3840;

    FUNCTION_MARKER(0x80105340, "SLUS_004.02-BNS-logical-08");
    do {
        uint32 value = r_u32(source);
        uint32 byte0 = r_u8(0x80105240 + (value & 255));
        uint32 byte1 = r_u8(0x80105240 + ((value >> 8) & 255));
        uint32 byte2 = r_u8(0x80105240 + ((value >> 16) & 255));
        uint32 byte3 = r_u8(0x80105240 + (value >> 24));
        source += 4;
        w_u32(destination, byte0 | (byte1 << 8) | (byte2 << 16) | (byte3 << 24));
        destination += 4;
    } while (--count);
}

uint32 t3_bns_08_801053c0(uint32 scale, uint32 mode)
{
    uint32 index;

    FUNCTION_MARKER(0x801053C0, "SLUS_004.02-BNS-logical-08");
    if (mode > 1)
        return 255;
    if (mode)
        scale = 0 - scale;
    index = 255;
    do {
        uint32 value = (index * scale) >> 6;
        if (mode)
            value += 255;
        w_u8(0x80105240 + index, value);
    } while (index--);
    return 0xFFFFFFFF;
}

void t3_bns_08_80105444(uint32 pixels)
{
    // Preserve the original inclusive counter and final read
    uint32 count = 57601;
    uint32 value;

    FUNCTION_MARKER(0x80105444, "SLUS_004.02-BNS-logical-08");
    value = r_u32(pixels);
    do {
        w_u32(pixels, 0xFFFFFFFF - value);
        pixels += 4;
        value = r_u32(pixels);
    } while (--count);
}

void t3_bns_08_800fda2c(uint32 unused, uint32 white)
{
    uint32 first;
    uint32 second;
    uint32 pixels;
    uint32 row;
    uint32 column;
    uint32 color;
    PSX_RECT rectangle;
    DISPENV display;

    FUNCTION_MARKER(0x800FDA2C, "SLUS_004.02-BNS-logical-08");
    (void)unused;
    first = r_u32(0x800B95BC);
    second = r_u32(0x800B95C0);
    rectangle.x = (sint16)first;
    rectangle.y = (sint16)(first >> 16);
    rectangle.w = (sint16)second;
    rectangle.h = (sint16)(second >> 16);
    pixels = 0x801144B8;
    for (row = 0; row < 480; ++row) {
        color = white ? 0xFFFFFFFF : 0;
        for (column = 0; column < 240; ++column) {
            w_u32(pixels, color);
            pixels += 4;
        }
    }
    t3_80029dc0(0);
    VSync(0);
    LoadImagePSX(&rectangle, psx_addr(0x801144B8, 460800));
    DrawSync(0);
    SetDefDispEnv(&display, 0, 0, 384, 480);
    display.isinter = 1;
    display.screen.x = r_s8(0x80097F24);
    display.screen.y = r_s8(0x80097F25);
    display.screen.w = 258;
    display.screen.h = 240;
    t3_80029dc0(0);
    VSync(0);
    PutDispEnv(&display);
    pixels = 0x801144B8;
    for (row = 0; row < 480; ++row) {
        color = 0;
        if (white) {
            sint32 top = r_s16(0x800AE232);
            if ((sint32)row >= top && (sint32)row <= top + r_s16(0x800AE236) - 1)
                color = 0xFFFFFFFF;
        }
        for (column = 0; column < 240; ++column) {
            w_u32(pixels, color);
            pixels += 4;
        }
    }
    t3_80029dc0(0);
    VSync(0);
    LoadImagePSX(&rectangle, psx_addr(0x801144B8, 460800));
    DrawSync(0);
    SetDefDispEnv(&display, 0, 0, 384, 480);
    display.isinter = 1;
    display.screen.x = r_s8(0x80097F24);
    display.screen.y = r_s8(0x80097F25);
    display.screen.w = 258;
    display.screen.h = 240;
    t3_80029dc0(0);
    VSync(0);
    PutDispEnv(&display);
}

static sint32 t3_fmv_caption_frame(void *context, uint32 number)
{
    (void)context;
    t3_bns_08_800ff8e0((sint16)number);
    return 1;
}

static sint32 t3_fmv_audio_tick(void *context)
{
    (void)context;
    t3_8006b6fc();
    return 1;
}

static sint32 t3_fmv_final_frame(void *context, uint32 step)
{
    const XportFmvPresentation *presentation = (const XportFmvPresentation *)context;
    DISPENV display;
    PSX_RECT *rectangle;
    sint32 mode;
    sint32 counter;
    uint32 page;
    uint32 band;
    uint32 target_y;

    if (!presentation || !gpu_get_display_env(&display) ||
        (display.disp.y != 0 && display.disp.y != 240))
        return -1;
    mode = r_s16(0x8011443C);
    if (mode != 1 && mode != 2)
        return -1;
    page = (uint32)display.disp.y / 240;
    if (!step) {
        t3_8006bc98(0, -1);
        t3_8006bc98(0, 180);
        w_u16(0x80105F88, (uint16)display.disp.x);
        w_u16(0x80105F8A, (uint16)display.disp.y);
        rectangle = (PSX_RECT *)psx_addr(0x80105F88, 8);
        StoreImage(rectangle, psx_addr(0x801144B8, 230400));
        if (mode == 2)
            t3_bns_08_80105444(0x801144B8);
        w_u16(0x80114440, 124);
        w_u16(0x80114430, page);
        t3_8006ad28(6);
        return 1;
    }
    counter = r_s16(0x80114440);
    t3_bns_08_801053c0(counter < 64 ? (uint32)counter : 64, (uint32)mode - 1);
    for (band = 0; band < 15; ++band) {
        t3_bns_08_80105340(0x801144B8 + band * 15360, 0x8014C8B8);
        target_y = r_u16(0x80105F98 + page * 4) + band * 16;
        w_u16(0x80105F92, target_y);
        LoadImagePSX((PSX_RECT *)psx_addr(0x80105F90, 8), psx_addr(0x8014C8B8, 15360));
    }
    page = !page;
    w_u16(0x80114430, page);
    SetDefDispEnv(&display, 0, page * 240, presentation->width, presentation->height);
    display.isrgb24 = 1;
    display.isinter = 1;
    display.screen.x = r_s8(0x80097F24);
    display.screen.y = r_s8(0x80097F25);
    display.screen.w = 256;
    display.screen.h = 240;
    PutDispEnv(&display);
    counter = (sint16)(r_u16(0x80114440) - 2);
    w_u16(0x80114440, counter);
    return counter >= 0;
}

static sint32 t3_fmv_caption_strip(void *context, uint32 word_x, uint8 *rgb, uint32 rows)
{
    uint32 bytes;
    void *scratch;

    (void)context;
    if (!rows || rows > 256)
        return 0;
    if (r_s16(0x80184CC0) < 0)
        return 1;
    bytes = rows * 48;
    scratch = psx_addr(0x801244B8, 15360);
    memcpy(scratch, rgb, bytes);
    t3_bns_08_800ffc24(word_x, 0x801244B8);
    memcpy(rgb, scratch, bytes);
    return 1;
}

sint32 t3_fmv_presentation_configure(XportFmvPresentation *presentation, uint32 width, uint32 height, uint32 caption)
{
    if (!presentation || !width || width > 320 || (width & 15) || !height || height > 240 || caption > 5)
        return 0;
    memset(presentation, 0, sizeof(*presentation));
    presentation->width = width <= 256 ? 256 : 320;
    presentation->height = 240;
    presentation->video_x = (presentation->width - width) / 2;
    presentation->video_y = (240 - height) / 2;
    presentation->bottom_rows = caption == 3 ? 32 : 0;
    presentation->screen_x = r_s8(0x80097F24);
    presentation->screen_y = r_s8(0x80097F25);
    presentation->screen_width = 256;
    presentation->screen_height = 240;
    presentation->interlace = 1;
    presentation->controllers = 3;
    presentation->skip_buttons = PADstart;
    presentation->skip_held = 1;
    presentation->context = presentation;
    presentation->tick = t3_fmv_audio_tick;
    if (caption) {
        presentation->frame = t3_fmv_caption_frame;
        presentation->strip = t3_fmv_caption_strip;
    }
    return 1;
}

sint32 t3_fmv_presentation_init(XportFmvPresentation *presentation, uint32 width, uint32 height, uint32 caption)
{
    if (!t3_fmv_presentation_configure(presentation, width, height, caption))
        return 0;
    if (caption) {
        t3_bns_08_800ff7a8();
        t3_bns_08_800ff830((sint32)caption - 1);
        t3_bns_08_800ff8e0(0);
    }
    return 1;
}

sint32 t3_fmv_finish_init(XportFmvPresentation *presentation, uint32 mode)
{
    if (!presentation || mode > 2)
        return 0;
    w_u16(0x8011443C, mode);
    presentation->context = presentation;
    presentation->finish = mode ? t3_fmv_final_frame : NULL;
    return 1;
}

uint32 t3_8006adb0(uint32 index, uint32 word0, uint32 word1, uint32 word2, uint32 word3, uint32 word4, uint32 word5)
{
    uint32 record;

    FUNCTION_MARKER(0x8006ADB0, "SLUS_004.02");
    record = 0x800A0640 + index * 24;
    w_u32(record, word0);
    w_u32(record + 4, word1);
    w_u32(record + 8, word2);
    w_u32(record + 12, word3);
    w_u32(record + 16, word4);
    w_u32(record + 20, word5);
    w_u16(0x800A0638 + index * 2, 1);
    return 1;
}

uint32 t3_8006b540(uint32 volume)
{
    FUNCTION_MARKER(0x8006B540, "SLUS_004.02");
    w_u16(0x800A068A, volume);
    return 0x800A0000;
}

uint32 t3_8006b978(sint32 volume)
{
    sint32 previous;
    uint32 scaled;
    uint32 value;
    sint64 product;
    uint32 high;

    FUNCTION_MARKER(0x8006B978, "SLUS_004.02");
    previous = r_s16(0x800A0688);
    if (volume >= 0) {
        scaled = (uint32)(sint32)(sint16)volume * 127;
        value = (uint32)r_s16(0x800A068A) * scaled;
        product = (sint64)(sint32)value * (sint32)0x82061029;
        w_u16(0x800A0688, volume);
        w_u16(0x800A0694, 1);
        high = (uint32)(sint32)(product >> 32) + value;
        value = (uint32)((sint32)high >> 13) - (uint32)((sint32)value >> 31);
        w_u16(0x800A0690, value);
    }
    return (uint32)previous;
}

uint32 t3_bns_01_800b7f80(uint32 side, uint32 selection)
{
    uint32 actor = 0x800A9228 + side * 6284;
    uint32 table = 0x800B25F8 + (uint32)r_s16(actor + 22) * 8;
    uint32 record;
    uint32 source;
    uint32 packed;
    uint32 cursor;
    uint32 index;
    sint32 count;

    FUNCTION_MARKER(0x800B7F80, "SLUS_004.02-BNS-logical-01");
    if (!r_u32(table + 4))
        return 0;
    record = r_u32(table) + selection * 12;
    (void)r_u32(record);
    source = r_u32(record + 4);
    packed = r_u32(record + 8);
    count = (sint16)(packed >> 16);
    if (!count) {
        cursor = 0x800B8C10;
        for (index = 0; index < 50; ++index) {
            uint32 value;
            w_u8(cursor, 0);
            w_u8(cursor + 1, 0);
            value = r_u32(cursor);
            w_u32(cursor, value & 0x8000FFFF);
            cursor += 4;
        }
        w_u32(0x800B8D48 + side * 4, 0);
        w_u32(0x800B8B4C + side * 4, 30);
        return 0;
    }
    w_u32(0x800B8CE0, (uint32)count);
    for (index = 0; (sint32)index < count; ++index) {
        uint32 value = r_u16(source);
        source += 2;
        w_u16(0x800B8CE4 + index * 2, value);
    }
    return (uint32)count;
}

uint32 t3_8002a914(void)
{
    const uint32 first_actor = 0x800A9228;
    const uint32 second_actor = 0x800AAAB4;
    const uint32 third_actor = 0x800AC340;
    uint32 value;
    uint32 buttons;
    uint32 mode;
    uint32 primary;
    uint32 first;
    uint32 second;

    FUNCTION_MARKER(0x8002A914, "SLUS_004.02");
    /* The audited native prefix excludes reads of an unassigned third actor */
    t3_80029638(1);
    w_u32(0x80095460, 0);
    value = r_u32(0x80095484);
    w_u32(0x80095464, 0);
    mode = r_u32(0x800AFA88);
    w_u32(0x80095484, value + 1);
    if (mode != 8 || !r_u32(0x800AFAAC))
        w_u32(0x800ADBCC, (r_u16(0x800ADEF8) + 2) * 600);
    w_u32(0x8009547C, 0);
    w_u32(0x8009546C, 0);
    w_u32(0x80095468, 0);
    w_u32(0x8009548C, 0);
    w_u32(0x80095490, 0);
    w_u32(0x80095494, 0);
    t3_8002bc00(0);
    w_u16(0x800ADE78, 0);
    t3_80063070();
    w_u32(0x80095498, 0);
    w_u32(0x80095480, 0);
    w_u32(0x800954A4, 0);
    w_u32(0x800954A0, 0);
    t3_8003cb48();
    w_u32(0x800954A8, 0);
    w_u32(0x800954AC, 0);
    if (r_s16(first_actor + 0x18) == 15) {
        t3_8004f2dc(first_actor);
        if (r_u32(0x800AFA88) == 8) {
            if (!r_u32(0x800B6BF4))
                buttons = r_u16(0x800ADD68);
            else
                buttons = r_u16(0x800ADD6A);
        } else {
            buttons = r_u16(0x800ADD68);
        }
        if (buttons & 0x4000)
            w_u32(0x800954A8, 1);
    }
    if (r_s16(second_actor + 0x18) == 15) {
        t3_8004f2dc(second_actor);
        if (r_u16(0x800ADD6A) & 0x4000)
            w_u32(0x800954AC, 1);
    }
    t3_80069aa8(0);
    t3_80069f74();
    t3_8006a158(first_actor);
    t3_8006a370(first_actor);
    t3_8006a370(second_actor);
    if (r_u32(0x800AFA88) == 8)
        t3_8006a370(third_actor);
    t3_80031d0c(2);
    t3_8003c258(first_actor);
    t3_8003c258(second_actor);
    if (r_u32(0x800AFA88) == 8)
        t3_8003c258(third_actor);
    t3_8006c858();
    t3_8006e614();
    t3_8006d7cc();
    t3_8004b688(r_u32(0x80095484) != 1);
    t3_80046cd4();
    t3_800774c4();
    t3_8004ac64();
    t3_8004acf0();
    t3_8003a120();
    t3_8002c050(first_actor);
    t3_8002c050(second_actor);
    mode = r_u32(0x800AFA88);
    if (mode == 8) {
        t3_8002c050(third_actor);
        if (r_u32(0x800AFA88) == mode)
            goto reset_team;
    }
    if (r_u32(0x800AFA88) == 7) {
        t3_8002bd78(first_actor, second_actor, 1);
        t3_8002bd78(second_actor, first_actor, 1);
        t3_8002bd78(third_actor, first_actor, 1);
        t3_8002c044(third_actor);
    } else {
        t3_8002bd78(first_actor, second_actor, 1);
        t3_8002bd78(second_actor, first_actor, 1);
    }
    goto reset_common;

reset_team:
    primary = t3_bns_02_800b297c();
    first = t3_bns_02_800b298c(0);
    second = t3_bns_02_800b298c(1);
    if (r_u32(0x800AFAAC) < 4) {
        w_u32(second, 0);
        w_u32(first, 0);
        w_u32(primary, 0);
        w_u32(second + 8, 0);
        w_u32(first + 8, 0);
        w_u32(primary + 8, 0);
    } else {
        w_u32(primary, (uint32)-1000);
        w_u32(primary + 8, 0);
        w_u32(second, 1000);
        w_u32(first, 1000);
        w_u32(second + 8, 0);
        w_u32(first + 8, 0);
    }
    w_u16(second + 14, 0x4000);
    w_u16(first + 14, 0x4000);
    w_u16(primary + 14, 0x4000);
    t3_8002bd78(primary, first, 0);
    t3_8002bd78(first, primary, 0);
    t3_8002bd78(second, primary, 0);
    w_u8(first + 0xC3, 0);
    w_u8(second + 0xC3, 0);
    w_u8(first + 0xC2, 1);
    w_u8(second + 0xC2, 1);

reset_common:
    value = 2;
    if (r_u32(0x800AFA88) == 8)
        value = 3;
    t3_80043684(value);
    t3_80065ed8(first_actor, second_actor);
    t3_80068394(0, first_actor, second_actor);
    t3_80059654();
    if (r_u32(0x800AFA88) == 8)
        t3_bns_02_800b2bf8();
    else
        t3_8004e238();
    t3_8004e580((uint32)r_s16(first_actor + 0x44), (uint32)r_s16(second_actor + 0x44));
    t3_80039034();
    t3_80039eec();
    t3_8003ebf0();
    mode = r_u32(0x800AFA88);
    if (mode == 7) {
        t3_bns_03_800b065c(0);
        t3_bns_03_800b065c(1);
        t3_bns_03_800b07c8();
        mode = r_u32(0x800AFA88);
    }
    if (mode == 5)
        t3_bns_01_800b2894();
    t3_8006ae14(r_u16(0x800ADD08), 0);
    return t3_80029638(0);
}

uint32 t3_8002a40c(uint32 character0, uint32 enabled0, uint32 variant0, uint32 character1, uint32 enabled1, uint32 variant1)
{
    uint32 entry_mode;
    uint32 difficulty;
    uint32 archive;
    uint32 result;
    uint32 normalized_first;
    uint32 normalized_second;
    uint32 selected_character;
    uint32 selected_variant;
    uint32 last_actor;
    uint32 last_character;
    uint32 last_variant;
    uint32 model0;
    uint32 packets0;
    uint32 model1;
    uint32 packets1;
    uint32 model2;
    uint32 packets2;
    uint32 variant;

    FUNCTION_MARKER(0x8002A40C, "SLUS_004.02");
    enabled0 &= 0xFFFF;
    enabled1 &= 0xFFFF;
    t3_80029638(1);
    entry_mode = r_u32(0x800AFA88);
    w_u32(0x80095484, 0);
    difficulty = r_u8(0x800AFAA8) + 1;
    w_u16(0x800ADDFC, (uint16)difficulty);
    if (difficulty >= 6)
        w_u16(0x800ADDFC, 5);
    w_u16(0x800ADF3C, (uint16)(r_u16(0x800ADDFC) * 2 - 1));
    if (entry_mode == 8) {
        if (!r_u16(0x800A9128)) {
            selected_character = (uint32)(sint32)(sint16)character0;
            selected_variant = (uint32)(sint32)(sint16)variant0;
        }
        else {
            selected_character = (uint32)(sint32)(sint16)character1;
            selected_variant = (uint32)(sint32)(sint16)variant1;
        }
        w_u16(0x800A9128, 0);
        enabled0 = 0;
        w_u16(0x800A912A, 1);
        t3_8004f274(0x800A9228, selected_character, selected_variant);
        enabled1 = 1;
        if (r_u32(0x800AFAAC) < 4) {
            variant = t3_bns_02_800b2a80(r_u16(0x800ADC84) - 15, 0);
            t3_8004f274(0x800AAAB4, 21, variant);
            last_variant = t3_bns_02_800b2a80(r_u16(0x800ADC84) - 15, 1);
        }
        else {
            t3_8004f274(0x800AAAB4, 19, 0);
            last_variant = 0;
        }
        last_actor = 0x800AC340;
        last_character = 21;
    }
    else {
        t3_8004f274(0x800A9228, (uint32)(sint32)(sint16)character0, (uint32)(sint32)(sint16)variant0);
        last_actor = 0x800AAAB4;
        last_character = (uint32)(sint32)(sint16)character1;
        last_variant = (uint32)(sint32)(sint16)variant1;
    }
    t3_8004f274(last_actor, last_character, last_variant);
    t3_80051304();
    normalized_first = t3_8002d1dc((uint32)r_s16(0x800A923E));
    normalized_second = (uint32)r_s16(0x800AAACA);
    w_u16(0x800A923E, (uint16)normalized_first);
    normalized_second = t3_8002d1dc(normalized_second);
    w_u16(0x800AAACA, (uint16)normalized_second);
    if (r_u32(0x800AFA88) == 8) {
        result = t3_8002d1dc((uint32)r_s16(0x800AC356));
        w_u16(0x800AC356, (uint16)result);
    }
    archive = t3_80069aa8(0);
    w_u8(0x800A92ED, enabled0 != 0);
    w_u8(0x800AAB79, enabled1 != 0);
    if (r_u32(0x800AFA88) == 8)
        w_u8(0x800AC405, 1);
    t3_8006c618(r_u16(0x800ADC84), (uint32)r_s16(0x800A9244), (uint32)r_s16(0x800AAAD0), (uint32)r_s16(0x800A923E), (uint32)r_s16(0x800AAACA));
    t3_80036600(archive);
    t3_8007641c(0x800A9228, 0x800AAAB4);
    t3_8006e560();
    t3_8004b688(1);
    t3_80074f44();
    t3_80035f2c(r_u16(0x800ADC84));
    t3_80035dec();
    t3_80069f74();
    t3_8006a158(0x800B0000);
    t3_80036310(0x800A9228, &model0, &packets0);
    t3_80036310(0x800AAAB4, &model1, &packets1);
    if (r_u32(0x800AFA88) == 8 && !r_u32(0x800B6C10))
        t3_80036310(0x800AC340, &model2, &packets2);
    t3_80035ce0();
    t3_80035ce0();
    if (r_u32(0x800AFA88) == 8 && !r_u32(0x800B6C10))
        t3_80035ce0();
    t3_80035ce8(0x800A9228, model0, packets0);
    t3_80035ce8(0x800AAAB4, model1, packets1);
    if (r_u32(0x800AFA88) == 8 && !r_u32(0x800B6C10))
        t3_80035ce8(0x800AC340, model2, packets2);
    t3_8006c63c(1, 1);
    t3_8006c63c(2, 1);
    if (r_u32(0x800AFA88) == 8)
        t3_bns_02_800b2fcc(0, selected_character, selected_variant);
    else
        t3_8004e79c((uint32)(sint32)(sint16)character0, (uint32)(sint32)(sint16)variant0, (uint32)(sint32)(sint16)character1, (uint32)(sint32)(sint16)variant1);
    t3_8004ea60();
    if (r_u32(0x800AFA88) == 8) {
        result = t3_bns_02_800b297c();
    }
    else {
        t3_800516cc(0x800A9228);
        t3_800516cc(0x800AAAB4);
        if (r_u32(0x800AFA88) != 7)
            goto clear_results;
        result = 0x800AC340;
    }
    t3_800516cc(result);

clear_results:
    w_u16(0x800A926C, 0);
    w_u16(0x800AAAF8, 0);
    if (r_u32(0x800AFA88) == 8)
        w_u16(0x800AC384, 0);
    t3_80031d0c(1);
    t3_80039960(r_u16(0x800ADC84));
    return t3_80029638(0);
}

uint32 t3_800361ec(uint32 object, uint32 model_slot)
{
    uint32 index;
    uint32 result;
    FUNCTION_MARKER(0x800361EC, "SLUS_004.02");
    w_u16(object + 0x14, 0);
    for (index = 0; index < 92; ++index) {
        if ((uint32)r_s8(0x800958C4 + index) == model_slot) {
            w_u16(object + 0x14, (uint16)index);
            break;
        }
    }
    index = r_u16(object + 0x14);
    result = index < 92 ? (uint32)r_s8(0x800958C4 + (uint32)(sint32)(sint16)index) : 0xFFFFFFFF;
    w_u16(object + 0x1C, (uint16)result);
    return result;
}

void t3_bns_00_800b3ca0(void)
{
    const uint32 first = 0x800A9228;
    const uint32 second = 0x800AAAB4;
    uint32 reset_position;
    uint32 actor;
    uint32 other;
    uint32 alternate;
    uint32 model_slot;
    uint32 stage;
    uint32 other_character;
    uint32 other_identity;
    uint32 model;
    uint32 packets;
    uint32 row;
    uint32 slot;
    uint32 tile;
    uint32 owner;
    uint32 ordering;
    uint32 first_angle;
    uint32 second_angle;
    sint32 packed;
    sint32 packed_again;

    FUNCTION_MARKER(0x800B3CA0, "SLUS_004.02-BNS-logical-00");
    w_u32(0x800B4A50, 0);
    w_u32(0x800B4A58, 0);
    w_u32(0x800B4A5C, 0);
    w_u32(0x800B4A84, 0);
    w_u32(0x800B4A88, 0);
    t3_800482b0(6);
    w_u8(0x800AFAA1, 1);
    t3_8004b688(0);
    t3_8003c258(first);
    t3_8003c258(second);
    /* The final muted voice leaves the first reset selector zero */
    t3_8002bd78(first, second, 0);
    /* Recover the pure texture selector retained by the preceding reset */
    reset_position = t3_80036294(first) & 255;
    t3_8002bd78(second, first, reset_position);
    t3_80074f44();
    for (row = 0; row < 2; ++row) {
        slot = 0x800B4A6C + row * 10;
        w_u16(slot, 0);
        w_u16(slot + 8, 0);
        w_u16(slot + 2, 22);
        w_u16(slot + 4, 0);
    }
    row = r_u8(0x800AFAA5) & 1;
    alternate = r_u8(0x800AFB39);
    if (row) {
        w_u32(0x800B4A4C, 0x800B0728);
        actor = first;
        other = second;
    }
    else {
        w_u32(0x800B4A4C, 0x800B075C);
        actor = second;
        other = first;
    }
    if (r_s16(actor + 0x18) == 13)
        model_slot = alternate ? 19 : 18;
    else
        model_slot = alternate ? 27 : 26;
    t3_800361ec(actor, model_slot);
    packed = r_s16(actor + 0x14);
    if (packed < 0)
        packed += 3;
    packed_again = r_s16(actor + 0x14);
    w_u16(actor + 0x16, (uint16)(packed >> 2));
    if (packed_again < 0)
        packed_again += 3;
    w_u16(actor + 0x18, (uint16)(packed_again >> 2));
    stage = r_u16(0x800ADC84);
    model_slot = (uint32)r_s16(first + 0x1C);
    other_character = (uint32)r_s16(second + 0x16);
    other_identity = r_u8(other + 0x1E);
    t3_8006c618(stage, model_slot, model_slot, other_character, other_character);
    t3_80036310(actor, &model, &packets);
    t3_80035ce0();
    t3_80035ce8(actor, model, packets);
    model = t3_80035ed0(other_identity);
    packets = t3_80035eec(other_identity);
    t3_80035ce8(other, model, packets);
    t3_8006c63c(1, 1);
    t3_8006c63c(2, 1);
    w_u16(first + 0x0E, 0x4000);
    w_u16(second + 0x0E, 0x4000);
    first_angle = r_u16(first + 0x0E);
    second_angle = r_u16(second + 0x0E);
    w_u16(first + 0x0C, 0);
    w_u32(first, 0);
    w_u32(first + 4, 0);
    w_u16(first + 0x10, 0);
    w_u32(first + 8, 0);
    w_u16(second + 0x0C, 0);
    w_u32(second, 0);
    w_u32(second + 4, 0);
    w_u16(second + 0x10, 0);
    w_u32(second + 8, 0);
    w_u16(first + 0x2C, (uint16)first_angle);
    w_u16(second + 0x2C, (uint16)second_angle);
    for (row = 0; row < 2; ++row) {
        tile = 0x800B4A8C + row * 16;
        setTile((TILE *)psx_addr(tile, 16));
        w_u8(tile + 4, 0);
        w_u8(tile + 5, 0);
        w_u8(tile + 6, 0);
        w_u16(tile + 8, 0);
        w_u16(tile + 10, 0);
        w_u16(tile + 12, 368);
        w_u16(tile + 14, 480);
    }
    if (!r_u32(0x800ADCA4)) {
        row = r_u32(0x800ADEFC);
        owner = r_u32(0x800A8C54);
        ordering = r_u32(owner + 4) + 4092;
        tile = 0x800B4A8C + (row << 4);
        AddPrim(psx_addr(ordering, 4), psx_addr(tile, 4));
    }
}

uint32 t3_800663f8(uint32 data, uint32 output, uint32 flags)
{
    uint16 samples[7];
    uint32 length;
    uint32 sample;
    uint32 index;
    sint32 depth;
    sint32 value;

    FUNCTION_MARKER(0x800663F8, "SLUS_004.02");
    if (data) {
        w_u32(0x800A8CB4, data + 2);
        w_u16(0x800A8CBA, 0);
        length = t3_80038cd8(data + 2);
        w_u16(0x800A8CB8, length);
        if (flags)
            return (uint32)((sint32)(sint16)length + 1);
        return r_u16(data);
    }
    sample = r_u16(0x800A8CBA);
    data = r_u32(0x800A8CB4);
    w_u16(0x800A8CBA, sample + 1);
    if (sample >= r_u16(0x800A8CB8)) {
        w_u16(output + 14, flags ? 450 : 550);
        w_u16(output + 12, flags ? 0 : 320);
        return 0;
    }
    t3_80038ce4(data, samples, (uint32)(sint32)(sint16)sample);
    if (flags) {
        for (index = 0; index < 6; ++index)
            w_u16(output + index * 2, (sint32)(sint16)samples[index] / 2);
        w_u16(output + 14, 450);
        w_u16(output + 12, 0);
    } else {
        depth = (sint32)(sint16)samples[6] / 8;
        if (depth < 0)
            depth = 0;
        else if (depth >= 321 || r_s16(0x800A8CBA) >= r_s16(0x800A8CB8))
            depth = 320;
        for (index = 0; index < 6; ++index) {
            value = (sint32)(sint16)samples[index] * 20;
            w_u16(output + index * 2, value / 32);
        }
        w_u16(output + 14, 550);
        w_u16(output + 12, depth);
    }
    return 0;
}

uint32 t3_800666f0(void)
{
    uint32 camera[6];
    uint32 eye_x;
    uint32 eye_y;
    uint32 eye_z;
    uint32 target_x;
    uint32 target_y;
    uint32 target_z;
    uint32 distance;
    uint32 scale;
    uint32 angle;
    uint32 sine;
    uint32 cosine;
    uint32 depth;
    uint32 center_x;
    uint32 center_y;
    uint32 center_z;
    uint32 world_x;
    uint32 world_y;
    uint32 world_z;
    uint32 world_target_x;
    uint32 world_target_y;
    uint32 world_target_z;

    FUNCTION_MARKER(0x800666F0, "SLUS_004.02");
    eye_x = (uint32)r_s16(0x800A0428);
    distance = (uint32)r_s16(0x800A0436);
    target_x = (uint32)r_s16(0x800A042E);
    eye_y = (uint32)r_s16(0x800A042A);
    target_y = (uint32)r_s16(0x800A0430);
    eye_z = (uint32)r_s16(0x800A042C);
    target_z = (uint32)r_s16(0x800A0432);
    depth = (uint32)r_s16(0x800A0434);
    if (r_s16(0x800A8CBC)) {
        eye_z = (uint32)(sint32)(sint16)(0 - eye_z);
        target_z = (uint32)(sint32)(sint16)(0 - target_z);
    }
    angle = (r_u32(0x800A8CB0) & 0xFFF) * 2;
    sine = (uint32)r_s16(0x8001E8C4 + angle);
    cosine = (uint32)r_s16(0x8001F0C4 + angle);
    if (r_s16(0x800A8CDA)) {
        scale = (uint32)r_s16(0x800A8CD4);
        eye_x = (uint32)(sint32)(sint16)(target_x +
            t3_camera_divide((eye_x - target_x) * scale, distance));
        eye_y = (uint32)(sint32)(sint16)(target_y +
            t3_camera_divide((eye_y - target_y) * scale, distance));
        eye_z = (uint32)(sint32)(sint16)(target_z +
            t3_camera_divide((eye_z - target_z) * scale, distance));
        distance = scale;
    }
    center_x = r_u32(0x800A8CC4);
    world_x = center_x + t3_camera_divide(cosine * eye_x - sine * eye_z, 4096);
    center_z = r_u32(0x800A8CCC);
    world_z = center_z + t3_camera_divide(sine * eye_x + cosine * eye_z, 4096);
    world_target_x = center_x + t3_camera_divide(cosine * target_x - sine * target_z, 4096);
    world_target_z = center_z + t3_camera_divide(sine * target_x + cosine * target_z, 4096);
    center_y = r_u32(0x800A8CC8);
    world_y = center_y + eye_y;
    world_target_y = center_y + target_y;
    t3_80063eec(camera, world_x, world_y, world_z,
        world_target_x, world_target_y, world_target_z, (sint32)distance);
    t3_80064020(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], 2);
    t3_800663f8(0, 0x800A0428, 0);
    return depth;
}

uint32 t3_80066a18(void)
{
    uint32 camera[6];
    uint32 eye_x;
    uint32 eye_y;
    uint32 eye_z;
    uint32 target_x;
    uint32 target_y;
    uint32 target_z;
    uint32 distance;
    uint32 scale;
    uint32 angle;
    uint32 sine;
    uint32 cosine;
    uint32 depth;
    uint32 remaining;
    uint32 next;
    uint32 data;
    uint32 center_x;
    uint32 center_y;
    uint32 center_z;
    uint32 world_x;
    uint32 world_y;
    uint32 world_z;
    uint32 world_target_x;
    uint32 world_target_y;
    uint32 world_target_z;

    FUNCTION_MARKER(0x80066A18, "SLUS_004.02");
    eye_x = (uint32)r_s16(0x800A0428);
    distance = (uint32)r_s16(0x800A0436);
    target_x = (uint32)r_s16(0x800A042E);
    eye_y = (uint32)r_s16(0x800A042A);
    target_y = (uint32)r_s16(0x800A0430);
    eye_z = (uint32)r_s16(0x800A042C);
    target_z = (uint32)r_s16(0x800A0432);
    depth = (uint32)r_s16(0x800A0434);
    if (r_s16(0x800A8CBC)) {
        eye_z = (uint32)(sint32)(sint16)(0 - eye_z);
        target_z = (uint32)(sint32)(sint16)(0 - target_z);
    }
    angle = (r_u32(0x800A8CB0) & 0xFFF) * 2;
    sine = (uint32)r_s16(0x8001E8C4 + angle);
    cosine = (uint32)r_s16(0x8001F0C4 + angle);
    if (r_s16(0x800A8CDA)) {
        scale = (uint32)r_s16(0x800A8CD4);
        eye_x = (uint32)(sint32)(sint16)(target_x +
            t3_camera_divide((eye_x - target_x) * scale, distance));
        eye_y = (uint32)(sint32)(sint16)(target_y +
            t3_camera_divide((eye_y - target_y) * scale, distance));
        eye_z = (uint32)(sint32)(sint16)(target_z +
            t3_camera_divide((eye_z - target_z) * scale, distance));
        distance = scale;
    }
    center_x = r_u32(0x800A8CC4);
    world_x = center_x + t3_camera_divide(cosine * eye_x - sine * eye_z, 4096);
    center_z = r_u32(0x800A8CCC);
    world_z = center_z + t3_camera_divide(sine * eye_x + cosine * eye_z, 4096);
    world_target_x = center_x + t3_camera_divide(cosine * target_x - sine * target_z, 4096);
    world_target_z = center_z + t3_camera_divide(sine * target_x + cosine * target_z, 4096);
    center_y = r_u32(0x800A8CC8);
    world_y = center_y + eye_y;
    world_target_y = center_y + target_y;
    t3_80063eec(camera, world_x, world_y, world_z,
        world_target_x, world_target_y, world_target_z, (sint32)distance);
    t3_80064020(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], 2);
    t3_800663f8(0, 0x800A0428, 1);
    remaining = r_u16(0x800A041A) - 1;
    w_u16(0x800A041A, remaining);
    if ((sint16)remaining > 0)
        return depth;
    if (r_u8(0x800A0418) == 1)
        return 1;
    next = r_u8(0x800A0419);
    w_u16(0x800A8CBA, 0);
    next = (next + 1) & 255;
    w_u8(0x800A0419, next);
    data = t3_80067070(0, next, 0, 0);
    remaining = t3_800663f8(data, 0, 1);
    w_u16(0x800A041A, remaining - 1);
    t3_800663f8(0, 0x800A0428, 1);
    return depth;
}

void t3_80067f5c(uint32 actor, uint32 opponent, uint32 reset, uint32 distance)
{
    uint32 camera[6];
    uint32 record;
    sint32 kind;
    uint32 heading;
    uint32 random;
    uint32 pitch;
    uint32 yaw;
    uint32 offset;
    uint32 factor;
    uint32 start;
    uint32 end;
    uint32 radius;
    uint32 pitch_index;
    uint32 yaw_index;
    uint32 sine_pitch;
    uint32 cosine_pitch;
    uint32 sine_yaw;
    uint32 cosine_yaw;
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 focal;
    uint32 target_x;
    uint32 target_y;
    uint32 target_z;

    FUNCTION_MARKER(0x80067F5C, "SLUS_004.02");
    record = r_u32(0x800A0438);
    kind = r_s16(record);
    if (kind != 0 && kind != 1) {
        t3_80062a10();
        return;
    }
    if (!kind)
        distance = t3_camera_divide(distance * 114, 128);
    if (reset) {
        heading = t3_80066148(actor, opponent);
        random = (uint32)randPSX();
        record = r_u32(0x800A0438);
        pitch = (uint32)r_s16(record + 16 + (random & 3) * 2) << 6;
        random = (uint32)randPSX();
        record = r_u32(0x800A0438);
        offset = (uint32)r_s16(record + 24 + (random & 3) * 2) << 6;
        if (r_s16(0x800A8CDC))
            pitch = 0;
        if (r_s16(0x800A8C90))
            yaw = heading - offset;
        else
            yaw = heading + offset;
        w_u32(0x800A8CB0, yaw);
        w_u32(0x800A8CAC, pitch);
    } else {
        pitch = r_u32(0x800A8CAC);
        yaw = r_u32(0x800A8CB0);
    }
    if (!kind) {
        t3_80064e10(actor, opponent, pitch, yaw, reset, distance, 1);
        return;
    }
    factor = t3_camera_divide((uint32)r_s16(0x800A8C9C) << 12,
        (uint32)r_s16(0x800A8C9E)) - 1024;
    if ((sint32)factor <= 0)
        factor = 0;
    record = r_u32(0x800A0438);
    start = r_u32(record + 8);
    end = r_u32(record + 12);
    radius = t3_camera_divide((start - end) * factor, 4096) + end;
    radius = t3_camera_divide(radius * (uint32)r_s16(0x800A8CD4), 750);
    pitch_index = pitch;
    if ((sint32)pitch_index < 0)
        pitch_index += 63;
    pitch_index = (uint32)((sint32)pitch_index >> 5) & 0x1FFE;
    sine_pitch = (uint32)r_s16(0x8001E8C4 + pitch_index);
    cosine_pitch = (uint32)r_s16(0x8001F0C4 + pitch_index);
    yaw_index = (t3_camera_divide(yaw, 64) + 1024) & 0xFFF;
    yaw_index *= 2;
    cosine_yaw = (uint32)r_s16(0x8001F0C4 + yaw_index);
    sine_yaw = (uint32)r_s16(0x8001E8C4 + yaw_index);
    offset = t3_camera_divide(radius * cosine_yaw, 4096);
    offset = t3_camera_divide(offset * cosine_pitch, 4096);
    x = r_u32(0x800A0448) - offset;
    y = r_u32(0x800A044C) - t3_camera_divide(radius * sine_pitch, 4096);
    offset = t3_camera_divide(radius * sine_yaw, 4096);
    offset = t3_camera_divide(offset * cosine_pitch, 4096);
    z = r_u32(0x800A0450) - offset;
    focal = (uint32)r_s16(0x800A8CD4);
    target_x = r_u32(0x800A0448);
    target_y = r_u32(0x800A044C);
    target_z = r_u32(0x800A0450);
    t3_80063eec(camera, x, y, z, target_x, target_y, target_z, (sint32)focal);
    t3_80064020(camera[0], camera[1], camera[2], camera[3], camera[4], camera[5], 1);
}

uint32 t3_800461c0(uint32 camera)
{
    uint32 source;
    uint32 destination;
    uint32 end;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;

    FUNCTION_MARKER(0x800461C0, "SLUS_004.02");
    source = camera;
    destination = 0x800A85C0;
    end = camera + 32;
    do {
        first = r_u32(source);
        second = r_u32(source + 4);
        third = r_u32(source + 8);
        fourth = r_u32(source + 12);
        w_u32(destination, first);
        w_u32(destination + 4, second);
        w_u32(destination + 8, third);
        w_u32(destination + 12, fourth);
        source += 16;
        destination += 16;
    } while (source != end);
    first = r_u32(source);
    w_u32(destination, first);
    return t3_80045ca8(camera, 0x800ADF70);
}

uint32 t3_800513b4(uint32 character)
{
    uint32 entry;
    uint32 count;

    FUNCTION_MARKER(0x800513B4, "SLUS_004.02");
    entry = 0x8009804C + (uint32)((sint32)character % 22) * 8;
    count = r_u16(entry) + 1;
    if (count > 65535)
        count = 65535;
    w_u16(entry, count);
    count = r_u16(0x80097F1C) + 1;
    if (count > 65535)
        count = 65535;
    w_u16(0x80097F1C, count);
    return t3_8004c390();
}

uint32 t3_80051458(uint32 winner, uint32 loser)
{
    uint32 entry;
    uint32 count;

    FUNCTION_MARKER(0x80051458, "SLUS_004.02");
    entry = 0x8009804E + (uint32)((sint32)winner % 22) * 8;
    count = r_u16(entry) + 1;
    if (count > 65535)
        count = 65535;
    w_u16(entry, count);
    entry = 0x80098050 + (uint32)((sint32)loser % 22) * 8;
    count = r_u16(entry) + 1;
    if (count > 65535)
        count = 65535;
    w_u16(entry, count);
    count = r_u16(0x80097F1C) + 1;
    if (count > 65535)
        count = 65535;
    w_u16(0x80097F1C, count);
    return t3_8004c390();
}

uint32 t3_80051550(uint32 first, uint32 second)
{
    uint32 entry;
    uint32 count;

    FUNCTION_MARKER(0x80051550, "SLUS_004.02");
    entry = 0x80098052 + (uint32)((sint32)first % 22) * 8;
    count = r_u16(entry) + 1;
    if (count > 65535)
        count = 65535;
    w_u16(entry, count);
    count = r_u16(0x80097F1C) + 1;
    if (count > 65535)
        count = 65535;
    w_u16(0x80097F1C, count);
    entry = 0x80098052 + (uint32)((sint32)second % 22) * 8;
    count = r_u16(entry) + 1;
    if (count > 65535)
        count = 65535;
    w_u16(entry, count);
    count = r_u16(0x80097F1C) + 1;
    if (count > 65535)
        count = 65535;
    w_u16(0x80097F1C, count);
    return t3_8004c390();
}

void t3_80069100(uint32 actor, uint32 opponent)
{
    uint32 phase;
    uint32 timer;
    uint32 value;
    sint32 index;
    sint32 remainder;

    FUNCTION_MARKER(0x80069100, "SLUS_004.02");
    t3_8006465c(actor, opponent, 0);
    phase = r_u16(0x800A04C6);
    switch (phase) {
    case 10:
        t3_80066084(actor, opponent, 1, 5517);
        goto start_timer;
    case 11:
        t3_80066084(actor, opponent, 0, 5517);
        break;
    case 20:
        t3_80068d50(actor, 0xFFFFFFFF);
        goto start_timer;
    case 21:
    case 41:
        t3_80068d50(actor, 0);
        break;
    case 30:
        t3_80068ba8(actor, opponent, 0xFFFFFFFF);
        goto start_timer;
    case 31:
        t3_80068ba8(actor, opponent, 0);
        break;
    case 40:
        t3_80068d50(actor, 0xFFFFFFFE);
start_timer:
        value = r_u16(0x800A04C6);
        w_u32(0x80098570, 720);
        w_u16(0x800A04C6, value + 1);
        break;
    default:
        index = r_s16(0x800A04C4);
        remainder = index % 4;
        w_u16(0x800A04C4, (uint32)remainder);
        value = r_u16(0x800242BC + (uint32)remainder * 2);
        w_u16(0x800A04C6, value);
        t3_80065ed8(actor, opponent);
        break;
    }
    timer = r_u32(0x80098570) - 1;
    w_u32(0x80098570, timer);
    if ((sint32)timer < 0) {
        value = r_u16(0x800A04C4) + 1;
        index = (sint16)value;
        w_u16(0x800A04C4, value);
        remainder = index % 4;
        value = r_u16(0x800242BC + (uint32)remainder * 2);
        w_u16(0x800A04C6, value);
    }
    t3_80064080(0);
}

void t3_80068d50(uint32 actor, uint32 reset)
{
    uint32 first_x;
    uint32 first_z;
    uint32 second_x;
    uint32 second_z;
    uint32 kind;
    uint32 frame;
    uint32 heading;
    uint32 height;
    uint32 lateral;
    uint32 index;
    uint32 sine;
    uint32 cosine;
    uint32 x;
    uint32 z;
    uint32 yaw;
    uint32 pitch;
    uint32 dot;

    FUNCTION_MARKER(0x80068D50, "SLUS_004.02");
    first_x = r_u32(0x800ADCA8);
    first_z = r_u32(0x800ADCB0);
    second_x = r_u32(0x800ADCB8);
    second_z = r_u32(0x800ADCC0);
    if ((sint32)reset < 0) {
        w_u32(0x800A0468, ~reset);
        heading = (uint32)r_s16(actor + 14);
        w_u32(0x800A046C, 0);
        w_u32(0x800A0470, heading);
    }
    kind = r_u32(0x800A0468);
    if (!kind) {
        height = 0 - 1578;
        lateral = 0;
    } else {
        /* The initialized camera owner selects modes 0 and 1 */
        frame = r_u32(0x800A046C);
        index = (t3_camera_divide(frame << 11, 720) & 0xFFF) * 2;
        sine = (uint32)r_s16(0x8001E8C4 + index);
        height = t3_camera_divide(sine * 100, 4096) - 1450;
        lateral = t3_camera_divide(frame * 8000, 720) - 4000;
    }
    frame = r_u32(0x800A046C);
    w_u32(0x800A046C, frame + 1);
    if (!r_u8(actor + 0x86)) {
        heading = 1024 - (uint32)(r_s16(actor + 14) / 16);
        w_u32(0x800A0470, heading);
    } else {
        heading = r_u32(0x800A0470);
    }
    index = heading;
    if ((sint32)index < 0)
        index += 63;
    index = (uint32)((sint32)index >> 5) & 0x1FFE;
    cosine = (uint32)r_s16(0x8001F0C4 + index);
    sine = (uint32)r_s16(0x8001E8C4 + index);
    x = first_x - t3_camera_divide(4597 * cosine, 4096);
    z = first_z - t3_camera_divide(4597 * sine, 4096);
    x += t3_camera_divide(lateral * sine, 4096);
    z -= t3_camera_divide(lateral * cosine, 4096);
    yaw = t3_8004b39c(t3_camera_divide(second_x + first_x, 2) - x,
        t3_camera_divide(second_z + first_z, 2) - z);
    yaw = ((uint32)(sint32)(sint16)yaw - 1024) << 6;
    index = 0 - yaw;
    if ((sint32)index < 0)
        index += 63;
    index = (uint32)((sint32)index >> 5) & 0x1FFE;
    sine = (uint32)r_s16(0x8001E8C4 + index);
    dot = sine * (first_x - x);
    cosine = (uint32)r_s16(0x8001F0C4 + index);
    dot += cosine * (first_z - z);
    pitch = t3_8004b39c(t3_camera_divide(dot, 4096), 0 - height);
    pitch = ((uint32)(sint32)(sint16)pitch << 6) - 6912;
    t3_80064020(x, height, z, pitch, yaw, 500, 0);
}

uint32 t3_800337a0(uint32 ball)
{
    uint32 mode;
    uint32 buffer;
    uint32 record;
    uint32 first;
    uint32 second;
    uint32 third;
    uint32 fourth;
    uint32 value;
    uint32 state;
    sint32 frame;

    FUNCTION_MARKER(0x800337A0, "SLUS_004.02");
    mode = r_u32(0x8009BCA8);
    if (mode == 7) {
        frame = r_s32(0x8009BCB8) % 30;
        buffer = r_u32(0x800ADE68);
        record = buffer ? buffer + (uint32)frame * 36 + 32504 : 0;
        first = r_u32(ball + 104);
        second = r_u32(ball + 108);
        third = r_u32(ball + 112);
        fourth = r_u32(ball + 116);
        w_u32(record, first);
        w_u32(record + 4, second);
        w_u32(record + 8, third);
        w_u32(record + 12, fourth);
        value = r_u32(ball + 152);
        w_u32(record + 16, value);
        first = r_u32(ball + 328);
        second = r_u32(ball + 332);
        w_u32(record + 26, first);
        w_u32(record + 30, second);
        value = r_u8(ball + 326);
        w_u8(record + 24, value);
        value = r_u8(ball + 327);
        w_u8(record + 25, value);
        value = r_u16(ball + 336);
        w_u16(record + 34, value);
        return value;
    }
    if (mode != 1 && mode != 3)
        return (sint32)mode < 4 ? 1 : 7;
    frame = (r_s32(0x8009BCBC) >> 1) % 30;
    buffer = r_u32(0x800ADE68);
    record = buffer ? buffer + (uint32)frame * 36 + 32504 : 0;
    first = r_u32(record);
    second = r_u32(record + 4);
    third = r_u32(record + 8);
    fourth = r_u32(record + 12);
    w_u32(ball + 104, first);
    w_u32(ball + 108, second);
    w_u32(ball + 112, third);
    w_u32(ball + 116, fourth);
    value = r_u32(record + 16);
    w_u32(ball + 152, value);
    first = r_u32(record + 26);
    second = r_u32(record + 30);
    w_u32(ball + 328, first);
    w_u32(ball + 332, second);
    value = r_u8(record + 24);
    w_u8(ball + 326, value);
    value = r_u8(record + 25);
    w_u8(ball + 327, value);
    value = r_u16(record + 34);
    w_u16(ball + 336, value);
    state = r_u8(record + 24);
    if (state == 7)
        return t3_bns_03_800b425c(1, 0);
    if (state < 8) {
        if (state == 6)
            return t3_bns_03_800b425c(0, 0);
        return 6;
    }
    if (state == 9)
        return t3_bns_03_800b425c(0, 1);
    return 9;
}
