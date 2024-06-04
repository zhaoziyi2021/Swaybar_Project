/**
  ******************************************************************************
  * @file    MAIN.h
  * @author  zzy
  * @version V1.0.0
  * @date    19-03-2024
  * @brief   main function
  ******************************************************************************
  */

#ifndef _MAIN_H_
#define _MAIN_H_

#include  "XC22xxNREGS.h"
#include  "IO.h"
#include  "CAN.h"
#include  "gd32a50x.h"
#include  "LowPowerApi.h"
#include  "Service.h"
#include  "TaskStep.h"
#include  "RTE.h"
#include  "DTC.h"
#include  <string.h>
#include  "SoftTimerDrv.h"
#include  "EB_Prj.h"
#include  "EncryptAlg1.h"
#include  "eepromapi.h"
#include  "gd32a50x_fmc.h"
#include  "Swaybar.h"
#include  "CANBusOff.h"

#define far  __far
#define _inline  inline
#define _interrupt  __interrupt
#define DIAG_CPID_ADDRESS 0x0803A000

extern void FlashEep_Init(void);

#endif
