add_library(Objects Objects.cpp Objects.h)
target_link_libraries(Objects PRIVATE 
    Lexer)