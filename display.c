#include "inc/display.h"
#include <intrins.h>
#include <at89c51rc2.h>

void initP1P3(void) {           
    // Inicijalizacija portova koji se koriste za LCD 
    P1 = 0xE0;
    P3 = 0xF3;
}

void wait1s(void) {             
    uint8_t i;

    // Cuvaju se podesavanja za Timer1
    // Timer0 u 16bitnom rezimu rada
    TMOD = (TMOD&0xF0) | 0x01;  
    ET0 = 0;

    // Polling metoda - 5ms*200=1s
    for(i=0;i<200;i++) {
        //5ms
        TH0 = 0xEE;             
        TL0 = 0x00;             
        TR0 = 1;                    
        while(!TF0); 
        TF0 = 0;                 
    }
    
    TR0=0;                        
}

void wait50micro(void) {
    // Cuvaju se podesavanja za Timer1
    // Timer0 u 16bitnom rezimu rada
    TMOD = (TMOD&0xF0) | 0x01; 
    ET0 = 0;

    // 50us
    TH0=0xFF;
    TL0=0xD2;
    TR0=1;                     
    while(!TF0);
    TF0=0;
    TR0=0;   
}

void wait2ms(void) {
    // Cuvaju se podesavanja za Timer1
    // Timer0 u 16bitnom rezimu rada
    TMOD = (TMOD&0xF0) | 0x01;
    ET0 = 0; 

    // 2ms
    TH0=0xF8;           
    TL0=0xCD;
    TR0=1;
    while(!TF0);
    TF0=0;
    TR0=0;  
}

void entry_setDisplay(void){
    LCD_RS=0;
              
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;
    
    _nop_();
          
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=1;
    LCD_D5=1;  // upisom karaktera pozicija kursora se inkrementira na desno
    LCD_D4=0;  // displej nije siftovan, prikazuju se karakteri na adresama 0x00-0x07, i 0x40-0x47
    LCD_EN=0;

    wait50micro();
}

void function_setDisplay(void){
    LCD_RS=0;

//  LCD_EN=1;
//  LCD_D7=0;
//  LCD_D6=0;
//  LCD_D5=1;  // 4bit interface
//  LCD_D4=0;
//  LCD_EN=0;
//
//  _nop_();
              
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=1;  // 4bit interface
    LCD_D4=0;
    LCD_EN=0;
    
    _nop_();
          
    LCD_EN=1;
    LCD_D7=1;  // displej koristi dva reda      
    LCD_D6=0;  // nije upaljen trenutno
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;

    wait50micro();
}

void clearDisplay(void) {
    // Funkcija koja brise sve sto pise na displeju, i u memoriji               
    LCD_RS=0;
              
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;
    
    _nop_();
          
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=1;
    LCD_EN=0;

    wait2ms();
    wait2ms();
}

void on_offDisplay(void){
    LCD_RS=0;
              
    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;
    
    _nop_();
          
    LCD_EN=1;
    LCD_D7=1;
    LCD_D6=1; // upaljen displej
    LCD_D5=1; // upaljen kursor
    LCD_D4=1; // upaljen blink indikator kursora
    LCD_EN=0;

    wait50micro();  
}

void initDisplay(void) {     
    // Inicijalizacija displeja na 4bitni rezim rada             
    initP1P3(); 

    // Iskljucivanje X2 moda rada
    CKCON0 &= ~(1<<0);

    // Cekanje nakon paljenja
    wait1s();

    // Konfiguracija 4bitnog rezima
    function_setDisplay();

    // Paljenje displeja
    on_offDisplay();

    // Ciscenje memorije i displeja
    clearDisplay();

    // Podesavanje unosa
    entry_setDisplay();
           
    wait2ms();

    // Paljenje pozadinskog osvetljenja
    LCD_BL=1;
}

void writeChar(uint8_t karakter) {
    // Funkcija koja pise 1 karakter na displej 
    LCD_RS=1;

    LCD_EN=1;                     
    LCD_D7=(karakter >> 7) & 0x01; 
    LCD_D6=(karakter >> 6) & 0x01; 
    LCD_D5=(karakter >> 5) & 0x01; 
    LCD_D4=(karakter >> 4) & 0x01; 
    LCD_EN=0;

    _nop_();

    LCD_EN=1;
    LCD_D7=(karakter >> 3) & 0x01; 
    LCD_D6=(karakter >> 2) & 0x01;
    LCD_D5=(karakter >> 1) & 0x01; 
    LCD_D4=(karakter >> 0) & 0x01; 
    LCD_EN=0;

    wait50micro();
}

void writeLine(uint8_t* message) {  
    // Funkcija koja pise jedan string na displej - iterira kroz string i poziva funkciju writeChar za svako slovo
    uint8_t n=0;
    while(message[n]!='\0') {
        writeChar(message[n]);
        n++;
    }
}

void newLine(void) {                        
    // Funkcija za prelazak u novi red na displeju
    LCD_RS=0;

    LCD_EN=1;   
    LCD_D7=1;
    LCD_D6=1;  // DDRAM adresa se postavlja na 0x40
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;

    _nop_();

    LCD_EN=1;
    LCD_D7=0;
    LCD_D6=0;
    LCD_D5=0;
    LCD_D4=0;
    LCD_EN=0;

    wait2ms();
    wait2ms();
}
