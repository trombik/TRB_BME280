#if !defined(_TRB_BME280_common_h)
#define _TRB_BME280_common_h
#include "bme280.h"

#ifdef __cplusplus
extern "C" {
#endif

struct bme280_dev
trb_bme280_create_i2c_dev(const uint8_t i2c_address, const struct bme280_settings settings);

void
trb_bme280_delay_ms(const uint32_t period);

int8_t
trb_bme280_i2c_read(const uint8_t dev_id, const uint8_t reg_addr, uint8_t *reg_data, const uint16_t len);

int8_t
trb_bme280_i2c_write(const uint8_t dev_id, const uint8_t reg_addr, uint8_t *reg_data, const uint16_t len);

#ifdef __cplusplus
}
#endif

#endif // _TRB_BME280_common_h
