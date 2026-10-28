#include "t3_audio.h"
#include "psx_spu.h"
#include "psx_xa.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

typedef struct T3_FILE_AUDIO
{
    FILE *file;
    uint32 cursor;
    uint32 position;
    uint32 end;
    sint32 channel;
    uint8 file_number;
    sint32 loop;
    sint32 status;
    XaState decoder;
    sint16 pcm[XPORT_FMV_AUDIO_FRAMES * 2];
    uint32 pcm_frames;
    uint32 pcm_position;
} T3_FILE_AUDIO;

static T3_FILE_AUDIO file_audio;
static XportCleanup previous_cleanup;
static sint32 cleanup_installed;

static void t3_file_audio_cleanup(void)
{
    t3_file_audio_stop();
    if (previous_cleanup)
        previous_cleanup();
}

// Refill decoded PCM from the selected file under the existing audio lock
static sint32 t3_file_audio_refill(void)
{
    uint8 raw[2352];
    uint32 index;
    sint32 wrapped = 0;

    while (file_audio.status == 1) {
        if (file_audio.cursor >= file_audio.end) {
            if (file_audio.loop < 0) {
                file_audio.status = 2;
                return 0;
            }
            if (wrapped || fseek(file_audio.file, (long)((uint32)file_audio.loop * 2352), SEEK_SET) != 0) {
                file_audio.status = -1;
                return 0;
            }
            wrapped = 1;
            file_audio.cursor = (uint32)file_audio.loop;
            xa_reset(&file_audio.decoder);
        }
        if (xport_fread(raw, 1, sizeof(raw), file_audio.file) != sizeof(raw)) {
            file_audio.status = -1;
            return 0;
        }
        ++file_audio.cursor;
        if (file_audio.channel >= 0) {
            if (raw[15] != 2 || !(raw[18] & 4) || raw[16] != file_audio.file_number || raw[17] != file_audio.channel)
                continue;
            if (xa_decode_sector(&file_audio.decoder, raw, file_audio.pcm, XA_MAX_SECTOR_FRAMES, &file_audio.pcm_frames) != XA_OK ||
                !file_audio.pcm_frames) {
                file_audio.status = -1;
                return 0;
            }
        } else {
            for (index = 0; index < 1176; ++index)
                file_audio.pcm[index] = (sint16)((uint32)raw[index * 2] | ((uint32)raw[index * 2 + 1] << 8));
            file_audio.pcm_frames = 588;
        }
        file_audio.pcm_position = 0;
        return 1;
    }
    return 0;
}

static uint32 t3_file_audio_pcm(void *context, sint16 *output, uint32 frames)
{
    uint32 written = 0;
    (void)context;

    while (written < frames) {
        uint32 available;
        uint32 count;
        if (file_audio.pcm_position == file_audio.pcm_frames && !t3_file_audio_refill())
            break;
        available = file_audio.pcm_frames - file_audio.pcm_position;
        count = frames - written;
        if (count > available)
            count = available;
        memcpy(output + written * 2, file_audio.pcm + file_audio.pcm_position * 2, count * 2 * sizeof(*output));
        written += count;
        file_audio.pcm_position += count;
        if (file_audio.pcm_position == file_audio.pcm_frames)
            file_audio.position = file_audio.cursor;
    }
    return written;
}

void t3_file_audio_stop(void)
{
    spu_set_cd_pcm_source(NULL, NULL);
    xport_audio_lock();
    if (file_audio.file)
        fclose(file_audio.file);
    memset(&file_audio, 0, sizeof(file_audio));
    xport_audio_unlock();
}

sint32 t3_file_audio_prepare(const char *path, uint32 first_sector, uint32 end_sector)
{
    FILE *file;
    long size;
    uint32 sectors;
    sint32 result;

    if (!path || (uint64)first_sector * 2352 > LONG_MAX)
        return 0;
    file = xport_fopen(path, "rb");
    if (!file)
        return 0;
    result = fseek(file, 0, SEEK_END) == 0;
    size = result ? ftell(file) : -1;
    result = size > 0 && size % 2352 == 0;
    if (result) {
        sectors = (uint32)size / 2352;
        if (!end_sector)
            end_sector = sectors;
        result = end_sector <= sectors && first_sector < end_sector &&
            fseek(file, (long)(first_sector * 2352), SEEK_SET) == 0;
    }
    if (fclose(file) != 0)
        result = 0;
    return result;
}

sint32 t3_file_audio_play(const char *path, uint32 first_sector, uint32 end_sector, sint32 channel, sint32 loop_sector)
{
    FILE *file;
    long size;
    uint32 sectors;

    if (!path || channel < -1 || channel > 255 || (uint64)first_sector * 2352 > LONG_MAX)
        return 0;
    file = xport_fopen(path, "rb");
    if (!file)
        return 0;
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return 0;
    }
    size = ftell(file);
    if (size <= 0 || size % 2352) {
        fclose(file);
        return 0;
    }
    sectors = (uint32)size / 2352;
    if (!end_sector)
        end_sector = sectors;
    if (end_sector > sectors || first_sector >= end_sector || loop_sector < -1 ||
        (loop_sector >= 0 && (uint32)loop_sector >= end_sector) ||
        fseek(file, (long)(first_sector * 2352), SEEK_SET) != 0) {
        fclose(file);
        return 0;
    }
    t3_file_audio_stop();
    xport_audio_lock();
    file_audio.file = file;
    file_audio.cursor = first_sector;
    file_audio.position = first_sector;
    file_audio.end = end_sector;
    file_audio.channel = channel;
    file_audio.file_number = 1;
    file_audio.loop = loop_sector;
    file_audio.status = 1;
    xa_reset(&file_audio.decoder);
    xport_audio_unlock();
    if (!cleanup_installed) {
        previous_cleanup = xport_get_cleanup();
        xport_set_cleanup(t3_file_audio_cleanup);
        cleanup_installed = 1;
    }
    spu_set_cd_pcm_source(t3_file_audio_pcm, NULL);
    return 1;
}

sint32 t3_file_audio_status(uint32 *sector)
{
    sint32 status;
    xport_audio_lock();
    status = file_audio.status;
    if (sector)
        *sector = file_audio.position;
    xport_audio_unlock();
    return status;
}

sint32 t3_file_audio_resume_fmv(const char *path, uint32 first_sector, uint32 end_sector, sint32 channel)
{
    XportFmvAudioRemainder *audio;
    FILE *file;
    long bytes;
    uint32 cursor;
    sint32 idle;

    if (!path || channel < -1 || channel > 255 || first_sector >= end_sector ||
        (uint64)end_sector * 2352 > LONG_MAX)
        return 0;
    xport_audio_lock();
    idle = file_audio.file == NULL && file_audio.status == 0;
    xport_audio_unlock();
    if (!idle)
        return 0;
    file = xport_fopen(path, "rb");
    if (!file)
        return 0;
    bytes = fseek(file, 0, SEEK_END) == 0 ? ftell(file) : -1;
    if (bytes <= 0 || bytes % 2352 != 0 || end_sector > (uint32)bytes / 2352)
    {
        fclose(file);
        return 0;
    }
    audio = (XportFmvAudioRemainder *)malloc(sizeof(*audio));
    if (!audio)
    {
        fclose(file);
        return 0;
    }
    if (!xport_fmv_take_audio(audio) || (channel >= 0 && audio->channel != channel) ||
        audio->pcm_frames > XPORT_FMV_AUDIO_FRAMES || audio->next_sector > end_sector - first_sector)
    {
        free(audio);
        fclose(file);
        return 0;
    }
    cursor = first_sector + audio->next_sector;
    if (fseek(file, (long)(cursor * 2352), SEEK_SET) != 0)
    {
        free(audio);
        fclose(file);
        return 0;
    }
    xport_audio_lock();
    file_audio.file = file;
    file_audio.cursor = cursor;
    file_audio.position = cursor - (audio->pcm_frames != 0 && cursor > first_sector);
    file_audio.end = end_sector;
    file_audio.channel = audio->channel;
    file_audio.file_number = audio->file;
    file_audio.loop = -1;
    file_audio.decoder = audio->decoder;
    memcpy(file_audio.pcm, audio->pcm, audio->pcm_frames * 2 * sizeof(sint16));
    file_audio.pcm_frames = audio->pcm_frames;
    file_audio.pcm_position = 0;
    file_audio.status = 1;
    xport_audio_unlock();
    free(audio);
    if (!cleanup_installed)
    {
        previous_cleanup = xport_get_cleanup();
        xport_set_cleanup(t3_file_audio_cleanup);
        cleanup_installed = 1;
    }
    spu_set_cd_pcm_source(t3_file_audio_pcm, NULL);
    return 1;
}
