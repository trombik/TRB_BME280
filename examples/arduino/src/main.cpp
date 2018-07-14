#include <Arduino.h>
#include <TRB_BME280.h>

#if defined(TRB_BME280_I2C_BRZO)
#include <brzo_i2c.h>
#elif defined(TRB_BME280_I2C_LIB_TINYWIREM)
#include <TinyWireM.h>
#elif defined(TRB_BME280_I2C_LIB_I2C)
#include <I2C.h>
#define I2C_FAST 1
#elif defined(TRB_BME280_I2C_WIRE)
#include <Wire.h>
#else
#error I2C library is not defined
#endif

#if defined(ESP8266)
extern "C" {
#include <user_interface.h>
}
#endif

#if defined(ARDUINO_AVR_ATTINYX5)
#include "TinyDebugSerial.h"
TinyDebugSerial mySerial = TinyDebugSerial();
#define Serial mySerial
#endif

struct bme280_dev dev;
struct bme280_data data;

void
halt()
{
	Serial.println(F("Halting. Please reset"));
	while (1) {
#if !defined(ARDUINO_AVR_ATTINYX5)
		yield();
#endif
	}
}

char *
float2string(float f)
{
	/* -ddddd.dd */
	/* 5 digits + 2 digits + 1 character for `-` + 1 character for `.` 9
	 * characters in total. the buffer needs another byte for NULL
	 * terminater. minimum buffer size is 10. */
	static char buffer[10];

	/* as it cannot handle more than 100000, return invalid strings, as in
	 * float */
	if (f >= 100000 || f <= -100000) {
		strlcpy(buffer, "-XXXXX.XX", sizeof(buffer));
	} else {
		dtostrf(f, 9, 2, buffer);
	}
	return buffer;
}

void
setup()
{
	int8_t result;
	uint8_t settings_sel;
	struct bme280_settings settings;

	Serial.begin(115200);
#if !defined(ARDUINO_AVR_ATTINYX5)
	while (!Serial) {};
#endif
	Serial.println();

#if defined(ESP8266)
	Serial.print(F("system_get_chip_id(): "));
	Serial.println(system_get_chip_id(), HEX);
	Serial.print(F("system_get_sdk_version(): "));
	Serial.println(system_get_sdk_version());
#endif

#if defined(TRB_BME280_I2C_BRZO)
	Serial.println(F("Initializing I2C (brzo)"));
	brzo_i2c_setup(GPIO_SDA, GPIO_SCL, 2000); /* clock stretching timeout of 2000 usec */
#elif defined(TRB_BME280_I2C_LIB_TINYWIREM)
	Serial.println(F("Initializing I2C (TinyWireM)"));
	TinyWireM.begin();
#elif defined(TRB_BME280_I2C_LIB_I2C)
	Serial.println(F("Initializing I2C (I2c)"));
	I2c.begin();
#else
#if defined(ESP32)
	Serial.println(F("Initializing I2C (Wire for ESPs)"));
	Wire.begin(GPIO_SDA, GPIO_SCL, 400000L);
#elif defined(ESP8266)
	Serial.println(F("Initializing I2C (Wire for ESPs)"));
	Wire.begin(GPIO_SDA, GPIO_SCL);
#else
	Serial.println(F("Initializing I2C (Wire)"));
	Wire.begin();
#endif
#endif

	settings.osr_h = BME280_OVERSAMPLING_1X;
	settings.osr_p = BME280_OVERSAMPLING_16X;
	settings.osr_t = BME280_OVERSAMPLING_2X;
	settings.filter = BME280_FILTER_COEFF_16;
	settings.standby_time = BME280_STANDBY_TIME_1_MS;

	dev = bme280_create_i2c_dev(I2C_ADDRESS_BME280, settings);
	/* set SCL freq to 400KHz */
#if defined(TRB_BME280_I2C_BRZO)
	bme280_brzo_set_scl_freq(400);
#elif defined(TRB_BME280_I2C_LIB_I2C)
	I2c.setSpeed(I2C_FAST);
#elif !defined(ESP32) && !defined(TRB_BME280_I2C_LIB_TINYWIREM)
	Wire.setClock(400000L);
#endif
	result = bme280_init(&dev);
	if (result == BME280_OK) {
		Serial.print(F("Chip ID: 0x"));
		Serial.println(dev.chip_id, HEX);
		Serial.print(F("Device ID (I2C address): 0x"));
		Serial.println(dev.dev_id, HEX);
	} else {
		Serial.print(F("Failed to bme280_init(): "));
		Serial.println(result);
		halt();
	}
	settings_sel =
	    BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL |
	    BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
	result = bme280_set_sensor_settings(settings_sel, &dev);
	if (result != 0) {
		Serial.println(F("Failed to bme280_set_sensor_settings()"));
		halt();
	}
	if (bme280_set_sensor_mode(BME280_FORCED_MODE, &dev) != 0) {
		Serial.println(F("Failed to set sensor to BME280_FORCED_MODE"));
		halt();
	}
	trb_bme280_delay_ms(100);
}

void
loop()
{
	if (bme280_get_sensor_data(BME280_ALL, &data, &dev) != 0) {
		Serial.println(F("Failed to bme280_get_sensor_data()"));
	} else {
#if defined(ARDUINO_AVR_ATTINYX5)
		/* avoid float to keep the binary size within 8KB.
		 * Program:    7640 bytes (93.3% Full) */
		Serial.print(data.temperature / 100);
		Serial.println(F(" degree Celsius"));
		Serial.print(data.humidity / 1024);
		Serial.println(F("%"));
		Serial.print(data.pressure / 1000);
		Serial.println(F("Pa"));
#else
		Serial.print(F("Temp: "));
		Serial.print(float2string((float)data.temperature / 100));
		Serial.println(F(" degree Celsius"));
		Serial.print(F("Humi: "));
		Serial.print(float2string((float)data.humidity / 1024));
		Serial.println(F("%"));
		Serial.print(F("Pres: "));
		Serial.print(float2string((float)data.pressure / 1000));
		Serial.println(F("Pa"));
#endif
	}
	if (bme280_set_sensor_mode(BME280_FORCED_MODE, &dev) != 0)
		Serial.println(F("Failed to set sensor to BME280_FORCED_MODE"));
	trb_bme280_delay_ms(LOOP_INTERVAL);
}
