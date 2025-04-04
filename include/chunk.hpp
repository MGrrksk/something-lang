#pragma once
#include <vector>
#include <value.hpp>

struct Chunk {
    std::vector<unsigned char> code;
    std::vector<Value> values;
    void addOp(unsigned char op);
    void addParam(unsigned short param);
    void addConst(Value value);
};