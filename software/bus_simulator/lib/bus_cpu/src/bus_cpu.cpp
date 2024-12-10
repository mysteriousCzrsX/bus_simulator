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
            if(!user_input_ready){
                return NEED_INPUT;
            }
            transmiter = &user_input;
            user_input_ready = false;
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
            receiver = &Ra;
            break;
        case 2:
            receiver = &Rb;
            break;
        case 3:
            receiver = &Rc;
            break;
        case 4:
            receiver = &R1;
            ALU.set_opcode(alu_opcode);
            break;
        case 5:
            receiver = &R2;
            ALU.set_opcode(alu_opcode);
            break;
        case 6:
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

bus_cpu_state bus_cpu::schedule_execution(){
    ucycle_status ucycle_return;
    if (internal_state == EXECUTION){
        ucycle_return = process_microcycle();
        switch (ucycle_return){
            case SUCCESS:
            Serial.println("CPU> Micro cycle done");
                switch (type){
                    case CYCLE:
                        if(Gs == 0){
                            internal_state = CYCLE_DONE;
                            Serial.println("CPU> Cycle done");
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
                        internal_state = EXCEPTION;
                        break;
                }
                if (Rp_address > 15){
                    Serial.println("CPU> Execution done");
                    internal_state = DONE;
                }
                break;
            case NEED_INPUT:
                internal_state = USER_INPUT;
                Serial.println("CPU> Need user input");
                break;
            case ERROR:
                internal_state = EXCEPTION;
                break;
            default:
                internal_state = EXCEPTION;
                break;
        }
    }
    return internal_state;
}

void bus_cpu::start_execution(){
    if(internal_state == READY){
        Serial.println("CPU> Starting execution");
        internal_state = EXECUTION;
    }
}

void bus_cpu::continue_execution(){
    if(internal_state == CYCLE_DONE){
        Serial.println("CPU> Continue execution");
        internal_state = EXECUTION;
    }
}

void bus_cpu::take_user_input(uint8_t input){
    user_input = input;
    user_input_ready = true;
    internal_state = EXECUTION;
    Serial.print("CPU> User input: ");
    Serial.println(user_input);
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
    uint8_t addr = (Ri << 2) | Gs;
    if(addr > RAM_SIZE){
        addr = RAM_SIZE;
    }
    return addr;
}

void bus_cpu::set_execution_type(const execution_type _type){
    type = _type;
}