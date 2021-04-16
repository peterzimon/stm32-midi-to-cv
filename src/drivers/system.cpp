#include "system.h"

void System::init(void) {
    HAL_Init();
    initSysClock();
}

/* 
 * Configure system clock
 * Notes:
 * uint32_t HAL_RCC_GetSysClockFreq(void) returns system clock frequency
 * ------------------------------------------------------------------------- */
void System::initSysClock(void) {

    // Set up an external high frequency oscillator to be used. 
    RCC_OscInitTypeDef oscillatorConfig;
    oscillatorConfig.OscillatorType   = RCC_OSCILLATORTYPE_HSE;         // Sets up a high speed external oscillator. We know it's an 8MHz crystal
    oscillatorConfig.HSEState         = RCC_HSE_ON;                     // Turns it on
    oscillatorConfig.HSEPredivValue   = RCC_HSE_PREDIV_DIV1;            // Divider is 1, we want the full speed
    oscillatorConfig.HSIState         = RCC_HSI_ON;                     // @TODO: check if this is needed
    oscillatorConfig.PLL.PLLState     = RCC_PLL_ON;                     // Use PLL 
    oscillatorConfig.PLL.PLLSource    = RCC_PLLSOURCE_HSE;              // Sets the PLL source to external crystal
    oscillatorConfig.PLL.PLLMUL       = RCC_PLL_MUL9;                   // Multiply the clock source by 9 to get the max 72MHz frequency
    
    HAL_RCC_OscConfig(&oscillatorConfig);

    // Set up clock
    RCC_ClkInitTypeDef clockConfig;
    clockConfig.ClockType       =   RCC_CLOCKTYPE_SYSCLK | 
                                    RCC_CLOCKTYPE_HCLK | 
                                    RCC_CLOCKTYPE_PCLK1 | 
                                    RCC_CLOCKTYPE_PCLK2;                // Set up all clock types at once
    clockConfig.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;              // Set system clock source to PLL clock
    clockConfig.AHBCLKDivider   = RCC_SYSCLK_DIV1;                      // Set AHB clock to 72MHz. It allows to be the max
    clockConfig.APB1CLKDivider  = RCC_HCLK_DIV2;                        // APB1 clock can be max 36MHz so divide it by 2
    clockConfig.APB2CLKDivider  = RCC_HCLK_DIV1;                        // APB2 clock can run on 72MHz so divide it by 1

    HAL_RCC_ClockConfig(&clockConfig, FLASH_LATENCY_2);                 // Flash latency is 2 for ¯\_(ツ)_/¯
}

/* 
 * System callback functions
 * ------------------------------------------------------------------------- */
extern "C" {

void SysTick_Handler(void) { 
    HAL_IncTick();
}
void NMI_Handler(void) { }
void HardFault_Handler(void) { while (1) {} }
void MemManage_Handler(void) { while (1) {} }
void BusFault_Handler(void) { while (1) {} }
void UsageFault_Handler(void) { while (1) {} }
void SVC_Handler(void) { }
void DebugMon_Handler(void) { }
void PendSV_Handler(void) { }

/* 
* HAL MSP stuff. This is a callback from HAL_Init where it's possible to
* override default configurations.
* ------------------------------------------------------------------------- */
void HAL_MspInit(void) {
    __HAL_RCC_AFIO_CLK_ENABLE();        // ???
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();      // Enable serial wire JTAG configuration
}

}