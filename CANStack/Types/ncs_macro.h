/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       :  ncs_macro.h
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

#ifndef __NCS_MACRO_H__
#define __NCS_MACRO_H__




/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Public Macros                                                               */




/******************** UNITARY TEST QUALIFIER MODIFICATION ************************/
/* To be abble to access simply private function or data during function unitary */
/* test step.                                                                    */
/* "NCS_UNITARY_TEST" should de defined only for this step by compiler command   */
/* line setting.                                                                 */

#ifdef NCS_UNITARY_TEST
    #define NCS_PRI_FCT
    #define NCS_PRI_VAR
#else
    #define NCS_PRI_FCT   static
    #define NCS_PRI_VAR   static
#endif




/***************************** BITFIELD MANIPULATION *****************************/
/* These 4 macros work on any size variable compatible with bitfield operations  */
/* Id est unsigned types such as u8, u16 or u32 types.                           */
/* Var       parameter stands for the variable used                              */
/* BitField  parameter stands for the list of bits to work on                    */
/* Type      parameter stands for the type of the variable used                  */


/* To set some bits into a variable (those set into BitField *
 * a read modify write operation is done here, be careful to *
 * be in critical section when using this macro              */
#define NCS_SET_BIT(Var,BitField,Type)      ((Var) |= (Type)(BitField))

/* To reset some bits into a variable (those set into BitField *
 * a read modify write operation is done here, be careful to  *
 * be in critical section when using this macro               */
#define NCS_RESET_BIT(Var,BitField,Type)    ((Var) &= (Type)~(Type)(BitField))

/* To test to 1 all selected bits into a variable (those set into BitField */
#define NCS_TST_BIT_SET(Var,BitField,Type)  (0U != ((Var) & (Type)(BitField)))

/* To test to 0 all selected bits into a variable (those set into BitField */
#define NCS_TST_BIT_RESET(Var,BitField,Type)  (0U == ((Var) & (Type)(BitField)))



/************************ MIRRORING ENCODING AND CHECKING ************************/
/* These 6 macros are used for variable protection in memory with a mirrorring   */

/* 8 bits mirror checking                                    *
 * a read modify write operation is done here, be careful to *
 * be in critical section when using this macro              */
#define NCS_SDF_CHECK_U8_MIRROR(val1, val2)\
((u8)(val1) == ((u8)((u8)(val2) ^ 0xFFU)))\

/* 8 bits mirror encoding                                      *
 * when using this macro, be careful to be in critical section *
 * between write the variable and its mirror                   */
#define NCS_SDF_GET_U8_MIRROR(val) ((u8)((u8)(val) ^ 0xFFU))

/* 16 bits mirror checking                                   *
 * a read modify write operation is done here, be careful to *
 * be in critical section when using this macro              */
#define NCS_SDF_CHECK_U16_MIRROR(val1, val2)\
((u16)(val1) == ((u16)((u16)(val2) ^ 0xFFFFU)))\

/* 16 bits mirror encoding                                     *
 * when using this macro, be careful to be in critical section *
 * between write the variable and its mirror                   */
#define NCS_SDF_GET_U16_MIRROR(val) ((u16)((u16)(val) ^ 0xFFFFU))

/* 32 bits mirror checking                                   *
 * a read modify write operation is done here, be careful to *
 * be in critical section when using this macro              */
#define NCS_SDF_CHECK_U32_MIRROR(val1, val2) \
((u32)(val1) == (u32)((u32)(val2) ^ 0xFFFFFFFFU))\

/* 32 bits mirror encoding                                     *
 * when using this macro, be careful to be in critical section *
 * between write the variable and its mirror                   */
#define NCS_SDF_GET_U32_MIRROR(val) ((u32)((u32)(val) ^ 0xFFFFFFFFU))

/*                                                                               */
/*********************************************************************************/

#endif      /* __NCS_MACRO_H__ */
/* End of file */

