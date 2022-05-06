#ifndef CAPTOUCHSENS_H_
#define CAPTOUCHSENS_H_

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "capTouchSens.pio.h"
#include "hardware/irq.h"


template<uint sensPinCount>
class capTouchSens {
    PIO pio;
    uint excPinBase;
    uint sensPinBase;
    uint sensPinPolMsk;

    uint capVal[sensPinCount];
    uint sumBuf[4];
    uint sumCnt;
    uint sampleCnt;
    uint sumShift;
    uint aktivPin;

    void setSensCh(uint sm, uint gpio)
    {
        uint32_t tmp = pio->sm[sm].execctrl;
        tmp &= PIO_SM0_EXECCTRL_JMP_PIN_BITS;
        tmp |= gpio << PIO_SM0_EXECCTRL_JMP_PIN_LSB;
        pio->sm[sm].execctrl = tmp;
    }

    inline void setNextSensPins()
    {
        uint tmp = aktivPin;
        for(int i = 0; i < 4; i++)
        {
            setSensCh(i, tmp + sensPinBase);
            if(tmp == sensPinCount - 1)
            {
                tmp = 0;
            }
            else
            {
                tmp++;
            }
        }
    }

public:
    void init(PIO aPio, uint aExcPinBase, uint aSensPinBase, uint aSensPinPolMsk)
    {
        pio = aPio;
        excPinBase = aExcPinBase;
        sensPinBase = aSensPinBase;
        sensPinPolMsk = aSensPinPolMsk;

        capTouchSens_init(aPio, aExcPinBase, aSensPinBase, sensPinCount, aSensPinPolMsk);

        for(uint i = 0; i < sensPinCount; i++)
        {
            sumBuf[i] = 0;
        }
        setOverSampling(256);
        sumCnt = 0;
        aktivPin = 0;

        setNextSensPins();
    }

    void enable() 
    {
        pio_set_sm_mask_enabled (pio, 0xF, true); 
    };
    
    void setOverSampling(uint aSampleCnt)
    {
        uint tmp = aSampleCnt - 1;
        uint cnt = 0;
        while(tmp)
        {
            tmp >>= 1;
            cnt++;
        }
        sumShift = cnt;
        sampleCnt = 1 << cnt;
    }

    void setIrqHandler(void (*aIrqHandler)(void))
    {
        pio_set_irq0_source_enabled(pio, pis_interrupt0 , true);
        if(pio == pio0)
        {       
            irq_set_exclusive_handler(PIO0_IRQ_0, aIrqHandler);
            irq_set_enabled (PIO0_IRQ_0, true);
        }
        else
        {
            irq_set_exclusive_handler(PIO1_IRQ_0, aIrqHandler);
            irq_set_enabled (PIO1_IRQ_0, true);
        }
    }

    inline void irqHandler()
    {
        pio_interrupt_clear(pio, 0);

        for(int i = 0; i < 4; i++)
        {
            sumBuf[i] += pio->rxf[i];
        }

        if(sumCnt == sampleCnt - 1)
        {
            for(int i = 0; i < 4; i++)
            {
                capVal[aktivPin] = sumBuf[i] >> sumShift;
                if(aktivPin == sensPinCount - 1)
                {
                    aktivPin = 0;
                }
                else
                {
                    aktivPin++;
                }
            }
            sumCnt = 0;
            setNextSensPins();
        }
        else
        {
            sumCnt++;
        }
    }
};

#endif /* CAPTOUCHSENS_H_ */
