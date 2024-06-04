/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : EB_Pub.h
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

#ifndef EB_PUB_H
#define EB_PUB_H

#define EB_NO_NRC_78                   U8_MAX

/*-------------------------------------------------------------------------------*/
/* - Public Variables                                                            */

/*-------------------------------------------------------------------------------*/
/* - Public Function Prototypes                                                  */
extern void EB_Init (void);
extern void EB_Manage (void);
/*add by BWF for when receive consecutive request frame only response the first frame*/
extern void EB_TpReqManage(void);
#endif      /* EB_PUB_H */

