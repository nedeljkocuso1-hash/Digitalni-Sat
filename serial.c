#include "inc/serial.h"
#include "inc/protocol.h"
#include <at89c51rc2.h>

uint8_t *tx_buffer;
char rx_buffer[BUFFER_SIZE];
uint8_t rxw_index = 0;
uint8_t rxr_index = 0;
uint8_t rx_byte;

void Serial_Init(uint8_t SMOD1, uint8_t SPD, uint8_t br1)
{
    PCON &= ~((1<<6) | (1<<7));
    PCON |= (SMOD1 << 7);
    SCON &= ~(1<<7);
    SCON |= (1<<6);
    SCON |= (1<<4);
    BDRCON |= ((1<<2) | (1<<3) | (1<<4));
    BDRCON &= ~(1<<1);
    BDRCON |= (SPD << 1);
    BRL = br1;
}

void send(uint8_t* msg)
{
    tx_buffer = msg;
    SBUF = *tx_buffer;
}

void SerialIT(void) interrupt 4
{
    if(TI)
    {
        ++tx_buffer;
        if(*tx_buffer != '\0')
        {
            SBUF = *tx_buffer;
        }
        TI = 0;
    }
    else if(RI)
    {
        rx_byte = SBUF;
        if(rx_byte == MSG_START)
        {
            rxr_index = rxw_index;
        }
        rx_buffer[rxw_index] = rx_byte;
        if(rx_byte == MSG_END)
        {
            uint8_t len = 0;
            if(rxw_index < rxr_index)
            {
                len = BUFFER_SIZE - rxr_index + rxw_index - 1;
            }
            else
            {
                len = rxw_index - rxr_index - 1;
            }
            parse(rx_buffer, rxr_index, len);
        }
        rxw_index = (rxw_index + 1) % BUFFER_SIZE;
        RI = 0;
    }
}
void send_alarm()
{
    send("ALARM!\r\n");
}
