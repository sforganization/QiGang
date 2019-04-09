
#include "SysComment.h"
#include "stm32f10x_it.h"
#include "stm32f10x_iwdg.h"
#include "san_flash.h"
#include "usart2.h"




#define LUO_LIAO_PAST_TIME		150  //500ms ��ʱʱ�� ���Ϸ���ͨ����ʱ
#define LUO_LIAO_WAIT_TIME		1000  //1000ms ��ʱʱ�� ǰ��������ʱ
#define TUI_LIAO_WAIT_TIME		1000  //700ms ��ʱʱ�� ���϶�������ʱ
#define TUI_WAN_WAIT_TIME		600   //600ms ��ʱʱ�� �Ƶ���ͷ
#define TUI_OVER_TIME			4000   //4000ms ���ϳ�ʱ ����
#define CUI_QI_WAIT				80		// 50ms ������ʱ
#define REPET_WAIT				200		// 50ms ������ʱ


/*******************************************************************************
* ����: ��������
* ����: ������ϴ�����ĩ��4S���Ӧ���� ������� �������׻�λ
* �β�:		
* ����: ��
* ˵��: 
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
		u2_printf("���ϱ�����\n");
#endif

		while(InputRead(BAO_JING_IN_READ))
        {
            ;
        }      
#if ENABLE_DEBUG
		u2_printf("���������\n");
#endif
		BAO_JING_OFF();
		LUO_LIAO_OFF();
		ZHEN_PAN_ON();
		SysTask.TuiLiaoState = TUIL_WAIT;
	}
}
/*******************************************************************************
* ����: ��������
* ����: ������ϴ��������˵�ԭʼλ����������
* �β�:		
* ����: ��
* ˵��: 
*******************************************************************************/
void ZhenPanTask(void)
{
    
#if ENABLE_DEBUG
                    u8 static u8State = 0;
#endif

	if ((SysTask.LuoLiaoState == LUOL_LEFT) || (SysTask.LuoLiaoState == LUOL_WAIT) || (SysTask.LuoLiaoState == LUOL_REPET)) //�����Ѿ����,����ʲô״̬��ֹͣ����
	{
       /* if(u8State){
            u8State = 0;  
		SysTask.u32LuoLiaolWaitTime=3000;
		}
		if (SysTask.u32LuoLiaolWaitTime) //�ȴ���ʱʱ�䵽��
		{		*/
#if ENABLE_DEBUG
        if(u8State){
            u8State = 0;
				u2_printf("---����stop---\n");
            }
#endif
	//	u2_printf("---����stop---");
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
				u2_printf("****����start****\n");
            }
#endif
		ZHEN_PAN_ON();
	}
}


/*******************************************************************************
   * ����: ���ϴ���������
   * ����: �ɴ���������״̬Ǩ��
   * �β�:		
   * ����: ��
   * ˵��: 
   *******************************************************************************/
void LuoLiaoSensorTask(void)
{
	switch (SysTask.LuoLiaoState)
	{
		case LUOL_INIT: //��ʼ״̬
			if (!InputRead(LUO_LIAO_SENSOR_PIN_READ)) //��������Ӧ������
			{
#if ENABLE_DEBUG
				u2_printf("Luo : ��������Ӧ������\n");
#endif

				SysTask.u8LuoLiaoComplte = 0;		//��û���������

				SysTask.LuoLiaoState = LUOL_PAST1;	//�ȿ��Ʒ��������䵽�ұ�

				//digitalWrite(LUO_LIAO_SENSOR_PIN, HIGH);
			}
			SysTask.u32LuoLiaolWaitTime = LUO_LIAO_PAST_TIME ;

			break;
//
		case LUOL_PAST1: //�ȴ�ͨ��
		


			//SysTask.u32LuoLiaolWaitTime = LUO_LIAO_PAST_TIME ;

			if (SysTask.u32LuoLiaolWaitTime) //�ȴ���ʱʱ�䵽��
				return;

#if ENABLE_DEBUG
			u2_printf("1 ��50Ms\n");
#endif

				SysTask.LuoLiaoState = LUOL_RIGHT;
#if ENABLE_DEBUG
			u2_printf("Luo : ���Ʒ��������䵽�ұ�\n");
#endif
				break;
				
//
		case LUOL_RIGHT: //���Ʒ��������䵽�ұ�


			FEN_LIAO_RIGHT();
				SysTask.LuoLiaoState = LUOL_SENS;	//
			    SysTask.u32LuoLiaolWaitTime = 2; //��ʱһ��ʱ�䣬��ֹˮ������������
#if ENABLE_DEBUG
			u2_printf("�ȴ��ٴμ��\n");
#endif
		/*	if (InputRead(LUO_LIAO_SENSOR_PIN)) //�ȴ�������ȫ���´�����
			{
				SysTask.LuoLiaoState = LUOL_SENS;	//
			    SysTask.u32LuoLiaolWaitTime = 10; //��ʱһ��ʱ�䣬��ֹˮ������������
#if ENABLE_DEBUG
			u2_printf("�ȴ��ٴμ��");
#endif
			}*/
			if (SysTask.u32LuoLiaolWaitTime) //�ȴ���ʱʱ�䵽��
				return;
			 break;

		case LUOL_SENS: //�ٴμ������
           
			if (!InputRead(LUO_LIAO_SENSOR_PIN_READ)) //��������Ӧ������
			{
#if ENABLE_DEBUG
				u2_printf("Luo : ��������Ӧ������\n");
#endif

				SysTask.LuoLiaoState = LUOL_PAST2;	//�ȿ��Ʒ��������䵽���
//				digitalWrite(LUO_LIAO_SENSOR_PIN, HIGH);
			}
SysTask.u32LuoLiaolWaitTime = LUO_LIAO_PAST_TIME ;
			break;


					case LUOL_PAST2: //�ȴ�ͨ��
		


		//	SysTask.u32LuoLiaolWaitTime = LUO_LIAO_PAST_TIME;

			if (SysTask.u32LuoLiaolWaitTime) //�ȴ���ʱʱ�䵽��
				return;
#if ENABLE_DEBUG
			u2_printf("2 ��50Ms\n");
#endif
				SysTask.LuoLiaoState = LUOL_LEFT;
				break;

		case LUOL_LEFT: //���Ʒ��������䵽���
#if ENABLE_DEBUG
			u2_printf("Luo : ���Ʒ��������䵽���\n");
#endif

			FEN_LIAO_LEFT();
			SysTask.LuoLiaoState = LUOL_WAIT; //
			SysTask.u32LuoLiaolWaitTime = LUO_LIAO_WAIT_TIME; //��ʱһ��ʱ��

		//	SysTask.u8LuoLiaoComplte = 1; //�������
#if ENABLE_DEBUG
				u2_printf("�ȴ��������\n");
#endif
			break;

case LUOL_WAIT: //�ȴ��������
			
			if (SysTask.u32LuoLiaolWaitTime) //�ȴ���ʱʱ�䵽��
					return;
				
				CUI_QI_ON();
					SysTask.u32LuoLiaolWaitTime = CUI_QI_WAIT; 
					SysTask.LuoLiaoState = LUOL_REPET;
				break;
/*#if ENABLE_DEBUG
			mySerial.print("LuoLiaoTask : �������");
#endif*/ 
            
//            if(SysTask.TuiLiaoState == TUIL_INIT)  //������ϻ��ڽ����У�������״̬
case LUOL_REPET: //�ȴ���������
              
			  	
			if (SysTask.u32LuoLiaolWaitTime) //�ȴ���ʱʱ�䵽��
				return;
				CUI_QI_OFF();
			//	SysTask.u32LuoLiaolWaitTime = REPET_WAIT;

				SysTask.u8LuoLiaoComplte = 1;					//�������״̬��ʾ��,�������ת��TuiLiaoSensorTask����Ϊ�����������Ͽ���ͬʱ���У�������״̬����������״̬��	
			
				break;

		default:
			SysTask.LuoLiaoState = LUOL_INIT;
			SysTask.u8LuoLiaoComplte = 0; //��û���������
			break;
	}
}


/*******************************************************************************
* ����: ���ϴ���������
* ����: �ɴ���������״̬Ǩ��
* �β�:		
* ����: ��
* ˵��: 
*******************************************************************************/
void TuiLiaoSensorTask(void)
{
	switch (SysTask.TuiLiaoState)
	{
		case TUIL_INIT: //��ʼ״̬
			if (!(InputRead(TUI_LIAO_SENSOR_A_PIN_READ)) && (SysTask.u8LuoLiaoComplte == 1)) //��������ԭ�㣬���������
			{
#if ENABLE_DEBUG
				u2_printf("������ɣ�\n");
#endif

				SysTask.u8LuoLiaoComplte = 0;		//���������ɱ�־

				SysTask.u8TuiLiaoComplte = 0;		//��û���������
				 

			/*	if (SysTask.u32LuoLiaolWaitTime) //�ȴ���ʱʱ�䵽��
				return;
				LUO_LIAO_ON();						//�����Ϸ���
				*/
				CUI_QI_OFF();
				SysTask.TuiLiaoState = TUIL_LUO_LIAO; //�ȿ�������
#if ENABLE_DEBUG
				u2_printf("�����Ϸ���\n");
#endif
				LUO_LIAO_ON();						//�����Ϸ���
				
//				digitalWrite(TUI_LIAO_SENSOR_A_PIN, HIGH);
//				digitalWrite(TUI_LIAO_SENSOR_B_PIN, HIGH);
			}
				SysTask.u32TuiLiaolWaitTime = TUI_LIAO_WAIT_TIME;
			break;

		case TUIL_LUO_LIAO: //����
			if (SysTask.u32TuiLiaolWaitTime)
				return;

#if ENABLE_DEBUG
			u2_printf("Tui: �����Ѿ��������\n");
#endif

			LUO_LIAO_OFF(); //�ر����Ϸ���
			SysTask.LuoLiaoState        = LUOL_INIT; //������ɣ���������״̬�������̿�ʼ����
			SysTask.u8LuoLiaoComplte    = 0; //�������״̬��ʾ��,�������ת��TuiLiaoSensorTask����Ϊ�����������Ͽ���ͬʱ���У�������״̬����������״̬��
			SysTask.TuiLiaoState        = TUIL_TUI_LIAO; //
			//SysTask.u32TuiLiaolWaitTime = 300; //��ʱһ��ʱ�䣬�ù�����ȫ���뵽���ϲ���
			break;

		case TUIL_TUI_LIAO: //����
//			if (SysTask.u32TuiLiaolWaitTime)
//				return;

#if ENABLE_DEBUG
			u2_printf("TuiLiaoTask : ����...\n");
#endif

			TUI_LIAO_ON();
			SysTask.TuiLiaoState = TUIL_TUI_RND; //
			SysTask.u32TuiLiaolWaitTime = TUI_OVER_TIME;
			break;

		case TUIL_TUI_RND: //�ȴ����� ���յ�
		
			while (InputRead(TUI_LIAO_SENSOR_B_PIN_READ)) //���������ϵ��յ�
			{ 
				BaoJingTask();
				return;
            }

#if ENABLE_DEBUG
			u2_printf("TuiLiaoTask : ���ϵ��յ�...\n");
#endif

//			digitalWrite(TUI_LIAO_SENSOR_B_PIN, HIGH);
			SysTask.u32TuiLiaolWaitTime = TUI_WAN_WAIT_TIME; //��ʱһ��ʱ�䣬�ù��ӱ�����
			SysTask.TuiLiaoState = TUIL_WAIT; //
			break;

		case TUIL_WAIT: //�ȴ����� ���
			if (SysTask.u32TuiLiaolWaitTime)
			{
			//	u2_printf(SysTask.u32TuiLiaolWaitTime);
				return;
			}

#if ENABLE_DEBUG
			u2_printf("TuiLiaoTask : �������꣬����\n");
#endif

			TUI_LIAO_OFF();
			SysTask.u8TuiLiaoComplte = 1; //�������
			SysTask.TuiLiaoState = TUIL_INIT;
			break;

		default:
			SysTask.TuiLiaoState = TUIL_INIT;
			SysTask.u8TuiLiaoComplte = 1; //�������
			break;
	}
}

/*******************************************************************************
* ����: 
* ����: 
* �β�:		
* ����: ��
* ˵��: 
*******************************************************************************/
void GPIOInit(void)
{
	/* ����IOӲ����ʼ���ṹ����� */
	GPIO_InitTypeDef GPIO_InitStructure;

	OUTPUT_RCC_CLOCKCMD(OUTPUT_RCC_CLOCKGPIO, ENABLE); //ʹ��B�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin =   FEN_LIAO_PIN
                                |   LUO_LIAO_PIN
                                |   TUI_LIAO_PIN
                                |   BAO_JING_OUT
                                |   ZHEN_PAN_PIN
                                |   CUI_QI_OUT;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_Init(OUTPUT_GPIO, &GPIO_InitStructure);

	INPUT_RCC_CLOCKCMD(INPUT_RCC_CLOCKGPIO, ENABLE); //ʹ�ܶ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin =   TUI_LIAO_SENSOR_B_PIN
                                |   BAO_JING_IN;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
	GPIO_Init(INPUT_GPIO, &GPIO_InitStructure);

    //����PB3 PB4 PA15��ʼ����֮������ٴε���GPIOA GPIOB�ӿڵ�ʱ�Ӵ���Ļ�����ʹ��PB3 PA15���ű��JTAG������
	INPUT_RCC_CLOCKCMD(INPUT_RCC_CLOCKGPIO, ENABLE); //ʹ�ܶ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin =   LUO_LIAO_SENSOR_PIN
                                |   TUI_LIAO_SENSOR_A_PIN;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
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
* ����: 
* ����: ������
* �β�:		
* ����: ��
* ˵��: 
*******************************************************************************/
void MainTask(void)
{
	ZhenPanTask();									//��������
	LuoLiaoSensorTask();							//��������
	TuiLiaoSensorTask();							//��������
}

/*******************************************************************************
* ����: 
* ����: ϵͳ���ֱ�����ʼ��
* �β�:		
* ����: ��
* ˵��: 
*******************************************************************************/
void SysInit(void)
{
#if ENABLE_DEBUG
	usart2_init(19200);						//��ʼ������2			
#endif

	GPIOInit();

	SysTask.LuoLiaoState = LUOL_INIT;
	SysTask.u8LuoLiaoComplte = 0;					//��û���������

	SysTask.TuiLiaoState = TUIL_INIT;
	SysTask.u8TuiLiaoComplte = 1;					//�������

#if ENABLE_DEBUG
	u2_printf("Hello, world?---------\n");
#endif

}


