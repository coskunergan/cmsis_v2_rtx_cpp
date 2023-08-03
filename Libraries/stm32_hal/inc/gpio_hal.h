/*
    GPIO Hal Library

    Created on: July 31, 2023

    Author: Coskun ERGAN
*/
#pragma once
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "mutex.h"
#include <functional>
#include <forward_list>
#include <memory>

namespace gpio_hal
{    
    class GpioBase
    {
    public:
        typedef uint32_t gpio_num_t;
    protected:
        bool _active_low = false;        
    };

    class GpioInput : public GpioBase
    {
    private:
        static std::forward_list<std::pair<uint16_t, std::function<void()>>>  forwardListOfPairs;                
        static std::unique_ptr<std::mutex> m_mutex;
        GPIO_TypeDef * _port;
        uint16_t _gpio;
        uint8_t _port_source;
        void _init(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow);

    public:
        enum class gpio_int_type_t { no_int, fall, rise, rise_and_fall };
        GpioInput(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow);
        GpioInput(GPIO_TypeDef *const port, const uint16_t gpio);
        GpioInput(void);
        void init(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow);
        void init(GPIO_TypeDef *const port, const uint16_t gpio);
        int read(void);

        void enablePullup(void);
        void disablePullup(void);
        void enablePulldown(void);
        void disablePulldown(void);
        void enablePullupPulldown(void);
        void disablePullupPulldown(void);

        void enableInterrupt(gpio_int_type_t int_type);
        void setISRHandler(std::function<void()> &&handler);
        void clearISRHandlers();
        static void gpio_isr_callback(const uint16_t gpio);
    };

    class GpioOutput : public GpioBase
    {
    private:
        uint16_t _gpio;
        GPIO_TypeDef *_port;
        bool _level = false;
        void _init(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow);

    public:
        GpioOutput(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow);
        GpioOutput(GPIO_TypeDef *const port, const uint16_t gpio);
        GpioOutput(void);
        void init(GPIO_TypeDef *const port, const uint16_t gpio, const bool activeLow);
        void init(GPIO_TypeDef *const port, const uint16_t gpio);
        void on(void);
        void off(void);
        void toggle(void);
        void setLevel(int level);

    };

 
}
