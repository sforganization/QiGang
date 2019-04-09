
#include "delay.h"
#include "sys.h"
#include "usart2.h"
#include "usart.h"

#include "timer.h"
#define _MAININC_
#include "SysComment.h"
#undef _MAININC_
void SysTickTask(void);

/*******************************************************************************
* 名称: 
* 功能: 
* 形参:		
* 返回: 无
* 说明: 
*******************************************************************************/
void DebugGPIOInit(void)
{
	/* 定义IO硬件初始化结构体变量 */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能(开启)引脚对应IO端口时钟                         + */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


int main(void)
{
	delay_init();									//延时函数初始化	  
	NVIC_Configuration();							//设置NVIC中断分组2:2位抢占优先级，2位响应优先级 	LED_Init();				 

    DebugGPIOInit();
	//uart_init(115200);								//初始化串口1波特率为115200			
	SysInit();
	GENERAL_TIMx_Configuration();   //时钟节拍 1ms TIM2
	
	while (1)
	{
		MainTask();

		if (SysTask.u16Tick)
		{
			SysTask.u16Tick--;
			SysTickTask();
		}
	}
}


/*******************************************************************************
* 名称: 
* 功能: 
* 形参:		
* 返回: 无
* 说明: 
*******************************************************************************/
void SysTickTask(void)
{
	vu16 static 	u16SecTick = 0; 				//秒计数

    u8 static state = 0;
	if (u16SecTick++ >= 1000)
	{
		u16SecTick			= 0;
	}
    
    if(SysTask.u32LuoLiaolWaitTime)  //
        SysTask.u32LuoLiaolWaitTime--;

        
    if(SysTask.u32TuiLiaolWaitTime)  //
        SysTask.u32TuiLiaolWaitTime--;

}


