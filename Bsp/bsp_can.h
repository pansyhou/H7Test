//
// Created by 16771 on 2023/5/14.
//

#ifndef H7TEST_BSP_CAN_H
#define H7TEST_BSP_CAN_H
#include "main.h"
#include "fdcan.h"

//CAN�ҷ�����ID
typedef enum {
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_M3508_MOTOR1_ID = 0x201,    //̧�����
    CAN_M3508_MOTOR2_ID = 0x202,    //pitch
    CAN_M3508_MOTOR3_ID = 0x203,    //roll
    CAN_M3508_MOTOR4_ID = 0x204,
    CAN_M3508_MOTOR5_ID = 0x205,
    CAN_M3508_MOTOR6_ID = 0x206,
    CAN_M3508_MOTOR7_ID = 0x207,
    CAN_M3508_MOTOR8_ID = 0x208,
    CAN_GM6020_MOTOR1_ID = 0x205,
    CAN_GM6020_MOTOR2_ID = 0x206,
    CAN_GM6020_MOTOR3_ID = 0x207,
    CAN_GM6020_MOTOR4_ID = 0x208,
    CAN_GM6020_MOTOR5_ID = 0x209,
    CAN_GM6020_MOTOR6_ID = 0x20A,
    CAN_GM6020_MOTOR7_ID = 0x20B
} CAN_msg_e;

//CAN������ն�id
typedef enum {
    C620_OR_C610_1_TO_4_ID = 0x200,
    C620_OR_C610_5_TO_8_ID = 0x1FF,
    GM6020_1_TO_4_ID = 0x1FF,
    GM6020_5_TO_7_ID = 0x2FF,
} ESC_Receive_Stdid;

extern FDCAN_HandleTypeDef hfdcan1;

typedef void (*can_stdmsg_rx_callback_t)(FDCAN_HandleTypeDef *header);//���ݴ���ָ��

typedef struct can_manage_obj
{
    FDCAN_HandleTypeDef *hfdcan;//CAN���
//    fifo_t tx_fifo;    //FIFO�ṹ��
//    uint8_t *tx_fifo_buffer;//FIFO������
//    uint8_t is_sending;     //CAN�Ƿ��ڷ���
    can_stdmsg_rx_callback_t can_rec_callback;//��Ӧ��CAN���ջص�����ָ��
}can_manage_obj;

extern struct can_manage_obj can1_manage;
extern struct can_manage_obj can2_manage;

extern int32_t ECF_CAN_Rx_Callback_Register(can_manage_obj *m_obj, can_stdmsg_rx_callback_t fun);

extern void ECF_CAN_Init(void);

extern uint32_t ECF_CAN_Send_Msg_FIFO(FDCAN_HandleTypeDef *hfdcan, uint16_t std_id, uint8_t *data, uint16_t len);

///**************����ʽ���ͺ���**************/
//extern void ECF_CAN1_Send_Msg_Block(int16_t stdid, uint8_t *data, uint16_t len);
//extern void ECF_CAN2_Send_Msg_Block(int16_t stdid, uint8_t *data, uint16_t len);
//
///**************��ͨ���ܻᶪ���ķ��ͺ���**************/
//extern void ECF_CAN1_Send_Msg_NO_Block(int16_t stdid, uint8_t *data, uint16_t len);
//extern void ECF_CAN2_Send_Msg_NO_Block(int16_t stdid, uint8_t *data, uint16_t len);

//�߼���װ���ͺ�����ֻΪ���͵���� ��ϸע�Ϳ��������
extern void CAN1_C620_OR_C610_201_TO_204_SendMsg(int16_t ESC_201, int16_t ESC_202, int16_t ESC_203, int16_t ESC_204);

extern void CAN1_C620_OR_C610_205_TO_208_SendMsg(int16_t ESC_201, int16_t ESC_202, int16_t ESC_203, int16_t ESC_204);

extern void CAN1_GM6020_1_TO_4_SendMsg(int16_t ESC_201, int16_t ESC_202, int16_t ESC_203, int16_t ESC_204);

extern void CAN1_GM6020_5_TO_8_SendMsg(int16_t ESC_201, int16_t ESC_202, int16_t ESC_203, int16_t ESC_204);



#endif //H7TEST_BSP_CAN_H
