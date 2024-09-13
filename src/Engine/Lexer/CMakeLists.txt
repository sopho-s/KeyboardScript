add_library(Lexer Lexer.cpp Lexer.h)
target_link_libraries(Lexer PRIVATE 
    Logging)