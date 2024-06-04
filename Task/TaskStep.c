/**
  ******************************************************************************
  * @file    TaskStep.c
  * @author  zzy
  * @version V1.1.0
  * @date    19-03-2024
  * @brief   Task Service Routines.
  ******************************************************************************
  */
#include "main.h"

volatile ubyte Sys_Tick_Set_Flag = 0;

void Task5msOffset0ms(void);
void Task20msOffset0ms(void);
void Task10msOffset0ms(void);
void Task20msOffset10ms(void);

static sTask mTaskTab[]=
{
    {Task5msOffset0ms,0,5,0},
    {Task20msOffset0ms,0,20,0},
    {Task10msOffset0ms,0,10,0},
    {Task20msOffset10ms,0,30,10}
};


//=================================================================================================
// [Function Name] void GetTimingTickStatus(void)
// [Parameters] 
// [Introduction] Obtain the status of the benchmark timing flag
// [Return value] Benchmark timing flag status
// [Creator] 2017-09-18 ZYW
//=================================================================================================
ubyte GetTimingTickStatus(void)
{
    return Sys_Tick_Set_Flag;
}


//=================================================================================================
// [Function Name] void ClearTimingTickStatus(void)
// [Parameters] 
// [Introduction] Clear reference timing flag
// [Return value] None
// [Creator]
//=================================================================================================
void ClearTimingTickFlag(void)
{
    Sys_Tick_Set_Flag = FALSE;
}


//=================================================================================================
// [Function Name] void Run_Task(void)
// [Parameters] 
// [Introduction] Main Task
// [Return value] None
// [Creator]
//=================================================================================================
void Run_Task(void)
{
    if(GetTimingTickStatus() == TRUE)
    {
        for(ubyte i=0; i<ARRAYSIZE(mTaskTab); i++)
        {
            mTaskTab[i].Tick_Count += SYS_TICK_STEP;
            if(mTaskTab[i].Tick_Count >= mTaskTab[i].Period)
            {
                mTaskTab[i].Tick_Count = mTaskTab[i].StartTime;
                mTaskTab[i].fTask();
            }
        }
        ClearTimingTickFlag();
    }
}


//=================================================================================================
// [Function Name] void Task20msOffset0ms(void)
// [Parameters] 
// [Introduction] Tick 20ms task
// [Return value] None
// [Creator] 2017-09-18 ZYW
//=================================================================================================
void Task20msOffset0ms(void)
{
    
    if(BcmEnterSleepCheck() == BCM_READY_FOR_SLEEP)
    {
        if(ChkSoftTimerState(Ic_Sleep_Timer) == SOFT_TIMER_ST_REACH)
        {
            SoftTimerStop(Ic_Sleep_Timer);
            ic_wakeup_src = ic_wakeup_clear;
            // NVIC_SystemReset();
        }

        if(ChkSoftTimerState(Ic_Sleep_Timer) != SOFT_TIMER_ST_RUNING)
        {
            SoftTimerStart(Ic_Sleep_Timer, IC_SLEEP_TIMEROUT);
        }
    }
    else
    {
        SoftTimerStop(Ic_Sleep_Timer);
    }

    if(WDT_Disable == 0)
    {
        // 80ms
        fwdgt_counter_reload();
    }
}


//=================================================================================================
// [Function Name] void Task10msOffset0ms(void)
// [Parameters] None
// [Introduction] Tick 10ms task
// [Return value] None
// [Creator]
//=================================================================================================
void Task10msOffset0ms(void)
{
    Can_GetBusOffStatus();
    EB_Manage();
    SoftTimerInc();
    SoftTimerResultJudge();
    GetDigtalIOState();
    /*Interface of the bottom drive code and the model*/
    RTE();
    Swaybar_Mainfunction();
}


//=================================================================================================
// [Function Name] void Task20msOffset10ms(void)
// [Parameters] None
// [Introduction] Tick 20ms task,for Task20msOffset0ms Offset 10ms
// [Return value] None
// [Creator]
//=================================================================================================
void Task20msOffset10ms(void)
{
    // None
}


//=================================================================================================
// [Function Name] void Task5msOffset0ms(void)
// [Parameters] None
// [Introduction] Tick 5ms task Offset 5ms
// [Return value] None
// [Creator] 2023-12-25 ZZY
//=================================================================================================
void Task5msOffset0ms(void)
{
    static uword PullTimeCnt = 0;
    static uword LinkTimeCnt = 0;
    SrvIO_Mainfunction();

#if 1
    if((CAN_Rcv.DebugMessage[0] != 0x00) || 
        (CAN_Rcv.DebugMessage[1] != 0x00))
    {
        if(TravelSensorisLink)
        {
            PullTimeCnt = 0;
        }
        else if(TravelSensorisOther)
        {
            PullTimeCnt ++;
            LinkTimeCnt ++;
        }
        else
        {
            LinkTimeCnt = 0;
        }

        BCanDebugSend(0xD1,
            IO_Sts.Ad_DrvOutput,
            IO_Sts.PWM_DutyCycle,
            (ubyte)((IO_Sts.Ad_DrvOutputCnrrent & 0xFF00) >> 8),
            (ubyte)(IO_Sts.Ad_DrvOutputCnrrent & 0x00FF),
            (ubyte)(((LinkTimeCnt*5) & 0xFF00) >> 8),
            (ubyte)((LinkTimeCnt*5) & 0x00FF),
            (ubyte)(((PullTimeCnt*5) & 0xFF00) >> 8),
            (ubyte)((PullTimeCnt*5) & 0x00FF));
    }
#endif

}

