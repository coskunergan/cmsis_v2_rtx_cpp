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

using namespace cmsis;

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void app_main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    printf("Restart..    \n");

    //throw std::system_error(0, os_category(), "TEST!!");

    std::thread main_thread([]
    {
        sys::timer ledTimer(std::chrono::milliseconds(200), []
        {
            GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
            return true;
        });
        ledTimer.start();
        sys::chrono::high_resolution_clock::time_point tp1 = sys::chrono::high_resolution_clock::now();
        std::mutex my_mutex;
        for(;;)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(900));
            my_mutex.lock();
            printf("\nRun: %08X  \n", (uint32_t)(sys::chrono::high_resolution_clock::now() - tp1).count());
            my_mutex.unlock();
        }
    });
}
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
