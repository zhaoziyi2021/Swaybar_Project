//****************************************************************************
// Include Header Files
//****************************************************************************
#include "MAIN.h"

//****************************************************************************
// Function Declare
//****************************************************************************
// static void delay_1ms(uint32_t count);
void IO_Init(void);
void PWM_Init(void);
void ADC_Init(void);
uword ADC_GetData(ubyte Channel);


//=================================================================================================
// [Function Name] void IO_Init(void)
// [Parameters] none
// [Introduction] Hardware schematic version ASB-V0.3-1
// [Return value] none
// [Creator] 240514 zhaoziyi
//=================================================================================================
// static void delay_1ms(uint32_t count)
// {
//     uword delay = 0;
//     delay = count*1000;
//     while(0U != delay) {
//     }
// }


//=================================================================================================
// [Function Name] void IO_Init(void)
// [Parameters] none
// [Introduction] Hardware schematic version ASB-V0.3-1
// [Return value] none
// [Creator] 240514 zhaoziyi
//=================================================================================================
void IO_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    /* PIN75 PA13 L_INPUT_IO1 */
    // 低有效输入
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_13);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_bit_reset(GPIOA, GPIO_PIN_13);
    /* PIN74 PA12 DRV_LED_OUTPUT_IO1 */
    // 此为输出驱动LED的控制引脚，理论上：拉高此引脚，对外输出低电平。（低有效输出） 
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_bit_reset(GPIOA, GPIO_PIN_12);
    /* PIN10 PA10 12V_Control_EN_IO */
    // 此为INPUT、OUTPUT功能供电使能引脚，当需要做输入采集和输出控制LED时，请使能此引脚；不用时拉低，可降功耗。
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_bit_reset(GPIOA, GPIO_PIN_10);

    rcu_periph_clock_enable(RCU_GPIOB);
    /* CAN0_STB */
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_bit_reset(GPIOB, GPIO_PIN_12);

    rcu_periph_clock_enable(RCU_GPIOD);
    /* PIN65 PD15 BUCK1_EN */
    // 此为降压模块的使能引脚，使能后，降压模块开始工作，才能输出目标电压。
    // 特别注意：BUCK_EN与BOOST_EN不要同时使能，否则容易烧坏PCB。
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
    gpio_bit_reset(GPIOD, GPIO_PIN_15);
    /* BOOST1_EN */
    // 此为升压模块的使能引脚，使能后，升压模块开始工作，才能输出目标电压。
    // 特别注意：BUCK_EN与BOOST_EN不要同时使能，否则容易烧坏PCB。
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_14);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    gpio_bit_reset(GPIOD, GPIO_PIN_14);

    rcu_periph_clock_enable(RCU_GPIOE);
    /* PIN1 PE2 LED0 */
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    gpio_bit_reset(GPIOE, GPIO_PIN_2);
    /* PIN2 PE3 LED1 */
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    gpio_bit_reset(GPIOE, GPIO_PIN_3);
    /* PIN49 PE15 Relay_IO */
    //此为继电器控制引脚，使能后，继电器吸合。继电器吸合前与升压模块连接，继电器吸合后与降压模块连接。需根据升降压的选择进行切换。
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
    gpio_bit_reset(GPIOE, GPIO_PIN_15);
    /* PIN46 PE12 STEPUPCHOOSE_IO */
    //此为升压模块的电压选择引脚，使能前，输出14V电压；使能后，输出18V电压。
    //（但，当前电阻取值有误差，实际为16V和18V）
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_12);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
    gpio_bit_reset(GPIOE, GPIO_PIN_12);
    /* PIN45 PE11 STEPDOWNCHOOSE_IO */
    //此为降压模块的电压选择引脚，使能前，输出5V电压；使能后，输出10V电压。
    //（但，当前电阻取值有误差，实际为5V和10V）
    //后续版本会取消10V输出，后续版本更改时，需注意程序的兼容性。
    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_11);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_bit_reset(GPIOE, GPIO_PIN_11);
} //  End of function IO_Init


//=================================================================================================
// [Function Name] void PWM_Init(void)
// [Parameters] none
// [Introduction] configure the PWM peripheral
// [Return value] none
// [Creator] 20240326 zzy
//=================================================================================================
void PWM_Init(void)
{
    /* TIMER1 configuration: PWM input mode ------------------------
        the external signal is connected to TIMER0_CH3 pin
        the rising edge is used as active edge
        the TIMER0 CH3CV is used to compute the frequency value
        the TIMER0 CH2CV is used to compute the duty cycle value
     ------------------------------------------------------------ */

    timer_ic_parameter_struct timer_icinitpara;
    timer_parameter_struct timer_initpara;

    /* enable ADC0 ADC1 clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /*configure PA0(TIMER0 CH3) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_0);

    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(TIMER0_Channel_IRQn, 1, 1);

    rcu_periph_clock_enable(RCU_TIMER0);
    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler         = 99;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 8060;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    /* TIMER0 configuration */
    /* TIMER0 CH3 PWM input capture configuration */
    timer_channel_input_struct_para_init(&timer_icinitpara);
    timer_icinitpara.icpolarity  = TIMER_IC_POLARITY_RISING;
    timer_icinitpara.icselection = TIMER_IC_SELECTION_DIRECTTI;
    timer_icinitpara.icprescaler = TIMER_IC_PSC_DIV1;
    timer_icinitpara.icfilter    = 0x0;
    timer_input_pwm_capture_config(TIMER0, TIMER_CH_3, &timer_icinitpara);

    /* slave mode selection: TIMER0 */
    timer_input_trigger_source_select(TIMER0, TIMER_SMCFG_TRGSEL_CI3FE3);
    timer_slave_mode_select(TIMER0, TIMER_SLAVE_MODE_RESTART);

    /* select the master slave mode */
    timer_master_slave_mode_config(TIMER0, TIMER_MASTER_SLAVE_MODE_ENABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
    /* clear channel 3 interrupt bit */
    timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_CH3);
    /* channel 3 interrupt enable */
    timer_interrupt_enable(TIMER0, TIMER_INT_CH3);

    /* TIMER1 counter enable */
    timer_enable(TIMER0);
}


//=================================================================================================
// [Function Name] void TIMER0_Channel_IRQHandler(void)
// [Parameters] none
// [Introduction] Interrupt triggering, detecting PWM information from position sensors
// [Return value] none
// [Creator] 20240604 zzy
//=================================================================================================
void TIMER0_Channel_IRQHandler(void)
{
    uint32_t ic1value = 0, ic2value = 0;
  
    if(SET == timer_interrupt_flag_get(TIMER0, TIMER_INT_FLAG_CH3)) {
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER0, TIMER_INT_FLAG_CH3);
        /* read channel 0 capture value */
        ic1value = timer_channel_capture_value_register_read(TIMER0, TIMER_CH_3) + 1;

        if(0 != ic1value) {
            /* read channel 1 capture value */
            ic2value = timer_channel_capture_value_register_read(TIMER0, TIMER_CH_2) + 1;

            /* calculate the duty cycle value */
            IO_Sts.PWM_DutyCycle = (uint8_t)((ic2value * 100) / ic1value);
            /* calculate the frequency value */
            IO_Sts.PWM_Frequency = (uint16_t)(806000 / ic1value);
        }
    }
}


//=================================================================================================
// [Function Name] void ADC_Init(void)
// [Parameters] none
// [Introduction] configure the ADC peripheral
//                PD0   DRVUPOUT_ADC            ADC1_IN4
//                PC12  DRVDOWNOUT_ADC          ADC1_IN5
//                PA3   DRV_OUTPUT_CURRENT_ADC  ADC0_IN11
//                PA4   UBR_ADC                 ADC0_IN10
//                PC11  NTC_ADC                 ADC0_IN0
// [Return value] none
// [Creator] 20240604 zzy
//=================================================================================================
void ADC_Init(void)
{
    /* enable ADC0 ADC1 clock */
    rcu_periph_clock_enable(RCU_ADC0);
    rcu_periph_clock_enable(RCU_ADC1);

    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAHB_DIV10);

    /* reset ADC */
    adc_deinit(ADC0);
    adc_deinit(ADC1);

    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);

    /* ADC continuous function disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, DISABLE);

    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_INSERTED_CHANNEL, 1);
    adc_channel_length_config(ADC1, ADC_INSERTED_CHANNEL, 1);

    /* enable ADC interface */
    adc_enable(ADC0);
    adc_enable(ADC1);

    // delay_1ms(1);

    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    adc_calibration_enable(ADC1);

    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOC);	
    rcu_periph_clock_enable(RCU_GPIOA);	

    /* config the GPIO as analog mode */
    gpio_mode_set(GPIOD, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_12 | GPIO_PIN_11);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_3 | GPIO_PIN_4);
} //  End of function ADC_Init


//=================================================================================================
// [Function Name] uword ADC_GetData(ubyte Channel)
// [Parameters] Channel 
//                PD0   DRVUPOUT_ADC            ADC1_IN4
//                PC12  DRVDOWNOUT_ADC          ADC1_IN5
//                PA3   DRV_OUTPUT_CURRENT_ADC  ADC0_IN11
//                PA4   UBR_ADC                 ADC0_IN10
//                PC11  NTC_ADC                 ADC0_IN0
// [Introduction] AD voltage acquisition
// [Return value] Voltage return value, ten times the true value
// [Creator] 20240604 zzy
//=================================================================================================
uword ADC_GetData(ubyte Channel)
{
    uword RetVolt = 0x0000;
    
    if(Channel > UBR_ADC)
    {
        RetVolt = 0x0000;
    }
    else
    {
        switch(Channel)
        {
            case DRVUPOUT_ADC:
                /* ADC regular channel config */
                adc_regular_channel_config(ADC1, 0U, ADC_CHANNEL_4, ADC_SAMPLETIME_479POINT5); // 采样周期 
                /* ADC software trigger enable */
                adc_software_trigger_enable(ADC1, ADC_REGULAR_CHANNEL); // ADC软件触发使能
                /* ADC external trigger enable */
                adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);
                adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
                /* wait the end of conversion flag */
                while(!adc_flag_get(ADC1, ADC_FLAG_EOC));
                /* clear the end of conversion flag */
                adc_flag_clear(ADC1, ADC_FLAG_EOC);
                /* return regular channel sample value */
                RetVolt = adc_regular_data_read(ADC1);
                RetVolt = (uword)(((ulong)RetVolt*1320>>12) / 10);
                if(RetVolt > 255)
                {
                    RetVolt = 255;
                }

                break;

            case DRVDOWNOUT_ADC:
                /* ADC regular channel config */
                adc_regular_channel_config(ADC1, 0U, ADC_CHANNEL_5, ADC_SAMPLETIME_479POINT5); // 采样周期 
                /* ADC software trigger enable */
                adc_software_trigger_enable(ADC1, ADC_REGULAR_CHANNEL); // ADC软件触发使能
                /* ADC external trigger enable */
                adc_external_trigger_config(ADC1, ADC_REGULAR_CHANNEL, ENABLE);
                adc_external_trigger_source_config(ADC1, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
                /* wait the end of conversion flag */
                while(!adc_flag_get(ADC1, ADC_FLAG_EOC));
                /* clear the end of conversion flag */
                adc_flag_clear(ADC1, ADC_FLAG_EOC);
                /* return regular channel sample value */
                RetVolt = adc_regular_data_read(ADC1);
                RetVolt = (uword)(((ulong)RetVolt*660>>12) / 10);
                if(RetVolt > 255)
                {
                    RetVolt = 255;
                }

                break;

            case DRV_OUTPUT_CURRENT_ADC:
                /* ADC regular channel config */
                adc_regular_channel_config(ADC0, 0U, ADC_CHANNEL_11, ADC_SAMPLETIME_479POINT5); // 采样周期 
                /* ADC software trigger enable */
                adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // ADC软件触发使能
                /* ADC external trigger enable */
                adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
                adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
                /* wait the end of conversion flag */
                while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
                /* clear the end of conversion flag */
                adc_flag_clear(ADC0, ADC_FLAG_EOC);
                /* return regular channel sample value */
                RetVolt = adc_regular_data_read(ADC0);
                RetVolt = ((uint16_t)RetVolt * 3300 / 4096);
                if (RetVolt < 2500)
                {
                    RetVolt = 2500;
                }
                RetVolt = (uint16_t)(( RetVolt - 2500 ) * 5 ) / 2;

                break;
                
            case UBR_ADC:
                /* ADC regular channel config */
                adc_regular_channel_config(ADC0, 0U, ADC_CHANNEL_10, ADC_SAMPLETIME_479POINT5); // 采样周期 
                /* ADC software trigger enable */
                adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // ADC软件触发使能
                /* ADC external trigger enable */
                adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
                adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
                /* wait the end of conversion flag */
                while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
                /* clear the end of conversion flag */
                adc_flag_clear(ADC0, ADC_FLAG_EOC);
                /* return regular channel sample value */
                RetVolt = adc_regular_data_read(ADC0);
                RetVolt = (uword)(((ulong)RetVolt*1980>>12) / 10);
                RetVolt += 3;
                if(RetVolt > 255)
                {
                    RetVolt = 255;
                }

                break;

            case NTC_ADC:
                /* ADC regular channel config */
                adc_regular_channel_config(ADC0, 0U, ADC_CHANNEL_0, ADC_SAMPLETIME_479POINT5); // 采样周期 
                /* ADC software trigger enable */
                adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // ADC软件触发使能
                /* ADC external trigger enable */
                adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
                adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_NONE);
                /* wait the end of conversion flag */
                while(!adc_flag_get(ADC0, ADC_FLAG_EOC));
                /* clear the end of conversion flag */
                adc_flag_clear(ADC0, ADC_FLAG_EOC);
                /* return regular channel sample value */
                RetVolt = adc_regular_data_read(ADC0);

                break;
        }
    }

    return RetVolt;
}


