#include "ui.h"

/* 
 * Configure LEDs
 * ------------------------------------------------------------------------- */
void UI::init(void) {

    // Enable GPIO clocks (only  port C needs to be enabled)
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Init LED GPIO port
    GPIOInitStruct.Pin      = LED_PIN;
    GPIOInitStruct.Mode     = GPIO_MODE_OUTPUT_PP;
    GPIOInitStruct.Pull     = GPIO_NOPULL;
    GPIOInitStruct.Speed    = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIOInitStruct);
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

    GPIOInitStruct.Pin      = DEBUG_BUTTON_PIN;
    GPIOInitStruct.Mode     = GPIO_MODE_INPUT;
    GPIOInitStruct.Pull     = GPIO_PULLUP;
    HAL_GPIO_Init(DEBUG_BUTTON_PORT, &GPIOInitStruct);
}