#if !defined(_TRB_BME280_h)

#if defined(TRB_BME280_I2C_ESP_IDF)
#include "TRB_BME280_I2C_ESP_IDF.c"

#elif defined(TRB_BME280_I2C_BRZO)
#include "TRB_BME280_I2C_brzo.c"

#endif

#include "TRB_BME280_common.c"

#endif // !defined(_TRB_BME280_h)
