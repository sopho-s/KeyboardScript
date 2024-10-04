add_library(sparse spp_config.h spp_dlalloc.h spp_memory.h spp_smartptr.h spp_stdint.h spp_timer.h spp_traits.h spp.h)
set_target_properties(sparse PROPERTIES LINKER_LANGUAGE CXX)