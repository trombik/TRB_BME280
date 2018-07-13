#include <stdint.h>
#include <bme280.h>
#include <TRB_BME280_common.h>

struct bme280_dev
bme280_create_i2c_dev(const uint8_t i2c_address, const struct bme280_settings settings)
{
	struct bme280_dev dev;

	dev.dev_id = i2c_address;
	dev.intf = BME280_I2C_INTF;
	dev.read = trb_bme280_read;
	dev.write = trb_bme280_write;
	dev.delay_ms = trb_bme280_delay_ms;
	dev.settings = settings;
	return dev;
}
