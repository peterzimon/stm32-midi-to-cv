// All retarget code credit to Carmine Noviello for this code
// https://github.com/cnoviello/mastering-stm32/blob/master/nucleo-f030R8/system/include/retarget/retarget.h

#include "serial.h"

// Retarget definitions and variables
#if !defined(OS_USE_SEMIHOSTING)
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
UART_HandleTypeDef *gHuart;
#endif

void Serial::init(void) {
    
    // Initialize UART for console logging on USART2
    consoleUARTConfig.Instance             = USART2;
    consoleUARTConfig.Init.BaudRate        = 115200;
    consoleUARTConfig.Init.WordLength      = UART_WORDLENGTH_8B;
    consoleUARTConfig.Init.StopBits        = UART_STOPBITS_1;
    consoleUARTConfig.Init.Parity          = UART_PARITY_NONE;
    consoleUARTConfig.Init.Mode            = UART_MODE_TX;             // Transmitting data only
    consoleUARTConfig.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
    consoleUARTConfig.Init.OverSampling    = UART_OVERSAMPLING_16;

    HAL_UART_Init(&consoleUARTConfig);

    #if !defined(OS_USE_SEMIHOSTING)
    _retargetInit();
    #endif

    // Initialize UART for MIDI communication on USART3
    midiUARTConfig.Instance             = USART3;
    midiUARTConfig.Init.BaudRate        = 31250;
    midiUARTConfig.Init.WordLength      = UART_WORDLENGTH_8B;
    midiUARTConfig.Init.StopBits        = UART_STOPBITS_1;
    midiUARTConfig.Init.Parity          = UART_PARITY_NONE;
    midiUARTConfig.Init.Mode            = UART_MODE_RX;             // Only receive for now
    midiUARTConfig.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
    midiUARTConfig.Init.OverSampling    = UART_OVERSAMPLING_16;

    HAL_UART_Init(&midiUARTConfig);

    clrScreen();
}

// Writing console
void Serial::clrScreen() {
    HAL_UART_Transmit(&consoleUARTConfig, (uint8_t*)"\033[0;0H", strlen("\033[0;0H"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&consoleUARTConfig, (uint8_t*)"\033[2J", strlen("\033[2J"), HAL_MAX_DELAY);
}

// Reading MIDI
bool Serial::midiReadable(void) {
    return USART3->SR & USART_FLAG_RXNE;
}

uint8_t Serial::midiRead(void) {
    return USART3->DR;
}

void Serial::_retargetInit(void) {
    #if !defined(OS_USE_SEMIHOSTING)
    gHuart = &consoleUARTConfig;

    /* Disable I/O buffering for STDOUT stream, so that
    * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
    #endif
}

/**
 * HAL functions
*/
extern "C" 
{

// Init pins that can be used as UART. This is a callback function that is
// inside HAL_UAR_Init(); and is designed to setup these kinda things.
// Note that this function is called for all UARTs so we need to check
// for the given UART.
void HAL_UART_MspInit(UART_HandleTypeDef * uart) {
    GPIO_InitTypeDef uartGPIOConfig;
    if (uart->Instance == USART2) {         // Is it UART2? Fuck ye

        // UART2 has the following pins on the MCU:
        // TX --> PA2
        // RX --> PA3
        // So we need to set these up.

        __HAL_RCC_USART2_CLK_ENABLE();      // Need to enable UART...
        __HAL_RCC_GPIOA_CLK_ENABLE();       // ...and the GPIOA port clock.

        uartGPIOConfig.Pin = GPIO_PIN_2;    // This is TX pin to transmit data
        uartGPIOConfig.Mode = GPIO_MODE_AF_PP;      // I guess this could be GPIO_MODE_AF_PP too
        uartGPIOConfig.Speed = GPIO_SPEED_FREQ_HIGH;

        HAL_GPIO_Init(GPIOA, &uartGPIOConfig);
    }

    if (uart->Instance == USART3) {         // Is it UART3

        // UART2 has the following pins on the MCU:
        // TX --> PB10
        // RX --> PB11
        // So we need to set these up.

        __HAL_RCC_USART3_CLK_ENABLE();      // Need to enable UART...
        __HAL_RCC_GPIOB_CLK_ENABLE();       // ...and the GPIOA port clock.

        uartGPIOConfig.Pin = GPIO_PIN_11;    // This is TX pin to transmit data
        uartGPIOConfig.Mode = GPIO_MODE_INPUT;      // I guess this could be GPIO_MODE_AF_PP too
        uartGPIOConfig.Speed = GPIO_SPEED_FREQ_HIGH;
        uartGPIOConfig.Pull = GPIO_PULLUP;

        HAL_GPIO_Init(GPIOA, &uartGPIOConfig);
    }
}

}


/**
 * Retarget standard output. 
 * https://shawnhymel.com/1873/how-to-use-printf-on-stm32/
*/
#if !defined(OS_USE_SEMIHOSTING)
extern "C" {

int _isatty(int fd) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 1;

    errno = EBADF;
    return 0;
}

int _write(int fd, char* ptr, int len) {
    HAL_StatusTypeDef hstatus;

    if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
        hstatus = HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
        if (hstatus == HAL_OK)
        return len;
        else
        return EIO;
    }
    errno = EBADF;
    return -1;
}

int _close(int fd) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
        return 0;

    errno = EBADF;
    return -1;
}

int _lseek(int fd, int ptr, int dir) {
    (void) fd;
    (void) ptr;
    (void) dir;

    errno = EBADF;
    return -1;
}

int _read(int fd, char* ptr, int len) {
    HAL_StatusTypeDef hstatus;

    if (fd == STDIN_FILENO) {
        hstatus = HAL_UART_Receive(gHuart, (uint8_t *) ptr, 1, HAL_MAX_DELAY);
        if (hstatus == HAL_OK)
        return 1;
        else
        return EIO;
    }
    errno = EBADF;
    return -1;
}

int _fstat(int fd, struct stat* st) {
    if (fd >= STDIN_FILENO && fd <= STDERR_FILENO) {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return 0;
}

}
#endif