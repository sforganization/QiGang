
#include "timer.h"

#include "SysComment.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/07
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************	  
extern vu16 	USART2_RX_STA;
extern int		tick;

//定时器4中断服务程序			
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //是更新中断
	{
		USART2_RX_STA		|= 1 << 15; 			//标记接收完成
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); //清除TIM4更新中断标志	  
		TIM_Cmd(TIM4, DISABLE); 					//关闭TIM4 
	}
}


//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
//通用定时器中断初始化
//这里始终选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数		 
void TIM4_Int_Init(u16 arr, u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4时钟使能    

	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; 		//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler = psc;		//设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);		//使能指定的TIM4中断,允许更新中断

	TIM_Cmd(TIM4, ENABLE);							//开启定时器4

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); 				//根据指定的参数初始化VIC寄存器

}


/* 宏定义 --------------------------------------------------------------------*/
/********************通用定时器TIM参数定义，只限TIM2 & TIM3 & TIM4 & TIM5************/
#define GENERAL_TIMx			TIM2
#define GENERAL_TIM_APBxClock_FUN RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK 		RCC_APB1Periph_TIM2
#define GENERAL_TIM_IRQ 		TIM2_IRQn
#define GENERAL_TIM_INT_FUN 	TIM2_IRQHandler



//定时器2中断服务程序			 用于tick时钟
void TIM2_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //是更新中断
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除TIM4更新中断标志	 
		SysTask.u16Tick++;
	}
}


/**
  * 函数功能: 通用定时器 TIMx,x[2,3,4,5]中断优先级配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void GENERAL_TIMx_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 设置中断组为0 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* 设置中断来源 */
	NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ;

	/* 设置主优先级为 0 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

	/* 设置抢占优先级为3 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;

	/*定时器使能 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * 中断周期为 = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
 */
/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{  TIM_Prescaler		   都有
 *	TIM_CounterMode			   TIMx,x[6,7]没有，其他都有
 *	TIM_Period			   都有
 *	TIM_ClockDivision	   TIMx,x[6,7]没有，其他都有
 *	TIM_RepetitionCounter  TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */
void GENERAL_TIMx_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* 开启TIMx_CLK,x[2,3,4,5],即内部时钟CK_INT=72M */
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);

	/* 通用定时器 TIMx,x[2,3,4,5]中断优先级配置 */
	GENERAL_TIMx_NVIC_Configuration();

	/* 自动重装载寄存器周的值(计数值) */
	TIM_TimeBaseStructure.TIM_Period = 1000;

	/* 累计 TIM_Period个频率后产生一个更新或者中断
	   时钟预分频数为71，则驱动计数器的时钟CK_CNT = CK_INT / (71+1)=1M */
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	/* 时钟分频因子 ，没有用到，不用管 */
	//TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

	/* 计数器计数模式，基本定时器TIM6和TIM7只能向上计数，没有计数模式的设置，不用管 
		*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	/* 重复计数器的值，通用定时器没有，不用管 */
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

	/* 初始化定时器TIMx, x[2,3,4,5] */
	TIM_TimeBaseInit(GENERAL_TIMx, &TIM_TimeBaseStructure);

	/* 清除计数器中断标志位 */
	TIM_ClearITPendingBit(GENERAL_TIMx, TIM_IT_Update);

	/* 开启计数器中断 */
	TIM_ITConfig(GENERAL_TIMx, TIM_IT_Update, ENABLE);

	/* 使能计数器: */
	TIM_Cmd(GENERAL_TIMx, ENABLE);
}

















