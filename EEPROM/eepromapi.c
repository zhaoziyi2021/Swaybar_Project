/*
 * eepromapi.c
 *
 *  Created on: 2015-12-25
 *      Author: ThinkPad
 */


#include "main.h"



#define  STARTBYTE    3//must be modified

VEH_PARAMETER VEH_Parameter;
DYNAMIC_DATA  Dynamic_Data;


ubyte Page1DataBuffer[128];//Flash PAR
ubyte EepromPageBuffer0[128];
ubyte EepromPageBuffer1[128];


uword EEpromInitSatus;
uword EEpromRWSatus;
ubyte testreturn=0;


ubyte HW_Door_Control_Count=0;
ubyte HW_Washer_CMD_Count;
uword HW_MFU_CMD_Count;
ubyte IO_Control_Left_Turn_Lamp_Count=0;
ubyte IO_Control_Right_Turn_Lamp_Count=0;
ubyte IO_Control_Left_Turn_Lamp=0;
ubyte IO_Control_Right_Turn_Lamp=0;
ubyte IO_Control_Alarm_Horn_Count=0;
ubyte IO_Control_Alarm_Buzzer_Count=0;


ubyte WriteEEromPatameter(uword *wdatapoint,ubyte pageadd)
{
    ubyte error=0;
    uint32_t address = 0x00U;
    uint32_t address_start = 0x00U;
    uint32_t data0;
    uint32_t *p_data;
    p_data = (uint32_t *)wdatapoint;
   /* program flash */
    if(pageadd == 0)
    {
        address = 0x08C00000U;
    }
    else if(pageadd == 1)
    {
        address = 0x08C00080U;
    }
    else if(pageadd == 2)
    {
        address = 0x08C00100U;
    }
    else if(pageadd == 3)
    { 
        address = 0x08C00180U;
    }
    else
    {
        error=1;
    }
    address_start = address;

    fmc_unlock();
    ob_unlock();

    while(address < (address_start + 128)) 
    {
        data0 = p_data[(address - address_start) / 4];
        eeprom_word_program(address, data0);
        address += sizeof(uint32_t);
    }

    ob_lock();
    fmc_lock();

    return(error);
}

ubyte ReadEEromPatameter(uword *rdatapoint,ubyte pageadd)
{
    ubyte error=0;
    uint32_t address = 0x00U;
    uint32_t address_start = 0x00U;
    uint32_t data0;
    uint32_t *p_data;
    p_data = (uint32_t *)rdatapoint;
    /* program flash */
    if(pageadd == 0)
    {
        address = 0x08C00000U;
    }
    else if(pageadd == 1)
    {
        address = 0x08C00080U;
    }
    else if(pageadd == 2)
    {
        address = 0x08C00100U;
    }
    else if(pageadd == 3)
    { 
        address = 0x08C00180U;
    }
    else
    {
        error=1;
    }
    address_start = address;
    
    while(address < (address_start + 128)) 
    {
        fmc_unlock();
        ob_unlock();
        data0 = eeprom_word_read(address);
        ob_lock();
        fmc_lock();
        
        p_data[(address - address_start) / 4] = data0;
        address += sizeof(uint32_t);
    }

    return(error);
}


ubyte FlashPage1Programm(ulong addr,uword *data)
{
    ubyte ret = 1;
    fmc_state_enum rte_fmc;
    
    fmc_unlock();
    ob_unlock();

    rte_fmc = fmc_page_erase(addr);

    if(rte_fmc == FMC_READY)
    {
        rte_fmc = fmc_fast_program(addr, (uint64_t* )data);

        if(rte_fmc == FMC_READY)
        {
            ret = 0;
        }
    }

    ob_lock();
    fmc_lock();

    return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////
// CalcChecksum
// --------------------------------------------------------------------------------------
// Checksum Calculation Routine
/////////////////////////////////////////////////////////////////////////////////////////
ubyte CalcChecksum(ubyte Data_Length, ubyte *Data_Buffer)
{
    uword sum_calc = 0;
    ubyte i;
    ubyte temp=0;

    for(i = 0; i < Data_Length; i++)
    {
        sum_calc += Data_Buffer[i];

        if(sum_calc&0xFF00)
        {
          // Adds carry
            sum_calc = (sum_calc&0x00FF) + 1;
        }
    }
    //sum_calc ^= 0x00FF;
    temp=(ubyte)sum_calc;
  return temp;
}

void FlashProgrammCounter(void)
{
    ulong temp=0;

    temp  =  Page1DataBuffer[3];
    temp  =  ((temp<<8)|Page1DataBuffer[4]);
    temp  =  ((temp<<8)|Page1DataBuffer[5]);

    temp+=1;

    Page1DataBuffer[3]=(ubyte)((temp&0xFF0000)>>16);
    Page1DataBuffer[4]=(ubyte)((temp&0x00FF00)>>8);
    Page1DataBuffer[5]=(ubyte)((temp&0x0000FF)>>0);
}

void Eeprom0ProgrammCounter(void)
{
    ulong temp=0;

    temp  =  EepromPageBuffer0[122];
    temp  =  ((temp<<8)|EepromPageBuffer0[123]);
    temp  =  ((temp<<8)|EepromPageBuffer0[124]);

    temp+=1;

    EepromPageBuffer0[122]=(ubyte)((temp&0xFF0000)>>16);
    EepromPageBuffer0[123]=(ubyte)((temp&0x00FF00)>>8);
    EepromPageBuffer0[124]=(ubyte)((temp&0x0000FF)>>0);
}

void Eeprom1ProgrammCounter(void)
{
    ulong temp=0;

    temp  =  EepromPageBuffer1[122];
    temp  =  ((temp<<8)|EepromPageBuffer1[123]);
    temp  =  ((temp<<8)|EepromPageBuffer1[124]);

    temp+=1;

    EepromPageBuffer1[122]=(ubyte)((temp&0xFF0000)>>16);
    EepromPageBuffer1[123]=(ubyte)((temp&0x00FF00)>>8);
    EepromPageBuffer1[124]=(ubyte)((temp&0x0000FF)>>0);
}

ulong ReadProgrammCounter(ubyte Page_Num)
{
    ulong temp=0;

    if(Page_Num==0)
    {
        temp  =  EepromPageBuffer0[122];
        temp  =  ((temp<<8)|EepromPageBuffer0[123]);
        temp  =  ((temp<<8)|EepromPageBuffer0[124]);
    }
    else if(Page_Num==1)
    {
        temp  =  EepromPageBuffer1[3];
        temp  =  ((temp<<8)|EepromPageBuffer1[4]);
        temp  =  ((temp<<8)|EepromPageBuffer1[5]);
    }
    else if(Page_Num==2)
    {
        temp  =  Page1DataBuffer[122];
        temp  =  ((temp<<8)|Page1DataBuffer[123]);
        temp  =  ((temp<<8)|Page1DataBuffer[124]);
    }
    return temp;
}

ubyte ReadConfigDID(void)
{
    ubyte check_sum=0;

    for(ubyte i=0;i<128;i++)
    {
        Page1DataBuffer[i] = *((ubyte *)(DIAG_CPID_ADDRESS+i));//junjun
    }

    check_sum=(~CalcChecksum(125,&Page1DataBuffer[0]));
    if((check_sum+Page1DataBuffer[125])==0xFF)
    {
        VEH_Parameter.BYTE0.Byte=Page1DataBuffer[6];
        VEH_Parameter.BYTE1.Byte=Page1DataBuffer[7];
        VEH_Parameter.BYTE2.Byte=Page1DataBuffer[8];
        VEH_Parameter.BYTE3.Byte=Page1DataBuffer[9];
        VEH_Parameter.BYTE4.Byte=Page1DataBuffer[10];
        VEH_Parameter.BYTE5.Byte=Page1DataBuffer[11];
        VEH_Parameter.BYTE6.Byte=Page1DataBuffer[12];
        VEH_Parameter.BYTE7.Byte=Page1DataBuffer[13];

        if((VEH_Parameter.BYTE0.Byte == 0x00)&&(VEH_Parameter.BYTE1.Byte == 0x00)&&
            (VEH_Parameter.BYTE2.Byte == 0x00)&&(VEH_Parameter.BYTE3.Byte == 0x00)&&
            (VEH_Parameter.BYTE4.Byte == 0x00)&&(VEH_Parameter.BYTE5.Byte == 0x00)&&
            (VEH_Parameter.BYTE6.Byte == 0x00)&&(VEH_Parameter.BYTE7.Byte == 0x00))
        {
            VEH_Parameter.BYTE0.Byte = 0xFF;
            VEH_Parameter.BYTE1.Byte = 0xFF;
            VEH_Parameter.BYTE2.Byte = 0xFF;
            VEH_Parameter.BYTE3.Byte = 0xFF;
            VEH_Parameter.BYTE4.Byte = 0xFF;
            VEH_Parameter.BYTE5.Byte = 0xFF;
            VEH_Parameter.BYTE6.Byte = 0xFF;
            VEH_Parameter.BYTE7.Byte = 0xFF;
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

/*********************************************************************************/
/* Name: ReadParaFromFlash                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Read Para from Flash                                             */
/*                                                                               */
/*********************************************************************************/
ubyte ReadParaFromFlash(ushort *puwLen, ubyte *aubUdsData,ubyte offset_addr, ubyte length)
{
    ubyte pp=0;
    ubyte *ppsourcepiont;
    ubyte check_sum=0;

    for(ubyte i=0;i<128;i++)
    {
        Page1DataBuffer[i] = *((ubyte *)(DIAG_CPID_ADDRESS+i));
    }

    check_sum=(~CalcChecksum(125,&Page1DataBuffer[0]));
    if((check_sum+Page1DataBuffer[125])==0xFF)
    {
        ppsourcepiont=(ubyte *)&Page1DataBuffer[offset_addr];
        for(pp=0;pp<length;pp++)
        {
            *(aubUdsData+pp+3)=*(ppsourcepiont+pp);
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

/*********************************************************************************/
/* Name: WriteParaToFlash                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Write Para to Flash                                              */
/*                                                                               */
/*********************************************************************************/
ubyte WriteParaToFlash(ushort *puwLen, ubyte *aubUdsData,ubyte offset_addr, ubyte length)
{
    ubyte pp=0;
    ubyte error=0;
    ubyte *ppobjectpiont;
    ubyte check_sum=0;

    for(ubyte i=0;i<128;i++)
    {
        Page1DataBuffer[i] = *((ubyte *)(DIAG_CPID_ADDRESS+i));
    }

    check_sum=(~CalcChecksum(125,&Page1DataBuffer[0]));
    if((check_sum+Page1DataBuffer[125])==0xFF)
    {
        ppobjectpiont=(ubyte *)&Page1DataBuffer[offset_addr];
        for(pp=0;pp<length;pp++)
        {
            *(ppobjectpiont+pp)=*(aubUdsData+pp+STARTBYTE);
        }
    }
    else
    {
        return 1;
    }

    FlashProgrammCounter();

    check_sum=CalcChecksum(125,&Page1DataBuffer[0]);
    Page1DataBuffer[125]=check_sum;

    error = FlashPage1Programm(DIAG_CPID_ADDRESS,(uword*)Page1DataBuffer);

    if(error!=0)
    {
        return 1;
    }

    return 0;
}


/*********************************************************************************/
/* Name: ReadDoorAjarSwitchInputStatus                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Read Door Ajar Switch Input Status from RAM                      */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void ReadDoorAjarSwitchInputStatus(ushort *puwLen, ubyte *aubUdsData)
{
    aubUdsData[3]=Dynamic_Data.Door_Ajar_Switch_Input_Status.Byte;
}

/*********************************************************************************/
/* Name: DoorLockUnlockSwitchInputStatus                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Read Door Lock Unlock Switch Input Status from RAM               */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void ReadDoorLockUnlockSwitchInputStatus(ushort *puwLen, ubyte *aubUdsData)
{
    aubUdsData[3]=Dynamic_Data.Door_Lock_Unlock_Switch_Input_Status.Byte;
}

/*********************************************************************************/
/* Name: IGNOtherSwitchInputStatus                                               */
/*-------------------------------------------------------------------------------*/
/* Description: Read IGN Other Switch Input Status from RAM                      */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void ReadIGNOtherSwitchInputStatus(ushort *puwLen, ubyte *aubUdsData)
{
    aubUdsData[3]=Dynamic_Data.IGN_Other_Switch_Input_Status.Byte;
}

/*********************************************************************************/
/* Name: LockUnlockOutputStatus                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Read Lock Unlock Output Status from RAM                          */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void ReadLockUnlockOutputStatus(ushort *puwLen, ubyte *aubUdsData)
{
    aubUdsData[3]=Dynamic_Data.Lock_Unlock_Output_Status.Byte;
}
/*********************************************************************************/
/* Name: IlluminatedEntryBatterySaverOutputStatus                                */
/*-------------------------------------------------------------------------------*/
/* Description: Read Illuminated Entry Battery Saver Output Status from RAM      */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void ReadIlluminatedEntryBatterySaverOutputStatus(ushort *puwLen, ubyte *aubUdsData)
{
    aubUdsData[3]=Dynamic_Data.Illuminated_Entry_Battery_Saver_Output_Status.Byte;
}

/*********************************************************************************/
/* Name: HornOthersOutputStatus                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Read Horn Others Output Status from RAM                          */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void ReadHornOthersOutputStatus(ushort *puwLen, ubyte *aubUdsData)
{
    aubUdsData[3]=Dynamic_Data.Horn_Others_Output_Status.Byte;
}
/*********************************************************************************/
/* Name: ExteriorLightOutputStatus                                               */
/*-------------------------------------------------------------------------------*/
/* Description: Read Exterior Light Output Status from RAM                       */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void ReadExteriorLightOutputStatus(ushort *puwLen, ubyte *aubUdsData)
{
    aubUdsData[3]=Dynamic_Data.Exterior_Light_Output_Status.Byte;
}
/*********************************************************************************/
/* Name: WiperWasherOutputStatus                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: Read Wiper Washer Output Status from RAM                         */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void ReadWiperWasherOutputStatus(ushort *puwLen, ubyte *aubUdsData)
{
    aubUdsData[3]=Dynamic_Data.Wiper_Washer_Output_Status.Byte;
}



/*********************************************************************************/
/* Name: TurnCornerBrakeLampOutputStatus                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Read Turn Corner Brake Lamp Output Status from RAM               */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void ReadTurnCornerBrakeLampOutputStatus(ushort *puwLen, ubyte *aubUdsData)
{
    aubUdsData[3]=Dynamic_Data.Turn_Corner_Brake_Lamp_Output_Status.Byte;
}

/*********************************************************************************/
/* Name: RTETurnLampControlBrakeLamp                                             */
/*-------------------------------------------------------------------------------*/
/* Description: Turn Lamp Brake Lamp Control                                     */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void RTETurnLampControlBrakeLamp(ushort *puwLen, ubyte *aubUdsData)
{
    
}

/*********************************************************************************/
/* Name: RTELampControl                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Lamp Control                                                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void RTELampControl(ushort *puwLen, ubyte *aubUdsData)
{

}

/*********************************************************************************/
/* Name: RTEWiperControl                                                         */
/*-------------------------------------------------------------------------------*/
/* Description:  Wiper Control                                                   */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void RTEWiperControl(ushort *puwLen, ubyte *aubUdsData)
{

}

/*********************************************************************************/
/* Name: RTEATWSControl                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: ATWS Control                                                     */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void RTEATWSControl(ushort *puwLen, ubyte *aubUdsData)
{

}

/*********************************************************************************/
/* Name: RTEDoorControl                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Door Control                                                     */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void RTEDoorControl(ushort *puwLen, ubyte *aubUdsData)
{

}

/*********************************************************************************/
/* Name: RTEInteriorLampBSRControl                                               */
/*-------------------------------------------------------------------------------*/
/* Description: Interior Lamp BSR Control                                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void RTEInteriorLampBSRControl(ushort *puwLen, ubyte *aubUdsData)
{

}

/*********************************************************************************/
/* Name: RTEWindowControl                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Window Control                                                   */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : puwLen / aubUdsData                               */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void RTEWindowControl(ushort *puwLen, ubyte *aubUdsData)
{}

void IOControlClrReturn(void)
{

}
void IOControl(void)
{

}

ubyte Write_Model_Memory_Poweron_Count = 0;
//=================================================================================================
//���� �� �� �ơ� void WriteModelMemoryStateDeal(void)
//����       ����
//����       �ܡ� дģ�ʹ洢ֵ to EEPROM
//����   ��  ֵ�� None
//����   ��  �ߡ� 2018-05-05 ZYW
//=================================================================================================
void WriteModelMemoryStateDeal(void)
{}

//=================================================================================================
//���� �� �� �ơ� void ReadModelMemoryStateDeal(void)
//����       ����
//����       �ܡ� ��ģ�ʹ洢ֵ from EEPROM
//����   ��  ֵ�� None
//����   ��  �ߡ� 2018-08-24 ZYW
//=================================================================================================
ubyte ReadModelMemoryStateDeal(void)
{

    return 0;
}

ubyte WriteEepromSecurityAccessErrCnt(void)
{
    ubyte error=0;
    ubyte check_sum=0;

    error=ReadEEromPatameter((uword*)&EepromPageBuffer0[0],0);
    if(error==0)
    {
        check_sum=(~CalcChecksum(125,&EepromPageBuffer0[0]));
        if((check_sum+EepromPageBuffer0[125])==0xFF)
        {
            EepromPageBuffer0[120]=SecurityAccessErrCnt;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }
    Eeprom0ProgrammCounter();
    check_sum=CalcChecksum(125,&EepromPageBuffer0[0]);
    EepromPageBuffer0[125]=check_sum;

    error=WriteEEromPatameter((uword*)EepromPageBuffer0,0);
    if(error==1)
    {
        return 1;
    }

    return 0;
}

ubyte ReadEepromSecurityAccessErrCnt(void)
{
    ubyte error=0;
    ubyte check_sum=0;

    error=ReadEEromPatameter((uword*)&EepromPageBuffer0[0],0);
    if(error==0)
    {
        check_sum=(~CalcChecksum(125,&EepromPageBuffer0[0]));
        if((check_sum+EepromPageBuffer0[125])==0xFF)
        {
            SecurityAccessErrCnt   =    EepromPageBuffer0[120];
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 1;
    }

    return 0;

}

void SoftwareReset(void)
{
    SCU_SWRSTCON = 0x0000;       //SWCFG =0000,0000b (standard start)
    SCU_RSTCON0 = 0xC000;        //.SW=application reset
    SCU_SWRSTCON|=0x0003;         // generate reset
}

ubyte WriteRkeFlashParameter(void)
{
    return 0;
}

ubyte ClearRkeEepromParameter(void)
{
    return 0;
}

void ReadRkeParameter(void)
{

}

