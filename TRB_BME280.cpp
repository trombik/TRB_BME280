#if !defined(_TRB_BME280_cpp)
#define _TRB_BME280_cpp

#if defined(TRB_BME280_I2C_WIRE)
#include "TRB_BME280_I2C_Wire.cpp"

#elif defined(TRB_BME280_I2C_BRZO)
#include "TRB_BME280_I2C_brzo.c"

#elif defined(TRB_BME280_I2C_LIB_I2C)
#include "TRB_BME280_I2C_LIB_I2C.cpp"

#else
#error I2C library is not chosen
#endif

#include "TRB_BME280_common.c"

#endif
