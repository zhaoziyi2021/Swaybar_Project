/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : IL_Pub.h
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

#ifndef IL_PUB_H
#define IL_PUB_H

/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                */

#define IL_TRUE                   (tIlCANBoolean)1U
#define IL_FALSE                  (tIlCANBoolean)0U

/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */

extern void IL_Init(void);

#define IL_EnableEvtFrame(ubState)
extern void IL_RxDataInd (tComFrameIdx uFrameIdx);



/*************************************************
*                  ComConfig 0                   *
*************************************************/ 
/*-------------------------------------------------------------------------------*/
/* - Rx frames                                                                   */

  
/*-------------------------------------------------------------------------------*/
/* - Tx frames                                                                   */


#endif /* #define IL_PUB_H */
