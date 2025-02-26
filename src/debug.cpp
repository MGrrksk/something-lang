#include <debug.hpp>

void logToken(const Token& token) {
    switch (token.type) {
        case TT_LPAREN: printf("LPAREN"); break;
        case TT_RPAREN: printf("RPAREN"); break;
        case TT_LBRACK: printf("LBRACK"); break;
        case TT_RBRACK: printf("RBRACK"); break;
        case TT_LBRACE: printf("LBRACE"); break;
        case TT_RBRACE: printf("RBRACE"); break;
        case TT_COMMA: printf("COMMA"); break;
        case TT_DOT: printf("DOT"); break;
        case TT_PLUS: printf("PLUS"); break;
        case TT_MINUS: printf("MINUS"); break;
        case TT_STAR: printf("STAR"); break;
        case TT_DBLSTAR: printf("DBLSTAR"); break;
        case TT_SLASH: printf("SLASH"); break;
        case TT_DBLSLASH: printf("DBLSLASH"); break;
        case TT_PERCENT: printf("PERCENT"); break;
        case TT_EQUAL: printf("EQUAL"); break;
        case TT_DBLEQUAL: printf("DBLEQUAL"); break;
        case TT_BANG: printf("BANG"); break;
        case TT_BNGEQUAL: printf("BNGEQUAL"); break;
        case TT_GREATER: printf("GREATER"); break;
        case TT_GTEQUAL: printf("GTEQUAL"); break;
        case TT_LESS: printf("LESS"); break;
        case TT_LSEQUAL: printf("LSEQUAL"); break;
        case TT_ID: printf("ID"); break;
        case TT_NUMBER: printf("NUMBER"); break;
        case TT_STRING: printf("STRING"); break;
        case TT_AND: printf("AND"); break;
        case TT_OR: printf("OR"); break;
        case TT_XOR: printf("XOR"); break;
        case TT_TRUE: printf("TRUE"); break;
        case TT_FALSE: printf("FALSE"); break;
        case TT_NOTHING: printf("NOTHING"); break;
        case TT_DO: printf("DO"); break;
        case TT_IF: printf("IF"); break;
        case TT_ELSE: printf("ELSE"); break;
        case TT_WHILE: printf("WHILE"); break;
        case TT_FOR: printf("FOR"); break;
        case TT_DEF: printf("DEF"); break;
        case TT_FN: printf("FN"); break;
        case TT_RETURN: printf("RETURN"); break;
        case TT_SEP: printf("SEP"); break;
        case TT_ERROR: printf("ERROR"); break;
        case TT_EOF: printf("EOF"); break;
    }
    // In these cases `token.lexeme` is nullptr or separate terminated string.
    if (token.type > TT_SEP || token.lexeme == nullptr)  printf(" `%s`[%d] %d:%d\n", token.lexeme, token.length, token.line, token.collumn);
    // Otherwise `token.lexeme` is an unterminated string.
    else printf(" `%.*s`[%d] %d:%d\n", token.length, token.lexeme, token.length, token.line, token.collumn);
}