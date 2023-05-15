//
// Created by 16771 on 2023/5/13.
//
#include "bsp_dr16.h"

#define Sbus_RX_Buffer_Num 36
#define RC_FRAME_LENGTH 18
#define RC_CHANNAL_ERROR_VALUE 700  //ң�س�������
uint8_t Sbus_RX_Buffer[Sbus_RX_Buffer_Num];

// ң�������ݽṹ�����
 RC_ctrl_t rc_ctl;

void RC_Init(){
    //ʹ��DMA���ڽ���
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);//����IDLE�ж�
    __HAL_DMA_DISABLE(huart1.hdmarx);
    __HAL_DMA_ENABLE(huart1.hdmarx);
    HAL_UART_Receive_DMA(&huart1,Sbus_RX_Buffer,Sbus_RX_Buffer_Num);//����DMA����
//    HAL_UART_Receive_IT(&huart1, Sbus_RX_Buffer, Sbus_RX_Buffer_Num);

}

//void RC_Init(UART_HandleTypeDef *huart, uint8_t *Rx1_Buff, uint8_t *Rx2_Buff,
//             uint16_t Data_Buff_Lenth) {
//    // ʹ��DMA���ڽ���
//    SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);
//    // ʹ�ܿ����ж�
//    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
//    //    //����DMA���䣬������1�����ݰ��˵�recvive_buff��
//    //    HAL_UART_Receive_DMA(&huart1, sbus_rx_buf[0], 36 );
//    // ʧЧDMA
//    __HAL_DMA_DISABLE(huart->hdmarx);
//    while (huart->hdmarx->Instance & DMA_SxCR_EN) {
//        __HAL_DMA_DISABLE(huart->hdmarx);
//    }
//    huart->hdmarx->Instance->PAR = (uint32_t) & (huart->Instance->DR);
//    // �ڴ滺����1
//    huart->hdmarx->Instance->M0AR = (uint32_t)(Rx1_Buff);
//    // �ڴ滺����2
//    huart->hdmarx->Instance->M1AR = (uint32_t)(Rx2_Buff);
//    // ���ݳ���
//    huart->hdmarx->Instance->NDTR = Data_Buff_Lenth;
//    // ʹ��˫������
//    SET_BIT(huart->hdmarx->Instance->CR, DMA_SxCR_DBM);
//    // ʹ��DMA
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

    RC_CTRL->key.v = ((int16_t)pData[14]) | ((int16_t)pData[15] << 8);    //!< KeyBoard value,������������
    RC_CTRL->rc.ch[4] = ((int16_t)pData[16]) | ((int16_t)pData[17] << 8); //���Ͻǻ���

    RC_CTRL->rc.ch[0] -= RC_CH_VALUE_OFFSET;
    RC_CTRL->rc.ch[1] -= RC_CH_VALUE_OFFSET;
    RC_CTRL->rc.ch[2] -= RC_CH_VALUE_OFFSET;
    RC_CTRL->rc.ch[3] -= RC_CH_VALUE_OFFSET;
    RC_CTRL->rc.ch[4] -= RC_CH_VALUE_OFFSET;

    return 0;
}

/************************** Dongguan-University of Technology*-ACE**************************
 * @brief ң��������ֵ����
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
 * @brief ����ң����
 *
 * @param��� dma_buf_num
 ************************** Dongguan-University of Technology*-ACE***************************/
void RC_Restart(UART_HandleTypeDef *huart, uint16_t dma_buf_num) {
    __HAL_UART_DISABLE(huart);
    __HAL_DMA_DISABLE(huart->hdmarx);

//    huart->hdmarx->Instance->NDTR = dma_buf_num; // ����д�볤��

    __HAL_UART_ENABLE(huart);
    __HAL_DMA_ENABLE(huart->hdmarx);
}

/************************** Dongguan-University of Technology*-ACE**************************
 * @brief ң����������
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
 * @brief �ж�ң�����Ƿ��������go to ���ͳһ���������ݹ���
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
 * @brief ECF�汾ң������ʼ����Ĭ��C��
 *
 ************************** Dongguan-   University of Technology*-ACE***************************/
void ECF_RC_Init(void) {
    RC_Init();
}

/************************** Dongguan-University of Technology*-ACE**************************
 * @brief ECF�汾ң�����жϷ��������ǵ���it�ж��������HAL�������־λǰ�������
 *
 ************************** Dongguan-   University of Technology*-ACE***************************/
void ECF_RC_UART_Handler(void){
    RC_DataProcess(Sbus_RX_Buffer, &rc_ctl);
}

