add_library(TreeGen TreeGen.cpp TreeGen.h)
target_link_libraries(TreeGen PRIVATE Lexer Logging Objects)