#ifndef SERVICEDRV_H
#define SERVICEDRV_H

/*Include files*/
#include  "XC22xxNREGS.h"

#define IO_INPUT_FILTER_NUM 10
#define IO_INPUT_FILTER_READY_TIME (IO_INPUT_FILTER_NUM+3)
#define WIPER_INT_IO_INPUT_FILTER_READY_TIME 4

#define PCAN_Swaybar_SEND_PERIOD 10
#define BCAN_HS_VER_SEND_PERIOD 100

typedef struct
{
    ubyte   count;
    ubyte   value;
}IO_FILTER_C;

typedef struct
{
    IO_FILTER_C SwaybarSW;
    IO_FILTER_C Reserved2;
    ubyte Init_Ready                       :1;
}IO_INPUT_STATUS;

extern IO_INPUT_STATUS           IO_Input_Status;

typedef struct
{
    ubyte Ad_Voltage;
    ubyte Ad_DrvUpOut;
    ubyte Ad_DrvDownOut;
    ubyte Ad_DrvOutput;
    uword Ad_DrvOutputCnrrent;
    uword Ad_BoardTemp;
    uword PWM_Frequency;
    ubyte PWM_DutyCycle;
}IO_Sts_Type;
extern IO_Sts_Type IO_Sts;

typedef struct
{
    ubyte Timeout_Flag;
    ubyte Rcv_Flag;
    uword Count;
}RCV_TIMEOUT;

typedef struct
{
    ubyte TCU_Drivemode;
    ubyte TCU_3_RollingCounter;
    ubyte TCU_3_Checksum;
    RCV_TIMEOUT Timeout;
}mag_TCU_3;

typedef struct
{
    ubyte ABS_VehicleSpeedValid;
    uword ABS_VehicleSpeed;
    ubyte ABS_3_RollingCounter;
    ubyte ABS_3_Checksum;
    RCV_TIMEOUT Timeout;
}mag_ABS_3;

typedef struct
{
    ubyte AWD_SystemOperatingMode;
    ubyte AWD_1_RollingCounter;
    ubyte AWD_1_Checksum;
    RCV_TIMEOUT Timeout;
}mag_AWD_1;

typedef struct
{
    mag_TCU_3 TCU_3;
    mag_ABS_3 ABS_3;
    mag_AWD_1 AWD_1;
    ubyte DebugMessage[4];
}CAN_RCV;
extern CAN_RCV CAN_Rcv;

typedef struct
{
    union
    {
        struct
        {
            ubyte     Swaybar_FrontSwaybarSts             :3;
            ubyte     R0                                  :1;
            ubyte     Swaybar_FrontSwaybarGreenLamp       :2;
            ubyte     Swaybar_FrontSwaybarYellowLamp      :2;
        }bits;
        ubyte     Byte;
    }BYTE0;

    union
    {
        struct
        {
            ubyte     R0                                  :4;
            ubyte     Swaybar_FrontSwaybarSW              :2;
            ubyte     R1                                  :2;
        }bits;
        ubyte     Byte;
    }BYTE1;

    /*BYT2 - BYTE4*/
    ulong Swaybar_FronSwaybarFault;

    union
    {
        struct
        {
            ubyte     Swaybar_StatusNotice                :4;
            ubyte     R0                                  :4;
        }bits;
        ubyte     Byte;
    }BYTE5;

    union
    {
        struct
        {
            ubyte     Swaybar_1_RollingCounter            :4;
            ubyte     R0                                  :4;
        }bits;
        ubyte     Byte;
    }BYTE6;

    union
    {
        struct
        {
            ubyte     Swaybar_1_Checksum                  :8;
        }bits;
        ubyte     Byte;
    }BYTE7;
}msg_Swaybar_1;

typedef struct
{
    msg_Swaybar_1 Swaybar_1;
}CAN_TX;
extern CAN_TX CAN_Tx;

extern ubyte Vehicle_VIN[17];
extern ubyte Vehicle_BAT_SN[24];

extern void GetDigtalIOState(void);
extern void SrvIO_Mainfunction(void);
extern ubyte CheckSum(ubyte *data, uword length);
extern void BCanDebugSend(
    ulong ulID,
    ubyte byte0,ubyte byte1,ubyte byte2,ubyte byte3,
    ubyte byte4,ubyte byte5,ubyte byte6,ubyte byte7);
extern void SrvIOInit(void);
extern void SrvComInit(void);
extern void SrvComStep(void);

#endif	/* end of header file */
