/*
 * StackTsk.c
 *
 *  Created on: 7 апр. 2018 г.
 *      Author: dimich
 */

/* Includes ------------------------------------------------------------------*/
#include <ENC28J60.h>


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

}
