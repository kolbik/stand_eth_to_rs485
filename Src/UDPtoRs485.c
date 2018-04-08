/*
 * UDPtoRs485.c
 *
 *  Created on: 7 апр. 2018 г.
 *      Author: dimich
 */

#include <UDPtoRs485.h>
#include <string.h>
#include <UDP.h>

SM_UDP_TO_RS485 smUdpToRs485State = SM_UDP_TO_RS485_GET_SOCKET;

#define UDP_TO_RS485_PORT	50000

static UDP_SOCKET UdpToRs485Socket = INVALID_UDP_SOCKET;

static volatile cnt_rcv_packet = 0;


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
		if(UDPIsGetReady(UdpToRs485Socket) < 258) {
			cnt_rcv_packet++;
		}
    	break;
    }

}
