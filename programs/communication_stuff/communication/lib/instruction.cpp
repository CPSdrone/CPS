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

template <typename T> 
T instruction::resultAs()
{
    T ret = *reinterpret_cast<T>(rcv_buf);
    return ret;
}

instruction::~instruction()
{
if(heap_allocated)
    delete[] rcv_buf;
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


instruction create_ins::serialPrint(const char* text, uint8_t len){
    instruction ret;
    ret.result_s = 0;
    ret.instr_s = len+1;
    ret.bin.reserve(len+1);
    memcpy(ret.bin.data()+1, text, len);
    ret.bin[0] = inst_list::serialPrint;
    return ret;
}

instruction create_ins::serialPrint(const std::string str){
    return serialPrint(str.c_str(), str.size()+1); // +1 for null termination
}
