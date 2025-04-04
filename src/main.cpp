#include <fstream>
#include <string>
#include <cstdio>
#include <lexer.hpp>
#include <compiler.hpp>
#include <debug.hpp>

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
    Compiler compiler;
    compiler.reset(lexer);
    Chunk chunk = compiler.compile();
    for (unsigned char op : chunk.code) printf("%d ", op);
    printf("\n");
    return !compiler.hadError;
}

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        printf("\e[1;31mFatal error: \e[1;37minvalid command syntax. Execution aborted.\n\e[1;34mNote: \e[0mtype `smth --help` for more information.\n");
        return 1;
    }
    if (std::string("--help") == argv[1]) printf("%s\n", HELP_MSG);
    else return run(argv[1]) ? 0 : 1;
    return 0;
}