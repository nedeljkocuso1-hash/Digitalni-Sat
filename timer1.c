#include <at89c51rc2.h>
#include "inc/timer.h"

bool proslo_5ms = false;
bool proslo_1s = false;  // Dodan
bool proslo_vreme = false;
uint8_t brojac_5ms = 0;
uint16_t brojac_1s = 0;  // Za 1s
uint16_t brojac_polas = 0;
uint16_t brojac_2s = 0;

void init_timer1(uint8_t mod_timer1, uint8_t brojac_nizi, uint8_t brojac_visi)
{
    TMOD &= 0x0f;
    TMOD |= (mod_timer1 & 0xf0);
    TL1 = brojac_nizi;
    TH1 = brojac_visi;
}

void start_timer1()
{
    TR1 = 1;
}

void omoguci_prekid()
{
    ET1 = 1;
}

void prekid_timer1() interrupt 3
{
    if(++brojac_5ms == 20)  // Ispravljeno: 20*5ms = 100ms? Cekaj, za 5ms interval, ali za 1s: 200*5ms=1s
    {
        proslo_5ms = true;
        brojac_5ms = 0;
    }

    if(++brojac_1s == 20)  // 200*5ms = 1s
    {
        proslo_1s = true;
        brojac_1s = 0;
    }
}
