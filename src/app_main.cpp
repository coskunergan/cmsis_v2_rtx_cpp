/*
    Main Program

    Created on: March 13, 2023

    Author: Coskun ERGAN
*/

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "os.h"
#include "thread.h"
#include "timer.h"
#include "mutex.h"
#include "chrono.h"
#include "osexception.h"
#include "buzzer.h"
#include "encoder_pot.h"

using namespace cmsis;



/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void app_main(void)
{
    printf("Restart..    \n");

    buzzer buzzer;  

    encoder_pot encoder(std::chrono::milliseconds(100));
    
    //throw std::system_error(0, os_category(), "TEST!!");

    std::thread main_thread([&]
    {
        sys::timer buzzerTimer(std::chrono::milliseconds(3000), [&]
        {
            buzzer.beep(std::chrono::milliseconds(1));
            return true;
        });
        buzzerTimer.start();
        sys::chrono::high_resolution_clock::time_point tp1 = sys::chrono::high_resolution_clock::now();
        std::mutex my_mutex;
        for(;;)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(900));
            my_mutex.lock();
            //printf("\nRun: %08X  \n", (uint32_t)(sys::chrono::high_resolution_clock::now() - tp1).count());
            //printf("\nRun: %d  \n", encoder.get_count());
            my_mutex.unlock();
        }
    });
}
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
