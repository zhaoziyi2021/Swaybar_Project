/*********************************************************************************/
/*                                                                               */
/*                                  CAN Stack                                    */
/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : EB_Cbk.h
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

#ifndef EB_CBK_H
#define EB_CBK_H


/* Callback called by COM for default management */
extern void APP_COM_MonNodeStatusInd(tComMonNodeIdx uNodeIdx, tComMonStatus eFrameStatus);

/* Callback called by COM on transmission timeout (N_As) */
extern void APP_ComNAsInd (tComFrameIdx uFrameIdx);

/* Callback called by COM when a transmission is confirmed OK (acknowledgement) */
extern void APP_ComTxConf (tComFrameIdx uFrameIdx);

/* Callback called by COM in the COM_ModeModify () function */
extern void APP_ComModeModifyExtension (tComMode eCurMode);

/* Callback called by COM when a frame is transmitted */
extern void APP_ComTxFrameInd (tComFrameIdx uFrameIdx);

/* Callback called when a monitored frame is transmitted or received (after filter) */
extern void APP_ComBackInd (tComMonNodeIdx uNodeIdx);

/* Callback called when a monitored frame is not transmitted or not received (after filter) */
extern void APP_ComLossInd (tComMonNodeIdx uNodeIdx);

#if (CAN_WAKE_UP_DETECTION  == CAN_WAKE_UP_ACTIVATE)
extern void APP_CanWakeUpInd (void);
#endif

/* Error in the CAN controller during CAN_Activate and CAN_Desactivate */
extern void APP_CanCtrlModeErrorInd (void);

/* Callback called by TP for managing transmited segmented frames */
extern void APP_TpDataBufferEmptyInd(tTpMsgIdx uMsgIdx);
/* Callback called by TP when a transmission is complete (OK or NOK)*/
extern void APP_TpTxConf(tTpMsgIdx uMsgIdx, u8 ebStatus);
/* Callback called by TP when a reception is complete (OK or NOK) */
extern void APP_TpRxInd(tTpMsgIdx uMsgIdx, u8 ebStatus);

extern void APP_UdsSessionStatusInd (tUdsSessionType eUdsSessType);

#endif      /* EB_CBK_H */
