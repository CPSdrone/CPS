#include "instruction.h"

instruction::instruction(size_t res_S, size_t ins_S, ...){
    result_s = res_S;
    instr_s = ins_S;
    
    // Fill byte array (biniary representation)
    va_list Bytes;
    va_start(Bytes, ins_S);
    bin.reserve(ins_S);
    for (size_t i = 0; i < ins_S; i++)
    {
    // Va_args upgrades char to int
    uint8_t byte = va_arg(Bytes, uint32_t);
    bin[i] = byte;
    }
    va_end(Bytes);
}


int instruction::transfer_len(){
    return (instr_s > result_s) ? instr_s : result_s;
}


instruction create_ins::pinSet(uint8_t pin, uint8_t type, uint8_t val){
    instruction ret( 
        0,
        4, 
        inst_list::pinSet,
        type,
        pin,
        val
    );
    return ret;
}


instruction create_ins::pinGet(uint8_t pin, uint8_t type){
    instruction ret(
        4, // sizeof int
        3,
        inst_list::pinGet,
        type,
        pin
    );
    return ret;
}

instruction create_ins::analogWrite(uint8_t pin, uint8_t val){
    return pinSet(pin, inst_list::analog, val);
}

instruction create_ins::digitalWrite(uint8_t pin, uint8_t val){
    return pinSet(pin, inst_list::digital, val);
}

instruction create_ins::analogRead(uint8_t pin){
    return pinGet(pin, inst_list::analog);
}

instruction create_ins::digitalRead(uint8_t pin){
    return pinGet(pin, inst_list::digital);
}