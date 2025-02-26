#pragma once

enum TokenType {
    TT_LPAREN,
    TT_RPAREN,
    TT_LBRACK,
    TT_RBRACK,
    TT_LBRACE,
    TT_RBRACE,
    TT_COMMA,
    TT_DOT,
    TT_PLUS,
    TT_MINUS,
    TT_STAR,
    TT_DBLSTAR,
    TT_SLASH,
    TT_DBLSLASH,
    TT_PERCENT,
    TT_EQUAL,
    TT_DBLEQUAL,
    TT_BANG,
    TT_BNGEQUAL,
    TT_GREATER,
    TT_GTEQUAL,
    TT_LESS,
    TT_LSEQUAL,
    TT_ID,
    TT_NUMBER,
    TT_STRING,
    TT_AND,
    TT_OR,
    TT_XOR,
    TT_TRUE,
    TT_FALSE,
    TT_NOTHING,
    TT_DO,
    TT_IF,
    TT_ELSE,
    TT_WHILE,
    TT_FOR,
    TT_DEF,
    TT_FN,
    TT_RETURN,
    TT_SEP,
    TT_ERROR,
    TT_EOF
};

struct Token {
    TokenType type;
    const char* lexeme; // Unterminated (no '\0' at the end), points to original source string
    unsigned short length;
    unsigned short line;
    unsigned short collumn;
};