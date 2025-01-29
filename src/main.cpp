#include <cstdio>
#include <cstring>

const char* const HELP_MSG = "Usage of the `smth` tool:\n"
    "\t\e[1;37msmth --help | \e[0m\e[4;37mfile\e[0m";

bool run(const char* path);

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "\e[1;31mFatal error: \e[1;37minvalid command syntax. Execution aborted.\n\e[1;34mNote: \e[0mtype `smth --help` for more information.");
        return 1;
    }
    if (strcmp(argv[1], "--help") == 0) printf(HELP_MSG);
    else return run(argv[1]) ? 0 : 1;
    return 0; // Just to avoid warnings
}

bool run(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "\e[1;31mFatal error: \e[1;37mUnable to open and read from `%s` file. Execution aborted.\n\e[1;34mNote: \e[0mMake sure provided path is valid and the file actually exists.", path);
        return false;
    }
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    rewind(file);
    char* str = new char[length+1]; // +1 for ending null character
    fread(str, sizeof(char), length, file);
    fclose(file);
    str[length] = '\0'; // Making sure to end the string properly
    printf("File contents: %s\n", str);
    delete[] str;
    return true;
}