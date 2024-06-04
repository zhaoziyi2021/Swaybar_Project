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

#ifndef SOFT_TIMER_DRV_H
#define SOFT_TIMER_DRV_H

/*Include files*/
#include "SoftTimerAppExt.h"


/*declaration range definition*/
#ifdef SOFT_TIMER_DRV_C
#define SOFT_TIMER_DRV_EXT
#else
#define SOFT_TIMER_DRV_EXT extern
#endif

/*  structure definiton  */
typedef struct
{
    SOFT_TIMER_TMR_TYPE  m_CurVal;
    SOFT_TIMER_TMR_TYPE  m_EndVal;
    ARINT8U              m_State;
}t_SoftTimerStr;

/* macro definition */
#define SOFT_TIMER_CNT_RESET     0x00u



#endif /* End of header file */
