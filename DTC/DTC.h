
#ifndef DTC_H_
#define DTC_H_

#define DTC_NUM                         11
#if DTC_NUM > 60
    #error MAX IS 60
#endif
#define SNAP_SHOT                       0
#define WAKEUP_CYCLE_TIMES              40

#define CAN_TIMEOUT_TIMES               300  //3s
#define DTC_POWERON_COUNT               300  //3s
#define DTC_VOLTAGE_COUNT_MAX           50   //500ms
#define INVALID_DLC_NUM                 10
#define DTC_LOOP_IG
#define DTC_RCV_MSG_TIMEOUT_VALUE_SAS         25
#define DTC_RCV_MSG_TIMEOUT_VALUE_ESC         25
#define DTC_RCV_MSG_TIMEOUT_VALUE_BCM         50
#define DTC_RCV_MSG_TIMEOUT_VALUE_BMSH1       50
#define DTC_RCV_MSG_TIMEOUT_VALUE_BMSH2       250
#define DTC_RCV_MSG_TIMEOUT_VALUE_BMSH3       125
#define DTC_RCV_MSG_TIMEOUT_VALUE_VCU         25
#define DTC_RCV_MSG_TIMEOUT_VALUE_IPK         250

typedef struct
{
    union
    {
        struct
        {
            ubyte Test_Failed              :1;
            ubyte R0                       :1;
            ubyte Pending_DTC              :1;
            ubyte Confirmed_DTC            :1;
            ubyte R1                       :4;
        }bits;
        ubyte byte;
    }DTC_ST;

    union
    {
        struct
        {
            ubyte Cycle_Times              :6;
            ubyte Last_Cycle_Error_Flag    :1;
            ubyte Wakeup_Flag              :1;
        }bits;
        ubyte byte;
    }Status;

    ubyte FaultOccurentCounter;
#if SNAP_SHOT
    ubyte SnapshotRecord[6];
#endif
}DTC_OBJECT;
extern DTC_OBJECT DTC_Object[DTC_NUM];

typedef struct
{
    ubyte Flag;
    uword Count;
}DTC_JUDGE_OBJECT;

typedef struct
{
    DTC_JUDGE_OBJECT  Battery_Voltage_Abnormal_Low;
    DTC_JUDGE_OBJECT  Battery_Voltage_Abnormal_High;
    DTC_JUDGE_OBJECT  BCAN_Control_Module_Communication_Bus_Off;
    DTC_JUDGE_OBJECT  Lost_Communication_with_TCU;
    DTC_JUDGE_OBJECT  Lost_Communication_with_ABS;
    DTC_JUDGE_OBJECT  Lost_Communication_with_AWD;
    DTC_JUDGE_OBJECT  Travel_Sensor_Unstable;
    DTC_JUDGE_OBJECT  Driver_Voltage_Abnormality;
    DTC_JUDGE_OBJECT  Driver_Open_Circuit;
    DTC_JUDGE_OBJECT  Driver_Overcurrent;
    DTC_JUDGE_OBJECT  Driver_Stuck;
    ubyte TCU_3_RxFrame_Flag;
    ubyte ABS_3_RxFrame_Flag;
    ubyte AWD_1_RxFrame_Flag;
}DTC_JUDGE;
extern DTC_JUDGE DTC_Judge;

typedef struct
{
    uword DTC_PowerModel_PowerOn_Count;
    ubyte DTC_PowerModel_PowerOn_Valid;
    uword DTC_PowerModel_Voltage_Valid_Count;
    ubyte DTC_PowerModel_Voltage_Valid;
    uword IG_ON_Count;
    ubyte IG_ON_Valid;
    ubyte BD_Bussoff_Recovery;
    uword BD_Bussoff_Recovery_Count;
}DTC_CONTROL;
extern DTC_CONTROL Dtc_Control;

extern void DTCInit(void);
extern ubyte DTCWakeUpDeal(void);
extern ubyte DTCSleepDeal(void);
// extern void DTCDrivingCycleBeginBDCAN(void);
extern void DTCDrivingCycleEndBDCAN(void);
extern void ClearDiagnosticInfor(void);
extern void ReportNumberOfDTCBySTMask(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask);
extern void ReportDTCBySTMask(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask);
extern void ReportSupportedDtc(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask);
#if SNAP_SHOT
extern ubyte ReportDTCSnapshotRecordByNumber(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask);
#endif
extern ubyte ReportDTCExtendedDataRecordByNumber(ushort *puwLen, ubyte *aubUdsData);
extern void DtcRecordControl(void);

#endif
