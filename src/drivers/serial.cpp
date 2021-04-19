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
    
    #ifdef SERIAL_DEBUG
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

// Reading MIDI
bool Serial::midiReadable(void) {
    return USART3->SR & USART_FLAG_RXNE;
}

uint8_t Serial::midiRead(void) {
    return USART3->DR;
}

// Writing console
void Serial::clrScreen() {
    #ifdef SERIAL_DEBUG
    HAL_UART_Transmit(&consoleUARTConfig, (uint8_t*)"\033[0;0H", strlen("\033[0;0H"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&consoleUARTConfig, (uint8_t*)"\033[2J", strlen("\033[2J"), HAL_MAX_DELAY);
    #endif
}

void Serial::_retargetInit(void) {
    #ifdef SERIAL_DEBUG
    #if !defined(OS_USE_SEMIHOSTING)
    gHuart = &consoleUARTConfig;

    /* Disable I/O buffering for STDOUT stream, so that
    * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
    #endif
    #endif
}

/**
 * Retarget standard output. 
 * https://shawnhymel.com/1873/how-to-use-printf-on-stm32/
*/
#ifdef SERIAL_DEBUG
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
#endif

// Related HAL Msp callback functions in system.cpp