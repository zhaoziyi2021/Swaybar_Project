/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       :  ncs_cbk.h
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

#ifndef __NCS_CBK_H__
#define __NCS_CBK_H__

/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* List of NCS callback included into all prj source files                       */

/*critical section API                                                           */
extern void APP_InterruptEnable (u8 ebStatus);
extern u8 APP_InterruptDisable (void);


#endif  /* __NCS_CBK_H__ */
/* End of file */
