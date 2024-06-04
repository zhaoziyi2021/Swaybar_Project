/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : UDS_Cfg.c
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


/******************************** FILE INCLUSION *********************************/
/*                                                                               */
/*-------------------------------------------------------------------------------*/
/* - Header Files: UDS                                                           */
#include "EB_Prj.h"
/*                                                                               */
/*********************************************************************************/

/* Diagnostic services configuration 1, stored in Read-Only memory */
const tUdsSrvCfg1 m_astDiagSrvCfg1[UDS_SRV_CFG1_MAX] =
{
    {UDS_SRV_DSC, 0x01U, 0x7U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, DefaultSession},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_DSC, 0x02U, 0x6U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ProgrammingSession},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_DSC, 0x03U, 0x5U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ExtendedDiagnosticSession},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_ER, 0x01U, 0x7U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, HardReset},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_ER, 0x02U, 0x7U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, KeyOffOnReset},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_ER, 0x03U, 0x7U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, SoftReset},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_SA, 0x01U, 0x4U, 2U, UDS_ADDR_PHYSICAL, SecurityLevel1},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_SA, 0x02U, 0x4U, 6U, UDS_ADDR_PHYSICAL, SecurityLevel1_req},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_SA, 0x03U, 0x4U, 2U, UDS_ADDR_PHYSICAL, SecurityLevel2},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_SA, 0x11U, 0x2U, 2U, UDS_ADDR_PHYSICAL, SecurityLevelFBL},    /*  UDS_SESSION_PROGRAMMING_MASK */
    {UDS_SRV_TP, 0x00U, 0x5U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, TesterPresent},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RTE, 0x00U, 0x2U, 1U, UDS_ADDR_PHYSICAL_FUNCTIONAL, RequestTransferExit},    /*  UDS_SESSION_PROGRAMMING_MASK */
    {UDS_SRV_TD, 0x00U, 0x2U, 0U, UDS_ADDR_PHYSICAL_FUNCTIONAL, TransferData},    /*  UDS_SESSION_PROGRAMMING_MASK */
    {UDS_SRV_RD, 0x00U, 0x2U, 11U, UDS_ADDR_PHYSICAL_FUNCTIONAL, RequestDownload},    /*  UDS_SESSION_PROGRAMMING_MASK */
    {UDS_SRV_RMBA, 0x00U, 0x4U, 8U, UDS_ADDR_PHYSICAL, ReadMemoryByAddress},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_WMBA, 0x00U, 0x4U, 0U, UDS_ADDR_PHYSICAL, WriteMemoryByAddress},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDTCI, 0x01U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ReportNumberOfDTCByStatusMask},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDTCI, 0x02U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ReportDTCByStatusMask},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDTCI, 0x04U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ReportDTCSnapshotRecordByDTCNumber},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDTCI, 0x06U, 0x5U, 6U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ReportDTCExtendedDataRecordByDTCNumber},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDTCI, 0x0aU, 0x5U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ReportSupportedDTC},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDTCI, 0x12U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ReportNumberOfEmissionsRelatedOBDDTCByStatusMask},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDTCI, 0x13U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ReportEmissionsRelatedOBDDTCByStatusMask},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_CDTCI, 0x00U, 0x5U, 4U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ClearDiagnosticInformation},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_CDTCS, 0x01U, 0x4U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ControlDTCSetting_ON},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_CDTCS, 0x02U, 0x4U, 2U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ControlDTCSetting_OFF},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_CC, 0x00U, 0x4U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, EnableRxAndTx},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_CC, 0x01U, 0x4U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, EnableRxAndDisableTx},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_CC, 0x02U, 0x4U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, DisableRxAndEnableTx},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_CC, 0x03U, 0x4U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, DisableRxAndTx},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
};

/* Diagnostic services configuration 2, stored in Read-Only memory */
const tUdsSrvCfg2 m_astDiagSrvCfg2[UDS_SRV_CFG2_MAX] =
{
    /*Diag 0x22 Services*/
    {UDS_SRV_RDBI, 0xf187U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, vehicleManufacturerSparePartNumber},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xf18AU, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, systemSupplierIdentifierDataIdentifier},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xf18bU, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ECUManufacturingDataDataIdentifier},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xf18cU, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ECUSeriaNumberDataIdentifier},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xf190U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, VINDataIdentifier},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xf192U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, systemSupplierECUHardwareNumber},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xf194U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, systemSupplierECUSoftwareNumber},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xf198U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, RepairShopCodeOrTesterSerialNumberDataIdebtifier},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xf19dU, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ECUInstallationDataDataIdentifier},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xF188U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, vehicleManufacturerECUSoftwareNumberDataIdentifier},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xF191U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, vehicleManufacturerECUHardwareNumberDataIdentifier},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RDBI, 0xF191U, 0x5U, 3U, UDS_ADDR_PHYSICAL_FUNCTIONAL, ECUFunctionConfiguration},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    /*Diag 0x2E Services*/
    {UDS_SRV_WDBI, 0xf190U, 0x4U, 20U, UDS_ADDR_PHYSICAL, VINDataIdentifier_2E},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_WDBI, 0xf198U, 0x4U, 3U, UDS_ADDR_PHYSICAL, RepairShopCodeOrTesterSerialNumberDataIdebtifier_2E},    /* UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_WDBI, 0xf19dU, 0x4U, 7U,  UDS_ADDR_PHYSICAL, ECUInstallationDataDataIdentifier_2E},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_WDBI, 0xF101U, 0x4U, 7U, UDS_ADDR_PHYSICAL, ECUFunctionConfiguration_2E},    /* UDS_SESSION_DEFAULT_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    /*Diag 0x2F Services */
    {UDS_SRV_IOCBI, 0xf011U, 0x4U, 7U,  UDS_ADDR_PHYSICAL, SwaybarControl},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
};

/* Diagnostic services configuration 3, stored in Read-Only memory */
const tUdsSrvCfg3 m_astDiagSrvCfg3[UDS_SRV_CFG3_MAX] =
{
    {UDS_SRV_RC, 0x01U, 0xff00U, 0x6U, 0U, UDS_ADDR_PHYSICAL, EraseMemory_Start},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x02U, 0xff00U, 0x6U, 0U, UDS_ADDR_PHYSICAL, EraseMemory_Stop},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x03U, 0xff00U, 0x6U, 0U, UDS_ADDR_PHYSICAL, EraseMemory_Result},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x01U, 0xff01U, 0x6U, 0U, UDS_ADDR_PHYSICAL, CheckProgrammingDependence_Start},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x02U, 0xff01U, 0x6U, 0U, UDS_ADDR_PHYSICAL, CheckProgrammingDependence_Stop},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x03U, 0xff01U, 0x6U, 0U, UDS_ADDR_PHYSICAL, CheckProgrammingDependence_Result},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x01U, 0xff02U, 0x6U, 0U, UDS_ADDR_PHYSICAL, CheckProgrammingPreconditions_Start},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x02U, 0xff02U, 0x6U, 0U, UDS_ADDR_PHYSICAL, CheckProgrammingPreconditions_Stop},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x03U, 0xff02U, 0x6U, 0U, UDS_ADDR_PHYSICAL, CheckProgrammingPreconditions_Result},    /*  UDS_SESSION_PROGRAMMING_MASK | UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x01U, 0xaffcU, 0x4U, 4U, UDS_ADDR_PHYSICAL, BT_CAN_Mapping_Start},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x02U, 0xaffcU, 0x4U, 4U, UDS_ADDR_PHYSICAL, BT_CAN_Mapping_Stop},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x03U, 0xaffcU, 0x4U, 4U, UDS_ADDR_PHYSICAL, BT_CAN_Mapping_Result},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x01U, 0xaffdU, 0x4U, 4U, UDS_ADDR_PHYSICAL, PT_CAN_Mapping_Start},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x02U, 0xaffdU, 0x4U, 4U, UDS_ADDR_PHYSICAL, PT_CAN_Mapping_Stop},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x03U, 0xaffdU, 0x4U, 4U, UDS_ADDR_PHYSICAL, PT_CAN_Mapping_Result},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x01U, 0xaffeU, 0x4U, 4U, UDS_ADDR_PHYSICAL, ST_CAN_Mapping_Start},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x02U, 0xaffeU, 0x4U, 4U, UDS_ADDR_PHYSICAL, ST_CAN_Mapping_Stop},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
    {UDS_SRV_RC, 0x03U, 0xaffeU, 0x4U, 4U, UDS_ADDR_PHYSICAL, ST_CAN_Mapping_Result},    /*  UDS_SESSION_EXTENDED_DIAG_MASK */
};

/* Diagnostic session timeout */
/* Range : (0 .. 2^32) */
const tUdsSessionTimeout m_uUdsSessTimeout = UDS_SESS_TIMEOUT;

/* Diagnostic services number of the configuration 1,2 and 3
   managed by this software layer Range : (0 .. 2^32) */
const tUdsSrvIdx  m_uUdsSrvCfg1Max = UDS_SRV_CFG1_MAX;
const tUdsSrvIdx  m_uUdsSrvCfg2Max = UDS_SRV_CFG2_MAX;
const tUdsSrvIdx  m_uUdsSrvCfg3Max = UDS_SRV_CFG3_MAX;
const tUdsSrvIdx  m_uUdsSrvCfg4Max = UDS_SRV_CFG4_MAX;
const tUdsSrvIdx  m_uUdsSrvCfg5Max = UDS_SRV_CFG5_MAX;

/* Declare the default value of the m_eSessionCode variable */
const tUdsSessionProtType m_eSessionCodeDefaultValue = UDS_SESSION_PROT_DEFAULT;

/* Declare the default value of the m_eSessionMask variable */
const tUdsSessionProtMask m_eSessionMaskDefaultValue = UDS_SESSION_PROT_DEFAULT_MASK;

