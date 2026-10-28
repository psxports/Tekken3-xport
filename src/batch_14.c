#include "game_functions.h"

void t3_80036cac(uint32 geometry, uint32 output, uint32 chain, uint32 phase)
{
    uint32 stream;
    uint32 count;
    uint32 packed;
    uint32 index;
    uint32 pass;
    uint32 stage;
    uint32 half_entry;
    uint32 depth_output = 0x1F8001B8;
    uint32 screen_output = 0x1F800000;
    uint32 source_depth;
    uint32 source_screen;
    uint32 depth;
    uint32 screen;
    uint32 extra_depth;
    uint32 extra_screen;
    uint32 sum;
    uint32 first_xy;
    uint32 first_z;
    uint32 second_xy;
    uint32 second_z;
    uint32 third_xy;
    uint32 third_z;
    uint32 first_depth;
    uint32 second_depth;
    uint32 third_depth;
    uint32 color;
    uint32 color_base = 0x1F8001B6;
    uint32 shape;
    uint32 polygon;
    uint32 flags;
    uint32 first_index;
    uint32 second_index;
    uint32 third_index;
    uint32 clip_index;
    uint32 ordering_entry;
    uint32 ordering_tag;
    uint32 packet_link;
    uint32 fourth_screen;
    sint32 remaining;
    sint32 clip;
    sint32 bias;
    FUNCTION_MARKER(0x80036CAC, "SLUS_004.02");
    stream = r_u32(geometry);
    if (!stream)
        return;
    depth = r_u32(stream);
    stream += 4;
    for (pass = 0; pass < 2; ++pass) {
        count = r_u32(stream);
        stream += 4;
        packed = 0;
        while (count) {
            if (count == 0x80000000) {
                xport_mips_overflow_exception(0x80036D18);
                return;
            }
            --count;
            if (!packed) {
                packed = r_u32(stream);
                stream += 4;
            }
            index = packed & 255;
            packed >>= 8;
            source_depth = (pass ? 0x8009CD4E : 0x1F8001B6) + index;
            source_screen = (pass ? 0x8009BD4C : 0x1F7FFFFC) + index * 2;
            depth = r_s16(source_depth);
            screen = r_u32(source_screen);
            w_u16(depth_output, depth);
            w_u32(screen_output, screen);
            if ((sint32)depth_output > 0x7FFFFFFD) {
                xport_mips_overflow_exception(0x80036D60);
                return;
            }
            depth_output += 2;
            if ((sint32)screen_output > 0x7FFFFFFB) {
                xport_mips_overflow_exception(0x80036D68);
                return;
            }
            screen_output += 4;
        }
    }
    count = r_u32(stream);
    if ((sint32)stream > 0x7FFFFFFB) {
        xport_mips_overflow_exception(0x80036D7C);
        return;
    }
    stream += 4;
    if (!count)
        return;
    if ((sint32)count < -2147483645) {
        xport_mips_overflow_exception(0x80036D88);
        return;
    }
    remaining = (sint32)(count - 3);
    if (remaining >= 0) {
        xport_gte_write_data(0, r_u32(stream));
        xport_gte_write_data(1, r_u32(stream + 4));
        xport_gte_write_data(2, r_u32(stream + 8));
        xport_gte_write_data(3, r_u32(stream + 12));
        xport_gte_write_data(4, r_u32(stream + 16));
        xport_gte_write_data(5, r_u32(stream + 20));
        if ((sint32)stream > 0x7FFFFFE7) {
            xport_mips_overflow_exception(0x80036DAC);
            return;
        }
        stream += 24;
        xport_gte_execute(0x4A280030);
        remaining -= 3;
        for (;;) {
            if ((sint32)screen_output > 0x7FFFFFF3) {
                xport_mips_overflow_exception(0x80036E28);
                return;
            }
            screen_output += 12;
            if ((sint32)depth_output > 0x7FFFFFF9) {
                xport_mips_overflow_exception(0x80036E2C);
                return;
            }
            depth_output += 6;
            w_u32(screen_output - 12, xport_gte_read_data(12));
            if (remaining < 0)
                break;
            first_xy = r_u32(stream);
            first_z = r_u32(stream + 4);
            second_xy = r_u32(stream + 8);
            second_z = r_u32(stream + 12);
            third_xy = r_u32(stream + 16);
            third_z = r_u32(stream + 20);
            w_u32(screen_output - 12, xport_gte_read_data(12));
            w_u32(screen_output - 8, xport_gte_read_data(13));
            w_u32(screen_output - 4, xport_gte_read_data(14));
            xport_gte_write_data(0, first_xy);
            xport_gte_write_data(1, first_z);
            xport_gte_write_data(2, second_xy);
            xport_gte_write_data(3, second_z);
            xport_gte_write_data(4, third_xy);
            xport_gte_write_data(5, third_z);
            first_depth = xport_gte_read_data(17);
            second_depth = xport_gte_read_data(18);
            third_depth = xport_gte_read_data(19);
            xport_gte_execute(0x4A280030);
            w_u16(depth_output - 6, (sint32)first_depth >> 5);
            w_u16(depth_output - 4, (sint32)second_depth >> 5);
            w_u16(depth_output - 2, (sint32)third_depth >> 5);
            if ((sint32)stream > 0x7FFFFFE7) {
                xport_mips_overflow_exception(0x80036E20);
                return;
            }
            stream += 24;
            remaining -= 3;
        }
        w_u32(screen_output - 8, xport_gte_read_data(13));
        w_u32(screen_output - 4, xport_gte_read_data(14));
        first_depth = xport_gte_read_data(17);
        second_depth = xport_gte_read_data(18);
        third_depth = xport_gte_read_data(19);
        w_u16(depth_output - 6, (sint32)first_depth >> 5);
        w_u16(depth_output - 4, (sint32)second_depth >> 5);
        w_u16(depth_output - 2, (sint32)third_depth >> 5);
    }
    remaining += 2;
    if (remaining >= 0) {
        xport_gte_write_data(0, r_u32(stream));
        xport_gte_write_data(1, r_u32(stream + 4));
        if (remaining) {
            xport_gte_write_data(2, r_u32(stream + 8));
            xport_gte_write_data(3, r_u32(stream + 12));
            if ((sint32)stream > 0x7FFFFFEF) {
                xport_mips_overflow_exception(0x80036E88);
                return;
            }
            stream += 16;
            xport_gte_execute(0x4A280030);
            if ((sint32)screen_output > 0x7FFFFFF7) {
                xport_mips_overflow_exception(0x80036E90);
                return;
            }
            screen_output += 8;
            if ((sint32)depth_output > 0x7FFFFFFB) {
                xport_mips_overflow_exception(0x80036E94);
                return;
            }
            depth_output += 4;
            w_u32(screen_output - 8, xport_gte_read_data(12));
            w_u32(screen_output - 4, xport_gte_read_data(13));
            first_depth = xport_gte_read_data(17);
            second_depth = xport_gte_read_data(18);
            w_u16(depth_output - 4, (sint32)first_depth >> 5);
            w_u16(depth_output - 2, (sint32)second_depth >> 5);
        } else {
            if ((sint32)stream > 0x7FFFFFF7) {
                xport_mips_overflow_exception(0x80036EC0);
                return;
            }
            stream += 8;
            xport_gte_execute(0x4A180001);
            if ((sint32)screen_output > 0x7FFFFFFB) {
                xport_mips_overflow_exception(0x80036EC8);
                return;
            }
            screen_output += 4;
            if ((sint32)depth_output > 0x7FFFFFFD) {
                xport_mips_overflow_exception(0x80036ECC);
                return;
            }
            depth_output += 2;
            w_u32(screen_output - 4, xport_gte_read_data(14));
            w_u16(depth_output - 2, (sint32)xport_gte_read_data(19) >> 5);
        }
    }
    source_depth = r_u32(geometry);
    depth = r_u32(source_depth);
    depth_output = 0x1F8001B6 + depth;
    if ((~(0x1F8001B6 ^ depth) & (0x1F8001B6 ^ depth_output)) & 0x80000000) {
        xport_mips_overflow_exception(0x80036F00);
        return;
    }
    extra_screen = depth * 2;
    screen_output = 0x1F7FFFFC + extra_screen;
    if ((~(0x1F7FFFFC ^ extra_screen) & (0x1F7FFFFC ^ screen_output)) & 0x80000000) {
        xport_mips_overflow_exception(0x80036F04);
        return;
    }
    for (stage = 0; stage < 6; ++stage) {
        count = r_u32(stream);
        stream += 4;
        packed = 0;
        while (count) {
            if (stage >= 1 && stage <= 4) {
                if (count == 0x80000000) {
                    xport_mips_overflow_exception(stage <= 2 ? 0x80036FE8 : 0x80037084);
                    return;
                }
                --count;
            }
            if (!packed) {
                packed = r_u32(stream);
                stream += 4;
            }
            index = packed & 255;
            half_entry = packed & 0x100;
            if (stage == 5) {
                if (count == 0x80000000) {
                    xport_mips_overflow_exception(0x8003712C);
                    return;
                }
                --count;
                if ((packed & 3) != 1) {
                    depth = r_s16(depth_output);
                    screen = r_u32(screen_output);
                    depth = (sint32)depth >> 1;
                    screen = ((sint32)screen >> 1) & 0xFFFF7FFF;
                    w_u16(depth_output, depth);
                    w_u32(screen_output, screen);
                }
            } else {
                pass = stage == 0 || stage == 1 || stage == 3;
                source_depth = (pass ? 0x8009CD4E : 0x1F8001B6) + index;
                source_screen = (pass ? 0x8009BD4C : 0x1F7FFFFC) + index * 2;
                depth = r_s16(depth_output);
                if (stage <= 2) {
                    if (stage == 0) {
                        screen = r_u32(screen_output);
                        if (packed & 0x100) {
                            depth = (sint32)depth >> 1;
                            screen = ((sint32)screen >> 1) & 0xFFFF7FFF;
                        }
                        extra_depth = r_s16(source_depth);
                        extra_screen = r_u32(source_screen);
                    } else {
                        extra_depth = r_s16(source_depth);
                        extra_screen = r_u32(source_screen);
                        screen = r_u32(screen_output);
                        if (packed & 0x100) {
                            depth = (sint32)depth >> 1;
                            screen = ((sint32)screen >> 1) & 0xFFFF7FFF;
                        }
                    }
                    depth += extra_depth;
                    sum = screen + extra_screen;
                    if ((~(screen ^ extra_screen) & (screen ^ sum)) & 0x80000000) {
                        xport_mips_overflow_exception(stage == 0 ? (packed & 0x100 ? 0x80036F6C : 0x80036FA8) : 0x80037048);
                        return;
                    }
                    if (stage == 0)
                        w_u16(source_depth, depth);
                    w_u16(depth_output, depth);
                    if (stage == 0)
                        w_u32(source_screen, sum);
                    w_u32(screen_output, sum);
                } else {
                    screen = r_u32(screen_output);
                    if (packed & 0x100) {
                        depth = (sint32)depth >> 1;
                        screen = ((sint32)screen >> 1) & 0xFFFF7FFF;
                        w_u16(depth_output, depth);
                        w_u32(screen_output, screen);
                    }
                    w_u16(source_depth, depth);
                    w_u32(source_screen, screen);
                }
            }
            if ((sint32)depth_output > 0x7FFFFFFD) {
                xport_mips_overflow_exception(stage == 0 ? (half_entry ? 0x80036F80 : 0x80036FBC) : stage <= 2 ? 0x80037054 : stage <= 4 ? 0x800370E8 : 0x80037154);
                return;
            }
            depth_output += 2;
            if ((sint32)screen_output > 0x7FFFFFFB) {
                xport_mips_overflow_exception(stage == 0 ? (half_entry ? 0x80036F84 : 0x80036FC0) : stage <= 2 ? 0x80037058 : stage <= 4 ? 0x800370EC : 0x80037158);
                return;
            }
            screen_output += 4;
            if (stage == 0) {
                if (count == 0x80000000) {
                    xport_mips_overflow_exception(half_entry ? 0x80036F88 : 0x80036FC4);
                    return;
                }
                --count;
            }
            packed >>= stage == 5 ? 2 : 9;
        }
    }
    if (phase) {
        color_base = 0x1F800290;
        stream = r_u32(geometry + 8) + 4;
        screen_output = 0x1F800294;
        for (pass = 0; pass < 2; ++pass) {
            count = r_u32(stream);
            if ((sint32)stream > 0x7FFFFFFB) {
                xport_mips_overflow_exception(0x80037194);
                return;
            }
            stream += 4;
            packed = 0;
            while (count) {
                if (!packed) {
                    packed = r_u32(stream);
                    if ((sint32)stream > 0x7FFFFFFB) {
                        xport_mips_overflow_exception(0x800371B0);
                        return;
                    }
                    stream += 4;
                }
                index = (packed & 255) * 2;
                color = r_u32((pass ? 0x8009D550 : 0x1F800290) + index);
                if (count == 0x80000000) {
                    xport_mips_overflow_exception(0x800371D4);
                    return;
                }
                --count;
                w_u32(screen_output, color);
                if ((sint32)screen_output > 0x7FFFFFFB) {
                    xport_mips_overflow_exception(0x800371DC);
                    return;
                }
                screen_output += 4;
                packed >>= 8;
            }
        }
        count = r_u32(stream);
        if ((sint32)stream > 0x7FFFFFFB) {
            xport_mips_overflow_exception(0x800371F4);
            return;
        }
        stream += 4;
        if ((sint32)count < -2147483645) {
            xport_mips_overflow_exception(0x800371F8);
            return;
        }
        remaining = (sint32)(count - 3);
        color = r_u32(0x800ADE1C);
        if (remaining >= 0) {
            xport_gte_write_data(0, r_u32(stream));
            xport_gte_write_data(1, r_u32(stream + 4));
            xport_gte_write_data(2, r_u32(stream + 8));
            xport_gte_write_data(3, r_u32(stream + 12));
            xport_gte_write_data(4, r_u32(stream + 16));
            xport_gte_write_data(5, r_u32(stream + 20));
            for (;;) {
                xport_gte_write_data(6, color);
                remaining -= 3;
                xport_gte_execute(0x4B18043F);
                if ((sint32)stream > 0x7FFFFFE7) {
                    xport_mips_overflow_exception(0x80037274);
                    return;
                }
                stream += 24;
                if ((sint32)screen_output > 0x7FFFFFF3) {
                    xport_mips_overflow_exception(0x8003727C);
                    return;
                }
                screen_output += 12;
                if (remaining < 0)
                    break;
                first_xy = r_u32(stream);
                first_z = r_u32(stream + 4);
                second_xy = r_u32(stream + 8);
                second_z = r_u32(stream + 12);
                third_xy = r_u32(stream + 16);
                third_z = r_u32(stream + 20);
                w_u32(screen_output - 12, xport_gte_read_data(20));
                w_u32(screen_output - 8, xport_gte_read_data(21));
                w_u32(screen_output - 4, xport_gte_read_data(22));
                xport_gte_write_data(0, first_xy);
                xport_gte_write_data(1, first_z);
                xport_gte_write_data(2, second_xy);
                xport_gte_write_data(3, second_z);
                xport_gte_write_data(4, third_xy);
                xport_gte_write_data(5, third_z);
            }
            w_u32(screen_output - 12, xport_gte_read_data(20));
            w_u32(screen_output - 8, xport_gte_read_data(21));
            w_u32(screen_output - 4, xport_gte_read_data(22));
        }
        remaining += 2;
        if (remaining >= 0) {
            xport_gte_write_data(0, r_u32(stream));
            xport_gte_write_data(1, r_u32(stream + 4));
            xport_gte_write_data(6, color);
            if (remaining) {
                if ((sint32)stream > 0x7FFFFFEF) {
                    xport_mips_overflow_exception(0x800372AC);
                    return;
                }
                stream += 16;
                xport_gte_execute(0x4B08041B);
                first_xy = r_u32(stream - 8);
                first_z = r_u32(stream - 4);
                xport_gte_write_data(0, first_xy);
                xport_gte_write_data(1, first_z);
                xport_gte_write_data(6, color);
                sum = xport_gte_read_data(22);
                if ((sint32)screen_output > 0x7FFFFFF7) {
                    xport_mips_overflow_exception(0x800372CC);
                    return;
                }
                screen_output += 8;
                xport_gte_execute(0x4B08041B);
                w_u32(screen_output - 8, sum);
                w_u32(screen_output - 4, xport_gte_read_data(22));
            } else {
                if ((sint32)stream > 0x7FFFFFF7) {
                    xport_mips_overflow_exception(0x800372E0);
                    return;
                }
                stream += 8;
                xport_gte_execute(0x4B08041B);
                if ((sint32)screen_output > 0x7FFFFFFB) {
                    xport_mips_overflow_exception(0x800372E8);
                    return;
                }
                screen_output += 4;
                w_u32(screen_output - 4, xport_gte_read_data(22));
            }
        }
        source_screen = r_u32(geometry + 8);
        index = r_u32(source_screen);
        screen_output = color_base + index;
        if ((~(color_base ^ index) & (color_base ^ screen_output)) & 0x80000000) {
            xport_mips_overflow_exception(0x80037300);
            return;
        }
        for (pass = 0; pass < 2; ++pass) {
            count = r_u32(stream);
            if ((sint32)stream > 0x7FFFFFFB) {
                xport_mips_overflow_exception(0x8003730C);
                return;
            }
            stream += 4;
            packed = 0;
            while (count) {
                if (!packed) {
                    packed = r_u32(stream);
                    if ((sint32)stream > 0x7FFFFFFB) {
                        xport_mips_overflow_exception(0x80037328);
                        return;
                    }
                    stream += 4;
                }
                index = (packed & 255) * 2;
                packed >>= 8;
                source_screen = (pass ? 0x1F800290 : 0x8009D550) + index;
                color = r_u32(screen_output);
                if (count == 0x80000000) {
                    xport_mips_overflow_exception(0x80037350);
                    return;
                }
                --count;
                w_u32(source_screen, color);
                if ((sint32)screen_output > 0x7FFFFFFB) {
                    xport_mips_overflow_exception(0x8003735C);
                    return;
                }
                screen_output += 4;
            }
        }
    }
    stream = r_u32(geometry + 12);
    for (shape = 0; shape < 4; ++shape) {
        count = r_u32(stream);
        if ((sint32)stream > 0x7FFFFFFB) {
            xport_mips_overflow_exception(shape == 0 ? 0x80037388 : shape == 1 ? 0x80037488 : shape == 2 ? 0x80037594 : 0x800376B8);
            return;
        }
        stream += 4;
        while (count) {
            polygon = r_u32(stream);
            first_index = polygon & 0x1FC;
            second_index = (polygon >> 7) & 0x1FC;
            third_index = (polygon >> 14) & 0x1FC;
            clip_index = shape & 1 ? polygon >> 23 : third_index;
            xport_gte_write_data(12, r_u32(0x1F800000 + first_index));
            xport_gte_write_data(13, r_u32(0x1F800000 + second_index));
            xport_gte_write_data(14, r_u32(0x1F800000 + clip_index));
            xport_gte_execute(0x4B400006);
            if (shape & 1) {
                flags = r_u32(stream + 4);
                if (shape == 1)
                    first_depth = r_u16(0x1F8001B8 + first_index / 2);
            } else {
                first_depth = r_u16(0x1F8001B8 + first_index / 2);
                second_depth = r_u16(0x1F8001B8 + third_index / 2);
                flags = polygon;
            }
            --count;
            clip = (sint32)xport_gte_read_data(24);
            bias = (sint32)flags >> 25;
            if ((flags & 0x1000000) && clip <= 0)
                bias = -bias;
            if (shape == 1)
                second_depth = r_u16(0x1F8001B8 + clip_index / 2);
            if ((flags & 0x1000000) || clip > 0) {
                if (shape == 3) {
                    first_depth = r_u16(0x1F8001B8 + first_index / 2);
                    second_depth = r_u16(0x1F8001B8 + clip_index / 2);
                }
                sum = first_depth + second_depth - (uint32)bias;
                ordering_entry = chain + ((sum * 2) & 0xFFC);
                if (shape & 1)
                    fourth_screen = r_u32(0x1F800000 + third_index);
                ordering_tag = r_u32(ordering_entry);
                packet_link = output & 0xFFFFFF;
                if (phase) {
                    if (shape == 0) {
                        index = r_u32(stream + 4);
                        source_screen = color_base + index;
                        if ((~(color_base ^ index) & (color_base ^ source_screen)) & 0x80000000) {
                            xport_mips_overflow_exception(0x80037448);
                            return;
                        }
                        color = r_u32(source_screen);
                        w_u32(output + 4, color | 0x24000000);
                    } else if (shape == 1) {
                        color = r_u32(color_base + (flags & 0x1FC));
                        sum = color + 0x2C000000;
                        if ((~(color ^ 0x2C000000) & (color ^ sum)) & 0x80000000) {
                            xport_mips_overflow_exception(0x8003755C);
                            return;
                        }
                        w_u32(output + 4, sum);
                    } else if (shape == 2) {
                        packed = r_u32(stream + 4);
                        color = r_u32(color_base + (packed & 0x1FC));
                        first_xy = r_u32(color_base + ((packed >> 7) & 0x1FC));
                        second_xy = r_u32(color_base + (packed >> 16));
                        sum = color + 0x34000000;
                        if ((~(color ^ 0x34000000) & (color ^ sum)) & 0x80000000) {
                            xport_mips_overflow_exception(0x8003767C);
                            return;
                        }
                        w_u32(output + 4, sum);
                        w_u32(output + 16, first_xy);
                        w_u32(output + 28, second_xy);
                    } else {
                        packed = r_u32(stream + 8);
                        color = r_u32(color_base + (flags & 0x1FC));
                        first_xy = r_u32(color_base + (packed & 0x1FC));
                        second_xy = r_u32(color_base + ((packed >> 7) & 0x1FC));
                        third_xy = r_u32(color_base + (packed >> 16));
                        sum = color + 0x3C000000;
                        if ((~(color ^ 0x3C000000) & (color ^ sum)) & 0x80000000) {
                            xport_mips_overflow_exception(0x800377B4);
                            return;
                        }
                        w_u32(output + 4, sum);
                        w_u32(output + 16, first_xy);
                        w_u32(output + 28, second_xy);
                        w_u32(output + 40, third_xy);
                    }
                }
                if (shape == 0) {
                    w_u32(output + 8, xport_gte_read_data(12));
                    w_u32(output + 16, xport_gte_read_data(13));
                    w_u32(output + 24, xport_gte_read_data(14));
                } else if (shape == 1) {
                    w_u32(output + 8, xport_gte_read_data(12));
                    w_u32(output + 16, xport_gte_read_data(13));
                    w_u32(output + 24, fourth_screen);
                    w_u32(output + 32, xport_gte_read_data(14));
                } else if (shape == 2) {
                    w_u32(output + 8, xport_gte_read_data(12));
                    w_u32(output + 20, xport_gte_read_data(13));
                    w_u32(output + 32, xport_gte_read_data(14));
                } else {
                    w_u32(output + 8, xport_gte_read_data(12));
                    w_u32(output + 20, xport_gte_read_data(13));
                    w_u32(output + 32, fourth_screen);
                    w_u32(output + 44, xport_gte_read_data(14));
                }
                w_u32(output, (ordering_tag & 0xFFFFFF) | (shape == 0 ? 0x07000000 : shape == 3 ? 0x0C000000 : 0x09000000));
                w_u32(ordering_entry, packet_link);
            }
            index = shape == 3 ? 12 : 8;
            if ((sint32)stream > 0x7FFFFFFF - (sint32)index) {
                xport_mips_overflow_exception(shape == 0 ? 0x80037478 : shape == 1 ? 0x80037584 : shape == 2 ? 0x800376A8 : 0x800377EC);
                return;
            }
            stream += index;
            index = shape == 0 ? 32 : shape == 3 ? 52 : 40;
            if ((shape == 1 || shape == 3) && (sint32)output > 0x7FFFFFFF - (sint32)index) {
                xport_mips_overflow_exception(shape == 1 ? 0x8003758C : 0x800377F4);
                return;
            }
            output += index;
        }
    }
}

uint32 t3_800b0788(void)
{
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x800B0788, "SLUS_004.02");
    SetGraphDebug(0);
    SetDispMask(0);
    ResetGraph(0);
    rectangle.w = 384;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.h = 480;
    return ClearImage(&rectangle, 0, 0, 0);
}

uint32 t3_800b0708(void)
{
    sint32 state;
    FUNCTION_MARKER(0x800B0708, "SLUS_004.02");
    state = r_s16(0x800AE224);
    if (!state) {
        t3_8004ca40(0x8012867C, 0x800B8D58);
        w_u16(0x800AE224, 1);
        return 1;
    }
    if (state == 1) {
        SetDispMask(1);
        w_u8(0x800AFA9F, 255);
        return t3_8004f8f8(3);
    }
    return 1;
}

uint32 t3_8002f3ac(uint32 actor, uint32 opponent)
{
    uint32 animation;
    uint32 other_animation;
    uint32 next;
    uint32 flags;
    uint32 frame;
    uint32 alternate_frame;
    uint32 value;
    uint32 extra;
    uint32 angle;
    uint32 other_angle;
    uint32 x;
    uint32 z;
    uint32 table;
    uint32 script;
    uint32 snap_position;
    sint32 direction;
    sint32 count;
    sint32 state;
    sint32 kind;
    sint32 delta;
    sint32 divisor;
    int transition = 0;
    FUNCTION_MARKER(0x8002F3AC, "SLUS_004.02");
    value = r_u8(actor + 0xC3);
    if (!value)
        return 0;
    value = r_u16(actor + 0x128) - 1;
    w_u16(actor + 0x128, value);
    if ((sint16)value < 0)
        w_u16(actor + 0x128, 0);
    next = r_u32(actor + 0x1A4);
    if (next) {
        direction = r_s16(actor + 0x6E);
        if (direction > 0) {
            state = r_s16(actor + 0x58);
            count = r_s16(actor + 0x198);
            if (state >= count) {
                transition = 1;
            } else {
                animation = r_u32(actor + 0x54);
                transition = state >= (sint32)r_u8(animation + 0x18);
            }
        } else if (!direction) {
            transition = 1;
        } else {
            state = r_s16(actor + 0x58);
            count = r_s16(actor + 0x198);
            transition = state <= count || state <= 0;
        }
    }
    if (!transition) {
        value = r_u16(actor + 0x5C);
        extra = r_u16(actor + 0x76);
        w_u16(actor + 0x5C, value + 1);
        count = r_s16(actor + 0x76);
        w_u8(actor + 0xDA, 0);
        if (count > 0) {
            value = extra - 1;
            w_u16(actor + 0x76, value);
            value <<= 16;
            if (value)
                return value;
            frame = r_u16(actor + 0x58);
            animation = r_u32(actor + 0x54);
            w_u16(actor + 0x76, 0);
            value = frame + 1;
            w_u16(actor + 0x58, value);
            w_u16(actor + 0x50, value);
            w_u32(actor + 0x4C, animation);
            return value;
        }
        count = r_s16(actor + 0x6A);
        extra = r_u16(actor + 0x6A);
        if (count >= 0) {
            animation = r_u32(actor + 0x54);
            state = r_s16(actor + 0x58);
            value = r_u8(animation + 0x1B);
            frame = r_u16(actor + 0x58);
            if (state != (sint32)value) {
                alternate_frame = r_u16(actor + 0x50);
                w_u16(actor + 0x58, frame + 1);
            } else if (count > 0) {
                alternate_frame = r_u16(actor + 0x50);
                w_u16(actor + 0x6A, extra - 1);
            } else {
                animation = r_u32(actor + 0x54);
                value = frame + 1;
                w_u16(actor + 0x58, value);
                w_u16(actor + 0x50, value);
                w_u32(actor + 0x4C, animation);
                return value;
            }
            value = alternate_frame + 1;
            w_u16(actor + 0x50, value);
            return value;
        }
        if (r_s16(actor + 0x92)) {
            animation = r_u32(actor + 0x54);
            state = r_s16(actor + 0x58);
            extra = r_u8(animation + 0x1B);
            frame = r_u16(actor + 0x58);
            value = frame + 1;
            if (state != (sint32)extra) {
                w_u16(actor + 0x58, value);
                w_u16(actor + 0x50, value);
            }
            return value;
        }
        frame = r_u16(actor + 0x58);
        extra = r_u16(actor + 0x6E);
        value = frame + extra;
        w_u16(actor + 0x58, value);
        if ((sint16)value <= 0)
            w_u16(actor + 0x58, 1);
        value = r_u16(actor + 0x58);
        w_u16(actor + 0x50, value);
        return value;
    }
    if (r_u8(actor + 0x8B) == 3)
        w_u16(actor + 0x198, 1);
    frame = r_u16(actor + 0x58);
    value = r_u16(actor + 0xA0);
    next = r_u32(actor + 0x1A4);
    w_u16(actor + 0x134, frame);
    w_u16(actor + 0x136, value);
    value = r_u16(next + 0x10);
    extra = r_u8(actor + 0x8B);
    w_u16(actor + 0x138, value);
    if (extra == 3)
        w_u16(actor + 0x198, 1);
    w_u8(actor + 0x86, 0);
    w_u8(actor + 0xB8, 0);
    w_u8(actor + 0x82, 0);
    w_u8(actor + 0xB9, 0);
    w_u16(actor + 0x6A, 0xFFFF);
    x = r_u32(opponent + 0xF68);
    w_u32(actor + 0xAC, x);
    z = r_u32(opponent + 0xF70);
    w_u16(actor + 0x6E, 1);
    state = r_s16(actor + 0xEA);
    w_u16(actor + 0x36, 0);
    w_u8(actor + 0xB6, 0);
    w_u8(actor + 0x8C, 0);
    w_u8(actor + 0xBC, 0);
    w_u8(actor + 0xBA, 0);
    w_u16(actor + 0x76, 0);
    w_u8(actor + 0x83, 0);
    w_u8(actor + 0x84, 0);
    w_u8(actor + 0x85, 0);
    w_u16(actor + 0x104, 0);
    w_u32(actor + 0xB0, z);
    value = r_u16(actor + 0xEA);
    /* Preserve class 1 when EA is retained */
    if (state && (r_u16(r_u32(actor + 0x1A4) + 0x14) >> 14) != 1) {
        w_u16(actor + 0x5E, value);
    } else {
        next = r_u32(actor + 0x1A4);
        value = r_u16(next + 0x14);
        w_u16(actor + 0x5E, value);
        w_u8(actor + 0xB7, value >> 14);
        w_u16(actor + 0x5E, value & 0x3FFF);
        if (r_u8(actor + 0xB7) == 1)
            w_u16(actor + 0x5E, 0);
    }
    next = r_u32(actor + 0x1A4);
    animation = r_u32(actor + 0x1A4);
    value = r_u32(next + 4);
    w_u32(actor + 0x60, value);
    value = r_u16(animation + 8);
    flags = r_u32(actor + 0x60);
    w_u16(actor + 0x64, value);
    value = r_u16(actor + 0x60);
    w_u16(actor + 0x66, value & 0x18);
    value = r_u16(actor + 0x64);
    w_u8(actor + 0x69, (flags >> 11) & 31);
    w_u8(actor + 0x68, value >> 8);
    if (r_u32(0x800AFA88) != 5)
        w_u8(actor + 0xCA, r_u8(actor + 0xC5) == 0);
    flags = r_u32(actor + 0x60);
    if ((flags & 0x100) && !r_u8(actor + 0xCA)) {
        value = r_u16(actor + 0x66);
        w_u32(actor + 0x60, flags & 0xFFFFFFE7);
        w_u16(actor + 0x66, value & 0xFFE7);
    }
    next = r_u32(actor + 0x1A4);
    if ((r_u32(next + 0x24) >> 8) & 0x20) {
        w_u8(actor + 0xB5, 1);
        t3_8002c0e8(actor);
    } else {
        w_u8(actor + 0xB5, 0);
        t3_8002c10c(actor);
    }
    t3_8002c130(actor);
    w_u8(actor + 0x89, r_u8(actor + 0xCF) != 0);
    if (!r_u8(actor + 0xCE)) {
        w_u8(actor + 0x87, 0);
        w_u16(actor + 0x92, 0);
        w_u16(actor + 0x94, 0);
    }
    if (!r_u8(actor + 0xD1))
        w_u8(actor + 0x88, 0);
    if (r_u8(actor + 0xD0))
        goto increment_transition_count;
    animation = r_u32(actor + 0x54);
    if ((r_u32(animation + 0x24) >> 8) & 0x400) {
        next = r_u32(actor + 0x1A4);
        if ((r_u32(next + 0x24) >> 8) & 0x400)
            goto increment_transition_count;
    }
    if (r_s16(actor + 0x74) < 0) {
        next = r_u32(actor + 0x1A4);
        if ((r_u32(next + 0x24) >> 8) & 0x400)
            goto increment_transition_count;
    }
    if (!r_u8(actor + 0x8A) && r_s16(actor + 0x74) >= 0)
        goto clear_transition_count;
    value = r_u16(actor + 0x19A);
    if (value - 26 < 2)
        goto transition_count_ready;
    if ((sint16)value == 34)
        goto increment_transition_count;
    state = r_s16(actor + 0x1A0);
    if (state == 2470)
        goto transition_count_ready;
    if (state == 3270)
        goto increment_transition_count;
clear_transition_count:
    w_u16(actor + 0x80, 0);
    goto transition_count_ready;
increment_transition_count:
    value = r_u16(actor + 0x80);
    w_u16(actor + 0x80, value + 1);
transition_count_ready:
    value = r_u8(actor + 0xD0);
    extra = r_u8(actor + 0xCE);
    w_u8(actor + 0x8A, value);
    if (extra && r_u32(actor + 0x3F4))
        w_u16(actor + 0x76, r_u16(actor + 0xEE));
    value = r_u8(actor + 0x8B);
    if (value == 3 || value == 1) {
        w_u8(actor + 0xDA, 1);
    } else {
        next = r_u32(actor + 0x1A4);
        animation = r_u32(actor + 0x54);
        value = r_u32(next);
        extra = r_u32(animation);
        if (value != extra)
            w_u8(actor + 0xDA, 1);
    }
    animation = r_u32(actor + 0x54);
    flags = r_u32(animation + 0x24);
    snap_position = (flags >> 28) & 1;
    if ((flags >> 8) & 0x100)
        w_u16(actor + 0x122, 120);
    next = r_u32(actor + 0x1A4);
    if ((r_u32(next + 0x24) >> 8) & 8)
        w_u16(actor + 0x11C, 9);
    if (r_s16(actor + 0x19C)) {
        angle = r_u16(actor + 0x2C);
        other_angle = r_u16(actor + 0x0E);
        w_u16(actor + 0x19C, 0);
        w_u16(actor + 0x2C, angle + 0x8000);
        w_u16(actor + 0x0E, other_angle + 0x8000);
    }
    if (r_u8(actor + 0xC1) && r_s16(actor + 0x19A) != 34) {
        animation = r_u32(actor + 0x54);
        angle = r_u16(actor + 0x2C);
        extra = r_u16(animation + 0x12);
        w_u16(actor + 0x2C, angle + extra);
        w_u16(actor + 0x0E, angle + extra);
    }
    t3_80043a94(actor, opponent);
    kind = r_s16(actor + 0x19A);
    switch (kind) {
    case 1:
        w_u16(actor + 0x0E, r_u16(actor + 0x2C));
        goto advance_frames;
    case 25:
        w_u16(actor + 0x74, 0);
        goto advance_frames;
    case 2: case 3: case 18: case 19: case 20: case 21: case 22: case 29:
advance_frames:
        if (t3_80031070(actor)) {
            frame = r_u16(actor + 0x58);
            alternate_frame = r_u16(actor + 0x50);
            w_u16(actor + 0x58, frame + 1);
            w_u16(actor + 0x50, alternate_frame + 1);
        } else {
            w_u16(actor + 0x58, 1);
            w_u16(actor + 0x50, 1);
        }
        next = r_u32(actor + 0x1A4);
        state = r_s16(actor + 0x58);
        value = r_u8(next + 0x18);
        if ((sint32)value < state) {
            value = r_u8(next + 0x18);
            w_u16(actor + 0x58, value);
            w_u16(actor + 0x50, value);
        }
        break;
    default:
        break;
    }
    switch (kind) {
    case 0:
        next = r_u32(actor + 0x1A4);
        angle = r_u16(actor + 0x2A);
        x = r_u32(actor + 0xF68);
        z = r_u32(actor + 0xF70);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        w_u16(actor + 0x74, 0);
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x2C, angle);
        w_u16(actor + 0x0E, angle);
        w_u32(actor, x);
        w_u32(actor + 8, z);
        goto finish_transition;
    case 1: case 2: case 3: case 22: case 25:
        next = r_u32(actor + 0x1A4);
        x = r_u32(actor + 0xF68);
        z = r_u32(actor + 0xF70);
        w_u8(actor + 0xB9, kind <= 3 ? 11 : 8);
        if (kind != 25)
            w_u16(actor + 0x74, 0);
        if (kind == 22) {
            w_u16(actor + 0x114, 0);
            w_u16(actor + 0x116, 0);
        }
        w_u8(actor + 0xBE, 1);
        w_u32(actor + 0x4C, next);
        w_u32(actor, x);
        w_u32(actor + 8, z);
        goto finish_transition;
    case 4: case 5:
        alternate_frame = r_u16(actor + 0x50);
        animation = r_u32(actor + 0x4C);
        w_u16(actor + 0x58, 1);
        value = alternate_frame + 1;
        w_u16(actor + 0x50, value);
        extra = r_u8(animation + 0x18);
        if ((sint32)extra < (sint16)value)
            w_u16(actor + 0x50, r_u8(animation + 0x18));
        extra = r_u16(actor + 0x6C);
        if (kind == 4) {
            angle = r_u16(actor + 0x2C);
            state = r_s16(actor + 0x30);
            w_u8(actor + 0xB9, 11);
            w_u16(actor + 0x6A, extra);
            w_u16(actor + 0x0E, angle);
            if (state < 0x4FA5) {
                next = r_u32(actor + 0x1A4);
                count = (sint32)r_u8(next + 0x2D) - 1;
                w_u16(actor + 0x114, count);
                if (count <= 0)
                    w_u16(actor + 0x114, 1);
                angle = r_u16(actor + 0x2A);
                other_angle = r_u16(actor + 0x2C);
                divisor = r_s16(actor + 0x114);
                delta = (sint16)(angle - other_angle);
                w_u16(actor + 0x116, delta / divisor);
            }
        } else {
            state = r_s16(actor + 0x30);
            angle = r_u16(actor + 0x2C);
            w_u16(actor + 0x6A, extra);
            w_u16(actor + 0x0E, angle);
            w_u8(actor + 0xB9, state < 0x4FA4 ? 4 : 6);
            w_u16(actor + 0x114, 0);
            w_u16(actor + 0x116, 0);
        }
        w_u16(actor + 0x74, 0);
        goto finish_transition;
    case 6: case 23:
        next = r_u32(actor + 0x1A4);
        angle = r_u16(actor + 0x2C);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x0E, angle);
        if (snap_position) {
            x = r_u32(actor + 0xF68);
            z = r_u32(actor + 0xF70);
            w_u32(actor, x);
            w_u32(actor + 8, z);
        }
        if (kind == 23)
            w_u8(actor + 0xB9, 9);
        w_u16(actor + 0x74, 0);
        goto finish_transition;
    case 7:
        next = r_u32(actor + 0x1A4);
        frame = r_u8(next + 0x18);
        angle = r_u16(actor + 0x2C);
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x0E, angle);
        w_u16(actor + 0x58, frame);
        w_u16(actor + 0x50, frame);
        if (snap_position) {
            x = r_u32(actor + 0xF68);
            z = r_u32(actor + 0xF70);
            w_u32(actor, x);
            w_u32(actor + 8, z);
        }
        w_u16(actor + 0x6E, 0xFFFF);
        w_u16(actor + 0x74, 0);
        goto finish_transition;
    case 8:
        value = r_u16(actor + 0x58) - 1;
        w_u16(actor + 0x58, value);
        if ((sint16)value <= 0)
            w_u16(actor + 0x58, 1);
        frame = r_u16(actor + 0x58);
        next = r_u32(actor + 0x1A4);
        angle = r_u16(actor + 0x2C);
        x = r_u32(actor + 0xF68);
        z = r_u32(actor + 0xF70);
        w_u16(actor + 0x6E, 0xFFFF);
        w_u8(actor + 0xBE, 1);
        w_u16(actor + 0x74, 0);
        w_u16(actor + 0x50, frame);
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x0E, angle);
        w_u32(actor, x);
        w_u32(actor + 8, z);
        goto finish_transition;
    case 9: case 15: case 24:
        next = r_u32(actor + 0x1A4);
        angle = r_u16(actor + 0x34);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        if (kind == 24) {
            w_u16(actor + 0x74, 0);
            w_u8(actor + 0xB9, 8);
        } else {
            w_u8(actor + 0xB9, kind == 9 ? 5 : 1);
            w_u16(actor + 0x74, 0);
            w_u16(actor + 0x114, 0);
            w_u16(actor + 0x116, 0);
        }
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x0E, angle);
        goto snap_and_finish;
    case 10: case 11:
        angle = r_u16(actor + (kind == 10 ? 0x2C : 0x34));
        w_u16(actor + 0x0E, angle);
        next = r_u32(actor + 0x1A4);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        w_u32(actor + 0x4C, next);
        if (r_u8(next + 0x2D)) {
            value = r_u8(next + 0x2D) - 1;
        } else {
            animation = r_u32(actor + 0x54);
            value = r_u8(animation + 0x18);
            value = value < 16 ? r_u8(animation + 0x18) : 16;
        }
        w_u16(actor + 0x114, value);
        if (r_s16(actor + 0x114) <= 0)
            w_u16(actor + 0x114, 1);
        value = r_u16(actor + 0x30);
        angle = r_u16(actor + 0x2A);
        w_u16(actor + 0x34, value < 0x4000 ? angle : angle + 0x8000);
        angle = r_u16(actor + 0x34);
        other_angle = r_u16(actor + 0x2C);
        divisor = r_s16(actor + 0x114);
        delta = (sint16)(angle - other_angle);
        w_u16(actor + 0x116, delta / divisor);
        if (snap_position) {
            x = r_u32(actor + 0xF68);
            z = r_u32(actor + 0xF70);
            w_u32(actor, x);
            w_u32(actor + 8, z);
        }
        w_u8(actor + 0xB9, 11);
        w_u16(actor + 0x74, 0);
        goto finish_transition;
    case 12: case 13: case 14:
        next = r_u32(actor + 0x1A4);
        if (kind != 12)
            other_angle = r_u16(actor + 0x34);
        state = r_s16(actor + 0x30);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        w_u32(actor + 0x4C, next);
        if (kind != 12)
            w_u16(actor + 0x0E, other_angle);
        angle = r_u16(actor + (state < 0x4FA4 ? 0x2A : 0x34));
        w_u8(actor + 0xB9, state < 0x4FA4 ? 16 - kind : 6);
        w_u16(actor + 0x0E, angle);
        w_u16(actor + 0x74, 0);
        w_u16(actor + 0x114, 0);
        w_u16(actor + 0x116, 0);
        goto snap_and_finish;
    case 16:
        next = r_u32(actor + 0x1A4);
        angle = r_u16(actor + 0x2A);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        w_u8(actor + 0xB9, 11);
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x0E, angle);
        count = (sint32)r_u8(next + 0x2D) - 1;
        w_u16(actor + 0x114, count);
        if (count <= 0)
            w_u16(actor + 0x114, 1);
        angle = r_u16(actor + 0x2A);
        other_angle = r_u16(actor + 0x2C);
        divisor = r_s16(actor + 0x114);
        delta = (sint16)(angle - other_angle);
        value = delta / divisor;
        w_u16(actor + 0x74, 0);
        w_u8(actor + 0x82, 1);
        w_u32(actor + 0x78, 0);
        w_u32(actor + 0x7C, 0);
        w_u16(actor + 0x116, value);
        goto snap_and_finish;
    case 17:
        goto finish_transition;
    case 18: case 19:
        next = r_u32(actor + 0x1A4);
        angle = r_u16(actor + 0x2A);
        w_u8(actor + 0x82, 1);
        if (kind == 19)
            w_u8(actor + 0xB9, 11);
        if (kind == 19)
            other_angle = r_u16(actor + 0x2C);
        w_u16(actor + 0x74, 0);
        w_u32(actor + 0x78, 0);
        w_u32(actor + 0x7C, 0);
        if (kind == 18)
            w_u8(actor + 0xB9, 11);
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x0E, angle);
        if (kind == 19) {
            next = r_u32(actor + 0x1A4);
            w_u16(actor + 0x2C, other_angle + 0x8000);
        }
        count = (sint32)r_u8(next + 0x2D) - 1;
        w_u16(actor + 0x114, count);
        if (count <= 0)
            w_u16(actor + 0x114, 1);
        angle = r_u16(actor + 0x2A);
        other_angle = r_u16(actor + 0x2C);
        divisor = r_s16(actor + 0x114);
        delta = (sint16)(angle - other_angle);
        w_u16(actor + 0x116, delta / divisor);
        goto snap_and_finish;
    case 20: case 21:
        state = r_s16(actor + 0x30);
        next = r_u32(actor + 0x1A4);
        w_u32(actor + 0x4C, next);
        w_u8(actor + 0xB9, state < 0x4FA4 ? (kind == 20 ? 4 : 2) : 6);
        x = r_u32(actor + 0xF68);
        z = r_u32(actor + 0xF70);
        w_u16(actor + 0x74, 0);
        w_u16(actor + 0x114, 0);
        w_u16(actor + 0x116, 0);
        w_u8(actor + 0xBE, 1);
        w_u32(actor, x);
        w_u32(actor + 8, z);
        goto finish_transition;
    case 26: case 27:
        frame = r_u16(actor + 0x58);
        alternate_frame = r_u16(actor + 0x50);
        next = r_u32(actor + 0x1A4);
        w_u16(actor + 0x58, frame + 1);
        w_u16(actor + 0x50, alternate_frame + 1);
        state = r_s16(actor + 0x58);
        value = r_u8(next + 0x18);
        if ((sint32)value < state) {
            value = r_u8(next + 0x18);
            w_u16(actor + 0x58, value);
            w_u16(actor + 0x50, value);
        }
        next = r_u32(actor + 0x1A4);
        angle = r_u16(actor + 0x2C);
        w_u32(actor + 0x4C, next);
        state = r_s16(actor + 0x19A);
        w_u16(actor + 0x0E, angle);
        w_u8(actor + 0xB6, state == 26 ? 1 : 2);
        goto finish_transition;
    case 28:
        angle = r_u16(actor + 0x2C);
        next = r_u32(actor + 0x1A4);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        w_u8(actor + 0xB9, 10);
        w_u16(actor + 0x74, 0);
        w_u16(actor + 0x114, 0);
        w_u16(actor + 0x116, 0);
        w_u16(actor + 0x0E, angle);
        w_u32(actor + 0x4C, next);
        if (snap_position) {
            x = r_u32(actor + 0xF68);
            z = r_u32(actor + 0xF70);
            w_u32(actor, x);
            w_u32(actor + 8, z);
        }
        w_u16(actor + 0x128, 24);
        goto finish_transition;
    case 29:
        angle = r_u16(actor + 0x2C);
        next = r_u32(actor + 0x1A4);
        w_u8(actor + 0xB9, 10);
        w_u16(actor + 0x74, 0);
        w_u16(actor + 0x114, 0);
        w_u16(actor + 0x116, 0);
        w_u16(actor + 0x0E, angle);
        w_u32(actor + 0x4C, next);
        goto snap_and_finish;
    case 30:
        w_u16(actor + 0x90, 5);
        state = r_s16(actor + 0x90);
        count = r_s16(actor + 0x90);
        x = r_u32(actor + 0xF6C);
        value = r_u16(0x8001A632 + (uint32)state * 4);
        extra = r_u16(0x8001A630 + (uint32)count * 4);
        w_u16(actor + 0x8E, 1);
        w_u32(actor + 4, x);
        w_u16(actor + 0x1A0, value);
        w_u16(actor + 0x96, extra);
        t3_8002f074(actor);
        frame = r_u16(actor + 0x198);
        next = r_u32(actor + 0x1A4);
        w_u16(actor + 0x58, frame);
        value = r_u16(actor + 0x94);
        w_u16(actor + 0x50, frame);
        animation = r_u32(actor + 0x54);
        w_u8(actor + 0xB8, 1);
        w_u16(actor + 0x92, 1);
        w_u16(actor + 0x74, 0);
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x94, value + 1);
        goto set_recovery_height;
    case 33:
        next = r_u32(actor + 0x1A4);
        state = r_s16(actor + 0x74);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        w_u32(actor + 0x4C, next);
        if (!state) {
            angle = r_u16(actor + 0x2A);
            count = r_s16(actor + 0x40);
            w_u16(actor + 0x2C, angle - 0x8000);
            if (count == 1)
                w_u16(actor + 0x2C, angle - 0x4000);
            else if (count == 2)
                w_u16(actor + 0x2C, angle - 0x10000);
            else if (count == 3)
                w_u16(actor + 0x2C, angle - 0xC000);
            angle = r_u16(actor + 0x2C);
            w_u16(actor + 0x114, 16);
            w_u16(actor + 0x0E, angle);
            other_angle = r_u16(opponent + 0x2C);
            delta = (sint16)(other_angle - angle);
            w_u16(actor + 0x116, delta / 16);
            goto snap_opponent_position;
        }
        if (state < 0)
            goto face_opponent;
        next = r_u32(actor + 0x1A4);
        if ((r_u32(next + 0x24) >> 8) & 0x800) {
face_opponent:
            angle = r_u16(opponent + 0x0E);
            w_u16(actor + 0x2C, angle);
            w_u16(actor + 0x0E, angle);
snap_opponent_position:
            x = r_u32(opponent + 0xF68);
            w_u32(actor, x);
            z = r_u32(opponent + 0xF70);
            w_u32(actor + 8, z);
        }
        state = r_s16(actor + 0x74);
        value = r_u16(actor + 0x74);
        w_u16(actor + 0x106, 0);
        w_u16(actor + 0x108, 0);
        w_u16(actor + 0x10A, 0);
        w_u16(actor + 0x74, state < 0 ? 1 : value + 1);
        goto finish_transition;
    case 34:
        if (r_s16(actor + 0x74) >= 0) {
            animation = r_u32(actor + 0x54);
            value = r_s16(animation + 0x14);
            if (value) {
                state = r_s16(actor + 0x58);
                extra = r_u8(animation + 0x2D);
                if (state < (sint32)extra) {
                    w_u8(actor + 0xBD, 1);
                    goto recovery_frames;
                }
            }
            w_u8(actor + 0xBD, r_s16(opponent + 0x122) != 0);
        }
recovery_frames:
        next = r_u32(actor + 0x1A4);
        state = r_s16(actor + 0x74);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        w_u8(actor + 0x86, 1);
        w_u32(actor + 0x4C, next);
        if (state >= 0) {
            angle = r_u16(actor + 0x2C);
            x = r_u32(actor + 0xF68);
            z = r_u32(actor + 0xF70);
            angle += 0x8000;
            w_u16(actor + 0x2C, angle);
            w_u16(actor + 0x0E, angle);
            w_u32(actor, x);
            w_u32(actor + 8, z);
            t3_80041998(actor);
        } else {
            next = r_u32(actor + 0x1A4);
            if ((r_u32(next + 0x24) >> 8) & 0x800) {
                angle = r_u16(actor + 0x2C);
                x = r_u32(actor + 0xF68);
                z = r_u32(actor + 0xF70);
                w_u16(actor + 0x0E, angle);
                w_u32(actor, x);
                w_u32(actor + 8, z);
            }
        }
        state = r_s16(actor + 0x74);
        value = r_u16(actor + 0x74);
        w_u16(actor + 0x74, state > 0 ? 0xFFFF : value - 1);
        other_animation = r_u32(opponent + 0x1A4);
        if (r_u8(other_animation + 0x1B)) {
            next = r_u32(actor + 0x1A4);
            value = r_u8(other_animation + 0x1B);
            extra = r_u8(next + 0x18);
            w_u16(actor + 0x11A, value + extra);
        } else {
            next = r_u32(actor + 0x1A4);
            value = r_u8(other_animation + 0x18);
            extra = r_u8(next + 0x18);
            delta = (sint32)value - (sint32)extra;
            if (delta > 0)
                value = r_u8(next + 0x18) + delta;
            else
                value = r_u8(other_animation + 0x18);
            w_u16(actor + 0x11A, value);
            count = r_s16(actor + 0x11A);
            if (count < 0) {
                w_u16(actor + 0x11A, 0);
                count = r_s16(actor + 0x11A);
            }
            if (count >= 301)
                w_u16(actor + 0x11A, 300);
        }
        other_animation = r_u32(opponent + 0x1A4);
        count = r_s16(other_animation + 0x14);
        value = (count * 32) / 45 + 34;
        w_u16(actor + 0x118, value);
        if ((sint16)value >= 66)
            w_u16(actor + 0x118, 66);
        if (r_s16(actor + 0x118) < 35)
            w_u16(actor + 0x118, 34);
        w_u16(actor + 0x106, 0);
        w_u16(actor + 0x108, 0);
        w_u16(actor + 0x10A, 0);
        goto finish_transition;
    case 35:
        table = r_u32(actor + 0x1A8);
        count = r_s16(table + 0x28);
        goto start_recovery_curve;
    case 36:
        x = r_u32(actor + 0xF6C);
        state = r_s16(actor + 0x94);
        w_u32(actor + 4, x);
        if (!state) {
            w_u16(actor + 0x9A, 0);
            w_u16(actor + 0x9C, 0);
            w_u16(actor + 0x9E, 0);
        }
        t3_8002f074(actor);
        frame = r_u16(actor + 0x198);
        next = r_u32(actor + 0x1A4);
        w_u8(actor + 0xB8, 1);
        w_u16(actor + 0x92, 1);
        value = r_u16(actor + 0x94);
        w_u16(actor + 0x74, 0);
        w_u16(actor + 0x58, frame);
        animation = r_u32(actor + 0x54);
        w_u16(actor + 0x50, frame);
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x94, value + 1);
set_recovery_height:
        flags = r_u32(animation + 0x24);
        value = 0;
        if (!((flags >> 8) & 0x10)) {
            count = r_s16(actor + 0x118);
            extra = r_u16(actor + 0xE8);
            value = extra + count / 2;
        }
        if ((sint16)value >= 66)
            value = 66;
        next = r_u32(actor + 0x1A4);
        extra = r_u8(next + 0x18);
        w_u16(actor + 0x118, value);
        animation = r_u32(actor + 0x54);
        w_u8(actor + 0xB9, 11);
        w_u16(actor + 0x11A, extra);
        flags = r_u32(animation + 0x24);
        if ((flags >> 8) & 0x10) {
            table = r_u32(actor + 0x1A8);
            angle = r_u16(actor + 0x34);
            extra = r_u16(table + 0x1C);
            w_u16(actor + 0x0E, angle + extra);
        } else {
            w_u16(actor + 0x0E, r_u16(actor + 0x34));
        }
        w_u16(actor + 0x114, 0);
        w_u16(actor + 0x116, 0);
        goto snap_and_finish;
    case 37: case 38: case 39: case 40: case 41: case 42: case 43:
        animation = r_u32(actor + 0x54);
        flags = r_u32(animation + 0x24);
        value = 0;
        if (!((flags >> 8) & 0x10)) {
            count = r_s16(actor + 0x118);
            extra = r_u16(actor + 0xE8);
            value = extra + count / 2;
        }
        if ((sint16)value >= 66)
            value = 66;
        next = r_u32(actor + 0x1A4);
        table = r_u32(actor + 0x1A8);
        extra = r_u8(next + 0x18);
        w_u16(actor + 0x118, value);
        w_u16(actor + 0x11A, extra);
        count = r_s16(table + (kind == 37 ? 0x26 : kind == 38 ? 0x22 : 0x1E));
start_recovery_curve:
        script = 0x8001075C + (uint32)count * 2;
        w_u32(actor + 0x110, script);
        value = r_u16(script);
        w_u32(actor + 0x110, script + 2);
        w_u16(actor + 0x106, value);
        value = r_u16(script + 2);
        w_u32(actor + 0x110, script + 4);
        if (kind >= 39) {
            state = r_s16(actor + 0x19A);
            w_u16(actor + 0x10A, 8);
            w_u16(actor + 0x108, value);
            table = r_u32(actor + 0x1A8);
            extra = r_u16(table + 0x1C);
            angle = r_u16(actor + (state == 42 ? 0x34 : 0x2A));
            w_u16(actor + 0x10C, angle + extra + 0x8000);
            t3_80031348(actor);
            next = r_u32(actor + 0x1A4);
            w_u16(actor + 0x58, 1);
            w_u16(actor + 0x50, 1);
            w_u16(actor + 0x74, 0);
            w_u32(actor + 0x4C, next);
        } else {
            table = r_u32(actor + 0x1A8);
            w_u16(actor + 0x10A, 8);
            w_u16(actor + 0x108, value);
            extra = r_u16(table + (kind == 37 ? 0x24 : 0x1C));
            next = r_u32(actor + 0x1A4);
            w_u16(actor + 0x58, 1);
            w_u16(actor + 0x50, 1);
            angle = r_u16(actor + 0x2A);
            if (kind == 37)
                w_u16(actor + 0x118, 0);
            w_u16(actor + 0x74, 0);
            w_u32(actor + 0x4C, next);
            w_u16(actor + 0x10C, angle + extra + 0x8000);
            if (kind != 37) {
                angle = r_u16(actor + 0x34);
                w_u8(actor + 0xB9, 7);
                w_u16(actor + 0x114, 0);
                w_u16(actor + 0x116, 0);
                w_u16(actor + 0x0E, angle);
                goto snap_and_finish;
            }
        }
        angle = r_u16(actor + 0x2C);
        w_u8(actor + 0xB9, 11);
        w_u16(actor + 0x114, 0);
        w_u16(actor + 0x116, 0);
        w_u16(actor + 0x0E, angle);
        goto snap_and_finish;
    default:
        next = r_u32(actor + 0x1A4);
        angle = r_u16(actor + 0x2A);
        w_u16(actor + 0x58, 1);
        w_u16(actor + 0x50, 1);
        w_u32(actor + 0x4C, next);
        w_u16(actor + 0x0E, angle);
        w_u16(actor + 0x2C, angle);
        goto snap_and_finish;
    }
snap_and_finish:
    if (snap_position) {
        x = r_u32(actor + 0xF68);
        z = r_u32(actor + 0xF70);
        w_u32(actor, x);
        w_u32(actor + 8, z);
    }
finish_transition:
    next = r_u32(actor + 0x1A4);
    value = r_u16(actor + 0x1A0);
    w_u32(actor + 0x1A4, 0);
    w_u32(actor + 0x54, next);
    w_u16(actor + 0x1A0, 0xFFFF);
    w_u16(actor + 0xA0, value);
    frame = r_u8(next + 0x2D);
    script = r_u32(next + 0x28);
    value = 0;
    if (frame) {
        if (r_u8(script + 2))
            value = 2;
        else if (r_u8(script))
            value = 1;
    }
    w_u8(actor + 0xB4, value);
    value = r_u8(actor + 0xDA) ? 1 : r_u16(actor + 0x58);
    w_u16(actor + 0x5C, value);
    value = r_u16(actor + 0x19A);
    state = r_s16(actor + 0x74);
    w_u16(actor + 0x19A, 0);
    w_u16(actor + 0x19C, 0);
    w_u16(actor + 0x19E, 0);
    w_u8(actor + 0xC1, 0);
    w_u16(actor + 0xA2, value);
    count = r_s16(actor + 0x16);
    w_u8(actor + 0xC4, state != 0);
    value = 1;
    if (count == 19 && r_s16(actor + 0x58) == 1) {
        animation = r_u32(actor + 0x54);
        value = r_u8(animation + 0x19);
        if (value != 1)
            w_u32(actor + 4, 0);
    }
    return value;
}

uint32 t3_80057fb8(uint32 state)
{
    uint32 total = 0;
    uint32 selected = 0;
    uint32 code;
    uint32 selection_kind;
    uint32 continuation;
    uint32 finish_mode;
    uint32 entry;
    uint32 index;
    sint32 count;
    FUNCTION_MARKER(0x80057FB8, "SLUS_004.02");
    if (r_s32(state + 28) >= 2951)
        return 0;
    if (r_s16(state + 532)) {
        code = r_u32(state + 564);
        if (code == 535)
            goto active_extra;
        selection_kind = 0;
        continuation = 0;
    } else {
        if (!r_s16(state + 524))
            goto final_filter;
        code = r_u32(state + 564);
        switch (code) {
        case 271:
            selection_kind = 2;
            continuation = 3;
            break;
        case 535:
            selection_kind = 1;
            continuation = 1;
            break;
        case 799:
            selection_kind = 3;
            continuation = 1;
            break;
        case 1042:
            code = r_u16(r_u32(0x800AFA5C) + 96);
            if (code == 0x2021 || code == 0x2829) {
                selection_kind = 0;
                continuation = 1;
            } else {
                selection_kind = 1;
                continuation = 2;
            }
            break;
        case 1798:
            selection_kind = 0;
            continuation = 3;
            break;
        default:
            goto final_filter;
        }
    }
    count = r_s16(state + 112);
    for (index = 0; (sint32)index < count; ++index) {
        uint32 accept = 0;
        entry = 0x8009F978 + index * 12;
        if (!r_s16(entry + 8)) {
            if (selection_kind == 0) {
                uint32 animation = r_u32(entry);
                uint32 flags = r_u32(animation + 4);
                uint32 frame = r_u8(animation + 45);
                accept = (flags & 1) && frame == 0;
            } else if (selection_kind == 1) {
                uint32 condition = r_u32(entry + 4);
                uint32 animation = r_u32(entry);
                uint32 opcode = r_u16(condition);
                uint32 flags = r_u32(animation + 4);
                accept = opcode != 0xC002 && (flags & 65535) == 0x1052;
            } else if (selection_kind == 2) {
                accept = r_u16(r_u32(entry) + 4) == 0x2829;
            } else {
                uint32 opcode = r_u16(r_u32(entry + 4));
                if (opcode != 0xC002) {
                    uint32 flags = r_u32(r_u32(entry) + 4);
                    accept = !(flags & 256)
                        && ((flags & 65535) == 0x1052 || (flags & 65535) == 0x2829);
                }
            }
            if (accept) {
                w_u16(entry + 8, 1);
                ++selected;
            }
        }
    }
    if (count > 0) {
        uint32 accumulated = r_u16(state + 114);
        total = selected;
        w_u16(state + 114, accumulated + total);
    }
    if (continuation == 0)
        goto active_extra;
    if (continuation == 1)
        goto final_filter;
    if (continuation == 2) {
        uint32 seed = r_u32(0x800ADCA0);
        uint32 random = seed & 4095;
        w_u32(0x800ADCA0, seed * 5 + 3);
        if (random % 50 == 0) {
            selected = 0;
            count = r_s16(state + 112);
            for (index = 0; (sint32)index < count; ++index) {
                entry = 0x8009F978 + index * 12;
                if (!r_s16(entry + 8)) {
                    uint32 animation = r_u32(entry);
                    uint32 flags = r_u32(animation + 4);
                    uint32 frame = r_u8(animation + 45);
                    if ((flags & 1) && !frame) {
                        w_u16(entry + 8, 1);
                        ++selected;
                    }
                }
            }
            if (count > 0)
                w_u16(state + 114, r_u16(state + 114) + selected);
            total += selected;
        }
    }
    if (r_s32(state + 24) < 3)
        goto final_filter;
    finish_mode = 1;
    goto filter_commands;

active_extra:
    if (r_s32(state + 24) < 3)
        return total;
    finish_mode = 0;
    goto filter_commands;

final_filter:
    if (r_s32(state + 36) >= -70 || r_s32(state + 24) >= 3)
        return total;
    finish_mode = 2;

filter_commands:
    selected = 0;
    count = r_s16(state + 112);
    for (index = 0; (sint32)index < count; ++index) {
        sint32 status;
        uint32 condition;
        uint32 opcode;
        entry = 0x8009F978 + index * 12;
        status = r_s16(entry + 8);
        condition = r_u32(entry + 4);
        opcode = r_u16(condition);
        if (!status && opcode == 0xC002) {
            w_u16(entry + 8, 1);
            ++selected;
        }
    }
    if (count > 0)
        w_u16(state + 114, r_u16(state + 114) + selected);
    if (finish_mode == 2)
        return selected;
    total += selected;
    if (finish_mode == 1)
        goto final_filter;
    return total;
}

uint32 t3_800793b8(uint32 actor, uint32 ordering, uint32 packet)
{
    uint32 first_x, first_z, second_x, second_z, center_x, center_z;
    uint32 x_extent, z_extent, edge, reach, width, kind, temporary;
    uint32 value, other, third, fourth, base, indices, index;
    const uint32 vertices = 0x1F800010;
    FUNCTION_MARKER(0x800793B8, "SLUS_004.02");
    first_x = r_u32(actor + 3468);
    first_z = r_u32(actor + 3476);
    second_x = r_u32(actor + 3264);
    second_z = r_u32(actor + 3272);
    w_u32(0x1F800004, 0);
    center_x = (uint32)((sint32)(first_x + second_x) >> 1);
    center_z = (uint32)((sint32)(first_z + second_z) >> 1);
    w_u32(0x1F800000, center_x);
    w_u32(0x1F800008, center_z);
    SetRotMatrix((MATRIX *)psx_addr(0x800ADF70, 32));
    ApplyRotMatrixLVPSX(0x1F800000, 0x1F800024);
    first_x -= center_x;
    value = r_u32(0x1F800024);
    other = r_u32(0x800ADF84);
    first_z -= center_z;
    w_u32(0x1F800024, value + other);
    value = r_u32(0x1F800028);
    other = r_u32(0x800ADF88);
    third = r_u32(0x1F80002C);
    fourth = r_u32(0x800ADF8C);
    w_u32(0x1F800028, value + other);
    w_u32(0x1F80002C, third + fourth);
    SetTransMatrix((MATRIX *)psx_addr(vertices, 32));
    value = r_u32(actor + 84);
    second_x -= center_x;
    value = r_u32(value + 4);
    second_z -= center_z;
    edge = 160;
    reach = 220;
    width = 300;
    if ((value & 0x204) && r_s32(actor + 2452) >= -499) {
        edge = 200;
        reach = 400;
        width = 400;
        value = r_u32(actor + 2448);
        other = r_u32(actor + 2456);
        first_x = value - center_x;
        first_z = other - center_z;
        first_x += (sint32)first_x < 0 ? -128 : 128;
        first_z += (sint32)first_z < 0 ? -128 : 128;
        second_x = (sint32)first_x < 0 ? 64 : (uint32)-64;
        second_z = (sint32)first_z < 0 ? 64 : (uint32)-64;
    }
    x_extent = first_x - second_x;
    z_extent = first_z - second_z;
    if ((sint32)x_extent < 0)
        x_extent = 0 - x_extent;
    if ((sint32)z_extent < 0)
        z_extent = 0 - z_extent;
    if ((sint32)z_extent < (sint32)(x_extent * 2)) {
        kind = 0;
        if (((sint32)x_extent >> 1) < (sint32)z_extent) {
            kind = 1;
            edge = edge * 71 / 100;
            reach = reach * 71 / 100;
            width = width * 71 / 100;
        }
        if ((sint32)first_x < (sint32)second_x) {
            temporary = first_x;
            first_x = second_x;
            second_x = temporary;
            temporary = first_z;
            first_z = second_z;
            second_z = temporary;
        }
        if ((sint32)x_extent < (sint32)(reach * 2)) {
            first_x = reach;
            second_x = 0 - reach;
        }
    } else {
        kind = 2;
        if ((sint32)first_z < (sint32)second_z) {
            temporary = first_x;
            first_x = second_x;
            second_x = temporary;
            temporary = first_z;
            first_z = second_z;
            second_z = temporary;
        }
        if ((sint32)z_extent < (sint32)(reach * 2)) {
            first_z = reach;
            second_z = 0 - reach;
        }
    }
    if (kind == 0) {
        w_u32(vertices, first_x - edge);
        w_u16(vertices + 12, first_z - width);
        w_u32(vertices + 8, first_x - reach);
        w_u32(vertices + 24, first_x - reach);
        w_u16(vertices + 28, first_z + width);
        w_u16(vertices + 36, second_z + width);
        w_u16(vertices + 52, second_z - width);
        w_u16(vertices + 4, first_z);
        w_u32(vertices + 16, first_x);
        w_u16(vertices + 20, first_z);
        w_u32(vertices + 32, second_x + reach);
        w_u32(vertices + 40, second_x);
        w_u16(vertices + 44, second_z);
        w_u32(vertices + 48, second_x + reach);
        w_u32(vertices + 56, second_x + edge);
        w_u16(vertices + 60, second_z);
    } else if (kind == 1) {
        uint32 quarter = width >> 2;
        if ((sint32)first_z < 0) {
            w_u32(vertices, first_x - edge);
            w_u16(vertices + 4, first_z + edge);
            w_u32(vertices + 8, first_x + quarter);
            w_u16(vertices + 12, first_z + width + width);
            w_u32(vertices + 24, first_x - width);
            w_u16(vertices + 28, first_z - reach);
            w_u32(vertices + 32, second_x + reach - quarter);
            w_u16(vertices + 36, second_z - width - edge);
            w_u32(vertices + 48, second_x + width);
            w_u16(vertices + 52, second_z + reach + quarter);
            w_u32(vertices + 56, second_x + edge);
            value = second_z - edge;
        } else {
            w_u32(vertices, first_x - edge);
            w_u16(vertices + 4, first_z - edge);
            w_u32(vertices + 8, first_x + quarter);
            w_u16(vertices + 12, first_z - width - width);
            w_u32(vertices + 24, first_x - width);
            w_u16(vertices + 28, first_z + reach);
            w_u32(vertices + 32, second_x + reach - quarter);
            w_u16(vertices + 36, second_z + width + edge);
            w_u32(vertices + 48, second_x + width);
            w_u16(vertices + 52, second_z - reach - quarter);
            w_u32(vertices + 56, second_x + edge);
            value = second_z + edge;
        }
        w_u32(vertices + 16, first_x);
        w_u16(vertices + 20, first_z);
        w_u32(vertices + 40, second_x);
        w_u16(vertices + 44, second_z);
        w_u16(vertices + 60, value);
    } else {
        w_u16(vertices + 4, first_z - edge);
        w_u32(vertices + 8, first_x + width);
        w_u32(vertices + 24, first_x - width);
        w_u16(vertices + 12, first_z - reach);
        w_u16(vertices + 28, first_z - reach);
        w_u32(vertices + 32, second_x - width);
        w_u32(vertices + 48, second_x + width);
        value = second_z + edge;
        w_u32(vertices, first_x);
        w_u32(vertices + 16, first_x);
        w_u16(vertices + 20, first_z);
        w_u16(vertices + 36, second_z + reach);
        w_u32(vertices + 40, second_x);
        w_u16(vertices + 44, second_z);
        w_u16(vertices + 52, second_z + reach);
        w_u32(vertices + 56, second_x);
        w_u16(vertices + 60, value);
    }
    value = r_u32(actor + 4724);
    base = r_u32(value + ordering * 4);
    t3_80036af8(vertices, (uint32 *)psx_addr(0x1F800070, 32), (uint16 *)psx_addr(0x1F800090, 16), 8);
    indices = 0x80098A60;
    w_u32(base, packet | 0x0C000000);
    for (index = 0; index < 4; ++index) {
        value = r_u8(indices);
        value = r_u32(0x1F800070 + value * 4);
        w_u32(base + index * 52 + 8, value);
        value = r_u8(indices + 1);
        value = r_u32(0x1F800070 + value * 4);
        w_u32(base + index * 52 + 20, value);
        value = r_u8(indices + 2);
        value = r_u32(0x1F800070 + value * 4);
        w_u32(base + index * 52 + 32, value);
        value = r_u8(indices + 3);
        value = r_u32(0x1F800070 + value * 4);
        indices -= 4;
        w_u32(base + index * 52 + 44, value);
    }
    return (base + 156) & 0xFFFFFF;
}

uint32 t3_80079af4(uint32 mode)
{
    FUNCTION_MARKER(0x80079AF4, "SLUS_004.02");
    return mode == 4 ? 0 : 0xF0C0;
}

uint32 t3_80079b30(uint32 mode, uint32 buffer)
{
    uint32 index;
    FUNCTION_MARKER(0x80079B30, "SLUS_004.02");
    if (mode == 4) {
        for (index = 0; index < 3; ++index) {
            w_u32(0x800A3970 + index * 4, 0);
            w_u32(0x800A397C + index * 4, 0);
        }
        return 0;
    }
    for (index = 0; index < 2; ++index) {
        w_u32(0x800A3970 + index * 4, buffer);
        buffer += 0x73C0;
        w_u32(0x800A397C + index * 4, buffer);
        buffer += 0x4A0;
    }
    if (mode == 3) {
        w_u32(0x800A3978, buffer - 0x39E0);
        w_u32(0x800A3984, buffer - 0x250);
        return 0x800A3970;
    }
    w_u32(0x800A3978, 0);
    w_u32(0x800A3984, 0);
    return 0x800A3978;
}

uint32 t3_80079c38(uint32 index)
{
    FUNCTION_MARKER(0x80079C38, "SLUS_004.02");
    return r_u32(0x800A3970 + index * 4);
}

uint32 t3_80079c54(uint32 index)
{
    FUNCTION_MARKER(0x80079C54, "SLUS_004.02");
    return r_u32(0x800A397C + index * 4);
}

uint32 t3_80079290(uint32 packets, uint32 color, uint32 actor)
{
    uint32 index;
    FUNCTION_MARKER(0x80079290, "SLUS_004.02");
    for (index = 0; index < 2; ++index) {
        uint32 packet = r_u32(0x800ADED8 + actor * 8 + index * 4);
        uint32 texture = (uint32)GetTPage(0, 2, 512, 0) << 16;
        uint32 link = 0;
        uint32 remaining;
        w_u32(packets + index * 4, packet);
        for (remaining = 4; remaining; --remaining) {
            w_u32(packet, link | 0x0C000000);
            w_u32(packet + 4, 0x3E000000);
            w_u32(packet + 16, color);
            w_u32(packet + 28, 0);
            w_u32(packet + 40, remaining == 1 || remaining == 3 ? color : 0);
            link = packet & 0xFFFFFF;
            w_u32(packet + 12, 0x7FD10000);
            w_u32(packet + 24, texture);
            w_u32(packet + 36, 0);
            w_u32(packet + 48, 0);
            packet += 52;
        }
    }
    return 0;
}

uint32 t3_80079144(uint32 text, uint32 count)
{
    FUNCTION_MARKER(0x80079144, "SLUS_004.02");
    while ((sint32)count > 0) {
        uint32 character;
        --count;
        do {
            character = r_u8(text);
            ++text;
        } while (character);
    }
    return text;
}

uint32 t3_80079170(uint8 *output, uint32 text, uint32 budget)
{
    FUNCTION_MARKER(0x80079170, "SLUS_004.02");
    while ((sint32)budget > 0) {
        uint32 character = r_u8(text);
        if (character == 252) {
            *output = 0;
            return text + 1;
        }
        if (character == 251) {
            budget -= 10;
        } else if (character >= 161) {
            budget -= 2;
        } else if (character < 32) {
            character = r_u8(text);
            budget += r_u8(0x800989F7 + character);
        } else {
            --budget;
        }
        character = r_u8(text);
        ++text;
        *output++ = (uint8)character;
    }
    *output = 0;
    return text;
}

uint32 t3_80079208(uint32 text)
{
    uint32 length = 0;
    uint32 character;
    FUNCTION_MARKER(0x80079208, "SLUS_004.02");
    character = r_u8(text++);
    while (character) {
        if (character == 252)
            length += 100;
        else if (character == 251)
            length += 10;
        else if (character >= 161)
            length += 2;
        else if (character < 32)
            length += r_u8(0x800989F7 + character);
        else
            ++length;
        character = r_u8(text++);
    }
    return length;
}

uint32 t3_80078df8(uint32 paused)
{
    PSX_RECT rectangle;
    uint32 index;
    uint32 countdown;
    uint32 ordering;
    uint32 packet;
    uint32 x;
    uint32 width;
    uint32 y;
    uint32 height;
    FUNCTION_MARKER(0x80078DF8, "SLUS_004.02");
    if (r_u32(0x800ADCA4)) {
        if (r_u32(0x800AFA88) == 7)
            return t3_80078550(0, r_u32(0x800B6BF4));
        for (index = 0; index < 2; ++index) {
            if ((r_u8(0x800AFAA5) >> index) & 1)
                t3_80078550(index, index);
        }
        return 0;
    }
    countdown = r_u8(0x800A8672);
    if (countdown) {
        w_u8(0x800A8672, countdown - 1);
        return countdown - 1;
    }
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    x = r_u16(0x800AE230);
    y = r_u16(0x800AE232);
    rectangle.x = (sint16)x;
    rectangle.y = (sint16)y;
    rectangle.w = 0;
    rectangle.h = 0;
    packet = t3_8004db28(ordering, packet, &rectangle);
    ordering = r_u32(0x800A9218);
    w_u32(0x800ADF64, packet);
    packet = t3_8004db28(ordering + 4, packet, &rectangle);
    w_u32(0x800ADF64, packet);
    if (r_u16(paused == 1 ? 0x800ADF08 : 0x800ADF0A) & 0x8F0) {
        w_u8(0x80098A41, 0);
        w_u8(0x80098A40, 0);
        w_u8(0x800A8672, 2);
        t3_800484c8();
        t3_800753bc(0, 0x50F4, 0);
    }
    if (r_u32(0x800AFA88) == 8) {
        t3_8004ce74(0x80027C1C, 0, 0, 6, 440, 6, 5, 6, 5);
        t3_80078550(0, r_u32(0x800B6BF4));
    } else {
        t3_8004ce74(0x80027C1C, 0, 0, ((paused - 1) * 184) | 6, 440, 6, 5, 6, 5);
        for (index = 0; index < 2; ++index) {
            if ((r_u8(0x800AFAA5) >> index) & 1)
                t3_80078550(index, index);
        }
    }
    ordering = r_u32(0x800A9218);
    packet = r_u32(0x800ADF64);
    x = r_u16(0x800AE230);
    width = r_u16(0x800AE234);
    y = r_u16(0x800AE232);
    height = r_u16(0x800AE236);
    rectangle.x = (sint16)x;
    rectangle.y = 92;
    rectangle.w = (sint16)width;
    rectangle.h = (sint16)(y + height - 92);
    packet = t3_8004db28(ordering, packet, &rectangle);
    x = r_u16(0x800AE230);
    ordering = r_u32(0x800A9218);
    rectangle.y = 116;
    rectangle.h = 319;
    width = r_u16(0x800AE234);
    w_u32(0x800ADF64, packet);
    rectangle.x = (sint16)x;
    rectangle.w = (sint16)width;
    packet = t3_8004db28(ordering + 4, packet, &rectangle);
    x = r_u16(0x800AE230);
    y = r_u16(0x800AE232);
    width = r_u16(0x800AE234);
    ordering = r_u32(0x800A8C54);
    rectangle.h = 32;
    w_u32(0x800ADF64, packet);
    rectangle.x = (sint16)x;
    rectangle.y = (sint16)y;
    rectangle.w = (sint16)width;
    ordering = r_u32(ordering + 16);
    packet = t3_8004db28(ordering, packet, &rectangle);
    w_u32(0x800ADF64, packet);
    return packet;
}

uint32 t3_80061ef0(uint32 state)
{
    uint32 opponent;
    uint32 actor;
    uint32 animation;
    uint32 direction;
    uint32 flags;
    uint32 angle;
    sint32 code;
    sint32 frame;
    sint32 end;
    sint32 level;
    sint32 height;
    sint32 actor_height;
    sint32 opponent_height;
    int failed_direction_reads_actor;
    FUNCTION_MARKER(0x80061EF0, "SLUS_004.02");
    opponent = r_u32(0x800A867C);
    actor = r_u32(0x800AFA5C);
    animation = r_u32(opponent + 84);
    code = r_s16(opponent + 160);
    frame = r_s16(opponent + 88);
    if (code == 3556) {
        w_u16(state + 140, 8);
        if ((uint32)(frame - 9) < 5 && r_s32(state + 20) < 2
            && !((uint32)randPSX() & 31)) {
            t3_80059394(state);
            goto reset_cooldown;
        }
        end = r_u8(animation + 45);
        if (frame < end - 30) {
            level = r_s32(state + 20);
            if (level < 2)
                goto reset_cooldown;
            if (level < 3) {
                w_u16(state + 104, r_u16(state + 104) - 4);
                return 0x2000;
            }
            actor = r_u32(0x800AFA5C);
            height = r_s16(actor + 48);
            failed_direction_reads_actor = 0;
            goto choose_direction;
        }
        if (frame < end - 20) {
            actor = r_u32(0x800AFA5C);
            height = r_s16(actor + 48);
            if (height >= 14336) {
                if (r_s32(state + 20) < 3)
                    return 0x2000;
                failed_direction_reads_actor = 0;
                goto choose_direction;
            }
            level = r_s32(state + 20);
            if (level < 3)
                return 0;
            if (level < 4)
                return 0x2000;
            goto random_guard;
        }
        end = r_u8(animation + 46);
        if (frame < end) {
            actor = r_u32(0x800AFA5C);
            height = r_s16(actor + 48);
            if (height >= 24576) {
                w_u16(state + 104, 8);
                return 0x8000;
            }
            if (height >= 14336) {
                if (r_s32(state + 20) >= 2)
                    return 0x2000;
                w_u16(state + 104, 8);
                return 0;
            }
            failed_direction_reads_actor = 1;
            goto choose_direction;
        }
        if (frame < end + 40) {
            if (r_s32(state + 20) < 2)
                goto reset_cooldown;
        } else if (r_s32(state + 20) < 3) {
            goto reset_cooldown;
        }
        goto random_guard;
    }
    if (code != 366)
        return 0xFFFFFFFF;
    w_u16(state + 140, 8);
    if (frame < (sint32)r_u8(animation + 45) - 8) {
        angle = r_u16(actor + 46);
        actor_height = r_s16(actor + 4728);
        opponent_height = r_s16(opponent + 4728);
        direction = 0xFFFFFFFF;
        if (((angle + 8192) & 65535) < 16385
            && !(r_u32(r_u32(actor + 84) + 4) & 0x400000)) {
            uint32 distance = r_u32(state + 28);
            direction = actor_height < opponent_height;
            if (distance >= 7001)
                direction = 0xFFFFFFFF;
        }
        level = r_s32(state + 20);
        if (level <= 0)
            goto reset_cooldown;
        if (level < 3)
            return 0x2000;
        if (!((uint32)randPSX() & 7)
            && !r_s16(r_u32(0x800AFA5C) + 50) && direction < 2) {
            t3_80061a58(state, direction);
            return 0xFFFFFFFE;
        }
        goto random_guard;
    }
    if (frame >= (sint32)r_u8(animation + 46))
        return 0xFFFFFFFF;
    if (!r_s16(actor + 50)) {
        if (r_s32(state + 28) >= 7001)
            return 0;
        return 0x6000;
    }
    level = r_s32(state + 20);
    if (level < 2) {
        uint32 choice = (uint32)randPSX() & 3;
        return r_u16(0x8002309C + choice * 2);
    }
    if (level < 3)
        return 0x6000;
    goto random_guard;

choose_direction:
    opponent = r_u32(0x800A867C);
    actor_height = r_s16(actor + 4728);
    opponent_height = r_s16(opponent + 4728);
    animation = r_u32(actor + 84);
    flags = r_u32(animation + 4);
    angle = r_u16(actor + 60);
    if (!(flags & 0x400000) && r_u32(state + 28) < 7001) {
        direction = 0;
        if (angle - 257 >= 32511) {
            direction = 1;
            if (((angle + 32767) & 65535) >= 32510)
                goto direction_failed;
        }
        if (opponent_height < actor_height)
            direction = 1 - direction;
        t3_80061a58(state, direction);
        t3_80061a58(state, direction);
        return 0xFFFFFFFE;
    }
direction_failed:
    if (failed_direction_reads_actor)
        return (uint32)(r_s16(r_u32(0x800AFA5C) + 50) == 0) << 13;
    return 0x2000;

reset_cooldown:
    w_u16(state + 104, 65535);
    return 0x2000;

random_guard:
    return (uint32)(((uint32)randPSX() & 7) == 0) << 13;
}

uint32 t3_8003172c(uint32 actor)
{
    union {
        uint32 words[18];
        uint16 halves[36];
    } points;
    uint16 depths[4];
    uint8 indices[50];
    sint32 mode;
    sint32 scale;
    sint32 first_angle;
    sint32 second_angle;
    sint32 cosine;
    sint32 sine;
    sint32 second_cosine;
    sint32 second_sine;
    uint32 packet;
    uint32 ordering;
    uint32 index;
    FUNCTION_MARKER(0x8003172C, "SLUS_004.02");
    mode = r_s16(actor + 24);
    if (mode != 4) {
        if (mode != 15)
            return 15;
        mode = r_s16(actor + 22);
        if (mode != 4)
            return (uint32)mode;
    }
    t3_8003785c(0x800ADE20, actor + 2972);
    t3_80036af8(0x8009577C, points.words + 1, depths, 4);
    scale = (sint16)depths[0] < 10 ? 200 : 2000 / (sint16)depths[0];
    first_angle = ratan2((sint16)points.halves[4] - (sint16)points.halves[2],
        (sint16)points.halves[5] - (sint16)points.halves[3]);
    second_angle = ratan2((sint16)points.halves[8] - (sint16)points.halves[6],
        (sint16)points.halves[9] - (sint16)points.halves[7]);
    cosine = r_s16(0x8001F0C4 + ((uint32)first_angle & 4095) * 2) * scale / 4096;
    sine = r_s16(0x8001E8C4 + ((uint32)first_angle & 4095) * 2) * scale / 4096;
    second_cosine = r_s16(0x8001F0C4 + ((uint32)second_angle & 4095) * 2) * scale / 4096;
    second_sine = r_s16(0x8001E8C4 + ((uint32)second_angle & 4095) * 2) * scale / 4096;
    points.halves[0] = (uint16)(points.halves[2] - sine);
    points.halves[1] = (uint16)(points.halves[3] - cosine);
    points.halves[10] = (uint16)(points.halves[8] + second_sine);
    points.halves[11] = (uint16)(points.halves[9] + second_cosine);
    for (index = 0; index < 6; ++index) {
        uint32 upper = 12 + index * 2;
        uint32 lower = 24 + index * 2;
        if (index == 3) {
            cosine = second_cosine;
            sine = second_sine;
        }
        points.halves[upper] = (uint16)(points.halves[index * 2] + cosine);
        points.halves[upper + 1] = (uint16)(points.halves[index * 2 + 1] - sine);
        points.halves[lower] = (uint16)(points.halves[index * 2] - cosine);
        points.halves[lower + 1] = (uint16)(points.halves[index * 2 + 1] + sine);
        if (index == 0 || index == 5) {
            points.halves[upper] = (uint16)(points.halves[upper] - (cosine >> 2));
            points.halves[upper + 1] = (uint16)(points.halves[upper + 1] + (sine >> 2));
            points.halves[lower] = (uint16)(points.halves[lower] + (cosine >> 2));
            points.halves[lower + 1] = (uint16)(points.halves[lower + 1] - (sine >> 2));
        }
    }
    points.halves[10] = (uint16)(points.halves[10] + (sine >> 2));
    points.halves[11] = (uint16)(points.halves[11] + (cosine >> 2));
    mode = r_s16(actor + 18);
    index = r_u32(0x800ADEFC);
    packet = r_u32(0x8009BC88 + (uint32)mode * 8 + index * 4);
    for (index = 0; index < 48; index += 16) {
        uint32 first = r_u32(0x80019F24 + index);
        uint32 second = r_u32(0x80019F28 + index);
        uint32 third = r_u32(0x80019F2C + index);
        uint32 fourth = r_u32(0x80019F30 + index);
        uint32 byte;
        for (byte = 0; byte < 4; ++byte) {
            indices[index + byte] = (uint8)(first >> (byte * 8));
            indices[index + byte + 4] = (uint8)(second >> (byte * 8));
            indices[index + byte + 8] = (uint8)(third >> (byte * 8));
            indices[index + byte + 12] = (uint8)(fourth >> (byte * 8));
        }
    }
    {
        uint8 first = r_u8(0x80019F54);
        uint8 second = r_u8(0x80019F55);
        indices[48] = first;
        indices[49] = second;
    }
    ordering = r_u32(r_u32(0x800A8C54) + 4);
    for (index = 0; index < 10; ++index) {
        uint32 face = index * 5;
        uint32 depth_entry;
        uint32 link;
        w_u16(packet + 8, points.halves[indices[face] * 2]);
        w_u16(packet + 10, points.halves[indices[face] * 2 + 1]);
        w_u16(packet + 20, points.halves[indices[face + 1] * 2]);
        w_u16(packet + 22, points.halves[indices[face + 1] * 2 + 1]);
        w_u16(packet + 32, points.halves[indices[face + 2] * 2]);
        w_u16(packet + 34, points.halves[indices[face + 2] * 2 + 1]);
        w_u16(packet + 44, points.halves[indices[face + 3] * 2]);
        w_u16(packet + 46, points.halves[indices[face + 3] * 2 + 1]);
        depth_entry = ordering + (((uint32)(sint32)(sint16)depths[indices[face + 4]] - 8) & 1023) * 4;
        link = r_u32(depth_entry);
        w_u32(packet, (link & 0xFFFFFF) | 0x0C000000);
        link = r_u32(depth_entry);
        w_u32(depth_entry, (link & 0xFF000000) | (packet & 0xFFFFFF));
        packet += 52;
    }
    return 0;
}

