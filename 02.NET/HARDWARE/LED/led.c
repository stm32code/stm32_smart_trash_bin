#include "delay.h"
#include "led.h"
// �����豸����
#include "esp8266.h"
// ��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��
// LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(Relay_GPIO_CLK, ENABLE); // ʹ�ܶ˿�ʱ��C

	GPIO_InitStructure.GPIO_Pin = Relay_GPIO_PIN;	  // �˿�����, �������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	GPIO_Init(Relay_GPIO_PORT, &GPIO_InitStructure);  // ������� ��IO���ٶ�Ϊ50MHz

	LED = OFF;
}
/**
 * @brief  MQTT����LEDģ��
 * @param  ��
 * @retval ��
 */
void Mqtt_LED(char *Tips, int Cmd)
{

	ESP8266_Clear(); // ��ջ���
	// printf("cmd:%d",Cmd);
	switch (Cmd)
	{
	case 1:
		LED = ON;
		// OneNet_SendMqtt();          //�ϴ����ĵ�������

		break;

	case 0:
		LED = OFF;
		//	OneNet_SendMqtt();          //�ϴ����ĵ�������

		break;
	default:
		/* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
		// OneNet_Publish(Tips, "ERROR:2");

		break;
	}
	return;
}
/*********************************************************************************
 * @Function	:	ϵͳ����PB3��PB4Ϊ��ͨIO��
 * @Input		:	deviceSta,�豸״̬
 * @Output		: 	None
 * @Return		: 	None
 * @Others		: 	JTAG���Է�ʽ����Ӱ��
 * @Date			:  2020-04-22
 **********************************************************************************/
void System_PB34_setIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // ��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}
