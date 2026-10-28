#include "game_functions.h"
#include "psx_gpu.h"
#include "t3_audio.h"
#include "t3_data.h"

uint32 t3_bns_09_800edda0(uint32 state)
{
    uint32 result;
    uint32 style;
    uint32 direction;
    uint32 first;
    uint32 second;
    uint32 first_index;
    uint32 second_index;
    uint32 text;
    uint32 length;
    uint32 timer;
    uint32 x;
    uint32 y;
    uint32 color;
    uint32 packet;
    uint32 ordering;
    uint32 first_offset;
    uint32 second_offset;

    FUNCTION_MARKER(0x800EDDA0, "SLUS_004.02-BNS-logical-09");
    result = r_u32(0x800FA640);
    if (result == 0)
        return result;
    if (r_u8(state + 28) == 1) {
        first = r_u32(0x800FA638);
        second = r_u32(0x800FA63C);
        style = 2;
        if (first != second) {
            first_index = r_u8(state + 30);
            second_index = r_u8(state + 31);
            first = r_u32(0x800FA638 + first_index * 4);
            second = r_u32(0x800FA638 + second_index * 4);
            style = (sint32)second >= (sint32)first;
        }
        direction = r_u8(state + 30) != 0 ? 1 : 0xFFFFFFFF;
    } else {
        first = r_u32(0x800FA638);
        second = r_u32(0x800FA63C);
        if (first == second) {
            style = 2;
            direction = 0xFFFFFFFF;
        } else if ((sint32)second < (sint32)first) {
            style = 3;
            direction = 0xFFFFFFFF;
        } else {
            style = 4;
            direction = 1;
        }
    }
    text = t3_8004cc08(0x1F800000, 0x80097D08);
    t3_8004cc08(text, r_u32(0x800B8E08 + style * 8));
    length = t3_8007a418(text);
    timer = r_u32(0x800FA644);
    x = timer != 0 ? (uint32)((sint32)(timer * 368) / 30) * direction + 184 : 184;
    first_offset = r_u8(state + 101);
    second_offset = r_u8(state + 114);
    x -= (uint32)((sint32)(length * 22) >> 1);
    color = r_u32(0x800B8E0C + style * 8);
    y = (5 - (first_offset + second_offset + 1) / 3) * 15 + 158;
    t3_8004ce74(0x1F800000, color, 2, x, y);
    first = r_u32(0x800FA638);
    second = r_u32(0x800FA63C);
    color = first == second ? 0xC0C0C0 : (sint32)second < (sint32)first ? 0xFF : 0xC0FF;
    result = r_u32(0x800FA644);
    if (result == 0) {
        packet = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        packet = t3_8004da10(ordering + 28, packet, color | 0x62000000,
            ((x - 8) & 0xFFFF) | ((y - 2) << 16), ((length * 22 + 16) & 0xFFFF) | 0x002E0000);
        ordering = r_u32(0x800A9218);
        w_u32(0x800ADF64, packet);
        result = t3_8004daa8(ordering + 28, packet, 0);
        w_u32(0x800ADF64, result);
    }
    return result;
}

uint32 t3_bns_09_800ee074(uint32 entries, sint32 selected, uint32 x, uint32 y, uint32 alternate)
{
    uint32 packet;
    uint32 ordering;
    uint32 count;
    uint32 index;
    uint32 code;
    uint32 flags;
    uint32 left;
    uint32 right;
    uint32 top;
    uint32 bottom;

    FUNCTION_MARKER(0x800EE074, "SLUS_004.02-BNS-logical-09");
    packet = r_u32(0x800ADF64);
    count = r_u8(entries + 12);
    ordering = r_u32(0x800A9218);
    if (count >= 8)
        count = 8;
    for (index = 0; index < 8; ++index) {
        code = r_u8(entries + index);
        if (code >= 88)
            code = 88;
        flags = 0;
        if ((sint32)index < selected)
            flags = 96;
        else if ((sint32)index == selected && index < count)
            flags = alternate ? 1 : 2;
        if (index < r_u8(entries + 12)) {
            packet = t3_8004b928(ordering, packet, x, y, code, flags);
        } else {
            left = (x + 1) & 0xFFFF;
            right = (x + 35) & 0xFFFF;
            top = (y + 2) << 16;
            bottom = (y + 62) << 16;
            packet = t3_8004d8b0(ordering, packet, 0x38000000, 0,
                0x0C0C0C, 0x0C0C0C, left | top, right | top, left | bottom, right | bottom);
            left = x & 0xFFFF;
            right = (x + 36) & 0xFFFF;
            top = y << 16;
            bottom = (y + 64) << 16;
            packet = t3_8004d7d4(ordering, packet, 0x28303030,
                left | top, right | top, left | bottom, right | bottom);
        }
        x += 36;
    }
    w_u32(0x800ADF64, packet);
    return 0x800B0000;
}

uint32 t3_bns_09_800f0aa8(void)
{
    FUNCTION_MARKER(0x800F0AA8, "SLUS_004.02-BNS-logical-09");
    t3_80029638(1);
    t3_8004ca40(0x800F23AC, 0x800B9394);
    return t3_80029638(0);
}

uint32 t3_bns_09_800f0aec(void)
{
    FUNCTION_MARKER(0x800F0AEC, "SLUS_004.02-BNS-logical-09");
    t3_80029638(1);
    t3_8004ca40(0x800F23AC, 0x800D8400);
    return t3_80029638(0);
}

uint32 t3_bns_09_800f22c8(void)
{
    uint32 total = 0;
    uint32 index;

    FUNCTION_MARKER(0x800F22C8, "SLUS_004.02-BNS-logical-09");
    for (index = 0; index < 10; ++index)
        total += r_u32(0x800AFAC8 + index * 8);
    return (sint32)total > 359999 ? 359999 : total;
}

uint32 t3_bns_09_800f2310(uint8 *destination, sint32 index)
{
    uint32 source;
    uint32 word;
    uint32 half;

    FUNCTION_MARKER(0x800F2310, "SLUS_004.02-BNS-logical-09");
    if (index >= 10)
        return 1;
    source = 0x800AFAC8 + (uint32)index * 8;
    destination[4] = r_u8(source + 4);
    destination[5] = r_u8(source + 5);
    word = r_u32(source);
    destination[0] = (uint8)word;
    destination[1] = (uint8)(word >> 8);
    destination[2] = (uint8)(word >> 16);
    destination[3] = (uint8)(word >> 24);
    half = r_u16(source + 6);
    destination[6] = (uint8)half;
    destination[7] = (uint8)(half >> 8);
    return 0;
}

uint32 t3_bns_09_800f2368(void)
{
    FUNCTION_MARKER(0x800F2368, "SLUS_004.02-BNS-logical-09");
    return r_u8(0x800980FE) & 1;
}

void t3_800ff0c4(void)
{
    // Initialized gallery paths overwrite both unused register carriers
    t3_bns_08_800ff0c4(0, 0);
}

static void t3_gallery_black_tile(void)
{
    uint32 tile = 0x801143D8 + r_u32(0x800ADEFC) * 16;
    uint32 ordering;

    setTile(psx_addr(tile, 16));
    w_u8(tile + 6, 0);
    w_u8(tile + 5, 0);
    w_u8(tile + 4, 0);
    ordering = r_u32(0x800A9218);
    w_u16(tile + 12, 368);
    w_u16(tile + 8, 0);
    w_u16(tile + 10, 0);
    w_u16(tile + 14, 480);
    t3_8007ba74(ordering, tile);
}

static sint32 t3_gallery_next_series(void)
{
    uint32 kind = r_u8(0x800ADC91);
    uint32 unlocked = r_s16(0x800ADC9C) != 0;
    uint32 attempt;

    if (kind < 1 || kind > 3)
        kind = 3;
    for (attempt = 0; attempt < 3; ++attempt) {
        kind = kind == 3 ? 1 : kind + 1;
        if (!unlocked && kind != 3)
            continue;
        if (t3_data_select_series(kind))
            return (sint32)kind;
#ifdef _DEBUG
        fprintf(stderr, "Gallery resources unavailable for Tekken %u\n", kind);
#endif
    }
    return -1;
}

// Replace disc changes with extracted-series selection
uint32 t3_bns_08_800ff0c4(uint32 incoming_a2, uint32 incoming_s3)
{
    uint32 buffer;
    uint32 saved_packet;
    uint32 first_input;
    uint32 second_input;
    uint32 buttons;
    uint32 phase;
    uint32 selection;
    uint32 mode;
    uint32 action;
    sint32 kind;

    FUNCTION_MARKER(0x800FF0C4, "SLUS_004.02-BNS-logical-08");
    buffer = r_u32(0x800ADEFC);
    saved_packet = r_u32(0x800ADF64);
    w_u32(0x800ADF64, 0x80184DE0 + buffer * 20480);
    first_input = r_u16(0x800ADF08);
    second_input = r_u16(0x800ADF0A);
    phase = (uint32)r_s16(0x800AE224);
    buttons = first_input | second_input;
    switch (phase) {
    case 0:
        t3_80055884(0);
        t3_8004b688(0);
        w_u8(0x800ADC91, 3);
        w_u16(0x800ADC9C, t3_bns_08_80100f58());
        t3_bns_08_8010215c();
        selection = t3_bns_08_800fefd4();
        mode = r_u8(0x800ADC90);
        w_u8(0x800ADC94, selection);
        w_u8(0x800ADC96, selection);
        if (mode == 0) {
            w_u16(0x800AE224, 2);
            break;
        }
        w_u16(0x800AE224, 1);
        t3_8004ca40(0x80105FA0, 0x801143D8);
        t3_bns_08_80102ae4(4);
        t3_bns_08_80102ae4(5);
        t3_bns_08_80102ae4(6);
        t3_bns_08_80102ae4(7);
        break;
    case 1:
        t3_bns_08_80101fe8(0, first_input, incoming_a2);
        action = t3_bns_08_8010217c(incoming_s3);
        switch (action) {
        case 0:
            break;
        case 1:
            mode = r_u8(0x800ADC90);
            if (mode == 1)
                w_u16(0x800AE224, 2);
            else if (mode - 2 < 2)
                t3_8006b194(r_u8(0x800ADC95), 0);
            break;
        case 2:
            t3_8006b54c(0);
            break;
        case 3:
            t3_8006b6c8(60);
            break;
        case 4:
            w_u16(0x800AE224, 10);
            break;
        case 5:
            w_u16(0x800AE224, 4);
            break;
        default:
            t3_8007ae58(1);
            goto play_movie;
        }
        break;
    case 2:
play_movie:
        t3_bns_08_800fdc78(r_u8(0x800ADC94));
        while (t3_8006c558() != 0)
            t3_bns_08_800fdfe4();
        if (r_u8(0x800ADC90) == 0) {
            t3_bns_08_80102eec(1, (sint32)incoming_s3);
            t3_80074e94();
            t3_8006ae14(4, 1);
            w_u16(0x800AE224, 3);
        } else {
            w_u16(0x800AE224, 1);
            t3_bns_08_80102ae4(6);
        }
        break;
    case 3:
        t3_gallery_black_tile();
        if (t3_bns_08_80102eec(0, (sint32)incoming_s3) != 0)
            t3_80050190();
        break;
    case 4:
        t3_8006b54c(0);
        t3_bns_08_8010215c();
        w_u16(0x800AE224, 6);
        t3_bns_08_80101fe8(1, 0, 0);
        break;
    case 5:
        if ((buttons & 0x8F0) != 0)
            w_u16(0x800AE224, 6);
        t3_bns_08_80101fe8(1, 0, 0);
        break;
    case 6:
        t3_bns_08_80101fe8(r_s16(0x800ADC9C) != 0 ? 2 : 4, 0, 0);
        w_u16(0x800AE224, 8);
        break;
    case 7:
        t3_bns_08_80101fe8(4, 0, 0);
        w_u16(0x800AE224, 8);
        break;
    case 8:
        w_u16(0x800AE224, 9);
        break;
    case 9:
        t3_bns_08_80101fe8(3, 0, 0);
        kind = t3_gallery_next_series();
        t3_bns_08_8010215c();
        if (r_s16(0x800ADC9C) != 0) {
            if ((uint32)kind - 1 < 3) {
                w_u8(0x800ADC91, kind);
                w_u16(0x800AE224, 1);
                t3_bns_08_80102ae4((uint32)kind * 2 - 2);
                t3_bns_08_80102ae4((uint32)kind * 2 - 1);
                break;
            }
        } else if (kind == 3) {
            w_u8(0x800ADC91, 3);
            w_u16(0x800AE224, 1);
            t3_bns_08_80102ae4(4);
            t3_bns_08_80102ae4(5);
            break;
        }
        w_u16(0x800AE224, 5);
        break;
    case 10:
        t3_gallery_black_tile();
        t3_8006b54c(0);
        if (t3_data_select_series(3)) {
            t3_8004f8f8(4);
        } else {
            t3_bns_08_80101fe8(5, 0, 0);
            if ((buttons & 0x8F0) != 0)
                w_u16(0x800AE224, 7);
        }
        break;
    default:
        t3_8007ae58(1);
        break;
    }
    w_u32(0x800ADF64, saved_packet);
    return 0x800B0000;
}

typedef struct T3_GALLERY_MOVIE
{
    uint32 kind;
    uint32 selection;
} T3_GALLERY_MOVIE;

static T3_GALLERY_MOVIE gallery_movie;

// Preserve gallery setup around the shared fullscreen service
uint32 t3_bns_08_800fdc78(uint32 selection)
{
    uint32 kind;
    uint32 table;
    uint32 descriptors;
    uint32 entry;
    uint32 descriptor;
    uint32 value;
    DISPENV display;

    kind = t3_8006ad18();
    t3_8006c588(31);
    t3_800697d4(-1);
    switch (kind) {
    case 1:
        table = 0x800B94FC;
        descriptors = 0x800B908C;
        break;
    case 2:
        table = 0x800B933C;
        descriptors = 0x800B8ECC;
        break;
    case 3:
        table = 0x800B91AC;
        descriptors = 0x800B8D58;
        break;
    default:
        return t3_8007ae58(1);
    }
    entry = table + (selection << 4);
    descriptor = descriptors + ((uint32)r_u8(entry) << 4);
    gallery_movie.kind = kind;
    gallery_movie.selection = selection;
    w_u16(0x80114438, r_u16(entry + 10) & 4);
    w_u16(0x8011443A, (r_u16(entry + 10) >> 8) & 15);
    w_u16(0x8011443C, r_u16(entry + 10) & 3);
    w_u16(0x80114444, r_u16(entry + 10) & 8);
    w_u32(0x80114450, r_u32(entry + 12));
    t3_8006b540(r_u8(entry + 8));
    value = t3_8006b978(-1);
    t3_8006b978((sint32)value);
    t3_8006bc98(127, -1);
    t3_8006ba44();
    w_u16(0x8011444C, r_s16(0x8011443A) == 3);
    if (r_s16(0x8011443C) == 3) {
        w_u16(0x8011443C, 0);
        w_u16(0x8011443E, 1);
    } else {
        w_u16(0x8011443E, 0);
    }
    w_u32(0x8011445C, (uint32)r_s16(entry + 6));
    w_u32(0x80114468, r_u32(descriptor));
    value = r_s16(entry + 2) < 257 ? 256 : 320;
    w_u16(0x80114460, value);
    w_u16(0x80114462, 240);
    w_u16(0x80114464, r_u16(entry + 2));
    value = r_u16(entry + 4);
    w_u32(0x80114458, 0);
    w_u16(0x80114432, 1);
    w_u16(0x80114434, 0);
    w_u16(0x80114466, value);
    t3_8006b54c(0);
    t3_8006ad28(5);
    if (r_s16(0x8011443A) != 0) {
        t3_bns_08_800ff7a8();
        t3_bns_08_800ff830(r_s16(0x8011443A) - 1);
    }
    w_u16(0x80114436, 0);
    SetDispMask(0);
    if (r_s16(0x8011443A) != 0)
        t3_bns_08_800ff8e0(r_s16(0x80114432));
    w_u16(0x80114448, 1);
    w_u16(0x8011444A, 3);
    SetDefDispEnv(&display, 0, 0, 384, 240);
    display.isinter = 1;
    display.screen.x = r_s8(0x80097F24);
    display.screen.w = 258;
    display.screen.h = 240;
    display.screen.y = r_s8(0x80097F25);
    t3_80029dc0(0);
    VSync(0);
    PutDispEnv(&display);
    w_u16(0x80114446, 0);
    w_u16(0x80114430, 0);
    t3_8006ad28(5);
    w_u16(0x80114442, 1);
    return 0;
}

// Delegate playback and its finish loop while retaining game cleanup
uint32 t3_bns_08_800fdfe4(void)
{
    XportFmvPresentation presentation;
    uint32 phase;
    uint32 value;
    sint32 result;

    t3_8006b6fc();
    if (t3_8006c558() == 0)
        return 0xFFFFFFFF;
    if (r_s16(0x80114442) > 0) {
        ClearImage((PSX_RECT *)psx_addr(0x80105F80, 8), 0, 0, 0);
        w_u16(0x80114442, r_u16(0x80114442) - 1);
    }
    phase = (uint32)r_s16(0x80114448);
    if (phase == 1) {
        value = (uint16)(r_u16(0x8011444A) - 1);
        w_u16(0x8011444A, value);
        if (value == 0) {
            t3_80029dc0(0);
            VSync(0);
            w_u16(0x80114448, 2);
        }
        t3_80029dc0(0);
        VSync(0);
        return 0;
    }
    if (phase == 2) {
        result = -1;
        if (t3_fmv_presentation_configure(&presentation, r_u16(0x80114464),
            r_u16(0x80114466), r_u16(0x8011443A)) &&
            t3_fmv_finish_init(&presentation, r_u16(0x8011443C))) {
            result = t3_menu05_play_movie(gallery_movie.kind, gallery_movie.selection,
                r_u16(0x80114438) != 0, &presentation);
        }
        if (result == 1) {
            w_u16(0x80114436, 1);
            if (r_s16(0x8011443E) == 0) {
                t3_8006bc98(0, -1);
                if (r_s16(0x80114434) == 0)
                    t3_file_audio_stop();
            }
            w_u16(0x80114448, 5);
            w_u16(0x80114446, 1);
        } else {
            t3_8006bc98(0, -1);
            if (r_s16(0x80114434) == 0)
                t3_file_audio_stop();
            if (result != 2)
                t3_bns_08_800fda2c(0x80114478, 0);
            w_u16(0x80114448, 5);
            w_u16(0x8011443E, 0);
            w_u16(0x8011443C, 0);
            w_u16(0x80114446, result == 2 ? 0xFFFE : 0xFFFF);
        }
        return 0;
    }
    if (phase == 5) {
        t3_bns_08_800fda2c(0x80114478, r_u16(0x80114444));
        value = (uint32)r_s16(0x8011443E);
        w_u16(0x80114448, 0);
        t3_8006ad28(value != 0 ? 7 : 1);
        return (uint32)r_s16(0x80114446);
    }
    t3_8007ae58(1);
    return 0;
}

uint32 t3_bns_08_800fef94(uint32 selection)
{
    FUNCTION_MARKER(0x800FEF94, "SLUS_004.02-BNS-logical-08");
    return t3_bns_08_800fdc78(selection);
}

uint32 t3_bns_08_800fefb4(void)
{
    FUNCTION_MARKER(0x800FEFB4, "SLUS_004.02-BNS-logical-08");
    return t3_bns_08_800fdfe4();
}

static void t3_credits_brightness(uint32 distance)
{
    uint32 candidate = ((distance * 2) | 1) & 255;

    if (candidate >= 91) {
        uint32 color = (90 - ((candidate - 90) >> 1)) & 255;

        w_u8(0x80184DB8, color);
        if (color < 80)
            w_u8(0x80184DB8, 80);
    }
}

static void t3_credits_fade(uint32 source, uint32 first, uint32 inverted, uint32 half)
{
    sint32 red;
    sint32 green;
    sint32 blue;
    uint32 left;
    uint32 top;
    uint32 text_left;
    uint32 right;
    uint32 width;
    uint32 red_byte;
    uint32 green_byte;
    uint32 blue_byte;
    uint32 product = first * 7;

    red = t3_bns_08_80104be8(half ? (sint32)product / 2 : (sint32)product, 253);
    product = r_u32(source) * (half ? 3 : 6);
    w_u8(0x80184D96, inverted ? 253 - (uint32)red : (uint32)red);
    green = t3_bns_08_80104be8((sint32)product, 219);
    product = r_u32(source) * 5;
    w_u8(0x80184D97, inverted ? 219 - (uint32)green : (uint32)green);
    blue = t3_bns_08_80104be8(half ? (sint32)product / 2 : (sint32)product, 182);
    if (inverted) {
        left = r_u32(0x80184D9C);
        top = r_u32(0x80184DA0);
        text_left = r_u32(0x80184DA4);
        right = r_u32(0x80184DA8);
        width = r_u32(0x80184DB0);
        w_u8(0x80184D98, 182 - (uint32)blue);
    } else {
        w_u8(0x80184D98, blue);
        width = r_u32(0x80184DB0);
        left = r_u32(0x80184D9C);
        top = r_u32(0x80184DA0);
        text_left = r_u32(0x80184DA4);
        right = r_u32(0x80184DA8);
    }
    red_byte = r_u8(0x80184D96);
    green_byte = r_u8(0x80184D97);
    blue_byte = r_u8(0x80184D98);
    t3_bns_08_80104190(left, top, text_left, right, width, red_byte, green_byte, blue_byte);
}

uint32 t3_bns_08_80102eec(uint32 initialize, sint32 incoming_style)
{
    uint32 phase;
    uint32 buffer;
    uint32 index;
    uint32 pointer;
    uint32 width;
    uint32 center;
    uint32 margin;
    uint32 left;
    uint32 right;
    uint32 y;
    uint32 text_y;
    uint32 font;
    uint32 color;
    uint32 distance;
    uint32 counter;
    uint32 timer;
    uint32 result;
    uint32 first_input;
    uint32 second_input;
    sint32 signed_timer;
    sint32 terminal;
    sint32 style = incoming_style;

    FUNCTION_MARKER(0x80102EEC, "SLUS_004.02-BNS-logical-08");
    w_u32(0x80184DDC, r_u32(0x800A9218) + 28);
    if (initialize != 0) {
        w_u32(0x80184D90, 0);
        w_u32(0x80184D88, 480);
        w_u32(0x80184D8C, 0);
        w_u8(0x80184D94, 120);
        w_u8(0x80184D95, 0);
        w_u32(0x80184DAC, 32);
        w_u32(0x80184DB4, 0);
        w_u32(0x8010ED64, 0);
        w_u32(0x80184DBC, 0);
        w_u32(0x80184DD0, 0);
        w_u32(0x80184DCC, 0);
        w_u8(0x80184D96, 253);
        w_u8(0x80184D97, 219);
        w_u8(0x80184D98, 182);
        t3_bns_08_80104f30(0x8010ED70);
        t3_bns_08_80104f30(0x8010FF68);
        t3_bns_08_80104f30(0x80111D30);
        goto finish;
    }
    first_input = r_u16(0x800ADF08);
    second_input = r_u16(0x800ADF0A);
    if (((first_input | second_input) & 0x8F0) != 0)
        w_u32(0x8010ED68, 1);
    buffer = r_u32(0x800ADEFC);
    w_u32(0x80184DD4, 0x80184DE0 + buffer * 20480);
    phase = r_u32(0x80184D90);
    w_u32(0x80184DD8, 0x8018EDE0 + buffer * 30720);
    switch (phase) {
    case 0:
        if (r_s32(0x8010ED64) >= 31) {
            t3_800753bc(0, 0xC7C0, 0);
            w_u32(0x80184D90, 1);
            w_u32(0x8010ED64, 0);
        }
        break;
    case 1:
        if (r_u32(0x8010ED64) == 240) {
            t3_8006ae14(4, 0);
            w_u32(0x80184D90, 2);
        }
        break;
    case 2:
        if (t3_8006b9e0() == 0) {
            w_u32(0x80184D90, 3);
            w_u32(0x80184D8C, 0);
            w_u32(0x8010ED64, 0);
        }
        break;
    case 3:
        if (r_u32(0x8010ED64) == 60) {
            w_u32(0x80184D90, 4);
            w_u32(0x8010ED64, 0);
        }
        break;
    case 4:
        w_u8(0x80184DB8, 90);
        while ((index = r_u32(0x80184DBC)), r_u32(0x8010E24C + index * 4) != 0) {
            font = index == 0;
            y = (font ? 0 : 18) + 220;
            index = r_u32(0x80184DBC);
            pointer = r_u32(0x8010E24C + index * 4);
            width = t3_bns_08_80104c04(pointer, font);
            w_u32(0x80184DC4, width);
            center = 152 - (uint32)((sint32)width / 2);
            w_u32(0x80184DB0, width);
            margin = r_u32(0x80184DAC);
            signed_timer = r_s32(0x8010ED64);
            w_u32(0x80184D9C, center);
            w_u32(0x80184DA0, y);
            w_u32(0x80184DA4, center + margin);
            w_u32(0x80184DA8, center + margin + width);
            if (signed_timer < 37) {
                if (signed_timer < 30)
                    t3_credits_brightness(440 - y);
                timer = r_u32(0x8010ED64);
                t3_credits_fade(0x8010ED64, timer, 1, 0);
            } else if (signed_timer >= 181) {
                distance = 210 - (uint32)signed_timer;
                w_u8(0x80184DB8, 3 * distance + 1);
                w_u32(0x80184DC8, r_u8(0x80184D94) - 4 * distance);
            }
            if (r_s32(0x8010ED64) < 210) {
                left = r_u32(0x80184DA4);
                text_y = r_u32(0x80184DA0);
                index = r_u32(0x80184DBC);
                color = r_u8(0x80184DB8);
                pointer = r_u32(0x8010E24C + index * 4);
                t3_bns_08_80104fa0(left, text_y, font, color, pointer);
            }
            w_u32(0x80184DBC, r_u32(0x80184DBC) + 1);
        }
        w_u32(0x80184DBC, 0);
        if (r_u32(0x8010ED64) == 239)
            w_u32(0x80184D90, 5);
        break;
    case 5:
        y = r_u32(0x80184D88);
        if ((sint32)y < -18) {
            index = r_u32(0x80184D8C) + 1;
            pointer = r_u32(0x8010E258 + index * 4);
            w_u32(0x80184D88, y + 18);
            w_u32(0x80184D8C, index);
            if (pointer == 0) {
                w_u8(0x80184DB8, 0);
                w_u32(0x80184D88, 0);
                w_u32(0x80184D90, 6);
                return 0;
            }
        }
        w_u8(0x80184DB8, 90);
        w_u32(0x80184DBC, r_u32(0x80184D8C));
        y = r_u32(0x80184D88);
        w_u32(0x80184DC0, y);
        if ((sint32)y < 480) {
            for (;;) {
                pointer = r_u32(0x8010E258 + r_u32(0x80184DBC) * 4);
                if (pointer == 0 && r_u32(0x8010ED6C) == 0) {
                    w_u32(0x8010ED6C, 1);
                    break;
                }
                switch (r_u8(pointer)) {
                case '0': style = 0; break;
                case '1': style = 1; break;
                case '2': style = 2; break;
                case '3': style = r_u8(0x80097F26) ? 3 : -1; break;
                case '4': style = r_u8(0x80097F26) ? -1 : 0; break;
                case '5': style = (r_u32(0x80097EF4) & 0x40000) ? 1 : -1; break;
                default: break;
                }
                if (style == 0 || style == 1 || style == 3) {
                    index = r_u32(0x80184DBC);
                    pointer = r_u32(0x8010E258 + index * 4);
                    width = t3_bns_08_80104c04(pointer, style == 1 ? 0 : 1);
                    center = 152 - (uint32)((sint32)width / 2);
                    w_u32(0x80184DC4, width);
                    margin = r_u32(0x80184DAC);
                    w_u32(0x80184D9C, center);
                    w_u32(0x80184DB0, width);
                    left = center + margin;
                    y = r_u32(0x80184DC0);
                    if (style == 0)
                        terminal = r_s32(0x8010ED6C);
                    right = left + width;
                    w_u32(0x80184DA4, left);
                    w_u32(0x80184DA8, right);
                    text_y = y + (style == 0 ? 4 : style == 3 ? 20 : 0);
                    w_u32(0x80184DA0, text_y);
                    if (style == 0 && terminal != 0) {
                        if (terminal == 1)
                            w_u32(0x80184DD0, r_u32(0x80184DD0) + 1);
                        if (r_u32(0x80184DD0) == 620) {
                            w_u32(0x8010ED6C, 2);
                            w_u32(0x80184DD0, 0);
                        }
                        terminal = r_s32(0x8010ED6C);
                        if (terminal >= 2) {
                            counter = r_u32(0x80184DCC) + 1;
                            w_u32(0x80184DCC, counter);
                            if (terminal == 2) {
                                color = r_u8(0x80184DCC);
                                w_u32(0x80184DC8, counter);
                                w_u8(0x80184DB8, 80 - color);
                                if (counter == 80) {
                                    w_u32(0x8010ED6C, 3);
                                    w_u32(0x80184DCC, 0);
                                }
                                t3_credits_fade(0x80184DC8, counter, 0, 0);
                            } else if (terminal == 3) {
                                w_u32(0x80184DC8, counter);
                                w_u8(0x80184DB8, 1);
                                if (counter == 80) {
                                    w_u32(0x8010ED6C, 4);
                                    w_u8(0x80184DB8, 0);
                                    w_u32(0x80184D88, 0);
                                    w_u32(0x80184D90, 6);
                                    return 0;
                                }
                                t3_credits_fade(0x80184DC8, counter, 1, 0);
                            } else {
                                w_u8(0x80184DB8, 80);
                            }
                        }
                    } else if (y - (style == 0 ? 356 : style == 1 ? 360 : 340) < 80) {
                        distance = 440 - text_y;
                        w_u32(0x80184DC8, distance);
                        t3_credits_brightness(distance);
                        t3_credits_fade(0x80184DC8, r_u32(0x80184DC8), 1, 0);
                    } else if (y - (style == 0 ? 16 : style == 1 ? 20 : 0) < 80) {
                        distance = 100 - text_y;
                        color = r_u8(0x80184DA0);
                        w_u32(0x80184DC8, distance);
                        w_u8(0x80184DB8, color - 19);
                        t3_credits_fade(0x80184DC8, distance, 0, 1);
                    } else if (y - (style == 0 ? 16 : style == 1 ? 20 : 0) < 420) {
                        w_u8(0x80184DB8, 80);
                    } else {
                        w_u8(0x80184DB8, 1);
                        t3_bns_08_80104190(center, text_y, left, right, width, 253, 219, 182);
                    }
                    if (style != 1)
                        (void)r_u32(0x80184DB4);
                    if (style == 1) {
                        y = r_u32(0x80184DC0);
                        left = r_u32(0x80184DA4);
                        color = r_u8(0x80184DB8);
                    } else {
                        left = r_u32(0x80184DA4);
                        color = r_u8(0x80184DB8);
                        y = r_u32(0x80184DC0);
                    }
                    index = r_u32(0x80184DBC);
                    pointer = r_u32(0x8010E258 + index * 4);
                    if (style == 1)
                        (void)r_u32(0x80184DB4);
                    t3_bns_08_80104fa0(left, y + (style == 0 ? 4 : style == 3 ? 20 : 0), style == 1 ? 0 : 1, color, pointer + 1);
                }
                index = r_u32(0x80184DBC);
                y = r_u32(0x80184DC0) + 18;
                w_u32(0x80184DC0, y);
                w_u32(0x80184DBC, index + 1);
                if ((sint32)y >= 480)
                    break;
            }
        }
        if (r_u32(0x8010ED6C) == 0) {
            if (r_u32(0x80184DB4) != 0)
                w_u32(0x80184D88, r_u32(0x80184D88) - 2);
            else
                w_u32(0x80184D88, r_u32(0x80184D88) - 1);
        }
        break;
    case 6:
        if (r_u32(0x8010ED64) == 9060) {
            w_u32(0x8010ED64, 0);
            w_u32(0x80184D90, 7);
        }
        break;
    case 7:
        signed_timer = r_s32(0x8010ED64);
        if (signed_timer >= 180)
            return 1;
        if (signed_timer < 11)
            w_u8(0x80184DB8, r_u8(0x80184DB8) + 12);
        else if (signed_timer >= 121)
            w_u8(0x80184DB8, r_u8(0x80184DB8) - 2);
        t3_bns_08_80104ca4(r_u8(0x80184DB8));
        break;
    default:
        break;
    }
    counter = r_u32(0x80184DB4) + 1;
    w_u32(0x80184DB4, counter);
    if (counter == 2)
        w_u32(0x80184DB4, 0);
finish:
    timer = r_u32(0x8010ED64);
    result = r_u32(0x8010ED68);
    w_u32(0x8010ED64, timer + 1);
    return result;
}

static uint32 t3_credits_quad(uint32 packet, uint32 color0, uint32 color1, uint32 color2, uint32 color3, uint32 xy0, uint32 xy1, uint32 xy2, uint32 xy3)
{
    uint32 ordering = r_u32(0x80184DDC);

    w_u32(0x80184DD4, packet);
    return t3_8004d8b0(ordering, packet, color0, color1, color2, color3, xy0, xy1, xy2, xy3);
}

uint32 t3_bns_08_80104190(uint32 left, uint32 top, uint32 text_left, uint32 right, uint32 width, uint32 red, uint32 green, uint32 blue)
{
    uint32 packet;
    uint32 ordering;
    uint32 bright;
    uint32 dim;
    uint32 bright_command;
    uint32 dim_command;
    uint32 edge_left;
    uint32 fade_left;
    uint32 inner_left;
    uint32 cap_left;
    uint32 stem_left;
    uint32 join_left;
    uint32 text_right;
    uint32 inner_right;
    uint32 cap_right;
    uint32 outer_right;
    uint32 stem_right;
    uint32 edge_right;
    uint32 join_right;
    uint32 row0;
    uint32 row1;
    uint32 row2;
    uint32 row3;
    uint32 row4;
    uint32 row5;

    FUNCTION_MARKER(0x80104190, "SLUS_004.02-BNS-logical-08");
    ordering = r_u32(0x80184DDC);
    packet = r_u32(0x80184DD4);
    red &= 255;
    green &= 255;
    blue &= 255;
    bright = red | (green << 8) | (blue << 16);
    dim = (red >> 1) | ((green >> 1) << 8) | ((blue >> 1) << 16);
    bright_command = bright | 0x3A000000;
    dim_command = dim | 0x3A000000;
    packet = t3_8004daa8(ordering, packet, 32);

    edge_left = (left - 32) & 0xFFFF;
    fade_left = (left - 16) & 0xFFFF;
    inner_left = (left + 24) & 0xFFFF;
    cap_left = (left + 48) & 0xFFFF;
    stem_left = (left + 36) & 0xFFFF;
    join_left = (left + 22) & 0xFFFF;
    text_right = (text_left + width - 24) & 0xFFFF;
    text_left = (text_left + 16) & 0xFFFF;
    inner_right = (right - 24) & 0xFFFF;
    cap_right = right & 0xFFFF;
    outer_right = (right + 40) & 0xFFFF;
    stem_right = (right - 12) & 0xFFFF;
    edge_right = (right + 56) & 0xFFFF;
    join_right = (right + 2) & 0xFFFF;
    row0 = (top - 2) << 16;
    row1 = (top + 1) << 16;
    row2 = (top + 4) << 16;
    row3 = (top + 7) << 16;
    row4 = (top + 10) << 16;
    row5 = (top + 13) << 16;

    packet = t3_credits_quad(packet, 0x3A000000, dim, 0, dim,
        fade_left | row0, inner_left | row0, fade_left | row1, inner_left | row1);
    packet = t3_credits_quad(packet, dim_command, dim, dim, bright,
        inner_left | row0, cap_left | row0, inner_left | row1, cap_left | row1);
    packet = t3_credits_quad(packet, dim_command, dim, bright, bright,
        text_left | row0, text_right | row0, text_left | row1, text_right | row1);
    packet = t3_credits_quad(packet, dim_command, dim, dim, bright,
        cap_right | row0, inner_right | row0, cap_right | row1, inner_right | row1);
    packet = t3_credits_quad(packet, 0x3A000000, dim, 0, dim,
        outer_right | row0, cap_right | row0, outer_right | row1, cap_right | row1);

    packet = t3_credits_quad(packet, 0x3A000000, bright, 0, bright,
        edge_left | row1, stem_left | row1, edge_left | row2, stem_left | row2);
    packet = t3_credits_quad(packet, bright_command, bright, bright, bright,
        stem_left | row1, cap_left | row1, stem_left | row2, cap_left | row2);
    packet = t3_credits_quad(packet, bright_command, bright, bright, bright,
        text_left | row1, text_right | row1, text_left | row2, text_right | row2);
    packet = t3_credits_quad(packet, bright_command, bright, bright, bright,
        inner_right | row1, stem_right | row1, inner_right | row2, stem_right | row2);
    packet = t3_credits_quad(packet, bright_command, 0, bright, 0,
        stem_right | row1, edge_right | row1, stem_right | row2, edge_right | row2);

    packet = t3_credits_quad(packet, 0x3A000000, dim, 0, bright,
        edge_left | row2, join_left | row2, edge_left | row3, join_left | row3);
    packet = t3_credits_quad(packet, dim_command, bright, bright, bright,
        join_left | row2, cap_left | row2, join_left | row3, cap_left | row3);
    packet = t3_credits_quad(packet, bright_command, bright, bright, bright,
        text_left | row2, text_right | row2, text_left | row3, text_right | row3);
    packet = t3_credits_quad(packet, dim_command, bright, bright, bright,
        join_right | row2, inner_right | row2, join_right | row3, inner_right | row3);
    packet = t3_credits_quad(packet, 0x3A000000, dim, 0, bright,
        edge_right | row2, join_right | row2, edge_right | row3, join_right | row3);

    packet = t3_credits_quad(packet, 0x3A000000, bright, 0, bright,
        edge_left | row3, stem_left | row3, edge_left | row4, stem_left | row4);
    packet = t3_credits_quad(packet, bright_command, bright, bright, bright,
        stem_left | row3, cap_left | row3, stem_left | row4, cap_left | row4);
    packet = t3_credits_quad(packet, bright_command, bright, bright, bright,
        text_left | row3, text_right | row3, text_left | row4, text_right | row4);
    packet = t3_credits_quad(packet, bright_command, bright, bright, bright,
        inner_right | row3, stem_right | row3, inner_right | row4, stem_right | row4);
    packet = t3_credits_quad(packet, bright_command, 0, bright, 0,
        stem_right | row3, edge_right | row3, stem_right | row4, edge_right | row4);

    packet = t3_credits_quad(packet, dim_command, 0, dim, 0,
        inner_left | row4, fade_left | row4, inner_left | row5, fade_left | row5);
    packet = t3_credits_quad(packet, bright_command, dim, dim, dim,
        cap_left | row4, inner_left | row4, cap_left | row5, inner_left | row5);
    packet = t3_credits_quad(packet, bright_command, bright, dim, dim,
        text_left | row4, text_right | row4, text_left | row5, text_right | row5);
    packet = t3_credits_quad(packet, bright_command, dim, dim, dim,
        inner_right | row4, cap_right | row4, inner_right | row5, cap_right | row5);
    packet = t3_credits_quad(packet, dim_command, 0, dim, 0,
        cap_right | row4, outer_right | row4, cap_right | row5, outer_right | row5);
    w_u32(0x80184DD4, packet);
    return packet;
}

uint32 t3_bns_08_80104ca4(uint32 color)
{
    uint32 packet;
    uint32 next_packet;
    uint32 ordering;
    uint32 tag;
    uint32 link;
    uint32 result;
    uint32 part;
    uint32 left;
    uint32 right;
    uint32 texture_right;

    FUNCTION_MARKER(0x80104CA4, "SLUS_004.02-BNS-logical-08");
    next_packet = r_u32(0x80184DD8);
    for (part = 0; part < 2; ++part)
    {
        left = part == 0 ? 54 : 310;
        right = part == 0 ? 310 : 315;
        texture_right = part == 0 ? 255 : 4;
        w_u8(next_packet + 3, 9);
        w_u8(r_u32(0x80184DD8) + 7, 0x2C);
        packet = r_u32(0x80184DD8);
        w_u8(packet + 7, r_u8(packet + 7) & 0xFE);
        packet = r_u32(0x80184DD8);
        w_u8(packet + 7, r_u8(packet + 7) | 2);
        w_u8(r_u32(0x80184DD8) + 4, color);
        w_u8(r_u32(0x80184DD8) + 5, color);
        w_u8(r_u32(0x80184DD8) + 6, color);
        packet = r_u32(0x80184DD8);
        w_u16(packet + 22, part == 0 ? 10 : 11);
        w_u16(packet + 14, 0x2C28);
        w_u8(packet + 12, 0);
        w_u8(r_u32(0x80184DD8) + 13, 128);
        w_u8(r_u32(0x80184DD8) + 20, texture_right);
        w_u8(r_u32(0x80184DD8) + 21, 128);
        w_u8(r_u32(0x80184DD8) + 28, 0);
        w_u8(r_u32(0x80184DD8) + 29, 172);
        w_u8(r_u32(0x80184DD8) + 36, texture_right);
        w_u8(r_u32(0x80184DD8) + 37, 172);
        packet = r_u32(0x80184DD8);
        ordering = r_u32(0x80184DDC);
        tag = r_u32(packet);
        w_u16(packet + 8, left);
        w_u16(packet + 10, 220);
        w_u16(packet + 16, right);
        w_u16(packet + 18, 220);
        w_u16(packet + 24, left);
        w_u16(packet + 26, 265);
        w_u16(packet + 32, right);
        w_u16(packet + 34, 265);
        link = r_u32(ordering);
        w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
        next_packet = packet + 40;
        link = r_u32(ordering);
        w_u32(0x80184DD8, next_packet);
        result = (link & 0xFF000000) | (packet & 0xFFFFFF);
        w_u32(ordering, result);
    }
    return result;
}

uint32 t3_bns_08_80104fa0(uint32 x, uint32 y, uint32 font, uint32 color, uint32 text)
{
    uint32 widths;
    uint32 space;
    uint32 row_base;
    uint32 bottom;
    uint32 character;
    uint32 width;
    uint32 texture_x;
    uint32 texture_y;
    uint32 packet;
    uint32 other_packet;
    uint32 ordering;
    uint32 tag;
    uint32 link;
    sint32 columns;
    sint32 cell_width;
    sint32 glyph;

    FUNCTION_MARKER(0x80104FA0, "SLUS_004.02-BNS-logical-08");
    color &= 255;
    if ((font & 0xFFFF) != 0)
    {
        columns = 21;
        row_base = 0;
        cell_width = 12;
        space = 8;
        widths = 0x800BB918;
    }
    else
    {
        columns = 14;
        row_base = 36;
        cell_width = 18;
        space = 12;
        widths = 0x800BB958;
    }
    bottom = y + 12;
    for (;;)
    {
        character = r_u8(text);
        ++text;
        if (character == 0)
            return 0;
        if (character == 32)
            width = space;
        else
        {
            glyph = (sint32)character - (character < 0x31 ? 33 : 34);
            texture_y = (uint32)((glyph / columns) * 12) + row_base;
            width = r_u8(widths + (uint32)glyph);
            texture_x = (uint32)((glyph % columns) * cell_width);
            packet = r_u32(0x80184DD8);
            w_u8(packet + 3, 9);
            packet = r_u32(0x80184DD8);
            w_u8(packet + 7, 0x2C);
            packet = r_u32(0x80184DD8);
            w_u8(packet + 7, r_u8(packet + 7) & 0xFE);
            packet = r_u32(0x80184DD8);
            w_u8(packet + 7, r_u8(packet + 7) | 2);
            w_u8(r_u32(0x80184DD8) + 4, color);
            w_u8(r_u32(0x80184DD8) + 5, color);
            w_u8(r_u32(0x80184DD8) + 6, color);
            packet = r_u32(0x80184DD8);
            w_u8(packet + 12, texture_x);
            other_packet = r_u32(0x80184DD8);
            w_u16(packet + 22, 42);
            w_u16(packet + 14, 0x1828);
            w_u8(other_packet + 13, texture_y);
            w_u8(r_u32(0x80184DD8) + 20, texture_x + width);
            w_u8(r_u32(0x80184DD8) + 21, texture_y);
            w_u8(r_u32(0x80184DD8) + 28, texture_x);
            w_u8(r_u32(0x80184DD8) + 29, texture_y + 12);
            w_u8(r_u32(0x80184DD8) + 36, texture_x + width);
            w_u8(r_u32(0x80184DD8) + 37, texture_y + 12);
            packet = r_u32(0x80184DD8);
            w_u16(packet + 16, x + width);
            w_u16(packet + 32, x + width);
            ordering = r_u32(0x800A9218);
            tag = r_u32(packet);
            w_u16(packet + 8, x);
            w_u16(packet + 10, y);
            w_u16(packet + 18, y);
            w_u16(packet + 24, x);
            w_u16(packet + 26, bottom);
            w_u16(packet + 34, bottom);
            link = r_u32(ordering + 24);
            w_u32(packet, (tag & 0xFF000000) | (link & 0xFFFFFF));
            link = r_u32(ordering + 24);
            w_u32(0x80184DD8, packet + 40);
            w_u32(ordering + 24, (link & 0xFF000000) | (packet & 0xFFFFFF));
        }
        x += width;
    }
}

sint32 t3_bns_08_80104be8(sint32 value, sint32 limit)
{
    FUNCTION_MARKER(0x80104BE8, "SLUS_004.02-BNS-logical-08");
    return value < limit ? value : limit;
}

uint32 t3_bns_08_80104c04(uint32 text, uint32 font)
{
    uint32 widths;
    uint32 space;
    uint32 width;
    uint32 character;

    FUNCTION_MARKER(0x80104C04, "SLUS_004.02-BNS-logical-08");
    widths = font != 0 ? 0x800BB918 : 0x800BB958;
    space = font != 0 ? 8 : 10;
    character = r_u8(text);
    width = 0;
    while (character != 0)
    {
        if (character < 0x31)
        {
            if (character == 0x20)
                width += space;
            else
                width += r_u8(widths + r_u8(text) - 33);
        }
        else
            width += r_u8(widths + r_u8(text) - 34);
        ++text;
        character = r_u8(text);
    }
    return width;
}

uint32 t3_bns_08_80104f30(uint32 tim)
{
    uint32 result;
    T3_TIM_IMAGE image;

    FUNCTION_MARKER(0x80104F30, "SLUS_004.02-BNS-logical-08");
    result = t3_8007efc8(tim);
    if (result != 0)
        return result;
    t3_80029638(1);
    t3_8007efd8(&image);
    t3_8007c650(image.clut_rectangle, image.clut_pixels);
    DrawSync(0);
    t3_8007c650(image.image_rectangle, image.image_pixels);
    DrawSync(0);
    return t3_80029638(0);
}

uint32 t3_bns_08_801000fc(uint32 x, uint32 y, sint32 index)
{
    uint32 packet;
    uint32 descriptor;
    uint32 ordering;
    uint32 clut_x;
    uint32 clut_y;
    uint32 texture_x;
    uint32 texture_y;
    uint32 result;

    FUNCTION_MARKER(0x801000FC, "SLUS_004.02-BNS-logical-08");
    packet = r_u32(0x800ADF64);
    if (index < 0)
        return 0x800B9984;
    descriptor = 0x800B9984 + (uint32)index * 12;
    clut_x = r_u16(descriptor);
    ordering = r_u32(0x800A9218);
    clut_y = (uint32)r_s16(descriptor + 2);
    texture_x = (uint32)r_s16(descriptor + 4);
    texture_y = (uint32)r_s16(descriptor + 6);
    ordering += 28;
    setSprt(psx_addr(packet, 20));
    w_u8(packet + 12, (texture_x & 0x7F) << 1);
    w_u16(packet + 16, 48);
    w_u16(packet + 18, 64);
    w_u16(packet + 8, x);
    w_u16(packet + 10, y);
    w_u8(packet + 13, texture_y);
    w_u16(packet + 14, (clut_y << 6) | ((clut_x >> 4) & 63));
    w_u8(packet + 6, 128);
    w_u8(packet + 5, 128);
    w_u8(packet + 4, 128);
    t3_8007ba74(ordering, packet);
    result = t3_8004daa8(ordering, packet + 20,
        ((texture_y & 0x100) >> 4) | ((texture_x & 0x380) >> 6) | 0x80);
    w_u32(0x800ADF64, result);
    return result;
}

uint32 t3_bns_08_80100234(uint32 x, uint32 y)
{
    uint32 packet;
    uint32 ordering;
    uint32 clut_x;
    uint32 clut_y;
    uint32 texture_x;
    uint32 texture_y;
    uint32 result;

    FUNCTION_MARKER(0x80100234, "SLUS_004.02-BNS-logical-08");
    packet = r_u32(0x800ADF64);
    clut_x = r_u16(0x800B9C60);
    ordering = r_u32(0x800A9218);
    clut_y = (uint32)r_s16(0x800B9C62);
    texture_x = (uint32)r_s16(0x800B9C64);
    texture_y = (uint32)r_s16(0x800B9C66);
    ordering += 28;
    setSprt(psx_addr(packet, 20));
    w_u8(packet + 12, 96);
    w_u16(packet + 16, 48);
    w_u16(packet + 18, 64);
    w_u16(packet + 8, x);
    w_u16(packet + 10, y);
    w_u8(packet + 13, texture_y);
    w_u16(packet + 14, (clut_y << 6) | ((clut_x >> 4) & 63));
    w_u8(packet + 6, 128);
    w_u8(packet + 5, 128);
    w_u8(packet + 4, 128);
    t3_8007ba74(ordering, packet);
    result = t3_8004daa8(ordering, packet + 20,
        ((texture_y & 0x100) >> 4) | ((texture_x & 0x3C0) >> 6));
    w_u32(0x800ADF64, result);
    return result;
}

uint32 t3_bns_08_800ffe60(uint32 table, uint32 selection)
{
    uint32 source;
    uint32 character;
    uint32 flags;
    uint32 mask;

    FUNCTION_MARKER(0x800FFE60, "SLUS_004.02-BNS-logical-08");
    source = r_u32(table + selection * 28 + 16);
    character = r_u8(source);
    switch (character)
    {
    case '0':
        flags = r_u32(0x80097EF8);
        mask = (flags & 0x10) != 0;
        if (flags & 0x80000)
            mask |= 2;
        if (mask == 0)
            return 1;
        source = mask == 1 ? 0x800BAB40 :
            (mask == 2 ? 0x800BB0E4 : 0x800BB710);
        break;
    case '1':
        mask = (r_u32(0x80097EF8) & 0x800) != 0;
        if (r_u32(0x80097EFC) & 0x800)
            mask |= 2;
        if (mask == 0)
            return 1;
        source = mask == 1 ? 0x800BAABC :
            (mask == 2 ? 0x800BB728 : 0x800BB730);
        break;
    case '2':
        mask = (r_u32(0x80097EF8) & 0x10000) != 0;
        if (r_u32(0x80097EFC) & 0x10000)
            mask |= 2;
        if (mask == 0)
            return 1;
        source = mask == 1 ? 0x800BB73C : 0x800BB0F8;
        break;
    case '3':
        flags = r_u32(0x80097EF8);
        mask = (flags & 0x4000) != 0;
        if (flags & 0x100000)
            mask |= 2;
        if (mask == 0)
            return 1;
        source = mask == 1 ? 0x800BB104 :
            (mask == 2 ? 0x800BB0D8 : 0x800BB74C);
        break;
    }
    return t3_8004cc08(0x80184D08, source);
}

uint32 t3_bns_08_80100fdc(uint32 count, uint32 table)
{
    uint32 selected;
    uint32 packet;
    uint32 ordering;
    uint32 row;
    uint32 column;
    uint32 item;
    uint32 descriptor;
    uint32 portrait_table;
    uint32 clut_x;
    uint32 clut_y;
    uint32 texture_x;
    uint32 texture_y;
    uint32 x;
    uint32 y;
    uint32 page;
    uint32 color;
    uint32 active;
    uint32 mode;
    uint32 pointer;
    uint32 length;
    sint32 available;
    sint32 portrait;
    sint32 action;
    sint32 message;

    FUNCTION_MARKER(0x80100FDC, "SLUS_004.02-BNS-logical-08");
    selected = r_u8(0x800ADC96);
    if (r_s16(0x800ADC9A) == 0 && (r_u32(0x800AFA4C) & 0x1C) != 0) {
        ordering = r_u32(0x800A9218) + 28;
        packet = r_u32(0x800ADF64);
        x = (selected % 6) * 48 + 38;
        y = ((selected / 6 - r_u8(0x800ADC97)) << 6) + 69;
        w_u32(0x800ADF64, t3_bns_08_8010063c(packet, ordering, 2, x, y));
    }
    y = 72;
    for (row = 0; row < 4; ++row) {
        x = 40;
        for (column = 0; column < 6; ++column) {
            item = (row + r_u8(0x800ADC97)) * 6;
            active = r_u8(0x800ADC92);
            item += column;
            if ((sint32)item >= (sint32)active)
                break;
            descriptor = table + item * 28;
            available = (sint32)t3_bns_08_801004a8(descriptor);
            if (available >= 0 || r_s16(descriptor) >= 0) {
                packet = r_u32(0x800ADF64);
                clut_x = r_u16(0x800B9C60);
                ordering = r_u32(0x800A9218) + 28;
                clut_y = (uint32)r_s16(0x800B9C62);
                texture_x = (uint32)r_s16(0x800B9C64);
                texture_y = (uint32)r_s16(0x800B9C66);
                setSprt(psx_addr(packet, 20));
                w_u16(packet + 8, x);
                w_u8(packet + 12, 96);
                w_u8(packet + 13, texture_y);
                w_u16(packet + 16, 48);
                w_u16(packet + 14, (clut_y << 6) | ((clut_x >> 4) & 63));
                w_u8(packet + 6, 128);
                w_u8(packet + 5, 128);
                w_u8(packet + 4, 128);
                w_u16(packet + 10, y);
                w_u16(packet + 18, 64);
                t3_8007ba74(ordering, packet);
                page = ((texture_y & 0x100) >> 4) | ((texture_x & 0x3C0) >> 6);
                packet = t3_8004daa8(ordering, packet + 20, page);
                if (available >= 0) {
                    portrait = r_s16(descriptor + 4);
                    w_u32(0x800ADF64, packet);
                } else {
                    w_u32(0x800ADF64, packet);
                    portrait = 0;
                }
                if (portrait >= 0) {
                    portrait_table = 0x800B9984 + (uint32)portrait * 12;
                    clut_x = r_u16(portrait_table);
                    ordering = r_u32(0x800A9218) + 28;
                    clut_y = (uint32)r_s16(portrait_table + 2);
                    texture_x = (uint32)r_s16(portrait_table + 4);
                    texture_y = (uint32)r_s16(portrait_table + 6);
                    setSprt(psx_addr(packet, 20));
                    w_u16(packet + 8, x);
                    w_u8(packet + 12, (texture_x & 127) << 1);
                    w_u8(packet + 13, texture_y);
                    w_u16(packet + 16, 48);
                    w_u16(packet + 14, (clut_y << 6) | ((clut_x >> 4) & 63));
                    w_u8(packet + 6, 128);
                    w_u8(packet + 5, 128);
                    w_u8(packet + 4, 128);
                    w_u16(packet + 10, y);
                    w_u16(packet + 18, 64);
                    t3_8007ba74(ordering, packet);
                    page = ((texture_y & 0x100) >> 4) | ((texture_x & 0x380) >> 6) | 0x80;
                    w_u32(0x800ADF64, t3_8004daa8(ordering, packet + 20, page));
                }
            }
            x += 48;
        }
        y += 64;
    }
    if (r_s16(0x800ADC9C) != 0) {
        color = r_s16(0x800ADC9A) == 3 ? 5 : 10;
        active = r_s16(0x800ADC9A) == 3;
        t3_bns_08_80100c2c(color, 8, 6, 82, 344, 0x800BB794, active);
        color = r_s16(0x800ADC9A) == 2 ? 5 : 10;
        active = r_s16(0x800ADC9A) == 2;
        t3_bns_08_80100c2c(color, 8, 6, 162, 344, 0x800BB79C, active);
        x = 252;
    } else {
        x = 162;
    }
    color = r_s16(0x800ADC9A) == 1 ? 5 : 10;
    active = r_s16(0x800ADC9A) == 1;
    t3_bns_08_80100c2c(color, 8, 6, x, 344, 0x800BB7A4, active);
    if (r_s16(0x800ADC9A) == 0 && (sint32)selected < (sint32)count) {
        descriptor = table + selected * 28;
        if ((sint32)t3_bns_08_801004a8(descriptor) < 0) {
            if (r_s16(descriptor + 4) <= 0)
                return 386;
            return t3_8004ce74(0x800BB7AC, 1u, 6u, 193u, 386u);
        }
        t3_bns_08_800ffe60(table, selected);
        portrait = r_s16(descriptor + 4);
        if (portrait > 0) {
            portrait_table = 0x800B9984 + (uint32)portrait * 12;
            packet = r_u32(0x800ADF64);
            clut_x = r_u16(portrait_table);
            ordering = r_u32(0x800A9218) + 28;
            clut_y = (uint32)r_s16(portrait_table + 2);
            texture_x = (uint32)r_s16(portrait_table + 4);
            texture_y = (uint32)r_s16(portrait_table + 6);
            setSprt(psx_addr(packet, 20));
            w_u16(packet + 8, 30);
            w_u16(packet + 10, 384);
            w_u8(packet + 12, (texture_x & 127) << 1);
            w_u16(packet + 16, 48);
            w_u16(packet + 18, 64);
            w_u8(packet + 13, texture_y);
            w_u16(packet + 14, (clut_y << 6) | ((clut_x >> 4) & 63));
            w_u8(packet + 6, 128);
            w_u8(packet + 5, 128);
            w_u8(packet + 4, 128);
            t3_8007ba74(ordering, packet);
            page = ((texture_y & 0x100) >> 4) | ((texture_x & 0x380) >> 6) | 0x80;
            w_u32(0x800ADF64, t3_8004daa8(ordering, packet + 20, page));
        }
        length = t3_8007a418(0x80184D08);
        x = ((233u - length * 13) >> 1) + 96;
        t3_8004ce74(0x800BA154, 1u, 6u, x, 386u, 0x80184D08u);
        if (r_u32(descriptor + 24) == 0) {
            length = t3_8007a418(r_u32(descriptor + 20));
            x = ((230u - length * 9) >> 1) + 100;
            return t3_8004ce74(0x800BA154, 0u, 5u, x, 422u, r_u32(descriptor + 20));
        }
        length = t3_8007a418(r_u32(descriptor + 20));
        x = ((230u - length * 9) >> 1) + 100;
        t3_8004ce74(0x800BA154, 0u, 5u, x, 413u, r_u32(descriptor + 20));
        length = t3_8007a418(r_u32(descriptor + 24));
        x = ((230u - length * 9) >> 1) + 100;
        return t3_8004ce74(0x800BA154, 0u, 5u, x, 431u, r_u32(descriptor + 24));
    }
    mode = r_u8(0x800ADC90);
    action = r_s16(0x800ADC9A);
    message = -1;
    if (mode == 1) {
        if (action == 2)
            message = 5;
        else if (action == 3)
            message = 0;
        else if (action == 1)
            message = 7;
    } else {
        if (action == 2)
            message = 4;
        else if (action == 1)
            message = 7;
        else if (action == 3)
            message = 1;
        else if (action == 4)
            message = 6;
    }
    if (message < 0)
        return 0x80110000;
    pointer = 0x8010E1E4 + (uint32)message * 4;
    length = t3_8007a418(r_u32(pointer));
    x = (368u - length * 9) >> 1;
    return t3_8004ce74(0x800BA154, 0u, 6u, x, 400u, r_u32(pointer));
}

uint32 t3_bns_08_80100c2c(uint32 color, uint32 border_x, uint32 border_y, uint32 x, uint32 y, uint32 text, uint32 selected)
{
    uint32 text_width;
    uint32 left;
    uint32 top;
    uint32 width;
    uint32 height;
    uint32 packet;
    uint32 tile;
    uint32 ordering;
    uint32 value;
    uint32 background;
    uint32 result;

    FUNCTION_MARKER(0x80100C2C, "SLUS_004.02-BNS-logical-08");
    text_width = t3_8007a418(text) * 9;
    t3_8004ce74(0x800BA154, 0u, color, x, y, text);
    packet = r_u32(0x800ADF64);
    left = (x - border_x) & 0xFFFF;
    top = (y - border_y) & 0xFFFF;
    width = ((border_x << 1) + text_width) & 0xFFFF;
    height = ((border_y << 1) + 16) & 0xFFFF;
    ordering = r_u32(0x800A9218) + 28;
    setTile(psx_addr(packet, 16));
    w_u32(packet + 4, (r_u32(packet + 4) & 0xFF000000) | 0xFFFFFF);
    w_u16(packet + 8, left);
    w_u16(packet + 12, width);
    w_u16(packet + 10, top);
    w_u16(packet + 14, 2);
    t3_8007ba74(ordering, packet);
    tile = packet + 16;
    setTile(psx_addr(tile, 16));
    w_u32(tile + 4, (r_u32(tile + 4) & 0xFF000000) | 0xFFFFFF);
    w_u16(tile + 8, left);
    w_u16(tile + 12, 2);
    w_u16(tile + 10, top);
    w_u16(tile + 14, height);
    t3_8007ba74(ordering, tile);
    tile = packet + 32;
    setTile(psx_addr(tile, 16));
    w_u32(tile + 4, (r_u32(tile + 4) & 0xFF000000) | 0xFFFFFF);
    w_u16(tile + 8, left);
    w_u16(tile + 12, width);
    w_u16(tile + 10, top + height - 2);
    w_u16(tile + 14, 2);
    t3_8007ba74(ordering, tile);
    tile = packet + 48;
    setTile(psx_addr(tile, 16));
    w_u32(tile + 4, (r_u32(tile + 4) & 0xFF000000) | 0xFFFFFF);
    w_u16(tile + 8, left + width - 2);
    w_u16(tile + 12, 2);
    w_u16(tile + 10, top);
    w_u16(tile + 14, height);
    t3_8007ba74(ordering, tile);
    w_u32(0x800ADF64, packet + 64);
    if (selected != 0) {
        value = t3_8004dfe0(r_u32(0x800AE218) << 4);
        background = t3_8004df6c(0xFF4020, (value >> 1) + 128);
    } else {
        background = 0x800000;
    }
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218) + 28;
    setTile(psx_addr(packet, 16));
    w_u16(packet + 8, x - border_x);
    value = r_u32(packet + 4);
    w_u16(packet + 12, (border_x << 1) + text_width);
    w_u16(packet + 14, (border_y << 1) + 16);
    w_u16(packet + 10, y - border_x);
    w_u32(packet + 4, (value & 0xFF000000) | background);
    result = t3_8007ba74(ordering, packet);
    w_u32(0x800ADF64, packet + 16);
    return result;
}

uint32 t3_bns_08_80100350(uint32 x, uint32 y, uint32 index)
{
    uint32 descriptor;
    uint32 packet;
    uint32 ordering;
    uint32 texture_x;
    uint32 texture_y;
    uint32 flags;
    uint32 height;
    uint32 page;
    uint32 result;
    sint32 clut_x;
    sint32 clut_y;

    FUNCTION_MARKER(0x80100350, "SLUS_004.02-BNS-logical-08");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    descriptor = 0x800B9C6C + index * 14;
    clut_x = r_s16(descriptor);
    clut_y = r_s16(descriptor + 2);
    texture_x = (uint32)r_s16(descriptor + 4);
    texture_y = (uint32)r_s16(descriptor + 6);
    ordering += 28;
    setSprt(psx_addr(packet, 20));
    w_u16(packet + 8, x);
    w_u16(packet + 10, y);
    flags = r_u16(descriptor + 12);
    w_u8(packet + 12, (texture_x & 63) << ((flags & 1) != 0 ? 1 : 2));
    w_u8(packet + 13, texture_y);
    w_u16(packet + 16, r_u16(descriptor + 8));
    height = r_u16(descriptor + 10);
    w_u16(packet + 14, ((uint32)clut_y << 6) | ((uint32)(clut_x >> 4) & 63));
    w_u8(packet + 6, 128);
    w_u8(packet + 5, 128);
    w_u8(packet + 4, 128);
    w_u16(packet + 18, height);
    t3_8007ba74(ordering, packet);
    flags = r_u16(descriptor + 12);
    page = ((flags & 1) << 7) | ((texture_y & 0x100) >> 4) | ((texture_x & 0x380) >> 6);
    result = t3_8004daa8(ordering, packet + 20, page);
    w_u32(0x800ADF64, result);
    return result;
}

uint32 t3_bns_08_8010193c(uint32 count, uint32 table)
{
    uint32 packet;
    uint32 tile;
    uint32 ordering;
    uint32 selected;
    uint32 row;
    uint32 item;
    uint32 color;
    uint32 text_y;
    uint32 highlight_y;
    uint32 value;
    uint32 pointer;
    uint32 length;
    uint32 center;
    uint32 mode;
    uint32 active;
    sint32 action;
    sint32 message;

    FUNCTION_MARKER(0x8010193C, "SLUS_004.02-BNS-logical-08");
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218) + 28;
    selected = r_u8(0x800ADC96);
    setTile(psx_addr(packet, 16));
    w_u32(packet + 4, (r_u32(packet + 4) & 0xFF000000) | 0xFFFFFF);
    w_u16(packet + 8, 24);
    w_u16(packet + 12, 206);
    w_u16(packet + 10, 72);
    w_u16(packet + 14, 2);
    t3_8007ba74(ordering, packet);
    tile = packet + 16;
    setTile(psx_addr(tile, 16));
    w_u32(tile + 4, (r_u32(tile + 4) & 0xFF000000) | 0xFFFFFF);
    w_u16(tile + 8, 24);
    w_u16(tile + 12, 2);
    w_u16(tile + 10, 72);
    w_u16(tile + 14, 290);
    t3_8007ba74(ordering, tile);
    tile = packet + 32;
    setTile(psx_addr(tile, 16));
    w_u32(tile + 4, (r_u32(tile + 4) & 0xFF000000) | 0xFFFFFF);
    w_u16(tile + 8, 24);
    w_u16(tile + 12, 206);
    w_u16(tile + 10, 360);
    w_u16(tile + 14, 2);
    t3_8007ba74(ordering, tile);
    tile = packet + 48;
    setTile(psx_addr(tile, 16));
    w_u32(tile + 4, (r_u32(tile + 4) & 0xFF000000) | 0xFFFFFF);
    w_u16(tile + 8, 228);
    w_u16(tile + 12, 2);
    w_u16(tile + 10, 72);
    w_u16(tile + 14, 290);
    t3_8007ba74(ordering, tile);
    w_u32(0x800ADF64, packet + 64);
    text_y = 80;
    highlight_y = 79;
    for (row = 0; row < 14; ++row) {
        item = r_u8(0x800ADC97);
        mode = r_u8(0x800ADC90);
        item += row;
        color = mode == 2 ? 11 : 10;
        if (r_s16(0x800ADC9A) == 0 && item == selected)
            color = 5;
        pointer = r_u32(table + item * 28 + 16);
        t3_8004ce74(0x800BB7B8, 0u, color, 30u, text_y, item + 1, pointer);
        if (r_s16(0x800ADC9A) == 0 && item == selected) {
            value = t3_8004dfe0(r_u32(0x800AE218) << 4);
            color = t3_8004df6c(0xFF4020, (value >> 1) + 128);
            packet = r_u32(0x800ADF64);
            ordering = r_u32(0x800A9218) + 28;
            setTile(psx_addr(packet, 16));
            w_u16(packet + 8, 25);
            w_u16(packet + 12, 204);
            value = r_u32(packet + 4);
            w_u16(packet + 10, highlight_y);
            w_u16(packet + 14, 18);
            w_u32(packet + 4, (value & 0xFF000000) | color);
            t3_8007ba74(ordering, packet);
            w_u32(0x800ADF64, packet + 16);
        }
        highlight_y += 20;
        text_y += 20;
    }
    color = r_s16(0x800ADC9A) == 2 ? 5 : 10;
    active = r_s16(0x800ADC9A) == 2;
    t3_bns_08_80100c2c(color, 8, 6, 250, 200, 0x800BB7C8, active);
    color = r_s16(0x800ADC9A) == 4 ? 5 : 10;
    active = r_s16(0x800ADC9A) == 4;
    t3_bns_08_80100c2c(color, 8, 6, 250, 230, 0x800BB7D0, active);
    color = r_s16(0x800ADC9A) == 3 ? 5 : 10;
    active = r_s16(0x800ADC9A) == 3;
    t3_bns_08_80100c2c(color, 8, 6, 250, 260, 0x800BB794, active);
    color = r_s16(0x800ADC9A) == 1 ? 5 : 10;
    active = r_s16(0x800ADC9A) == 1;
    t3_bns_08_80100c2c(color, 8, 6, 250, 290, 0x800BB7A4, active);
    if (r_s16(0x800ADC9A) == 0 && (sint32)selected < (sint32)count) {
        pointer = table + selected * 28 + 16;
        length = t3_8007a418(r_u32(pointer));
        center = (368u - length * 13) >> 1;
        t3_8004ce74(0x800BA154, 1u, 6u, center + 16, 390u, r_u32(pointer));
        t3_bns_08_80100350(center - 14, 386, 7);
        if (r_u8(0x800ADC90) == 2)
            t3_8004ce74(0x800BB7DC, 0u, 5u, 260u, 422u);
        else
            t3_8004ce74(0x800BB7EC, 0u, 5u, 260u, 422u);
    } else {
        mode = r_u8(0x800ADC90);
        action = r_s16(0x800ADC9A);
        message = -1;
        if (mode == 1) {
            if (action == 2)
                message = 5;
            else if (action == 3)
                message = 0;
            else if (action == 1)
                message = 7;
        } else {
            if (action == 2)
                message = 4;
            else if (action == 1)
                message = 7;
            else if (action == 3)
                message = 1;
            else if (action == 4)
                message = 6;
        }
        if (message >= 0) {
            pointer = 0x8010E1E4 + (uint32)message * 4;
            length = t3_8007a418(r_u32(pointer));
            center = (368u - length * 9) >> 1;
            t3_8004ce74(0x800BA154, 0u, 6u, center, 400u, r_u32(pointer));
        }
    }
    t3_bns_08_80100350(242, 90, r_u8(0x800ADC91) * 2u - 2);
    return t3_bns_08_80100350(242, 346, r_u8(0x800ADC91) * 2u - 1);
}

uint32 t3_bns_08_8010063c(uint32 packet, uint32 ordering, uint32 index, uint32 x, uint32 y)
{
    uint32 descriptor;
    uint32 tile;
    uint32 first;
    uint32 second;
    uint32 edge;

    FUNCTION_MARKER(0x8010063C, "SLUS_004.02-BNS-logical-08");
    descriptor = 0x800BA028 + (index << 5);
    setTile(psx_addr(packet, 16));
    w_u32(packet + 4, (r_u32(packet + 4) & 0xFF000000) | r_u32(descriptor + 28));
    w_u16(packet + 8, r_u16(descriptor) + x);
    w_u16(packet + 12, r_u16(descriptor + 4));
    w_u16(packet + 10, r_u16(descriptor + 2) + y);
    w_u16(packet + 14, r_u8(descriptor + 11));
    t3_8007ba74(ordering, packet);
    tile = packet + 16;
    setTile(psx_addr(tile, 16));
    w_u32(tile + 4, (r_u32(tile + 4) & 0xFF000000) | r_u32(descriptor + 28));
    w_u16(tile + 8, r_u16(descriptor) + x);
    w_u16(tile + 12, r_u8(descriptor + 10));
    w_u16(tile + 10, r_u16(descriptor + 2) + y);
    w_u16(tile + 14, r_u16(descriptor + 6));
    t3_8007ba74(ordering, tile);
    tile = packet + 32;
    setTile(psx_addr(tile, 16));
    w_u32(tile + 4, (r_u32(tile + 4) & 0xFF000000) | r_u32(descriptor + 28));
    w_u16(tile + 8, r_u16(descriptor) + x);
    w_u16(tile + 12, r_u16(descriptor + 4));
    first = r_u16(descriptor + 2);
    second = r_u16(descriptor + 6);
    edge = r_u8(descriptor + 11);
    w_u16(tile + 10, second + first + y - edge);
    w_u16(tile + 14, r_u8(descriptor + 11));
    t3_8007ba74(ordering, tile);
    tile = packet + 48;
    setTile(psx_addr(tile, 16));
    w_u32(tile + 4, (r_u32(tile + 4) & 0xFF000000) | r_u32(descriptor + 28));
    first = r_u16(descriptor);
    second = r_u16(descriptor + 4);
    edge = r_u8(descriptor + 10);
    w_u16(tile + 8, second + first + x - edge);
    w_u16(tile + 12, r_u8(descriptor + 10));
    w_u16(tile + 10, r_u16(descriptor + 2) + y);
    w_u16(tile + 14, r_u16(descriptor + 6));
    t3_8007ba74(ordering, tile);
    return packet + 64;
}

uint32 t3_bns_08_800ffcdc(uint32 x, uint32 y)
{
    uint32 packet;
    uint32 ordering;
    uint32 texture;
    uint32 packed_y;

    FUNCTION_MARKER(0x800FFCDC, "SLUS_004.02-BNS-logical-08");
    packed_y = y << 16;
    packet = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    texture = r_u32(0x800BB700);
    ordering += 28;
    packet = t3_8004d9bc(ordering, packet, 0x65000000, (x & 0xFFFF) | packed_y, 0x14000C, texture);
    packet = t3_8004daa8(ordering, packet, 31);
    ordering = r_u32(0x800A9218);
    texture = r_u32(0x800BB704);
    w_u32(0x800ADF64, packet);
    ordering += 28;
    packet = t3_8004d9bc(ordering, packet, 0x65000000, ((x + 18) & 0xFFFF) | packed_y, 0x14000C, texture);
    packet = t3_8004daa8(ordering, packet, 31);
    ordering = r_u32(0x800A9218);
    texture = r_u32(0x800BB708);
    w_u32(0x800ADF64, packet);
    ordering += 28;
    packet = t3_8004d9bc(ordering, packet, 0x65000000, ((x + 36) & 0xFFFF) | packed_y, 0x14000C, texture);
    packet = t3_8004daa8(ordering, packet, 31);
    ordering = r_u32(0x800A9218);
    texture = r_u32(0x800BB70C);
    w_u32(0x800ADF64, packet);
    ordering += 28;
    packet = t3_8004d9bc(ordering, packet, 0x65000000, ((x + 80) & 0xFFFF) | packed_y, 0x14000C, texture);
    packet = t3_8004daa8(ordering, packet, 31);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_08_80100834(uint32 presentation)
{
    sint32 message;
    uint32 pointer;
    uint32 length;
    uint32 ordering;
    uint32 packet;
    uint32 flags;
    uint32 border;
    uint32 blend;
    uint32 result;

    FUNCTION_MARKER(0x80100834, "SLUS_004.02-BNS-logical-08");
    message = -1;
    if (presentation == 0)
        t3_bns_08_80102ca8();
    switch (presentation) {
    case 1:
        t3_8004ce74(0x800BB75C, 0u, 9u, 24u, 5u, 6u);
        t3_bns_08_800ffcdc(197, 432);
        message = r_s16(0x800ADC9C) != 0 ? 2 : 3;
        break;
    case 2:
    case 3:
        message = r_s16(0x800ADC9C) != 0 ? 2 : 3;
        break;
    case 5:
        t3_8004ce74(0x800BB75C, 0u, 9u, 24u, 5u, 6u);
        t3_bns_08_800ffcdc(197, 432);
        message = 3;
        break;
    case 4:
    case 6:
        message = 3;
        break;
    default:
        break;
    }
    if (message >= 0) {
        pointer = 0x8010E1E4 + (uint32)message * 4;
        length = t3_8007a418(r_u32(pointer));
        t3_8004ce74(0x800BA154, 0u, 6u, (368u - length * 9) >> 1, 400u, r_u32(pointer));
    }
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    ordering += 28;
    flags = r_u16(0x800BA050);
    border = r_u8(0x800BA052);
    blend = flags & 2;
    if (border != 0) {
        packet = t3_bns_08_8010063c(packet, ordering, 1, 0, 0);
        flags = r_u16(0x800BA050);
    }
    if (flags & 1) {
        setPolyG4(psx_addr(packet, 36));
        w_u16(packet + 8, r_u16(0x800BA048));
        w_u16(packet + 10, r_u16(0x800BA04A));
        w_u16(packet + 16, r_u16(0x800BA048) + r_u16(0x800BA04C));
        w_u16(packet + 18, r_u16(0x800BA04A));
        w_u16(packet + 24, r_u16(0x800BA048));
        w_u16(packet + 26, r_u16(0x800BA04A) + r_u16(0x800BA04E));
        w_u16(packet + 32, r_u16(0x800BA048) + r_u16(0x800BA04C));
        w_u16(packet + 34, r_u16(0x800BA04A) + r_u16(0x800BA04E));
        w_u32(packet + 4, (r_u32(packet + 4) & 0xFF000000) | r_u32(0x800BA054) | blend);
        w_u32(packet + 12, (r_u32(packet + 12) & 0xFF000000) | r_u32(0x800BA058));
        w_u32(packet + 20, (r_u32(packet + 20) & 0xFF000000) | r_u32(0x800BA05C));
        w_u32(packet + 28, (r_u32(packet + 28) & 0xFF000000) | r_u32(0x800BA060));
        pointer = packet;
        packet += 36;
        t3_8007ba74(ordering, pointer);
    }
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    ordering += 28;
    flags = r_u16(0x800BA030);
    border = r_u8(0x800BA032);
    blend = flags & 2;
    if (border != 0) {
        packet = t3_bns_08_8010063c(packet, ordering, 0, 0, 0);
        flags = r_u16(0x800BA030);
    }
    result = flags & 1;
    if (result != 0) {
        setPolyG4(psx_addr(packet, 36));
        w_u16(packet + 8, r_u16(0x800BA028));
        w_u16(packet + 10, r_u16(0x800BA02A));
        w_u16(packet + 16, r_u16(0x800BA028) + r_u16(0x800BA02C));
        w_u16(packet + 18, r_u16(0x800BA02A));
        w_u16(packet + 24, r_u16(0x800BA028));
        w_u16(packet + 26, r_u16(0x800BA02A) + r_u16(0x800BA02E));
        w_u16(packet + 32, r_u16(0x800BA028) + r_u16(0x800BA02C));
        w_u16(packet + 34, r_u16(0x800BA02A) + r_u16(0x800BA02E));
        w_u32(packet + 4, (r_u32(packet + 4) & 0xFF000000) | r_u32(0x800BA034) | blend);
        w_u32(packet + 12, (r_u32(packet + 12) & 0xFF000000) | r_u32(0x800BA038));
        w_u32(packet + 20, (r_u32(packet + 20) & 0xFF000000) | r_u32(0x800BA03C));
        w_u32(packet + 28, (r_u32(packet + 28) & 0xFF000000) | r_u32(0x800BA040));
        pointer = packet;
        packet += 36;
        result = t3_8007ba74(ordering, pointer);
    }
    w_u32(0x800ADF64, packet);
    return result;
}

uint32 t3_bns_08_80101fe8(uint32 presentation, uint32 table, uint32 count)
{
    uint32 kind;
    uint32 mode;

    FUNCTION_MARKER(0x80101FE8, "SLUS_004.02-BNS-logical-08");
    kind = r_u8(0x800ADC91);
    if (presentation != 0)
        return t3_bns_08_80100834(presentation);
    w_u8(0x800ADC91, kind);
    mode = r_u8(0x800ADC90);
    if (mode == 1) {
        if (kind == 2) {
            count = 30;
            table = 0x800BA648;
        } else if (kind < 3) {
            count = 12;
            if (kind == 1)
                table = 0x800BABAC;
            else
                t3_8007ae58(1);
        } else {
            count = 26;
            if (kind == 3)
                table = 0x800BA160;
            else
                t3_8007ae58(1);
        }
        w_u8(0x800ADC92, count);
    } else if (mode - 2 < 2) {
        if (kind == 2) {
            count = 28;
            table = 0x800BB19C;
        } else if (kind < 3) {
            count = 14;
            if (kind == 1)
                table = 0x800BB55C;
            else
                t3_8007ae58(1);
        } else {
            count = 29;
            if (kind == 3)
                table = 0x800BADAC;
            else
                t3_8007ae58(1);
        }
        w_u8(0x800ADC93, count);
    }
    mode = r_u8(0x800ADC90);
    if (mode == 1)
        t3_bns_08_80100fdc(count, table);
    else if (mode >= 2 && mode < 4)
        t3_bns_08_8010193c(count, table);
    else
        return mode < 4;
    return t3_bns_08_80100834(presentation);
}
