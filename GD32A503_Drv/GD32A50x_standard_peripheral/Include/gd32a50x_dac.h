/*!
    \file    gd32fxxx_dac.h
    \brief   definitions for the DAC

    \version 2021-3-2, V1.0.0, firmware for GD32A50x
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

#ifndef GD32A50X_DAC_H
#define GD32A50X_DAC_H

#include "gd32a50x.h"

/* DAC definitions */
#define DAC                           DAC_BASE                             /*!< DAC base address */

/* registers definitions */
#define DAC_CTL                       REG32(DAC + 0x00000000U)             /*!< DAC control register */
#define DAC_SWT                       REG32(DAC + 0x00000004U)             /*!< DAC software trigger register */
#define OUT_R12DH                     REG32(DAC + 0x00000008U)             /*!< DAC_OUT 12-bit right-aligned data holding register */
#define OUT_L12DH                     REG32(DAC + 0x0000000CU)             /*!< DAC_OUT 12-bit left-aligned data holding register */
#define OUT_R8DH                      REG32(DAC + 0x00000010U)             /*!< DAC_OUT 8-bit right-aligned data holding register */
#define OUT_DO                        REG32(DAC + 0x00000014U)             /*!< DAC_OUT data output register */
#define DAC_STAT                      REG32(DAC + 0x00000018U)             /*!< DAC status register */

/* bits definitions */
/* DAC_CTL */
#define DAC_CTL_DEN                   BIT(0)                               /*!< DAC_OUT enable/disable bit */
#define DAC_CTL_DBOFF                 BIT(1)                               /*!< DAC_OUT output buffer turn on/turn off bit */
#define DAC_CTL_DTEN                  BIT(2)                               /*!< DAC_OUT trigger enable/disable bit */
#define DAC_CTL_DTSEL                 BITS(3,4)                            /*!< DAC_OUT trigger source selection enable/disable bits */
#define DAC_CTL_DWM                   BITS(6,7)                            /*!< DAC_OUT noise wave mode */
#define DAC_CTL_DWBW                  BITS(8,11)                           /*!< DAC_OUT noise wave bit width */
#define DAC_CTL_DDMAEN                BIT(12)                              /*!< DAC_OUT DMA enable/disable bit */
#define DAC_CTL_DDUDRIE               BIT(13)                              /*!< DAC_OUT DMA underrun interrupt enable/disable bit */
#define DAC_CTL_DDISC                 BIT(14)  

/* DAC_SWT */
#define DAC_SWT_SWTR                  BIT(0)                               /*!< DAC_OUT software trigger bit, cleared by hardware */

/* OUT_R12DH */
#define OUT_R12DH_OUT_DH              BITS(0,11)                           /*!< DAC_OUT 12-bit right-aligned data bits */

/* OUT_L12DH */
#define OUT_L12DH_OUT_DH              BITS(4,15)                           /*!< DAC_OUT 12-bit left-aligned data bits */

/* OUT_R8DH */
#define OUT_R8DH_OUT_DH               BITS(0,7)                            /*!< DAC_OUT 8-bit right-aligned data bits */

/* OUT_DO */
#define OUT_DO_OUT_DO                 BITS(0,11)                           /*!< DAC_OUT 12-bit output data bits */

/* DAC_STAT0 */
#define DAC_STAT_DDUDR                BIT(13)                              /*!< DAC_OUT DMA underrun flag */

/* constants definitions */
/* DAC trigger source */
#define CTL_DTSEL(regval)             (BITS(3,4) & ((uint32_t)(regval) << 3))
#define DAC_TRIGGER_EXTRIG            CTL_DTSEL(0)                         /*!< external trigger source */
#define DAC_TRIGGER_SOFTWARE          CTL_DTSEL(1)                         /*!< software trigger */

/* DAC noise wave mode */
#define CTL_DWM(regval)               (BITS(6,7) & ((uint32_t)(regval) << 6))
#define DAC_WAVE_DISABLE              CTL_DWM(0)                           /*!< wave disable */
#define DAC_WAVE_MODE_LFSR            CTL_DWM(1)                           /*!< LFSR noise mode */
#define DAC_WAVE_MODE_TRIANGLE        CTL_DWM(2)                           /*!< triangle noise mode */

/* DAC noise wave bit width */
#define DWBW(regval)                  (BITS(8,11) & ((uint32_t)(regval) << 8))
#define DAC_WAVE_BIT_WIDTH_1          DWBW(0)                              /*!< bit width of the wave signal is 1 */
#define DAC_WAVE_BIT_WIDTH_2          DWBW(1)                              /*!< bit width of the wave signal is 2 */
#define DAC_WAVE_BIT_WIDTH_3          DWBW(2)                              /*!< bit width of the wave signal is 3 */
#define DAC_WAVE_BIT_WIDTH_4          DWBW(3)                              /*!< bit width of the wave signal is 4 */
#define DAC_WAVE_BIT_WIDTH_5          DWBW(4)                              /*!< bit width of the wave signal is 5 */
#define DAC_WAVE_BIT_WIDTH_6          DWBW(5)                              /*!< bit width of the wave signal is 6 */
#define DAC_WAVE_BIT_WIDTH_7          DWBW(6)                              /*!< bit width of the wave signal is 7 */
#define DAC_WAVE_BIT_WIDTH_8          DWBW(7)                              /*!< bit width of the wave signal is 8 */
#define DAC_WAVE_BIT_WIDTH_9          DWBW(8)                              /*!< bit width of the wave signal is 9 */
#define DAC_WAVE_BIT_WIDTH_10         DWBW(9)                              /*!< bit width of the wave signal is 10 */
#define DAC_WAVE_BIT_WIDTH_11         DWBW(10)                             /*!< bit width of the wave signal is 11 */
#define DAC_WAVE_BIT_WIDTH_12         DWBW(11)                             /*!< bit width of the wave signal is 12 */

/* unmask LFSR bits in DAC LFSR noise mode */
#define DAC_LFSR_BIT0                 DAC_WAVE_BIT_WIDTH_1                 /*!< unmask the LFSR bit0 */
#define DAC_LFSR_BITS1_0              DAC_WAVE_BIT_WIDTH_2                 /*!< unmask the LFSR bits[1:0] */
#define DAC_LFSR_BITS2_0              DAC_WAVE_BIT_WIDTH_3                 /*!< unmask the LFSR bits[2:0] */
#define DAC_LFSR_BITS3_0              DAC_WAVE_BIT_WIDTH_4                 /*!< unmask the LFSR bits[3:0] */
#define DAC_LFSR_BITS4_0              DAC_WAVE_BIT_WIDTH_5                 /*!< unmask the LFSR bits[4:0] */
#define DAC_LFSR_BITS5_0              DAC_WAVE_BIT_WIDTH_6                 /*!< unmask the LFSR bits[5:0] */
#define DAC_LFSR_BITS6_0              DAC_WAVE_BIT_WIDTH_7                 /*!< unmask the LFSR bits[6:0] */
#define DAC_LFSR_BITS7_0              DAC_WAVE_BIT_WIDTH_8                 /*!< unmask the LFSR bits[7:0] */
#define DAC_LFSR_BITS8_0              DAC_WAVE_BIT_WIDTH_9                 /*!< unmask the LFSR bits[8:0] */
#define DAC_LFSR_BITS9_0              DAC_WAVE_BIT_WIDTH_10                /*!< unmask the LFSR bits[9:0] */
#define DAC_LFSR_BITS10_0             DAC_WAVE_BIT_WIDTH_11                /*!< unmask the LFSR bits[10:0] */
#define DAC_LFSR_BITS11_0             DAC_WAVE_BIT_WIDTH_12                /*!< unmask the LFSR bits[11:0] */

/* DAC data alignment */
#define DATA_ALIGN(regval)            (BITS(0,1) & ((uint32_t)(regval) << 0))
#define DAC_ALIGN_12B_R               DATA_ALIGN(0)                        /*!< 12-bit right-aligned data */
#define DAC_ALIGN_12B_L               DATA_ALIGN(1)                        /*!< 12-bit left-aligned data */
#define DAC_ALIGN_8B_R                DATA_ALIGN(2)                        /*!< 8-bit right-aligned data */

/* triangle amplitude in DAC triangle noise mode */
#define DAC_TRIANGLE_AMPLITUDE_1      DAC_WAVE_BIT_WIDTH_1                 /*!< triangle amplitude is 1 */
#define DAC_TRIANGLE_AMPLITUDE_3      DAC_WAVE_BIT_WIDTH_2                 /*!< triangle amplitude is 3 */
#define DAC_TRIANGLE_AMPLITUDE_7      DAC_WAVE_BIT_WIDTH_3                 /*!< triangle amplitude is 7 */
#define DAC_TRIANGLE_AMPLITUDE_15     DAC_WAVE_BIT_WIDTH_4                 /*!< triangle amplitude is 15 */
#define DAC_TRIANGLE_AMPLITUDE_31     DAC_WAVE_BIT_WIDTH_5                 /*!< triangle amplitude is 31 */
#define DAC_TRIANGLE_AMPLITUDE_63     DAC_WAVE_BIT_WIDTH_6                 /*!< triangle amplitude is 63 */
#define DAC_TRIANGLE_AMPLITUDE_127    DAC_WAVE_BIT_WIDTH_7                 /*!< triangle amplitude is 127 */
#define DAC_TRIANGLE_AMPLITUDE_255    DAC_WAVE_BIT_WIDTH_8                 /*!< triangle amplitude is 255 */
#define DAC_TRIANGLE_AMPLITUDE_511    DAC_WAVE_BIT_WIDTH_9                 /*!< triangle amplitude is 511 */
#define DAC_TRIANGLE_AMPLITUDE_1023   DAC_WAVE_BIT_WIDTH_10                /*!< triangle amplitude is 1023 */
#define DAC_TRIANGLE_AMPLITUDE_2047   DAC_WAVE_BIT_WIDTH_11                /*!< triangle amplitude is 2047 */
#define DAC_TRIANGLE_AMPLITUDE_4095   DAC_WAVE_BIT_WIDTH_12                /*!< triangle amplitude is 4095 */


/* DAC interrupt */
#define DAC_INT_DDUDRIE               DAC_CTL_DDUDRIE                     /*!< DAC_OUT DMA underrun interrupt enable */

/* DAC interrupt flag */
#define DAC_INT_FLAG_DDUDR            DAC_STAT_DDUDR                      /*!< DAC_OUT DMA underrun interrupt flag */

/* DAC flag */
#define DAC_FLAG_DDUDR                DAC_STAT_DDUDR                      /*!< DAC_OUT DMA underrun flag */

/* function declarations */
/* initialization functions */
/* deinitialize DAC */
void dac_deinit(void);
/* enable DAC */
void dac_enable(void);
/* disable DAC */
void dac_disable(void);
/* enable DAC DMA */
void dac_dma_enable(void);
/* disable DAC DMA */
void dac_dma_disable(void);
/* enable DAC output buffer */
void dac_output_buffer_enable(void);
/* disable DAC output buffer */
void dac_output_buffer_disable(void);
/* get DAC output value */
uint16_t dac_output_value_get(void);
/* set DAC data holding register value */
void dac_data_set(uint32_t dac_align, uint16_t data);

/* DAC trigger configuration */
/* enable DAC trigger */
void dac_trigger_enable(void);
/* disable DAC trigger */
void dac_trigger_disable(void);
/* configure DAC trigger source */
void dac_trigger_source_config(uint32_t triggersource);
/* enable DAC software trigger */
void dac_software_trigger_enable(void);
/* disable DAC software trigger */
void dac_software_trigger_disable(void);

/* DAC wave mode configuration */
/* configure DAC wave mode */
void dac_wave_mode_config(uint32_t wave_mode);
/* configure DAC wave bit width */
void dac_wave_bit_width_config(uint32_t bit_width);
/* configure DAC LFSR noise mode */
void dac_lfsr_noise_config(uint32_t unmask_bits);
/* configure DAC triangle noise mode */
void dac_triangle_noise_config(uint32_t amplitude);

/* interrupt and flag functions */
/* get DAC flag */
FlagStatus dac_flag_get(uint32_t flag);
/* clear DAC flag */
void dac_flag_clear(uint32_t flag);
/* enable DAC interrupt */
void dac_interrupt_enable(uint32_t interrupt);
/* disable DAC interrupt */
void dac_interrupt_disable(uint32_t interrupt);
/* get DAC interrupt flag */
FlagStatus dac_interrupt_flag_get(uint32_t int_flag);
/* clear DAC interrupt flag */
void dac_interrupt_flag_clear(uint32_t int_flag);

#endif /* GD32A50X_DAC_H */
