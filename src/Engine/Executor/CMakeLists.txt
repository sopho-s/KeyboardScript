add_library(Executor Executor.cpp Executor.h)
target_link_libraries(Executor
    PRIVATE
    Lexer
    Logging
    TreeGen
    Objects
    Builtins
    _String
    tsl)