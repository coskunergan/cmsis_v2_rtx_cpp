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
#include "button.h"
#include "buzzer.h"
#include "encoder_pot.h"

using namespace cmsis;

button butt;
buzzer buzz;
encoder_pot encoder;

#define Button_Pin   		GPIO_Pin_0
#define Button_Port			GPIOA
#define Button_Clk		    RCC_AHBPeriph_GPIOA
#define Button_Source  		GPIO_PinSource0
#define Button_ExtiPin  	EXTI_PinSource0
#define Button_ExtiPort 	EXTI_PortSourceGPIOA
#define Button_ExtiLine     EXTI_Line0
#define Button_IRQn         EXTI0_IRQn

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void pre_init()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    printf_init();
    //------------------------------
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    SYSCFG_EXTILineConfig(Button_ExtiPort, Button_ExtiPin);
    RCC_AHBPeriphClockCmd(Button_Clk, ENABLE);
    GPIO_InitStructure.GPIO_Pin = Button_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(Button_Port, &GPIO_InitStructure);
    EXTI_ClearITPendingBit(Button_ExtiLine);

    EXTI_InitStructure.EXTI_Line = Button_ExtiLine;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = Button_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    butt.init(std::chrono::seconds(3));
    butt.check([]
    {
        return !GPIO_ReadInputDataBit(Button_Port, Button_Pin);
    });
    //------------------------------
    buzz.init();
    //------------------------------
    encoder.init(std::chrono::milliseconds(50));
}
/****************************************************************************/
void app_main()
{
    try
    {
        printf("\rRestart..  ");

        buzz.beep(std::chrono::milliseconds(50));

        butt.press([]
        {
            printf("\rButton Pressed..");
            buzz.beep(std::chrono::milliseconds(50));
        });
        
        butt.longpress([]
        {
            printf("\rButton LongPressed..");
            buzz.beep(std::chrono::milliseconds(200));
        });

        encoder.hook([]
        {
            printf("\rEnc: %d    ", encoder.get_count());
            buzz.beep(std::chrono::milliseconds(50));
        });

        //throw std::system_error(0, os_category(), "TEST!!");

        std::thread main_thread([]
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
