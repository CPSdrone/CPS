#include "spi.h"

/* TODO: 
    * set mode by setting a flag
    * check if multiple values can be sent by adruino */

static void pabort(const char *s)
{
    perror(s);
    abort();
}

spi_sock::spi_sock(uint32_t speed, uint8_t mode, uint16_t delay)
{
    this->speed = speed;
    this->mode = mode;
    this->delay = delay;

    // Start thread
    th1 = std::thread(executor, std::ref(queue), std::ref(run_thread));
}

spi_sock::~spi_sock()
{
    if (fd != -1)
        close(fd);
    
    run_thread = false;
    th1.join();
}

void spi_sock::transfer_raw_zeroed(const char *msg, unsigned int len, char *response)
{
    int ret;
    uint8_t *tx = (uint8_t *)msg;

    //uint8_t rx[len];

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)response,
        .len = static_cast<__u32>(len),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    /* Send */
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message"); // In testing throws error despite working
}

void spi_sock::transfer_raw(const char *msg, int len, char *response)
{
    // Zero revieve buffer to avoid ioctl errors
    memset(response, 0, len);
    transfer_raw_zeroed(msg, len, response);
}

void spi_sock::wait_btw_transfers(){
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void spi_sock::send_instruction(instruction *ins, uint8_t *resp)
{
    
}

void spi_sock::send_instruction(instruction *ins){
    // Create response buf
    uint8_t resp[RCV_LEN];
    send_instruction(ins, resp);

}

void spi_sock::exchange(instruction *previous, instruction *next){
    int len = std::max(previous->result_s, next->instr_s) +1; // One extra byte to recieve the whole message
    // Create buffers
    uint8_t tx[len];
    tx[0] = next->instr_s; // Prepend instruction size
    memcpy(tx + 1, next->bin.data(), next->instr_s);
    uint8_t rx[len];

    bool sent = false;
    
    transfer_raw((char*)tx, len, (char*)rx);
}


void spi_sock::executor(MT_Queue<instruction*> &q, bool &run_thread){
    instruction *ins;
    std::promise<void> *prom = nullptr;
    while(run_thread)
    {
        // Signal 
        if(prom != nullptr){
            prom->set_value();
            prom = nullptr;
        }

    }
}