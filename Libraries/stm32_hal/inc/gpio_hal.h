/*
    GPIO Hal Library

    Created on: July 31, 2023

    Author: Coskun ERGAN
*/
#pragma once
#include "RTE_Components.h"
#include  CMSIS_device_header
#include <functional>
#include <list>

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
        GPIO_TypeDef *_port;
        void _init(const gpio_num_t pin, const bool activeLow);
        static bool _interrupt_service_installed;

        void _clearEventHandlers();

        struct interrupt_args
        {
            bool _event_handler_set = false;
            bool _custom_event_handler_set = false;
            uint16_t _gpio;
        } _interrupt_args;

    public:
        enum class gpio_int_type_t { no_int, fall, rise, rise_and_fall };
        GpioInput(const gpio_num_t pin, const bool activeLow);
        GpioInput(const gpio_num_t pin);
        GpioInput(void);
        void init(const gpio_num_t pin, const bool activeLow);
        void init(const gpio_num_t pin);
        int read(void);

        void enablePullup(void);
        void disablePullup(void);
        void enablePulldown(void);
        void disablePulldown(void);
        void enablePullupPulldown(void);
        void disablePullupPulldown(void);

        void enableInterrupt(gpio_int_type_t int_type);
        void setEventHandler(std::function<bool()> &&handler);
        static void gpio_isr_callback(void *arg);
    };

    class GpioOutput : public GpioBase
    {
    private:
        uint16_t _gpio;
        GPIO_TypeDef *_port;
        int _level = 0;
        void _init(const gpio_num_t pin, const bool activeLow);

    public:
        GpioOutput(const gpio_num_t pin, const bool activeLow);
        GpioOutput(const gpio_num_t pin);
        GpioOutput(void);
        void init(const gpio_num_t pin, const bool activeLow);
        void init(const gpio_num_t pin);
        void on(void);
        void off(void);
        void toggle(void);
        void setLevel(int level);
        gpio_num_t make_pin(GPIO_TypeDef * port, uint16_t gpio);
    };
}
