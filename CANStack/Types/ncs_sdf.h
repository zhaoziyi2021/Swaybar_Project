/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : ncs_sdf.h
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

#ifndef NCS_SDF_H
#define NCS_SDF_H


/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Header Files: Variable Types                                                */

/*-------------------------------------------------------------------------------*/
/* - Header Files: <NomCouche>                                                   */

/*                                                                               */
/*********************************************************************************/

/****************************** PUBLIC DEFINITIONS *******************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Configuration Constants                                                     */

/*-------------------------------------------------------------------------------*/
/* - Configuration Variable or Register Aliases                                  */

/*-------------------------------------------------------------------------------*/
/* - Public Constants                                                            */

#define SDF_UDS_SESSION_CODE 0x0000U /*Variable of the m_eSessionCode of the UDS layer */
#define SDF_UDS_SESSION_MASK 0x0001U /*Variable of the m_eSessionMask of the UDS layer */
#define SDF_COM_MODE		 0x0010U /*Variable of the m_eComMode of the COM layer */
#define SDF_NM_MODE			 0x0020U /*Variable of the m_eNmMode of the NM layer */
#define SDF_NM_MON_AUTHOR	 0x0021U /*Variable of the m_eMonAuthor of the NM layer */
#define SDF_NM_BUS_STATUS    0x0022U /*Variable of the m_aeBusStatus of the NM layer */
#define SDF_NM_BUSOFF_STATE  0x0023U /*Variable of the m_NmBusOffStateMachine of the NM layer */
#define SDF_NM_FAULT_STATUS  0x0024U /*Variable of the table m_eMonConfStatus of NM layer */
#define SDF_JDD_MODE		 0x0030U /*Variable of the m_eJddMode of the JDD layer */
#define SDF_JDD_MUTE_STATUS  0x0031U /*Variable of the m_eJddMuteStatus of the JDD layer */
#define SDF_OM_OPER_MODE	 0x0040U /*Variable of the m_eOperMode of the OM layer */
#define SDF_OM_OPER_MODE_OLD 0x0041U /*Variable of the m_eOperModeOld of the OM layer */
#define SDF_GAT_MODE		 0x0060U /*Variable of the m_eGatMode of the GAT layer */
#define SDF_GAT_CUR_FUNC_MODE 0x0061U /*Variable of the m_eGatCurFuncMode of the GAT layer */
#define SDF_LGHD_MODE   	 0x0070U /*Variable of the m_eJddMode of the LGHD layer */
#define SDF_LGHD_EVTX		 0x0071U /*Variable of m_ubEvTxOK and  m_eEvtDefTxInProgress of the LGHD layer */

/*SDF 8 bytes variables values*/
#define SDF_U8_VALUE1 0x69U
#define SDF_U8_VALUE2 0x96U
#define SDF_U8_VALUE3 0xC3U
#define SDF_U8_VALUE4 0x3CU
#define SDF_U8_VALUE5 0xAAU
#define SDF_U8_VALUE6 0x55U

/*SDF 16 bytes variables values*/
#define SDF_U16_VALUE1 0x6969U
#define SDF_U16_VALUE2 0x9669U
#define SDF_U16_VALUE3 0xC369U
#define SDF_U16_VALUE4 0x3C69U
#define SDF_U16_VALUE5 0xAA69U
#define SDF_U16_VALUE6 0x5569U
#define SDF_U16_VALUE7 0x6996U
#define SDF_U16_VALUE8 0x9696U
#define SDF_U16_VALUE9 0xC396U
#define SDF_U16_VALUE10 0x3C96U
#define SDF_U16_VALUE11 0xAA96U
#define SDF_U16_VALUE12 0x5596U
#define SDF_U16_VALUE13 0x69C3U
#define SDF_U16_VALUE14 0x96C3U
#define SDF_U16_VALUE15 0xC3C3U
#define SDF_U16_VALUE16 0x3CC3U
#define SDF_U16_VALUE17 0xAAC3U
#define SDF_U16_VALUE18 0x55C3U
#define SDF_U16_VALUE19 0x693CU
#define SDF_U16_VALUE20 0x963CU
#define SDF_U16_VALUE21 0xC33CU
#define SDF_U16_VALUE22 0x3C3CU
#define SDF_U16_VALUE23 0xAA3CU
#define SDF_U16_VALUE24 0x553CU
#define SDF_U16_VALUE25 0x69AAU
#define SDF_U16_VALUE26 0x96AAU
#define SDF_U16_VALUE27 0xC3AAU
#define SDF_U16_VALUE28 0x3CAAU
#define SDF_U16_VALUE29 0xAAAAU
#define SDF_U16_VALUE30 0x55AAU
#define SDF_U16_VALUE31 0x6955U
#define SDF_U16_VALUE32 0x9655U
#define SDF_U16_VALUE33 0xC355U
#define SDF_U16_VALUE34 0x3C55U
#define SDF_U16_VALUE35 0xAA55U
#define SDF_U16_VALUE36 0x5555U


/*-------------------------------------------------------------------------------*/
/* - Public Types                                                                 */

/*-------------------------------------------------------------------------------*/
/* - Public functions                                                                */
extern void NCS_SdfVariableCoherencyError(u16 uwParam);

extern void NCS_SdfVariableRangeError(u16 uwParam);
/*-------------------------------------------------------------------------------*/
/* - Public Macros                                                               */
#ifdef NCS_SDF_ON

    /* This macro describes the variable declaration with its mirror */
    /* Deviation MISRA-1 */
    #define NCS_SDF_DECLARE_VAR_WITH_MIRROR(Type,Var)\
        NCS_PRI_VAR Type Var;\
        NCS_PRI_VAR Type Var##Mirror;\


    /* This macro declare a constant corresponding to a default for a variable */
    /* Deviation MISRA-1 */
    #define NCS_SDF_DECLARE_DEFAULT_VALUE(Type,Var,DefVal)\
    const Type Var##DefaultValue = (DefVal);

    /* This macro declare a reference corresponding to a default for a variable */
    /* Deviation MISRA-1 */
    #define NCS_SDF_REFERENCE_DEFAULT_VALUE(Type,Var)\
    extern const Type Var##DefaultValue;

    /* This macro set the default value of the variable and its mirror *
     * a read modify write operation is done here, be careful to       *
     * be in critical section when using this macro                    */
    /* Deviation MISRA-1 */
    #define NCS_SDF_SET_DEFAULT_VALUE(Var,Type)\
    (Var) = Var##DefaultValue;\
    Var##Mirror = NCS_SDF_GET_##Type##_MIRROR(Var##DefaultValue);\


    /*This macro set the variable value and its mirror */
    /* Deviation MISRA-1 */
    #define NCS_SDF_SET_VAR_VALUE(Var, Val,Type)\
    {\
        u8 eAppStatus_SetVar;\
        eAppStatus_SetVar = APP_InterruptDisable();\
        (Var) = (Val);\
        Var##Mirror = NCS_SDF_GET_##Type##_MIRROR(Val);\
        APP_InterruptEnable(eAppStatus_SetVar);\
    }


    /* This macro check the variable coherency with a mirror, if the variable
    is not coherent it reinitialise the default value and indicate the error
    via the "NCS_SdfVariableCoherencyError" callback*/
    /* Deviation MISRA-1 */
    #define NCS_SDF_CHECK_VAR_COHERENCY_BEGIN(Var,Param,Type)\
        {\
            u8 eAppStatus_Coherency;\
            u8 uMirrorVar;\
            eAppStatus_Coherency = APP_InterruptDisable();\
            uMirrorVar = NCS_SDF_GET_U8_MIRROR(Var##Mirror);\
            if( (Var) != uMirrorVar) \
            { \
                NCS_SDF_SET_DEFAULT_VALUE_##Var(Type); \
                APP_InterruptEnable(eAppStatus_Coherency);\
                NCS_SdfVariableCoherencyError(Param);\
            } \
            else \
            {\
                APP_InterruptEnable(eAppStatus_Coherency);\


    #define NCS_SDF_CHECK_VAR_COHERENCY_END }}


    /* This macro reinitialise the default value and indicate the error via the
    "NCS_SdfVariableRangeError" callback */
    /* Deviation MISRA-1 */
    #define NCS_SDF_VAR_RANGE_ERROR_IND(Var,Param,Type)\
        {\
            u8 eAppStatus_RangeError;\
            eAppStatus_RangeError = APP_InterruptDisable();\
            NCS_SDF_SET_DEFAULT_VALUE_##Var(Type); \
            APP_InterruptEnable(eAppStatus_RangeError);\
            NCS_SdfVariableRangeError(Param);\
        }

#else

    /* Nothing to do */
    /* Deviation MISRA-1 */
    #define NCS_SDF_DECLARE_VAR_WITH_MIRROR(Type,Var)\
        NCS_PRI_VAR Type (Var);

    /* Nothing to do */
    #define NCS_SDF_DECLARE_DEFAULT_VALUE(Type,Var,DefVal)

    /* Nothing to do */
    #define NCS_SDF_REFERENCE_DEFAULT_VALUE(Type,Var)

    /* Nothing to do */
    #define NCS_SDF_SET_DEFAULT_VALUE(Var)

    /*This macro set only the variable value */
    #define NCS_SDF_SET_VAR_VALUE(Var, Val,Type)\
        (Var) = (Val);

    /* Nothing to do */
    #define NCS_SDF_CHECK_VAR_COHERENCY_BEGIN(Var,Param,Type)

    /* Nothing to do */
    #define NCS_SDF_CHECK_VAR_COHERENCY_END

    /* Nothing to do */
    #define NCS_SDF_VAR_RANGE_ERROR_IND(Var,Param,Type)

#endif

/*                                                                               */
/*********************************************************************************/


#endif /* NCS_SDF_H */
