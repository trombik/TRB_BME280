#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include <stdint.h>

#include "TRB_BME280_I2C_ESP_IDF.h"

#define ACK_CHECK_EN 0x1 /* I2C master will check ack from slave */

#ifdef __cplusplus
extern "C" {
#endif

void
trb_bme280_delay_ms(const uint32_t period)
{
	vTaskDelay(period / portTICK_PERIOD_MS);
}

int8_t
trb_bme280_i2c_read(const uint8_t dev_id, const uint8_t reg_addr, uint8_t *reg_data, const uint16_t len)
{
	int8_t result = 0;
	i2c_cmd_handle_t command;
	esp_err_t r;
	static char log_tag[] = "trb_bme280_i2c_read";

	command = i2c_cmd_link_create();

	/* I2C start */
	ESP_ERROR_CHECK(i2c_master_start(command));
	/* write I2C address with WRITE bit */
	ESP_ERROR_CHECK(i2c_master_write_byte(command,
	    (dev_id << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN));
	/* write register address */
	ESP_ERROR_CHECK(i2c_master_write_byte(command, reg_addr, ACK_CHECK_EN));
	/* repeated start */
	ESP_ERROR_CHECK(i2c_master_start(command));
	/* write I2C address with READ bit */
	ESP_ERROR_CHECK(i2c_master_write_byte(command,
	    (dev_id << 1) | I2C_MASTER_READ, ACK_CHECK_EN));
	/* when requested to read more than a byte, read (len - 1) byte
	* sequentially with ACK enabled */
	if (len > 1) {
		i2c_master_read(command, reg_data, len - 1, I2C_MASTER_ACK);
	}
	/* read the last byte with NACK */
	ESP_ERROR_CHECK(i2c_master_read_byte(command,
	    reg_data + len - 1, I2C_MASTER_NACK));
	/* I2C stop */
	ESP_ERROR_CHECK(i2c_master_stop(command));

	/* start the transaction */
	r = i2c_master_cmd_begin(I2C_NUM_0, command, 10 / portTICK_PERIOD_MS);
	result = r == ESP_OK ? 0 : 1;
	i2c_cmd_link_delete(command);
	if (result != 0) {
		ESP_LOGE(log_tag, "i2c_master_cmd_begin() failed: %d", result);
	}
	if (len > 1) {
		for (uint8_t i = 0; i < len; i++) {
		ESP_LOGD(log_tag, "Register: 0x%x Data: 0x%x len: %d index: %d",
		    reg_addr + i, reg_data[i], len, i);
		}
	} else {
		ESP_LOGD(log_tag, "Register: 0x%x Data: 0x%x len: %d",
		    reg_addr, *reg_data, len);
	}
	return result;
}

int8_t
trb_bme280_i2c_write(const uint8_t dev_id, const uint8_t reg_addr, uint8_t *reg_data, const uint16_t len)
{
	uint8_t i;
	i2c_cmd_handle_t command;
	esp_err_t r;
	static char log_tag[] = "trb_bme280_i2c_write";
	command = i2c_cmd_link_create();

	/* I2C start */
	ESP_ERROR_CHECK(i2c_master_start(command));
	/* write I2C address with WRITE bit */
	ESP_ERROR_CHECK(i2c_master_write_byte(command,
	    (dev_id << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN));
	/* write register address */
	ESP_ERROR_CHECK(i2c_master_write_byte(command, reg_addr, ACK_CHECK_EN));
	/* write data */
	ESP_ERROR_CHECK(i2c_master_write(command, reg_data, len, ACK_CHECK_EN));
	/* I2C stop */
	i2c_master_stop(command);
	/* start transaction */
	r = i2c_master_cmd_begin(I2C_NUM_0, command, 10 / portTICK_PERIOD_MS);
	i2c_cmd_link_delete(command);

	for (i = 0; i < len; i++) {
		ESP_LOGD(log_tag, "Register: %x Data: %x len: %d index: %d",
		    reg_addr, *reg_data, len, i);
	}
	if (r != ESP_OK) {
		ESP_LOGE(log_tag, "i2c_master_cmd_begin() failed: %d", r);
		return r;
	}
	/* without 10ms, at the least, the sensor returns invalid values */
	trb_bme280_delay_ms(10);
	return 0;
}

#ifdef __cplusplus
}
#endif
