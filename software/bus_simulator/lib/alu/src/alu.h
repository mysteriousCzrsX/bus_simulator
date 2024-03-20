#ifndef _ALU
#define _ALU

#include<stdint.h>

class alu
{
private:
    uint8_t opcode;
public:
    alu();
    ~alu();
    void set_opcode(const uint8_t _opcode);
    uint8_t calculate(const uint8_t operand1, const uint8_t operand2);
};


#endif