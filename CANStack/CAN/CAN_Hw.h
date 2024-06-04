/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : CAN_Hw.h
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

#ifndef CAN_HW_H
#define CAN_HW_H

#include "XC22xxNREGS.h"

/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                */

#define CAN_MULTICAN_BASE_ADDR      0x200000UL
#define CAN_NODE_BASE_ADDR          0x200U + CAN_MULTICAN_BASE_ADDR
#define CAN_GLOBALMODULE_BASE_ADDR  CAN_MULTICAN_BASE_ADDR
#define CAN_MO_BASE_ADDR            0x1000U + CAN_MULTICAN_BASE_ADDR


/*----------------------------------------*/
/* Global CAN Module registers */
/*----------------------------------------*/

#define CAN_MSID0_ADDR              (*((volatile u16 *)(CAN_GLOBALMODULE_BASE_ADDR + 0x0180U)))
#define CAN_MSPND0_ADDR_L           (*((volatile u16 *)(CAN_GLOBALMODULE_BASE_ADDR + 0x0140U)))
#define CAN_MSPND0_ADDR_H           (*((volatile u16 *)(CAN_GLOBALMODULE_BASE_ADDR + 0x0142U)))

#define CAN_PANCTR_ADDR_L           (*((volatile u16 *)(0x1C4U + CAN_GLOBALMODULE_BASE_ADDR)))
#define CAN_PANCTR_ADDR_H           (*((volatile u16 *)(0x1C6U + CAN_GLOBALMODULE_BASE_ADDR)))
#define CAN_MITR_ADDR               (*((volatile u16 *)(0x1CCU + CAN_GLOBALMODULE_BASE_ADDR)))
#define CAN_MSIMASK_ADDR_L          (*((volatile u16 *)(0x1C0U + CAN_GLOBALMODULE_BASE_ADDR)))
#define CAN_MSIMASK_ADDR_H          (*((volatile u16 *)(0x1C2U + CAN_GLOBALMODULE_BASE_ADDR)))


/* PANCTR_L register details  */
#define CAN_PANCTRL_PANCMD_FIELD        0x00FFU    /* Panel Command */
#define CAN_PANCTRL_BUSY_BIT            0x0100U    /* Panel Busy */
#define CAN_PANCTRL_RBUSY_BIT           0x0200U    /* Result Busy */

/* Command list*/
/* See datasheet ch 21-53 (p1456) */
#define CAN_PANCTRL_CMD_NOOPERATION     0x00U   /* No effect cmd */
#define CAN_PANCTRL_CMD_INITLISTS       0x01U   /* Reset CTRL and LIST field of all message objects */
#define CAN_PANCTRL_CMD_STATICALLOC     0x02U   /* Allocate a given message to a list */


/* PANCTR_H register details  */
#define CAN_PANCTRH_PANAR1_FIELD        0x00FFU /* Panel Argument 1 */
#define CAN_PANCTRH_PANAR2_FIELD        0xFF00U /* Panel Argument 2 */
#define CAN_PANCTRH_PANAR2_SHIFT        8U



/*----------------------------------------*/
/* Mailbox registers */
/*----------------------------------------*/

#define CAN_MO_FCR_ADDR_L(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0000U + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_FCR_ADDR_H(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0002U + ((u16)IdxMessagebox) * 0x20U)))

#define CAN_MO_FGPR_ADDR_L(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0004U + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_FGPR_ADDR_H(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0006U + ((u16)IdxMessagebox) * 0x20U)))

#define CAN_MO_IPR_ADDR_L(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0008U + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_IPR_ADDR_H(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x000AU + ((u16)IdxMessagebox) * 0x20U)))

#define CAN_MO_AMR_ADDR_L(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x000CU + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_AMR_ADDR_H(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x000EU + ((u16)IdxMessagebox) * 0x20U)))

#define CAN_MO_DATA_ADDR_LL(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0010U + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_DATA_ADDR_LH(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0012U + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_DATA_ADDR_HL(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0014U + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_DATA_ADDR_HH(IdxMessagebox)   (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0016U + ((u16)IdxMessagebox) * 0x20U)))

#define CAN_MO_AR_ADDR_L(IdxMessagebox)     (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x0018U + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_AR_ADDR_H(IdxMessagebox)     (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x001AU + ((u16)IdxMessagebox) * 0x20U)))

#define CAN_MO_CTR_ADDR_L(IdxMessagebox)     (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x001CU + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_CTR_ADDR_H(IdxMessagebox)     (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x001EU + ((u16)IdxMessagebox) * 0x20U)))

#define CAN_MO_STAT_ADDR_L(IdxMessagebox)     (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x001CU + ((u16)IdxMessagebox) * 0x20U)))
#define CAN_MO_STAT_ADDR_H(IdxMessagebox)     (*((volatile u16 *)(CAN_MO_BASE_ADDR + 0x001EU + ((u16)IdxMessagebox) * 0x20U)))

#define SetListCommand(ValueH, ValueL) CAN_PANCTRH = ValueH; CAN_PANCTRL = ValueL; while(CAN_PANCTRL & CAN_PANCTR_BUSY);
#define CAN_PANCTR_BUSY       0x0100


/* CAN_MO_CTR_ADDR H and L register details */
/* H register to SET bit */
/* L register to RESET bit */
#define CAN_MO_CTR_RXPND_BIT        0x0001U     /* Receive Pending */
#define CAN_MO_CTR_TXPND_BIT        0x0002U     /* Transmit Pending */
#define CAN_MO_CTR_RXUPD_BIT        0x0004U     /* Receive Updating */
#define CAN_MO_CTR_NEWDAT_BIT       0x0008U     /* New Data */
#define CAN_MO_CTR_MSGLST_BIT       0x0010U     /* Message Lost */
#define CAN_MO_CTR_MSGVAL_BIT       0x0020U     /* Message Valid */
#define CAN_MO_CTR_RTSEL_BIT        0x0040U     /* Receive/Transmit Selected */
#define CAN_MO_CTR_RXEN_BIT         0x0080U     /* Receive Enable */
#define CAN_MO_CTR_TXRQ_BIT         0x0100U     /* Transmit Request */
#define CAN_MO_CTR_TXEN0_BIT        0x0200U     /* Transmit Enable 0 */
#define CAN_MO_CTR_TXEN1_BIT        0x0400U     /* Transmit Enable 1 */
#define CAN_MO_CTR_SETDIR_BIT       0x0800U     /* Message Direction */



/* CAN_MO_FCR_ADDR H register details */
#define CAN_MO_FCRH_RXIE_BIT        0x0001U     /* Receive Interrupt Enable */
#define CAN_MO_FCRH_TXIE_BIT        0x0002U     /* Transmit Interrupt Enable */
#define CAN_MO_FCRH_OVIE_BIT        0x0004U     /* Overflow Interrupt Enable */
#define CAN_MO_FCRH_DLC_FIELD       0x0F00U     /* Data Length Code */


/* CAN_MO_AMR_ADDR H register details */
#define CAN_MO_AMR_MIDE_BIT         0x2000U
#define CAN_MO_AMR_ID_FIELD         0x1FFFU

#define CAN_MO_AMR_STDIDENT_SHIFT   02U
#define CAN_MO_AMR_EXTIDENT_SHIFT   16U


/* CAN_MO_AR_ADDR H register details */
#define CAN_MO_AR_IDE_BIT           0x2000U
#define CAN_MO_AR_PRI_FIELD         0xC000U
#define CAN_MO_AR_PRI_LISTORDER     0x4000U

#define CAN_MO_AR_ID_FIELD          0x1FFFU
#define CAN_MO_AR_EXTID_SHIFT       16U








/*----------------------------------------*/
/* Node CAN controler registers */
/*----------------------------------------*/

#define CAN_CTRL_SHIFT          0x100U

#define CAN_NCR_ADDR            (*((volatile u16 *)(CAN_NODE_BASE_ADDR+ 0x000U + CAN_CTRL_SEL * CAN_CTRL_SHIFT)))
#define CAN_NSR_ADDR            (*((volatile u16 *)(CAN_NODE_BASE_ADDR+ 0x004U + CAN_CTRL_SEL * CAN_CTRL_SHIFT)))
#define CAN_NIPR_ADDR           (*((volatile u16 *)(CAN_NODE_BASE_ADDR+ 0x008U + CAN_CTRL_SEL * CAN_CTRL_SHIFT)))
#define CAN_NBTR_ADDR_H         (*((volatile u16 *)(CAN_NODE_BASE_ADDR+ 0x012U + CAN_CTRL_SEL * CAN_CTRL_SHIFT)))
#define CAN_NBTR_ADDR_L         (*((volatile u16 *)(CAN_NODE_BASE_ADDR+ 0x010U + CAN_CTRL_SEL * CAN_CTRL_SHIFT)))


/* NCR register details */
#define CAN_NCR_INIT_BIT        0x0001U
#define CAN_NCR_TRIE_BIT        0x0002U
#define CAN_NCR_LECIE_BIT       0x0004U
#define CAN_NCR_ALIE_BIT        0x0008U
#define CAN_NCR_CANDIS_BIT      0x0010U
#define CAN_NCR_CCE_BIT         0x0040U
#define CAN_NCR_CALM_BIT        0x0080U
#define CAN_NCR_SUSEN_BIT       0x0100U


/* NSR register details */
#define CAN_NSR_LEC_FIELD       0x0007U /* Last Error Code */
#define CAN_NSR_TXOK_BIT        0x0008U /* Message Transmitted Successfully */
#define CAN_NSR_RXOK_BIT        0x0010U /* Message Received Successfully */
#define CAN_NSR_ALERT_BIT       0x0020U /* Alert Warning */
#define CAN_NSR_EWRN_BIT        0x0040U /* Error Warning Status */
#define CAN_NSR_BOFF_BIT        0x0080U /* Bus-off Status */
#define CAN_NSR_LLE_BIT         0x0100U /* List Length Error */
#define CAN_NSR_LOE_BIT         0x0200U /* List Object Error */
#define CAN_NSR_SUSACK_BIT      0x0400U /* Suspend Acknowledge */



/* NIPRO register details */
#define CAN_NIPR_ALINP_FIELD    0x000FU     /* Alert Interrupt Node Pointer */
#define CAN_NIPR_LECINP_FIELD   0x00F0U     /* Last Error Code Interrupt Node Pointer */
#define CAN_NIPR_TRINP_FIELD    0x0F00U     /* Transfer OK Interrupt Node Pointer */
#define CAN_NIPR_CFCINP_FIELD   0xF000U     /* Frame Counter Interrupt Node Pointer */

/* NBTR_L register details */
#define CAN_NBTRL_BRP_FIELD     0x003FU     /* Baud Rate Prescaler */
#define CAN_NBTRL_SJW_FIELD     0x00C0U     /* (Re)Synchronization Jump Width */
#define CAN_NBTRL_TSEG1_FIELD   0x0F00U     /* Time Segment Before Sample Point */
#define CAN_NBTRL_TSEG2_FIELD   0x7000U     /* Time Segment After Sample Point */
#define CAN_NBTRL_DIV8_FIELD    0x8000U      /* Divide Prescaler Clock by 8*/


/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                */

/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */

/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */

/*-------------------------------------------------------------------------------*/
/* - Public Callback Prototypes                                                  */

/*                                                                               */
/*********************************************************************************/


#endif
