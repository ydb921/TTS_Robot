#ifndef _SYSTEM_USART_H
#define _SYSTEM_USART_H
/**
 * @file tts_queue.c
 * @author DB (ydbdby921@gmail.com)
 * @brief ����ȫ�����涼�ǴӶ�ͷȡ��
 * @version 1.5
 * @date 2022-12-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <string.h>
#include "system.h"

#define QueueTaskSize 256 //�������޸Ĵ�С�������𲻱�Ҫ�Ĵ��󣬿���ѡ��������С�İ汾

typedef uint8_t Type_Queue_Data;

typedef enum
{
    Queue_Ready,
    Queue_Busy,
    Queue_Error
} QueueStatus_t;

typedef struct
{
    Type_Queue_Data *Head;
    Type_Queue_Data *Tail;
    Type_Queue_Data Buff[QueueTaskSize + 1];
    QueueStatus_t Status;//����״̬
} Type_Queue;

typedef struct
{
    Type_Queue Rx_Data;                           //�������ݽ��ն���
    Type_Queue Tx_Data;                           //�������ݷ��Ͷ���
} Queue_TypeDef;

#define TTSQueue_SendByte_(x, y) TTSQueue_SendByte(x, (Type_Queue_Data*)(y),strlen(y))
#define TTSQueue_ReceiveByte_(x, y, z) QueueDataIn((x).Rx_Data, y, z)
#define TTSQueueDataOutByte(x, y) QueueDataOut(x,y,1)
#define TTSQueueDataInByte(x, y) QueueDataIn(x,y,1)
/*basic_Function*/
#define QueueInit(x, y) TTS_QueueInit(x, y,  QueueTaskSize + 1)
#define QueueEmpty(x)        TTS_QueueEmpty((Type_Queue_Data **)&(x).Head, (Type_Queue_Data **)&(x).Tail, (Type_Queue_Data *)(x).Buff)
#define QueueDataIn(x, y, z) TTS_QueueDataIn((Type_Queue_Data **)&(x).Head, (Type_Queue_Data **)&(x).Tail, (Type_Queue_Data *)(x).Buff, sizeof((x).Buff), (y), (z))
#define QueueDataOut(x, y, z)   TTS_QueueDataOut((Type_Queue_Data **)&(x).Head, (Type_Queue_Data **)&(x).Tail, (Type_Queue_Data *)(x).Buff, sizeof((x).Buff), (y),(z))
#define QueueDataLen(x)      TTS_QueueDataLen((Type_Queue_Data **)&(x).Head, (Type_Queue_Data **)&(x).Tail, sizeof((x).Buff))

typedef void (*Queue_SendData_t)(uint8_t ID, Type_Queue_Data *Data, uint16_t Length);

/**
 * @brief ���һ������
 */
void TTS_QueueEmpty(Type_Queue_Data **Head, Type_Queue_Data **Tail, Type_Queue_Data *HBuff);

/**
 * @brief ��������
 */
void TTS_QueueDataIn(Type_Queue_Data **Head, Type_Queue_Data **Tail, Type_Queue_Data *HBuff, uint16_t Len,
                     Type_Queue_Data *HData, uint16_t DataLen);

/**
 * @brief ���ݳ���
 */
Type_Queue_Data TTS_QueueDataOut(Type_Queue_Data **Head, Type_Queue_Data **Tail,
                                 Type_Queue_Data *HBuff, uint16_t Len, Type_Queue_Data *Data, uint16_t DataLen);

/**
 * @brief �ж϶��������ݵĳ���
 */
uint16_t TTS_QueueDataLen(Type_Queue_Data **Head, Type_Queue_Data **Tail, uint16_t Len);

/**
 * @brief ���г�ʼ��
 */
void TTS_QueueInit(Queue_TypeDef *Queue_Task, uint8_t Num, __attribute__((unused)) uint16_t memory_Size);

/**
 * ��ȡ��ռ�ó���
 * @param ID ���
 * @return ���س���
 */
uint16_t TTSQueue_USize(uint8_t ID);

/**
 * ��ȡ
 * @param ID
 * @param Data
 */
void TTSQueue_ReadByte(uint8_t ID, Type_Queue_Data *Data, uint8_t Length);

/**
 * �鿴
 * @param ID
 * @param Data
 */
void TTSQueue_ViewHeadBit(uint8_t ID, Type_Queue_Data *Data);

void TTSQueue_ViewTailBit(uint8_t ID, Type_Queue_Data *Data);

/*���͹���*/
void TTSQueue_SendConfig(Queue_SendData_t User_Send);

void TTSQueue_SendByte(uint8_t ID, Type_Queue_Data *Data, uint8_t Length);

void TTSQueue_Send(uint8_t ID, Type_Queue_Data *Data, uint8_t Length);

void TTSQueue_Send_IT(uint8_t ID);

void TTSQueue_SendCallback(uint8_t ID);

#endif
