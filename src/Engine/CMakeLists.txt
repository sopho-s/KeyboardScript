add_subdirectory(Lexer)
add_subdirectory(Logging)
add_subdirectory(TreeGen)
add_library(Engine Engine.cpp Engine.h)
target_link_libraries(Engine
    PRIVATE
    Lexer
    ArgsParse
    TreeGen)