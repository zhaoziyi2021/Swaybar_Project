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

#ifndef SOFT_TIMER_CFG_EXT_H
#define SOFT_TIMER_CFG_EXT_H

/*Include files*/
#include "STDDEF.h"
#include "EB_Prj.h"

/* macro definition  */ 



typedef ARINT16U SOFT_TIMER_TMR_TYPE;
/*config the name and index of each soft timer*/
enum
{
    Security_Access_Timer,
    Tester_Absent_Timer,
    RKE1_Learned_Timer,
    RKE2_Learned_Timer,
    DTC_Record_Wait_Timer,
    DTC_Record_Wait_Timer1,
    ptcan_BusOff_Recovery_Timer,

    IO_Init_Timer,
    ic_wakeup_form_FL_DoorAjar_Timer,
    ic_wakeup_form_FR_DoorAjar_Timer,
    ic_wakeup_form_RL_DoorAjar_Timer,
    ic_wakeup_form_RR_DoorAjar_Timer,
    ic_wakeup_form_TrunkAjar_Timer,
    ic_wakeup_form_HoodAjar_Timer,
    ic_wakeup_form_FL_Door_Latch_Timer,
    ic_wakeup_form_FR_Door_Latch_Timer,
    ic_wakeup_form_RL_Door_Latch_Timer,
    ic_wakeup_form_RR_Door_Latch_Timer,
    ic_wakeup_form_CentralLock_SW_Timer,
    ic_wakeup_form_TrunkUnlock_SW_Timer,
    ic_wakeup_form_hazard_Timer,
    ic_wakeup_form_rke_Timer,
    lin_send_wakeup_Timer,

    Model_Save_EE_Filter_Timer,
    Ic_Sleep_Timer,

    CNT_OF_SOFT_TIMERS
};

#endif
