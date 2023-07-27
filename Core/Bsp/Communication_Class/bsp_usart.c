#include "bsp_usart.h"
#include "../../User/User.h"

extern UART_HandleTypeDef Debug_UARTx;

extern UART_HandleTypeDef AT_UARTx;

extern UART_HandleTypeDef Line_UARTx;

extern UART_HandleTypeDef Recognition_UARTx;

/*消息队列任务*/
Queue_TypeDef USART_Queue[QueueTask_Num];

/*多字节缓存*/
static uint8_t UART_ToIdleDataBuff[QueueTask_Num][UART_ToIdleDataSize + 1] = {0};

/*单字节缓存*/
static uint8_t UART_DataBuff;

static inline void UARTx_Send_Data(uint8_t ID, Type_Queue_Data *Data, uint16_t Length)
{
    HAL_StatusTypeDef UART_Status = HAL_BUSY;
    while (1) {
        switch (ID) {
            case ATTask_Debug:UART_Status = HAL_UART_Transmit_IT(&AT_UARTx, (uint8_t *) Data, Length);
                break;
            case Recognition_Debug:UART_Status = HAL_UART_Transmit_IT(&Recognition_UARTx, (uint8_t *) Data, Length);
                break;
            case LineTask_Debug: UART_Status = HAL_UART_Transmit_IT(&Line_UARTx, (uint8_t *) Data, Length);
                break;
            case QueueTask_Debug:UART_Status = HAL_UART_Transmit_DMA(&Debug_UARTx, (uint8_t *) Data, Length);
                break;
            default:Error_Handler();
                break;
        }
        if (UART_Status == HAL_OK)
            break;
    }
}

void UART_Config(void)
{
    /*消息队列初始化*/
    QueueInit(USART_Queue, QueueTask_Num);
    TTSQueue_SendConfig(UARTx_Send_Data);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    //	if (HAL_UART_Receive_IT(&Protocol_UARTx, (uint8_t*)&UART_DataBuff, 1) != HAL_OK)
//		Error_Handler();
    if (HAL_UARTEx_ReceiveToIdle_IT(&Line_UARTx, UART_ToIdleDataBuff[LineTask_Debug], UART_ToIdleDataSize) != HAL_OK)
        Error_Handler();
    if (HAL_UARTEx_ReceiveToIdle_IT(&Recognition_UARTx, UART_ToIdleDataBuff[Recognition_Debug], UART_ToIdleDataSize)
        != HAL_OK)
        Error_Handler();
    if (HAL_UARTEx_ReceiveToIdle_IT(&AT_UARTx, UART_ToIdleDataBuff[ATTask_Debug], UART_ToIdleDataSize) != HAL_OK)
        Error_Handler();
    if (HAL_UARTEx_ReceiveToIdle_DMA(&Debug_UARTx, UART_ToIdleDataBuff[QueueTask_Debug], UART_ToIdleDataSize) != HAL_OK)
        Error_Handler();
    printf("\r\n\tSerial Start\n");
    TTSQueue_SendByte_(ATTask_Debug, "{#hello world}$");
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &Debug_UARTx) {
//        HAL_UART_Transmit(&Debug_UARTx, UART_ToIdleDataBuff[QueueTask_Debug], Size, 1000);
//        TTSQueue_SendByte(QueueTask_Debug, UART_ToIdleDataBuff[QueueTask_Debug], Size);
        QueueDataIn(USART_Queue[QueueTask_Debug].Rx_Data, UART_ToIdleDataBuff[QueueTask_Debug], Size);
        if (HAL_UARTEx_ReceiveToIdle_DMA(&Debug_UARTx, UART_ToIdleDataBuff[QueueTask_Debug], UART_ToIdleDataSize)
            != HAL_OK)
            Error_Handler();
    }
    else if (huart == &Line_UARTx) {
//        QueueDataIn(USART_Queue[LineTask_Debug].Rx_Data, UART_ToIdleDataBuff[LineTask_Debug], Size);
        if (UART_ToIdleDataBuff[LineTask_Debug][0] == 0x7e &&
            UART_ToIdleDataBuff[LineTask_Debug][4] == 0xfe && Size >= 4) {
            Spcp_Callback(LineTask_Debug, &UART_ToIdleDataBuff[LineTask_Debug][1], 2,
                          &UART_ToIdleDataBuff[LineTask_Debug][3]);
        }
        if (HAL_UARTEx_ReceiveToIdle_IT(&Line_UARTx, UART_ToIdleDataBuff[LineTask_Debug], UART_ToIdleDataSize)
            != HAL_OK)
            Error_Handler();
    }
    else if (huart == &AT_UARTx) {
//        QueueDataIn(USART_Queue[ATTask_Debug].Rx_Data, UART_ToIdleDataBuff[ATTask_Debug], Size);
        Spcp_Callback(ATTask_Debug, UART_ToIdleDataBuff[ATTask_Debug], Size, UART_ToIdleDataBuff[ATTask_Debug]);
        if (HAL_UARTEx_ReceiveToIdle_IT(&AT_UARTx, UART_ToIdleDataBuff[ATTask_Debug], UART_ToIdleDataSize) != HAL_OK)
            Error_Handler();
    }
    else if (huart == &Recognition_UARTx) {
        QueueDataIn(USART_Queue[Recognition_Debug].Rx_Data, UART_ToIdleDataBuff[Recognition_Debug], Size);
        if (HAL_UARTEx_ReceiveToIdle_IT(&Recognition_UARTx, UART_ToIdleDataBuff[Recognition_Debug], UART_ToIdleDataSize)
            != HAL_OK)
            Error_Handler();
    }

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	if (huart == &Protocol_UARTx)
//	HAL_Receive_IT_OK(QueueTask_Protocol)
//	else
//    if (huart == &Line_UARTx) {
//        HAL_Receive_IT_OK(LineTask_Debug)
//    }
//    else if (huart == &Debug_UARTx) {
//        HAL_Receive_IT_OK(QueueTask_Debug)
//    }
//	huart->RxState = HAL_UART_STATE_READY;
    __HAL_UNLOCK(huart);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &Debug_UARTx)
        TTSQueue_SendCallback(QueueTask_Debug);
    else if (huart == &Line_UARTx)
        TTSQueue_SendCallback(LineTask_Debug);
    else if (huart == &AT_UARTx)
        TTSQueue_SendCallback(ATTask_Debug);
    else if (huart == &Recognition_UARTx)
        TTSQueue_SendCallback(Recognition_Debug);
}

int fgetc(FILE *f)//可使用scanf、getchar等函数
{
    int ch;
    HAL_UART_Receive(&Debug_UARTx, (uint8_t *) &ch, 1, 1000);
    return (ch);
}


#if 1
int _write(int file, char *ptr, int len)
{
//    HAL_UART_Transmit_DMA(&Debug_UARTx, (uint8_t *) ptr, len);
    TTSQueue_Send(QueueTask_Debug, (Type_Queue_Data *) ptr, len);
    return len;
}
#else
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
//    TTSQueue_SendByte(QueueTask_Debug, (Type_Queue_Data*)&ch, 1);
//	HAL_UART_Transmit(&Debug_UARTx, (uint8_t*)&ch, 1, 1000);
//	HAL_UART_Transmit_DMA(&Debug_UARTx, (uint8_t*)&ch, 1);
    return (ch);
}

#endif
