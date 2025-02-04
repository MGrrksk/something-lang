#pragma once
#include <token.hpp>

class Lexer {
    char* start;
    char* curr;
    unsigned short line, col;
    bool canEndStmt;
    bool skipWhitespace();
    bool isKeyword(const char* keyword);
    char eat();
    bool nextIs(char c);
    bool isEOF();
    public:
        void reset(const char* src);
        Token next();
};