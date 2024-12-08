#include "bus_cpu.h"

bus_cpu::bus_cpu()
:ALU(){
    reset();
    RAM.fill(0);
    Rp.fill(0);
}

bus_cpu::~bus_cpu(){

}

void bus_cpu::reset(){
    Ra = 0;
    Rb = 0;
    Rc = 0;
    Rwy = 0;
    R1 = 0;
    R2 = 0;
    Ri = 0;
    Gs = 0;
}

void bus_cpu::process_microcycle(const uint8_t Rp_pointer = 0){
    uint8_t address = calculate_address(); //idk if this works 
    uint8_t instruction = RAM[address]; 
    uint8_t alu_opcode = instruction & 0b00000011;
    uint8_t rx_ctrl = (instruction & 0b00011100) >> 2;
    uint8_t tx_ctrl = (instruction & 0b11100000) >> 4;
    uint8_t* transmiter;
    uint8_t* receiver;
    uint8_t alu_result = 0;

    switch (tx_ctrl){
        case 0:
            transmiter = &Rp[Rp_pointer]; 
            break;
        case 1:
            transmiter = &Ra;
            break;
        case 2:
            transmiter = &Rb;
            break;
        case 3:
            transmiter = &Rc;
            break;
        case 4:
            transmiter = &alu_result;
            break;
        case 5: 
            //call user input function
            uint8_t user_input;
            transmiter = &user_input;
            break;
        default:
            //assert sth
            return;
            break;
    }

    switch(rx_ctrl){
        case 0:
            receiver = &Ri;
            ALU.set_opcode(alu_opcode);
            break;
        case 1:
            receiver = &Ri;
            break;
        case 2:
            receiver = &Ra;
            break;
        case 3:
            receiver = &Rb;
            break;
        case 4:
            receiver = &Rc;
            break;
        case 5:
            receiver = &R1;
            ALU.set_opcode(alu_opcode);
            break;
        case 6:
            receiver = &R2;
            ALU.set_opcode(alu_opcode);
            break;
        case 7:
            receiver = &Rwy;
            break;
        default:
            //assert sth
            return;
            break;
    }
    
    alu_result = ALU.calculate(R1, R2);
    *receiver = *transmiter;

    Gs++;
    if(Gs > 3){
        Gs = 0;
    }
}

void bus_cpu::execute_program(){
    static uint8_t program_pointer = 0;
    while (program_pointer < 16){
        //schould probably use task scheduler
        program_pointer++;
        return;
    }
}

void bus_cpu::execute_program_cycle(const uint8_t program_pointer){
    static uint8_t ucycle_count = 0;
    static uint8_t cycle_time = 0;
    while(ucycle_count < 4){
        process_microcycle(program_pointer);
        //????
    }
}

void bus_cpu::execute_cycle(const uint8_t Ri_value){
    for(uint8_t i = 0; i < 4; i++){
        execute_micro_cycle(Ri_value);
    }
}

void bus_cpu::execute_micro_cycle(const uint8_t Ri_value){
    Ri = Ri_value;
    process_microcycle();
}

bool bus_cpu::set_RAM(const uint8_t address, const uint8_t value){
    if(address > RAM_SIZE || value > 255){
        return false;
    }
    RAM[address] = value;
    return true;
}

bool bus_cpu::set_Rp(const uint8_t address, const uint8_t value){
    if(address > PROGRAM_SIZE || value > 255){
        return false;
    }
    Rp[address] = value;
    return true;
}

void bus_cpu::get_register_values(bus_cpu_status &registers){
    registers.Ra = Ra;
    registers.Rb = Rb;
    registers.Rc = Rc;
    registers.Rwy = Rwy;
    registers.RAM_address = calculate_address();
    registers.RAM_value = RAM[registers.RAM_address];
}

inline uint8_t bus_cpu::calculate_address(){
    return (Ri << 2) | Gs;
}