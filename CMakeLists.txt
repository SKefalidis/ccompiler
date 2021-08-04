cmake_minimum_required(VERSION 3.5)

project(ccompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ccompiler
    main.cpp
    lexer.h lexer.cpp
    token.h token.cpp
    parser.h parser.cpp
    goal.h goal.cpp
    function.h function.cpp
    statement.h statement.cpp
    expression.h expression.cpp
    return.h return.cpp
    literal.h literal.cpp
    )
