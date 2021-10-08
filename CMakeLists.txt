cmake_minimum_required(VERSION 3.5)

project(ccompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ccompiler
    README.md
    src/main.cpp
    src/lexer.h src/lexer.cpp
    src/token.h src/token.cpp
    src/parser.h src/parser.cpp
    src/blockgenerator.h src/blockgenerator.cpp
    src/basicblock.h src/basicblock.cpp
    src/instruction.h src/instruction.cpp
    src/peepholeoptimizer.h src/peepholeoptimizer.cpp
    src/controlflowgraph.h src/controlflowgraph.cpp
    src/goal.h src/goal.cpp
    src/functiondeclaration.h src/functiondeclaration.cpp
    src/functioncall.h src/functioncall.cpp
    src/statement.h src/statement.cpp
    src/additiveexpression.h src/additiveexpression.cpp
    src/literal.h src/literal.cpp
    src/visitor.h src/visitor.cpp
    src/generatorvisitor.h src/generatorvisitor.cpp
    src/functiondeclarationvisitor.h src/functiondeclarationvisitor.cpp
    src/unaryoperator.h src/unaryoperator.cpp
    src/term.h src/term.cpp
    src/factor.h src/factor.cpp
    src/node.h src/node.cpp
    src/relationalexpression.h src/relationalexpression.cpp
    src/equalityexpression.h src/equalityexpression.cpp
    src/andexpression.h src/andexpression.cpp
    src/orexpression.h src/orexpression.cpp
    src/expression.h src/expression.cpp
    src/expressionoptional.h src/expressionoptional.cpp
    src/blockitem.h src/blockitem.cpp
    src/declaration.h src/declaration.cpp
    src/condexpression.h src/condexpression.cpp
    src/type.h src/type.cpp
    )
