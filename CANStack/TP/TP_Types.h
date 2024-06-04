/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       :  TP_Types.h
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


#ifndef TP_TYPES_H
#define TP_TYPES_H

/* Timer period in milliseconds for the TP periodic task */
/* Range : (1 .. 10) */
#define TP_MANAGE_PERIOD            10U

/* Timer period in milliseconds for forbidden reception of frames */
#define TP_RX_ENABLE_CPT_VALUE           (u16)(1000U / TP_MANAGE_PERIOD)

/*Constant for precompile condition  */
/*utilisation of functions TP_SetMsgData and TP_GetMsgData */
#define TP_DATA_COPY_ADR            0
/*utilisation of functions TP_SetMsgDataCopy and TP_GetMsgDataCopy */
#define TP_DATA_COPY_BUF            1
#define TP_DATA_COPY          TP_DATA_COPY_ADR

/* Maximum value of TP timeouts and delays */
/* Range : (0 .. 2^32) */
#define TP_TIMEOUT_MAX              (150U / TP_MANAGE_PERIOD )

/*max data in Tp message */
#define TP_DIAG_MSG_DATA_MAX           300U

/* TP timeout and delay definition */
/* The type size depends on the number of managed messages */
#if (TP_TIMEOUT_MAX <= U8_MAX)
    typedef u8 tTpTimeout;
#elif (TP_TIMEOUT_MAX <= U16_MAX)
    typedef u16 tTpTimeout;
#elif (TP_TIMEOUT_MAX <= U32_MAX)
    typedef u32 tTpTimeout;
#endif

/* TX Retransmission */
#define TP_TXRETRANSMISSION_ACTIVATE            1U
#define TP_TXRETRANSMISSION_DEACTIVATE          0U

#define TP_TXRETRANSMISSION         TP_TXRETRANSMISSION_DEACTIVATE

/* Frame reception authorization for a TP channel */
typedef u8 tTpRxEnable;

/* TP functions status */
typedef u8 tTpStatus;

/* TP message index definition */
typedef u8 tTpMsgIdx;



/* TP Buffer data type de finition */
/* The type size TP buffer size */
#if (TP_DIAG_MSG_DATA_MAX <= U8_MAX)
    typedef u8 tTpBufferData;
#elif (TP_DIAG_MSG_DATA_MAX <= U16_MAX)
    typedef u16 tTpBufferData;
#endif

/* Transport Protocol message configuration, stored in Read-Only memory */
typedef struct
{
    u8          ebTxStatus;
    u8          ebRxStatus;
    u8          *paubTxData;       /* Pointer on the transmission data buffer */
    u8          *paubRxData;       /* Pointer on the reception data buffer */
    u16         uwTxLen;
    u16         uwTxLenCnt;
    u16         uwRxLen;
    u16         uwRxLenCnt;
    tTpTimeout  uTxCnt;
    tTpTimeout  uRxCnt;
    u8          ubRxBs;
    u8          ubRxBsCnt;
    u8          ubRxSnCnt;
    u8          ubTxSnCnt;
    u8          ubTxBs;
    u8          ubTxBsCnt;
    u8          ubTxSTmin;
    u8          ubRxSTmin;
    u8          ubRxFCWaitCnt;       /* Number of FCWait frame received in a row */
} tTpMsg;

/* Transport Protocol message configuration, stored in Read-Only memory */
typedef struct
{
    tComFrameIdx uTxFrameIdx;       /* COM frame index for transmission */
    tComFrameIdx uRxFrameIdx;       /* COM frame index for reception */
    u8           *paubTxData;       /* Pointer on the transmission data buffer */
    u8           *paubRxData;       /* Pointer on the reception data buffer */
    u16          uwDataMax;         /* Maximum data number for a transfer */
    tTpTimeout   uNBs;              /* Timeout between (Tx FF or Tx CF) and Rx FC */
    tTpTimeout   uNCr;              /* Timeout between (Tx FC or Rx CF) and Rx CF */
                                    /* the value 0 desactivates the timeout */
    u8           ubRxSTmin;         /* STmin value of the transmitted FC */
    u8           ubRxBs;            /* BS value of the transmitted FC */
    u8           ubConfig;          /* Config */
    u8           ubRxFCWaitMax;      /* Maximum number of FCWait frame received in a row */
} tTpMsgCfg;

#endif      /* TP_TYPES_H */
