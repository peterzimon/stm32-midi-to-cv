#ifndef SYSTEM_H
#define SYSTEM_H

#include "stm32f1xx_hal.h"

class System {
    public: 
        void init(void);

    private:
        void initSysClock(void);
};

#endif

