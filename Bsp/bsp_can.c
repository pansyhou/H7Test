//
// Created by 16771 on 2023/5/14.
//
#include "bsp_can.h"
#include "bsp_Motor_Encoder.h"

can_manage_obj can1_manage;
can_manage_obj can2_manage;
void ECF_CAN_Init(void) {
    can1_manage.hfdcan = &hfdcan1;
    can1_manage.can_rec_callback = NULL;


    //filter init
    FDCAN_FilterTypeDef sFilterConfig1;

    sFilterConfig1.IdType = FDCAN_STANDARD_ID;
    sFilterConfig1.FilterIndex = 0;
    sFilterConfig1.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig1.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig1.FilterID1 = 0;
    sFilterConfig1.FilterID2 = 0;
    HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig1);

    sFilterConfig1.IdType = FDCAN_EXTENDED_ID;
    sFilterConfig1.FilterIndex = 0;
    sFilterConfig1.FilterType = FDCAN_FILTER_MASK;
    sFilterConfig1.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    sFilterConfig1.FilterID1 = 0;
    sFilterConfig1.FilterID2 = 0;
    HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig1);

    //配置全局过滤器以拒绝所有不匹配的帧
    //reject frames which unmatched  , such as remote frames
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);
    //开启Rx FIFO 0 的新消息通知
    //Activate FX fifo 0 notification
    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    //开启溢满覆盖模式，当这个模式开启时，如果fifo溢满则会覆盖旧数据
    //configure the overwrite mode ,when fifo is full ,  overwrite old data
    HAL_FDCAN_ConfigRxFifoOverwrite(&hfdcan1, FDCAN_RX_FIFO0, FDCAN_RX_FIFO_OVERWRITE);

    //FDCAN rx start
    HAL_FDCAN_Start(&hfdcan1);

}


int32_t ECF_CAN_Rx_Callback_Register(can_manage_obj *m_obj, can_stdmsg_rx_callback_t fun){
    m_obj->can_rec_callback = fun;
}


void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    /*     CAN_RxHeaderTypeDef	Rxmessage;
     */
    //    if(hfdcan == &hfdcan1)
    //				CAN1_chassis_receive(&hfdcan1);
    //		if(hfdcan == &hfdcan2)
    //				CAN2_chassis_receive(&hfdcan1);
    //			if(CAN1_receive_callback != NULL)
    //					CAN1_receive_callback(hcan)	;
    if (hfdcan == &hfdcan1)
    {
        if(can1_manage.can_rec_callback!=NULL){
            can1_manage.can_rec_callback(&hfdcan1);
        }
    }
//    if (hfdcan == &hfdcan2){
//        if(can2_manage.can_rec_callback!=NULL){
//            can2_manage.can_rec_callback(&hfdcan2);
//    }
}



static FDCAN_TxHeaderTypeDef Txmessage;				//发送的消息


uint32_t ECF_CAN_Send_Msg_FIFO(FDCAN_HandleTypeDef *hfdcan, uint16_t std_id, uint8_t *data, uint16_t len){

    Txmessage.Identifier = std_id;                   //发送ID
    Txmessage.IdType     = FDCAN_STANDARD_ID;		//标识符类型stdid
    Txmessage.TxFrameType = FDCAN_DATA_FRAME;		//传输data的帧类型
    Txmessage.DataLength  = FDCAN_DLC_BYTES_8;		//长度0-64
    Txmessage.ErrorStateIndicator = FDCAN_ESI_ACTIVE;//指定错误状态指示器（发送节点错误活跃
    Txmessage.BitRateSwitch = FDCAN_BRS_OFF;   		//指定发送的T恤帧是带位率转换还是不带
    Txmessage.FDFormat = FDCAN_CLASSIC_CAN;			//指定发送帧是classic 还是fd
    Txmessage.TxEventFifoControl = FDCAN_NO_TX_EVENTS;//指定帧开始时捕获的时间戳计数器值传播（不存储tx事件
    Txmessage.MessageMarker = 0; 					//指定复制到Tx EventFIFO元素中的消息标记用于识别Tx信息状态


    HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &Txmessage, data);
}


void CAN1_C620_OR_C610_201_TO_204_SendMsg(int16_t ESC_201, int16_t ESC_202, int16_t ESC_203, int16_t ESC_204){
    uint8_t Can_Send_Data1[8];
    Can_Send_Data1[0] = ESC_201 >> 8;
    Can_Send_Data1[1] = ESC_201;
    Can_Send_Data1[2] = ESC_202 >> 8;
    Can_Send_Data1[3] = ESC_202;
    Can_Send_Data1[4] = ESC_203 >> 8;
    Can_Send_Data1[5] = ESC_203;
    Can_Send_Data1[6] = ESC_204 >> 8;
    Can_Send_Data1[7] = ESC_204;

    ECF_CAN_Send_Msg_FIFO(&hfdcan1, C620_OR_C610_1_TO_4_ID, Can_Send_Data1, 8);
}

void CAN1_C620_OR_C610_205_TO_208_SendMsg(int16_t ESC_201, int16_t ESC_202, int16_t ESC_203, int16_t ESC_204) {
    uint8_t Can_Send_Data2[8];
    Can_Send_Data2[0] = ESC_201 >> 8;
    Can_Send_Data2[1] = ESC_201;
    Can_Send_Data2[2] = ESC_202 >> 8;
    Can_Send_Data2[3] = ESC_202;
    Can_Send_Data2[4] = ESC_203 >> 8;
    Can_Send_Data2[5] = ESC_203;
    Can_Send_Data2[6] = ESC_204 >> 8;
    Can_Send_Data2[7] = ESC_204;

    ECF_CAN_Send_Msg_FIFO(&hfdcan1, C620_OR_C610_5_TO_8_ID, Can_Send_Data2, 8);

}

/************************** Dongguan-University of Technology -ACE**************************
 * @brief 通过CAN1实现GM6020的四个电机控制,1号电机到4号,注意！，1到四号的电机和C6020电调里的4-8中的ID冲突
 * 所以一般将电机调到4-8号里发
 *
 * @param ESC_201
 * @param ESC_202
 * @param ESC_203
 * @param ESC_204
************************** Dongguan-University of Technology -ACE***************************/
void CAN1_GM6020_1_TO_4_SendMsg(int16_t ESC_201, int16_t ESC_202, int16_t ESC_203, int16_t ESC_204) {
    static uint8_t Can_Send_Data3[8];
    Can_Send_Data3[0] = ESC_201 >> 8;
    Can_Send_Data3[1] = ESC_201;
    Can_Send_Data3[2] = ESC_202 >> 8;
    Can_Send_Data3[3] = ESC_202;
    Can_Send_Data3[4] = ESC_203 >> 8;
    Can_Send_Data3[5] = ESC_203;
    Can_Send_Data3[6] = ESC_204 >> 8;
    Can_Send_Data3[7] = ESC_204;

    ECF_CAN_Send_Msg_FIFO(&hfdcan1, GM6020_1_TO_4_ID, Can_Send_Data3, 8);

}

//5号到8号电机
void CAN1_GM6020_5_TO_8_SendMsg(int16_t ESC_201, int16_t ESC_202, int16_t ESC_203, int16_t ESC_204) {
    static uint8_t Can_Send_Data4[8];
    Can_Send_Data4[0] = ESC_201 >> 8;
    Can_Send_Data4[1] = ESC_201;
    Can_Send_Data4[2] = ESC_202 >> 8;
    Can_Send_Data4[3] = ESC_202;
    Can_Send_Data4[4] = ESC_203 >> 8;
    Can_Send_Data4[5] = ESC_203;
    Can_Send_Data4[6] = ESC_204 >> 8;
    Can_Send_Data4[7] = ESC_204;


    ECF_CAN_Send_Msg_FIFO(&hfdcan1, GM6020_5_TO_7_ID, Can_Send_Data4, 8);

}