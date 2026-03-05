#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdbool.h>

void GlobalIT_enable(bool on_off);
void SerialIT_enable(bool on_off);

#endif
