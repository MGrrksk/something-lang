#include <lexer.hpp>
#include <cctype>
#include <cstring>

void Lexer::reset(const char* src) {
    start = curr = (char*)src;
    line = col = 1;
}
Token Lexer::next() {
    if (skipWhitespace()) return {TT_SEP, nullptr, 1, line, col};
    if (isEOF()) {
        if (canEndStmt) {
            canEndStmt = false;
            return {TT_SEP, nullptr, 1, line, col};
        } else return {TT_EOF, nullptr, 1, line, col};
    }
    start = curr;
    switch (eat()) {
        case '(': canEndStmt = false; return {TT_LPAREN, start, 1, line, (unsigned short)(col - (curr - start))};
        case ')': canEndStmt = true; return {TT_RPAREN, start, 1, line, (unsigned short)(col - (curr - start))};
        case '[': canEndStmt = false; return {TT_LBRACK, start, 1, line, (unsigned short)(col - (curr - start))};
        case ']': canEndStmt = true; return {TT_RBRACK, start, 1, line, (unsigned short)(col - (curr - start))};
        case '{': canEndStmt = false; return {TT_LBRACE, start, 1, line, (unsigned short)(col - (curr - start))};
        case '}': canEndStmt = false; return {TT_RBRACE, start, 1, line, (unsigned short)(col - (curr - start))};
        case ',': canEndStmt = false; return {TT_COMMA, start, 1, line, (unsigned short)(col - (curr - start))};
        case '.': canEndStmt = false; return {TT_DOT, start, 1, line, (unsigned short)(col - (curr - start))};
        case '+': canEndStmt = false; return {TT_PLUS, start, 1, line, (unsigned short)(col - (curr - start))};
        case '-': canEndStmt = false; return {TT_MINUS, start, 1, line, (unsigned short)(col - (curr - start))};
        case '*': canEndStmt = false; return nextIs('*') ? Token{TT_DBLSTAR, start, 2, line, (unsigned short)(col - (curr - start))} : Token{TT_STAR, start, 1, line, (unsigned short)(col - (curr - start))};
        case '/': canEndStmt = false; return nextIs('/') ? Token{TT_DBLSLASH, start, 2, line, (unsigned short)(col - (curr - start))} : Token{TT_SLASH, start, 1, line, (unsigned short)(col - (curr - start))};
        case '%': canEndStmt = false; return {TT_PERCENT, start, 1, line, (unsigned short)(col - (curr - start))};
        case '!': canEndStmt = false; return nextIs('=') ? Token{TT_BNGEQUAL, start, 2, line, (unsigned short)(col - (curr - start))} : Token{TT_BANG, start, 1, line, (unsigned short)(col - (curr - start))};
        case '=': canEndStmt = false; return nextIs('=') ? Token{TT_DBLEQUAL, start, 2, line, (unsigned short)(col - (curr - start))} : Token{TT_EQUAL, start, 1, line, (unsigned short)(col - (curr - start))};
        case '>': canEndStmt = false; return nextIs('=') ? Token{TT_GTEQUAL, start, 2, line, (unsigned short)(col - (curr - start))} : Token{TT_GREATER, start, 1, line, (unsigned short)(col - (curr - start))};
        case '<': canEndStmt = false; return nextIs('=') ? Token{TT_LSEQUAL, start, 2, line, (unsigned short)(col - (curr - start))} : Token{TT_LESS, start, 1, line, (unsigned short)(col - (curr - start))};
        case '"': {
            canEndStmt = true;
            while (!isEOF() && *curr != '"') {
                if (*curr == '\n') {
                    col = 1;
                    line++;
                }
                eat();
            }
            if (isEOF()) return {TT_ERROR, "Unterminated string literal.", (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};
            eat();
            return {TT_STRING, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};
        }
        default: {
            if (isdigit(*start)) {
                canEndStmt = true;
                while (isdigit(*curr)) eat();
                if (*curr == '.' && isdigit(*(curr + 1))) {
                    eat();
                    while (isdigit(*curr)) eat();
                }
                return {TT_NUMBER, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};
            } else if (isalpha(*start) || *start == '_') {
                canEndStmt = true;
                while (isalnum(*curr) || *curr == '_') eat();
                switch (*start) {
                    case 'a': if (isKeyword("nd")) {canEndStmt = false; return {TT_AND, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};} break;
                    case 'd':
                        if (isKeyword("ef")) {canEndStmt = false; return {TT_DEF, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};}
                        else if (isKeyword("o")) {canEndStmt = false; return {TT_DO, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};}
                        break;
                    case 'e': if (isKeyword("lse")) {canEndStmt = false; return {TT_ELSE, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};} break;
                    case 'f':
                        if (isKeyword("alse")) {canEndStmt = true; return {TT_FALSE, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};}
                        else if (isKeyword("n")) {canEndStmt = false; return {TT_FN, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};}
                        else if (isKeyword("or")) {canEndStmt = false; return {TT_FOR, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};}
                        break;
                    case 'i': if (isKeyword("f")) {canEndStmt = false; return {TT_IF, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};} break;
                    case 'n': if (isKeyword("othing")) {canEndStmt = true; return {TT_NOTHING, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};} break;
                    case 'o': if (isKeyword("r")) {canEndStmt = false; return {TT_OR, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};} break;
                    case 'r': if (isKeyword("eturn")) {canEndStmt = false; return {TT_RETURN, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};} break;
                    case 't': if (isKeyword("rue")) {canEndStmt = true; return {TT_TRUE, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};} break;
                    case 'w': if (isKeyword("hile")) {canEndStmt = false; return {TT_WHILE, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};} break;
                    case 'x': if (isKeyword("or")) {canEndStmt = false; return {TT_XOR, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};} break;
                    default: break;
                }
                return {TT_ID, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};
            } else return {TT_ERROR, "Unexpected character.", 1, line, (unsigned short)(col - (curr - start))};
        }
    }
    return {TT_ERROR, "Unexpected error.", 1, line, col};
}

bool Lexer::skipWhitespace() {
    while (!isEOF()) {
        switch (*curr) {
            case ' ':
            case '\t':
                eat(); break;
            case '\r':
                curr++; break;
            case '\n': {
                if (canEndStmt) {
                    canEndStmt = false;
                    return true;
                } else eat();
                col = 1;
                line++;
                break;
            }
            case '#': while (!isEOF() && *curr != '\n') eat(); break;
            default: return false;
        }
    }
    return false;
}
bool Lexer::isKeyword(const char* keyword) {
    return curr - start == strlen(keyword) + 1 && memcmp(start + 1, keyword, strlen(keyword)) == 0;
}
char Lexer::eat() {
    col++;
    return *curr++;
}
bool Lexer::nextIs(char c) {
    if (*curr == c) {
        eat();
        return true;
    } else return false;
}
bool Lexer::isEOF() {
    return *curr == '\0';
}