#ifndef __UART_H
#define __UART_H

#include <stdio.h>
#include "main.h"

#define Debug_UARTx huart1
#define AT_UARTx huart4
#define Line_UARTx huart3
#define Recognition_UARTx huart2
#define Debug_print printf
#define UART_ToIdleDataSize QueueTaskSize
#define print(chart, age...) printf("%s µÄ %d ÐÐ: " chart "\t\t %s \r\n", __FUNCTION__, __LINE__, ##age, __FILE__)
#define  HAL_Receive_IT_OK(x)    { \
TTSQueue_ReceiveByte_(USART_Queue[x], &UART_DataBuff, 1); \
while (HAL_UART_Receive_IT(huart, (uint8_t*)&UART_DataBuff, 1) != HAL_OK); \
}

typedef enum
{
    QueueTask_Debug, // Queue1
    ATTask_Debug, //
    LineTask_Debug, //
    Recognition_Debug,
    QueueTask_Num,
} Queue_Task_Enum_TypeDef;

void Uart_SendStr(UART_HandleTypeDef *pUARTx, uint8_t *str);

void UART_Config(void);

#endif /* __UART_H */
