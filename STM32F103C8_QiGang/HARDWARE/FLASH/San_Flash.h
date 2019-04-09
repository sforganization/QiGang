/**
******************************************************************************
* @file    
* @authors  
* @version  V1.0.0
* @date     2015.09.15
* @brief    
******************************************************************************
*/

#ifndef  __SAN_FLASH_H__ 
#define  __SAN_FLASH_H__


#ifdef _MAININC_
#define EXTERN
#else
#define EXTERN extern
#endif

/* 头文件  ------------------------------------------------------------*/
#include "stm32f10x.h"


/*
 *   Flash的擦除要求必须整页擦除，所以也必须整页写入，否则可能会丢失数据
 *   需要更深入了解可以参考《STM32F10xxx 闪存编程参考手册》
 */



#define FLASH_SIZE 64          //所选MCU的FLASH容量大小(单位为K)          STM32F103C8 64k 

#if FLASH_SIZE<256
  #define SECTOR_SIZE           1024    //字节小型MCU 1k每页
#else 
  #define SECTOR_SIZE           2048    //字节
#endif

#define FLASH_SAVE_ADDR         0X0800C004 	    //设置FLASH 保存地址(必须为偶数，且所在扇区,要大于本代码所占用到的扇区.
										        //否则,写操作的时候,可能会导致擦除整个扇区,从而引起部分程序丢失.引起死机.

										

u32 FLASH_ReadWord(u32 address);

void FLASH_ReadMoreData(u32 startAddress, u16 * readData, u16 countToRead);

s8 FLASH_WriteMoreData(u32 startAddress, u16 * writeData, u16 countToWrite);

#endif
/******************************* FILE END ***********************************/
