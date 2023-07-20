/*
    Main Program

    Created on: March 13, 2023

    Author: Coskun ERGAN
*/
#include "printf_io.h"
#include "OS.h"
#include "Thread.h"
#include "Timer.h"
#include "Mutex.h"
#include "Chrono.h"
#include "OSException.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
int main(int argc, char **argv)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    printf_init();

    printf("Restart..\r\n");

    cmsis::kernel::initialize();

    std::thread main_thread([]
    {
        try
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
                printf("Run: %d        \r\n", (uint32_t)(sys::chrono::high_resolution_clock::now() - tp1).count());
                my_mutex.unlock();                
            }
        }
        catch(std::exception &e)
        {            
            printf("\r\nERR: %s", e.what());
            while(1);
        }
    });

    cmsis::kernel::start();

    return 0;
}
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
