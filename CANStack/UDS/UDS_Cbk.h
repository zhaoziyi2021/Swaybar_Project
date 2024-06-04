/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : UDS_Cbk.h
************************************************************************************************
*   Project/Product :
*   Title           :
*   Author          : BWFENG
************************************************************************************************
*   Description     : This is common module for body network
*
************************************************************************************************
*   Limitations     : NONE
*
************************************************************************************************
*   END_FILE_HDR*/

#ifndef UDS_CBK_H
#define UDS_CBK_H

extern void UDS_SessionStatusInd (tUdsSessionType eUdsSessType);
extern tUdsStatus UDS_RxServiceInd (tUdsSrvIdx uSrvIdx, u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus UDS_LongRequestResponseInd (u16 uwLen, u8 *aubUdsData);
extern void UDS_P2AboutToExpireInd(void);

/* Callback function prototype : Service */
extern tUdsStatus DefaultSession(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ProgrammingSession(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ExtendedDiagnosticSession(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus HardReset(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus KeyOffOnReset(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus SoftReset(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus SecurityLevel1(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus SecurityLevel1_req(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus SecurityLevel2(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus SecurityLevelFBL(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus EnableRxAndTx(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus EnableRxAndDisableTx(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus DisableRxAndEnableTx(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus DisableRxAndTx(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus TesterPresent(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ControlDTCSetting_ON(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ControlDTCSetting_OFF(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ReadMemoryByAddress(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus WriteMemoryByAddress(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ClearDiagnosticInformation(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ReportNumberOfDTCByStatusMask(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ReportDTCByStatusMask(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ReportDTCSnapshotRecordByDTCNumber(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ReportDTCExtendedDataRecordByDTCNumber(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ReportSupportedDTC(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ReportNumberOfEmissionsRelatedOBDDTCByStatusMask(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ReportEmissionsRelatedOBDDTCByStatusMask(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus RequestDownload(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus TransferData(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus RequestTransferExit(u16 *puwLen, u8 *aubUdsData);

/* Callback function prototype : Service_DID */
extern tUdsStatus VINDataIdentifier(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus VINDataIdentifier_2E(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus systemSupplierECUHardwareNumber(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus systemSupplierECUSoftwareNumber(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ECUManufacturingDataDataIdentifier(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus vehicleManufacturerSparePartNumber(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus systemSupplierIdentifierDataIdentifier(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ECUSeriaNumberDataIdentifier(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus RepairShopCodeOrTesterSerialNumberDataIdebtifier(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus RepairShopCodeOrTesterSerialNumberDataIdebtifier_2E(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ECUInstallationDataDataIdentifier(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus vehicleManufacturerECUSoftwareNumberDataIdentifier(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus vehicleManufacturerECUHardwareNumberDataIdentifier(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ECUFunctionConfiguration(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ECUFunctionConfiguration_2E(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ECUInstallationDataDataIdentifier_2E(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus SwaybarControl(u16 *puwLen, u8 *aubUdsData);

/* Callback function prototype : Routine_Control */
extern tUdsStatus EraseMemory_Start(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus EraseMemory_Stop(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus EraseMemory_Result(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus CheckProgrammingDependence_Start(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus CheckProgrammingDependence_Stop(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus CheckProgrammingDependence_Result(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus CheckProgrammingPreconditions_Start(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus CheckProgrammingPreconditions_Stop(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus CheckProgrammingPreconditions_Result(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus BT_CAN_Mapping_Start(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus BT_CAN_Mapping_Stop(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus BT_CAN_Mapping_Result(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus PT_CAN_Mapping_Start(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus PT_CAN_Mapping_Stop(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus PT_CAN_Mapping_Result(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ST_CAN_Mapping_Start(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ST_CAN_Mapping_Stop(u16 *puwLen, u8 *aubUdsData);
extern tUdsStatus ST_CAN_Mapping_Result(u16 *puwLen, u8 *aubUdsData);

/* Callback function prototype : OBD */

#endif      /* UDS_CBK_H */
