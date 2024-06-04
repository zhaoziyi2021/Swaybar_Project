/*
 * Swaybar.h
 *
 *  Created on: 2024-5-30
 *      Author: zhaoziyi
 * 
 */

#ifndef _SWAYBAR_H_
#define _SWAYBAR_H_

#include "MAIN.h"

#define Swaybar_DebugEN             (1)

#define Swaybar_Drv_Link            (0x00)
#define Swaybar_Drv_Hold            (0x01)
#define Swaybar_Drv_Pull0           (0x02)
#define Swaybar_Drv_Pull1           (0x03)
#define Swaybar_Drv_Pull2           (0x04)
#define Swaybar_Drv_Pull3           (0x05)
#define Swaybar_Drv_StuckPull0      (0x06)
#define Swaybar_Drv_StuckPull1      (0x07)
#define Swaybar_Drv_StuckLink0      (0x08)
#define Swaybar_Drv_StuckLink1      (0x09)
#define Swaybar_Drv_DiagCheck       (0xFF)

#define SwaybarError_VoltLow        (0x00000001)
#define SwaybarError_VoltHi         (0x00000002)
#define SwaybarError_CANTimeout     (0x00000004)
#define SwaybarError_TravelSensor   (0x00000008)
#define SwaybarError_DrvVoltErr     (0x00000010)
#define SwaybarError_DrvOpen        (0x00000020)
#define SwaybarError_DrvStuck       (0x00000040)
#define SwaybarError_DrvOverCurrent (0x00000080)
#define SwaybarError_BusOff         (0x00000200)

// Travel_Sensor
// (00 -  19) Link 
// (20 -  80) linking or Pulling
// (81 - 100) Pull
#define PWM_DutyCycle_Link          (30U)
#define PWM_DutyCycle_Pull          (70U)

#define TravelSensorisLink          (IO_Sts.PWM_DutyCycle < PWM_DutyCycle_Link)
#define TravelSensorisOther         ((IO_Sts.PWM_DutyCycle >= PWM_DutyCycle_Link) && (IO_Sts.PWM_DutyCycle <= PWM_DutyCycle_Pull))
#define TravelSensorisPull          (IO_Sts.PWM_DutyCycle > PWM_DutyCycle_Pull)

extern uint8_t DriverState;
extern uint8_t StuckLink;
extern uint8_t StuckPull;

extern void Swaybar_Mainfunction(void);

#endif
