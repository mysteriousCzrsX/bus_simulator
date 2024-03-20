#include "alu.h"

alu::alu(){
    opcode = 0;
}

void alu::set_opcode(const uint8_t _opcode){
    opcode = opcode << 2;
    opcode |= _opcode;
}

uint8_t alu::calculate(const uint8_t operand1, const uint8_t operand2){
    opcode = opcode >> 1;
    uint8_t calculation_result = 0;
   if((opcode & 0b00010000)){
    //logic operations
        switch(opcode & 0b00001111){
            case 0:
                calculation_result = operand1 + operand2;
                break;
            case 1:
                calculation_result = operand1 - operand2;
                break;
            case 2:
                calculation_result = operand1 * operand2;
                break;
            case 3:
                calculation_result = operand1 / operand2;
                break;
            case 4:
                calculation_result = operand1 & operand2;
                break;
            case 5:
                calculation_result = operand1 | operand2;
                break;
            case 6:
                calculation_result = operand1 ^ operand2;
                break;
            case 7:
                calculation_result = ~operand1;
                break;
            case 8:
                calculation_result = operand1 << operand2;
                break;
            case 9:
                calculation_result = operand1 >> operand2;
                break;
            case 10:
                calculation_result = operand1;
                break;
            case 11:
                calculation_result = operand2;
                break;
            case 12:
                calculation_result = operand1;
                break;
            case 13:
                calculation_result = operand2;
                break;
            case 14:
                calculation_result = operand1;
                break;
            case 15:
                calculation_result = operand2;
                break;
            default:
                //assert sth
                return 0;
                break;
        }
   }
   else{
    //arithmetic operations
        switch(opcode & 0b00001111){
            case 0:
                calculation_result = operand1 + operand2;
                break;
            case 1:
                calculation_result = operand1 - operand2;
                break;
            case 2:
                calculation_result = operand1 * operand2;
                break;
            case 3:
                calculation_result = operand1 / operand2;
                break;
            case 4:
                calculation_result = operand1 & operand2;
                break;
            case 5:
                calculation_result = operand1 | operand2;
                break;
            case 6:
                calculation_result = operand1 ^ operand2;
                break;
            case 7:
                calculation_result = ~operand1;
                break;
            case 8:
                calculation_result = operand1 << operand2;
                break;
            case 9:
                calculation_result = operand1 >> operand2;
                break;
            case 10:
                calculation_result = operand1;
                break;
            case 11:
                calculation_result = operand2;
                break;
            case 12:
                calculation_result = operand1;
                break;
            case 13:
                calculation_result = operand2;
                break;
            case 14:
                calculation_result = operand1;
                break;
            case 15:
                calculation_result = operand2;
                break;
            default:
                //assert sth
                return 0;
                break;
        }
   }
}