/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       :  TP_Cbk.h
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

#ifndef TP_CBK_H
#define TP_CBK_H



extern void TP_TxConf (tTpMsgIdx uMsgIdx, u8 ebStatus);
extern void TP_RxInd (tTpMsgIdx uMsgIdx, u8 ebStatus);

extern void TP_DataBufferEmptyInd (tTpMsgIdx uMsgIdx);
extern void TP_RxEnableInd (tTpMsgIdx uMsgIdx, tTpRxEnable uRxEnable);

#endif      /* TP_CBK_H */
