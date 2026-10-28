#include "t3_sound.h"
#include "psx_spu.h"
#include "game_functions.h"

void t3_8008aa28(uint32 header, sint32 bank, uint32 sound_address)
{
    uint32 capacity = r_u8(header) == 112 && r_s32(header + 4) >= 5 ? 128 : 64;
    uint32 programs = r_u16(header + 18);
    uint32 bytes = programs <= capacity ? 32 + capacity * 16 + programs * 512 + 512 : 32;

    spu_vab_open_head_sticky((const uint8 *)psx_addr(header, bytes), bytes, (sint16)bank, sound_address);
}

void t3_8008ae58(uint32 body, sint32 bank)
{
    SpuVabNativeView view = {0};
    uint32 bytes;

    spu_vab_native_view((sint16)bank, &view);
    bytes = view.body_bytes > 0x7EFF0 ? 0x7EFF0 : view.body_bytes;
    spu_vab_trans_body_native((const uint8 *)psx_addr(body, bytes), bytes, (sint16)bank);
}

uint32 t3_800885d8(sint32 voice, sint32 bank, uint32 program, uint32 tone, uint32 note, uint32 fine, uint32 left, uint32 right)
{
    return (uint32)(sint32)SsUtKeyOnV((sint16)voice, (sint16)bank, (sint16)program, (sint16)tone,
        (sint16)note, (sint16)fine, (sint16)left, (sint16)right);
}

uint32 t3_80088938(sint32 voice)
{
    uint16 selected = (uint16)voice;

    xport_audio_lock();
    if (spu_sequence_scheduler.processing == 1 ||
        spu_vm_state()->immediate_key_on_busy == 1 || selected >= 24) {
        xport_audio_unlock();
        return 0xFFFFFFFF;
    }
    spu_vm_state()->note.voice = selected;
    spu_sequence_vm_key_off_now();
    xport_audio_unlock();
    return 0;
}

static uint32 t3_sound_delta(sint16 sequence, sint16 separation)
{
    return spu_sequence_scheduler_read_delta(&spu_sequence_scheduler, sequence, separation);
}

static uint32 t3_sound_next(sint16 sequence, sint16 separation)
{
    return spu_sequence_scheduler_next_separation(&spu_sequence_scheduler, sequence, separation);
}

static uint32 t3_sound_end(sint16 sequence, sint16 separation)
{
    return spu_sequence_scheduler_end_of_track(&spu_sequence_scheduler, sequence, separation,
        t3_sound_next, spu_sequence_vm_sequence_key_off);
}

static uint32 t3_sound_event(sint16 sequence, sint16 separation)
{
    return spu_sequence_scheduler_read_event(&spu_sequence_scheduler, sequence, separation,
        &spu_sequence_handlers, t3_sound_delta, t3_sound_end);
}

static uint32 t3_sound_play(sint16 sequence, sint16 separation)
{
    return spu_sequence_scheduler_play_tick(&spu_sequence_scheduler, sequence, separation,
        t3_sound_event);
}

static uint32 t3_sound_get_volume(sint16 handle, uint16 *left, uint16 *right)
{
    return spu_sequence_vm_get_sequence_volume(&spu_sequence_scheduler, handle, left, right);
}

static uint32 t3_sound_set_volume(sint16 handle, sint16 left, sint16 right, sint16 mode)
{
    return spu_sequence_vm_set_sequence_volume(&spu_sequence_scheduler, handle, left, right, mode);
}

static uint32 t3_sound_crescendo(sint16 sequence, sint16 separation)
{
    return spu_sequence_scheduler_crescendo(&spu_sequence_scheduler, sequence, separation,
        t3_sound_get_volume, t3_sound_set_volume);
}

static uint32 t3_sound_decrescendo(sint16 sequence, sint16 separation)
{
    return spu_sequence_scheduler_decrescendo(&spu_sequence_scheduler, sequence, separation,
        t3_sound_get_volume, t3_sound_set_volume);
}

static void t3_sound_tempo(sint16 sequence, sint16 separation)
{
    spu_sequence_scheduler_tempo_ramp(&spu_sequence_scheduler, sequence, separation);
}

static uint32 t3_sound_pause(sint16 sequence, sint16 separation)
{
    return spu_sequence_scheduler_pause(&spu_sequence_scheduler, sequence, separation,
        spu_sequence_vm_sequence_key_off);
}

static void t3_sound_resume(sint16 sequence, sint16 separation)
{
    spu_sequence_scheduler_resume(&spu_sequence_scheduler, sequence, separation);
}

static uint32 t3_sound_stop(sint16 sequence, sint16 separation)
{
    return spu_sequence_scheduler_stop(&spu_sequence_scheduler, sequence, separation,
        spu_sequence_vm_sequence_key_off);
}

void t3_sound_bind(void)
{
    static const SpuSequencePumpOps operations = {
        spu_sequence_vm_flush,
        t3_sound_play,
        t3_sound_crescendo,
        t3_sound_decrescendo,
        t3_sound_tempo,
        t3_sound_pause,
        t3_sound_resume,
        t3_sound_stop
    };

    spu_voice_modulation_callbacks.volume = spu_sequence_vm_volume_modulation_tick;
    spu_voice_modulation_callbacks.pitch = spu_sequence_vm_pan_modulation_tick;
    spu_sequence_bind_pump(&operations);
}
