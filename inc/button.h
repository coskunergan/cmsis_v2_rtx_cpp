/*
    Button Lib

    Created on: July 26, 2023

    Author: Coskun ERGAN
*/

#if !defined(__INCLUDE_BUTTON_H__)
#define __INCLUDE_BUTTON_H__

#include "RTE_Components.h"
#include  CMSIS_device_header
#include <chrono>
#include "mutex.h"
#include "timer.h"
#include "threadflag.h"
#include <functional>

const int max_button_num = 3;
const std::chrono::milliseconds m_button_check_msec = std::chrono::milliseconds(100);

class button
{
public:
    void check(int id, std::function<bool()> &&handler)
    {
        m_button_check_handler[id] = std::move(handler);
    }

    void press(int id, std::function<void()> &&handler)
    {
        if(!m_thread_ptr)
        {
            std::thread button_thread([&]
            {
                cmsis::this_thread::flags th_flag;
                uint8_t count[max_button_num];
                int flag;
                uint32_t mask;
                uint32_t m_mask;
                uint32_t temp_mask;
                th_flag.clear(0x7FFFFFFF);
                for(;;)
                {
                    th_flag.wait_for(0x7FFFFFFF, std::chrono::milliseconds(m_button_check_msec), mask);
                    flag = 0;
                    th_flag.clear(0x7FFFFFFF);
                    if(mask && mask != -2)
                    {
                        m_mask |= mask;
                    }
                    temp_mask = m_mask;
                    while(temp_mask)
                    {
                        do
                        {
                            if(temp_mask & 0x1)
                            {
                                temp_mask >>= 1;
                                break;
                            }                            
                            temp_mask >>= 1;
                            flag++;
                        }
                        while(1);

                        if(count[flag] == 0)
                        {
                            count[flag]++;
                            m_button_press_handler[flag]();
                        }
                        else
                        {
                            if(m_button_check_handler[flag]())
                            {
                                count[flag]++;
                                if((count[flag] * std::chrono::milliseconds(m_button_check_msec)) > std::chrono::milliseconds(m_long_press_msec[flag]))
                                {
                                    if(m_button_longpress_handler[flag])
                                    {
                                        m_button_longpress_handler[flag]();
                                    }
                                    count[flag]=0;
                                    m_mask &= ~(1UL << flag);                                
                                }
                            }
                            else
                            {
                                count[flag]=0;
                                m_mask &= ~(1UL << flag);
                            }
                        }
                        flag++;
                    }
                }
            });
            m_thread_ptr = std::make_unique<sys::thread>(std::move(button_thread));
        }
        m_button_press_handler[id] = std::move(handler);
    }

    void longpress(int id, std::chrono::milliseconds(long_press_msec), std::function<void()> &&handler)
    {
        m_long_press_msec[id] = long_press_msec;
        m_button_longpress_handler[id] = std::move(handler);
    }

    void isr_handler(int id)
    {
        m_th_flag.set(*m_thread_ptr, (1 << id));
    }

private:
    std::unique_ptr<sys::timer> m_tim_ptr;
    std::chrono::milliseconds m_long_press_msec[max_button_num];
    std::function<bool()> m_button_check_handler[max_button_num];
    std::function<void()> m_button_press_handler[max_button_num];
    std::function<void()> m_button_longpress_handler[max_button_num];
    std::unique_ptr<sys::thread> m_thread_ptr;
    cmsis::thread_flags m_th_flag;

};

#endif
