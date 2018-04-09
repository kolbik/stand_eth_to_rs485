/*
 * UDPtoRs485.c
 *
 *  Created on: 7 апр. 2018 г.
 *      Author: dimich
 */

#include <UDPtoRs485.h>
#include <string.h>
#include <UDP.h>

extern void PrintLog(BYTE *mas, WORD len);

static BYTE masFromUdp[100];

SM_UDP_TO_RS485 smUdpToRs485State = SM_UDP_TO_RS485_GET_SOCKET;

#define UDP_TO_RS485_PORT	50000
#define UDP_FROM_RS485_PORT	50200

static UDP_SOCKET UdpToRs485Socket = INVALID_UDP_SOCKET;
static UDP_SOCKET Rs485ToUdpSocket = INVALID_UDP_SOCKET;

static NODE_INFO	Remote;


/*********************************************************************
 * Function:        void UdpToRs485Task(void)
 *
 * PreCondition:    FuncNull() is already called
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
void UdpToRs485Task(void)
{

    switch(smUdpToRs485State)
    {
    case SM_UDP_TO_RS485_DISABLED:
	    if (UdpToRs485Socket != INVALID_UDP_SOCKET) {
	        UDPClose(UdpToRs485Socket);
	        UdpToRs485Socket = INVALID_UDP_SOCKET;
		}
    	break;
    case SM_UDP_TO_RS485_GET_SOCKET:
		// Open a socket to send and receive broadcast messages on
    	UdpToRs485Socket = UDPOpen(0, NULL, UDP_TO_RS485_PORT);
		if(UdpToRs485Socket == INVALID_UDP_SOCKET)
			break;

		smUdpToRs485State = SM_UDP_TO_RS485_WAIT_DATA;
		// No break

    case SM_UDP_TO_RS485_WAIT_DATA:
		{
			WORD len;
			len = UDPIsGetReady(UdpToRs485Socket);

			if(!len) {
				break;
			}
			else {
				UDPGetArray(masFromUdp, len);
				UDPDiscard();

				PrintLog(masFromUdp, len);

				smUdpToRs485State = SM_UDP_TO_RS485_SEND_REPLY;
			}
		}
    	break;

    case SM_UDP_TO_RS485_SEND_REPLY:
    	Rs485ToUdpSocket = UDPOpen(0, NULL, UDP_FROM_RS485_PORT);
    	if(UDPIsPutReady(Rs485ToUdpSocket) < 100)
    		break;

     	UDPPut('T');
    	UDPPut('E');
    	UDPPut('S');
    	UDPPut('T');
    	UDPPut('-');
    	UDPPut('O');
    	UDPPut('K');

    	// Transmit the packet
    	UDPFlush();
    	UDPClose(Rs485ToUdpSocket);

       	smUdpToRs485State = SM_UDP_TO_RS485_WAIT_DATA;
    	break;
    }

}
