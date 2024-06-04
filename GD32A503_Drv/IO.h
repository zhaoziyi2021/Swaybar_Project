
#ifndef _IO_H_
#define _IO_H_

#include "gd32a50x.h"

#define DRVUPOUT_ADC            (0x00)
#define DRVDOWNOUT_ADC          (0x01)
#define DRV_OUTPUT_CURRENT_ADC  (0x02)
#define UBR_ADC                 (0x03)
#define NTC_ADC                 (0x04)

#define CanStbSleepPin()            gpio_bit_set(GPIOB, GPIO_PIN_12)
#define CanStbWakePin()             gpio_bit_reset(GPIOB, GPIO_PIN_12)

#define INPUT_OUTPUT_EN()           gpio_bit_set(GPIOA, GPIO_PIN_10)
#define INPUT_OUTPUT_DISEN()        gpio_bit_reset(GPIOA, GPIO_PIN_10)

#define DRV_SW_LED_ON()             gpio_bit_set(GPIOA, GPIO_PIN_12)
#define DRV_SW_LED_OFF()            gpio_bit_reset(GPIOA, GPIO_PIN_12)

#define LED0_ON()                   gpio_bit_set(GPIOE, GPIO_PIN_2)
#define LED0_OFF()                  gpio_bit_reset(GPIOE, GPIO_PIN_2)
#define LED1_ON()                   gpio_bit_set(GPIOE, GPIO_PIN_3)
#define LED1_OFF()                  gpio_bit_reset(GPIOE, GPIO_PIN_3)

#define DRV_DOWN_EN_H()             gpio_bit_set(GPIOD, GPIO_PIN_15)
#define DRV_DOWN_EN_L()             gpio_bit_reset(GPIOD, GPIO_PIN_15)
#define DRV_DOWN_10V_H()            gpio_bit_set(GPIOE, GPIO_PIN_11)
#define DRV_DOWN_5V_L()             gpio_bit_reset(GPIOE, GPIO_PIN_11)

#define DRV_UP_EN_H()               gpio_bit_set(GPIOD, GPIO_PIN_14)
#define DRV_UP_EN_L()               gpio_bit_reset(GPIOD, GPIO_PIN_14)
#define DRV_UP_18V_H()              gpio_bit_set(GPIOE, GPIO_PIN_12)
#define DRV_UP_14V_L()              gpio_bit_reset(GPIOE, GPIO_PIN_12)

#define DRV_SELECT_DOWN_H()         gpio_bit_set(GPIOE, GPIO_PIN_15)
#define DRV_SELECT_UP_L()           gpio_bit_reset(GPIOE, GPIO_PIN_15)

#define GET_DRV_SELECT_STS()        gpio_input_bit_get(GPIOE, GPIO_PIN_15)


extern FlagStatus Testcnt[2];

extern void IO_Init(void);
extern void ADC_Init(void);
extern void PWM_Init(void);
extern uword ADC_GetData(ubyte Channel);

#endif  // ifndef _IO_H_
