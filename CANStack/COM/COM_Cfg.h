/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : COM_Cfg.h
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

#ifndef COM_CFG_H
#define COM_CFG_H
/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Customer Configuration Constants                                            */


/*-------------------------------------------------------------------------------*/
/* - NCS Configuration Constants                                                 */

/* Communication periodic or mixed frames in reception number managed by this software layer */
/* Range : (0 .. 2^32) */
#define COM_RX_PERIODIC_FRAME_MAX        0U

/* Interaction Layer integration */
#define COM_WITH_IL        COM_TRUE
/*-------------------------------------------------------------------------------*/
/* - Configuration Variable or Register Aliases                                  */

/*-------------------------------------------------------------------------------*/
/* - Public Constants                                                            */

/* COM frames index definition (COM_Cfg.h) */
/**********************************************
*         Frames for configuration: 0*
**********************************************/
#define COM_FRAME_IDX_Diag_BCM_RQ_0x770                  0U
#define COM_FRAME_IDX_Diag_Fun_0x7DF                     1U
#define COM_FRAME_IDX_Diag_BCM_Rsp_0x778                 2U


#define COM_NODE_IDX_Monitoring_0                  0U

/* Management of the check consistency frame */
#define FRAME_CONSISTENCY_CHECK FRAME_CONSISTENCY_CHECK_ON 


/*-------------------------------------------------------------------------------*/
/* - Public Macros                                                               */

/*                                                                               */
/*********************************************************************************/





#endif      /* COM_CFG_H */
