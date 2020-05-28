#include "spi.h"
#include "instruction.h"
#include <ctime>
#include <iostream>
#include <vector>

/* Limits:
    * Can send at most 95 bytes (chars)
    * ~ 8 ms per sent byte */

void do_stuff(spi_sock &spiS, uint8_t *snd_buf, uint8_t *rcv_buf, int len){
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Send data
    //spiS.transfer_raw(snd_buf, len, rcv_buf);
    spiS.transfer_bytes(snd_buf, len, rcv_buf);
    
    
    printf("Sent     : %s\n", snd_buf);
    printf("Got back : %s\n", rcv_buf);

    // Measure time
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    //std::cout << "Sending took " << duration << " mili seconds!"<< '\n' ;

    // Log duration
    static double max_dur, min_dur = duration;
    if(duration > max_dur)
        max_dur = duration;
    else if(duration < min_dur)
        min_dur = duration;
    std::cout << "Min duration: " << min_dur << " Max dur.: " << max_dur << ", This dur.: "<< duration << '\n';

}

struct stats{
    // min, max, avg
    double sum = 0;
    uint32_t c = 0;
    double min = 10000;
    double max = 0;

    void add(double len)
    {
        sum+=len;
        c++;
        if(len < min)
            min = len;
        else if(max < len)
            max = len;
    }
};


int main(int argc, char const *argv[])
{
    spi_sock spiS(SPI_CLOCK_1MHz*6.6); // *6.6

    uint8_t snd_buf[10000] = {0xff, };
    uint8_t rcv_buf[10000] = {0xff, };

    std::string message = "Hello world!\n";
    
    //auto insr = create_ins::serialPrint(message.c_str(), message.length()+1);
    auto insr = create_ins::analogRead(1);
    insr.malloc_response();
    /*
    snd_buf[0] = insr.instr_s;
    memcpy(snd_buf + 1, insr.bin.data(), snd_buf[0]);
    

    spiS.transfer_bytes(snd_buf, snd_buf[0]+1, rcv_buf);

    do{
        char useless = 0xff;
        usleep(500);
        spiS.transfer_raw(&useless, 1, rcv_buf);
    }while(rcv_buf[0] == 0xff);
    size_t len = rcv_buf[0];
    printf("Size of response : %d\n", len);
    if(len > 0){
        uint8_t garbage[len];
        memset(garbage, 0xff, len);
        spiS.transfer_bytes(garbage, len, rcv_buf);
        printf("Got back %d\n", * (int16_t*)rcv_buf);
    }
    */
    stats sts;

    #define N_RUNS 5000
    for (size_t i = 0; i < N_RUNS; i++)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        spiS.send_instruction(insr);
        if(insr.result_s > 0)
            printf("Got back %d\n", * insr.rcv_buf);

        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        sts.add(duration);
    }
    printf("All took:\nmin: %f\nmax: %f\navg: %f\n", sts.min, sts.max, sts.sum/sts.c);



    /*
    while(1){
        auto start_time = std::chrono::high_resolution_clock::now();
        
        uint8_t len = 1 + std::sprintf(snd_buf + 1, "raspi"); // +1 for null termination
        snd_buf[0] = len;
        
        spiS.transfer_bytes(snd_buf, len+1, rcv_buf); // write command

        do
        {
            char useless = 0xff;
            usleep(1);
            spiS.transfer_raw(&useless, 1, rcv_buf);
        } while (rcv_buf[0] == 0xff);
        
        len = rcv_buf[0];

        memset(snd_buf, 0xff, 100);
        spiS.transfer_bytes(snd_buf, len, rcv_buf);
        printf("Response : '%s'\n", rcv_buf);

        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(end_time - start_time).count();

        std::cout << "It lasted " << duration << "ms" << '\n' ;
    }
    //spiS.transfer_raw(snd_buf, len+1, rcv_buf); // write command
    */
/*
    len = rcv_buf[0];
    printf("Recieving %d bytes\n", len);
    
    // Inform arduino of transfer
    //spiS.transfer_raw(rcv_buf, 1, rcv_buf);
    char tx[len+1];
    tx[0] = len;
    tx[1] = 0xff;
    
    spiS.transfer_raw(tx, (int) len+1, rcv_buf);
    rcv_buf[len] = 0;
    printf("Got back : '%s'\n", rcv_buf);
*/

    


    



    /*
    int len = 95;
    memset(snd_buf+len, 0, 1);
    while(1){
        do_stuff(std::ref(spiS), snd_buf, rcv_buf, len);
      
        for (size_t i = 0; i < len; i++)
        {
            char c = rcv_buf[i];
            if(  c < 'A' || 'Z' < c  ) // Check for letters
                if(c != '.' && c!= ' ' && c != '\0'){
                    printf("We have a problem : '%c'\n", c);
                    return 1;
                }
        }
      
    }
    */
    

    return 0;
}
