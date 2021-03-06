Import("env")
src_filter = ["+<TRB_BME280.h>"]
env.Replace(SRC_FILTER=src_filter)

#print env.Dump()
build_flags = env.ParseFlags(env['BUILD_FLAGS'])
cppdefines = build_flags.get("CPPDEFINES")

if "TRB_BME280_I2C_ESP_IDF" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_BME280.c>"])
if "TRB_BME280_I2C_BRZO" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_BME280.cpp>"])
if "TRB_BME280_I2C_WIRE" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_BME280.cpp>"])
if "TRB_BME280_I2C_LIB_I2C" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_BME280.cpp>"])
if "TRB_BME280_I2C_TINYWIREM" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_BME280.cpp>"])
print env.get("SRC_FILTER")
