/**
  ******************************************************************************
  * @file    MAIN.c
  * @author  zzy
  * @version V1.0.0
  * @date    19-03-2024
  * @brief   main function
  ******************************************************************************
  */

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "MAIN.h"

static void MAIN_vInit(void);
static void systick_config(void);
void systick_config(void);
void FlashEep_Init(void);
void SysTick_Handler(void);

//=================================================================================================
// [Function Name] void FlashEep_Init(void)
// [Parameters] None
// [Introduction] main function init
// [Return value] None
// [Creator]
//=================================================================================================
void FlashEep_Init(void)
{
    /******************EEPROM state monitor********************/
    /*Before powering on and using EEPROM, check if the flag is set to 1
        If the flag position is 1, it is necessary to clear the flag bit
        and change the eeprom configuration, so that the eeprom data will
        be cleared, then restarted and changed back*/
    /* if FMC_STAT1-> RSTERR is 1,eeprom will can't write new data */
    if(fmc_flag_get(FMC_BANK1_FLAG_RSTERR) == 0x01U)
    {
        /* clean RSTERR flag */
        fmc_flag_clear(FMC_BANK1_FLAG_RSTERR);
        /* reconfiguration eeprom size to init eeprom data */
        fmc_unlock();
        ob_unlock();
        /* configure the data flash size as 48k and EEPROM backup size as 16k */
        if ((16U * 0x400U) != dflash_size_get())
        {
            ob1_eeprom_parameter_config(OB1CS_DF_16K_EF_48K, OB1CS_EPSIZE_4K);
            /*reset mcu to reload option byte*/
            ob_reset();
        }
    }
    else
    {
        /* configure EEPROM backup size as 64K*/
        if (0x00000000U != dflash_size_get())
        {
            fmc_unlock();
            ob_unlock();
            ob1_eeprom_parameter_config(OB1CS_DF_0K_EF_64K, OB1CS_EPSIZE_4K);
            /*reset mcu to reload option byte*/
            ob_reset();
        }
    }

    /* configure the shared RAM as EEPROM SRAM */
    fmc_sram_mode_config(EEPROM_SRAM_MODE);
}


//=================================================================================================
// [Function Name] void MAIN_vInit(void)
// [Parameters] None
// [Introduction] main function init
// [Return value] None
// [Creator] 
//=================================================================================================
static void MAIN_vInit(void)
{
    __set_PRIMASK(1); 
    systick_config();

    IO_Init();
    ADC_Init();
    PWM_Init();
    CAN_vInit();
    EB_Init();

    ic_wakeup_src = (IC_WAKEUP_SRC)rcu_flag_get(RCU_FLAG_SWRST);
    if(0 != rcu_flag_get(RCU_FLAG_SWRST)) 
    {
        rcu_all_reset_flag_clear();
        // BcmEnterSleepDeal();
        ic_wakeup_src = ic_wakeup_form_Lowpower;
    }

    WDT_Disable = 0;
    fwdgt_config(50, FWDGT_PSC_DIV64);
    systick_config();

    __set_PRIMASK(0);
}


//=================================================================================================
// [Function Name] void systick_config(void)
// [Parameters] None
// [Introduction] configure systick
// [Return value] None
// [Creator]
//=================================================================================================
static void systick_config(void)
{
    /* setup systick timer for 200Hz interrupts */
    if(SysTick_Config(SystemCoreClock / 200U)) {
        /* capture error */
        while(1) {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}


//=================================================================================================
// [Function Name] void SysTick_Handler(void)
// [Parameters] None
// [Introduction] this function handles SysTick exception
// [Return value] None
// [Creator]
//=================================================================================================
void SysTick_Handler(void)
{
    Sys_Tick_Set_Flag = 1;
}


//=================================================================================================
// [Function Name] void main(void)
// [Parameters] None
// [Introduction] main function
// [Return value] None
// [Creator]
//=================================================================================================
void main(void)
{
    MAIN_vInit();
    VariableInitialization();

    // fwdgt_enable();

    while(1)
    {
        Run_Task();
    }

}

