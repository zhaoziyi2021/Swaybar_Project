/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : UDS_Types.h
************************************************************************************************
*   Project/Product :
*   Title           :
*   Author          : BWFENG
************************************************************************************************
*   Description     : This is common module for body network
*
************************************************************************************************
*   Limitations     : NONE
*
************************************************************************************************
*   END_FILE_HDR*/

#ifndef UDS_TYPES_H
#define UDS_TYPES_H

/*-------------------------------------------------------------------------------*/
/* - Configuration Constants                                                     */

/* used in order to set the CAN type */
#define PSA         0U
#define RSA_MIXED   1U
#define RSA_UDS     2U

#define UDS_EB_CAN_TYPE            RSA_UDS

/* Timer period in milliseconds for the UDS periodic task
   Range : (1 .. 100) */
#define UDS_MANAGE_PERIOD            50U

/* Diagnostic services number of the configuration 1 managed by this software layer
   Range : (0 .. 2^32) */
#define UDS_SRV_CFG1_MAX            30U

/* Diagnostic services number of the configuration 2 managed by this software layer
   Range : (0 .. 2^32) */
#define UDS_SRV_CFG2_MAX            17U

/* Diagnostic services number of the configuration 3 managed by this software layer
   Range : (0 .. 2^32) */
#define UDS_SRV_CFG3_MAX            18U

/* Diagnostic services number of the configuration 4 managed by this software layer
   Range : (0 .. 2^32) */
#define UDS_SRV_CFG4_MAX            0U

/* Diagnostic services number of the configuration 5 managed by this software layer
   Range : (0 .. 2^32) */
#define UDS_SRV_CFG5_MAX            0U

/* Authorize the services callback configuration
   Range : (0 .. 1) */
#define UDS_SRV_CALLBACK             1U

/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                */

#define UDS_FALSE       (tUdsBoolean) 0x00U
#define UDS_TRUE        (tUdsBoolean) 0x01U
typedef u8 tUdsBoolean;

/* Diagnostic request reception callbacks status */
#define UDS_ACK            (tUdsStatus) 0x40U    /* Positive answer */
#define UDS_NRC_11         (tUdsStatus) 0x11U    /* Non-supported service */
#define UDS_NRC_12         (tUdsStatus) 0x12U    /* Non-supported sub-service */
#define UDS_NRC_22         (tUdsStatus) 0x22U    /* Unsatisfied conditions */
#define UDS_NRC_33         (tUdsStatus) 0x33U    /* ECU is locked */
#define UDS_NRC_35         (tUdsStatus) 0x35U    /* Invalid key */
#define UDS_NRC_72         (tUdsStatus) 0x72U    /* Erasing or writing error volatil memory */
#define UDS_NRC_78         (tUdsStatus) 0x78U    /* Request well received / Answer constitution in progress */
#define UDS_NRC_7E         (tUdsStatus) 0x7EU    /* Sub-service supported by the ECU but not in the active session */
#define UDS_NRC_7F         (tUdsStatus) 0x7FU    /* Service supported by the ECU but not in the active session */
#define UDS_ERR_COHE       (tUdsStatus) 0xFFU    /* SDF UDS error */
#if ( UDS_EB_CAN_TYPE == RSA_MIXED )
#define UDS_NRC_21         (tUdsStatus) 0x21U    /* Busy-Repeat Request */
#define UDS_NRC_23         (tUdsStatus) 0x23U    /* Routine Not Complete */
#define UDS_NRC_INVALID_DID (tUdsStatus) 0x12U   /* DID implemented in other session and session supported by the service */
                                                 /*   Code to be defined by RSA */
#else
#define UDS_NRC_13         (tUdsStatus) 0x13U    /* Invalid request length or invalid format */
#define UDS_NRC_14         (tUdsStatus) 0x14U    /* ECU is busy */
#define UDS_NRC_24         (tUdsStatus) 0x24U    /* Links not respected */
#define UDS_NRC_26         (tUdsStatus) 0x26    /* Failure prevents Execution Of Requested Action */
#define UDS_NRC_31         (tUdsStatus) 0x31U    /* Invalid parameters */
#define UDS_NRC_36         (tUdsStatus) 0x36U    /* Unlocked attempt overrun */
#define UDS_NRC_37         (tUdsStatus) 0x37U    /* delay between 2 unlocked attempt is elapsed */
#define UDS_NRC_70         (tUdsStatus) 0x70U    /* Unsatisfied conditions  for downloading */
#define UDS_NRC_71         (tUdsStatus) 0x71U    /* Error during "TransferData" Stop procedure */
#define UDS_NRC_73         (tUdsStatus) 0x73U    /* Error in erasing or writing sequence  */
#define UDS_NRC_92         (tUdsStatus) 0x92U    /* Voltage too high */
#define UDS_NRC_93         (tUdsStatus) 0x93U    /* Voltage too low */
#define UDS_NRC_INVALID_DID (tUdsStatus) 0x31U   /* DID implemented in other session and session supported by the service */
                                                 /*   Code to be defined by PSA */
#endif
typedef u8 tUdsStatus;

/* Diagnostic session type */
#define UDS_SESSION_PROT_DEFAULT                SDF_U16_VALUE1
#define UDS_SESSION_PROT_PROGRAMMING            SDF_U16_VALUE2
#define UDS_SESSION_PROT_EXTENDED_DIAG          SDF_U16_VALUE3
#define UDS_SESSION_PROT_SUPPLIER               SDF_U16_VALUE4
#define UDS_SESSION_PROT_OTHER_01               SDF_U16_VALUE5
#define UDS_SESSION_PROT_OTHER_02               SDF_U16_VALUE6
#define UDS_SESSION_PROT_OTHER_03               SDF_U16_VALUE7
#define UDS_SESSION_PROT_OTHER_04               SDF_U16_VALUE8
typedef u16 tUdsSessionProtType;
typedef u8 tUdsSessionType;

/* Diagnostic session masks, with SDF protection */
#define UDS_SESSION_PROT_DEFAULT_MASK                SDF_U16_VALUE1
#define UDS_SESSION_PROT_PROGRAMMING_MASK            SDF_U16_VALUE2
#define UDS_SESSION_PROT_EXTENDED_DIAG_MASK          SDF_U16_VALUE3
#define UDS_SESSION_PROT_SUPPLIER_MASK               SDF_U16_VALUE4
#define UDS_SESSION_PROT_OTHER_01_MASK               SDF_U16_VALUE5
#define UDS_SESSION_PROT_OTHER_02_MASK               SDF_U16_VALUE6
#define UDS_SESSION_PROT_OTHER_03_MASK               SDF_U16_VALUE7
#define UDS_SESSION_PROT_OTHER_04_MASK               SDF_U16_VALUE8
typedef u16 tUdsSessionProtMask;

/* Diagnostic session masks, without protection, for configuration parameter */
#define UDS_SESSION_DEFAULT_MASK        (tUdsSessionMask) 0x01U
#define UDS_SESSION_PROGRAMMING_MASK    (tUdsSessionMask) 0x02U
#define UDS_SESSION_EXTENDED_DIAG_MASK  (tUdsSessionMask) 0x04U
#define UDS_SESSION_SUPPLIER_MASK       (tUdsSessionMask) 0x08U
#define UDS_SESSION_OTHER_01_MASK       (tUdsSessionMask) 0x10U
#define UDS_SESSION_OTHER_02_MASK       (tUdsSessionMask) 0x20U
#define UDS_SESSION_OTHER_03_MASK       (tUdsSessionMask) 0x40U
#define UDS_SESSION_OTHER_04_MASK       (tUdsSessionMask) 0x80U
typedef u8 tUdsSessionMask;

/* Routine status defines of PSA and RSA_MIXED variants */
#define UDS_ROUTINE_STATUS_INIT                 (tRoutineStatus) 0x00U
#define UDS_ROUTINE_STATUS_IN_PROGRESS          (tRoutineStatus) 0x01U
#define UDS_ROUTINE_STATUS_FINISHED_OK          (tRoutineStatus) 0x02U
#define UDS_ROUTINE_STATUS_FINISHED_NOK         (tRoutineStatus) 0x03U
#define UDS_ROUTINE_STATUS_STOPPED              (tRoutineStatus) 0x04U
#define UDS_ROUTINE_STATUS_NOT_STARTED          (tRoutineStatus) 0x05U
#define UDS_ROUTINE_STATUS_NOT_SUPPORTED        (tRoutineStatus) 0x0FU

/* Routine status defines of RSA Variant */
#define UDS_RSA_ROUTINE_STATUS_COMPLETED            (tRoutineStatus) 0x00U
#define UDS_RSA_ROUTINE_STATUS_ABORTED              (tRoutineStatus) 0x01U
#define UDS_RSA_ROUTINE_STATUS_ACTIVE               (tRoutineStatus) 0x02U
#define UDS_RSA_ROUTINE_STATUS_INIT                 (tRoutineStatus) 0x04U
#define UDS_RSA_ROUTINE_STATUS_STOPPED              (tRoutineStatus) 0x08U
#define UDS_RSA_ROUTINE_STATUS_NOT_STARTED          (tRoutineStatus) 0x0CU
#define UDS_RSA_ROUTINE_STATUS_NOT_SUPPORTED        (tRoutineStatus) 0x0FU

/* Routine Status Mask for RSA variant */
#define UDS_ROUTINE_STATUS_MASK     0x0FU

/* Routine Type for RSA variant */
#define UDS_ROUTINE_TYPE_1              0x10U
#define UDS_ROUTINE_TYPE_2              0x20U
#define UDS_ROUTINE_TYPE_3              0x30U
#define UDS_ROUTINE_TYPE_NOT_SUPPORTED  0xF0U

/* Routine status */
#define UDS_ROUTINE_INIT            UDS_RSA_ROUTINE_STATUS_INIT
#define UDS_ROUTINE_IN_PROGRESS     UDS_RSA_ROUTINE_STATUS_ACTIVE
#define UDS_ROUTINE_FINISHED_OK     UDS_RSA_ROUTINE_STATUS_COMPLETED
#define UDS_ROUTINE_FINISHED_NOK    UDS_RSA_ROUTINE_STATUS_ABORTED
#define UDS_ROUTINE_STOPPED         UDS_RSA_ROUTINE_STATUS_STOPPED
#define UDS_ROUTINE_NOT_STARTED     UDS_RSA_ROUTINE_STATUS_NOT_STARTED
#define UDS_ROUTINE_NOT_SUPPORTED   UDS_RSA_ROUTINE_STATUS_NOT_SUPPORTED
typedef u8 tRoutineStatus;

/* Long request sequence step */
typedef u8 tUdsLongRequestState;

/* Addressing mode (functionnal or physical) */
#define UDS_ADDR_PHYSICAL       0x01U
#define UDS_ADDR_FUNCTIONAL     0x02U
#define UDS_ADDR_PHYSICAL_FUNCTIONAL     0x03U
typedef u8 tUdsAddrMode;

/* Addressing Type DIAG / OBD */
#define UDS_TYPE_DIAG           0x00U
#define UDS_TYPE_OBD            0x01U
#define UDS_TYPE_DIAG_OBD       0x02U
typedef u8 tUdsAddrType;

/* Diagnostic service index definition
   The type size depends on the number of managed frames */
#if ((((UDS_SRV_CFG1_MAX <= U8_MAX) && (UDS_SRV_CFG2_MAX <= U8_MAX)) && ((UDS_SRV_CFG3_MAX <= U8_MAX) && (UDS_SRV_CFG4_MAX <= U8_MAX))) && (UDS_SRV_CFG5_MAX <= U8_MAX))
    typedef u8 tUdsSrvIdx;
#elif ((((UDS_SRV_CFG1_MAX <= U16_MAX) && (UDS_SRV_CFG2_MAX <= U16_MAX)) && ((UDS_SRV_CFG3_MAX <= U16_MAX) && (UDS_SRV_CFG4_MAX <= U16_MAX))) && (UDS_SRV_CFG5_MAX <= U16_MAX))
    typedef u16 tUdsSrvIdx;
#elif ((((UDS_SRV_CFG1_MAX <= U32_MAX) && (UDS_SRV_CFG2_MAX <= U32_MAX)) && ((UDS_SRV_CFG3_MAX <= U32_MAX) && (UDS_SRV_CFG4_MAX <= U32_MAX))) && (UDS_SRV_CFG5_MAX <= U32_MAX))
    typedef u32 tUdsSrvIdx;
#endif

/* Diagnostic session timeout
   Range : (0 .. 2^32) */
#define UDS_SESS_TIMEOUT         ( 5000U / UDS_MANAGE_PERIOD )

/* Diagnostic session timeout counter definition
   The type size depends on the number of managed frames */
#if (UDS_SESS_TIMEOUT <= U8_MAX)
    typedef u8 tUdsSessionTimeout;
#elif (UDS_SESS_TIMEOUT <= U16_MAX)
    typedef u16 tUdsSessionTimeout;
#elif (UDS_SESS_TIMEOUT <= U32_MAX)
    typedef u32 tUdsSessionTimeout;
#endif 

#if ((UDS_EB_CAN_TYPE == RSA_UDS) || (UDS_EB_CAN_TYPE == PSA))
    typedef u16 tRi;
#elif (UDS_EB_CAN_TYPE == RSA_MIXED)
    typedef u8 tRi;
#endif

#if (UDS_SRV_CALLBACK)
typedef tUdsStatus (* tUdsRxMsgInd) (u16 *puwLen, u8 *aubUdsData);
#endif

typedef struct
{
    tRi uwRi;
    tRoutineStatus eStart;
    tRoutineStatus eStatus;

}tRoutine;


/* Diagnostic services configuration 1, stored in Read-Only memory */
typedef struct
{
    u8   ubSrvId;                       /* Service value */
    u8   ubSubSrvId;                    /* Sub-service value */
    tUdsSessionMask   eSessionMasks;    /* Session masks for which this service is authorized */
    u16  uwReqLen;                      /* Sub-service request length */
    tUdsAddrMode ubAddrMode;            /* Mode value (physical / functional / both) */
    #if (UDS_SRV_CALLBACK)     /* Individual callback for each service */
		tUdsRxMsgInd  pfuRxMsgInd; /* Request reception notification callback */
    #endif
} tUdsSrvCfg1;

/* Diagnostic services configuration 2, stored in Read-Only memory */
typedef struct
{
    u8   ubSrvId;                       /* Service value */
    u16  uwDid;                        /* Data identifier */
    tUdsSessionMask   eSessionMasks;    /* Session masks for which this service is authorized */
    u16  uwReqLen;                      /* Sub-service request length */
    tUdsAddrMode ubAddrMode;            /* Mode value (physical / functional / both) */
    #if (UDS_SRV_CALLBACK)     /* Individual callback for each service */
		tUdsRxMsgInd  pfuRxMsgInd; /* Request reception notification callback */
    #endif
} tUdsSrvCfg2;

/* Diagnostic services configuration 3, stored in Read-Only memory */
typedef struct
{
    u8   ubSrvId;                       /* Service value */
    u8   ubSubSrvId;                    /* Sub-service value */
    tRi  uwRi;                            /* Routine identifier */
    tUdsSessionMask   eSessionMasks;    /* Session masks for which this service is authorized */
    u16  uwReqLen;                      /* Sub-service request length */
    tUdsAddrMode ubAddrMode;            /* Mode value (physical / functional / both) */
    #if (UDS_SRV_CALLBACK)     /* Individual callback for each service */
		tUdsRxMsgInd  pfuRxMsgInd; /* Request reception notification callback */
    #endif
} tUdsSrvCfg3;

/* Diagnostic services configuration 4, stored in Read-Only memory */
typedef struct
{
    u8   ubSrvId;                       /* Service value */
    u8   ubLid;                          /* Routine Local identifier */
    tUdsSessionMask   eSessionMasks;    /* Session masks for which this service is authorized */
    u16  uwReqLen;                      /* Sub-service request length */
    tUdsAddrMode ubAddrMode;            /* Mode value (physical / functional / both) */
    #if (UDS_SRV_CALLBACK)     /* Individual callback for each service */
		tUdsRxMsgInd  pfuRxMsgInd; /* Request reception notification callback */
    #endif
} tUdsSrvCfg4;

/* Diagnostic services configuration 5, stored in Read-Only memory */
typedef struct
{
    u8   ubSrvId;                       /* Service value */
    u16  uwReqLen;                      /* Sub-service request length */
    tUdsAddrMode ubAddrMode;            /* Mode value (physical / functional / both) */
    #if (UDS_SRV_CALLBACK)     /* Individual callback for each service */
		tUdsRxMsgInd  pfuRxMsgInd; /* Request reception notification callback */
    #endif
} tUdsSrvCfg5;

#endif      /* UDS_TYPES_H */
