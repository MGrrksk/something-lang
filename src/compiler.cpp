#include <compiler.hpp>
#include <opcodes.hpp>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>

void Compiler::reset(const Lexer& lexer) {
    this->lexer = lexer;
    chunk.code.clear();
    hadError = false;
    eat();
}
Chunk Compiler::compile() {
    expression();
    return chunk;
}

void Compiler::expression() {
    equality();
}
void Compiler::equality() {
    comparison();
    while (nextIs({TT_BNGEQUAL, TT_DBLEQUAL})) {
        TokenType oper = prev.type;
        comparison();
        switch (oper) {
            case TT_BNGEQUAL: chunk.code.push_back(OP_NEQ); break;
            case TT_DBLEQUAL: chunk.code.push_back(OP_EQ); break;
            default: break;
        }
    }
}
void Compiler::comparison() {
    term();
    while (nextIs({TT_GREATER, TT_GTEQUAL, TT_LESS, TT_LSEQUAL})) {
        TokenType oper = prev.type;
        term();
        switch (oper) {
            case TT_GREATER: chunk.code.push_back(OP_GT); break;
            case TT_GTEQUAL: chunk.code.push_back(OP_GTEQ); break;
            case TT_LESS: chunk.code.push_back(OP_LT); break;
            case TT_LSEQUAL: chunk.code.push_back(OP_LTEQ); break;
            default: break;
        }
    }
}
void Compiler::term() {
    factor();
    while (nextIs({TT_PLUS, TT_MINUS})) {
        TokenType oper = prev.type;
        factor();
        switch (oper) {
            case TT_PLUS: chunk.code.push_back(OP_ADD); break;
            case TT_MINUS: chunk.code.push_back(OP_SUB); break;
            default: break;
        }
    }
}
void Compiler::factor() {
    exponent();
    while (nextIs({TT_STAR, TT_SLASH, TT_DBLSLASH, TT_PERCENT})) {
        TokenType oper = prev.type;
        exponent();
        switch (oper) {
            case TT_STAR: chunk.code.push_back(OP_MULT); break;
            case TT_SLASH: chunk.code.push_back(OP_DIV); break;
            case TT_DBLSLASH: chunk.code.push_back(OP_IDIV); break;
            case TT_PERCENT: chunk.code.push_back(OP_MOD); break;
            default: break;
        }
    }
}
void Compiler::exponent() {
    unary();
    while (nextIs({TT_DBLSTAR})) {
        unary();
        chunk.code.push_back(OP_POW);
    }
}
void Compiler::unary() {
    if (nextIs({TT_BANG, TT_MINUS})) {
        TokenType oper = prev.type;
        unary();
        switch (oper) {
            case TT_BANG: chunk.code.push_back(OP_NOT); break;
            case TT_MINUS: chunk.code.push_back(OP_NEG); break;
            default: break;
        }
    } else primary();
}
void Compiler::primary() {
    if (nextIs({TT_NUMBER})) {
        double number = strtod(prev.lexeme, nullptr);
        if (number == 0) chunk.addOp(OP_0);
        else if (number == 1) chunk.addOp(OP_1);
        else chunk.addConst({number});
    }
    else if (nextIs({TT_TRUE})) chunk.addOp(OP_TRUE);
    else if (nextIs({TT_FALSE})) chunk.addOp(OP_FALSE);
    else if (nextIs({TT_NOTHING})) chunk.addOp(OP_NTH);
    else if (nextIs({TT_LPAREN})) {
        expression();
        if (!nextIs({TT_RPAREN})) error(next, "Expected `)` after expression.");
    } else error(next, "Expected expression.");
}

Token& Compiler::eat() {
    prev = next;
    next = lexer.next();
    return prev;
}
bool Compiler::nextIs(std::initializer_list<TokenType> types) {
    for (TokenType type : types)
        if (next.type == type) {
            eat();
            return true;
        }
    return false;
}
void Compiler::error(const Token& token, const char* msg, ...) {
    if (hadError) return;
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "\e[1;31mError[\e[1;35m%u:%u\e[1;31m]: \e[1;37m", token.line, token.collumn);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\e[0m\n");
    va_end(args);
    hadError = true;
}