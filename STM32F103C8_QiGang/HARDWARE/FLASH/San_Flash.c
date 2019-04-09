

/**
******************************************************************************
* @file 	
* @authors	
* @version	V1.0.0
* @date 	
* @brief	
******************************************************************************
*/
#include "san_flash.h"
#include "stm32f10x_flash.h"



/**********************************************************************
* 名称: 
* 功能:
* 形参: 
* 返回: 无
* 说明: 
**********************************************************************/

//读取指定地址的半字(16位数据)
u16 FLASH_ReadHalfWord(u32 address)
{
	return * (__IO u16 *)address;
}

//从指定地址开始读取多个数据
void FLASH_ReadMoreData(u32 startAddress, u16 * readData, u16 countToRead)
{
	u16 			dataIndex;

	for (dataIndex = 0; dataIndex < countToRead; dataIndex++)
	{
		readData[dataIndex] = FLASH_ReadHalfWord(startAddress + dataIndex * 2);
	}
}



//读取指定地址的全字(32位数据)
u32 FLASH_ReadWord(u32 address)
{
	u32 			temp1, temp2;

	temp1				= * (__IO u16 *)
	address;
	temp2				= * (__IO u16 *) (address + 2);
	return (temp2 << 16) +temp1;
}


//从指定地址开始写入多个数据
s8 FLASH_WriteMoreData(u32 startAddress, u16 * writeData, u16 countToWrite)
{
    FLASH_Status status;
	if (startAddress < FLASH_BASE || ((startAddress + countToWrite * 2) >= (FLASH_BASE + 1024 * FLASH_SIZE)))
	{
        return -1; //非法地址
	}

	FLASH_Unlock(); 								//解锁写保护
	u32 			offsetAddress = startAddress - FLASH_BASE; //计算去掉0X08000000后的实际偏移地址
	u32 			sectorPosition = offsetAddress / SECTOR_SIZE; //计算扇区地址，对于STM32F103VET6为0~255

	u32 			sectorStartAddress = sectorPosition * SECTOR_SIZE + FLASH_BASE; //对应扇区的首地址


	status = FLASH_ErasePage(sectorStartAddress);			//擦除这个扇区
    if(status != FLASH_COMPLETE)
        goto END;
        
	u16 			dataIndex;

	for (dataIndex = 0; dataIndex < countToWrite; dataIndex++)
	{
		status = FLASH_ProgramHalfWord(startAddress + dataIndex * 2, writeData[dataIndex]);
        
        if(status != FLASH_COMPLETE)
            goto END;
	}

END:
	FLASH_Lock();									//上锁写保护
    
    if(status != FLASH_COMPLETE)
        return -1;
    else
        return 0; 
}


