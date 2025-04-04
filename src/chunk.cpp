#include <chunk.hpp>
#include <opcodes.hpp>

void Chunk::addOp(unsigned char op) {
    code.push_back(op);
}
void Chunk::addParam(unsigned short param) {
    code.push_back((param >> 8) & 0xFF);
    code.push_back(param & 0xFF);
}
void Chunk::addConst(Value value) {
    addOp(OP_CONST);
    addParam(values.size());
    values.push_back(value);
}