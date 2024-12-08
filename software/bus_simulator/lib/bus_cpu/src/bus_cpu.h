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
    uint8_t Rp_address;
};

enum bus_cpu_state{
    READY = 0,
    EXECUTION = 1,
    CYCLE_DONE = 2,
    DONE = 3,
    USER_INPUT = 4,
    PAUSED = 5,
    EXCEPTION = 6
};

enum ucycle_status{
    SUCCESS = 0,
    NEED_INPUT = 1,
    ERROR = 2
};


enum execution_type{
    CYCLE = 0,
    MICRO_CYCLE = 1,
    PROGRAM = 2  
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

    uint8_t Rp_address;
    uint8_t user_input;
    bool user_input_ready = false;

    std::array<uint8_t, RAM_SIZE> RAM;
    std::array<uint8_t, PROGRAM_SIZE> Rp;

    alu ALU;

    bus_cpu_state internal_state;

    ucycle_status process_microcycle();
    inline uint8_t calculate_address();
public:
    bus_cpu();
    ~bus_cpu();
    void reset();
    void clear();
    void start_execution();
    void pause_execution();
    void continue_execution();
    void take_user_input(uint8_t input);

    bus_cpu_state schedule_execution(const execution_type type);
    bool set_RAM(const uint8_t address, const uint8_t value);
    uint8_t get_RAM(const uint8_t address);
    bool set_Rp(const uint8_t address, const uint8_t value);
    uint8_t get_Rp(const uint8_t address);
    void get_register_values(bus_cpu_status &registers);
};

#endif