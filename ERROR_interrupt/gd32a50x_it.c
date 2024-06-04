/*!
    \file    gd32a50x_it.c
    \brief   interrupt service routines

    \version 2022-01-30, V1.0.0, firmware for GD32A50x
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32a50x_it.h"
//#include "systick.h"
#include "gd32a50x_fmc.h"
//#include "gd32a503v_eval.h"
#include "MAIN.h"

// /* clear bank0 pengding flags */
// extern void fmc_bank0_flags_clear(void);
// /* clear bank1 pengding flags */
// extern void fmc_bank1_flags_clear(void);


#define FLASH_ECC_CLEAR
#define EEPROM_ECC_CLEAR
#define OB0_ECC_CLEAR
#define OB1_ECC_CLEAR


// /*!
// 	\brief      clear pengding flags
// 	\param[in]  none
// 	\param[out] none
// 	\retval     none
// */
// void fmc_bank1_flags_clear(void)
// {
// 	fmc_flag_clear(FMC_BANK1_FLAG_END);
// 	fmc_flag_clear(FMC_BANK1_FLAG_WPERR);
// 	fmc_flag_clear(FMC_BANK1_FLAG_PGAERR);
// 	fmc_flag_clear(FMC_BANK1_FLAG_PGERR);
// 	fmc_flag_clear(FMC_BANK1_FLAG_PGSERR);
// 	fmc_flag_clear(FMC_BANK1_FLAG_CBCMDERR);
// 	fmc_flag_clear(FMC_BANK1_FLAG_RSTERR);
// }

// /*!
// 	\brief      clear pengding flags
// 	\param[in]  none
// 	\param[out] none
// 	\retval     none
// */
// void fmc_bank0_flags_clear(void)
// {
// 	fmc_flag_clear(FMC_BANK0_FLAG_END);
// 	fmc_flag_clear(FMC_BANK0_FLAG_WPERR);
// 	fmc_flag_clear(FMC_BANK0_FLAG_PGAERR);
// 	fmc_flag_clear(FMC_BANK0_FLAG_PGERR);
// 	fmc_flag_clear(FMC_BANK0_FLAG_PGSERR);
// 	fmc_flag_clear(FMC_BANK0_FLAG_CBCMDERR);
// 	fmc_flag_clear(FMC_BANK0_FLAG_RSTERR);
// }

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/

void NMI_Handler(void)
{
#ifdef FLASH_ECC_CLEAR
    uint32_t err_addr = 0;
    static uint64_t DiagData[16] = {0};

    if((RESET != fmc_flag_get(FMC_FLAG_ECCDET)) && (RESET != syscfg_flag_get(SYSCFG_FLAG_FLASHECCERR))){
        fmc_unlock();
        
        if(fmc_flag_get(FMC_FLAG_BK1ECC)){
            err_addr = 0x8040000 + 8*(FMC_ECCCS & 0x00007FFF);
            err_addr = err_addr/0x400;
            err_addr = err_addr * 0x400;
            // fmc_bank1_flags_clear();
            fmc_page_erase(err_addr);
        } else {
            err_addr = 0x8000000 + 8*(FMC_ECCCS & 0x00007FFF);
            err_addr = err_addr/0x400;
            err_addr = err_addr * 0x400;

            // fmc_bank0_flags_clear();
            fmc_page_erase(err_addr);

            if(err_addr == DIAG_CPID_ADDRESS)
            {
                fmc_fast_program(DIAG_CPID_ADDRESS, DiagData);
            }

        }
        fmc_flag_clear(FMC_FLAG_ECCDET);
        fmc_flag_clear(FMC_FLAG_ECCCOR);
        syscfg_flag_clear(SYSCFG_FLAG_FLASHECCERR);
        fmc_lock();
    }
#endif

#ifdef OB0_ECC_CLEAR
    if(RESET != fmc_interrupt_flag_get(FMC_INT_FLAG_OB0ECCDET) )
	{
		fmc_interrupt_flag_clear(FMC_INT_FLAG_OB0ECCDET);
		fmc_unlock();
		ob_unlock();
		ob_erase();
		/* enable the option bytes programming */
		FMC_CTL1 |= FMC_CTL1_OB0PG;
		/*init OB*/
		REG32(0x1ffff800) = 0x807F5AA5;
		REG32(0x1ffff804) = 0xFFFFFFFF;
		REG32(0x1ffff808) = 0xFFFFFFFF;
		REG32(0x1ffff80c) = 0xFFFFFFFF;
		REG32(0x1ffff810) = 0xFFFFFFFF;		
		/* reset the OB0PG bit */
		FMC_CTL1 &= ~FMC_CTL1_OB0PG;
		// ob_reset();
    }
#endif

    while(1) {
    }
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
// void HardFault_Handler(void)
// {
//     /* if Hard Fault exception occurs, go to infinite loop */
//     while(1) {
//     }
// }

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
// void SysTick_Handler(void)
// {
//     Sys_Tick_Set_Flag = 1;
// }

void FMC_IRQHandler(void)
{
#ifdef EEPROM_ECC_CLEAR
    if(RESET != fmc_interrupt_flag_get(FMC_INT_FLAG_EPECCDET))
	{
        /*restore eeprom data*/
	    fmc_interrupt_flag_clear(FMC_INT_FLAG_EPECCDET);
        fmc_flag_clear(FMC_BANK1_FLAG_RSTERR);
		//while(1);
		fmc_unlock();
		ob_unlock();
		ob1_eeprom_parameter_config(OB1CS_DF_16K_EF_48K, OB1CS_EPSIZE_4K);
		// ob_reset();
	}
#endif

#ifdef OB0_ECC_CLEAR
    if(RESET != fmc_interrupt_flag_get(FMC_INT_FLAG_OB0ECCDET) )
	{
		fmc_interrupt_flag_clear(FMC_INT_FLAG_OB0ECCDET);
		fmc_unlock();
		ob_unlock();
		ob_erase();
		/* enable the option bytes programming */
		FMC_CTL1 |= FMC_CTL1_OB0PG;
		/*init OB*/
		REG32(0x1ffff800) = 0x807F5AA5;
		REG32(0x1ffff804) = 0xFFFFFFFF;
		REG32(0x1ffff808) = 0xFFFFFFFF;
		REG32(0x1ffff80c) = 0xFFFFFFFF;
		REG32(0x1ffff810) = 0xFFFFFFFF;		
		/* reset the OB0PG bit */
		FMC_CTL1 &= ~FMC_CTL1_OB0PG;
		// ob_reset();
    }
#endif

#ifdef OB1_ECC_CLEAR
    if(RESET != fmc_interrupt_flag_get(FMC_INT_FLAG_OB1ECCDET))
	{
		fmc_interrupt_flag_clear(FMC_INT_FLAG_OB1ECCDET);
		fmc_unlock();
		ob_unlock();
		ob1_lock_config(OB1CS_OB1_NOT_LK);
		ob1_epload_config(OB1CS_EPLOAD_LOAD_EPDATA);
		ob1_eeprom_parameter_config(OB1CS_DF_16K_EF_48K,OB1CS_EPSIZE_2K);
		// ob_reset();
	}
#endif


    while(1) {
    }
}