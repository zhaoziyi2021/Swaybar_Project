/*
 * @Author: ShiZhan shizhan@senpujie.com
 * @Date: 2024-05-10 11:55:30
 * @LastEditors: ShiZhan shizhan@senpujie.com
 * @LastEditTime: 2024-06-04 09:14:52
 * @FilePath: \Swaybar_Project\Service\Srv_IO.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef SRV_IO_C
#define SRV_IO_C

/* include files */
#include "main.h"

IO_INPUT_STATUS IO_Input_Status;
IO_Sts_Type IO_Sts;
void SrvIO_Mainfunction(void);


//=================================================================================================
// [Function Name] void SrvIOInit(void)
// [Parameters] None
// [Introduction] 
// [Return value] None
// [Creator]
//=================================================================================================
void SrvIOInit(void)
{
    (void) memset((void *) &IO_Input_Status, 0, sizeof(IO_INPUT_STATUS));
    (void) memset((void *) &IO_Sts,          0, sizeof(IO_Sts_Type));
}


//=================================================================================================
// [Function Name] static void DioFilter(ubyte dio_in, ubyte *local_count, 
//						ubyte *local_result, ubyte filter_times)
// [Parameters] 
// [Introduction] 
// [Return value] None
// [Creator]
//=================================================================================================
static void DioFilter(ubyte dio_in, ubyte *local_count, ubyte *local_result, ubyte filter_times)
{
    if (dio_in != 0)
    {
        if (*local_count < filter_times)
        {
            *local_count += 1;
        }
        else
        {
            *local_result = 1;
        }
    }
    else
    {
        if (*local_count > 1)
        {
            *local_count -= 1;
        }
        else
        {
            *local_result = 0U;
        }
    }
}


//=================================================================================================
// [Function Name] void GetDigtalIOState(void)
// [Parameters] None
// [Introduction] 
// [Return value] None
// [Creator]
//=================================================================================================
void GetDigtalIOState(void)
{
    if(IO_Input_Status.Init_Ready == 0)
    {
        if(ChkSoftTimerState(IO_Init_Timer) == SOFT_TIMER_ST_REACH)
        {
            SoftTimerStop(IO_Init_Timer);
            IO_Input_Status.Init_Ready = 1;
        }

        if(IO_Input_Status.Init_Ready == 0)
        {
            if(ChkSoftTimerState(IO_Init_Timer) != SOFT_TIMER_ST_RUNING)
            {
                SoftTimerStart(IO_Init_Timer, IO_INPUT_FILTER_READY_TIME);
            }
        }
    }

    DioFilter(gpio_input_bit_get(GPIOA, GPIO_PIN_12), &IO_Input_Status.SwaybarSW.count, &IO_Input_Status.SwaybarSW.value, IO_INPUT_FILTER_NUM);
    DioFilter(gpio_input_bit_get(GPIOA, GPIO_PIN_13), &IO_Input_Status.Reserved2.count, &IO_Input_Status.Reserved2.value, IO_INPUT_FILTER_NUM);
}


//=================================================================================================
// [Function Name] ubyte SrvIO_Mainfunction(void)
// [Parameters] None
// [Introduction] 5ms Task
// [Return value] None
// [Creator] 20240517 zzy
//=================================================================================================
void SrvIO_Mainfunction(void)
{
    // Battery voltage ,uint 0.1V
    IO_Sts.Ad_Voltage = (ubyte)ADC_GetData(UBR_ADC);

    // Drv Volt ,uint 0.1V
    IO_Sts.Ad_DrvUpOut = (ubyte)ADC_GetData(DRVUPOUT_ADC);
    IO_Sts.Ad_DrvDownOut = (ubyte)ADC_GetData(DRVDOWNOUT_ADC);
    if(SET == GET_DRV_SELECT_STS())
    {
        IO_Sts.Ad_DrvOutput = IO_Sts.Ad_DrvDownOut;
    }
    else
    {
        IO_Sts.Ad_DrvOutput = IO_Sts.Ad_DrvUpOut;
    }

    // Drv Cnrrent ,uint 1mA
    IO_Sts.Ad_DrvOutputCnrrent = ADC_GetData(DRV_OUTPUT_CURRENT_ADC);
}

#endif	/* end of source file */
