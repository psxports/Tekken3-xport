#include "game_functions.h"

uint32 t3_bns_05_800dd3c8(void)
{
    uint32 phase;
    uint32 mask;
    uint32 buttons;
    uint32 other_buttons;
    uint32 block;
    uint32 attempt;
    uint32 result;
    uint32 text;
    uint32 color;
    uint32 tail;
    uint32 width;

    FUNCTION_MARKER(0x800DD3C8, "SLUS_004.02-BNS-logical-05");
    phase = r_u8(0x800EC074);
    mask = phase == 0 || phase == 4 || phase == 6 ? 0xF0 : 0x58F0;
    buttons = r_u16(0x800ADF08);
    other_buttons = r_u16(0x800ADF0A);
    if (((buttons | other_buttons) & mask) != 0)
    {
        t3_800753bc(0, 0x546C, 0);
        return 1;
    }
    phase = r_u8(0x800EC074);
    switch (phase)
    {
    case 0:
        for (block = 0; block < 2; ++block)
        {
            for (attempt = 0; attempt < 2; ++attempt)
            {
                result = t3_8004c138(0, 0xFFFFFFFF);
                if (result == 0 || result == 5)
                    break;
            }
            if (result != 3)
                break;
        }
        w_u8(0x800EC074, result + 1);
        return 0;
    case 1:
        return 2;
    case 4:
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        t3_8004cc08(tail, 0x800B9390);
        width = t3_8007a418(tail) * 9;
        t3_8004ce74(0x1F800000, 7, 0, 184 - (uint32)((sint32)width >> 1), 360);
        /* Fall through to confirmation */
    case 6:
        buttons = r_u16(0x800ADF08);
        other_buttons = r_u16(0x800ADF0A);
        if (((buttons | other_buttons) & 0x800) != 0)
        {
            result = 0;
            if (r_u8(0x800EC074) != 6)
            {
                for (block = 0; block < 2; ++block)
                {
                    for (attempt = 0; attempt < 2; ++attempt)
                    {
                        result = t3_8004c1cc();
                        if (result == 0)
                            break;
                    }
                    if (result != 3)
                        break;
                }
            }
            if (result == 0)
            {
                for (block = 0; block < 2; ++block)
                {
                    for (attempt = 0; attempt < 2; ++attempt)
                    {
                        result = t3_8004c138(0, 1);
                        if (result == 0 || result == 5)
                            break;
                    }
                    if (result != 3)
                        break;
                }
            }
            w_u8(0x800EC074, result + 1);
            t3_800753bc(0, 0x4D87, 0);
        }
        t3_8004ce74(0x800B93FC, 0, 4, 21, 2, 6);
        t3_8004ce74(0x800B941C, 0, 4, 23, 5, 6);
        t3_bns_05_800dc098(152, 412);
        return 0;
    case 2:
        text = 0x800B93D4;
        color = 2;
        break;
    case 3:
        text = 0x800B9328;
        color = 2;
        break;
    case 5:
        text = 0x800B9338;
        color = 5;
        break;
    default:
        text = 0x800B935C;
        color = 3;
        break;
    }
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    t3_8004cc08(tail, text);
    width = t3_8007a418(tail) * 9;
    t3_8004ce74(0x1F800000, color, 0, 184 - (uint32)((sint32)width >> 1), 360);
    return 0;
}

uint32 t3_bns_05_800dcb7c(void)
{
    uint32 phase;
    uint32 mask;
    uint32 buttons;
    uint32 other_buttons;
    uint32 result;
    uint32 block;
    uint32 attempt;
    uint32 text;
    uint32 color;
    uint32 tail;
    uint32 width;

    FUNCTION_MARKER(0x800DCB7C, "SLUS_004.02-BNS-logical-05");
    phase = r_u8(0x800EC074);
    mask = phase == 0 ? 0xF0 : 0x58F0;
    buttons = r_u16(0x800ADF08);
    other_buttons = r_u16(0x800ADF0A);
    if (((buttons | other_buttons) & mask) != 0)
    {
        t3_800753bc(0, 0x546C, 0);
        return 1;
    }
    phase = r_u8(0x800EC074);
    switch (phase)
    {
    case 0:
        buttons = r_u16(0x800ADF08);
        other_buttons = r_u16(0x800ADF0A);
        if (((buttons | other_buttons) & 0x800) != 0)
        {
            for (block = 0; block < 2; ++block)
            {
                for (attempt = 0; attempt < 2; ++attempt)
                {
                    result = t3_8004c240();
                    if (result == 0)
                        break;
                }
                if (result != 3)
                    break;
            }
            w_u8(0x800EC074, result + 1);
            t3_800753bc(0, 0x4D87, 0);
        }
        t3_8004ce74(0x800B92D0, 0, 4, 21, 2, 6);
        t3_8004ce74(0x800B92F0, 0, 4, 23, 5, 6);
        t3_bns_05_800dc098(152, 412);
        return 0;
    case 1:
        text = 0x800B9310;
        color = 1;
        break;
    case 2:
    case 4:
        text = 0x800B931C;
        color = 2;
        break;
    case 3:
        text = 0x800B9328;
        color = 2;
        break;
    case 5:
        text = 0x800B9338;
        color = 5;
        break;
    case 6:
        text = 0x800B9348;
        color = 5;
        break;
    default:
        text = 0x800B935C;
        color = 3;
        break;
    }
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    t3_8004cc08(tail, text);
    width = t3_8007a418(tail) * 9;
    t3_8004ce74(0x1F800000, color, 0, 184 - (uint32)((sint32)width >> 1), 360);
    return 0;
}

uint32 t3_bns_05_800dcf28(void)
{
    uint32 phase;
    uint32 mask;
    uint32 buttons;
    uint32 other_buttons;
    uint32 block;
    uint32 attempt;
    uint32 result;
    uint32 text;
    uint32 color;
    uint32 tail;
    uint32 width;

    FUNCTION_MARKER(0x800DCF28, "SLUS_004.02-BNS-logical-05");
    phase = r_u8(0x800EC074);
    mask = phase == 0 || phase == 4 ? 0xF0 : 0x58F0;
    buttons = r_u16(0x800ADF08);
    other_buttons = r_u16(0x800ADF0A);
    if (((buttons | other_buttons) & mask) != 0)
    {
        t3_800753bc(0, 0x546C, 0);
        return 1;
    }
    phase = r_u8(0x800EC074);
    switch (phase)
    {
    case 4:
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        t3_8004cc08(tail, 0x800B9390);
        width = t3_8007a418(tail) * 9;
        t3_8004ce74(0x1F800000, 7, 0, 184 - (uint32)((sint32)width >> 1), 360);
        /* Fall through to the save prompt */
    case 0:
        t3_8004ce74(0x800B93A8, 0, 4, 21, 2, 6);
        t3_8004ce74(0x800B92F0, 0, 4, 23, 5, 6);
        t3_bns_05_800dc098(152, 412);
        buttons = r_u16(0x800ADF08);
        other_buttons = r_u16(0x800ADF0A);
        if (((buttons | other_buttons) & 0x800) == 0)
            return 0;
        result = 0;
        if (r_u8(0x800EC074) != 0)
        {
            for (block = 0; block < 2; ++block)
            {
                for (attempt = 0; attempt < 2; ++attempt)
                {
                    result = t3_8004c1cc();
                    if (result == 0)
                        break;
                }
                if (result != 3)
                    break;
            }
        }
        if (result == 0)
        {
            for (block = 0; block < 2; ++block)
            {
                for (attempt = 0; attempt < 2; ++attempt)
                {
                    result = t3_8004c138(0, 1);
                    if (result == 0 || result == 5)
                        break;
                }
                if (result != 3)
                    break;
            }
        }
        w_u8(0x800EC074, result + 1);
        t3_800753bc(0, 0x4D87, 0);
        return 0;
    case 1:
        text = 0x800B93C8;
        color = 1;
        break;
    case 2:
        text = 0x800B93D4;
        color = 2;
        break;
    case 3:
        text = 0x800B9328;
        color = 2;
        break;
    case 5:
        text = 0x800B9338;
        color = 5;
        break;
    default:
        text = 0x800B935C;
        color = 3;
        break;
    }
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    t3_8004cc08(tail, text);
    width = t3_8007a418(tail) * 9;
    t3_8004ce74(0x1F800000, color, 0, 184 - (uint32)((sint32)width >> 1), 360);
    return 0;
}

uint32 t3_bns_05_800dd8b4(uint32 state)
{
    uint32 phase;
    uint32 result;
    uint32 source;
    uint32 destination;
    uint32 index;
    uint32 count;
    uint32 first;
    uint32 second;

    FUNCTION_MARKER(0x800DD8B4, "SLUS_004.02-BNS-logical-05");
    phase = r_u32(state + 4);
    switch (phase)
    {
    case 0:
        w_u8(0x800EC074, 0);
        t3_bns_05_800dc804(state);
        break;
    case 1:
        if (t3_bns_05_800dcf28() != 0)
            w_u32(state + 4, 0);
        break;
    case 2:
        if (t3_bns_05_800dcb7c() != 0)
            w_u32(state + 4, 0);
        break;
    case 3:
        w_u8(0x80097F11, 1);
        result = t3_bns_05_800dd3c8();
        w_u8(0x80097F11, 0);
        if (result == 1)
        {
            w_u32(state + 4, 0);
        }
        else if (result == 2)
        {
            w_u8(0x80097F11, 1);
            w_u32(state + 4, 0);
        }
        break;
    default:
        break;
    }
    t3_bns_05_800da8b4();
    source = 0x800EAE90;
    destination = 0x800EC030;
    count = 0;
    for (index = 0; index < 8; ++index)
    {
        first = r_u32(source);
        second = r_u32(source + 4);
        w_u32(destination, first);
        w_u32(destination + 4, second);
        if (r_u32(source) != 7 || r_u8(0x80097F26) != 0)
        {
            ++count;
            destination += 8;
        }
        source += 8;
    }
    w_u32(0x800EC070, count);
    return t3_bns_05_800dc474(state);
}

uint32 t3_bns_05_800dc804(uint32 state)
{
    uint32 buttons;
    uint32 other_buttons;
    uint32 selection;
    uint32 limit;
    uint32 vertical;
    uint32 horizontal;
    uint32 candidate;
    uint32 table;
    uint32 entry;
    uint32 kind;
    uint32 output;
    uint32 value;
    uint32 selected;
    uint32 result;

    FUNCTION_MARKER(0x800DC804, "SLUS_004.02-BNS-logical-05");
    buttons = r_u16(0x800ADF00);
    other_buttons = r_u16(0x800ADF02);
    selection = r_u32(state);
    limit = r_u32(state + 40);
    buttons |= other_buttons;
    vertical = ((buttons >> 14) & 1) - ((buttons >> 12) & 1);
    horizontal = ((buttons >> 13) & 1) - (buttons >> 15);
    candidate = selection + vertical;
    if (candidate >= limit)
        candidate = limit - (selection + 1);
    if (vertical != 0)
        t3_800753bc(0, 0x546C, 0);
    selected = candidate;
    table = r_u32(state + 32);
    w_u32(state, selected);
    entry = table + selected * 20;
    kind = r_u8(entry + 12);
    if (kind == 1 || kind == 2)
    {
        output = r_u32(entry);
        value = r_u8(output);
        limit = r_u8(entry + 13);
        candidate = value + horizontal;
        if (candidate >= limit)
            candidate = limit - (value + 1);
        if (horizontal != 0)
            t3_800753bc(0, 0x546C, 0);
        if (kind == 2 && horizontal != 0 && (candidate & 0xFF) != 0)
        {
            w_u32(state + 4, 3);
        }
        else
        {
            table = r_u32(state + 32);
            output = r_u32(table + selected * 20);
            w_u8(output, candidate);
        }
    }
    selection = r_u32(state);
    table = r_u32(state + 32);
    entry = table + selection * 20;
    kind = r_u8(entry + 14);
    if (kind == 1 || kind == 2)
    {
        buttons = r_u16(0x800ADF08);
        other_buttons = r_u16(0x800ADF0A);
        result = (buttons | other_buttons) & 0x8F0;
        if (result != 0)
        {
            output = r_u32(entry);
            value = r_u8(entry + 15);
            w_u8(output, value);
            result = t3_800753bc(0, 0x4CEB, 0);
        }
        if (kind == 2)
        {
            buttons = r_u16(0x800ADD68);
            other_buttons = r_u16(0x800ADD6A);
            result = buttons | other_buttons;
            if (result == 0x103)
            {
                buttons = r_u16(0x800ADF08);
                other_buttons = r_u16(0x800ADF0A);
                result = buttons | other_buttons;
                if (result == 0x100)
                {
                    result = (r_u8(0x80097F0B) + 1) & 1;
                    w_u8(0x80097F0B, result);
                }
            }
        }
    }
    else if (kind == 3)
    {
        buttons = r_u16(0x800ADF08);
        other_buttons = r_u16(0x800ADF0A);
        result = selection + 1;
        if (((buttons | other_buttons) & 0x8F0) != 0)
        {
            w_u32(state + 4, result);
            result = t3_800753bc(0, 0x546C, 0);
        }
    }
    else
    {
        result = kind < 2 ? 2 : 0x800B0000;
    }
    kind = r_u8(state + 48);
    if (kind != 0)
    {
        result = 1;
        if (kind == 1)
        {
            buttons = r_u16(0x800ADF08);
            other_buttons = r_u16(0x800ADF0A);
            result = (buttons | other_buttons) & 0x100;
            if (result != 0)
            {
                output = r_u32(state + 44);
                value = r_u8(state + 49);
                w_u8(output, value);
                result = t3_800753bc(0, 0x4CEB, 0);
            }
        }
    }
    return result;
}

uint32 t3_bns_05_800dc474(uint32 state)
{
    uint32 tail;
    uint32 text;
    uint32 color;
    uint32 x;
    uint32 y;
    uint32 count;
    uint32 index;
    uint32 offset;
    uint32 table;
    uint32 selection;
    uint32 entry;
    uint32 output;
    uint32 value;
    uint32 label_color;
    uint32 value_color;
    uint32 width;
    uint32 timer;
    uint32 ordering_entry;
    uint32 packet;
    uint32 kind;
    uint32 spacing;

    FUNCTION_MARKER(0x800DC474, "SLUS_004.02-BNS-logical-05");
    tail = t3_8004cc08(0x1F800000, 0x80097D08);
    text = r_u32(state + 36);
    if (text != 0)
    {
        t3_8004cc08(tail, text);
        color = r_u8(0x80097F0B) != 0 ? 1 : 6;
        y = r_u32(state + 12);
        x = r_u32(state + 8);
        t3_8004ce74(0x1F800000, color, 1, x, y);
    }
    y = r_u32(state + 20);
    count = r_u32(state + 40);
    x = r_u32(state + 16);
    if ((sint32)count <= 0)
        return x - 3;
    index = 0;
    offset = 0;
    do
    {
        table = r_u32(state + 32);
        selection = r_u32(state);
        entry = table + offset;
        output = r_u32(entry);
        value = r_u8(output);
        label_color = 10;
        if (index == selection)
        {
            kind = r_u16(entry + 16);
            label_color = 5;
            if (kind == 1)
            {
                text = r_u32(entry + 4);
                width = t3_8007a418(text) * 9;
            }
            else
            {
                width = r_u32(state + 28);
            }
            timer = r_u32(0x800AE218);
            ordering_entry = r_u32(0x800A9218);
            packet = r_u32(0x800ADF64);
            color = t3_8004dfe0(timer << 4);
            color = t3_8004df6c(0xFF4020, (color >> 1) + 128);
            packet = t3_8004da10(ordering_entry + 16, packet, color | 0x60000000,
                ((x - 3) & 0xFFFF) | ((y - 3) << 16), ((width + 6) & 0xFFFF) | 0x160000);
            w_u32(0x800ADF64, packet);
        }
        entry = r_u32(state + 32) + offset;
        value_color = 6;
        if (r_u8(entry + 12) != 0)
        {
            output = r_u32(entry);
            color = r_u8(entry + 15);
            if (color == r_u8(output))
                value_color = 1;
        }
        tail = t3_8004cc08(0x1F800000, 0x80097D08);
        entry = r_u32(state + 32) + offset;
        text = r_u32(entry + 4);
        t3_8004cc08(tail, text);
        entry = r_u32(state + 32) + offset;
        kind = r_u16(entry + 16);
        switch (kind)
        {
        case 1:
            t3_8004ce74(0x1F800000, label_color, 0, x, y);
            break;
        case 2:
            t3_8004ce74(0x1F800000, label_color, 0, x, y);
            entry = r_u32(state + 32) + offset;
            table = r_u32(entry + 8);
            text = r_u32(table + value * 4);
            t3_8004cc08(tail, text);
            width = t3_8007a418(tail) * 9;
            selection = r_u32(state + 28);
            t3_8004ce74(0x1F800000, value_color, 0, x + selection - width, y, value);
            break;
        case 3:
            t3_8004ce74(0x1F800000, label_color, 0, x, y);
            t3_8004cc08(tail, 0x800B92B4);
            selection = r_u32(state + 28);
            t3_8004ce74(0x1F800000, value_color, 0, x + selection - 9, y, value);
            break;
        case 4:
            t3_8004ce74(0x1F800000, label_color, 0, x, y);
            t3_8004cc08(tail, 0x800B92B8);
            selection = r_u32(state + 28);
            t3_8004ce74(0x1F800000, value_color, 0, x + selection - 18, y, value);
            break;
        default:
            break;
        }
        offset += 20;
        ++index;
        spacing = r_u32(state + 24);
        count = r_u32(state + 40);
        y += spacing;
    }
    while ((sint32)index < (sint32)count);
    return 0;
}

uint32 t3_bns_05_800df3d0(void)
{
    uint32 controller;
    uint32 state;
    uint32 row;
    uint32 source;
    uint32 index;
    uint32 field;
    uint32 output;
    uint32 input;
    uint32 value;

    FUNCTION_MARKER(0x800DF3D0, "SLUS_004.02-BNS-logical-05");
    for (controller = 0; controller < 2; ++controller)
    {
        state = 0x800EC078 + controller * 488;
        w_u32(state, controller);
        w_u32(state + 4, 0);
        w_u32(state + 8, 0);
        w_u32(state + 20, controller * 184);
        w_u32(state + 24, 0);
        w_u32(state + 12, 0);
        w_u32(state + 16, 0);
        row = state + 40;
        source = 0x800EAF0C;
        for (index = 0; index < 8; ++index)
        {
            w_u32(row, 0);
            w_u32(row + 12, 0);
            value = r_u16(source);
            w_u32(row + 8, value);
            value = r_u8(source + 2);
            w_u32(row + 4, value);
            value = r_u8(source + 3);
            w_u32(row + 16, value);
            for (field = 0; field < 8; ++field)
            {
                value = r_u16(source + 4 + field * 2);
                w_u32(row + 20 + field * 4, value);
            }
            value = r_u32(source + 20);
            w_u32(row + 52, value);
            row += 56;
            source += 24;
        }
        output = 0x80097EB0 + (controller << 5);
        input = 0x80097F28 + (controller << 3);
        for (index = 0; index < 8; ++index)
        {
            value = r_u8(input);
            ++input;
            value = r_u16(0x800B94D0 + value * 2);
            w_u16(output, value);
            output += 2;
        }
        input = 0x800B94EC;
        for (index = 0; index < 8; ++index)
        {
            value = r_u16(input);
            input += 2;
            w_u16(output, value);
            output += 2;
        }
    }
    return 0;
}

uint32 t3_bns_05_800dde64(void)
{
    uint32 initial_page;
    uint32 page;
    uint32 state;
    uint32 check_exit;
    uint32 saved_mode;
    uint32 exit_result;
    uint32 buffer;
    uint32 saved_packet;
    uint32 source;
    uint32 destination;
    uint32 count;
    uint32 index;
    uint32 first;
    uint32 second;
    uint32 entry;
    uint32 output;
    uint32 value;
    sint32 phase;

    FUNCTION_MARKER(0x800DDE64, "SLUS_004.02-BNS-logical-05");
    initial_page = r_u8(0x800EC020);
    check_exit = 1;
    if (initial_page == 3)
    {
        page = r_u8(0x800EC020);
        value = r_u32(0x800EAD28 + page * 52);
        if (value != 0 && value != initial_page)
            check_exit = 0;
    }
    if (check_exit != 0)
    {
        saved_mode = r_u32(0x800AFA88);
        w_u32(0x800AFA88, 0);
        exit_result = t3_8005101c();
        w_u32(0x800AFA88, saved_mode);
        if (exit_result != 0)
            return 1;
    }
    buffer = r_u32(0x800ADEFC);
    saved_packet = r_u32(0x800ADF64);
    w_u32(0x800ADF64, 0x8015CDA0 + (((buffer << 4) - buffer) << 10));
    phase = r_s16(0x800AE224);
    if (phase == 0)
    {
        w_u8(0x80097F41, 1);
        t3_80029638(1);
        t3_8004ca40(0x800E2A84, 0x800D4D68);
        t3_80029638(0);
        source = 0x800EAE90;
        destination = 0x800EC030;
        count = 0;
        for (index = 0; index < 8; ++index)
        {
            first = r_u32(source);
            second = r_u32(source + 4);
            w_u32(destination, first);
            w_u32(destination + 4, second);
            if (r_u32(source) != 7 || r_u8(0x80097F26) != 0)
            {
                ++count;
                destination += 8;
            }
            source += 8;
        }
        w_u32(0x800EC070, count);
        t3_bns_05_800df3d0();
        state = 0x800EAD24;
        for (index = 0; index < 6; ++index)
        {
            w_u32(state, 0);
            w_u32(state + 4, 0);
            state += 52;
        }
        w_u8(0x800EC020, 0);
        w_u16(0x800AE224, 1);
    }
    if (phase == 0 || phase == 1)
    {
        page = r_u8(0x800EC020);
        state = 0x800EAD24 + page * 52;
        switch (page)
        {
        case 0:
            t3_bns_05_800dc804(state);
            t3_bns_05_800dc474(state);
            break;
        case 1:
            t3_bns_05_800dc804(state);
            index = r_u32(state);
            entry = r_u32(state + 32);
            value = r_u16(entry + index * 20 + 18);
            t3_bns_05_800dc2f8(value);
            t3_bns_05_800dc474(state);
            break;
        case 2:
            if (r_u32(state + 4) == 0)
            {
                t3_bns_05_800e0860();
                w_u32(state + 4, r_u32(state + 4) + 1);
            }
            if (t3_bns_05_800e0b1c() != 0)
            {
                output = r_u32(state + 44);
                value = r_u8(state + 49);
                w_u32(state + 4, 0);
                w_u8(output, value);
            }
            break;
        case 3:
            t3_bns_05_800dd8b4(state);
            break;
        case 4:
            if (t3_bns_05_800df014() != 0)
                w_u8(0x800EC020, 0);
            break;
        case 5:
            t3_bns_05_800dda30(state);
            break;
        default:
            w_u16(0x800AE224, 2);
            break;
        }
    }
    else if (phase == 2)
    {
        t3_8004f8f8(4);
    }
    t3_bns_05_800da4bc(6);
    w_u32(0x800ADF64, saved_packet);
    return 0;
}

uint32 t3_8002943c(const sint16 *rectangle)
{
    uint32 count;
    uint32 packet;
    uint32 width;
    uint32 height;

    FUNCTION_MARKER(0x8002943C, "SLUS_004.02");
    count = r_u32(0x8009BC54);
    if ((sint32)count >= 32)
        return 1;
    packet = r_u32(0x8009BC50);
    w_u16(packet, 3);
    w_u16(packet + 4, (uint16)rectangle[0]);
    w_u16(packet + 6, (uint16)rectangle[1]);
    width = (uint16)rectangle[2];
    w_u32(0x8009BC54, count + 1);
    w_u16(packet + 8, width);
    height = (uint16)rectangle[3];
    w_u32(0x8009BC50, packet + 12);
    w_u16(packet + 10, height);
    return 0;
}

uint32 t3_800295b8(void)
{
    uint32 y;
    uint32 selected;

    FUNCTION_MARKER(0x800295B8, "SLUS_004.02");
    y = (uint32)r_s8(0x80097F25);
    if (y >= 8)
        y = 7;
    selected = r_u32(0x80095438 + y * 4);
    w_u32(0x80095430, selected);
    return 0x80090000;
}

uint32 t3_8004f870(void)
{
    uint32 value;
    uint32 result;

    FUNCTION_MARKER(0x8004F870, "SLUS_004.02");
    value = r_u8(0x80097F3E) + 1;
    result = value < 4;
    if (result == 0)
        value = 0;
    w_u8(0x80097F3E, value);
    return result;
}

uint32 t3_8006c558(void)
{
    FUNCTION_MARKER(0x8006C558, "SLUS_004.02");
    return r_u32(0x800A866C);
}

uint32 t3_80050ee4(uint32 controller)
{
    uint32 buttons;
    uint32 result;

    FUNCTION_MARKER(0x80050EE4, "SLUS_004.02");
    buttons = r_u16(0x800ADD68 + (controller << 1));
    result = buttons ^ 0x80C;
    if (buttons & 0x800)
    {
        result = result == 0;
        w_u8(0x800AFA97, result);
    }
    return result;
}

uint32 t3_8005135c(void)
{
    uint32 index;

    FUNCTION_MARKER(0x8005135C, "SLUS_004.02");
    for (index = 0; index < 10; ++index)
    {
        w_u32(0x800AFAC8, 0);
        w_u16(0x800AFACE, 0);
        w_u8(0x800AFACC, 0);
        w_u8(0x800AFACD, 0);
    }
    return 0;
}

uint32 t3_800516bc(void)
{
    FUNCTION_MARKER(0x800516BC, "SLUS_004.02");
    w_u8(0x800980FC, 2);
    return 2;
}

uint32 t3_bns_05_800de198(uint32 controller)
{
    uint32 destination;
    uint32 source;
    uint32 index;
    uint32 code;
    uint32 value;

    FUNCTION_MARKER(0x800DE198, "SLUS_004.02-BNS-logical-05");
    destination = 0x80097EB0 + (controller << 5);
    source = 0x80097F28 + (controller << 3);
    for (index = 0; index < 8; ++index)
    {
        code = r_u8(source++);
        value = r_u16(0x800B94D0 + code * 2);
        w_u16(destination, value);
        destination += 2;
    }
    source = 0x800B94EC;
    for (index = 0; index < 8; ++index)
    {
        value = r_u16(source);
        source += 2;
        w_u16(destination, value);
        destination += 2;
    }
    return 0;
}

uint32 t3_bns_05_800db5b4(void)
{
    uint32 buttons;
    uint32 directions;
    uint32 value;
    uint32 first;
    uint32 second;
    uint32 missing_controller;
    sint32 phase;
    uint32 x;
    uint32 y;
    uint32 source;
    uint32 destination;
    uint32 flags;
    uint32 count;
    uint32 index;
    uint32 selected;
    uint32 next;
    uint32 step;
    uint32 scroll;
    uint32 negative;
    uint32 ordering;
    uint32 cursor;
    uint32 saved_cursor;
    uint32 buffer;
    uint32 label_end;
    uint32 entry;
    uint32 length;
    uint32 locked;
    uint32 color;
    uint32 dark;
    uint32 light;
    PSX_RECT rectangle;

    FUNCTION_MARKER(0x800DB5B4, "SLUS_004.02-BNS-logical-05");
    first = r_u16(0x800ADF08);
    second = r_u16(0x800ADF0A);
    x = r_u16(0x800ADF00);
    y = r_u16(0x800ADF02);
    buttons = first | second;
    directions = x | y;
    missing_controller = t3_8002a0e4(0) == 0 || t3_8002a0e4(1) == 0;
    phase = r_s16(0x800AE224);
    switch (phase)
    {
    case 0:
        w_u16(0x800ADE14, 258);
        x = r_u8(0x80097F24);
        w_u16(0x800ADE16, 240);
        y = r_u8(0x80097F25);
        w_u16(0x800ADE10, (uint32)(sint32)(sint8)x);
        w_u16(0x800ADE12, (uint32)(sint32)(sint8)y);
        t3_800295b8();
        t3_bns_05_800db0a4();
        t3_80028f5c();
        t3_80029638(1);
        t3_8004ca40(0x800E2A84, 0x800D499C);
        if (r_u8(0x80097F0B))
            t3_8004c6fc(0x800E2A84, 0x800DA430);
        t3_80029638(0);
        w_u16(0x800AE1FA, 0);
        w_u16(0x800AE1F8, 0);
        t3_bns_05_800de198(0);
        t3_bns_05_800de198(1);
        source = 0x800B8E44;
        destination = 0x800EBFC8;
        count = 0;
        w_u32(0x800EBFB8, 0);
        for (index = 0; index < 10; ++index)
        {
            first = r_u32(source);
            second = r_u32(source + 4);
            w_u32(destination, first);
            w_u32(destination + 4, second);
            w_u8(destination + 7, 0);
            value = r_u8(source + 4);
            if (value == 7)
                value = r_u8(0x80097F26);
            else if (value == 10)
                value = r_u8(0x80097F27);
            else
                value = 3;
            if (value)
            {
                if (value < 3)
                    w_u8(destination + 7, 1);
                ++count;
                destination += 8;
            }
            source += 8;
        }
        value = r_u8(0x80097F41);
        w_u32(0x800EBFBC, count);
        if (value)
        {
            selected = 0;
            entry = 0x800EBFC8;
            w_u8(0x80097F41, 0);
            if ((sint32)count > 0)
            {
                do
                {
                    if (r_u8(entry + 4) == 9)
                        break;
                    ++selected;
                    entry += 8;
                } while ((sint32)selected < (sint32)count);
            }
            if ((sint32)selected >= 11)
                selected = 0;
            w_u8(0x80097F40, selected);
        }
        w_u32(0x800AE214, 0);
        w_u32(0x800EBFC0, 0);
        w_u32(0x800EBFC4, 0);
        w_u16(0x800AE224, 1);
        break;
    case 1:
        value = r_u32(0x800AE214);
        if ((sint32)value >= 2)
            SetDispMask(1);
        value = r_u32(0x800AE214) + 1;
        w_u32(0x800AE214, value);
        if ((sint32)value >= 481)
        {
            w_u32(0x800EBFB8, 1);
            w_u16(0x800AE224, 3);
            break;
        }
        step = ((directions >> 14) & 1) - ((directions >> 12) & 1);
        selected = r_u8(0x80097F40);
        count = r_u32(0x800EBFBC);
        if (selected >= count)
            selected = 0;
        for (;;)
        {
            next = selected + step;
            if (next < count)
                selected = next;
            else
                selected = count - (selected + 1);
            scroll = r_u32(0x800EBFC0);
            w_u32(0x800EBFC0, scroll + (((step << 3) - step) << 2));
            flags = r_u8(0x800EBFCD + (selected << 3));
            if (flags != 1 || missing_controller != 1)
                break;
            if (step == 0)
                step = 1;
        }
        w_u8(0x80097F40, selected);
        if (step)
        {
            w_u32(0x800AE214, 8);
            t3_800753bc(0, 21612, 0);
        }
        if (buttons & 0x8F0)
        {
            t3_800753bc(0, 19847, 0);
            w_u32(0x800EBFB8, 1);
            w_u32(0x800EBFC0, 0);
            w_u32(0x800EBFC4, 256);
            first = r_u16(0x800ADF08);
            w_u32(0x800EBFAC, 0);
            if (first & 0x8F0)
                w_u32(0x800EBFAC, 1);
            second = r_u16(0x800ADF0A);
            if (second & 0x8F0)
                w_u32(0x800EBFAC, r_u32(0x800EBFAC) | 2);
            w_u16(0x800AE224, 2);
        }
        break;
    case 2:
        selected = r_u8(0x80097F40);
        value = r_u8(0x800EBFCC + (selected << 3));
        if (value == 9)
            t3_8004f8f8(5);
        else if (value == 10)
        {
            first = r_u8(0x80097F27);
            if (first < 3)
                w_u8(0x80097F27, first + 1);
            w_u16(0x800ADF10, 0);
            t3_8004ff20(19);
        }
        else
        {
            if (value == 7)
            {
                first = r_u8(0x80097F26);
                if (first < 3)
                    w_u8(0x80097F26, first + 1);
            }
            t3_bns_05_800da910(value, r_u32(0x800EBFAC));
        }
        SetDispMask(1);
        break;
    case 3:
        t3_8004f8f8(3);
        break;
    default:
        break;
    }

    rectangle.w = 368;
    rectangle.h = 480;
    ordering = r_u32(0x800A9218);
    buffer = r_u32(0x800ADEFC);
    saved_cursor = r_u32(0x800ADF64);
    rectangle.x = 0;
    rectangle.y = 0;
    cursor = (((buffer << 4) - buffer) << 10) + 0x8015CDA0;
    w_u32(0x800ADF64, cursor);
    cursor = t3_8004db28(ordering + 28, cursor, &rectangle);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, cursor);
    cursor = t3_8004d8b0(ordering + 28, cursor, 0x3AE0E0E0, 0x00E0E0E0, 0, 0,
        0x013C0044, 0x013C012C, 0x01580044, 0x0158012C);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, cursor);
    cursor = t3_8004d8b0(ordering + 28, cursor, 0x3A000000, 0, 0x00F8F8F8, 0x00F8F8F8,
        0x01940044, 0x0194012C, 0x01B00044, 0x01B0012C);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, cursor);
    cursor = t3_8004daa8(ordering + 28, cursor, 64);
    scroll = r_u32(0x800EBFC0);
    w_u32(0x800ADF64, cursor);
    if (scroll)
    {
        negative = (sint32)scroll < 0;
        if (negative)
            scroll = 0 - scroll;
        scroll -= (uint32)((sint32)(scroll + 3) >> 2);
        if ((sint32)scroll <= 0)
            scroll = 0;
        if (negative)
            scroll = 0 - scroll;
    }
    selected = r_u8(0x80097F40);
    w_u32(0x800EBFC0, scroll);
    y = scroll + 278;
    if (selected < 3)
        selected = selected - 3 + r_u32(0x800EBFBC);
    else
        selected -= 3;
    label_end = t3_8004cc08(0x1F800000, 0x80097D08);
    for (index = 0; index < 7; ++index)
    {
        count = r_u32(0x800EBFBC);
        if (count)
            selected %= count;
        entry = 0x800EBFC8 + (selected << 3);
        source = r_u32(entry);
        t3_8004cc08(label_end, source);
        length = t3_8007a418(label_end);
        x = 184 - (uint32)((sint32)(length * 13) >> 1);
        locked = r_u32(0x800EBFB8);
        color = r_u8(entry + 6);
        if (locked == 0 && r_u8(entry + 7) && (r_u32(0x800AE218) & 3) == 0)
            color = 14;
        t3_8004ce74(0x1F800000, color, (uint32)1, x, y);
        ++selected;
        y += 28;
    }
    cursor = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    rectangle.x = 48;
    rectangle.y = 316;
    rectangle.w = 320;
    rectangle.h = 116;
    cursor = t3_8004db28(ordering + 28, cursor, &rectangle);
    scroll = r_u32(0x800EBFC0);
    w_u32(0x800ADF64, cursor);
    if (scroll)
        w_u32(0x800EBFC4, 0);
    else
    {
        value = r_u32(0x800EBFC4) + 24;
        w_u32(0x800EBFC4, value);
        if ((sint32)value >= 257)
            w_u32(0x800EBFC4, 256);
        dark = t3_8004df6c(0x00C0C0C0, r_u32(0x800EBFC4));
        light = t3_8004df6c(0x00F8F8F8, r_u32(0x800EBFC4));
        cursor = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        cursor = t3_8004d8b0(ordering + 8, cursor, 0x38000000, light, 0, light,
            0x01680000, 0x016800B8, 0x016A0000, 0x016A00B8);
        ordering = r_u32(0x800A9218);
        w_u32(0x800ADF64, cursor);
        cursor = t3_8004d8b0(ordering + 8, cursor, light | 0x38000000, 0, light, 0,
            0x016800B8, 0x01680170, 0x016A00B8, 0x016A0170);
        ordering = r_u32(0x800A9218);
        w_u32(0x800ADF64, cursor);
        cursor = t3_8004d8b0(ordering + 8, cursor, 0x38000000, light, 0, light,
            0x01820000, 0x018200B8, 0x01840000, 0x018400B8);
        ordering = r_u32(0x800A9218);
        w_u32(0x800ADF64, cursor);
        cursor = t3_8004d8b0(ordering + 8, cursor, light | 0x38000000, 0, light, 0,
            0x018200B8, 0x01820170, 0x018400B8, 0x01840170);
        ordering = r_u32(0x800A9218);
        w_u32(0x800ADF64, cursor);
        cursor = t3_8004d8b0(ordering + 8, cursor, 0x38000000, dark, 0, dark,
            0x01680008, 0x016800B8, 0x01840008, 0x018400B8);
        ordering = r_u32(0x800A9218);
        w_u32(0x800ADF64, cursor);
        cursor = t3_8004d8b0(ordering + 8, cursor, dark | 0x38000000, 0, dark, 0,
            0x016800B8, 0x01680168, 0x018400B8, 0x01840168);
        w_u32(0x800ADF64, cursor);
    }
    t3_bns_05_800da4bc(5);
    w_u32(0x800ADF64, saved_cursor);
    return 0x800B0000;
}

uint32 t3_bns_05_800da910(uint32 mode, uint32 controller)
{
    uint32 state = 0x800AFA88;
    uint32 index;
    uint32 option_a;
    uint32 option_b;
    uint32 option_c;
    uint32 side;
    uint32 resource;

    FUNCTION_MARKER(0x800DA910, "SLUS_004.02-BNS-logical-05");
    t3_8006c588(31);
    t3_800697d4(-1);
    w_u16(0x800AAAC6, 1);
    w_u16(0x800AC352, 1);
    w_u8(0x800AAAD2, 1);
    w_u16(0x800A923A, 0);
    w_u8(0x800A9246, 0);
    w_u8(0x800AC35E, 2);
    w_u32(state, mode);
    w_u8(state + 0x19, 0);
    w_u8(state + 0x0F, 0);
    w_u8(state + 0x11, 0);
    w_u8(state + 0x12, 0);
    w_u8(state + 0x13, 0);
    w_u32(0x800954B0, 0);
    w_u32(0x800954B4, 0);
    w_u32(state + 0x24, 0);
    w_u32(state + 0x28, 0);
    w_u32(state + 0x2C, 0);
    w_u32(state + 0x30, 0);
    w_u32(state + 0x34, 0);
    w_u16(0x800AE212, 0);
    option_a = r_u8(0x80097F0A);
    option_b = r_u8(0x80097F10);
    option_c = r_u8(0x80097F07);
    w_u16(0x800ADED4, 0);
    w_u8(state + 0x0D, option_a);
    w_u8(state + 0x0E, option_b);
    w_u8(state + 0x20, option_c);
    w_u16(0x800AE208, r_u8(0x80097F06));
    w_u16(0x800ADEF8, r_u8(0x80097F08));
    w_u16(0x800AFA58, r_u8(0x80097F09));
    w_u8(0x800AE211, 0);
    for (index = 0; index < 2; ++index)
    {
        t3_8002a16c(index, r_u8(0x80097F0C + index));
        t3_80050ee4(index);
        t3_800517b0(index);
        if (r_u8(0x80097F19) == 0)
            w_u8(0x80097F0E + index, 88);
        w_u16(0x800ADD5C + index * 2, 22);
        w_u16(0x800ADD98 + index * 2, 0);
        w_u16(0x800ADFBC + index * 2, 0);
        w_u16(0x800AE1F8 + index * 2, 0);
    }
    side = (controller ^ 1) & 1;
    t3_8005118c(side);
    w_u16(0x800AE1F8 + side * 2, 1);
    t3_80031d0c(0);

    switch (mode)
    {
    case 0:
        w_u8(state + 4, 1);
        w_u8(state + 5, 1);
        w_u8(state + 6, 0);
        w_u8(state + 7, 1);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
        w_u8(state + 10, 0);
        w_u8(state + 11, 1);
        w_u8(state + 12, 0);
        t3_8005135c();
        break;
    case 1:
        w_u8(state + 4, 1);
        w_u8(state + 7, 1);
        w_u8(state + 8, 1);
        w_u8(state + 10, 1);
        w_u8(state + 11, 1);
        w_u8(state + 12, 1);
        w_u8(state + 5, 0);
        w_u8(state + 6, 0);
        w_u8(state + 9, 0);
        w_u16(0x800AE1FA, 1);
        w_u16(0x800AE1F8, 1);
        w_u16(state + 0x3A, 3);
        w_u16(state + 0x3E, 3);
        w_u16(state + 0x40, 0);
        w_u16(state + 0x38, 0);
        w_u16(state + 0x3C, 0);
        w_u32(state + 0x48, 0xFFFFFFFF);
        w_u32(state + 0x4C, 0xFFFFFFFF);
        w_u32(state + 0x44, 0xFFFFFFFF);
        break;
    case 2:
        w_u8(state + 4, 1);
        w_u8(state + 5, 1);
        w_u8(state + 6, 1);
        w_u8(state + 8, 1);
        w_u8(state + 9, 1);
        w_u8(state + 10, 1);
        w_u8(state + 11, 1);
        w_u8(state + 12, 1);
        w_u8(state + 7, 0);
        w_u8(state + 0x20, 0);
        w_u8(state + 0x65, 4);
        w_u8(state + 0x72, 4);
        for (index = 0; index < 22; ++index)
            w_u16(state + 0x9E - index * 2, 0);
        break;
    case 3:
        w_u8(state + 11, 1);
        w_u8(state + 4, 0);
        w_u8(state + 5, 0);
        w_u8(state + 6, 0);
        w_u8(state + 7, 0);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
        w_u8(state + 10, 0);
        w_u8(state + 12, 0);
        w_u8(state + 13, 0);
        w_u16(0x800AE208, 1);
        w_u8(state + 0x20, 1);
        w_u16(0x800ADEF8, 2);
        w_u16(0x800AFA58, 0);
        t3_8005135c();
        break;
    case 4:
        w_u8(state + 7, 1);
        w_u8(state + 9, 1);
        w_u8(state + 10, 1);
        w_u8(state + 11, 1);
        w_u8(state + 4, 0);
        w_u8(state + 5, 0);
        w_u8(state + 6, 0);
        w_u8(state + 8, 0);
        w_u8(state + 12, 0);
        w_u16(0x800AE208, 1);
        w_u8(state + 0x20, 0);
        w_u16(0x800ADEF8, 2);
        w_u16(0x800AFA58, 0);
        w_u32(state + 0x40, 0);
        w_u32(state + 0x44, 0);
        w_u32(state + 0x48, 0);
        for (index = 0; index < 22; ++index)
            w_u16(state + 0x8A - index * 2, 0);
        w_u32(state + 0x4C, 0);
        for (index = 0; index < 4; ++index)
            w_u32(state + 0x5C - index * 4, 22);
        break;
    case 5:
        w_u8(state + 4, 1);
        w_u8(state + 11, 1);
        w_u8(state + 5, 0);
        w_u8(state + 6, 0);
        w_u8(state + 7, 0);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
        w_u8(state + 10, 0);
        w_u8(state + 12, 0);
        w_u8(state + 0x20, 0);
        w_u16(0x800ADEF8, 5);
        w_u32(0x800954B4, 1);
        w_u16(0x800AFA58, 0);
        goto finish;
    case 6:
        w_u8(state + 4, 0);
        w_u8(state + 5, 0);
        w_u8(state + 6, 0);
        w_u8(state + 7, 0);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
        w_u8(state + 10, 1);
        w_u8(state + 11, 1);
        w_u8(state + 12, 0);
        t3_8002a16c(0, 0);
        t3_8002a16c(1, 0);
        w_u16(0x800ADEF8, 2);
        w_u16(0x800ADED4, 1);
        t3_bns_05_800daf30();
        goto finish;
    case 7:
        w_u8(state + 4, 1);
        w_u8(state + 5, 1);
        w_u8(state + 8, 1);
        w_u8(state + 12, 1);
        w_u8(state + 6, 0);
        w_u8(state + 7, 0);
        w_u8(state + 9, 0);
        w_u8(state + 10, 0);
        w_u8(state + 11, 0);
        w_u16(0x800ADEF8, 5);
        w_u16(0x800AFA58, 0);
        w_u16(state + 0x3A, 3);
        w_u16(state + 0x3E, 3);
        w_u16(state + 0x40, 0);
        w_u16(state + 0x38, 0);
        w_u16(state + 0x3C, 0);
        w_u32(state + 0x48, 0xFFFFFFFF);
        w_u32(state + 0x4C, 0xFFFFFFFF);
        w_u32(state + 0x44, 0xFFFFFFFF);
        break;
    case 8:
        w_u8(state + 4, 1);
        w_u8(state + 5, 0);
        w_u8(state + 6, 0);
        w_u8(state + 7, 0);
        w_u8(state + 8, 0);
        w_u8(state + 9, 0);
        w_u8(state + 10, 0);
        w_u8(state + 11, 0);
        w_u8(state + 12, 0);
        w_u16(0x800ADEF8, 4);
        w_u8(state + 0x20, 0);
        w_u16(0x800AFA58, 0);
        break;
    default:
        goto finish;
    }
    t3_800516bc();
finish:
    if (r_u16(0x800ADEF8) >= 5)
        w_u32(0x800954B0, 1);
    resource = t3_bns_05_800daec8(mode);
    w_u8(state + 0x16, resource);
    w_u16(0x800AE204, 7);
    w_u16(0x800AE224, 0);
    return 0x800B0000;
}

uint32 t3_bns_05_800da4bc(uint32 draw_mode)
{
    uint32 ordering;
    uint32 cursor;
    uint32 result;
    uint32 clut;

    FUNCTION_MARKER(0x800DA4BC, "SLUS_004.02-BNS-logical-05");
    switch (draw_mode)
    {
    case 0:
        ordering = r_u32(0x800A9218);
        cursor = r_u32(0x800ADF64);
        result = t3_8004da10(ordering, cursor, 0x60000000, 0, 0x01E00170);
        w_u32(0x800ADF64, result);
        return result;
    case 1:
    case 7:
        return 0x800DA880;
    case 2:
        t3_8004ee14();
        /* Fall through */
    case 3:
        cursor = r_u32(0x800ADF64);
        ordering = r_u32(0x800A9218);
        result = t3_8004db9c(ordering + 28, cursor, 151, 388, 72, 16, 800, 0, 0x7820, 0);
        w_u32(0x800ADF64, result);
        /* Fall through */
    case 4:
        t3_8004ce74(0x800B8D58, (uint32)6, (uint32)36, (uint32)412);
        t3_8004ce74(0x800B8D84, (uint32)6, (uint32)99, (uint32)432);
        /* Fall through */
    case 5:
        clut = 0x7EA0;
        break;
    case 6:
        clut = 0x7D20;
        break;
    default:
        ordering = r_u32(0x800A9218);
        cursor = r_u32(0x800ADF64);
        result = t3_8004e000(ordering + 28, cursor, 0);
        w_u32(0x800ADF64, result);
        return result;
    }

    ordering = r_u32(0x800A9218);
    cursor = r_u32(0x800ADF64);
    result = t3_8004db9c(ordering, cursor, 0, 0, 128, 256, 512, 0, clut, 128);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, result);
    result = t3_8004db9c(ordering, result, 128, 0, 128, 256, 576, 0, clut + 0x40, 128);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, result);
    result = t3_8004db9c(ordering, result, 256, 0, 112, 256, 640, 0, clut + 0x80, 128);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, result);
    result = t3_8004db9c(ordering, result, 0, 256, 128, 224, 512, 256, clut + 0xC0, 128);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, result);
    result = t3_8004db9c(ordering, result, 128, 256, 128, 224, 576, 256, clut + 0x100, 128);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, result);
    result = t3_8004db9c(ordering, result, 256, 256, 112, 224, 640, 256, clut + 0x140, 128);
    w_u32(0x800ADF64, result);
    return result;
}

uint32 t3_bns_05_800da8b4(void)
{
    uint32 x;
    uint32 y;

    FUNCTION_MARKER(0x800DA8B4, "SLUS_004.02-BNS-logical-05");
    w_u16(0x800ADE14, 258);
    x = r_u8(0x80097F24);
    w_u16(0x800ADE16, 240);
    y = r_u8(0x80097F25);
    w_u16(0x800ADE10, (uint32)(sint32)(sint8)x);
    w_u16(0x800ADE12, (uint32)(sint32)(sint8)y);
    return t3_800295b8();
}

uint32 t3_bns_05_800daec8(uint32 mode)
{
    FUNCTION_MARKER(0x800DAEC8, "SLUS_004.02-BNS-logical-05");
    switch (mode)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
        return 0;
    case 5:
        return 1;
    case 7:
        return 3;
    case 8:
        return 2;
    default:
        return t3_8007ae58(1);
    }
}

uint32 t3_bns_05_800dbfb8(uint32 x, uint32 y, uint32 direction)
{
    uint32 ordering;
    uint32 cursor;
    uint32 texture;
    uint32 result;

    FUNCTION_MARKER(0x800DBFB8, "SLUS_004.02-BNS-logical-05");
    ordering = r_u32(0x800A9218);
    cursor = r_u32(0x800ADF64);
    ordering += 16;
    switch (direction)
    {
    case 0x10:
        texture = r_u32(0x800B92A8);
        break;
    case 0x20:
        texture = r_u32(0x800B92B0);
        break;
    case 0x40:
        texture = r_u32(0x800B92AC);
        break;
    default:
        texture = r_u32(0x800B92A4);
        break;
    }
    result = t3_8004d9bc(ordering, cursor, 0x65000000, (x & 0xFFFF) | (y << 16), 0x0014000C, texture);
    result = t3_8004daa8(ordering, result, 31);
    w_u32(0x800ADF64, result);
    return result;
}

uint32 t3_bns_05_800dc098(uint32 x, uint32 y)
{
    uint32 cursor;
    uint32 ordering;
    uint32 texture;
    uint32 packed_y = y << 16;

    FUNCTION_MARKER(0x800DC098, "SLUS_004.02-BNS-logical-05");
    cursor = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    texture = r_u32(0x800B92A4);
    ordering += 16;
    cursor = t3_8004d9bc(ordering, cursor, 0x65000000, (x & 0xFFFF) | packed_y, 0x0014000C, texture);
    cursor = t3_8004daa8(ordering, cursor, 31);
    ordering = r_u32(0x800A9218);
    texture = r_u32(0x800B92A8);
    w_u32(0x800ADF64, cursor);
    ordering += 16;
    cursor = t3_8004d9bc(ordering, cursor, 0x65000000, ((x + 18) & 0xFFFF) | packed_y, 0x0014000C, texture);
    cursor = t3_8004daa8(ordering, cursor, 31);
    ordering = r_u32(0x800A9218);
    texture = r_u32(0x800B92AC);
    w_u32(0x800ADF64, cursor);
    ordering += 16;
    cursor = t3_8004d9bc(ordering, cursor, 0x65000000, ((x + 36) & 0xFFFF) | packed_y, 0x0014000C, texture);
    cursor = t3_8004daa8(ordering, cursor, 31);
    ordering = r_u32(0x800A9218);
    texture = r_u32(0x800B92B0);
    w_u32(0x800ADF64, cursor);
    ordering += 16;
    cursor = t3_8004d9bc(ordering, cursor, 0x65000000, ((x + 80) & 0xFFFF) | packed_y, 0x0014000C, texture);
    cursor = t3_8004daa8(ordering, cursor, 31);
    w_u32(0x800ADF64, cursor);
    return cursor;
}

uint32 t3_bns_05_800dc21c(uint32 x, uint32 y, uint32 width, uint32 pulse)
{
    uint32 ordering;
    uint32 cursor;
    uint32 color;
    uint32 phase;

    FUNCTION_MARKER(0x800DC21C, "SLUS_004.02-BNS-logical-05");
    ordering = r_u32(0x800A9218);
    cursor = r_u32(0x800ADF64);
    ordering += 16;
    if (pulse)
    {
        phase = r_u32(0x800AE218);
        color = t3_8004dfe0(phase << 4);
        color = t3_8004df6c(0x00FF4020, (color >> 1) + 128);
    }
    else
        color = 0x00FF4020;
    cursor = t3_8004da10(ordering, cursor, color | 0x60000000,
        ((x - 3) & 0xFFFF) | ((y - 3) << 16), ((width + 6) & 0xFFFF) | 0x00160000);
    w_u32(0x800ADF64, cursor);
    return cursor;
}

uint32 t3_bns_05_800dc2f8(uint32 mask)
{
    uint32 cursor;
    uint32 ordering;
    uint32 count;
    uint32 entry = 0x800EC030;
    uint32 index = 0;
    uint32 y = 42;
    uint32 bit;
    uint32 texture;
    uint32 size;

    FUNCTION_MARKER(0x800DC2F8, "SLUS_004.02-BNS-logical-05");
    cursor = r_u32(0x800ADF64);
    ordering = r_u32(0x800A9218);
    count = r_u32(0x800EC070);
    ordering += 28;
    if (count)
    {
        do
        {
            bit = r_u32(entry);
            texture = r_u32(entry + 4);
            if (((mask >> (bit & 31)) & 1) == 0)
                texture = (texture & 0xFFFF) | 0x7E5E0000;
            cursor = t3_8004d9bc(ordering, cursor, 0x64808080, (y << 16) | 247, 0x000E0060, texture);
            y += 14;
            ++index;
            count = r_u32(0x800EC070);
            entry += 8;
        } while (index < count);
    }
    count = r_u32(0x800EC070);
    size = (((count << 3) - count) << 17) | 96;
    cursor = t3_8004da10(ordering, cursor, 0x60000000, 0x002A00F7, size);
    count = r_u32(0x800EC070);
    size = (((((count << 3) - count) << 1) + 4) << 16) | 98;
    cursor = t3_8004da10(ordering, cursor, 0x60C0C0C0, 0x002800F6, size);
    cursor = t3_8004daa8(ordering, cursor, 11);
    w_u32(0x800ADF64, cursor);
    return cursor;
}

uint32 t3_bns_05_800db0a4(void)
{
    sint16 rectangle[4];
    uint32 top;
    uint32 width;
    uint32 bottom;
    uint32 remaining;

    FUNCTION_MARKER(0x800DB0A4, "SLUS_004.02-BNS-logical-05");
    top = (uint32)r_s16(0x800AE232);
    width = (uint32)r_s16(0x800AE234);
    if (top)
    {
        rectangle[0] = 0;
        rectangle[1] = 0;
        rectangle[2] = (sint16)width;
        rectangle[3] = (sint16)top;
        t3_8002943c(rectangle);
        top = (uint32)r_s16(0x800AE232);
    }
    bottom = (uint32)r_s16(0x800AE236);
    bottom += top;
    remaining = 480 - bottom;
    if (remaining == 0)
        return remaining;
    rectangle[0] = 0;
    rectangle[1] = (sint16)bottom;
    rectangle[2] = (sint16)width;
    rectangle[3] = (sint16)remaining;
    return t3_8002943c(rectangle);
}

uint32 t3_bns_05_800e1f24(uint32 selection)
{
    FUNCTION_MARKER(0x800E1F24, "SLUS_004.02-BNS-logical-05");
    return t3_bns_05_800e1048(selection);
}

uint32 t3_bns_05_800e1f44(void)
{
    FUNCTION_MARKER(0x800E1F44, "SLUS_004.02-BNS-logical-05");
    return t3_bns_05_800e1374();
}

uint32 t3_bns_05_800db12c(void)
{
    FUNCTION_MARKER(0x800DB12C, "SLUS_004.02-BNS-logical-05");
    t3_80029638(1);
    t3_8004ca40(0x800E2A84, 0x800EBFA4);
    t3_8004ca40(0x800E2A84, 0x800BA088);
    if (r_u8(0x80097F0B))
        t3_8004c6fc(0x800E2A84, 0x800DA430);
    return t3_80029638(0);
}

uint32 t3_bns_05_800db1b8(void)
{
    uint32 phase;
    uint32 value;
    uint32 next;
    uint32 ordering;
    uint32 cursor;
    uint32 result;
    uint32 draw_mode;
    uint32 buffer;
    uint32 x;
    uint32 y;

    FUNCTION_MARKER(0x800DB1B8, "SLUS_004.02-BNS-logical-05");
    phase = (uint32)r_s16(0x800AE224);
    switch (phase)
    {
    case 0:
        w_u16(0x800ADE14, 258);
        x = r_u8(0x80097F24);
        w_u16(0x800ADE16, 240);
        y = r_u8(0x80097F25);
        w_u16(0x800ADE10, (uint32)(sint32)(sint8)x);
        w_u16(0x800ADE12, (uint32)(sint32)(sint8)y);
        t3_800295b8();
        t3_bns_05_800db0a4();
        w_u32(0x800EBFA8, 0);
        value = r_u8(0x80097F3E);
        if (value == 2)
        {
            w_u32(0x800B04F0, 1);
            w_u16(0x800AE224, 1);
        }
        else if (value == 3)
            w_u16(0x800AE224, 3);
        else
        {
            w_u32(0x800B04F0, 0);
            w_u16(0x800AE224, 1);
        }
        w_u32(0x800EACC4, 8);
        w_u32(0x800AE214, 0);
        break;
    case 1:
        value = r_u32(0x800B04F0);
        w_u32(0x800EACC4, 1);
        t3_bns_05_800e1f24(value & 1);
        w_u16(0x800AE224, 2);
        break;
    case 2:
        do
        {
            result = t3_bns_05_800e1f44();
            w_u32(0x800EBFA8, result);
            if ((sint32)result > 0)
                w_u16(0x800AE224, 6);
        } while (t3_8006c558());
        break;
    case 3:
        w_u32(0x800EACC4, 0);
        w_u32(0x800AE214, 0);
        w_u16(0x800AE224, 4);
        /* Fall through */
    case 4:
        value = r_u32(0x800AE214);
        next = value + 10;
        w_u32(0x800AE214, next);
        if ((sint32)next < 256)
        {
            ordering = r_u32(0x800A9218);
            cursor = r_u32(0x800ADF64);
            result = t3_8004e000(ordering + 12, cursor, (sint32)(value + 266));
            w_u32(0x800ADF64, result);
            break;
        }
        w_u32(0x800AE214, 0);
        w_u16(0x800AE224, 5);
        /* Fall through */
    case 5:
        ordering = r_u32(0x800A9218);
        cursor = r_u32(0x800ADF64);
        result = t3_8004e000(ordering + 12, cursor, 512);
        value = r_u32(0x800AE214);
        w_u32(0x800ADF64, result);
        next = value + 1;
        w_u32(0x800AE214, next);
        if ((sint32)next >= 11)
            w_u16(0x800AE224, 6);
        break;
    case 6:
        w_u32(0x800EACC4, 5);
        w_u32(0x800AE214, 256);
        w_u16(0x800AE224, 7);
        /* Fall through */
    case 7:
        value = r_u32(0x800AE214);
        if ((sint32)value > 0)
        {
            cursor = r_u32(0x800ADF64);
            ordering = r_u32(0x800A9218);
            w_u32(0x800AE214, value - 3);
            result = t3_8004e000(ordering + 12, cursor, (sint32)(value + 253));
            w_u32(0x800ADF64, result);
        }
        else
        {
            w_u32(0x800AE214, 300);
            w_u16(0x800AE224, 8);
        }
        break;
    case 8:
        value = r_u32(0x800AE214);
        w_u32(0x800EACC4, 2);
        next = value - 1;
        w_u32(0x800AE214, next);
        if (next == 0)
        {
            w_u32(0x800AE214, 256);
            w_u16(0x800AE224, 9);
        }
        break;
    case 9:
        value = r_u32(0x800AE214);
        if ((sint32)value > 0)
        {
            cursor = r_u32(0x800ADF64);
            ordering = r_u32(0x800A9218);
            next = value - 8;
            w_u32(0x800AE214, next);
            result = t3_8004e000(ordering + 28, cursor, (sint32)next);
            w_u32(0x800ADF64, result);
        }
        else
        {
            w_u32(0x800EACC4, 8);
            w_u16(0x800AE224, 10);
        }
        break;
    default:
        t3_8004ca40(0x800E2A84, 0x800D499C);
        t3_8004f870();
        w_u32(0x800EACC4, 8);
        t3_bns_05_800da910(6, 0);
        break;
    }

    if (t3_80050f38() || r_u32(0x800EBFA8) == 0xFFFFFFFF)
    {
        t3_8004f870();
        t3_8004b61c(1);
        t3_800753bc(0, 19648, 0);
        t3_80028f5c();
        t3_8004f8f8(4);
        return 1;
    }
    draw_mode = r_u32(0x800EACC4);
    buffer = r_u32(0x800ADEFC);
    cursor = r_u32(0x800ADF64);
    w_u32(0x800ADF64, (((buffer << 4) - buffer) << 10) + 0x8015CDA0);
    result = t3_bns_05_800da4bc(draw_mode);
    w_u32(0x800ADF64, cursor);
    return result;
}
