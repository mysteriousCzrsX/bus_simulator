#ifndef _BUS_bus_cpu
#define _BUS_bus_cpu

#include<stdint.h>
#include<array>
#include "alu.h"

#define RAM_SIZE 64
#define PROGRAM_SIZE RAM_SIZE / 4

struct bus_cpu_status{
    uint8_t Ra;
    uint8_t Rb;
    uint8_t Rc;
    uint8_t Rwy;
    uint8_t RAM_address;
    uint8_t RAM_value;
};

class bus_cpu{
private:
    uint8_t Ra;
    uint8_t Rb;
    uint8_t Rc;
    uint8_t Rwy;
    uint8_t R1;
    uint8_t R2;
    uint8_t Ri; //instruction pointer
    uint8_t Gs;

    std::array<uint8_t, RAM_SIZE> RAM;
    std::array<uint8_t, PROGRAM_SIZE> Rp;

    alu ALU;

    void process_microcycle(const uint8_t Rp_pointer);
    inline uint8_t calculate_address();
public:
    bus_cpu();
    ~bus_cpu();
    void reset();
    void execute_program();
    void execute_program_cycle(const uint8_t program_pointer);

    void execute_cycle(const uint8_t Ri_value);
    void execute_micro_cycle(const uint8_t Ri_value);
    bool set_RAM(const uint8_t address, const uint8_t value);
    bool set_Rp(const uint8_t address, const uint8_t value);
    void get_register_values(bus_cpu_status &registers);
};

#endif