#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>

#define MSG_START '<'
#define MSG_END '>'
#define BUFFER_SIZE 18

// Eksterne varijable za vreme i alarm
extern uint8_t sati, minuti, sekunde;
extern uint8_t alarm_sati, alarm_minuti, alarm_sekunde;

void parse(char* buffer, uint8_t start, uint8_t len);
#endif
