#define WITH_REPEATED_START false
#define WITHOUT_REPEATED_START true

#include <Arduino.h>
#include <TinyWireM.h>
#include <bme280.h>
#include "TRB_BME280_I2C_TinyWireM.h"

#ifdef __cplusplus
extern "C" {
#endif

void
user_delay_ms(const uint32_t period)
{
	uint32_t now = millis();
	/* do NOOP instead of delay(), which does not block */
	while (millis() < now + period) {}
}

int8_t
trb_bme280_read(const uint8_t dev_id, const uint8_t reg_addr, uint8_t *reg_data, const uint16_t len)
{
	int8_t result = 0;

	/* start transaction */
	TinyWireM.beginTransmission(dev_id);
	/* write register address */
	TinyWireM.write(reg_addr);
	/* read the values with repeated start, and end the transaction with stop
	* bit */
	result = TinyWireM.endTransmission(WITH_REPEATED_START);
	if (result != 0)
		return result;
	/* requestFrom() accepts I2C address and length, not the 3rd argument in
	* the latest requestFrom() in arduino */
	if (TinyWireM.requestFrom(dev_id, (size_t)len) != len)
		return -1;
	for (uint16_t i = 0; i <= len - 1; i++) {
		reg_data[i] = TinyWireM.read();
	}
	return result;
}

int8_t
trb_bme280_write(const uint8_t dev_id, const uint8_t reg_addr, uint8_t *reg_data, const uint16_t len)
{
	int8_t result = 0;

	/* start transaction */
	TinyWireM.beginTransmission(dev_id);
	/* write register address */
	TinyWireM.write(reg_addr);
	/* write values */
	TinyWireM.write(reg_data, len);
	/* end end transaction */
	result = TinyWireM.endTransmission();
	user_delay_ms(10);
	return result;
}

#ifdef __cplusplus
}
#endif
