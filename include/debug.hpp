#pragma once
#include <cstdio>
#include <token.hpp>

extern FILE* logFile;

// Prints token data in following format:
// `type` \``lexeme`\`[`length`] `line`:`collumn`
void logToken(const Token& token);