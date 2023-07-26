/*
    Encoder Pot Lib

    Created on: July 22, 2023

    Author: Coskun ERGAN
*/

#if !defined(__INCLUDE_ENCODER_POT_H__)
#define __INCLUDE_ENCODER_POT_H__

#include "RTE_Components.h"
#include  CMSIS_device_header
#include <chrono>
#include "mutex.h"
#include "timer.h"
#include "thread.h"
#include "threadflag.h"
#include <functional>
#include <atomic>

const uint16_t EncoderA_Pin       = GPIO_Pin_2;
GPIO_TypeDef *const EncoderA_Port = GPIOA;
const uint32_t EncoderA_Clk       = RCC_AHBPeriph_GPIOA;
const uint8_t EncoderA_Source     = GPIO_PinSource2;
const uint8_t EncoderA_ExtiPin    = EXTI_PinSource2;
const uint8_t EncoderA_ExtiPort   = EXTI_PortSourceGPIOA;
const uint32_t EncoderA_ExtiLine  = EXTI_Line2;
const IRQn_Type EncoderA_IRQn     = EXTI2_IRQn;

const uint16_t EncoderB_Pin       = GPIO_Pin_4;
GPIO_TypeDef *const EncoderB_Port = GPIOB;
const uint32_t EncoderB_Clk       = RCC_AHBPeriph_GPIOB;

class encoder_pot
{
public:
    void init(std::chrono::milliseconds(msec))
    {
        GPIO_InitTypeDef GPIO_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
        SYSCFG_EXTILineConfig(EncoderA_ExtiPort, EncoderA_ExtiPin);
        RCC_AHBPeriphClockCmd(EncoderA_Clk, ENABLE);
        RCC_AHBPeriphClockCmd(EncoderB_Clk, ENABLE);
        GPIO_InitStructure.GPIO_Pin = EncoderA_Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
        GPIO_Init(EncoderA_Port, &GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = EncoderB_Pin;
        GPIO_Init(EncoderB_Port, &GPIO_InitStructure);
        m_count = 0;
        m_step_time = msec;
        EXTI_ClearITPendingBit(EncoderA_ExtiLine);
        isr_enable();
        NVIC_InitStructure.NVIC_IRQChannel = EncoderA_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }

    void hook(std::function<void()> &&handler)
    {
        if(!encoder_handler)
        {
            encoder_handler = std::move(handler);
            std::thread encoder_thread([&]
            {
                cmsis::this_thread::flags th_flag;
                for(;;)
                {
                    th_flag.wait(1);
                    th_flag.clear(1);
                    if(encoder_handler)
                    {
                        encoder_handler();
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(m_step_time));
                    isr_enable();
                }
            });
            m_thread_ptr = std::make_unique<sys::thread>(std::move(encoder_thread));
        }
    }

    void isr_handler(void)
    {
        if(GPIO_ReadInputDataBit(EncoderB_Port, EncoderB_Pin))
        {
            m_count--;
        }
        else
        {
            m_count++;
        }
        isr_disable();
        m_th_flag.set(*m_thread_ptr, 1);
    }

    int get_count()
    {
        return m_count.load();
    }

    void set_count(int val)
    {
        return m_count.store(val);
    }

private:
    std::atomic_int m_count;
    std::function<void()> encoder_handler;
    std::unique_ptr<sys::thread> m_thread_ptr;
    std::chrono::milliseconds m_step_time;
    cmsis::thread_flags m_th_flag;

    void isr_enable()
    {
        EXTI_InitTypeDef EXTI_InitStructure;
        EXTI_InitStructure.EXTI_Line = EncoderA_ExtiLine;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);
    }

    void isr_disable()
    {
        EXTI_InitTypeDef EXTI_InitStructure;
        EXTI_InitStructure.EXTI_Line = EncoderA_ExtiLine;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_LineCmd = DISABLE;
        EXTI_Init(&EXTI_InitStructure);
    }

};

#endif

