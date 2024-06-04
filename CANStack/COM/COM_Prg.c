/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : COM_Prg.c
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

/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */

u8 ubConfigurationSelection;

/* Data buffer */
u8 m_ComaaubData[COM_FRAME_MAX_RAM][COM_DATA_MAX];

// zhaoziyi
extern can_mailbox_descriptor_struct g_transmit_message;


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

/* Protocol handler mailbox status */
#define COM_MAILBOX_FREE        0U
#define COM_MAILBOX_BUSY        1U
typedef u8 tComMailboxStatus;


/* Transmission FIFO index definition */
/* The type size depends on the transmission FIFO size */
#if (COM_TX_FIFO_SIZE <= U8_MAX)
    typedef u8 tComFrameIdxFifoIdx;
#elif (COM_TX_FIFO_SIZE <= U16_MAX)
    typedef u16 tComFrameIdxFifoIdx;
#elif (COM_TX_FIFO_SIZE <= U32_MAX)
    typedef u32 tComFrameIdxFifoIdx;
#endif


/* Transmission mailbox FIFO, which contains only the COM frame index */
typedef struct
{
    tComFrameIdxFifoIdx    uWrIdx;         /* Write index (0 .. COM_TX_FIFO_SIZE - 1) */
    tComFrameIdxFifoIdx    uRdIdx;         /* Read index (0 .. COM_TX_FIFO_SIZE - 1) */
    tComFrameIdx           auFrameIdx[COM_TX_FIFO_SIZE]; /* FIFO (0 .. COM_FRAME_MAX - 1) */
} tComTxFrameIdxFifo;


/* Frame status */
#define COM_FRAME_STATUS_TX_REQ             0x01U
#define COM_FRAME_STATUS_RX_LOST_FLAG       0x01U
#define COM_FRAME_STATUS_TX_PROGRESS        0x02U
#define COM_FRAME_STATUS_RX_FLAG            0x02U
#define COM_FRAME_STATUS_PERIOD_EN          0x04U
#define COM_FRAME_STATUS_TX_CONF            0x08U
#define COM_MON_NODE_RELOAD                 0x08U
#define COM_MON_NODE_TASK                   0x10U
#define COM_MON_NODE_STATUS_OK              0x20U
#define COM_MON_NODE_STATUS_NOK             0x00U
#define COM_FRAME_STATUS_TX_DATA            0x80U

/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */

/* COM software layer status */
/* Range : (COM_MODE_OFF, COM_MODE_BUS_ON, COM_MODE_BUS_OFF) */
NCS_PRI_VAR tComMode m_eComMode;

/* Frame length for dynamic frames in transmission */
#if (COM_TX_DYNAMIC_FRAME_MAX_RAM)
    NCS_PRI_VAR u8 m_aubDynamicFrameLen[COM_TX_DYNAMIC_FRAME_MAX_RAM];
#endif
/* Length for last frames in reception */
NCS_PRI_VAR u8 m_aubRxFrameLen[COM_RX_FRAME_MAX_RAM];

#ifndef BOOTLOADER
/* Transmission mailbox FIFO, which contains only the COM frame index */
NCS_PRI_VAR tComTxFrameIdxFifo m_stTxFrameIdxFifo;
#endif

/* Protocol handler transmission mailbox status */
/* Range : (COM_MAILBOX_FREE .. COM_MAILBOX_BUSY) */
NCS_PRI_VAR tComMailboxStatus m_ebTxMailboxStatus[COM_TX_MAILBOX_NB_RAM];

/* Index of the frame which is in transmission in the protocol handler */
/* Range : (0 .. COM_FRAME_MAX - 1) */
NCS_PRI_VAR tComFrameIdx m_uTxFrameIdx[COM_TX_MAILBOX_NB_RAM];

/* Transmission monitoring counter (only one for all the monitored frames */
/* because of using only one protocol handler mailbox for transmission) */
/* Range : (0 .. COM_TX_MON_TIMEOUT_MAX - 1) */
NCS_PRI_VAR tComTxTimeout m_uTxTimeoutCnt[COM_TX_MAILBOX_NB_RAM];

/* Data Link Layer frames */
#ifdef NCS_SDF_DATA_MIRROR
NCS_PRI_VAR u8 m_aaubDataMirror[COM_RX_FRAME_MAX_RAM][COM_DATA_MAX];   /* Data buffer mirror */
#endif
u8 m_aubStatus[COM_FRAME_MAX_RAM];                /* Status */

#ifndef BOOTLOADER
NCS_PRI_VAR tComExTime m_auComFrameExTimeCnt[COM_FRAME_MAX_RAM];      /* Exclusion time counter (0 .. COM_EXCLUSION_TIME_MAX) */

NCS_PRI_VAR tComPeriod m_auComFramePeriodCnt[COM_FRAME_MAX_RAM];      /* Period counter (0 .. COM_PERIOD_MAX) */

/* Monitored nodes in transmission and reception */

NCS_PRI_VAR u8 m_aeComMonOldStatus[COM_MON_NODE_MAX];          /* Previous status (COM_MON_OK or COM_MON_NOK) */
NCS_PRI_VAR u8 m_aubComMonFilterCnt[COM_MON_NODE_MAX];         /* Filtering counter */
NCS_PRI_VAR u8 m_eComNodeConfStatus[COM_MON_NODE_MAX];         /* Indication memorisation */
NCS_PRI_VAR tComMonStatus m_aeFrameStatus[COM_RX_FRAME_MAX_RAM];
#endif
/*-------------------------------------------------------------------------------*/
/* - Private Macros                                                              */

/* Macros for transmission FIFO */

#define _COM_TxFifoEmpty(stFifo)      ((stFifo).uWrIdx == (stFifo).uRdIdx)

#define _COM_TxFifoFull(stFifo)       ((((stFifo).uWrIdx + 1) % (COM_TX_FIFO_SIZE)) == ((stFifo).uRdIdx))

#define _COM_TxFifoRdIndInc(stFifo)   ((stFifo).uRdIdx = (tComFrameIdxFifoIdx)(((stFifo).uRdIdx + 1) % (COM_TX_FIFO_SIZE)))
#define _COM_TxFifoWrIndInc(stFifo)   ((stFifo).uWrIdx = (tComFrameIdxFifoIdx)(((stFifo).uWrIdx + 1) % (COM_TX_FIFO_SIZE)))

#if (CAN_CTRL_NB>1U)

  #define COM_CAN_DESACTIVATE\
                CAN_Desactivate(COM_CAN_BUS_IDX)
  #define COM_CAN_ACTIVATE\
                CAN_Activate(COM_CAN_BUS_IDX)
  #define COM_CAN_SET_MAILBOX_MODE(X,Y)\
                CAN_SetMailboxMode(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_SET_MAILBOX_STD_MASK(X,Y)\
                CAN_SetMailboxStdMask(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_SET_MAILBOX_EXT_MASK(X,Y)\
                CAN_SetMailboxExtMask(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_SET_MAILBOX_IDE(X,Y)\
                CAN_SetMailboxIde(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_SET_MAILBOX_RTR(X,Y)\
                CAN_SetMailboxRtr(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_GET_MAILBOX_RTR(X)\
                CAN_GetMailboxRtr(COM_CAN_BUS_IDX,X)
  #define COM_CAN_SET_MAILBOX_STD_IDENT(X,Y)\
                CAN_SetMailboxStdIdent(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_SET_MAILBOX_EXT_IDENT(X,Y)\
                CAN_SetMailboxExtIdent(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_GET_MAILBOX_STD_IDENT(X)\
                CAN_GetMailboxStdIdent(COM_CAN_BUS_IDX,X)
  #define COM_CAN_GET_MAILBOX_EXT_IDENT(X)\
                CAN_GetMailboxExtIdent(COM_CAN_BUS_IDX,X)
  #define COM_CAN_SET_MAILBOX_INTERRUPT_CFG(X,Y)\
                CAN_SetMailboxInterruptCfg(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_MAILBOX_INTERRUPT_ENABLE(X)\
                CAN_MailboxInterruptEnable(COM_CAN_BUS_IDX,X)
  #define COM_CAN_SET_GENERAL_INTERRUPT_CFG(X)\
                CAN_SetGeneralInterruptCfg(COM_CAN_BUS_IDX,X)
  #define COM_CAN_INTERRUPT_LEVEL_CFG\
                CAN_InterruptLevelCfg(COM_CAN_BUS_IDX)
  #define COM_CAN_INTERRUPT_ENABLE\
                CAN_InterruptEnable(COM_CAN_BUS_IDX)
  #define COM_CAN_INTERRUPT_DISABLE\
                CAN_InterruptDisable(COM_CAN_BUS_IDX)
  #define COM_CAN_RESET_MAILBOX_TX_FLAG(X)\
                CAN_ResetMailboxTxFlag(COM_CAN_BUS_IDX,X)
  #define COM_CAN_GET_MAILBOX_LEN(X)\
                CAN_GetMailboxLen(COM_CAN_BUS_IDX,X)
  #define COM_CAN_GET_MAILBOX_DATA(X,Y)\
                CAN_GetMailboxData(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_GET_MAILBOX_DATA_WITH_MIRROR(X,Y,Z)\
                CAN_GetMailboxDataWithMirror(COM_CAN_BUS_IDX,X,Y,Z)
  #define COM_CAN_CANCEL_FRAME(X)\
                CAN_CancelFrame(COM_CAN_BUS_IDX,X)
  #define COM_CAN_SET_GENERAL_STD_MASK(X)\
                CAN_SetGeneralStdMask(COM_CAN_BUS_IDX,X)
  #define COM_CAN_SET_GENERAL_EXT_MASK(X)\
                CAN_SetGeneralExtMask(COM_CAN_BUS_IDX,X)
  #define COM_CAN_TRANSMIT_FRAME(X)\
                CAN_TransmitFrame(COM_CAN_BUS_IDX,X)
  #define COM_CAN_SET_MAILBOX_DATA(X,Y)\
                CAN_SetMailboxData(COM_CAN_BUS_IDX,X,Y)
  #define COM_CAN_SET_MAILBOX_LEN(X,Y)\
                CAN_SetMailboxLen(COM_CAN_BUS_IDX,X,Y)
#else

  #define COM_CAN_DESACTIVATE\
                CAN_Desactivate()
  #define COM_CAN_ACTIVATE\
                CAN_Activate()
  #define COM_CAN_SET_MAILBOX_MODE(X,Y)\
                CAN_SetMailboxMode(X,Y)
  #define COM_CAN_SET_MAILBOX_STD_MASK(X,Y)\
                CAN_SetMailboxStdMask(X,Y)
  #define COM_CAN_SET_MAILBOX_EXT_MASK(X,Y)\
                CAN_SetMailboxExtMask(X,Y)
  #define COM_CAN_SET_MAILBOX_IDE(X,Y)\
                CAN_SetMailboxIde(X,Y)
  #define COM_CAN_SET_MAILBOX_RTR(X,Y)\
                CAN_SetMailboxRtr(X,Y)
  #define COM_CAN_GET_MAILBOX_RTR(X)\
                CAN_GetMailboxRtr(X)
  #define COM_CAN_SET_MAILBOX_STD_IDENT(X,Y)\
                CAN_SetMailboxStdIdent(X,Y)
  #define COM_CAN_SET_MAILBOX_EXT_IDENT(X,Y)\
                CAN_SetMailboxExtIdent(X,Y)
  #define COM_CAN_GET_MAILBOX_STD_IDENT(X)\
                CAN_GetMailboxStdIdent(X)
  #define COM_CAN_GET_MAILBOX_EXT_IDENT(X)\
                CAN_GetMailboxExtIdent(X)
  #define COM_CAN_SET_MAILBOX_INTERRUPT_CFG(X,Y)\
                CAN_SetMailboxInterruptCfg(X,Y)
  #define COM_CAN_MAILBOX_INTERRUPT_ENABLE(X)\
                CAN_MailboxInterruptEnable(X)
  #define COM_CAN_SET_GENERAL_INTERRUPT_CFG(X)\
                CAN_SetGeneralInterruptCfg(X)
  #define COM_CAN_INTERRUPT_LEVEL_CFG\
                CAN_InterruptLevelCfg()
  #define COM_CAN_INTERRUPT_ENABLE\
                CAN_InterruptEnable()
  #define COM_CAN_INTERRUPT_DISABLE\
                CAN_InterruptDisable()
  #define COM_CAN_RESET_MAILBOX_TX_FLAG(X)\
                CAN_ResetMailboxTxFlag(X)
  #define COM_CAN_GET_MAILBOX_LEN(X)\
                CAN_GetMailboxLen(X)
  #define COM_CAN_GET_MAILBOX_DATA(X,Y)\
                CAN_GetMailboxData(X,Y)
  #define COM_CAN_GET_MAILBOX_DATA_WITH_MIRROR(X,Y,Z)\
                CAN_GetMailboxDataWithMirror(X,Y,Z)
  #define COM_CAN_CANCEL_FRAME(X)\
                CAN_CancelFrame(X)
  #define COM_CAN_SET_GENERAL_STD_MASK(X)\
                CAN_SetGeneralStdMask(X)
  #define COM_CAN_SET_GENERAL_EXT_MASK(X)\
                CAN_SetGeneralExtMask(X)
  #define COM_CAN_TRANSMIT_FRAME(X)\
		        CAN_TransmitFrame(X)
  #define COM_CAN_SET_MAILBOX_DATA(X,Y)\
                CAN_SetMailboxData(X,Y)
  #define COM_CAN_SET_MAILBOX_LEN(X,Y)\
                CAN_SetMailboxLen(X,Y)
#endif
/*-------------------------------------------------------------------------------*/
/* - Private Function Prototypes                                                 */

NCS_PRI_FCT void COM_Activate (void);
NCS_PRI_FCT void COM_Desactivate (void);
#ifndef BOOTLOADER
NCS_PRI_FCT tComStatus COM_PeriodicStartStop (tComPeriodicMode eMode, tComFrameIdx uFrameIdx);
#endif
#if (COM_FILTERING_METHOD == COM_LINEAR_FILTERING)
NCS_PRI_FCT tComFrameIdx COM_FindRxFrameIdxFromIdent (tComFrameIdx uExTFrameIdx, tCanMailboxIdx uMailboxIdx);
#endif
NCS_PRI_FCT void COM_TxFrame (tCanMailboxIdx uMailboxIdx, tComFrameIdx uFrameIdx);
#ifndef BOOTLOADER
NCS_PRI_FCT void COM_TransmissionEnding (tComFrameIdx uFrameIdx);
NCS_PRI_FCT void COM_TxNextFrame (tCanMailboxIdx uMailboxIdx);
NCS_PRI_FCT void COM_MonNodeCntInit (tComMonNodeIdx uNodeIdx, tComMonStatus eFrameStatus);
NCS_PRI_FCT void COM_MonNodeInd (tComMonNodeIdx uNodeIdx, tComMonStatus eFrameStatus);
#endif
#if ((COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS)  || (COM_EB_CAN_TYPE == COM_EB_JAC_CAN_HS) || (COM_EB_CAN_TYPE == COM_EB_FIAT_CAN) || (COM_EB_CAN_TYPE == COM_EB_VW_CAN_HS))
NCS_PRI_FCT void COM_TxFail (tCanMailboxIdx uMailboxIdx);
#endif

/*-------------------------------------------------------------------------------*/
/* - Private Callback Prototypes                                                 */
NCS_PRI_FCT void COM_TxMonManage (void);
#ifndef BOOTLOADER
NCS_PRI_FCT void COM_TxManage (void);
NCS_PRI_FCT void COM_ExTimeManage (void);
NCS_PRI_FCT void COM_RxMonManage (void);
NCS_PRI_FCT void COM_MonNodeManage (void);
#endif

/*                                                                               */
/*********************************************************************************/




/******************************** IMPLEMENTATION *********************************/
/*                                                                               */

/*-------------------------------------------------------------------------------*/
/* - Public Functions                                                            */


/*********************************************************************************/
/* Name: COM_Init                                                                */
/*-------------------------------------------------------------------------------*/
/* Description: Initialize the COM layer to OFF state                            */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_eComMode: COM mode                              */
/*                             m_astComFrame: COM frames                         */
/*                             m_aubDynamicFrameLen: COM Tx frames length        */
/*                             m_aubRxFrameLen: COM Rx frames length             */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

void COM_Init (void)
{
    m_eComMode = COM_MODE_OFF;

#ifndef BOOTLOADER
    /* Frame datas and status initialisation */
    {
        /* Index for frame array */
        tComFrameIdx uFrameIdx;

        /* Index for data array */
        u8 ubDataIdx;

        #ifdef NCS_SDF_DATA_MIRROR
        /* Frame datas mirror initialisation, only for frames in reception */
        for (uFrameIdx = 0; uFrameIdx < COM_RX_FRAME_MAX; uFrameIdx ++)
        {
            for (ubDataIdx = 0; ubDataIdx < COM_DATA_MAX; ubDataIdx ++)
            {
                m_aaubDataMirror[uFrameIdx][ubDataIdx] = NCS_SDF_GET_U8_MIRROR(m_aaubComFrameInitData[uFrameIdx][ubDataIdx]);
            }
        }
        #endif
        /* Frame datas and status initialisation */
        for (uFrameIdx = 0; uFrameIdx < COM_FRAME_MAX; uFrameIdx ++)
        {
            for (ubDataIdx = 0; ubDataIdx < COM_DATA_MAX; ubDataIdx ++)
            {
                m_ComaaubData[uFrameIdx][ubDataIdx] = m_aaubComFrameInitData[uFrameIdx][ubDataIdx];
            }

            /* The COM WRITE buffers are consistent */
            m_aubStatus[uFrameIdx] = 0x00;
        }

        /* Dynamic frames in transmission length initialisation */
        /* (only if dynamic frames in transmission are defined) */
        #if (COM_TX_DYNAMIC_FRAME_MAX_RAM)
            for (uFrameIdx = COM_RX_FRAME_MAX; uFrameIdx < (COM_RX_FRAME_MAX + COM_TX_DYNAMIC_FRAME_MAX); uFrameIdx ++)
            {
                m_aubDynamicFrameLen[uFrameIdx - COM_RX_FRAME_MAX] = m_aubComFrameLenNominal[uFrameIdx];
            }
        #endif

        /* RX frames length initialisation */
        for (uFrameIdx = 0; uFrameIdx < COM_RX_FRAME_MAX; uFrameIdx ++)
        {
            m_aubRxFrameLen[uFrameIdx] = m_aubComFrameLenNominal[uFrameIdx];
        }
    } /* end of frame datas initialisation */
#endif
}

/* - Fin COM_Init                                                                */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_ModeModify                                                          */
/*-------------------------------------------------------------------------------*/
/* Description:                                                                  */
/*********************************************************************************/
/* Input parameters          : eMode: COM mode asked by the Application          */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_MODE: asked transition is invalid         */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_eComMode: COM mode                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_Activate                                      */
/*                             COM_Desactivate                                   */
/* Called macros             : None                                              */
/* Called callbacks          : COM_ModeModifyExtension                           */
/*********************************************************************************/

tComStatus COM_ModeModify (tComMode eMode)
{
    /* Return status */
    tComStatus eStatus;
    eStatus = COM_ERR_OK;

    /* COM current mode */
    switch (m_eComMode)
    {
        case COM_MODE_BUS_ON :
            /* COM mode asked by Application */
            switch (eMode)
            {
                /* COM disabling */
                case COM_MODE_OFF :

                    #ifndef BOOTLOADER
                    /* Periodic transmission frames and reception monitoring disabling */
                    eStatus = COM_Periodic (COM_PERIODIC_STOP);
                    #endif

                    /* COM desactivation */
                    COM_Desactivate ();

                    /* COM mode modification memorisation */
                    m_eComMode = eMode;

                    /* COM mode switching extension in Application */
                    COM_ModeModifyExtension (eMode);
                    break;

                case COM_MODE_BUS_OFF :
                    /* OSCCOM-157: Com_Desactivate must not be called in case of */
                    /*BUS_OFF because it can set the controller to SLEEP mode    */
                    /*depending on CAN_WAKEUP_DETECTION state                    */
                    /* COM_Desactivate (); */

                    /* COM mode modification memorisation */
                    m_eComMode = eMode;

                    /* COM mode switching extension in Application */
                    COM_ModeModifyExtension (eMode);
                    break;

                /* Invalid transition */
                default :
                    eStatus = COM_ERR_MODE;
                    break;
            } /* end switch (eMode) */
            break; /* end case COM_MODE_BUS_ON */


        case COM_MODE_BUS_OFF :
            /* COM mode asked by Application */
            switch (eMode)
            {
                /* COM disabling */
                case COM_MODE_OFF :

                    #ifndef BOOTLOADER
                    /* Periodic transmission frames and reception monitoring disabling */
                    eStatus = COM_Periodic (COM_PERIODIC_STOP);
                    #endif

                    /* COM desactivation */
                    COM_Desactivate ();

                    /* COM mode modification memorisation */
                    m_eComMode = eMode;

                    /* COM mode switching extension in Application */
                    COM_ModeModifyExtension (eMode);
                    break;

                /* COM enabling */
                case COM_MODE_BUS_ON :
                    /* COM activation */
                    COM_Activate ();

                    /* COM mode modification memorisation */
                    m_eComMode = eMode;

                    /* COM mode switching extension in Application */
                    COM_ModeModifyExtension (eMode);
                    break;

                /* Invalid transition */
                default :
                    eStatus = COM_ERR_MODE;
                    break;
            } /* end switch (eMode) */
            break; /* end case COM_MODE_BUS_OFF */


        case COM_MODE_OFF :
            /* COM mode asked by Application */
            switch (eMode)
            {
                /* COM enabling */
                case COM_MODE_BUS_ON :
                    /* COM activation */
                    COM_Activate ();

                    /* COM mode modification memorisation */
                    m_eComMode = eMode;

                    /* COM mode switching extension in Application */
                    COM_ModeModifyExtension (eMode);
                    break;

                /* Invalid transitions */
                default :
                    eStatus = COM_ERR_MODE;
                    break;
            } /* end switch (eMode) */
            break; /* end case COM_MODE_OFF */

         default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            eStatus = COM_ERR_COHE;
            break;

    }


    /* Return the status */
    return (eStatus);
}

/* - Fin COM_ModeModify                                                          */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: COM_Periodic                                                            */
/*-------------------------------------------------------------------------------*/
/* Description: COM periodic transmission or reception monitoring for all frames */
/*              start or stop and initialisation with first period value         */
/*********************************************************************************/
/* Input parameters          : eMode: periodic mode                              */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_OFF: COM is disabled                      */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_PeriodicStartStop                             */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#ifndef BOOTLOADER
tComStatus COM_Periodic (tComPeriodicMode eMode)
{
    /* Return status */
    tComStatus eStatus;
    eStatus = COM_ERR_OK;

    /* Transmission, reception and bus are enabled */
    /* COM current mode */
    switch (m_eComMode)
    {
        case COM_MODE_BUS_ON :
        {
            /* Temporary index to manage all the frames */
            tComFrameIdx uFrameIdx;

            /* For all the frames in transmission or reception */
            for (uFrameIdx = 0; uFrameIdx < COM_FRAME_MAX; uFrameIdx ++)
            {
                /* Period initialisation and start or stop */
                eStatus = COM_PeriodicStartStop (eMode, uFrameIdx);
            }

            /* Ignore return status of COM_PeriodicStartStop */
            if (eStatus != COM_ERR_OK)
            {
                eStatus = COM_ERR_OK;
            }
        }
        break;

        case COM_MODE_BUS_OFF :
        case COM_MODE_OFF :

            /* COM disabled */
            eStatus = COM_ERR_OFF;

        break;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            eStatus = COM_ERR_COHE;
            break;
    }

    /* Return the status */
    return (eStatus);
}
#endif
/* - Fin COM_Periodic                                                            */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_PeriodicFrame                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: COM periodic transmission or reception monitoring for one frame  */
/*              start or stop and initialisation with first period value         */
/*********************************************************************************/
/* Input parameters          : eMode: periodic mode                              */
/*                             uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_OFF: COM is disabled                      */
/*                             COM_ERR_IDX: frame index is out of range          */
/*                             COM_ERR_FRAME_MODE: frame is in direct mode       */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_PeriodicStartStop                             */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#ifndef BOOTLOADER
tComStatus COM_PeriodicFrame (tComPeriodicMode eMode, tComFrameIdx uFrameIdx)
{
    tComStatus eStatus;
    eStatus = COM_ERR_OK;

    /* Transmission, reception and bus are enabled */
    /* COM current mode */
    switch (m_eComMode)
    {
        case COM_MODE_BUS_ON :
        {
         /* The frame index input parameter is out of range */
            if (uFrameIdx >= COM_FRAME_MAX)
            {
                eStatus = COM_ERR_IDX;
            }

            else
            {
                /* Period initialisation and start or stop */
                eStatus = COM_PeriodicStartStop (eMode, uFrameIdx);
            }
        }

        break;

        case COM_MODE_BUS_OFF :
        case COM_MODE_OFF :

            /* COM disabled */
            eStatus = COM_ERR_OFF;

        break;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
             eStatus = COM_ERR_COHE;
            break;
    }

    /* Return the status */
    return (eStatus);
}
#endif
/* - Fin COM_PeriodicFrame                                                       */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: COM_SendFrame                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Frame in transmission transmission request                       */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_OFF: transmission is not allowed          */
/*                             COM_ERR_IDX: frame index is out of range          */
/*                             COM_ERR_TX_MSG_LOST: transmission already pending */
/*                             COM_ERR_FRAME_MODE: frame is in periodic mode     */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : APP_InterruptDisable                              */
/*                             APP_InterruptEnable                               */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

tComStatus COM_SendFrame (tComFrameIdx uFrameIdx)
{
    tComStatus eStatus;
    eStatus = COM_ERR_OK;

    switch (m_eComMode)
    {
        case COM_MODE_BUS_ON :
        {
            /* Transmission is allowed */

            /* The frame index input parameter is out of range or */
            /* the frame is configured in reception, then error */
            if ((uFrameIdx < COM_RX_FRAME_MAX) || (uFrameIdx >= COM_FRAME_MAX))
            {
                eStatus = COM_ERR_IDX;
            }

            /* The frame is configured in transmission */
            else
            {
                #ifndef BOOTLOADER
                /* The frame is configured in direct or mixed transmission, */
                /* the transmission request is memorised. */
                if ((m_aubComFrameConfig[uFrameIdx] & COM_MODE) != COM_MODE_PERIODIC)
                {
                    /* A transmission is pending for this frame */
                    if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_TX_DATA)
                    {
                        eStatus = COM_ERR_TX_MSG_LOST;
                    }

                    /* No transmission pending */
                    else
                    {
                        /* Application interrupts disabling, to prevent variable reading or writing */
                        /* when reading and writing the variable */
                        u8 ebStatus;
                        ebStatus = APP_InterruptDisable ();

                        /* Flag set to request a transmission */
                        NCS_SET_BIT(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_TX_REQ, u8);

                        /* Application interrupts enabling, the variable is coherent */
                        APP_InterruptEnable (ebStatus);
                    } /* end else (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_TX_DATA) */
                }

                /* The frame mode does not permit to request a transmission */
                else
                {
                    eStatus = COM_ERR_FRAME_MODE;
                }
                #else
                /* The transmission mailbox is busy */
                m_ebTxMailboxStatus[uFrameIdx] = COM_MAILBOX_BUSY;
                
                /* In Bootloader mode, strat the trasmission now (only direct transmission mode is used) */
                COM_TxFrame (uFrameIdx - COM_TX_MAILBOX_BASE_IDX, uFrameIdx);
                #endif
            } /* end else (uFrameIdx >= COM_FRAME_MAX) */
        }

        break;

        case COM_MODE_BUS_OFF :
        case COM_MODE_OFF :

            /* Transmission is not allowed, function ending */
            eStatus = COM_ERR_OFF;

        break;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            /* Transmission is not allowed, function ending */
            eStatus = COM_ERR_COHE;
        break;
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin COM_SendFrame                                                           */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_SetFrameData                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Frame in transmission data update                                */
/*              The data update is realized if a transmission is pending         */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/*                             aubData: COM frame datas                          */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_IDX: frame index is out of range          */
/*                             COM_ERR_TX_MSG_LOST: transmission already pending */
/*                             COM_ERR_NULL: input buffer is null                */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/*                             m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : APP_InterruptDisable                              */
/*                             APP_InterruptEnable                               */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#ifndef BOOTLOADER
tComStatus COM_SetFrameData (tComFrameIdx uFrameIdx, const u8 *aubData)
{
    /* Return status */
    tComStatus eStatus;
    eStatus = COM_ERR_OK;


    /* The frame index input parameter is out of range */
    if (uFrameIdx >= COM_FRAME_MAX)
    {
        eStatus = COM_ERR_IDX;
    }

    /* Input buffer is null, then error */
    else if (aubData == NULL)
    {
        eStatus = COM_ERR_NULL;
    }

    else
    {
        /* Datas copy is possible */
        {
        /* Index for data array */
        u8 ubDataIdx;

        /* Application, CAN and timer interrupts disabling, to prevent buffers */
        /* reading or writing when reading and writing the buffers */
        u8 ebStatus;
        ebStatus = APP_InterruptDisable ();

        /* For frames in reception, frame datas and mirror copy */
#ifdef NCS_SDF_DATA_MIRROR
        if (uFrameIdx < COM_RX_FRAME_MAX)
        {
            for (ubDataIdx = 0; ubDataIdx < COM_DATA_MAX; ubDataIdx ++)
            {
                m_ComaaubData[uFrameIdx][ubDataIdx] = aubData[ubDataIdx];
                m_aaubDataMirror[uFrameIdx][ubDataIdx] = NCS_SDF_GET_U8_MIRROR(aubData[ubDataIdx]);
            }
        }
        /* For frames in transmission, frame datas copy */
        else
        {
            for (ubDataIdx = 0; ubDataIdx < COM_DATA_MAX; ubDataIdx ++)
            {
                m_ComaaubData[uFrameIdx][ubDataIdx] = aubData[ubDataIdx];
            }
        }
#else
        for (ubDataIdx = 0; ubDataIdx < COM_DATA_MAX; ubDataIdx ++)
        {
            m_ComaaubData[uFrameIdx][ubDataIdx] = aubData[ubDataIdx];
        }
#endif
        /* Application, CAN and timer interrupts enabling, the datas are coherents */
        APP_InterruptEnable (ebStatus);
        }


        /* Only for the frames in transmission */
        if (uFrameIdx >= COM_RX_FRAME_MAX)
        {
            /* A transmission is pending for this frame */
            if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_TX_DATA)
            {
                eStatus = COM_ERR_TX_MSG_LOST;
            }

            /* No transmission pending */
            else
            {
                /* Nothing to do */
            }
        }
        /* For the frame in reception */
        else
        {
            /* Nothing to do */
        }
    } /* end else (uFrameIdx >= COM_FRAME_MAX) */


    /* Return the status */
    return (eStatus);
}
#else
tComStatus COM_SetFrameData (tComFrameIdx uFrameIdx, const u8 *aubData)
{
    /* In Bootloader mode, direct copy into the CAN controller mailbox */
    COM_CAN_SET_MAILBOX_DATA ((tCanMailboxIdx)(uFrameIdx), aubData);

    /* Return the status */
    return (COM_ERR_OK);
}
#endif

/* - Fin COM_SetFrameData                                                        */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: COM_GetFrameData                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Reading of frame in reception or transmission data               */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/*                             aubData: COM frame datas                          */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_IDX: frame index is out of range          */
/*                             COM_ERR_RX_MSG_LOST: datas lost                   */
/*                             COM_ERR_NULL: output buffer is null               */
/*                             COM_ERR_BUF: buffer RAM error during reading      */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/*                             m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : APP_InterruptDisable                              */
/*                             APP_InterruptEnable                               */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#ifndef BOOTLOADER
tComStatus COM_GetFrameData (tComFrameIdx uFrameIdx, u8 *aubData)
{
    /* Return status */
    tComStatus eStatus;
#ifdef NCS_SDF_DATA_MIRROR
    u8 ubMirrorData;
#endif

    eStatus = COM_ERR_OK;

    /* The frame index input parameter is out of range */
    if (uFrameIdx >= COM_FRAME_MAX)
    {
        eStatus = COM_ERR_IDX;
    }

    /* Output buffer is null, then error */
    else if (aubData == NULL)
    {
        eStatus = COM_ERR_NULL;
    }

    else
    {
        /* Treatment also done if datas have been lost */
        /* Datas copy with protection against CAN and timer interrupts */
        {
            /* Index for data copy and real frame length */
            u8 ubDataIdx;
            u8 ubLen;
            /* Application, CAN and timer interrupts disabling, to prevent buffers */
            /* reading or writing when reading and writing the buffers */
            u8 ebStatus;
            ebStatus = APP_InterruptDisable ();

#if ((COM_EB_CAN_TYPE == COM_EB_PSA_CAN_LS_2010) || (COM_EB_CAN_TYPE == COM_EB_PSA_CAN_HS_IS_2010) || (COM_EB_CAN_TYPE == COM_EB_PSA_CAN_HS_M_2010))
            /* Reception frame */
            if (uFrameIdx < COM_RX_FRAME_MAX)
            {
                ubLen = m_aubRxFrameLen[uFrameIdx];
            }
            /* Transmission frame */
            else
            {
                ubLen = m_aubComFrameLenNominal[uFrameIdx];
            }
#endif

#if ((COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS)  || (COM_EB_CAN_TYPE == COM_EB_JAC_CAN_HS) || (COM_EB_CAN_TYPE == COM_EB_FIAT_CAN) || (COM_EB_CAN_TYPE == COM_EB_VW_CAN_HS))
            /* Reception frame */
            if (uFrameIdx < COM_RX_FRAME_MAX)
            {
                if ((m_aubComFrameConfig[uFrameIdx] & COM_LENGTH_TYPE) == COM_LENGTH_DYNAMIC)
                {
                    ubLen = m_aubRxFrameLen[uFrameIdx];
                }
                else
                {
                    ubLen = m_aubComFrameLenNominal[uFrameIdx];
                }
            }
            /* Transmission frame */
            else
            {
                ubLen = m_aubComFrameLenNominal[uFrameIdx];
            }
#endif

            /* For frames in reception, frame datas and mirror copy */
            if (uFrameIdx < COM_RX_FRAME_MAX)
            {
                /* The frame is configured in reception, datas copy is possible */
                /* Data has been updated more than one time since last reading */
                if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_RX_LOST_FLAG)
                {
                    /* Return status */
                    eStatus = COM_ERR_RX_MSG_LOST;
                }
                else
                {
                    /* Nothing to do */
                }

                /* The datas have been read, then flags reset */
                NCS_RESET_BIT(m_aubStatus[uFrameIdx], (COM_FRAME_STATUS_RX_FLAG
                                                     | COM_FRAME_STATUS_RX_LOST_FLAG), u8);

                for (ubDataIdx = 0; ubDataIdx < ubLen; ubDataIdx ++)
                {
                    #ifdef NCS_SDF_DATA_MIRROR
                    /* Data byte and mirror are consistent, then copy */
                    ubMirrorData = NCS_SDF_GET_U8_MIRROR(m_aaubDataMirror[uFrameIdx][ubDataIdx]);
                    if(m_ComaaubData[uFrameIdx][ubDataIdx] == ubMirrorData)
                    {
                        aubData[ubDataIdx] = m_ComaaubData[uFrameIdx][ubDataIdx];
                    }
                    else
                    {
                        eStatus = COM_ERR_BUF;
                    }
                    #else
                        aubData[ubDataIdx] = m_ComaaubData[uFrameIdx][ubDataIdx];
                    #endif
                }
            }
            /* For frames in transmission, frame datas copy */
            else
            {
                for (ubDataIdx = 0; ubDataIdx < ubLen; ubDataIdx ++)
                {
                    aubData[ubDataIdx] = m_ComaaubData[uFrameIdx][ubDataIdx];
                }
            }

            /* Application, CAN and timer interrupts enabling, the datas are coherents */
            APP_InterruptEnable (ebStatus);
        }
    } /* end else (uFrameIdx >= COM_FRAME_MAX) */


    /* Return the status */
    return (eStatus);
}
#else
tComStatus COM_GetFrameData (tComFrameIdx uFrameIdx, u8 *aubData)
{

    /* Index for data copy and real frame length */
    u8 ubDataIdx;
    u8 ubLen;
    /* Application, CAN and timer interrupts disabling, to prevent buffers */
    /* reading or writing when reading and writing the buffers */
    u8 ebStatus;

    
#if ((COM_EB_CAN_TYPE == COM_EB_PSA_CAN_LS_2010) || (COM_EB_CAN_TYPE == COM_EB_PSA_CAN_HS_IS_2010) || (COM_EB_CAN_TYPE == COM_EB_PSA_CAN_HS_M_2010))
    /* Reception frame */
    if (uFrameIdx < COM_RX_FRAME_MAX)
    {
        ubLen = m_aubRxFrameLen[uFrameIdx];

        ebStatus = APP_InterruptDisable ();
        

        for (ubDataIdx = 0; ubDataIdx < ubLen; ubDataIdx ++)
        {
            aubData[ubDataIdx] = m_ComaaubData[uFrameIdx][ubDataIdx];
        }
        
        /* Application, CAN and timer interrupts enabling, the datas are coherents */
        APP_InterruptEnable (ebStatus);
    }
    /* Transmission frame */
    else
    {
        COM_CAN_GET_MAILBOX_DATA((tCanMailboxIdx)(uFrameIdx), aubData);
    }
#endif
    
#if ((COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS)  || (COM_EB_CAN_TYPE == COM_EB_JAC_CAN_HS) || (COM_EB_CAN_TYPE == COM_EB_FIAT_CAN) || (COM_EB_CAN_TYPE == COM_EB_VW_CAN_HS))
    /* Reception frame */
    if (uFrameIdx < COM_RX_FRAME_MAX)
    {
        if ((m_aubComFrameConfig[uFrameIdx] & COM_LENGTH_TYPE) == COM_LENGTH_DYNAMIC)
        {
            ubLen = m_aubRxFrameLen[uFrameIdx];
        }
        else
        {
            ubLen = m_aubComFrameLenNominal[uFrameIdx];
        }
        
        ebStatus = APP_InterruptDisable ();        

        for (ubDataIdx = 0; ubDataIdx < ubLen; ubDataIdx ++)
        {
            aubData[ubDataIdx] = m_ComaaubData[uFrameIdx][ubDataIdx];
        }
        
        /* Application, CAN and timer interrupts enabling, the datas are coherents */
        APP_InterruptEnable (ebStatus);    
    }
    /* Transmission frame */
    else
    {
        COM_CAN_GET_MAILBOX_DATA((tCanMailboxIdx)(uFrameIdx), aubData);
    }
#endif

    /* Return the status */
    return (COM_ERR_OK);
}
#endif

/* - Fin COM_GetFrameData                                                        */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_GetFrameDataWithMirror                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Reading of frame in reception or transmission data               */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/*                             aubData: COM frame datas                          */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_IDX: frame index is out of range          */
/*                             COM_ERR_RX_MSG_LOST: datas lost                   */
/*                             COM_ERR_NULL: output buffer is null               */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : APP_InterruptDisable                              */
/*                             APP_InterruptEnable                               */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#ifndef BOOTLOADER
#ifdef NCS_SDF_DATA_MIRROR
tComStatus COM_GetFrameDataWithMirror (tComFrameIdx uFrameIdx, u8 *aubData, u8 *aubMirror)
{
    /* Return status */
    tComStatus eStatus;
    eStatus = COM_ERR_OK;


    /* The frame index input parameter is out of range */
    if (uFrameIdx >= COM_FRAME_MAX)
    {
        eStatus = COM_ERR_IDX;
    }

    /* Output buffer is null, then error */
    else if (aubData == NULL)
    {
        eStatus = COM_ERR_NULL;
    }

    /* Output buffer is null, then error */
    else if (aubMirror == NULL)
    {
        eStatus = COM_ERR_NULL;
    }

    else
    {
        /* Treatment also done if datas have been lost */
        /* Datas copy with protection against CAN and timer interrupts */
        {
            u8 ubLen;
            /* Index for data copy and real frame length */
            u8 ubDataIdx;
            /* Application, CAN and timer interrupts disabling, to prevent buffers */
            /* reading or writing when reading and writing the buffers */
            u8 ebStatus;
            ebStatus = APP_InterruptDisable ();

            /* Reception frame */
            if (uFrameIdx < COM_RX_FRAME_MAX)
            {
                ubLen = m_aubRxFrameLen[uFrameIdx];
            }
            /* Transmission frame */
            else
            {
                ubLen = m_aubComFrameLenNominal[uFrameIdx];
            }

            /* For frames in reception, frame datas and mirror copy */
            if (uFrameIdx < COM_RX_FRAME_MAX)
            {
                /* The frame is configured in reception, datas copy is possible */
                /* Data has been updated more than one time since last reading */
                if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_RX_LOST_FLAG)
                {
                    /* Return status */
                    eStatus = COM_ERR_RX_MSG_LOST;
                }
                else
                {
                    /* Nothing to do */
                }

                /* The datas have been read, then flags reset */
                NCS_RESET_BIT(m_aubStatus[uFrameIdx], (COM_FRAME_STATUS_RX_FLAG
                                                     | COM_FRAME_STATUS_RX_LOST_FLAG), u8);

                for (ubDataIdx = 0; ubDataIdx < ubLen; ubDataIdx ++)
                {
                    aubData[ubDataIdx] = m_ComaaubData[uFrameIdx][ubDataIdx];
                    aubMirror[ubDataIdx] = m_aaubDataMirror[uFrameIdx][ubDataIdx];
                }
            }
            /* For frames in transmission, frame datas copy */
            else
            {
                for (ubDataIdx = 0; ubDataIdx < ubLen; ubDataIdx ++)
                {
                    aubData[ubDataIdx] = m_ComaaubData[uFrameIdx][ubDataIdx];
                }
            }

            /* Application, CAN and timer interrupts enabling, the datas are coherents */
            APP_InterruptEnable (ebStatus);
        }
    } /* end else (uFrameIdx >= COM_FRAME_MAX) */


    /* Return the status */
    return (eStatus);
}
#endif
#endif
/* - Fin COM_GetFrameDataWithMirror                                              */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: COM_GetFrameRxFlag                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Reading of the reception flag of a reception frame               */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/* Output parameters         : pebRxFlag: COM frame reception flag               */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_OFF: reception is not allowed             */
/*                             COM_ERR_IDX: frame index is out of range          */
/*                             COM_ERR_NULL: output flag is null                 */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

tComStatus COM_GetFrameRxFlag (tComFrameIdx uFrameIdx, tComBoolean *pebRxFlag)
{
    tComStatus eStatus;
    eStatus = COM_ERR_OK;

    switch (m_eComMode)
    {
        case COM_MODE_BUS_ON :
        case COM_MODE_BUS_OFF :
        {
            /* Reception is active */

            /* The frame index input parameter is out of range or */
            /* the frame is configured in transmission, no datas copy */
            if (uFrameIdx >= COM_RX_FRAME_MAX)
            {
                eStatus = COM_ERR_IDX;
            }

            /* Output flag is null, then error */
            else if (pebRxFlag == NULL)
            {
                eStatus = COM_ERR_NULL;
            }

            else
            {
                /* Return the frame reception flag status */
                if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_RX_FLAG)
                {
                    *pebRxFlag = COM_TRUE;
                }
                else
                {
                    *pebRxFlag = COM_FALSE;
                }
            } /* end else (uFrameIdx >= COM_RX_FRAME_MAX) */
        }
        break;

        case COM_MODE_OFF :

            eStatus = COM_ERR_OFF;

        break;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            eStatus = COM_ERR_COHE;
            break;
    }

    /* Return the status */
    return (eStatus);
}

/* - Fin COM_GetFrameRxFlag                                                      */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: COM_MonNodeInit                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Transmission and reception monitoring initialisation             */
/*********************************************************************************/
/* Input parameters          : uNodeIdx: COM monitored node index                */
/*                             eNodeStatus: node status                          */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_NODE: node index is out of range          */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_astComMonNode: COM monitored nodes              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_MonNodeCntInit                                */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#ifndef BOOTLOADER
tComStatus COM_MonNodeInit (tComMonNodeIdx uNodeIdx, tComMonStatus eNodeStatus)
{
    /* Return status */
    tComStatus eStatus;
    eStatus = COM_ERR_OK;


    /* Node index is in the correct range */
    if (uNodeIdx < COM_MON_NODE_MAX)
    {

        /* Node status initialisation */
        m_aeComMonOldStatus[uNodeIdx] = eNodeStatus;

        /* Filtering re-initialisation */
        COM_MonNodeCntInit (uNodeIdx, eNodeStatus);

        /* Indication memorisation initialisation */
        m_eComNodeConfStatus[uNodeIdx] = eNodeStatus;
    }

    /* Node index is out of range */
    else
    {
        eStatus = COM_ERR_NODE;
    }


    /* Return the status */
    return (eStatus);
}
#endif

/* - Fin COM_MonNodeInit                                                         */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_SetDynamicFrameLen                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Frame in transmission dynamic length update                      */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/*                             ubLen: COM frame length                           */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_IDX: frame index is out of range          */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_aubDynamicFrameLen: COM Tx frames length        */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#if (COM_TX_DYNAMIC_FRAME_MAX_RAM)
tComStatus COM_SetDynamicFrameLen (tComFrameIdx uFrameIdx, u8 ubLen)
{
    /* Return status */
    tComStatus eStatus;
    eStatus = COM_ERR_OK;


    /* The frame index input parameter is out of range (static frame) or */
    /* the frame is configured in reception, then error */
    if ((uFrameIdx < COM_RX_FRAME_MAX) || (uFrameIdx >= (COM_RX_FRAME_MAX + COM_TX_DYNAMIC_FRAME_MAX)))
    {
        eStatus = COM_ERR_IDX;
    }

    else
    {
        /* The dynamic frame length is memorized */
        m_aubDynamicFrameLen[uFrameIdx - COM_RX_FRAME_MAX] = ubLen;
    } /* end else ((uFrameIdx < COM_RX_FRAME_MAX) || ... */


    /* Return the status */
    return (eStatus);
}
#endif

/* - Fin COM_SetDynamicFrameLen                                                  */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_GetLastRxFrameLen                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Frame in reception length reading                                */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/* Output parameters         : pubLen: COM frame length                          */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_IDX: frame index is out of range          */
/*                             COM_ERR_NULL: output length is null               */
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
tComStatus COM_GetLastRxFrameLen (tComFrameIdx uFrameIdx, u8 *pubLen)
{
    /* Return status */
    tComStatus eStatus;
    eStatus = COM_ERR_OK;


    /* The frame index input parameter is out of range (static frame) or */
    /* the frame is configured in reception, then error */
    if (uFrameIdx >= COM_RX_FRAME_MAX)
    {
        eStatus = COM_ERR_IDX;
    }

    /* Output length is null, then error */
    else if (pubLen == NULL)
    {
        eStatus = COM_ERR_NULL;
    }

    else
    {
        /* The dynamic frame length is memorized */
        *pubLen = m_aubRxFrameLen[uFrameIdx];
    } /* end else (uFrameIdx >= COM_FRAME_MAX) */


    /* Return the status */
    return (eStatus);
}

/* - Fin COM_GetLastRxFrameLen                                                   */
/*-------------------------------------------------------------------------------*/




/*-------------------------------------------------------------------------------*/
/* - Private Functions                                                           */

/*********************************************************************************/
/* Name: COM_Activate                                                            */
/*-------------------------------------------------------------------------------*/
/* Description: COM activation                                                   */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*                             m_uTxFrameIdx: COM frame index, Tx in progress    */
/*                             m_ebTxMailboxStatus: transmission mailbox status  */
/*                             m_stTxFrameIdxFifo: transmission FIFO             */
/*                             m_uTxTimeoutCnt: timeout in transmission          */
/*-------------------------------------------------------------------------------*/
/* Called functions          :                                                   */
/* Called macros             : COM_CAN_DESACTIVATE                               */
/*                             COM_CAN_SET_MAILBOX_MODE                          */
/*                             COM_CAN_SET_MAILBOX_STD_MASK                      */
/*                             COM_CAN_SET_MAILBOX_EXT_MASK                      */
/*                             COM_CAN_SET_MAILBOX_IDE                           */
/*                             COM_CAN_SET_MAILBOX_RTR                           */
/*                             COM_CAN_SET_MAILBOX_STD_IDENT                     */
/*                             COM_CAN_SET_MAILBOX_EXT_IDENT                     */
/*                             COM_CAN_SET_MAILBOX_INTERRUPT_CFG                 */
/*                             COM_CAN_ACTIVATE                                  */
/*                             COM_CAN_INTERRUPT_LEVEL_CFG                       */
/*                             COM_CAN_INTERRUPT_ENABLE                          */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT void COM_Activate (void)
{
    /* Frame variables initialisation */
    {
        /* Index for frame array */
        tComFrameIdx uFrameIdx;

        /* Protection of the write access on the array m_aubStatus[]*/
        u8 ebStatus;
        ebStatus = APP_InterruptDisable ();


        /* Frame variables initialisation */
        for (uFrameIdx = 0; uFrameIdx < COM_FRAME_MAX; uFrameIdx ++)
        {
            /* The transmission status flags are reset */
            NCS_RESET_BIT(m_aubStatus[uFrameIdx], (COM_FRAME_STATUS_TX_REQ
                                                 | COM_FRAME_STATUS_TX_PROGRESS
                                                 | COM_FRAME_STATUS_TX_DATA
                                                 | COM_FRAME_STATUS_TX_CONF), u8);
        }

        APP_InterruptEnable(ebStatus);
        /* End Protection of the write access on the array m_aubStatus[]*/


        /* Exclusion time is initialised only from COM_MODE_OFF */
        /* When the ECU enters in BUS-OFF, the exclusion time must be managed */
        switch (m_eComMode)
        {
            case COM_MODE_BUS_ON :
            case COM_MODE_BUS_OFF :

                /* Nothing to do */

            break;

            case COM_MODE_OFF :
                #ifndef BOOTLOADER
                /* Frame variables initialisation */
                for (uFrameIdx = 0; uFrameIdx < COM_FRAME_MAX; uFrameIdx ++)
                {
                    m_auComFrameExTimeCnt[uFrameIdx] = 0;
                }
                #endif
            break;

            default :
                m_eComMode = m_eComModeDefaultValue;
                NCS_SdfVariableRangeError (SDF_COM_MODE);
                break;
        }
    } /* end of frame variables initialisation */


    /* Transmission variables initialisation */
    /* FIFO empty and transmission mailbox free */
    {
        /* Index for protocol handler transmission mailboxes used by COM */
        tCanMailboxIdx uMailboxIdx;

        for (uMailboxIdx = 0; uMailboxIdx < COM_TX_MAILBOX_NB; uMailboxIdx ++)
        {
            m_ebTxMailboxStatus[uMailboxIdx] = COM_MAILBOX_FREE;
            m_uTxFrameIdx[uMailboxIdx] = 0;
            m_uTxTimeoutCnt[uMailboxIdx] = 0;
        }
        #ifndef BOOTLOADER
        m_stTxFrameIdxFifo.uWrIdx = 0;
        m_stTxFrameIdxFifo.uRdIdx = 0;
        #endif
    }


    /* Bus initialisation */
    {
        /* Index for protocol handler mailboxes */
        tCanMailboxIdx uMailboxIdx;
        u8 uNbRxMbx;
        u8 uRxMbxExtIdx;

        uNbRxMbx = COM_RX_MAILBOX_NB;
        uRxMbxExtIdx = COM_RX_MAILBOX_EXT_IDX;

        /* CAN bus disabling for configuration mode */
        COM_CAN_DESACTIVATE;

        /* Protocol handler reception mailboxes configuration */
        /* only done if the CAN protocol handler hardware */
        /* does not impose the number of reception mailboxes */
        if (uNbRxMbx > 0)
        {
            for (uMailboxIdx = COM_RX_MAILBOX_BASE_IDX; uMailboxIdx < COM_RX_MAILBOX_BASE_IDX + COM_RX_MAILBOX_NB; uMailboxIdx ++)
            {
                /* Configuration only done if the CAN protocol handler hardware */
                /* does not impose the number of reception mailboxes */
                #if (COM_CAN_RX_MAILBOX_NB == 0U)
                    COM_CAN_SET_MAILBOX_MODE (uMailboxIdx, CAN_MAILBOX_MODE_RX);
                #endif

          #if (COM_FILTERING_METHOD == COM_LINEAR_FILTERING)
                    /* Configuration only done if the CAN protocol handler mailboxes have an individual mask */
                #ifdef CAN_MAILBOX_INDIVIDUAL_MASK
                    #if (COM_EXTENDED_ID == COM_TRUE)
                        COM_CAN_SET_MAILBOX_EXT_MASK (uMailboxIdx, CAN_MASK_UNIQUE_IDENT);
                    #else
                        COM_CAN_SET_MAILBOX_STD_MASK (uMailboxIdx, CAN_MASK_UNIQUE_IDENT);
                    #endif
                #endif
          #else
                /* the identifiers are configured in the CAN driver */
          #endif

                #if (COM_EXTENDED_ID == COM_TRUE)
                    COM_CAN_SET_MAILBOX_IDE (uMailboxIdx, CAN_IDENT_EXT);
                #else
                    COM_CAN_SET_MAILBOX_IDE (uMailboxIdx, CAN_IDENT_STD);
                #endif

                COM_CAN_SET_MAILBOX_RTR (uMailboxIdx, CAN_FRAME_TYPE_DATA);

                /* the identifiers are configured in the CAN driver if the filtering method is not LINEAR_FILTERING */
          #if (COM_FILTERING_METHOD == COM_LINEAR_FILTERING)
            #if (COM_EXTENDED_ID == COM_TRUE)
                COM_CAN_SET_MAILBOX_EXT_IDENT (uMailboxIdx, m_auwComFrameIdent[(tComFrameIdx)(uMailboxIdx)]);
            #else
                COM_CAN_SET_MAILBOX_STD_IDENT (uMailboxIdx, m_auwComFrameIdent[(tComFrameIdx)(uMailboxIdx)]);
            #endif
          #endif
                COM_CAN_SET_MAILBOX_INTERRUPT_CFG (uMailboxIdx, CAN_INTERRUPT_RX_OK);
                COM_CAN_MAILBOX_INTERRUPT_ENABLE (uMailboxIdx);
            }

            /* the identifiers are configured in the CAN driver if the filtering method is not LINEAR_FILTERING */
          #if (COM_FILTERING_METHOD == COM_LINEAR_FILTERING)
            /* Protocol handler reception extension mailbox configuration, */
            /* if one is needed for COM */
            if (uRxMbxExtIdx != COM_RX_MAILBOX_EXT_NONE)
            {
                /* Configuration only done if the CAN protocol handler hardware */
                /* does not impose the number of reception mailboxes */
                #if (COM_CAN_RX_MAILBOX_NB == 0U)
                    COM_CAN_SET_MAILBOX_MODE (COM_RX_MAILBOX_EXT_IDX, CAN_MAILBOX_MODE_RX_EXT);
                #endif

                /* Configuration only done if the CAN protocol handler mailboxes have an individual mask */
            #ifdef CAN_MAILBOX_INDIVIDUAL_MASK
                #if (COM_EXTENDED_ID == COM_TRUE)
                    COM_CAN_SET_MAILBOX_EXT_MASK (COM_RX_MAILBOX_EXT_IDX, CAN_MASK_ALL_IDENT);
                #else
                    COM_CAN_SET_MAILBOX_STD_MASK (COM_RX_MAILBOX_EXT_IDX, CAN_MASK_ALL_IDENT);
                #endif
            #endif

            #if (COM_EXTENDED_ID == COM_TRUE)
                COM_CAN_SET_MAILBOX_IDE (COM_RX_MAILBOX_EXT_IDX, CAN_IDENT_EXT);
            #else
                COM_CAN_SET_MAILBOX_IDE (COM_RX_MAILBOX_EXT_IDX, CAN_IDENT_STD);
            #endif
                COM_CAN_SET_MAILBOX_RTR (COM_RX_MAILBOX_EXT_IDX, CAN_FRAME_TYPE_DATA);
            #if (COM_EXTENDED_ID == COM_TRUE)
                COM_CAN_SET_MAILBOX_EXT_IDENT (COM_RX_MAILBOX_EXT_IDX, CAN_MASK_ALL_IDENT);
            #else
                COM_CAN_SET_MAILBOX_STD_IDENT (COM_RX_MAILBOX_EXT_IDX, CAN_MASK_ALL_IDENT);
            #endif
                COM_CAN_SET_MAILBOX_INTERRUPT_CFG (COM_RX_MAILBOX_EXT_IDX, CAN_INTERRUPT_RX_OK);
                COM_CAN_MAILBOX_INTERRUPT_ENABLE (COM_RX_MAILBOX_EXT_IDX);
            }
          #endif /* end #if (COM_FILTERING_METHOD == COM_LINEAR_FILTERING) */
        }

        /* Protocol handler transmission mailboxes configuration */
        for (uMailboxIdx = COM_TX_MAILBOX_BASE_IDX; ((uMailboxIdx < COM_TX_MAILBOX_BASE_IDX + COM_TX_MAILBOX_NB)); uMailboxIdx ++)
        {
            /* Configuration only done if the CAN protocol handler hardware */
            /* does not impose the number of transmission mailboxes */
            #if (COM_CAN_TX_MAILBOX_NB == 0U)
                COM_CAN_SET_MAILBOX_MODE (uMailboxIdx, CAN_MAILBOX_MODE_TX);
            #endif

            #if (COM_EXTENDED_ID == COM_TRUE)
                COM_CAN_SET_MAILBOX_IDE (uMailboxIdx, CAN_IDENT_EXT);
            #else
                COM_CAN_SET_MAILBOX_IDE (uMailboxIdx, CAN_IDENT_STD);
            #endif
            COM_CAN_SET_MAILBOX_INTERRUPT_CFG (uMailboxIdx, CAN_INTERRUPT_TX_OK);
            COM_CAN_MAILBOX_INTERRUPT_ENABLE (uMailboxIdx);
        }


        /* CAN general acceptance configuration */
    #if (COM_EXTENDED_ID == COM_TRUE)
        COM_CAN_SET_GENERAL_EXT_MASK (CAN_ACCEPT_MASK_ALL_EXT_IDENT);
    #else
        COM_CAN_SET_GENERAL_STD_MASK (CAN_MASK_ALL_IDENT);
    #endif

        /* CAN general interrupt configuration and enabling */
        COM_CAN_ACTIVATE;
      //  COM_CAN_SET_GENERAL_INTERRUPT_CFG (CAN_INTERRUPT_TX_OK | CAN_INTERRUPT_RX_OK | CAN_INTERRUPT_BUS_OFF | CAN_INTERRUPT_WAKE_UP);
      //  COM_CAN_INTERRUPT_LEVEL_CFG;
        COM_CAN_INTERRUPT_ENABLE;
    } /* end of bus initialisation */
}

/* - Fin COM_Activate                                                            */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_Desactivate                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: COM desactivation                                                */
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
/* Called macros             : COM_CAN_DESACTIVATE                            */
/*                             COM_CAN_INTERRUPT_DISABLE                      */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT void COM_Desactivate (void)
{
    /* CAN interrupt disabling */
    /* Only done if CAN WAKE-UP is not detected by the protocol handler */
    #if ( CAN_WAKE_UP_DETECTION == CAN_WAKE_UP_DESACTIVATE )
        COM_CAN_DESACTIVATE;
        COM_CAN_INTERRUPT_DISABLE;
    #endif
}

/* - Fin COM_Desactivate                                                         */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_PeriodicStartStop                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: COM periodic transmission or reception monitoring for one frame  */
/*              start or stop and initialisation with first period value         */
/*********************************************************************************/
/* Input parameters          : eMode: periodic mode                              */
/*                             uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
/* Return value              : eStatus: function status                          */
/*                             COM_ERR_OK: no error                              */
/*                             COM_ERR_FRAME_MODE: frame is in direct mode       */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : APP_InterruptDisable                              */
/*                             APP_InterruptEnable                               */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#ifndef BOOTLOADER
NCS_PRI_FCT tComStatus COM_PeriodicStartStop (tComPeriodicMode eMode, tComFrameIdx uFrameIdx)
{
    /* Return status */
    tComStatus eStatus;
    u8 uNodeIdx;
    eStatus = COM_ERR_OK;
    uNodeIdx = 0;

    /* The frame is periodic or mixed, period initialisation and start or stop */
    if ((m_aubComFrameConfig[uFrameIdx] & COM_MODE) != COM_MODE_DIRECT)
    {
        /* Period initialisation with first period */
        /*+1 is add to FrameFirst period to correct a schift on first transmission period */
        m_auComFramePeriodCnt[uFrameIdx] = (tComPeriod)(m_auComFrameFirstPeriod[uFrameIdx] +1) ;


        /* Find the corresponding Node of the frame */
        for(uNodeIdx = 0; uNodeIdx<COM_MON_NODE_MAX;uNodeIdx++)
        {
            if(m_auComMonFrameIdx[uNodeIdx] == uFrameIdx)
            {
                /* Set the counter to the initial value */
                m_aubComMonFilterCnt[uNodeIdx] = m_aubComMonFilterNok[uNodeIdx];
            }
            else
            {
                /* Nothing to do */
            }
        }

        {
            /* Application interrupts disabling, to prevent variable reading or writing */
            /* when reading and writing the variable */
            u8 ebStatus;
            ebStatus = APP_InterruptDisable ();

            /* Periodic management start */
            if (eMode == COM_PERIODIC_START)
            {
                NCS_SET_BIT(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_PERIOD_EN, u8);
            }
            /* Periodic management stop */
            else
            {
                NCS_RESET_BIT(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_PERIOD_EN, u8);
            }

            /* Application interrupts enabling, the variable is coherent */
            APP_InterruptEnable (ebStatus);
        }
    }

    /* The frame is direct, then error */
    else
    {
        eStatus = COM_ERR_FRAME_MODE;
    }


    /* Return the status */
    return (eStatus);
}
#endif

/* - Fin COM_PeriodicStartStop                                                   */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_FindRxFrameIdxFromIdent                                             */
/*-------------------------------------------------------------------------------*/
/* Description: Research in COM reception frames of the frame index that         */
/*              corresponds to the received identifier                           */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: received frame mailbox index         */
/* Output parameters         : None                                              */
/* Return value              : uFrameIdx: COM frame index                        */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astComFrameCfg: COM frames config               */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : CAN_GetMailboxStdIdent                            */
/*                             CAN_GetMailboxExtIdent                            */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#if (COM_FILTERING_METHOD == COM_LINEAR_FILTERING)
NCS_PRI_FCT tComFrameIdx COM_FindRxFrameIdxFromIdent (tComFrameIdx uExTFrameIdx, tCanMailboxIdx uMailboxIdx)
{
    tComFrameIdx uFrameIdx;
    /* Received frame identifier reading */
    #if (COM_EXTENDED_ID == COM_TRUE)
        u32 uwIdent;
        uwIdent = COM_CAN_GET_MAILBOX_EXT_IDENT (uMailboxIdx);
    #else
        u16 uwIdent;
        uwIdent = COM_CAN_GET_MAILBOX_STD_IDENT (uMailboxIdx);
    #endif

    /* Frame index that corresponds to the received identifier */
    uFrameIdx = uExTFrameIdx;

    /* Search the frame index which corresponds to the received identifier */
    /* in the data frames in reception mode */
    while (uFrameIdx < COM_RX_FRAME_MAX)
    {
        /* The identifier exists in COM frames */
        if (m_auwComFrameIdent[uFrameIdx] == uwIdent)
        {
            break;
        }
        else
        {
            /* Next frame */
            uFrameIdx ++;
        }
    }

    /* Function result return */
    return (uFrameIdx);
}
#endif

/* - Fin COM_FindRxFrameIdxFromIdent                                             */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_TxFrame                                                             */
/*-------------------------------------------------------------------------------*/
/* Description: Send a frame directly via the protocol handler                   */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                             uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*                             m_uTxFrameIdx: COM frame index, Tx in progress    */
/*                             m_uTxTimeoutCnt: timeout in transmission          */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : COM_CAN_SET_MAILBOX_STD_IDENT                     */
/*                             COM_CAN_SET_MAILBOX_EXT_IDENT                     */
/*                             COM_CAN_SET_MAILBOX_LEN                           */
/*                             COM_CAN_SET_MAILBOX_RTR                           */
/*                             COM_CAN_SET_MAILBOX_DATA                          */
/*                             COM_CAN_TRANSMIT_FRAME                            */
/* Called callbacks          : COM_TxFrameInd                                    */
/*********************************************************************************/

NCS_PRI_FCT void COM_TxFrame (tCanMailboxIdx uMailboxIdx, tComFrameIdx uFrameIdx)
{
    /* Callback called before frame transmission to allow data update */
    COM_TxFrameInd (uFrameIdx);


    /* Frame transmission : mailbox filling with identifier, length, RTR and datas */
    /* Then, transmission request */
    #if (COM_EXTENDED_ID == COM_TRUE)
        COM_CAN_SET_MAILBOX_EXT_IDENT ((tCanMailboxIdx)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX), m_auwComFrameIdent[uFrameIdx]);
    #else
        COM_CAN_SET_MAILBOX_STD_IDENT ((tCanMailboxIdx)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX), m_auwComFrameIdent[uFrameIdx]);
    #endif

    /* If dynamic frames in transmission are defined, */
    /* distinction between static and dynamic frame length */
    #if (COM_TX_DYNAMIC_FRAME_MAX_RAM)
        /* For a dynamic frame length, get the real frame length */
        if ((m_aubComFrameConfig[uFrameIdx] & COM_LENGTH_TYPE) == COM_LENGTH_DYNAMIC)
        {
            COM_CAN_SET_MAILBOX_LEN ((tCanMailboxIdx)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX), m_aubDynamicFrameLen[uFrameIdx - COM_RX_FRAME_MAX]);
        }
        /* For a static frame length, use the configured frame length */
        else
        {
            COM_CAN_SET_MAILBOX_LEN ((tCanMailboxIdx)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX), m_aubComFrameLenNominal[uFrameIdx]);
        }
    /* If no dynamic frames in transmission are defined, */
    /* use only the frame length configuration stored in FLASH */
    #else
        // COM_CAN_SET_MAILBOX_LEN ((tCanMailboxIdx)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX), m_aubComFrameLenNominal[uFrameIdx]);
        // zhaoziyi set data Length
        // can_mailbox_receive_data_read(CAN0, (uint32_t)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX), &g_transmit_message);
        g_transmit_message.data_bytes = (uint32_t)m_aubComFrameLenNominal[uFrameIdx];

    #endif
    /* RTR */
        COM_CAN_SET_MAILBOX_RTR ((tCanMailboxIdx)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX), CAN_FRAME_TYPE_DATA);

    
#ifndef BOOTLOADER
    /* Datas copy */
    {
        /* Application interrupts disabling, to prevent variable reading or writing */
        /* when reading and writing the variable */
        u8 ebStatus;
        ebStatus = APP_InterruptDisable ();

        /* Datas copy */
        // COM_CAN_SET_MAILBOX_DATA ((tCanMailboxIdx)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX), m_ComaaubData[uFrameIdx]);
        // zhaoziyi 
        g_transmit_message.data = (uint32_t *)m_ComaaubData[uFrameIdx];

        /* The frame transmission is memorised */
        NCS_SET_BIT(m_aubStatus[uFrameIdx], (COM_FRAME_STATUS_TX_DATA), u8);

        /* Application interrupts enabling, the variable is coherent */
        APP_InterruptEnable (ebStatus);
    }
#endif

    /* Timeout in transmission is initialised */
    m_uTxTimeoutCnt[uMailboxIdx] = m_auComFrameTxTimeout[uFrameIdx];

    /* The frame for which transmission is in progress is memorised */
    m_uTxFrameIdx[uMailboxIdx] = uFrameIdx;

    /* Tranmsission request */
    // COM_CAN_TRANSMIT_FRAME ((tCanMailboxIdx)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX));
    // zhaoziyi add code for Transmit
    g_transmit_message.code = CAN_MB_TX_STATUS_DATA;
    g_transmit_message.id = 0x778;
    can_mailbox_config(CAN0, (uint32_t)(uMailboxIdx + COM_TX_MAILBOX_BASE_IDX), &g_transmit_message);

}

/* - Fin COM_TxFrame                                                             */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: COM_TransmissionEnding                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Reset of transmission flags and counters after a transmission    */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#ifndef BOOTLOADER
NCS_PRI_FCT void COM_TransmissionEnding (tComFrameIdx uFrameIdx)
{
    /* The frame transmission is not in progress : */
    /*     - the transmission request flag is cleared */
    /*     - the transmission in progress flag is cleared */
    /*     - the exclusion time is initialised */

    /* Application interrupts disabling, to prevent variable reading or writing */
    /* when reading and writing the variable */
    u8 ebStatus;
    ebStatus = APP_InterruptDisable ();

    /* Flags modification */
    NCS_RESET_BIT(m_aubStatus[uFrameIdx], (COM_FRAME_STATUS_TX_REQ
                                         | COM_FRAME_STATUS_TX_PROGRESS
                                         | COM_FRAME_STATUS_TX_DATA), u8);
    NCS_SET_BIT(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_TX_CONF, u8);

    /* Application interrupts enabling, the variable is coherent */
    APP_InterruptEnable (ebStatus);
}
#endif

/* - Fin COM_TransmissionEnding                                                  */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_TxNextFrame                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Send the next frame in the transmission FIFO (if exists)         */
/*              directly via the protocol handler                                */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*                             m_uTxFrameIdx: COM frame index, Tx in progress    */
/*                             m_ebTxMailboxStatus: transmission mailbox status  */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_TransmissionEnding                            */
/*                             COM_TxFrame                                       */
/* Called macros             : _COM_TxFifoRdIndInc                               */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#ifndef BOOTLOADER
NCS_PRI_FCT void COM_TxNextFrame (tCanMailboxIdx uMailboxIdx)
{
    /* The frame m_uTxFrameIdx is transmitted without error */
    COM_TransmissionEnding (m_uTxFrameIdx[uMailboxIdx]);


    /* Next FIFO frame transmission, if exists (FIFO is not empty) */
    if (! _COM_TxFifoEmpty(m_stTxFrameIdxFifo))
    {
        /* Frame transmission */
        COM_TxFrame (uMailboxIdx, m_stTxFrameIdxFifo.auFrameIdx[m_stTxFrameIdxFifo.uRdIdx]);


        /* The frame is considered transmitted, so it is removed from FIFO */
        _COM_TxFifoRdIndInc(m_stTxFrameIdxFifo);
    } /* end if (! _COM_TxFifoEmpty(m_stTxFrameIdxFifo)) */

    /* There is no other frame to transmit (FIFO is empty), */
    /* then the transmission ressource is free */
    else
    {
        m_ebTxMailboxStatus[uMailboxIdx] = COM_MAILBOX_FREE;
    }
}
#endif

/* - Fin COM_TxNextFrame                                                         */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: COM_MonNodeCntInit                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Transmission and reception monitoring filter initialisation      */
/*********************************************************************************/
/* Input parameters          : uNodeIdx: COM monitored node index                */
/*                             eFrameStatus: node status                         */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astComMonNodeCfg: COM monitored nodes           */
/* Modified private variables: m_astComMonNode: COM monitored nodes              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#ifndef BOOTLOADER
NCS_PRI_FCT void COM_MonNodeCntInit (tComMonNodeIdx uNodeIdx, tComMonStatus eFrameStatus)
{
    /* Filtering re-initialisation */
    if (eFrameStatus == COM_MON_OK)
    {
        m_aubComMonFilterCnt[uNodeIdx] = m_aubComMonFilterOk[uNodeIdx];
    }
    else
    {
        m_aubComMonFilterCnt[uNodeIdx] = m_aubComMonFilterNok[uNodeIdx];
    }
}
#endif

/* - Fin COM_MonNodeCntInit                                                      */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_MonNodeInd                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: Transmission and reception monitoring indication                 */
/*              to Application and Network Management                            */
/*********************************************************************************/
/* Input parameters          : uNodeIdx: COM monitored node index                */
/*                             eFrameStatus: node status                         */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_astComMonNodeCfg: COM monitored nodes           */
/* Modified private variables: m_astComMonNode: COM monitored nodes              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : COM_ComBackInd                                    */
/*                             COM_ComLossInd                                    */
/*                             NM_MonNode                                        */
/*********************************************************************************/
#ifndef BOOTLOADER
NCS_PRI_FCT void COM_MonNodeInd (tComMonNodeIdx uNodeIdx, tComMonStatus eFrameStatus)
{
    /* The frame transmission/reception or non transmission/reception */
    /* has not been signaled to Application */
    /* (indication of communication loss or back with a monitored node) */
    if (m_eComNodeConfStatus[uNodeIdx] != eFrameStatus)
    {
        m_eComNodeConfStatus[uNodeIdx] = eFrameStatus;

        /* Frame transmission/reception indication : communication back with node */
        if (eFrameStatus == COM_MON_OK)
        {
            COM_ComBackInd (uNodeIdx);
        }
        /* Frame non transmission/reception indication : communication loss with node */
        else
        {
            /* Index for data copy */
            u8 ubDataIdx;
            u8 uFrameIdx;
            /* Index for node frame */
            uFrameIdx = m_auComMonFrameIdx[uNodeIdx];


            /* For a frame in reception, the datas take their default value */
            /* (with associated mask) */
            if (uFrameIdx < COM_RX_FRAME_MAX)
            {
                /* Application, CAN and timer interrupts disabling, to prevent buffers */
                /* reading or writing when reading and writing the buffers */
                u8 ebStatus;
                ebStatus = APP_InterruptDisable ();

                for (ubDataIdx = 0; ubDataIdx < m_aubComFrameLenNominal[uFrameIdx]; ubDataIdx ++)
                {
                    m_ComaaubData[uFrameIdx][ubDataIdx] |= (u8)(m_aaubComFrameComLossData[uFrameIdx][ubDataIdx] & m_aaubComFrameComLossMask[uFrameIdx][ubDataIdx]);
                    m_ComaaubData[uFrameIdx][ubDataIdx] &= (u8)(~((u8)((u8)(~(m_aaubComFrameComLossData[uFrameIdx][ubDataIdx])) & m_aaubComFrameComLossMask[uFrameIdx][ubDataIdx])));
                    #ifdef NCS_SDF_DATA_MIRROR
                    m_aaubDataMirror[uFrameIdx][ubDataIdx] = NCS_SDF_GET_U8_MIRROR(m_ComaaubData[uFrameIdx][ubDataIdx]);
                    #endif
                }

                /* Set the frame length with nominal value */
                m_aubRxFrameLen[uFrameIdx] = m_aubComFrameLenNominal[uFrameIdx];

                /* Application, CAN and timer interrupts enabling, the datas are coherents */
                APP_InterruptEnable (ebStatus);
            }
            else
            {
                /* Nothing to do */
            }


            /* Communication loss indication */
            COM_ComLossInd (uNodeIdx);
        }
    }
    else
    {
        /* Nothing to do */
    }

    /* NM indication for default management */
    COM_MonNodeStatusInd (uNodeIdx, eFrameStatus);
}
#endif

/* - Fin COM_MonNodeInd                                                          */
/*-------------------------------------------------------------------------------*/





/*-------------------------------------------------------------------------------*/
/* - Public Callbacks                                                            */

/*********************************************************************************/
/* Name: COM_Manage                                                              */
/*-------------------------------------------------------------------------------*/
/* Description: COM management                                                   */
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
/* Called callbacks          : COM_TxManage                                      */
/*                             COM_ExTimeManage                                  */
/*                             COM_TxMonManage                                   */
/*                             COM_RxMonManage                                   */
/*                             COM_MonNodeManage                                 */
/*********************************************************************************/

void COM_Manage (void)
{
    #ifndef BOOTLOADER
    /* Transmission exclusion time management */
    COM_ExTimeManage ();
    #endif

    /* Transmission monitoring management */
    COM_TxMonManage ();

    #ifndef BOOTLOADER
    /* Frame transmission and transmission monitoring management */
    COM_TxManage ();

    /* Reception monitoring management */
    COM_RxMonManage ();

    /* Transmission and reception monitoring management */
    COM_MonNodeManage ();
    #endif
    /* Bus off recovery,Add by BWF*/
    //CAN_ErrorStatusInterrupt();

    /*Add by BWF*/
    BusOffRecovery();
}

/* - Fin COM_Manage                                                              */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_TxConf                                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Successful transmission confirmation from under layer used for   */
/*              the transmission monitoring and for pending frames transmission  */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: CAN mailbox in transmission index    */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*                             m_uTxFrameIdx: COM frame index, Tx in progress    */
/*-------------------------------------------------------------------------------*/
/* Called functions          :  COM_TxNextFrame                                  */
/* Called macros             : _COM_CALLBACK_TX_CONF                             */
/* Called callbacks          : COM_MonNode                                       */
/*********************************************************************************/

#ifndef BOOTLOADER
void CAN_TxConf (tCanMailboxIdx uMailboxIdx)
{
    switch (m_eComMode)
    {
        case COM_MODE_BUS_ON :

            /* Transmission is allowed */

            /* The mailbox is used by COM for transmission */
            if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && (uMailboxIdx < (COM_TX_MAILBOX_BASE_IDX + COM_TX_MAILBOX_NB)))
            {
                /* Mailbox index is transformed for COM using */
                uMailboxIdx -= COM_TX_MAILBOX_BASE_IDX;


                /* Only if the mailbox was transmitting a frame */
                if (m_ebTxMailboxStatus[uMailboxIdx] == COM_MAILBOX_BUSY)
                {
                        tComFrameIdx uTxFrameIdx;
                    /* Frame transmitted memorisation for indication */
                    /* after transmitting another frame */
                    uTxFrameIdx = m_uTxFrameIdx[uMailboxIdx];

                    /* Next FIFO frame transmission, if exists (FIFO is not empty) */
                    COM_TxNextFrame (uMailboxIdx);

                    {
                        /* Application interrupts disabling, to prevent variable reading or writing */
                        /* when reading and writing the variable */
                        u8 ebStatus;
                        ebStatus = APP_InterruptDisable ();

                        /* The successful transmission is memorized for monitoring */
                        NCS_SET_BIT(m_aubStatus[uTxFrameIdx],
                                    (COM_MON_NODE_TASK | COM_MON_NODE_STATUS_OK), u8);

                        /* Application interrupts enabling, the variable is coherent */
                        APP_InterruptEnable (ebStatus);
                    }

                    /* The successful transmission is notified */
                    #ifndef BOOTLOADER
                    #if((COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS) || (COM_EB_CAN_TYPE == COM_EB_VW_CAN_HS))
                        /* Notification to the NM layer the successful transmission */
                        /* to can go into BUS-ON state, if the NM layer was into BUS-OFF state or CONFIRMED BUS-OFF state */
                        COM_BusOffRxTxOk();
                    #endif
                    #endif
                    _COM_CALLBACK_TX_CONF(uTxFrameIdx);
                }
            } /* end if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && ... */

            else
            {
                /* Nothing to do */
            }

        break;

        case COM_MODE_BUS_OFF :
        case COM_MODE_OFF :

            /* Nothing to do */

        break;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            break;
    }
}
#else
void CAN_TxConf (tCanMailboxIdx uMailboxIdx)
{
    if (m_eComMode == COM_MODE_BUS_ON)
    {
        /* The mailbox is used by COM for transmission */
        if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && (uMailboxIdx < (COM_TX_MAILBOX_BASE_IDX + COM_TX_MAILBOX_NB)))
        {
            /* Mailbox index is transformed for COM using */
            uMailboxIdx -= COM_TX_MAILBOX_BASE_IDX;
            
            /* There is no other frame to transmit */
            m_ebTxMailboxStatus[uMailboxIdx] = COM_MAILBOX_FREE;

            /* The successful transmission is notified */
            _COM_CALLBACK_TX_CONF(m_uTxFrameIdx[uMailboxIdx]);
        } /* end if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && ... */
    }
}
#endif
/* - Fin CAN_TxConf                                                              */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_TxFail                                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Aborted transmission confirmation from under layer used for      */
/*              the transmission monitoring and for pending frames transmission  */
/*              This callback is called after the CAN_CancelFrame () function    */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: CAN mailbox in transmission index    */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_uTxFrameIdx: COM frame index, Tx in progress    */
/* Modified private variables: m_ebTxMailboxStatus: transmission mailbox status  */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_TxNextFrame                                   */
/* Called macros             : _COM_CALLBACK_TX_MON_TIMEOUT                      */
/* Called callbacks          : COM_MonNode                                       */
/*********************************************************************************/

#ifndef BOOTLOADER
void CAN_TxFail (tCanMailboxIdx uMailboxIdx)
{
    switch (m_eComMode)
    {
        /* COM is activated */
        case COM_MODE_BUS_ON :
        case COM_MODE_BUS_OFF :

            /* The mailbox is used by COM for transmission */
            if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && (uMailboxIdx < (COM_TX_MAILBOX_BASE_IDX + COM_TX_MAILBOX_NB)))
            {
                /* Mailbox index is transformed for COM using */
                uMailboxIdx -= COM_TX_MAILBOX_BASE_IDX;


                /* Only if the mailbox was transmitting a frame */
                if (m_ebTxMailboxStatus[uMailboxIdx] == COM_MAILBOX_BUSY)
                {
                        tComFrameIdx uFrameIdx;
                    /* COM transmitted frame index */
                    uFrameIdx = m_uTxFrameIdx[uMailboxIdx];

                    {
                        /* Application interrupts disabling, to prevent variable reading or writing */
                        /* when reading and writing the variable */
                        u8 ebStatus;
                        ebStatus = APP_InterruptDisable ();

                        /* The transmission error is memorized for monitoring */
                        NCS_SET_BIT(m_aubStatus[uFrameIdx], COM_MON_NODE_TASK, u8);
                        NCS_RESET_BIT(m_aubStatus[uFrameIdx], (COM_MON_NODE_STATUS_OK), u8);

                        /* Application interrupts enabling, the variable is coherent */
                        APP_InterruptEnable (ebStatus);
                    }


                    /* The transmission error is notified to the Application, */
                    /* if a callback is defined for transmission error */
                    _COM_CALLBACK_TX_MON_TIMEOUT(uFrameIdx);

                    switch(m_eComMode)
                    {
                        /* The transmission is allowed */
                        case COM_MODE_BUS_ON :

                            /* Next FIFO frame transmission, if exists (FIFO is not empty) */
                            COM_TxNextFrame (uMailboxIdx);

                        break;

                        case COM_MODE_BUS_OFF :
                            /* There is no other frame to transmit (BUS-OFF), then the */
                            /* transmission ressource is free to desactivate monitoring */
                            m_ebTxMailboxStatus[uMailboxIdx] = COM_MAILBOX_FREE;

                        break;

                        case COM_MODE_OFF :

                            /* Nothing to do */

                        break;

                        default :
                            m_eComMode = m_eComModeDefaultValue;
                            NCS_SdfVariableRangeError (SDF_COM_MODE);
                            break;
                    }

                }

            } /* end if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && ... */

            else
            {
                /* Nothing to do */
            }

        break;

        case COM_MODE_OFF :

            /* Nothing to do */

        break;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            break;
    }
}
#else
void CAN_TxFail (tCanMailboxIdx uMailboxIdx)
{
    if ((m_eComMode == COM_MODE_BUS_ON) || (m_eComMode == COM_MODE_BUS_OFF))
    {
        /* The mailbox is used by COM for transmission */
        if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && (uMailboxIdx < (COM_TX_MAILBOX_BASE_IDX + COM_TX_MAILBOX_NB)))
        {
            /* Mailbox index is transformed for COM using */
            uMailboxIdx -= COM_TX_MAILBOX_BASE_IDX;
            
            /* There is no other frame to transmit */
            m_ebTxMailboxStatus[uMailboxIdx] = COM_MAILBOX_FREE;
        
            /* The transmission error is notified to the Application, */
            /* if a callback is defined for transmission error */
            _COM_CALLBACK_TX_MON_TIMEOUT(m_uTxFrameIdx[uMailboxIdx]);
        } /* end if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && ... */
    }
}
#endif
/* - Fin CAN_TxFail                                                              */
/*-------------------------------------------------------------------------------*/


#if ((COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS) || (COM_EB_CAN_TYPE == COM_EB_JAC_CAN_HS) || (COM_EB_CAN_TYPE == COM_EB_FIAT_CAN) || (COM_EB_CAN_TYPE == COM_EB_VW_CAN_HS))
/*********************************************************************************/
/* Name: COM_TxFail                                                              */
/*-------------------------------------------------------------------------------*/
/* Description: After TxTimeout, the transmission has to be monitored            */
/*              This function is called by COM_TxMonManage                       */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: CAN mailbox in transmission index    */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_uTxFrameIdx: COM frame index, Tx in progress    */
/* Modified private variables: m_ebTxMailboxStatus: transmission mailbox status  */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : _COM_CALLBACK_TX_MON_TIMEOUT                      */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#ifndef BOOTLOADER
NCS_PRI_FCT void COM_TxFail (tCanMailboxIdx uMailboxIdx)
{
    tComFrameIdx uFrameIdx;

    switch (m_eComMode)
    {
        /* COM is activated */
        case COM_MODE_BUS_ON :
        case COM_MODE_BUS_OFF :

            /* The mailbox is used by COM for transmission */
            if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && (uMailboxIdx < (COM_TX_MAILBOX_BASE_IDX + COM_TX_MAILBOX_NB)))
            {
                /* Mailbox index is transformed for COM using */
                uMailboxIdx -= COM_TX_MAILBOX_BASE_IDX;

                /* COM transmitted frame index */
                uFrameIdx = m_uTxFrameIdx[uMailboxIdx];

                if (NCS_TST_BIT_SET(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_PERIOD_EN, u8))
                {
                    /* Only if the mailbox was transmitting a frame */
                    if (m_ebTxMailboxStatus[uMailboxIdx] == COM_MAILBOX_BUSY)
                    {
                        /* Application interrupts disabling, to prevent variable reading or writing */
                        /* when reading and writing the variable */
                        u8 ebStatus;
                        ebStatus = APP_InterruptDisable ();

                        /* The transmission error is memorized for monitoring */
                        NCS_SET_BIT(m_aubStatus[uFrameIdx], COM_MON_NODE_TASK, u8);
                        NCS_RESET_BIT(m_aubStatus[uFrameIdx], (COM_MON_NODE_STATUS_OK), u8);

                        /* Application interrupts enabling, the variable is coherent */
                        APP_InterruptEnable (ebStatus);

                        /* The transmission error is notified to the Application, */
                        /* if a callback is defined for transmission error */
                        _COM_CALLBACK_TX_MON_TIMEOUT(uFrameIdx);
                    }
                }

            } /* end if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && ... */

            else
            {
                /* Nothing to do */
            }

        break;

        case COM_MODE_OFF :

            /* Nothing to do */

        break;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            break;
    }
}
#else
NCS_PRI_FCT void COM_TxFail (tCanMailboxIdx uMailboxIdx)
{
    if ((m_eComMode == COM_MODE_BUS_ON) || (m_eComMode == COM_MODE_BUS_OFF))
    {
        /* The mailbox is used by COM for transmission */
        if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && (uMailboxIdx < (COM_TX_MAILBOX_BASE_IDX + COM_TX_MAILBOX_NB)))
        {
            /* Mailbox index is transformed for COM using */
            uMailboxIdx -= COM_TX_MAILBOX_BASE_IDX;

            /* The transmission error is notified to the Application, */
            /* if a callback is defined for transmission error */
            _COM_CALLBACK_TX_MON_TIMEOUT(m_uTxFrameIdx[uMailboxIdx]);
        } /* end if ((COM_TX_MAILBOX_BASE_IDX <= uMailboxIdx) && ... */
    }
}
#endif
/* - Fin COM_TxFail                                                              */
/*-------------------------------------------------------------------------------*/
#endif

/*********************************************************************************/
/* Name: CAN_RxFrame                                                             */
/*-------------------------------------------------------------------------------*/
/* Description: Received frame indication from under layer, used for the         */
/*              frame reception and the reception monitoring                     */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: CAN received frame mailbox index     */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/*                             COM_FindRxFrameIdxFromIdent                       */
/*                             COM_SendFrame                                     */
/* Called macros             : COM_CAN_GET_MAILBOX_STD_IDENT                     */
/*                             COM_CAN_GET_MAILBOX_EXT_IDENT                     */
/*                             COM_CAN_GET_MAILBOX_LEN                           */
/*                             COM_CAN_GET_MAILBOX_RTR                           */
/*                             COM_CAN_GET_MAILBOX_DATA                          */
/* Called callbacks          : COM_MonNode                                       */
/*                             m_astHsComFrameCfg[].pfuRxFrameInd                */
/*                             m_astHsComFrameCfg[].pfuRxDataInd                 */
/*                             m_astHsComFrameCfg[].pfuRxInvalidDLC              */
/*                             IL_RxData                                           */
/*********************************************************************************/

void CAN_RxFrame (tCanMailboxIdx uMailboxIdx)
{

    /* Return status of  frame reception callback*/
    tComStatus eStatus;
    eStatus = COM_RX_IND_CHECK_OK;

    switch (m_eComMode)
    {
        /* Reception is allowed */
        case COM_MODE_BUS_ON :
            /* The mailbox is used by COM for reception */
            if (uMailboxIdx < COM_RX_MAILBOX_NB_TMP)
            {
                /* In case of a data frame, frame reception */
                if (COM_CAN_GET_MAILBOX_RTR (uMailboxIdx) == CAN_FRAME_TYPE_DATA)
                {
                    /* Received frame index of COM configured frames in reception */
                    tComFrameIdx uFrameIdx;

#if (COM_FILTERING_METHOD == COM_LINEAR_FILTERING)
                    /* In case of extension receive mailbox */
                    if (uMailboxIdx == COM_RX_MAILBOX_EXT_IDX)
                    {
                        /* Search the frame index which corresponds to the received identifier */
                        /* in the data frames in reception */
                        uFrameIdx = COM_FindRxFrameIdxFromIdent (COM_RX_MAILBOX_EXT_IDX, uMailboxIdx);
                    } /* end if (uMailboxIdx == COM_RX_MAILBOX_EXT_IDX) */
                    /* In case of protocol handler receive mailbox */
                    else
                    {
                        /* The frame index is the mailbox index zero-based */
                        uFrameIdx = (tComFrameIdx)(uMailboxIdx - COM_RX_MAILBOX_BASE_IDX);
                    } /* end else (uMailboxIdx == COM_RX_MAILBOX_EXT_IDX) */
#else
                #if (COM_EXTENDED_ID == COM_TRUE)
                    uFrameIdx = m_auwComFrameIdx[COM_CAN_GET_MAILBOX_EXT_IDENT (uMailboxIdx)];
                #else
                    uFrameIdx = m_auwComFrameIdx[COM_CAN_GET_MAILBOX_STD_IDENT (uMailboxIdx)];
                #endif
#endif



                    /* Common behavior (protocol handler mailbox or extension mailbox) */
                    /* to copy the received frame is its buffer, if exists */
                    if (uFrameIdx < COM_RX_FRAME_MAX)
                    {
                        u8 ubLength;
                        /* Frame reception indication by callback, if configured */
                        {
                            /* Source code optimisation : pointer on the callback */
                            /* Deviation MISRA-5 */
                            u8 (* const pfuRxFrame) (tComFrameIdx) = m_apfuComFrameRxInd[uFrameIdx];

                            /* Frame reception indication by callback, if configured */
                            if (pfuRxFrame != NULL)
                            {
                                eStatus = pfuRxFrame (uFrameIdx);
                            }
                            else
                            {
                                /* Nothing to do */
                            }

                            #ifndef BOOTLOADER
                            #if((COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS) || (COM_EB_CAN_TYPE == COM_EB_VW_CAN_HS))
                                /* Notification to the NM layer the successful reception a configured frame */
                                /* to can go into BUS-ON state if the NM layer was into BUS-OFF state or CONFIRMED BUS-OFF state */
                                COM_BusOffRxTxOk();
                            #endif
                            #endif
                        }
                        if (eStatus == COM_RX_IND_CHECK_OK)
                        {
                            //add by zhaoziyi
                            can_mailbox_descriptor_struct mdpar;
                            u8 ubComMbxData[8];


                            /* Memorise the frame length */
                            // ubLength = COM_CAN_GET_MAILBOX_LEN (uMailboxIdx);
                            // zhaoziyi add
                            mdpar.data = (uint32_t *)(ubComMbxData);
                            if(0 != can_mailbox_receive_data_read(CAN0, uMailboxIdx, &mdpar))
                            {
                                ubLength = (u8)mdpar.data_bytes;
                            }
                            else
                            {
                                ubLength = (u8)0;
                            }

                            /* Received frame length is superior or equal than configuration, */
                            /* then the datas are to be used */
                            if ((ubLength >= m_aubComFrameLenMin[uFrameIdx]) && (ubLength <= COM_DATA_MAX))
                            {
                                /* Received frame datas reading */
                                {
                                    /* Application interrupts disabling, to prevent variable reading or writing */
                                    /* when reading and writing the variable */
                                    u8 ebStatus;
                                    ebStatus = APP_InterruptDisable ();

    #if ((COM_EB_CAN_TYPE == COM_EB_PSA_CAN_LS_2010) || (COM_EB_CAN_TYPE == COM_EB_PSA_CAN_HS_IS_2010) || (COM_EB_CAN_TYPE == COM_EB_PSA_CAN_HS_M_2010))

    #ifdef NCS_SDF_DATA_MIRROR
                                    /* Received frame datas reading */
                                    COM_CAN_GET_MAILBOX_DATA_WITH_MIRROR  (uMailboxIdx, m_ComaaubData[uFrameIdx], m_aaubDataMirror[uFrameIdx]);
    #else
                                    COM_CAN_GET_MAILBOX_DATA(uMailboxIdx, m_ComaaubData[uFrameIdx]);
    #endif

                                    /* For dynamic frame, copy real received frame length */
                                    if ((m_aubComFrameConfig[uFrameIdx] & COM_LENGTH_TYPE) == COM_LENGTH_DYNAMIC)
                                    {
                                        m_aubRxFrameLen[uFrameIdx] = ubLength;
                                    }
                                    /* For static frame, fill the non received bytes and put length to Nominal value */
                                    else
                                    {
                                        u8 ubDataIdx;
                                        /* If LMin < received frame length < LNominal, apply default value for missing data bytes until LNominal*/
                                        for (ubDataIdx = ubLength; ubDataIdx < m_aubComFrameLenNominal[uFrameIdx]; ubDataIdx ++)
                                        {
                                            m_ComaaubData[uFrameIdx][ubDataIdx] = m_aaubComFrameComLossData[uFrameIdx][ubDataIdx] ;

     #ifdef NCS_SDF_DATA_MIRROR
                                            m_aaubDataMirror[uFrameIdx][ubDataIdx] = NCS_SDF_GET_U8_MIRROR(m_ComaaubData[uFrameIdx][ubDataIdx]);
    #endif
                                        }
                                        m_aubRxFrameLen[uFrameIdx] = m_aubComFrameLenNominal[uFrameIdx];
                                    }


                                    /* Datas were previously received but not read, data loss */
                                    if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_RX_FLAG)
                                    {
                                        NCS_SET_BIT(m_aubStatus[uFrameIdx],
                                                    (COM_MON_NODE_TASK |
                                                    COM_MON_NODE_STATUS_OK |
                                                    COM_MON_NODE_RELOAD |
                                                    COM_FRAME_STATUS_RX_LOST_FLAG), u8);
                                    }
                                    /* Data reception indication */
                                    else
                                    {
                                        NCS_SET_BIT(m_aubStatus[uFrameIdx],
                                                    (COM_MON_NODE_TASK |
                                                    COM_MON_NODE_STATUS_OK |
                                                    COM_MON_NODE_RELOAD |
                                                    COM_FRAME_STATUS_RX_FLAG), u8);
                                    }

                                    /* Application interrupts enabling, the variable is coherent */
                                    APP_InterruptEnable (ebStatus);
                                }

                                /* Data reception indication by callback, if configured */
                                {
                                    /* Source code optimisation : pointer on the callback */
                                    /* Deviation MISRA-5 */
                                    void (* const pfuRxData) (tComFrameIdx) = m_apfuComFrameRxDataInd[uFrameIdx];

                                    /* Data reception indication by callback, if configured */
                                    if (pfuRxData != NULL)
                                    {
                                        pfuRxData (uFrameIdx);
                                    }
                                    else
                                    {
                                        /* Nothing to do */
                                    }
                                }


    #elif ((COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS) || (COM_EB_CAN_TYPE == COM_EB_FIAT_CAN) || (COM_EB_CAN_TYPE == COM_EB_VW_CAN_HS))

                                    /* For dynamic frame, copy real received frame length */
                                    if ((m_aubComFrameConfig[uFrameIdx] & COM_LENGTH_TYPE) == COM_LENGTH_DYNAMIC)
                                    {
    #ifdef NCS_SDF_DATA_MIRROR
                                        /* Received frame data reading */
                                        COM_CAN_GET_MAILBOX_DATA_WITH_MIRROR  (uMailboxIdx, m_ComaaubData[uFrameIdx], m_aaubDataMirror[uFrameIdx]);
    #else
                                        COM_CAN_GET_MAILBOX_DATA(uMailboxIdx, m_ComaaubData[uFrameIdx]);
    #endif
                                        m_aubRxFrameLen[uFrameIdx] = ubLength;

                                        /* Data reception indication by callback, if configured */
                                        {
                                            /* Source code optimisation : pointer on the callback */
                                            /* Deviation MISRA-5 */
                                            void (* const pfuRxData) (tComFrameIdx) = m_apfuComFrameRxDataInd[uFrameIdx];

                                            /* Data reception indication by callback, if configured */
                                            if (pfuRxData != NULL)
                                            {
                                                pfuRxData (uFrameIdx);
                                            }
                                            else
                                            {
                                                /* Nothing to do */
                                            }
                                        }
                                    }
                                    /* For static frame, fill the non received bytes and put length to Nominal value */
                                    else if (ubLength >= m_aubComFrameLenNominal[uFrameIdx])
                                    {
                                        u8 ubDataIdx;
                                        // u8 ubComMbxData[COM_DATA_MAX];

                                        // COM_CAN_GET_MAILBOX_DATA(uMailboxIdx, ubComMbxData);
                                        // Det by zhaoziyi 


                                        for (ubDataIdx = 0; ubDataIdx < m_aubComFrameLenNominal[uFrameIdx]; ubDataIdx++)
                                        {
                                            m_ComaaubData[uFrameIdx][ubDataIdx] = ubComMbxData[ubDataIdx];
    #ifdef NCS_SDF_DATA_MIRROR
                                            m_aaubDataMirror[uFrameIdx][ubDataIdx] = NCS_SDF_GET_U8_MIRROR(m_ComaaubData[uFrameIdx][ubDataIdx]);
    #endif
                                        }

                                        m_aubRxFrameLen[uFrameIdx] = m_aubComFrameLenNominal[uFrameIdx];

                                        /* Data reception indication by callback, if configured */
                                        {
                                            /* Source code optimisation : pointer on the callback */
                                            /* Deviation MISRA-5 */
                                            void (* const pfuRxData) (tComFrameIdx) = m_apfuComFrameRxDataInd[uFrameIdx];

                                            /* Data reception indication by callback, if configured */
                                            if (pfuRxData != NULL)
                                            {
                                                pfuRxData (uFrameIdx);
                                            }
                                            else
                                            {
                                                /* Nothing to do */
                                            }
                                        }

                                    }
                                    /* For static frame, notified when number of bytes received is lower than expected (if configured)*/
                                    else if (ubLength < m_aubComFrameLenNominal[uFrameIdx])
                                    {
                                        /* Source code optimisation : pointer on the callback */
                                        /* Deviation MISRA-5 */
                                        void (* const pfuRxInvalidDLC) (tComFrameIdx) = m_apfuComFrameRxInvalidDLCInd[uFrameIdx];

                                        #if (FRAME_CONSISTENCY_CHECK == FRAME_CONSISTENCY_CHECK_OFF)
                                        u8 ubDataIdx;
                                        u8 ubComMbxData[COM_DATA_MAX];
                                        COM_CAN_GET_MAILBOX_DATA(uMailboxIdx, ubComMbxData);
                                        #endif

                                        m_aubRxFrameLen[uFrameIdx] = ubLength;

                                        if( pfuRxInvalidDLC != NULL )
                                        {
                                            pfuRxInvalidDLC (uFrameIdx);
                                        }
                                        else
                                        {
                                            /* Nothing to do */
                                        }
                                        /* Allows to update data even if the frame length is lower than expected */
                                        #if (FRAME_CONSISTENCY_CHECK == FRAME_CONSISTENCY_CHECK_OFF)
                                            for (ubDataIdx = 0; ubDataIdx < ubLength; ubDataIdx++)
                                            {
                                                m_ComaaubData[uFrameIdx][ubDataIdx] = ubComMbxData[ubDataIdx];
                                            #ifdef NCS_SDF_DATA_MIRROR
                                                m_aaubDataMirror[uFrameIdx][ubDataIdx] = NCS_SDF_GET_U8_MIRROR(m_ComaaubData[uFrameIdx][ubDataIdx]);
                                            #endif
                                            }

                                            /* Data reception indication by callback, if configured */
                                            {
                                                /* Source code optimisation : pointer on the callback */
                                                /* Deviation MISRA-5 */
                                                void (* const pfuRxData) (tComFrameIdx) = m_apfuComFrameRxDataInd[uFrameIdx];

                                                /* Data reception indication by callback, if configured */
                                                if (pfuRxData != NULL)
                                                {
                                                    pfuRxData (uFrameIdx);
                                                }
                                                else
                                                {
                                                    /* Nothing to do */
                                                }
                                            }
                                        #endif
                                    }
                                    else
                                    {
                                        /* Nothing to do */
                                    }


                                    /* Datas were previously received but not read, data loss */
                                    if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_RX_FLAG)
                                    {
                                        NCS_SET_BIT(m_aubStatus[uFrameIdx],
                                                    (COM_MON_NODE_TASK |
                                                    COM_MON_NODE_STATUS_OK |
                                                    COM_MON_NODE_RELOAD |
                                                    COM_FRAME_STATUS_RX_LOST_FLAG), u8);
                                    }
                                    /* Data reception indication */
                                    else
                                    {
                                        NCS_SET_BIT(m_aubStatus[uFrameIdx],
                                                    (COM_MON_NODE_TASK |
                                                    COM_MON_NODE_STATUS_OK |
                                                    COM_MON_NODE_RELOAD |
                                                    COM_FRAME_STATUS_RX_FLAG), u8);
                                    }

                                    /* Application interrupts enabling, the variable is coherent */
                                    APP_InterruptEnable (ebStatus);
                                }


#elif ( COM_EB_CAN_TYPE == COM_EB_JAC_CAN_HS )

                                    /* For dynamic frame, copy real received frame length */
                                    if ((m_aubComFrameConfig[uFrameIdx] & COM_LENGTH_TYPE) == COM_LENGTH_DYNAMIC)
                                    {
    #ifdef NCS_SDF_DATA_MIRROR
                                        /* Received frame data reading */
                                        COM_CAN_GET_MAILBOX_DATA_WITH_MIRROR  (uMailboxIdx, m_ComaaubData[uFrameIdx], m_aaubDataMirror[uFrameIdx]);
    #else
                                        COM_CAN_GET_MAILBOX_DATA(uMailboxIdx, m_ComaaubData[uFrameIdx]);
    #endif
                                        m_aubRxFrameLen[uFrameIdx] = ubLength;

                                        /* Data reception indication by callback, if configured */
                                        {
                                            /* Source code optimisation : pointer on the callback */
                                            /* Deviation MISRA-5 */
                                            void (* const pfuRxData) (tComFrameIdx) = m_apfuComFrameRxDataInd[uFrameIdx];

                                            /* Data reception indication by callback, if configured */
                                            if (pfuRxData != NULL)
                                            {
                                                pfuRxData (uFrameIdx);
                                            }
                                            else
                                            {
                                                /* Nothing to do */
                                            }
                                        }
                                    }

                                    /* static frame */
                                    else if (ubLength == m_aubComFrameLenNominal[uFrameIdx])
                                    {
                                        u8 ubDataIdx;
                                        u8 ubComMbxData[COM_DATA_MAX];

                                        COM_CAN_GET_MAILBOX_DATA(uMailboxIdx, ubComMbxData);

                                        for (ubDataIdx = 0; ubDataIdx < m_aubComFrameLenNominal[uFrameIdx]; ubDataIdx++)
                                        {
                                            m_ComaaubData[uFrameIdx][ubDataIdx] = ubComMbxData[ubDataIdx];
    #ifdef NCS_SDF_DATA_MIRROR
                                            m_aaubDataMirror[uFrameIdx][ubDataIdx] = NCS_SDF_GET_U8_MIRROR(m_ComaaubData[uFrameIdx][ubDataIdx]);
    #endif
                                        }

                                        m_aubRxFrameLen[uFrameIdx] = m_aubComFrameLenNominal[uFrameIdx];

                                        /* Data reception indication by callback, if configured */
                                        {
                                            /* Source code optimisation : pointer on the callback */
                                            /* Deviation MISRA-5 */
                                            void (* const pfuRxData) (tComFrameIdx) = m_apfuComFrameRxDataInd[uFrameIdx];

                                            /* Data reception indication by callback, if configured */
                                            if (pfuRxData != NULL)
                                            {
                                                pfuRxData (uFrameIdx);
                                            }
                                            else
                                            {
                                                /* Nothing to do */
                                            }
                                        }

                                    }
                                    /* For static frame, notified when number of bytes received is different than expected (if configured) */
                                    else
                                    {
                                        /* Source code optimisation : pointer on the callback */
                                        /* Deviation MISRA-5 */
                                        void (* const pfuRxInvalidDLC) (tComFrameIdx) = m_apfuComFrameRxInvalidDLCInd[uFrameIdx];

                                        m_aubRxFrameLen[uFrameIdx] = ubLength;

                                        if( pfuRxInvalidDLC != NULL )
                                        {
                                            pfuRxInvalidDLC (uFrameIdx);
                                        }
                                        else
                                        {
                                            /* Nothing to do */
                                        }
                                    }

                                    /* Data were previously received but not read, data loss */
                                    if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_RX_FLAG)
                                    {
                                        NCS_SET_BIT(m_aubStatus[uFrameIdx],
                                                    (COM_MON_NODE_TASK |
                                                    COM_MON_NODE_STATUS_OK |
                                                    COM_MON_NODE_RELOAD |
                                                    COM_FRAME_STATUS_RX_LOST_FLAG), u8);
                                    }
                                    /* Data reception indication */
                                    else
                                    {
                                        NCS_SET_BIT(m_aubStatus[uFrameIdx],
                                                    (COM_MON_NODE_TASK |
                                                    COM_MON_NODE_STATUS_OK |
                                                    COM_MON_NODE_RELOAD |
                                                    COM_FRAME_STATUS_RX_FLAG), u8);
                                    }

                                    /* Application interrupts enabling, the variable is coherent */
                                    APP_InterruptEnable (ebStatus);
                                }
    #endif



    #if (COM_WITH_IL == COM_TRUE)
                                /* In case of using of Interaction Upper Layer */
                                /* Data reception indication */
                                IL_RxDataInd (uFrameIdx);
    #endif
                            } /* end if (COM_CAN_GET_MAILBOX_LEN (uMailboxIdx) >= ... */
                            else
                            {
                                /* Nothing to do */
                            } /* end else (COM_CAN_GET_MAILBOX_LEN (uMailboxIdx) >= ... */
                        } /* end if (eStatus == COM_RX_IND_CHECK_OK) */
                        else
                        {
                            /* Nothing to do */
                        }
                    } /* end if (uFrameIdx < COM_FRAME_MAX) */
                    else
                    {
                        /* Nothing to do */
                    } /* end else (uFrameIdx < COM_FRAME_MAX) */
                } /* end if (COM_CAN_GET_MAILBOX_RTR (uMailboxIdx) == CAN_FRAME_TYPE_DATA) */
            }/* end if (uMailboxIdx < COM_RX_MAILBOX_NB_TMP) */
            break;

        case COM_MODE_BUS_OFF :
        case COM_MODE_OFF :
        /* Nothing to do */
        break ;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            break;
    }
}

/* - Fin CAN_RxFrame                                                             */
/*-------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------*/
/* - Private Callbacks                                                           */

/*********************************************************************************/
/* Name: COM_TxManage                                                            */
/*-------------------------------------------------------------------------------*/
/* Description: Transmission monitoring management                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*                             m_uTxFrameIdx: COM frame index, Tx in progress    */
/*                             m_ebTxMailboxStatus: transmission mailbox status  */
/*                             m_stTxFrameIdxFifo: transmission FIFO             */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_TxFrame                                       */
/* Called macros             : _COM_TxFifoEmpty                                  */
/*                             _COM_TxFifoFull                                   */
/*                             _COM_TxFifoWrIndInc                               */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#ifndef BOOTLOADER
NCS_PRI_FCT void COM_TxManage (void)
{
    switch (m_eComMode)
    {
         /* Transmission is active */
         case COM_MODE_BUS_ON :
        {
            /* Temporary index to manage all the frames in transmission */
            tComFrameIdx uFrameIdx;

            /* For all the frames in transmission */
            for (uFrameIdx = COM_RX_FRAME_MAX; uFrameIdx < COM_FRAME_MAX; uFrameIdx ++)
            {
                /* If the frame is periodic or mixed and its period is activated, */
                /* then period management */
                if (NCS_TST_BIT_SET(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_PERIOD_EN, u8))
                {
                    if (m_auComFramePeriodCnt[uFrameIdx] <= 1)
                    {
                        /* Protection of the write access on the array m_aubStatus[]*/
                        u8 ebStatus;
                        ebStatus = APP_InterruptDisable ();

                        /* Period re-initialisation and Transmission request */
                        m_auComFramePeriodCnt[uFrameIdx] = m_auComFramePeriod[uFrameIdx];
                        NCS_SET_BIT(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_TX_REQ, u8);

                        #if ((COM_EB_CAN_TYPE == COM_EB_PSA_CAN_HS_IS_2010) || (COM_EB_CAN_TYPE == COM_EB_PSA_CAN_HS_M_2010))
                            /* Transmission failure detection and indication for monitoring */
                            if (NCS_TST_BIT_SET(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_TX_PROGRESS, u8))
                            {
                                 /* The transmission error is memorized for monitoring */
                                NCS_SET_BIT(m_aubStatus[uFrameIdx], COM_MON_NODE_TASK, u8);
                                NCS_RESET_BIT(m_aubStatus[uFrameIdx], (COM_MON_NODE_STATUS_OK), u8);
                            }
                        #endif

                        /* Application interrupts enabling, the variable is coherent */
                        APP_InterruptEnable (ebStatus);
                    }
                    else
                    {
                        /* Period management */
                        m_auComFramePeriodCnt[uFrameIdx] --;
                    }
                }

                /* A direct transmission is requested (1) */
                /* and a request is not already in progress (2) */
                if ((NCS_TST_BIT_SET(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_TX_REQ, u8)) &&        /*(1) */
                    (NCS_TST_BIT_RESET(m_aubStatus[uFrameIdx], COM_FRAME_STATUS_TX_PROGRESS, u8)))  /*(2) */
                {
                    /* The exclusion time is elapsed, transmission is autorised */
                    if (m_auComFrameExTimeCnt[uFrameIdx] == 0)
                    {
                        /* Index of the first mailbox in transmission which is free */
                        tCanMailboxIdx uTxMailboxIdx = 0;


                        /* The frame transmission is memorised */
                        {
                            /* Application interrupts disabling, to prevent variable reading or writing */
                            /* when reading and writing the variable */
                            u8 ebStatus;
                            ebStatus = APP_InterruptDisable ();

                            /* The frame transmission is memorised */
                            NCS_SET_BIT(m_aubStatus[uFrameIdx],
                                        (COM_FRAME_STATUS_TX_PROGRESS), u8);

                            /* Application interrupts enabling, the variable is coherent */
                            APP_InterruptEnable (ebStatus);
                        }


                        /* Research of a free transmission mailbox */
                        while ((uTxMailboxIdx < COM_TX_MAILBOX_NB) &&
                               (m_ebTxMailboxStatus[uTxMailboxIdx] != COM_MAILBOX_FREE))
                        {
                            uTxMailboxIdx ++;
                        }

                        /* A transmission mailbox is free, */
                        /* then direct transmission in the protocol handler */
                        if (uTxMailboxIdx < COM_TX_MAILBOX_NB)
                        {
                            /* The transmission mailbox is busy */
                            m_ebTxMailboxStatus[uTxMailboxIdx] = COM_MAILBOX_BUSY;

                            /* Frame transmission */
                            COM_TxFrame (uTxMailboxIdx, uFrameIdx);


                        }

                        /* There is no transmission mailbox free, */
                        /* add frame in transmission FIFO */
                        else
                        {
                            /* FIFO not full, frame add in the transmission FIFO */
                            if (! (_COM_TxFifoFull(m_stTxFrameIdxFifo)))
                            {
                                /* Frame index memorisation in FIFO */
                                m_stTxFrameIdxFifo.auFrameIdx[m_stTxFrameIdxFifo.uWrIdx] = uFrameIdx;

                                /* FIFO add memorisation */
                                _COM_TxFifoWrIndInc(m_stTxFrameIdxFifo);
                            }
                            /* FIFO is full, transmission aborted, */
                            /* but the frame is not lost. It can be transmitted */
                            /* in another COM_TxManage () service */
                            else
                            {
                                /* Nothing to do */
                            }
                        } /* end else (uTxMailboxIdx < COM_TX_MAILBOX_NB) */
                    } /* end if (m_auComFrameExTimeCnt[uFrameIdx] == 0) */
                    else
                    {
                        /* Nothing to do */
                    }
                } /* end if (((m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_TX_REQ) || ... */


                else
                {
                    /* Nothing to do */
                }
            } /* end for (uFrameIdx = COM_RX_FRAME_MAX; uFrameIdx < ... */
        }
        break;

        case COM_MODE_BUS_OFF :
        case COM_MODE_OFF :

            /* Nothing to do */

        break ;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            break;
    }
}
#endif

/* - Fin COM_TxManage                                                            */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_ExTimeManage                                                        */
/*-------------------------------------------------------------------------------*/
/* Description: Management of frame in transmission exclusion time               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#ifndef BOOTLOADER
NCS_PRI_FCT void COM_ExTimeManage (void)
{
    switch (m_eComMode)
    {
        /* Transmission is active */
        case COM_MODE_BUS_ON :
        case COM_MODE_BUS_OFF :
        {
            /* Temporary index to manage all the frames in transmission */
            tComFrameIdx uFrameIdx;

            /* For all the frames in transmission */
            for (uFrameIdx = COM_RX_FRAME_MAX; uFrameIdx < COM_FRAME_MAX; uFrameIdx ++)
            {
                /* Exclusion time counter initialisation */
                if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_TX_CONF)
                {
                    m_auComFrameExTimeCnt[uFrameIdx] = m_auComFrameExTime[uFrameIdx];

                    {
                        /* Application interrupts disabling, to prevent variable reading or writing */
                        /* when reading and writing the variable */
                        u8 ebStatus;
                        ebStatus = APP_InterruptDisable ();

                        NCS_RESET_BIT(m_aubStatus[uFrameIdx], (COM_FRAME_STATUS_TX_CONF), u8);

                        /* Application interrupts enabling, the variable is coherent */
                        APP_InterruptEnable (ebStatus);
                    }
                }

                /* Exclusion time counter management */
                else
                {
                    /* If the exclusion time is not elapsed, it is managed */
                    if (m_auComFrameExTimeCnt[uFrameIdx] > 0)
                    {
                        m_auComFrameExTimeCnt[uFrameIdx] --;
                    }
                    else
                    {
                        /* Nothing to do */
                    }
                }
            }
        }
           break;

           case COM_MODE_OFF :

            /* Nothing to do */

        break ;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            break;
    }
}
#endif
/* - Fin COM_ExTimeManage                                                        */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_TxMonManage                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Transmission monitoring management                               */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_ebTxMailboxStatus: transmission mailbox status  */
/* Modified private variables: m_astComFrame: COM frames                         */
/*                             m_uTxTimeoutCnt: timeout in transmission          */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                                */
/* Called macros             : COM_CAN_CANCEL_FRAME                                */
/* Called callbacks          : None                                              */
/*********************************************************************************/

NCS_PRI_FCT void COM_TxMonManage (void)
{
#if (COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS)
    tComStatus eStatus;
#endif

    switch (m_eComMode)
    {
        /* Transmission is active */
        case COM_MODE_BUS_ON :
        case COM_MODE_BUS_OFF :
        {
            /* Index of transmission mailbox to manage */
            tCanMailboxIdx uTxMailboxIdx;


            /* For all the transmission mailboxes */
            for (uTxMailboxIdx = 0; uTxMailboxIdx < COM_TX_MAILBOX_NB; uTxMailboxIdx ++)
            {
                /* Transmission mailbox is used, the timeout must be managed */
                if (m_ebTxMailboxStatus[uTxMailboxIdx] == COM_MAILBOX_BUSY)
                {
                    /* The timeout in transmission is elapsed, */
                    /* it is an error for transmission monitoring */
                    if (m_uTxTimeoutCnt[uTxMailboxIdx] == 1)
                    {
#if (COM_EB_CAN_TYPE == COM_EB_PSA_CAN_LS_2010)
                        /* The trame is cancelled in the CAN protocol handler */
                        COM_CAN_CANCEL_FRAME ((tCanMailboxIdx)(uTxMailboxIdx + COM_TX_MAILBOX_BASE_IDX));
#elif (COM_EB_CAN_TYPE == COM_EB_RSA_CAN_HS)
                        eStatus = COM_TxTimeoutInd (m_uTxFrameIdx[uTxMailboxIdx]);

                        /* Upper layer requires to abort the frame in the CAN controller */
                        if (eStatus == COM_ERR_ABORT)
                        {
                            /* The trame is cancelled in the CAN protocol handler */
                            COM_CAN_CANCEL_FRAME ((tCanMailboxIdx)(uTxMailboxIdx + COM_TX_MAILBOX_BASE_IDX));
                        }
                        else
                        {
                            COM_TxFail((tCanMailboxIdx)(uTxMailboxIdx + COM_TX_MAILBOX_BASE_IDX));
                            /* Timeout in transmission is initialised */
                            m_uTxTimeoutCnt[uTxMailboxIdx] = m_auComFrameTxTimeout[m_uTxFrameIdx[uTxMailboxIdx]];
                        }

#elif ((COM_EB_CAN_TYPE == COM_EB_JAC_CAN_HS) || (COM_EB_CAN_TYPE == COM_EB_VW_CAN_HS))
                        COM_TxFail((tCanMailboxIdx)(uTxMailboxIdx + COM_TX_MAILBOX_BASE_IDX));
                        /* Timeout in transmission is initialised */
                        m_uTxTimeoutCnt[uTxMailboxIdx] = m_auComFrameTxTimeout[m_uTxFrameIdx[uTxMailboxIdx]];
#endif
                        /* The timeout is indicated to the COM layer */
                        /* in the CAN_TxFail () callback */
                    } /* end if (m_uTxTimeoutCnt[uTxMailboxIdx] == 1) */

                    /* Timeout in transmission management */
                    else if (m_uTxTimeoutCnt[uTxMailboxIdx] > 1)
                    {
                        m_uTxTimeoutCnt[uTxMailboxIdx] --;
                    }

                    /* No timeout management (value 0) */
                    else
                    {
                        /* Nothing to do */
                    }
                } /* end if (m_ebTxMailboxStatus[uTxMailboxIdx] == COM_MAILBOX_BUSY) */
                else
                {
                    /* Nothing to do */
                }
            } /* end for (uTxMailboxIdx = 0; uTxMailboxIdx < ... */
        }
           break;

           case COM_MODE_OFF :

            /* Nothing to do */

        break ;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            break;
    }
}

/* - Fin COM_TxMonManage                                                         */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_RxMonManage                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Reception monitoring management                                  */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_astComFrameCfg: COM frames config               */
/* Modified private variables: m_astComFrame: COM frames                         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : COM_MonNode                                       */
/*********************************************************************************/
#ifndef BOOTLOADER
NCS_PRI_FCT void COM_RxMonManage (void)
{
#if (COM_RX_PERIODIC_FRAME_MAX > 0)
    /* Temporary index to manage all the frames in reception */
    tComFrameIdx uFrameIdx;
#endif
    
    switch (m_eComMode)
    {
        /* Reception is active */
        case COM_MODE_BUS_ON :
        case COM_MODE_BUS_OFF :
            /* For all the periodic or mixed frames in reception */
#if (COM_RX_PERIODIC_FRAME_MAX > 0)
            for (uFrameIdx = 0; uFrameIdx < COM_RX_PERIODIC_FRAME_MAX; uFrameIdx ++)
            {
                /* Reception monitoring is active for this frame */
                if (m_aubStatus[uFrameIdx] & COM_FRAME_STATUS_PERIOD_EN)
                {
                    /* A reception is memorized for this frame */
                    if (m_aubStatus[uFrameIdx] & COM_MON_NODE_RELOAD)
                    {
                        /* Monitoring counter re-initialisation */
                        m_auComFramePeriodCnt[uFrameIdx] = m_auComFramePeriod[uFrameIdx];

                        {
                            /* Application interrupts disabling, to prevent variable reading or writing */
                            /* when reading and writing the variable */
                            u8 ebStatus;
                            ebStatus = APP_InterruptDisable ();

                            /* Frame status memorisation for treatment */
                            m_aeFrameStatus[uFrameIdx] = m_aubStatus[uFrameIdx];

                            /* The task is realized */
                            /* The task flag is reset all the time, because if the monitoring is disabled */
                            /* the task must not pend until the monitoring will be activated */
                            NCS_RESET_BIT(m_aubStatus[uFrameIdx],
                                          (COM_MON_NODE_TASK | COM_MON_NODE_STATUS_OK | COM_MON_NODE_RELOAD), u8);

                            /* The reload flag is reset */
                            /*NCS_RESET_BIT(m_aubStatus[uFrameIdx], (COM_MON_NODE_RELOAD), u8); */

                            /* Application interrupts enabling, the variable is coherent */
                            APP_InterruptEnable (ebStatus);
                        }
                    }

                    /* No reception is memorized for this frame */
                    else
                    {
                        /* Reception timeout is elapsed and frame not received */
                        if (m_auComFramePeriodCnt[uFrameIdx] <= 1)
                        {
                            /* Monitoring counter re-initialisation */
                            m_auComFramePeriodCnt[uFrameIdx] = m_auComFramePeriod[uFrameIdx];

                            {
                                /* Application interrupts disabling, to prevent variable reading or writing */
                                /* when reading and writing the variable */
                                u8 ebStatus;
                                ebStatus = APP_InterruptDisable ();

                                /* Frame status memorisation for treatment */
                                m_aeFrameStatus[uFrameIdx] = m_aubStatus[uFrameIdx];

                                /* The reception error is memorized for monitoring */
                                NCS_SET_BIT(m_aeFrameStatus[uFrameIdx],
                                            (COM_MON_NODE_TASK), u8);

                                /* The task is realized */
                                /* The task flag is reset all the time, because if the monitoring is disabled */
                                /* the task must not pend until the monitoring will be activated */
                                NCS_RESET_BIT(m_aubStatus[uFrameIdx],
                                              (COM_MON_NODE_TASK | COM_MON_NODE_STATUS_OK), u8);


                                /* The reception error is memorized for monitoring */
                                /*NCS_SET_BIT(m_aubStatus[uFrameIdx],
                                            (COM_MON_NODE_TASK), u8);*/

                                /* Application interrupts enabling, the variable is coherent */
                                APP_InterruptEnable (ebStatus);
                            }
                        }
                        else
                        {
                             /* Application interrupts disabling, to prevent variable reading or writing */
                            /* when reading and writing the variable */
                            u8 ebStatus;
                            ebStatus = APP_InterruptDisable ();

                            /* Frame status memorisation for treatment */
                            m_aeFrameStatus[uFrameIdx] = m_aubStatus[uFrameIdx];

                            /* The task is realized */
                            /* The task flag is reset all the time, because if the monitoring is disabled */
                            /* the task must not pend until the monitoring will be activated */
                            NCS_RESET_BIT(m_aubStatus[uFrameIdx],
                                          (COM_MON_NODE_TASK | COM_MON_NODE_STATUS_OK), u8);

                            /* Application interrupts enabling, the variable is coherent */
                            APP_InterruptEnable (ebStatus);

                            /* Period management */
                            m_auComFramePeriodCnt[uFrameIdx] --;


                        }
                    }
                }
                else
                {
                    /* Application interrupts disabling, to prevent variable reading or writing */
                    /* when reading and writing the variable */
                    u8 ebStatus;
                    ebStatus = APP_InterruptDisable ();

                    /* Frame status memorisation for treatment */
                    m_aeFrameStatus[uFrameIdx] = m_aubStatus[uFrameIdx];

                    /* The task is realized */
                    /* The task flag is reset all the time, because if the monitoring is disabled */
                    /* the task must not pend until the monitoring will be activated */
                    NCS_RESET_BIT(m_aubStatus[uFrameIdx],
                                  (COM_MON_NODE_TASK | COM_MON_NODE_STATUS_OK), u8);

                    /* Application interrupts enabling, the variable is coherent */
                    APP_InterruptEnable (ebStatus);
                }
            } /* end for (uFrameIdx = 0; uFrameIdx < COM_RX_PERIODIC_FRAME_MAX; uFrameIdx ++) */
#endif
            break;

        case COM_MODE_OFF :

            /* Nothing to do */
            break ;

        default :
            m_eComMode = m_eComModeDefaultValue;
            NCS_SdfVariableRangeError (SDF_COM_MODE);
            break;
    }
}
#endif
/* - Fin COM_RxMonManage                                                         */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_MonNodeManage                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Transmission and reception monitoring management                 */
/*********************************************************************************/
/* Input parameters          : uFrameIdx: COM frame index                        */
/*                             eFrameStatus: frame transmission status           */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_eComMode: COM mode                              */
/*                             m_astComMonNodeCfg: COM monitored nodes           */
/* Modified private variables: m_astComMonNode: COM monitored nodes              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : COM_MonNodeCntInit                                */
/*                             COM_MonNodeInd                                    */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#ifndef BOOTLOADER
NCS_PRI_FCT void COM_MonNodeManage (void)
{
    /* Node index */
    /* Range : (0 .. COM_MON_NODE_MAX - 1) */
    tComMonNodeIdx uNodeIdx;

    /* For all the monitored nodes */
    for (uNodeIdx = 0; uNodeIdx < COM_MON_NODE_MAX; uNodeIdx ++)
    {
        /* Frame status */
        tComMonStatus eFrameStatus;

#if(COM_MON_NODE_TX > 0)
        /*For Tx monitoring nodes*/
        if(uNodeIdx < COM_MON_NODE_TX)
        {
            /* Application interrupts disabling, to prevent variable reading or writing */
            /* when reading and writing the variable */
            u8 ebStatus;
            ebStatus = APP_InterruptDisable ();

            /* Frame status memorisation for treatment */
            eFrameStatus = m_aubStatus[m_auComMonFrameIdx[uNodeIdx]];

             /* The task is realized */
            /* The task flag is reset all the time, because if the monitoring is disabled */
            /* the task must not pend until the monitoring will be activated */
            NCS_RESET_BIT(m_aubStatus[m_auComMonFrameIdx[uNodeIdx]],
                          (COM_MON_NODE_TASK | COM_MON_NODE_STATUS_OK), u8);

            /* Application interrupts enabling, the variable is coherent */
            APP_InterruptEnable (ebStatus);
        }
        /*For Rx monitoring nodes*/
        else
#endif
        {
            /* Frame status memorisation for treatment */
            eFrameStatus = m_aeFrameStatus[m_auComMonFrameIdx[uNodeIdx]];
        }

        /* A reception error is memorized for monitoring and */
        /* monitoring is activated */
        if ((NCS_TST_BIT_SET(eFrameStatus, COM_MON_NODE_TASK, u8)) &&
            (NCS_TST_BIT_SET(eFrameStatus, COM_FRAME_STATUS_PERIOD_EN, u8)))
        {
            /* Temporary variable to memorize filtering counter value */
            u8 ubFilterCnt;

            /* Transform the m_aubStatus variable encoding to tComMonStatus type */
            if (eFrameStatus & COM_MON_NODE_STATUS_OK)
            {
                eFrameStatus = COM_MON_OK;
                ubFilterCnt = m_aubComMonFilterOk[uNodeIdx];
            }
            else
            {
                eFrameStatus = COM_MON_NOK;
                ubFilterCnt = m_aubComMonFilterNok[uNodeIdx];
            }


            /* In case of frame transmission/reception or transmission/reception timeout filtering */
            if (ubFilterCnt > 1)
            {
                /* The frame status has not changed */
                if (m_aeComMonOldStatus[uNodeIdx] == eFrameStatus)
                {
                    /* Filtering management */
                    if (m_aubComMonFilterCnt[uNodeIdx] > 1)
                    {
                        m_aubComMonFilterCnt[uNodeIdx] --;
                    }

                    /* The filtering is finished */
                    else
                    {
                        /* Filtering re-initialisation */
                        COM_MonNodeCntInit (uNodeIdx, eFrameStatus);

                        /* The frame status indication to Application and Network Management */
                        COM_MonNodeInd (uNodeIdx, eFrameStatus);
                    } /* end if (m_aubComMonFilterCnt[uNodeIdx] == 0) */
                } /* end if (m_aeComMonOldStatus[uNodeIdx] == eFrameStatus) */


                /* The frame status has changed */
                else
                {
                    /* Filtering re-initialisation */
                    COM_MonNodeCntInit (uNodeIdx, eFrameStatus);
                    m_aubComMonFilterCnt[uNodeIdx] --;

                    /* Node status memorisation */
                    m_aeComMonOldStatus[uNodeIdx] = eFrameStatus;
                }
            } /* end if (((eFrameStatus == COM_MON_OK) && ... */


            /* In case of no filtering before indication */
            else
            {
                /* The frame status has changed */
                if (m_aeComMonOldStatus[uNodeIdx] != eFrameStatus)
                {
                    /* The frame status is memorised */
                    m_aeComMonOldStatus[uNodeIdx] = eFrameStatus;
                }
                else
                {
                    /* Nothing to do */
                }

                /* The frame status indication to Application and Network Management */
                COM_MonNodeInd (uNodeIdx, eFrameStatus);
            } /* end else (((eFrameStatus == COM_MON_OK) && ... */
        } /* end if ((eFrameStatus & COM_MON_NODE_TASK) && ... */
        else
        {
            /* Nothing to do */
        }
    } /* end for (uNodeIdx = 0; uNodeIdx < COM_MON_NODE_MAX; uNodeIdx ++) */
}
#endif

/* - Fin COM_MonNodeManage                                                       */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: COM_SelectCfg                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: Selection of the configuration for initialize stack              */
/*********************************************************************************/
/* Input parameters          : ubSelectCfg: configuration index                  */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : ubConfigurationSelection                          */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/
#if ( COM_CONFIG_NB_MAX > 1U)
void COM_SelectCfg (u8 ubSelectCfg)
{
    if(ubSelectCfg < COM_CONFIG_NB_MAX)
    {
        ubConfigurationSelection = ubSelectCfg;
    }
    else
    {
        ubConfigurationSelection = 0;
    }
}
#endif
