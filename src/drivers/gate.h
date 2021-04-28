#ifndef GATE_H
#define GATE_H

#include "stm32f1xx_hal.h"
#include "settings.h"

class Gate {
    public:
        void init(void);
        void update(int *gates);
        void setGate(uint16_t gate, int value);

    private:
        GPIO_InitTypeDef _pinConfig;
        uint16_t _gatePins[VOICES] = {GATE_A, GATE_B, GATE_C, GATE_D};
};

#endif