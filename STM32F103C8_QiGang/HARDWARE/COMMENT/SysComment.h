

#ifndef __SYSCOMMENT_H__
#define __SYSCOMMENT_H__

#ifdef _MAININC_
#define EXTERN

#else

#define EXTERN					extern
#endif


/* 头文件  ------------------------------------------------------------*/
#include "stm32f10x.h"

#define VOID					void	

//联合体由于数据大小端问题--所有变量必须使用字节声明
//设备信息

#define ENABLE_DEBUG			        1	//是否使能debug功能，使能的话会用到一个串口输出


//注意：输入的电压可能是5V  手册《STM32F103x8B_DS_CH_V10》16页，有FT的脚才支持5V，一般PA脚都没有支持的。 PB5也不支持

//输入脚
#define INPUT_RCC_CLOCKCMD              RCC_APB2PeriphClockCmd
#define INPUT_RCC_CLOCKGPIO             RCC_APB2Periph_GPIOB

#define INPUT_GPIO                      GPIOB        //输入脚，为了方便初始化，全部使用 PB
#define LUO_LIAO_SENSOR_PIN             GPIO_Pin_6   //落料传感器
#define TUI_LIAO_SENSOR_A_PIN           GPIO_Pin_7   //推料传感器 A
#define TUI_LIAO_SENSOR_B_PIN           GPIO_Pin_8   //推料传感器 B 
#define BAO_JING_IN                     GPIO_Pin_9   //报警输入



#define LUO_LIAO_SENSOR_PIN_READ             6   //落料传感器
#define TUI_LIAO_SENSOR_A_PIN_READ           7   //推料传感器 A
#define TUI_LIAO_SENSOR_B_PIN_READ           8   //推料传感器 B
#define BAO_JING_IN_READ                     9   //报警输入

#define INPUT_GPIO_ADDR                 GPIOB_IDR_Addr       
#define InputRead(n)                    BIT_ADDR(INPUT_GPIO_ADDR, n)  //读取PB 输入 

//输出脚
#define OUTPUT_RCC_CLOCKCMD              RCC_APB2PeriphClockCmd
#define OUTPUT_RCC_CLOCKGPIO             RCC_APB2Periph_GPIOA

#define OUTPUT_GPIO                     GPIOA        //输出脚，为了方便初始化，全部使用 PB
#define FEN_LIAO_PIN                    GPIO_Pin_4   //控制分料气缸
#define FEN_LIAO_RIGHT()                GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_4)   //分料到右边
#define FEN_LIAO_LEFT()                 GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_4)  //分料到左边

#define LUO_LIAO_PIN                    GPIO_Pin_5   //控制落料气缸
#define LUO_LIAO_ON()                   GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_5)   //
#define LUO_LIAO_OFF()                  GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_5)  //

#define TUI_LIAO_PIN                    GPIO_Pin_6   //控制推料气缸
#define TUI_LIAO_ON()                   GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_6)   //
#define TUI_LIAO_OFF()                  GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_6)  //

#define ZHEN_PAN_PIN                    GPIO_Pin_7    //控制振盘
#define ZHEN_PAN_ON()                   GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_7)    //
#define ZHEN_PAN_OFF()                  GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_7)  //

#define CUI_QI_OUT                      GPIO_Pin_8   //吹气输出
#define CUI_QI_ON()                     GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_8)   //
#define CUI_QI_OFF()                    GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_8)  //

#define BAO_JING_OUT                    GPIO_Pin_9   //报警输出
#define BAO_JING_ON()                   GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_9)   //
#define BAO_JING_OFF()                  GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_9)  //


typedef enum 
{
    FALSE = 0, TRUE = !FALSE
} bool;


typedef enum 
{
    LUOL_INIT = 0,
    LUOL_PAST1, //落料通过1
    LUOL_RIGHT,  //右边
    LUOL_SENS,  //再次检测落料
    LUOL_PAST2, //落料通过2
    LUOL_LEFT,   //左边
    LUOL_WAIT,  //等待落料完成
    LUOL_REPET,//落料位置修正
    LUOL_DEF = 0XFF, 
} LuoLiaoState_T;

typedef enum 
{
    TUIL_INIT = 0,
    TUIL_LUO_LIAO,  //控制落料
    TUIL_TUI_LIAO,  //控制推料
    TUIL_TUI_RND,  //到达终点
    TUIL_WAIT,      //等待推料完成
    
    TUIL_DEF = 0XFF, 
}  TuiLiaoState_T;



typedef struct 
{
    volatile u16    u16Tick;						//

    
    LuoLiaoState_T	    LuoLiaoState;  

    u32         	u32LuoLiaolWaitTime;			// 落料等待时间
   
    u8              u8LuoLiaoComplte;               //是否完成落料


    TuiLiaoState_T	    TuiLiaoState;  

    u32         	u32TuiLiaolWaitTime;			// 推料等待时间
    u8              u8TuiLiaoComplte;               //是否完成推
} SYS_TASK;



/* 全局变量 -----------------------------------------------------------*/

/* 全局变量 -----------------------------------------------------------*/
EXTERN vu8		mSysIWDGDog; //软狗标记
EXTERN vu32 	mSysSoftDog; //软狗计数器 
EXTERN vu16 	mSysTick; //节拍器
EXTERN vu16 	mSysSec; //节拍器
EXTERN vu16 	mTimeRFRX; //接收间隔-仿真
EXTERN vu16 	mDelay;


EXTERN SYS_TASK SysTask;

EXTERN void MainTask(void);
EXTERN void SysInit(void);



#endif

