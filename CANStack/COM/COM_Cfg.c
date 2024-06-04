
/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : COM_Cfg.c
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



/***************************** PRIVATE DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Private Constants                                                           */

/*-------------------------------------------------------------------------------*/
/* - Private Types                                                               */

/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */


/* Data Link Layer frame configuration, stored in Read-Only memory */
/*************************************************
*                  ComConfig 0                   *
*************************************************/

const u16 m_auwComFrameIdent[COM_FRAME_MAX] =
    {
        /* RX */
        0x770,  0x7df,
        /* TX */
        0x778, 
    };


const u8 m_aubComFrameLenNominal[COM_FRAME_MAX] =
    {
        /* RX */
        8, 8, 

        /* TX */
        8, 
    };


const u8 m_aubComFrameLenMin[COM_RX_FRAME_MAX] =
    {
        /* RX */
        1, 1, 
    };


const u8 m_aubComFrameConfig[COM_FRAME_MAX] =
    {
        /* RX */
        COM_MODE_DIRECT, 
        COM_MODE_DIRECT, 

        /* TX */
        COM_MODE_DIRECT,
    };


const u8 m_aaubComFrameInitData[COM_FRAME_MAX][COM_DATA_MAX] =
    {
        /* RX */
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

        /* TX */ 
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

    };


const u8 m_aaubComFrameComLossData[COM_RX_FRAME_MAX][COM_DATA_MAX] =
    {
        /* RX */
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   };


const u8 m_aaubComFrameComLossMask[COM_RX_FRAME_MAX][COM_DATA_MAX] =
    {
        /* RX */
        {0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   };


const tComPeriod m_auComFramePeriod[COM_FRAME_MAX] =
    {
        /* RX */
        0 / COM_MANAGE_PERIOD,
        0 / COM_MANAGE_PERIOD,

        /* TX */
        0 / COM_MANAGE_PERIOD,
    };


const tComPeriod m_auComFrameFirstPeriod[COM_FRAME_MAX] =
    {
        /* RX */
        0 / COM_MANAGE_PERIOD,
        0 / COM_MANAGE_PERIOD,

        /* TX */
        0 / COM_MANAGE_PERIOD,
    };

const tComTxTimeout m_auComFrameTxTimeout[COM_FRAME_MAX] =
    {
        /* RX */
        0 / COM_MANAGE_PERIOD,
        0 / COM_MANAGE_PERIOD,

         /* TX */
        0 / COM_MANAGE_PERIOD,
    };


const tComExTime m_auComFrameExTime[COM_FRAME_MAX] =
    {
        /* RX */
        0 / COM_MANAGE_PERIOD,
        0 / COM_MANAGE_PERIOD,

        /* TX */
        0 / COM_MANAGE_PERIOD,
    };


const tComRxFrameInd m_apfuComFrameRxInd[COM_RX_FRAME_MAX] =
    {
        NULL,
        NULL,
    };


const tComRxDataInd m_apfuComFrameRxDataInd[COM_RX_FRAME_MAX] =
   {
        NULL,
        NULL,
    };


const tComRxDataInd m_apfuComFrameRxInvalidDLCInd[COM_RX_FRAME_MAX] =
    {
        NULL,
        NULL,
    };

/* Frame Idx position in function of the identifier */
const tComFrameIdx m_auwComFrameIdx[COM_MAX_IDENT] =
    {
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
        COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX, COM_FRAME_MAX,
    };

/* This part manage all configuration in case of many configured */
/* Monitored nodes configuration in transmission and reception, stored in Read-Only memory */
const tComFrameIdx m_auComMonFrameIdx[COM_MON_NODE_MAX] =
    {
        COM_FRAME_IDX_Diag_BCM_RQ_0x770,
    };
    
    
const u8 m_aubComMonFilterOk[COM_MON_NODE_MAX] =
    {
        1,
    };


const u8 m_aubComMonFilterNok[COM_MON_NODE_MAX] =
    {
        5,
    };

/* Declare the default value of the m_eComMode variable */
const tComMode m_eComModeDefaultValue = COM_MODE_OFF;

/*-------------------------------------------------------------------------------*/
/* - Private Macros                                                              */

/*-------------------------------------------------------------------------------*/
/* - Private Function Prototypes                                                 */

/*                                                                               */
/*********************************************************************************/
