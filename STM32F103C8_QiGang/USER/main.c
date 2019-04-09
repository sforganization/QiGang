
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
* ����: 
* ����: 
* �β�:		
* ����: ��
* ˵��: 
*******************************************************************************/
void DebugGPIOInit(void)
{
	/* ����IOӲ����ʼ���ṹ����� */
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��(����)���Ŷ�ӦIO�˿�ʱ��                         + */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


int main(void)
{
	delay_init();									//��ʱ������ʼ��	  
	NVIC_Configuration();							//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 	LED_Init();				 

    DebugGPIOInit();
	//uart_init(115200);								//��ʼ������1������Ϊ115200			
	SysInit();
	GENERAL_TIMx_Configuration();   //ʱ�ӽ��� 1ms TIM2
	
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
* ����: 
* ����: 
* �β�:		
* ����: ��
* ˵��: 
*******************************************************************************/
void SysTickTask(void)
{
	vu16 static 	u16SecTick = 0; 				//�����

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


