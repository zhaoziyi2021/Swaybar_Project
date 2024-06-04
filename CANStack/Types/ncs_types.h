/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : ncs_types.h
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


#ifndef __NCS_TYPES_H__
#define __NCS_TYPES_H__

/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Constants                                                            */

/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                */

/* EXACT type definition */
/* Deviation MISRA-1 */
typedef unsigned char           u8;         /* Range : 0..255 */
/* Deviation MISRA-1 */
typedef unsigned short int      u16;        /* Range : 0..65535 */
/* Deviation MISRA-1 */
typedef unsigned long int       u32;        /* Range : 0..4294967295 */
/* Deviation MISRA-1 */
typedef signed char             s8;         /* Range : -128..127 */
/* Deviation MISRA-1 */
typedef signed short int        s16;        /* Range : -32768..32767 */
/* Deviation MISRA-1 */
typedef signed long int         s32;        /* Range : -2147483648..2147483647 */


/* AT LEAST type definition */
/* Deviation MISRA-1 */
typedef unsigned int            u8m;         /* Range : at least 0..255 */
/* Deviation MISRA-1 */
typedef unsigned int            u16m;        /* Range : at least 0..65535 */
/* Deviation MISRA-1 */
typedef unsigned long int       u32m;        /* Range : at least 0..4294967295 */
/* Deviation MISRA-1 */
typedef signed int              s8m;         /* Range : at least -128..127 */
/* Deviation MISRA-1 */
typedef signed int              s16m;        /* Range : at least -32768..32767 */
/* Deviation MISRA-1 */
typedef signed long int         s32m;        /* Range : at least -2147483648..2147483647 */


#ifdef NCS_UNITARY_TEST
/* Const definition into CODE segment */
#define NCS_CONST_SEG
/* data definition into DATA segment */
#define NCS_DATA_SEG
#else
#define NCS_CONST_SEG   /* erasing of 'code' definition */
#define NCS_DATA_SEG    /* Erasing of 'data' definition */
#endif

#ifndef TRUE
    #define TRUE    (1U)
#endif

#ifndef FALSE
    #define FALSE   (0U)
#endif


/* NULL pointer definition if not already defined */
//#ifndef NULL       //changed by bwf
    /* Deviation MISRA-2 */
//    #define NULL   (0)
//#endif

#endif      /* __NCS_TYPES_H__ */
/* End of file */

