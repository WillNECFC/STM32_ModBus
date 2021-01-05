#include "port.h"

extern osThreadId defaultTaskHandle;
extern osThreadId ModBusHandle;
extern osMessageQId ModBusQuHandle;
extern osTimerId ModBusTimerHandle;
/*---Event Stste---*/
typedef struct
{
	eMBEventType Event;
	bool Valid;
} xMB_Event_Info_Tag;
xMB_Event_Info_Tag xMB_Event;
extern BOOL xMBPortEventInit(void)
{
	xMB_Event.Valid = false;
	xMB_Event.Event = EV_READY;
	return true;
}

BOOL xMBPortEventPost(eMBEventType eEvent)
{
	xMB_Event.Valid = true;
	xMB_Event.Event = eEvent;
	return true;
}

BOOL xMBPortEventGet(eMBEventType *eEvent)
{
	if (xMB_Event.Valid == true)
	{
		*eEvent = xMB_Event.Event;
		xMB_Event.Valid = false;
		return TRUE;
	}
	else
	{
		return false;
	}
}
/**/
uint8_t GetData;
typedef struct
{
	uint8_t ReadData;
	bool ReadSts;
} ModBusUart_Tag;
ModBusUart_Tag ModBusData;
BOOL xMBPortSerialInit(UCHAR ucPort, ULONG ulBaudRate,
					   UCHAR ucDataBits, eMBParity eParity)
{
	MX_USART1_UART_Init();
	HAL_UART_Receive_IT(&huart1, &(ModBusData.ReadData), 1);
	ModBusData.ReadSts = false;
	return true;
}

void vMBPortClose(void)
{
}

void xMBPortSerialClose(void)
{
}
typedef struct
{
	bool TxEn;
	bool RxEn
} MB_TxRx_Tag;
MB_TxRx_Tag MB_TxRxSts;
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
	if (xRxEnable)
	{
		MB_TxRxSts.RxEn = true;
		HAL_UART_Receive_IT(&huart1, &(ModBusData.ReadData), 1);
	}
	else
	{
		MB_TxRxSts.RxEn = false;
	}
	if (xTxEnable)
	{
		MB_TxRxSts.TxEn = true;
		pxMBFrameCBTransmitterEmpty();
	}
	else
	{
		MB_TxRxSts.TxEn = false;
	}
}
uint8_t UARTBuf[100];
uint8_t GetIndes = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if ((huart->Instance == USART1))
	{
		ModBusData.ReadSts = true;
	/*	UARTBuf[GetIndes] = ModBusData.ReadData;
		GetIndes++;
		if(GetIndes>=100)
		{
			GetIndes=0;
		}*/
		if (MB_TxRxSts.RxEn)
		{
			pxMBFrameCBByteReceived();
		}
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if ((huart->Instance == USART1))
	{
		pxMBFrameCBTransmitterEmpty(); /*发送完成*/
	}
}

BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
	if (ModBusData.ReadSts == true)
	{
		*pucByte = ModBusData.ReadData;
		ModBusData.ReadSts = false;
		HAL_UART_Receive_IT(&huart1, &ModBusData.ReadData, 1);
		return true;
	}
	else
	{
		return false;
	}
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{	
	static uint8_t SendTempData; /**这里最好用全局或static 类型,出现过地址出错的情况**/
	SendTempData=ucByte;
	UARTBuf[GetIndes] =ucByte;
		GetIndes++;
		if(GetIndes>=100)
		{
			GetIndes=0;
		}
	HAL_UART_Transmit_IT(&huart1, &SendTempData, 1);
	//pxMBFrameCBTransmitterEmpty(); /*发送完成*/

	return true;
}

/* ----------------------- Timers functions ---------------------------------*/
uint8_t ModBusDelayCnt = 0;
BOOL xMBPortTimersInit(USHORT usTimeOut50us)
{
	ModBusDelayCnt = (usTimeOut50us + 1) / 18; /*延时2ms*/
	return true;
}

void xMBPortTimersClose(void)
{
}
void ModBusTimeOut(void const *argument)
{
	static bool InitSts = false;
	/* USER CODE BEGIN ModBusTimeOut */
	if (InitSts == true)
	{
		pxMBPortCBTimerExpired();
	}
	InitSts = true;
	/* USER CODE END ModBusTimeOut */
}
void vMBPortTimersEnable(void)
{
	BaseType_t Temp;
	xTimerChangePeriodFromISR(ModBusTimerHandle, ModBusDelayCnt, &Temp);
	if (Temp == pdTRUE)
	{
		taskYIELD();
	}
}

void vMBPortTimersDisable(void)
{
	BaseType_t Temp;
	xTimerStopFromISR(ModBusTimerHandle, &Temp);
	if (Temp == pdTRUE)
	{
		taskYIELD();
	}
}

void vMBPortTimersDelay(USHORT usTimeOutMS)
{
	osDelay(usTimeOutMS);
}
void Dummy(bool x)
{
	uint32_t Cnt;
	while (!x)
	{
		Cnt++;
	}
}
