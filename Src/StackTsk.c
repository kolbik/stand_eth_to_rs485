/*
 * StackTsk.c
 *
 *  Created on: 7 апр. 2018 г.
 *      Author: dimich
 */

/* Includes ------------------------------------------------------------------*/
#include <StackTsk.h>
#include <ARP.h>
#include <IP.h>
#include <ICMP.h>
#include <MAC.h>
#include <ENC28J60.h>
#include <stdio.h>
#include <string.h>

NODE_INFO remoteNode;

extern void PrintLog(BYTE *mas, WORD len);

void MacAdressToStr(const MAC_ADDR *MACAddr)
{
	char str[30];

	sprintf(str,"%02X:%02X:%02X:%02X:%02X:%02X \r\n",
			MACAddr->v[0],MACAddr->v[1],MACAddr->v[2],MACAddr->v[3],MACAddr->v[4],MACAddr->v[5]);

	PrintLog((unsigned char*)str, strlen(str));

}

/*********************************************************************
 * Function:        void StackInit(void)
 * PreCondition:    None
 * Input:           None
 * Output:          Stack and its componets are initialized
 * Side Effects:    None
 * Note:            This function must be called before any of the
 *                  stack or its component routines are used.
 ********************************************************************/
void StackInit(void)
{
	MACInit();
}


/*********************************************************************
 * Function:        void StackTask(void)
 * PreCondition:    StackInit() is already called.
 * Input:           None
 * Output:          Stack FSM is executed.
 * Side Effects:    None
 * Note:            This FSM checks for new incoming packets,
 *                  and routes it to appropriate stack components.
 *                  It also performs timed operations.
 *
 *                  This function must be called periodically to
 *                  ensure timely responses.
 ********************************************************************/
void StackTask(void)
{
    WORD dataCount;
    IP_ADDR tempLocalIP;
	BYTE cFrameType;
	BYTE cIPFrameType;
	MAC_ADDR macDest;


	#if defined(STACK_USE_TCP)
	// Perform all TCP time related tasks (retransmit, send acknowledge, close connection, etc)
	TCPTick();
	#endif


	// Process as many incomming packets as we can
	while(MACGetHeader(&remoteNode.MACAddr, &cFrameType, &macDest))
	{
		switch(cFrameType)
		{
			case MAC_ARP:
				{
					ARPProcess();
					//const char *str = "cFrameType = MAC_ARP\r\n";
					//PrintLog((BYTE *)str, strlen(str));
				}
				break;

			case MAC_IP:
				{
					MacAdressToStr(&remoteNode.MACAddr);
					MacAdressToStr(&macDest);

					const char *str = "cFrameType = MAC_IP\r\n";
					PrintLog((BYTE *)str, strlen(str));
				}

				if(!IPGetHeader(&tempLocalIP, &remoteNode, &cIPFrameType, &dataCount))
					break;

				if(cIPFrameType == IP_PROT_ICMP)
				{
					ICMPProcess(&remoteNode, dataCount);
					const char *str = "cIPFrameType == IP_PROT_ICMP\r\n";
					PrintLog((BYTE *)str, strlen(str));

					#if defined(STACK_USE_IP_GLEANING)
					if(AppConfig.Flags.bInConfigMode && AppConfig.Flags.bIsDHCPEnabled)
					{
						// Accoriding to "IP Gleaning" procedure,
						// when we receive an ICMP packet with a valid
						// IP address while we are still in configuration
						// mode, accept that address as ours and conclude
						// configuration mode.
						if(tempLocalIP.Val != 0xffffffff)
						{
							AppConfig.Flags.bInConfigMode = FALSE;
							AppConfig.MyIPAddr = tempLocalIP;
							myDHCPBindCount--;
						}
					}
					#endif
					break;
				}

				#if defined(STACK_USE_TCP)
				if(cIPFrameType == IP_PROT_TCP)
				{
					TCPProcess(&remoteNode, &tempLocalIP, dataCount);
					break;
				}
				#endif

				//#if defined(STACK_USE_UDP)
				if(cIPFrameType == IP_PROT_UDP)
				{
					const char *str = "cIPFrameType == IP_PROT_UDP\r\n";
					PrintLog((BYTE *)str, strlen(str));
					//UDPProcess(&remoteNode, &tempLocalIP, dataCount);
					break;
				}
				//#endif

				break;
		}
	}
}
