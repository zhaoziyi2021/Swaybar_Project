/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : UDS_Cfg.h
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


#ifndef UDS_CFG_H
#define UDS_CFG_H



/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Configuration Constants                                                     */




/* Define if NRC 7E is implemented for RC request */
#define RC_NRC_IMPLEMENTATION            UDS_NRC_7E

/* Define NRC78 first timing */
#define UDS_P2            50U / UDS_MANAGE_PERIOD

/* Define NRC78 period */
#define UDS_P2STAR            2000U / UDS_MANAGE_PERIOD

/* SPREC (Session Parameter RECord) functionality */
#define UDS_SPREC   UDS_SPREC_ON

#define UDS_SESSION_DEFAULT            0x01U
#define UDS_SESSION_PROGRAMMING            0x02U
#define UDS_SESSION_EXTENDED_DIAG            0x03U
#define UDS_SESSION_SUPPLIER            0x60U
#define UDS_SESSION_OTHER_01            0x40U
#define UDS_SESSION_OTHER_02            0x41U
#define UDS_SESSION_OTHER_03            0x42U
#define UDS_SESSION_OTHER_04            0x43U

	
	
	


/*-------------------------------------------------------------------------------*/
/* - Configuration Variable or Register Aliases                                  */



/*-------------------------------------------------------------------------------*/
/* - Public Constants                                                            */



/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                */


/*-------------------------------------------------------------------------------*/
/* - Public Macros                                                               */

/*                                                                               */
/*********************************************************************************/

#endif      /* UDS_CFG_H */
