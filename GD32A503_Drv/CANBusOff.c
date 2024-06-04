#ifndef _CANBUSOFF_C_
#define _CANBUSOFF_C_

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "CANBusOff.h"

#define tBusOffRecoveryL1               ((50U) / Autosar_NM_ticks_10ms)
#define tBusOffRecoveryL2               ((200U) / Autosar_NM_ticks_10ms)
#define Autosar_NM_ticks_10ms           (10U)

#define BusOff_counter_L1_to_L2         (10U)
#define BusOff_Counter_DTCFlag          (10U)

extern can_mailbox_descriptor_struct g_transmit_message;

ubyte CAN0BusOffSts = 0;
ubyte CAN1BusOffSts = 0;
ubyte CAN0BusOffDTCFlag = 0;

static void SetBcanTxen0(void);

void BusOffRecovery(void);
void Can_GetBusOffStatus(void);

//=================================================================================================
// [Function Name] BusOffRecovery
// [Parameters] 
// [Introduction]
// [Return value] None
// [Creator] 20240409 zzy
//=================================================================================================
void BusOffRecovery(void)
{ 
    static ubyte PreCAN0BusOffFlag = 0x00;
    static uword BusOffTime = 0x0000;
    static ubyte BusOffCnt_L1 = 0x00;
    static ubyte BusOffCnt_L2 = 0x00;

    // init 
    if((PreCAN0BusOffFlag == 0) &&
        (CAN0BusOffSts == 1))
    {
        BusOffTime = 0;
        BusOffCnt_L1 = 0;
        BusOffCnt_L2 = 0;
        CAN0BusOffDTCFlag = 0;
    }
    PreCAN0BusOffFlag = CAN0BusOffSts;

    if(CAN0BusOffSts == 1)
    {
        if(BusOffCnt_L1 <= BusOff_counter_L1_to_L2)
        {
            if(BusOffTime <= tBusOffRecoveryL1)
            {
                BusOffTime ++;
            }
            else
            {
                BusOffCnt_L1 ++;
                BusOffTime = 0;
                SetBcanTxen0();
            }
        }
        else
        {
            if(BusOffTime <= tBusOffRecoveryL2)
            {
                BusOffTime ++;
            }
            else
            {
                BusOffCnt_L2 ++;
                BusOffTime = 0;
                SetBcanTxen0();

                DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Count ++;
            }
        }
    }
    else
    {
        BusOffTime = 0;
        BusOffCnt_L1 = 0;
        BusOffCnt_L2 = 0;
        CAN0BusOffDTCFlag = 0;
        DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Count = 0;
    }


    // DTC
    if(BusOffCnt_L2 > BusOff_Counter_DTCFlag)
    {
        CAN0BusOffDTCFlag = 1;
    }
}


//=================================================================================================
// [Function Name] static void SetBcanTxen0(void)
// [Parameters]
// [Introduction]
// [Return value] None
// [Creator] 2023-12-25 ZZY
//=================================================================================================
static void SetBcanTxen0(void)
{
    ubyte Data[8];
    stCAN_SWObj  stCAN_SWObj_Tx;

    CAN_Tx.Swaybar_1.BYTE6.bits.Swaybar_1_RollingCounter = 0;

    Data[0] = CAN_Tx.Swaybar_1.BYTE0.Byte;
    Data[1] = CAN_Tx.Swaybar_1.BYTE1.Byte;
    Data[2] = (ubyte)((CAN_Tx.Swaybar_1.Swaybar_FronSwaybarFault >> 16) & 0x000000ff);
    Data[3] = (ubyte)((CAN_Tx.Swaybar_1.Swaybar_FronSwaybarFault >> 8) & 0x000000ff);
    Data[4] = (ubyte)(CAN_Tx.Swaybar_1.Swaybar_FronSwaybarFault & 0x000000ff);
    Data[5] = CAN_Tx.Swaybar_1.BYTE5.Byte;
    Data[6] = CAN_Tx.Swaybar_1.BYTE6.Byte;

    CAN_Tx.Swaybar_1.BYTE7.bits.Swaybar_1_Checksum = CheckSum(Data, 7);

    stCAN_SWObj_Tx.ubData[0] = Data[0];
    stCAN_SWObj_Tx.ubData[1] = Data[1];
    stCAN_SWObj_Tx.ubData[2] = Data[2];
    stCAN_SWObj_Tx.ubData[3] = Data[3];
    stCAN_SWObj_Tx.ubData[4] = Data[4];
    stCAN_SWObj_Tx.ubData[5] = Data[5];
    stCAN_SWObj_Tx.ubData[6] = Data[6];
    stCAN_SWObj_Tx.ubData[7] = CAN_Tx.Swaybar_1.BYTE7.Byte;
    g_transmit_message.data = (uint32_t *)(stCAN_SWObj_Tx.ubData);
    g_transmit_message.code = CAN_MB_TX_STATUS_DATA;
    g_transmit_message.id = 0x388;
    can_mailbox_config(CAN0, 6, &g_transmit_message);
}


//=================================================================================================
// [Function Name] void ResetBcanTxen0(void)
// [Parameters] Reset CAN drive
// [Introduction]
// [Return value] None
// [Creator] 2023-12-25 ZZY
//=================================================================================================
void ResetBcanTxen0(void)
{
    //Reset CAN drive,Clear CAN send buffer
    fwdgt_counter_reload();
    CAN_CAN0Init();
    fwdgt_counter_reload();
}


//=================================================================================================
// [Function Name] void Can_GetBusOffStatus(void)
// [Parameters] Reset CAN drive
// [Introduction]
// [Return value] None
// [Creator] 2023-12-25 ZZY
//=================================================================================================
void Can_GetBusOffStatus(void)
{
    if (can_error_state_get(CAN0) == 2)
    {
        ResetBcanTxen0();
        CAN0BusOffSts = 1;
    }
}


#endif
