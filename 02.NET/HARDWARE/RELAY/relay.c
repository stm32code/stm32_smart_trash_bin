#include "relay.h"
#include "massage.h"
#include "delay.h"
#include "esp8266.h"



void RELAY_GPIO_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RELAY1_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RELAY1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN);
}
/**
 * @brief  MQTT操作LED模块
 * @param  无
 * @retval 无
 */
void Mqtt_Relay(u8 Cmd)
{

	switch (Cmd)
	{
	case 1:
		RELAY1_ON;
		break;
	case 0:
		RELAY1_OFF;
		break;
	}
	// while (data_json_init() != MY_SUCCESSFUL)
	// 	delay_ms(5); // 发送设备数据
}