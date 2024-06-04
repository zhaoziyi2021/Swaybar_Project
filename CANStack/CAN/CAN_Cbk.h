/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : CAN_Cbk.h
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

#ifndef CAN_CBK_H
#define CAN_CBK_H


/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Header Files:                                                               */


/*                                                                               */
/*********************************************************************************/




/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */

extern void CAN_CtrlModeErrorInd (void);

#if (CAN_WAKE_UP_DETECTION  == CAN_WAKE_UP_ACTIVATE)
// Description: Callback called by CAN driver when CAN wake-up is detected
extern void CAN_WakeUpInd (void);
#endif

extern void CAN_TxConf (tCanMailboxIdx uMailboxIdx);
extern void CAN_TxFail (tCanMailboxIdx uMailboxIdx);
extern void CAN_RxFrame (tCanMailboxIdx uMailboxIdx);
extern void CAN_BusOffInd (void);

/*                                                                               */
/*********************************************************************************/

#endif

