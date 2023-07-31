/*
    GPIO Hal Library

    Created on: July 31, 2023

    Author: Coskun ERGAN
*/

#include "gpio_hal.h"

namespace gpio_hal
{
    bool GpioInput::_interrupt_service_installed{false};

    void GpioInput::gpio_isr_callback(void *args)
    {
        int32_t pin = (reinterpret_cast<struct interrupt_args *>(args))->_gpio;
        bool event_handler_set = (reinterpret_cast<struct interrupt_args *>(args))->_event_handler_set;

        // if (queue_enabled)
        // {
        //     xQueueSendFromISR(queue_handle, &pin, NULL);
        // }
        // else if (custom_event_handler_set)
        // {
        //     esp_event_isr_post_to(custom_event_loop_handle, INPUT_EVENTS, pin, nullptr, 0, nullptr);
        // }
        // else if (event_handler_set)
        // {
        //     esp_event_isr_post(INPUT_EVENTS, pin, nullptr, 0, nullptr);
        // }
    }

    void GpioInput::_init(const gpio_num_t pin, const bool activeLow)
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
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(port, &GPIO_InitStructure);
        _active_low = activeLow;
        _interrupt_args._gpio = gpio;
        _port = port;
    }

    GpioInput::GpioInput(const gpio_num_t pin, const bool activeLow)
    {
        _init(pin, activeLow);
    }

    GpioInput::GpioInput(const gpio_num_t pin)
    {
        _init(pin, false);
    }

    GpioInput::GpioInput(void)
    {
    }

    void GpioInput::init(const gpio_num_t pin, const bool activeLow)
    {
        _init(pin, activeLow);
    }

    void GpioInput::init(const gpio_num_t pin)
    {
        _init(pin, false);
    }

    int GpioInput::read(void)
    {
        return _active_low ? !GPIO_ReadInputDataBit(_port, _interrupt_args._gpio) : GPIO_ReadInputDataBit(_port, _interrupt_args._gpio);
    }

    void GpioInput::enablePullup(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _interrupt_args._gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::disablePullup(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _interrupt_args._gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::enablePulldown(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _interrupt_args._gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::disablePulldown(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _interrupt_args._gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::enablePullupPulldown(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _interrupt_args._gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::disablePullupPulldown(void)
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = _interrupt_args._gpio;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(_port, &GPIO_InitStructure);
    }

    void GpioInput::enableInterrupt(gpio_int_type_t int_type)
    {
        // esp_err_t status{ESP_OK};

        // // Invert triggers if active low is enabled
        // if (_active_low)
        // {
        //     switch (int_type)
        //     {
        //     case GPIO_INTR_POSEDGE:
        //         int_type = GPIO_INTR_NEGEDGE;
        //         break;
        //     case GPIO_INTR_NEGEDGE:
        //         int_type = GPIO_INTR_POSEDGE;
        //         break;
        //     case GPIO_INTR_LOW_LEVEL:
        //         int_type = GPIO_INTR_HIGH_LEVEL;
        //         break;
        //     case GPIO_INTR_HIGH_LEVEL:
        //         int_type = GPIO_INTR_LOW_LEVEL;
        //         break;
        //     default:
        //         break;
        //     }
        // }

        // if (!_interrupt_service_installed)
        // {
        //     status = gpio_install_isr_service(0);
        //     if (ESP_OK == status)
        //     {
        //         _interrupt_service_installed = true;
        //     }
        // }

        // if (ESP_OK == status)
        // {
        //     status = gpio_set_intr_type(_interrupt_args._pin, int_type);
        // }

        // if (ESP_OK == status)
        // {
        //     status = gpio_isr_handler_add(_interrupt_args._pin, gpio_isr_callback, (void*) &_interrupt_args);
        // }
        //return status;
    }

    void GpioInput::setEventHandler(std::function<bool()> &&handler)
    {
        // esp_err_t status{ESP_OK};

        // taskENTER_CRITICAL(&_eventChangeMutex);

        // status = _clearEventHandlers();

        // status = esp_event_handler_instance_register(INPUT_EVENTS, _interrupt_args._pin, Gpio_e_h, 0, nullptr);

        // if (ESP_OK == status)
        // {
        //     _interrupt_args._event_handler_set = true;
        // }

        // taskEXIT_CRITICAL(&_eventChangeMutex);

        //return status;
    }

    void GpioInput::_clearEventHandlers()
    {
        // esp_err_t status {ESP_OK};

        // if(_interrupt_args._custom_event_handler_set)
        // {
        //     esp_event_handler_unregister_with(_interrupt_args._custom_event_loop_handle, INPUT_EVENTS, _interrupt_args._pin, _event_handle);
        //     _interrupt_args._custom_event_handler_set = false;
        // }
        // else if (_interrupt_args._event_handler_set)
        // {
        //     esp_event_handler_instance_unregister(INPUT_EVENTS, _interrupt_args._pin, nullptr);
        //     _interrupt_args._event_handler_set = false;
        // }

        // _interrupt_args._queue_handle = nullptr;
        // _interrupt_args._queue_enabled = false;

        //return status;
    }
}