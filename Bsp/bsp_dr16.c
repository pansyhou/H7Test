//
// Created by 16771 on 2023/5/13.
//
#include "bsp_dr16.h"

#define Sbus_RX_Buffer_Num 36
#define RC_FRAME_LENGTH 18
#define RC_CHANNAL_ERROR_VALUE 700  //遥控出错上限
uint8_t Sbus_RX_Buffer[Sbus_RX_Buffer_Num];

// 遥控器数据结构体变量
 RC_ctrl_t rc_ctl;

void RC_Init(){
    //使能DMA串口接收
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);//开启IDLE中断
    __HAL_DMA_DISABLE(huart1.hdmarx);
    __HAL_DMA_ENABLE(huart1.hdmarx);
    HAL_UART_Receive_DMA(&huart1,Sbus_RX_Buffer,Sbus_RX_Buffer_Num);//开启DMA接收
//    HAL_UART_Receive_IT(&huart1, Sbus_RX_Buffer, Sbus_RX_Buffer_Num);

}

//void RC_Init(UART_HandleTypeDef *huart, uint8_t *Rx1_Buff, uint8_t *Rx2_Buff,
//             uint16_t Data_Buff_Lenth) {
//    // 使能DMA串口接收
//    SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);
//    // 使能空闲中断
//    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
//    //    //设置DMA传输，将串口1的数据搬运到recvive_buff中
//    //    HAL_UART_Receive_DMA(&huart1, sbus_rx_buf[0], 36 );
//    // 失效DMA
//    __HAL_DMA_DISABLE(huart->hdmarx);
//    while (huart->hdmarx->Instance & DMA_SxCR_EN) {
//        __HAL_DMA_DISABLE(huart->hdmarx);
//    }
//    huart->hdmarx->Instance->PAR = (uint32_t) & (huart->Instance->DR);
//    // 内存缓冲区1
//    huart->hdmarx->Instance->M0AR = (uint32_t)(Rx1_Buff);
//    // 内存缓冲区2
//    huart->hdmarx->Instance->M1AR = (uint32_t)(Rx2_Buff);
//    // 数据长度
//    huart->hdmarx->Instance->NDTR = Data_Buff_Lenth;
//    // 使能双缓冲区
//    SET_BIT(huart->hdmarx->Instance->CR, DMA_SxCR_DBM);
//    // 使能DMA
//    __HAL_DMA_ENABLE(huart->hdmarx);
//}


void  RC_UART_Handler(){
    RC_DataProcess(Sbus_RX_Buffer, &rc_ctl);
}

int RC_DataProcess(volatile const uint8_t *pData, RC_ctrl_t *RC_CTRL) {

    RC_CTRL->rc.ch[0] = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;        //!< Channel 0
    RC_CTRL->rc.ch[1] = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF; //!< Channel 1
    RC_CTRL->rc.ch[2] = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |          //!< Channel 2
                         ((int16_t)pData[4] << 10)) & 0x07FF;

    RC_CTRL->rc.ch[3] = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5] << 7)) & 0x07FF; //!< Channel 3

    RC_CTRL->rc.s1 = ((pData[5] >> 4) & 0x0003);      //!< Switch left
    RC_CTRL->rc.s2 = ((pData[5] >> 4) & 0x000C) >> 2; //!< Switch right

    RC_CTRL->mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);   //!< Mouse X axis
    RC_CTRL->mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);   //!< Mouse Y axis
    RC_CTRL->mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8); //!< Mouse Z axis

    RC_CTRL->mouse.press_l = pData[12]; //!< Mouse Left Is Press ?
    RC_CTRL->mouse.press_r = pData[13]; //!< Mouse Right Is Press ?

    RC_CTRL->key.v = ((int16_t)pData[14]) | ((int16_t)pData[15] << 8);    //!< KeyBoard value,就是其他按键
    RC_CTRL->rc.ch[4] = ((int16_t)pData[16]) | ((int16_t)pData[17] << 8); //左上角滑轮

    RC_CTRL->rc.ch[0] -= RC_CH_VALUE_OFFSET;
    RC_CTRL->rc.ch[1] -= RC_CH_VALUE_OFFSET;
    RC_CTRL->rc.ch[2] -= RC_CH_VALUE_OFFSET;
    RC_CTRL->rc.ch[3] -= RC_CH_VALUE_OFFSET;
    RC_CTRL->rc.ch[4] -= RC_CH_VALUE_OFFSET;

    return 0;
}

/************************** Dongguan-University of Technology*-ACE**************************
 * @brief 遥控器绝对值处理
 *
 * @param num
 * @return int16_t
 ************************** Dongguan-University of Technology*-ACE***************************/
static int16_t RC_abs(int16_t num) {
    if (num < 0)
        return -num;
    else
        return num;
}


/************************** Dongguan-University of Technology*-ACE**************************
 * @brief 重启遥控器
 *
 * @param入参 dma_buf_num
 ************************** Dongguan-University of Technology*-ACE***************************/
void RC_Restart(UART_HandleTypeDef *huart, uint16_t dma_buf_num) {
    __HAL_UART_DISABLE(huart);
    __HAL_DMA_DISABLE(huart->hdmarx);

//    huart->hdmarx->Instance->NDTR = dma_buf_num; // 重新写入长度

    __HAL_UART_ENABLE(huart);
    __HAL_DMA_ENABLE(huart->hdmarx);
}

/************************** Dongguan-University of Technology*-ACE**************************
 * @brief 遥控数据清零
 *
 ************************** Dongguan-University of Technology*-ACE***************************/
void RC_DataReload(void) {
    rc_ctl.rc.ch[0] = 0;
    rc_ctl.rc.ch[1] = 0;
    rc_ctl.rc.ch[2] = 0;
    rc_ctl.rc.ch[3] = 0;
    rc_ctl.rc.ch[4] = 0;
    rc_ctl.mouse.x = 0;
    rc_ctl.mouse.y = 0;
    rc_ctl.mouse.z = 0;
    rc_ctl.mouse.press_l = 0;
    rc_ctl.mouse.press_r = 0;
    rc_ctl.key.v = 0;
}

/************************** Dongguan-University of Technology*-ACE**************************
 * @brief 判断遥控器是否出错，采用go to 语句统一将错误数据归零
 *
 * @return uint8_t
************************** Dongguan-University of Technology*-ACE***************************/
bool_t RC_Check_Data_IS_ERROR(void) {
    if (RC_abs(rc_ctl.rc.ch[0]) > RC_CHANNAL_ERROR_VALUE) {
        goto error;
    }
    if (RC_abs(rc_ctl.rc.ch[1]) > RC_CHANNAL_ERROR_VALUE) {
        goto error;
    }
    if (RC_abs(rc_ctl.rc.ch[2]) > RC_CHANNAL_ERROR_VALUE) {
        goto error;
    }
    if (RC_abs(rc_ctl.rc.ch[4]) > RC_CHANNAL_ERROR_VALUE) {
        goto error;
    }
    if (rc_ctl.rc.s1 == 0) {
        goto error;
    }
    if (rc_ctl.rc.s2 == 0) {
        goto error;
    }

    return 0;
    error:
    rc_ctl.rc.ch[0] = 0;
    rc_ctl.rc.ch[1] = 0;
    rc_ctl.rc.ch[2] = 0;
    rc_ctl.rc.ch[3] = 0;
    rc_ctl.rc.ch[4] = 0;

    rc_ctl.rc.s1 = RC_SW_DOWN;
    rc_ctl.rc.s2 = RC_SW_DOWN;
    rc_ctl.mouse.x = 0;
    rc_ctl.mouse.y = 0;
    rc_ctl.mouse.z = 0;
    rc_ctl.mouse.press_l = 0;
    rc_ctl.mouse.press_r = 0;
    rc_ctl.key.v = 0;
    return 1;
}

/************************** Dongguan-University of Technology*-ACE**************************
 * @brief ECF版本遥控器初始化，默认C板
 *
 ************************** Dongguan-   University of Technology*-ACE***************************/
void ECF_RC_Init(void) {
    RC_Init();
}

/************************** Dongguan-University of Technology*-ACE**************************
 * @brief ECF版本遥控器中断服务函数，记得扔it中断里（建议在HAL库清楚标志位前放这个）
 *
 ************************** Dongguan-   University of Technology*-ACE***************************/
void ECF_RC_UART_Handler(void){
    RC_DataProcess(Sbus_RX_Buffer, &rc_ctl);
}

