//
// Created by 16771 on 2023/5/13.
//

#ifndef ENGINEERING_A_2023_H7_BSP_DR16_H
#define ENGINEERING_A_2023_H7_BSP_DR16_H

#include "main.h"
#include <struct_typedef.h>
#include "stm32h7xx_hal_def.h"
/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;


/* 遥控文档内容-BEGIN */
/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN            ((uint16_t)364 )			//通道最小值
#define RC_CH_VALUE_OFFSET         ((uint16_t)1024)			//通道中间值
#define RC_CH_VALUE_MAX            ((uint16_t)1684)			//通道最大值
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_ERROR                ((uint16_t)0) //出现严重错误
#define RC_SW_UP                   ((uint16_t)1)
#define RC_SW_MID                  ((uint16_t)3)
#define RC_SW_DOWN                 ((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W       ((uint16_t)1 << 0)
#define KEY_PRESSED_OFFSET_S       ((uint16_t)1 << 1)
#define KEY_PRESSED_OFFSET_A       ((uint16_t)1 << 2)
#define KEY_PRESSED_OFFSET_D       ((uint16_t)1 << 3)
#define KEY_PRESSED_OFFSET_SHIFT   ((uint16_t)1 << 4)
#define KEY_PRESSED_OFFSET_CTRL    ((uint16_t)1 << 5)
#define KEY_PRESSED_OFFSET_Q       ((uint16_t)1 << 6)
#define KEY_PRESSED_OFFSET_E       ((uint16_t)1 << 7)
#define KEY_PRESSED_OFFSET_R       ((uint16_t)1 << 8)
#define KEY_PRESSED_OFFSET_F       ((uint16_t)1 << 9)
#define KEY_PRESSED_OFFSET_G       ((uint16_t)1 << 10)
#define KEY_PRESSED_OFFSET_Z       ((uint16_t)1 << 11)
#define KEY_PRESSED_OFFSET_X       ((uint16_t)1 << 12)
#define KEY_PRESSED_OFFSET_C       ((uint16_t)1 << 13)
#define KEY_PRESSED_OFFSET_V       ((uint16_t)1 << 14)
#define KEY_PRESSED_OFFSET_B       ((uint16_t)1 << 15)
/* ----------------------- Data Struct ------------------------------------- */


/*
* 遥控器接收数据结构体*/
//__packed为字节对齐
typedef __packed struct{
    struct{
        int16_t ch[5];
        uint8_t s1;
        uint8_t s2;
    } rc;
    struct{
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;
    /* keyboard key information */
    struct{
        int32_t kv0;
        int32_t kv1;
        int32_t kqe;
        int32_t kzx;
        int32_t kcv;
        int32_t kcs;
//        int32_t kv6;
        uint16_t v;
    } key;
    int8_t Flag;
} RC_ctrl_t;


//
void    RC_Init(void);
void    RC_UART_Handler(void);
int RC_DataProcess(volatile const uint8_t *pData, RC_ctrl_t *RC_CTRL);
RC_ctrl_t *RC_Get_RC_Pointer(void);
bool_t    RC_Check_Data_IS_ERROR(void);
void    RC_Restart(UART_HandleTypeDef *huart,uint16_t dma_buf_num);
void    RC_DataReload(void);

void    ECF_RC_Init(void);
void    ECF_RC_UART_Handler(void);



#endif //ENGINEERING_A_2023_H7_BSP_DR16_H
