/**

  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "key.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 板载按键IO引脚初始化.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：使用宏定义方法代替具体引脚号，方便程序移植，只要简单修改key.h
  *           文件相关宏定义就可以方便修改引脚。
  */
void KEY_GPIO_Init(void)
{
   /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStructure;

  

  //KEY1 用做输入，jtag默认，做为普通IO需要复用 且要放在所有ＩＯ初始化最后，因为两次调用RCC_APB2PeriphClockCmd会恢复jtag功能
  //所以把指纹P15  IO  初始化也放到这里来

    //一.在时钟配置中打开复用时钟：
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    //二.对PB3引脚进行重映射：
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 

	/* 使能(开启)KEY1引脚对应IO端口时钟 */  
  RCC_APB2PeriphClockCmd(KEY1_RCC_CLOCKGPIO|KEY2_RCC_CLOCKGPIO|KEY3_RCC_CLOCKGPIO | KEY4_RCC_CLOCKGPIO, ENABLE);
   
  /* 设定KEY1对应引脚IO编号 */
  GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;  
  /* 设定KEY1对应引脚IO最大操作速度 ：GPIO_Speed_50MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  /* 设定KEY1对应引脚IO为上拉输入模式 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
  /* 初始化KEY1对应引脚IO */
    
  GPIO_Init(KEY1_GPIO, &GPIO_InitStructure);

  
  /* 设定KEY2对应引脚IO编号 */
  GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;  
  /* 初始化KEY2对应引脚IO */
  GPIO_Init(KEY2_GPIO, &GPIO_InitStructure);

  
  /* 设定KEY3对应引脚IO编号 */
  GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN | KEY4_GPIO_PIN | KEY5_GPIO_PIN | KEY6_GPIO_PIN | KEY7_GPIO_PIN;  
  /* 初始化KEY3对应引脚IO */
  GPIO_Init(KEY3_GPIO, &GPIO_InitStructure);


  //指纹P15   IO初始化 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//输入下拉模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO	
  
}



/**
  * 函数功能: 简单粗暴的延时函数
  * 输入参数: time；延时时间设置
  * 返 回 值: 无
  * 说    明：按键在按下与弹开过程存在抖动，一般需要软件消抖，软件消抖最简单方法
  *           就是加延时。
  */
void KEY_ScanDelay(void)
{  
  uint32_t i,j;
  for(i=0;i<100;++i)
    for(j=0;j<1000;++j){ }		
}

#if 0
/**
  * 函数功能: 读取按键KEY1的状态
  * 输入参数：无
  * 返 回 值: KEY_DOWN：按键被按下；
  *           KEY_UP  ：按键没被按下
  * 说    明：无。
  */
KEYState_TypeDef KEY1_StateRead(void)
{
//  /* 读取此时按键值并判断是否是被按下状态，如果是被按下状态进入函数内 */
//  if(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
//  {
//    /* 延时一小段时间，消除抖动 */
//    KEY_ScanDelay();
//    /* 延时时间后再来判断按键状态，如果还是按下状态说明按键确实被按下 */
//    if(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
//    {
//      /* 等待按键弹开才退出按键扫描函数 */
//      while(GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL);
//       /* 按键扫描完毕，确定按键被按下，返回按键被按下状态 */
//      return KEY_DOWN;
//    }
//  }
//  /* 按键没被按下，返回没被按下状态 */
//  return KEY_UP;
    return GPIO_ReadInputDataBit(KEY1_GPIO,KEY1_GPIO_PIN);
}

/**
  * 函数功能: 读取按键KEY2的状态
  * 输入参数：无
  * 返 回 值: KEY_DOWN：按键被按下；
  *           KEY_UP  ：按键没被按下
  * 说    明：无。
  */
KEYState_TypeDef KEY2_StateRead(void)
{
//  /* 读取此时按键值并判断是否是被按下状态，如果是被按下状态进入函数内 */
//  if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
//  {
//    /* 延时一小段时间，消除抖动 */
//    KEY_ScanDelay();
//    /* 延时时间后再来判断按键状态，如果还是按下状态说明按键确实被按下 */
//    if(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
//    {
//      /* 等待按键弹开才退出按键扫描函数 */
//      while(GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL);
//       /* 按键扫描完毕，确定按键被按下，返回按键被按下状态 */
//      return KEY_DOWN;
//    }
//  }
//  /* 按键没被按下，返回没被按下状态 */
//  return KEY_UP;
    return GPIO_ReadInputDataBit(KEY2_GPIO,KEY2_GPIO_PIN);
}
#endif
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
