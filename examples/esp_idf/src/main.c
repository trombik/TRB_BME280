#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <driver/gpio.h>
#include <driver/i2c.h>
#include <esp_err.h>
#include <bme280.h>
#include <sdkconfig.h>

#include <TRB_BME280_ESP_IDF.h>

esp_err_t
i2c_init(const gpio_num_t sda, const gpio_num_t scl)
{
	i2c_config_t i2c_config;
	esp_err_t r;

	i2c_config.mode = I2C_MODE_MASTER;
	i2c_config.sda_io_num = sda;
	i2c_config.scl_io_num = scl;
	i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE; // TODO disable the pullups
	i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
	i2c_config.master.clk_speed = 1000000;

	r = i2c_param_config(I2C_NUM_0, &i2c_config);
	ESP_ERROR_CHECK(r);
	i2c_driver_install(I2C_NUM_0, i2c_config.mode, 0, 0, 0);
	ESP_ERROR_CHECK(r);
	return r;
}

void
task_measure(void * pvParameters)
{
	int8_t result;
	uint8_t settings_sel;
	static char log_tag[] = "measure";
	struct bme280_dev dev;
	struct bme280_data data;
	struct bme280_settings settings;

	ESP_LOGI(log_tag, "Initialinzing I2C...");
	i2c_init((gpio_num_t)GPIO_SDA, (gpio_num_t)GPIO_SCL);
	ESP_LOGI(log_tag, "I2C initialized.");

	settings.osr_h = BME280_OVERSAMPLING_1X;
	settings.osr_p = BME280_OVERSAMPLING_16X;
	settings.osr_t = BME280_OVERSAMPLING_2X;
	settings.filter = BME280_FILTER_COEFF_16;
	settings.standby_time = BME280_STANDBY_TIME_1_MS;

	dev = bme280_create_i2c_dev(I2C_ADDRESS_BME280, settings);

	result = bme280_init(&dev);
	if (result == BME280_OK) {
		ESP_LOGI(log_tag, "Chip ID: 0x%x", dev.chip_id);
		ESP_LOGI(log_tag, "Device ID (I2C address): 0x%x", dev.dev_id);
	} else {
		ESP_LOGI(log_tag, "Failed to bme280_init: %d", result);
		vTaskDelete(NULL);
	}
	settings_sel =
	    BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL |
	    BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
	result = bme280_set_sensor_settings(settings_sel, &dev);
	if (result != 0) {
		ESP_LOGE(log_tag, "Failed to bme280_set_sensor_settings()");
		vTaskDelete(NULL);
	}
	if (bme280_set_sensor_mode(BME280_FORCED_MODE, &dev) != 0) {
		ESP_LOGE(log_tag, "Failed to set sensor to BME280_FORCED_MODE");
		vTaskDelete(NULL);
	}
	vTaskDelay(100);

	for(;;) {
		if (bme280_get_sensor_data(BME280_ALL, &data, &dev) != 0) {
			ESP_LOGE(log_tag, "Failed to bme280_get_sensor_data()");
		} else {
			ESP_LOGI(log_tag, "Temp: %.2f", (float)data.temperature / 100);
			ESP_LOGI(log_tag, "Humi: %.2f", (float)data.humidity / 1024);
			ESP_LOGI(log_tag, "Pres: %.2f", (float)data.pressure / 1000);
		}

		if (bme280_set_sensor_mode(BME280_FORCED_MODE, &dev) != 0) {
			ESP_LOGE(log_tag, "Failed to set sensor to BME280_FORCED_MODE");
			vTaskDelay(1000 / portTICK_PERIOD_MS);
			continue;
		}
		vTaskDelay(LOOP_INTERVAL / portTICK_PERIOD_MS);
	}
}

void
app_main(void)
{

	static char LOG_TAG[] = "app_main";
	esp_chip_info_t chip_info;

	esp_chip_info(&chip_info);
	ESP_LOGI(LOG_TAG, "This is ESP32 chip with %d CPU cores, WiFi%s%s, silicon revision %d",
	    chip_info.cores,
	    (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
	    (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "",
	    chip_info.revision);

	ESP_LOGI(LOG_TAG, "%dMB %s flash\n",
	    spi_flash_get_chip_size() / (1024 * 1024),
	    (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
	TaskHandle_t handle_measure;
	xTaskCreate(task_measure,
	    "measure",
	    configMINIMAL_STACK_SIZE * 3,
	    NULL,
	    1,
	    &handle_measure);
	configASSERT(handle_measure);
	if (handle_measure == NULL) {
		ESP_LOGW(LOG_TAG, "Failed to create measure task");
		vTaskDelete(handle_measure);
	}

	ESP_LOGI(LOG_TAG, "ESP32 Measure starting up...");
	for(;;) {
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
