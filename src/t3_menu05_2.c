#include "game_functions.h"
#include "psx_gpu.h"
#include "t3_menu05_movie_paths.h"
#include "t3_audio.h"
#include <string.h>

typedef struct T3_MENU05_MOVIE
{
    uint32 kind;
    uint32 selection;
    uint32 first_sector;
    uint32 last_sector;
    DISPENV last_display;
    sint32 last_display_valid;
} T3_MENU05_MOVIE;

static T3_MENU05_MOVIE menu05_movie;

static sint32 t3_menu05_movie_tick(void *context)
{
    T3_MENU05_MOVIE *movie = (T3_MENU05_MOVIE *)context;

    t3_8006b6fc();
    if (xport_fmv_ready())
        movie->last_display_valid = gpu_get_display_env(&movie->last_display);
    return 1;
}

static sint32 t3_menu05_movie_frame(void *context, uint32 number)
{
    uint32 previous;

    (void)context;
    w_u16(0x800EC512, number);
    previous = r_u32(0x8015CD98);
    w_u32(0x8015CD9C, previous + 1 != number);
    w_u32(0x8015CD98, number);
    return 1;
}

static sint32 t3_menu05_movie_finish(void *context, uint32 step)
{
    T3_MENU05_MOVIE *movie = (T3_MENU05_MOVIE *)context;

    (void)step;
    movie->last_display_valid = gpu_get_display_env(&movie->last_display);
    return movie->last_display_valid ? 0 : -1;
}

// Configure the game presentation without copying the original playback loop
sint32 t3_menu05_fmv_presentation_init(XportFmvPresentation *presentation, uint32 width, uint32 height, uint32 flags)
{
    uint32 canvas_width;

    if (!presentation || width == 0 || width > 320 || (width & 15) != 0 ||
        height == 0 || height > 240 || (height & 15) != 0)
        return 0;
    canvas_width = width <= 256 ? 256 : 320;
    memset(presentation, 0, sizeof(*presentation));
    presentation->width = canvas_width;
    presentation->height = 240;
    presentation->video_x = (sint32)(canvas_width - width) / 2;
    presentation->video_y = (sint32)(240 - height) / 2;
    presentation->bottom_rows = ((flags >> 8) & 15) == 3 ? 32 : 0;
    presentation->screen_x = r_s8(0x80097F24);
    presentation->screen_y = r_s8(0x80097F25);
    presentation->screen_width = 256;
    presentation->screen_height = 240;
    presentation->interlace = 1;
    presentation->controllers = 3;
    presentation->skip_buttons = 0x800;
    presentation->skip_held = 1;
    return 1;
}

void t3_bns_05_800e0dfc(uint32 unused, uint32 white)
{
    uint32 first;
    uint32 second;
    uint32 pixels;
    uint32 row;
    uint32 column;
    uint32 color;
    sint32 top;
    PSX_RECT rectangle;
    DISPENV display;

    FUNCTION_MARKER(0x800E0DFC, "SLUS_004.02-BNS-logical-05");
    (void)unused;
    first = r_u32(0x800BA080);
    second = r_u32(0x800BA084);
    rectangle.x = (sint16)first;
    rectangle.y = (sint16)(first >> 16);
    rectangle.w = (sint16)second;
    rectangle.h = (sint16)(second >> 16);
    pixels = 0x800EC598;
    for (row = 0; row < 480; ++row)
    {
        color = white ? 0xFFFFFFFF : 0;
        for (column = 0; column < 240; ++column)
        {
            w_u32(pixels, color);
            pixels += 4;
        }
    }
    t3_80029dc0(0);
    VSync(0);
    LoadImagePSX(&rectangle, psx_addr(0x800EC598, 460800));
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
    pixels = 0x800EC598;
    for (row = 0; row < 480; ++row)
    {
        color = 0;
        if (white != 0)
        {
            top = r_s16(0x800AE232);
            if ((sint32)row >= top && (sint32)row <= top + r_s16(0x800AE236) - 1)
                color = 0xFFFFFFFF;
        }
        for (column = 0; column < 240; ++column)
        {
            w_u32(pixels, color);
            pixels += 4;
        }
    }
    t3_80029dc0(0);
    VSync(0);
    LoadImagePSX(&rectangle, psx_addr(0x800EC598, 460800));
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

// Select an EOF-delimited gallery asset without a second player loop
sint32 t3_menu05_play_t3_movie(uint32 selection, sint32 repeat, const XportFmvPresentation *presentation)
{
    static const char *const paths[] = {
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_021_F01942.STR",
        "DATA/TEKKEN3/MOVIE/XAS_022.STR",
        "DATA/TEKKEN3/MOVIE/XAS_022.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_001_F00640.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_002_F00451.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_003_F00816.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_004_F00686.STR",
        "DATA/TEKKEN3/MOVIE/XAS_005.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_006_F00741.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_007_F00724.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_008_F00673.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_009_F00800.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_010_F00817.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_011_F00824.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_012_F00388.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_013_F00682.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_014_F00640.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_015_F00716.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_016_F00540.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_017_F00818.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_019_F01002.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_018_F00553.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_020_F00694.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_012_F00450.STR",
        "DATA/TEKKEN3/MOVIE/CLIPS/XAS_017_F00928.STR"
    };

    sint32 result;
    uint32 first;
    uint32 end;
    uint32 vsync_callback;

    if (selection >= sizeof(paths) / sizeof(paths[0]))
        return -1;
    t3_file_audio_stop();
    vsync_callback = VSyncCallbackPSX(0);
    if (!repeat && !presentation)
        result = xport_fmv_play(paths[selection]);
    else
        result = xport_fmv_play_presented(paths[selection], 0, repeat, presentation);
    VSyncCallbackPSX(vsync_callback);
    if (result == 1 && !repeat && selection < 3)
    {
        first = selection == 0 ? 231752 : 246488;
        end = selection == 0 ? 246488 : 249552;
        if (!t3_file_audio_resume_fmv("DATA/TEKKEN3/TEKKEN3.XAS", first, end, 1))
        {
            xport_fmv_discard_audio();
            return -1;
        }
    }
    else
        xport_fmv_discard_audio();
    return result;
}

// Delegate every supported series to the shared fullscreen player
sint32 t3_menu05_play_movie(uint32 kind, uint32 selection, sint32 repeat, const XportFmvPresentation *presentation)
{
    const char *path;
    uint32 count;
    uint32 first;
    sint32 result;
    uint32 vsync_callback;

    if (kind == 3)
        return t3_menu05_play_t3_movie(selection, repeat, presentation);
    if (kind != 1 && kind != 2)
        return -1;
    count = kind == 1 ? 12 : 28;
    if (selection >= count)
        return -1;
    path = t3_menu05_optional_movie_paths[kind - 1][selection];
    t3_file_audio_stop();
    vsync_callback = VSyncCallbackPSX(0);
    if (!repeat && !presentation)
        result = xport_fmv_play(path);
    else
        result = xport_fmv_play_presented(path, 0, repeat, presentation);
    VSyncCallbackPSX(vsync_callback);
    if (result == 1 && !repeat && kind == 2 && selection < 2)
    {
        first = selection == 0 ? 152048 : 155048;
        if (!t3_file_audio_resume_fmv("DATA/TEKKEN2/TEKKEN2.XAS", first, first + 3000, -1))
        {
            xport_fmv_discard_audio();
            return -1;
        }
    }
    else
        xport_fmv_discard_audio();
    return result;
}

uint32 t3_bns_05_800def60(uint32 state, uint32 selected)
{
    uint32 identity;
    uint32 x;
    uint32 y;

    FUNCTION_MARKER(0x800DEF60, "SLUS_004.02-BNS-logical-05");
    if (selected != 0)
    {
        identity = r_u32(state);
        if ((r_u16(0x800ADF08 + (identity << 1)) & 0x8F0) != 0)
            return 1;
    }
    x = r_u32(state + 20) + 20;
    y = r_u32(state + 24) + 418;
    t3_8004ce74(0x800B95E8, selected != 0 ? 5 : 10, 0, x, y);
    if (selected != 0)
        t3_bns_05_800dc21c(x, y, 36, 1);
    return 0;
}

uint32 t3_bns_05_800df708(uint32 record)
{
    uint32 mask;
    uint32 index;
    uint32 count;
    uint32 filled;
    uint32 pairs[44];
    uint32 result;

    FUNCTION_MARKER(0x800DF708, "SLUS_004.02-BNS-logical-05");
    mask = 0x3FF;
    for (index = 0; index < 22; ++index)
    {
        if (r_u32(0x80097F4C + index * 8) <= 359998)
            mask |= (uint32)1 << index;
    }
    mask &= 0xFFDFFFFF;
    count = t3_8004ccac(mask);
    w_u32(record + 4, count);
    filled = 0;
    for (index = 0; index < 22; ++index)
    {
        if ((mask & ((uint32)1 << index)) != 0)
        {
            pairs[filled * 2] = index;
            pairs[filled * 2 + 1] = r_u32(0x80097F4C + index * 8);
            ++filled;
        }
    }
    result = t3_8004cd80(pairs, count, 1);
    for (index = 0; index < count; ++index)
    {
        w_u8(record + index + 8, pairs[index * 2] & 255);
        result = index + 1 < count;
    }
    return result;
}

uint32 t3_bns_05_800df558(uint32 wins, uint32 x, uint32 y)
{
    uint32 label;
    uint32 digits;
    uint32 tail;
    uint32 format;

    FUNCTION_MARKER(0x800DF558, "SLUS_004.02-BNS-logical-05");
    label = (sint32)wins < 2 ? 0x800B96A0 : 0x800B96A4;
    if ((sint32)wins < 10)
        digits = 1;
    else if ((sint32)wins < 100)
        digits = 2;
    else
        digits = 3;
    if ((sint32)wins > 99999999)
        wins = 99999999;
    tail = t3_8004cc08(0x1F800000, 0x800B9634);
    t3_8004cc08(tail, 0x800B9640);
    format = r_u32(0x800EAFC8 + (digits << 2));
    t3_8004cc08(tail + 3 - digits, format);
    t3_8004ce74(0x1F800000, 6, 0, x, y, wins);
    return t3_8004ce74(0x800B96AC, 6, 0, x + 36, y, label);
}

uint32 t3_bns_05_800e0358(uint32 record)
{
    sint32 count;
    uint32 rank;
    uint32 result;
    uint32 row;
    uint32 y;
    uint32 character;
    uint32 entry;
    uint32 wins;
    uint32 losses;
    uint32 rate;
    uint32 digits;
    sint32 suffix;
    uint32 position;
    uint32 text;
    uint32 tail;
    uint32 name_table;
    uint32 width;

    FUNCTION_MARKER(0x800E0358, "SLUS_004.02-BNS-logical-05");
    count = r_s32(record + 4);
    rank = r_u32(record);
    if (count >= 11)
        count = 10;
    result = t3_8004ce74(0x800B9784, 6, 0, 117, 126);
    y = 146;
    for (row = 0; (sint32)row < count; ++row)
    {
        suffix = (sint32)rank % 10;
        character = r_u8(record + rank + 8);
        entry = 0x8009804C + (character << 3);
        if (suffix >= 4 || rank - 10 < 10)
            suffix = 3;
        position = rank + 1;
        if ((sint32)position >= 101)
            position = 99;
        text = r_u32(0x800EAFEC + ((uint32)suffix << 2));
        t3_8004ce74(0x800B9678, 6, 0, 18, y, position, text);
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        name_table = t3_8004efc8((character % 22) << 2);
        text = r_u32(name_table);
        t3_8004cc08(tail, text);
        width = t3_8007a418(tail) * 9;
        t3_8004ce74(0x1F800000, 6, 0, 126 - (uint32)((sint32)width >> 1), y);
        wins = r_u16(entry + 2);
        losses = r_u16(entry + 4);
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        rate = t3_8004cd20(wins, wins + losses);
        if (rate == 0)
        {
            t3_8004cc08(tail, 0x800B964C);
            t3_8004ce74(0x1F800000, 6, 0, 171, y);
        }
        else if (rate == 1000)
        {
            t3_8004cc08(tail, 0x800B9654);
            t3_8004ce74(0x1F800000, 1, 0, 171, y);
        }
        else
        {
            t3_8004cc08(tail, 0x800B965C);
            t3_8004ce74(0x1F800000, 6, 0, 171, y, rate / 10, rate % 10);
        }
        wins = r_u16(entry + 2);
        if (wins < 10)
            digits = 1;
        else if (wins < 100)
            digits = 2;
        else if (wins < 1000)
            digits = 3;
        else if (wins < 10000)
            digits = 4;
        else
            digits = 5;
        tail = t3_8004cc08(0x1F800000, 0x800B9634);
        t3_8004cc08(tail, 0x800B9640);
        text = r_u32(0x800EAFC8 + (digits << 2));
        t3_8004cc08(tail + 5 - digits, text);
        t3_8004ce74(0x1F800000, 6, 0, 234, y, wins);
        losses = r_u16(entry + 4);
        if (losses < 10)
            digits = 1;
        else if (losses < 100)
            digits = 2;
        else if (losses < 1000)
            digits = 3;
        else if (losses < 10000)
            digits = 4;
        else
            digits = 5;
        tail = t3_8004cc08(0x1F800000, 0x800B9634);
        t3_8004cc08(tail, 0x800B9640);
        text = r_u32(0x800EAFC8 + (digits << 2));
        ++rank;
        t3_8004cc08(tail + 5 - digits, text);
        t3_8004ce74(0x1F800000, 6, 0, 288, y, losses);
        y += 20;
        result = 0;
    }
    return result;
}

uint32 t3_bns_05_800dfe3c(uint32 record)
{
    sint32 count;
    uint32 rank;
    uint32 result;
    uint32 row;
    uint32 y;
    uint32 character;
    uint32 total;
    uint32 all_total;
    sint32 suffix;
    uint32 position;
    uint32 text;
    uint32 tail;
    uint32 name_table;
    uint32 width;
    uint32 rate;
    uint32 digits;

    FUNCTION_MARKER(0x800DFE3C, "SLUS_004.02-BNS-logical-05");
    count = r_s32(record + 4);
    rank = r_u32(record);
    if (count >= 11)
        count = 10;
    result = t3_8004ce74(0x800B9760, 6, 0, 117, 126);
    y = 146;
    for (row = 0; (sint32)row < count; ++row)
    {
        character = r_u8(record + rank + 8);
        total = t3_80051660(character);
        suffix = (sint32)rank % 10;
        if (suffix >= 4 || rank - 10 < 10)
            suffix = 3;
        position = rank + 1;
        if ((sint32)position >= 101)
            position = 99;
        text = r_u32(0x800EAFEC + ((uint32)suffix << 2));
        t3_8004ce74(0x800B9678, 6, 0, 18, y, position, text);
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        name_table = t3_8004efc8((character % 22) << 2);
        text = r_u32(name_table);
        t3_8004cc08(tail, text);
        width = t3_8007a418(tail) * 9;
        t3_8004ce74(0x1F800000, 6, 0, 126 - (uint32)((sint32)width >> 1), y);
        all_total = r_u32(0x800EC450);
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        rate = t3_8004cd20(total, all_total);
        if (rate == 0)
        {
            t3_8004cc08(tail, 0x800B964C);
            t3_8004ce74(0x1F800000, 6, 0, 225, y);
        }
        else if (rate == 1000)
        {
            t3_8004cc08(tail, 0x800B9654);
            t3_8004ce74(0x1F800000, 1, 0, 225, y);
        }
        else
        {
            t3_8004cc08(tail, 0x800B965C);
            t3_8004ce74(0x1F800000, 6, 0, 225, y, rate / 10, rate % 10);
        }
        if ((sint32)total < 10)
            digits = 1;
        else if ((sint32)total < 100)
            digits = 2;
        else if ((sint32)total < 1000)
            digits = 3;
        else if ((sint32)total < 10000)
            digits = 4;
        else
            digits = 5;
        if ((sint32)total > 99999999)
            total = 99999999;
        tail = t3_8004cc08(0x1F800000, 0x800B9634);
        t3_8004cc08(tail, 0x800B9640);
        text = r_u32(0x800EAFC8 + (digits << 2));
        ++rank;
        t3_8004cc08(tail + 5 - digits, text);
        t3_8004ce74(0x1F800000, 6, 0, 288, y, total);
        y += 20;
        result = 0;
    }
    return result;
}

uint32 t3_bns_05_800dfb38(uint32 record)
{
    sint32 count;
    uint32 rank;
    uint32 result;
    uint32 row;
    uint32 y;
    uint32 entry;
    uint32 wins;
    uint32 character;
    sint32 suffix;
    uint32 position;
    uint32 suffix_text;
    uint32 tail;
    uint32 name_table;
    uint32 name;
    uint32 width;

    FUNCTION_MARKER(0x800DFB38, "SLUS_004.02-BNS-logical-05");
    count = r_s32(record + 4);
    rank = r_u32(record);
    if (count >= 11)
        count = 10;
    result = t3_8004ce74(0x800B973C, 6, 0, 117, 126);
    y = 146;
    for (row = 0; (sint32)row < count; ++row)
    {
        suffix = (sint32)rank % 10;
        entry = 0x80097FFC + ((uint32)r_u8(record + rank + 8) << 3);
        if (suffix >= 4 || rank - 10 < 10)
            suffix = 3;
        position = rank + 1;
        if ((sint32)position >= 101)
            position = 99;
        suffix_text = r_u32(0x800EAFEC + ((uint32)suffix << 2));
        t3_8004ce74(0x800B9678, 6, 0, 18, y, position, suffix_text);
        character = r_u16(entry);
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        ++rank;
        name_table = t3_8004efc8((character % 22) << 2);
        name = r_u32(name_table);
        t3_8004cc08(tail, name);
        width = t3_8007a418(tail) * 9;
        t3_8004ce74(0x1F800000, 6, 0, 126 - (uint32)((sint32)width >> 1), y);
        wins = r_u16(entry + 2);
        t3_bns_05_800df558(wins, 198, y);
        t3_8004ce74(0x800B96AC, 6, 0, 306, y, entry + 4);
        y += 20;
        result = 0;
    }
    return result;
}

uint32 t3_bns_05_800df824(uint32 record)
{
    sint32 count;
    uint32 rank;
    uint32 result;
    uint32 row;
    uint32 y;
    uint32 character;
    sint32 suffix;
    uint32 position;
    uint32 suffix_text;
    uint32 time_record;
    uint32 tail;
    uint32 name_table;
    uint32 name;
    uint32 width;
    sint32 frames;
    sint32 seconds;
    sint32 minutes;
    sint32 fraction;

    FUNCTION_MARKER(0x800DF824, "SLUS_004.02-BNS-logical-05");
    count = r_s32(record + 4);
    rank = r_u32(record);
    if (count >= 11)
        count = 10;
    result = t3_8004ce74(0x800B9718, 6, 0, 117, 126);
    y = 146;
    for (row = 0; (sint32)row < count; ++row)
    {
        suffix = (sint32)rank % 10;
        character = r_u8(record + rank + 8);
        time_record = 0x80097F4C + (character << 3);
        if (suffix >= 4 || rank - 10 < 10)
            suffix = 3;
        position = rank + 1;
        if ((sint32)position >= 101)
            position = 99;
        suffix_text = r_u32(0x800EAFEC + ((uint32)suffix << 2));
        t3_8004ce74(0x800B9678, 6, 0, 18, y, position, suffix_text);
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        name_table = t3_8004efc8((character % 22) << 2);
        name = r_u32(name_table);
        t3_8004cc08(tail, name);
        width = t3_8007a418(tail) * 9;
        t3_8004ce74(0x1F800000, 6, 0, 126 - (uint32)((sint32)width >> 1), y);
        frames = r_s32(time_record);
        if (frames > 359999)
            frames = 359999;
        seconds = frames / 60;
        minutes = seconds / 60;
        fraction = ((frames % 60) * 100) / 60;
        ++rank;
        t3_8004ce74(0x800B9688, 6, 0, 198, y, minutes, seconds % 60, fraction);
        t3_8004ce74(0x800B96AC, 6, 0, 306, y, time_record + 4);
        y += 20;
        result = 0;
    }
    return result;
}

uint32 t3_bns_05_800e0b1c(void)
{
    uint32 buttons;
    uint32 other_buttons;
    uint32 page;
    uint32 delta;
    uint32 record;
    uint32 count;
    uint32 scroll;
    uint32 candidate;
    uint32 tail;
    uint32 label;
    uint32 width;

    FUNCTION_MARKER(0x800E0B1C, "SLUS_004.02-BNS-logical-05");
    buttons = r_u16(0x800ADF08);
    other_buttons = r_u16(0x800ADF0A);
    if (((buttons | other_buttons) & 0x1F0) != 0)
    {
        t3_800753bc(0, 0x4CEB, 0);
        return 1;
    }
    buttons = r_u16(0x800ADF00);
    other_buttons = r_u16(0x800ADF02);
    page = r_u32(0x800EC448);
    buttons |= other_buttons;
    delta = ((buttons >> 13) & 1) - (buttons >> 15);
    page += delta;
    w_u32(0x800EC448, page);
    page = t3_8004cc80(0, page, 3);
    w_u32(0x800EC448, page);
    if (delta != 0)
        t3_800753bc(0, 0x546C, 0);
    page = r_u32(0x800EC448);
    record = 0x800EC458 + (page << 5);
    count = r_u32(record + 4);
    scroll = r_u32(record);
    if ((sint32)count < 11)
        scroll = 0;
    else
    {
        buttons = r_u16(0x800ADF00);
        other_buttons = r_u16(0x800ADF02);
        buttons |= other_buttons;
        delta = ((buttons >> 14) & 1) - ((buttons >> 12) & 1);
        candidate = scroll + delta;
        if (candidate < count - 9)
            scroll = candidate;
        if (scroll != r_u32(record))
            t3_800753bc(0, 0x546C, 0);
    }
    page = r_u32(0x800EC448);
    w_u32(record, scroll);
    switch (page)
    {
    case 0:
        t3_bns_05_800df824(record);
        break;
    case 1:
        t3_bns_05_800dfb38(record);
        break;
    case 2:
        t3_bns_05_800dfe3c(record);
        break;
    case 3:
        t3_bns_05_800e0358(record);
        break;
    }
    t3_8004ce74(0x800B97A8, 6, 1, 139, 40);
    page = r_u32(0x800EC448);
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    label = r_u32(0x800EAFFC + (page << 2));
    t3_8004cc08(tail, label);
    width = t3_8007a418(tail) * 9;
    t3_8004ce74(0x1F800000, 6, 0, 184 - (uint32)((sint32)width >> 1), 90);
    page = r_u32(0x800EC448);
    t3_8004ce74(0x800B97B8, 6, 0, 14, 2, page + 1);
    t3_8004ce74(0x800B97DC, 0, 63, 380, 7, 6);
    t3_8004ce74(0x800B97FC, 0, 63, 420, 2, 6);
    t3_bns_05_800dc098(161, 418);
    return 0;
}

uint32 t3_bns_05_800dee4c(uint32 state, uint32 selected)
{
    uint32 identity;
    uint32 buttons;
    uint32 destination;
    uint32 index;
    uint32 value;
    uint32 color;
    uint32 x;
    uint32 y;

    FUNCTION_MARKER(0x800DEE4C, "SLUS_004.02-BNS-logical-05");
    if (selected != 0)
    {
        identity = r_u32(state);
        buttons = r_u16(0x800ADF08 + (identity << 1));
        if ((buttons & 0x8F0) != 0)
        {
            w_u8(0x80097F0C + identity, 0);
            identity = r_u32(state);
            destination = 0x80097F28 + (identity << 3);
            for (index = 0; index < 8; ++index)
            {
                value = r_u8(0x800B94FC + index);
                w_u8(destination + index, value);
            }
            w_u32(state + 12, 1);
            t3_800753bc(0, 0x4D87, 0);
        }
    }
    color = selected != 0 ? 5 : 10;
    x = r_u32(state + 20) + 20;
    y = r_u32(state + 24) + 392;
    t3_8004ce74(0x800B95D8, color, 0, x, y);
    if (selected != 0)
        t3_bns_05_800dc21c(x, y, 63, 1);
    return 0;
}

uint32 t3_8002a0fc(uint32 controller)
{
    FUNCTION_MARKER(0x8002A0FC, "SLUS_004.02");
    return r_u8(0x800A9158 + controller * 42);
}

uint32 t3_bns_05_800de21c(uint32 x, uint32 y, uint32 code, uint32 depth, uint32 highlighted, uint32 used_mask)
{
    uint32 color;
    uint32 texture;
    uint32 packet;
    uint32 ordering_entry;
    uint32 text;
    uint32 width;
    uint32 result;

    FUNCTION_MARKER(0x800DE21C, "SLUS_004.02-BNS-logical-05");
    if ((sint32)code >= 13)
        return 0;
    color = 5;
    if (highlighted == 0)
        color = (used_mask & (1u << (code & 31))) != 0 ? 10 : 6;
    if (code == 11 || code == 10)
    {
        texture = ((color | 0x7E10) << 16) | (code == 11 ? 0x8A : 0x80);
        packet = r_u32(0x800ADF64);
        ordering_entry = r_u32(0x800A9218);
        packet = t3_8004d9bc(ordering_entry + (depth << 2), packet, 0x65000000,
            ((x + 8) & 0xFFFF) | ((y + 3) << 16), 0x0010000A, texture);
        ordering_entry = r_u32(0x800A9218);
        w_u32(0x800ADF64, packet);
        packet = t3_8004daa8(ordering_entry + (depth << 2), packet, 11);
        w_u32(0x800ADF64, packet);
    }
    text = r_u32(0x800EAED0 + (code << 2));
    width = t3_8007a418(text) * 9;
    t3_8004ce74(0x800B9554, depth, color, 0,
        x - ((uint32)((sint32)width >> 1) - 32), y + 3, text);
    result = 0x800B0000;
    if (highlighted != 0)
    {
        color = t3_8004dfe0(r_u32(0x800AE218) << 4);
        color = t3_8004df6c(0xFF4020, (color >> 1) + 128);
        packet = r_u32(0x800ADF64);
        ordering_entry = r_u32(0x800A9218);
        result = t3_8004da10(ordering_entry + (depth << 2), packet, color | 0x60000000,
            ((x + 1) & 0xFFFF) | ((y + 2) << 16), 0x0012003E);
        w_u32(0x800ADF64, result);
    }
    return result;
}

static uint32 t3_menu05_lerp_eighth(uint32 current, uint32 target, uint32 factor)
{
    uint32 difference;
    uint32 step;

    if ((sint32)current < (sint32)target)
    {
        difference = target - current;
        step = (uint32)((sint32)(difference * factor) >> 3);
        return current + step;
    }
    difference = current - target;
    step = (uint32)((sint32)(difference * factor) >> 3);
    return current - step;
}

uint32 t3_bns_05_800de42c(uint32 state, uint32 row, uint32 incoming_expanded, uint32 incoming_depth, uint32 *next_expanded)
{
    uint32 kind;
    uint32 expanded;
    uint32 depth;
    uint32 identity;
    uint32 row_index;
    uint32 used_mask;
    uint32 mapped;
    uint32 index;
    uint32 code;
    uint32 highlighted;
    uint32 item_depth;
    uint32 current;
    uint32 target;
    uint32 factor;
    uint32 local_x;
    uint32 local_y;
    uint32 base_x;
    uint32 base_y;
    uint32 x;
    uint32 y;
    uint32 height;
    uint32 color;
    uint32 packet;
    uint32 ordering_entry;
    uint32 device_type;
    uint32 endpoints;
    uint32 target_x;
    uint32 target_y;
    uint32 endpoint;

    FUNCTION_MARKER(0x800DE42C, "SLUS_004.02-BNS-logical-05");
    kind = r_u32(row);
    expanded = incoming_expanded;
    depth = incoming_depth;
    if (kind == 0)
    {
        depth = r_u32(row + 16);
        expanded = 0;
    }
    else if (kind == 1 || kind == 2)
    {
        depth = 4;
        expanded = 1;
    }
    else if (kind == 3)
    {
        depth = 3;
        expanded = 0;
    }
    identity = r_u32(state);
    row_index = r_u32(row + 4);
    used_mask = r_u32(state + 36);
    mapped = r_u8(0x80097F28 + (identity << 3) + row_index);
    for (index = 0; index < (expanded != 0 ? 13u : 1u); ++index)
    {
        code = expanded != 0 ? index : mapped;
        highlighted = expanded != 0 && code == mapped;
        item_depth = depth + highlighted;
        current = r_u32(row + 20);
        target = r_u32(row + 28);
        factor = r_u32(row + 12);
        local_x = t3_menu05_lerp_eighth(current, target, factor);
        if (expanded != 0)
        {
            current = r_u32(row + 24);
            base_x = r_u32(state + 20);
        }
        else
        {
            base_x = r_u32(state + 20);
            current = r_u32(row + 24);
        }
        target = r_u32(row + 32);
        factor = r_u32(row + 12);
        x = base_x + local_x;
        local_y = t3_menu05_lerp_eighth(current, target + code * 18, factor);
        base_y = r_u32(state + 24);
        t3_bns_05_800de21c(x, base_y + local_y, code, item_depth,
            highlighted, expanded != 0 ? used_mask : 0);
    }
    current = r_u32(row + 20);
    target = r_u32(row + 28);
    factor = r_u32(row + 12);
    local_x = t3_menu05_lerp_eighth(current, target, factor);
    current = r_u32(row + 24);
    base_x = r_u32(state + 20);
    target = r_u32(row + 32);
    factor = r_u32(row + 12);
    x = base_x + local_x;
    local_y = t3_menu05_lerp_eighth(current, target, factor);
    factor = r_u32(row + 12);
    packet = r_u32(0x800ADF64);
    ordering_entry = r_u32(0x800A9218);
    base_y = r_u32(state + 24);
    y = base_y + local_y;
    height = (uint32)((sint32)(factor * 216) >> 3);
    color = r_u32(row + 52);
    packet = t3_8004da10(ordering_entry + (depth << 2), packet, 0x60000000,
        ((x + 1) & 0xFFFF) | ((y + 2) << 16), ((height + 18) << 16) | 62);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering_entry + (depth << 2), packet, color | 0x60000000,
        (x & 0xFFFF) | (y << 16), ((height + 22) << 16) | 64);
    current = r_u32(row + 36);
    target = r_u32(row + 44);
    factor = r_u32(row + 12);
    w_u32(0x800ADF64, packet);
    local_x = t3_menu05_lerp_eighth(current, target, factor);
    current = r_u32(row + 40);
    target = r_u32(row + 48);
    factor = r_u32(row + 12);
    x += local_x;
    local_y = t3_menu05_lerp_eighth(current, target, factor);
    y += local_y;
    device_type = r_u32(state + 16);
    packet = r_u32(0x800ADF64);
    row_index = r_u32(row + 4);
    ordering_entry = r_u32(0x800A9218);
    base_y = r_u32(state + 24);
    endpoints = 0x800B9564 + (row_index << 2) + (device_type != 9 ? 32 : 0);
    target_x = r_u16(endpoints);
    base_x = r_u32(state + 20);
    target_x += base_x;
    target_y = r_u16(endpoints + 2);
    endpoint = (target_x & 0xFFFF) | ((target_y + base_y) << 16);
    packet = t3_8004da5c(ordering_entry + (depth << 2), packet, color | 0x40000000,
        ((x + 2) & 0xFFFF) | (y << 16), endpoint);
    ordering_entry = r_u32(0x800A9218);
    target_x = r_u16(endpoints);
    base_x = r_u32(state + 20);
    base_y = r_u32(state + 24);
    w_u32(0x800ADF64, packet);
    target_x += base_x;
    target_y = r_u16(endpoints + 2);
    endpoint = (target_x & 0xFFFF) | ((target_y + base_y) << 16);
    packet = t3_8004d71c(ordering_entry + (depth << 2), packet, color | 0x20000000,
        (x & 0xFFFF) | (y << 16), ((x + 5) & 0xFFFF) | (y << 16), endpoint);
    w_u32(0x800ADF64, packet);
    *next_expanded = color | 0x20000000;
    return packet;
}

uint32 t3_bns_05_800de97c(uint32 state, uint32 row)
{
    uint32 held;
    uint32 buttons;
    uint32 old_kind;
    uint32 kind;
    uint32 counter;
    uint32 direction;
    uint32 identity;
    uint32 index;
    uint32 value;
    uint32 mask;
    uint32 bit;

    FUNCTION_MARKER(0x800DE97C, "SLUS_004.02-BNS-logical-05");
    held = r_u32(state + 28);
    buttons = r_u32(row + 8);
    old_kind = r_u32(row);
    if ((held & buttons) != 0)
    {
        counter = r_u32(row + 12) + 1;
        w_u32(row + 12, counter);
        if ((sint32)counter < 9)
        {
            w_u32(row, 1);
        }
        else
        {
            w_u32(row + 12, 8);
            w_u32(row, 2);
        }
    }
    else
    {
        counter = r_u32(row + 12) - 1;
        w_u32(row + 12, counter);
        if ((sint32)counter > 0)
        {
            w_u32(row, 3);
        }
        else
        {
            w_u32(row + 12, 0);
            w_u32(row, 0);
        }
    }
    if (r_u32(row + 12) != 0)
        w_u32(state + 12, 1);
    kind = r_u32(row);
    if (old_kind == 0 && kind == 1)
    {
        t3_800753bc(0, 0x45CA, 0);
        kind = r_u32(row);
    }
    if (kind == 2)
    {
        buttons = r_u32(state + 32);
        direction = ((buttons >> 14) & 1) - ((buttons >> 12) & 1);
        if (direction != 0)
            t3_800753bc(0, 0x546C, 0);
        identity = r_u32(state);
        index = r_u32(row + 4);
        value = r_u8(0x80097F28 + (identity << 3) + index);
        value = t3_8004cc80(0, direction + value, 12);
        identity = r_u32(state);
        index = r_u32(row + 4);
        w_u8(0x80097F28 + (identity << 3) + index, value);
    }
    identity = r_u32(state);
    index = r_u32(row + 4);
    value = r_u8(0x80097F28 + (identity << 3) + index);
    mask = r_u32(state + 36);
    bit = 1u << (value & 31);
    w_u32(state + 36, mask | bit);
    return bit;
}

uint32 t3_bns_05_800ded08(uint32 state, uint32 selected)
{
    uint32 identity;
    uint32 destination;
    uint32 enabled;
    uint32 color;
    uint32 value_color;
    uint32 x;
    uint32 y;
    uint32 text;

    FUNCTION_MARKER(0x800DED08, "SLUS_004.02-BNS-logical-05");
    identity = r_u32(state);
    destination = 0x80097F0C + identity;
    enabled = r_u8(destination);
    if (selected != 0 && (r_u16(0x800ADF08 + (identity << 1)) & 0xA000) != 0)
    {
        enabled = (enabled + 1) & 1;
        w_u8(destination, enabled);
        identity = r_u32(state);
        w_u32(state + 12, 1);
        w_u16(0x800AE1F8 + (identity << 1), 1);
        identity = r_u32(state);
        t3_8002a16c(identity, enabled);
        identity = r_u32(state);
        t3_80028ec0(identity, 13);
        t3_800753bc(0, 0x546C, 0);
    }
    color = selected != 0 ? 5 : 10;
    value_color = enabled != 0 ? 6 : 1;
    x = r_u32(state + 20) + 20;
    y = r_u32(state + 24) + 366;
    text = r_u32(0x800EAF04 + ((uint32)(enabled != 0) << 2));
    t3_8004ce74(0x800B95BC, color, 0, x, y, value_color, text);
    if (selected != 0)
        t3_bns_05_800dc21c(x, y, 144, 1);
    return 0;
}

uint32 t3_bns_05_800df014(void)
{
    uint32 controller;
    uint32 state;
    uint32 control;
    uint32 row;
    uint32 selected;
    uint32 identity;
    uint32 buttons;
    uint32 destination;
    uint32 index;
    uint32 value;
    uint32 x;
    uint32 y;
    uint32 color;
    uint32 direction;
    uint32 requested_exit;
    uint32 first_busy;
    uint32 second_busy;
    uint32 expanded;
    uint32 font_disabled;
    uint32 packet;

    FUNCTION_MARKER(0x800DF014, "SLUS_004.02-BNS-logical-05");
    // The caller inherits the final ClearOTagR address mask in A2
    expanded = 0x00FFFFFF;
    requested_exit = 0;
    for (controller = 0; controller < 2; ++controller)
    {
        state = 0x800EC078 + controller * 488;
        control = state + 4;
        if (r_u32(control + 4) == 0)
        {
            identity = r_u32(state);
            if (r_u16(0x800ADF08 + (identity << 1)) != 0)
                w_u32(control + 4, 1);
        }
        w_u32(control + 8, 0);
        for (row = 0; row < 4; ++row)
        {
            selected = row == r_u32(control);
            switch (row)
            {
            case 0:
                t3_bns_05_800deb30(state, selected, expanded, requested_exit);
                break;
            case 1:
                t3_bns_05_800ded08(state, selected);
                break;
            case 2:
                if (selected != 0)
                {
                    identity = r_u32(state);
                    buttons = r_u16(0x800ADF08 + (identity << 1));
                    if ((buttons & 0x8F0) != 0)
                    {
                        w_u8(0x80097F0C + identity, 0);
                        identity = r_u32(state);
                        destination = 0x80097F28 + (identity << 3);
                        for (index = 0; index < 8; ++index)
                        {
                            value = r_u8(0x800B94FC + index);
                            w_u8(destination + index, value);
                        }
                        w_u32(control + 8, 1);
                        t3_800753bc(0, 0x4D87, 0);
                    }
                }
                color = selected != 0 ? 5 : 10;
                x = r_u32(control + 16) + 20;
                y = r_u32(control + 20) + 392;
                font_disabled = r_u32(0x800ADCA4);
                t3_8004ce74(0x800B95D8, color, 0, x, y);
                expanded = font_disabled != 0 ? 0 : r_u16(0x1F800390);
                if (selected != 0)
                {
                    packet = t3_bns_05_800dc21c(x, y, 63, 1);
                    expanded = r_u32(packet - 12);
                }
                break;
            case 3:
                if (selected != 0)
                {
                    identity = r_u32(state);
                    buttons = r_u16(0x800ADF08 + (identity << 1));
                    if ((buttons & 0x8F0) != 0)
                    {
                        requested_exit |= 1;
                        break;
                    }
                }
                color = selected != 0 ? 5 : 10;
                x = r_u32(control + 16) + 20;
                y = r_u32(control + 20) + 418;
                font_disabled = r_u32(0x800ADCA4);
                t3_8004ce74(0x800B95E8, color, 0, x, y);
                expanded = font_disabled != 0 ? 0 : r_u16(0x1F800390);
                if (selected != 0)
                {
                    packet = t3_bns_05_800dc21c(x, y, 36, 1);
                    expanded = r_u32(packet - 12);
                }
                break;
            }
        }
        if (r_u32(control + 8) == 0)
        {
            buttons = r_u16(0x800ADF00 + (controller << 1));
            direction = ((buttons >> 14) & 1) - ((buttons >> 12) & 1);
            if (direction != 0)
                t3_800753bc(0, 0x546C, 0);
            value = r_u32(control);
            value = t3_8004cc80(0, direction + value, 3);
            w_u32(control, value);
            expanded = 3;
        }
    }
    color = r_u8(0x80097F0B) != 0 ? 1 : 6;
    t3_8004ce74(0x800B95F8, color, 1, 74, 40);
    if ((r_u16(0x800ADF08) & 0x100) != 0)
        requested_exit = 1;
    if ((r_u16(0x800ADF0A) & 0x100) != 0)
        requested_exit = 1;
    if (requested_exit == 0)
        return 0;
    first_busy = r_u32(0x800EC084);
    second_busy = r_u32(0x800EC26C);
    if ((first_busy | second_busy) != 0)
        return 0;
    t3_bns_05_800df3d0();
    t3_800753bc(0, 0x4CEB, 0);
    return 1;
}

uint32 t3_bns_05_800dda30(uint32 state)
{
    uint32 buttons;
    uint32 other_buttons;
    uint32 pressed;
    uint32 other_pressed;
    sint32 old_x;
    sint32 old_y;
    sint32 x;
    sint32 y;
    sint32 scaled_x;
    uint32 color;
    uint32 twice_y;
    uint32 ordering_entry;
    uint32 packet;
    sint32 left;
    sint32 top;
    uint32 width;
    uint32 height;
    uint32 center_x;
    uint32 center_y;
    uint32 line_x;
    uint32 line_y;
    uint32 right_x;
    uint32 bottom_y;

    FUNCTION_MARKER(0x800DDA30, "SLUS_004.02-BNS-logical-05");
    (void)state;
    t3_bns_05_800da8b4();
    buttons = r_u16(0x800ADF00);
    other_buttons = r_u16(0x800ADF02);
    pressed = r_u16(0x800ADF08);
    other_pressed = r_u16(0x800ADF0A);
    buttons |= other_buttons;
    if (((pressed | other_pressed) & 0x1F0) != 0)
    {
        w_u8(0x800EC020, 0);
        t3_800753bc(0, 0x4CEB, 0);
    }
    old_x = r_s8(0x80097F24);
    old_y = r_s8(0x80097F25);
    x = old_x + (sint32)(((buttons >> 13) & 1) - (buttons >> 15));
    y = old_y + (sint32)(((buttons >> 14) & 1) - ((buttons >> 12) & 1));
    if (x < -6)
        x = -6;
    if (x >= 15)
        x = 14;
    if (y < 0)
        y = 0;
    if (y >= 9)
        y = 8;
    pressed = r_u16(0x800ADF08);
    other_pressed = r_u16(0x800ADF0A);
    if ((pressed | other_pressed) == 0x800)
    {
        x = -2;
        y = 0;
        t3_800753bc(0, 0x4D87, 0);
    }
    else if (old_x != x || old_y != y)
    {
        t3_800753bc(0, 0x546C, 0);
    }
    w_u8(0x80097F24, (uint32)x);
    w_u8(0x80097F25, (uint32)y);
    if (x < 0)
        scaled_x = -((-x * 91) >> 6);
    else
        scaled_x = (x * 91 + 63) >> 6;
    color = r_u8(0x80097F0B) != 0 ? 1 : 6;
    twice_y = (uint32)old_y << 1;
    t3_8004ce74(0x800B9458, color, 1, 91 - (uint32)scaled_x, 40 - twice_y);
    t3_8004ce74(0x800B9470, 0, 34 - (uint32)scaled_x, 378 - twice_y, 2, 6);
    t3_8004ce74(0x800B9490, 0, 34 - (uint32)scaled_x, 414 - twice_y, 5, 6);
    t3_bns_05_800dc098(159 - (uint32)scaled_x, 412 - twice_y);
    ordering_entry = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    left = r_s16(0x800AE230);
    height = r_u16(0x800AE236);
    top = r_s16(0x800AE232);
    width = r_u16(0x800AE234);
    center_y = (uint32)(top + ((sint32)(sint16)height >> 1));
    line_y = (center_y - 80) << 16;
    center_x = (uint32)(left + ((sint32)(sint16)width >> 1));
    line_x = (center_x - 46) & 0xFFFF;
    right_x = (uint32)((sint32)(sint16)width + left - 8) & 0xFFFF;
    bottom_y = (uint32)((sint32)(sint16)height + top - 13);
    packet = t3_8004da10(ordering_entry, packet, 0x60E0E0E0,
        (((uint32)left + 6) & 0xFFFF) | line_y, 0x00A00002);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering_entry, packet, 0x60E0E0E0,
        line_x | (((uint32)top + 10) << 16), 0x0003005C);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering_entry, packet, 0x60E0E0E0,
        right_x | line_y, 0x00A00002);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering_entry, packet, 0x60E0E0E0,
        line_x | (bottom_y << 16), 0x0003005C);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering_entry, packet, 0x60E0E0E0,
        ((center_x - 1) & 0xFFFF) | line_y, 0x00A00002);
    ordering_entry = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004da10(ordering_entry, packet, 0x60E0E0E0,
        line_x | ((center_y - 1) << 16), 0x0003005C);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_bns_05_800deb30(uint32 state, uint32 selected, uint32 incoming_expanded, uint32 incoming_depth)
{
    uint32 value;
    uint32 enabled;
    uint32 index;
    uint32 row;
    uint32 expanded;
    uint32 x;
    uint32 y;
    uint32 device;
    uint32 texture_y;
    uint32 clut;
    uint32 ordering;
    uint32 packet;

    FUNCTION_MARKER(0x800DEB30, "SLUS_004.02-BNS-logical-05");
    value = t3_8002a0fc(r_u32(state));
    w_u32(state + 16, value);
    w_u32(state + 36, 0);
    if (selected != 0)
    {
        value = r_u16(0x800ADD68 + (r_u32(state) << 1));
        w_u32(state + 28, value);
        value = r_u16(0x800ADF00 + (r_u32(state) << 1));
        enabled = r_u32(state + 8);
        w_u32(state + 32, value);
        if (enabled == 0)
            w_u32(state + 28, 0);
        if (r_u8(state + 28) != 0)
            w_u32(state + 12, 1);
        if (t3_8004ccac(r_u8(state + 28)) != 1)
            w_u32(state + 28, 0);
        row = state + 40;
        for (index = 0; index < 8; ++index, row += 56)
            t3_bns_05_800de97c(state, row);
    }
    expanded = incoming_expanded;
    row = state + 40;
    for (index = 0; index < 8; ++index, row += 56)
        t3_bns_05_800de42c(state, row, expanded, incoming_depth, &expanded);
    x = r_u32(state + 20) + 20;
    y = r_u32(state + 24) + 136;
    device = r_u32(state + 16);
    texture_y = device == 9 ? 256 : 0;
    clut = device == 9 ? 0x7CE0 : 0x7CA0;
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    packet = t3_8004db9c(ordering, packet, x, y, 144, 136, 384, texture_y, clut, 128);
    w_u32(0x800ADF64, packet);
    x = r_u32(state + 20) + 20;
    y = r_u32(state + 24) + 340;
    t3_8004ce74(0x800B95A4, selected != 0 ? 5 : 10, 0, x, y);
    if (selected != 0)
        t3_bns_05_800dc21c(x, y, 63, r_u32(state + 12) == 0);
    return 0;
}

uint32 t3_bns_05_800daf30(void)
{
    uint32 total;
    uint32 index;
    uint32 flag;
    uint32 mask;
    uint32 character;
    uint32 first_roster;
    uint32 count;
    uint32 first;
    uint32 second;
    uint32 phase;
    uint32 remainder;
    uint8 candidates[24];

    FUNCTION_MARKER(0x800DAF30, "SLUS_004.02-BNS-logical-05");
    total = 0;
    for (index = 0; index < 22; ++index)
        total += t3_80051660(index);
    flag = (total & 0x200) != 0;
    mask = r_u32(0x80097EF0);
    count = 0;
    first_roster = r_u8(0x800E2A74);
    for (index = 0; index < 14; ++index)
    {
        character = index == 0 ? first_roster : r_u8(0x800E2A74 + index);
        if ((mask & ((uint32)1 << (character & 31))) != 0)
        {
            candidates[count] = (uint8)character;
            ++count;
        }
    }
    if (count == 0)
    {
        candidates[0] = (uint8)first_roster;
        count = 1;
    }
    first = r_u8(0x80097F13) + 1;
    if ((sint32)first >= (sint32)count)
        first = 0;
    phase = r_u32(0x800AE218) & 0xFFF;
    remainder = count == 1 ? phase : phase % (count - 1);
    second = (remainder + first + 1) % count;
    w_u8(0x80097F13, first);
    character = candidates[first];
    w_u16(0x800ADD98, flag);
    w_u16(0x800A9128, 1);
    w_u16(0x800ADD5C, character);
    t3_8004f0e4(0);
    character = candidates[second];
    w_u16(0x800ADD9A, flag);
    w_u16(0x800A912A, 1);
    w_u16(0x800ADD5E, character);
    return t3_8004f0e4(1);
}

uint32 t3_bns_05_800dfd64(uint32 record)
{
    uint32 mask;
    uint32 count;
    uint32 index;
    uint32 filled;
    uint32 pairs[44];
    uint32 result;

    FUNCTION_MARKER(0x800DFD64, "SLUS_004.02-BNS-logical-05");
    mask = r_u32(0x800EC44C) & 0x003FFFFF;
    count = t3_8004ccac(mask);
    w_u32(record + 4, count);
    filled = 0;
    for (index = 0; index < 22; ++index)
    {
        if ((mask & ((uint32)1 << index)) != 0)
        {
            pairs[filled * 2] = index;
            pairs[filled * 2 + 1] = t3_80051660(index);
            ++filled;
        }
    }
    result = t3_8004cd80(pairs, count, 0);
    for (index = 0; (sint32)index < (sint32)count; ++index)
    {
        w_u8(record + 8 + index, pairs[index * 2] & 255);
        result = (sint32)(index + 1) < (sint32)count;
    }
    return result;
}

uint32 t3_bns_05_800e0218(uint32 record)
{
    uint32 mask;
    uint32 count;
    uint32 index;
    uint32 filled;
    uint32 pairs[44];
    uint32 source;
    uint32 wins;
    uint32 losses;
    uint32 rate;
    uint32 result;

    FUNCTION_MARKER(0x800E0218, "SLUS_004.02-BNS-logical-05");
    mask = r_u32(0x800EC44C) & 0x003FFFFF;
    count = t3_8004ccac(mask);
    w_u32(record + 4, count);
    filled = 0;
    for (index = 0; index < 22; ++index)
    {
        if ((mask & ((uint32)1 << index)) != 0)
        {
            pairs[filled * 2] = index;
            source = 0x8009804C + index * 8;
            wins = r_u16(source + 2);
            losses = r_u16(source + 4);
            rate = t3_8004cd20(wins, wins + losses);
            wins = r_u16(source + 2);
            losses = r_u16(source + 4);
            pairs[filled * 2 + 1] = (rate << 20) + wins + losses;
            ++filled;
        }
    }
    result = t3_8004cd80(pairs, count, 0);
    for (index = 0; (sint32)index < (sint32)count; ++index)
    {
        w_u8(record + 8 + index, pairs[index * 2] & 255);
        result = (sint32)(index + 1) < (sint32)count;
    }
    return result;
}

uint32 t3_bns_05_800e0860(void)
{
    uint32 index;
    uint32 total;
    uint32 page;
    uint32 record;
    uint32 mask;
    uint32 count;
    uint32 filled;
    uint32 pairs[44];
    sint32 reverse;

    FUNCTION_MARKER(0x800E0860, "SLUS_004.02-BNS-logical-05");
    w_u32(0x800EC44C, r_u32(0x80097EF0) & 0xFFDFFFFF);
    total = 0;
    for (index = 0; index < 22; ++index)
        total += t3_80051660(index);
    w_u32(0x800EC450, total);
    for (page = 0; page < 4; ++page)
    {
        record = 0x800EC458 + page * 32;
        w_u32(record, 0);
        w_u32(record + 4, 0);
        if (page == 0)
        {
            mask = 0x3FF;
            for (index = 0; index < 22; ++index)
            {
                if (r_u32(0x80097F4C + index * 8) <= 359998)
                    mask |= (uint32)1 << index;
            }
            mask &= 0xFFDFFFFF;
            count = t3_8004ccac(mask);
            w_u32(record + 4, count);
            filled = 0;
            for (index = 0; index < 22; ++index)
            {
                if ((mask & ((uint32)1 << index)) != 0)
                {
                    pairs[filled * 2] = index;
                    pairs[filled * 2 + 1] = r_u32(0x80097F4C + index * 8);
                    ++filled;
                }
            }
            t3_8004cd80(pairs, count, 1);
            for (index = 0; index < count; ++index)
                w_u8(record + 8 + index, pairs[index * 2] & 255);
        }
        else if (page == 1)
        {
            w_u32(record + 4, 10);
            for (reverse = 9; reverse >= 0; --reverse)
                w_u8(record + 8 + reverse, reverse);
        }
        else if (page == 2)
        {
            mask = r_u32(0x800EC44C) & 0x003FFFFF;
            count = t3_8004ccac(mask);
            w_u32(record + 4, count);
            filled = 0;
            for (index = 0; index < 22; ++index)
            {
                if ((mask & ((uint32)1 << index)) != 0)
                {
                    pairs[filled * 2] = index;
                    pairs[filled * 2 + 1] = t3_80051660(index);
                    ++filled;
                }
            }
            t3_8004cd80(pairs, count, 0);
            for (index = 0; (sint32)index < (sint32)count; ++index)
                w_u8(record + 8 + index, pairs[index * 2] & 255);
        }
        else
            t3_bns_05_800e0218(record);
    }
    w_u32(0x800EC448, 0);
    return 0x800F0000;
}

uint32 t3_8006ad18(void)
{
    FUNCTION_MARKER(0x8006AD18, "SLUS_004.02");
    return (uint32)r_s16(0x800A05DC);
}

uint32 t3_bns_05_800e1048(uint32 selection)
{
    uint32 kind;
    uint32 table;
    uint32 descriptors;
    uint32 entry;
    uint32 descriptor;
    uint32 value;
    DISPENV display;

    FUNCTION_MARKER(0x800E1048, "SLUS_004.02-BNS-logical-05");
    kind = t3_8006ad18();
    t3_8006c588(31);
    t3_800697d4(-1);
    switch (kind)
    {
    case 1:
        table = 0x800B9FC0;
        descriptors = 0x800B9B50;
        break;
    case 2:
        table = 0x800B9E00;
        descriptors = 0x800B9990;
        break;
    case 3:
        table = 0x800B9C70;
        descriptors = 0x800B981C;
        break;
    default:
        return t3_8007ae58(1);
    }
    entry = table + (selection << 4);
    descriptor = descriptors + ((uint32)r_u8(entry) << 4);
    menu05_movie.kind = kind;
    menu05_movie.selection = selection;
    menu05_movie.last_sector = r_u32(descriptor + 4);
    menu05_movie.first_sector = r_u32(descriptor);
    menu05_movie.last_display_valid = 0;
    w_u16(0x800EC518, r_u16(entry + 10) & 4);
    w_u16(0x800EC51A, (r_u16(entry + 10) >> 8) & 15);
    w_u16(0x800EC51C, r_u16(entry + 10) & 3);
    w_u16(0x800EC524, r_u16(entry + 10) & 8);
    w_u32(0x800EC530, r_u32(entry + 12));
    t3_8006b540(r_u8(entry + 8));
    value = t3_8006b978(-1);
    t3_8006b978((sint32)value);
    t3_8006bc98(127, -1);
    t3_8006ba44();
    w_u16(0x800EC52C, r_s16(0x800EC51A) == 3);
    if (r_s16(0x800EC51C) == 3)
    {
        w_u16(0x800EC51C, 0);
        w_u16(0x800EC51E, 1);
    }
    else
        w_u16(0x800EC51E, 0);
    w_u32(0x800EC53C, (uint32)r_s16(entry + 6));
    w_u32(0x800EC548, r_u32(descriptor));
    value = r_s16(entry + 2) < 257 ? 256 : 320;
    w_u16(0x800EC540, value);
    w_u16(0x800EC542, 240);
    w_u16(0x800EC544, r_u16(entry + 2));
    value = r_u16(entry + 4);
    w_u32(0x800EC538, 0);
    w_u16(0x800EC512, 1);
    w_u16(0x800EC514, 0);
    w_u16(0x800EC546, value);
    t3_8006b54c(0);
    t3_8006ad28(5);
    w_u32(0x8015CD98, 0);
    w_u16(0x800EC516, 0);
    SetDispMask(0);
    w_u16(0x800EC528, 1);
    w_u16(0x800EC52A, 3);
    SetDefDispEnv(&display, 0, 0, 384, 240);
    display.isinter = 1;
    display.screen.x = r_s8(0x80097F24);
    display.screen.w = 258;
    display.screen.h = 240;
    display.screen.y = r_s8(0x80097F25);
    t3_80029dc0(0);
    VSync(0);
    PutDispEnv(&display);
    w_u16(0x800EC526, 0);
    w_u16(0x800EC510, 0);
    t3_8006ad28(5);
    w_u16(0x800EC522, 1);
    return 0;
}

uint32 t3_bns_05_800e1374(void)
{
    XportFmvPresentation presentation;
    uint32 phase;
    uint32 value;
    sint32 result;

    FUNCTION_MARKER(0x800E1374, "SLUS_004.02-BNS-logical-05");
    t3_8006b6fc();
    if (t3_8006c558() == 0)
        return 0xFFFFFFFF;
    if (r_s16(0x800EC522) > 0)
    {
        ClearImage((PSX_RECT *)psx_addr(0x800EB00C, 8), 0, 0, 0);
        w_u16(0x800EC522, r_u16(0x800EC522) - 1);
    }
    phase = (uint32)r_s16(0x800EC528);
    if (phase == 1)
    {
        value = (uint16)(r_u16(0x800EC52A) - 1);
        w_u16(0x800EC52A, value);
        if (value == 0)
        {
            t3_80029dc0(0);
            VSync(0);
            w_u16(0x800EC528, 2);
        }
        t3_80029dc0(0);
        VSync(0);
        return 0;
    }
    if (phase == 2)
    {
        result = -1;
        if (t3_menu05_fmv_presentation_init(&presentation,
            r_u16(0x800EC544), r_u16(0x800EC546), (uint32)r_u16(0x800EC51A) << 8))
        {
            presentation.context = &menu05_movie;
            presentation.tick = t3_menu05_movie_tick;
            presentation.frame = t3_menu05_movie_frame;
            presentation.finish = t3_menu05_movie_finish;
            result = t3_menu05_play_movie(menu05_movie.kind, menu05_movie.selection,
                r_u16(0x800EC518) != 0, &presentation);
        }
        if (result == 1 && !menu05_movie.last_display_valid)
            result = -1;
        if (menu05_movie.last_display_valid)
        {
            PutDispEnv(&menu05_movie.last_display);
            SetDispMask(1);
        }
        if (result == 1)
        {
            w_u16(0x800EC516, 1);
            w_u16(0x800EC528, 3);
            return 0;
        }
        t3_8006bc98(0, -1);
        if (r_s16(0x800EC514) == 0)
            t3_file_audio_stop();
        if (result != 2)
            t3_bns_05_800e0dfc(0x800EC558, 0);
        w_u16(0x800EC528, 5);
        w_u16(0x800EC51E, 0);
        w_u16(0x800EC51C, 0);
        w_u16(0x800EC526, result == 2 ? 0xFFFE : 0xFFFF);
        return 0;
    }
    if (phase == 3)
    {
        if ((uint32)r_u16(0x800EC51C) - 1 < 2)
            return 0;
        if (r_s16(0x800EC51E) == 0)
        {
            t3_8006bc98(0, -1);
            if (r_s16(0x800EC514) == 0)
                t3_file_audio_stop();
        }
        w_u16(0x800EC528, 5);
        w_u16(0x800EC526, 1);
        return 0;
    }
    if (phase == 5)
    {
        t3_bns_05_800e0dfc(0x800EC558, r_u16(0x800EC524));
        value = (uint32)r_s16(0x800EC51E);
        w_u16(0x800EC528, 0);
        if (value != 0)
        {
            t3_8006b524(menu05_movie.first_sector, menu05_movie.last_sector);
            t3_8006ad28(7);
        }
        else
            t3_8006ad28(1);
        return (uint32)r_s16(0x800EC526);
    }
    t3_8007ae58(1);
    return 0;
}
