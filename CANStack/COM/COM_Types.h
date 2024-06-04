/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : COM_Types.h
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
#ifndef COM_TYPES_H
#define COM_TYPES_H

/*Number of configuration available for COM module */
#define COM_CONFIG_IDX_0               0U
#define COM_CONFIG_NB_MAX               1U
/* Allows to select the configuration at the ECU start-up*/
extern u8 ubConfigurationSelection;

/* used in order to set the CAN type */
#define COM_EB_PSA_CAN_LS_2010       0U
#define COM_EB_PSA_CAN_HS_IS_2010    1U
#define COM_EB_PSA_CAN_HS_M_2010     2U
#define COM_EB_RSA_CAN_HS            3U
#define COM_EB_JAC_CAN_HS            4U
#define COM_EB_FIAT_CAN              5U
#define COM_EB_VW_CAN_HS             6U

#define COM_EB_CAN_TYPE          COM_EB_RSA_CAN_HS

/* Timer period in milliseconds for the COM periodic task */
/* Range : (1 .. 10) */
#define COM_MANAGE_PERIOD               5U

/* Monitored nodes number in transmission and reception */
/* Range : (0 .. 255) */
#define COM_FILTERING_METHOD  COM_LINEAR_FILTERING

/* Bus frame data length */
#define COM_DATA_MAX                    CAN_DATA_MAX

/* CAN controller choice */        
#define COM_CAN_BUS_IDX        0U
#define COM_CAN_MAILBOX_NB     CAN_MAILBOX_NB
#define COM_CAN_TX_MAILBOX_NB  CAN_TX_MAILBOX_NB
#define COM_CAN_RX_MAILBOX_NB  CAN_RX_MAILBOX_NB

/* Communication frames in reception number managed by this software layer */
/* Range : (0 .. 2^32) */
#define COM_RX_FRAME_MAX                  2U
#define COM_RX_FRAME_MAX_RAM              COM_RX_FRAME_MAX

/* Communication frames in transmission number managed by this software layer */
/* Range : (0 .. 2^32) */
#define COM_TX_FRAME_MAX                  1U

/* Communication frames in extension number managed by this software layer */
/* Range : (0 .. 2^32) */
#define COM_EXT_FRAME_MAX              0U

/* Communication frames number managed by this software layer */
/* Range : (0 .. 2^32) */
#define COM_FRAME_MAX                  3U
#define COM_FRAME_MAX_RAM              COM_FRAME_MAX

/* Protocol handler Mailbox number used for transmission */
/* Range : (0 .. CAN_MAILBOX_NB) */
/* Number of transmission mailboxes is imposed by protocol handler hardware */
#if (COM_CAN_TX_MAILBOX_NB != 0U)
    #define COM_TX_MAILBOX_NB_RAM           COM_CAN_TX_MAILBOX_NB
    #define COM_TX_MAILBOX_NB           COM_CAN_TX_MAILBOX_NB
/* If no hardware restriction, user configuration */
#else

    #define COM_TX_MAILBOX_NB           (COM_CAN_MAILBOX_NB - COM_RX_FRAME_MAX)
    #define COM_TX_MAILBOX_NB_RAM       COM_TX_MAILBOX_NB
#endif

/* Transmission mailbox software FIFO size */
/* Range : (COM_FRAME_MAX .. 2^32) */

#if ( COM_TX_FRAME_MAX < COM_TX_MAILBOX_NB)
#define COM_TX_FIFO_SIZE                1U
#else
#define COM_TX_FIFO_SIZE                ( COM_TX_FRAME_MAX + 1U - COM_TX_MAILBOX_NB )
#endif

/* Maximum value of COM transmission frame exclusion times */
/* Range : (0 .. 2^32) */
#define COM_TX_EXCLUSION_TIME_MAX                ( 0U / COM_MANAGE_PERIOD )

/* Maximum value of COM transmission frame monitoring timeouts */
/* Range : (0 .. 2^32) */
#define COM_TX_MON_TIMEOUT_MAX                ( 0U / COM_MANAGE_PERIOD )

/* Maximum value of COM transmission frame periods */
/* or reception monitoring periods */
/* Range : (0 .. 2^32) */
#define COM_PERIOD_MAX                  ( 0U / COM_MANAGE_PERIOD )


/* Monitored nodes number in transmission */
/* Range : (0 .. 255) */
#define COM_MON_NODE_TX                0U

/* Monitored nodes number in reception */
/* Range : (0 .. 255) */
#define COM_MON_NODE_RX                1U

/* Monitored nodes number in extension */
/* Range : (0 .. 255) */
#define COM_MON_NODE_EXT    0U

/* Monitored nodes number in transmission and reception */
/* Range : (0 .. 255) */
#define COM_MON_NODE_MAX                (COM_MON_NODE_TX + COM_MON_NODE_RX + COM_MON_NODE_EXT)

/* Protocol handler Mailbox number used for reception */
/* Range : (0 .. CAN_MAILBOX_NB) */
/* Number of reception mailboxes is imposed by protocol handler hardware */
#if (COM_CAN_RX_MAILBOX_NB != 0U)

    #define COM_RX_MAILBOX_NB_TMP                COM_CAN_RX_MAILBOX_NB
/* If no hardware restriction, user configuration */
#else

    #define COM_RX_MAILBOX_NB_TMP                (COM_CAN_MAILBOX_NB - COM_TX_MAILBOX_NB)
#endif

/* Protocol handler Mailbox base used for transmission */
/* It means that the CAN mailboxes from COM_TX_MAILBOX_BASE_IDX */
/* to COM_TX_MAILBOX_BASE_IDX + COM_TX_MAILBOX_NB - 1 are used for transmission */
/* Range : (0 .. CAN_MAILBOX_NB - 1) */

#define COM_TX_MAILBOX_BASE_IDX                (COM_CAN_MAILBOX_NB - COM_TX_MAILBOX_NB)

/* Protocol handler Mailbox base used for reception */
/* It means that the CAN mailboxes from COM_RX_MAILBOX_BASE_IDX */
/* to COM_RX_MAILBOX_BASE_IDX + COM_RX_MAILBOX_NB - 1 are used for reception */
/* Range : (0 .. CAN_MAILBOX_NB - 1) */
#define COM_RX_MAILBOX_BASE_IDX         0U

/* Protocol handler Mailbox used for reception extension */
/* Range : (0 .. CAN_MAILBOX_NB - 1) / 255 : disabled */
/* COM uses more frames in reception than the protocol handler reception mailboxes, */
/* then it needs a reception mailbox in extension mode */
#define COM_RX_MAILBOX_EXT_NONE         255U


#if (COM_RX_FRAME_MAX > COM_RX_MAILBOX_NB_TMP)

    #define COM_RX_MAILBOX_NB           (COM_RX_MAILBOX_NB_TMP - 1U)
    #define COM_RX_MAILBOX_EXT_IDX      (COM_RX_MAILBOX_BASE_IDX + COM_RX_MAILBOX_NB)

/* COM does not need to use a reception mailbox in extension mode */
#elif (COM_RX_FRAME_MAX == COM_RX_MAILBOX_NB_TMP)
    #define COM_RX_MAILBOX_NB           COM_RX_MAILBOX_NB_TMP
    #define COM_RX_MAILBOX_EXT_IDX      COM_RX_MAILBOX_EXT_NONE

/* COM does not need to use a reception mailbox in extension mode */
#else /* #if (COM_RX_FRAME_MAX > COM_RX_MAILBOX_NB_TMP) */

    #define COM_RX_MAILBOX_NB           COM_RX_MAILBOX_NB_TMP
    #define COM_RX_MAILBOX_EXT_IDX      COM_RX_MAILBOX_EXT_NONE
#endif  /* #if (COM_RX_FRAME_MAX > COM_RX_MAILBOX_NB_TMP) */

/* COM reception filtering mode */
#define COM_DIRECT_FILTERING    0U
#define COM_LINEAR_FILTERING    1U

/* Activation/Deactivation of frame consistency check */
#define FRAME_CONSISTENCY_CHECK_OFF             0U
#define FRAME_CONSISTENCY_CHECK_ON              1U

/* Management of the extended ID */

#define COM_EXTENDED_ID    COM_FALSE
/* COM software layer status */
#define COM_MODE_OFF                    SDF_U8_VALUE1
#define COM_MODE_BUS_ON                 SDF_U8_VALUE2
#define COM_MODE_BUS_OFF                SDF_U8_VALUE3
typedef u8 tComMode;

/* COM periodic mode */
#define COM_PERIODIC_STOP               0U
#define COM_PERIODIC_START              1U
typedef u8 tComPeriodicMode;

/* Frame transmission mode */
#define COM_DIR                         0x01U
#define COM_DIR_RX                      0x00U
#define COM_DIR_TX                      0x01U

#define COM_MODE                        0x06U
#define COM_MODE_DIRECT                 0x00U
#define COM_MODE_PERIODIC               0x02U
#define COM_MODE_MIXED                  0x04U

/* Frame type : static or dynamic length */
#define COM_LENGTH_TYPE                 0x08U
#define COM_LENGTH_STATIC               0x00U
#define COM_LENGTH_DYNAMIC              0x08U

/* COM functions status */
#define COM_ERR_BASE                    0x10U
#define COM_ERR_OK                      0x00U
#define COM_ERR_OFF                     ( COM_ERR_BASE + 0x01U )
#define COM_ERR_MODE                    ( COM_ERR_BASE + 0x02U )
#define COM_ERR_IDX                     ( COM_ERR_BASE + 0x03U )
#define COM_ERR_FRAME_MODE              ( COM_ERR_BASE + 0x04U )
#define COM_ERR_NODE                    ( COM_ERR_BASE + 0x05U )
#define COM_ERR_TX_MSG_LOST             ( COM_ERR_BASE + 0x06U )
#define COM_ERR_TX_FIFO_FULL            ( COM_ERR_BASE + 0x07U )
#define COM_ERR_RX_MSG_LOST             ( COM_ERR_BASE + 0x08U )
#define COM_ERR_NULL                    ( COM_ERR_BASE + 0x09U )
#define COM_ERR_BUF                     ( COM_ERR_BASE + 0x0AU )
#define COM_ERR_COHE                    ( COM_ERR_BASE + 0x0BU )
#define COM_RX_IND_CHECK_OK             ( COM_ERR_BASE + 0x0CU )
#define COM_RX_IND_CHECK_REJECT         ( COM_ERR_BASE + 0x0DU )
#define COM_ERR_ABORT                   ( COM_ERR_BASE + 0x0EU )
typedef u8 tComStatus;

/* COM frame index definition */
/* The type size depends on the number of managed frames */
#if (COM_FRAME_MAX_RAM <= U8_MAX)
    typedef u8 tComFrameIdx;
#elif (COM_FRAME_MAX_RAM <= U16_MAX)
    typedef u16 tComFrameIdx;
#elif (COM_FRAME_MAX_RAM <= U32_MAX)
    typedef u32 tComFrameIdx;
#endif

/* COM period definition */
/* The type size depends on the maximum period managed by COM */
#if (COM_PERIOD_MAX <= U8_MAX)
    typedef u8 tComPeriod;
#elif (COM_PERIOD_MAX <= U16_MAX)
    typedef u16 tComPeriod;
#elif (COM_PERIOD_MAX <= U32_MAX)
    typedef u32 tComPeriod;
#endif

/* COM transmission monitoring timeout definition */
/* The type size depends on the maximum monitoring timeout managed by COM */
#if (COM_TX_MON_TIMEOUT_MAX <= U8_MAX)
    typedef u8 tComTxTimeout;
#elif (COM_TX_MON_TIMEOUT_MAX <= U16_MAX)
    typedef u16 tComTxTimeout;
#elif (COM_TX_MON_TIMEOUT_MAX <= U32_MAX)
    typedef u32 tComTxTimeout;
#endif

/* COM transmission exclusion time definition */
/* The type size depends on the maximum exclusion time managed by COM */
#if (COM_TX_EXCLUSION_TIME_MAX <= U8_MAX)
    typedef u8 tComExTime;
#elif (COM_TX_EXCLUSION_TIME_MAX <= U16_MAX)
    typedef u16 tComExTime;
#elif (COM_TX_EXCLUSION_TIME_MAX <= U32_MAX)
    typedef u32 tComExTime;
#endif

/* COM boolean type */
#define COM_FALSE               0U
#define COM_TRUE                1U
typedef u8 tComBoolean;

/* Monitoring status */
#define COM_MON_OK              0U
#define COM_MON_NOK             1U
typedef u8 tComMonStatus;

/* COM node index definition */
/* The type size depends on the number of monitored nodes */
#if (COM_MON_NODE_MAX <= U8_MAX)
    typedef u8 tComMonNodeIdx;
#elif (COM_MON_NODE_MAX <= U16_MAX)
    typedef u16 tComMonNodeIdx;
#elif (COM_MON_NODE_MAX <= U32_MAX)
    typedef u32 tComMonNodeIdx;
#endif

/* Reception frame callback */
typedef u8 (* tComRxFrameInd) (tComFrameIdx uFrameIdx);

/* Reception data callback */
typedef void (* tComRxDataInd) (tComFrameIdx uFrameIdx);


#define COM_TX_DYNAMIC_FRAME_MAX            0U
#define COM_TX_DYNAMIC_FRAME_MAX_RAM        COM_TX_DYNAMIC_FRAME_MAX

/* Data Link frame configuration, stored in Read-Only memory */
/*************************************************
*                     Config 0                   *
*************************************************/ 
#if (COM_EXTENDED_ID == COM_TRUE)
extern const u32 m_auwComFrameIdent[COM_FRAME_MAX];
#else
extern const u16 m_auwComFrameIdent[COM_FRAME_MAX];
#endif
extern const u8 m_aubComFrameLenNominal[COM_FRAME_MAX];
extern const u8 m_aubComFrameLenMin[COM_RX_FRAME_MAX];
extern const u8 m_aubComFrameConfig[COM_FRAME_MAX];
extern const u8 m_aaubComFrameInitData[COM_FRAME_MAX][COM_DATA_MAX];
extern const u8 m_aaubComFrameComLossData[COM_RX_FRAME_MAX][COM_DATA_MAX];
extern const u8 m_aaubComFrameComLossMask[COM_RX_FRAME_MAX][COM_DATA_MAX];
extern const tComPeriod m_auComFramePeriod[COM_FRAME_MAX];
extern const tComPeriod m_auComFrameFirstPeriod[COM_FRAME_MAX];
extern const tComTxTimeout m_auComFrameTxTimeout[COM_FRAME_MAX];
extern const tComExTime m_auComFrameExTime[COM_FRAME_MAX];
extern const tComRxFrameInd m_apfuComFrameRxInd[COM_RX_FRAME_MAX];
extern const tComRxDataInd m_apfuComFrameRxDataInd[COM_RX_FRAME_MAX];
extern const tComRxDataInd m_apfuComFrameRxInvalidDLCInd[COM_RX_FRAME_MAX];
    
/*Frame Idx position in function of the identifier */
extern const tComFrameIdx m_auwComFrameIdx[];

#endif      /* COM_TYPES_H */
