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

const size_t max_button_num = 3;
const std::chrono::milliseconds m_button_check_msec = std::chrono::milliseconds(100);

class button
{
public:
    void check(size_t id, std::function<bool()> &&handler)
    {
        m_button_check_handler[id] = std::move(handler);
    }

    void press(size_t id, std::function<void()> &&handler)
    {
        if(!m_thread_ptr)
        {
            std::thread button_thread([&]
            {
                cmsis::this_thread::flags th_flag;
                uint8_t count[max_button_num];
                size_t id;
                uint32_t mask;
                uint32_t m_mask;
                uint32_t temp_mask;
                for(;;)
                {
                    auto status = th_flag.wait_for(0x7FFFFFFF, std::chrono::milliseconds(m_button_check_msec), mask);
                    if(status == decltype(status)::no_timeout)
                    {
                        m_mask |= mask;
                    }
                    temp_mask = m_mask;
                    id = 0;
                    while(temp_mask)
                    {
                        while((temp_mask & 1) == 0)// find set bit
                        {
                            id++;
                            temp_mask >>= 1;
                        }
                        if(count[id] == 0)
                        {
                            count[id]++;
                            m_button_press_handler[id]();
                        }
                        else
                        {
                            if(m_button_check_handler[id]())
                            {
                                count[id]++;
                                if((count[id] * std::chrono::milliseconds(m_button_check_msec)) > std::chrono::milliseconds(m_long_press_msec[id]))
                                {
                                    if(m_button_longpress_handler[id])
                                    {
                                        m_button_longpress_handler[id]();
                                    }
                                    count[id] = 0;
                                    m_mask &= ~(1UL << id);
                                }
                            }
                            else
                            {
                                count[id] = 0;
                                m_mask &= ~(1UL << id);
                            }
                        }
                        id++;
                        temp_mask >>= 1;
                    }
                }
            });
            m_thread_ptr = std::make_unique<sys::thread>(std::move(button_thread));
        }
        m_button_press_handler[id] = std::move(handler);
    }

    void longpress(size_t id, std::chrono::milliseconds(long_press_msec), std::function<void()> &&handler)
    {
        m_long_press_msec[id] = long_press_msec;
        m_button_longpress_handler[id] = std::move(handler);
    }

    void isr_handler(size_t id)
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
