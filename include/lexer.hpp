#pragma once
#include <token.hpp>
#include <stack.hpp>

// Lexical analysis class.
// Returns tokens one by one (not all at once).
class Lexer {
    char* start;
    char* curr;
    unsigned short line, col;
    bool canEndStmt;
    // Stack of enclosing parentheses. Bool value detrmines whether statement can end in it.
    // (e.g. in square brackets `[]` newline does not end statement (`false`), but in braces `{}` it does (`true`).)
    Stack<bool, 64> parenStack;
    unsigned short skipWhitespace();
    bool isKeyword(const char* keyword);
    char eat();
    bool nextIs(char c);
    bool isEOF();
    public:
        void reset(const char* src);
        Token next();
};