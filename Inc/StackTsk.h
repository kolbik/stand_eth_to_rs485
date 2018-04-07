/*
 * StackTsk.h
 *
 *  Created on: 7 апр. 2018 г.
 *      Author: dimich
 */

#ifndef STACKTSK_H_
#define STACKTSK_H_

#include <GenericTypeDefs.h>

typedef struct __attribute__((__packed__)) _MAC_ADDR
{
    BYTE v[6];
} MAC_ADDR;

#define IP_ADDR		DWORD_VAL

typedef struct __attribute__((__packed__)) _NODE_INFO
{
    MAC_ADDR    MACAddr;
    IP_ADDR     IPAddr;
} NODE_INFO;

typedef struct __attribute__((__packed__)) _APP_CONFIG
{
	IP_ADDR		MyIPAddr;
	IP_ADDR		MyMask;
	IP_ADDR		MyGateway;
	IP_ADDR		PrimaryDNSServer;
	IP_ADDR		SecondaryDNSServer;
	IP_ADDR		DefaultIPAddr;
	IP_ADDR		DefaultMask;
	MAC_ADDR	MyMACAddr;
	BYTE		NetBIOSName[16];
	struct
	{
		unsigned char : 6;
		unsigned char bIsDHCPEnabled : 1;
		unsigned char bInConfigMode : 1;
	} Flags;
} APP_CONFIG;

void StackInit(void);
void StackTask(void);

#endif /* STACKTSK_H_ */
