/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : UDS_Prg.c
************************************************************************************************
*   Project/Product :
*   Title           :
*   Author          : BWFENG
************************************************************************************************
*   Description     : This is common module for body network
*
************************************************************************************************
*   Limitations     : NONE
*
************************************************************************************************
*   END_FILE_HDR*/

/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Header Files: UDS                                                           */
#include "EB_Prj.h"

/*                                                                               */
/*********************************************************************************/


/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */
u16 uwLen = 0;
u8  aubUdsData[5] = {0};
/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */


/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */




/*                                                                               */
/*********************************************************************************/




/***************************** PRIVATE DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Private Constants                                                           */

/* Routine control type */
#define UDS_START_ROUTINE           (u8) 0x01U
#define UDS_STOP_ROUTINE            (u8) 0x02U
#define UDS_REQUEST_ROUTINE_RESULT  (u8) 0x03U

#if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
#define UDS_SUPPRESS_POS_RESP_MASK  (u8) 0x7FU
#else
#define UDS_SUPPRESS_POS_RESP_MASK  (u8) 0xFFU
#endif

/*-------------------------------------------------------------------------------*/
/* - Private Types                                                               */


/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */

#ifndef NCS_UNITARY_TEST
/* Current session code (DSC service parameter) */
NCS_PRI_VAR tUdsSessionProtType m_eSessionCode;
#else
/* Current session code (DSC service parameter) */
tUdsSessionProtType m_eSessionCode;
#endif

#ifndef NCS_UNITARY_TEST
/* Current session mask
   Range : (UDS_SESSION_DEFAULT_MASK, UDS_SESSION_EXTENDED_DIAG,
    UDS_SESSION_PROGRAMMING_MASK) */
NCS_PRI_VAR tUdsSessionProtMask m_eSessionMask;
#else
tUdsSessionProtMask m_eSessionMask;
#endif

/* Sessions time counter, to limit a Diagnostic session to UDS_SESS_TIMEOUT */
NCS_PRI_VAR tUdsSessionTimeout m_uSessionTimeCnt;

#if (UDS_SRV_CFG2_MAX)
/* Current data identifier */
NCS_PRI_VAR u16 m_uwDid;
#endif

/* Current data Local identifier */
#if (UDS_SRV_CFG4_MAX)
NCS_PRI_VAR u8 m_ubLid;
#endif

/* Current routine identifier */
#if (UDS_SRV_CFG3_MAX)
    #if (UDS_EB_CAN_TYPE == RSA_MIXED)
        NCS_PRI_VAR u8 m_uwRi;
    #else
        NCS_PRI_VAR u16 m_uwRi;
    #endif
#endif

/* Current service index of the Diagnostic services configuration array */
NCS_PRI_VAR tUdsSrvIdx m_uSrvIdx;


NCS_PRI_VAR tUdsBoolean m_eSuppresPosRspMsgIndicationBit;


/* Current routine */
#if (UDS_SRV_CFG3_MAX)
    NCS_PRI_VAR tRoutine m_stRoutine;
#endif

/* Current start service index */
NCS_PRI_VAR tUdsSrvIdx m_uStartSrvIdx;

/* Current start sub-service index */
NCS_PRI_VAR tUdsSrvIdx m_uStartSubSrvIdx;

/* addressing mode */
NCS_PRI_VAR tUdsAddrMode m_eUdsAddrMode;

/* Long request behavior enabling */
typedef struct
{
     tUdsLongRequestState m_eState;        /* Sequence step */
     u16 m_ubCnt;                        /* Counter for periodic transmissions */
     u8 m_aubDiagNack78Data[UDS_LEN_NRC]; /* Data buffer for NRC78 only */
     u8 *m_paubData;                    /* Pointer to the TP data buffer */
     u16 m_uwLen;                        /* Final response length */
     tUdsBoolean m_ubtRespPending;         /* A response is pending. Must wait for TxConf before any other response transmission. */
} tUdsLongRequestStr;

NCS_PRI_VAR tUdsLongRequestStr strLongRequest;

/*-------------------------------------------------------------------------------*/
/* - Private Macros                                                              */

/*-------------------------------------------------------------------------------*/
/* - Private Function Prototypes                                                 */

#if (UDS_SRV_CFG1_MAX)
NCS_PRI_FCT tUdsStatus UDS_FilteringCfg1(u16 *puwLen, u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering1(u16 *puwLen, const u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_SubServiceFiltering1(u16 *puwLen, const u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_SessionFiltering1 (const u16 *puwLen, const u8 *aubUdsData);
#endif
#if (UDS_SRV_CFG2_MAX)
NCS_PRI_FCT tUdsStatus UDS_FilteringCfg2(u16 *puwLen, u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering2(u16 *puwLen, const u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_SessionFiltering2 (u16 *puwLen, const u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_DidFiltering(u16 *puwLen, const u8 *aubUdsData);
#endif
#if (UDS_SRV_CFG3_MAX)
    NCS_PRI_FCT tUdsStatus UDS_FilteringCfg3(u16 *puwLen, u8 *aubUdsData);
    NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering3(u16 *puwLen, const u8 *aubUdsData);
    NCS_PRI_FCT tUdsStatus UDS_SessionFiltering3 (const u16 *puwLen, const u8 *aubUdsData);
    NCS_PRI_FCT tUdsStatus UDS_RiFiltering(u16 *puwLen, const u8 *aubUdsData);
    NCS_PRI_FCT tUdsStatus UDS_SubServiceFiltering3(u16 *puwLen, const u8 *aubUdsData);
  #if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
    NCS_PRI_FCT tUdsStatus UDS_StartRoutineControl(u16 *puwLen, u8 *aubUdsData);
    NCS_PRI_FCT tUdsStatus UDS_StopRoutineControl(u16 *puwLen, u8 *aubUdsData);
    NCS_PRI_FCT tUdsStatus UDS_GetRoutineStatus(u16 *puwLen, u8 *aubUdsData);
  #elif ( UDS_EB_CAN_TYPE == RSA_MIXED )
    NCS_PRI_FCT tUdsStatus UDS_RoutineByLocalIdentifier(u16 *puwLen, u8 *aubUdsData);
  #endif
#endif
#if (UDS_SRV_CFG4_MAX)
NCS_PRI_FCT tUdsStatus UDS_FilteringCfg4(u16 *puwLen, u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering4(u16 *puwLen, const u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_SessionFiltering4 (u16 *puwLen, const u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_LidFiltering(u16 *puwLen, const u8 *aubUdsData);
#endif

#if (UDS_SRV_CFG5_MAX)
NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering5(u16 *puwLen, u8 *aubUdsData);
NCS_PRI_FCT tUdsStatus UDS_FilteringCfg5(u16 *puwLen, u8 *aubUdsData);
#endif

NCS_PRI_FCT tUdsBoolean UDS_NegativeAnswer (u16 *puwLen, u8 *aubUdsData, tUdsStatus eStatus);
#if ((UDS_SPREC == UDS_SPREC_ON) || (UDS_EB_CAN_TYPE == RSA_MIXED))
NCS_PRI_FCT tUdsBoolean UDS_PositiveAnswer (u16 *puwLen, u8 *aubUdsData);
#else
NCS_PRI_FCT tUdsBoolean UDS_PositiveAnswer (const u16 *puwLen, u8 *aubUdsData);
#endif
#if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
NCS_PRI_FCT tUdsBoolean UDS_AnswerOrNot (const u16 *puwLen, const u8 *aubUdsData);
#endif
NCS_PRI_FCT tUdsStatus UDS_DiagSession (tUdsSessionType eUdsSessionType);


/*                                                                               */
/*********************************************************************************/




/******************************** IMPLEMENTATION *********************************/
/*                                                                               */

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */


/*********************************************************************************/
/* Name: UDS_Init                                                                */
/*-------------------------------------------------------------------------------*/
/* Description: Initialize the UDS layer internal variables                      */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_eSessionCode: current session code              */
/*                             m_uSessionTimeCnt: session time counter           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

void UDS_Init (void)
{
    /* Implements 000-07-D028-D-80101-1
       At power on, the current session is "Default Diagnostic" */
    m_eSessionCode = UDS_SESSION_PROT_DEFAULT;

    m_eSessionMask = UDS_SESSION_PROT_DEFAULT_MASK;

    /* All the actives sessions are closed, there is no need to check timeout */
    m_uSessionTimeCnt = 0U;

    /* Initialize long request variable */
    strLongRequest.m_eState = UDS_LONG_REQ_OFF;
    strLongRequest.m_ubCnt = 0U;
    strLongRequest.m_paubData = NULL;
    strLongRequest.m_uwLen = 0U;
    strLongRequest.m_ubtRespPending = UDS_FALSE;

    /* Initialize addressing mode */
    m_eUdsAddrMode = UDS_ADDR_PHYSICAL;

#if (UDS_SRV_CFG3_MAX)
    /* Initialize variable containing the status of the current routine
       and a flag indicating if startup routine hase been requested */
    m_stRoutine.uwRi = 0U; 
    m_stRoutine.eStart = UDS_ROUTINE_INIT; 
    m_stRoutine.eStatus = UDS_ROUTINE_INIT; 
#endif
}


/* - End UDS_Init                                                                */
/*-------------------------------------------------------------------------------*/
#if 0
static void RKE_Match_Process(void)
{
    if(strLongRequest.m_eState == UDS_LONG_REQ_ON)
    {
        Tester_Absent_Flg = FALSE;
    }
    else
    {
        /*do nothing*/
    }
    if(ChkSoftTimerState(RKE1_Learned_Timer) == SOFT_TIMER_ST_REACH)
    {
        uwLen = 5;
        *(aubUdsData+0) = 0x6F;
        *(aubUdsData+1) = 0x58;
        *(aubUdsData+2) = 0xA7;
        *(aubUdsData+3) = 0x03;
        *(aubUdsData+4) = 0xF3;
        SoftTimerStop(RKE1_Learned_Timer);
        RKE1LearnedFlg = FALSE;
        strLongRequest.m_paubData = aubUdsData;
        strLongRequest.m_uwLen = uwLen;
        strLongRequest.m_eState = UDS_LONG_REQ_END;
    }
    else if(ChkSoftTimerState(RKE2_Learned_Timer) == SOFT_TIMER_ST_REACH)
    {
        uwLen = 5;
        *(aubUdsData+0) = 0x6F;
        *(aubUdsData+1) = 0x58;
        *(aubUdsData+2) = 0xA7;
        *(aubUdsData+3) = 0x03;
        *(aubUdsData+4) = 0xF3;
        SoftTimerStop(RKE2_Learned_Timer);
        RKE2LearnedFlg = FALSE;
        strLongRequest.m_paubData = aubUdsData;
        strLongRequest.m_uwLen = uwLen;
        strLongRequest.m_eState = UDS_LONG_REQ_END;
    }
    else
    {
        if((Rke_Var.RKE_State == 0xF1)&&(RKE1LearnedFlg == TRUE))
        {
            SoftTimerStop(RKE1_Learned_Timer);
            RKE1LearnedFlg = FALSE;
            uwLen = 5;
            *(aubUdsData+0) = 0x6F;
            *(aubUdsData+1) = 0x58;
            *(aubUdsData+2) = 0xA7;
            *(aubUdsData+3) = 0x03;
            *(aubUdsData+4) = 0xF1;
            strLongRequest.m_paubData = aubUdsData;
            strLongRequest.m_uwLen = uwLen;
            strLongRequest.m_eState = UDS_LONG_REQ_END;
        }
        else if((Rke_Var.RKE_State == 0xF2)&&(RKE2LearnedFlg == TRUE))
        {
            SoftTimerStop(RKE2_Learned_Timer);
            RKE2LearnedFlg = FALSE;
            uwLen = 5;
            *(aubUdsData+0) = 0x6F;
            *(aubUdsData+1) = 0x58;
            *(aubUdsData+2) = 0xA7;
            *(aubUdsData+3) = 0x03;
            *(aubUdsData+4) = 0xF2;
            strLongRequest.m_paubData = aubUdsData;
            strLongRequest.m_uwLen = uwLen;
            strLongRequest.m_eState = UDS_LONG_REQ_END;
        }
        else
        {
            /*do nothing*/
        }
    }
}
#endif
/*********************************************************************************/
/* Name: UDS_RxRequest                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Realize the treatment of a received Diagnostic request           */
/*              Filter the request according the service value                   */
/*              If the request length = 0 or the filter function return a        */
/*              negative code, a negative answer is automatically transmitted    */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsBoolean : UDS_TRUE if *puwLen !=0 else        */
/*                                           UDS_FALSE                           */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_eSessionCode: current session code              */
/*                             m_uSessionTimeCnt: session time counter           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : UDS_FilteringCfg1,UDS_FilteringCfg2,              */
/*                             UDS_FilteringCfg2,UDS_NegativeAnswer              */
/*                             UDS_PositiveAnswer,UDS_AnswerOrNot                */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

tUdsBoolean UDS_RxRequest (u16 *puwLen, u8 *aubUdsData)
{
    tUdsBoolean eUdsBoolean = UDS_TRUE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        /* When the returned length is equal to NULL, or aubUdsData = NULL
           return UDS_FALSE */
        eUdsBoolean = UDS_FALSE;
    }
    
    else
    {
        /* call generic function with default parameter */
        eUdsBoolean = UDS_RxRequestWithAddrMode (puwLen, aubUdsData, UDS_ADDR_PHYSICAL, UDS_TYPE_DIAG_OBD);
    }
    
   return eUdsBoolean;

}

/* - End UDS_RxRequest                                                           */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: UDS_RxRequestWithAddrMode                                               */
/*-------------------------------------------------------------------------------*/
/* Description: Realize the treatment of a received Diagnostic request but with  */
/*              the notion of physical and functionnal behaviour                 */
/*              Filter the request according the service value                   */
/*              If the request length = 0 or the filter function return a        */
/*              negative code, a negative answer is sytematically transmitted    */
/*              when physical behavior is activate. A negative answer is         */
/*              transmitted in specific case when functional behavior is active  */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/*                             eUdsAddrMode: addressing mode                     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsBoolean : UDS_TRUE if *puwLen !=0 else        */
/*                                           UDS_FALSE                           */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_eSessionCode: current session code              */
/*                             m_uSessionTimeCnt: session time counter           */
/*                             m_eUdsAddrMode: physical/functionnal addressing   */
/*-------------------------------------------------------------------------------*/
/* Called functions          : UDS_FilteringCfg1,UDS_FilteringCfg2,              */
/*                             UDS_FilteringCfg2,UDS_NegativeAnswer              */
/*                             UDS_PositiveAnswer,UDS_AnswerOrNot                */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

tUdsBoolean UDS_RxRequestWithAddrMode (u16 *puwLen, u8 *aubUdsData, tUdsAddrMode eUdsAddrMode, tUdsAddrType eUdsAddrType)
{
    tUdsBoolean eUdsBoolean;
    eUdsBoolean = UDS_TRUE;

    m_eSuppresPosRspMsgIndicationBit = UDS_FALSE;
    m_eUdsAddrMode = eUdsAddrMode; /* set mode to physical or functional */
    
    /* Implements 000-07-D028-D-80602-1
       If the received request is empty, no answer (it is ignored) */
    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        /* When the returned length is equal to NULL, or aubUdsData = NULL
           return UDS_FALSE */
        eUdsBoolean = UDS_FALSE;
    }

    /* If the received request is not empty, treatment */
    else
    {

        /* UDS status of the different fonction */
        tUdsStatus eUdsStatus;
        eUdsBoolean = UDS_TRUE;

        m_uSrvIdx = 0;
        m_uStartSrvIdx = 0;
        m_uStartSubSrvIdx = 0;

        /* Implements 000-07-D028-D-80601-1
           If a session is open, a valid request reloads session timeout */
        switch(m_eSessionCode)
        {
            case UDS_SESSION_PROT_DEFAULT :
                /* Nothing to do */
            break;

            case UDS_SESSION_PROT_OTHER_01:
            case UDS_SESSION_PROT_OTHER_02:
            case UDS_SESSION_PROT_OTHER_03:
            case UDS_SESSION_PROT_OTHER_04:
            case UDS_SESSION_PROT_PROGRAMMING :
            case UDS_SESSION_PROT_EXTENDED_DIAG :
            case UDS_SESSION_PROT_SUPPLIER :

            {
                /* Protection of the session time counter reload
               (this allow the UDS_Manage () callback to interrupt
               the counter reload without disabling interrupts) */
                tUdsSessionProtType eSessionCodeOld;
                eSessionCodeOld = m_eSessionCode;

                m_eSessionCode = UDS_SESSION_PROT_DEFAULT;

                /* The session timeout is reloaded */
                m_uSessionTimeCnt = m_uUdsSessTimeout;

                /* Memorise the asked session code */
                m_eSessionCode = eSessionCodeOld;
            }
            break;

            default :
                /* Indicate of range error */
                m_eSessionCode = m_eSessionCodeDefaultValue;
                NCS_SdfVariableRangeError (SDF_UDS_SESSION_CODE);
                eUdsBoolean = UDS_FALSE;
            break;


        }

        #if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
        if(eUdsBoolean == UDS_TRUE)
        {
            /* Check if it is necessary to send the positive answer */
            switch (aubUdsData[0])
            {
                case UDS_SRV_DSC:
                case UDS_SRV_ER:
                case UDS_SRV_SA:
                case UDS_SRV_TP:
                case UDS_SRV_CDTCS:
                case UDS_SRV_LC:
                case UDS_SRV_RC:
                case UDS_SRV_CC:
                    eUdsBoolean = UDS_AnswerOrNot(puwLen,aubUdsData);
                    break;
                    
                default:
                    break;
            }
        }
        else
        {
            /* Nothing do do */
        }
        #endif

        if(eUdsBoolean == UDS_TRUE)
        {
            /* Manage the filtering of the different request
               according the service                         */
            switch (aubUdsData[0])
            {
                case UDS_SRV_DSC:
                case UDS_SRV_ER:
                case UDS_SRV_SA:
                case UDS_SRV_TP:
                case UDS_SRV_CDTCS:
                case UDS_SRV_RMBA:
                case UDS_SRV_WMBA:
                case UDS_SRV_CDTCI:
                case UDS_SRV_RDTCI:
                case UDS_SRV_RD:
                case UDS_SRV_RU:
                case UDS_SRV_TD:
                case UDS_SRV_RTE:
                case UDS_SRV_LC:
                case UDS_SRV_CC:
                    if ((eUdsAddrType == UDS_TYPE_DIAG) || (eUdsAddrType == UDS_TYPE_DIAG_OBD))
                    {
#if (UDS_SRV_CFG1_MAX)
                        eUdsStatus = UDS_FilteringCfg1(puwLen,aubUdsData);
#else 
                        eUdsStatus = UDS_NRC_11;
#endif
                    }
                    else
                    {
                        eUdsStatus = UDS_NRC_11;
                    }
                    break;

            #if (UDS_SRV_CFG2_MAX)
                case UDS_SRV_RDBI:
                case UDS_SRV_IOCBI:
                case UDS_SRV_WDBI:
                case UDS_SRV_RSDBI:
                    if ((eUdsAddrType == UDS_TYPE_DIAG) || (eUdsAddrType == UDS_TYPE_DIAG_OBD))
                    {
                        eUdsStatus = UDS_FilteringCfg2(puwLen,aubUdsData);
                    }
                    else
                    {
                        eUdsStatus = UDS_NRC_11;
                    }
                    break;
            #endif

            #if (UDS_SRV_CFG3_MAX)
                case UDS_SRV_RC: /* UDS_SRV_SRBLID has the same value than UDS_SRV_RC, no case for this one! */
                case UDS_SRV_SPRBLID:
                    if ((eUdsAddrType == UDS_TYPE_DIAG) || (eUdsAddrType == UDS_TYPE_DIAG_OBD))
                    {
                        eUdsStatus = UDS_FilteringCfg3(puwLen,aubUdsData);
                    }
                    else
                    {
                        eUdsStatus = UDS_NRC_11;
                    }
                    break;
            #endif

            #if (UDS_SRV_CFG4_MAX)
                case UDS_SRV_RDBLID:
                case UDS_SRV_IOCBLID:
                case UDS_SRV_WDBLID:
                case UDS_SRV_DLIDDY:
                    if ((eUdsAddrType == UDS_TYPE_DIAG) || (eUdsAddrType == UDS_TYPE_DIAG_OBD))
                    {
                        eUdsStatus = UDS_FilteringCfg4(puwLen,aubUdsData);
                    }
                    else
                    {
                        eUdsStatus = UDS_NRC_11;
                    }
                    break;
            #endif

            #if (UDS_SRV_CFG5_MAX)
                case UDS_SRV_OBD_SID_00:
                case UDS_SRV_OBD_SID_01:
                case UDS_SRV_OBD_SID_02:
                case UDS_SRV_OBD_SID_03:
                case UDS_SRV_OBD_SID_04:
                case UDS_SRV_OBD_SID_05:
                case UDS_SRV_OBD_SID_06:
                case UDS_SRV_OBD_SID_07:
                case UDS_SRV_OBD_SID_08:
                case UDS_SRV_OBD_SID_09:
                case UDS_SRV_OBD_SID_10:
                case UDS_SRV_OBD_SID_11:
                case UDS_SRV_OBD_SID_12:
                case UDS_SRV_OBD_SID_13:
                case UDS_SRV_OBD_SID_14:
                case UDS_SRV_OBD_SID_15:
                    if ((eUdsAddrType == UDS_TYPE_OBD) || (eUdsAddrType == UDS_TYPE_DIAG_OBD))
                    {
                        eUdsStatus = UDS_FilteringCfg5(puwLen,aubUdsData);
                    }
                    else
                    {
                        eUdsStatus = UDS_NRC_11;
                    }

                    break;
            #endif

                default:
                    eUdsStatus = UDS_NRC_11;
                    break;
            }

            /* Implements 000-07-D028-D-80603-1
               Fill the buffer data and length with negative code */
            if(eUdsStatus != UDS_ACK)
            {
                /* If there is SDF error desactive the ECU response */
                if (eUdsStatus == UDS_ERR_COHE)
                {
                    *puwLen = 0U;
                }
                /* OSCUDS-96 : If a response pending is requested by the application */
                /* set the returned value with UDS_NRC_78 to indicate to the EB layer */
                /* that the Rx segmentation channel must be locked to avoid interlacing */
                /* The Rx segmentation channel will be unlocked on final response transmission. */
                else if (eUdsStatus == UDS_NRC_78)
                {
                    /* Extension : set UDS_NRC_78 instead of UDS_FALSE or UDS_TRUE */
                    eUdsBoolean = UDS_NRC_78;
                    *puwLen = 0U;
                }
                else
                {
                    eUdsBoolean = UDS_NegativeAnswer (puwLen, aubUdsData, eUdsStatus);
                }

            }
            /* Implements 000-07-D028-D-80604-1 */
            else if(m_eSuppresPosRspMsgIndicationBit == UDS_FALSE)
            {
                /* This is a positive answer, transform the service identifier */
                eUdsBoolean = UDS_PositiveAnswer(puwLen,aubUdsData);
            }
            else
            {
                /* No need to send the positive answer */
                *puwLen = 0U;
            }

        }/* End if(eUdsBoolean == UDS_TRUE) */

        else
        {
             /* No need to send the positive answer */
             *puwLen = 0U;
        }
   }

   return eUdsBoolean;
}

/* - End UDS_RxRequestWithAddrMode                                               */
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
/* - Private Functions                                                           */


#if (UDS_SRV_CFG1_MAX)
/*********************************************************************************/
/* Name: UDS_FilteringCfg1                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Realize the filtering of the service, subservice, session        */
/*              of the config 1 array structur m_astDiagSrvCfg1                  */
/*              Check the request length.                                        */
/*              The SDS and TP services are automatically managed                */
/*              If the received service is authorized, a callback is called      */
/*-------------------------------------------------------------------------------*/
/* Implements: 000-07-D028-D-80702-1                                             */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas    */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_xx)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg1                                  */
/*                             m_eSessionCode: current session code             */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : UDS_ServiceFiltering1,UDS_SubServiceFiltering1    */
/*                             UDS_SessionFiltering1,UDS_NegativeAnswer          */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
NCS_PRI_FCT tUdsStatus UDS_FilteringCfg1(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }

    else
    {
        /* Implements 000-07-D028-D-80703-1
           Service Filtering */
        eUdsStatus = UDS_ServiceFiltering1(puwLen, aubUdsData);

        if(eUdsStatus == UDS_ACK)
        {
            /* Sub-service filtering */
            switch (aubUdsData[0])
            {
                case UDS_SRV_CDTCI:  //14
                case UDS_SRV_TD:  //36
                case UDS_SRV_RD:  //34
                case UDS_SRV_RTE:  //37
                case UDS_SRV_RMBA:  //23
                case UDS_SRV_WMBA:  //3d
                    /* Nothing to do */
                    break;

                default:
                    eUdsStatus = UDS_SubServiceFiltering1(puwLen, aubUdsData);
                    break;
            }

            if(eUdsStatus == UDS_ACK)
            {
                /* Session filtering */
                switch (aubUdsData[0])
                {
                    case UDS_SRV_DSC:   //10      //add by bwf,SEM need if session mode change
                    case UDS_SRV_ER:    //11
                    case UDS_SRV_SA:    //27
                    case UDS_SRV_CDTCS:  //85
                    case UDS_SRV_RDTCI:  //19
                    case UDS_SRV_CDTCI:   //14
                    case UDS_SRV_RMBA:  //23
                    case UDS_SRV_WMBA:   //3d
                    case UDS_SRV_TP:   //3e
                    case UDS_SRV_RD:   //34
                    case UDS_SRV_RU:   //35
                    case UDS_SRV_TD:   //36
                    case UDS_SRV_RTE:  //37
                    case UDS_SRV_LC:   //87
                    case UDS_SRV_CC:   //28
                        eUdsStatus = UDS_SessionFiltering1(puwLen, aubUdsData);
                        break;

                    default:
                        /* Nothing to do */
                        break;
                }

                if(eUdsStatus == UDS_ACK)
                {
                    /* The request length is correct (1) or
                      the length must not be checked now (2)
                      (it will be checked in the request callback) */
                     if ((*puwLen == m_astDiagSrvCfg1[m_uSrvIdx].uwReqLen) ||
                                       (m_astDiagSrvCfg1[m_uSrvIdx].uwReqLen == 0U))
                     {
                        /* If the service is DSC */
                        if (aubUdsData[0] == UDS_SRV_DSC)
                        {
                            /* Indicate the received service to manage the negative
                               code UDS_NRC_22 */

                            /* Individual callback for each service */
                            #if (UDS_SRV_CALLBACK)
                            if (m_astDiagSrvCfg1[m_uSrvIdx].pfuRxMsgInd != NULL)
                            {
                            #endif
                                #if (UDS_SRV_CALLBACK)
                                    eUdsStatus =
                                    m_astDiagSrvCfg1[m_uSrvIdx].pfuRxMsgInd
                                                    (puwLen, aubUdsData);
                                /* Same callback for all the services */
                                #else
                                    eUdsStatus = UDS_RxServiceInd
                                                 (m_uSrvIdx, puwLen, aubUdsData);
                                #endif

                                /* The answer length is the same than the request
                                       one,then no modification of *puwLen */
                            #if (UDS_SRV_CALLBACK)
                            }
                            else
                            {
                                /* Nothing to do */
                            }
                            #endif
                            
                            /* Positive answer */
                            if (eUdsStatus == UDS_ACK)
                            {

                                eUdsStatus = UDS_DiagSession((tUdsSessionType)(
                                        aubUdsData[1] & UDS_SUPPRESS_POS_RESP_MASK));

                            }

                        }
                        /* For all the service of the configuration 1 array */
                        else   //change by bwf for 78,20180829
                        {
                            /* If configured, indicate the received service
                              and wait for the answer parameters
                              (length and datas) */

                            /* Individual callback for each service */
                            #if (UDS_SRV_CALLBACK)
                            if (m_astDiagSrvCfg1[m_uSrvIdx].pfuRxMsgInd
                                != NULL)
                            {
                            #endif
                                /* Indicate the received service
                                   Individual callback for each service */
                                #if (UDS_SRV_CALLBACK)
                                    eUdsStatus =
                                        m_astDiagSrvCfg1[m_uSrvIdx].pfuRxMsgInd
                                    (puwLen, aubUdsData);
                                /* Same callback for all the services */
                                #else
                                   eUdsStatus =
                                   UDS_RxServiceInd (m_uSrvIdx, puwLen, aubUdsData);
                                #endif

                                /* Long request */
                                if (UDS_NRC_78 == eUdsStatus)
                                {
                                    strLongRequest.m_aubDiagNack78Data[0] = UDS_NRC;
                                    strLongRequest.m_aubDiagNack78Data[1] = aubUdsData[0];
                                    strLongRequest.m_aubDiagNack78Data[2] = eUdsStatus;

                                    /* Set timer to send the NRC78 */
                                    #if (UDS_P2 == 0U)
                                        strLongRequest.m_ubCnt = UDS_P2;
                                    #else
                                        strLongRequest.m_ubCnt = UDS_P2 - 1U;
                                    #endif
                                    strLongRequest.m_eState = UDS_LONG_REQ_ON;

                                    /* Nothing to answer because NRC78 answer will be sent later */
                                    *puwLen = 0U;
                                }
                                 
                            /* Individual callback for each service */
                            #if (UDS_SRV_CALLBACK)
                            }
                            else
                            {
                                /* Nothing to do */
                            }
                            #endif
                        }

                     } /* if ((*puwLen == m_astDiagSrvCfg1[m_uSrvIdx].uwReqLen) ||
                                   (m_astDiagSrvCfg1[uSrvIdx].uwReqLen == 0))   */

                     /* Implements 000-07-D028-D-80701-1 */
                     else
                     {
                        /* Bad length, Negative answer */
                        #if ( UDS_EB_CAN_TYPE == RSA_MIXED )
                        eUdsStatus = UDS_NRC_12;
                        #else
                        eUdsStatus = UDS_NRC_13;
                        #endif
                     }

                } /* End if(eUdsStatus == UDS_ACK) */

                else
                {
                    /* Nothing to do */
                }


            } /* End if(eUdsStatus == UDS_ACK) */

            else
            {
                /* Nothing to do */
            }

        } /* End if(eUdsStatus == UDS_ACK) */

        else
        {
            /* Nothing to do */
        }

    }

    return eUdsStatus;

}

/* - End UDS_FilteringCfg1                                                       */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_ServiceFiltering1                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service in implemented is the configuration 1       */
/*  array                                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas    */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_11)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg1                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSrvIdx : start service index              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering1(u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;
    
    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }

    else
    {
        if(*puwLen != 0U)
        {
            /* Search the first service equal to aubUdsData[0] into the
               configuration 1 array                                       */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg1Max))
            {
                /* search for service and mode (func / phy or both) */
                if ((m_astDiagSrvCfg1[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                    ((m_astDiagSrvCfg1[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode))
                {
                    /* service/mode found */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++;
                }
            }
            /* If the service has been found return positiv answer else
               return a NCR_11
               Implements 000-07-D028-D-80801-1                        */
            if (eFlagFound == UDS_TRUE)
            {
                /* Save the start service index */
                m_uStartSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                eUdsStatus = UDS_NRC_11;
            }
        }
        else
        {
            eUdsStatus = UDS_NRC_11;
        }
    }

    return eUdsStatus;

}
/* - End UDS_ServiceFiltering1                                                   */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_SubServiceFiltering1                                                */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service/ sub-service is implemented in the          */
/*  configuration 1 array                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas    */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_12)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg1                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSubSrvIdx : start sub-service index       */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_SubServiceFiltering1(u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    u8 ubSubSrv;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;
    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        if(*puwLen >= 2U)
        {
            /* Mask the SuppresPosRspMsgIndicationBit */
            switch (aubUdsData[0])
            {
                case UDS_SRV_DSC: //10
                case UDS_SRV_ER:   //11
                /*change by bwf for $27  not support the prohibition of positive response*/
                //case UDS_SRV_SA:   //27
                case UDS_SRV_TP:   //3E
                case UDS_SRV_CDTCS: //85
                case UDS_SRV_LC:    //87
                case UDS_SRV_CC:   //28
                    ubSubSrv = (u8) (aubUdsData[1] & UDS_SUPPRESS_POS_RESP_MASK);
                    break;

                default:
                    ubSubSrv = aubUdsData[1];
                    break;
            }

            /* Search the first service/sub-service equal to aubUdsData[0]/
              aubUdsData[1] into the configuration 1 array                   */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg1Max))
            {
                /* search for service and subsrv and mode (func / phy or both) */
                if ((m_astDiagSrvCfg1[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                    (((u8)(m_astDiagSrvCfg1[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode) &&
                    (m_astDiagSrvCfg1[m_uSrvIdx].ubSubSrvId == ubSubSrv)))
                {
                    /* service/subsrv/mode are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }
            /* If the sub-service has been found return positiv answer else
               return a NCR_12
               Implements 000-07-D028-D-80901-1                        */
            if (eFlagFound == UDS_TRUE)
            {
                /* Save the start sub-service index */
                m_uStartSubSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                eUdsStatus = UDS_NRC_12;
            }
        }
        else
        {
            //eUdsStatus = UDS_NRC_12;
            /*change by bwf for SEM*/
            eUdsStatus = UDS_NRC_13;
        }
    }

    return eUdsStatus;

}
/* - End UDS_SubServiceFiltering1                                                */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: UDS_SessionFiltering1                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service/ sub-service/session is implemented in the  */
/*  configuration 1 array                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas    */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_7E or UDS_NRC_7F)  */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg1                                  */
/*                             m_uSrvIdx: current service index                  */
/*                             m_uStartSrvIdx : start service index              */
/*                             m_uStartSubSrvIdx : start sub-service index       */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_SessionFiltering1 (const u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;
    u8 ubSubSrv;
    tUdsSessionMask eSessionMask;
    
    eUdsStatus = UDS_ACK;
    eFlagFound = UDS_FALSE;
    eSessionMask = UDS_SESSION_DEFAULT_MASK;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
          /* Mask the SuppresPosRspMsgIndicationBit */
        switch (aubUdsData[0])
        {
            case UDS_SRV_DSC:
            case UDS_SRV_ER:
            case UDS_SRV_SA:
            case UDS_SRV_TP:
            case UDS_SRV_CDTCS:
            case UDS_SRV_LC:
            case UDS_SRV_CC:
                ubSubSrv = (u8) (aubUdsData[1] & UDS_SUPPRESS_POS_RESP_MASK);
                break;

            default:
                ubSubSrv = aubUdsData[1];
                break;
        }

        m_uSrvIdx = m_uStartSrvIdx;

        switch(m_eSessionMask)
        {
            case UDS_SESSION_PROT_OTHER_01_MASK:
                eSessionMask = UDS_SESSION_OTHER_01_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_02_MASK:
                eSessionMask = UDS_SESSION_OTHER_02_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_03_MASK:
                eSessionMask = UDS_SESSION_OTHER_03_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_04_MASK:
                eSessionMask = UDS_SESSION_OTHER_04_MASK;
                break;
            case UDS_SESSION_PROT_DEFAULT_MASK:
                eSessionMask = UDS_SESSION_DEFAULT_MASK;
                break;
            case UDS_SESSION_PROT_PROGRAMMING_MASK:
                eSessionMask = UDS_SESSION_PROGRAMMING_MASK;
                break;
            case UDS_SESSION_PROT_EXTENDED_DIAG_MASK:
                eSessionMask = UDS_SESSION_EXTENDED_DIAG_MASK;
                break;
            case UDS_SESSION_PROT_SUPPLIER_MASK:
                eSessionMask = UDS_SESSION_SUPPLIER_MASK;
                break;

            default :
                /* Indicate of range error */
                m_eSessionMask = m_eSessionMaskDefaultValue;
                NCS_SdfVariableRangeError (SDF_UDS_SESSION_MASK);
                eUdsStatus = UDS_ERR_COHE;
            break;
        }
        
        
        if (eUdsStatus == UDS_ACK)
        {
            /* Check if the service and session is implemented */
            while (((!eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg1Max)) &&
                   (m_astDiagSrvCfg1[m_uSrvIdx].ubSrvId == aubUdsData[0]) )
            {
                if (m_astDiagSrvCfg1[m_uSrvIdx].eSessionMasks & eSessionMask )
                {
                    /* service/session are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }

            }


            if (eFlagFound == UDS_TRUE)
            {
                switch (aubUdsData[0])
                {
                    case UDS_SRV_TD:
                    case UDS_SRV_RD:
                    case UDS_SRV_RTE:
                    case UDS_SRV_CDTCI:
                    case UDS_SRV_RMBA:
                    case UDS_SRV_WMBA:

                        /* Nothing to do */
                        break;

                    default:
                        m_uSrvIdx = m_uStartSubSrvIdx ;
                        eFlagFound = UDS_FALSE;

                         /* Check if the service/sub-service/session is implemented */
                        while (((!eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg1Max)) &&
                               (m_astDiagSrvCfg1[m_uSrvIdx].ubSubSrvId == ubSubSrv) )
                        {
                            if(m_astDiagSrvCfg1[m_uSrvIdx].eSessionMasks & eSessionMask )
                            {
                                /* service/sub-service/session are OK */
                                eFlagFound = UDS_TRUE;
                            }
                            else
                            {
                                m_uSrvIdx++ ;
                            }
                        }
                        break;
                }

                /* If service/sub-service/session are OK return positiv answer
                   Implements 000-07-D028-D-81003-1                          */
                if (eFlagFound == UDS_TRUE)
                {
                    eUdsStatus = UDS_ACK;
                }
                /* Service et Sub-service is implemented in the ECU but not in the
                   session return a NCR_7E
                   Implements 000-07-D028-D-81002-1                              */
                else
                {
                    eUdsStatus = UDS_NRC_7E;
                }
            }
            /* Service is implemented in the ECU but not in the
               session return a NCR_7F
               Implements 000-07-D028-D-81001-1               */
            else
            {
                eUdsStatus = UDS_NRC_7F;
            }
        }
    }

    return (eUdsStatus);
}


/* - End UDS_SessionFiltering1                                                   */
/*-------------------------------------------------------------------------------*/
#endif /* End #if (UDS_SRV_CFG1_MAX) */

#if (UDS_SRV_CFG2_MAX)
/*********************************************************************************/
/* Name: UDS_FilteringCfg2                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Realize the filtering of the service, DID, session               */
/*              of the config 2 array structur m_astDiagSrvCfg2                  */
/*              Check the request length.                                        */
/*              If the received service is authorized, a callback is called      */
/*-------------------------------------------------------------------------------*/
/* Implements: 000-07-D028-D-81102-1                                             */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_xx)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg1                                  */
/*                             m_eSessionCode: current session code              */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : UDS_ServiceFiltering2,UDS_DidFiltering            */
/*                             UDS_SessionFiltering2                             */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_FilteringCfg2(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;


    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        m_uwDid = (u16) (((u16)aubUdsData[1] << 8U) + ((u16)aubUdsData[2]));

        /* Implements 000-07-D028-D-81103-1
           Service Filtering */
        eUdsStatus = UDS_ServiceFiltering2(puwLen, aubUdsData);

        /*add by bwf for $22 and $2F satisfied SEM start*/
        if(*puwLen < 3 )
        {
            eUdsStatus = UDS_NRC_13;
        }
        else
        {
            /*do nothing*/
        }
        /*add by bwf for $22 and $2F satisfied SEM end*/

        if(eUdsStatus == UDS_ACK)
        {
            /* Session Filtering */
            eUdsStatus = UDS_SessionFiltering2(puwLen, aubUdsData);

            if(eUdsStatus == UDS_ACK)
            {
                /* DID Filtering */
                eUdsStatus = UDS_DidFiltering(puwLen, aubUdsData);

                if(eUdsStatus == UDS_ACK)
                {

                    /* The request length is correct (1) or
                      the length must not be checked now (2)
                      (it will be checked in the request callback) */
                    if ((*puwLen == m_astDiagSrvCfg2[m_uSrvIdx].uwReqLen) ||
                                      (m_astDiagSrvCfg2[m_uSrvIdx].uwReqLen == 0U))
                    {
                       /* If configured, indicate the received service
                         and wait for the answer parameters
                         (length and datas) */

                       /* Individual callback for each service */
                       #if (UDS_SRV_CALLBACK)
                       if (m_astDiagSrvCfg2[m_uSrvIdx].pfuRxMsgInd != NULL)
                       {
                       #endif
                          /* Indicate the received service
                          Individual callback for each service */
                          #if (UDS_SRV_CALLBACK)
                            eUdsStatus =
                                m_astDiagSrvCfg2[m_uSrvIdx].pfuRxMsgInd
                                (puwLen, aubUdsData);
                          /* Same callback for all the services */
                          #else
                            eUdsStatus =
                                UDS_RxServiceInd (m_uSrvIdx, puwLen, aubUdsData);
                          #endif

                          /* Long request */
                          if (UDS_NRC_78 == eUdsStatus)
                          {
                              strLongRequest.m_aubDiagNack78Data[0] = UDS_NRC;
                              strLongRequest.m_aubDiagNack78Data[1] = aubUdsData[0];
                              strLongRequest.m_aubDiagNack78Data[2] = eUdsStatus;

                              /* Set timer to send the NRC78 */
                              #if (UDS_P2 == 0U)
                                 strLongRequest.m_ubCnt = UDS_P2;
                              #else
                                 strLongRequest.m_ubCnt = UDS_P2 - 1U;
                              #endif
                              strLongRequest.m_eState = UDS_LONG_REQ_ON;
                                
                              /* Nothing to answer because NRC78 answer will be sent later */
                              *puwLen = 0U;
                          }
                           
                       /* Individual callback for each service */
                       #if (UDS_SRV_CALLBACK)
                       }
                       else
                       {
                           /* Nothing to do */
                       }
                       #endif
                    }


                    /* Implements 000-07-D028-D-81101-1 */
                    else
                    {
                        /* Bad length, Negative answer */
                        #if ( UDS_EB_CAN_TYPE == RSA_MIXED )
                        eUdsStatus = UDS_NRC_12;
                        #else
                        eUdsStatus = UDS_NRC_13;
                        #endif
                    }

                } /* End if(eUdsStatus == UDS_ACK) */

                else
                {
                    /* Nothing to do */
                }

            } /* End if(eUdsStatus == UDS_ACK) */

            else
            {

                /* Nothing to do */
            }

        } /* End if(eUdsStatus == UDS_ACK) */

        else
        {
            /* Nothing to do */
        }
    }
    return eUdsStatus;

}

/* - End UDS_FilteringCfg2                                                       */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_ServiceFiltering2                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service in implemented is the configuration 2       */
/*  array                                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_11)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg2                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSrvIdx : start service index              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering2(u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        if(*puwLen != 0U)
        {
            /* Search the first service equal to aubUdsData[0] into the
               configuration 2 array                                       */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg2Max))
            {
                /* search for service and mode (func / phy or both) */
                if ((m_astDiagSrvCfg2[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                    ((m_astDiagSrvCfg2[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode))
                {
                    /* service/mode are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }
            /* If the service has been found return positiv answer else
               return a NCR_11
               Implements 000-07-D028-D-81201-1                        */
            if (eFlagFound == UDS_TRUE)
            {
                /* Save the start service index */
                m_uStartSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                eUdsStatus = UDS_NRC_11;
            }
        }
        else
        {
            eUdsStatus = UDS_NRC_11;
        }
    }

    return eUdsStatus;

}

/* - End UDS_ServiceFiltering2                                                   */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_SessionFiltering2                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service/session is implemented in the               */
/*  configuration 2 array                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_7F)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg2                                  */
/*                             m_uSrvIdx: current service index                  */
/*                             m_uStartSrvIdx : start service index              */
/*                             m_uStartSubSrvIdx : start sub-service index       */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_SessionFiltering2 (u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;
    tUdsSessionMask eSessionMask;

    eUdsStatus = UDS_ACK;
    eFlagFound = UDS_FALSE;
    eSessionMask = UDS_SESSION_DEFAULT_MASK;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        m_uSrvIdx = m_uStartSrvIdx;

        switch(m_eSessionMask)
        {
            case UDS_SESSION_PROT_OTHER_01_MASK:
                eSessionMask = UDS_SESSION_OTHER_01_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_02_MASK:
                eSessionMask = UDS_SESSION_OTHER_02_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_03_MASK:
                eSessionMask = UDS_SESSION_OTHER_03_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_04_MASK:
                eSessionMask = UDS_SESSION_OTHER_04_MASK;
                break;
            case UDS_SESSION_PROT_DEFAULT_MASK:
                eSessionMask = UDS_SESSION_DEFAULT_MASK;
                break;
            case UDS_SESSION_PROT_PROGRAMMING_MASK:
                eSessionMask = UDS_SESSION_PROGRAMMING_MASK;
                break;
            case UDS_SESSION_PROT_EXTENDED_DIAG_MASK:
                eSessionMask = UDS_SESSION_EXTENDED_DIAG_MASK;
                break;
            case UDS_SESSION_PROT_SUPPLIER_MASK:
                eSessionMask = UDS_SESSION_SUPPLIER_MASK;
                break;

            default :
                /* Indicate of range error */
                m_eSessionMask = m_eSessionMaskDefaultValue;
                NCS_SdfVariableRangeError (SDF_UDS_SESSION_MASK);
                eUdsStatus = UDS_ERR_COHE;
            break;
        }
        
        
        if (eUdsStatus == UDS_ACK)
        {
            /* Check if the service and session is implemented */
            while (((!eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg2Max)) &&
                   (m_astDiagSrvCfg2[m_uSrvIdx].ubSrvId == aubUdsData[0]) )
            {
                if (m_astDiagSrvCfg2[m_uSrvIdx].eSessionMasks & eSessionMask )
                {
                    /* service/session are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }

            /* If service/session are OK return positiv answer
               Implements 000-07-D028-D-81302-1   */
            if (eFlagFound == UDS_TRUE)
            {
                m_uSrvIdx = m_uStartSrvIdx;

                eFlagFound = UDS_FALSE;

                /* Check if the service/DID/session is implemented */
                while (((!eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg2Max)) &&
                       (m_astDiagSrvCfg2[m_uSrvIdx].ubSrvId == aubUdsData[0]))
                {
                    if( (m_astDiagSrvCfg2[m_uSrvIdx].uwDid == m_uwDid) &&
                        ((m_astDiagSrvCfg2[m_uSrvIdx].eSessionMasks & eSessionMask) != 0U))
                    {
                        /* service/DID/session are OK */
                        eFlagFound = UDS_TRUE;
                    }
                    else
                    {
                        m_uSrvIdx++ ;
                    }

                }

                /* If service/DID/session are OK return positive answer
                   Implements 000-07-D028-D-81303-2                          */
                if (eFlagFound == UDS_TRUE)
                {
                    eUdsStatus = UDS_ACK;
                }
                /* Service et Sub-service is implemented in the ECU but not in the
                   session return a UDS_NRC_DID_NOT_IMPL_CUR_SESS (code to define)
                   Implements 000-07-D028-D-81304-2                              */
                else
                {
                    eUdsStatus = UDS_NRC_INVALID_DID;
                }
            }

            /* Service is implemented in the ECU but not in the
               session return a NCR_7F
               Implements 000-07-D028-D-81301-1               */
            else
            {
                eUdsStatus = UDS_NRC_7F;
            }
        }
    }

    return (eUdsStatus);
}


/* - End UDS_SessionFiltering2                                                   */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_DidFiltering                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service/ DID is implemented in the                  */
/*  configuration 2 array                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_31)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg2                                  */
/*                             m_uwDid                                           */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSubSrvIdx : start sub-service index       */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_DidFiltering(u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        if(*puwLen >= 3)
        {
            m_uSrvIdx = m_uStartSrvIdx;

            /* Search the first service/DID equal to aubUdsData[0]/
              m_uwDid into the configuration 2 array                 */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg2Max))
            {            
                /* search for service and did and mode (func / phy or both) */
                if (((m_astDiagSrvCfg2[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                     (m_astDiagSrvCfg2[m_uSrvIdx].uwDid == m_uwDid)) && 
                    ((u8)(m_astDiagSrvCfg2[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode))
                {
                    /* service/did/mode are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }
            /* If the DID has been found return positiv answer else
               return a NCR_31
               Implements 000-07-D028-D-81401-1                        */
            if (eFlagFound == UDS_TRUE)
            {
                /* Save the start sub-service index */
                m_uStartSubSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                /* Non implemented Did */
                #if ( UDS_EB_CAN_TYPE == RSA_MIXED )
                eUdsStatus = UDS_NRC_12;
                #else
                eUdsStatus = UDS_NRC_31;
                #endif
            }

        }
        else
        {
            /* Bad parameters */
            #if ( UDS_EB_CAN_TYPE == RSA_MIXED )
            eUdsStatus = UDS_NRC_12;
            #else
            eUdsStatus = UDS_NRC_31;
            #endif
        }
   }

    return eUdsStatus;


}
/* - End UDS_DidFiltering                                                        */
/*-------------------------------------------------------------------------------*/
#endif /* End #if (UDS_SRV_CFG2_MAX) */

#if (UDS_SRV_CFG3_MAX)
/*********************************************************************************/
/* Name: UDS_FilteringCfg3                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Realize the filtering of the service, subservice, session and    */
/*              RI of the config 3 array structur m_astDiagSrvCfg3               */
/*              Check the request length.                                        */
/*              If the received service is authorized, a callback is called      */
/*-------------------------------------------------------------------------------*/
/* Implements: 000-07-D028-D-81502-1                                             */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_xx)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg1                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uwRi : Current Routine identifier               */
/*-------------------------------------------------------------------------------*/
/* Called functions          : UDS_ServiceFiltering3,UDS_RiFiltering             */
/*                             UDS_SessionFiltering3,UDS_SubServiceFiltering3    */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/


NCS_PRI_FCT tUdsStatus UDS_FilteringCfg3(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        #if (UDS_EB_CAN_TYPE == RSA_MIXED)
            m_uwRi = (u8) aubUdsData[1];
        #else    
            m_uwRi = (u16)(((u16)aubUdsData[2] << 8) + ((u16)aubUdsData[3]));
        #endif
        
        /* Implements 000-07-D028-D-81503-1
           Service Filtering */
        eUdsStatus = UDS_ServiceFiltering3(puwLen, aubUdsData);

        if(eUdsStatus == UDS_ACK)
        {
            /* Subservice doesn't exist for SPRBLID service*/
            if(aubUdsData[0] != UDS_SRV_SPRBLID )
            {
                /* Sub-service filtering */
                eUdsStatus = UDS_SubServiceFiltering3(puwLen, aubUdsData);
            }

            if(eUdsStatus == UDS_ACK)
            {
                /* Session Filtering */
                eUdsStatus = UDS_SessionFiltering3(puwLen, aubUdsData);
                    
                if(eUdsStatus == UDS_ACK)
                {
                    /* DID Filtering */
                    eUdsStatus = UDS_RiFiltering(puwLen, aubUdsData);

                    if(eUdsStatus == UDS_ACK)
                    {

                        /* The request length is correct (1) or
                          the length must not be checked now (2)
                          (it will be checked in the request callback) */
                        if ((*puwLen == m_astDiagSrvCfg3[m_uSrvIdx].uwReqLen) ||
                                               (m_astDiagSrvCfg3[m_uSrvIdx].uwReqLen == 0U))
                        {
                        #if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
                            switch (aubUdsData[1] & UDS_SUPPRESS_POS_RESP_MASK)
                            {
                                case UDS_START_ROUTINE:
                                    eUdsStatus = UDS_StartRoutineControl(puwLen, aubUdsData);
                                    break;

                                case UDS_STOP_ROUTINE:
                                    eUdsStatus = UDS_StopRoutineControl(puwLen, aubUdsData);
                                    break;

                                case UDS_REQUEST_ROUTINE_RESULT:
                                    eUdsStatus = UDS_GetRoutineStatus(puwLen, aubUdsData);
                                    break;

                                default:
                                    eUdsStatus = UDS_NRC_12;
                                    break;
                            }

                        #else    /* Routine Control for RSA*/
                            eUdsStatus = UDS_RoutineByLocalIdentifier(puwLen, aubUdsData);
                        #endif

                            /* Long request */
                            if (UDS_NRC_78 == eUdsStatus)
                            {
                                strLongRequest.m_aubDiagNack78Data[0] = UDS_NRC;
                                strLongRequest.m_aubDiagNack78Data[1] = aubUdsData[0];
                                strLongRequest.m_aubDiagNack78Data[2] = eUdsStatus;

                                /* Set timer to send the NRC78 */
                                #if (UDS_P2 == 0U)
                                    strLongRequest.m_ubCnt = UDS_P2;
                                #else
                                    strLongRequest.m_ubCnt = UDS_P2 - 1U;
                                #endif
                                strLongRequest.m_eState = UDS_LONG_REQ_ON;

                                /* Nothing to answer because NRC78 answer will be sent later */
                                *puwLen = 0U;
                            }
                        }
                        /* Implements 000-07-D028-D-81501-1 */
                        else
                        {
                            /* Bad length, Negative answer */
                            #if ( UDS_EB_CAN_TYPE == RSA_MIXED )
                            eUdsStatus = UDS_NRC_12;
                            #else
                            eUdsStatus = UDS_NRC_13;
                            #endif
                        }

                    } /* End if(eUdsStatus == UDS_ACK) */

                    else
                    {
                        /* Nothing to do */
                    }
                } /* End if(eUdsStatus == UDS_ACK) */

                else
                {
                    /* Nothing to do */
                }
            } /* End if(eUdsStatus == UDS_ACK) */

            else
            {
                /* Nothing to do */
            }
        } /* End if(eUdsStatus == UDS_ACK) */

        else
        {
            /* Nothing to do */
        }
    }
    return eUdsStatus;

}

/* - End UDS_FilteringCfg3                                                       */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_ServiceFiltering3                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service in implemented is the configuration 3       */
/*  array                                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_11)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg3                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSrvIdx : start service index              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/


NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering3(u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        if(*puwLen != 0U)
        {
            /* Search the first service equal to aubUdsData[0] into the
               configuration 1 array                                       */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg3Max))
            {            
                /* search for service and mode (func / phy or both) */
                if ((m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                    ((m_astDiagSrvCfg3[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode))
                {
                    /* service/mode are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }
            /* If the service has been found return positiv answer else
               return a NCR_11
               Implements 000-07-D028-D-81601-1                        */
            if (eFlagFound == UDS_TRUE)
            {
                /* Save the start service index */
                m_uStartSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                eUdsStatus = UDS_NRC_11;
            }
        }
        else
        {
            eUdsStatus = UDS_NRC_11;
        }
    }
    return eUdsStatus;

}

/* - End UDS_ServiceFiltering3                                                   */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_SubServiceFiltering3                                                */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service/ sub-service is implemented in the          */
/*  configuration 3 array                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_12)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg3                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSubSrvIdx : start sub-service index       */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_SubServiceFiltering3(u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    u8 ubSubSrv;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
    #if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
        if(*puwLen >= 2U)
        {
            /* Mask the SuppresPosRspMsgIndicationBit */
             ubSubSrv = (u8) (aubUdsData[1] & UDS_SUPPRESS_POS_RESP_MASK);
    #else
        if(*puwLen >= 3U)
        {
                ubSubSrv = (u8) aubUdsData[2];
    #endif
            /* Search the first service/sub-service equal to aubUdsData[0]/
              aubUdsData[1] into the configuration 3 array                   */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg3Max))
            {            
                /* search for service and subsrv and mode (func / phy or both) */
                if (((m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                     (m_astDiagSrvCfg3[m_uSrvIdx].ubSubSrvId == ubSubSrv)) &&
                    ((u8)(m_astDiagSrvCfg3[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode))
                {
                    /* service/subsrv/mode are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }
            /* If the sub-service has been found return positiv answer else
               return a NCR_12
               Implements 000-07-D028-D-81701-1                        */
            if(m_uSrvIdx < m_uUdsSrvCfg3Max)
            {
                /* Save the start sub-service index */
                m_uStartSubSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                eUdsStatus = UDS_NRC_12;
            }
        }

        else
        {
            /*eUdsStatus = UDS_NRC_12;*/
            /*change by zyw for SEM*/
            eUdsStatus = UDS_NRC_13;
        }
    }

    return eUdsStatus;


}


/* - End UDS_SubServiceFiltering3                                                */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: UDS_SessionFiltering3                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service/ Routine identifier /session is implemented */
/* in the configuration 3 array                                                  */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_7E or UDS_NRC_7F)  */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg3                                  */
/*                             m_ubSessionMask                                   */
/*                             m_uwRi                                            */
/*                             m_uSrvIdx: current service index                  */
/*                             m_uStartSrvIdx : start service index              */
/*                             m_uStartSubSrvIdx : start sub-service index       */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_SessionFiltering3 (const u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;
    tUdsSessionMask eSessionMask;
    
    eUdsStatus = UDS_ACK;
    eFlagFound = UDS_FALSE;
    eSessionMask = UDS_SESSION_DEFAULT_MASK;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        m_uSrvIdx = m_uStartSrvIdx;

        switch(m_eSessionMask)
        {
            case UDS_SESSION_PROT_OTHER_01_MASK:
                eSessionMask = UDS_SESSION_OTHER_01_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_02_MASK:
                eSessionMask = UDS_SESSION_OTHER_02_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_03_MASK:
                eSessionMask = UDS_SESSION_OTHER_03_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_04_MASK:
                eSessionMask = UDS_SESSION_OTHER_04_MASK;
                break;
            case UDS_SESSION_PROT_DEFAULT_MASK:
                eSessionMask = UDS_SESSION_DEFAULT_MASK;
                break;
            case UDS_SESSION_PROT_PROGRAMMING_MASK:
                eSessionMask = UDS_SESSION_PROGRAMMING_MASK;
                break;
            case UDS_SESSION_PROT_EXTENDED_DIAG_MASK:
                eSessionMask = UDS_SESSION_EXTENDED_DIAG_MASK;
                break;
            case UDS_SESSION_PROT_SUPPLIER_MASK:            
                eSessionMask = UDS_SESSION_SUPPLIER_MASK;
                break;

            default :
                /* Indicate of range error */
                m_eSessionMask = m_eSessionMaskDefaultValue;
                NCS_SdfVariableRangeError (SDF_UDS_SESSION_MASK);
                eUdsStatus = UDS_ERR_COHE;
            break;
        }
        
        
        if (eUdsStatus == UDS_ACK)
        {
            /* Check if the service and session is implemented */
            while (((!eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg3Max)) &&
                   (m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == aubUdsData[0]) )
            {
                if (m_astDiagSrvCfg3[m_uSrvIdx].eSessionMasks & eSessionMask )
                {
                    /* service/session are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }


            if (eFlagFound == UDS_TRUE)
            {
                m_uSrvIdx = m_uStartSrvIdx ;

                eFlagFound = UDS_FALSE;



                /* Check if the service/Ri/session is implemented */
                while ((!eFlagFound) && (
                    (m_uSrvIdx < m_uUdsSrvCfg3Max) &&
                    (m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == aubUdsData[0])) )
                {
                   if(m_astDiagSrvCfg3[m_uSrvIdx].uwRi == m_uwRi)
                   {
                        /* If service/sub-service/session are OK return positiv answer
                           Implements 000-07-D028-D-81803-1                          */
                        if(m_astDiagSrvCfg3[m_uSrvIdx].eSessionMasks & eSessionMask)
                        {
                            eFlagFound = UDS_TRUE;
                            eUdsStatus = UDS_ACK;
                        }
                        /* Service et Sub-service is implemented in the ECU but not in the
                            session return a NCR_7E
                             Implements 000-07-D028-D-81802-1                              */
                        else
                        {
                            eUdsStatus = RC_NRC_IMPLEMENTATION;
                        }
                    }
                    else
                    {
                        /* Nothing to do */
                    }

                    if(!eFlagFound)
                    {
                        m_uSrvIdx++ ;
                    }
                    else
                    {
                        /* Nothing to do */
                    }
                }
            }
            /* Service is implemented in the ECU but not in the
               session return a NCR_7F
               Implements 000-07-D028-D-81801-1               */
            else
            {
                eUdsStatus = UDS_NRC_7F;
            }
        }
    }

    return (eUdsStatus);
}

/* - End UDS_SessionFiltering3                                                   */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: UDS_RiFiltering                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service/ sub-service / Ri is implemented in the     */
/*  configuration 3 array                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_31)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg3                                  */
/*                             m_uwDid                                           */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSubSrvIdx : start sub-service index       */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_RiFiltering(u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    u8 ubSubSrv;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {

        #if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
        if(*puwLen >= 4)
        {
            m_uSrvIdx = m_uStartSubSrvIdx;

             /* Mask the SuppresPosRspMsgIndicationBit */
             ubSubSrv = (u8) (aubUdsData[1] & UDS_SUPPRESS_POS_RESP_MASK);

            /* Search the first service/DID equal to aubUdsData[0]/
              m_uwDid into the configuration 3 array                 */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg3Max))
            {            
                /* search for service and subsrv and ri and mode (func / phy or both) */
                if (((m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                     (m_astDiagSrvCfg3[m_uSrvIdx].ubSubSrvId == ubSubSrv)) &&                 
                    ((m_astDiagSrvCfg3[m_uSrvIdx].uwRi == m_uwRi) &&
                     ((u8)(m_astDiagSrvCfg3[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode)))
                {
                    /* service/subsrv/ri/mode are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }             
        #elif ( UDS_EB_CAN_TYPE == RSA_MIXED )
        if(*puwLen >= 2)
        {
            m_uSrvIdx = m_uStartSubSrvIdx;
            
            /* Mask the SuppresPosRspMsgIndicationBit */
             ubSubSrv = (u8) aubUdsData[2];

            if (aubUdsData[0] == UDS_SRV_SPRBLID)
            {
                /* Search the first service/DID equal to aubUdsData[0]/
                  m_uwDid into the configuration 3 array                 */
                while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg3Max))
                {                
                    /* search for service and ri and mode (func / phy or both) */                    
                    if (((m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                         (m_astDiagSrvCfg3[m_uSrvIdx].uwRi == m_uwRi)) &&
                        ((u8)(m_astDiagSrvCfg3[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode))
                    {
                        /* service/ri/mode are OK */
                        eFlagFound = UDS_TRUE;
                    }
                    else
                    {
                        m_uSrvIdx++ ;
                    }
                }
            }
            else
            {
                /* Search the first service/DID equal to aubUdsData[0]/
                  m_uwDid into the configuration 3 array                 */
                while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg3Max))
                {
                    /* search for service and subsrv and ri and mode (func / phy or both) */                    
                    if (((m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                         (m_astDiagSrvCfg3[m_uSrvIdx].ubSubSrvId == ubSubSrv)) &&
                        ((m_astDiagSrvCfg3[m_uSrvIdx].uwRi == m_uwRi) &&
                         ((u8)(m_astDiagSrvCfg3[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode)))
                    {
                        /* service/subsrv/ri/mode are OK */
                        eFlagFound = UDS_TRUE;
                    }
                    else
                    {
                        m_uSrvIdx++ ;
                    }
                }
            }
        #endif
           
            if (eFlagFound == UDS_TRUE)
            {
                /* Save the start sub-service index */
                m_uStartSubSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                #if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
                    eUdsStatus = UDS_NRC_31;
                #elif ( UDS_EB_CAN_TYPE == RSA_MIXED )
                    eUdsStatus = UDS_NRC_12;
                #endif
            }
        }

        else
        {
            #if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
            eUdsStatus = UDS_NRC_31;
            #elif ( UDS_EB_CAN_TYPE == RSA_MIXED )
            eUdsStatus = UDS_NRC_12;
            #endif
        }
    }

    return eUdsStatus;


}

/* - End UDS_RiFiltering                                                         */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: UDS_StartRoutineControl                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: This function called the callback and set the routine status     */
/* according the value return by the callback                                    */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_22)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg3                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_stRoutine : Current routine definition          */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
NCS_PRI_FCT tUdsStatus UDS_StartRoutineControl(u16 *puwLen, u8 *aubUdsData)
{

    tUdsStatus eUdsStatus;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        /* Check if routine is in progress that the Routine identifier is identical
          else return NCR_22 */
        if( (m_stRoutine.eStart == UDS_ROUTINE_IN_PROGRESS)
                && (m_stRoutine.uwRi!= m_uwRi))
        {
            eUdsStatus = UDS_NRC_22;
        }

        else
        {
            eUdsStatus = UDS_ACK;

            m_stRoutine.uwRi = m_uwRi;

            /* If configured, indicate the received service
             and wait for the answer parameters
             (length and datas) */
            /* Implements 000-07-D028-D-82101-1 */
            /* Individual callback for each service */
            #if (UDS_SRV_CALLBACK)
            if (m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd != NULL)
            {
               /* Indicate the received service
                  Individual callback for each service */
                eUdsStatus = m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd
                             (puwLen, aubUdsData);
            }
            else
            {
               /* Nothing to do */
            }
            #else
                /* Same callback for all the services */
                eUdsStatus = UDS_RxServiceInd (m_uSrvIdx, puwLen, aubUdsData);
            #endif    /* UDS_SRV_CALLBACK */

            /* Implements 000-07-D028-D-82102-1 */
            if(eUdsStatus == UDS_ACK)
            {
//                if((aubUdsData[4] & UDS_ROUTINE_STATUS_MASK) == UDS_ROUTINE_IN_PROGRESS)
                {
                    m_stRoutine.eStart = UDS_ROUTINE_IN_PROGRESS;

                }
//                else
//                {
//                   /* Nothing to do */
//                }

                m_stRoutine.eStatus = aubUdsData[4];
            }
        }

    }

return eUdsStatus;

}
#endif
/* - End UDS_StartRoutineControl                                                 */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_StopRoutineControl                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: This function check the routine is started. If yes, called       */
/* the callback and set the routine status with STOP. If No return NCR_24        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_24)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg3                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_stRoutine : Current routine definition          */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
NCS_PRI_FCT tUdsStatus UDS_StopRoutineControl(u16 *puwLen, u8 *aubUdsData)
{

    tUdsStatus eUdsStatus = UDS_ACK;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        if((m_stRoutine.eStart != UDS_ROUTINE_NOT_STARTED)
                && (m_stRoutine.uwRi== m_uwRi))
        {
            /* If configured, indicate the received service
             and wait for the answer parameters
             (length and datas) */
            /* Implements 000-07-D028-D-82201-1 */
            /* Individual callback for each service */
            #if (UDS_SRV_CALLBACK)
            if (m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd != NULL)
            {
                /* Indicate the received service
                   Individual callback for each service */
                eUdsStatus = m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd
                             (puwLen, aubUdsData);
            }
            else
            {
               /* Nothing to do */
            }
            #else
                /* Same callback for all the services */
                eUdsStatus = UDS_RxServiceInd (m_uSrvIdx, puwLen, aubUdsData);
            #endif    /* UDS_SRV_CALLBACK */

            if(eUdsStatus == UDS_ACK)
            {

                m_stRoutine.eStatus = aubUdsData[4];
                m_stRoutine.eStart = UDS_ROUTINE_STOPPED;

            }
            else
            {
                m_stRoutine.eStart = UDS_ROUTINE_NOT_STARTED;
            }
        }

        /* Implements 000-07-D028-D-82202-1 */
        else
        {

            eUdsStatus = UDS_NRC_24;
        }
    }


return eUdsStatus;

}
#endif
/* - End UDS_StopRoutineControl                                                  */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_GetRoutineStatus                                                    */
/*-------------------------------------------------------------------------------*/
/* Description: This function get the routine status if the routine is started   */
/* else return NCR_24                                                            */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_24)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_stRoutine : Current routine definition          */
/*                             m_uwRi : Current routine identifier               */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
NCS_PRI_FCT tUdsStatus UDS_GetRoutineStatus(u16 *puwLen, u8 *aubUdsData)
{

    tUdsStatus eUdsStatus = UDS_ACK;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {

        if((m_stRoutine.eStart != UDS_ROUTINE_NOT_STARTED)
                && (m_stRoutine.uwRi== m_uwRi))
        {
            /* If configured, indicate the received service
             and wait for the answer parameters
             (length and datas) */
            /* Implements 000-07-D028-D-82201-1 */
            /* Individual callback for each service */
            #if (UDS_SRV_CALLBACK)
            if (m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd != NULL)
            {
               /* Indicate the received service
                  Individual callback for each service */
                eUdsStatus = m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd
                             (puwLen, aubUdsData);
            }
            else
            {
               /* Implements 000-07-D028-D-82301-1 */
               aubUdsData[4] = m_stRoutine.eStatus;               
            }
            #else
                /* Same callback for all the services */
                eUdsStatus = UDS_RxServiceInd (m_uSrvIdx, puwLen, aubUdsData);
            #endif /* UDS_SRV_CALLBACK */
        }
        /* Implements 000-07-D028-D-82302-1 */
        else
        {
            eUdsStatus = UDS_NRC_24;
        }

    }

return eUdsStatus;

}
#endif
/* - End UDS_GetRoutineStatus                                                    */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_RoutineByLocalIdentifier                                            */
/*-------------------------------------------------------------------------------*/
/* Description: This function realize Routine Control for NISSAN specifications  */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_24)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_stRoutine : Current routine definition          */
/*                             m_uwRi : Current routine identifier               */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#if( UDS_EB_CAN_TYPE == RSA_MIXED )
NCS_PRI_FCT tUdsStatus UDS_RoutineByLocalIdentifier (u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus= UDS_ERR_COHE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {

        if ( m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == UDS_SRV_SRBLID )
        {
                /* Check if routine is in progress that the Routine identifier is identical
                   else return NCR_22 */
                if( (m_stRoutine.eStart == UDS_ROUTINE_IN_PROGRESS)
                        && (m_stRoutine.uwRi!= m_uwRi))
                {
                    eUdsStatus = UDS_NRC_22;
                }
                else
                {
                    eUdsStatus = UDS_ACK;

                    m_stRoutine.uwRi = m_uwRi;

                    /* If configured, indicate the received service
                     and wait for the answer parameters
                     (length and datas) */
                    /* Individual callback for each service */
                    #if (UDS_SRV_CALLBACK)
                    if (m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd != NULL)
                    {
                        eUdsStatus =
                               m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd
                           (puwLen, aubUdsData);
                    }
                    else
                    {
                       /* Nothing to do */
                    }
                    #else
                    eUdsStatus =
                          UDS_RxServiceInd (m_uSrvIdx, puwLen, aubUdsData);
                    #endif

                    if(eUdsStatus == UDS_ACK)
                    {
                        if(aubUdsData[2] == UDS_ROUTINE_IN_PROGRESS)
                        {
                            m_stRoutine.eStart = UDS_ROUTINE_IN_PROGRESS;

                        }
                        else
                        {
                           /* Nothing to do */
                        }

                        m_stRoutine.eStatus = aubUdsData[2];
                }
            }
        } /* End if ( m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == UDS_SRV_SRBLID )*/
        else if ( m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == UDS_SRV_SPRBLID )
        {            
            eUdsStatus = UDS_ACK;
            
            if((m_stRoutine.eStart != UDS_ROUTINE_NOT_STARTED)
                && (m_stRoutine.uwRi== m_uwRi))
            {
                /* If configured, indicate the received service
                 and wait for the answer parameters
                 (length and datas) */
                /* Individual callback for each service */
               #if (UDS_SRV_CALLBACK)
                if (m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd != NULL)
                {
                    eUdsStatus =
                           m_astDiagSrvCfg3[m_uSrvIdx].pfuRxMsgInd
                       (puwLen, aubUdsData);
                }
                else
                {
                   /* Nothing to do */
                }
                #else
                eUdsStatus =
                      UDS_RxServiceInd (m_uSrvIdx, puwLen, aubUdsData);
                #endif
                
                if(eUdsStatus == UDS_ACK)
                {
                    m_stRoutine.eStatus = aubUdsData[2];
                    m_stRoutine.eStart = UDS_ROUTINE_STOPPED;
                }
                else
                {
                    m_stRoutine.eStart = UDS_ROUTINE_NOT_STARTED;
                }
            }
            else
            {
                eUdsStatus = UDS_NRC_22;
            }
        } /* End else if (( m_astDiagSrvCfg3[m_uSrvIdx].ubSrvId == UDS_SRV_SPRBLID ) */
        else
        {
            /* MISRA-C:2004 Rule 14.10 */
        }
    }

return eUdsStatus;
}
#endif
/* - End UDS_RoutineByLocalIdentifier                                            */
/*-------------------------------------------------------------------------------*/
#endif /* End #if (UDS_SRV_CFG3_MAX) */

#if (UDS_SRV_CFG4_MAX)
/*********************************************************************************/
/* Name: UDS_FilteringCfg4                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Realize the filtering of the service, LID, session               */
/*              of the config 4 array structur m_astDiagSrvCfg4                  */
/*              Check the request length.                                        */
/*              If the received service is authorized, a callback is called      */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_xx)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg4                                  */
/*                             m_eSessionCode: current session code              */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : UDS_ServiceFiltering4,UDS_LidFiltering            */
/*                             UDS_SessionFiltering4                             */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
NCS_PRI_FCT tUdsStatus UDS_FilteringCfg4(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;


    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        switch(aubUdsData[0])
        {
            case UDS_SRV_RDBLID:
            case UDS_SRV_WDBLID:
            case UDS_SRV_IOCBLID:
                m_ubLid = (u8)aubUdsData[1];
                break;
                
            case UDS_SRV_DLIDDY:
                /* We don't check the Lid for this service */
                break;
                
            default:
                /* Nothing to do */
                break;
                
        }

        /* Service Filtering */
        eUdsStatus = UDS_ServiceFiltering4(puwLen, aubUdsData);

        if(eUdsStatus == UDS_ACK)
        {
            /* Session Filtering */
            eUdsStatus = UDS_SessionFiltering4(puwLen, aubUdsData);

            if(eUdsStatus == UDS_ACK)
            {
                /* DID Filtering */
                if( aubUdsData[0] != UDS_SRV_DLIDDY)
                {
                    eUdsStatus = UDS_LidFiltering(puwLen, aubUdsData);
                }    

                if(eUdsStatus == UDS_ACK)
                {

                    /* The request length is correct (1) or
                      the length must not be checked now (2)
                      (it will be checked in the request callback) */
                    if ((*puwLen == m_astDiagSrvCfg4[m_uSrvIdx].uwReqLen) ||
                                      (m_astDiagSrvCfg4[m_uSrvIdx].uwReqLen == 0U))
                    {
                       /* If configured, indicate the received service
                         and wait for the answer parameters
                         (length and datas) */

                       /* Individual callback for each service */
                       #if (UDS_SRV_CALLBACK)
                       if (m_astDiagSrvCfg4[m_uSrvIdx].pfuRxMsgInd != NULL)
                       {
                       #endif
                           /* Indicate the received service
                              Individual callback for each service */
                           #if (UDS_SRV_CALLBACK)
                               eUdsStatus =
                                   m_astDiagSrvCfg4[m_uSrvIdx].pfuRxMsgInd
                               (puwLen, aubUdsData);
                           /* Same callback for all the services */
                           #else
                              eUdsStatus =
                              UDS_RxServiceInd (m_uSrvIdx, puwLen, aubUdsData);
                           #endif

                           /* Long request */
                           if (UDS_NRC_78 == eUdsStatus)
                           {
                               strLongRequest.m_aubDiagNack78Data[0] = UDS_NRC;
                               strLongRequest.m_aubDiagNack78Data[1] = aubUdsData[0];
                               strLongRequest.m_aubDiagNack78Data[2] = eUdsStatus;

                               /* Set timer to send the NRC78 */
                               #if (UDS_P2 == 0U)
                                  strLongRequest.m_ubCnt = UDS_P2;
                               #else
                                  strLongRequest.m_ubCnt = UDS_P2 - 1U;
                               #endif
                               strLongRequest.m_eState = UDS_LONG_REQ_ON;

                               /* Nothing to answer because NRC78 answer will be sent later */
                               *puwLen = 0U;
                           }
                            
                       /* Individual callback for each service */
                       #if (UDS_SRV_CALLBACK)
                       }
                       else
                       {
                           /* Nothing to do */
                       }
                       #endif
                    }

                    /* Implements 000-07-D028-D-81101-1 */
                    else
                    {
                        /* Bad length, Negative answer */
                        #if ( UDS_EB_CAN_TYPE == RSA_MIXED )
                        eUdsStatus = UDS_NRC_12;
                        #else
                        eUdsStatus = UDS_NRC_13;
                        #endif
                    }

                } /* End if(eUdsStatus == UDS_ACK) */

                else
                {
                    /* Nothing to do */
                }

            } /* End if(eUdsStatus == UDS_ACK) */

            else
            {
                /* Nothing to do */
            }

        } /* End if(eUdsStatus == UDS_ACK) */

        else
        {
            /* Nothing to do */
        }

    }

    return eUdsStatus;

}

/* - End UDS_FilteringCfg4                                                       */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_ServiceFiltering4                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service in implemented is the configuration 4       */
/*  array                                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_11)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg4                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSrvIdx : start service index              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering4(u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        if(*puwLen != 0U)
        {
            /* Search the first service equal to aubUdsData[0] into the
               configuration 4 array                                       */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg4Max))
            {            
                /* search for service and mode (func / phy or both) */
                if ((m_astDiagSrvCfg4[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                    ((m_astDiagSrvCfg4[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode))
                {
                    /* service/mode are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }
            /* If the service has been found return positiv answer else
               return a NCR_11
               Implements 000-07-D028-D-81201-1                        */
            if (eFlagFound == UDS_TRUE)
            {
                /* Save the start service index */
                m_uStartSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                eUdsStatus = UDS_NRC_11;
            }
        }
        else
        {
            eUdsStatus = UDS_NRC_11;
        }
    }

    return eUdsStatus;

}

/* - End UDS_ServiceFiltering4                                                   */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_SessionFiltering4                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service/session is implemented in the               */
/*  configuration 4 array                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_7F)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg4                                  */
/*                             m_uSrvIdx: current service index                  */
/*                             m_uStartSrvIdx : start service index              */
/*                             m_uStartSubSrvIdx : start sub-service index       */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_SessionFiltering4 (u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;
    tUdsSessionMask eSessionMask;
    
    eUdsStatus = UDS_ACK;
    eFlagFound = UDS_FALSE;
    eSessionMask = UDS_SESSION_DEFAULT_MASK;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        m_uSrvIdx = m_uStartSrvIdx;

        switch(m_eSessionMask)
        {
            case UDS_SESSION_PROT_OTHER_01_MASK:
                eSessionMask = UDS_SESSION_OTHER_01_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_02_MASK:
                eSessionMask = UDS_SESSION_OTHER_02_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_03_MASK:
                eSessionMask = UDS_SESSION_OTHER_03_MASK;
                break;
            case UDS_SESSION_PROT_OTHER_04_MASK:
                eSessionMask = UDS_SESSION_OTHER_04_MASK;
                break;
            case UDS_SESSION_PROT_DEFAULT_MASK:
                eSessionMask = UDS_SESSION_DEFAULT_MASK;
                break;
            case UDS_SESSION_PROT_PROGRAMMING_MASK:
                eSessionMask = UDS_SESSION_PROGRAMMING_MASK;
                break;
            case UDS_SESSION_PROT_EXTENDED_DIAG_MASK:
                eSessionMask = UDS_SESSION_EXTENDED_DIAG_MASK;
                break;
            case UDS_SESSION_PROT_SUPPLIER_MASK:
                eSessionMask = UDS_SESSION_SUPPLIER_MASK;
                break;

            default :
                /* Indicate of range error */
                m_eSessionMask = m_eSessionMaskDefaultValue;
                NCS_SdfVariableRangeError (SDF_UDS_SESSION_MASK);
                eUdsStatus = UDS_ERR_COHE;
            break;
        }
        
        
        if (eUdsStatus == UDS_ACK)
        {
            /* Check if the service and session is implemented */
            while (((!eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg4Max)) &&
                   (m_astDiagSrvCfg4[m_uSrvIdx].ubSrvId == aubUdsData[0]) )
            {
                if (m_astDiagSrvCfg4[m_uSrvIdx].eSessionMasks & eSessionMask )
                {
                    /* service/session are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }

            /* If service/session are OK return positiv answer
               Implements 000-07-D028-D-81302-1   */
            if (eFlagFound == UDS_TRUE)
            {
                m_uSrvIdx = m_uStartSrvIdx;
                /* We don't check Lid for DLIDDY service */
                if ( aubUdsData[0] != UDS_SRV_DLIDDY )
                {
                    eFlagFound = UDS_FALSE;

                    /* Check if the service/LID/session is implemented */
                    while (((!eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg4Max)) &&
                           (m_astDiagSrvCfg4[m_uSrvIdx].ubSrvId == aubUdsData[0]))
                    {
                        if( (m_astDiagSrvCfg4[m_uSrvIdx].ubLid == m_ubLid) &&
                            ((m_astDiagSrvCfg4[m_uSrvIdx].eSessionMasks & eSessionMask) != 0U))
                        {
                            /* service/LID/session are OK */
                            eFlagFound = UDS_TRUE;
                        }
                        else
                        {
                            m_uSrvIdx++ ;
                        }

                    }

                    /* If service/LID/session are OK return positive answer
                       Implements 000-07-D028-D-81303-2                          */
                    if (eFlagFound == UDS_TRUE)
                    {
                        eUdsStatus = UDS_ACK;
                    }
                    /* Service et Sub-service is implemented in the ECU but not in the
                       session return a UDS_NRC_DID_NOT_IMPL_CUR_SESS (code to define)
                       Implements 000-07-D028-D-81304-2                              */
                    else
                    {
                        eUdsStatus = UDS_NRC_INVALID_DID;
                    }
                }    
            }

            /* Service is implemented in the ECU but not in the
               session return a NCR_7F
               Implements 000-07-D028-D-81301-1               */
            else
            {
                eUdsStatus = UDS_NRC_7F;
            }
        }
    }

    return (eUdsStatus);
}


/* - End UDS_SessionFiltering4                                                   */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_LidFiltering                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service/ LID is implemented in the                  */
/*  configuration 4 array                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_31)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg4                                  */
/*                             m_ubLid                                           */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSubSrvIdx : start sub-service index       */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_LidFiltering(u16 *puwLen, const u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        if(*puwLen >= 2U)
        {
            m_uSrvIdx = m_uStartSrvIdx;

            /* Search the first service/LID equal to aubUdsData[0]/
              m_ubLid into the configuration 4 array                 */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg4Max))
            {            
                /* search for service and lid and mode (func / phy or both) */
                if (((m_astDiagSrvCfg4[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                     (m_astDiagSrvCfg4[m_uSrvIdx].ubLid == m_ubLid)) &&
                    ((u8)(m_astDiagSrvCfg4[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode))
                {
                    /* service/lid/mode are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }
            /* If the DID has been found return positiv answer else
               return a NCR_31
               Implements 000-07-D028-D-81401-1                        */
            if (eFlagFound == UDS_TRUE)
            {
                /* Save the start sub-service index */
                m_uStartSubSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                /* Non implemented Did */
                #if ( UDS_EB_CAN_TYPE == RSA_MIXED )
                eUdsStatus = UDS_NRC_12;
                #else
                eUdsStatus = UDS_NRC_31;
                #endif
            }

        }
        else
        {
            /* Bad parameters */
            #if ( UDS_EB_CAN_TYPE == RSA_MIXED )
            eUdsStatus = UDS_NRC_12;
            #else
            eUdsStatus = UDS_NRC_31;
            #endif
        }
   }

    return eUdsStatus;


}
/* - End UDS_LidFiltering                                                        */
/*-------------------------------------------------------------------------------*/
#endif /* End #if (UDS_SRV_CFG4_MAX) */




#if (UDS_SRV_CFG5_MAX)
/*********************************************************************************/
/* Name: UDS_FilteringCfg5                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Realize the filtering of the service of the config 5 array       */
/*              structur m_astDiagSrvCfg5                                        */
/*              Check the request length.                                        */
/*              If the received service is authorized, a callback is called      */
/*-------------------------------------------------------------------------------*/
/* Implements: 000-07-D028-D-80702-1                                             */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_xx)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg5                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : UDS_ServiceFiltering5,UDS_NegativeAnswer          */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
NCS_PRI_FCT tUdsStatus UDS_FilteringCfg5(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }

    else
    {
        /* Implements 000-07-D028-D-80703-1
           Service Filtering */
        eUdsStatus = UDS_ServiceFiltering5(puwLen, aubUdsData);

        if(eUdsStatus == UDS_ACK)
        {
            /* The request length is correct (1) or
              the length must not be checked now (2)
              (it will be checked in the request callback) */
            if ( (*puwLen == m_astDiagSrvCfg5[m_uSrvIdx].uwReqLen) ||
                  (m_astDiagSrvCfg5[m_uSrvIdx].uwReqLen == 0U) )
            {
                /* Individual callback for each service */
                #if (UDS_SRV_CALLBACK)
                if (m_astDiagSrvCfg5[m_uSrvIdx].pfuRxMsgInd != NULL)
                {
                #endif
                    #if (UDS_SRV_CALLBACK)
                        eUdsStatus = m_astDiagSrvCfg5[m_uSrvIdx].pfuRxMsgInd
                                        (puwLen, aubUdsData);
                    /* Same callback for all the services */
                    #else
                        eUdsStatus = UDS_RxServiceInd
                                     (m_uSrvIdx, puwLen, aubUdsData);
                    #endif

                        /* Long request */
                        if (UDS_NRC_78 == eUdsStatus)
                        {
                            strLongRequest.m_aubDiagNack78Data[0] = UDS_NRC;
                            strLongRequest.m_aubDiagNack78Data[1] = aubUdsData[0];
                            strLongRequest.m_aubDiagNack78Data[2] = eUdsStatus;
                    
                            /* Set timer to send the NRC78 */
                            #if (UDS_P2 == 0U)
                                strLongRequest.m_ubCnt = UDS_P2;
                            #else
                                strLongRequest.m_ubCnt = UDS_P2 - 1U;
                            #endif
                            strLongRequest.m_eState = UDS_LONG_REQ_ON;
                    
                            /* Nothing to answer because NRC78 answer will be sent later */
                            *puwLen = 0U;
                        }

                #if (UDS_SRV_CALLBACK)
                }
                else
                {
                    /* Nothing to do */
                }
                #endif    
            } /* if ((*puwLen == m_astDiagSrvCfg1[m_uSrvIdx].uwReqLen) ||
                       (m_astDiagSrvCfg1[uSrvIdx].uwReqLen == 0))   */

             /* Implements 000-07-D028-D-XXXXX-1 */
             else
             {
                /* Bad length */
#if ( UDS_EB_CAN_TYPE == RSA_MIXED )
                eUdsStatus = UDS_NRC_12;
#else
                eUdsStatus = UDS_NRC_13;
#endif                
             }

        } /* End if(eUdsStatus == UDS_ACK) */

        else
        {
            /* Nothing to do */
        }
    } 

    return eUdsStatus;

}

/* - End UDS_FilteringCfg5                                                       */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: UDS_ServiceFiltering5                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Check if the service in implemented is the configuration 5       */
/*  array                                                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: received Diagnostic request length        */
/*                             aubUdsData: received Diagnostic request datas     */
/* Output parameters         : None                                              */
/* Return value              : eUdsStatus (UDS_ACK or UDS_NRC_11)                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astDiagSrvCfg5                                  */
/*                             m_uSrvIdx: current service index                  */
/* Modified private variables: m_uStartSrvIdx : start service index              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_ServiceFiltering5(u16 *puwLen, u8 *aubUdsData)
{
    tUdsStatus eUdsStatus = UDS_ERR_COHE;
    tUdsBoolean eFlagFound;

    eFlagFound = UDS_FALSE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsStatus = UDS_ERR_COHE;
    }
    else
    {
        if(*puwLen != 0U)
        {
            /* Search the first service equal to aubUdsData[0] into the
               configuration 5 array                                       */
            while ((UDS_FALSE == eFlagFound) && (m_uSrvIdx < m_uUdsSrvCfg5Max))
            {
                /* search for service and mode (func / phy or both) */
                if ((m_astDiagSrvCfg5[m_uSrvIdx].ubSrvId == aubUdsData[0]) &&
                    ((m_astDiagSrvCfg5[m_uSrvIdx].ubAddrMode & m_eUdsAddrMode) == m_eUdsAddrMode))
                {
                    /* service/mode are OK */
                    eFlagFound = UDS_TRUE;
                }
                else
                {
                    m_uSrvIdx++ ;
                }
            }
            if (eFlagFound == UDS_TRUE)
            {
                /* Save the start service index */
                m_uStartSrvIdx = m_uSrvIdx;

                eUdsStatus = UDS_ACK;
            }
            else
            {
                eUdsStatus = UDS_NRC_11;
            }
        }
        else
        {
            eUdsStatus = UDS_NRC_11;
        }
    }
    return eUdsStatus;
}

/* - End UDS_ServiceFiltering5                                                   */
/*-------------------------------------------------------------------------------*/
#endif

/*********************************************************************************/
/* Name: UDS_LongRequestEnd                                                     */
/*-------------------------------------------------------------------------------*/
/* Description: Answer when a long request has been completed                    */
/*********************************************************************************/
/* Input parameters          : uwLen: Diagnostic answer length                   */
/*                             aubUdsData: Diagnostic answer datas              */
/*                             eStatus: Diagnostic answer negative status        */
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
void UDS_LongRequestEnd (u16 uwLen, u8 *aubUdsData, tUdsStatus eStatus)
{
    if ((strLongRequest.m_eState == UDS_LONG_REQ_ON) && (aubUdsData != NULL))
    {
        /* Positive answer */
        if(eStatus==UDS_ACK)
        {
            /* This is a positive answer, transform the service identifier */
            aubUdsData[0] += UDS_ACK;
        }
        else
        {
            (void)UDS_NegativeAnswer (&uwLen, aubUdsData, eStatus);
        }

        /* UDS_Manage will send the response */
        strLongRequest.m_paubData = aubUdsData;
        strLongRequest.m_uwLen = uwLen;
        strLongRequest.m_eState = UDS_LONG_REQ_END;
    }
}
/* - Fin UDS_LongRequestEnd                                                     */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_NegativeAnswer                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Fill the data bytes of the negative Diagnostic answer            */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : eStatus: Diagnostic answer negative status        */
/*                             puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsBoolean : UDS_TRUE if *puwLen !=0 else        */
/*                                           UDS_FALSE                           */
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

NCS_PRI_FCT tUdsBoolean UDS_NegativeAnswer (u16 *puwLen, u8 *aubUdsData, tUdsStatus eStatus)
{
    tUdsBoolean eUdsBoolean = UDS_TRUE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsBoolean = UDS_FALSE;
    }
    else
    {
        if (m_eUdsAddrMode == UDS_ADDR_FUNCTIONAL)
        {
            switch(eStatus)
            {                              
                /* According to ISO 14229-1:2005(E) NRC_31 must not be sent if */
                /* addressing mode is functional                               */
                #if ( UDS_EB_CAN_TYPE != RSA_MIXED )
                /* RequestOutOfRange case 3 / 8 */
                case UDS_NRC_31:
                #endif
                /* ServiceNotSupported case 4 / 9 */
                case UDS_NRC_11:
                /* SubServiceNotSupported case 5 / 10 */
                case UDS_NRC_12:

                /*Geely UDS Test:                                                  */
                /*Request a service that is not supported by the function function */
                /*in the current session mode, expect ECU to not respond           */
                case UDS_NRC_7E:
                case UDS_NRC_7F:
                
                    *puwLen = 0U;
                    break;
                
                default:
                    /* Implements 000-07-D028-D-80201-1 */
                    /* Negative answer, NCRXX */
                    aubUdsData[2] = eStatus;
                    aubUdsData[1] = aubUdsData[0];
                    aubUdsData[0] = UDS_NRC;
                    *puwLen = UDS_LEN_NRC;
                break;
            }
        }
        else
        {
            /* Implements 000-07-D028-D-80201-1 */
            /* Negative answer, NCRXX */
            aubUdsData[2] = eStatus;
            aubUdsData[1] = aubUdsData[0];
            aubUdsData[0] = UDS_NRC;
            *puwLen = UDS_LEN_NRC;
        }
    }

    return eUdsBoolean;
}

/* - End UDS_NegativeAnswer                                                      */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_PositiveAnswer                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: If the length is superior to 0, add 0x40 to first byte           */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Output parameters         : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Return value              : eUdsBoolean : UDS_TRUE if *puwLen !=0 else        */
/*                                           UDS_FALSE                           */
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
#if ((UDS_SPREC == UDS_SPREC_ON) || (UDS_EB_CAN_TYPE == RSA_MIXED))
NCS_PRI_FCT tUdsBoolean UDS_PositiveAnswer (u16 *puwLen, u8 *aubUdsData)
#else
NCS_PRI_FCT tUdsBoolean UDS_PositiveAnswer (const u16 *puwLen, u8 *aubUdsData)
#endif
{
    tUdsBoolean eUdsBoolean = UDS_TRUE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsBoolean = UDS_FALSE;
    }
    else
    {
        /* Check if the length is different of 0 */
        if(*puwLen != 0U)
        {
            /* DSCPR management */
            #if (UDS_SPREC == UDS_SPREC_ON)
                if (UDS_SRV_DSC == aubUdsData[0])
                {
                    *puwLen += 4U;
                    if( *puwLen == 6U )
                    {
                        aubUdsData[2] = (u8) ((UDS_P2*UDS_MANAGE_PERIOD) >> 8U);
                        aubUdsData[3] = (u8) (UDS_P2*UDS_MANAGE_PERIOD);
                        aubUdsData[4] = (u8) ((UDS_P2STAR*UDS_MANAGE_PERIOD/10U) >> 8U);
                        aubUdsData[5] = (u8) ((UDS_P2STAR*UDS_MANAGE_PERIOD/10U) & 0xFFU);
                    }
                    else
                    {
                        eUdsBoolean = UDS_FALSE;
                    }
                }
            #endif

            #if(UDS_EB_CAN_TYPE == RSA_MIXED)
                if (UDS_SRV_TP == aubUdsData[0])
                {
                    /* Only service is required in the positive response message */
                    /* (TP) Ref to chapter $9.2.5.3 from 36_00_013_C specification */
                    *puwLen = 1U;
                }
            #endif
            
            /* Add 0x40 to the first byte else doesn't the following bytes
               Implements 000-07-D028-D-80301-1
               Implements 000-07-D028-D-80302-1                          */
            aubUdsData[0] = (u8) (aubUdsData[0] + UDS_ACK);
        }
        else
        {
            eUdsBoolean = UDS_FALSE;
        }
    }
    return eUdsBoolean;

}

/* - End UDS_PositiveAnswer                                                      */
/*-------------------------------------------------------------------------------*/
#if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
/*********************************************************************************/
/* Name: UDS_AnswerOrNot                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: The function check the bit 8 of the second bytes and set         */
/* m_eSuppresPosRspMsgIndicationBit with UDS_TRUE if the bit = 1 else            */
/* with UDS_FALSE if the bit = 0                                                 */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : puwLen: Diagnostic answer length                  */
/*                             aubUdsData: Diagnostic answer datas               */
/* Output parameters         : None                                              */
/* Return value              : eUdsBoolean : UDS_TRUE if *puwLen !=0 else        */
/*                                           UDS_FALSE                           */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_eSuppresPosRspMsgIndicationBit                  */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsBoolean UDS_AnswerOrNot (const u16 *puwLen, const u8 *aubUdsData)
{
    tUdsBoolean eUdsBoolean = UDS_TRUE;

    if ((puwLen == NULL) || (aubUdsData == NULL))
    {
        eUdsBoolean = UDS_FALSE;
    }
    else
    {
        /* If the bit 8 of the second byte = 1 set m_eSuppresPosRspMsgIndicationBit
           with UDS_TRUE else  set m_eSuppresPosRspMsgIndicationBit with UDS_FALSE
           Implements 000-07-D028-D-80401-1                                    */
        if(NCS_TST_BIT_SET (aubUdsData[1], 0x80U, u8))
        {
            m_eSuppresPosRspMsgIndicationBit = UDS_TRUE;
        }
        else
        {
            m_eSuppresPosRspMsgIndicationBit = UDS_FALSE;
        }
    }

    return eUdsBoolean;

}

/* - End UDS_AnswerOrNot                                                         */
/*-------------------------------------------------------------------------------*/
#endif

/*********************************************************************************/
/* Name: UDS_DiagSession                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: This function update the active session                          */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : eUdsSessionType : Session type                    */
/* Output parameters         : None                                              */
/* Return value              : tUdsStatus                                        */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_eSuppresPosRspMsgIndicationBit                  */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tUdsStatus UDS_DiagSession (tUdsSessionType eUdsSessionType)
{
    tUdsStatus eUdsStatus;
    tUdsSessionProtType eUdsSessTypeProt;
    tUdsSessionProtMask eUdsTmpSessProtMask;
    eUdsStatus = UDS_ACK;

    /* Translate the input parameter to an internal protected value (SDF) */
    switch (eUdsSessionType)
    {
        case UDS_SESSION_PROGRAMMING:
            eUdsSessTypeProt = UDS_SESSION_PROT_PROGRAMMING;
            eUdsTmpSessProtMask = UDS_SESSION_PROT_PROGRAMMING_MASK;
            break;
        case UDS_SESSION_EXTENDED_DIAG:
            eUdsSessTypeProt = UDS_SESSION_PROT_EXTENDED_DIAG;
            eUdsTmpSessProtMask = UDS_SESSION_PROT_EXTENDED_DIAG_MASK;
            break;
        case UDS_SESSION_SUPPLIER:
            eUdsSessTypeProt = UDS_SESSION_PROT_SUPPLIER;
            eUdsTmpSessProtMask = UDS_SESSION_PROT_SUPPLIER_MASK;
            break;
        case UDS_SESSION_OTHER_01:
            eUdsSessTypeProt = UDS_SESSION_PROT_OTHER_01;
            eUdsTmpSessProtMask = UDS_SESSION_PROT_OTHER_01_MASK;
            break;
        case UDS_SESSION_OTHER_02:
            eUdsSessTypeProt = UDS_SESSION_PROT_OTHER_02;
            eUdsTmpSessProtMask = UDS_SESSION_PROT_OTHER_02_MASK;
            break;
        case UDS_SESSION_OTHER_03:
            eUdsSessTypeProt = UDS_SESSION_PROT_OTHER_03;
            eUdsTmpSessProtMask = UDS_SESSION_PROT_OTHER_03_MASK;
            break;
        case UDS_SESSION_OTHER_04:
            eUdsSessTypeProt = UDS_SESSION_PROT_OTHER_04;
            eUdsTmpSessProtMask = UDS_SESSION_PROT_OTHER_04_MASK;
            break;
        default:
            eUdsSessTypeProt = UDS_SESSION_PROT_DEFAULT;
            eUdsTmpSessProtMask = UDS_SESSION_PROT_DEFAULT_MASK;
            break;
    }
    
    switch(m_eSessionCode)
    {
        case UDS_SESSION_PROT_DEFAULT :
             /* If there is any opened session,
            this is a session entry */
            if(eUdsSessTypeProt != UDS_SESSION_PROT_DEFAULT)
            {
               /* The session timeout is reloaded
                   (for session time measure activation,
                   the counter must be loaded before the session
                   code,because of UDS_Manage () callback behavior
                   concerning session time) */
                    m_uSessionTimeCnt = m_uUdsSessTimeout;
            }
            else
            {
                /* Nothing to do */
            }

            /* If the session have changed */
            if(m_eSessionCode != eUdsSessTypeProt)
            {

                /* Memorise the asked session code */
                m_eSessionCode = eUdsSessTypeProt;

                #if (UDS_SRV_CFG3_MAX)
                /* Initialize in new session entry */
                m_stRoutine.eStart = UDS_ROUTINE_NOT_STARTED;
                #endif

                /* Indicate the session entry */
                UDS_SessionStatusInd (eUdsSessionType);
            }
            else
            {
                /* Nothing to do */
            }
            break;

        case UDS_SESSION_PROT_OTHER_01:
        case UDS_SESSION_PROT_OTHER_02:
        case UDS_SESSION_PROT_OTHER_03:
        case UDS_SESSION_PROT_OTHER_04:
        case UDS_SESSION_PROT_PROGRAMMING :
        case UDS_SESSION_PROT_EXTENDED_DIAG :
        case UDS_SESSION_PROT_SUPPLIER :
            /* If a session is open, and the asked session
               is the standard one, close the current session*/
            if(eUdsSessTypeProt == UDS_SESSION_PROT_DEFAULT)
            {
                 /* The session timeout is stopped */
                 m_uSessionTimeCnt = 0U;
            }
            else
            {
                /* Nothing to do */
            }

            /* If the session have changed */
            if(m_eSessionCode != eUdsSessTypeProt)
            {

                /* Memorise the asked session code */
                m_eSessionCode = eUdsSessTypeProt;

                #if (UDS_SRV_CFG3_MAX)
                /* Initialize in new session entry */
                m_stRoutine.eStart = UDS_ROUTINE_NOT_STARTED;
                #endif
                
                /* Indicate the session entry */
                UDS_SessionStatusInd (eUdsSessionType);
            }
            else
            {
                /* Nothing to do */
            }
            break;

        default :
            /* Indicate of range error */
            m_eSessionCode = m_eSessionCodeDefaultValue;
            NCS_SdfVariableRangeError (SDF_UDS_SESSION_CODE);
            eUdsStatus = UDS_ERR_COHE;
            break;
    }


    if(eUdsStatus == UDS_ACK)
    {
        /* Initialise the session mask according to the session type
           Initialise the timeout session to UDS_SESS_TIMEOUT for
           the programming and the extended diag session
           Implements 000-07-D028-D-80501-1
           Implements 000-07-D028-D-80503-1                         */
        m_eSessionMask = eUdsTmpSessProtMask;
    }

    return eUdsStatus;
}

/* - End UDS_DiagSession                                                         */
/*-------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------*/
/* - Public Callbacks                                                            */


/*********************************************************************************/
/* Name: UDS_EndOfRoutineControlInd                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Manage the end of routine                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : eRoutineStatus : Routine status                   */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_stRoutine : Current routine definition          */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : DIAG_SessionStatusInd                             */
/*********************************************************************************/
#if (UDS_SRV_CFG3_MAX)
void UDS_EndOfRoutineControlInd(tRoutineStatus eRoutineStatus)
{
    /* Update the routine status and indicate the routine is stopped
       Implements 000-07-D028-D-82401-1 */
    m_stRoutine.eStatus = eRoutineStatus;

    m_stRoutine.eStart = UDS_ROUTINE_STOPPED;
}
#endif

/* - End UDS_EndOfRoutineControlInd                                              */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: UDS_Manage                                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Manage the session duration                                      */
/*-------------------------------------------------------------------------------*/
/* Implements: 000-07-D028-D-82001                                               */
/*             000-07-D028-D-82002                                               */
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
/* Called callbacks          : UDS_SessionStatusInd                              */
/*********************************************************************************/

void UDS_Manage (void)
{
    tUdsStatus eUdsStatus;
    eUdsStatus = UDS_NRC_11;

    //RKE_Match_Process();

    switch(m_eSessionCode)
    {
        case UDS_SESSION_PROT_DEFAULT :
            /* Long request management */
            switch (strLongRequest.m_eState)
            {
            case UDS_LONG_REQ_END:
                if (UDS_FALSE == strLongRequest.m_ubtRespPending)
                {
                    /* Stop the long request management */
                    strLongRequest.m_eState = UDS_LONG_REQ_OFF;

                    /* The session timeout is reloaded */
                    m_uSessionTimeCnt = m_uUdsSessTimeout;

                    /* Send response */
                    /* it is the last transmission, it is not required to lock the channel anymore */
                    (void)UDS_LongRequestResponseInd (strLongRequest.m_uwLen, strLongRequest.m_paubData);
                }
                break;

            case  UDS_LONG_REQ_ON:
                if (strLongRequest.m_ubCnt == 1U)
                {
            /* Notify the application for the P2/P2* expiration */
                    UDS_P2AboutToExpireInd();
                }    
            
                if (strLongRequest.m_ubCnt == 0U)
                {
                    /* It's time to send cyclic NRC_78 */
                    /* To be done only if the previous response is completed */
                    if (UDS_FALSE == strLongRequest.m_ubtRespPending)
                    {
                        /* The session timeout is reloaded */
                        m_uSessionTimeCnt = m_uUdsSessTimeout;

                        /* The NRC78 answer timer is reloaded */
                        strLongRequest.m_ubCnt = UDS_P2STAR - 1U;

                        eUdsStatus = UDS_LongRequestResponseInd (UDS_LEN_NRC, strLongRequest.m_aubDiagNack78Data);

                        /* If the transmission is started, lock the NRC_78 channel until acknowledge reception */
                        if (UDS_ACK == eUdsStatus)
                        {
                            strLongRequest.m_ubtRespPending = UDS_TRUE;
                        }
                    }
                }
                /* The session timeout is not elapsed, the management */
                else
                {
                    strLongRequest.m_ubCnt --;
                }
                break;

            default:
                /* No long request */
                /* Nothing to do */
                break;
            }
            break;

        case UDS_SESSION_PROT_OTHER_01:
        case UDS_SESSION_PROT_OTHER_02:
        case UDS_SESSION_PROT_OTHER_03:
        case UDS_SESSION_PROT_OTHER_04:
        case UDS_SESSION_PROT_PROGRAMMING :
        case UDS_SESSION_PROT_EXTENDED_DIAG :
        case UDS_SESSION_PROT_SUPPLIER :
            
                        /* Long request management */
                        switch (strLongRequest.m_eState)
                        {
                        case UDS_LONG_REQ_END:
                            if (UDS_FALSE == strLongRequest.m_ubtRespPending)
                            {
                                /* Stop the long request management */
                                strLongRequest.m_eState = UDS_LONG_REQ_OFF;
            
                                /* The session timeout is reloaded */
                                m_uSessionTimeCnt = m_uUdsSessTimeout;
            
                                /* Send response */
                 
                                /* it is the last transmission, it is not required to lock the channel anymore */
                                (void)UDS_LongRequestResponseInd (strLongRequest.m_uwLen, strLongRequest.m_paubData);
                            }
                            break;
            
                        case  UDS_LONG_REQ_ON:
                            if (strLongRequest.m_ubCnt == 1U)
                            {
                                /* Notify the application for the P2/P2* expiration */
                                UDS_P2AboutToExpireInd();
                            }    
                            
                            if (strLongRequest.m_ubCnt == 0U)
                            {
                                /* It's time to send cyclic NRC_78 */
                                /* To be done only if the previous response is completed */
                                if (UDS_FALSE == strLongRequest.m_ubtRespPending)
                                {
                                    /* The session timeout is reloaded */
                                    m_uSessionTimeCnt = m_uUdsSessTimeout;
            
                                    /* The NRC78 answer timer is reloaded */
                                    strLongRequest.m_ubCnt = UDS_P2STAR - 1;
        
                                    eUdsStatus = UDS_LongRequestResponseInd (UDS_LEN_NRC, strLongRequest.m_aubDiagNack78Data);
        
                                    /* If the transmission is started, lock the NRC_78 channel until acknowledge reception */
                                    if (UDS_ACK == eUdsStatus)
                                    {
                                        strLongRequest.m_ubtRespPending = UDS_TRUE;
                                    }
                                }
                            }
                            /* The session timeout is not elapsed, the management */
                            else
                            {
                                strLongRequest.m_ubCnt --;
                            }
                            break;
            
                        default:
                            /* No long request */
                            /* Nothing to do */
                            break;
                        }


            /* The session timeout is elapsed, then current session is closed */
            if (m_uSessionTimeCnt == 0U)
            {
                m_eSessionCode = UDS_SESSION_PROT_DEFAULT;
                m_eSessionMask = UDS_SESSION_PROT_DEFAULT_MASK;
                /*add by bwf start*/
                System_Control.Control_DTC_Setting=ON;
                SecurityLevel1_Access = FALSE;
                /*add by bwf end*/

#if (UDS_SRV_CFG3_MAX)
                m_stRoutine.eStart = UDS_ROUTINE_NOT_STARTED;
#endif
                /* Indicate the session closing */
                UDS_SessionStatusInd (UDS_SESSION_DEFAULT);
            }
            /* The session timeout is not elapsed, the management */
            else
            {
                m_uSessionTimeCnt --;
            }
            break;

        default:
            /* Indicate of range error */
            m_eSessionCode = m_eSessionCodeDefaultValue;
            NCS_SdfVariableRangeError (SDF_UDS_SESSION_CODE);
            break;
    }
}

/* - Fin UDS_Manage                                                              */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_ReloadTStopDiag                                                     */
/*-------------------------------------------------------------------------------*/
/* Description: Maintain the diagnostic session by reinitializing                */
/*              the T_Stop_Diag counter                                          */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eSessionCode : current session code             */
/* Modified private variables: m_uSessionTimeCnt: current session code           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

void UDS_ReloadTStopDiag (void)
{
    switch(m_eSessionCode)
    {
        case UDS_SESSION_PROT_DEFAULT :
            /* Nothing to do */
        break;

        case UDS_SESSION_PROT_OTHER_01:
        case UDS_SESSION_PROT_OTHER_02:
        case UDS_SESSION_PROT_OTHER_03:
        case UDS_SESSION_PROT_OTHER_04:
        case UDS_SESSION_PROT_PROGRAMMING :
        case UDS_SESSION_PROT_EXTENDED_DIAG :
        case UDS_SESSION_PROT_SUPPLIER :
            {
                /* Protection of the session time counter reload
               (this allow the UDS_Manage () callback to interrupt
               the counter reload without disabling interrupts) */
                tUdsSessionProtType eSessionCodeOld;
                eSessionCodeOld = m_eSessionCode;

                m_eSessionCode = UDS_SESSION_PROT_DEFAULT;

                /* The session timeout is reloaded */
                m_uSessionTimeCnt = m_uUdsSessTimeout;

                /* Memorise the asked session code */
                m_eSessionCode = eSessionCodeOld;
            }
            break;

        default :
            /* Indicate of range error */
            m_eSessionCode = m_eSessionCodeDefaultValue;
            NCS_SdfVariableRangeError (SDF_UDS_SESSION_CODE);
        break;
    }
}

/* - End UDS_ReloadTStopDiag                                                     */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: UDS_LongRequestRespTxConf                                               */
/*-------------------------------------------------------------------------------*/
/* Description: Manage the end of routine                                        */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : None                                                 */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: strLongRequest.m_ubtRespPending                   */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void UDS_LongRequestRespTxConf(void)
{
    strLongRequest.m_ubtRespPending = UDS_FALSE;
}
/* - End UDS_LongRequestRespTxConf                                               */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: UDS_ResetNRC78Timer                                                     */
/*-------------------------------------------------------------------------------*/
/* Description: Reset NRC78 UDS timer                                            */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: strLongRequest.m_ubCnt                            */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void UDS_ResetNRC78Timer(void)
{
    strLongRequest.m_ubCnt = 0;
}
/* - End UDS_ResetNRC78Timer                                                     */
/*-------------------------------------------------------------------------------*/

/*                                                                               */
/*********************************************************************************/
