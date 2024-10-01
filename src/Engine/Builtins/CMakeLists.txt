add_library(Builtins Builtins.cpp Builtins.h)
target_link_libraries(Builtins
    PRIVATE
    Logging
    Objects
    _String)