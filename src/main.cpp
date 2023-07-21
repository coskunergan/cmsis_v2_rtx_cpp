/*
    Main Program

    Created on: March 13, 2023

    Author: Coskun ERGAN
*/

#include "printf_io.h"
#include "os.h"
#include "thread.h"
#include "osexception.h"

extern void app_main(void);
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
int main(int argc, char **argv)
{
    printf_init();

    cmsis::kernel::initialize();

    std::thread app_main_thread([]
    {
#ifdef __cpp_exceptions
        try
        {
            app_main();
        }
        catch(std::exception &e)
        {
            printf("\n\nERR:%s", e.what());
        }
#else
        app_main();
#endif
    });

    cmsis::kernel::start();

    return 0;
}
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
