#include "spi.h"

#define GET_BYTE(a,x) *((uint8_t*) &ret +x)

template <int len> struct instruction;


// Create instruction
struct create_ins
{
    enum ins_prefix : uint8_t {
        pinSet, pinGet,
    };

    enum pinState : uint8_t {
        digital, analog,
    };

    /* Set pin PIN,
     using type TYPE (digital/analog),
     to value VAL : 
    execute <type>Write(PIN, VAL) on microcontroller*/
    instruction<4> pinSet(uint8_t pin, uint8_t type, uint8_t val){
        instruction<4> ret;
        ret.bin[0] = ins_prefix::pinSet;
        ret.bin[1] = pin;
        ret.bin[2] = type;
        ret.bin[3] = val;

        ret.snd_size = 4;
        ret.rcv_size = 0;

        return ret;
    }

    /* Get state of pin PIN :
     using type TYPE (digital/analog)
    execute <type>Read(PIN) on microcontroller */
    instruction<3> pinGet(uint8_t pin, uint8_t type){
        instruction<3> ret;
        ret.bin[0] = pinGet;
        ret.bin[1] = pin;
        ret.bin[2] = type;

        ret.snd_size = 3;
        ret.rcv_size = 4; // sizeof int

        return ret;
    }
    
};

template <int len>
struct instruction
{
    /* Sizes to exchange with arduino */
    int snd_size; // The len
    int rcv_size;

    // How many bytes need to be exchanged with arduino
    // returns the greater of SND_SIZE and RCV_SIZE
    int transfer_len(){
        return (snd_size > rcv_size) ? snd_size : rcv_size;
    }

    // Binary representation of the instruction
    uint8_t bin[len];
};



int main(int argc, char const *argv[])
{
    
    return 0;
}
