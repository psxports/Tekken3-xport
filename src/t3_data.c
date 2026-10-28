#include "t3_data.h"
#include "game_functions.h"
#include <limits.h>

static sint32 t3_data_file_size(const char *path, uint32 *size)
{
    FILE *file = xport_fopen(path, "rb");
    long length;
    sint32 result;

    if (!file)
        return 0;
    result = fseek(file, 0, SEEK_END) == 0;
    length = result ? ftell(file) : -1;
    if (fclose(file) != 0 || length <= 0 || (uint64)length > 0xFFFFFFFF)
        return 0;
    *size = (uint32)length;
    return 1;
}

static void t3_data_file_record(uint32 record, uint32 size, const char *name)
{
    uint32 index;

    // Keep the legacy location ABI at file-relative sector zero
    w_u32(record, 0x00000200);
    w_u32(record + 4, size);
    for (index = 0; index < 16; ++index) {
        uint8 value = (uint8)*name;
        w_u8(record + 8 + index, value);
        if (value)
            ++name;
    }
}

sint32 t3_data_init(void)
{
    uint32 bns_size;
    uint32 xas_size;

    if (!t3_data_file_size("DATA/TEKKEN3/TEKKEN3.BNS", &bns_size) ||
        !t3_data_file_size("DATA/TEKKEN3/TEKKEN3.XAS", &xas_size))
        return 0;
    if (bns_size % 2048 || xas_size % 2352)
        return 0;
    t3_data_file_record(0x800A05E0, bns_size, "TEKKEN3.BNS");
    t3_data_file_record(0x800A0670, xas_size, "TEKKEN3.XAS");
    return 1;
}

static sint32 t3_data_series_record(uint32 kind, uint32 *size, const char **name)
{
    static const char *const identities[] = {
        "DATA/TEKKEN1/SLUS_000.06",
        "DATA/TEKKEN2/SLUS_002.13",
        "DATA/TEKKEN3/SLUS_004.02"
    };
    static const char *const streams[] = {
        "DATA/TEKKEN2/TEKKEN2.XAS",
        "DATA/TEKKEN3/TEKKEN3.XAS"
    };
    uint32 identity_size;

    if (kind < 1 || kind > 3 || !t3_data_file_size(identities[kind - 1], &identity_size))
        return 0;
    if (kind == 1) {
        *size = identity_size;
        *name = "SLUS_000.06";
        return 1;
    }
    if (!t3_data_file_size(streams[kind - 2], size) || *size % 2352)
        return 0;
    *name = kind == 2 ? "TEKKEN2.XAS" : "TEKKEN3.XAS";
    return 1;
}

sint32 t3_data_series_available(uint32 kind)
{
    uint32 size;
    const char *name;

    return t3_data_series_record(kind, &size, &name);
}

sint32 t3_data_select_series(uint32 kind)
{
    uint32 size;
    const char *name;

    if (!t3_data_series_record(kind, &size, &name))
        return 0;
    t3_data_file_record(0x800A0640 + (kind - 1) * 24, size, name);
    w_u16(0x800A0638 + (kind - 1) * 2, 1);
    t3_8006acdc(kind);
    return 1;
}

sint32 t3_bns_read(uint32 sector, uint32 offset, void *destination, uint32 bytes)
{
    FILE *file;
    uint64 position;
    long length;
    sint32 result;

    if (!bytes)
        return 1;
    if (!destination)
        return 0;
    position = (uint64)sector * 2048 + offset;
    if (position > (uint64)LONG_MAX)
        return 0;
    file = xport_fopen("DATA/TEKKEN3/TEKKEN3.BNS", "rb");
    if (!file)
        return 0;
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return 0;
    }
    length = ftell(file);
    if (length < 0 || position > (uint64)length || bytes > (uint64)length - position) {
        fclose(file);
        return 0;
    }
    if (fseek(file, (long)position, SEEK_SET) != 0) {
        fclose(file);
        return 0;
    }
    result = xport_fread(destination, 1, bytes, file) == bytes;
    if (fclose(file) != 0)
        result = 0;
    return result;
}

sint32 t3_bns_pump(void)
{
    while (r_u8(0x800A069F)) {
        t3_8006c2a0(1);
        if (r_u16(0x800A05DA) && r_u16(0x800A05D8) == 9)
            return 0;
    }
    return 1;
}
