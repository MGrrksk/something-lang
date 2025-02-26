#include <lexer.hpp>
#include <cctype>
#include <cstring>

void Lexer::reset(const char* src) {
    canEndStmt = false;
    parenStack.clear();
    // Parentheses stack is initialized with `true` value to accept separators in global scope.
    parenStack.push(true);
    start = curr = (char*)src;
    line = col = 1;
}
Token Lexer::next() {
    if (unsigned short sepCol = skipWhitespace()) return {TT_SEP, nullptr, 1, line, sepCol};
    if (isEOF()) {
        if (canEndStmt) {
            canEndStmt = false;
            return {TT_SEP, nullptr, 1, line, col};
        } else return {TT_EOF, nullptr, 1, line, col};
    }
    start = curr;
    switch (eat()) {
        case '(': canEndStmt = false; parenStack.push(false); return {TT_LPAREN, start, 1, line, (unsigned short)(col - 1)};
        case ')': canEndStmt = true; parenStack.pop(); return {TT_RPAREN, start, 1, line, (unsigned short)(col - 1)};
        case '[': canEndStmt = false; parenStack.push(false); return {TT_LBRACK, start, 1, line, (unsigned short)(col - 1)};
        case ']': canEndStmt = true; parenStack.pop(); return {TT_RBRACK, start, 1, line, (unsigned short)(col - 1)};
        case '{': canEndStmt = false; parenStack.push(true); return {TT_LBRACE, start, 1, line, (unsigned short)(col - 1)};
        case '}': canEndStmt = false; parenStack.pop(); return {TT_RBRACE, start, 1, line, (unsigned short)(col - 1)};
        case ',': canEndStmt = false; return {TT_COMMA, start, 1, line, (unsigned short)(col - 1)};
        case '.': canEndStmt = false; return {TT_DOT, start, 1, line, (unsigned short)(col - 1)};
        case '+': canEndStmt = false; return {TT_PLUS, start, 1, line, (unsigned short)(col - 1)};
        case '-': canEndStmt = false; return {TT_MINUS, start, 1, line, (unsigned short)(col - 1)};
        case '*': canEndStmt = false; return nextIs('*') ? Token{TT_DBLSTAR, start, 2, line, (unsigned short)(col - 2)} : Token{TT_STAR, start, 1, line, (unsigned short)(col - 1)};
        case '/': canEndStmt = false; return nextIs('/') ? Token{TT_DBLSLASH, start, 2, line, (unsigned short)(col - 2)} : Token{TT_SLASH, start, 1, line, (unsigned short)(col - 1)};
        case '%': canEndStmt = false; return {TT_PERCENT, start, 1, line, (unsigned short)(col - 1)};
        case '!': canEndStmt = false; return nextIs('=') ? Token{TT_BNGEQUAL, start, 2, line, (unsigned short)(col - 2)} : Token{TT_BANG, start, 1, line, (unsigned short)(col - 1)};
        case '=': canEndStmt = false; return nextIs('=') ? Token{TT_DBLEQUAL, start, 2, line, (unsigned short)(col - 2)} : Token{TT_EQUAL, start, 1, line, (unsigned short)(col - 1)};
        case '>': canEndStmt = false; return nextIs('=') ? Token{TT_GTEQUAL, start, 2, line, (unsigned short)(col - 2)} : Token{TT_GREATER, start, 1, line, (unsigned short)(col - 1)};
        case '<': canEndStmt = false; return nextIs('=') ? Token{TT_LSEQUAL, start, 2, line, (unsigned short)(col - 2)} : Token{TT_LESS, start, 1, line, (unsigned short)(col - 1)};
        case ';': canEndStmt = true; return {TT_SEP, start, 1, line, (unsigned short)(col - 1)};
        case '\\': canEndStmt = false; if (nextIs('\n') || (nextIs('\r') && nextIs('\n'))) {col = 1; line++; return next();} else return {TT_ERROR, "Unexpected character.", 1, line, (unsigned short)(col - 1)};
        case '"': {
            canEndStmt = true;
            while (!isEOF() && *curr != '"') {
                if (*curr == '\n') return {TT_ERROR, "Unterminated string literal.", (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};
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
                    case 'a': if (isKeyword("nd")) {canEndStmt = false; return {TT_AND, start, 3, line, (unsigned short)(col - 3)};} break;
                    case 'd':
                        if (isKeyword("ef")) {canEndStmt = false; return {TT_DEF, start, 3, line, (unsigned short)(col - 3)};}
                        else if (isKeyword("o")) {canEndStmt = false; return {TT_DO, start, 2, line, (unsigned short)(col - 2)};}
                        break;
                    case 'e': if (isKeyword("lse")) {canEndStmt = false; return {TT_ELSE, start, 4, line, (unsigned short)(col - 4)};} break;
                    case 'f':
                        if (isKeyword("alse")) {canEndStmt = true; return {TT_FALSE, start, 5, line, (unsigned short)(col - 5)};}
                        else if (isKeyword("n")) {canEndStmt = false; return {TT_FN, start, 2, line, (unsigned short)(col - 2)};}
                        else if (isKeyword("or")) {canEndStmt = false; return {TT_FOR, start, 3, line, (unsigned short)(col - 3)};}
                        break;
                    case 'i': if (isKeyword("f")) {canEndStmt = false; return {TT_IF, start, 2, line, (unsigned short)(col - 2)};} break;
                    case 'n': if (isKeyword("othing")) {canEndStmt = true; return {TT_NOTHING, start, 7, line, (unsigned short)(col - 7)};} break;
                    case 'o': if (isKeyword("r")) {canEndStmt = false; return {TT_OR, start, 2, line, (unsigned short)(col - 2)};} break;
                    case 'r': if (isKeyword("eturn")) {canEndStmt = false; return {TT_RETURN, start, 6, line, (unsigned short)(col - 6)};} break;
                    case 't': if (isKeyword("rue")) {canEndStmt = true; return {TT_TRUE, start, 4, line, (unsigned short)(col - 4)};} break;
                    case 'w': if (isKeyword("hile")) {canEndStmt = false; return {TT_WHILE, start, 5, line, (unsigned short)(col - 5)};} break;
                    case 'x': if (isKeyword("or")) {canEndStmt = false; return {TT_XOR, start, 3, line, (unsigned short)(col - 3)};} break;
                    default: break;
                }
                return {TT_ID, start, (unsigned short)(curr - start), line, (unsigned short)(col - (curr - start))};
            } else return {TT_ERROR, "Unexpected character.", 1, line, (unsigned short)(col - 1)};
        }
    }
    return {TT_ERROR, "Unexpected error.", 1, line, col};
}

unsigned short Lexer::skipWhitespace() {
    unsigned short currCol = col;
    while (!isEOF()) {
        switch (*curr) {
            case ' ':
            case '\t':
                eat(); break;
            case '\r':
                curr++; break;
            case '\n': {
                if (canEndStmt && parenStack.peek()) {
                    canEndStmt = false;
                    return currCol;
                } else eat();
                col = 1;
                line++;
                break;
            }
            case '#': while (!isEOF() && *curr != '\n') eat(); break;
            default: return 0;
        }
    }
    return 0;
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