#ifndef T3_DATA_H
#define T3_DATA_H

#include "xport.h"

/* Read a BNS-relative byte range into a bounded native buffer */
sint32 t3_bns_read(uint32 sector, uint32 offset, void *destination, uint32 bytes);
sint32 t3_data_init(void);
sint32 t3_data_series_available(uint32 kind);
sint32 t3_data_select_series(uint32 kind);
sint32 t3_bns_pump(void);

#endif
