#ifndef _TTS_SPCP_H
#define _TTS_SPCP_H
/**
 * @file tts_spcp.h
 * @author DB (ydbdby921@gmail.com)
 * @brief ����tts_queue,�ݲ�֧�ֻ���Э��
 * @version 0.4
 * @date 2023-07-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <string.h>
#include "../TTS/tts_queue.h"

typedef uint8_t uSPCPData_t;

typedef enum
{
    Sum_Check,
    ADD_Check,
    XOR_Check,
    LRC_Check,
    CRC_Check,
    Parity_Check,
    HammingCode_Check,
} uSPCPCheck_TypeDef;

typedef enum
{
    Status_Frame,
    Status_FrameX,
    Status_Length_Hex,
    Status_Length_ASCII,
    Status_Data,
    Status_Callback,
    Status_Check,
} SPCPStatus_TypeDef;

typedef struct
{
    SPCPStatus_TypeDef Status;
    uint8_t Data;
    _Bool CheckFlag;
} uSPCPot_t;

typedef struct
{
    uSPCPot_t *Pot;
    SPCPStatus_TypeDef Status;
    uint8_t Step;
    uint8_t *Buff;
    uint8_t *FrameData;
    uint8_t Length;
    uSPCPot_t Check;
} uSPCP_t;

typedef void (*TTSSPCP_Callback_t)(uint8_t ID, uSPCPData_t *Data, uint8_t Length, uint8_t *FrameData);

/**
 * ��������
 * @param ID
 * @param Data
 * @param Length
 */
void SPCPSend_data(uint8_t ID, uSPCPData_t *Data, uint8_t Length);

/**
 * ���ݽ���
 * @param UserSPCP
 * @param Queue
 */
_Bool TTS_SpcpProc(uSPCP_t *UserSPCP, Type_Queue *Queue);

/**
 * ��������
 * @param SPCPot
 * @param Status
 * @param Data
 */
uint8_t Spcp_SetData(uSPCP_t *User_SPCP, uint8_t ID, SPCPStatus_TypeDef Status, uint8_t Data, _Bool CheckFlag);

/**
 * �ص�
 * @param Data
 */
void Spcp_SetCallback(TTSSPCP_Callback_t Data);

/**
 * ������Ϣ�����С
 * @param User_Spcp
 * @param Size
 */
void Spcp_Creation(uSPCP_t *User_Spcp, uint8_t Size);

/**
 * ɾ������
 * @param User_Spcp
 */
void Spcp_Free(uSPCP_t *User_Spcp);
#define IS_Spcp_Check_INSTANCE(INSTANCE)\
  (((INSTANCE) == Sum_Check)    || \
   ((INSTANCE) == ADD_Check)    || \
   ((INSTANCE) == Sum_Check)    || \
   ((INSTANCE) == Sum_Check)    || \
   ((INSTANCE) == Sum_Check)    || \
   ((INSTANCE) == Sum_Check))

/**
 * ��̬����Э�鲢����
 * @param UserSPCP
 * @param Queue
 * @param data
 * @param Length
 */
void TTS_SpcpSend(uSPCP_t *UserSPCP, Type_Queue *Queue, uint8_t *Data, uint8_t Length,uint8_t *Frame);

#endif/*_TTS_SPCP_H*/
