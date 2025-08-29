#include "stm32f10x.h"
#include "motor.h"
#include "delay.h"

u8 StateCurtain = 0; //�豸״̬
/***************************************************************************************************
 * ��  �� : ��ʼ�����������GPIO��
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************************/
void motor_init(void)
{
   //����IO��ʼ�����ýṹ��
   GPIO_InitTypeDef  GPIO_InitStructure;
   //��PA�˿�ʱ��
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	 
	 //���õ�IO��PF0~PF3
   GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;		
	 //����Ϊ�������
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 //IO���ٶ�Ϊ50MHz
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 //����IO
   GPIO_Init(GPIOB, &GPIO_InitStructure);	

}
/*********************************************************************************
* @Function	:	ϵͳ����PB3��PB4Ϊ��ͨIO��
* @Input		:	deviceSta,�豸״̬
* @Output		: 	None
* @Return		: 	None
* @Others		: 	JTAG���Է�ʽ����Ӱ��
* @Date			:  2020-04-22
**********************************************************************************/
void System_PB34_setIO( void )
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
}

/***************************************************************************
 * ��  �� : ���������������
 * ��  �� : X��ʾ����ת��Speed��ʾת������
 * ����ֵ : ��
 ***************************************************************************/
void MotorStep(uint8_t X,uint16_t Speed)
{
  if(X==1)   //˳ʱ��ת��
	{
		DD_0;CC_0;BB_1;AA_1;	
    delay_ms(Speed);  //ת�ٵ���

		DD_0;CC_1;BB_1;AA_0;	
    delay_ms(Speed);  //ת�ٵ���

		DD_1;CC_1;BB_0;AA_0;	
    delay_ms(Speed);  //ת�ٵ���

		DD_1;CC_0;BB_0;AA_1;	
    delay_ms(Speed);  //ת�ٵ���
  }
	else       //��ʱ��ת��
	{
		DD_1;CC_0;BB_0;AA_1;	
    delay_ms(Speed);  //ת�ٵ���

		DD_1;CC_1;BB_0;AA_0;	
    delay_ms(Speed);  //ת�ٵ���

		DD_0;CC_1;BB_1;AA_0;	
    delay_ms(Speed);  //ת�ٵ���

		DD_0;CC_0;BB_1;AA_1;	
    delay_ms(Speed);  //ת�ٵ���
  }	
}

/***************************************************************************
 * ��  �� : �������ֹͣת��
 * ��  �� : ��
 * ����ֵ : ��
 ***************************************************************************/
void MotorStop(void)
{
   DD_0;CC_0;BB_0;AA_0;
}

/***************************************************************************
 * ��  �� : ���������ת��
 * ��  �� : ��
 * ����ֵ : ��
 ***************************************************************************/
void Motor_Ctrl_Angle_F(int angle,int n){
	u16 j;
	for(j=0;j<64*angle/45;j++) 
	{
		MotorStep(0,n);
	}
}
void Motor_Ctrl_Angle_Z(int angle,int n){
	u16 j;
	for(j=0;j<64*angle/45;j++) 
	{
		MotorStep(1,n);
		}
	}
//switch(*RxMessage.Data)
//			{				 
//				case 0x11://�յ����״̬	
//						Motor_Ctrl_Angle_Z(180,3);
//						CAN_SetMsg(&TxMessage,1);
//						/*�ѱ��Ĵ洢���������䣬����*/
//						CAN_Transmit(CANx, &TxMessage);				
//					break;
//				case 0x12://�����ת180��	
//					 Motor_Ctrl_Angle_F(180,3);
//					 CAN_SetMsg(&TxMessage,2);
//						/*�ѱ��Ĵ洢���������䣬����*/
//					 CAN_Transmit(CANx, &TxMessage);	
//					break;
//				case 0x13://�����������	
//					  MotorStep(MOTOR_Z,3);
//						CAN_SetMsg(&TxMessage,3);
//						/*�ѱ��Ĵ洢���������䣬����*/
//						CAN_Transmit(CANx, &TxMessage);
//						flage=1;
//					break;
//				case 0x14://���ֹͣ����	
//						MotorStop();
//						CAN_SetMsg(&TxMessage,4);
//						/*�ѱ��Ĵ洢���������䣬����*/
//						CAN_Transmit(CANx, &TxMessage);
//						flage=0;
//					break;
//        default:   	
//        break;
//			}

