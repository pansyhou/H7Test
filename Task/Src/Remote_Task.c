/***
*                    .::::.
*                  .::::::::.
*                 :::::::::::  FUCK YOU
*             ..:::::::::::'
*           '::::::::::::'
*             .::::::::::
*        '::::::::::::::..
*             ..::::::::::::.
*           ``::::::::::::::::
*            ::::``:::::::::'        .:::.
*           ::::'   ':::::'       .::::::::.
*         .::::'      ::::     .:::::::'::::.
*        .:::'       :::::  .:::::::::' ':::::.
*       .::'        :::::.:::::::::'      ':::::.
*      .::'         ::::::::::::::'         ``::::.
*  ...:::           ::::::::::::'              ``::.
* ```` ':.          ':::::::::'                  ::::..
*                    '.:::::'                    ':'````..
*/

#include "FreeRTOS.h"
#include "task.h"
#include "bsp_can.h"
#include "BoardCommuni.h"



void Remote_Task(void *pvParameters)
{
    vTaskDelay(20);
    while(1)
    {
        Remote_Data_Deal();
        CAN_A2B_RC_Send();
        vTaskDelay(5);
    }
}
