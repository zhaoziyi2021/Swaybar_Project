/**
  ******************************************************************************
  * @file    TaskStep.h
  * @author  
  * @version V1.0.0
  * @date    18-09-2017
  * @brief   Task Service Routines.
  ******************************************************************************
  */
#ifndef _TASK_STEP_H_
#define _TASK_STEP_H_


#ifndef ARRAYSIZE
    #define ARRAYSIZE(a) (sizeof(a)/sizeof((a)[0]))
#endif

#define SYS_TICK_STEP  5

typedef struct{
    void(*fTask)(void);
    uword  Tick_Count;
    uword  Period;
    uword  StartTime;
}sTask;

extern ubyte CAN0BusOffFlag;
extern volatile ubyte Sys_Tick_Set_Flag;

extern void Run_Task(void);

#endif
