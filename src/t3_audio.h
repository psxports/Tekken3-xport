#ifndef T3_AUDIO_H
#define T3_AUDIO_H

#include "xport.h"

sint32 t3_file_audio_play(const char *path, uint32 first_sector, uint32 end_sector, sint32 channel, sint32 loop_sector);
sint32 t3_file_audio_prepare(const char *path, uint32 first_sector, uint32 end_sector);
void t3_file_audio_stop(void);
sint32 t3_file_audio_status(uint32 *sector);
// A channel of -1 accepts the XA identity returned by the player
sint32 t3_file_audio_resume_fmv(const char *path, uint32 first_sector, uint32 end_sector, sint32 channel);

#endif
