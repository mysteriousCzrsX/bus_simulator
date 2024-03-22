#ifndef _CPU
#define _CPU

#include<stdint.h>
#include<array>
#include "alu.h"

#define RAM_SIZE 64
#define PROGRAM_SIZE RAM_SIZE / 4

class cpu{
private:
    uint8_t Ra;
    uint8_t Rb;
    uint8_t Rc;
    uint8_t Rwy;
    uint8_t R1;
    uint8_t R2;
    uint8_t Ri; //instruction pointer
    uint8_t alu_ctrl;
    std::array<uint8_t, RAM_SIZE> RAM;
    std::array<uint8_t, PROGRAM_SIZE> Rp;
    alu ALU;
    void process_microcycle(const uint8_t address, const uint8_t Rp_pointer = 0);
public:
    cpu();
    ~cpu();
    void reset();
    void execute_program();
    void execute_cycle();
    void execute_micro_cycle();
};

#endif