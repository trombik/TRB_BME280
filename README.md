# `TRB_BME280`

A driver for `BME280` IC with multiple I2C drivers.

## Choosing I2C implementation

Define one of macros below.

| Header file | Framework | Platform | Driver |
|-------------|-----------|----------|--------|
| `TRB_BME280_I2C_ESP_IDF` | `esp-idf` | `espressif32` | Native I2C API |
| `TRB_BME280_I2C_WIRE` | `arduino` | `espressif32`, `espressif8266`, `atmelavr` | `Wire` |
| `TRB_BME280_I2C_BRZO` | `arduino` | `espressif8266` | `Brzo I2C` |

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

TBW

## Unit testing

See `[UNIT_TEST.md](UNIT_TEST.md)`.
