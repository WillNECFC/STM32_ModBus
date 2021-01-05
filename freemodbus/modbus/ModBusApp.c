#include "port.h"
#include "mb.h"
uint8_t i;
eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode)
{
	eMBErrorCode Err = MB_ENOERR;
	i++;
	return Err;
}
eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
	i=0;
	eMBErrorCode Err = MB_ENOERR;
	i++;
	return Err;
}
eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
	i=0;
	eMBErrorCode Err = MB_ENOERR;
	i++;
	return Err;
}
eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
	i=0;
	eMBErrorCode Err = MB_ENOERR;
	i++;
	return Err;
}
