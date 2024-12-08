#include "bus_cpu.h"

bus_cpu::bus_cpu()
:ALU(){
    clear();
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
    Rp_address = 0;
    internal_state = READY;
}

void bus_cpu::clear(){
    reset();
    RAM.fill(0);
    Rp.fill(0);
}

ucycle_status bus_cpu::process_microcycle(){
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
            transmiter = &Rp[Rp_address]; 
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
            return NEED_INPUT;
            uint8_t user_input;
            transmiter = &user_input;
            break;
        default:
            return ERROR;
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
            return ERROR;
            break;
    }
    
    alu_result = ALU.calculate(R1, R2);
    *receiver = *transmiter;

    Gs++;
    if(Gs > 3){
        Rp_address++;
        Gs = 0;
    }
    return SUCCESS;
}

bus_cpu_state schedule_execution(const execution_type type){
    ucycle_status status;
    if (internal_state == EXECUTION){
        status = process_microcycle();
        switch (status){
            case SUCCESS:
                switch (type){
                    case CYCLE:
                        if(Gs == 0){
                            internal_state = CYCLE_DONE;
                        }
                        break;
                    case MICRO_CYCLE:
                        internal_state = CYCLE_DONE;
                        break;
                    case PROGRAM:
                        if (Rp_address > 15){
                            internal_state = DONE;
                        }
                        break;
                    default:
                        internal_state = ERROR;
                        break;
                }
                if (Rp_address > 15){
                    internal_state = DONE;
                }
                break;
            case NEED_INPUT:
                internal_state = NEED_INPUT;
                break;
            case ERROR:
                internal_state = ERROR;
                break;
            default:
                internal_state = ERROR;
                break;
        }
    }
    return internal_state;
}

void bus_cpu::start_execution(){
    if(internal_state == READY){
        internal_state = EXECUTION;
    }
}

void bus_cpu::pause_execution(){
    if(internal_state == EXECUTION){
        internal_state = PAUSED;
    }
}

void bus_cpu::continue_execution(){
    if(internal_state == PAUSED || internal_state == CYCLE_DONE){
        internal_state = EXECUTION;
    }
}

bool bus_cpu::set_RAM(const uint8_t address, const uint8_t value){
    if(address > RAM_SIZE || value > 255){
        return false;
    }
    RAM[address] = value;
    return true;
}

uint8_t bus_cpu::get_RAM(const uint8_t address){
    if(address > RAM_SIZE){
        return 0;
    }
    return RAM[address];
}

bool bus_cpu::set_Rp(const uint8_t address, const uint8_t value){
    if(address > PROGRAM_SIZE || value > 255){
        return false;
    }
    Rp[address] = value;
    return true;
}

uint8_t bus_cpu::get_Rp(const uint8_t address){
    if(address > PROGRAM_SIZE){
        return 0;
    }
    return Rp[address];
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