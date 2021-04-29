#include "buttons.h"

void Buttons::init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIOInitStruct.Pin      = MODE_BUTTON_PIN;
    GPIOInitStruct.Mode     = GPIO_MODE_INPUT;
    GPIOInitStruct.Pull     = GPIO_PULLUP;
    HAL_GPIO_Init(MODE_BUTTON_PORT, &GPIOInitStruct);

    GPIOInitStruct.Pin      = DEBUG_BUTTON_PIN;
    GPIOInitStruct.Mode     = GPIO_MODE_INPUT;
    GPIOInitStruct.Pull     = GPIO_PULLUP;
    HAL_GPIO_Init(DEBUG_BUTTON_PORT, &GPIOInitStruct);
}

GPIO_PinState Buttons::readButton(GPIO_TypeDef *port, uint16_t pin) {
    return HAL_GPIO_ReadPin(port, pin);
}