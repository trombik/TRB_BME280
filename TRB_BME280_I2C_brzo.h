#ifndef _TRB_BME280_brzo_h
#define _TRB_BME280_brzo_h

#include "TRB_BME280_common.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t
bme280_brzo_get_scl_freq();

uint16_t
bme280_brzo_set_scl_freq(const uint16_t freq);

#ifdef __cplusplus
}
#endif

#endif
