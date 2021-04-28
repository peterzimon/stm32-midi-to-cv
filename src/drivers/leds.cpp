#include "leds.h"

void LEDs::init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Init LEDs
    GPIOInitStruct.Pin      = VOICE_A_LED_PIN | VOICE_B_LED_PIN | VOICE_C_LED_PIN | VOICE_D_LED_PIN;
    GPIOInitStruct.Mode     = GPIO_MODE_OUTPUT_PP;
    GPIOInitStruct.Pull     = GPIO_PULLDOWN;
    GPIOInitStruct.Speed    = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(VOICE_LED_PORT, &GPIOInitStruct);

    // Init Debug stuff
    GPIOInitStruct.Pin      = BUILT_IN_LED_PIN;
    GPIOInitStruct.Mode     = GPIO_MODE_OUTPUT_PP;
    GPIOInitStruct.Pull     = GPIO_NOPULL;
    GPIOInitStruct.Speed    = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUILT_IN_LED_PORT, &GPIOInitStruct);
    HAL_GPIO_WritePin(BUILT_IN_LED_PORT, BUILT_IN_LED_PIN, GPIO_PIN_SET);
}