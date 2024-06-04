/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : UDS_Pub.h
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

#ifndef UDS_PUB_H
#define UDS_PUB_H


/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Constants                                                            */

/* Null pointer value */
#define UDS_NULL                 (void *)(0)

/* Authorized Diagnostic services identifier */
#define UDS_SRV_DSC              0x10U
#define UDS_SRV_ER               0x11U
#define UDS_SRV_SA               0x27U
#define UDS_SRV_CC               0x28U
#define UDS_SRV_TP               0x3EU

#define UDS_SRV_RDBLID           0x21U /* Specific RSA */
#define UDS_SRV_WDBLID           0x3BU /* Specific RSA */
#define UDS_SRV_IOCBLID          0x30U /* Specific RSA */

#define UDS_SRV_SRBLID           0x31U /* Specific RSA */
#define UDS_SRV_SPRBLID          0x32U /* Specific RSA */
#define UDS_SRV_RC               0x31U /* Specific PSA */

#define UDS_SRV_CDTCS            0x85U
#define UDS_SRV_LC               0x87U
#define UDS_SRV_IOCBI            0x2FU

#define UDS_SRV_RMBA             0x23U
#define UDS_SRV_RSDBI            0x24U
#define UDS_SRV_DLIDDY           0x2CU /* Specific RSA */
#define UDS_SRV_WMBA             0x3DU
#define UDS_SRV_CDTCI            0x14U
#define UDS_SRV_RDTCI            0x19U
#define UDS_SRV_RD               0x34U
#define UDS_SRV_RU               0x35U
#define UDS_SRV_TD               0x36U
#define UDS_SRV_RTE              0x37U
#define UDS_SRV_RDBI             0x22U
#define UDS_SRV_WDBI             0x2EU
#define UDS_TEST                 0xABU

/* OBD services */
#define UDS_SRV_OBD_SID_00       0x00U
#define UDS_SRV_OBD_SID_01       0x01U
#define UDS_SRV_OBD_SID_02       0x02U
#define UDS_SRV_OBD_SID_03       0x03U
#define UDS_SRV_OBD_SID_04       0x04U
#define UDS_SRV_OBD_SID_05       0x05U
#define UDS_SRV_OBD_SID_06       0x06U
#define UDS_SRV_OBD_SID_07       0x07U
#define UDS_SRV_OBD_SID_08       0x08U
#define UDS_SRV_OBD_SID_09       0x09U
#define UDS_SRV_OBD_SID_10       0x0AU
#define UDS_SRV_OBD_SID_11       0x0BU
#define UDS_SRV_OBD_SID_12       0x0CU
#define UDS_SRV_OBD_SID_13       0x0DU
#define UDS_SRV_OBD_SID_14       0x0EU
#define UDS_SRV_OBD_SID_15       0x0FU


/* Authorized Diagnostic services length */
#define UDS_LEN_NRC              3U

/* Negative code */
#define UDS_NRC                  (u8)0x7FU

/* Long treatment enumeration steps */
#define UDS_LONG_REQ_OFF       0U	/* No NACK_78 running */
#define UDS_LONG_REQ_ON        1U	/* NACK_78 is sent every UDS_NRC78_PERIOD ms */
#define UDS_LONG_REQ_END       2U	/* The real response of the service has been received from the application */

/* SPREC available states */
#define UDS_SPREC_OFF          0U
#define UDS_SPREC_ON           1U

/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                */

/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */

#if ( UDS_SRV_CFG1_MAX)
/* Diagnostic services configuration 1, stored in Read-Only memory */
extern const tUdsSrvCfg1 m_astDiagSrvCfg1[UDS_SRV_CFG1_MAX];
#endif
#if ( UDS_SRV_CFG2_MAX)
/* Diagnostic services configuration 2, stored in Read-Only memory */
extern const tUdsSrvCfg2 m_astDiagSrvCfg2[UDS_SRV_CFG2_MAX];
#endif
#if ( UDS_SRV_CFG3_MAX)
/* Diagnostic services configuration 3, stored in Read-Only memory */
extern const tUdsSrvCfg3 m_astDiagSrvCfg3[UDS_SRV_CFG3_MAX];
#endif
#if ( UDS_SRV_CFG4_MAX)
/* Diagnostic services configuration 4, stored in Read-Only memory */
extern const tUdsSrvCfg4 m_astDiagSrvCfg4[UDS_SRV_CFG4_MAX];
#endif
#if ( UDS_SRV_CFG5_MAX)
/* Diagnostic services configuration 5, stored in Read-Only memory */
extern const tUdsSrvCfg5 m_astDiagSrvCfg5[UDS_SRV_CFG5_MAX];
#endif

/* Diagnostic session timeout
   Range : (0 .. 2^32) */
extern const tUdsSessionTimeout m_uUdsSessTimeout;

/* Diagnostic services number of the configuration 1,2,3,4 and 5
   managed by this software layer Range : (0 .. 2^32) */
extern const tUdsSrvIdx  m_uUdsSrvCfg1Max;
extern const tUdsSrvIdx  m_uUdsSrvCfg2Max;
extern const tUdsSrvIdx  m_uUdsSrvCfg3Max;
extern const tUdsSrvIdx  m_uUdsSrvCfg4Max;
extern const tUdsSrvIdx  m_uUdsSrvCfg5Max;

/* Declare the external default value of the m_eSessionCode variable */
extern const tUdsSessionProtType m_eSessionCodeDefaultValue;

/* Declare the external default value of the m_eSessionMask variable */
extern const tUdsSessionProtMask m_eSessionMaskDefaultValue;

#ifdef NCS_UNITARY_TEST
/* Current session code (DSC service parameter) */
extern tUdsSessionProtType m_eSessionCode;
extern tUdsSessionProtMask m_eSessionMask;
#endif
/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */

extern void UDS_Init (void);
extern tUdsBoolean UDS_RxRequest (u16 *puwLen, u8 *aubUdsData);
extern tUdsBoolean UDS_RxRequestWithAddrMode (u16 *puwLen, u8 *aubUdsData, tUdsAddrMode eUdsAddrMode, tUdsAddrType eUdsAddrType);
extern void UDS_ReloadTStopDiag (void);
extern void UDS_LongRequestEnd (u16 uwLen, u8 *aubUdsData, tUdsStatus eStatus);
extern void UDS_LongRequestRespTxConf(void);
extern void UDS_ResetNRC78Timer(void);

/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */

extern void UDS_Manage (void);
#if ( UDS_SRV_CFG3_MAX )
    extern void UDS_EndOfRoutineControlInd(tRoutineStatus eRoutineStatus);
#endif


#endif      /* UDS_PUB_H */
