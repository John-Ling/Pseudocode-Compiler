cmake_minimum_required(VERSION 3.0.0)
project(Pseudocode-Compiler VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Pseudocode-Compiler src/compiler.cpp include/compiler.h src/lexer.cpp include/lexer.h src/parser.cpp include/parser.h include/parser_nodes.h include/constants.h include/token.h src/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
