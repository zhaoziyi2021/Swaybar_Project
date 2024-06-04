/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : CAN_Cfg.h
************************************************************************************************
*   Project/Product :
*   Title           :
*   Author          : CATARC
************************************************************************************************
*   Description     : This is common module for body netword
*
************************************************************************************************
*   Limitations     : NONE
*
************************************************************************************************
*   END_FILE_HDR*/

#ifndef CAN_CFG_H
#define CAN_CFG_H


/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Header Files:   			                                                 */

#include "ncs.h"

/*                                                                               */
/*********************************************************************************/


/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Configuration Constants                                                     */


/* Number of CAN controller used */
#define CAN_CTRL_NB     1

/* Node used in monocontroller CAN*/
#define CAN_CTRL_SEL    0

#define CAN_BIT_TIME_BRP                1
#define CAN_BIT_TIME_PROPSEG            0
#define CAN_BIT_TIME_PSEG1              7
#define CAN_BIT_TIME_PSEG2              2
#define CAN_BIT_TIME_SMP                0	// one sample per bit
#define CAN_BIT_TIME_SJW                2

// CAN acceptance filters
// LSB : 7..0 : identifier(10..3)
// MSB : 7..5 : identifier(2..0)
//       4    : RTR
//       3    : IDE
//       2..0 : unused


/*-------------------------------------------------------------------------------*/
/* - Configuration Variable or Register Aliases                                  */

/*-------------------------------------------------------------------------------*/
/* - Public Constants                                                            */

/*-------------------------------------------------------------------------------*/
/* - Public Macros                                                               */

/*                                                                               */
/*********************************************************************************/




#endif      // CAN_CFG_H
