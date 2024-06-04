/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : APP_Prg.c
************************************************************************************************
*   Project/Product :
*   Title           :
*   Author          : BWFENG
************************************************************************************************
*   Description     : This is common module for body netword
*
************************************************************************************************
*   Limitations     : NONE
*
************************************************************************************************
*   END_FILE_HDR*/

#ifndef _APP_PRG_C
#define _APP_PRG_C
#endif
/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
#include "EB_Prj.h"
/*                                                                               */
/*********************************************************************************/

/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Static Variables                                                            */
static u8 akeys[4];
static u8 Set_Seed_Flg = 0;
static u8 SessionChangeFlg = DEFAULTSESSION;
static u8 AccessSeedFlg = FALSE;
static u8 SecurityLevel1Req = FALSE;
static u8 Preconditions = FALSE;

/*Product information defined start*/
const unsigned char SoftValidFlg[20]          @(0x0803FD00) = {0x55,0xAA,0xAA,0x55,0x00,0x00};//0xC4FD00
const unsigned char SoftWare[16]              @(0x0803FD20) = "SW07551S16AASE5A";
const unsigned char HardWare[16]              @(0x0803FD30) = "HW07551S10AASE5A";
const unsigned char PartNumber[11]            @(0x0803FD40) = "8890824404 ";
const unsigned char GeelySpare_PartNumber[10] @(0x0803FD50) = "Swaybar   ";
const unsigned char ECUSerialNumber[24]       @(0x0803FD60) = {0x31,0x36,0x42,0x32,0x30,0x30,0x30,0x31,
                                                                  0x30,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
                                                                  0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

const unsigned char Boot_SoftWare[16] = "SW07551C10AASE5A";



u8 SupplierID[10] = "220999";/*��Ӧ�̴���*/

u8 CommuMatrixVer[2] = {0x30,0x35};
u8 NetWorkMangVer[2] = {0x30,0x33};
u8 DiagProtoclVer[2] = {0x30,0x32};
u8 CommuSpecVer[2] = {0x30,0x32};
u8 SupplManDate[4] = {0x20,0x20,0x01,0x01};


u8 F188_VMECUSN[8] = {0x20,0x20,0x01,0x01,0x20,0x20,0x01,0x01};
u8 F198_VMECUHN[8] = {0x20,0x20,0x01,0x01,0x20,0x20,0x01,0x01};
/*Product information defined end*/

/* - Public Variables------------------------------------------------------------*/
u8 SecurityAccessErrCnt = 0;
u8 Tester_Absent_Flg = TRUE;
u8 PwrInitSecurityAccessFlg = FALSE;
u8 SecurityLevel1_Access = FALSE;
u8 RKE1LearnedFlg;
u8 RKE2LearnedFlg;

u8 Normal_Message_Transmit_Flg = TRUE;
u8 Normal_Message_Receive_Flg = TRUE;
u8 NM_Message_Transmit_Flg = TRUE;
u8 NM_Message_Receive_Flg = TRUE;
u8 WDT_Disable = 0;

u8 Diag_SwaybarControlEn = 0;
u8 Diag_SwaybarControlSts = 0;
u8 Diag_Time_Pull2 = 0;
u8 Diag_Time_Stuck = 0;
/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */
void Access_Security_PowerInit(void);
/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */

/*                                                                               */
/*********************************************************************************/

/***************************** PRIVATE DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Private Constants                                                           */

/*-------------------------------------------------------------------------------*/
/* - Private Types                                                               */

/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */

/* IT context */
NCS_PRI_VAR u8 m_ubItContext;

/*-------------------------------------------------------------------------------*/
/* - Private Macros                                                              */

/*-------------------------------------------------------------------------------*/
/* - Private Function Prototypes                                                 */

/*                                                                               */
/*********************************************************************************/

/******************************** IMPLEMENTATION *********************************/
/*                                                                               */

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */

/*------------------------Add By BWFENG START------------------------------------*/
/* - Public Functions                                                            */
/*********************************************************************************/
/* Name: Access_Security_PowerInit                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Access_Security_PowerInit                                                 */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
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
void Access_Security_PowerInit(void)
{
    (void)ReadEepromSecurityAccessErrCnt();

    if(SecurityAccessErrCnt > 0)
    {
        if(SecurityAccessErrCnt == 0xFF)
        {
            SecurityAccessErrCnt = 0;
            PwrInitSecurityAccessFlg = TRUE;
        }
        else
        {
            if(SecurityAccessErrCnt == 3 )
            {
                SecurityAccessErrCnt = SecurityAccessErrCnt-1;
            }
            SoftTimerStart(Security_Access_Timer, SECURITY_ACCESS_DENY_TIMEROUT);
        }
    }
    else
    {
        PwrInitSecurityAccessFlg = TRUE;
    }
}
/* - End Access_Security_PowerInit                                               */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: APP_InterruptEnable                                                     */
/*-------------------------------------------------------------------------------*/
/* Description: Resume interrupts on critical section exit.                      */
/*********************************************************************************/
/* Input parameters          : ebStatus: interrupt context to be resumed         */
/* Output parameters         : None                                              */
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
void APP_InterruptEnable (u8 ebStatus)
{
    /* Implement here a treatment for a critical section exit. */
    /* Interrupts must be resumed. */
    m_ubItContext = ebStatus;
}
/* - End APP_InterruptEnable                                                     */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: APP_InterruptDisable                                                    */
/*-------------------------------------------------------------------------------*/
/* Description: Suspend interrupts on critical section entry                     */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : ebStatus: interrupt context to be resumed later   */
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
u8 APP_InterruptDisable (void)
{
    /* Implement here a treatment for a critical section entry. */
    /* Interrupts must be suspended. */
    return (m_ubItContext);
}
/* - End APP_InterruptDisable                                                    */

/*********************************************************************************/
/* Name: APP_RestoreContext                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Application context reloaded from non volatile memory.           */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
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
void APP_RestoreContext (void)
{
    /* After asynchronous reload from the non volatile memory, */
    /* reactivate the context into the related stack layers. */
}
/* - End APP_RestoreContext                                                      */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: APP_Manage                                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Application initialisation.                                      */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
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
void APP_Manage (void)
{
}
/* - End APP_Manage                                                              */
/*********************************************************************************/

/*********************************************************************************/
/* Name: APP_ComNAsInd                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM when a transmission timeout (N_As)        */
/*              occured                                                          */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
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
void APP_ComNAsInd (tComFrameIdx uFrameIdx)
{
/* No dummy code here because for some compilers provoks an ERROR */ 
/* QAC warning accepted for this case */
}
/* - End APP_ComNAsInd                                                           */
/*********************************************************************************/

/*********************************************************************************/
/* Name: APP_ComTxConf                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM when a transmission is confirmed OK       */
/*              (acknowledgement)                                                */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
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
void APP_ComTxConf (tComFrameIdx uFrameIdx)
{
/* No dummy code here because for some compilers provoks an ERROR */ 
/* QAC warning accepted for this case */
}
/* - End APP_ComTxConf                                                           */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: APP_ComModeModifyExtension                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called in the COM_ModeModify () function,               */
/*              after the asked COM mode was taked into account                  */
/*********************************************************************************/
/* Input parameters          : eCurMode: current COM mode                        */
/* Output parameters         : None                                              */
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
void APP_ComModeModifyExtension (tComMode eCurMode)
{
/* No dummy code here because for some compilers provoks an ERROR */ 
/* QAC warning accepted for this case */
}
/* - End APP_ComModeModifyExtension                                              */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: APP_ComBackInd                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM when a transmission/reception monitored   */
/*              frame is transmitted/received (after filtering)                  */
/*********************************************************************************/
/* Input parameters          : uNodeIdx: COM monitored node index                */
/* Output parameters         : None                                              */
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
void APP_ComBackInd (tComMonNodeIdx uNodeIdx)
{  
    switch (uNodeIdx)
    {
        case COM_NODE_IDX_Monitoring_0:
        break;    
    default:
        break;
    }
}
/* - End APP_ComBackInd                                                          */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: APP_CAN_ComLossInd                                                    */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM when a transmission/reception monitored   */
/*              frame is not transmitted/received (after filtering)              */
/*********************************************************************************/
/* Input parameters          : uNodeIdx: COM monitored node index                */
/* Output parameters         : None                                              */
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
void APP_ComLossInd (tComMonNodeIdx uNodeIdx)
{
    switch (uNodeIdx)
    {
        case COM_NODE_IDX_Monitoring_0:
        break;
    
    default:
        break;
    }
}
/* - End APP_ComLossInd                                                          */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: APP_ComTxFrameInd                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called before frame transmission                        */
/*              to allow data update                                             */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
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
void APP_ComTxFrameInd (tComFrameIdx uFrameIdx)
{
/* No dummy code here because for some compilers provoks an ERROR */ 
/* QAC warning accepted for this case */
}
/* - End APP_ComTxFrameInd                                                       */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: APP_COM_MonNodeStatusInd                                                */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM to notice APP for default management      */
/*********************************************************************************/
/* Input parameters          : uNodeIdx: COM monitored node index                */
/*                             eFrameStatus: Monitored frame status              */
/* Output parameters         : None                                              */
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
void APP_COM_MonNodeStatusInd(tComMonNodeIdx uNodeIdx, tComMonStatus eFrameStatus)
{

}
/* - End APP_COM_MonNodeStatusInd                                                */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: CAN_BusOffInd                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: BUS-OFF indication from under layer                              */
/*-------------------------------------------------------------------------------*/
/* Implements:   000-07-D028-D-02230-1                                           */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void CAN_BusOffInd (void)
{
    /* Request CAN Controller to exit from Bus Off */
    CAN_BusOn();

    /* Resume CAN activity */
    CAN_Activate();
}
/* - End CAN_BusOffInd                                                           */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: UDS_P2AboutToExpiredInd                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when P2 timer is about to expire.                */
/*-------------------------------------------------------------------------------*/
/* Implements: 000-07-D028-D-XXXXX-1                                             */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : Non                                               */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void UDS_P2AboutToExpireInd(void)
{
   
}
/* - End UDS_P2AboutToExpiredInd                                                 */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: DefaultSession                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus DefaultSession(u16 *puwLen, u8 *aubUdsData)
{
    if(SessionChangeFlg != DEFAULTSESSION)
    {
        System_Control.Control_DTC_Setting=ON;
    }

    Normal_Message_Transmit_Flg = TRUE;
    Normal_Message_Receive_Flg = TRUE;

    NM_Message_Transmit_Flg = TRUE;
    NM_Message_Receive_Flg = TRUE;

    SecurityLevel1_Access = FALSE;

    SessionChangeFlg = DEFAULTSESSION;

    Diag_SwaybarControlEn = 0;
    Diag_SwaybarControlSts = 0;

    Preconditions = FALSE;

    return UDS_ACK;
}
/* - End DefaultSession                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ProgrammingSession                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ProgrammingSession(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    uword Data_Buf_App[64] = {0};
    fmc_state_enum rte;

    if((NRC22_CONDITION == NRC22_FAIL) ||
        (IO_Sts.Ad_Voltage > 160) ||
        (IO_Sts.Ad_Voltage < 90) ||
        (Preconditions == FALSE))
    {
        ret = UDS_NRC_22;
    }
    else
    {
        __set_PRIMASK(1);

        for(ubyte i=0;i<64;i++)
        {
            Data_Buf_App[i] = *((uword*)(APP_INFOCode_ADDRESS+i*2));
        }

        Data_Buf_App[2] = 0x2211;

        fmc_unlock();
        ob_unlock();
        rte = fmc_page_erase(APP_INFOCode_ADDRESS);

        if(FMC_READY == rte)
        {
            fmc_unlock();
            ob_unlock();
            rte = fmc_fast_program(APP_INFOCode_ADDRESS, (ulonglong* )Data_Buf_App);

            if(FMC_READY == rte)
            {
                ret = UDS_ACK;
            }
            else
            {
                ret = UDS_NRC_72;
            }
        }
        else
        {
            ret = UDS_NRC_72;
        }

        ob_lock();
        fmc_lock();

        __set_PRIMASK(0);

        WDT_Disable = 1;
    }

    return ret;
}
/* - End ProgrammingSession                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ExtendedDiagnosticSession                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ExtendedDiagnosticSession(u16 *puwLen, u8 *aubUdsData)
{
    //IL_u8Wr_BCM_Bat_Voltage(0x03);
    if(SessionChangeFlg != EXTENDEDSESSION)
    {
        System_Control.Control_DTC_Setting=ON;
    }

    //Bd_Can_Maping_To_Obd_Fun_Enable = 0;
    //Pt_Can_Maping_To_Obd_Fun_Enable = 0;
    //St_Can_Maping_To_Obd_Fun_Enable = 0;

    SecurityLevel1_Access = FALSE;

    SessionChangeFlg = EXTENDEDSESSION;
    //IOControlClrReturn();
    return UDS_ACK;
}
/* - End ExtendedDiagnosticSession                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: HardReset                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus HardReset(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION == 0)
    {
        if(SessionChangeFlg != DEFAULTSESSION)
        {
            WDT_Disable = 1;
            SrvComInit();
            ret = UDS_ACK;
        }
        else
        {
            ret = UDS_NRC_7F;
        }
    }
    else
    {
        ret = UDS_NRC_22;
    }

    return ret;
}
/* - End HardReset                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: KeyOffOnReset                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus KeyOffOnReset(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION)
    {
        ret = UDS_NRC_22;
    }

    return ret;
}
/* - End KeyOffOnReset                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: SoftReset                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus SoftReset(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION == 0)
    {
        if(SessionChangeFlg != DEFAULTSESSION)
        {
            WDT_Disable = 1;
            SrvComInit();
            ret = UDS_ACK;
        }
        else
        {
            ret = UDS_NRC_7F;
        }
    }
    else
    {
        ret = UDS_NRC_22;
    }

    return ret;
}
/* - End SoftReset                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: SecurityLevel1                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus SecurityLevel1(u16 *puwLen, u8 *aubUdsData)
{
    static u32 lseed;
    static u32 SK_Local;
    
    SecurityLevel1Req = TRUE;
    if((ChkSoftTimerState(Security_Access_Timer) == SOFT_TIMER_ST_REACH)
        ||((SecurityAccessErrCnt <= 2)&&(PwrInitSecurityAccessFlg == TRUE)))
    {
        if(SecurityAccessErrCnt == 3 )
        {
            SecurityAccessErrCnt = SecurityAccessErrCnt-1;
        }
        SoftTimerStop(Security_Access_Timer);
        PwrInitSecurityAccessFlg = TRUE;
        *puwLen = 6;
        if(Set_Seed_Flg == 0)
        {
            Set_Seed_Flg = 1;
            srand(GPT12E_T2);
        }
        else
        {
            /*do nothing*/
        }
        if(SecurityLevel1_Access == TRUE)
        {
            lseed = 0x0000;
            AccessSeedFlg = TRUE;
        }
        else
        {
            lseed = rand();
            AccessSeedFlg = FALSE;
        }

        aubUdsData[2] = (lseed&0xFF000000)>>24;
        aubUdsData[3] = (lseed&0x00FF0000)>>16;
        aubUdsData[4] = (lseed&0x0000FF00)>>8;
        aubUdsData[5] = (lseed&0x000000FF>>0);

        SK_Local = Key_Maker_A01(lseed);

        akeys[0] = (u8)(SK_Local & 0xFF000000)>>24;
        akeys[1] = (u8)(SK_Local & 0x00FF0000)>>16;
        akeys[2] = (u8)(SK_Local & 0x0000FF00)>>8;
        akeys[3] = (u8)(SK_Local & 0x000000FF)>>24;

        return UDS_ACK;

    }
    else
    {
        *puwLen = 3;
        SecurityLevel1Req = FALSE;
        return UDS_NRC_37;
    }
}
/* - End SecurityLevel1                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: SecurityLevel1_req                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus SecurityLevel1_req(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    if((AccessSeedFlg == TRUE)||(SecurityLevel1Req == FALSE))
    {
        *puwLen = 3;
        ret = UDS_NRC_24;
    }
    else
    {
        SecurityLevel1Req = FALSE;
        if(((akeys[0] == aubUdsData[2])&&(akeys[1] == aubUdsData[3])
         &&(akeys[2] == aubUdsData[4])&&(akeys[3] == aubUdsData[5]))
#if(DEBUG_VDEM == ON)
        || ((0xAA == aubUdsData[2])&&(0xBB == aubUdsData[3])&&(0xCC == aubUdsData[4])&&(0xDD == aubUdsData[5]))
#endif
        )
        {
            SecurityLevel1_Access = TRUE;
            SecurityAccessErrCnt = 0;
            (void)WriteEepromSecurityAccessErrCnt();
            PwrInitSecurityAccessFlg = TRUE;
            *puwLen = 2;
            ret = UDS_ACK;
        }
        else
        {
            *puwLen = 3;
            SecurityAccessErrCnt++;
            if(SecurityAccessErrCnt >= 3)
            {
                SecurityAccessErrCnt = 3;
            }
            /*need write EEPROM for SecurityAccessErrCnt*/
            (void)WriteEepromSecurityAccessErrCnt();

            if(SecurityAccessErrCnt <= 2)
            {
                ret = UDS_NRC_35;
            }
            else
            {

            }

            if(SecurityAccessErrCnt == 3)
            {
                SoftTimerStart(Security_Access_Timer, SECURITY_ACCESS_DENY_TIMEROUT);
                ret = UDS_NRC_36;
            }
            else
            {

            }

        }
    }
    return ret;
}
/* - End SecurityLevel1_req                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: SecurityLevel2                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus SecurityLevel2(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_NRC_12;
}
/* - End SecurityLevel2                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: SecurityLevelFBL                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus SecurityLevelFBL(u16 *puwLen, u8 *aubUdsData)
{

    return UDS_NRC_12;
}
/* - End SecurityLevelFBL                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: EnableRxAndTx                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus EnableRxAndTx(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION == 0)
    {
        if(*(aubUdsData+2) == 0x01)
        {
            *puwLen = 2;
            Normal_Message_Transmit_Flg = TRUE;
            Normal_Message_Receive_Flg = TRUE;
            ret = UDS_ACK;
        }
        else if(*(aubUdsData+2) == 0x02)
        {
            *puwLen = 2;
            NM_Message_Transmit_Flg = TRUE;
            NM_Message_Receive_Flg = TRUE;
            ret = UDS_ACK;
        }
        else if(*(aubUdsData+2) == 0x03)
        {
            *puwLen = 2;
            Normal_Message_Transmit_Flg = TRUE;
            Normal_Message_Receive_Flg = TRUE;
            NM_Message_Transmit_Flg = TRUE;
            NM_Message_Receive_Flg = TRUE;
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
        ret = UDS_NRC_22;
    }

    return ret;
}
/* - End EnableRxAndTx                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: EnableRxAndDisableTx                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus EnableRxAndDisableTx(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION == 0)
    {
        if(*(aubUdsData+2) == 0x01)
        {
            *puwLen = 2;
            Normal_Message_Transmit_Flg = FALSE;
            Normal_Message_Receive_Flg = TRUE;
            ret = UDS_ACK;
        }
        else if(*(aubUdsData+2) == 0x02)
        {
            *puwLen = 2;
            NM_Message_Transmit_Flg = FALSE;
            NM_Message_Receive_Flg = TRUE;
            ret = UDS_ACK;
        }
        else if(*(aubUdsData+2) == 0x03)
        {
            *puwLen = 2;
            Normal_Message_Transmit_Flg = FALSE;
            Normal_Message_Receive_Flg = TRUE;
            NM_Message_Transmit_Flg = FALSE;
            NM_Message_Receive_Flg = TRUE;
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
        ret = UDS_NRC_22;
    }
    return ret;

}
/* - End EnableRxAndDisableTx                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: DisableRxAndEnableTx                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus DisableRxAndEnableTx(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION == 0)
    {
        if(*(aubUdsData+2) == 0x01)
        {
            *puwLen = 2;
            ret = UDS_ACK;
            Normal_Message_Transmit_Flg = TRUE;
            Normal_Message_Receive_Flg = FALSE;
        }
        else if(*(aubUdsData+2) == 0x02)
        {
            *puwLen = 2;
            NM_Message_Transmit_Flg = TRUE;
            NM_Message_Receive_Flg = FALSE;
            ret = UDS_ACK;
        }
        else if(*(aubUdsData+2) == 0x03)
        {
            *puwLen = 2;
            Normal_Message_Transmit_Flg = TRUE;
            Normal_Message_Receive_Flg = FALSE;
            NM_Message_Transmit_Flg = TRUE;
            NM_Message_Receive_Flg = FALSE;
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
        ret = UDS_NRC_22;
    }

    return ret;

}
/* - End DisableRxAndEnableTx                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: DisableRxAndTx                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus DisableRxAndTx(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION == 0)
    {
        if(*(aubUdsData+2) == 0x01)
        {
            *puwLen = 2;
            Normal_Message_Transmit_Flg = FALSE;
            Normal_Message_Receive_Flg = FALSE;
            ret = UDS_ACK;
        }
        else if(*(aubUdsData+2) == 0x02)
        {
            *puwLen = 2;
            NM_Message_Transmit_Flg = FALSE;
            NM_Message_Receive_Flg = FALSE;
            ret = UDS_ACK;
        }
        else if(*(aubUdsData+2) == 0x03)
        {
            *puwLen = 2;
            Normal_Message_Transmit_Flg = FALSE;
            Normal_Message_Receive_Flg = FALSE;
            NM_Message_Transmit_Flg = FALSE;
            NM_Message_Receive_Flg = FALSE;
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
        ret = UDS_NRC_22;
    }

    return ret;

}
/* - End DisableRxAndTx                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: TesterPresent                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus TesterPresent(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End TesterPresent                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ControlDTCSetting_ON                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ControlDTCSetting_ON(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION == 0)
    {
        System_Control.Control_DTC_Setting = ON;
    }
    else
    {
        ret = UDS_NRC_22;
    }
    return ret;
}
/* - End ControlDTCSetting_ON                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ControlDTCSetting_OFF                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ControlDTCSetting_OFF(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION == 0)
    {
        System_Control.Control_DTC_Setting=OFF;
    }
    else
    {
        ret = UDS_NRC_22;
    }

    return ret;
}
/* - End ControlDTCSetting_OFF                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ReadMemoryByAddress                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ReadMemoryByAddress(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End ReadMemoryByAddress                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: WriteMemoryByAddress                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus WriteMemoryByAddress(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End WriteMemoryByAddress                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ClearDiagnosticInformation                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ClearDiagnosticInformation(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(NRC22_CONDITION == 1)
    {
        ret = UDS_NRC_22;
    }
    else if((*(aubUdsData+1) == 0xFF) && (*(aubUdsData+2) == 0xFF) && (*(aubUdsData+3) == 0xFF))
    {
        *puwLen = 1;
        ClearDiagnosticInfor();
        ret = UDS_ACK;
    }
    else
    {
        *puwLen = 3;
        ret = UDS_NRC_31;
    }
    return ret;

}
/* - End ClearDiagnosticInformation                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ReportNumberOfDTCByStatusMask                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ReportNumberOfDTCByStatusMask(u16 *puwLen, u8 *aubUdsData)
{
    ReportNumberOfDTCBySTMask(puwLen, aubUdsData, 0x09);
    return UDS_ACK;
}
/* - End ReportNumberOfDTCByStatusMask                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name:  ReportDTCByStatusMask                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus  ReportDTCByStatusMask(u16 *puwLen, u8 *aubUdsData)
{
    ReportDTCBySTMask(puwLen, aubUdsData,0x09);
    return UDS_ACK;
}
/* - End  ReportDTCByStatusMask                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ReportDTCSnapshotRecordByDTCNumber                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ReportDTCSnapshotRecordByDTCNumber(u16 *puwLen, u8 *aubUdsData)
{
#if SNAP_SHOT
    u8 ret = UDS_ACK;
    ret = ReportDTCSnapshotRecordByNumber(puwLen, aubUdsData,0x09);
    return ret;
#else
    *puwLen = 3;
    return UDS_NRC_12;
#endif
}
/* - End ReportDTCSnapshotRecordByDTCNumber                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ReportDTCExtendedDataRecordByDTCNumber                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ReportDTCExtendedDataRecordByDTCNumber(u16 *puwLen, u8 *aubUdsData)
{
    ubyte ret = UDS_ACK;
    ret = ReportDTCExtendedDataRecordByNumber(puwLen, aubUdsData);
    return ret;
}
/* - End ReportDTCExtendedDataRecordByDTCNumber                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ReportSupportedDTC                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ReportSupportedDTC(u16 *puwLen, u8 *aubUdsData)
{
    ReportSupportedDtc(puwLen, aubUdsData,0x09);
    return UDS_ACK;
}
/* - End ReportSupportedDTC                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ReportNumberOfEmissionsRelatedOBDDTCByStatusMask                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ReportNumberOfEmissionsRelatedOBDDTCByStatusMask(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3;
    return UDS_NRC_12;
}
/* - End ReportNumberOfEmissionsRelatedOBDDTCByStatusMask                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ReportEmissionsRelatedOBDDTCByStatusMask                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ReportEmissionsRelatedOBDDTCByStatusMask(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3;
    return UDS_NRC_12;
}
/* - End ReportEmissionsRelatedOBDDTCByStatusMask                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: RequestDownload                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus RequestDownload(u16 *puwLen, u8 *aubUdsData)
{
    //*puwLen = 2;
    return UDS_ACK;
}
/* - End RequestDownload                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: TransferData                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus TransferData(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End TransferData                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: RequestTransferExit                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service callback                        */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus RequestTransferExit(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End RequestTransferExit                                                     */


/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: VINDataIdentifier                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service_DID callback                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus VINDataIdentifier_2E(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    u8 result = 0;
    if(SecurityLevel1_Access == TRUE)
    {
        *puwLen = 3;
        result = WriteParaToFlash(puwLen, aubUdsData, FLASH_OFFSET_ADDR_VIN, 17);
        if(result == 0)
        {
            ret = UDS_ACK;
        }
        else
        {
            ret = UDS_NRC_72;
        }
    }
    else
    {
        ret = UDS_NRC_33;
    }
    return ret;
}
/* - End VINDataIdentifier                                                         */
/*-------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: VINDataIdentifier                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service_DID callback                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus VINDataIdentifier(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    u8 result = 0;
    *puwLen = 3+17;
    result = ReadParaFromFlash(puwLen, aubUdsData,FLASH_OFFSET_ADDR_VIN,17);
    if(result == 0)
    {
        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_72;
    }
    return ret;
}


//=================================================================================================
// [Function Name] tUdsStatus systemSupplierECUHardwareNumber(u16 *puwLen, u8 *aubUdsData)
// [Parameters] puwLen aubUdsData
// [Introduction] Diag 22 F192 HardWare
// [Return value] Diagnostic feedback
// [Creator] 20240327 zzy
//=================================================================================================
tUdsStatus systemSupplierECUHardwareNumber(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3+16;

    for(u8 i=0;i<16;i++)
    {
        aubUdsData[i+3] = HardWare[i];
    }

    return UDS_ACK;
}


//=================================================================================================
// [Function Name] tUdsStatus systemSupplierECUSoftwareNumber(u16 *puwLen, u8 *aubUdsData)
// [Parameters] puwLen aubUdsData
// [Introduction] Diag 22 F194 SoftWare
// [Return value] Diagnostic feedback
// [Creator] 20240327 zzy
//=================================================================================================
tUdsStatus systemSupplierECUSoftwareNumber(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3+16;

    for(u8 i=0;i<16;i++)
    {
        aubUdsData[i+3] = SoftWare[i];
    }

    return UDS_ACK;
}

/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ECUManufacturingDataDataIdentifier                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service_DID callback                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ECUManufacturingDataDataIdentifier(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3+4;

    for(u8 i=0;i<4;i++)
    {
        aubUdsData[i+3] = SupplManDate[i];
    }

    return UDS_ACK;
}
/* - End ECUManufacturingDataDataIdentifier                                               */



/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: vehicleManufacturerSparePartNumber                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service_DID callback                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus vehicleManufacturerSparePartNumber(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3+11;

    for(u8 i=0;i<11;i++)
    {
        aubUdsData[i+3] = PartNumber[i];
    }

    return UDS_ACK;
}
/* - End vehicleManufacturerSparePartNumber                                      */

/*********************************************************************************/
/* Name: RepairShopCodeOrTesterSerialNumberDataIdebtifier                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service_DID callback                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus RepairShopCodeOrTesterSerialNumberDataIdebtifier(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    u8 result = 0;
    *puwLen = 3+16;

    result = ReadParaFromFlash(puwLen, aubUdsData, FLASH_OFFSET_ADDR_REPAIR_OR_TESTER_SN, 16);

    if(result == 0)
    {
        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_72;
    }
    return ret;
}


tUdsStatus RepairShopCodeOrTesterSerialNumberDataIdebtifier_2E(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    u8 result = 0;
    if(SecurityLevel1_Access == TRUE)
    {
        *puwLen = 3;
        result = WriteParaToFlash(puwLen, aubUdsData, FLASH_OFFSET_ADDR_REPAIR_OR_TESTER_SN, 16);
        if(result == 0)
        {
            ret = UDS_ACK;
        }
        else
        {
            ret = UDS_NRC_72;
        }
    }
    else
    {
        ret = UDS_NRC_33;
    }
    return ret;
}
/* - End RepairShopCodeOrTesterSerialNumberDataIdebtifier                                                         */
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ECUInstallationDataDataIdentifier                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service_DID callback                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ECUInstallationDataDataIdentifier(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    u8 result = 0;
    *puwLen = 3+4;
    result = ReadParaFromFlash(puwLen, aubUdsData,FLASH_OFFSET_ADDR_ECU_INSTALLATION_DATE,4);
    if(result == 0)
    {
        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_72;
    }
    return ret;
}
/* - End ECUInstallationDataDataIdentifier                                       */


//=================================================================================================
// [Function Name] tUdsStatus vehicleManufacturerECUSoftwareNumberDataIdentifier(u16 *puwLen, u8 *aubUdsData)
// [Parameters] puwLen aubUdsData
// [Introduction] Diag 22 F188
// [Return value] Diagnostic feedback
// [Creator] 20240327 zzy
//=================================================================================================
tUdsStatus vehicleManufacturerECUSoftwareNumberDataIdentifier(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3+8;

    for(u8 i=0;i<8;i++)
    {
        aubUdsData[i+3] = F188_VMECUSN[i];
    }

    return UDS_ACK;
}


//=================================================================================================
// [Function Name] tUdsStatus vehicleManufacturerECUHardwareNumberDataIdentifier(u16 *puwLen, u8 *aubUdsData)
// [Parameters] puwLen aubUdsData
// [Introduction] Diag 22 F191
// [Return value] Diagnostic feedback
// [Creator] 20240327 zzy
//=================================================================================================
tUdsStatus vehicleManufacturerECUHardwareNumberDataIdentifier(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3+8;

    for(u8 i=0;i<8;i++)
    {
        aubUdsData[i+3] = F198_VMECUHN[i];
    }

    return UDS_ACK;
}


//=================================================================================================
// [Function Name] tUdsStatus ECUFunctionConfiguration(u16 *puwLen, u8 *aubUdsData)
// [Parameters] puwLen aubUdsData
// [Introduction] Diag 22 F101
// [Return value] Diagnostic feedback
// [Creator] 20240517 zzy
//=================================================================================================
tUdsStatus ECUFunctionConfiguration(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    u8 result = 0;
    *puwLen = 3+4;
    result = ReadParaFromFlash(puwLen, aubUdsData,FLASH_OFFSET_ADDR_ECUCfg, 4);
    if(result == 0)
    {
        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_72;
    }
    return ret;
}


//=================================================================================================
// [Function Name] tUdsStatus ECUFunctionConfiguration_2E(u16 *puwLen, u8 *aubUdsData)
// [Parameters] puwLen aubUdsData
// [Introduction] Diag 2E F101
// [Return value] Diagnostic feedback
// [Creator] 20240517 zzy
//=================================================================================================
tUdsStatus ECUFunctionConfiguration_2E(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    u8 result = 0;
    if(SecurityLevel1_Access == TRUE)
    {
        *puwLen = 3;
        result = WriteParaToFlash(puwLen, aubUdsData, FLASH_OFFSET_ADDR_ECUCfg, 4);
        if(result == 0)
        {
            if((aubUdsData[3] == 0x00) || (aubUdsData[3] == 0xFF))
            {
                Diag_Time_Pull2 = 12;
            }
            else
            {
                Diag_Time_Pull2 = aubUdsData[3];
            }
            
            if((aubUdsData[4] == 0x00) || (aubUdsData[4] == 0xFF))
            {
                Diag_Time_Stuck = 20;
            }
            else
            {
                Diag_Time_Stuck = aubUdsData[4];
            }

            ret = UDS_ACK;
        }
        else
        {
            ret = UDS_NRC_72;
        }
    }
    else
    {
        ret = UDS_NRC_33;
    }
    return ret;
}


/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ECUInstallationDataDataIdentifier_2E                                    */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service_DID callback                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ECUInstallationDataDataIdentifier_2E(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    u8 result = 0;

    u8 i = 0;
    u8 data[4] = {0};
    u8 data_H4[4] = {0};
    u8 data_L4[4] = {0};

    if(SecurityLevel1_Access == TRUE)
    {
        *puwLen = 3;

        // check BCD and date format
        for(i = 0; i < 4; i++)
        {
            data[i] = *(aubUdsData + 3 + i);
            data_L4[i] = data[i] & 0x0f;
            data_H4[i] = (u8)((data[i] & 0xf0) >> 4);

            // check BCD
            if((data_L4[i] >= 0x0A) || (data_H4[i] >= 0x0A))
            {
                ret = UDS_NRC_31;
                break;
            }
            // check Month format
            else if(data[2] > 0x12)
            {
                ret = UDS_NRC_31;
                break;
            }
            // check Day format
            else if(data[3] > 0x31)
            {
                ret = UDS_NRC_31;
                break;
            }
            else
            {
                // check pass
            }
        }

        if(ret != UDS_NRC_31)
        {
            result = WriteParaToFlash(puwLen, aubUdsData, FLASH_OFFSET_ADDR_ECU_INSTALLATION_DATE, 4);
            
            if(result == 0)
            {
                ret = UDS_ACK;
            }
            else
            {
                ret = UDS_NRC_72;
            }
        }
    }
    else
    {
        ret = UDS_NRC_33;
    }

    return ret;
}
/* - End ECUInstallationDataDataIdentifier_2E                                    */
/*-------------------------------------------------------------------------------*/


//=================================================================================================
// [Function Name] tUdsStatus SwaybarControl(u16 *puwLen, u8 *aubUdsData)
// [Parameters] puwLen aubUdsData
// [Introduction] Diag 2F F011 0x03
// [Return value] Diagnostic feedback
// [Creator] 20240409 zzy
//=================================================================================================
tUdsStatus SwaybarControl(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(aubUdsData[3] == 0x00)
    {
        // N/A
        ret = UDS_NRC_31;
    }
    else if(aubUdsData[3] == 0x03)
    {
        *puwLen = 5;
        Diag_SwaybarControlEn  = (u8)((aubUdsData[4] & 0x80) >> 7);
        Diag_SwaybarControlSts = (u8)(aubUdsData[4] & 0x7F);
    }
    else
    {
        *puwLen = 3;
        ret = UDS_NRC_31;
    }

    return ret;
}


/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: systemSupplierIdentifierDataIdentifier                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service_DID callback                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus systemSupplierIdentifierDataIdentifier(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3+10;

    for(u8 i=0;i<10;i++)
    {
        aubUdsData[i+3] = SupplierID[i];
    }

    return UDS_ACK;
}
/* - End systemSupplierIdentifierDataIdentifier                                                         */
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ECUSeriaNumberDataIdentifier                                            */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Service_DID callback                    */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ECUSeriaNumberDataIdentifier(u16 *puwLen, u8 *aubUdsData)
{
    *puwLen = 3+10;
    for(u8 i=0;i<10;i++)
    {
        aubUdsData[i+3] = ECUSerialNumber[i];
    }
    return UDS_ACK;
}
/* - End ECUSeriaNumberDataIdentifier                                                         */
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: EraseMemory_Start                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus EraseMemory_Start(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End EraseMemory_Start                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: EraseMemory_Stop                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus EraseMemory_Stop(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End EraseMemory_Stop                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: EraseMemory_Result                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus EraseMemory_Result(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End EraseMemory_Result                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: CheckProgrammingDependence_Start                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus CheckProgrammingDependence_Start(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End CheckProgrammingDependence_Start                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: CheckProgrammingDependence_Stop                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus CheckProgrammingDependence_Stop(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End CheckProgrammingDependence_Stop                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: CheckProgrammingDependence_Result                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus CheckProgrammingDependence_Result(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End CheckProgrammingDependence_Result                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: CheckProgrammingPreconditions_Start                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus CheckProgrammingPreconditions_Start(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eStatus = UDS_ACK;
    *puwLen = 5;
    aubUdsData[4] = 0x00;

    Preconditions = TRUE;

    return(eStatus);
}
/* - End CheckProgrammingPreconditions_Start                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: CheckProgrammingPreconditions_Stop                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus CheckProgrammingPreconditions_Stop(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End CheckProgrammingPreconditions_Stop                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: CheckProgrammingPreconditions_Result                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus CheckProgrammingPreconditions_Result(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End CheckProgrammingPreconditions_Result                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: BT_CAN_Mapping_Start                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus BT_CAN_Mapping_Start(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    *puwLen = 4;
    if(SecurityLevel1_Access == TRUE)
    {
        //*puwLen = 1;
        //Bd_Can_Maping_To_Obd_Fun_Enable = 1;
        //Pt_Can_Maping_To_Obd_Fun_Enable = 0;
        //St_Can_Maping_To_Obd_Fun_Enable = 0;

        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_33;
    }
    return ret;
}
/* - End BT_CAN_Mapping_Start                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: BT_CAN_Mapping_Stop                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus BT_CAN_Mapping_Stop(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    *puwLen = 4;
    if(SecurityLevel1_Access == TRUE)
    {
        //*puwLen = 1;
        //Bd_Can_Maping_To_Obd_Fun_Enable = 0;
        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_33;
    }
    return ret;
}
/* - End BT_CAN_Mapping_Stop                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: BT_CAN_Mapping_Result                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus BT_CAN_Mapping_Result(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End BT_CAN_Mapping_Result                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: PT_CAN_Mapping_Start                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus PT_CAN_Mapping_Start(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;

    if(SecurityLevel1_Access == TRUE)
    {
        *puwLen = 4;
        //Pt_Can_Maping_To_Obd_Fun_Enable = 1;
        //Bd_Can_Maping_To_Obd_Fun_Enable = 0;
        //St_Can_Maping_To_Obd_Fun_Enable = 0;
        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_33;
    }
    return ret;
}
/* - End PT_CAN_Mapping_Start                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: PT_CAN_Mapping_Stop                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus PT_CAN_Mapping_Stop(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    *puwLen = 4;
    if(SecurityLevel1_Access == TRUE)
    {
        //*puwLen = 1;
        //Pt_Can_Maping_To_Obd_Fun_Enable = 0;
        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_33;
    }
    return ret;
}
/* - End PT_CAN_Mapping_Stop                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: PT_CAN_Mapping_Result                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus PT_CAN_Mapping_Result(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End PT_CAN_Mapping_Result                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ST_CAN_Mapping_Start                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ST_CAN_Mapping_Start(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    *puwLen = 4;
    if(SecurityLevel1_Access == TRUE)
    {
        //*puwLen = 1;
        //St_Can_Maping_To_Obd_Fun_Enable = 1;
        //Bd_Can_Maping_To_Obd_Fun_Enable = 0;
        //Pt_Can_Maping_To_Obd_Fun_Enable = 0;
        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_33;
    }
    return ret;
}
/* - End ST_CAN_Mapping_Start                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ST_CAN_Mapping_Stop                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ST_CAN_Mapping_Stop(u16 *puwLen, u8 *aubUdsData)
{
    u8 ret = UDS_ACK;
    *puwLen = 4;
    if(SecurityLevel1_Access == TRUE)
    {
        //*puwLen = 1;
        //St_Can_Maping_To_Obd_Fun_Enable = 0;
        ret = UDS_ACK;
    }
    else
    {
        ret = UDS_NRC_33;
    }
    return ret;
}
/* - End ST_CAN_Mapping_Stop                                                         */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: ST_CAN_Mapping_Result                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called when UDS Routine control callback                */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : puwLen / aubUdsData                               */
/* Output parameters         : None                                              */
/* Return value              : UDS status                                        */
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
tUdsStatus ST_CAN_Mapping_Result(u16 *puwLen, u8 *aubUdsData)
{
    return UDS_ACK;
}
/* - End ST_CAN_Mapping_Result                                                         */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: APP_UdsSessionStatusInd                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by DIAG when the ECU enters or goes out from     */
/*              a Diagnostic session                                             */
/*********************************************************************************/
/* Input parameters          : eDiagSessStatus: DIAG session status              */
/* Output parameters         : None                                              */
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

void APP_UdsSessionStatusInd (tUdsSessionType eUdsSessType)
{
/* No dummy code here because for some compilers provoks an ERROR */ 
/* QAC warning accepted for this case */

    if(eUdsSessType == UDS_SESSION_DEFAULT)
    {
        // NM_RxAndTx_Enable = TRUE;
        NM_Message_Transmit_Flg = TRUE;
        NM_Message_Receive_Flg = TRUE;
        COM_Periodic (COM_PERIODIC_START);
    }
    else if(eUdsSessType == UDS_SESSION_PROGRAMMING)
    {
        //IL_u8Wr_BCM_Bat_Voltage(0x02);
    }
    else if(eUdsSessType == UDS_SESSION_EXTENDED_DIAG)
    {
        //IL_u8Wr_BCM_Bat_Voltage(0x03);
    }
    else
    {
        /*do nothing*/
    }

}

/* - End APP_UdsSessionStatusInd                                                 */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: APP_TpDataBufferEmptyInd                                                */
/*-------------------------------------------------------------------------------*/
/* Description: Call TP buffer is totaly transmitted and need to be reload       */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
void APP_TpDataBufferEmptyInd (tTpMsgIdx uMsgIdx)
{
/* No dummy code here because for some compilers provoks an ERROR */ 
/* QAC warning accepted for this case */
}
/* - End APP_TpDataBufferEmptyInd                                                */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: APP_TpTxConf                                                            */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by TP when a transmission is complete (OK or NOK)*/
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                             ebStatus: TP message status                       */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*********************************************************************************/
void APP_TpTxConf (tTpMsgIdx uMsgIdx, u8 ebStatus)
{
    /* No dummy code here because for some compilers provoks an ERROR */ 
    /* QAC warning accepted for this case */
}
/* - End APP_TpTxConf                                                            */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: APP_TpRxInd                                                             */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by TP when a reception is complete (OK or NOK)   */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                             ebStatus: TP message status                       */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*********************************************************************************/
void APP_TpRxInd (tTpMsgIdx uMsgIdx, u8 ebStatus)
{
/* No dummy code here because for some compilers provoks an ERROR */ 
/* QAC warning accepted for this case */
}
/* - End APP_TpRxInd                                                             */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: APP_CanWakeUpInd                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by EB when a WAKE-UP is detected by the          */
/*              protocol handler (frame reception)                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*********************************************************************************/
#if (CAN_WAKE_UP_DETECTION  == CAN_WAKE_UP_ACTIVATE)
void APP_CanWakeUpInd (void)
{

}
#endif

/*********************************************************************************/
/* Name: APP_CanCtrlModeErrorInd                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: Error in the CAN controller during CAN_Activate and              */
/*              CAN_Desactivate                                                  */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
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
void APP_CanCtrlModeErrorInd (void)
{
    
}
/* - End APP_CanCtrlModeErrorInd                                                 */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: NCS_SdfVariableCoherencyError                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Call when a coherenrcy error on a SDF variable is detected       */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
void NCS_SdfVariableCoherencyError(u16 uwParam)
{
    /* No dummy code here because for some compilers provoks an ERROR */ 
    /* QAC warning accepted for this case */

}
/* - End NCS_SdfVariableCoherencyErroR                                           */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: NCS_SdfVariableRangeError                                               */
/*-------------------------------------------------------------------------------*/
/* Description: Call when a range error on a SDF variable is detected            */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
void NCS_SdfVariableRangeError(u16 uwParam)
{
    /* No dummy code here because for some compilers provoks an ERROR */ 
    /* QAC warning accepted for this case */
}
/* - End NCS_SdfVariableRangeError                                               */
/*-------------------------------------------------------------------------------*/

void HardFault_Handler(void)
{
    while(1)
    {
    }
}