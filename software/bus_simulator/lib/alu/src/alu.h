#ifndef _ALU
#define _ALU

#include<stdint.h>

class alu
{
private:
    /* data */
public:
    alu(/* args */);
    ~alu();
    void operation_select();
    void calculate(uint8_t op);
};


#endif