/*
 * @Author: your name
 * @Date: 2021-01-09 15:32:34
 * @LastEditTime: 2021-01-12 14:35:44
 * @LastEditors: Please set LastEditors
 * @Description: 定义PC键盘,遥控结构体,键盘结构体
 */
#ifndef __REMOTEDEAL_H_
#define __REMOTEDEAL_H_
#include "bsp_dr16.h"
#include "filter.h"
#include <Detect_Task.h>
#include "BoardConfig.h"
#if defined (configUSE_C_Board ) || defined (configUSE_F4)
#include "stm32f4xx_hal_def.h"
#endif



#if defined (configUSE_H7 )
#include "stm32h7xx_hal_def.h"
#endif
/* ----------------------- PC Key Definition-------------------------------- */

typedef enum {
    Follow_Independent = 0,
    Arm_Independent = 1,
    Global_Status_End
} Global_Status_e;

typedef enum {
    Reserve_Close = 0,
    Reserve_Open = 1,
    Reserve_Status_End
} Reserve_Status_e;

typedef enum {
    Layout_Shift = 0,
	Layout_Ctrl = 1,
	Layout_Normal = 2,
	Layout_End
} Key_layout_e;



typedef enum {
    Arm_Control_Classic = 0,
    Arm_Control_Triangle,
    Arm_Control_Coordinate,
    Arm_Control_End
} Arm_Control_e;

/****************键盘控制状态结构体**************************/
typedef  struct
{
//	unsigned char Electromagnet : 1; //电磁铁
//	unsigned char Magazine : 1;		 //弹仓
//	unsigned char Clip : 1;			 //A板
//	unsigned char Auto_Clamp : 1;	 //自动夹取
//	unsigned char Up_motor : 1;		 //抬升电机
//	unsigned char Flex_motor : 1;	 //伸缩电机
//	unsigned char Flip_motor : 1;	 //翻转电机
//	unsigned char Clamp_motor : 1;	 //夹子电机
//
//	/* -----------------------------以下为新结构体------------------------ */
//
//	/* ----------------------------底盘状态 -------------------------------*/
//	unsigned char Rotation : 2;	   //大陀螺
//	unsigned char Wiggle : 2;	   //扭腰
//	unsigned char Independent : 2; //独立行走
//
//	/* ----------------------------上装状态------------------------------- */
//	unsigned char Grasp_Up : 2;	  //自动夹取
//	unsigned char Translation  : 1; //上装复位
//	unsigned char Telescoping : 1;	  //松夹
//	unsigned char Clap : 1;	  //兑换一个
//	unsigned char Flip : 1;	  //兑换两个
//
//	/*-----------------------------底盘功能状态----------------------------*/
//	unsigned char RFID : 1;	   //救援卡
//	unsigned char Barrier : 1; //救援爪
//	unsigned char Gimbal_Yaw :1;//云台yaw轴

    //分为整车状态和自动状态
    // global status
    unsigned char Global_Status : 1 ; //0时为底盘独立模式，1为取矿独立模式

    unsigned char Reserve_Status: 1;  //储矿0为关,1为开

    unsigned char Arm_Control_Method: 2;  //储矿0为关,1为开

	unsigned char Key_layout: 2;	//键盘层

} KeyBoard_State_t;

/*Remote结构体*/
typedef  struct
{
    RC_ctrl_t *RC_ctrl;
    uint16_t last_key;
	first_order_filter_type_t RC_X;
    first_order_filter_type_t RC_Y;
    first_order_filter_type_t RC_Z;
    first_order_filter_type_t KM_X;
    first_order_filter_type_t KM_Y;
    first_order_filter_type_t KM_Z;

    first_order_filter_type_t KQE;
    first_order_filter_type_t KZX;
    first_order_filter_type_t KCV;
    first_order_filter_type_t KCS;
	KeyBoard_State_t state;
} REMOTE_t;

/*Remote结构体初始化*/
void Remote_Data_Init(void);

/*Remote数据处理*/
void Remote_Data_Deal(void);

/*返回Remote数据指针*/
REMOTE_t *Return_RemoteDeal_Point(void);

extern REMOTE_t REMOTE;

#endif
