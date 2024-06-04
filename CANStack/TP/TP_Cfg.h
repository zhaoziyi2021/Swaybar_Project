/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       :  TP_Cfg.h
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


#ifndef TP_CFG_H
#define TP_CFG_H


/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Configuration Constants                                                     */

/* used in order to set the CAN type */
#define TP_EB_PSA_CAN_LS_2010       0U
#define TP_EB_PSA_CAN_HS_IS_2010    1U
#define TP_EB_PSA_CAN_HS_M_2010     2U
#define TP_EB_ISO                   3U

#define TP_EB_CAN_TYPE          TP_EB_ISO


/* TP messages number managed by this software layer */
/* Range : (0 .. 2^32) */
#define TP_MSG_MAX                  2U

#ifdef NCS_UNITARY_TEST
/* Transport Protocol messages configuration, stored in Read-Only memory */
extern u16 m_aubRxEnableCnt[TP_MSG_MAX];
#endif
extern const tTpMsgCfg m_astTpMsgCfg[TP_MSG_MAX];


#define TP_DIAG_MSG_1_DATA_MAX           300U
#define TP_DIAG_MSG_2_DATA_MAX           300U
/* Communication dynamic frames in reception management authorization */
#ifdef COM_TX_DYNAMIC_FRAME_MAX_RAM
    #define TP_COM_TX_DYNAMIC_FRAME_MAX        COM_TX_DYNAMIC_FRAME_MAX_RAM
#else
    /* For compatibility with previous version of COM module */
    #define TP_COM_TX_DYNAMIC_FRAME_MAX        COM_TX_DYNAMIC_FRAME_MAX
#endif

/*-------------------------------------------------------------------------------*/
/* - Configuration Variable or Register Aliases                                  */

/*-------------------------------------------------------------------------------*/
/* - Public Constants                                                            */

/* TP messages index definition (TP_Cfg.h)*/

#define TP_MSG_IDX_Diag_BCM_Phy_RQ    0U
#define TP_MSG_IDX_Diag_BCM_Fun_RQ    1U
/*-------------------------------------------------------------------------------*/
/* - Public Macros                                                               */

/*                                                                               */
/*********************************************************************************/





#endif      /* TP_CFG_H */
