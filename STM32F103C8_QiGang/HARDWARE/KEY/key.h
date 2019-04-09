#ifndef __KEY_H__
#define __KEY_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* 类型定义 --------------------------------------------------------------*/
typedef enum
{
  KEY_UP   = 1,
  KEY_DOWN = 0,
}KEYState_TypeDef;

/* 宏定义 --------------------------------------------------------------------*/
/* 1 2 3 4 是锁开关 5 是屏幕左边 6是中间 7  是右边*/
#define KEY1_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define KEY1_GPIO_PIN                 GPIO_Pin_3
#define KEY1_GPIO                     GPIOB
#define KEY1_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为高电平，所以这里设置为0 */

#define KEY2_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define KEY2_GPIO_PIN                 GPIO_Pin_4
#define KEY2_GPIO                     GPIOB
#define KEY2_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */


#define KEY3_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define KEY3_GPIO_PIN                 GPIO_Pin_5
#define KEY3_GPIO                     GPIOB
#define KEY3_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */


#define KEY4_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define KEY4_GPIO_PIN                 GPIO_Pin_6
#define KEY4_GPIO                     GPIOB
#define KEY4_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */


#define KEY5_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB 
#define KEY5_GPIO_PIN                 GPIO_Pin_7
#define KEY5_GPIO                     GPIOB
#define KEY5_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */


#define KEY6_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define KEY6_GPIO_PIN                 GPIO_Pin_8
#define KEY6_GPIO                     GPIOB
#define KEY6_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */


#define KEY7_RCC_CLOCKGPIO            RCC_APB2Periph_GPIOB
#define KEY7_GPIO_PIN                 GPIO_Pin_9
#define KEY7_GPIO                     GPIOB
#define KEY7_DOWN_LEVEL               0  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */





/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void KEY_GPIO_Init(void);
#if 0
KEYState_TypeDef KEY1_StateRead(void);
KEYState_TypeDef KEY2_StateRead(void);
#endif

#endif  // __KEY_H__

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
