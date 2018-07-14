#if !defined(_TRB_BME280_h)

#if defined(TRB_BME280_I2C_ESP_IDF)
#include "TRB_BME280_I2C_ESP_IDF.h"

#elif defined(TRB_BME280_I2C_WIRE)
#include "TRB_BME280_I2C_Wire.h"

#elif defined(TRB_BME280_I2C_BRZO)
#include "TRB_BME280_I2C_brzo.h"

#elif defined(TRB_BME280_I2C_LIB_I2C)
#include "TRB_BME280_I2C_LIB_I2C.h"
#endif

#endif // !defined(_TRB_BME280_h)
