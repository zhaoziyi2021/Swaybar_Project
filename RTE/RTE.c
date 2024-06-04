/*
 * RTE.c
 *
 *  Created on: 2015-10-09
 *      Author: ThinkPad
 */

#include "MAIN.h"

SYSTEM_CONTROL System_Control;
ubyte CfgDID_VDEMEn = 0;
extern void __no_operation(void);


//=================================================================================================
// [Function Name] void Delay(uword delay)
// [Parameters]
// [Introduction] Short delay
// [Return value] None
// [Creator] 2018-09-10 ZYW
//=================================================================================================
void Delay(uword delay)
{
    uword i = 0;

    for(i=delay;i>0;i--)
    {
        __no_operation();
    }
}


//=================================================================================================
// [Function Name] void RTE(void)
// [Parameters]
// [Introduction] Interface of the bottom drive code and the model
// [Return value] None
// [Creator] 20240319 zzy
//=================================================================================================
void RTE(void)
{
    SrvComStep();
    DtcRecordControl();
}


//=================================================================================================
// [Function Name] void VariableInitialization(void)
// [Parameters] None
// [Introduction] Initialize global variables
// [Return value] None
// [Creator] 20240319 zzy
//=================================================================================================
void VariableInitialization(void)
{
    __set_PRIMASK(1);

    uword * puwLen = NULL;
    ubyte Data[20] = {0};

    (void) memset((void *)&System_Control, 0,sizeof(SYSTEM_CONTROL));

    CanStbWakePin();
    
    SoftTimerInit();
    SrvIOInit();
    SrvComInit();
    LowPowerApiInit();
    FlashEep_Init();
    (void)ReadConfigDID();
    Access_Security_PowerInit();
    DTCInit();

    (void)ReadParaFromFlash(puwLen, Data, FLASH_OFFSET_ADDR_ECUCfg, 4);
    if((Data[3] == 0x00) || (Data[3] == 0xFF))
    {
        Diag_Time_Pull2 = 12; //Default 120ms
    }
    else
    {
        Diag_Time_Pull2 = Data[3];
    }

    if((Data[4] == 0x00) || (Data[4] == 0xFF))
    {
        Diag_Time_Stuck = 50; //Default 500ms
    }
    else
    {
        Diag_Time_Stuck = Data[4];
    }

    __set_PRIMASK(0);
}
