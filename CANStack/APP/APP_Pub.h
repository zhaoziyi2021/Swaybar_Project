/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : APP_Pub.h
************************************************************************************************
*   Project/Product :
*   Title           :
*   Author          : BWFENG
************************************************************************************************
*   Description     : This is common module for body netword
*
************************************************************************************************
*   Limitations     : NONE
*
************************************************************************************************
*   END_FILE_HDR*/

#ifndef APP_PUB_H
#define APP_PUB_H

#include "ncs_types.h"
/*declaration range definition*/
#ifdef  _APP_PRG_C
#define _APP_PRG_C_EXT
#else
#define _APP_PRG_C_EXT extern
#endif

/* macro definition */
#define DEFAULTSESSION                             1U
#define PROGRAMMINGSESSION                         2U
#define EXTENDEDSESSION                            3U
#define SECURITY_ACCESS_DENY_TIMEROUT              1000U   //10S
#define TESTER_ABSENT_TIMEROUT                     500U
#define RKE1_LEARNED_TIMEROUT                      3000U
#define RKE2_LEARNED_TIMEROUT                      3000U  //30S
#define DTC_RECORDWAIT_TIMEROUT                    200U  //2S
//#define APP_INFO_ADDRESS	                       0xC4fc00
//#define READ_ONLY_DID_DEBUG
#define APP_INFOCode_ADDRESS	                   0x0803FD00
#define FINGERPRINT_INFO_ADDRESS	               0x0803FE00


#define FLASH_OFFSET_ADDR_CONFIG_DID               6
#define FLASH_OFFSET_ADDR_VEHICLE_NAME             31
#define FLASH_OFFSET_ADDR_SYSTEM_NAME              39
#define FLASH_OFFSET_ADDR_LICENSE_NUM              47
#define FLASH_OFFSET_ADDR_ECU_INSTALLATION_DATE    56
#define FLASH_OFFSET_ADDR_REPAIR_OR_TESTER_SN      64
#define FLASH_OFFSET_ADDR_P_OR_CONFIG_DATE         80
#define FLASH_OFFSET_ADDR_VIN                      84
#define FLASH_OFFSET_ADDR_PROGCOUNTER              101
#define FLASH_OFFSET_ADDR_PROGATTEMP               102

#define FLASH_OFFSET_ADDR_ECUCfg                   103
#define FLASH_OFFSET_ADDR_SWSTS                    107
#define FLASH_OFFSET_ADDR_INIT_FLAG                108

#define NRC22_CONDITION	  	((CAN_Rcv.ABS_3.ABS_VehicleSpeed > (5 * 160 / 9)) && (CAN_Rcv.ABS_3.ABS_VehicleSpeedValid == 0x00))
#define NRC22_FAIL          (1)
#define NRC22_PASS          (0)

/*extern variable*/
_APP_PRG_C_EXT u8 Normal_Message_Transmit_Flg;
_APP_PRG_C_EXT u8 Normal_Message_Receive_Flg;
_APP_PRG_C_EXT u8 NM_Message_Transmit_Flg;
_APP_PRG_C_EXT u8 NM_Message_Receive_Flg;
_APP_PRG_C_EXT u8 SecurityAccessErrCnt;
_APP_PRG_C_EXT u8 Tester_Absent_Flg;
_APP_PRG_C_EXT u8 SecurityLevel1_Access;
_APP_PRG_C_EXT u8 WDT_Disable;

_APP_PRG_C_EXT u8 Diag_SwaybarControlEn;
_APP_PRG_C_EXT u8 Diag_SwaybarControlSts;
_APP_PRG_C_EXT u8 Diag_Time_Pull2;
_APP_PRG_C_EXT u8 Diag_Time_Stuck;

void HardFault_Handler(void);
extern void APP_Manage (void);
/* Application context reloaded from non volatile memory */
extern void APP_RestoreContext (void);
_APP_PRG_C_EXT void Access_Security_PowerInit(void);
#endif      /* APP_PUB_H */

