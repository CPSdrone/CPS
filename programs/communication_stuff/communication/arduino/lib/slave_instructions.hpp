#ifndef SLAVE_INSTRUCTIONS_HPP
#define SLAVE_INSTRUCTIONS_HPP

#include <Arduino.h>


// List of instructions
namespace inst_list {
  enum ins_prefix : uint8_t{
    pinSet, 
    pinGet,
    serialPrint,
    // ...
    ins_count,
  };

  // Special for pin instructions
  enum pinType : uint8_t {
    digital,
    analog,
  };
}



// Define functions to be executed on arduino
struct slave_instructions
{
//  General function signature : uint (size) f(uint *ins, uint *result)
//  Read remaining instruction from INS, write result into RESULT and return number of bytes written
//  On arduino, give snd_buf[1] as result, than set snd_buf[0] to return value of function

  uint8_t (*instr[inst_list::ins_prefix::ins_count])(uint8_t *, uint8_t *) = {
    [(int)inst_list::ins_prefix::pinSet] = &pinSet,
    [(int)inst_list::ins_prefix::pinGet] = &pinGet,
    [(int)inst_list::ins_prefix::serialPrint] = &serialPrint,
  };

  static uint8_t pinSet(uint8_t *ins, uint8_t *res){
    uint8_t type = ins[0];
    uint8_t pin  = ins[1];
    uint8_t val  = ins[2];
    if(type == inst_list::pinType::analog){
        Serial.print("analog write on pin "+String(pin));
        Serial.println(" value "+ String(val));
        analogWrite(pin, val);
    }
    else if(type == inst_list::pinType::digital){
        Serial.print("digital write on pin "+String(pin));
        Serial.println(" value "+ String(val));
        digitalWrite(pin, val);
    }
    #ifdef DEBUG
    else
        Serial.println("Pin type not found!");
    #endif

    return 0;
  }

  static uint8_t pinGet(uint8_t *ins, uint8_t *res){
    uint8_t type = ins[0];
    uint8_t pin  = ins[1];
    int result;
    if(type == inst_list::pinType::analog)
        result = analogRead(pin);
    else if(type == inst_list::pinType::digital)
        result = digitalRead(pin);
    #ifdef DEBUG
    else
        Serial.println("Pin type not found!");
    #endif
    *(int*)res = result;
    return sizeof(int); // 4 bytes
  }
  
  static uint8_t serialPrint(uint8_t *ins, uint8_t *res){
      Serial.print((char*)ins);
      return 0;
  }
  

};

#endif