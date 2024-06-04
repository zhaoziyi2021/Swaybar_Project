/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : EB_Prg.c
************************************************************************************************
*   Project/Product :
*   Title           :
*   Author          : CATARC
************************************************************************************************
*   Description     : This is common module for body netword
*
************************************************************************************************
*   Limitations     : NONE
*
************************************************************************************************
*   END_FILE_HDR*/

/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
#include "EB_Prj.h"

/*                                                                               */
/*********************************************************************************/

/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */

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

/*-------------------------------------------------------------------------------*/
/* - Private Types                                                               */

/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */

/* Private variable to memorize the value of the TP message Idx used for response pending (NRC78) */
NCS_PRI_VAR u8 m_ubTpMsgIdxNrc78;
    
NCS_PRI_VAR u8 m_ubDiagManageCnt;






/*-------------------------------------------------------------------------------*/
/* - Private Macros                                                              */
/*changed by BWF for when receive consecutive request frame only response the first frame*/
//NCS_PRI_FCT void EB_TpReqManage(void);




/*-------------------------------------------------------------------------------*/
/* - Private Function Prototypes                                                 */

/*                                                                               */
/*********************************************************************************/


/******************************** IMPLEMENTATION *********************************/
/*                                                                               */

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */


/*********************************************************************************/
/* Name: EB_Init                                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: General initialisation function of the integrated CAN layers     */
/*********************************************************************************/
void EB_Init(void)
{
    
    u8 ubFrameIdx;

    u8 ubFrameMonitoredIdx;

    /* Initialization of manage counters */
    m_ubTpMsgIdxNrc78 = EB_NO_NRC_78;
    m_ubDiagManageCnt = 0U;

    /* Init */
    COM_Init ();
    IL_Init ();
    TP_Init ();
    UDS_Init ();


    /* Preprocess symbol to desactivate CAN tranceiver */
#ifndef EB_DISABLE_CAN_TRCV
    /* Map the EN, STB\ and NERR pins of the CAN transceiver on the CPU  */
    /* TJA1040 CAN transceiver in NORMAL mode (EN and STB\ are put to 1) */
    CAN_TransceiverPinCfg();
    CAN_SetTransceiverMode (1U, 1U);
#endif

    /* Initialization of mute and absent frames */
    for(ubFrameMonitoredIdx = 0U; ubFrameMonitoredIdx < COM_MON_NODE_MAX; ubFrameMonitoredIdx++)
    {
        (void)COM_MonNodeInit (ubFrameMonitoredIdx, COM_MON_OK);
    }

    /* CAN activation */
    (void)CAN_Init();

    (void)CAN_Desactivate();

    /* CAN bit timing configuration */
   // (void)CAN_BitTimeCfg();

    /* OSCEB-101: Do not call CAN_Activate */
    /* It will be done only when all MBX are really configured */

    (void)COM_ModeModify (COM_MODE_BUS_ON);


     /* Activation of periodic frames to be send */
    for (ubFrameIdx = COM_RX_FRAME_MAX; ubFrameIdx < COM_FRAME_MAX; ubFrameIdx ++)
    {
        (void)COM_PeriodicFrame (COM_PERIODIC_START, ubFrameIdx);
    }
  
    /* Start ABSENT frames monitoring */
    for (ubFrameIdx = 0U; ubFrameIdx < COM_RX_FRAME_MAX; ubFrameIdx ++)
    {
        (void)COM_PeriodicFrame (COM_PERIODIC_START, ubFrameIdx);
    }
}
/* - End EB_Init                                                                 */
/*-------------------------------------------------------------------------------*/
/*********************************************************************************/
/* Name: EB_TpReqManage                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by the Scheduler                                 */
/*              It links the received frame to the Transport Protocol            */
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
/* Called functions          : COM_GetFrameRxFlag                                */
/*                             COM_GetFrameData                                  */
/*                             OM_GetOperMode                                    */
/*                             TP_ReceiveFrame                                   */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

/*NCS_PRI_FCT*/ void EB_TpReqManage (void)
{
    /* Function status */
    tComStatus eStatus;
    /* Tp frame reception flag */
    u8 ebRxFlag;
    

    /* Check if a Diagnostic request frame has been received */
    eStatus = COM_GetFrameRxFlag (COM_FRAME_IDX_Diag_BCM_RQ_0x770, &ebRxFlag);

    /* EB CAN layer function has been well executed */
    if (eStatus == COM_ERR_OK)
    {
        /* A Diagnostic request frame has been received */
        if (ebRxFlag == COM_TRUE)
        {
            (void)TP_ReceiveFrame (TP_MSG_IDX_Diag_BCM_Phy_RQ);
        }
    }

    /* Check if a Diagnostic request frame has been received */
    eStatus = COM_GetFrameRxFlag (COM_FRAME_IDX_Diag_Fun_0x7DF, &ebRxFlag);

    /* EB CAN layer function has been well executed */
    if (eStatus == COM_ERR_OK)
    {
        /* A Diagnostic request frame has been received */
        if (ebRxFlag == COM_TRUE)
        {
            (void)TP_ReceiveFrame (TP_MSG_IDX_Diag_BCM_Fun_RQ);
        }
    }
}

/* - End EB_TpReqManage                                                          */
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/
/* - Public Callbacks                                                            */

/*********************************************************************************/
/* Name: COM_NAsInd                                                              */
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
/* Modified private variables: m_ubNAsCnt: N_As counter of SUPERVISION frame     */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : TP_ComNAsInd                                      */
/*********************************************************************************/
void COM_NAsInd (tComFrameIdx uFrameIdx)
{
    /* For the Transport Protocol, N_As indication
       Associate the COM frame to the TP message */
    if (uFrameIdx == COM_FRAME_IDX_Diag_BCM_Rsp_0x778)
    {
        TP_ComNAsInd (TP_MSG_IDX_Diag_BCM_Phy_RQ);
    }
    if (uFrameIdx == COM_FRAME_IDX_Diag_BCM_Rsp_0x778)
    {
        TP_ComNAsInd (TP_MSG_IDX_Diag_BCM_Fun_RQ);
    }
    APP_ComNAsInd(uFrameIdx);
}
/* - End COM_NAsInd                                                              */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: COM_TxTimeoutInd                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM when a transmission timeout (N_As)        */
/*              occured, to check if CAN frame abort has to be performed         */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
/* Return value              : status: COM_ERR_OK or COM_ERR_ABORT               */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : TP_ComNAsInd                                      */
/*********************************************************************************/
tComStatus COM_TxTimeoutInd (tComFrameIdx uFrameIdx)
{
    tComStatus eStatus;
    eStatus = COM_ERR_OK;
    /* For the Transport Protocol, N_As indication
       Associate the COM frame to the TP message */
    if (uFrameIdx == COM_FRAME_IDX_Diag_BCM_Rsp_0x778)
    {
        eStatus = COM_ERR_ABORT;
    }
    if (uFrameIdx == COM_FRAME_IDX_Diag_BCM_Rsp_0x778)
    {
        eStatus = COM_ERR_ABORT;
    }
    return eStatus;
}
/* - End COM_TxTimeoutInd                                                        */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: COM_TxConf                                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM when a transmission is confirmed OK       */
/*              (acknowledgement)                                                */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*********************************************************************************/
void COM_TxConf (tComFrameIdx uFrameIdx)
{
    /* For the Transport Protocol, Tx confirmation
    Associate the COM frame to the TP message */
    if (uFrameIdx == COM_FRAME_IDX_Diag_BCM_Rsp_0x778)
    {
        TP_ComTxConf (TP_MSG_IDX_Diag_BCM_Phy_RQ);
    }
    if (uFrameIdx == COM_FRAME_IDX_Diag_BCM_Rsp_0x778)
    {
        TP_ComTxConf (TP_MSG_IDX_Diag_BCM_Fun_RQ);
    }


    APP_ComTxConf(uFrameIdx);
    
}
/* - End COM_TxConf                                                              */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: COM_ComBackInd                                                          */
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
/* Called functions          : IL_BoolWr_PERTE_COM                               */
/* Called macros             : None                                              */
/* Called callbacks          : APP_CAN_ComBackInd                                */
/*********************************************************************************/
void COM_ComBackInd (tComMonNodeIdx uNodeIdx)
{
    APP_ComBackInd (uNodeIdx);
}
/* - End COM_ComBackInd                                                          */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: COM_ComLossInd                                                          */
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
/* Modified private variables: m_ubComLossCnt: PERTE_COM counter of SUPERVISION  */
/*-------------------------------------------------------------------------------*/
/* Called functions          : IL_u8Wr_C_DEFAUT_PERTE_COM                        */
/*                           : IL_BoolWr_PERTE_COM                               */
/*                           : OM_Modify                                         */
/* Called macros             : None                                              */
/* Called callbacks          : APP_CAN_ComLossInd                                */
/*********************************************************************************/
void COM_ComLossInd (tComMonNodeIdx uNodeIdx)
{
    APP_ComLossInd (uNodeIdx);
}
/* - End COM_ComLossInd                                                          */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: COM_ModeModifyExtension                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM in the COM_ModeModify () function,        */
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
/* Called callbacks          : APP_ComModeModifyExtension                        */
/*********************************************************************************/
void COM_ModeModifyExtension (tComMode eCurMode)
{
    /* OSCEB-73 : Init TP layer when the COM layer switches to BUS_ON mode */ 
    if (COM_MODE_BUS_ON == eCurMode) 
    { 
        TP_Init(); 
    } 
    APP_ComModeModifyExtension (eCurMode);
}
/* - End COM_ModeModifyExtension                                                 */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: COM_TxFrameInd                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM before frame transmission                 */
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
void COM_TxFrameInd (tComFrameIdx uFrameIdx)
{
    APP_ComTxFrameInd (uFrameIdx);
}
/* - End COM_TxFrameInd                                                          */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: COM_MonNodeStatusInd                                                    */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by COM to notice NM for default management       */
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
/* Called functions          : NM_MonNode                                        */
/* Called macros             : None                                              */
/* Called callbacks          : APP_COM_MonNodeStatusInd                          */
/*********************************************************************************/
void COM_MonNodeStatusInd(tComMonNodeIdx uNodeIdx, tComMonStatus eFrameStatus)
{
    /* APP indication for default management */
    APP_COM_MonNodeStatusInd(uNodeIdx, eFrameStatus);

}
/* - End COM_MonNodeStatusInd                                                    */
/*-------------------------------------------------------------------------------*/


#ifndef BOOTLOADER
/*********************************************************************************/
/* Name: COM_BusOffRxTxOk                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Callback from COM to indicate to NM a successful TX/RX           */
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*********************************************************************************/
void COM_BusOffRxTxOk (void)
{
}
/* - End COM_BusOffRxTxOk                                                        */
/*-------------------------------------------------------------------------------*/
#endif

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    




/*********************************************************************************/
/* Name: CAN_WakeUpInd                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by CAN when a WAKE-UP is detected by the         */
/*              protocol handler (frame reception)                               */
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
/* Called callbacks          : APP_CanWakeUpInd                                  */
/*********************************************************************************/
#if (CAN_WAKE_UP_DETECTION  == CAN_WAKE_UP_ACTIVATE)
void CAN_WakeUpInd (void)
{
    
    APP_CanWakeUpInd ();
}
#endif
/* - End CAN_WakeUpInd                                                           */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: CAN_CtrlModeErrorInd                                                    */
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
/* Called callbacks          : APP_CanCtrlModeErrorInd                           */
/*********************************************************************************/
void CAN_CtrlModeErrorInd (void)
{
    APP_CanCtrlModeErrorInd ();
}
/* - End CAN_CtrlModeErrorInd                                                    */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: EB_Manage                                                               */
/*-------------------------------------------------------------------------------*/
/* Description: EB cyclic job called by the main loop                            */
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
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void EB_Manage (void)
{

    /* Scheduler to receive a segmented frame */
    EB_TpReqManage();

    /* Communication management (BUS-OFF duration) */
    COM_Manage ();

    /* Transport protocol management (BUS-OFF duration) */
    TP_Manage();

    /* Scheduler for Diagnostic */
    if (m_ubDiagManageCnt >= ((UDS_MANAGE_PERIOD / EB_MANAGE_PERIOD) - 1U))
    {
        m_ubDiagManageCnt = 0U;
        
        /* Diagnostic management (session duration) */
        UDS_Manage ();
    }
    else
    {
        m_ubDiagManageCnt ++;
    }

}
/* - End EB_CAN_Manage                                                           */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: TP_DataBufferEmptyInd                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Call TP buffer is totaly transmitted and need to be reload       */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
void TP_DataBufferEmptyInd (tTpMsgIdx uMsgIdx)
{
    APP_TpDataBufferEmptyInd(uMsgIdx);
}
/* - End TP_DataBufferEmptyInd                                                   */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: TP_TxConf                                                               */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by TP when a transmission is complete (OK or NOK)*/
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                             ebStatus: TP message status                       */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*********************************************************************************/
void TP_TxConf (tTpMsgIdx uMsgIdx, u8 ebStatus)
{
    /* NRC_78 TxConf is received */
    if ( uMsgIdx == m_ubTpMsgIdxNrc78 )
    {
        /* Transmission to UDS */
        UDS_LongRequestRespTxConf();
    }
    
    APP_TpTxConf(uMsgIdx, ebStatus);
}
/* - End TP_TxConf                                                               */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: TP_RxInd                                                                */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by TP when a reception is complete (OK or NOK)   */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                             ebStatus: TP message status                       */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*********************************************************************************/
void TP_RxInd (tTpMsgIdx uMsgIdx, u8 ebStatus)
{
     if ((ebStatus == TP_MSG_STATUS_OK) &&
         (uMsgIdx == TP_MSG_IDX_Diag_BCM_Phy_RQ))
    {
        /* Datas and length of the Diagnostic messages */
        u8 *paubUdsData;
        u16 uwLen;

        /* Transport Protocol functions return status */
        tTpStatus eStatus ;

        /* Read the received message datas */
        eStatus = TP_GetMsgData (uMsgIdx, &uwLen, &paubUdsData);

        /* ISO/FDIS 15765-4:2004(E) p12 */
        /* Always response with peer to peer identifier and never to broadcast */
        switch (uMsgIdx)
        {
            case TP_MSG_IDX_Diag_BCM_Fun_RQ:
                
    uMsgIdx = TP_MSG_IDX_Diag_BCM_Phy_RQ;

                break;

            default:
                break;
        }

        if (eStatus == TP_ERR_OK)
        {
            tUdsBoolean eStatusRxReq;
            /* Diagnostic request treatment */
            eStatusRxReq = UDS_RxRequestWithAddrMode (&uwLen, paubUdsData,UDS_ADDR_PHYSICAL, UDS_TYPE_DIAG_OBD);

            if (eStatusRxReq == UDS_TRUE)
            {            
                /* Answer to the request only it is asked by DIAG layer */
                if (uwLen > 0U)
                {
                    /* Write the answer message datas */
                    eStatus = TP_SetMsgData (uMsgIdx, uwLen, paubUdsData);

                    /* The message datas update is complete without any error */
                    if (eStatus == TP_ERR_OK)
                    {
                        /* Send the Diagnostic answer message */
                        (void)TP_SendMsg (uMsgIdx);
                    }
                    else
                    {
                        /* Nothing to do */
                    }
                }
                else
                {
                    /* Nothing to do */
                }
            }
            /* In the case of a NRC 78 */
            else if (eStatusRxReq == UDS_NRC_78)
            {
                /* Memorize the TpMsgIdx in order to manage the next possible NRC78 */
                /* coming later with UDS_LongRequestResponseInd callback */
                m_ubTpMsgIdxNrc78 = uMsgIdx;

                /* TP reception is locked while current long request is pending */
                (void)TP_RxEnable(m_ubTpMsgIdxNrc78, TP_RX_ENABLE_OFF);
            }
            else
            {
                /* Invalid parameter for function UDS_RxRequest */
                /* Nothing to do */
            }
        }
        else
        {
            /* Nothing to do */
        }
    }/* End of Uds message */
     if ((ebStatus == TP_MSG_STATUS_OK) &&
         (uMsgIdx == TP_MSG_IDX_Diag_BCM_Fun_RQ))
    {
        /* Datas and length of the Diagnostic messages */
        u8 *paubUdsData;
        u16 uwLen;

        /* Transport Protocol functions return status */
        tTpStatus eStatus ;

        /* Read the received message datas */
        eStatus = TP_GetMsgData (uMsgIdx, &uwLen, &paubUdsData);

        /* ISO/FDIS 15765-4:2004(E) p12 */
        /* Always response with peer to peer identifier and never to broadcast */
        switch (uMsgIdx)
        {
            case TP_MSG_IDX_Diag_BCM_Fun_RQ:
                
    uMsgIdx = TP_MSG_IDX_Diag_BCM_Phy_RQ;

                break;

            default:
                break;
        }

        if (eStatus == TP_ERR_OK)
        {
            tUdsBoolean eStatusRxReq;
            /* Diagnostic request treatment */
            eStatusRxReq = UDS_RxRequestWithAddrMode (&uwLen, paubUdsData,UDS_ADDR_FUNCTIONAL, UDS_TYPE_DIAG_OBD);

            if (eStatusRxReq == UDS_TRUE)
            {            
                /* Answer to the request only it is asked by DIAG layer */
                if (uwLen > 0U)
                {
                    /* Write the answer message datas */
                    eStatus = TP_SetMsgData (uMsgIdx, uwLen, paubUdsData);

                    /* The message datas update is complete without any error */
                    if (eStatus == TP_ERR_OK)
                    {
                        /* Send the Diagnostic answer message */
                        (void)TP_SendMsg (uMsgIdx);
                    }
                    else
                    {
                        /* Nothing to do */
                    }
                }
                else
                {
                    /* Nothing to do */
                }
            }
            /* In the case of a NRC 78 */
            else if (eStatusRxReq == UDS_NRC_78)
            {
                /* Memorize the TpMsgIdx in order to manage the next possible NRC78 */
                /* coming later with UDS_LongRequestResponseInd callback */
                m_ubTpMsgIdxNrc78 = uMsgIdx;

                /* TP reception is locked while current long request is pending */
                (void)TP_RxEnable(m_ubTpMsgIdxNrc78, TP_RX_ENABLE_OFF);
            }
            else
            {
                /* Invalid parameter for function UDS_RxRequest */
                /* Nothing to do */
            }
        }
        else
        {
            /* Nothing to do */
        }
    }/* End of Uds message */
    APP_TpRxInd (uMsgIdx, ebStatus);
}
/* - End TP_RxInd                                                                */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: TP_RxEnableInd                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Disable the TP frame reception for a given channel               */
/*              in a NRC_78                                                      */
/*-------------------------------------------------------------------------------*/
/* Implements:                                                                   */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                           : uRxEnable: enable/disable request                 */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_ubTpMsgIdxNrc78 : previous NRC_78 memorized     */
/*                           : m_ubTpUds2MsgIdxNrc78 : previous NRC_78 memorized */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
void TP_RxEnableInd (tTpMsgIdx uMsgIdx, tTpRxEnable uRxEnable)
{
    /* A long request is still pending */
    if ( (uMsgIdx == m_ubTpMsgIdxNrc78) && (TP_RX_ENABLE_ON == uRxEnable))
    {
        /* Re lock the TP channel because NRC_78 is not completed */
        (void)TP_RxEnable(m_ubTpMsgIdxNrc78, TP_RX_ENABLE_OFF);
    }
    else
    {
        /* Nothing to do */
    }
}
/* - End TP_RxEnableInd                                                            */
/*-------------------------------------------------------------------------------*/








/*********************************************************************************/
/* Name: UDS_SessionStatusInd                                                    */
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
/* Called functions          : Il_SetSignalByte                                  */
/* Called macros             : None                                              */
/* Called callbacks          : APP_UdsSessionStatusInd                           */
/*********************************************************************************/
void UDS_SessionStatusInd (tUdsSessionType eUdsSessType)
{    
    /* Diagnostic session beginning, put the SESS_DIAG bit to 1 */
    if (eUdsSessType == UDS_SESSION_EXTENDED_DIAG)
    {
    #ifdef PSA_IL_SESSION_DIAG
        /* Update the SESS_DIAG bit of the SUPERVISION frame1) */
        IL_BoolWr_SessDiag (1U) ;
    #endif

    #ifdef TELE_INCLUDE
        TELE_OpenSession();
    #endif
    }

    /* Diagnostic session ending, put the SESS_DIAG bit to 0 */
    else if (eUdsSessType == UDS_SESSION_DEFAULT)
    {
    #ifdef PSA_IL_SESSION_DIAG
        /* Update the SESS_DIAG bit of the SUPERVISION frame */
        IL_BoolWr_SessDiag (0U) ;
    #endif

    #ifdef TELE_INCLUDE
        TELE_CloseSession();
    #endif
    }
    else
    {

    }

    APP_UdsSessionStatusInd (eUdsSessType);
}
/* - End UDS_SessionStatusInd                                                    */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: UDS_LongRequestResponseInd                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Callback called by UDS to update long request diagnostic status  */
/*                                                                               */
/*********************************************************************************/
/* Input parameters          : uwLen, Data length                                */
/*                             aubUdsData : Data bytes u8                        */
/* Output parameters         : None                                              */
/* Return value              : UDS Status                                        */
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
tUdsStatus UDS_LongRequestResponseInd (u16 uwLen, u8 *aubUdsData)
{
    tTpStatus eStatus ;
    
    if (aubUdsData == NULL)
    {
        eStatus = UDS_ERR_COHE;
    }
    else
    {
        /* Write the answer message data */
        eStatus = TP_SetMsgData(m_ubTpMsgIdxNrc78, uwLen, aubUdsData);
        
        /* The message datas update is complete without any error */
        if (eStatus == TP_ERR_OK)
        {
            /* Send the Diagnostic answer message */
            (void)TP_SendMsg(m_ubTpMsgIdxNrc78);
        }

         /* NRC_78 tranmission request */
        if ((aubUdsData[0] == UDS_NRC) && (aubUdsData[2] == UDS_NRC_78))
        {
            /* nothing to do */
            /* NRC_78 will be sent */
        }
        else
        {
            /* Unlock TP channel because NRC_78 is completed */
            (void)TP_RxEnable(m_ubTpMsgIdxNrc78, TP_RX_ENABLE_ON);
            
            /* End of long request, flag is reset */
            m_ubTpMsgIdxNrc78 = EB_NO_NRC_78; 
        }
        eStatus = UDS_ACK;
    }
    return eStatus;
}
/* - End UDS_LongRequestResponseInd                                              */
/*-------------------------------------------------------------------------------*/




