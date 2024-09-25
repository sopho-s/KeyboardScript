add_subdirectory(Logging)
add_subdirectory(Objects)
add_subdirectory(Lexer)
add_subdirectory(TreeGen)
add_subdirectory(Builtins)
add_subdirectory(Executor)
add_library(Engine Engine.cpp Engine.h)
target_link_libraries(Engine
    PRIVATE
    TreeGen
    Lexer
    ArgsParse
    Objects
    Builtins
    Executor)