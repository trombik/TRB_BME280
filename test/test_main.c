#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_log.h>
#include <unity.h>

#include "TRB_BME280.h"

static char component[] = "[TRB_BME280]";
uint16_t reg_value_16;
uint8_t reg_value_8;

esp_err_t
i2c_init()
{
	i2c_config_t i2c_config;

	i2c_config.mode = I2C_MODE_MASTER;
	i2c_config.sda_io_num = GPIO_SDA;
	i2c_config.scl_io_num = GPIO_SCL;
	i2c_config.sda_pullup_en = GPIO_PULLUP_DISABLE;
	i2c_config.scl_pullup_en = GPIO_PULLUP_DISABLE;
	i2c_config.master.clk_speed = 400000L; // 400KHz

	TEST_ASSERT_EQUAL_INT8(0, i2c_param_config(I2C_NUM_0, &i2c_config));
	TEST_ASSERT_EQUAL_INT8(0, i2c_driver_install(I2C_NUM_0, i2c_config.mode, 0, 0, 0));
	return 0;
}

TEST_CASE("i2c_init", component)
{
	TEST_ASSERT_EQUAL_INT8(0, i2c_init());
	TEST_ASSERT_EQUAL_INT8(0, i2c_driver_delete(I2C_NUM_0));
}

TEST_CASE("trb_bme280_read", component)
{
}

TEST_CASE("trb_bme280_write", component)
{
}
