/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : COM_Pub.h
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

#ifndef COM_PUB_H
#define COM_PUB_H

/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */

/*-------------------------------------------------------------------------------*/
/* - Public Macros                                                               */

/* Wrapper for old version compatibility */
#define COM_GetDynamicFrameLen COM_GetLastRxFrameLen

/*-------------------------------------------------------------------------------*/
/* - NCS Configuration Constants                                                 */

#define COM_MAX_IDENT 2048U


/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                */

/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                           */

/* Declare the external default value of the m_eComMode variable */
extern const tComMode m_eComModeDefaultValue;

/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */

extern void COM_Init (void);

extern tComStatus COM_ModeModify (tComMode eMode);

#ifndef BOOTLOADER
extern tComStatus COM_Periodic (tComPeriodicMode eMode);
#endif

#ifndef BOOTLOADER
extern tComStatus COM_PeriodicFrame (tComPeriodicMode eMode, tComFrameIdx uFrameIdx);
#endif

extern tComStatus COM_SendFrame (tComFrameIdx uFrameIdx);
extern tComStatus COM_SetFrameData (tComFrameIdx uFrameIdx, const u8 *aubData);
extern tComStatus COM_GetFrameData (tComFrameIdx uFrameIdx, u8 *aubData);

#ifndef BOOTLOADER
extern tComStatus COM_MonNodeInit (tComMonNodeIdx uNodeIdx, tComMonStatus eNodeStatus);
#ifdef NCS_SDF_DATA_MIRROR
extern tComStatus COM_GetFrameDataWithMirror (tComFrameIdx uFrameIdx, u8 *aubData, u8 *aubMirror);
#endif
#endif

extern tComStatus COM_GetFrameRxFlag (tComFrameIdx uFrameIdx, tComBoolean *pebRxFlag);

#if (COM_TX_DYNAMIC_FRAME_MAX_RAM)
    extern tComStatus COM_SetDynamicFrameLen (tComFrameIdx uFrameIdx, u8 ubLen);
#endif

extern tComStatus COM_GetLastRxFrameLen (tComFrameIdx uFrameIdx, u8 *pubLen);

/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */

/* Monitored nodes configuration in transmission and reception, stored in Read-Only memory */
extern const tComFrameIdx m_auComMonFrameIdx[COM_MON_NODE_MAX];
extern const u8 m_aubComMonFilterOk[COM_MON_NODE_MAX];
extern const u8 m_aubComMonFilterNok[COM_MON_NODE_MAX];

extern u8 m_ComaaubData[COM_FRAME_MAX_RAM][COM_DATA_MAX];

/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */

extern void COM_Manage (void);
extern void COM_SelectCfg (u8 ubSelectCfg);

/*-------------------------------------------------------------------------------*/
/* - Public Macros                                                               */

/* Callbacks called by COM when a transmission timeout occured */
#define _COM_CALLBACK_TX_MON_TIMEOUT(uFrameIdx) COM_NAsInd (uFrameIdx)

/* Callbacks called by COM when a transmission is confirmed OK (acknowledgement) */
#define _COM_CALLBACK_TX_CONF(uFrameIdx) COM_TxConf (uFrameIdx)

/*                                                                               */
/*********************************************************************************/

#endif      /* COM_PUB_H */
