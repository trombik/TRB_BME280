#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_log.h>
#include <unity.h>

#include "TRB_BME280.h"

static char component[] = "[TRB_BME280]";
static uint8_t i2c_address = BME280_I2C_ADDR_PRIM;
static uint8_t soft_reset_word = 0xB6;
static uint8_t chip_id = BME280_CHIP_ID;
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
	TEST_ASSERT_EQUAL_INT8(0, i2c_init());
	TEST_ASSERT_EQUAL_INT8(0, trb_bme280_read(i2c_address, BME280_CHIP_ID_ADDR, &reg_value_8, 1));
	TEST_ASSERT_EQUAL_INT8(chip_id, reg_value_8);
	TEST_ASSERT_EQUAL_INT8(0, i2c_driver_delete(I2C_NUM_0));
}

TEST_CASE("trb_bme280_write", component)
{
	TEST_ASSERT_EQUAL_INT8(0, i2c_init());
	TEST_ASSERT_EQUAL_INT8(0, trb_bme280_write(i2c_address, BME280_RESET_ADDR, &soft_reset_word, 1));
	TEST_ASSERT_EQUAL_INT8(0, i2c_driver_delete(I2C_NUM_0));
}
