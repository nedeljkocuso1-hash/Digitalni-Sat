#ifndef TIMER1_H
#define TIMER1_H

#include <stdint.h>
#include <stdbool.h>

extern bool proslo_5ms;
extern bool proslo_1s;  // Dodan za 1s
extern bool proslo_vreme;

void init_timer1(uint8_t mod_timer1, uint8_t brojac_nizi, uint8_t brojac_visi);
void start_timer1();
void omoguci_prekid();
void prekid_timer1();

#endif
