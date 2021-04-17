/**
 * MIDI is just receive only for now.
*/

#ifndef SERIAL_H
#define SERIAL_H

#include <string.h>
#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/stat.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>

#include "settings.h"
#include "stm32f1xx_hal.h"


class Serial {
    public:
        UART_HandleTypeDef consoleUARTConfig;
        UART_HandleTypeDef midiUARTConfig;

        void init(void);
        
        // Read functions
        bool midiReadable(void);
        uint8_t midiRead(void);

        // Console functions
        void clrScreen(void);

    private:
        void _retargetInit(void);
};

extern "C" {

// void RetargetInit(UART_HandleTypeDef *huart);

int _isatty(int fd);
int _write(int fd, char* ptr, int len);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
int _read(int fd, char* ptr, int len);
int _fstat(int fd, struct stat* st);

}

#endif