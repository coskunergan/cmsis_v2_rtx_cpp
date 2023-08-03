/*
    GPIO Hal Library

    Created on: July 31, 2023

    Author: Coskun ERGAN
*/

#include "gpio_hal.h"

namespace gpio_hal
{
    std::forward_list<std::pair<uint16_t, std::function<void()>>> GpioInput::forwardListOfPairs;
    std::unique_ptr<std::mutex> GpioInput::m_mutex;

    void GpioInput::gpio_isr_callback(const uint16_t gpio)
    {
        for(auto const &currentPair : forwardListOfPairs)
        {
            if(currentPair.first == gpio)
            {
                currentPair.second();
            }
        }
    }

    void GpioInput::_init(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow)
    {
        if(port == GPIOA)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
            _port_source = EXTI_PortSourceGPIOA;
        }
        else if(port == GPIOB)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
            _port_source = EXTI_PortSourceGPIOB;
        }
        else if(port == GPIOC)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
            _port_source = EXTI_PortSourceGPIOC;
        }
        else if(port == GPIOD)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
            _port_source = EXTI_PortSourceGPIOD;
        }
        else if(port == GPIOE)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
            _port_source = EXTI_PortSourceGPIOE;
        }
        else if(port == GPIOF)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
            _port_source = EXTI_PortSourceGPIOF;
        }
        else if(port == GPIOG)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOG, ENABLE);
            _port_source = EXTI_PortSourceGPIOG;
        }
        else if(port == GPIOH)
        {
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, ENABLE);
            _port_source = EXTI_PortSourceGPIOH;
        }
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init((GPIO_TypeDef * const)port, &GPIO_InitStructure);
        _active_low = activeLow;
        _gpio = gpio;
        _port = port;
    }

    GpioInput::GpioInput(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow)
    {
        _init(port, gpio, activeLow);
    }

    GpioInput::GpioInput(GPIO_TypeDef *const port, const uint16_t gpio)
    {
        _init(port, gpio, false);
    }

    GpioInput::GpioInput(void)
    {
    }

    void GpioInput::init(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow)
    {
        _init(port, gpio, activeLow);
    }

    void GpioInput::init(GPIO_TypeDef *const port, const uint16_t gpio)
    {
        _init(port, gpio, false);
    }

    int GpioInput::read(void)
    {
        return _active_low ? !GPIO_ReadInputDataBit(_port, _gpio) : GPIO_ReadInputDataBit(_port, _gpio);
    }

    void GpioInput::enablePullup(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::disablePullup(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::enablePulldown(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::disablePulldown(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::enablePullupPulldown(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::disablePullupPulldown(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::enableInterrupt(gpio_int_type_t int_type)
    {
        IRQn_Type exti_irqn;
        uint32_t exti_line = _gpio;
        uint8_t exti_source;
        uint8_t pin_source;

        switch(_gpio)
        {
            case GPIO_Pin_0:
                pin_source = exti_source = EXTI_PinSource0;
                exti_irqn = EXTI0_IRQn;
                break;
            case GPIO_Pin_1:
                pin_source = exti_source = EXTI_PinSource1;
                exti_irqn = EXTI1_IRQn;
                break;
            case GPIO_Pin_2:
                pin_source = exti_source = EXTI_PinSource2;
                exti_irqn = EXTI2_IRQn;
                break;
            case GPIO_Pin_3:
                pin_source = exti_source = EXTI_PinSource3;
                exti_irqn = EXTI3_IRQn;
                break;
            case GPIO_Pin_4:
                pin_source = exti_source = EXTI_PinSource4;
                exti_irqn = EXTI4_IRQn;
                break;
            case GPIO_Pin_5:
                pin_source = exti_source = EXTI_PinSource5;
                exti_irqn = EXTI9_5_IRQn;
                break;
            case GPIO_Pin_6:
                pin_source = exti_source = EXTI_PinSource6;
                exti_irqn = EXTI9_5_IRQn;
                break;
            case GPIO_Pin_7:
                pin_source = exti_source = EXTI_PinSource7;
                exti_irqn = EXTI9_5_IRQn;
                break;
            case GPIO_Pin_8:
                pin_source = exti_source = EXTI_PinSource8;
                exti_irqn = EXTI9_5_IRQn;
                break;
            case GPIO_Pin_9:
                pin_source = exti_source = EXTI_PinSource9;
                exti_irqn = EXTI9_5_IRQn;
                break;
            case GPIO_Pin_10:
                pin_source = exti_source = EXTI_PinSource10;
                exti_irqn = EXTI15_10_IRQn;
                break;
            case GPIO_Pin_11:
                pin_source = exti_source = EXTI_PinSource11;
                exti_irqn = EXTI15_10_IRQn;
                break;
            case GPIO_Pin_12:
                pin_source = exti_source = EXTI_PinSource12;
                exti_irqn = EXTI15_10_IRQn;
                break;
            case GPIO_Pin_13:
                pin_source = exti_source = EXTI_PinSource13;
                exti_irqn = EXTI15_10_IRQn;
                break;
            case GPIO_Pin_14:
                pin_source = exti_source = EXTI_PinSource14;
                exti_irqn = EXTI15_10_IRQn;
                break;
            case GPIO_Pin_15:
                pin_source = exti_source = EXTI_PinSource15;
                exti_irqn = EXTI15_10_IRQn;
                break;
            default:
                break;
        }

        SYSCFG_EXTILineConfig(_port_source, exti_source);
        EXTI_InitTypeDef EXTI_InitStructure;
        EXTI_InitStructure.EXTI_Line = static_cast<uint32_t>(_gpio);
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        switch(int_type)
        {
            case gpio_int_type_t::no_int:
                EXTI_InitStructure.EXTI_LineCmd = DISABLE;
                break;
            case gpio_int_type_t::fall:
                EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
                break;
            case gpio_int_type_t::rise:
                EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
                break;
            case gpio_int_type_t::rise_and_fall:
                EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
                break;
        }
        EXTI_Init(&EXTI_InitStructure);

        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel = exti_irqn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }

    void GpioInput::setISRHandler(std::function<void()> &&handler)
    {
        if(!GpioInput::m_mutex)
        {
            GpioInput::m_mutex = std::unique_ptr<std::mutex>(new std::mutex());
        }
        clearISRHandlers();
        std::lock_guard<std::mutex> lg(*GpioInput::m_mutex);
        std::pair<uint16_t, std::function<void()>> pair = make_pair(_gpio, std::move(handler));
        forwardListOfPairs.push_front(pair);        
    }

    void GpioInput::clearISRHandlers()
    {       
        if(!GpioInput::m_mutex)
        {
            GpioInput::m_mutex = std::unique_ptr<std::mutex>(new std::mutex());
        }         
        std::lock_guard<std::mutex> lg(*GpioInput::m_mutex);
        forwardListOfPairs.remove_if([&](const std::pair<uint16_t, std::function<void()>> &pair)
        {
            return (pair.first == _gpio) ? true : false ;
        });        
    }

    extern "C" void EXTI0_IRQHandler(void)
    {
        GpioInput::gpio_isr_callback(GPIO_Pin_0);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
    extern "C" void EXTI1_IRQHandler(void)
    {
        GpioInput::gpio_isr_callback(GPIO_Pin_1);
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
    extern "C" void EXTI2_IRQHandler(void)
    {
        GpioInput::gpio_isr_callback(GPIO_Pin_2);
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
    extern "C" void EXTI3_IRQHandler(void)
    {
        GpioInput::gpio_isr_callback(GPIO_Pin_3);
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
    extern "C" void EXTI4_IRQHandler(void)
    {
        GpioInput::gpio_isr_callback(GPIO_Pin_4);
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    extern "C" void EXTI9_5_IRQHandler(void)
    {
        if(EXTI_GetFlagStatus(EXTI_Line5))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_5);
            EXTI_ClearITPendingBit(EXTI_Line5);
        }
        if(EXTI_GetFlagStatus(EXTI_Line6))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_6);
            EXTI_ClearITPendingBit(EXTI_Line6);
        }
        if(EXTI_GetFlagStatus(EXTI_Line7))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_7);
            EXTI_ClearITPendingBit(EXTI_Line7);
        }
        if(EXTI_GetFlagStatus(EXTI_Line8))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_8);
            EXTI_ClearITPendingBit(EXTI_Line8);
        }
        if(EXTI_GetFlagStatus(EXTI_Line9))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_9);
            EXTI_ClearITPendingBit(EXTI_Line9);
        }
    }
    extern "C" void EXTI10_15_IRQHandler(void)
    {
        if(EXTI_GetFlagStatus(EXTI_Line10))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_10);
            EXTI_ClearITPendingBit(EXTI_Line10);
        }
        if(EXTI_GetFlagStatus(EXTI_Line11))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_11);
            EXTI_ClearITPendingBit(EXTI_Line11);
        }
        if(EXTI_GetFlagStatus(EXTI_Line12))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_12);
            EXTI_ClearITPendingBit(EXTI_Line12);
        }
        if(EXTI_GetFlagStatus(EXTI_Line13))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_13);
            EXTI_ClearITPendingBit(EXTI_Line13);
        }
        if(EXTI_GetFlagStatus(EXTI_Line14))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_14);
            EXTI_ClearITPendingBit(EXTI_Line14);
        }
        if(EXTI_GetFlagStatus(EXTI_Line15))
        {
            GpioInput::gpio_isr_callback(GPIO_Pin_15);
            EXTI_ClearITPendingBit(EXTI_Line15);
        }
    }
}