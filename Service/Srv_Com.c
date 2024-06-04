#ifndef SRV_COM_C
#define SRV_COM_C

/* include files */
#include "main.h"

CAN_RCV CAN_Rcv;
CAN_TX CAN_Tx;
ubyte Vehicle_VIN[17] = {0};
ubyte Station_VIN_D[17] = {0x55,0xAA,0xAA,0x55,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16};
ubyte Vehicle_BAT_SN[24] = {0};
ubyte TimeOut_Counter = 0;
//Prohibit application message sending when BusOff occurs

extern can_mailbox_descriptor_struct g_transmit_message;

static void RcvTimeoutDeal(
    RCV_TIMEOUT *Timeout, 
    uword Timeout_Max, 
    void(*Pfun)(void));
static void msg_TCU_3_Timeout(void);
static void msg_ABS_3_Timeout(void);
static void msg_AWD_1_Timeout(void);

ubyte CheckSum(ubyte *data, uword length);
void SrvComInit(void);
void SrvComStep(void);
void BCanDebugSend(
    ulong ulID,
    ubyte byte0,ubyte byte1,ubyte byte2,ubyte byte3,
    ubyte byte4,ubyte byte5,ubyte byte6,ubyte byte7);

//=================================================================================================
// [Function Name] ubyte CheckSum(ubyte *data, uword length)
// [Parameters] data, length
// [Introduction] CRC Polynom：x8 + x4 + x3 + x2 + 1
//                The Value of the POLY is 0x1D
//                The initial value checksum signal is 0xFF
// [Return value] CheckSum value
// [Creator] 2024-03-21 zzy
//=================================================================================================
ubyte CheckSum(ubyte *data, uword length)
{
    ubyte i;
    // Initial value
    ubyte crc = 0xFF;
    while(length--)
    {
        crc ^= *data++;
        for ( i = 0; i < 8; i++ )
        {
            if ( crc & 0x80 )
                crc = (crc << 1) ^ 0x1D;
            else
                crc <<= 1;
        }
    }

    return (crc^0xFF);
}


//=================================================================================================
// [Function Name] void SrvComInit(void)
// [Parameters] None
// [Introduction]
// [Return value] None
// [Creator] 2018-08-21 ZYW
//=================================================================================================
void SrvComInit(void)
{
    ubyte buf[20];
    ushort *puwLen = 0;
    ubyte error = 0;

    (void) memset((void *)&CAN_Rcv, 0, sizeof(CAN_RCV));

    error = ReadParaFromFlash(puwLen, buf, FLASH_OFFSET_ADDR_VIN, 17);
    if(error == 0)
    {
        memcpy(Vehicle_VIN, &buf[3], 17);
    }
}


//=================================================================================================
// [Function Name] void SrvComStep(void)
// [Parameters] None
// [Introduction] Periodic scheduling
// [Return value] None
// [Creator] 2018-08-21 ZYW
//=================================================================================================
void SrvComStep(void)
{
    ubyte Data[8];
    stCAN_SWObj  stCAN_SWObj_Tx;
    static uint8_t Pcan_Swaybar_1_Send_Count = PCAN_Swaybar_SEND_PERIOD - 1;

    // ----------------------------------------------------------------------
    // CAN Structure ---> CAN BUS
    // ----------------------------------------------------------------------
    if(CAN0BusOffSts == 0x01)
    {
        Pcan_Swaybar_1_Send_Count = PCAN_Swaybar_SEND_PERIOD - 1;
    }
    else
    {
        Pcan_Swaybar_1_Send_Count++;

        if(Pcan_Swaybar_1_Send_Count >= PCAN_Swaybar_SEND_PERIOD)
        {
            Pcan_Swaybar_1_Send_Count = 0;

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

            CAN_Tx.Swaybar_1.BYTE6.bits.Swaybar_1_RollingCounter ++;
        }
    }

    // ----------------------------------------------------------------------
    // CAN Timeout
    // ----------------------------------------------------------------------
    RcvTimeoutDeal(&CAN_Rcv.TCU_3.Timeout, 25, msg_TCU_3_Timeout);
    RcvTimeoutDeal(&CAN_Rcv.ABS_3.Timeout, 50, msg_ABS_3_Timeout);
    RcvTimeoutDeal(&CAN_Rcv.AWD_1.Timeout, 50, msg_AWD_1_Timeout);
}


//=================================================================================================
// [Function Name] RcvTimeoutDeal
// [Parameters] Timeout Timeout_Max Pfun
// [Introduction]
// [Return value] None
// [Creator] 2019-12-06 ZYW
//=================================================================================================
static void RcvTimeoutDeal(RCV_TIMEOUT *Timeout, uword Timeout_Max, void(*Pfun)(void))
{
    if(Timeout->Rcv_Flag == 1)
    {
        Timeout->Rcv_Flag = 0;
        Timeout->Count = 0;
        Timeout->Timeout_Flag = 0;
    }
    else
    {
        if(Timeout->Count < Timeout_Max)
        {
            Timeout->Count++;
        }
        else
        {
            Timeout->Timeout_Flag = 1;
            Timeout->Count = 0;
            Pfun();
        }
    }
}


//=================================================================================================
// [Function Name] msg_TCU_3_Timeout
// [Parameters] None
// [Introduction] Signal loss processing
// [Return value] None
// [Creator] 20240328 zzy
//=================================================================================================
static void msg_TCU_3_Timeout(void)
{
    CAN_Rcv.TCU_3.TCU_Drivemode = 0x00;
}


//=================================================================================================
// [Function Name] msg_ABS_3_Timeout
// [Parameters] None
// [Introduction] Signal loss processing
// [Return value] None
// [Creator] 20240328 zzy
//=================================================================================================
static void msg_ABS_3_Timeout(void)
{
    CAN_Rcv.ABS_3.ABS_VehicleSpeedValid = 0x01;
    CAN_Rcv.ABS_3.ABS_VehicleSpeed = 0x0000;
}


//=================================================================================================
// [Function Name] msg_AWD_1_Timeout
// [Parameters] None
// [Introduction] Signal loss processing
// [Return value] None
// [Creator] 20240328 zzy
//=================================================================================================
static void msg_AWD_1_Timeout(void)
{
    CAN_Rcv.AWD_1.AWD_SystemOperatingMode = 0x00;
}


//=================================================================================================
// [Function Name] BCanDebugSend
// [Parameters]
// [Introduction] CAN0 Send debugging message 
// [Return value] none
// [Creator] 2018-08-24 ZYW
//=================================================================================================
void BCanDebugSend(
    ulong ulID,
    ubyte byte0,ubyte byte1,ubyte byte2,ubyte byte3,
    ubyte byte4,ubyte byte5,ubyte byte6,ubyte byte7)
{
    stCAN_SWObj  stCAN_SWObj_Tx;

    stCAN_SWObj_Tx.ubData[0] = byte0;
    stCAN_SWObj_Tx.ubData[1] = byte1;
    stCAN_SWObj_Tx.ubData[2] = byte2;
    stCAN_SWObj_Tx.ubData[3] = byte3;
    stCAN_SWObj_Tx.ubData[4] = byte4;
    stCAN_SWObj_Tx.ubData[5] = byte5;
    stCAN_SWObj_Tx.ubData[6] = byte6;
    stCAN_SWObj_Tx.ubData[7] = byte7;

    g_transmit_message.data = (uint32_t *)(stCAN_SWObj_Tx.ubData);
    g_transmit_message.code = CAN_MB_TX_STATUS_DATA;
    g_transmit_message.id = ulID;
    can_mailbox_config(CAN0, 30, &g_transmit_message);
}


#endif
