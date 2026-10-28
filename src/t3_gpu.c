#include "game_functions.h"
#include "psx_gpu.h"

uint32 t3_8007efc8(uint32 tim)
{
    w_u32(0x800A3B88, tim);
    return 0;
}

T3_TIM_IMAGE *t3_8007efd8(T3_TIM_IMAGE *image)
{
    uint32 source = r_u32(0x800A3B88);
    uint32 offset = source & 0x1FFFFFFF;
    uint32 pixels;
    uint32 clut;
    const uint8 *body;
    GpuTimImage decoded;

    if (!image || offset > PSX_DRAM_SIZE - 8 || r_u32(source) != 0x10)
        return NULL;
    body = (const uint8 *)psx_addr(source + 4, PSX_DRAM_SIZE - offset - 4);
    if (!gpu_decode_tim_body(body, PSX_DRAM_SIZE - offset - 4, &decoded))
        return NULL;
    pixels = source + 4 + (uint32)(decoded.pixel_data - body);
    clut = decoded.clut_data != NULL ? source + 4 + (uint32)(decoded.clut_data - body) : 0;
    image->mode = decoded.mode;
    image->clut_rectangle = clut != 0 ? clut - 8 : 0;
    image->clut_pixels = clut;
    image->image_rectangle = pixels - 8;
    image->image_pixels = pixels;
    w_u32(0x800A3B88, pixels - 12 + (r_u32(pixels - 12) & ~3u));
    return image;
}

void t3_8007c890(uint32 ordering, uint32 count)
{
    if (r_u8(0x80098BBA) >= 2)
        xport_guest_call3(r_u32(0x80098BB4), 0x800280F4, ordering, count);
    ClearOTagRNativePSX((uint32 *)psx_addr(ordering, count * 4), (sint32)count, 0x80098C78, 0x80098C64);
}

uint32 t3_8007c7c8(uint32 table, uint32 count)
{
    return ClearOTagPSX(table, count, 0x80098C78, 0x80098C64, 0x80098BBA, 0x80098BB4, 0x800280DC);
}

void t3_8007bca4(uint32 packet)
{
    setLineF2((LINE_F2 *)psx_addr(packet, sizeof(LINE_F2)));
}

uint32 t3_8007ba74(uint32 first, uint32 second)
{
    AddPrim(psx_addr(first, 4), psx_addr(second, 4));
    return r_u32(first);
}

void t3_8007c650(uint32 rectangle, uint32 pixels)
{
    sint32 width = r_s16(rectangle + 4);
    sint32 height = r_s16(rectangle + 6);
    uint32 bytes = width > 0 && height > 0 ? (uint32)width * (uint32)height * 2 : 0;

    LoadImagePSX((PSX_RECT *)psx_addr(rectangle, 8), (uint32 *)psx_addr(pixels, bytes));
}

void t3_8007c710(uint32 rectangle, uint32 x, uint32 y)
{
    MoveImage((PSX_RECT *)psx_addr(rectangle, 8), (sint16)(uint16)x, (sint16)(uint16)y);
}

void t3_8007c5b8(uint32 rectangle, uint32 red, uint32 green, uint32 blue)
{
    ClearImage2((PSX_RECT *)psx_addr(rectangle, 8), (uint8)red, (uint8)green, (uint8)blue);
}

void t3_800827f8(uint32 vertices, uint32 screens, uint16 *depths, uint16 *interpolation, uint16 *flags, uint32 count)
{
    uint32 depth;
    uint32 factor;
    uint32 flag;

    do
    {
        xport_gte_write_data(0, r_u32(vertices));
        xport_gte_write_data(1, r_u32(vertices + 4));
        xport_gte_execute(0x4A180001);
        vertices += 8;
        if (count == 0x80000000)
        {
            xport_mips_overflow_exception(0x80082818);
            return;
        }
        --count;
        w_u32(screens, xport_gte_read_data(14));
        depth = xport_gte_read_data(19);
        factor = xport_gte_read_data(8);
        flag = xport_gte_read_flag();
        *depths++ = (uint16)depth;
        *interpolation++ = (uint16)factor;
        *flags++ = (uint16)(flag >> 12);
        screens += 4;
    }
    while ((sint32)count > 0);
}

uint32 t3_80081858(uint32 matrix, uint32 x, uint32 y, uint32 z, uint32 *output_x, uint32 *output_y, uint32 *output_z)
{
    uint32 result;
    uint32 coefficient;

    if (r_u32(matrix) == 0x1000 && r_s16(matrix + 4) == 0)
        *output_x = x;
    else
    {
        result = (uint32)r_s16(matrix) * x;
        result += (uint32)r_s16(matrix + 2) * y;
        result += (uint32)r_s16(matrix + 4) * z;
        *output_x = (uint32)((sint32)result >> 12);
    }
    coefficient = (uint32)r_s16(matrix + 6);
    if (coefficient == 0)
    {
        if (r_u32(matrix + 8) == 0x1000)
        {
            *output_y = y;
            goto third_row;
        }
        coefficient = (uint32)r_s16(matrix + 6);
    }
    result = coefficient * x;
    result += (uint32)r_s16(matrix + 8) * y;
    result += (uint32)r_s16(matrix + 10) * z;
    *output_y = (uint32)((sint32)result >> 12);

third_row:
    if (r_u32(matrix + 12) == 0 && r_s16(matrix + 16) == 0x1000)
    {
        *output_z = z;
        return 0x1000;
    }
    result = (uint32)r_s16(matrix + 12) * x;
    result += (uint32)r_s16(matrix + 14) * y;
    result += (uint32)r_s16(matrix + 16) * z;
    result = (uint32)((sint32)result >> 12);
    *output_z = result;
    return result;
}
