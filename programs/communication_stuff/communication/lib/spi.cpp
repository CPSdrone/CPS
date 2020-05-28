#include "spi.h"

static void pabort(const char *s)
{
    perror(s);
    abort();
}

std::mutex spi_sock::m;

spi_sock::spi_sock(uint32_t speed, uint8_t mode, uint16_t delay)
{
    this->speed = speed;
    this->mode = mode;
    this->delay = delay;

    /* Connect do SPI port */
    fd = open(device, O_RDWR);
    if(fd == -1)
        pabort("Failed to create socket\n");
    //ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    printf("Created an SPI socket!\n");
}

spi_sock::~spi_sock()
{
    if (fd != -1)
        close(fd);
}

void spi_sock::transfer_raw_zeroed(const uint8_t *msg, unsigned int len, uint8_t *response)
{
    int ret;
    uint8_t *tx = (uint8_t *)msg;

    //uint8_t rx[len];

    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(tr));
    tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)response,
        .len = static_cast<__u32>(len),
        .speed_hz = speed,
        .delay_usecs = delay,
        .bits_per_word = bits,
    };

    /* Send */
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    
    /*
    if (ret < 1)
        pabort("can't send spi message"); // In testing ioctl throws error despite working
    */
        
}

void spi_sock::transfer_raw(const uint8_t *msg, int len, uint8_t *response)
{
    // Zero revieve buffer to avoid ioctl errors
    memset(response, 0, len);
    transfer_raw_zeroed(msg, len, response);
}

void spi_sock::transfer_bytes(const uint8_t *msg, uint32_t len, uint8_t *resp)
{
    memset(resp, 0, len);
    for (size_t i = 0; i < len; i++)
        transfer_raw_zeroed(msg+i, 1, resp+i);
    
}

void spi_sock::wait_btw_transfers(){
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 1ms works, setting to 10 for testing
}

uint8_t spi_sock::wait_until_R2RCV(){
    // Check if waiting for instructions
    uint8_t snd_buf = SIGNAL_POLL;
    uint8_t rcv_buf = 0x00;
    
    do{
        uint8_t useless = 0xff;
        std::this_thread::sleep_for(POLL_TIME);
        transfer_raw(&snd_buf, 1, &rcv_buf);
    }while(rcv_buf == 0xff);
    
    return rcv_buf;
}

void spi_sock::send_instruction(instruction &ins)
{
    if(ins.rcv_buf == nullptr && ins.result_s > 0){
        std::cerr << "No memory allocated for return buffer\n";
        std::exit(-1);
    }
    std::lock_guard<std::mutex> lock(m);
    
    uint8_t len = ins.instr_s;
    uint8_t tmp[len];
    tmp[0] = 0x00;
    transfer_raw_zeroed(&len, 1, tmp);
    transfer_bytes(ins.bin.data(), len, tmp );
    
    uint8_t len = wait_until_R2RCV();
    #ifdef DEBUG
    if (len != ins.result_s)
        std::cout << "Response size mismatch!" << '\n' ;
    #endif
    if(len > 0){
        uint8_t garbage[len];
        memset(garbage, 0xff, len);
        transfer_bytes(garbage, len, ins.rcv_buf);
    }
}
