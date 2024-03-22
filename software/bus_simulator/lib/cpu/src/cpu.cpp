#include "cpu.h"

cpu::cpu()
:ALU(){
    reset();
}

void cpu::reset(){
    Ra = 0;
    Rb = 0;
    Rc = 0;
    Rwy = 0;
    R1 = 0;
    R2 = 0;
    Ri = 0;
    instr_counter = 0;
}

void cpu::process_cycle(){
    for(uint8_t i = 0; i < 4; i++){
        process_microcycle();
    }
}

void cpu::process_microcycle(){
    uint8_t instruction = RAM[Ri];
    uint8_t alu_opcode = instruction & 0b00000011;
    uint8_t rx_ctrl = (instruction & 0b00011100) >> 2;
    uint8_t tx_ctrl = (instruction & 0b11100000) >> 4;
    uint8_t* transmiter;
    uint8_t* receiver;
    uint8_t alu_result = 0;

    switch (tx_ctrl){
        case 0:
            transmiter = &instr_counter; 
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
    instr_counter ++;
}

void cpu::execute_program(){

}

void cpu::execute_cycle(){

}

void cpu::execute_micro_cycle(){
    
}