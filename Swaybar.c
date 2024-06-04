/*
 * Swaybar.c
 *
 *  Created on: 2024-5-24
 *      Author: zhaoziyi
 * 
 *   IO Output: 1. DriverState;
 *              2. DRV_SW_LED_ON & DRV_SW_LED_OFF;
 *  CAN OutPut: 1. FrontSwaybarSts;
 *              2. FrontSwaybarGreenLamp;
 *              3. FrontSwaybarYellowLamp
 *              4. FrontSwaybarSW;
 *              5. FronSwaybarFault
 *              6. FrontSwaybarGreenLamp;
 *              7. StatusNotice
 */

#ifndef SWAYBAR_C
#define SWAYBAR_C

/* include files */
#include "Swaybar.h"

#define Swaybar_DrvCtr_Pull2Tm              (10U)
#define Swaybar_DrvCtr_DutyCycleLow         (30U)
#define Swaybar_DrvCtr_DutyCycleHi          (60U)

#define Swaybar_Stuck_TabSize               (4U)
#define Swaybar_Stuck_Distance              (5U)
#define Swaybar_Stuck_FlashCnt              (5U)
#define Swaybar_Stuck_FlashTm               (10U)
#define Swaybar_Stuck_Pull3Tm               (20U)
#define Swaybar_Stuck_LinkDelayTm           (5U)

#define Swaybar_StsNoticeTm                 (500U)

static uint8_t FrontSwaybarSts = 0x00;
static uint8_t FrontSwaybarGreenLamp = 0x00;
static uint8_t FrontSwaybarYellowLamp = 0x00;
static uint8_t FrontSwaybarSW = 0x00;
static uint8_t StatusNotice = 0x00;
static uint8_t SwaybarSwSts = 0x00;
static uint8_t SwaybarPullCtrlStage = 0x00;
static uint8_t SwaybarPullEn = 0x00;
static uint32_t FronSwaybarFault = 0x00;

uint8_t StuckLink = 0x00;
uint8_t StuckPull = 0x00;
uint8_t DriverState = 0x00;

/* static function */
static void Swaybar_DrvControl(uint8_t Cmd);
static void Swaybar_DrvControlPull(void);
static void Swaybar_StuckCheck(void);
static void Swaybar_ErrorCheck(void);
static void Swaybar_LampControl(void);
static void Swaybar_StsNotice(void);

void Swaybar_Init(void);
void Swaybar_Mainfunction(void);


//=================================================================================================
// [Function Name]void Swaybar_DrvControl(void)
// [Parameters] Cmd
// [Introduction] Swaybar Driver State
// [Return value] None
// [Creator] 20240402 zzy
//=================================================================================================
static void Swaybar_DrvControl(uint8_t Cmd)
{
    ubyte SwaybarControlSts;

    if(Diag_SwaybarControlEn)
    {
        SwaybarControlSts = Diag_SwaybarControlSts;
    }
    else
    {
        SwaybarControlSts = Cmd;
    }

    switch (SwaybarControlSts)
    {
        case Swaybar_Drv_Link:
            // 0V,real 11.3
            DRV_DOWN_EN_L();
            DRV_DOWN_5V_L();
            DRV_UP_EN_L();
            DRV_UP_14V_L();
            DRV_SELECT_UP_L();
            DriverState = Swaybar_Drv_Link;
            break;

        case Swaybar_Drv_Hold:
            // 5V,Real 7.16
            DRV_DOWN_EN_H();
            DRV_DOWN_5V_L();
            DRV_UP_EN_L();
            DRV_UP_14V_L();
            DRV_SELECT_DOWN_H();
            DriverState = Swaybar_Drv_Hold;
            break;

        case Swaybar_Drv_Pull0:
            // 10V,Real 10.22
            DRV_DOWN_EN_H();
            DRV_DOWN_10V_H();
            DRV_UP_EN_L();
            DRV_UP_14V_L();
            DRV_SELECT_DOWN_H();
            DriverState = Swaybar_Drv_Pull0;
            break;

        case Swaybar_Drv_Pull1:
            // Reserved!!!!
            DRV_DOWN_EN_H();
            DRV_DOWN_10V_H();
            DRV_UP_EN_L();
            DRV_UP_14V_L();
            DRV_SELECT_DOWN_H();
            DriverState = Swaybar_Drv_Pull1;
            break;

        case Swaybar_Drv_Pull2:
            // 14V,Real 16.45
            DRV_DOWN_EN_L();
            DRV_DOWN_5V_L();
            DRV_UP_EN_H();
            DRV_UP_14V_L();
            DRV_SELECT_UP_L();
            DriverState = Swaybar_Drv_Pull2;
            break;

        case Swaybar_Drv_Pull3:
            // 18V,Real 18.3
            DRV_DOWN_EN_L();
            DRV_DOWN_5V_L();
            DRV_UP_EN_H();
            DRV_UP_18V_H();
            DRV_SELECT_UP_L();
            DriverState = Swaybar_Drv_Pull3;
            break;

        case Swaybar_Drv_StuckPull0:
            // 10V,Real 10.22
            DRV_DOWN_EN_H();
            DRV_DOWN_10V_H();
            DRV_UP_EN_L();
            DRV_UP_14V_L();
            DRV_SELECT_DOWN_H();
            DriverState = Swaybar_Drv_StuckPull0;
            break;

        case Swaybar_Drv_StuckPull1:
            // 18V,Real 18.3
            DRV_DOWN_EN_L();
            DRV_DOWN_5V_L();
            DRV_UP_EN_H();
            DRV_UP_18V_H();
            DRV_SELECT_UP_L();
            DriverState = Swaybar_Drv_StuckPull1;
            break;

        case Swaybar_Drv_StuckLink0:
            // 0V,real 11.3
            DRV_DOWN_EN_L();
            DRV_DOWN_5V_L();
            DRV_UP_EN_L();
            DRV_UP_14V_L();
            DRV_SELECT_UP_L();
            DriverState = Swaybar_Drv_StuckLink0;
            break;

        case Swaybar_Drv_StuckLink1:
            // 10V,Real 10.22
            DRV_DOWN_EN_H();
            DRV_DOWN_10V_H();
            DRV_UP_EN_L();
            DRV_UP_14V_L();
            DRV_SELECT_DOWN_H();
            DriverState = Swaybar_Drv_StuckLink1;
            break;

        default:
            break;
    }
}


//=================================================================================================
// [Function Name]static void Swaybar_DrvControlPull(void)
// [Parameters] void
// [Introduction] Swaybar Driver State
// [Return value] None
// [Creator] 20240517 zzy
//=================================================================================================
static void Swaybar_DrvControlPull(void)
{
    static ubyte TimeCnt_Ctrl1 = 0;

    if(TravelSensorisPull)
    {
        SwaybarPullCtrlStage = 0x00;
        Swaybar_DrvControl(Swaybar_Drv_Hold);
    }
    else 
    {
        FrontSwaybarSts = 0x02;  // Disconnecting

        if(SwaybarPullCtrlStage == 0x00)
        {
            Swaybar_DrvControl(Swaybar_Drv_Pull2);
            SwaybarPullCtrlStage = 0x01;
            TimeCnt_Ctrl1 = 0x00;
        }
        else if (SwaybarPullCtrlStage == 0x01)
        {
            TimeCnt_Ctrl1 ++;

            if(TimeCnt_Ctrl1 >= Swaybar_DrvCtr_Pull2Tm)
            {
                SwaybarPullCtrlStage = 0x02;
            }
        }
        else if((DriverState == Swaybar_Drv_Pull3) ||
            (DriverState == Swaybar_Drv_StuckPull0) ||
            (DriverState == Swaybar_Drv_StuckPull1))
        {
            // In SwaybarPullCtrlStage 2,Check Stuck Frist
        }
        else if((SwaybarPullCtrlStage == 0x02) && 
            (IO_Sts.PWM_DutyCycle < Swaybar_DrvCtr_DutyCycleLow))
        {
            Swaybar_DrvControl(Swaybar_Drv_Pull2);
        }
        else if((SwaybarPullCtrlStage == 0x02) && 
            (IO_Sts.PWM_DutyCycle < Swaybar_DrvCtr_DutyCycleHi))
        {
            Swaybar_DrvControl(Swaybar_Drv_Pull1);
        }
        else if(SwaybarPullCtrlStage == 0x02)
        {
            Swaybar_DrvControl(Swaybar_Drv_Pull0);
        }
        else 
        {
            // go on
        }
    }
}


//=================================================================================================
// [Function Name]void Swaybar_StuckCheck(void)
// [Parameters] None
// [Introduction] Pull Link Stuck Check 
// [Return value] None
// [Creator] 20240403 zzy
//=================================================================================================
static void Swaybar_StuckCheck(void)
{
    static uint16_t StuckLinkTm = 0x0000;
    static uint16_t StuckPullTm = 0x0000;

    static uint8_t StuckLinkCnt = 0x00;
    static uint8_t StuckPullCnt = 0x00;

    static uint8_t StuckLinkTabCnt = 0x00;
    static uint8_t StuckLinkTab[Swaybar_Stuck_TabSize] = {0x00};
    static uint8_t StuckPullTabCnt = 0x00;
    static uint8_t StuckPullTab[Swaybar_Stuck_TabSize] = {0x00};

    uint8_t i = 0x00;

    // Link Stuck Check
    if(TravelSensorisLink)
    {
        StuckLinkTm = 0x0000;
        StuckLinkCnt = 0x00;
        StuckLinkTabCnt = 0;
        for(i=0;i<Swaybar_Stuck_TabSize;i++)
        {
            StuckLinkTab[i] = 0x00;
        }

        StuckLink = 0;

        FrontSwaybarSts = 0x01; // Connected
    }
    else if(StuckLink == 0x01)
    {
        //error occurrence
    }
    else if(DriverState == Swaybar_Drv_Link)
    {
        if(StuckLinkTm < Swaybar_Stuck_LinkDelayTm)
        {
            // Tm < Swaybar_Stuck_LinkDelayTm No Error
            StuckLinkTm ++;
        }
        else
        {
            StuckLinkTm = Swaybar_Stuck_LinkDelayTm;

            // Stuck Checking
            if(StuckLinkTabCnt < Swaybar_Stuck_TabSize)
            {
                StuckLinkTab[StuckLinkTabCnt] = IO_Sts.PWM_DutyCycle;
                StuckLinkTabCnt ++;
            }
            else if(((StuckLinkTab[0] >= StuckLinkTab[Swaybar_Stuck_TabSize-1]) && 
                    (StuckLinkTab[0] - StuckLinkTab[Swaybar_Stuck_TabSize-1] < Swaybar_Stuck_Distance)) ||
                (StuckLinkTab[0] < StuckLinkTab[Swaybar_Stuck_TabSize-1]))
            {
                Swaybar_DrvControl(Swaybar_Drv_StuckLink0);
                StuckLinkTm = 0;
            }
            else
            {
                for(i = 1; i < Swaybar_Stuck_TabSize; i++)
                {
                    StuckLinkTab[i - 1] = StuckLinkTab[i];
                }
                StuckLinkTab[Swaybar_Stuck_TabSize-1] = IO_Sts.PWM_DutyCycle;
            }
        }
    }
    else if((DriverState == Swaybar_Drv_StuckLink0) ||
        (DriverState == Swaybar_Drv_StuckLink1))
    {
        // Flash
        if(StuckLinkTm < Swaybar_Stuck_FlashTm)
        {
            StuckLinkTm ++;
            Swaybar_DrvControl(Swaybar_Drv_StuckLink0);
        }
        else if(StuckLinkTm < (Swaybar_Stuck_FlashTm * 2 - 1))
        {
            StuckLinkTm ++;
            Swaybar_DrvControl(Swaybar_Drv_StuckLink1);
        }
        else
        {
            StuckLinkTm = 0;

            if(StuckLinkCnt >= (Swaybar_Stuck_FlashCnt - 1))
            {
                // Error
                StuckLink = 1;
                Swaybar_DrvControl(Swaybar_Drv_Link);

                StuckLinkTm = 0x0000;
                StuckLinkCnt = 0x00;
                StuckLinkTabCnt = 0;
                for(i=0;i<Swaybar_Stuck_TabSize;i++)
                {
                    StuckLinkTab[i] = 0x00;
                }
            }
            else
            {
                StuckLinkCnt ++;
            }
        }
    }

    // Pull Stuck Check
    if(TravelSensorisPull)
    {
        StuckPullTm = 0x0000;
        StuckPullCnt = 0x00;
        StuckPullTabCnt = 0x00;
        for(i=0;i<Swaybar_Stuck_TabSize;i++)
        {
            StuckLinkTab[i] = 0x00;
        }

        StuckPull = 0;

        FrontSwaybarSts = 0x00; // Disconnected
    }
    else if(SwaybarPullCtrlStage == 0x02)
    {
        if((DriverState == Swaybar_Drv_Pull0) ||
            (DriverState == Swaybar_Drv_Pull1) ||
            (DriverState == Swaybar_Drv_Pull2))
        {
            // Pull Stuck0: Stuck Checking
            if(StuckPullTabCnt < Swaybar_Stuck_TabSize)
            {
                StuckPullTab[StuckPullTabCnt] = IO_Sts.PWM_DutyCycle;
                StuckPullTabCnt ++;
            }
            else if(((StuckPullTab[Swaybar_Stuck_TabSize-1] >= StuckPullTab[0]) && 
                    (StuckPullTab[Swaybar_Stuck_TabSize-1] - StuckPullTab[0] < Swaybar_Stuck_Distance)) ||
                (StuckPullTab[Swaybar_Stuck_TabSize-1] < StuckPullTab[0]))
            {
                Swaybar_DrvControl(Swaybar_Drv_Pull3);
                StuckPullTm = 0;
            }
            else
            {
                for(i = 1; i < Swaybar_Stuck_TabSize; i++)
                {
                    StuckPullTab[i - 1] = StuckPullTab[i];
                }
                StuckPullTab[Swaybar_Stuck_TabSize-1] = IO_Sts.PWM_DutyCycle;
            }
        }
        // Pull Stuck1: Volt change to Pull3
        else if(DriverState == Swaybar_Drv_Pull3)
        {
            if(StuckPullTm < Swaybar_Stuck_Pull3Tm)
            {
                // wait Driver
                StuckPullTm ++;
            }
            else
            {
                StuckPullTm = 0;
                Swaybar_DrvControl(Swaybar_Drv_StuckPull0);
                StuckPullCnt = 0;
            }
        }
        // Pull Stuck2: Volt change to flash
        else if((DriverState == Swaybar_Drv_StuckPull0) ||  
            (DriverState == Swaybar_Drv_StuckPull1))
        {
            if(StuckPullTm < Swaybar_Stuck_FlashTm)
            {
                StuckPullTm ++;
                Swaybar_DrvControl(Swaybar_Drv_StuckPull0);
            }
            else if(StuckPullTm < (Swaybar_Stuck_FlashTm * 2 - 1))
            {
                StuckPullTm ++;
                Swaybar_DrvControl(Swaybar_Drv_StuckPull1);
            }
            else
            {
                StuckPullTm = 0;

                if(StuckPullCnt >= (Swaybar_Stuck_FlashCnt - 1))
                {
                    // Pull Stuck3: Set Error, Volt change to Link;
                    StuckPull = 1;

                    DRV_SW_LED_OFF();
                    LED0_OFF();

                    Swaybar_DrvControl(Swaybar_Drv_Link);

                    StuckPullTm = 0x0000;
                    StuckPullCnt = 0x00;
                    StuckPullTabCnt = 0x00;
                    for(i=0;i<Swaybar_Stuck_TabSize;i++)
                    {
                        StuckLinkTab[i] = 0x00;
                    }
                }
                else
                {
                    StuckPullCnt ++;
                }
            }
        }
    }
}


//=================================================================================================
// [Function Name]void Swaybar_ErrorCheck(void)
// [Parameters] None
// [Introduction] Swaybar Error Check
// [Return value] None
// [Creator] 20240521 zzy
//=================================================================================================
static void Swaybar_ErrorCheck(void)
{
    if(DTC_Judge.Battery_Voltage_Abnormal_Low.Flag == 0x01)
    {
        FronSwaybarFault |= SwaybarError_VoltLow;
    }
    else
    {
        FronSwaybarFault &= ~SwaybarError_VoltLow;
    }

    if(DTC_Judge.Battery_Voltage_Abnormal_High.Flag == 0x01)
    {
        FronSwaybarFault |= SwaybarError_VoltHi;
    }
    else
    {
        FronSwaybarFault &= ~SwaybarError_VoltHi;
    }

    if((CAN_Rcv.TCU_3.Timeout.Timeout_Flag == 0x01) ||
        (CAN_Rcv.ABS_3.Timeout.Timeout_Flag == 0x01) ||
        (CAN_Rcv.AWD_1.Timeout.Timeout_Flag == 0x01))
    {
        FronSwaybarFault |= SwaybarError_CANTimeout;
    }
    else
    {
        FronSwaybarFault &= ~SwaybarError_CANTimeout;
    }

    if(DTC_Judge.Travel_Sensor_Unstable.Flag == 0x01)
    {
        FronSwaybarFault |= SwaybarError_TravelSensor;
    }
    else
    {
        FronSwaybarFault &= ~SwaybarError_TravelSensor;
    }

    if(DTC_Judge.Driver_Voltage_Abnormality.Flag == 0x01)
    {
        FronSwaybarFault |= SwaybarError_DrvVoltErr;
    }
    else
    {
        FronSwaybarFault &= ~SwaybarError_DrvVoltErr;
    }

    if(DTC_Judge.Driver_Open_Circuit.Flag == 0x01)
    {
        FronSwaybarFault |= SwaybarError_DrvOpen;
    }
    else
    {
        FronSwaybarFault &= ~SwaybarError_DrvOpen;
    }

    if((StuckLink == 0x01) || (StuckPull == 0x01))
    {
        FronSwaybarFault |= SwaybarError_DrvStuck;
    }
    else
    {
        FronSwaybarFault &= ~SwaybarError_DrvStuck;
    }

    if(DTC_Judge.Driver_Overcurrent.Flag == 0x01)
    {
        FronSwaybarFault |= SwaybarError_DrvOverCurrent;
    }
    else
    {
        FronSwaybarFault &= ~SwaybarError_DrvOverCurrent;
    }

    if(DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Flag == 0x01)
    {
        FronSwaybarFault |= SwaybarError_BusOff;
    }
    else
    {
        FronSwaybarFault &= ~SwaybarError_BusOff;
    }

#if Swaybar_DebugEN
    FronSwaybarFault = FronSwaybarFault & 0x000000CF;
#endif

    if(FronSwaybarFault != 0x00000000)
    {
        FrontSwaybarSts = 0x04; // Fault_Unknown state

        SwaybarPullCtrlStage = 0x00;
        SwaybarSwSts = 0x02;
        FrontSwaybarSW = 0x02;
        SwaybarPullEn = 0x00;
    }
}


//=================================================================================================
// [Function Name] void Swaybar_LampControl(void)
// [Parameters] None
// [Introduction] 0x388 FrontSwaybarGreenLamp FrontSwaybarYellowLamp
// [Return value] None
// [Creator] 20240527 zzy
//=================================================================================================
static void Swaybar_LampControl(void)
{
    // GreenLamp Swaybar Travel
    if(TravelSensorisLink)
    {
        // 0x0:OFF
        FrontSwaybarGreenLamp = 0x00;
    }
    else if(TravelSensorisPull)
    {
        // 0x1:ON
        FrontSwaybarGreenLamp = 0x01;
    }
    else if(TravelSensorisOther)
    {
        // 0x2:flashing
        FrontSwaybarGreenLamp = 0x02;
    }
    else 
    {
        // 0x3:Not used
    }

    // YellowLamp Swaybar Error Sts
    if(FronSwaybarFault == 0x00000000)
    {
        // 0x0:OFF
        // No Error
        FrontSwaybarYellowLamp = 0x00;
    }
    else if((FronSwaybarFault & 0x0000000F) != 0x00000000)
    {
        // 0x1:ON
        // Common Error
        FrontSwaybarYellowLamp = 0x01;
    }
    else
    {
        // 0x2:flashing
        // Serious Error
        FrontSwaybarYellowLamp = 0x02;
    }
}


//=================================================================================================
// [Function Name]void Swaybar_StsNotice(void)
// [Parameters] None
// [Introduction] 0x388 Swaybar_StatusNotice restore default values
// [Return value] None
// [Creator] 20240402 zzy
//=================================================================================================
static void Swaybar_StsNotice(void)
{
    static ubyte ProStatusNotice = 0;
    static uword NoticeTimeCnt = 0;

    if((StatusNotice != ProStatusNotice) && 
        (StatusNotice != 0x00))
    {
        NoticeTimeCnt = 0x01;
    }

    if(NoticeTimeCnt > 0)
    {
        if(NoticeTimeCnt > Swaybar_StsNoticeTm)//5s
        {
            StatusNotice = 0;
        }

        NoticeTimeCnt ++;
    }

    ProStatusNotice = StatusNotice;
}


//=================================================================================================
// [Function Name]void Swaybar_Init(void)
// [Parameters] None
// [Introduction] Swaybar Init
// [Return value] None
// [Creator] 20240402 zzy
//=================================================================================================
void Swaybar_Init(void)
{
    uword * puwLen = NULL;
    ubyte Data[20] = {0};

    (void)ReadParaFromFlash(puwLen, Data, FLASH_OFFSET_ADDR_SWSTS, 1);

    if((Data[3] == 0x01) && (CAN_Rcv.AWD_1.AWD_SystemOperatingMode == 0x06))
    {
        SwaybarSwSts = 0x01;
        FrontSwaybarSW = 0x01;
    }
    else
    {
        SwaybarSwSts = 0x00;
        FrontSwaybarSW = 0x02;
    }

    StatusNotice = 0x00;

    CAN_Tx.Swaybar_1.BYTE0.bits.Swaybar_FrontSwaybarGreenLamp = 0x00;
    CAN_Tx.Swaybar_1.BYTE0.bits.Swaybar_FrontSwaybarSts = 0x00;
    CAN_Tx.Swaybar_1.BYTE0.bits.Swaybar_FrontSwaybarYellowLamp = 0x00;
    CAN_Tx.Swaybar_1.BYTE1.bits.Swaybar_FrontSwaybarSW = FrontSwaybarSW;
    CAN_Tx.Swaybar_1.Swaybar_FronSwaybarFault = 0x00000000;
    CAN_Tx.Swaybar_1.BYTE5.bits.Swaybar_StatusNotice = 0x00;
}


//=================================================================================================
// [Function Name]void Swaybar_Mainfunction(void)
// [Parameters] None
// [Introduction] Swaybar cycle tasks
// [Return value] None
// [Creator] 20240517 zzy
//=================================================================================================




void Swaybar_Mainfunction(void)
{
    static uint8_t PreSwaybarIO = 0x00;
    static uint8_t Flag4LPull = 0x00;
    uint16_t puwLen = 0x00;
    uint8_t aubUdsData[3] = {0x00};

    /*---------------------------------------------------------------------------------------------*/
    // check Swaybar Sw Sts
#if Swaybar_DebugEN
    IO_Input_Status.SwaybarSW.value = CAN_Rcv.DebugMessage[0];
#endif

    if((IO_Input_Status.SwaybarSW.value == 0x01) &&
        (PreSwaybarIO == 0x00))
    {
        if((SwaybarSwSts == 0x00) || 
            (SwaybarSwSts == 0x02))
        {
            SwaybarSwSts = 0x01;
            FrontSwaybarSW = 0x01;

            SwaybarPullCtrlStage = 0x00;
        }
        else
        {
            SwaybarSwSts = 0x02;
            FrontSwaybarSW = 0x02;
        }
    }
    PreSwaybarIO = IO_Input_Status.SwaybarSW.value;
    /*---------------------------------------------------------------------------------------------*/


    /*---------------------------------------------------------------------------------------------*/
    // Swaybar Pull check
    if(SwaybarSwSts == 0x01)
    {
        if((CAN_Rcv.ABS_3.ABS_VehicleSpeed < (23 * 160 / 9)) &&
            // 0x3:Mode 4H Lock;0x6:Mode 4L Lock
            ((CAN_Rcv.AWD_1.AWD_SystemOperatingMode == 0x03) || (CAN_Rcv.AWD_1.AWD_SystemOperatingMode == 0x06)) &&
            // No Error or Just StuckPull Error
            ((FronSwaybarFault == 0x0000) || ((SwaybarError_DrvStuck == FronSwaybarFault) && (StuckPull == 0x01))))
        {
            DRV_SW_LED_ON();
            LED0_ON();

            SwaybarPullEn = 0x01;

            // Memory 4L causes Pull
            if((CAN_Rcv.AWD_1.AWD_SystemOperatingMode == 0x06) &&
                (Flag4LPull == 0x00))
            {
                Flag4LPull = 0x01;
                aubUdsData[2] = 0x01;
                (void)WriteParaToFlash(&puwLen, aubUdsData, FLASH_OFFSET_ADDR_SWSTS, 1);
            }
        }
        else
        {
            // If the separation button is pressed and other 
            // reasons are not checked, the init status will 
            // be restored
            SwaybarSwSts = 0x00;
            FrontSwaybarSW = 0x02;

            if(((0x0000 != FronSwaybarFault) && (SwaybarError_DrvStuck != FronSwaybarFault)) || 
                ((SwaybarError_DrvStuck == FronSwaybarFault) && (StuckLink == 0x00)))
            {
                // FronSwaybarFault not ok
                StatusNotice = 0x03;
            }
            else if(CAN_Rcv.ABS_3.ABS_VehicleSpeed >= (23 * 160 / 9))
            {
                // ABS_VehicleSpeed not ok
                StatusNotice = 0x02;
            }
            else
            {
                // AWD_SystemOperatingMode not ok
                StatusNotice = 0x01;
            }
        }
    }

    // Swaybar Linking check
    if((FronSwaybarFault == SwaybarError_DrvStuck) &&
        (StuckPull == 0x01))
    {
        // allow Pull
    }
    else if((SwaybarSwSts == 0x02) ||
        (CAN_Rcv.ABS_3.ABS_VehicleSpeed >= (29 * 160 / 9)) ||
        // 0x2:Mode 2H Lock
        (CAN_Rcv.AWD_1.AWD_SystemOperatingMode == 0x02) || 
        // 0x5:Mode Neutral
        (CAN_Rcv.AWD_1.AWD_SystemOperatingMode == 0x05) ||
        (FronSwaybarFault != 0x0000))
    {
        DRV_SW_LED_OFF();
        LED0_OFF();

        if((DriverState != Swaybar_Drv_StuckLink0) &&
            (DriverState != Swaybar_Drv_StuckLink1))
        {
            Swaybar_DrvControl(Swaybar_Drv_Link);
        }

        FrontSwaybarSts = 0x03; // Connecting
        SwaybarPullEn = 0x00;

        // clear Memory 4L causes Pull
        if(Flag4LPull == 0x01)
        {
            Flag4LPull = 0x00;
            aubUdsData[2] = 0x00;
            (void)WriteParaToFlash(&puwLen, aubUdsData, FLASH_OFFSET_ADDR_SWSTS, 1);
        }
    }

    if(SwaybarPullEn)
    {
        Swaybar_DrvControlPull();
    }
    /*---------------------------------------------------------------------------------------------*/


    /*---------------------------------------------------------------------------------------------*/
    Swaybar_StuckCheck();
    Swaybar_ErrorCheck();
    Swaybar_LampControl();
    Swaybar_StsNotice();

    // Empty command,used to check if the Swaybar is controlled by Diag
    Swaybar_DrvControl(Swaybar_Drv_DiagCheck);

    // CAN Message 0x388
    CAN_Tx.Swaybar_1.BYTE0.bits.Swaybar_FrontSwaybarSts = FrontSwaybarSts;
    CAN_Tx.Swaybar_1.BYTE0.bits.Swaybar_FrontSwaybarGreenLamp = FrontSwaybarGreenLamp;
    CAN_Tx.Swaybar_1.BYTE0.bits.Swaybar_FrontSwaybarYellowLamp = FrontSwaybarYellowLamp;
    CAN_Tx.Swaybar_1.BYTE1.bits.Swaybar_FrontSwaybarSW = FrontSwaybarSW;
    CAN_Tx.Swaybar_1.Swaybar_FronSwaybarFault = FronSwaybarFault; /*BYT2 - BYTE4*/
    CAN_Tx.Swaybar_1.BYTE5.bits.Swaybar_StatusNotice = StatusNotice;
    /*---------------------------------------------------------------------------------------------*/
}

#endif

