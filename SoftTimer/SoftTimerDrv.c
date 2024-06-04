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

#ifndef SOFT_TIMER_DRV_C
#define SOFT_TIMER_DRV_C
#endif

/* include files */
#include "SoftTimerDrv.h"

/* static function declaration */

/* variable definition */
static t_SoftTimerStr itpSoftTimer[CNT_OF_SOFT_TIMERS];



/* BEGIN_FUNCTION_HDR
*********************************************************************************************************
* Function Name : void SoftTimerInit(void)
*
* Description   : none
*
* Inputs        : none
*
* Outputs       : none
*
* Limitations   : nothing
*********************************************************************************************************
END_FUNCTION_HDR */
void SoftTimerInit(void)
{
    ARINT16U i = 0;
    
    for(i = 0; i < CNT_OF_SOFT_TIMERS; i++)
    {
        itpSoftTimer[i].m_State = SOFT_TIMER_ST_OFF;
    }
}

/* BEGIN_FUNCTION_HDR
*********************************************************************************************************
* Function Name : void SoftTimerInit(HRINT16U SoftTimerIndex)
*
* Description   : none
*
* Inputs        : none
*
* Outputs       : none
*
* Limitations   : nothing
*********************************************************************************************************
END_FUNCTION_HDR */
ARINT8U ChkSoftTimerState(ARINT16U SoftTimerIndex)
{
    ARINT8U retVal = SOFT_TIMER_ST_OFF;
    
    if(SoftTimerIndex < CNT_OF_SOFT_TIMERS)
    {
        retVal = itpSoftTimer[SoftTimerIndex].m_State;
    }
    else
    {
        /*do nothing*/
    }
    
    return retVal;
}

/* BEGIN_FUNCTION_HDR
*********************************************************************************************************
* Function Name : void SoftTimerStop(ARINT16U SoftTimerIndex)
*
* Description   : none
*
* Inputs        : none
*
* Outputs       : none
*
* Limitations   : nothing
*********************************************************************************************************
END_FUNCTION_HDR */
void SoftTimerStop(ARINT16U SoftTimerIndex)
{        
    if(SoftTimerIndex < CNT_OF_SOFT_TIMERS)
    {
        itpSoftTimer[SoftTimerIndex].m_State = SOFT_TIMER_ST_STOP;
    }
    else
    {
        /*do nothing*/
    }
}


/* BEGIN_FUNCTION_HDR
*********************************************************************************************************
* Function Name : void SoftTimerStart(ARINT16U SoftTimerIndex, SOFT_TIMER_TMR_TYPE aTimeEndVal)
*
* Description   : none
*
* Inputs        : none
*
*
* Outputs       : none
*
* Limitations   : nothing
*********************************************************************************************************
END_FUNCTION_HDR */
void SoftTimerStart(ARINT16U SoftTimerIndex, SOFT_TIMER_TMR_TYPE aTimeEndVal)
{
    if(SoftTimerIndex < CNT_OF_SOFT_TIMERS)
    {
        itpSoftTimer[SoftTimerIndex].m_CurVal = SOFT_TIMER_CNT_RESET;
        itpSoftTimer[SoftTimerIndex].m_EndVal = aTimeEndVal;
        itpSoftTimer[SoftTimerIndex].m_State  = SOFT_TIMER_ST_RUNING;
    }
    else
    {
        /*do nothing*/
    } 
}

/* BEGIN_FUNCTION_HDR
*********************************************************************************************************
* Function Name : void SoftTimerInc(void)
*
* Description   : none
*
* Inputs        : none
*
* Outputs       : none
*
* Limitations   : nothing
*********************************************************************************************************
END_FUNCTION_HDR */
void SoftTimerInc(void)
{
    ARINT16U i = 0;
    
    for(i = 0; i < CNT_OF_SOFT_TIMERS; i++)
    {
        if(itpSoftTimer[i].m_State == SOFT_TIMER_ST_RUNING)
        {
            itpSoftTimer[i].m_CurVal++;
            if(itpSoftTimer[i].m_CurVal >= itpSoftTimer[i].m_EndVal)
            {
                itpSoftTimer[i].m_State = SOFT_TIMER_ST_REACH;
            }
            else
            {
                /* do noting */
            }
        }
        else
        {
            /* do nothing */
        }
    }
}

/* BEGIN_FUNCTION_HDR
*********************************************************************************************************
* Function Name : void SoftTimerResultJudge(void)
*
* Description   : none
*
* Inputs        : none
*
*
* Outputs       : none
*
* Limitations   : nothing
*********************************************************************************************************
END_FUNCTION_HDR */
void SoftTimerResultJudge(void)
{
    if(ChkSoftTimerState(Tester_Absent_Timer) == SOFT_TIMER_ST_REACH)
    {
        Tester_Absent_Flg = TRUE;
        (void)DefaultSession(NULL_PTR, NULL_PTR);
    }
    else
    {
        /*do nothing*/
    }
}
