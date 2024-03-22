#include "alu.h"

alu::alu(){
    opcode = 0;
}

void alu::set_opcode(const uint8_t _opcode){
    //this needs be called  3 times
    opcode = opcode << 2;
    opcode |= _opcode;
}

uint8_t alu::calculate(const uint8_t operand1, const uint8_t operand2){
    uint8_t sihfted_opcode = opcode >> 1; //ifx one too many bit shift
    uint8_t calculation_result = 0;
   if((sihfted_opcode & 0b00010000)){
    //logic operations
        switch(sihfted_opcode & 0b00001111){
            case 0:
                calculation_result = ~operand1;
                break;
            case 1:
                calculation_result = ~(operand1 & operand2);
                break;
            case 2:
                calculation_result = ~operand1 | operand2;
                break;
            case 3:
                calculation_result = 1;
                break;
            case 4:
                calculation_result = operand1 ^ operand2;
                break;
            case 5:
                calculation_result = 0 ;//tba
                break;
            case 6:
                calculation_result = 0; //tba
                break;
            case 7:
                calculation_result = operand1 | operand2;
                break;
            case 8:
                calculation_result = 0; //tba
                break;
            case 9:
                calculation_result = ~(operand1 & operand2); //duplication ??
                break;
            case 10:
                calculation_result = operand1 & operand2;
                break;
            case 11:
                calculation_result = ~(operand1 ^ operand2);
                break;
            case 12:
                calculation_result = ~operand2;
                break;
            case 13:
                calculation_result = ~(operand1 | operand2);
                break;
            case 14:
                calculation_result = 0; //tba
                break;
            case 15:
                calculation_result = ~(operand1 ^ operand2); // duplication ??
                break;
            default:
                //assert sth
                return 0;
                break;
        }
   }
   else{
    //arithmetic operations
        switch(sihfted_opcode & 0b00001111){
            case 0:
                calculation_result = 0;
                break;
            case 1:
                calculation_result = operand1 / operand2;
                break;
            case 2:
                calculation_result = (operand1 * operand2) - 1;
                break;
            case 3:
                calculation_result = operand1  + operand2;
                break;
            case 4:
                calculation_result = (operand1 * operand1) - (operand2 * operand2);
                break;
            case 5:
                calculation_result = operand1 * 2;
                break;
            case 6:
                calculation_result = (operand1 + operand2) / 2;
                break;
            case 7:
                calculation_result = operand1 * operand2;
                break;
            case 8:
                calculation_result = operand1 % operand2;
                break;
            case 9:
                calculation_result = operand1 / 2;
                break;
            case 10:
                calculation_result = (operand1 * operand2) - (operand1 + operand2);
                break;
            case 11:
                calculation_result = operand1 - operand2;
                break;
            case 12:
                calculation_result = (operand1 - operand2) * (operand1 - operand2);
                break;
            case 13:
                calculation_result = operand1;
                break;
            case 14:
                calculation_result = operand1 * operand1;
                break;
            case 15:
                calculation_result = operand1 - 1;
                break;
            default:
                //assert sth
                return 0;
                break;
        }
   }
}