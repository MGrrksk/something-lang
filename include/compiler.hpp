#pragma once
#include <lexer.hpp>
#include <chunk.hpp>

class Compiler {
    Lexer lexer;
    Chunk chunk;
    Token prev;
    Token next;
    Token& eat();
    bool nextIs(std::initializer_list<TokenType> types);
    void error(const Token& token, const char* msg, ...);
    // Recursive descent parser functions
    void expression();
    void equality();
    void comparison();
    void term();
    void factor();
    void exponent();
    void unary();
    void primary();
    public:
        bool hadError = false;
        void reset(const Lexer& lexer);
        Chunk compile();
};