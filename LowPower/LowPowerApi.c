/*
 * LowPowerApi.c
 *
 *  Created on: 2015-12-11
 *      Author: ThinkPad
 */

#include "MAIN.h"

IC_WAKEUP_SRC ic_wakeup_src;

ubyte Bcan_Wakeup_Flag = FALSE;
ubyte Bcan_Wakeup_Event = FALSE;
uword Bcan_Wakeup_Event_True_Count = 0;

ubyte FL_Door_Latch_Pre = 0;
ubyte FR_Door_Latch_Pre = 0;
ubyte RL_Door_Latch_Pre = 0;
ubyte RR_Door_Latch_Pre = 0;
//=================================================================================================
// [Function Name] void LowPowerApiInit(void)
// [Parameters]
// [Introduction] 初始化LowPowerApi
// [Return value] 无
// [Creator] 2018-08-21 ZYW
//=================================================================================================
void LowPowerApiInit(void)
{
    if((ic_wakeup_src == ic_wakeup_form_FL_Door_Latch)||
       (ic_wakeup_src == ic_wakeup_form_FR_Door_Latch)||
       (ic_wakeup_src == ic_wakeup_form_RL_Door_Latch)||
       (ic_wakeup_src == ic_wakeup_form_RR_Door_Latch))
    {
        // BcanWakeupCheck();
    }

    FL_Door_Latch_Pre = P15_IN_P5;
    FR_Door_Latch_Pre = P10_IN_P7;
    RL_Door_Latch_Pre = P5_IN_P5;
    RR_Door_Latch_Pre = P6_IN_P2;
}

//=================================================================================================
// [Function Name] void BcanWakeupCheck(void)
// [Parameters]
// [Introduction] BCAN唤醒检测
// [Return value] None
// [Creator] 2018-06-26 ZYW
//=================================================================================================
void BcanWakeupCheck(void)
{
    // if(NM_AUTOSAR_OUTPUT.CanNM_Sleep_Flag == 1)
    // {
    //     Bcan_Wakeup_Event = TRUE;
    // }
}

//=================================================================================================
// [Function Name] ubyte CanSleepWakeUpCheck(void)
// [Parameters]
// [Introduction] Sleep wake-up detection
// [Return value] None
// [Creator] 2018-09-04 ZYW
//=================================================================================================
unsigned char CanSleepWakeUpCheck(void)
{
    // ----------------------------------------------------------------------
    // BAT+ 上电唤醒
    // ----------------------------------------------------------------------
    #if 0
    if(BCM_Can_Wakeup_Source == ic_wakeup_form_PowerOn)
    {
        if(NM_AUTOSAR_OUTPUT.CanNM_Sleep_Flag == TRUE)
        {
            Bcan_Wakeup_Event = TRUE;
        }

        BCM_Can_Wakeup_Source = ic_wakeup_clear;
    }

    if(Bcan_Wakeup_Event == TRUE)
    {
        Bcan_Wakeup_Event_True_Count++;
        if(Bcan_Wakeup_Event_True_Count>=2)
        {
            Bcan_Wakeup_Flag = TRUE;
        }

        if(Bcan_Wakeup_Event_True_Count>=3)
        {
            Bcan_Wakeup_Flag = FALSE;
            Bcan_Wakeup_Event = FALSE;
            Bcan_Wakeup_Event_True_Count = 0;
        }
    }

    if(Bcan_Wakeup_Flag == TRUE)
    {
        return(SRC_POWER_ON);
    }
    #endif

    if(Tester_Absent_Flg == FALSE)//诊断仪在线
    {
        return(SRC_UDS);
    }
    else
    {

    }
    #if RF_DEBUG
    return(1);
    #endif

    // if(CAN_Rcv.CAN_Debug_Rcv.BYTE0.bits.Debug_Send_Enable_0 == 1)
    // {
    //     return(1);
    // }
    // else
    // {

    // }
    return(0);
}

//=================================================================================================
// [Function Name] ubyte BcmEnterSleepCheck(void)
// [Parameters]
// [Introduction] 休眠检测
// [Return value] None
// [Creator] 2018-08-23 ZYW
//=================================================================================================
ubyte BcmEnterSleepCheck(void)
{
    // ----------------------------------------------------------------------
    // 输入采集唤醒源
    // ----------------------------------------------------------------------
    //if(IO_ubReadPin(IO_P5_4)==0)//IO_Input_Status.Reserved2.value == 0)
    //{
    //	return(SRC_KL15);
    //}

    // if(NM_AUTOSAR_OUTPUT.CanNM_Sleep_Flag == 0)
    // {
    //     return(1);
    // }
    // else
    // {

    // }

    // if(VBAT_Unable_Flag == TRUE)
    // {
    //     return(1);
    // }
    // else
    // {

    // }

    if(Tester_Absent_Flg == FALSE)
    {
        return(1);
    }
    else
    {

    }
    return(0);
}

//=================================================================================================
// [Function Name] void BcmEnterSleepDeal(void)
// [Parameters]
// [Introduction]
// [Return value] None
// [Creator] 2018-08-23 ZYW
//=================================================================================================
void BcmEnterSleepDeal(void)
{
    (void)DTCSleepDeal();
    SysTick->VAL = 0UL;        
    SysTick->CTRL = 0;
    CAN_vInit();//Solve the problem of single node can not sleep.
    
    /* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);

    /* enter sleep mode */
    pmu_wakeup_pin_enable(PMU_WAKEUP_PIN1);
    pmu_to_standbymode(WFI_CMD);
}
