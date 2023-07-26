/*
    Button Lib

    Created on: July 26, 2023

    Author: Coskun ERGAN
*/

#if !defined(__INCLUDE_BUTTON_H__)
#define __INCLUDE_BUTTON_H__

#include <chrono>
#include "mutex.h"
#include "timer.h"
#include "eventflag.h"
#include <functional>

class button
{
public:
    void init(std::chrono::milliseconds(long_press_msec))
    {
        m_long_press_msec = long_press_msec;
        m_button_check_msec = std::chrono::milliseconds(200);
        count = 0;
    }

    void check(std::function<bool()> &&handler)
    {
        m_button_check_handler = std::move(handler);
    }

    void press(std::function<void()> &&handler)
    {
        if(!m_button_press_handler)
        {
            std::thread button_thread([&]
            {
                cmsis::event evt_flag(1);
                event_flag = std::make_unique<cmsis::event>(std::move(evt_flag));
                for(;;)
                {
                    event_flag->clear(1);
                    event_flag->wait(1);
                    if(m_button_press_handler)
                    {
                        m_button_press_handler();
                    }
                    do
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(m_button_check_msec));
                        count++;
                        if((count * std::chrono::milliseconds(m_button_check_msec)) > std::chrono::milliseconds(m_long_press_msec))
                        {
                            if(m_button_longpress_handler)
                            {
                                m_button_longpress_handler();
                            }
                            break;
                        }
                    }
                    while(m_button_check_handler());
                    count = 0;
                }
            });
            m_thread_ptr = std::make_unique<sys::thread>(std::move(button_thread));
        }
        m_button_press_handler = std::move(handler);
    }

    void longpress(std::function<void()> &&handler)
    {
        m_button_longpress_handler = std::move(handler);
    }

    void isr_handler()
    {
        event_flag->set(1);
    }

private:
    int count;
    std::unique_ptr<sys::timer> m_tim_ptr;
    std::chrono::milliseconds m_long_press_msec;
    std::chrono::milliseconds m_button_check_msec;
    std::function<bool()> m_button_check_handler;
    std::function<void()> m_button_press_handler;
    std::function<void()> m_button_longpress_handler;
    std::unique_ptr<sys::thread> m_thread_ptr;
    std::unique_ptr<cmsis::event> event_flag;

};

#endif
