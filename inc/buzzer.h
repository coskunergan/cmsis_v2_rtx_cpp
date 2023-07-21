/*
    Buzzer Lib

    Created on: March 13, 2023

    Author: Coskun ERGAN
*/

#if !defined(__INCLUDE_BUZZER_H__)
#define __INCLUDE_BUZZER_H__

#include <chrono>
#include "mutex.h"
#include "timer.h"

#define Buzzer_Port 	GPIOA
#define Buzzer_Pin 	    GPIO_Pin_11
#define Buzzer_Clk      RCC_AHBPeriph_GPIOA

class buzzer 
{
public:
    buzzer()
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
        GPIO_InitStructure.GPIO_Pin = Buzzer_Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
        GPIO_Init(Buzzer_Port, &GPIO_InitStructure);
        GPIO_SetBits(Buzzer_Port, Buzzer_Pin);// off
    }

    ~buzzer()
    {
        GPIO_InitStructure.GPIO_Pin = Buzzer_Pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
        GPIO_Init(Buzzer_Port, &GPIO_InitStructure);
    }    

    void beep(std::chrono::milliseconds(msec))
    {
        m_buzz_mutex.lock();
        sys::timer buzz_timer(std::chrono::milliseconds(msec), [&]
        {
            return beep_off();
        });
        m_buzz_timer = std::move(buzz_timer);
        beep_on();
        m_buzz_timer.start();
    }

private:
    GPIO_InitTypeDef GPIO_InitStructure;
    sys::timer m_buzz_timer;
    std::mutex m_buzz_mutex;

    void beep_on()
    {
        GPIO_ResetBits(Buzzer_Port, Buzzer_Pin);
    }

    bool beep_off()
    {
        GPIO_SetBits(Buzzer_Port, Buzzer_Pin);
        m_buzz_mutex.unlock();
        return false;
    }
};

#endif
