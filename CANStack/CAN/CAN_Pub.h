/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : CAN_Pub.h
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

#ifndef CAN_PUB_H
#define CAN_PUB_H


/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Header Files:                                                               */


/*                                                                               */
/*********************************************************************************/




/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Constants                                                            */


/* CAN frame type */
#define CAN_FRAME_TYPE_DATA             0U
#define CAN_FRAME_TYPE_REMOTE           1U

/* Substitute Remote Request */
#define CAN_SRR_ACTIVATION               0U
#define CAN_SRR_DEACTIVATION             1U

/* CAN mask */
#if (COM_EXTENDED_ID == COM_TRUE)
#define CAN_MASK_ALL_IDENT              0x0000000U
#define CAN_MASK_UNIQUE_IDENT           0x1FFFFFFU
#define CAN_ACCEPT_MASK_ALL_EXT_IDENT   0x0000000U
#else
#define CAN_MASK_ALL_IDENT       0x000U
#define CAN_MASK_UNIQUE_IDENT    0x7FFU
#endif

/* CAN identifier type */
#define CAN_IDENT_STD                   0U
#define CAN_IDENT_EXT                   1U

/* CAN mailbox mode */
#define CAN_MAILBOX_MODE_DISABLED       0xFFU
#define CAN_MAILBOX_MODE_TX             0x00U
#define CAN_MAILBOX_MODE_RX             0x08U
#define CAN_MAILBOX_MODE_RX_MASK1       0x10U
#define CAN_MAILBOX_MODE_RX_MASK2       0x18U
#define CAN_MAILBOX_MODE_RX_MASK3       0x20U
#define CAN_MAILBOX_MODE_RX_MASK4       0x28U
#define CAN_MAILBOX_MODE_RX_EXT         CAN_MAILBOX_MODE_RX_MASK1

/* CAN interrupts */
#define CAN_INTERRUPT_NONE              0x0000U
#define CAN_INTERRUPT_TX_OK             0x0007U
#define CAN_INTERRUPT_RX_OK             0x0100U
#define CAN_INTERRUPT_RX_OVRUN          0x0200U
#define CAN_INTERRUPT_BUS_OFF           0x4400U
#define CAN_INTERRUPT_ERROR_STATUS      0x4000U
#define CAN_INTERRUPT_WAKE_UP           0x8000U

/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                */

/* CAN functions status */
#define CAN_ERR_OK                      0x00U
#define CAN_ERR_CORRUPTION              0x01U
#define CAN_ERR_SLEEP					0x02U
#define CAN_ERR_WAKEUP					0x03U
#define CAN_ERR_WAKEUP_SLEEP  			0x04U
typedef u8 Can_ReturnType;


#ifdef CAN_CORRUPT_DEBUG
  #define CAN_CONST
#else
  #define CAN_CONST    const
#endif


/* CAN mailbox configuration */
typedef struct
{
    u8   ubBrp;                  /* Baudrate : BRP */
    u8   ubPropSeg;              /* Baudrate : PROP_SEG */
    u8   ubPSeg1;                /* Baudrate : PHASE_SEG_1 */
    u8   ubPSeg2;                /* Baudrate : PHASE_SEG_2 */
    u8   ubSmp;                  /* Baudrate : SAMPLE_MODE */
    u8   ubSjw;                  /* Baudrate : SJW */
}tCanBaudRateConfig;

/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */

extern CAN_CONST u8 m_ubCanBitBrp ;
extern CAN_CONST u8 m_ubCanTimePropseg  ;
extern CAN_CONST u8 m_ubCanTimePseg1 ;
extern CAN_CONST u8 m_ubCanTimePseg2 ;
extern CAN_CONST u8 m_ubCanBitTimeSmp ;
extern CAN_CONST u8 m_ubCanBitTimeSjw ;


extern CAN_CONST u16 m_ubCanAcceptIdent0  ;
extern CAN_CONST u16 m_ubCanAcceptMask0 ;
extern CAN_CONST u16 m_ubCanAcceptIdent1  ;
extern CAN_CONST u16 m_ubCanAcceptMask1 ;
extern CAN_CONST u16 m_ubCanAcceptIdent2 ;
extern CAN_CONST u16 m_ubCanAcceptMask2 ;
extern CAN_CONST u16 m_ubCanAcceptIdent3  ;
extern CAN_CONST u16 m_ubCanAcceptMask3  ;

/*Add By BWF*/
extern u8 Received_Inplication_MsgFlg;
extern u16 BusOffRecoveryCnt;
/* Manage bit timing dinamycaly by interacting with APP */
extern tCanBaudRateConfig m_stbitTimingUpdate;
/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */

/* CAN_Prg.c */

extern void CAN_SetMailboxStdIdent (tCanMailboxIdx uMailboxIdx, u16 uwIdent);
extern void CAN_SetMailboxExtIdent (tCanMailboxIdx uMailboxIdx, u32 ulIdent);
extern void CAN_SetMailboxLen (tCanMailboxIdx uMailboxIdx, u8 ubLen);
extern void CAN_SetMailboxRtr (tCanMailboxIdx uMailboxIdx, u8 ubRtr);
extern void CAN_SetMailboxExtRtr (tCanMailboxIdx uMailboxIdx, u8 ubRtr);
extern void CAN_SetMailboxExtSrr (tCanMailboxIdx uMailboxIdx, u8 ubSrr);
extern void CAN_SetMailboxData (tCanMailboxIdx uMailboxIdx, const u8 *aubData);

extern u16 CAN_GetMailboxStdIdent (tCanMailboxIdx uMailboxIdx);
extern u32 CAN_GetMailboxExtIdent (tCanMailboxIdx uMailboxIdx);
extern u8 CAN_GetMailboxLen (tCanMailboxIdx uMailboxIdx);
extern u8 CAN_GetMailboxRtr (tCanMailboxIdx uMailboxIdx);
extern u8 CAN_GetMailboxExtRtr (tCanMailboxIdx uMailboxIdx);
extern u8 CAN_GetMailboxExtSrr (tCanMailboxIdx uMailboxIdx);
extern void CAN_GetMailboxData (tCanMailboxIdx uMailboxIdx, u8 *aubData);
extern void CAN_GetMailboxDataWithMirror (tCanMailboxIdx uMailboxIdx, u8 *aubData, u8 *aubMirror);

extern void CAN_TransmitFrame (tCanMailboxIdx uMailboxIdx);
extern void CAN_CancelFrame (tCanMailboxIdx uMailboxIdx);

#if ((CAN_TX_MAILBOX_NB == 0) || (CAN_RX_MAILBOX_NB == 0))
    extern void CAN_SetMailboxMode (tCanMailboxIdx uMailboxIdx, u8 ubMode);
#endif

#ifdef CAN_MAILBOX_INDIVIDUAL_MASK
    extern void CAN_SetMailboxStdMask (tCanMailboxIdx uMailboxIdx, u16 uwMask);
	extern void CAN_SetMailboxExtMask (tCanMailboxIdx uMailboxIdx, u32 uwMask);
#endif
extern void CAN_SetMailboxIde (tCanMailboxIdx uMailboxIdx, u8 ubIde);

extern void CAN_SetMailboxInterruptCfg (tCanMailboxIdx uMailboxIdx, u16 uwIntCfg);
extern void CAN_MailboxInterruptEnable (tCanMailboxIdx uMailboxIdx);

//extern void CAN_SetGeneralInterruptCfg (u16 uwIntCfg);

extern void CAN_SetGeneralStdMask (u16 uwMask);
extern void CAN_SetGeneralExtMask (u32 m_ubCanExtAcceptMask);

extern void CAN_Activate (void);
extern void CAN_Desactivate (void);

extern void CAN_Init (void);
//extern void CAN_BitTimeCfg (void);

extern void CAN_PowerOnInit (void);
extern void CAN_Sleep (void);

extern void CAN_BusOff (void);
extern void CAN_BusOn (void);

extern u8 CAN_GetSampledNerrBit (void);

extern u8 CAN_SleepStateCheck (void);

#ifdef CAN_MAILBOX_INDIVIDUAL_INTERRUPT
    extern void CAN_ResetMailboxTxFlag (tCanMailboxIdx uMailboxIdx);
    extern void CAN_ResetMailboxRxFlag (tCanMailboxIdx uMailboxIdx);
#endif


/* CAN_Trcv.c */
extern void CAN_TransceiverPinCfg (void);
extern void CAN_SetTransceiverMode (u8 ubEn, u8 ubStb);
extern void CAN_GetNerrBit (u8 * const pbNerr);
extern void CAN_PortCfg (void);

/* CAN_Int.c */
//extern void CAN_InterruptLevelCfg (void);
extern void CAN_InterruptEnable (void);
extern void CAN_InterruptDisable (void);


/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */

extern void CAN_Interrupt (void);
extern void CAN_ErrorStatusInterrupt (void);
#ifdef CAN_WAKE_UP_DETECTION
    extern void CAN_WakeUpInterrupt (void);
#endif

/*                                                                               */
/*********************************************************************************/



#endif      // CAN_PUB_H
