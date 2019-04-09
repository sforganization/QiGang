
#include "SysComment.h"
#include "stm32f10x_it.h"
#include "stm32f10x_iwdg.h"
#include "san_flash.h"
#include "usart2.h"




#define LUO_LIAO_PAST_TIME		150  //500ms 延时时间 落料分料通过延时
#define LUO_LIAO_WAIT_TIME		1000  //1000ms 延时时间 前端落料延时
#define TUI_LIAO_WAIT_TIME		1000  //700ms 延时时间 等料斗落料延时
#define TUI_WAN_WAIT_TIME		600   //600ms 延时时间 推到尽头
#define TUI_OVER_TIME			4000   //4000ms 推料超时 报警
#define CUI_QI_WAIT				80		// 50ms 吹气延时
#define REPET_WAIT				200		// 50ms 吹气延时


/*******************************************************************************
* 名称: 报警任务
* 功能: 如果推料传感器末端4S后感应不到 输出报警 推料气缸回位
* 形参:		
* 返回: 无
* 说明: 
*******************************************************************************/
void BaoJingTask(void)
{
	if(InputRead(TUI_LIAO_SENSOR_B_PIN_READ)&&!(SysTask.u32TuiLiaolWaitTime))
	{
		BAO_JING_ON();
		TUI_LIAO_OFF();
		LUO_LIAO_ON();
		ZHEN_PAN_OFF();
#if ENABLE_DEBUG
		u2_printf("卡料报警！\n");
#endif

		while(InputRead(BAO_JING_IN_READ))
        {
            ;
        }      
#if ENABLE_DEBUG
		u2_printf("报警解除。\n");
#endif
		BAO_JING_OFF();
		LUO_LIAO_OFF();
		ZHEN_PAN_ON();
		SysTask.TuiLiaoState = TUIL_WAIT;
	}
}
/*******************************************************************************
* 名称: 振盘任务
* 功能: 如果推料传感器回退到原始位置启动振盘
* 形参:		
* 返回: 无
* 说明: 
*******************************************************************************/
void ZhenPanTask(void)
{
    
#if ENABLE_DEBUG
                    u8 static u8State = 0;
#endif

	if ((SysTask.LuoLiaoState == LUOL_LEFT) || (SysTask.LuoLiaoState == LUOL_WAIT) || (SysTask.LuoLiaoState == LUOL_REPET)) //落料已经完成,不论什么状态都停止振盘
	{
       /* if(u8State){
            u8State = 0;  
		SysTask.u32LuoLiaolWaitTime=3000;
		}
		if (SysTask.u32LuoLiaolWaitTime) //等待延时时间到来
		{		*/
#if ENABLE_DEBUG
        if(u8State){
            u8State = 0;
				u2_printf("---振盘stop---\n");
            }
#endif
	//	u2_printf("---振盘stop---");
		//u2_printf(SysTask.u32LuoLiaolWaitTime);
		ZHEN_PAN_OFF();
		//return;
	//	}
	}
	else 
	{
#if ENABLE_DEBUG
        if(!u8State){
            u8State = 1;
				u2_printf("****振盘start****\n");
            }
#endif
		ZHEN_PAN_ON();
	}
}


/*******************************************************************************
   * 名称: 上料传感器任务
   * 功能: 由传感器驱动状态迁移
   * 形参:		
   * 返回: 无
   * 说明: 
   *******************************************************************************/
void LuoLiaoSensorTask(void)
{
	switch (SysTask.LuoLiaoState)
	{
		case LUOL_INIT: //初始状态
			if (!InputRead(LUO_LIAO_SENSOR_PIN_READ)) //传感器感应到东西
			{
#if ENABLE_DEBUG
				u2_printf("Luo : 传感器感应到东西\n");
#endif

				SysTask.u8LuoLiaoComplte = 0;		//还没有完成落料

				SysTask.LuoLiaoState = LUOL_PAST1;	//先控制分料气缸落到右边

				//digitalWrite(LUO_LIAO_SENSOR_PIN, HIGH);
			}
			SysTask.u32LuoLiaolWaitTime = LUO_LIAO_PAST_TIME ;

			break;
//
		case LUOL_PAST1: //等待通过
		


			//SysTask.u32LuoLiaolWaitTime = LUO_LIAO_PAST_TIME ;

			if (SysTask.u32LuoLiaolWaitTime) //等待延时时间到来
				return;

#if ENABLE_DEBUG
			u2_printf("1 等50Ms\n");
#endif

				SysTask.LuoLiaoState = LUOL_RIGHT;
#if ENABLE_DEBUG
			u2_printf("Luo : 控制分料气缸落到右边\n");
#endif
				break;
				
//
		case LUOL_RIGHT: //控制分料气缸落到右边


			FEN_LIAO_RIGHT();
				SysTask.LuoLiaoState = LUOL_SENS;	//
			    SysTask.u32LuoLiaolWaitTime = 2; //延时一段时间，防止水果不规则，误判
#if ENABLE_DEBUG
			u2_printf("等待再次检测\n");
#endif
		/*	if (InputRead(LUO_LIAO_SENSOR_PIN)) //等待果子完全掉下传感器
			{
				SysTask.LuoLiaoState = LUOL_SENS;	//
			    SysTask.u32LuoLiaolWaitTime = 10; //延时一段时间，防止水果不规则，误判
#if ENABLE_DEBUG
			u2_printf("等待再次检测");
#endif
			}*/
			if (SysTask.u32LuoLiaolWaitTime) //等待延时时间到来
				return;
			 break;

		case LUOL_SENS: //再次检测落料
           
			if (!InputRead(LUO_LIAO_SENSOR_PIN_READ)) //传感器感应到东西
			{
#if ENABLE_DEBUG
				u2_printf("Luo : 传感器感应到东西\n");
#endif

				SysTask.LuoLiaoState = LUOL_PAST2;	//先控制分料气缸落到左边
//				digitalWrite(LUO_LIAO_SENSOR_PIN, HIGH);
			}
SysTask.u32LuoLiaolWaitTime = LUO_LIAO_PAST_TIME ;
			break;


					case LUOL_PAST2: //等待通过
		


		//	SysTask.u32LuoLiaolWaitTime = LUO_LIAO_PAST_TIME;

			if (SysTask.u32LuoLiaolWaitTime) //等待延时时间到来
				return;
#if ENABLE_DEBUG
			u2_printf("2 等50Ms\n");
#endif
				SysTask.LuoLiaoState = LUOL_LEFT;
				break;

		case LUOL_LEFT: //控制分料气缸落到左边
#if ENABLE_DEBUG
			u2_printf("Luo : 控制分料气缸落到左边\n");
#endif

			FEN_LIAO_LEFT();
			SysTask.LuoLiaoState = LUOL_WAIT; //
			SysTask.u32LuoLiaolWaitTime = LUO_LIAO_WAIT_TIME; //延时一段时间

		//	SysTask.u8LuoLiaoComplte = 1; //完成落料
#if ENABLE_DEBUG
				u2_printf("等待完成落料\n");
#endif
			break;

case LUOL_WAIT: //等待完成落料
			
			if (SysTask.u32LuoLiaolWaitTime) //等待延时时间到来
					return;
				
				CUI_QI_ON();
					SysTask.u32LuoLiaolWaitTime = CUI_QI_WAIT; 
					SysTask.LuoLiaoState = LUOL_REPET;
				break;
/*#if ENABLE_DEBUG
			mySerial.print("LuoLiaoTask : 完成落料");
#endif*/ 
            
//            if(SysTask.TuiLiaoState == TUIL_INIT)  //如果推料还在进行中，不更新状态
case LUOL_REPET: //等待修正落料
              
			  	
			if (SysTask.u32LuoLiaolWaitTime) //等待延时时间到来
				return;
				CUI_QI_OFF();
			//	SysTask.u32LuoLiaolWaitTime = REPET_WAIT;

				SysTask.u8LuoLiaoComplte = 1;					//完成落料状态标示，,这里会跳转到TuiLiaoSensorTask任务，为了落料与推料可以同时进行，在推料状态机更新落料状态机	
			
				break;

		default:
			SysTask.LuoLiaoState = LUOL_INIT;
			SysTask.u8LuoLiaoComplte = 0; //还没有完成落料
			break;
	}
}


/*******************************************************************************
* 名称: 推料传感器任务
* 功能: 由传感器驱动状态迁移
* 形参:		
* 返回: 无
* 说明: 
*******************************************************************************/
void TuiLiaoSensorTask(void)
{
	switch (SysTask.TuiLiaoState)
	{
		case TUIL_INIT: //初始状态
			if (!(InputRead(TUI_LIAO_SENSOR_A_PIN_READ)) && (SysTask.u8LuoLiaoComplte == 1)) //传感器在原点，且落料完成
			{
#if ENABLE_DEBUG
				u2_printf("落料完成！\n");
#endif

				SysTask.u8LuoLiaoComplte = 0;		//清空落料完成标志

				SysTask.u8TuiLiaoComplte = 0;		//还没有完成推料
				 

			/*	if (SysTask.u32LuoLiaolWaitTime) //等待延时时间到来
				return;
				LUO_LIAO_ON();						//打开落料阀门
				*/
				CUI_QI_OFF();
				SysTask.TuiLiaoState = TUIL_LUO_LIAO; //先控制落料
#if ENABLE_DEBUG
				u2_printf("打开落料阀门\n");
#endif
				LUO_LIAO_ON();						//打开落料阀门
				
//				digitalWrite(TUI_LIAO_SENSOR_A_PIN, HIGH);
//				digitalWrite(TUI_LIAO_SENSOR_B_PIN, HIGH);
			}
				SysTask.u32TuiLiaolWaitTime = TUI_LIAO_WAIT_TIME;
			break;

		case TUIL_LUO_LIAO: //落料
			if (SysTask.u32TuiLiaolWaitTime)
				return;

#if ENABLE_DEBUG
			u2_printf("Tui: 果子已经进入槽内\n");
#endif

			LUO_LIAO_OFF(); //关闭落料阀门
			SysTask.LuoLiaoState        = LUOL_INIT; //落料完成，更新落料状态机，振盘开始工作
			SysTask.u8LuoLiaoComplte    = 0; //完成落料状态标示，,这里会跳转到TuiLiaoSensorTask任务，为了落料与推料可以同时进行，在推料状态机更新落料状态机
			SysTask.TuiLiaoState        = TUIL_TUI_LIAO; //
			//SysTask.u32TuiLiaolWaitTime = 300; //延时一定时间，让果子完全掉入到推料槽内
			break;

		case TUIL_TUI_LIAO: //推料
//			if (SysTask.u32TuiLiaolWaitTime)
//				return;

#if ENABLE_DEBUG
			u2_printf("TuiLiaoTask : 推料...\n");
#endif

			TUI_LIAO_ON();
			SysTask.TuiLiaoState = TUIL_TUI_RND; //
			SysTask.u32TuiLiaolWaitTime = TUI_OVER_TIME;
			break;

		case TUIL_TUI_RND: //等待推料 到终点
		
			while (InputRead(TUI_LIAO_SENSOR_B_PIN_READ)) //传感器推料到终点
			{ 
				BaoJingTask();
				return;
            }

#if ENABLE_DEBUG
			u2_printf("TuiLiaoTask : 推料到终点...\n");
#endif

//			digitalWrite(TUI_LIAO_SENSOR_B_PIN, HIGH);
			SysTask.u32TuiLiaolWaitTime = TUI_WAN_WAIT_TIME; //延时一定时间，让果子被切完
			SysTask.TuiLiaoState = TUIL_WAIT; //
			break;

		case TUIL_WAIT: //等待推料 完成
			if (SysTask.u32TuiLiaolWaitTime)
			{
			//	u2_printf(SysTask.u32TuiLiaolWaitTime);
				return;
			}

#if ENABLE_DEBUG
			u2_printf("TuiLiaoTask : 果子切完，返回\n");
#endif

			TUI_LIAO_OFF();
			SysTask.u8TuiLiaoComplte = 1; //完成推料
			SysTask.TuiLiaoState = TUIL_INIT;
			break;

		default:
			SysTask.TuiLiaoState = TUIL_INIT;
			SysTask.u8TuiLiaoComplte = 1; //完成推料
			break;
	}
}

/*******************************************************************************
* 名称: 
* 功能: 
* 形参:		
* 返回: 无
* 说明: 
*******************************************************************************/
void GPIOInit(void)
{
	/* 定义IO硬件初始化结构体变量 */
	GPIO_InitTypeDef GPIO_InitStructure;

	OUTPUT_RCC_CLOCKCMD(OUTPUT_RCC_CLOCKGPIO, ENABLE); //使能B端口时钟
	GPIO_InitStructure.GPIO_Pin =   FEN_LIAO_PIN
                                |   LUO_LIAO_PIN
                                |   TUI_LIAO_PIN
                                |   BAO_JING_OUT
                                |   ZHEN_PAN_PIN
                                |   CUI_QI_OUT;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_Init(OUTPUT_GPIO, &GPIO_InitStructure);

	INPUT_RCC_CLOCKCMD(INPUT_RCC_CLOCKGPIO, ENABLE); //使能端口时钟
	GPIO_InitStructure.GPIO_Pin =   TUI_LIAO_SENSOR_B_PIN
                                |   BAO_JING_IN;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_Init(INPUT_GPIO, &GPIO_InitStructure);

    //当把PB3 PB4 PA15初始化好之后，如果再次调用GPIOA GPIOB接口的时钟代码的话，会使得PB3 PA15引脚变回JTAG的引脚
	INPUT_RCC_CLOCKCMD(INPUT_RCC_CLOCKGPIO, ENABLE); //使能端口时钟
	GPIO_InitStructure.GPIO_Pin =   LUO_LIAO_SENSOR_PIN
                                |   TUI_LIAO_SENSOR_A_PIN;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
	GPIO_Init(INPUT_GPIO, &GPIO_InitStructure);

    ZHEN_PAN_ON();
	FEN_LIAO_LEFT();
	LUO_LIAO_OFF();
	TUI_LIAO_OFF();
//	GPIO_SetBits(INPUT_GPIO, LUO_LIAO_SENSOR_PIN);
//	GPIO_SetBits(INPUT_GPIO, TUI_LIAO_SENSOR_A_PIN);
//	GPIO_SetBits(INPUT_GPIO, TUI_LIAO_SENSOR_B_PIN);
	BAO_JING_OFF();
	CUI_QI_OFF();
}

/*******************************************************************************
* 名称: 
* 功能: 主任务
* 形参:		
* 返回: 无
* 说明: 
*******************************************************************************/
void MainTask(void)
{
	ZhenPanTask();									//振盘任务
	LuoLiaoSensorTask();							//落料任务
	TuiLiaoSensorTask();							//推料任务
}

/*******************************************************************************
* 名称: 
* 功能: 系统各种变量初始化
* 形参:		
* 返回: 无
* 说明: 
*******************************************************************************/
void SysInit(void)
{
#if ENABLE_DEBUG
	usart2_init(19200);						//初始化串口2			
#endif

	GPIOInit();

	SysTask.LuoLiaoState = LUOL_INIT;
	SysTask.u8LuoLiaoComplte = 0;					//还没有完成落料

	SysTask.TuiLiaoState = TUIL_INIT;
	SysTask.u8TuiLiaoComplte = 1;					//完成推料

#if ENABLE_DEBUG
	u2_printf("Hello, world?---------\n");
#endif

}


