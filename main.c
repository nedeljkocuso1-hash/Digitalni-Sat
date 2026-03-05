#include <at89c51rc2.h>
#include "inc/timer.h"
#include "inc/serial.h"
#include "inc/protocol.h"
#include "inc/interrupts.h"
#include "inc/gpio.h"
#include "inc/display.h"
#include <stdint.h>

// Vreme
uint8_t sati = 0, minuti = 0, sekunde = 0;
uint8_t alarm_sati = 0, alarm_minuti = 0, alarm_sekunde = 0;

// Modovi
bool mod_vreme = true;  // true: pode�avanje vremena, false: alarma

// Funkcija za formatiranje vremena u string
void format_time(uint8_t h, uint8_t m, uint8_t s, uint8_t* str)
{
    str[0] = '0' + h / 10;
    str[1] = '0' + h % 10;
    str[2] = ':';
    str[3] = '0' + m / 10;
    str[4] = '0' + m % 10;
    str[5] = ':';
    str[6] = '0' + s / 10;
    str[7] = '0' + s % 10;
    str[8] = '\0';
}

// A�uriranje displeja
void updateDisplay()
{
    uint8_t time_str[9];
    uint8_t alarm_str[9];
    format_time(sati, minuti, sekunde, time_str);
    format_time(alarm_sati, alarm_minuti, alarm_sekunde, alarm_str);
    clearDisplay();
    writeLine(time_str);
    newLine();
    writeLine(alarm_str);
}

// Obrada tastera
void handle_buttons()
{
    uint8_t p0 = getP0();
    static uint8_t prev_p0 = 0xFF;
    if(p0 == prev_p0) return;  // Debouncing
    prev_p0 = p0;

    if(!(p0 & (1<<0)))  // P0.0: Mod
    {
        mod_vreme = !mod_vreme;
        updateDisplay();
    }
    if(!(p0 & (1<<1)))  // P0.1: Sati +
    {
        if(mod_vreme) { sati = (sati + 1) % 24; } else { alarm_sati = (alarm_sati + 1) % 24; }
        updateDisplay();
    }
    if(!(p0 & (1<<2)))  // P0.2: Sati -
    {
        if(mod_vreme) { sati = (sati == 0) ? 23 : sati - 1; } else { alarm_sati = (alarm_sati == 0) ? 23 : alarm_sati - 1; }
        updateDisplay();
    }
    if(!(p0 & (1<<3)))  // P0.3: Minuti +
    {
        if(mod_vreme) { minuti = (minuti + 1) % 60; } else { alarm_minuti = (alarm_minuti + 1) % 60; }
        updateDisplay();
    }
    if(!(p0 & (1<<4)))  // P0.4: Minuti -
    {
        if(mod_vreme) { minuti = (minuti == 0) ? 59 : minuti - 1; } else { alarm_minuti = (alarm_minuti == 0) ? 59 : alarm_minuti - 1; }
        updateDisplay();
    }
    if(!(p0 & (1<<5)) && mod_vreme)  // P0.5: Sekunde + (samo za vreme)
    {
        sekunde = (sekunde + 1) % 60;
        updateDisplay();
    }
}

void main()
{
    // Inicijalizacija
    init_gpio(0xFF, 0x00);  // P0 ulaz, P2 izlaz
    initDisplay();
    Serial_Init(0, 0, 0xFD);  // Prilagodi baud rate
    GlobalIT_enable(true);
    SerialIT_enable(true);
    init_timer1(0x10, 0x00, 0xEE);  // Za 5ms interval (prilagodi)
    omoguci_prekid();
    start_timer1();

    updateDisplay();

    while(1)
    {
        if(proslo_1s)
        {
            proslo_1s = false;
            sekunde++;
            if(sekunde == 60) { sekunde = 0; minuti++; }
            if(minuti == 60) { minuti = 0; sati++; }
            if(sati == 24) { sati = 0; }
            updateDisplay();

            // Provera alarma
            if(sati == alarm_sati && minuti == alarm_minuti && sekunde == alarm_sekunde)
            {
                send_alarm();
                // Opcionalno: trepni LED na P2
                setP2(0xFF);  // Upali
                wait1s();     // Koristi postojecu funkciju
                setP2(0x00);  // Ugasi
            }
        }
        handle_buttons();
    }
}
