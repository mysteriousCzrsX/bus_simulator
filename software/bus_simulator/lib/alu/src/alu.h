#ifndef _ALU
#define _ALU

#include<stdint.h>

class alu
{
private:
    uint8_t opcode;
    uint8_t opcode_set_count = 0;
public:
    alu();
    ~alu();
    void set_opcode(const uint8_t _opcode);
    void clear_opcode();
    uint8_t calculate(const uint8_t operand1, const uint8_t operand2);
};


#endif