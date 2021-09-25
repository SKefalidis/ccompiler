# ccompiler

A C-subset compiler based on Nora Sandler's series 'Writing a C Compiler' (https://norasandler.com/ and https://github.com/nlsandler/write_a_c_compiler).
Current additional features include:
- Support for prefix `++` and `--` operators.
- Support for `+=,` `-=`, `*=`, `/=` and `%=` operators.

## Grammar:

```
<program>           ::= { <function> | <declaration> }

<function>          ::= "int" <id> "(" [ "int" <id> { "," "int" <id> } ] ")" ( "{" { <block-item> } "}" | ";" )

<function-call>     ::= id "(" [ <exp> { "," <exp> } ] ")"

<block-item>        ::= <statement>
                    |   <declaration>

<declaration>       ::= "int" <id> [ = <exp> ] ";"

<statement>         ::= "return" <exp> ";"
                    |   <exp-option> ";"
                    |   "if" "(" <exp> ")" <statement> [ "else" <statement> ]
                    |   "{" { <block-item> } "}
                    |   "for" "(" <exp-option> ";" <exp-option> ";" <exp-option> ")" <statement>
                    |   "for" "(" <declaration> <exp-option> ";" <exp-option> ")" <statement>
                    |   "while" "(" <exp> ")" <statement>
                    |   "do" <statement> "while" "(" <exp> ")" ";"
                    |   "break" ";"
                    |   "continue" ";"

<exp-option>        ::= <exp>
                    |   ""

<exp>               ::= <id> "=" <exp>
                    |   <conditional-exp>

<conditional-exp>   ::= <logical-or-exp> [ "?" <exp> ":" <conditional-exp> ]

<logical-or-exp>    ::= <logical-and-exp> { "||" <logical-and-exp> }

<logical-and-exp>   ::= <equality-exp> { "&&" <equality-exp> }

<equality-exp>      ::= <relational-exp> { ("!=" | "==") <relational-exp> }

<relational-exp>    ::= <additive-exp> { ("<" | ">" | "<=" | ">=") <additive-exp> }

<additive-exp>      ::= <term> { ("+" | "-") <term> }

<term>              ::= <factor> { ("*" | "/") <factor> }

<factor>            ::= "(" <exp> ")"
                    |   <unary_op> <factor>
                    |   <int>
                    |   <id>
                    |   <function-call>

<unary_op>          ::= "!"
                    |   "~"
                    |   "-"
```