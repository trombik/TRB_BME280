#include <Arduino.h>
#include <I2C.h>
#include <bme280.h>
#include "TRB_BME280_I2C_LIB_I2C.h"

#ifdef __cplusplus
extern "C" {
#endif

void
trb_bme280_delay_ms(const uint32_t period)
{
	uint32_t now = millis();
	/* do NOOP instead of delay(), which does not block */
	while (millis() < now + period) {
#if defined(ESP8266) || defined(ESP32)
		/* avoid watchdog timeout */
		yield();
#endif
	}
}

int8_t
trb_bme280_read(const uint8_t dev_id, const uint8_t reg_addr, uint8_t *reg_data, const uint16_t len)
{
	/* Read register */
	return I2c.read(dev_id, reg_addr, len, reg_data);
}

int8_t
trb_bme280_write(const uint8_t dev_id, const uint8_t reg_addr, uint8_t *reg_data, const uint16_t len)
{
    return I2c.write(dev_id, reg_addr, reg_data, len);
}

#ifdef __cplusplus
}
#endif
