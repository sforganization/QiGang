

#ifndef __SYSCOMMENT_H__
#define __SYSCOMMENT_H__

#ifdef _MAININC_
#define EXTERN

#else

#define EXTERN					extern
#endif


/* ͷ�ļ�  ------------------------------------------------------------*/
#include "stm32f10x.h"

#define VOID					void	

//�������������ݴ�С������--���б�������ʹ���ֽ�����
//�豸��Ϣ

#define ENABLE_DEBUG			        1	//�Ƿ�ʹ��debug���ܣ�ʹ�ܵĻ����õ�һ���������


//ע�⣺����ĵ�ѹ������5V  �ֲᡶSTM32F103x8B_DS_CH_V10��16ҳ����FT�ĽŲ�֧��5V��һ��PA�Ŷ�û��֧�ֵġ� PB5Ҳ��֧��

//�����
#define INPUT_RCC_CLOCKCMD              RCC_APB2PeriphClockCmd
#define INPUT_RCC_CLOCKGPIO             RCC_APB2Periph_GPIOB

#define INPUT_GPIO                      GPIOB        //����ţ�Ϊ�˷����ʼ����ȫ��ʹ�� PB
#define LUO_LIAO_SENSOR_PIN             GPIO_Pin_6   //���ϴ�����
#define TUI_LIAO_SENSOR_A_PIN           GPIO_Pin_7   //���ϴ����� A
#define TUI_LIAO_SENSOR_B_PIN           GPIO_Pin_8   //���ϴ����� B 
#define BAO_JING_IN                     GPIO_Pin_9   //��������



#define LUO_LIAO_SENSOR_PIN_READ             6   //���ϴ�����
#define TUI_LIAO_SENSOR_A_PIN_READ           7   //���ϴ����� A
#define TUI_LIAO_SENSOR_B_PIN_READ           8   //���ϴ����� B
#define BAO_JING_IN_READ                     9   //��������

#define INPUT_GPIO_ADDR                 GPIOB_IDR_Addr       
#define InputRead(n)                    BIT_ADDR(INPUT_GPIO_ADDR, n)  //��ȡPB ���� 

//�����
#define OUTPUT_RCC_CLOCKCMD              RCC_APB2PeriphClockCmd
#define OUTPUT_RCC_CLOCKGPIO             RCC_APB2Periph_GPIOA

#define OUTPUT_GPIO                     GPIOA        //����ţ�Ϊ�˷����ʼ����ȫ��ʹ�� PB
#define FEN_LIAO_PIN                    GPIO_Pin_4   //���Ʒ�������
#define FEN_LIAO_RIGHT()                GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_4)   //���ϵ��ұ�
#define FEN_LIAO_LEFT()                 GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_4)  //���ϵ����

#define LUO_LIAO_PIN                    GPIO_Pin_5   //������������
#define LUO_LIAO_ON()                   GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_5)   //
#define LUO_LIAO_OFF()                  GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_5)  //

#define TUI_LIAO_PIN                    GPIO_Pin_6   //������������
#define TUI_LIAO_ON()                   GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_6)   //
#define TUI_LIAO_OFF()                  GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_6)  //

#define ZHEN_PAN_PIN                    GPIO_Pin_7    //��������
#define ZHEN_PAN_ON()                   GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_7)    //
#define ZHEN_PAN_OFF()                  GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_7)  //

#define CUI_QI_OUT                      GPIO_Pin_8   //�������
#define CUI_QI_ON()                     GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_8)   //
#define CUI_QI_OFF()                    GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_8)  //

#define BAO_JING_OUT                    GPIO_Pin_9   //�������
#define BAO_JING_ON()                   GPIO_ResetBits(OUTPUT_GPIO, GPIO_Pin_9)   //
#define BAO_JING_OFF()                  GPIO_SetBits(OUTPUT_GPIO, GPIO_Pin_9)  //


typedef enum 
{
    FALSE = 0, TRUE = !FALSE
} bool;


typedef enum 
{
    LUOL_INIT = 0,
    LUOL_PAST1, //����ͨ��1
    LUOL_RIGHT,  //�ұ�
    LUOL_SENS,  //�ٴμ������
    LUOL_PAST2, //����ͨ��2
    LUOL_LEFT,   //���
    LUOL_WAIT,  //�ȴ��������
    LUOL_REPET,//����λ������
    LUOL_DEF = 0XFF, 
} LuoLiaoState_T;

typedef enum 
{
    TUIL_INIT = 0,
    TUIL_LUO_LIAO,  //��������
    TUIL_TUI_LIAO,  //��������
    TUIL_TUI_RND,  //�����յ�
    TUIL_WAIT,      //�ȴ��������
    
    TUIL_DEF = 0XFF, 
}  TuiLiaoState_T;



typedef struct 
{
    volatile u16    u16Tick;						//

    
    LuoLiaoState_T	    LuoLiaoState;  

    u32         	u32LuoLiaolWaitTime;			// ���ϵȴ�ʱ��
   
    u8              u8LuoLiaoComplte;               //�Ƿ��������


    TuiLiaoState_T	    TuiLiaoState;  

    u32         	u32TuiLiaolWaitTime;			// ���ϵȴ�ʱ��
    u8              u8TuiLiaoComplte;               //�Ƿ������
} SYS_TASK;



/* ȫ�ֱ��� -----------------------------------------------------------*/

/* ȫ�ֱ��� -----------------------------------------------------------*/
EXTERN vu8		mSysIWDGDog; //�����
EXTERN vu32 	mSysSoftDog; //�������� 
EXTERN vu16 	mSysTick; //������
EXTERN vu16 	mSysSec; //������
EXTERN vu16 	mTimeRFRX; //���ռ��-����
EXTERN vu16 	mDelay;


EXTERN SYS_TASK SysTask;

EXTERN void MainTask(void);
EXTERN void SysInit(void);



#endif

