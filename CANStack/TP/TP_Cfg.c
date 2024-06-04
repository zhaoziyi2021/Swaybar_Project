/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       :  TP_Cfg.c
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


NCS_PRI_VAR u8 aubDiag1Data[TP_DIAG_MSG_1_DATA_MAX];
NCS_PRI_VAR u8 aubDiag2Data[TP_DIAG_MSG_2_DATA_MAX];

/* Transport Protocol messages configuration, stored in Read-Only memory */
const tTpMsgCfg m_astTpMsgCfg[TP_MSG_MAX] =
    {
        {COM_FRAME_IDX_Diag_BCM_Rsp_0x778, COM_FRAME_IDX_Diag_BCM_RQ_0x770, aubDiag1Data, aubDiag1Data, 300U, (75U / TP_MANAGE_PERIOD), (150U / TP_MANAGE_PERIOD), 1U , 8U, (TP_MSG_MODE_HALF_DUPLEX |TP_MSG_STMIN_FULL | TP_MSG_TYPE_DIAG | TP_MSG_RELOAD_FALSE | TP_MSG_PADDING_TRUE | TP_MSG_RETRANSMISSION_FALSE), 10U},
        {COM_FRAME_IDX_Diag_BCM_Rsp_0x778, COM_FRAME_IDX_Diag_Fun_0x7DF, aubDiag2Data, aubDiag2Data, 300U, (75U / TP_MANAGE_PERIOD), (150U / TP_MANAGE_PERIOD), 1U , 8U, (TP_MSG_MODE_HALF_DUPLEX |TP_MSG_STMIN_FULL | TP_MSG_TYPE_DIAG | TP_MSG_RELOAD_FALSE | TP_MSG_PADDING_TRUE | TP_MSG_RETRANSMISSION_FALSE), 10U},
    };

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
/* - Private Functions                                                           */

/*-------------------------------------------------------------------------------*/
/* - Public Callbacks                                                            */

/*                                                                               */
/*********************************************************************************/


