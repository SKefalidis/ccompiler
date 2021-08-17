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
    additiveexpression.h additiveexpression.cpp
    return.h return.cpp
    literal.h literal.cpp
    visitor.h visitor.cpp
    generatorvisitor.h generatorvisitor.cpp
    unaryoperator.h unaryoperator.cpp
    term.h term.cpp
    factor.h factor.cpp
    binaryop.h binaryop.cpp
    node.h node.cpp
    relationalexpression.h relationalexpression.cpp
    equalityexpression.h equalityexpression.cpp
    andexpression.h andexpression.cpp
    orexpression.h orexpression.cpp
    expression.h expression.cpp
    )
