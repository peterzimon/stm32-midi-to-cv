#include "gate.h"

void Gate::init(void) {
    _pinConfig.Pin = GATE_A | GATE_B | GATE_C | GATE_D;
    _pinConfig.Pull = GPIO_NOPULL;
    _pinConfig.Mode = GPIO_MODE_OUTPUT_PP;
    _pinConfig.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GATE_PORT, &_pinConfig);
}

void Gate::update(int *gates) {
    for (int i = 0; i < VOICES; i++) {
        setGate(_gatePins[i], gates[i]);
    }
}

/**
 * gate: [GATE_A|GATE_B|GATE_C|GATE_D]
 * value: [0|1]
*/
void Gate::setGate(uint16_t gate, int value) {
    HAL_GPIO_WritePin(GATE_PORT, gate, (value ? GPIO_PIN_SET : GPIO_PIN_RESET));
}