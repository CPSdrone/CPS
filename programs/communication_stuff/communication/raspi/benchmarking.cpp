#include "spi.h"
#include <ctime>
#include <iostream>

/* Limits:
    * Can send at most 95 bytes (chars)
    * ~ 8 ms per sent byte */

void do_stuff(spi_sock &spiS, char *snd_buf, char *rcv_buf, int len){
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Send data 
    spiS.transfer_raw(snd_buf, len, rcv_buf);
    
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
    std::cout << "Min duration: " << min_dur << " Max dur.: " << max_dur << ", This dur.:"<< duration << '\n';

}


int main(int argc, char const *argv[])
{
    //6 - perfect stability ## 5.75 - stable in testing
    spi_sock spiS(SPI_CLOCK_SPEED/5.8, SPI_MODE_0, 0); // /5.8

    char snd_buf[10000]; //10k
    char rcv_buf[10000];
    for (size_t i = 0; i < 10000; i++)
    {
        snd_buf[i] = 'a' + i % 26;
    }

    while(1){
        do_stuff(spiS, snd_buf, rcv_buf, 95); // 95 bytes works
        for (size_t i = 0; i < 95; i++)
        {
            char c = rcv_buf[i];
            if(  c < 'A' || 'Z' < c  ) // Check for letters
                if(c != '.' && c!= ' ' && c != '\0'){
                    printf("We have a problem : '%c'\n", c);
                    return 1;
                }
        }
    }

    return 0;
}
