#include "Gimbal_Task.h"
#include "bsp_dr16.h"
#include "SYSInit.h"
#include "Gimbal_Fsm.h"


Gimbal_t Gimbal;
//TODO:初始化试验


static void Gimbal_Init(void)
{
    //函数映射
    Gimbal.RC=Return_RemoteDeal_Point();     //获取遥控器处理数据后的结构体指针
    Gimbal.Gimbal_Fsm=Return_Gimbal_FSM();
    Gimbal.Graps=Return_Grasp_t_Pointer();
#ifdef configUSE_H7
    Gimbal.fdcan1Handle = &hfdcan1;
    Gimbal.fdcan2Handle = &hfdcan2;
#endif
    //基本FSM初始化
    Gimbal_Fsm_Init();
    Gimbal_Motor_Init();

    //状态驱动映射(在ChassisMotor里)
    Gimbal.PowerOff=Gimbal_PowerOff_Drive;
    //数据初始化
    //    Gimbal.RC=Return_RemoteDeal_Point();     //获取遥控器指针
    //    Chassis.Wheel_Init(&Chassis.C);     //轮子初始化
    //    Chassis.Fsm=Return_Chassis_FSM();   //获取fsm状态机指针
    //    Chassis.Fsm_Init();                 //状态机初始化
}


/***
 *                    _ooOoo_
 *                   o8888888o
 *                   88" . "88
 *                   (| -_- |)
 *                    O\ = /O
 *                ____/`---'\____
 *              .   ' \\| |// `.
 *               / \\||| : |||// \
 *             / _||||| -:- |||||- \
 *               | | \\\ - /// | |
 *             | \_| ''\---/'' | |
 *              \ .-\__ `-` ___/-. /
 *           ___`. .' /--.--\ `. . __
 *        ."" '< `.___\_<|>_/___.' >'"".
 *       | | : `- \`.;`\ _ /`;.`/ - ` : | |
 *         \ \ `-. \_ __\ /__ _/ .-` / /
 * ======`-.____`-.___\_____/___.-`____.-'======
 *                    `=---='
 *
 * .............................................
 *          佛祖保佑             永无BUG
 */
void Gimbal_Task(void *pvParameters)
{
    Gimbal_Init();
    vTaskDelay(100);
    while(1)
    {
        FSM_Deal(Gimbal.Gimbal_Fsm,Gimbal.RC->RC_ctrl->rc.s1,Gimbal.RC->RC_ctrl->rc.s2);
        vTaskDelay(5);
    }

}

