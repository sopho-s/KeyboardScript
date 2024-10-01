project(KeyboardScript)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(Engine)

add_executable(Key main.cpp)

add_library(ArgsParse ArgsParse.h ArgsParse.cpp)

target_link_libraries(Key
    PUBLIC
    Logging
    Lexer
    Engine
    ArgsParse
    TreeGen)