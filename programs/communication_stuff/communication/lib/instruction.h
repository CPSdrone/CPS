#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>   // std::vector
#include <stdarg.h> // variac functions f(, ...)
#include <iostream> // std::cout
#include <cstdio>
#include <future>
#include <cstring>
#include <cassert>

#define HIGH 0x1 // adruino constant
#define LOW  0x0 // adruino constant

class instruction
{
public:
    /* Sizes to exchange with arduino */
    size_t instr_s; // instruction size
    size_t result_s; // Result size
    uint8_t *rcv_buf = nullptr;

    // Binary representation of the instruction
    std::vector<uint8_t> bin;

    bool heap_allocated = false;    
    uint8_t* malloc_response(){
      assert(heap_allocated == false);

      rcv_buf = new uint8_t[result_s];
      heap_allocated = true;
      return rcv_buf;
    }

    // Get the result as type T
    template <typename T> T resultAs();

    // Create empty instruction
    instruction(){};

    // Create instruction expecting result of size RES_S 
    // and instruction size INS_S
    instruction(size_t res_S, size_t ins_S, ...);

    ~instruction();
};


// List of instructions
namespace inst_list {
  enum ins_prefix : uint8_t{
    pinSet, 
    pinGet,
    serialPrint,
    // ...
    ins_count,
  };

  enum pinType : uint8_t {
    digital,
    analog,
  };
  
}

// Create instruction
struct create_ins
{
    /* Set pin PIN,
     using type TYPE (digital/analog),
     to value VAL : 
    execute <type>Write(PIN, VAL) on microcontroller */
    static instruction pinSet(uint8_t pin, uint8_t type, uint8_t val);

    /* Get state of pin PIN :
     using type TYPE (digital/analog)
    execute <type>Read(PIN) on microcontroller */
    static instruction pinGet(uint8_t pin, uint8_t type);

    /* Set pin PIN to VAL (0-255) */
    static instruction analogWrite(uint8_t pin, uint8_t val);

    /* Set pin PIN to HIGH / LOW */
    static instruction digitalWrite(uint8_t pin, uint8_t val);

    /* Read pin value (0-1023 for Mega) */
    static instruction analogRead(uint8_t pin);

    /* Read pin value (HIGH / LOW) */
    static instruction digitalRead(uint8_t pin);

    
  // Misc
    /* Print TEXT of length LEN to serial */
    static instruction serialPrint(const char* text, uint8_t len);
    static instruction serialPrint(const std::string str);

};

#endif