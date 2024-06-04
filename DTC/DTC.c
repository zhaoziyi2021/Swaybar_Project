/*
 * DTC.c
 *
 *  Created on: 2016-1-8
 *      Author: ThinkPad
 */


#include "MAIN.H"

DTC_OBJECT DTC_Object[DTC_NUM];
DTC_JUDGE DTC_Judge;
DTC_CONTROL Dtc_Control;

#if SNAP_SHOT
ubyte DTC_Snapshot_Save_Flag = 0;
#endif

const ulong DTC_Number[DTC_NUM]=
{
    0x911716,/*No.01 B111716: Power supply voltage below threshold*/
    0x911717,/*No.02 B111717: Power supply voltage above threshold*/
    0xC07300,/*No.03 U007300: Control Module Communication Bus Off*/
    0xC10100,/*No.04 U010100: Lost Communication With TCU*/
    0xC12100,/*No.05 U012100: Lost Communication With ABS*/
    0xC11D00,/*No.06 U011D00: Lost Communication With AWD*/
    0xFFFFF0,/*No.07 UFFFFF0: Travel sensor unstable*/
    0xFFFFF1,/*No.08 UFFFFF1: Swaybar drive voltage abnormality*/
    0xFFFFF2,/*No.09 UFFFFF2: Swaybar driver open circuit*/
    0xFFFFF3,/*No.10 UFFFFF3: Swaybar driver overcurrent*/
    0xFFFFF4,/*No.11 UFFFFF4: Swaybar driver stuck*/
};


//=================================================================================================
// [Function Name] void DTCInit(void)
// [Parameters] None
// [Introduction] DTC Init,Power on for ignition cycle
// [Return value] None
// [Creator] 20240529 zhaoziyi
//=================================================================================================
void DTCInit(void)
{
    (void) memset((void *)&DTC_Judge, 0,sizeof(DTC_JUDGE));
    (void) memset((void *)&Dtc_Control, 0,sizeof(DTC_CONTROL));

    System_Control.Control_DTC_Setting = ON;
    System_Control.Read_Dtc_Failer = DTCWakeUpDeal();

    // Power on for ignition cycle
    for(ubyte i = 0; i < DTC_NUM; i++)
    {
        if(DTC_Object[i].Status.bits.Last_Cycle_Error_Flag == 0)/*�������޹��ϣ���pending��test failed*/
        {
            DTC_Object[i].DTC_ST.bits.Test_Failed = 0;
            DTC_Object[i].DTC_ST.bits.Pending_DTC = 0;

            if(DTC_Object[i].DTC_ST.bits.Confirmed_DTC == 1)/*���ȷ������?*/
            {
                DTC_Object[i].Status.bits.Cycle_Times++;
                if(DTC_Object[i].Status.bits.Cycle_Times >= WAKEUP_CYCLE_TIMES)
                {
                    DTC_Object[i].Status.bits.Cycle_Times = 0;
                    DTC_Object[i].DTC_ST.byte = 0;
                }
            }
        }
        DTC_Object[i].Status.bits.Last_Cycle_Error_Flag = 0;
        DTC_Object[i].DTC_ST.bits.Test_Failed = 0;
        // DTC_Object[i].Status.bits.Wakeup_Flag = 1;
    }
}


#if SNAP_SHOT
//=================================================================================================
// [Function Name] static ubyte DTCSPWakeUpDeal(void)
// [Parameters] None
// [Introduction] 
// [Return value] 
// [Creator]
//=================================================================================================
static ubyte DTCSPWakeUpDeal(void)//��eeprom�ָ�SP
{
    ubyte check_sum = 0;
    ubyte error = 0;

    error = ReadEEromPatameter((uword*)EepromPageBuffer1, 1);

    if(error == 0)
    {
        check_sum = CalcChecksum(125, (ubyte*)EepromPageBuffer1);

        if(check_sum == EepromPageBuffer1[125])
        {
            for(ubyte i=0; i<DTC_NUM; i++)
            {
                DTC_Object[i].SnapshotRecord[0]  = EepromPageBuffer1[i*6+0];
                DTC_Object[i].SnapshotRecord[1]  = EepromPageBuffer1[i*6+1];
                DTC_Object[i].SnapshotRecord[2]  = EepromPageBuffer1[i*6+2];
                DTC_Object[i].SnapshotRecord[3]  = EepromPageBuffer1[i*6+3];
                DTC_Object[i].SnapshotRecord[4]  = EepromPageBuffer1[i*6+4];
                DTC_Object[i].SnapshotRecord[5]  = EepromPageBuffer1[i*6+5];

                DTC_Snapshot_Save_Flag = 0;
            }
        }
        else
        {
            return 2;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}
#endif


//=================================================================================================
// [Function Name] ubyte DTCWakeUpDeal(void)
// [Parameters] None
// [Introduction]
// [Return value] None
// [Creator]
//=================================================================================================
ubyte DTCWakeUpDeal(void)
{
    ubyte check_sum = 0;
    ubyte error = 0;

    error = ReadEEromPatameter((uword*)EepromPageBuffer0, 0);

    if(error == 0)
    {
        check_sum = CalcChecksum(125, (ubyte*)EepromPageBuffer0);

        if(check_sum == EepromPageBuffer0[125])
        {
            for(ubyte i=0; i<DTC_NUM; i++)
            {
                DTC_Object[i].Status.byte = EepromPageBuffer0[i*3];
                DTC_Object[i].DTC_ST.byte = EepromPageBuffer0[i*3+1];
                DTC_Object[i].FaultOccurentCounter = EepromPageBuffer0[i*3+2];
            }

            #if SNAP_SHOT
            error = DTCSPWakeUpDeal();

            if(error != 0)
            {
                return 1;
            }
            #endif
        }
        else
        {
            return 2;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}


//=================================================================================================
// [Function Name] void DTCSleepDeal(void)
// [Parameters] None
// [Introduction] DTC processing logic before sleep
// [Return value] error flag
// [Creator]
//=================================================================================================
ubyte DTCSleepDeal(void)
{
    ubyte check_sum = 0;
    ubyte error = 0;
    ubyte dtc_different = 0;
    ubyte i=0;

    if(System_Control.Read_Dtc_Failer==0)
    {
        for(i=0; i<DTC_NUM; i++)
        {
            if((EepromPageBuffer0[i*3]   != DTC_Object[i].Status.byte) ||
               (EepromPageBuffer0[i*3+1] != DTC_Object[i].DTC_ST.byte) ||
               (EepromPageBuffer0[i*3+2] != DTC_Object[i].DTC_ST.byte))
            {
                EepromPageBuffer0[i*3]   = DTC_Object[i].Status.byte;
                EepromPageBuffer0[i*3+1] = DTC_Object[i].DTC_ST.byte;
                EepromPageBuffer0[i*3+2] = DTC_Object[i].FaultOccurentCounter;
                dtc_different = 1;
            }
        }

        if(dtc_different)
        {
            Eeprom0ProgrammCounter();
            check_sum = CalcChecksum(125, EepromPageBuffer0);
            EepromPageBuffer0[125] = check_sum;
            error = WriteEEromPatameter((uword*)EepromPageBuffer0,0);
        }

#if SNAP_SHOT
        if(DTC_Snapshot_Save_Flag)
        {
            for(ubyte i=0; i<DTC_NUM; i++)
            {
                EepromPageBuffer1[i*6+0] = DTC_Object[i].SnapshotRecord[0];
                EepromPageBuffer1[i*6+1] = DTC_Object[i].SnapshotRecord[1];
                EepromPageBuffer1[i*6+2] = DTC_Object[i].SnapshotRecord[2];
                EepromPageBuffer1[i*6+3] = DTC_Object[i].SnapshotRecord[3];
                EepromPageBuffer1[i*6+4] = DTC_Object[i].SnapshotRecord[4];
                EepromPageBuffer1[i*6+5] = DTC_Object[i].SnapshotRecord[5];
            }
            DTC_Snapshot_Save_Flag = 0;

            Eeprom1ProgrammCounter();
            check_sum = CalcChecksum(125, EepromPageBuffer1);
            EepromPageBuffer1[125] = check_sum;
            error = WriteEEromPatameter((uword*)EepromPageBuffer1,1);
        }
#endif

        /*�������ߺ���ر�������?,Ԥ��û�н���͹����ٴα������?*/
        (void) memset((void *)&DTC_Judge, 0,sizeof(DTC_JUDGE));
        (void) memset((void *)&Dtc_Control, 0,sizeof(DTC_CONTROL));
    }
    else
    {
        return 1;//error
    }

    if(error!=0)
    {
        return 1;//error
    }

    return 0;//no error
}


//=================================================================================================
// [Function Name] void DTCDrivingCycleBeginBDCAN(void)
// [Parameters] None
// [Introduction] 
// [Return value] None
// [Creator] 20240529 zhaoziyi
//=================================================================================================
// void DTCDrivingCycleBeginBDCAN(void)
// {
//     for(ubyte i = 0; i < DTC_NUM; i++)
//     {
//         if(DTC_Object[i].Status.bits.Last_Cycle_Error_Flag == 0)/*�������޹��ϣ���pending��test failed*/
//         {
//             DTC_Object[i].DTC_ST.bits.Test_Failed = 0;
//             DTC_Object[i].DTC_ST.bits.Pending_DTC = 0;

//             if(DTC_Object[i].DTC_ST.bits.Confirmed_DTC == 1)/*���ȷ������?*/
//             {
//                 DTC_Object[i].Status.bits.Cycle_Times++;
//                 if(DTC_Object[i].Status.bits.Cycle_Times >= WAKEUP_CYCLE_TIMES)
//                 {
//                     DTC_Object[i].Status.bits.Cycle_Times = 0;
//                     DTC_Object[i].DTC_ST.byte = 0;
//                 }
//             }
//         }
//         DTC_Object[i].Status.bits.Last_Cycle_Error_Flag = 0;
//         DTC_Object[i].DTC_ST.bits.Test_Failed = 0;
//         // DTC_Object[i].Status.bits.Wakeup_Flag = 1;
//     }
// }


//=================================================================================================
// [Function Name] void DTCDrivingCycleEndBDCAN(void)
// [Parameters] None
// [Introduction] 
// [Return value] None
// [Creator]
//=================================================================================================
void DTCDrivingCycleEndBDCAN(void)
{
    if(System_Control.WakeUp_Dtc_BD == 1)
    {
        System_Control.WakeUp_Dtc_BD = 0;

        for(ubyte i=0; i<DTC_NUM; i++)
        {
            DTC_Object[i].DTC_ST.bits.Test_Failed = 0;
        }

        (void) memset((void *)&DTC_Judge, 0,sizeof(DTC_JUDGE));
    }
}


//=================================================================================================
// [Function Name] void ClearDiagnosticInfor(void)
// [Parameters] None
// [Introduction] 
// [Return value] None
// [Creator]
//=================================================================================================
void ClearDiagnosticInfor(void)
{
    ubyte check_sum = 0;

    for(ubyte i=0; i<DTC_NUM; i++)
    {
        DTC_Object[i].Status.byte = 0;
        DTC_Object[i].DTC_ST.byte = 0;
        DTC_Object[i].FaultOccurentCounter = 0;
        EepromPageBuffer0[i*3] = 0;
        EepromPageBuffer0[i*3+1] = 0;
        EepromPageBuffer0[i*3+2] = 0;
    }

    Eeprom0ProgrammCounter();
    check_sum = CalcChecksum(125, EepromPageBuffer0);
    EepromPageBuffer0[125] = check_sum;
    (void)WriteEEromPatameter((uword*)EepromPageBuffer0,0);

#if SNAP_SHOT
    for(ubyte i=0; i<DTC_NUM; i++)
    {
        DTC_Object[i].SnapshotRecord[0]  = 0;
        DTC_Object[i].SnapshotRecord[1]  = 0;
        DTC_Object[i].SnapshotRecord[2]  = 0;
        DTC_Object[i].SnapshotRecord[3]  = 0;
        DTC_Object[i].SnapshotRecord[4]  = 0;
        DTC_Object[i].SnapshotRecord[5]  = 0;

        EepromPageBuffer1[i*6+0] = 0;
        EepromPageBuffer1[i*6+1] = 0;
        EepromPageBuffer1[i*6+2] = 0;
        EepromPageBuffer1[i*6+3] = 0;
        EepromPageBuffer1[i*6+4] = 0;
        EepromPageBuffer1[i*6+5] = 0;

        DTC_Snapshot_Save_Flag = 0;
    }

    Eeprom1ProgrammCounter();
    check_sum = CalcChecksum(125, EepromPageBuffer1);
    EepromPageBuffer1[125] = check_sum;
    (void)WriteEEromPatameter((uword*)EepromPageBuffer1,1);
#endif

    (void) memset((void *)&DTC_Judge, 0,sizeof(DTC_JUDGE));
}


//=================================================================================================
// [Function Name] void ReportNumberOfDTCBySTMask(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask)
// [Parameters] puwLen, aubUdsData, status_mask
// [Introduction] Diagnostic (19 01) call function, reporting the current number of DTC
// [Return value] None
// [Creator] zhaoziyi 
//=================================================================================================
void ReportNumberOfDTCBySTMask(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask)
{
    ubyte Mask_Temp = 0;
    uword Dtc_Num = 0;

    for(ubyte i=0; i<DTC_NUM; i++)
    {
        Mask_Temp = aubUdsData[2]&status_mask;
        Mask_Temp&=DTC_Object[i].DTC_ST.byte;

        if(Mask_Temp!=0)
        {
            Dtc_Num++;
        }
    }

    *puwLen = 6;
    aubUdsData[2]=status_mask;
    aubUdsData[3]=0x01;
    aubUdsData[4]=(ubyte)(Dtc_Num>>8);
    aubUdsData[5]=(ubyte)Dtc_Num;
}


//=================================================================================================
// [Function Name] void ReportDTCBySTMask(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask)
// [Parameters] puwLen, aubUdsData, status_mask
// [Introduction] Diagnostic (19 02) call function, reporting current DTC
// [Return value] None
// [Creator] zhaoziyi 
//=================================================================================================
void ReportDTCBySTMask(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask)
{
    ubyte Mask_Temp=0;
    ubyte i=0;
    uword j=0;

    for(i=0; i<DTC_NUM; i++)
    {
        Mask_Temp = aubUdsData[2]&status_mask;
        Mask_Temp&=DTC_Object[i].DTC_ST.byte;

        if(Mask_Temp!=0)
        {
            aubUdsData[j+3]=(ubyte)((DTC_Number[i]&0xFF0000)>>16);
            aubUdsData[j+4]=(ubyte)((DTC_Number[i]&0x00FF00)>>8);
            aubUdsData[j+5]=(ubyte)(DTC_Number[i]&0x0000FF);
            aubUdsData[j+6]=(DTC_Object[i].DTC_ST.byte&status_mask);
            j+=4;
        }
    }
    *puwLen = j+3;
    aubUdsData[2]=status_mask;
}


//=================================================================================================
// [Function Name] void ReportSupportedDtc(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask)
// [Parameters] puwLen, aubUdsData, status_mask
// [Introduction] Diagnostic (19 0A) call function, reporting DTC supported by ECU
// [Return value] None
// [Creator] zhaoziyi 
//=================================================================================================
void ReportSupportedDtc(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask)
{
    ubyte i=0;
    uword j=0;

    for(i=0; i<DTC_NUM; i++)
    {
        aubUdsData[j+3]=(ubyte)((DTC_Number[i]&0xFF0000)>>16);
        aubUdsData[j+4]=(ubyte)((DTC_Number[i]&0x00FF00)>>8);
        aubUdsData[j+5]=(ubyte)(DTC_Number[i]&0x0000FF);
        aubUdsData[j+6]=(DTC_Object[i].DTC_ST.byte&status_mask);
        j+=4;
    }
    *puwLen = j+3;
    aubUdsData[2]=status_mask;
}


#if SNAP_SHOT
//=================================================================================================
// [Function Name] u8 ReportDTCSnapshotRecordByNumber(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask)
// [Parameters] puwLen, aubUdsData, status_mask
// [Introduction] Diagnostic (19 04) call function, reporting DTC Snapshot Record
// [Return value] Diagnostic feedback
// [Creator] zhaoziyi 
//=================================================================================================
u8 ReportDTCSnapshotRecordByNumber(ushort *puwLen, ubyte *aubUdsData, ubyte status_mask)
{
    ubyte Mask_Temp=0;
    ubyte i=0;
    u8 ret = UDS_ACK;

    Mask_Temp = aubUdsData[5]&0x01;

    if(Mask_Temp!=0)
    {
        for(i=0; i<DTC_NUM; i++)
        {
            if((aubUdsData[2]==(ubyte)((DTC_Number[i]&0xFF0000)>>16))&&
               (aubUdsData[3]==(ubyte)((DTC_Number[i]&0x00FF00)>>8))&&
               (aubUdsData[4]==(ubyte)(DTC_Number[i]&0x0000FF)))
            {
                break;
            }
        }

        if(i<DTC_NUM)
        {
            if((DTC_Object[i].DTC_ST.byte&status_mask)!=0)
            {
                aubUdsData[5]=(DTC_Object[i].DTC_ST.byte&status_mask);
                aubUdsData[6]=0x01;
                aubUdsData[7]=0x01;

                aubUdsData[8]=0xFC;
                aubUdsData[9]=0xFF;
                aubUdsData[10]=DTC_Object[i].SnapshotRecord[0];
                aubUdsData[11]=DTC_Object[i].SnapshotRecord[1];
                aubUdsData[12]=DTC_Object[i].SnapshotRecord[2];
                aubUdsData[13]=DTC_Object[i].SnapshotRecord[3];
                aubUdsData[14]=DTC_Object[i].SnapshotRecord[4];
                aubUdsData[15]=DTC_Object[i].SnapshotRecord[5];

                *puwLen = 16;
                ret = UDS_ACK;
            }
            else
            {
                aubUdsData[5]=(DTC_Object[i].DTC_ST.byte&status_mask);

                *puwLen = 6;
                ret = UDS_ACK;
            }
        }
        else
        {
            *puwLen = 3;
            ret = UDS_NRC_31;//requestOutOfRange
        }
    }
    else
    {
        *puwLen = 3;
        ret = UDS_NRC_12;//subFunctionNotSupported
    }

   return ret;

}
#endif


//=================================================================================================
// [Function Name] ubyte ReportDTCExtendedDataRecordByNumber(ushort *puwLen, ubyte *aubUdsData)
// [Parameters] puwLen, aubUdsData, status_mask
// [Introduction] Diagnostic call function, reporting DTC Extended Data Record
// [Return value] Diagnostic feedback
// [Creator] zhaoziyi 
//=================================================================================================
ubyte ReportDTCExtendedDataRecordByNumber(ushort *puwLen, ubyte *aubUdsData)
{
    ubyte Mask_Temp=0;
    ubyte i=0;
    ubyte ret = UDS_ACK;

    Mask_Temp=aubUdsData[5]&0x01;

    if(Mask_Temp!=0)
    {
        for(i=0; i<DTC_NUM; i++)
        {
            if((aubUdsData[2]==(ubyte)((DTC_Number[i]&0xFF0000)>>16))&&
                (aubUdsData[3]==(ubyte)((DTC_Number[i]&0x00FF00)>>8))&&
                (aubUdsData[4]==(ubyte)(DTC_Number[i]&0x0000FF)))
            {
                break;
            }
        }

        if(i<DTC_NUM)
        {
            aubUdsData[5] = DTC_Object[i].DTC_ST.byte&0x09;
            aubUdsData[6] = DTC_Object[i].FaultOccurentCounter;
            aubUdsData[7] = DTC_Object[i].Status.bits.Cycle_Times;

            *puwLen = 8;
            ret = UDS_ACK;
        }
        else
        {
            *puwLen = 3;
            ret = UDS_NRC_31;
        }
    }
    else
    {
        *puwLen = 3;
        ret = UDS_NRC_31;
    }

   return ret;
}


//=================================================================================================
// [Function Name] static void DTCObjectUpdate(ubyte index, ubyte failure)
// [Parameters] index, failure
// [Introduction] DTC status update
// [Return value] None
// [Creator] zhaoziyi 
//=================================================================================================
static void DTCObjectUpdate(ubyte index, ubyte failure)
{
    if(index >= DTC_NUM)
    {
        // index error
    }
    else if(failure)
    {
        // if(DTC_Object[index].Status.bits.Wakeup_Flag)
        // {
        //     DTC_Object[index].Status.bits.Wakeup_Flag = 0;

        //     if(DTC_Object[index].DTC_ST.bits.Pending_DTC )
        //     {
        //         DTC_Object[index].DTC_ST.bits.Confirmed_DTC = 1;
        //     }
        // }

        if(DTC_Object[index].DTC_ST.bits.Test_Failed == 0)
        {
            DTC_Object[index].DTC_ST.bits.Test_Failed = 1;
            //DTC_Object[index].DTC_ST.bits.Pending_DTC = 1;
            DTC_Object[index].DTC_ST.bits.Confirmed_DTC = 1;
            DTC_Object[index].Status.bits.Cycle_Times = 0;
            DTC_Object[index].Status.bits.Last_Cycle_Error_Flag = 1;

            if(DTC_Object[index].FaultOccurentCounter < 0xFF)
            {
                DTC_Object[index].FaultOccurentCounter ++;
            }
            
#if SNAP_SHOT
            DTC_Object[index].SnapshotRecord[0] = CAN_Rcv.IPK_DateTime_Info.Second;
            DTC_Object[index].SnapshotRecord[1] = CAN_Rcv.IPK_DateTime_Info.Minute;
            DTC_Object[index].SnapshotRecord[2] = CAN_Rcv.IPK_DateTime_Info.Hour;
            DTC_Object[index].SnapshotRecord[3] = CAN_Rcv.IPK_DateTime_Info.Day;
            DTC_Object[index].SnapshotRecord[4] = CAN_Rcv.IPK_DateTime_Info.Month;
            DTC_Object[index].SnapshotRecord[5] = CAN_Rcv.IPK_DateTime_Info.Year;

            DTC_Snapshot_Save_Flag = 1;
#endif
        }
    }
    else
    {
        DTC_Object[index].DTC_ST.bits.Test_Failed = 0;
    }
}


//=================================================================================================
// [Function Name] void DTCDeal(void)
// [Parameters] None
// [Introduction]
// [Return value] None
// [Creator] zhaoziyi 240521
//=================================================================================================
void DTCDeal(void)
{
    /*No.03 U007300: Control Module Communication Bus Off*/
    if(DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Count >= 10)
    {
        DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Count = 10;
        DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Flag = 1;
    }
    else
    {
        DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Flag = 0;
    }

    /*No.04 U010100: Lost Communication With TCU*/
    if(DTC_Judge.TCU_3_RxFrame_Flag)
    {
        DTC_Judge.TCU_3_RxFrame_Flag = 0;

        DTC_Judge.Lost_Communication_with_TCU.Flag=0;
        DTC_Judge.Lost_Communication_with_TCU.Count=0;
    }

    if(DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Count > 0)
    {
        DTC_Judge.Lost_Communication_with_TCU.Count = 0;
    }
    else if(Dtc_Control.BD_Bussoff_Recovery)
    {
        if(DTC_Judge.Lost_Communication_with_TCU.Flag==0)
        {
            if(DTC_Judge.Lost_Communication_with_TCU.Count < 50) //500ms
            {
                DTC_Judge.Lost_Communication_with_TCU.Count++;
                if(DTC_Judge.Lost_Communication_with_TCU.Count >= 50) //500ms
                {
                    DTC_Judge.Lost_Communication_with_TCU.Flag=1;
                }
            }
        }
    }
    else
    {
        /*do nothing*/
    }

    /*No.05 U012100: Lost Communication With ABS*/
    if(DTC_Judge.ABS_3_RxFrame_Flag)
    {
        DTC_Judge.ABS_3_RxFrame_Flag = 0;

        DTC_Judge.Lost_Communication_with_ABS.Flag=0;
        DTC_Judge.Lost_Communication_with_ABS.Count=0;
    }

    if(DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Count>0)
    {
        DTC_Judge.Lost_Communication_with_ABS.Count = 0;
    }
    else if(Dtc_Control.BD_Bussoff_Recovery)
    {
        if(DTC_Judge.Lost_Communication_with_ABS.Flag == 0)
        {
            if(DTC_Judge.Lost_Communication_with_ABS.Count < 50) //500ms
            {
                DTC_Judge.Lost_Communication_with_ABS.Count++;
                if(DTC_Judge.Lost_Communication_with_ABS.Count >= 50) //500ms
                {
                    DTC_Judge.Lost_Communication_with_ABS.Flag=1;
                }
            }
        }
    }
    else
    {
        /*do nothing*/
    }

    /*No.06 U011D00: Lost Communication With AWD*/
    if(DTC_Judge.AWD_1_RxFrame_Flag)
    {
        DTC_Judge.AWD_1_RxFrame_Flag = 0;

        DTC_Judge.Lost_Communication_with_AWD.Flag=0;
        DTC_Judge.Lost_Communication_with_AWD.Count=0;
    }

    if(DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Count>0)
    {
        DTC_Judge.Lost_Communication_with_AWD.Count = 0;
    }
    else if(Dtc_Control.BD_Bussoff_Recovery)
    {
        if(DTC_Judge.Lost_Communication_with_AWD.Flag == 0)
        {
            if(DTC_Judge.Lost_Communication_with_AWD.Count < 50) //500ms
            {
                DTC_Judge.Lost_Communication_with_AWD.Count++;
                if(DTC_Judge.Lost_Communication_with_AWD.Count >= 50) //500ms
                {
                    DTC_Judge.Lost_Communication_with_AWD.Flag=1;
                }
            }
        }
    }
    else
    {
        /*do nothing*/
    }


    /*No.07 UFFFFF0: Travel sensor unstable*/
    if((IO_Sts.PWM_Frequency > 1100) || 
        (IO_Sts.PWM_Frequency < 900))
    {
        if(DTC_Judge.Travel_Sensor_Unstable.Flag == 0)
        {
            if(DTC_Judge.Travel_Sensor_Unstable.Count < 50) //500ms
            {
                DTC_Judge.Travel_Sensor_Unstable.Count++;

                if(DTC_Judge.Travel_Sensor_Unstable.Count >= 50) //500ms
                {
                    DTC_Judge.Travel_Sensor_Unstable.Flag = 1;
                }
            }
        }
    }
    else
    {
        if(DTC_Judge.Travel_Sensor_Unstable.Flag == 1)
        {
            if(DTC_Judge.Travel_Sensor_Unstable.Count > 0)
            {
                DTC_Judge.Travel_Sensor_Unstable.Count --;
            }
            else
            {
                DTC_Judge.Travel_Sensor_Unstable.Flag = 0;
            }
        }
        else
        {
            DTC_Judge.Travel_Sensor_Unstable.Flag = 0;
            DTC_Judge.Travel_Sensor_Unstable.Count = 0;
        }
    }


    /*No.08 UFFFFF1: Swaybar drive voltage abnormality*/
    if(((DriverState == Swaybar_Drv_Link) && (IO_Sts.Ad_DrvOutput > 10)) ||
        ((DriverState == Swaybar_Drv_Hold) && (0)) ||
        ((DriverState == Swaybar_Drv_Pull0) && (0)) ||
        ((DriverState == Swaybar_Drv_Pull1) && (0)) ||
        ((DriverState == Swaybar_Drv_Pull2) && (0)) ||
        ((DriverState == Swaybar_Drv_Pull3) && (0)))
    {
        if(DTC_Judge.Driver_Voltage_Abnormality.Flag == 0)
        {
            if(DTC_Judge.Driver_Voltage_Abnormality.Count < 50) //500ms
            {
                DTC_Judge.Driver_Voltage_Abnormality.Count++;

                if(DTC_Judge.Driver_Voltage_Abnormality.Count >= 50) //500ms
                {
                    DTC_Judge.Driver_Voltage_Abnormality.Flag = 1;
                }
            }
        }
    }
    else
    {
        if(DTC_Judge.Driver_Voltage_Abnormality.Flag == 1)
        {
            if(DTC_Judge.Driver_Voltage_Abnormality.Count > 0)
            {
                DTC_Judge.Driver_Voltage_Abnormality.Count --;
            }
            else
            {
                DTC_Judge.Driver_Voltage_Abnormality.Flag = 0;
            }
        }
        else
        {
            DTC_Judge.Driver_Voltage_Abnormality.Flag = 0;
            DTC_Judge.Driver_Voltage_Abnormality.Count = 0;
        }
    }


    /*No.09 UFFFFF2: Swaybar driver open circuit*/
    if(IO_Sts.Ad_DrvOutputCnrrent < 240)
    {
        if(DTC_Judge.Driver_Open_Circuit.Flag == 0)
        {
            if(DTC_Judge.Driver_Open_Circuit.Count < 50) //500ms
            {
                DTC_Judge.Driver_Open_Circuit.Count++;

                if(DTC_Judge.Driver_Open_Circuit.Count >= 50) //500ms
                {
                    DTC_Judge.Driver_Open_Circuit.Flag = 1;
                }
            }
        }
    }
    else
    {
        if(DTC_Judge.Driver_Open_Circuit.Flag == 1)
        {
            if(DTC_Judge.Driver_Open_Circuit.Count > 0)
            {
                DTC_Judge.Driver_Open_Circuit.Count --;
            }
            else
            {
                DTC_Judge.Driver_Open_Circuit.Flag = 0;
            }
        }
        else
        {
            DTC_Judge.Driver_Open_Circuit.Flag = 0;
            DTC_Judge.Driver_Open_Circuit.Count = 0;
        }
    }


    /*No.10 UFFFFF3: Swaybar driver overcurrent*/
    if(IO_Sts.Ad_DrvOutputCnrrent > 4000)
    {
        if(DTC_Judge.Driver_Overcurrent.Flag == 0)
        {
            if(DTC_Judge.Driver_Overcurrent.Count < 50) //500ms
            {
                DTC_Judge.Driver_Overcurrent.Count++;

                if(DTC_Judge.Driver_Overcurrent.Count >= 50) //500ms
                {
                    DTC_Judge.Driver_Overcurrent.Flag = 1;
                }
            }
        }
    }
    else
    {
        if(DTC_Judge.Driver_Overcurrent.Flag == 1)
        {
            if(DTC_Judge.Driver_Overcurrent.Count > 0)
            {
                DTC_Judge.Driver_Overcurrent.Count --;
            }
            else
            {
                DTC_Judge.Driver_Overcurrent.Flag = 0;
            }
        }
        else
        {
            DTC_Judge.Driver_Overcurrent.Flag = 0;
            DTC_Judge.Driver_Overcurrent.Count = 0;
        }
    }


    /*No.11 UFFFFF4: Swaybar driver stuck*/
    if((StuckLink == 0x01) || (StuckPull == 0x01))
    {
        DTC_Judge.Driver_Stuck.Flag = 1;
    }
    else
    {
        DTC_Judge.Driver_Stuck.Flag = 0;
    }


    DTCObjectUpdate(2, DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Flag);
    DTCObjectUpdate(3, DTC_Judge.Lost_Communication_with_TCU.Flag);
    DTCObjectUpdate(4, DTC_Judge.Lost_Communication_with_ABS.Flag);
    DTCObjectUpdate(5, DTC_Judge.Lost_Communication_with_AWD.Flag);
    DTCObjectUpdate(6, DTC_Judge.Travel_Sensor_Unstable.Flag);
    DTCObjectUpdate(7, DTC_Judge.Driver_Voltage_Abnormality.Flag);
    DTCObjectUpdate(8, DTC_Judge.Driver_Open_Circuit.Flag);
    DTCObjectUpdate(9, DTC_Judge.Driver_Overcurrent.Flag);
    DTCObjectUpdate(10, DTC_Judge.Driver_Stuck.Flag);
}


//=================================================================================================
// [Function Name]void DtcVoltageDeal(void)
// [Parameters] None
// [Introduction] DTC voltage detection
// [Return value] None
// [Creator] zhaoziyi 240521
//=================================================================================================
void DtcVoltageDeal(void)
{
    /*No.01 B111716: Power supply voltage below threshold*/
    if(DTC_Judge.Battery_Voltage_Abnormal_High.Flag==0)
    {
        if(IO_Sts.Ad_Voltage > 165)
        {
            DTC_Judge.Battery_Voltage_Abnormal_High.Count++;
            if(DTC_Judge.Battery_Voltage_Abnormal_High.Count>50)
            {
                DTC_Judge.Battery_Voltage_Abnormal_High.Flag=1;
                DTC_Judge.Battery_Voltage_Abnormal_High.Count=0;
            }
        }
        else
        {
            DTC_Judge.Battery_Voltage_Abnormal_High.Count=0;
        }
    }
    else
    {
        if(IO_Sts.Ad_Voltage < 160)
        {
            DTC_Judge.Battery_Voltage_Abnormal_High.Count++;
            if(DTC_Judge.Battery_Voltage_Abnormal_High.Count>50)
            {
                DTC_Judge.Battery_Voltage_Abnormal_High.Flag=0;
                DTC_Judge.Battery_Voltage_Abnormal_High.Count=0;
            }
        }
        else
        {
            DTC_Judge.Battery_Voltage_Abnormal_High.Count=0;
        }

    }

    /*No.02 B111717: Power supply voltage above threshold*/
    if(DTC_Judge.Battery_Voltage_Abnormal_Low.Flag==0)
    {
        if(IO_Sts.Ad_Voltage < 85)
        {
            DTC_Judge.Battery_Voltage_Abnormal_Low.Count++;
            if(DTC_Judge.Battery_Voltage_Abnormal_Low.Count>50)
            {
                DTC_Judge.Battery_Voltage_Abnormal_Low.Flag=1;
                DTC_Judge.Battery_Voltage_Abnormal_Low.Count=0;
            }
        }
        else
        {
            DTC_Judge.Battery_Voltage_Abnormal_Low.Count=0;
        }
    }
    else
    {
        if(IO_Sts.Ad_Voltage > 90)
        {
            DTC_Judge.Battery_Voltage_Abnormal_Low.Count++;
            if(DTC_Judge.Battery_Voltage_Abnormal_Low.Count>50)
            {
                DTC_Judge.Battery_Voltage_Abnormal_Low.Flag=0;
                DTC_Judge.Battery_Voltage_Abnormal_Low.Count=0;
            }
        }
        else
        {
            DTC_Judge.Battery_Voltage_Abnormal_Low.Count=0;
        }
    }
}


//=================================================================================================
// [Function Name]void DtcRecordControl(void)
// [Parameters] None
// [Introduction] DTC main function,cycle 10ms
// [Return value] None
// [Creator] 20240529
//=================================================================================================
void DtcRecordControl(void)
{
    if(Dtc_Control.DTC_PowerModel_PowerOn_Count<DTC_POWERON_COUNT)
    {
        Dtc_Control.DTC_PowerModel_PowerOn_Count++;

        if(Dtc_Control.DTC_PowerModel_PowerOn_Count>=DTC_POWERON_COUNT)//3S
        {
            Dtc_Control.DTC_PowerModel_PowerOn_Valid=1;
        }
    }

    if((System_Control.Control_DTC_Setting==ON)&&(Dtc_Control.DTC_PowerModel_PowerOn_Valid))
    {
        /*Not subject to voltage limitation*/
        DtcVoltageDeal();
        DTCObjectUpdate(0, DTC_Judge.Battery_Voltage_Abnormal_Low.Flag);
        DTCObjectUpdate(1, DTC_Judge.Battery_Voltage_Abnormal_High.Flag);
    }

    if(Dtc_Control.DTC_PowerModel_Voltage_Valid==0)
    {
        if((IO_Sts.Ad_Voltage>100)&&(IO_Sts.Ad_Voltage<150))
        {
            if(Dtc_Control.DTC_PowerModel_Voltage_Valid_Count<DTC_VOLTAGE_COUNT_MAX)
            {
                Dtc_Control.DTC_PowerModel_Voltage_Valid_Count++;
                if(Dtc_Control.DTC_PowerModel_Voltage_Valid_Count>=DTC_VOLTAGE_COUNT_MAX)//500ms
                {
                    Dtc_Control.DTC_PowerModel_Voltage_Valid=1;
                }
            }
        }
    }
    else
    {
        if((IO_Sts.Ad_Voltage<90)||(IO_Sts.Ad_Voltage>160))
        {
            Dtc_Control.DTC_PowerModel_Voltage_Valid_Count = 0;
            Dtc_Control.DTC_PowerModel_Voltage_Valid = 0;
        }
    }

    if(System_Control.Control_DTC_Setting == ON)
    {
        DTCDeal();
    }

    if(DTC_Judge.BCAN_Control_Module_Communication_Bus_Off.Flag)
    {
        Dtc_Control.BD_Bussoff_Recovery=0;
        Dtc_Control.BD_Bussoff_Recovery_Count=0;
    }
    else
    {
        if(Dtc_Control.BD_Bussoff_Recovery_Count<100)
        {
            Dtc_Control.BD_Bussoff_Recovery_Count++;
            if(Dtc_Control.BD_Bussoff_Recovery_Count>=100)
            {
                Dtc_Control.BD_Bussoff_Recovery=1;
                Dtc_Control.BD_Bussoff_Recovery_Count=0;
            }
        }
    }
}



