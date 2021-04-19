/**
 * References:
 * https://github.com/Dwigen/STM32_HAL_MCP4728
 * https://github.com/BenoitSchillings/mcp4728/blob/master/mcp4728.cpp
 * 
 * Use of EEPROM is not implemented
 * 
 * // TODO: implement DAC calibration
*/

#ifndef DAC_H
#define DAC_H

#include <inttypes.h>
#include <mcp4728.h>
#include "stm32f1xx_hal.h"

class DAC {
    public:
        void init(void);
        void write(uint16_t *values);
        void write(uint8_t channel, uint16_t value);
    
    private:
        I2C_HandleTypeDef _I2CConfig;
        MCP4728 _mcp4728;
};

#endif