/*
    GPIO Hal Library

    Created on: July 31, 2023

    Author: Coskun ERGAN
*/

#include "gpio_hal.h"

namespace gpio_hal
{
    void GpioOutput::_init(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow)
    {
        if(port == GPIOA)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
        }        
        else if(port == GPIOB)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
        }
        else if(port == GPIOC)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
        }
        else if(port == GPIOD)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
        }
        else if(port == GPIOE)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
        }
        else if(port == GPIOF)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
        }
        else if(port == GPIOG)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOG, ENABLE);
        }
        else if(port == GPIOH)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE);
        }

        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
        GPIO_Init((GPIO_TypeDef*)port, &GPIO_InitStructure);
        _active_low = activeLow;
        _gpio = gpio;
        _port = port;
        GPIO_WriteBit((GPIO_TypeDef*)port, gpio, _active_low ? Bit_SET : Bit_RESET);
    }

    GpioOutput::GpioOutput( GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow)
    {
        _init(port, gpio, activeLow);
    }

    GpioOutput::GpioOutput( GPIO_TypeDef *const port, const uint16_t gpio)
    {
        _init(port, gpio, false);
    }

    GpioOutput::GpioOutput(void)
    {
    }

    void GpioOutput::init(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow)
    {
        _init(port, gpio, activeLow);
    }

    void GpioOutput::init( GPIO_TypeDef *const port, const uint16_t gpio)
    {
        _init(port, gpio, false);
    }

    void GpioOutput::on(void)
    {
        _level = true;
        GPIO_WriteBit(_port, _gpio, _active_low ? Bit_RESET : Bit_SET);
    }

    void GpioOutput::off(void)
    {
        _level = false;
        GPIO_WriteBit(_port, _gpio, _active_low ? Bit_SET : Bit_RESET);
    }

    void GpioOutput::toggle(void)
    {
        _level = _level ? 0 : 1;
        GPIO_WriteBit(_port, _gpio, _level ? Bit_SET : Bit_RESET);
    }

    void GpioOutput::setLevel(int level)
    {
        _level = _active_low ? !level : level;
        GPIO_WriteBit(_port, _gpio, _level ? Bit_SET : Bit_RESET);
    }
}
