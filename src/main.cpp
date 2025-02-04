#include <fstream>
#include <string>
#include <cstdio>
#include <lexer.hpp>

const char* const HELP_MSG = "Usage of the `smth` tool:\n"
    "\t\e[1;37msmth --help | \e[0m\e[4;37mfile\e[0m";

// This function reads UTF-8 encoded file from `path` parameter, JIT-compiles its content and executes it.
bool run(const char* path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        printf("\e[1;31mFatal error: \e[1;37mUnable to open and read from `%s` file. Execution aborted.\n\e[1;34mNote: \e[0mMake sure provided path is valid and the file actually exists.\n", path);
        return false;
    }
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    Lexer lexer;
    lexer.reset(str.c_str());
    Token token;
    do {
        token = lexer.next();
        printf("Token type: %d, lexeme: %.*s, length: %d, line: %d, column: %d\n", token.type, token.lexeme == nullptr ? 6 : token.length, token.lexeme, token.length, token.line, token.collumn);
    } while (token.type != TT_EOF);
    return true;
}

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        printf("\e[1;31mFatal error: \e[1;37minvalid command syntax. Execution aborted.\n\e[1;34mNote: \e[0mtype `smth --help` for more information.\n");
        return 1;
    }
    if (std::string("--help") == argv[1]) printf("%s\n", HELP_MSG);
    else return run(argv[1]) ? 0 : 1;
    return 0; // Just to avoid warnings
}