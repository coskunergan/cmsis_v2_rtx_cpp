/*
    Main Program

    Created on: March 13, 2023

    Author: Coskun ERGAN
*/

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "os.h"
#include "thread.h"

extern void pre_init();
extern void app_main();
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
int main(int argc, char **argv)
{
    pre_init();

    cmsis::kernel::initialize();

    std::thread app_main_thread([]
    {
        try
        {
            app_main();
        }
        catch(std::exception &e)
        {
            printf("\rERR:%s", e.what());
        }
    });

    cmsis::kernel::start();

    return 0;
}
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
