/*
 * eepromapi.h
 *
 *  Created on: 2015-12-25
 *      Author: ThinkPad
 */

#ifndef EEPROMAPI_H_
#define EEPROMAPI_H_


#define MODEL_SAVE_EE_FILTER_TIMEROUT   50


/*General define*/
#define ON							(1)
#define OFF							(0)

/*EEPROM INIT RETURN*/
#define INIT_SUCCESS				(1)
#define DATA_INCOMPLETE				(2)
#define DATA_EMPTY					(4)
#define DATA_FULL					(8)


typedef struct
{
    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  EMS                 :1;
			ubyte  IPK                 :1;
			ubyte  BCM                 :1;
			ubyte  TCU                 :1;
			ubyte  ACU                 :1;
			ubyte  ESC                 :1;
			ubyte  PEPS                :1;
			ubyte  EPS                 :1;
		}bitsforbyte0;
	}BYTE0;

	union
	{
		ubyte Byte;
		struct
		{
			ubyte  SAS_TCM             :1;
			ubyte  FRS                 :1;
			ubyte  AC                  :1;
			ubyte  MMI                 :1;
			ubyte  PAS                 :1;
			ubyte  TPMS                :1;
			ubyte  T_BOX               :1;
			ubyte  ESCL                :1;
		}bitsforbyte1;
	}BYTE1;

	union
	{
		ubyte Byte;
		struct
		{
			ubyte  EPB                 :1;
			ubyte  FCS                 :1;
			ubyte  PTG                 :1;
			ubyte  DSCU                :1;
			ubyte  AFS_ADB             :1;
			ubyte  ABS                 :1;
			ubyte  IMMO                :1;
			ubyte  c_4WD               :1;
		}bitsforbyte2;
	}BYTE2;

	union
	{
		ubyte Byte;
		struct
		{
			ubyte  PTC                 :1;
			ubyte  EGSM                :1;
			ubyte  BSG                 :1;
			ubyte  BMSH                :1;
			ubyte  DCDC                :1;
			ubyte  VCU                 :1;
			ubyte  OBC                 :1;
			ubyte  IPU                 :1;
		}bitsforbyte3;
	}BYTE3;

	union
	{
		ubyte Byte;
		struct
		{
			ubyte  reserved1           :1;
			ubyte  AVAS                :1;
			ubyte  DVR                 :1;
			ubyte  HVSM                :1;
			ubyte  DMS                 :1;
			ubyte  GW                  :1;
			ubyte  reserved2           :1;
			ubyte  ACCM                :1;
		}bitsforbyte4;
	}BYTE4;

	union
	{
		ubyte Byte;
		struct
		{
			ubyte  AGS                 :1;
			ubyte  AVM                 :1;
			ubyte  HUD                 :1;
			ubyte  DDM                 :1;
			ubyte  PDM                 :1;
			ubyte  PCU                 :1;
			ubyte  TSU                 :1;
			ubyte  RSRS                :1;
		}bitsforbyte5;
	}BYTE5;

	union
	{
		ubyte Byte;
		struct
		{
			ubyte  CHM                 :1;
			ubyte  DMM                 :1;
			ubyte  BMSL                :1;
			ubyte  TACM                :1;
			ubyte  ECS                 :1;
			ubyte  RML                 :1;
			ubyte  SUM                 :1;
			ubyte  IB                  :1;
		}bitsforbyte5;
	}BYTE6;

	union
	{
		ubyte Byte;
		struct
		{
			ubyte  VRFR                :1;
			ubyte  Radio               :1;
			ubyte  OMU                 :1;
			ubyte  BLC_R               :1;
			ubyte  BLC_L               :1;
			ubyte  RCP3                :1;
			ubyte  RCP2                :1;
			ubyte  ALCM                :1;
		}bitsforbyte5;
	}BYTE7;

}VEH_PARAMETER;


typedef struct
{
    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Hazard_Switch_Input                :1;
			ubyte  Reserved                           :1;
			ubyte  Hood_Ajar                          :1;
			ubyte  Trunk_Ajar                         :1;
			ubyte  Rear_Right_Door_Ajar               :1;
			ubyte  Rear_Left_Door_Ajar                :1;
			ubyte  Front_Right_Door_Ajar              :1;
			ubyte  Front_Left_Door_Ajar               :1;
		}bits;
	}Door_Ajar_Switch_Input_Status;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Driver_Door_Lock_Status_Switch                :1;
			ubyte  Front_Right_Door_Lock_Status_Switch           :1;
			ubyte  Rear_Left_Door_Lock_Status_Switch             :1;
			ubyte  Rear_Right_Door_Lock_Status_Switch            :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
			ubyte  Central_Lock_Unlock_Switch                    :1;
			ubyte  Reserved2                                     :1;
		}bits;
	}Door_Lock_Unlock_Switch_Input_Status;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  ACC_Switch_Input                              :1;
			ubyte  Ignition_1_Switch_Input                       :1;
			ubyte  Mirror_Fold_Unfold_Switch_Input               :1;
			ubyte  Front_Wiper_Low_Speed_Switch_Input            :1;
			ubyte  Low_Beam_Switch_Input                         :1;
			ubyte  Reverse_Switch_Input                          :1;
			ubyte  Brake_Switch_Input                            :1;
			ubyte  Reserved                                      :1;
		}bits;
	}IGN_Other_Switch_Input_Status;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Reserved0                                     :1;
			ubyte  Central_Lock_Relay_Output                     :1;
			ubyte  Central_Unlock_Relay_Output                   :1;
			ubyte  Trunk_Release_Relay_Output                    :1;
			ubyte  CDL_Status_LED                                :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
			ubyte  Reserved3                                     :1;
		}bits;
	}Lock_Unlock_Output_Status;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Battery_Saver_Relay_Output                    :1;
			ubyte  Interior_Lamps_Output                         :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
			ubyte  Reserved3                                     :1;
			ubyte  Reserved4                                     :1;
			ubyte  Reserved5                                     :1;
		}bits;
	}Illuminated_Entry_Battery_Saver_Output_Status;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Alarm_Horn_Output                             :1;
			ubyte  Buzzer_Output                                 :1;
			ubyte  ATWS_LED                                      :1;
			ubyte  Mirror_Fold_Relay_Output                      :1;
			ubyte  Mirror_Unfold_Relay_Output                    :1;
			ubyte  Power_Window_Enable_Status_Output             :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
		}bits;
	}Horn_Others_Output_Status;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Reverse_Light_Relay_Output                    :1;
			ubyte  Front_Fog_Light_Output                        :1;
			ubyte  Rear_Fog_Light_Output                         :1;
			ubyte  Left_DRL_Light                                :1;
			ubyte  Low_Beam_Output                               :1;
			ubyte  Position_Light                                :1;
			ubyte  Right_DRL_Light                               :1;
			ubyte  High_Beam_Output                              :1;
		}bits;
	}Exterior_Light_Output_Status;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Front_Wiper_Power_OnOff_Relay_Output          :1;
			ubyte  Front_Wiper_HighLow_Speed_Relay_Output        :1;
			ubyte  Front_Washer_Relay_Output                     :1;
			ubyte  Rear_Wiper_Relay_Output                       :1;
			ubyte  Headlamp_Washer_Output                        :1;
			ubyte  Rear_Washer_Relay_Output                      :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
		}bits;
	}Wiper_Washer_Output_Status;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Left_Cornering_Light                          :1;
			ubyte  Right_Cornering_Light                         :1;
			ubyte  Brake_Light                                   :1;
			ubyte  Right_Turn_Lamp_Output                        :1;

			/*Adaptive model 024 025 027��Modified By Zhang Yuwen Start*/
			ubyte  Front_fog_Cornering_left_light                :1;
			ubyte  Front_fog_Cornering_right_light               :1;
			/*Adaptive model 024 025 027��Modified By Zhang Yuwen End*/

			ubyte  Reserved2                                     :1;
			ubyte  Left_Turn_Lamp_Output                         :1;
		}bits;
	}Turn_Corner_Brake_Lamp_Output_Status;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Left_Cornering_Light                          :1;
			ubyte  Right_Cornering_Light                         :1;
			ubyte  Brake_Lamp                                    :1;
			ubyte  Right_Turn_Lamp                               :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
			ubyte  Left_Turn_Lamp                                :1;
		}bits;
	}Turn_Lamp_ControlBrake_Lamp;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Reverse_Light                                 :1;
			ubyte  Front_Fog_Light                               :1;
			ubyte  Rear_Fog_Light                                :1;
			ubyte  Left_Day_Time_Running_Light                   :1;
			ubyte  Low_Beam_Light_Relay                          :1;
			ubyte  High_Beam_Relay                               :1;
			ubyte  Right_Day_Time_Running_Light                  :1;
			ubyte  Position_Light                                :1;
		}bits;
	}Light_Control;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Front_Washer                                  :1;
			ubyte  Headlamp_Washer                               :1;
			ubyte  Rear_Wiper                                    :1;
			ubyte  Rear_Washer                                   :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
			ubyte  Front_Wiper_Fast_Speed_Mode                   :1;
			ubyte  Front_Wiper_Slow_Speed_Mode                   :1;
		}bits;
	}Wiper_Control;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Buzzer                                        :1;
			ubyte  Alarm_Horn                                    :1;
			ubyte  ATWS_LED                                      :1;
			ubyte  Reserved0                                     :1;
			ubyte  Mirror_Fold_Output                            :1;
			ubyte  Mirror_Unfold_Output                          :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
		}bits;
	}ATWS_Control;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Reserved0                                     :1;
			ubyte  CDL_Status_LED                                :1;
			ubyte  Central_Door_Lock                             :1;
			ubyte  Central_Door_Unlock                           :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
			ubyte  Reserved3                                     :1;
			ubyte  Trunk_Release_Relay                           :1;
		}bits;
	}Door_Control;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Interior_Lamp                                 :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
			ubyte  Reserved3                                     :1;
			ubyte  Reserved4                                     :1;
			ubyte  Reserved5                                     :1;
			ubyte  Battery_Saver_Relay_BSR                       :1;
		}bits;
	}Interior_Lamp_BSR_Control ;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Window_Eanble_Output                          :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
			ubyte  Reserved3                                     :1;
			ubyte  Reserved4                                     :1;
			ubyte  Reserved5                                     :1;
			ubyte  Reserved6                                     :1;
		}bits;
	}Window_Control;
}DYNAMIC_DATA;

typedef struct
{
    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Left_Cornering_Light                          :1;
			ubyte  Right_Cornering_Light                         :1;
			ubyte  Brake_Lamp                                    :1;
			ubyte  Right_Turn_Lamp                               :1;

			/*Adaptive model 024 025 027��Modified By Zhang Yuwen Start*/
			ubyte  Front_fog_Cornering_left_light                :1;
			ubyte  Front_fog_Cornering_right_light               :1;
			/*Adaptive model 024 025 027��Modified By Zhang Yuwen End*/

			ubyte  Reserved2                                     :1;
			ubyte  Left_Turn_Lamp                                :1;
		}bits;
	}Turn_Lamp_ControlBrake_Lamp;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Reverse_Light                                 :1;
			ubyte  Front_Fog_Light                               :1;
			ubyte  Rear_Fog_Light                                :1;
			ubyte  Left_Day_Time_Running_Light                   :1;
			ubyte  Low_Beam_Light_Relay                          :1;
			ubyte  High_Beam_Relay                               :1;
			ubyte  Right_Day_Time_Running_Light                  :1;
			ubyte  Position_Light                                :1;
		}bits;
	}Light_Control;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Front_Washer                                  :1;
			ubyte  Headlamp_Washer                               :1;
			ubyte  Rear_Wiper                                    :1;
			ubyte  Rear_Washer                                   :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
			ubyte  Front_Wiper_Fast_Speed_Mode                   :1;
			ubyte  Front_Wiper_Slow_Speed_Mode                   :1;
		}bits;
	}Wiper_Control;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Buzzer                                        :1;
			ubyte  Alarm_Horn                                    :1;
			ubyte  ATWS_LED                                      :1;
			ubyte  Reserved0                                     :1;
			ubyte  Mirror_Fold_Output                            :1;
			ubyte  Mirror_Unfold_Output                          :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
		}bits;
	}ATWS_Control;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Reserved0                                     :1;
			ubyte  CDL_Status_LED                                :1;
			ubyte  Central_Door_Lock                             :1;
			ubyte  Central_Door_Unlock                           :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
			ubyte  Reserved3                                     :1;
			ubyte  Trunk_Release_Relay                           :1;
		}bits;
	}Door_Control;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Interior_Lamp                                 :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
			ubyte  Reserved3                                     :1;
			ubyte  Reserved4                                     :1;
			ubyte  Reserved5                                     :1;
			ubyte  Battery_Saver_Relay_BSR                       :1;
		}bits;
	}Interior_Lamp_BSR_Control ;

    union
	{
		ubyte Byte;
	    struct
		{
			ubyte  Window_Eanble_Output                          :1;
			ubyte  Reserved0                                     :1;
			ubyte  Reserved1                                     :1;
			ubyte  Reserved2                                     :1;
			ubyte  Reserved3                                     :1;
			ubyte  Reserved4                                     :1;
			ubyte  Reserved5                                     :1;
			ubyte  Reserved6                                     :1;
		}bits;
	}Window_Control;

	ubyte  Turn_Lamp_ControlBrake_Lamp_Enable;
	ubyte  Light_Control_Enable;
	ubyte  Wiper_Control_Enable;
	ubyte  ATWS_Control_Enable;
	ubyte  Door_Control_Enable;
	ubyte  Interior_Lamp_BSR_Control_Enable;
	ubyte  Window_Control_Enable;

}IO_CONTROL;


extern VEH_PARAMETER VEH_Parameter;
extern uword EEpromInitSatus;

extern ubyte Page1DataBuffer[128];
extern ubyte EepromPageBuffer0[128];
extern ubyte EepromPageBuffer1[128];

extern ubyte WriteEEromPatameter(uword *wdatapoint,ubyte pageadd);
extern ubyte ReadEEromPatameter(uword *rdatapoint,ubyte pageadd);
extern ubyte CalcChecksum(ubyte Data_Length, ubyte *Data_Buffer);
extern ulong ReadProgrammCounter(ubyte Page_Num);
extern void Eeprom0ProgrammCounter(void);
extern void Eeprom1ProgrammCounter(void);
extern ubyte ReadConfigDID(void);
extern ubyte ReadParaFromFlash(ushort *puwLen, ubyte *aubUdsData,ubyte offset_addr, ubyte length);
extern ubyte WriteParaToFlash(ushort *puwLen, ubyte *aubUdsData,ubyte offset_addr, ubyte length);
extern void WriteModelMemoryStateDeal(void);
extern ubyte ReadModelMemoryStateDeal(void);
extern ubyte WriteEepromSecurityAccessErrCnt(void);
extern ubyte ReadEepromSecurityAccessErrCnt(void);
/*EEPROM Page1 related function end*/

/*Read dynamic data function start*/
extern void ReadDoorAjarSwitchInputStatus(ushort *puwLen, ubyte *aubUdsData);
extern void ReadDoorLockUnlockSwitchInputStatus(ushort *puwLen, ubyte *aubUdsData);
extern void ReadIGNOtherSwitchInputStatus(ushort *puwLen, ubyte *aubUdsData);
extern void ReadLockUnlockOutputStatus(ushort *puwLen, ubyte *aubUdsData);
extern void ReadIlluminatedEntryBatterySaverOutputStatus(ushort *puwLen, ubyte *aubUdsData);
extern void ReadHornOthersOutputStatus(ushort *puwLen, ubyte *aubUdsData);
extern void ReadExteriorLightOutputStatus(ushort *puwLen, ubyte *aubUdsData);
extern void ReadWiperWasherOutputStatus(ushort *puwLen, ubyte *aubUdsData);
extern void ReadTurnCornerBrakeLampOutputStatus(ushort *puwLen, ubyte *aubUdsData);
/*Read dynamic data function end*/

/*IO control function start*/
extern void RTETurnLampControlBrakeLamp(ushort *puwLen, ubyte *aubUdsData);
extern void RTELampControl(ushort *puwLen, ubyte *aubUdsData);
extern void RTEWiperControl(ushort *puwLen, ubyte *aubUdsData);
extern void RTEATWSControl(ushort *puwLen, ubyte *aubUdsData);
extern void RTEDoorControl(ushort *puwLen, ubyte *aubUdsData);
extern void RTEInteriorLampBSRControl(ushort *puwLen, ubyte *aubUdsData);
extern void RTEWindowControl(ushort *puwLen, ubyte *aubUdsData);
extern void IOControl(void);
extern void IOControlClrReturn(void);
/*IO control function end*/

extern void SoftwareReset(void);



#endif /* EEPROMAPI_H_ */
