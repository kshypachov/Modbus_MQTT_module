#include "port.h"
#include <string.h>
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "socket.h"
#include "definition.h"

#ifndef MB_TCP_BUF_SIZE
	#define MB_TCP_BUF_SIZE  2048
#endif

uint8_t ucTCPRequestFrame [MB_TCP_BUF_SIZE]; // Регистр приема
uint16_t ucTCPRequestLen;
uint8_t ucTCPResponseFrame [MB_TCP_BUF_SIZE]; // Отправляем регистр
uint16_t ucTCPResponseLen;
uint8_t bFrameSent = FALSE; // Отправлять ли ответное решение


void modbus_tcps(USHORT sn, USHORT port)
{
	switch (getSn_SR (sn))// Получить статус сокета
	   {
	case SOCK_CLOSED:// сокет закрыт
		socket(sn,Sn_MR_TCP,port,0x00); // Открываем сокет
		break;
	case SOCK_INIT: // сокет уже инициализирован
		listen (sn); // слушаем
	case SOCK_ESTABLISHED: // сокет подключен
		if(getSn_IR(sn) & Sn_IR_CON)
		{
			setSn_IR(sn,Sn_IR_CON);
		}
		ucTCPRequestLen = getSn_RX_RSR (sn); // Получить длину полученных данных
			if(ucTCPRequestLen>0)
			{
				recv(sn,ucTCPRequestFrame, ucTCPRequestLen); // W5500 получает данные
				xMBPortEventPost (EV_FRAME_RECEIVED); // Отправляем событие EV_FRAME_RECEIVED для управления конечным автоматом в функции eMBpoll ()
				eMBPoll (); // Обрабатываем событие EV_FRAME_RECEIVED
				eMBPoll (); // Обрабатываем событие EV_EXECUTE
				if(bFrameSent)
				{
					bFrameSent = FALSE;
					send(sn,ucTCPResponseFrame,ucTCPResponseLen); // W5500 отправляет пакет данных ответа Modbus
				}
			}
		break;
	case SOCK_CLOSE_WAIT: // сокет ожидает закрытия
		disconnect (sn); // Закрываем соединение
		break;
	default:
		break;
   }
}

BOOL  xMBTCPPortInit( USHORT usTCPPort )
{
    SOCKET sn;
    sn=END_MODBUS_SOCKET;
    if(getSn_SR(sn)==SOCK_LISTEN)
    {
    	close(sn);  // Закрываем соединение
    }
    if(getSn_SR(sn)==SOCK_CLOSED)
    {
       socket(sn,Sn_MR_TCP,usTCPPort,0x00); // Открываем сокет
    }
    if (getSn_SR(sn)==SOCK_INIT)
    {
    	if (listen (sn) == SOCK_OK) return TRUE;// открываем сокет на прослушивание

    }
    return FALSE;
}

BOOL  xMBTCPPortGetRequest( UCHAR **ppucMBTCPFrame, USHORT * usTCPLength )
{
    *ppucMBTCPFrame = (uint8_t *) &ucTCPRequestFrame[0];
    *usTCPLength = ucTCPRequestLen;
    // Reset the buffer.
    ucTCPRequestLen = 0;
    return TRUE;
}


BOOL xMBTCPPortSendResponse( const UCHAR *pucMBTCPFrame, USHORT usTCPLength )
{
      memcpy(ucTCPResponseFrame,pucMBTCPFrame , usTCPLength);
      ucTCPResponseLen = usTCPLength;
      bFrameSent = TRUE; //Отправляем данные через W5500
      return bFrameSent;
}


void  vMBTCPPortClose( void )
{
};

void vMBTCPPortDisable( void )
{
};


eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs );

eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );

eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress,USHORT usNCoils,eMBRegisterMode eMode);

eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete);

