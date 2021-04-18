/**
 * References:
 * https://github.com/Dwigen/STM32_HAL_MCP4728
 * https://github.com/BenoitSchillings/mcp4728/blob/master/mcp4728.cpp
 * 
 * Use of EEPROM is not implemented
 * 
 * // TODO: implement DAC calibration
*/

#ifndef SMT32LIB_MCP4728_H
#define SMT32LIB_MCP4728_H

#define MCP4728_DEFAULT_VDD         5000
#define MCP4728_CMD_FASTWRITE		0x00
#define MCP4728_CMD_DACWRITE_MULTI	0x40
#define MCP4728_CMD_DACWRITE_SEQ	0x50
#define MCP4728_CMD_DACWRITE_SINGLE	0x58
#define MCP4728_CMD_ADDRWRITE		0x60
#define	MCP4728_CMD_VREFWRITE		0x80
#define MCP4728_CMD_GAINWRITE		0xC0
#define MCP4728_CMD_PWRDWNWRITE		0xA0

#define MCP4728_BASEADDR			0x60<<1
#define MCP4728_GENERAL_CALL        0x00

#define MCP4728_VREF_VDD			0x0
#define MCP4728_VREF_INTERNAL		0x1

#define MCP4728_GAIN_1				0x0
#define MCP4728_GAIN_2				0x1

#define MCP4728_CHANNEL_A			0x0
#define MCP4728_CHANNEL_B			0x1
#define MCP4728_CHANNEL_C			0x2
#define MCP4728_CHANNEL_D			0x3

#define MCP4728_PWRDWN_NORMAL		0x0
#define MCP4728_PWRDWN_1			0x1
#define MCP4728_PWRDWN_2			0x2
#define MCP4728_PWRDWN_3			0x3

#define MCP4728_UDAC_UPLOAD			0x1
#define MCP4728_UDAC_NOLOAD			0x0

#define MCP4728_GENERAL_RESET		0x06
#define MCP4728_GENERAL_WAKEUP		0x09
#define MCP4728_GENERAL_UPDATE	    0x08
#define MCP4728_GENERAL_READADDR	0x0C


#include <inttypes.h>
#include "stm32f1xx_hal.h"

class MCP4728 {
    public:
        void init(I2C_HandleTypeDef *I2CConfig, uint8_t deviceID);
        void setVDD(uint16_t vdd);
        void reset(void);
        void wake(void);
        void update(void);
        void setVref(uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4);
        void setVref(uint8_t channel, uint8_t value);
        void setGain(uint8_t value1, uint8_t value2, uint8_t value3, uint8_t value4);
        void setGain(uint8_t channel, uint8_t value);
        void write(uint16_t value1, uint16_t value2, uint16_t value3, uint16_t value4);
        void write(uint8_t channel, uint16_t value);

    private:
        I2C_HandleTypeDef *_I2CConfig;
        uint8_t _deviceID;
        uint8_t _deviceAddress;
        uint8_t _intVrefs[4];
        uint8_t _gains[4];
        uint16_t _values[4];

        uint16_t _vdd;

        void _writeVref(void);
        void _writeGain(void);
        void _fastWrite(void);
        void _generalCall(uint8_t command);

        void _transmit(uint8_t *data);
        uint8_t _hiByte(uint16_t hw);
        uint8_t _loByte(uint16_t hw);
};

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