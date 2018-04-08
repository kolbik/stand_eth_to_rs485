/*
 * UDPtoRs485.h
 *
 *  Created on: 7 апр. 2018 г.
 *      Author: dimich
 */

#ifndef UDPTORS485_H_
#define UDPTORS485_H_

typedef enum _SM_UDP_TO_RS485
{
	SM_UDP_TO_RS485_DISABLED = 0,
	SM_UDP_TO_RS485_GET_SOCKET,
	SM_UDP_TO_RS485_WAIT_DATA
} SM_UDP_TO_RS485;

void UdpToRs485Task(void);

#endif /* UDPTORS485_H_ */
