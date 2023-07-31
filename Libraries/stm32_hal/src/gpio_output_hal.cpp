/*
    GPIO Hal Library

    Created on: July 31, 2023

    Author: Coskun ERGAN
*/

#include "gpio_hal.h"

namespace gpio_hal
{
    void GpioOutput::_init(const gpio_num_t pin, const bool activeLow)
    {
        uint16_t gpio = pin;
        GPIO_TypeDef *port;

        switch(pin >> 16)
        {
            case 0:
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
                port = GPIOA;
                break;
            case 1:
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
                port = GPIOB;
                break;
            case 2:
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
                port = GPIOC;
                break;
            case 3:
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
                port = GPIOD;
                break;
            case 4:
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
                port = GPIOE;
                break;
            case 5:
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
                port = GPIOF;
                break;
            case 6:
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOG, ENABLE);
                port = GPIOG;
                break;
            case 7:
                RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE);
                port = GPIOH;
                break;
        }
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
        GPIO_Init(port, &GPIO_InitStructure);
        _active_low = activeLow;
        _gpio = gpio;
        _port = port;
        GPIO_WriteBit(_port, gpio, _active_low ? Bit_SET : Bit_RESET);
    }

    GpioOutput::GpioOutput(const gpio_num_t pin, const bool activeLow)
    {
        _init(pin, activeLow);
    }

    GpioOutput::GpioOutput(const gpio_num_t pin)
    {
        _init(pin, false);
    }

    GpioOutput::GpioOutput(void)
    {
    }

    void GpioOutput::init(const gpio_num_t pin, const bool activeLow)
    {
        _init(pin, activeLow);
    }

    void GpioOutput::init(const gpio_num_t pin)
    {
        _init(pin, false);
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

    GpioOutput::gpio_num_t GpioOutput::make_pin(GPIO_TypeDef *port, uint16_t gpio)
    {
        if(port == GPIOB)
        {
            return (gpio) | 0x10000;
        }
        else if(port == GPIOC)
        {
            return (gpio) | 0x20000;
        }
        else if(port == GPIOD)
        {
            return (gpio) | 0x30000;
        }
        else if(port == GPIOE)
        {
            return (gpio) | 0x40000;
        }
        else if(port == GPIOF)
        {
            return (gpio) | 0x50000;
        }
        else if(port == GPIOG)
        {
            return (gpio) | 0x60000;
        }
        else if(port == GPIOH)
        {
            return (gpio) | 0x70000;
        }
        return (gpio);
    }
}
