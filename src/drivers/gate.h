#ifndef GATE_H
#define GATE_H

#include "stm32f1xx_hal.h"
#include "settings.h"

#define GATE_PORT       GPIOB
#define GATE_A          GPIO_PIN_12
#define GATE_B          GPIO_PIN_13
#define GATE_C          GPIO_PIN_14
#define GATE_D          GPIO_PIN_15

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