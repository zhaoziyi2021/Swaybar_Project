/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : CAN_Types.h
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

#ifndef CAN_TYPES_H
#define CAN_TYPES_H

/* CAN mailbox index type definition */
typedef u8 tCanMailboxIdx;

typedef u8 tCanCtrlStatus;
#define CAN_BUS_OFF	0U
#define CAN_BUS_ON	1U

/*add by bwf start*/
#define SOFT_TIMER_STOP	    0U
#define SOFT_TIMER_START	1U
#define BUSOFF_TIME_OVER    8U
/*add by bwf end*/

#define CAN_DATA_MAX                    8U

// CAN protocol handler mailbox number
#define CAN_MAILBOX_NB                  3U

// Protocol handler Mailbox number used for transmission
// Range : (1 .. CAN_MAILBOX_NB) : imposed by hardware
//         0 : user configuration
#define CAN_TX_MAILBOX_NB               0U

// Protocol handler Mailbox number used for reception
// Range : (1 .. CAN_MAILBOX_NB) : imposed by hardware
//         0 : user configuration
#define CAN_RX_MAILBOX_NB               0U

#define CAN_WAKE_UP_ACTIVATE      1
#define CAN_WAKE_UP_DESACTIVATE      0

/* Upper layers automatic configuration with these next constants */
#define CAN_WAKE_UP_DETECTION      CAN_WAKE_UP_DESACTIVATE
/* CAN protocol handler mailboxes with individual mask (one mask per mailbox) :
 declare this constant if this functionnality is needed */
#define CAN_MAILBOX_INDIVIDUAL_MASK


#endif      /* CAN_TYPES_H */
