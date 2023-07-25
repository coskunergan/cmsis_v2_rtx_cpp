/*
    Main Program

    Created on: March 13, 2023

    Author: Coskun ERGAN
*/

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "printf_io.h"
#include "os.h"
#include "thread.h"
#include "timer.h"
#include "mutex.h"
#include "chrono.h"
#include "osexception.h"
#include "buzzer.h"
#include "encoder_pot.h"

using namespace cmsis;

buzzer buzz;
encoder_pot encoder;

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void pre_init()
{
    printf_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    buzz.init();
    encoder.init(std::chrono::milliseconds(50));
}
/****************************************************************************/
void app_main()
{
    try
    {
        printf("\rRestart..  ");

        encoder.hook([]
        {
            printf("\rEnc: %d    ", encoder.get_count());
            buzz.beep(std::chrono::milliseconds(50));
        });

        //throw std::system_error(0, os_category(), "TEST!!");

        std::thread main_thread([&]
        {
            sys::timer buzzerTimer(std::chrono::milliseconds(3000), []
            {
                buzz.beep(std::chrono::milliseconds(10));
                return true;
            });
            buzzerTimer.start();
            sys::chrono::high_resolution_clock::time_point tp1 = sys::chrono::high_resolution_clock::now();
            std::mutex my_mutex;
            for(;;)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                my_mutex.lock();
                printf("\r\nRun: %08X", (uint32_t)(sys::chrono::high_resolution_clock::now() - tp1).count());
                my_mutex.unlock();
            }
        });
    }
    catch(std::exception &e)
    {
        throw;
    }
}
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
