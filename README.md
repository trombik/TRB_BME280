# `TRB_BME280`

A driver for `BME280` IC with multiple I2C drivers.

## Choosing I2C implementation

Define one of variables below. The variable must be defined as build flag,
not in the code.

| Variable | Framework | Platform | Driver |
|-------------|-----------|----------|--------|
| `TRB_BME280_I2C_ESP_IDF` | `esp-idf` | `espressif32` | [Native I2C API](https://esp-idf.readthedocs.io/en/latest/api-reference/peripherals/i2c.html) |
| `TRB_BME280_I2C_WIRE` | `arduino` | `espressif32`, `espressif8266`, `atmelavr` | [`Wire`](https://www.arduino.cc/en/Reference/Wire) |
| `TRB_BME280_I2C_BRZO` | `arduino` | `espressif8266` | [`Brzo I2C`](https://github.com/pasko-zh/brzo_i2c) |
| `TRB_BME280_I2C_LIB_I2C` | `arduino` | `atmelavr` | [`I2C-Master-Library`](https://github.com/DSSCircuits/I2C-Master-Library) |

`platformio` example for `espressif32`.

```ini
[env:lolin32_espidf]
board = lolin32
platform = espressif32
framework = espidf
lib_deps = TRB_BME280
build_flags = -D TRB_BME280_I2C_ESP_IDF
```

## Usage

```c
#include <TRB_BME280.h>

# initialize I2C bus here

int8_t result;
uint8_t settings_sel;
struct bme280_dev dev;
struct bme280_data data;
struct bme280_settings settings;

settings.osr_h = BME280_OVERSAMPLING_1X;
settings.osr_p = BME280_OVERSAMPLING_16X;
settings.osr_t = BME280_OVERSAMPLING_2X;
settings.filter = BME280_FILTER_COEFF_16;
settings.standby_time = BME280_STANDBY_TIME_1_MS;
dev = trb_bme280_create_i2c_dev(I2C_ADDRESS_BME280, settings);

result = bme280_init(&dev);
settings_sel =
    BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL |
    BME280_OSR_HUM_SEL | BME280_FILTER_SEL;
result = bme280_set_sensor_settings(settings_sel, &dev);
result = bme280_set_sensor_mode(BME280_FORCED_MODE, &dev);
result = bme280_get_sensor_data(BME280_ALL, &data, &dev);

# do something with data.temperature, data.humidity, and data.pressure...
```

For more details, see [bme280.h](https://github.com/BoschSensortec/BME280_driver/blob/master/bme280.h).

Examples can be found under [examples](examples).

## Unit testing

See `[UNIT_TEST.md](UNIT_TEST.md)`.
