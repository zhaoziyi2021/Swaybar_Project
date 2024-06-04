/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : EncryptAlg1.h
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
#ifndef ENCRYPTALG1_H
#define ENCRYPTALG1_H


/*Include files*/
#include "ncs.h"

/*declaration range definition*/
#ifdef  _ENCRYPTALG1_C
#define _ENCRYPTALG1_C_EXT
#else
#define _ENCRYPTALG1_C_EXT extern
#endif


/* macro definition */


/* variable definition */


/* function declaration */
_ENCRYPTALG1_C_EXT u32 Key_Maker_A01(u32 seed );



#endif	/* end of header file */
