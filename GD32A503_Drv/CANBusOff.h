#ifndef _CANBUSOFF_H_
#define _CANBUSOFF_H_

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "main.h"

extern ubyte CAN0BusOffSts;
extern ubyte CAN0BusOffDTCFlag;

extern void BusOffRecovery(void);
extern void Can_GetBusOffStatus(void);

#endif  // ifndef _CANBUSOFF_H_
