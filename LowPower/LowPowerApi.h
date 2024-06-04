/*
 * LowPowerApi.h
 *
 *  Created on: 2015-12-11
 *      Author: ThinkPad
 */

#ifndef LOWPOWERAPI_H_
#define LOWPOWERAPI_H_

// #define  IC_SLEEP_TIMEROUT         200    /*进入低功耗时间*/
#define  IC_SLEEP_TIMEROUT            49     /*进入低功耗时间*/

#define SRC_POWER_ON                           1
#define SRC_KL15                               2
#define SRC_RF                                 3
#define SRC_UDS                                4
#define SRC_RCV_NM_PDU                         5

#define BCM_NOT_READY_FOR_SLEEP                          1
#define BCM_READY_FOR_SLEEP                              0

#define CAN_NOT_READY_FOR_SLEEP                         1
#define CAN_READY_FOR_SLEEP                             0

#define AutosarNM_IN_NO_ACTIVE_CHILD           0
#define AutosarNM_IN_Bus_Sleep_Mode            2
#define AutosarNM_IN_Prepare_Bus_Sleep_Mode    4

typedef enum
{
    ic_wakeup_clear                 = 0x00,
    ic_wakeup_form_acc              = 0x01,
    ic_wakeup_form_ig               = 0x02,
    ic_wakeup_form_hazard           = 0x03,
    ic_wakeup_form_fldoorAjar       = 0x04,
    ic_wakeup_form_frdoorAjar       = 0x05,
    ic_wakeup_form_rldoorAjar       = 0x06,
    ic_wakeup_form_rrdoorAjar       = 0x07,
    ic_wakeup_form_TrunkAjar        = 0x08,
    ic_wakeup_form_HoodAjar         = 0x09,
    ic_wakeup_form_FL_Door_Latch    = 0x0A,
    ic_wakeup_form_FR_Door_Latch    = 0x0B,
    ic_wakeup_form_RL_Door_Latch    = 0x0C,
    ic_wakeup_form_RR_Door_Latch    = 0x0D,
    ic_wakeup_form_centrol_lock     = 0x0E,
    ic_wakeup_form_Trunk_Release_Sw = 0x0F,
    ic_wakeup_form_can              = 0x10,
    ic_wakeup_form_rke              = 0x11,
    ic_wakeup_form_lin              = 0x12,
    ic_wakeup_form_esr              = 0x13,
    ic_wakeup_form_PowerOn          = 0x14,
    ic_wakeup_form_Lowpower         = 0x15,
}IC_WAKEUP_SRC;
extern IC_WAKEUP_SRC ic_wakeup_src;


//=================================================================================================
//���� �� �� �ơ� void LowPowerApiInit(void)
//����       ����
//����       �ܡ� ��ʼ��LowPowerApi
//����   ��  ֵ�� ��
//����   ��  �ߡ� 2018-08-21 ZYW
//=================================================================================================
extern void LowPowerApiInit(void);

//=================================================================================================
//���� �� �� �ơ� void BcanWakeupCheck(void)
//����       ����
//����       �ܡ� BCAN���Ѽ��
//����   ��  ֵ�� None
//����   ��  �ߡ� 2018-06-26 ZYW
//=================================================================================================
extern void BcanWakeupCheck(void);
//=================================================================================================
//���� �� �� �ơ� ubyte CanSleepWakeUpCheck(void)
//����       ����
//����       �ܡ� ���߻��Ѽ��
//����   ��  ֵ�� None
//����   ��  �ߡ� 2018-09-04 ZYW
//=================================================================================================
extern unsigned char CanSleepWakeUpCheck(void);

//=================================================================================================
//���� �� �� �ơ� ubyte BcmEnterSleepCheck(void)
//����       ����
//����       �ܡ� ���߼��
//����   ��  ֵ�� None
//����   ��  �ߡ� 2018-08-23 ZYW
//=================================================================================================
extern ubyte BcmEnterSleepCheck(void);

//=================================================================================================
//���� �� �� �ơ� void BcmEnterSleepDeal(void)
//����       ����
//����       �ܡ� ����
//����   ��  ֵ�� None
//����   ��  �ߡ� 2018-08-23 ZYW
//=================================================================================================
extern void BcmEnterSleepDeal(void);

#endif /* LOWPOWERAPI_H_ */
