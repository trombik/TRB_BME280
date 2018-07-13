Import("env")
src_filter = ["+<TRB_*.[ch]>"]
env.Replace(SRC_FILTER=src_filter)
