/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute                                  
* All rights exclusively reserved for Automotive Engineering Research Institute,               
* unless expressly agreed to otherwise.                                         
************************************************************************************************
*   File Name       : STDDEF.h
************************************************************************************************
*   Project/Product : 
*   Title           : Standard Define 
*   Author          : bwfeng
************************************************************************************************
*   Description     : This is common module for body netword
*
************************************************************************************************
*   Limitations     : NONE
*
************************************************************************************************
*
************************************************************************************************
*   Revision History:
* 
*   Version       Date         Initials     CR#           Descriptions
*   ---------   -----------  ------------  ----------  ---------------
*      1.0       16/01/29       bwfeng       N/A           Original
************************************************************************************************
*   END_FILE_HDR*/

#ifndef SOFT_TIMER_APP_EXT_H
#define SOFT_TIMER_APP_EXT_H

/*Include files*/
#include "SoftTimerCfgExt.h"

/*declaration range definition*/
#ifdef SOFT_TIMER_DRV_C 
#define SOFT_TIMER_APP_EXT 
#else 
#define SOFT_TIMER_APP_EXT extern 
#endif 

/*  structure definiton */


/* macro definition */

/*Soft timer state definition*/
#define SOFT_TIMER_ST_OFF          0x00u /*timer not started*/
#define SOFT_TIMER_ST_RUNING       0x01u /*timer is running*/
#define SOFT_TIMER_ST_STOP         0x02u /*timer has stopped*/
#define SOFT_TIMER_ST_REACH        0x03u /*timer reach its end*/


/* variable definition */


/* Type Definitions */


/* function declaration */
SOFT_TIMER_APP_EXT void     SoftTimerInit(void);
SOFT_TIMER_APP_EXT void     SoftTimerStop(ARINT16U awSoftTimerIndex);
SOFT_TIMER_APP_EXT ARINT8U  ChkSoftTimerState(ARINT16U awSoftTimerIndex);
SOFT_TIMER_APP_EXT void     SoftTimerStart(ARINT16U awSoftTimerIndex, SOFT_TIMER_TMR_TYPE aTmrEndVal);
SOFT_TIMER_APP_EXT void     SoftTimerInc(void);
SOFT_TIMER_APP_EXT void     SoftTimerResultJudge(void);

#endif
