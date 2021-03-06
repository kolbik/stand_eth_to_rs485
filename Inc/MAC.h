/*********************************************************************
 *
 *                  MAC Module Defs for Microchip Stack
 *
 *********************************************************************
 * FileName:        MAC.h
 * Dependencies:    StackTsk.h
 * Processor:       PIC18, PIC24F, PIC24H, dsPIC30F, dsPIC33F
 * Complier:        Microchip C18 v3.02 or higher
 *					Microchip C30 v2.01 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright � 2002-2007 Microchip Technology Inc.  All rights 
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and 
 * distribute: 
 * (i)  the Software when embedded on a Microchip microcontroller or 
 *      digital signal controller product (�Device�) which is 
 *      integrated into Licensee�s product; or
 * (ii) ONLY the Software driver source files ENC28J60.c and 
 *      ENC28J60.h ported to a non-Microchip device used in 
 *      conjunction with a Microchip ethernet controller for the 
 *      sole purpose of interfacing with the ethernet controller. 
 *
 * You should refer to the license agreement accompanying this 
 * Software for additional information regarding your rights and 
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT 
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL 
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF 
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS 
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER 
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT 
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Nilesh Rajbharti     4/27/01     Original        (Rev 1.0)
 * Nilesh Rajbharti     11/27/01    Added SLIP
 * Nilesh Rajbharti     2/9/02      Cleanup
 * Nilesh Rajbharti     5/22/02     Rev 2.0 (See version.log for detail)
 * Howard Schlunder     6/28/04     Added ENC28J60 specific features
 * Howard Schlunder		11/29/04	Added Get/SetLEDConfig macros
 ********************************************************************/
#ifndef __MAC_H
#define __MAC_H


#include "ENC28J60.h"
#include "StackTsk.h"


// The MAC_TX_BUFFER_COUNT must be equal to MAX_TCP_SOCKETS + 1
// (1 reserved for high priority messages), or else calls to
// TCPPut may fail when multiple TCP sockets have data pending
// in the output buffer that hasn't been ACKed.  Changing this
// value is recommended only if the rammifications of doing so
// are properly understood.
#if defined(NON_MCHP_MAC)
	#define MAC_TX_BUFFER_SIZE          (1024ul)
	#define MAC_TX_BUFFER_COUNT         (1u)

	#if (MAC_TX_BUFFER_SIZE <= 0 || MAC_TX_BUFFER_SIZE > 1500 )
		#error Invalid MAC_TX_BUFFER_SIZE value specified.
	#endif
#else
	#define MAC_TX_BUFFER_SIZE			(1500ul)
#endif

// A generic structure representing the Ethernet header starting all Ethernet
// frames
typedef struct _ETHER_HEADER
{
	MAC_ADDR        DestMACAddr;
	MAC_ADDR        SourceMACAddr;
	WORD_VAL        Type;
} ETHER_HEADER;

// Rest are Receive Buffers


#define MAC_IP      	(0x00u)
#define MAC_ARP     	(0x06u)
#define MAC_UNKNOWN 	(0xFFu)



/*
 * Microchip Ethernet controller specific MAC items
 */
#if !defined(RESERVED_TCP_MEMORY)
	#define RESERVED_TCP_MEMORY 0ul
#endif

// MAC RAM definitions
#define RAMSIZE	8192ul		
#define TXSTART ((RAMSIZE-1ul) - (1ul+1514ul+7ul) - RESERVED_TCP_MEMORY)
#define RXSTART	(0ul)						// Should be an even memory address; must be 0 for errata
#define	RXSTOP	((TXSTART-2ul) | 0x0001ul)	// Odd for errata workaround
#define RXSIZE	(RXSTOP-RXSTART+1ul)

#define BASE_TX_ADDR	(TXSTART + 1ul)
#define BASE_TCB_ADDR	(BASE_TX_ADDR + (1514ul+7ul))

#if RXSIZE < 1400
	#error Warning, Ethernet RX buffer is tiny.  Reduce MAX_TCP_SOCKETS, TCP_TX_FIFO_SIZE, or TCP_RX_FIFO_SIZE
#endif


WORD	MACCalcRxChecksum(WORD offset, WORD len);
WORD 	CalcIPBufferChecksum(WORD len);

void	MACPowerDown(void);
void 	MACPowerUp(void);
void	WritePHYReg(BYTE Register, WORD Data);
PHYREG	ReadPHYReg(BYTE Register);
void	SetRXHashTableEntry(MAC_ADDR DestMACAddr);

// ENC28J60 specific
void	SetCLKOUT(BYTE NewConfig);
BYTE	GetCLKOUT(void);

/******************************************************************************
 * Macro:        	void SetLEDConfig(WORD NewConfig)
 *
 * PreCondition:    SPI bus must be initialized (done in MACInit()).
 *
 * Input:           NewConfig - xxx0: Pulse stretching disabled
 *								xxx2: Pulse stretch to 40ms (default)
 *								xxx6: Pulse stretch to 73ms
 *								xxxA: Pulse stretch to 139ms
 *								
 *								xx1x: LEDB - TX
 *								xx2x: LEDB - RX (default)
 *								xx3x: LEDB - collisions
 *								xx4x: LEDB - link
 *								xx5x: LEDB - duplex
 *								xx7x: LEDB - TX and RX
 *								xx8x: LEDB - on
 *								xx9x: LEDB - off
 *								xxAx: LEDB - blink fast
 *								xxBx: LEDB - blink slow
 *								xxCx: LEDB - link and RX
 *								xxDx: LEDB - link and TX and RX
 *								xxEx: LEDB - duplex and collisions
 *
 *								x1xx: LEDA - TX
 *								x2xx: LEDA - RX
 *								x3xx: LEDA - collisions
 *								x4xx: LEDA - link (default)
 *								x5xx: LEDA - duplex
 *								x7xx: LEDA - TX and RX
 *								x8xx: LEDA - on
 *								x9xx: LEDA - off
 *								xAxx: LEDA - blink fast
 *								xBxx: LEDA - blink slow
 *								xCxx: LEDA - link and RX
 *								xDxx: LEDA - link and TX and RX
 *								xExx: LEDA - duplex and collisions
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Writes the value of NewConfig into the PHLCON PHY register.  
 *					The LED pins will beginning outputting the new 
 *					configuration immediately.
 *
 * Note:            
 *****************************************************************************/
#define SetLEDConfig(NewConfig)		WritePHYReg(PHLCON, NewConfig)


/******************************************************************************
 * Macro:        	WORD GetLEDConfig(void)
 *
 * PreCondition:    SPI bus must be initialized (done in MACInit()).
 *
 * Input:           None
 *
 * Output:          WORD -	xxx0: Pulse stretching disabled
 *							xxx2: Pulse stretch to 40ms (default)
 *							xxx6: Pulse stretch to 73ms
 *							xxxA: Pulse stretch to 139ms
 *								
 *							xx1x: LEDB - TX
 *							xx2x: LEDB - RX (default)
 *							xx3x: LEDB - collisions
 *							xx4x: LEDB - link
 *							xx5x: LEDB - duplex
 *							xx7x: LEDB - TX and RX
 *							xx8x: LEDB - on
 *							xx9x: LEDB - off
 *							xxAx: LEDB - blink fast
 *							xxBx: LEDB - blink slow
 *							xxCx: LEDB - link and RX
 *							xxDx: LEDB - link and TX and RX
 *							xxEx: LEDB - duplex and collisions
 *
 * 							x1xx: LEDA - TX
 *							x2xx: LEDA - RX
 *							x3xx: LEDA - collisions
 *							x4xx: LEDA - link (default)
 *							x5xx: LEDA - duplex
 *							x7xx: LEDA - TX and RX
 *							x8xx: LEDA - on
 *							x9xx: LEDA - off
 *							xAxx: LEDA - blink fast
 *							xBxx: LEDA - blink slow
 *							xCxx: LEDA - link and RX
 *							xDxx: LEDA - link and TX and RX
 *							xExx: LEDA - duplex and collisions
 *
 * Side Effects:    None
 *
 * Overview:        Returns the current value of the PHLCON register.
 *
 * Note:            None
 *****************************************************************************/
#define GetLEDConfig()		ReadPHYReg(PHLCON).Val


void MACInit(void);
BOOL MACIsLinked(void);

BOOL MACGetHeader(MAC_ADDR *remote, BYTE* type, MAC_ADDR *mac_dest);
void MACSetReadPtrInRx(WORD offset);
WORD MACSetWritePtr(WORD address);
WORD MACSetReadPtr(WORD address);
BYTE MACGet(void);
WORD MACGetArray(BYTE *val, WORD len);
void MACDiscardRx(void);
WORD MACGetFreeRxSize(void);
void MACMemCopyAsync(WORD destAddr, WORD sourceAddr, WORD len);
BOOL MACIsMemCopyDone(void);

void MACPutHeader(MAC_ADDR *remote, BYTE type, WORD dataLen);
BOOL MACIsTxReady(void);
void MACPut(BYTE val);
void MACPutArray(BYTE *val, WORD len);
void MACPutROMArray(const BYTE *val, WORD len);
void MACFlush(void);


#if defined(STACK_USE_SLIP)
#define NICCurrentTxBuffer      (0u)
#endif



#endif
