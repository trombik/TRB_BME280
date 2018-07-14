Import("env")
src_filter = ["+<TRB_BME280.[ch]>", "+<TRB_BME280_common.[ch]>"]
env.Replace(SRC_FILTER=src_filter)

#print env.Dump()
build_flags = env.ParseFlags(env['BUILD_FLAGS'])
cppdefines = build_flags.get("CPPDEFINES")

if "TRB_BME280_I2C_ESP_IDF" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_BME280_I2C_ESP_IDF.[ch]>"])
if "TRB_BME280_I2C_WIRE" in cppdefines:
    env.Append(SRC_FILTER=["+<TRB_BME280_I2C_Wire.h>", "+<TRB_BME280_I2C_Wire.cpp>"])
print env.get("SRC_FILTER")
