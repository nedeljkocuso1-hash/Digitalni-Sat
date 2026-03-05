#include "inc/protocol.h"
#include <stdint.h>
#include "inc/serial.h"

uint8_t* ok_msg = "OK\r\n";
uint8_t* err_msg = "ERROR\r\n";

extern void updateDisplay();  // Za a�uriranje displeja

void parse(char* buffer, uint8_t start_index, uint8_t len)
{
    if(buffer[start_index] == MSG_START)
    {
        char cmd[BUFFER_SIZE];
        uint8_t index = start_index + 1;
        uint8_t cmd_index = 0;
        while(cmd_index<len)
        {
            cmd[cmd_index++] = buffer[index];
            index = (index + 1) % BUFFER_SIZE;
        }
		cmd[cmd_index] = '\0';

        // TIME komanda: <TIME HH:MM:SS>
        if(cmd_index == 13 && cmd[0] == 'T' && cmd[1] == 'I' && cmd[2] == 'M' && cmd[3] == 'E' && cmd[4] == ' ')
        {
            uint8_t h = (cmd[5] - '0') * 10 + (cmd[6] - '0');
            uint8_t m = (cmd[8] - '0') * 10 + (cmd[9] - '0');
            uint8_t s = (cmd[11] - '0') * 10 + (cmd[12] - '0');
            if(h < 24 && m < 60 && s < 60)
            {
                sati = h; minuti = m; sekunde = s;
                send(ok_msg);
                updateDisplay();
            }
            else
            {
                send(err_msg);
            }
        }
        // ALARM komanda: <ALARM HH:MM:SS>
        else if(cmd_index == 14 && cmd[0] == 'A' && cmd[1] == 'L' && cmd[2] == 'A' && cmd[3] == 'R' && cmd[4] == 'M' && cmd[5] == ' ')
        {
            uint8_t h = (cmd[6] - '0') * 10 + (cmd[7] - '0');
            uint8_t m = (cmd[9] - '0') * 10 + (cmd[10] - '0');
            uint8_t s = (cmd[12] - '0') * 10 + (cmd[13] - '0');
            if(h < 24 && m < 60 && s < 60)
            {
                alarm_sati = h; alarm_minuti = m; alarm_sekunde = s;
                send(ok_msg);
                updateDisplay();
            }
            else
            {
                send(err_msg);
            }
        }
       
        else
        {
            send(err_msg);
        }
    }
    else
    {
        send(err_msg);
    }
}
