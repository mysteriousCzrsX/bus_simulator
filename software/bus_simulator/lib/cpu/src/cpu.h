#ifndef _CPU
#define _CPU

#include<stdint.h>
#include<array>

#define RAM_SIZE 64

class cpu
{
private:
    uint8_t Ra;
    uint8_t Rb;
    uint8_t Rc;
    uint8_t Rwy;
    uint8_t R1;
    uint8_t R2;
    //uint8_t RAM[RAM_SIZE];
    std::array<uint8_t, RAM_SIZE> RAM;
    uint8_t instr_pointer;
public:
    cpu();
    ~cpu();
    void reset();
    void process_cycle();
};

#endif