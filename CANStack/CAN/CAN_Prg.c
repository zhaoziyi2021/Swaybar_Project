/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : CAN_Prg.c
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

/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Header Files: CAN                                                           */

#include "EB_Prj.h"
#include "MAIN.h"

/*                                                                               */
/*********************************************************************************/




/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */
u8 Received_Inplication_MsgFlg = 0;
u16 BusOffRecoveryCnt;
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

// CAN boolean type
#define CAN_FALSE               0
#define CAN_TRUE                1


/*-------------------------------------------------------------------------------*/
/* - Private Variables                                                           */


/* NERR bit of the CAN transceiver after a frame reception */
NCS_PRI_VAR u8 m_bNerr;

/* Global variable for the bit timing configuration */
//NCS_PRI_VAR tCanBaudRateConfig m_CanBaudRateConfig;

/* Nodes BUS-OFF state */
NCS_PRI_VAR u8 m_eBusOffState[CAN_CTRL_NB];

/* Nodes controller state */
NCS_PRI_VAR u8 m_eCtrlState[CAN_CTRL_NB];



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


/*********************************************************************************/
/* Name: CAN_SetMailboxStdIdent                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Write a standard identifier in a CAN mailbox                     */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                           : uwIdent: frame identifier                         */
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

void CAN_SetMailboxStdIdent (tCanMailboxIdx uMailboxIdx, u16 uwIdent)
{
    // CAN_MO_AR_ADDR_H(uMailboxIdx) = (uwIdent << 2) | CAN_MO_AR_PRI_LISTORDER;
    // CAN_MO_AR_ADDR_L(uMailboxIdx) = 0x0000U;JUNJUN
}

/* - Fin CAN_SetMailboxStdIdent                                                  */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetMailboxLen                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Write the frame length in a CAN mailbox                          */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                           : ubLen: frame length (DLC)                         */
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

void CAN_SetMailboxLen (tCanMailboxIdx uMailboxIdx, u8 ubLen)
{
    /* Erase previous length */
    CAN_MO_FCR_ADDR_H(uMailboxIdx) &= (~CAN_MO_FCRH_DLC_FIELD);

    /* Write new length */
    CAN_MO_FCR_ADDR_H(uMailboxIdx) |=  (u16)(((u16)ubLen &0x0FU) << 8);
}

/* - Fin CAN_SetMailboxLen                                                       */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetMailboxRtr                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Write the RTR bit in a CAN mailbox                               */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                           : ubRtr: Remote Transmission Request bit            */
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

void CAN_SetMailboxRtr (tCanMailboxIdx uMailboxIdx, u8 ubRtr)
{
    /* No remote bit */
}

/* - Fin CAN_SetMailboxRtr                                                       */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetMailboxData                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Write the frame datas in a CAN mailbox                           */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                           : aubData: data buffer pointer                      */
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

void CAN_SetMailboxData (tCanMailboxIdx uMailboxIdx, const u8 *aubData)
{
    if(aubData != NULL)
    {
        CAN_MO_DATA_ADDR_LL(uMailboxIdx) = aubData[0] | (aubData[1] << 8);
        CAN_MO_DATA_ADDR_LH(uMailboxIdx) = aubData[2] | (aubData[3] << 8);
        CAN_MO_DATA_ADDR_HL(uMailboxIdx) = aubData[4] | (aubData[5] << 8);
        CAN_MO_DATA_ADDR_HH(uMailboxIdx) = aubData[6] | (aubData[7] << 8);
    }
}

/* - Fin CAN_SetMailboxData                                                      */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_GetMailboxStdIdent                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Read a standard identifier from a CAN mailbox                    */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/* Output parameters         : None                                              */
/* Return value              : uwIdent: frame identifier                         */
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

u16 CAN_GetMailboxStdIdent (tCanMailboxIdx uMailboxIdx)
{
    return ((u16)((CAN_MO_AR_ADDR_H(uMailboxIdx)  & 0x1FFF ) >>2 ));
}

/* - Fin CAN_GetMailboxStdIdent                                                  */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_GetMailboxLen                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Read the frame length from a CAN mailbox                         */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/* Output parameters         : None                                              */
/* Return value              : ubLen: frame length (DLC)                         */
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

u8 CAN_GetMailboxLen (tCanMailboxIdx uMailboxIdx)
{
    /* DLC read */
    return ((u8)((CAN_MO_FCR_ADDR_H(uMailboxIdx) & 0x0F00) >> 8));
}

/* - Fin CAN_GetMailboxLen                                                       */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_GetMailboxRtr                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Read the RTR bit from a CAN mailbox                              */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/* Output parameters         : None                                              */
/* Return value              : ubRtr: Remote Transmission Request bit            */
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

u8 CAN_GetMailboxRtr (tCanMailboxIdx uMailboxIdx)
{
    u8 ubReturn = 0;

    return ubReturn;
}

/* - Fin CAN_GetMailboxRtr                                                       */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_GetMailboxData                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Read the frame datas from a CAN mailbox                          */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                           : aubData: data buffer pointer                      */
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
//unsigned char lastrevrxdata[8];
//extern  u8 m_aubStatus[COM_FRAME_MAX_RAM];

#define COM_CAN_GET_canMAILBOX_STD_IDENT(X)\
              CAN_GetMailboxStdIdent(X)

void CAN_GetMailboxData (tCanMailboxIdx uMailboxIdx, u8 *aubData)
{
//	tComFrameIdx uFrameIdx;
    if (aubData != NULL)
    {
//    	uFrameIdx = m_auwComFrameIdx[COM_CAN_GET_canMAILBOX_STD_IDENT (uMailboxIdx)];
//    	if (m_aubStatus[uFrameIdx] & 0x02)
//        {
//            m_aubStatus[uFrameIdx] &=0xfd;
//        }
//    	else
        {
            aubData[0] = (u8)(CAN_MO_DATA_ADDR_LL(uMailboxIdx) );
            aubData[1] = (u8)(CAN_MO_DATA_ADDR_LL(uMailboxIdx) >> 8 );
            aubData[2] = (u8)(CAN_MO_DATA_ADDR_LH(uMailboxIdx) );
            aubData[3] = (u8)(CAN_MO_DATA_ADDR_LH(uMailboxIdx) >> 8 );
            aubData[4] = (u8)(CAN_MO_DATA_ADDR_HL(uMailboxIdx) );
            aubData[5] = (u8)(CAN_MO_DATA_ADDR_HL(uMailboxIdx) >> 8 );
            aubData[6] = (u8)(CAN_MO_DATA_ADDR_HH(uMailboxIdx) );//High bits dropped
            aubData[7] = (u8)(CAN_MO_DATA_ADDR_HH(uMailboxIdx) >> 8 );
        }



        //WCH增加 20130410
        /*
        falg=0;
        for(zz=0;zz<8;zz++)
        {
            if(lastrevrxdata[zz]!=aubData[zz])
            {
                falg=1;
                break;
            }
        }
        for(zz=0;zz<8;zz++)
        {
            lastrevrxdata[zz]=aubData[zz];
        }
        if(falg==0)
        {
            uFrameIdx = m_auwComFrameIdx[COM_CAN_GET_canMAILBOX_STD_IDENT (uMailboxIdx)];
            if (m_aubStatus[uFrameIdx] & 0x02)
            {
                m_aubStatus[uFrameIdx] &=0xfd;
            }
        }
*/


    }





}

/* - Fin CAN_GetMailboxData                                                      */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_GetMailboxDataWithMirror                                            */
/*-------------------------------------------------------------------------------*/
/* Description: Read the frame datas from a CAN mailbox with mirror encoding     */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/* Output parameters         : aubData: data buffer pointer                      */
/*                           : aubMirror: data mirror buffer pointer             */
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

void CAN_GetMailboxDataWithMirror (tCanMailboxIdx uMailboxIdx, u8 *aubData, u8 *aubMirror)
{
    if (aubData != NULL)
    {

        if(aubMirror != NULL)
        {
            aubData[0] = (u8)(CAN_MO_DATA_ADDR_LL(uMailboxIdx) );
            aubMirror[0] = NCS_SDF_GET_U8_MIRROR(aubData[0] );
            aubData[1] = (u8)(CAN_MO_DATA_ADDR_LL(uMailboxIdx) >> 8 );
            aubMirror[1] = NCS_SDF_GET_U8_MIRROR(aubData[1] );
            aubData[2] = (u8)(CAN_MO_DATA_ADDR_LH(uMailboxIdx) );
            aubMirror[2] = NCS_SDF_GET_U8_MIRROR(aubData[2] );
            aubData[3] = (u8)(CAN_MO_DATA_ADDR_LH(uMailboxIdx) >> 8 );
            aubMirror[3] = NCS_SDF_GET_U8_MIRROR(aubData[3] );
            aubData[4] = (u8)(CAN_MO_DATA_ADDR_HL(uMailboxIdx) );
            aubMirror[4] = NCS_SDF_GET_U8_MIRROR(aubData[4] );
            aubData[5] = (u8)(CAN_MO_DATA_ADDR_HL(uMailboxIdx) >> 8 );
            aubMirror[5] = NCS_SDF_GET_U8_MIRROR(aubData[5] );
            aubData[6] = (u8)(CAN_MO_DATA_ADDR_HH(uMailboxIdx) );//High bits dropped .
            aubMirror[6] = NCS_SDF_GET_U8_MIRROR(aubData[6] );
            aubData[7] = (u8)(CAN_MO_DATA_ADDR_HH(uMailboxIdx) >> 8 );
            aubMirror[7] = NCS_SDF_GET_U8_MIRROR(aubData[7] );
        }
    }
}

/* - Fin CAN_GetMailboxDataWithMirror                                            */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_TransmitFrame                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Request the transmission of a CAN mailbox                        */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
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

void CAN_TransmitFrame (tCanMailboxIdx uMailboxIdx)
{
    //junjun CAN_MO_CTR_ADDR_H(uMailboxIdx) = CAN_MO_CTR_TXRQ_BIT | CAN_MO_CTR_TXEN0_BIT | CAN_MO_CTR_TXEN1_BIT | CAN_MO_CTR_MSGVAL_BIT;
}

/* - Fin CAN_TransmitFrame                                                       */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_CancelFrame                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Abort the transmission of a CAN mailbox                          */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
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

void CAN_CancelFrame (tCanMailboxIdx uMailboxIdx)
{
    /* Disable Tx */
    CAN_MO_CTR_ADDR_L(uMailboxIdx)= CAN_MO_CTR_TXEN0_BIT;
}

/* - Fin CAN_CancelFrame                                                         */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetMailboxMode                                                      */
/*-------------------------------------------------------------------------------*/
/* Description: Configure a CAN mailbox mode in transmission, reception, ...     */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                             ubMode: mode (transmission, reception, ...)       */
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

#if ((CAN_TX_MAILBOX_NB == 0) || (CAN_RX_MAILBOX_NB == 0))
void CAN_SetMailboxMode (tCanMailboxIdx uMailboxIdx, u8 ubMode)
{
    switch (ubMode)
    {
        case CAN_MAILBOX_MODE_TX:

            /* Set DIR, TXEN0, TXEN1, MSGVAL */
            // CAN_MO_CTR_ADDR_H(uMailboxIdx) =    CAN_MO_CTR_NEWDAT_BIT | CAN_MO_CTR_MSGVAL_BIT | CAN_MO_CTR_SETDIR_BIT |
            //                                    CAN_MO_CTR_TXEN1_BIT | CAN_MO_CTR_TXEN0_BIT;

            // /* Reset other bits */
            //  CAN_MO_CTR_ADDR_L(uMailboxIdx) = 0;

            // /* Activate the transmit interrupt */
            // CAN_MO_FCR_ADDR_H(uMailboxIdx) |= CAN_MO_FCRH_TXIE_BIT;//JUNJUN ALL

            break;

        case CAN_MAILBOX_MODE_RX:

            /* Set RXEN, MSGVAL */
            // CAN_MO_CTR_ADDR_H(uMailboxIdx) = CAN_MO_CTR_RXEN_BIT | CAN_MO_CTR_MSGVAL_BIT;
            // CAN_MO_CTR_ADDR_L(uMailboxIdx)= 0x00;

            // /* Activate the receive interrupt */
            // CAN_MO_FCR_ADDR_H(uMailboxIdx) |= CAN_MO_FCRH_RXIE_BIT;//JUNJUN ALL

            break;

        default:
            /* Security path, nothing to do */
            break;
    }
}
#endif

/* - Fin CAN_SetMailboxMode                                                      */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetMailboxStdMask                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Configure a CAN mailbox identifier acceptance mask               */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                             uwMask: standard identifier acceptance mask       */
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

#ifdef CAN_MAILBOX_INDIVIDUAL_MASK
void CAN_SetMailboxStdMask (tCanMailboxIdx uMailboxIdx, u16 uwMask)
{
    /* Set Ident + mailbox only receive std ident */
    // CAN_MO_AMR_ADDR_H(uMailboxIdx) = ((uwMask << CAN_MO_AMR_STDIDENT_SHIFT) & CAN_MO_AMR_ID_FIELD)| CAN_MO_AMR_MIDE_BIT;

    // CAN_MO_AMR_ADDR_L(uMailboxIdx) = 0xFFFF;//JUNJUN ALL
}
#endif

/* - Fin CAN_SetMailboxStdMask                                                   */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetMailboxIde                                                       */
/*-------------------------------------------------------------------------------*/
/* Description: Configure a CAN mailbox identifier format (standard or extended) */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                             ubIde: identifier format (standard or extended)   */
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

void CAN_SetMailboxIde (tCanMailboxIdx uMailboxIdx, u8 ubIde)
{
     switch (ubIde)
        {
            case CAN_IDENT_STD:
                /* Set Standard frame IDE */
                // CAN_MO_AR_ADDR_H(uMailboxIdx) &= ~CAN_MO_AR_IDE_BIT ;JUNJUN
                break;

            case CAN_IDENT_EXT:
                /* Set Extended frame IDE */
                // CAN_MO_AR_ADDR_H(uMailboxIdx) |= CAN_MO_AR_IDE_BIT ;JUNJUN
                break;

            default:
               /* do nothing */
                break;
        }
}

/* - Fin CAN_SetMailboxIde                                                       */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetMailboxInterruptCfg                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Configure the interrupts of a CAN mailbox                        */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                             uwIntCfg: interrupt configuration                 */
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

void CAN_SetMailboxInterruptCfg (tCanMailboxIdx uMailboxIdx, u16 uwIntCfg)
{

    // if(uwIntCfg== CAN_INTERRUPT_TX_OK)JUNJUN ALL
    // {
    //     CAN_MO_FCR_ADDR_H(uMailboxIdx) = CAN_MO_FCRH_TXIE_BIT;
    // }
    // else if(uwIntCfg== CAN_INTERRUPT_RX_OK)
    // {
    //     CAN_MO_FCR_ADDR_H(uMailboxIdx) = CAN_MO_FCRH_RXIE_BIT;
    // }
    // else
    // {
    //     /* Nothing to do */
    // }
}

/* - Fin CAN_SetMailboxInterruptCfg                                              */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_MailboxInterruptEnable                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Enable the interrupts of a CAN mailbox                           */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
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

void CAN_MailboxInterruptEnable (tCanMailboxIdx uMailboxIdx)
{
}

/* - Fin CAN_MailboxInterruptEnable                                              */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetGeneralInterruptCfg                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Configure the global interrupts of the CAN protocol handler      */
/*********************************************************************************/
/* Input parameters          : uwIntCfg: global interrupt configuration          */
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
/*
void CAN_SetGeneralInterruptCfg (u16 uwIntCfg)
{

}
*/
/* - Fin CAN_SetGeneralInterruptCfg                                              */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetGeneralStdMask                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Configure the global standard identifier acceptance masks        */
/*********************************************************************************/
/* Input parameters          : uwMask: standard identifier acceptance mask       */
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

void CAN_SetGeneralStdMask (u16 uwMask)
{
    /* Nothing to do */
 }

/* - Fin CAN_SetGeneralStdMask                                                   */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_Activate                                                            */
/*-------------------------------------------------------------------------------*/
/* Description: CAN bus activation                                               */
/*              The protocol handler is switched to NORMAL mode                  */
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

void CAN_Activate (void)
{
    /* Enable Alert interrupt + disable Init */
    // CAN_NCR_ADDR = CAN_NCR_ALIE_BIT;//JUNJUN ALL

    /* Memorize controller state */
    m_eCtrlState[CAN_CTRL_SEL] = CAN_TRUE;

    /* Generate interrupt request to treat eventual pending interrupts */
    // CAN_MITR_ADDR = 1<<CAN_CTRL_SEL;

}

/* - Fin CAN_Activate                                                            */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_Desactivate                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: CAN bus desactivation                                            */
/*              The protocol handler is switched to INITIALISATION mode          */
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

void CAN_Desactivate (void)
{
    /* Go to init mode */
    // CAN_NCR_ADDR = CAN_NCR_INIT_BIT;JUNJUN

    /* Memorize controller state */
    m_eCtrlState[CAN_CTRL_SEL] = CAN_FALSE;
}

/* - Fin CAN_Desactivate                                                         */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_Init                                                                */
/*-------------------------------------------------------------------------------*/
/* Description: CAN protocol handler initialisation                              */
/*              CAN system clock configuration                                   */
/*              CAN protocol handler buffers and register access authorization   */
/*              CAN protocol handler Tx and Rx pins configuration                */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_ubTxStatus: CAN protocol handler status         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

void CAN_Init (void)
{
    //u8 uMailboxIdx;
    //u16 usTempRegister;
    //volatile u16 uwTemp;

    /* CAN bit timing configuration */
    // m_CanBaudRateConfig.ubBrp = CAN_BIT_TIME_BRP;
    // m_CanBaudRateConfig.ubPropSeg = CAN_BIT_TIME_PROPSEG;
    // m_CanBaudRateConfig.ubPSeg1 = CAN_BIT_TIME_PSEG1;
    // m_CanBaudRateConfig.ubPSeg2 = CAN_BIT_TIME_PSEG2;
    // m_CanBaudRateConfig.ubSmp = CAN_BIT_TIME_SMP;
    // m_CanBaudRateConfig.ubSjw = CAN_BIT_TIME_SJW;

//    MCAN_KSCCFG  =  0x0003;      /* load Kernel State Configuration Register */

//    uwTemp = MCAN_KSCCFG;    /* dummy read to avoid pipeline effects */

    /* Clock configuration = 100,00 MHz */
    /* Normal divider mode selected */
   // CAN_FDRL     =  0x43FF;      /* load Fractional Divider Register */

  //  uwTemp       =  CAN_FDRL;    /* dummy read to avoid pipeline effects */
 //   CAN_FDRL     =  0x43FF;      // load Fractional Divider Register

  //  uwTemp       =  CAN_FDRL;    // dummy read to avoid pipeline effects

    /* Wait for end of instruction */
  //  while( CAN_PANCTR_ADDR_L &CAN_PANCTRL_BUSY_BIT);

    /* Set controller mode and enable bit timing configuration */
 //   CAN_NCR_ADDR = CAN_NCR_INIT_BIT | CAN_NCR_CCE_BIT | CAN_NCR_TRIE_BIT | CAN_NCR_LECIE_BIT |CAN_NCR_ALIE_BIT;

    /* Reset all mailboxes */
 //   CAN_PANCTR_ADDR_L = CAN_PANCTRL_CMD_INITLISTS | CAN_PANCTRL_BUSY_BIT;

    /* Wait for end of instruction */
 //   while(CAN_PANCTR_ADDR_L & CAN_PANCTRL_BUSY_BIT);

    /* ALINP on CANSRC_3 for controller 0 */
   // CAN_NIPR0    =  0x0003;      // load NIPR0_LECINP, ALINP, CFCINP and TRINP

    /* Allocate mailboxes to the CAN controller message object list */
    /* See datasheet ch 21-53 (p1456) for details */
    /*
    for(uMailboxIdx= 0; uMailboxIdx < COM_FRAME_MAX; uMailboxIdx++)
    {

        usTempRegister = uMailboxIdx;


        usTempRegister |= (CAN_CTRL_SEL+1)<< CAN_PANCTRH_PANAR2_SHIFT;


        CAN_PANCTR_ADDR_H = usTempRegister;


        CAN_PANCTR_ADDR_L = CAN_PANCTRL_CMD_STATICALLOC | CAN_PANCTR_BUSY;


        while( CAN_PANCTR_ADDR_L &CAN_PANCTRL_BUSY_BIT);


        CAN_MO_IPR_ADDR_L(uMailboxIdx) = (u16)((u16)uMailboxIdx<<8) | (u16)((u16)CAN_CTRL_SEL<< 13);


        CAN_MO_IPR_ADDR_L(uMailboxIdx) &= 0xFF00U;


         CAN_MO_FCR_ADDR_L(uMailboxIdx) &= 0xFFF0U;


        CAN_MO_CTR_ADDR_L(uMailboxIdx) |= CAN_MO_CTR_MSGVAL_BIT;
    }
*/
   // CAN_PortCfg();

    /* Set mailbox event mask to accept all */
    //CAN_MSIMASK_ADDR_L = 0xFFFFU;
   // CAN_MSIMASK_ADDR_H = 0xFFFFU;

    m_eCtrlState[CAN_CTRL_SEL] = CAN_FALSE;
    m_eBusOffState[CAN_CTRL_SEL] = CAN_FALSE;
}

/* - Fin CAN_Init                                                                */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_BitTimeCfg                                                          */
/*-------------------------------------------------------------------------------*/
/* Description: CAN network bitrate configuration                                */
/*              Bitrate prescaler, propagation segment, phase segment 1,         */
/*              phase segment 2, synchronisation jump width and sample point mode*/
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
/*
void CAN_BitTimeCfg (void)
{
    u8 ubNBTRByte0;
    u8 ubNBTRByte1;
    u16 ulNBTRRegister;


    CAN_NCR_ADDR = CAN_NCR_CCE_BIT | CAN_NCR_INIT_BIT;


    ubNBTRByte0 = ((u8)(m_CanBaudRateConfig.ubSjw - 1) << 6) | (u8)(m_CanBaudRateConfig.ubBrp - 1);
    ubNBTRByte1 = ((u8)(m_CanBaudRateConfig.ubPSeg2 - 1) << 4) | (u8)(m_CanBaudRateConfig.ubPropSeg + m_CanBaudRateConfig.ubPSeg1 - 1);
    ulNBTRRegister = (u16)(0xFFFF & ((((u32)ubNBTRByte0)&0x00FF) | (((u32)((u32)ubNBTRByte1)<<8)&0xFF00)));
    CAN_NBTR_ADDR_L = ulNBTRRegister;


    CAN_NCR_ADDR &= ~CAN_NCR_CCE_BIT;

}
*/
/* - Fin CAN_BitTimeCfg                                                          */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_InitBitTimingConfig                                                 */
/*-------------------------------------------------------------------------------*/
/* Description: CAN network bitrate configuration dynamicaly                     */
/*              Bitrate prescaler, propagation segment, phase segment 1,         */
/*              phase segment 2, synchronisation jump width and sample point mode*/
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

void CAN_InitBitTimingConfig (tCanBaudRateConfig m_stbitTimingUpdate)
{
}

/* - Fin CAN_InitBitTimingConfig                                                 */
/*-------------------------------------------------------------------------------*/




/*********************************************************************************/
/* Name: CAN_PowerOnInit                                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Initialise the CAN protocol handler and put it in SLEEP mode.    */
/*              It allows the protocol handler to detect a WAKE-UP from CAN bus  */
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
/* Called functions          : CAN_Init                                          */
/*                             CAN_Desactivate                                   */
/*                             CAN_BitTimeCfg                                    */
/*                             CAN_Activate                                      */
/*                             CAN_SetGeneralInterruptCfg                        */
/*                             CAN_InterruptLevelCfg                             */
/*                             CAN_InterruptEnable                               */
/*                             CAN_Sleep                                         */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#if (CAN_WAKE_UP_DETECTION  == CAN_WAKE_UP_ACTIVATE)
void CAN_PowerOnInit (void)
{
    // CAN WAKE-UP interrupt configuration and enabling
    CAN_Init ();
    CAN_Desactivate ();
  //  CAN_BitTimeCfg ();
    CAN_Activate ();
   // CAN_SetGeneralInterruptCfg (CAN_INTERRUPT_WAKE_UP);
   // CAN_InterruptLevelCfg ();
    CAN_InterruptEnable ();
    CAN_Sleep ();
}
#endif

/* - Fin CAN_PowerOnInit                                                         */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: CAN_PowerOnInitWithBitTimingCfg                                         */
/*-------------------------------------------------------------------------------*/
/* Description: Initialise the CAN protocol handler and put it in SLEEP mode.    */
/*              It allows the protocol handler to detect a WAKE-UP from CAN bus  */
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
/* Called functions          : CAN_Init                                          */
/*                             CAN_Desactivate                                   */
/*                             CAN_InitBitTimingConfig                           */
/*                             CAN_BitTimeCfg                                    */
/*                             CAN_Activate                                      */
/*                             CAN_SetGeneralInterruptCfg                        */
/*                             CAN_InterruptLevelCfg                             */
/*                             CAN_InterruptEnable                               */
/*                             CAN_Sleep                                         */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#if (CAN_WAKE_UP_DETECTION  == CAN_WAKE_UP_ACTIVATE)
void CAN_PowerOnInitWithBitTimingCfg (void)
{
    /* CAN WAKE-UP interrupt configuration and enabling */
    CAN_Init ();
    CAN_Desactivate ();
    /* Required to be call after CAN_Init configuration */
    CAN_InitBitTimingConfig(m_stbitTimingUpdate);
  //  CAN_BitTimeCfg ();
    CAN_Activate ();
   // CAN_SetGeneralInterruptCfg (CAN_INTERRUPT_WAKE_UP);
   // CAN_InterruptLevelCfg ();
    CAN_InterruptEnable ();
    CAN_Sleep ();
}
#endif

/* - Fin CAN_PowerOnInitWithBitTimingCfg                                         */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_Sleep                                                               */
/*-------------------------------------------------------------------------------*/
/* Description: CAN bus sleep                                                    */
/*              The protocol handler is switched to SLEEP mode                   */
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
/* Called functions          : CAN_Activate                                      */
/*                             CAN_Desactivate                                   */
/* Called macros             : None                                              */
/* Called callbacks          : CAN_WakeUpInd                                     */
/*********************************************************************************/

#if (CAN_WAKE_UP_DETECTION  == CAN_WAKE_UP_ACTIVATE)
void CAN_Sleep (void)
{


}
#endif

/* - Fin CAN_Sleep                                                               */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_BusOff                                                              */
/*-------------------------------------------------------------------------------*/
/* Description: Force the protocol handler to stay in BUS-OFF state              */
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
/* Called functions          : CAN_Desactivate                                   */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

void CAN_BusOff (void)
{
    /* Switch the protocol handler to INITIALISATION mode */
    CAN_Desactivate (); // only for Node 0
    for (tCanMailboxIdx uMailboxIdx = 0; uMailboxIdx < CAN_MAILBOX_NB; uMailboxIdx ++)
    {
        /* OSCCAN-55: CAN_TxFail called here to unlock upper channel */
        CAN_TxFail(uMailboxIdx);
    }
}

/* - Fin CAN_BusOff                                                              */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_BusOn                                                               */
/*-------------------------------------------------------------------------------*/
/* Description: Switch the protocol handler in the BUS-ON state                  */
/*              This function must be called when the protocol handler is        */
/*              in BUS-OFF mode                                                  */
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
/* Called functions          : CAN_Activate                                      */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

void CAN_BusOn (void)
{

}

/* - Fin CAN_BusOn                                                               */
/*-------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: CAN_GetSampledNerrBit                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Read the NERR bit of the CAN transceiver that was sampled when   */
/*              a CAN frame was received                                         */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : bNerr: CAN transceiver sampled NERR bit           */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : m_bNerr: CAN transceiver sampled NERR bit         */
/* Modified private variables: None                                              */
/*-------------------------------------------------------------------------------*/
/* Called functions          : None                                              */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

u8 CAN_GetSampledNerrBit (void)
{
    // TJA1054 CAN transceiver NERR\ pin read
    return (m_bNerr);
}

/* - Fin CAN_GetSampledNerrBit                                                   */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_ResetMailboxTxFlag                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Clear the TxOK interrupt flag of the mailbox, to release         */
/*              the CAN mailbox event                                            */
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

#ifdef CAN_MAILBOX_INDIVIDUAL_INTERRUPT
void CAN_ResetMailboxTxFlag (tCanMailboxIdx uMailboxIdx)
{
}
#endif

/* - Fin CAN_ResetMailboxTxFlag                                                  */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_ResetMailboxRxFlag                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Clear the RxOK interrupt flag of the mailbox, to release         */
/*              the CAN mailbox event                                            */
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

#ifdef CAN_MAILBOX_INDIVIDUAL_INTERRUPT
void CAN_ResetMailboxRxFlag (tCanMailboxIdx uMailboxIdx)
{

}
#endif

/* - Fin CAN_ResetMailboxRxFlag                                                  */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: CAN_SleepStateCheck                                                     */
/*-------------------------------------------------------------------------------*/
/* Description: check that the conditions needed to detect a wake up interrupt   */
/*              are respected                                                    */
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

u8 CAN_SleepStateCheck (void)
{

    u8 ubErrStatus = CAN_ERR_OK;


    return (ubErrStatus);

}

/* - Fin CAN_SleepStateCheck                                                     */
/*-------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------*/
/* - Private Functions                                                           */

/*-------------------------------------------------------------------------------*/
/* - Public Callbacks                                                            */

/*********************************************************************************/
/* Name: CAN_Interrupt                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: CAN message Tx, Rx or error interrupt                            */
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
/* Called callbacks          : CAN_TxFail                                        */
/*                             CAN_TxConf                                        */
/*                             CAN_RxFrame                                       */
/*                             CAN_ErrorStatusInterrupt                          */
/*********************************************************************************/

void CAN_Interrupt (void)
{
    /* mailbox index */
    tCanMailboxIdx uMailboxIdx;

    /* Search the first mailbox with a pending interrupt request */
    uMailboxIdx = (u8)CAN_MSID0_ADDR;


    /* The handlers exists in the CAN driver */
    if (uMailboxIdx < 32U)
    {
        /* successful transmission of a frame */
        if(CAN_MO_STAT_ADDR_L(uMailboxIdx) & CAN_MO_CTR_TXPND_BIT)
        {
            /* Addressing CAN_MSPND0 */
            CAN_MSPND0_ADDR_L =(u16)(~(u16)(1 << uMailboxIdx));
            CAN_MSPND0_ADDR_H = 0xFFFFU;

            CAN_MO_CTR_ADDR_L(uMailboxIdx) = CAN_MO_CTR_TXPND_BIT|CAN_MO_CTR_NEWDAT_BIT;

            /* indicate the frame transmission to the COM layer */
            CAN_TxConf(uMailboxIdx);
        }
        /* successful reception of a frame */
        else if(CAN_MO_STAT_ADDR_L(uMailboxIdx) & CAN_MO_CTR_RXPND_BIT)
        {
            /* Addressing CAN_MSPND0 to clear flag */
            CAN_MSPND0_ADDR_L =(u16)(~(u16)(1 << uMailboxIdx));
            CAN_MSPND0_ADDR_H = (u16) 0xFFFFU;

            CAN_MO_CTR_ADDR_L(uMailboxIdx) = CAN_MO_CTR_RXPND_BIT|CAN_MO_CTR_NEWDAT_BIT|CAN_MO_CTR_MSGLST_BIT|CAN_MO_CTR_RXEN_BIT;

            /* indicate the frame reception to the COM layer */
            /* NOTE : correction of MISRA error : the paramater of CAN_RX_FRAME must be named exactly uMailboxIdx */
            CAN_RxFrame(uMailboxIdx);

            /* re enable the mailbox for reception */
            CAN_MO_CTR_ADDR_H(uMailboxIdx) = CAN_MO_CTR_RXEN_BIT;
        }

        /* Generate CAN0 IT, to prevent loosing an event */
        *(u16*)CAN_MITR = (u16)((u16)0x00000001U);
    }
}

/* - Fin CAN_Interrupt                                                           */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: CAN_ErrorStatusInterrupt                                                */
/*-------------------------------------------------------------------------------*/
/* Description: CAN status error interrupt                                       */
/*********************************************************************************/
/* Input parameters          : None                                              */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used public variables     : None                                              */
/* Modified public variables : None                                              */
/*-------------------------------------------------------------------------------*/
/* Used private variables    : None                                              */
/* Modified private variables: m_ubTxStatus: CAN protocol handler status         */
/*-------------------------------------------------------------------------------*/
/* Called functions          : CAN_Desactivate                                   */
/*                             CAN_CancelFrame                                   */
/* Called macros             : None                                              */
/* Called callbacks          : CAN_BusOffInd                                     */
/*********************************************************************************/

void CAN_ErrorStatusInterrupt (void)
{
    u16 uwSource;

    uwSource = CAN_NSR_ADDR;

    /* Reset error code */
    CAN_NSR_ADDR &=  ~CAN_NSR_LEC_FIELD;

    /* BUS-OFF entering source */
    if( ((uwSource & (CAN_NSR_BOFF_BIT|CAN_NSR_ALERT_BIT)) == (CAN_NSR_BOFF_BIT|CAN_NSR_ALERT_BIT))
    &&  (m_eBusOffState[CAN_CTRL_SEL] == CAN_FALSE))
    {
        /* Memorize that controller is in BUS-OFF state */
        m_eBusOffState[CAN_CTRL_SEL] = CAN_TRUE;

        /* BUS-OFF notification */
        CAN_BusOffInd();
    }

    /* BUS-OFF entering source */
    else if((uwSource & (CAN_NSR_BOFF_BIT|CAN_NSR_ALERT_BIT)) == CAN_NSR_ALERT_BIT)
    {
        if( ((CAN_NCR_ADDR & CAN_NCR_INIT_BIT) == CAN_NCR_INIT_BIT)
        &&  (m_eCtrlState[CAN_CTRL_SEL] == CAN_TRUE))
        {
            /* Memorize that controller is in BUS-ON state */
            m_eBusOffState[CAN_CTRL_SEL] = CAN_FALSE;

            /* BUS-OFF notification */
            CAN_BusOffInd();
        }

        /* Other cause, ignored */
        else
        {
            /* Memorize that controller is in BUS-ON state */
            m_eBusOffState[CAN_CTRL_SEL] = CAN_FALSE;
        }
    }
    else
    {
        /* Nothing to to */
    }
}


/* - Fin CAN_ErrorStatusInterrupt                                                */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_WakeUpInterrupt                                                     */
/*-------------------------------------------------------------------------------*/
/* Description: CAN WAKE-UP interrupt                                            */
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
/* Called functions          : CAN_Desactivate                                   */
/* Called macros             : None                                              */
/* Called callbacks          : None                                              */
/*********************************************************************************/

#if (CAN_WAKE_UP_DETECTION  == CAN_WAKE_UP_ACTIVATE)
void CAN_WakeUpInterrupt (void)
{

}
#endif

/* - Fin CAN_WakeUpInterrupt                                                     */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_RxPolling                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: polling of CAN RX mailboxes                                      */
/*********************************************************************************/
/* Input parameters          : ulValue: Value for the WDTCON0 register           */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*********************************************************************************/

void CAN_RxPolling ()
{
    /* mailbox index */
    tCanMailboxIdx uMailboxIdx;

    /* Search the first mailbox with a pending interrupt request */
    uMailboxIdx = (u8)CAN_MSID0_ADDR;

    /* The handlers exists in the CAN driver */
    if (uMailboxIdx < 32U)
    {
        /* successful reception of a frame */
        if(CAN_MO_STAT_ADDR_L(uMailboxIdx) & CAN_MO_CTR_RXPND_BIT)
        {
            /* Addressing CAN_MSPND0 to clear flag */
            CAN_MSPND0_ADDR_L =(u16)(~(u16)(1 << uMailboxIdx));

            CAN_MO_CTR_ADDR_L(uMailboxIdx) = CAN_MO_CTR_RXPND_BIT|CAN_MO_CTR_NEWDAT_BIT|CAN_MO_CTR_MSGLST_BIT|CAN_MO_CTR_RXEN_BIT;

            /* indicate the frame reception to the COM layer */
            /* NOTE : correction of MISRA error : the paramater of CAN_RX_FRAME must be named exactly uMailboxIdx */
            CAN_RxFrame(uMailboxIdx);

            /* re enable the mailbox for reception */
            CAN_MO_CTR_ADDR_H(uMailboxIdx) = CAN_MO_CTR_RXEN_BIT;
        }
    }
}

/* - End CAN_RxPolling                                                           */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_TxPolling                                                           */
/*-------------------------------------------------------------------------------*/
/* Description: polling of CAN TX mailboxes                                      */
/*              modification                                                     */
/*********************************************************************************/
/* Input parameters          : ulValue: Value for the WDTCON0 register           */
/* Output parameters         : None                                              */
/* Return value              : None                                              */
/*********************************************************************************/

void CAN_TxPolling ()
{
    /* mailbox index */
    tCanMailboxIdx uMailboxIdx;

    /* Search the first mailbox with a pending interrupt request */
    uMailboxIdx = (u8)CAN_MSID0_ADDR;

    /* The handlers exists in the CAN driver */
    if (uMailboxIdx < 32U)
    {
        /* successful transmission of a frame */
        if(CAN_MO_STAT_ADDR_L(uMailboxIdx) & CAN_MO_CTR_TXPND_BIT)
        {
            /* Addressing CAN_MSPND0 */
            CAN_MSPND0_ADDR_L =(u16)(~(u16)(1 << uMailboxIdx));

            CAN_MO_CTR_ADDR_L(uMailboxIdx) = CAN_MO_CTR_TXPND_BIT|CAN_MO_CTR_NEWDAT_BIT;

            /* indicate the frame transmission to the COM layer */
            CAN_TxConf(uMailboxIdx);
        }
    }
}

/* - End CAN_TxPolling                                                           */
/*-------------------------------------------------------------------------------*/





/*********************************************************************************/
/* Name: CAN_SetMailboxExtIdent                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Write a Enhanced identifier in a CAN mailbox                     */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                           : ulIdent: frame identifier                         */
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

void CAN_SetMailboxExtIdent (tCanMailboxIdx uMailboxIdx, u32 ulIdent)
{
    u16 ulTempsRegCopy;

    /* Extract register value */
    ulTempsRegCopy = CAN_MO_AR_ADDR_H(uMailboxIdx);

    /* Reset ID field */
    ulTempsRegCopy &= 0xE000U;

    /* Set Ident + mailbox only receive ext ident */
    ulTempsRegCopy |= (u16)((ulIdent >> CAN_MO_AR_EXTID_SHIFT) & CAN_MO_AR_ID_FIELD);

    /* Copy ID to register */
    CAN_MO_AR_ADDR_H(uMailboxIdx) = ulTempsRegCopy | CAN_MO_AR_PRI_LISTORDER;;
    CAN_MO_AR_ADDR_L(uMailboxIdx) = (u16)(ulIdent & 0x0000FFFF);
}

/* - Fin CAN_SetMailboxExtIdent                                                  */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: CAN_SetMailboxExtRtr                                                    */
/*-------------------------------------------------------------------------------*/
/* Description: Write the RTR bit in a CAN mailbox for extended ID               */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                           : ubRtr: Remote Transmission Request bit            */
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

void CAN_SetMailboxExtRtr (tCanMailboxIdx uMailboxIdx, u8 ubRtr)
{

}

/* - Fin CAN_SetMailboxExtRtr                                                    */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: CAN_SetMailboxExtSRR                                                    */
/*-------------------------------------------------------------------------------*/
/* Description: Write the SRR bit in a CAN mailbox for extended ID               */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                           : ubSrr: Substitute Remote Request bit              */
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

void CAN_SetMailboxExtSrr (tCanMailboxIdx uMailboxIdx, u8 ubSrr)
{

}

/* - Fin CAN_SetMailboxExtSrr                                                    */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: CAN_SetMailboxExtMask                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Configure a CAN mailbox identifier acceptance mask               */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                             uwMask: standard identifier acceptance mask       */
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

#ifdef CAN_MAILBOX_INDIVIDUAL_MASK
void CAN_SetMailboxExtMask (tCanMailboxIdx uMailboxIdx, u32 uwMask)
{
    /* Set Ident + mailbox only receive ext ident */
    CAN_MO_AMR_ADDR_H(uMailboxIdx) = (u16)((uwMask >> CAN_MO_AMR_EXTIDENT_SHIFT) & CAN_MO_AMR_ID_FIELD) | CAN_MO_AMR_MIDE_BIT;
    CAN_MO_AMR_ADDR_L(uMailboxIdx) = (u16)(uwMask & 0x0000FFFF);
}
#endif

/* - Fin CAN_SetMailboxExtMask                                                   */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_SetGeneralStdMask                                                   */
/*-------------------------------------------------------------------------------*/
/* Description: Configure the global standard identifier acceptance masks        */
/*********************************************************************************/
/* Input parameters          : uwMask: standard identifier acceptance mask       */
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

void CAN_SetGeneralExtMask (u32 m_ubCanExtAcceptMask)
{

}

/* - Fin CAN_SetGeneralStdMask                                                   */
/*-------------------------------------------------------------------------------*/



/*********************************************************************************/
/* Name: CAN_GetMailboxExtIdent                                                  */
/*-------------------------------------------------------------------------------*/
/* Description: Read an extended identifier from a CAN mailbox                   */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/* Output parameters         : None                                              */
/* Return value              : ulExtIdent: frame identifier                      */
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

u32 CAN_GetMailboxExtIdent (tCanMailboxIdx uMailboxIdx)
{
    u32 ulTempsRegCopy;

    /* Extract ID value */
    ulTempsRegCopy = (u32)(CAN_MO_AR_ADDR_H(uMailboxIdx) & CAN_MO_AR_ID_FIELD) << CAN_MO_AR_EXTID_SHIFT;
    ulTempsRegCopy |= (CAN_MO_AR_ADDR_L(uMailboxIdx) & 0x0000FFFF);

    return ulTempsRegCopy;
}

/* - Fin CAN_GetMailboxExtIdent                                                  */
/*-------------------------------------------------------------------------------*/

/*********************************************************************************/
/* Name: CAN_GetMailboxExtRtr                                                    */
/*-------------------------------------------------------------------------------*/
/* Description: Read the RTR bit from a extended CAN mailbox                     */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/* Output parameters         : None                                              */
/* Return value              : ubRtr: Remote Transmission Request bit            */
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

u8 CAN_GetMailboxExtRtr (tCanMailboxIdx uMailboxIdx)
{
    /* For the HCS08, the RTR is read only from a reception mailbox              *
     * The reception mailboxes are accessible with the same adresses (FIFO)      */

    /* The CAN mailbox RTR is contained by the CANRIDR3 register                 *
     * CANRIDR1 : 7..1 : identifier(6..0)                                        *
     *            0    : RTR                                                     */

    // This source code is OK only if CAN_FRAME_TYPE_DATA = 0
    // and CAN_FRAME_TYPE_REMOTE = 1
    if (uMailboxIdx == 0x00)
    {

    }

    return ( 0x01);
}

/* - Fin CAN_GetMailboxExtRtr                                                    */
/*-------------------------------------------------------------------------------*/


/*********************************************************************************/
/* Name: CAN_GetMailboxExtSrr                                                    */
/*-------------------------------------------------------------------------------*/
/* Description: Write the SRR bit in a CAN mailbox for extended ID               */
/*********************************************************************************/
/* Input parameters          : uMailboxIdx: mailbox index                        */
/*                           : ubSrr: Remote Transmission Request bit            */
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

u8 CAN_GetMailboxExtSrr (tCanMailboxIdx uMailboxIdx)
{
    /* For the HCS08, the SRR is read only from a reception mailbox              *
     * The reception mailboxes are accessible with the same adresses (FIFO)      */

    /* The CAN mailbox SRR is contained by the CANRIDR1 register                 *
     * CANRIDR1 : 7..5 : identifier(20..18)                                      *
     *            4    : SRR                                                     *
     *            3    : IDE                                                     *
     *            2..0 : identifier(17..15)                                      */

    return 0x00;


}

/* - Fin CAN_GetMailboxExtSrr                                                    */
/*-------------------------------------------------------------------------------*/



/*                                                                               */
/*********************************************************************************/



