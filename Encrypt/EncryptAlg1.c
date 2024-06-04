/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : EncryptAlg1.c
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
#ifndef _ENCRYPTALG1_C
#define _ENCRYPTALG1_C
#endif

/*Include files*/
#include "EncryptAlg1.h"

u32 Key_Maker_A01(u32 seed );

//=================================================================================================
// [Function Name] u32 Key_Maker_A01(u32 seed )
// [Parameters] seed
// [Introduction] Security unlocking algorithm formula
// [Return value] key
// [Creator] 20240410 zzy
//=================================================================================================
u32 Key_Maker_A01(u32 seed )
{
    u8 i = 0;
    u32 k = 0;
    u32 mask_A01_ECU = 0x9823b6e0;

    if (!((seed == 0) || (seed == 0xFFFFFFFF)))
    {
        for (i=0; i<35; i++)
        {
            if (seed & 0x80000000)
            {
                seed = seed << 1;
                seed = seed ^ (u32)mask_A01_ECU;
            }
            else
            {
                seed = seed <<1;
            }

            k = seed;
        }
    }

    return (k);
}