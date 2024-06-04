
/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       :  TP_Prg.c
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

/* Frame N_PCI definition */
#define TP_NPCI                         0xF0U
#define TP_NPCI_SINGLE_FRAME            0x00U
#define TP_NPCI_FIRST_FRAME             0x10U
#define TP_NPCI_CONSECUTIVE_FRAME       0x20U
#define TP_NPCI_FLOW_CONTROL            0x30U

/* Frame length definition */
#define TP_LEN_SINGLE_FRAME             8U
#define TP_LEN_FIRST_FRAME              8U
#define TP_LEN_MIN_CONSECUTIVE_FRAME    2U
#define TP_LEN_MAX_CONSECUTIVE_FRAME    8U
#define TP_LEN_FLOW_CONTROL             3U

/* Data length definition */
#define TP_DATA_MAX_SINGLE_FRAME        7U
#define TP_DATA_MAX_FIRST_FRAME         6U
#define TP_DATA_MAX_CONSECUTIVE_FRAME   7U
#define TP_DATA_MAX_FLOW_CONTROL        0U

/* Flow Control Flow Status definition */
#define TP_FS_CLEAR_TO_SEND             0U
#define TP_FS_WAIT                      1U
#define TP_FS_OVERFLOW                  2U

#define TP_STMIN_MAX                    170U

/*-------------------------------------------------------------------------------*/
/* - Private Types                                                               */

/* TP message internal status */
#define TP_MSG_STATUS_NONE              0U

#define TP_MSG_STATUS_TX_SF             1U
#define TP_MSG_STATUS_TX_FF             2U
#define TP_MSG_STATUS_TX_CF             3U
#define TP_MSG_STATUS_TX_FC             4U

#define TP_MSG_STATUS_RX_SF             5U
#define TP_MSG_STATUS_RX_FF             6U
#define TP_MSG_STATUS_RX_CF             7U
#define TP_MSG_STATUS_RX_FC             8U

#define TP_MSG_STATUS_TX_FC_OVERFLOW    9U
#define TP_MSG_STATUS_TX_CF_STMIN       10U

/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */

/* Transport Protocol messages */
NCS_PRI_VAR tTpMsg m_astTpMsg[TP_MSG_MAX];

/* Transport Protocol counters */
NCS_PRI_VAR tTpBufferData m_uTpBufferCnt[TP_MSG_MAX];
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
#ifndef BOOTLOADER
/* Monitored nodes status. Used for message retransmission */
NCS_PRI_VAR tNmMonNodeIdx m_auNodeStatus[TP_MSG_MAX];
/* TP msg retransmission counter */
NCS_PRI_VAR u8 m_aubTxCnt[TP_MSG_MAX];
#endif
#endif

/* Rx enable counter */
/* If diffent from 0, the reception is not allowed for the channel. */
NCS_PRI_VAR u16 m_aubRxEnableCnt[TP_MSG_MAX];


/*-------------------------------------------------------------------------------*/
/* - Private Macros                                                              */

/*-------------------------------------------------------------------------------*/
/* - Private Function Prototypes                                                 */

NCS_PRI_FCT tTpStatus TP_ReceiveSingleFrame (tTpMsgIdx uMsgIdx, u8 *aubData);
NCS_PRI_FCT tTpStatus TP_ReceiveFirstFrame (tTpMsgIdx uMsgIdx, u8 *aubData);
NCS_PRI_FCT tTpStatus TP_ReceiveConsecutiveFrame (tTpMsgIdx uMsgIdx, u8 *aubData);
NCS_PRI_FCT tTpStatus TP_ReceiveFlowControl (tTpMsgIdx uMsgIdx, u8 *aubData);

NCS_PRI_FCT tTpStatus TP_SendSingleFrame (tTpMsgIdx uMsgIdx);
NCS_PRI_FCT tTpStatus TP_SendFirstFrame (tTpMsgIdx uMsgIdx);
NCS_PRI_FCT tTpStatus TP_SendConsecutiveFrame (tTpMsgIdx uMsgIdx);
NCS_PRI_FCT tTpStatus TP_SendFlowControl (tTpMsgIdx uMsgIdx, u8 ubFlowStatus);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
NCS_PRI_FCT void TP_TxRetransmission (tTpMsgIdx uMsgIdx, u8 ebStatus);
#endif

/*                                                                               */
/*********************************************************************************/




/******************************** IMPLEMENTATION *********************************/
/*                                                                               */

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */


/*********************************************************************************/
/* Name: TP_Init                                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: Initialize the TP layer messages parameters                      */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

void TP_Init (void)
{
    tTpMsgIdx uMsgIdx;

    for (uMsgIdx = 0; uMsgIdx < TP_MSG_MAX; uMsgIdx ++)
    {
        /* Source code optimisation : pointer on TP message */
        tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];

        /* Source code optimisation : pointer on TP message configuration */
        tTpMsgCfg const *const pstTpMsgCfg = &m_astTpMsgCfg[uMsgIdx];

        pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;
        pstTpMsg->ebRxStatus = TP_MSG_STATUS_NONE;
        pstTpMsg->uTxCnt = 0;
        pstTpMsg->uRxCnt = 0;
        pstTpMsg->paubTxData = pstTpMsgCfg->paubTxData;
        pstTpMsg->paubRxData = pstTpMsgCfg->paubRxData;
        pstTpMsg->ubRxSTmin = pstTpMsgCfg->ubRxSTmin;
        pstTpMsg->ubRxBs = pstTpMsgCfg->ubRxBs;

        m_uTpBufferCnt[uMsgIdx] = 0;
        #if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
        #ifndef BOOTLOADER
        m_auNodeStatus[uMsgIdx] = NM_MON_NOK;
        m_aubTxCnt[uMsgIdx] = 0;
        #endif
        #endif
        m_aubRxEnableCnt[uMsgIdx] = 0;
    }
}

/* - Fin TP_Init                                                                 */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: TP_RxEnable                                                             */
/*-------------------------------------------------------------------------------*/
/* Description: Enable/disable the TP frame reception for a given channel        */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                           : uRxEnable: enable/disable request                 */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astTpMsg[uMsgIdx].ebRxStatus                    */
/* Modified private variables: m_aubRxEnableCnt: Counter to lock reception       */
/*                             m_astTpMsg: TP messages reception                 */
/*-------------------------------------------------------------------------------*/
/* Called functions          : TP_AbortMsg                                       */
/* Called macros             : None                                              */
/* Called callbacks          : TP_RxEnableInd                                    */
/*********************************************************************************/

tTpStatus TP_RxEnable (tTpMsgIdx uMsgIdx, tTpRxEnable uRxEnable)
{
    /* Return status */
    tTpStatus eStatus = TP_ERR_OK;

    /* The message index input parameter is in expected range */
    if ((uMsgIdx < TP_MSG_MAX) && (uRxEnable < TP_RX_ENABLE_MAX))
    {
        if (uRxEnable == TP_RX_ENABLE_ON )
        {
            m_aubRxEnableCnt[uMsgIdx] = 0;
        }
        else
        {
            m_aubRxEnableCnt[uMsgIdx] = TP_RX_ENABLE_CPT_VALUE;
            if ( m_astTpMsg[uMsgIdx].ebRxStatus != TP_MSG_STATUS_NONE )
            {
                /* Stop TP message reception */
                (void)TP_AbortMsg ( uMsgIdx );
            }
        }

        /* Callback for the application */
        /* Warning: to avoid recursive code, don't use the callback here */
        /* It can happen if TP_RxEnable is called from the callback to maintain the lock */
        /* ------------------------------------ */
        /* TP_RxEnableInd (uMsgIdx, uRxEnable); */
        /* ------------------------------------ */
    }
    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_RxEnable                                                             */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_SetStmin                                                             */
/*-------------------------------------------------------------------------------*/
/* Description: Set the Stmin parameter                                          */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

tTpStatus TP_SetStmin (tTpMsgIdx uMsgIdx, u8 ubRxStmin)
{
    /* Return status */
    tTpStatus eStatus = TP_ERR_OK;


    /* The message index input parameter is in expected range */
    if (uMsgIdx < TP_MSG_MAX)
    {
        /* Source code optimisation : pointer on TP message */
        tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];

        if(ubRxStmin > TP_STMIN_MAX)
        {
            ubRxStmin = TP_STMIN_MAX;
        }

        /* set stmin parameter */
        pstTpMsg->ubRxSTmin = ubRxStmin;
    }
    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_SetStmin                                                                 */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_SetBS                                                                */
/*-------------------------------------------------------------------------------*/
/* Description: Set the Block Size parameter                                     */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

tTpStatus TP_SetBS (tTpMsgIdx uMsgIdx, u8 ubRxBs)
{
    /* Return status */
    tTpStatus eStatus = TP_ERR_OK;


    /* The message index input parameter is in expected range */
    if (uMsgIdx < TP_MSG_MAX)
    {
        /* Source code optimisation : pointer on TP message */
        tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];

        /* set BS parameter */
        pstTpMsg->ubRxBs = ubRxBs;
    }
    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_SetBS                                                                */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: TP_SendMsg                                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Send a message using the Transport Protocol                      */
/*              With the message length, it chooses to transmit a Single Frame   */
/*              or a First Frame followed by Consecutive Frames                  */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_IDX: message index is out of range         */
/*                             TP_ERR_COM_OFF: transmission is not allowed       */
/*                             TP_ERR_COM_IDX: frame index is out of range       */
/*                             TP_ERR_COM_FRAME_MODE: frame is in periodic mode  */
/*                             TP_ERR_COM_TX_MSG_LOST: transmission is pending   */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : TP_SendSingleFrame                                */
/*                             TP_SendFirstFrame                                 */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

tTpStatus TP_SendMsg (tTpMsgIdx uMsgIdx)
{
    /* Return status (no initialization because value is alwais change before return) */
    tTpStatus eStatus;

    /* The message index input parameter is in expected range */
    if (uMsgIdx < TP_MSG_MAX)
    {
        /* If the message length is lower to TP_DATA_MAX_SINGLE_FRAME,
           transmit a Single Frame */
        if (m_astTpMsg[uMsgIdx].uwTxLen <= TP_DATA_MAX_SINGLE_FRAME)
        {
            eStatus = TP_SendSingleFrame (uMsgIdx);
        }
        /* If the message length is upper to TP_DATA_MAX_SINGLE_FRAME,
           transmit a First Frame followed by Consecutive Frames */
        else
        {
            eStatus = TP_SendFirstFrame (uMsgIdx);
        }

        /* Cancel segmentation if the COM layer rejects the transmission request */
        if (TP_MSG_STATUS_OK != eStatus)
        {
            (void)TP_AbortMsg(uMsgIdx);
            TP_TxConf (uMsgIdx, TP_MSG_STATUS_COM_NOK);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
            TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_COM_NOK);
#endif
        }
    }

    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }


    /* Return the status */
    return (eStatus);
}

/* - Fin TP_SendMsg                                                              */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_SetMsgData                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Message transmission buffer update                               */
/*              In fact, only the TP transmission data buffer pointer is updated */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                             uwLen: TP message datalength                      */
/*                             paubData: TP message data buffer address          */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_IDX: message index is out of range         */
/*                             TP_ERR_LEN: message datalength is out of range    */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#if (TP_DATA_COPY == TP_DATA_COPY_ADR)

tTpStatus TP_SetMsgData (tTpMsgIdx uMsgIdx, u16 uwLen, u8 *paubData)
{
    /* Return status */
    tTpStatus eStatus = TP_ERR_OK;

     /* The message index input parameter is in expected range */
    if (uMsgIdx < TP_MSG_MAX)
    {
        /* The message datalength is superior than buffer capacity or equal to 0,
           then error */
        if ( (m_astTpMsgCfg[uMsgIdx].ubConfig & TP_MSG_RELOAD ) == TP_MSG_RELOAD_FALSE)
        {
            if ((uwLen > m_astTpMsgCfg[uMsgIdx].uwDataMax) || (uwLen == 0))
            {
                eStatus = TP_ERR_LEN;
            }
            /* The message datalength is OK */
            else
            {
                /* Memorise the message length */
                m_astTpMsg[uMsgIdx].uwTxLen = uwLen;

                /* Memorise the message data buffer address */
                m_astTpMsg[uMsgIdx].paubTxData = paubData;
            }
        }
        else
        {
            if (uwLen == 0)
            {
                eStatus = TP_ERR_LEN;
            }
            /* The message datalength is OK */
            else
            {
                /* Memorise the message length */
                m_astTpMsg[uMsgIdx].uwTxLen = uwLen;

                /* Memorise the message data buffer address */
                m_astTpMsg[uMsgIdx].paubTxData = paubData;
            }
        }

    }

    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }


    /* Return the status */
    return (eStatus);
}
#endif

/* - Fin TP_SetMsgData                                                           */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_SetMsgDataCopy                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Message transmission buffer update                               */
/*              In fact, all the TP transmission data buffer is updated with the */
/*              data buffer in parameter (each byte is copied)                   */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                             uwLen: TP message datalength                      */
/*                             paubData: TP message data buffer                  */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_IDX: message index is out of range         */
/*                             TP_ERR_LEN: message datalength is out of range    */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#if (TP_DATA_COPY == TP_DATA_COPY_BUF)

tTpStatus TP_SetMsgDataCopy (tTpMsgIdx uMsgIdx, u16 uwLen, u8 *paubData)
{
    /* Return status */
    tTpStatus eStatus = TP_ERR_OK;

    /* The message index input parameter is in expected range */
    if (uMsgIdx < TP_MSG_MAX)
    {
        /* The message datalength is superior than buffer capacity or equal to 0,
           then error */
        if ((uwLen > m_astTpMsgCfg[uMsgIdx].uwDataMax) || (uwLen == 0))
        {
            eStatus = TP_ERR_LEN;
        }
        /* The message datalength is OK */
        else
        {
            /* Memorise the message length */
            m_astTpMsg[uMsgIdx].uwTxLen = uwLen;

            /* Copy the message data buffer to the TP message data buffer */
            {
                /* Index to use the data buffer */
                u16 uwIdx;

                for (uwIdx = 0; uwIdx < uwLen; uwIdx ++)
                {
                    m_astTpMsg[uMsgIdx].paubTxData[uwIdx] = paubData[uwIdx];
                }
            }
        }
    }

    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }


    /* Return the status */
    return (eStatus);
}
#endif
/* - Fin TP_SetMsgDataCopy                                                       */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_GetMsgData                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Message reception buffer reading                                 */
/*              In fact, only the TP reception data buffer pointer is copied     */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                             paubData: TP message data buffer address          */
/* Output parameters         : puwLen: TP message datalength pointer             */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_IDX: message index is out of range         */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#if (TP_DATA_COPY == TP_DATA_COPY_ADR)

tTpStatus TP_GetMsgData (tTpMsgIdx uMsgIdx, u16 *puwLen, u8 **paubData)
{
    /* Return status */
    tTpStatus eStatus = TP_ERR_OK;


    /* The message index input parameter is in expected range */
    if (uMsgIdx < TP_MSG_MAX)
    {
        if(puwLen != NULL)
        {
            /* Read the message length */
            *puwLen = m_astTpMsg[uMsgIdx].uwRxLen;
        }
        if(paubData != NULL)
        {
            /* Read the message data buffer address */
            *paubData = m_astTpMsg[uMsgIdx].paubRxData;
        }
    }

    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }


    /* Return the status */
    return (eStatus);
}
#endif
/* - Fin TP_GetMsgData                                                           */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_GetMsgDataCopy                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Message reception buffer reading                                 */
/*              In fact, all the TP reception data buffer is copied into the     */
/*              data buffer in parameter (each byte is copied)                   */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/*                             paubData: TP message data buffer address          */
/* Output parameters         : puwLen: TP message datalength pointer             */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_IDX: message index is out of range         */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#if (TP_DATA_COPY == TP_DATA_COPY_BUF)

tTpStatus TP_GetMsgDataCopy (tTpMsgIdx uMsgIdx, u16 *puwLen, u8 *paubData)
{
    /* Return status */
    tTpStatus eStatus = TP_ERR_OK;

    /* The message index input parameter is in expected range */

    if (uMsgIdx < TP_MSG_MAX)
    {
        /* Read the message length */
        if (puwLen != NULL)
        {
            *puwLen = m_astTpMsg[uMsgIdx].uwRxLen;
        }


        /* Copy the TP message data buffer to the message data buffer */
        {
            /* Index to use the data buffer */
            u16 uwIdx;

            if (paubData != NULL)
            {
                for (uwIdx = 0; uwIdx < m_astTpMsg[uMsgIdx].uwRxLen; uwIdx ++)
                {
                    paubData[uwIdx] = m_astTpMsg[uMsgIdx].paubRxData[uwIdx];
                }
            }
        }
    }

    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }


    /* Return the status */
    return (eStatus);
}
#endif

/* - Fin TP_GetMsgDataCopy                                                       */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_ReceiveFrame                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Receive a frame (Single Frame, First Frame, Consecutive Frame    */
/*              or Flow Control)                                                 */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_IDX: message index is out of range         */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astTpMsgCfg: TP messages configuration          */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_GetFrameData                                  */
/*                             COM_GetDynamicFrameLen                            */
/*                             TP_SendFlowControl                                */
/*                             TP_SendConsecutiveFrame                           */
/* Called macros             : None                                              */
/* Called callbacks          : TP_RxInd                                          */
/*                             TP_TxConf                                         */
/*********************************************************************************/

tTpStatus TP_ReceiveFrame (tTpMsgIdx uMsgIdx)
{
    /* Return status */
    tTpStatus eStatus;
    eStatus = TP_ERR_OK;

    /* The message index input parameter is in expected range */
    if (uMsgIdx < TP_MSG_MAX)
    {
    	/* Reception is enabled for the channel */
        if (m_aubRxEnableCnt[uMsgIdx] == 0U)
        {
            /* Temporary data buffer for TP frame constitution */
            u8 aubData[COM_DATA_MAX];

            tComStatus eComStatus;

            /* Source code optimisation : pointer on TP message configuration */
            tTpMsgCfg const *const pstTpMsgCfg = &m_astTpMsgCfg[uMsgIdx];

            /* Read the COM frame data buffer */
            eComStatus = COM_GetFrameData (pstTpMsgCfg->uRxFrameIdx, aubData);

            if (eComStatus == COM_ERR_OK)
            {
                /* Identify the received N_PCI */
                switch (aubData[0] & TP_NPCI)
                {
                    
                    /* Single Frame reception */
                    case TP_NPCI_SINGLE_FRAME :
                        eStatus = TP_ReceiveSingleFrame (uMsgIdx, aubData);
                        break;

                    /* First Frame reception */
                    case TP_NPCI_FIRST_FRAME :
                        /*change by bwf for Functional request does not support multi frame message communication start*/
                    	if(pstTpMsgCfg->uRxFrameIdx == 0)   //10--0x7DF
                    	{
                    		eStatus = TP_ReceiveFirstFrame (uMsgIdx, aubData);
                    	}
                    	else
                    	{

                    	}
                    	/*change by bwf for Functional request does not support multi frame message communication end*/
                        //eStatus = TP_ReceiveFirstFrame (uMsgIdx, aubData);
                        break;

                    /* Consecutive Frame reception */
                    case TP_NPCI_CONSECUTIVE_FRAME :
                        eStatus = TP_ReceiveConsecutiveFrame (uMsgIdx, aubData);
                        break;

                    /* Flow Control reception */
                    case TP_NPCI_FLOW_CONTROL :
                        eStatus = TP_ReceiveFlowControl (uMsgIdx, aubData);
                        break;

                    /* Unknown N_PDU reception : it is ignored */
                    default :
                        /* Nothing to do */
                        break;
                }
            }
            else
            {
                /*Data error, nothing to do */
            }
        }    /* end of if (m_aubRxEnableCnt[uMsgIdx] == 0) */
        /* The frame reception is actually forbidden */
        else
        {
            eStatus = TP_ERR_RX_DISABLED;
        }
    }

    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_ReceiveFrame                                                         */
/*-------------------------------------------------------------------------------*/





/*-------------------------------------------------------------------------------*/
/* - Private Functions                                                           */


/*********************************************************************************/
/* Name: TP_ReceiveSingleFrame                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Receive a Single Frame                                           */
/*********************************************************************************/
NCS_PRI_FCT tTpStatus TP_ReceiveSingleFrame (tTpMsgIdx uMsgIdx, u8 *aubData)
{
    /* Return status */
    tTpStatus eStatus;

    /* Received frame length */
    u8 ubLen;

    tComStatus eComStatus;

    /* Source code optimisation : pointer on TP message */
    tTpMsg * pstTpMsg;
    tTpMsgCfg const * pstTpMsgCfg;
    pstTpMsg = &m_astTpMsg[uMsgIdx];
    pstTpMsgCfg = &m_astTpMsgCfg[uMsgIdx];
    eStatus = TP_ERR_OK;


    /* Whatever the reception status and if there is no transmission in progress (1)
       or if a transmission is in progress and the message is in FULL-DUPLEX mode (2),
       this is a new reception */
    if ((pstTpMsg->ebTxStatus == TP_MSG_STATUS_NONE) ||       /* (1) */
         ((pstTpMsgCfg->ubConfig & TP_MSG_MODE) == TP_MSG_MODE_FULL_DUPLEX)) /* (2) */
    {
        /* If a reception is in progress, the previous reception is terminated */
        if (pstTpMsg->ebRxStatus != TP_MSG_STATUS_NONE)
        {
            pstTpMsg->ebRxStatus = TP_MSG_STATUS_NONE;

            TP_RxInd (uMsgIdx, TP_MSG_STATUS_NPDU);
        }
        /* In other cases, nothing to do */
        else
        {
            /* Nothing to do */
        }
        
        /* Frame reception */
        /* The SF_DL field of the Single Frame plus 1 is equal to the frame length
           and SF_DL is superior than 1, the protocol is respected */
        

        /* Reading the received data Length for the frame in Padding */
        eComStatus = COM_GetDynamicFrameLen (pstTpMsgCfg->uRxFrameIdx, &ubLen);                                


        if (eComStatus == COM_ERR_OK)
        {
            /* Frame length is only tested when padding is not used */
            if ((((pstTpMsgCfg->ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_TRUE) && (ubLen == COM_DATA_MAX))||
                (((pstTpMsgCfg->ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_FALSE) && ((ubLen == ((aubData[0] & 0x0FU) + 1U)) && (ubLen > 1U))))
            {
                /* Only value of SF_DL from 1 to 7 is valid */
                if (((aubData[0] & 0x0FU) > 0U) && ((aubData[0] & 0x0FU) < 8U))
                {
                    /* InterruptDisable */
                    /* The status is : Single Frame reception */
                    pstTpMsg->ebRxStatus = TP_MSG_STATUS_RX_SF;

                    /* Timeout and delay reception counter initialisation */
                    /* (there is no frame to wait) */
                    pstTpMsg->uRxCnt = 0;
                    /* InterruptEnable */


                    /* Get the message length */
                    pstTpMsg->uwRxLen = (u16) (aubData[0] & 0x0FU);

                    /* Copy the temporary buffer for COM frame to the TP message data */
                    {
                        /* Index to use the data buffer */
                        u8 ubIdx;

                        for (ubIdx = 0; ubIdx < (pstTpMsg->uwRxLen); ubIdx ++)
                        {
                            pstTpMsg->paubRxData[ubIdx] = aubData[ubIdx + 1];
                        }
                    }

                    /* End of message reception */
                    pstTpMsg->ebRxStatus = TP_MSG_STATUS_NONE;

                    /* Message reception indication */
                    TP_RxInd (uMsgIdx, TP_MSG_STATUS_OK);
                }
            }

#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
            /* For PSA variants, implement 463.229-0300-01E.1 requirement for diagnosis messages */
            else if ((pstTpMsgCfg->ubConfig & TP_MSG_TYPE) == TP_MSG_TYPE_DIAG)
            {
                TP_RxInd (uMsgIdx, TP_MSG_STATUS_DLC);
            }
#endif

            /* Protocol error, the frame is ignored */
            else
            {
            }
        }
        else
        {
            /*COM_GetDynamicFrameLen error, nothing to do */
        }
    }
    /* In other cases, the received frame is ignored */
    else
    {
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_ReceiveSingleFrame                                                   */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: TP_ReceiveFirstFrame                                                    */
/*-------------------------------------------------------------------------------*/
/* Description: Receive a First Frame                                            */
/*********************************************************************************/
NCS_PRI_FCT tTpStatus TP_ReceiveFirstFrame (tTpMsgIdx uMsgIdx, u8 *aubData)
{
    /* Return status */
    tTpStatus eStatus;

    /* Received frame length */
    u8 ubLen;

    tComStatus eComStatus;

    /* Source code optimisation : pointer on TP message */
    tTpMsg * pstTpMsg;
    tTpMsgCfg const * pstTpMsgCfg;
    pstTpMsg = &m_astTpMsg[uMsgIdx];
    pstTpMsgCfg = &m_astTpMsgCfg[uMsgIdx];
    eStatus = TP_ERR_OK;
    

    /* Whatever the reception status and if there is no transmission in progress (1)
       or if a transmission is in progress and the message is in FULL-DUPLEX mode (2),
       this is a new reception */
    if ((pstTpMsg->ebTxStatus == TP_MSG_STATUS_NONE) ||       /* (1) */
         ((pstTpMsgCfg->ubConfig & TP_MSG_MODE) == TP_MSG_MODE_FULL_DUPLEX)) /* (2) */
    {
        /* If a reception is in progress, the previous reception is terminated */
        if (pstTpMsg->ebRxStatus != TP_MSG_STATUS_NONE)
        {
            pstTpMsg->ebRxStatus = TP_MSG_STATUS_NONE;

            TP_RxInd (uMsgIdx, TP_MSG_STATUS_NPDU);
        }
        /* In other cases, nothing to do */
        else
        {
              /* Nothing to do */
        }


        /* Frame reception */
        /* The First Frame length respects the protocol */
        /* Reading the received data Length for the frame in Padding */
        eComStatus = COM_GetDynamicFrameLen (pstTpMsgCfg->uRxFrameIdx, &ubLen);
        
        if (eComStatus == COM_ERR_OK)
        {
            /* Frame length is only tested when padding is not used */
            if (ubLen == TP_LEN_FIRST_FRAME)
            {
                /* Get the message length */
                pstTpMsg->uwRxLenCnt = (u16)(((u16)(aubData[0] & 0x0FU) << 8U) + (u16)(aubData[1]));

                /* The FF_DL field of the First Frame respects the protocol */
                if (pstTpMsg->uwRxLenCnt > TP_DATA_MAX_SINGLE_FRAME)
                {
                    /* InterruptDisable */
                    /* The status is : First Frame reception */
                    pstTpMsg->ebRxStatus = TP_MSG_STATUS_RX_FF;

                    /* Timeout and delay reception counter initialisation */
                    /* (there is no frame to wait) */
                    pstTpMsg->uRxCnt = 0;
                    /* InterruptEnable */


                    /* The reception buffer size is sufficient to receive this message */
                    if (pstTpMsg->uwRxLenCnt <= pstTpMsgCfg->uwDataMax)
                    {
                        /* Message length memorisation */
                        pstTpMsg->uwRxLen = pstTpMsg->uwRxLenCnt;

                        /* The next frame that will be received is the first Consecutive Frame */
                        pstTpMsg->ubRxSnCnt = 1;

                        /* Copy the temporary buffer for COM frame to the TP message data */
                        {
                            /* Index to use the data buffer */
                            u8 ubIdx;

                            for (ubIdx = 0; ubIdx < TP_DATA_MAX_FIRST_FRAME; ubIdx ++)
                            {
                                pstTpMsg->paubRxData[ubIdx] = aubData[ubIdx + 2U];
                            }
                        }

                        /* Received data bytes number memorisation */
                        pstTpMsg->uwRxLenCnt = TP_DATA_MAX_FIRST_FRAME;

                        /* Send the Flow Control (Clear To Send) */
                        eStatus = TP_SendFlowControl (uMsgIdx, TP_FS_CLEAR_TO_SEND);
                        if (TP_MSG_STATUS_OK != eStatus)
                        {
                            (void)TP_AbortMsg(uMsgIdx);
                            TP_RxInd (uMsgIdx, TP_MSG_STATUS_COM_NOK);
                        }
                    }

                    /* The reception buffer size is not sufficient to receive this message,
                       then transmission of an Overflow Flow Control */
                    else
                    {
                        /* Send the Flow Control (Overflow) */
                        eStatus = TP_SendFlowControl (uMsgIdx, TP_FS_OVERFLOW);
                        if (TP_MSG_STATUS_OK != eStatus)
                        {
                            (void)TP_AbortMsg(uMsgIdx);
                            TP_RxInd (uMsgIdx, TP_MSG_STATUS_COM_NOK);
                        }
                    }
                }
            
#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
                /* For PSA variants, implement 463.229-0300-01E.1 requirement for diagnosis messages */
                /* A FF is received but the complete message data size is lower than 7. */
                /* Therefore, it should be managed with a SF */
                else if ((pstTpMsgCfg->ubConfig & TP_MSG_TYPE) == TP_MSG_TYPE_DIAG)
                {
                    TP_RxInd (uMsgIdx, TP_MSG_STATUS_DLC);
                }
#endif
            
                /* Protocol error, the frame is ignored */
                else
                {
                }
            }
        
#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
            /* For PSA variants, implement 463.229-0300-01E.1 requirement for diagnosis messages */
            else if ((pstTpMsgCfg->ubConfig & TP_MSG_TYPE) == TP_MSG_TYPE_DIAG)
            {
                TP_RxInd (uMsgIdx, TP_MSG_STATUS_DLC);
            }
#endif
        
            /* Protocol error, the frame is ignored */
            else
            {
            }
        }
        else
        {
            /*COM_GetDynamicFrameLen, nothing to do */
        }
    }
    /* In other cases, the received frame is ignored */
    else
    {
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_ReceiveFirstFrame                                                    */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: TP_ReceiveConsecutiveFrame                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Receive a Consecutive Frame                                      */
/*********************************************************************************/
NCS_PRI_FCT tTpStatus TP_ReceiveConsecutiveFrame (tTpMsgIdx uMsgIdx, u8 *aubData)
{
    /* Return status */
    tTpStatus eStatus;

    /* Received frame length */
    u8 ubLen;

    tComStatus eComStatus;

    /* Source code optimisation : pointer on TP message */
    tTpMsg * pstTpMsg;
    tTpMsgCfg const * pstTpMsgCfg;
    pstTpMsg = &m_astTpMsg[uMsgIdx];
    pstTpMsgCfg = &m_astTpMsgCfg[uMsgIdx];
    eStatus = TP_ERR_OK;
    

    /* If a reception is in progress (that can be waiting for the TxOK 
       of a Flow Control or for the reception of a Consecutive Frame),
       this Consecutive Frame is used for the message reception */
    if ((pstTpMsg->ebRxStatus != TP_MSG_STATUS_NONE) &&
        (pstTpMsg->ebRxStatus != TP_MSG_STATUS_TX_FC_OVERFLOW))
    {

        /* Frame reception */
        /* Reading the received data Length for the frame in Padding */
        eComStatus = COM_GetDynamicFrameLen (pstTpMsgCfg->uRxFrameIdx, &ubLen);    
        
        if (eComStatus == COM_ERR_OK)
        {

            /* If the minimal frame length is respected */
            /* Frame length must be to 8 when the padding is used */
            if ((((pstTpMsgCfg->ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_TRUE) && (ubLen == COM_DATA_MAX)) ||
                (((pstTpMsgCfg->ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_FALSE) && (ubLen >= TP_LEN_MIN_CONSECUTIVE_FRAME)))
            {
                /* If the SN field is correct */
                if ((pstTpMsg->ubRxSnCnt & 0x0FU) == (aubData[0] & 0x0FU))
                {
                    /* The Consecutive Frame length respects the protocol:
                       it is 8 if there is more than 7 bytes to receive (1) or
                       it is to the data bytes number to receive plus 1 (2) */
                    /* Frame length must be to 8 when the padding is used */
                    if ((((pstTpMsgCfg->ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_TRUE) &&     (ubLen == COM_DATA_MAX) )
                        || (((pstTpMsgCfg->ubConfig & TP_MSG_PADDING)== TP_MSG_PADDING_FALSE) && 
                          ((((pstTpMsg->uwRxLen - pstTpMsg->uwRxLenCnt) > 7) &&  /* (1) */
                          (ubLen == 8)) ||                                     /* (1) */
                         (((pstTpMsg->uwRxLen - pstTpMsg->uwRxLenCnt) <= 7) && /* (2) */
                          (ubLen == (u8)((pstTpMsg->uwRxLen - pstTpMsg->uwRxLenCnt) + 1))))))/* (2) */
                    {
                        /* InterruptDisable */
                        /* The ebRxStatus variable must not be put to the TP_MSG_STATUS_RX_CF value
                           because it has been already done after a Flow Control transmission (COM_TpTxConf)
                           But if the Consecutive Frame is received before the Flow Control TxConf
                           (this is due to Software architecture), it must be done */
                        pstTpMsg->ebRxStatus = TP_MSG_STATUS_RX_CF;

                        /* Timeout and delay reception counter initialisation */
                        /* (there is no frame to wait) */
                        pstTpMsg->uRxCnt = 0;
                        /* InterruptEnable */



                        /* The next Consecutive Frame SN is calculated */
                        pstTpMsg->ubRxSnCnt ++;


                        /* Copy the temporary buffer for COM frame to the TP message data */
                        {
                            /* Index to use the data buffer */
                            u8 ubIdx;

                            for (ubIdx = 1; (pstTpMsg->uwRxLenCnt < pstTpMsg->uwRxLen) && (ubIdx < COM_DATA_MAX); ubIdx ++)
                            {
                                /* The byte is copied */
                                pstTpMsg->paubRxData[pstTpMsg->uwRxLenCnt] = aubData[ubIdx];
                                
                                /* Increment uwRxLenCnt */
                                /* Shall not be done in the for(), MISRA restriction */
                                pstTpMsg->uwRxLenCnt ++;
                            }
                        }



                        /* All the data bytes have been received, end of message reception */
                        if (pstTpMsg->uwRxLenCnt == pstTpMsg->uwRxLen)
                        {
                            /* InterruptDisable */
                            /* End of message reception */
                            pstTpMsg->ebRxStatus = TP_MSG_STATUS_NONE;
                            /* InterruptEnable */

                            /* Message reception indication */
                            TP_RxInd (uMsgIdx, TP_MSG_STATUS_OK);
                        }
                        /* There are other data bytes to receive */
                        else
                        {
                            /* Next step is :
                               Wait for another Consecutive Frame because
                               the Flow Control is not required (1)
                               or the bloc is not complete (2) */

                            if (pstTpMsg->ubRxBs != 0)
                            {
                                pstTpMsg->ubRxBsCnt--;
                            }


                            if ((pstTpMsg->ubRxBs == 0) ||      /* (1) */
                                ((pstTpMsg->ubRxBsCnt) != 0))
                            {
                                /* InterruptDisable */
                                /* Timeout and delay reception counter initialisation */
                                /* (there is another Consecutive Frame to wait) */
                                pstTpMsg->uRxCnt = pstTpMsgCfg->uNCr;
                                /* InterruptEnable */
                            }
                            /* Transmit a Flow Control because the bloc is complete */
                            else
                            {
                                /* Send the Flow Control (Clear To Send) */
                                eStatus = TP_SendFlowControl (uMsgIdx, TP_FS_CLEAR_TO_SEND);
                                if (TP_MSG_STATUS_OK != eStatus)
                                {
                                    (void)TP_AbortMsg(uMsgIdx);
                                    TP_RxInd (uMsgIdx, TP_MSG_STATUS_COM_NOK);
                                }
                            }
                        }
                    }
                                                
#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
                    /* For PSA variants, implement 463.229-0300-01E.1 requirement for diagnosis messages */
                    else if ((pstTpMsgCfg->ubConfig & TP_MSG_TYPE) == TP_MSG_TYPE_DIAG)
                    {
                        (void)TP_AbortMsg(uMsgIdx);
                        TP_RxInd (uMsgIdx, TP_MSG_STATUS_DLC);
                    }
#endif
                                            
                    /* Protocol error, the frame is ignored */
                    else
                    {
                    }
                }

                /* SN field error */
                else
                {
                    /* End of message reception */
                    /* (no interrupt protection is necessary because the uRxCnt variable
                       is equal to 0 and TP_Manage does not read ebRxStatus in this case) */
                    pstTpMsg->ebRxStatus = TP_MSG_STATUS_NONE;

                    /* Timeout and delay reception counter initialisation */
                    /* (there is no frame to wait) */
                    pstTpMsg->uRxCnt = 0;
                    
#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
                    /* For PSA variants, implement 463.229-0300-01E.1 requirement for diagnosis messages */
                    if ((pstTpMsgCfg->ubConfig & TP_MSG_TYPE) == TP_MSG_TYPE_DIAG)
                    {
                        (void)TP_AbortMsg(uMsgIdx);
                    }
#endif
                    /* Message reception indication */
                    TP_RxInd (uMsgIdx, TP_MSG_STATUS_SN);
                }
            }
#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
            /* For PSA variants, implement 463.229-0300-01E.1 requirement for diagnosis messages */
            else if ((pstTpMsgCfg->ubConfig & TP_MSG_TYPE) == TP_MSG_TYPE_DIAG)
            {
                (void)TP_AbortMsg(uMsgIdx);
                TP_RxInd (uMsgIdx, TP_MSG_STATUS_DLC);
            }
#endif
            /* Protocol error, the frame is ignored */
            else
            {
            }
        }
        else
        {
            /*COM_GetDynamicFrameLen error, nothing to do */
        }
    }
    /* In other cases, the received frame is ignored */
    else
    {
        if (pstTpMsg->ebRxStatus != TP_MSG_STATUS_NONE)
        {
            (void)TP_AbortMsg(uMsgIdx);
            TP_RxInd (uMsgIdx, TP_MSG_STATUS_NPCI);
        }
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_ReceiveConsecutiveFrame                                              */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: TP_ReceiveFlowControl                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Receive a Flow Control Frame                                     */
/*********************************************************************************/
NCS_PRI_FCT tTpStatus TP_ReceiveFlowControl (tTpMsgIdx uMsgIdx, u8 *aubData)
{
    /* Return status */
    tTpStatus eStatus;

    /* Received frame length */
    u8 ubLen;

    tComStatus eComStatus;

    /* Source code optimisation : pointer on TP message */
    tTpMsg * pstTpMsg;
    tTpMsgCfg const * pstTpMsgCfg;
    pstTpMsg = &m_astTpMsg[uMsgIdx];
    pstTpMsgCfg = &m_astTpMsgCfg[uMsgIdx];
    eStatus = TP_ERR_OK;


    /* If a Flow Control is awaited */
    /* this Flow Control is used for the message transmission
       (that can be waiting for the TxOK of a First Frame or Consecutive Frame
       or waiting for the Flow Control reception) */

    /* OSCTP-51 : Remove this section to avoid corruption of the expected nb of CF */
    /* within the current BS, when an unexpected FC is received (must be ignored) */
    /*
    if ((pstTpMsg->ebTxStatus == TP_MSG_STATUS_TX_CF) &&
        (pstTpMsg->ubTxBs != 0))
    {
        (pstTpMsg->ubTxBsCnt)--;
    }
    */
    if (((pstTpMsg->ebTxStatus == TP_MSG_STATUS_TX_FF) || (pstTpMsg->ebTxStatus == TP_MSG_STATUS_RX_FC)) ||
        ((pstTpMsg->ebTxStatus == TP_MSG_STATUS_TX_CF) && ((pstTpMsg->ubTxBs != 0) && (pstTpMsg->ubTxBsCnt == 0)))
        )
    {

        /* Frame reception */
        /* The Flow Control length respects the protocol */
/* Reading the received data Length for the frame in Padding */
        eComStatus = COM_GetDynamicFrameLen (pstTpMsgCfg->uRxFrameIdx, &ubLen);    
        
        if (eComStatus == COM_ERR_OK)
        {

            /* Frame length is only tested when padding is not used */
            if ((((pstTpMsgCfg->ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_TRUE) && (ubLen == COM_DATA_MAX)) ||
                ( ((pstTpMsgCfg->ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_FALSE) && (ubLen == TP_LEN_FLOW_CONTROL)))
            {
                /* InterruptDisable */
                /* The ebRxStatus variable must not be put to the TP_MSG_STATUS_RX_FC value
                   because it has been already done after a First Frame or
                   Consecutive Frame transmission (COM_TpTxConf) */

                /* Timeout and delay reception counter initialisation */
                /* (there is no frame to wait) */
                pstTpMsg->uTxCnt = 0;
                /* InterruptEnable */

                /* If it is the first Flow Control of the message transfer, */
                /* its parameters must be memorised for all this transfer */
                if (pstTpMsg->uwTxLenCnt == TP_DATA_MAX_FIRST_FRAME)
                {
                    /* Memorise the Flow Control parameters for transmission */
                    pstTpMsg->ubTxBs = aubData[1];
                    pstTpMsg->ubTxSTmin = aubData[2];

                    /* The Flow Status is ClearToSend */
                    if ((aubData[0] & 0x0FU) == TP_FS_CLEAR_TO_SEND)
                    {
                        /* It is the beginning of a Block */
                        pstTpMsg->ubTxBsCnt = pstTpMsg->ubTxBs;

                        /* Send a Consecutive Frame */
                        eStatus = TP_SendConsecutiveFrame (uMsgIdx);
                        if (TP_MSG_STATUS_OK != eStatus)
                        {
                            (void)TP_AbortMsg(uMsgIdx);
                            TP_TxConf (uMsgIdx, TP_MSG_STATUS_COM_NOK);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                            TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_COM_NOK);
#endif
                        }
                    }
                    /* The Flow Status is Wait, then wait for another Flow Control */
                    else if ((aubData[0] & 0x0FU) == TP_FS_WAIT)
                    {
                        /* Timeout and delay reception counter initialisation */
                        /* (there is a Flow Control to wait) */
                        pstTpMsg->uTxCnt = pstTpMsgCfg->uNBs;
                        
                        /* Check maximum of FC Wait received */
                        if ( pstTpMsg->ubRxFCWaitCnt > 0 )
                        {
                            /* Decrement FC Wait counter */
                            pstTpMsg->ubRxFCWaitCnt--;
                        }
                        else
                        {
                            /* End of message transmission */
                            pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;
                            TP_TxConf (uMsgIdx, TP_MSG_STATUS_WFT_OVRN);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                            TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_WFT_OVRN);
#endif
                        }
                    }
                    /* The Flow Status is Overflow, then end of message transmission */
                    else if ((aubData[0] & 0x0FU) == TP_FS_OVERFLOW)
                    {
                        /* End of message transmission */
                        /* (no interrupt protection is necessary because the uTxCnt variable
                           is equal to 0 and TP_Manage does not read ebTxStatus in this case) */
                        pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;

                        /* Message transmission confirmation */
                        TP_TxConf (uMsgIdx, TP_MSG_STATUS_OVFLOW);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                        TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_OVFLOW);
#endif
                    }
                    /* The Flow Status is invalid, then end of message transmission */
                    else
                    {
                        /* End of message transmission */
                        /* (no interrupt protection is necessary because the uTxCnt variable
                           is equal to 0 and TP_Manage does not read ebTxStatus in this case) */
                        pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;

                        /* Message transmission confirmation */
                        TP_TxConf (uMsgIdx, TP_MSG_STATUS_FS);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                        TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_FS);
#endif
                    }
                }

                /* If it is not the first Flow Control of the message transfer,
                   its parameters (BS and STmin) must be identical as in
                   the first received Flow Control */
                else
                {
#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
                    /* The BS and STmin values are correct */
                    if ((pstTpMsg->ubTxBs == aubData[1]) &&
                        (pstTpMsg->ubTxSTmin == aubData[2]))
                    {
#endif
                        /* The Flow Status is ClearToSend */
                        if ((aubData[0] & 0x0FU) == TP_FS_CLEAR_TO_SEND)
                        {
                            /* It is the beginning of a Block */
                            pstTpMsg->ubTxBsCnt = pstTpMsg->ubTxBs;

                            /* Send a Consecutive Frame */
                            eStatus = TP_SendConsecutiveFrame (uMsgIdx);
                            if (TP_MSG_STATUS_OK != eStatus)
                            {
                                (void)TP_AbortMsg(uMsgIdx);
                                TP_TxConf (uMsgIdx, TP_MSG_STATUS_COM_NOK);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                                TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_COM_NOK);
#endif
                            }
                        }
                        /* The Flow Status is Wait, then wait for another Flow Control */
                        else if ((aubData[0] & 0x0FU) == TP_FS_WAIT)
                        {
                            /* Timeout and delay reception counter initialisation */
                            /* (there is a Flow Control to wait) */
                            pstTpMsg->uTxCnt = pstTpMsgCfg->uNBs;
                            
                            /* Check maximum of FC Wait received */
                            if ( pstTpMsg->ubRxFCWaitCnt > 0 )
                            {
                                /* Decrement FC Wait counter */
                                pstTpMsg->ubRxFCWaitCnt--;
                            }
                            else
                            {
                                /* End of message transmission */
                                pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;
                                TP_TxConf (uMsgIdx, TP_MSG_STATUS_WFT_OVRN);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                            TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_WFT_OVRN);
#endif
                            }
                        }
                        /* The Flow Status is Overflow or invalid, then end of
                           message transmission with error, because the
                           Flow Control (Overflow) is only allowed after the First Frame */
                        else
                        {
                            /* End of message transmission */
                            /*   (no interrupt protection is necessary because the uTxCnt variable
                               is equal to 0 and TP_Manage does not read ebTxStatus in this case) */
                            pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;

                            /* Message transmission confirmation */
                            TP_TxConf (uMsgIdx, TP_MSG_STATUS_FS);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                            TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_FS);
#endif
                        }
#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
                    }

                    /* The BS and STmin values are not correct, then
                       end of transmission with error */
                    else
                    {
                        /* End of message transmission */
                        /* (no interrupt protection is necessary because the uTxCnt variable
                           is equal to 0 and TP_Manage does not read ebTxStatus in this case) */
                        pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;

                        /* Message transmission confirmation */
                        TP_TxConf (uMsgIdx, TP_MSG_STATUS_NPDU);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                        TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_NPDU);
#endif
                    }
#endif
                }
            }

#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
            /* For PSA variants, implement 463.229-0300-01E.1 requirement for diagnosis messages */
            else if ((pstTpMsgCfg->ubConfig & TP_MSG_TYPE) == TP_MSG_TYPE_DIAG)
            {
                (void)TP_AbortMsg(uMsgIdx);
                TP_TxConf (uMsgIdx, TP_MSG_STATUS_DLC);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_DLC);
#endif
            }
#endif

            /* Protocol error, the frame is ignored */
            else
            {
            }
        }
        else
        {
            /*COM_GetDynamicFrameLen error, nothing to do */
        }
    }

#if ((TP_EB_CAN_TYPE == TP_EB_PSA_CAN_LS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_IS_2010) || (TP_EB_CAN_TYPE == TP_EB_PSA_CAN_HS_M_2010))
    /* For PSA variants, implement 463.229-0300-01E.1 requirement for diagnosis messages */
    else if ((((pstTpMsgCfg->ubConfig & TP_MSG_TYPE) == TP_MSG_TYPE_DIAG) &&
             ((pstTpMsg->ebTxStatus == TP_MSG_STATUS_TX_CF) || (pstTpMsg->ebTxStatus == TP_MSG_STATUS_TX_CF_STMIN))) &&
             ((pstTpMsg->ubTxBs == 0) || (pstTpMsg->ubTxBsCnt != 0)))
    {
        (void)TP_AbortMsg(uMsgIdx);
        TP_TxConf (uMsgIdx, TP_MSG_STATUS_NPCI);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
        TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_NPCI);
#endif
    }
#endif

    else
    {
        /* In other cases, the received frame is ignored */
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_ReceiveFlowControl                                                   */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: TP_SendSingleFrame                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Send a Single Frame                                              */
/*              At the function call, the uwLen parameter of the TP message      */
/*              is lower to 8                                                    */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_COM_OFF: transmission is not allowed       */
/*                             TP_ERR_COM_IDX: frame index is out of range       */
/*                             TP_ERR_COM_FRAME_MODE: frame is in periodic mode  */
/*                             TP_ERR_COM_TX_MSG_LOST: transmission is pending   */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astTpMsgCfg: TP messages configuration          */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_SetFrameData                                  */
/*                             COM_SetDynamicFrameLen                            */
/*                             COM_SendFrame                                     */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tTpStatus TP_SendSingleFrame (tTpMsgIdx uMsgIdx)
{
    /* Temporary data buffer for TP frame constitution */
    u8 aubData[COM_DATA_MAX];

    /* Index to use the data buffer */
    u8 ubIdx;



    {
        /* Source code optimisation : pointer on TP message */
        tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];


        /* The status is : Single Frame transmission */
        pstTpMsg->ebTxStatus = TP_MSG_STATUS_TX_SF;

        /* Timeout and delay transmission counter initialisation */
        /* (there is no frame to wait) */
        pstTpMsg->uTxCnt = 0;


        /* Write the N_PCI and SF_DL fields of the Single Frame */
        aubData[0] = (u8)((pstTpMsg->uwTxLen & 0x0FU) | TP_NPCI_SINGLE_FRAME);

        /* Copy the TP message data to the temporary buffer for COM frame transmission */
        /* (there is no need to check that uwLen < TP_DATA_MAX_SINGLE_FRAME, because */
        /* this verification is done before TP_SendSingleFrame () function call) */
        for (ubIdx = 0; ubIdx < pstTpMsg->uwTxLen; ubIdx ++)
        {
            aubData[ubIdx + 1] = pstTpMsg->paubTxData[ubIdx];
        }
    }


    {
        tComStatus eComStatus;

        /* Source code optimisation : pointer on TP message transmission frame index */
        tComFrameIdx const *const puTxFrameIdx = (const tComFrameIdx *)(&m_astTpMsgCfg[uMsgIdx].uTxFrameIdx);


        /* Write the COM frame length (because it is dynamic) */
        /* (the ubIdx is equal to m_astTpMsg[uMsgIdx].uwLen after the for loop) */
        /* Frame length is only written when padding is not used */
        if ((m_astTpMsgCfg[uMsgIdx].ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_FALSE)
        {
            #ifdef TP_COM_TX_DYNAMIC_FRAME_MAX
              #if (TP_COM_TX_DYNAMIC_FRAME_MAX > 0U)
                eComStatus = COM_SetDynamicFrameLen (*puTxFrameIdx, (u8)(ubIdx + 1));
              #else
                /* Initialization of eComStatus to avoid warning */
                eComStatus = COM_ERR_OK;
              #endif
            #else
              #error TP_COM_TX_DYNAMIC_FRAME_MAX must be defined
            #endif
        }
        else
        {
            /* to continue the program */
            eComStatus = COM_ERR_OK;
        }

        if(eComStatus == COM_ERR_OK)
        {
            /* Fill not used byte with padding value */
            /* ubIdx already points on the last databyte */
            for (; ubIdx < (COM_DATA_MAX - 1); ubIdx++)
            {
                aubData[ubIdx + 1] = TP_PADDING_BYTE_VALUE;
            }
            /* Update the COM frame data buffer */
            eComStatus = COM_SetFrameData (*puTxFrameIdx, aubData);

            if(eComStatus == COM_ERR_OK)
            {
                /* Request COM frame transmission */
                eComStatus = COM_SendFrame (*puTxFrameIdx);
            }
        }

        return (eComStatus);

    }
}

/* - Fin TP_SendSingleFrame                                                      */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_SendFirstFrame                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Send a First Frame                                               */
/*              At the function call, the uwLen parameter of the TP message      */
/*              is upper to 7                                                    */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_COM_OFF: transmission is not allowed       */
/*                             TP_ERR_COM_IDX: frame index is out of range       */
/*                             TP_ERR_COM_FRAME_MODE: frame is in periodic mode  */
/*                             TP_ERR_COM_TX_MSG_LOST: transmission is pending   */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astTpMsgCfg: TP messages configuration          */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_SetFrameData                                  */
/*                             COM_SetDynamicFrameLen                            */
/*                             COM_SendFrame                                     */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tTpStatus TP_SendFirstFrame (tTpMsgIdx uMsgIdx)
{
    /* Temporary data buffer for TP frame constitution */
    u8 aubData[COM_DATA_MAX];

    /* Index to use the data buffer */
    u8 ubIdx;


    {
        /* Source code optimisation : pointer on TP message */
        tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];

        /* Source code optimisation : pointer on TP message configuration */
        tTpMsgCfg const *const pstTpMsgCfg = &m_astTpMsgCfg[uMsgIdx];

        /* The status is : First Frame transmission */
        pstTpMsg->ebTxStatus = TP_MSG_STATUS_TX_FF;

        /* Write the N_PCI and FF_DL fields of the First Frame */
        aubData[0] = (u8)((u8)((u16)(pstTpMsg->uwTxLen >> 8U) & 0x000FU) | TP_NPCI_FIRST_FRAME);
        aubData[1] = (u8)(pstTpMsg->uwTxLen);

        /* Copy the TP message data to the temporary buffer for COM frame transmission
           (there is no need to check that ubIdx < uwLen, because at TP_SendFirstFrame ()
           function call, uwLen is upper to TP_DATA_MAX_FIRST_FRAME) */
        for (ubIdx = 0; ubIdx < TP_DATA_MAX_FIRST_FRAME; ubIdx ++)
        {
            aubData[ubIdx + 2] = pstTpMsg->paubTxData[ubIdx];
        }

        /* SN parameter for Consecutive Frames transmission */
        pstTpMsg->ubTxSnCnt = 1;

        /* Current transmitted data length */
        pstTpMsg->uwTxLenCnt = ubIdx;
        m_uTpBufferCnt[uMsgIdx] = ubIdx;

        /* Timeout and delay transmission counter initialisation
           (there is no frame to wait before transmission confirmation) */
        pstTpMsg->uTxCnt = 0;
        
        /* FC Wait counter initialization */
        pstTpMsg->ubRxFCWaitCnt = pstTpMsgCfg->ubRxFCWaitMax;
    }



    {
        tComStatus eComStatus;

        /* Source code optimisation : pointer on TP message transmission frame index */
        tComFrameIdx const *const puTxFrameIdx = (const tComFrameIdx *)(&m_astTpMsgCfg[uMsgIdx].uTxFrameIdx);


        /* Write the COM frame length (because it is dynamic) */
        /* Frame length is only written when padding is not used */
        if ((m_astTpMsgCfg[uMsgIdx].ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_FALSE)
        {
            #ifdef TP_COM_TX_DYNAMIC_FRAME_MAX
              #if (TP_COM_TX_DYNAMIC_FRAME_MAX > 0U)
                eComStatus = COM_SetDynamicFrameLen (*puTxFrameIdx, TP_LEN_FIRST_FRAME);
              #else
                /* Initialization of eComStatus to avoid warning */
                eComStatus = COM_ERR_OK;
              #endif
            #else
                #error TP_COM_TX_DYNAMIC_FRAME_MAX must be defined
            #endif
        }
        else
        {
            /* to continue the program */
            eComStatus = COM_ERR_OK;
        }
        

        if(eComStatus == COM_ERR_OK)
        {
            /* Update the COM frame data buffer */
            eComStatus = COM_SetFrameData (*puTxFrameIdx, aubData);

            if(eComStatus == COM_ERR_OK)
            {
                /* Request COM frame transmission */
                eComStatus = COM_SendFrame (*puTxFrameIdx);
            }
        }

        return (eComStatus);
    }


}

/* - Fin TP_SendFirstFrame                                                       */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: TP_SendConsecutiveFrame                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: Send a Consecutive Frame                                         */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_COM_OFF: transmission is not allowed       */
/*                             TP_ERR_COM_IDX: frame index is out of range       */
/*                             TP_ERR_COM_FRAME_MODE: frame is in periodic mode  */
/*                             TP_ERR_COM_TX_MSG_LOST: transmission is pending   */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astTpMsgCfg: TP messages configuration          */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_SetFrameData                                  */
/*                             COM_SetDynamicFrameLen                            */
/*                             COM_SendFrame                                     */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tTpStatus TP_SendConsecutiveFrame (tTpMsgIdx uMsgIdx)
{
    /* Temporary data buffer for TP frame constitution */
    u8 aubData[COM_DATA_MAX];

    /* Index to use the data buffer */
    u8 ubIdx;


    {
        /* Source code optimisation : pointer on TP message */
        tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];
        
        /* Source code optimisation : pointer on TP message configuration */
        tTpMsgCfg const *const pstTpMsgCfg = &m_astTpMsgCfg[uMsgIdx];

        /* The status is : Consecutive Frame transmission */
        pstTpMsg->ebTxStatus = TP_MSG_STATUS_TX_CF;

        /* Timeout and delay transmission counter initialisation
           (there is no frame to wait before transmission confirmation) */
        pstTpMsg->uTxCnt = 0;


        /* Write the N_PCI and SN fields of the Consecutive Frame */
        aubData[0] = (u8)(((pstTpMsg->ubTxSnCnt) & 0x0FU) | TP_NPCI_CONSECUTIVE_FRAME);


        /* Copy the TP message data to the temporary buffer for COM frame transmission
           (it is necessary to check the Consecutive Frame length) */
        for (ubIdx = 1; (pstTpMsg->uwTxLenCnt < pstTpMsg->uwTxLen) && (ubIdx < TP_LEN_MAX_CONSECUTIVE_FRAME); ubIdx ++)
        {
            aubData[ubIdx] = pstTpMsg->paubTxData[m_uTpBufferCnt[uMsgIdx]];

            m_uTpBufferCnt[uMsgIdx] ++;

            /*Reload data buffer if it is empty */
            if(m_uTpBufferCnt[uMsgIdx] ==  m_astTpMsgCfg[uMsgIdx].uwDataMax)
            {
                TP_DataBufferEmptyInd (uMsgIdx);
                /*Restart form the begining of the buffer point by pstTpMsg */
                m_uTpBufferCnt[uMsgIdx] = 0;
            }
            
            /* Increment uwTxLenCnt */
            /* Shall not be done in the for(), MISRA restriction */
            pstTpMsg->uwTxLenCnt ++;
        }

        /* SN parameter for Consecutive Frames transmission */
        pstTpMsg->ubTxSnCnt ++;
        
        /* FC Wait counter initialization */
        pstTpMsg->ubRxFCWaitCnt = pstTpMsgCfg->ubRxFCWaitMax;
    }


    {
        tComStatus eComStatus;

        /* Source code optimisation : pointer on TP message */
        tComFrameIdx const *const puTxFrameIdx = (const tComFrameIdx *)(&m_astTpMsgCfg[uMsgIdx].uTxFrameIdx);


        /* Write the COM frame length (because it is dynamic) */
        /* Frame length is only written when padding is not used */
        if ((m_astTpMsgCfg[uMsgIdx].ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_FALSE)
        {
            #ifdef TP_COM_TX_DYNAMIC_FRAME_MAX
              #if (TP_COM_TX_DYNAMIC_FRAME_MAX > 0U)
                eComStatus = COM_SetDynamicFrameLen (*puTxFrameIdx, ubIdx);
              #else
                /* Initialization of eComStatus to avoid warning */
                eComStatus = COM_ERR_OK;
              #endif
            #else
              #error TP_COM_TX_DYNAMIC_FRAME_MAX must be defined
            #endif
        }
        else
        {
            /* to continue the program */
            eComStatus = COM_ERR_OK;
        }
        

        if(eComStatus == COM_ERR_OK)
        {
            /* Fill not used byte with padding value */
            /* ubIdx already points on the last databyte */
            for (; ubIdx < COM_DATA_MAX; ubIdx++)
            {
                aubData[ubIdx] = TP_PADDING_BYTE_VALUE;
            }
            /* Update the COM frame data buffer */
            eComStatus = COM_SetFrameData (*puTxFrameIdx, aubData);

            if(eComStatus == COM_ERR_OK)
            {
                /* Request COM frame transmission */
                eComStatus = COM_SendFrame (*puTxFrameIdx);
            }
        }

        return (eComStatus);
    }
}

/* - Fin TP_SendConsecutiveFrame                                                 */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_SendFlowControl                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Send a Flow Control                                              */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             TP_ERR_OK: no error                               */
/*                             TP_ERR_COM_OFF: transmission is not allowed       */
/*                             TP_ERR_COM_IDX: frame index is out of range       */
/*                             TP_ERR_COM_FRAME_MODE: frame is in periodic mode  */
/*                             TP_ERR_COM_TX_MSG_LOST: transmission is pending   */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astTpMsgCfg: TP messages configuration          */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_SetFrameData                                  */
/*                             COM_SetDynamicFrameLen                            */
/*                             COM_SendFrame                                     */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT tTpStatus TP_SendFlowControl (tTpMsgIdx uMsgIdx, u8 ubFlowStatus)
{
    /* Temporary data buffer for TP frame constitution */
    u8 aubData[COM_DATA_MAX];
    
    /* Index to use the data buffer */
    u8 ubIdx;

    {
        /* Source code optimisation : pointer on TP message */
        tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];

        /* The status is : Flow Control transmission */
        if (ubFlowStatus == TP_FS_OVERFLOW)
        {
            /* For FS = OVERFLOW, wait for TxOK and end reception */
            pstTpMsg->ebRxStatus = TP_MSG_STATUS_TX_FC_OVERFLOW;
        }
        else
        {
            /* For other FS, wait for TxOK and wait for Consecutive Frame */
            pstTpMsg->ebRxStatus = TP_MSG_STATUS_TX_FC;
        }

        /* Write the N_PCI, BS and STmin fields of the Flow Control */
        aubData[0] = (u8)(TP_NPCI_FLOW_CONTROL | ubFlowStatus);
        aubData[1] = pstTpMsg->ubRxBs;
        aubData[2] = pstTpMsg->ubRxSTmin;
        /* Fill not used byte with padding value */
        for (ubIdx = TP_LEN_FLOW_CONTROL; ubIdx < COM_DATA_MAX; ubIdx++)
        {
            aubData[ubIdx] = TP_PADDING_BYTE_VALUE;
        }

        /* BS parameter for Consecutive Frames reception */
        pstTpMsg->ubRxBsCnt = pstTpMsg->ubRxBs;

        /* Timeout and delay reception counter initialisation
           (there is no frame to wait before transmission confirmation) */
        pstTpMsg->uRxCnt = 0;
    }


    {
        tComStatus eComStatus;

        /* Source code optimisation : pointer on TP message transmission frame index */
        tComFrameIdx const *const puTxFrameIdx = (const tComFrameIdx *)(&m_astTpMsgCfg[uMsgIdx].uTxFrameIdx);


        /* Write the COM frame length (because it is dynamic) */
        /* Frame length is only written when padding is not used */
        if ((m_astTpMsgCfg[uMsgIdx].ubConfig & TP_MSG_PADDING) == TP_MSG_PADDING_FALSE)
        {
            #ifdef TP_COM_TX_DYNAMIC_FRAME_MAX
              #if (TP_COM_TX_DYNAMIC_FRAME_MAX > 0U)
                eComStatus = COM_SetDynamicFrameLen (*puTxFrameIdx, TP_LEN_FLOW_CONTROL);
              #else
                /* Initialization of eComStatus to avoid warning */
                eComStatus = COM_ERR_OK;
              #endif
            #else
              #error TP_COM_TX_DYNAMIC_FRAME_MAX must be defined
            #endif      
        }
        else
        {
            /* to continue the program */
            eComStatus = COM_ERR_OK;
        }        

        if(eComStatus == COM_ERR_OK)
        {
            /* Update the COM frame data buffer */
            eComStatus = COM_SetFrameData (*puTxFrameIdx, aubData);

            if(eComStatus == COM_ERR_OK)
            {
                /* Request COM frame transmission */
                eComStatus = COM_SendFrame (*puTxFrameIdx);
            }
        }

     return (eComStatus);
    }

}

/* - Fin TP_SendFlowControl                                                      */
/*-------------------------------------------------------------------------------*/




/*-------------------------------------------------------------------------------*/
/* - Public Callbacks                                                            */


/*********************************************************************************/
/* Name: TP_Manage                                                               */
/*-------------------------------------------------------------------------------*/
/* Description: Frame transmission timeout                                       */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : TP_SendConsecutiveFrame                           */
/* Called macros             : None                                              */
/* Called callbacks          : TP_RxInd                                          */
/*                             TP_TxConf                                         */
/*                             TP_RxEnableInd                                    */
/*********************************************************************************/

void TP_Manage (void)
{
    /* Index to use address the messages */
    tTpMsgIdx uMsgIdx;


    /* Manage all the TP messages timeout and delays */
    for (uMsgIdx = 0; uMsgIdx < TP_MSG_MAX; uMsgIdx ++)
    {
        /* Source code optimisation : pointer on TP message */
        tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];

        /* Decrease RxEnable counter to unlock reception channel, if required */
        if (m_aubRxEnableCnt[uMsgIdx] > 0)
        {
            /* Callback must be managed before setting counter to 0 */
            /* The aim is to avoid preemption problem. */
            if (m_aubRxEnableCnt[uMsgIdx] == 1U)
            {
                /* Callback for the application */
                TP_RxEnableInd (uMsgIdx, TP_RX_ENABLE_ON);
            }
            m_aubRxEnableCnt[uMsgIdx]--;
        }

        /* Wait for the timeout or delay duration
           for message transmission */
        if (pstTpMsg->uTxCnt > 1)
        {
            pstTpMsg->uTxCnt --;
        }

        /* Timeout or delay duration for message transmission elapsed */
        else if (pstTpMsg->uTxCnt == 1)
        {
            /* Timeout and delay transmission counter initialisation */
            /* (there is no frame to wait) */
            pstTpMsg->uTxCnt = 0;


            /* The timeout for Flow Control reception is elapsed
               (after a First Frame or Consecutive Frame transmission)
               => error, then message transmission stop */
            if (pstTpMsg->ebTxStatus == TP_MSG_STATUS_RX_FC)
            {
                pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;

                TP_TxConf (uMsgIdx, TP_MSG_STATUS_NBS);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                TP_TxRetransmission (uMsgIdx, TP_MSG_STATUS_NBS);
#endif
            }
            /* The minimal delay between two Consecutive Frames (STmin) is elapsed */
            /* => next Consecutive Frame transmission */
            else if (pstTpMsg->ebTxStatus == TP_MSG_STATUS_TX_CF_STMIN)
            {
                tTpStatus eStatus;
                eStatus = TP_SendConsecutiveFrame (uMsgIdx);
                if (TP_MSG_STATUS_OK != eStatus)
                {
                    (void)TP_AbortMsg(uMsgIdx);
                    TP_TxConf (uMsgIdx, TP_MSG_STATUS_COM_NOK);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                    TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_COM_NOK);
#endif
                }
            }
            else
            {
                /* Nothing to do */
            }
        }

        /* Timeout or delay duration for message transmission disabled */
        else
        {
            /* Nothing to do */
        }


        /* Wait for the timeout or delay duration */
        /* for message reception */
        if (pstTpMsg->uRxCnt > 1U)
        {
            pstTpMsg->uRxCnt --;
        }

        /* Timeout or delay duration for message reception elapsed */
        else if (pstTpMsg->uRxCnt == 1U)
        {
            /* The timeout for Consecutive Frame reception is elapsed
               (after a Flow Control transmission)
               => error, then message reception stop */
            if (pstTpMsg->ebRxStatus == TP_MSG_STATUS_RX_CF)
            {
                pstTpMsg->ebRxStatus = TP_MSG_STATUS_NONE;

                TP_RxInd (uMsgIdx, TP_MSG_STATUS_NCR);
            }
            else
            {
                /* Nothing to do */
            }


            /* Timeout and delay reception counter initialisation */
            /* (there is no frame to wait) */
            pstTpMsg->uRxCnt = 0;
        }

        /* Timeout or delay duration for message reception disabled */
        else
        {
            /* Nothing to do */
        }
    }
}

/* - Fin TP_Manage                                                               */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_ComTxConf                                                            */
/*-------------------------------------------------------------------------------*/
/* Description: Frame transmission confirmation                                  */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : TP_TxConf                                         */
/*                             TP_TxConf                                         */
/*********************************************************************************/

void TP_ComTxConf (tTpMsgIdx uMsgIdx)
{
    /* Source code optimisation : pointer on TP message */
    tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];

    /* Source code optimisation : pointer on TP message configuration */
    tTpMsgCfg const *const pstTpMsgCfg = &m_astTpMsgCfg[uMsgIdx];



    /* After a Single Frame correct transmission,
       the message transmission is finished => transmission indication */
    if (pstTpMsg->ebTxStatus == TP_MSG_STATUS_TX_SF)
    {
        pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;

        TP_TxConf (uMsgIdx, TP_MSG_STATUS_OK);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
        TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_OK);
#endif
    }

    /* After a First Frame correct transmission,
       the timeout for Flow Control reception is started */
    else if (pstTpMsg->ebTxStatus == TP_MSG_STATUS_TX_FF)
    {
        pstTpMsg->ebTxStatus = TP_MSG_STATUS_RX_FC;
        pstTpMsg->uTxCnt = pstTpMsgCfg->uNBs;
    }

    /* After a Consecutive Frame correct transmission */
    else if (pstTpMsg->ebTxStatus == TP_MSG_STATUS_TX_CF)
    {
        /* All the message datas have been correctly transmitted
           => transmission indication */
        if (pstTpMsg->uwTxLenCnt == pstTpMsg->uwTxLen)
        {
            pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;

            TP_TxConf (uMsgIdx, TP_MSG_STATUS_OK);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
            TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_OK);
#endif
        }

        /* All the message datas have been correctly transmitted
           => message transmission continues */
        else
        {
            /* The bloc is not complete, then Consecutive Frame transmission
               after waiting the received STmin delay */

            (pstTpMsg->ubTxBsCnt)--;

            if ((pstTpMsg->ubTxBs == 0) || (pstTpMsg->ubTxBsCnt > 0))
            {
                /* The STmin is equal to 0, then the Consecutive Frame
                   is transmitted immediatly */
                if (pstTpMsg->ubTxSTmin == 0)
                {
                    tTpStatus eStatus;
                    eStatus = TP_SendConsecutiveFrame (uMsgIdx);
                    if (TP_MSG_STATUS_OK != eStatus)
                    {
                        (void)TP_AbortMsg(uMsgIdx);
                        TP_TxConf (uMsgIdx, TP_MSG_STATUS_COM_NOK);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
                        TP_TxRetransmission(uMsgIdx, TP_MSG_STATUS_COM_NOK);
#endif
                    }
                }
                /* The STmin is different of 0, then wait for STmin delay */
                else
                {
                    pstTpMsg->ebTxStatus = TP_MSG_STATUS_TX_CF_STMIN;
                    
                    /* If inferior to 127ms, use the value */
                    if (pstTpMsg->ubTxSTmin <= 0x7FU)
                    {
                        pstTpMsg->uTxCnt = (tTpTimeout)((pstTpMsg->ubTxSTmin / TP_MANAGE_PERIOD) + 1U);
                    }
                    /* Range in steps of 100us */
                    else if ((pstTpMsg->ubTxSTmin >= 0xF1U) && (pstTpMsg->ubTxSTmin <= 0xF9U))
                    {
                        pstTpMsg->uTxCnt = (tTpTimeout)((1U / TP_MANAGE_PERIOD) + 1U);
                    }
                    /* Invalid range, use maximum value */
                    else
                    {
                        pstTpMsg->uTxCnt = (tTpTimeout)((0x7FU / TP_MANAGE_PERIOD) + 1U);
                    }
                }
            }
            /* The bloc is complete, then wait for Flow Control reception
               during N_Bs */
            else
            {
                pstTpMsg->ebTxStatus = TP_MSG_STATUS_RX_FC;
                pstTpMsg->uTxCnt = pstTpMsgCfg->uNBs;
                pstTpMsg->ubRxFCWaitCnt = pstTpMsgCfg->ubRxFCWaitMax;
            }
        }
    }
    else
    {
        /* Nothing to do */
    }



    /* After a Flow Control correct transmission, */
    if (pstTpMsg->ebRxStatus == TP_MSG_STATUS_TX_FC)
    {
        /* the timeout for Consecutive Frame reception is started,
           for a Flow Control with FS = CLEAR_TO_SEND */
        pstTpMsg->ebRxStatus = TP_MSG_STATUS_RX_CF;
        pstTpMsg->uRxCnt = pstTpMsgCfg->uNCr;
    }
    else if (pstTpMsg->ebRxStatus == TP_MSG_STATUS_TX_FC_OVERFLOW)
    {
        /* the reception is ended, for a Flow Control with FS = OVERFLOW */
        pstTpMsg->ebRxStatus = TP_MSG_STATUS_NONE;
    }
    else
    {
        /* Nothing to do */
    }
}

/* - Fin TP_ComTxConf                                                            */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: TP_ComNAsInd                                                            */
/*-------------------------------------------------------------------------------*/
/* Description: Frame transmission timeout                                       */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : TP_RxInd                                          */
/*                             TP_TxConf                                         */
/*********************************************************************************/

void TP_ComNAsInd (tTpMsgIdx uMsgIdx)
{
    /* The timeout for frame transmission is elapsed
       (after a Flow Control transmission)
       => error, then message reception stop */
    if ((m_astTpMsg[uMsgIdx].ebRxStatus == TP_MSG_STATUS_TX_FC) ||
        (m_astTpMsg[uMsgIdx].ebRxStatus == TP_MSG_STATUS_TX_FC_OVERFLOW))
    {
        m_astTpMsg[uMsgIdx].ebRxStatus = TP_MSG_STATUS_NONE;

        TP_RxInd (uMsgIdx, TP_MSG_STATUS_NAR);
    }
    /* The timeout for frame transmission is elapsed
       (after a Single Frame, First Frame or Consecutive Frame transmission)
       => error, then message transmission stop */
    else
    {
        m_astTpMsg[uMsgIdx].ebTxStatus = TP_MSG_STATUS_NONE;

        TP_TxConf (uMsgIdx, TP_MSG_STATUS_NAS);
#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
        TP_TxRetransmission (uMsgIdx, TP_MSG_STATUS_NAS);
#endif
    }
}

/* - Fin TP_ComNAsInd                                                            */
/*-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------*/

/*                                                                               */
/*********************************************************************************/

/*********************************************************************************/
/* Name: TP_AbortMsg                                                             */
/*-------------------------------------------------------------------------------*/
/* Description: Stop TP message transmission                                     */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

tTpStatus TP_AbortMsg(tTpMsgIdx uMsgIdx)
{
    /* Return status */
    tTpStatus eStatus = TP_ERR_OK;


    /* The message index input parameter is in expected range */
    if (uMsgIdx < TP_MSG_MAX)
    {
        /* Source code optimisation : pointer on TP message */
        tTpMsg *const pstTpMsg = &m_astTpMsg[uMsgIdx];

        pstTpMsg->ebTxStatus = TP_MSG_STATUS_NONE;
        pstTpMsg->ebRxStatus = TP_MSG_STATUS_NONE;
        pstTpMsg->uTxCnt = 0;
        pstTpMsg->uRxCnt = 0;

        m_uTpBufferCnt[uMsgIdx] = 0;
        #if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
        #ifndef BOOTLOADER
        m_aubTxCnt[uMsgIdx] = 0;
        #endif
        #endif
    }
    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_AbortMsg                                                             */
/*-------------------------------------------------------------------------------*/

#if (TP_TXRETRANSMISSION == TP_TXRETRANSMISSION_ACTIVATE)
#ifndef BOOTLOADER
/*********************************************************************************/
/* Name: TP_MonNodeStatus                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Set ECU status                                                   */
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

tTpStatus TP_MonNodeStatus (tTpMsgIdx uMsgIdx, tNmMonStatus eNodeStatus)
{
    /* Return status */
    tTpStatus eStatus = TP_ERR_OK;


    /* The message index input parameter is in expected range */
    if (uMsgIdx < TP_MSG_MAX)
    {
        /*Copie TP message status */
        m_auNodeStatus[uMsgIdx] = eNodeStatus;
    }
    /* The message index input parameter is out of range, then error */
    else
    {
        eStatus = TP_ERR_IDX;
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin TP_MonNodeStatus                                                        */
/*-------------------------------------------------------------------------------*/
#endif
/*                                                                               */
/*********************************************************************************/

/*********************************************************************************/
/* Name: TP_TxRetransmission                                                     */
/*-------------------------------------------------------------------------------*/
/* Description: Frame re transmission                                            */
/*********************************************************************************/
/* Input parameters          : uMsgIdx: TP message index                         */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astTpMsg: TP messages                           */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : TP_RxInd                                          */
/*                             TP_TxConf                                         */
/*********************************************************************************/

NCS_PRI_FCT void TP_TxRetransmission (tTpMsgIdx uMsgIdx, u8 ebStatus)
{
    /*retransmission only if the 4 following conditions are compliant :
       1-TP message is configured to be retransmited in case of faillure
       2-Receiver node is present
       3-Less than 2 retransmission has been realized
       4-Error detect is N_As or N_Bs TimeOut */
    if ((((u8)(m_astTpMsgCfg[uMsgIdx].ubConfig & TP_MSG_RETRANSMISSION) == TP_MSG_RETRANSMISSION_TRUE) &&
        #ifndef BOOTLOADER
         (m_auNodeStatus[uMsgIdx] == NM_MON_OK)) &&
        ((m_aubTxCnt[uMsgIdx] < TP_MSG_RETRANSMISSION_MAX) &&
        #endif
         ((ebStatus == TP_MSG_STATUS_NAS) || (ebStatus == TP_MSG_STATUS_NBS))))
    {
        #ifndef BOOTLOADER
        m_aubTxCnt[uMsgIdx] ++ ;
        #endif
        
        /*Retransmit the segmented message */
        (void) TP_SendMsg (uMsgIdx);
    }
    else
    {
        #ifndef BOOTLOADER
        m_aubTxCnt[uMsgIdx] = 0;
        #endif
    }
}

/* - Fin TP_TxRetransmission                                                     */
/*-------------------------------------------------------------------------------*/
#endif
