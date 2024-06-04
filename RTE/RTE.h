/*
 *  RTE.h
 *
 *  Created on: 20240319
 *      Author: zzy
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

typedef struct
{
	ubyte Read_Dtc_Failer;
	ubyte Control_DTC_Setting;
	ubyte Lin_Sleep_Flag;
	//ubyte WakeUp_Can;
	ubyte WakeUp_Dtc_BD;
	ubyte WakeUp_Dtc_PT;
	ubyte WakeUp_Dtc;

}SYSTEM_CONTROL;

extern SYSTEM_CONTROL System_Control;
extern ubyte CfgDID_VDEMEn;

extern void Delay(uword delay);
extern void RTE(void);
extern void VariableInitialization(void);

#endif /* INTERFACE_H_ */
