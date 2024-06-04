/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : COM_Cbk.h
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

#ifndef COM_CBK_H
#define COM_CBK_H

/* Description: Callback called by Com when a frame is confirmed transmitted */
extern void COM_TxConf (tComFrameIdx uFrameIdx);

/* Description: Callback called by Com when a N_As timeout occur on a frame */
extern void COM_NAsInd (tComFrameIdx uFrameIdx);

#if (COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS)
/* Description: Callback called by Com when a N_As timeout occurs on a frame, to check if CAN frame abort has to be performed */
    extern tComStatus COM_TxTimeoutInd (tComFrameIdx uFrameIdx);
#endif

/* Callback called by COM when a monitored frame is transmitted or received (after filter) */
extern void COM_ComBackInd (tComMonNodeIdx uNodeIdx);

/* Callback called by COM when a monitored frame is not transmitted or not received (after filter) */
extern void COM_ComLossInd (tComMonNodeIdx uNodeIdx);

/* Callback called by COM in the COM_ModeModify () function */
extern void COM_ModeModifyExtension (tComMode eCurMode);

/* Description: Callback called by COM when a frame is transmitted */
extern void COM_TxFrameInd (tComFrameIdx uFrameIdx);

/* Callback called to notice NM for default management */
extern void COM_MonNodeStatusInd(tComMonNodeIdx uNodeIdx, tComMonStatus eFrameStatus);

#ifndef BOOTLOADER
/* Callback RSA called from COM to indicate to NM successfull RX/TX */
extern void COM_BusOffRxTxOk (void);
#endif






#endif      /* COM_CBK_H */
