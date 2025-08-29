// ��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

// �����豸����
#include "esp8266.h"

// Ӳ������
#include "delay.h"
#include "usart.h"

// C��
#include <string.h>
#include <stdio.h>

unsigned char esp8266_buf[128];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;

//==========================================================
//	�������ƣ�	ESP8266_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;
}

//==========================================================
//	�������ƣ�	ESP8266_WaitRecive
//
//	�������ܣ�	�ȴ��������
//
//	��ڲ�����	��
//
//	���ز�����	REV_OK-�������		REV_WAIT-���ճ�ʱδ���
//
//	˵����		ѭ�����ü���Ƿ�������
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if (esp8266_cnt == 0) // ������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return REV_WAIT;

	if (esp8266_cnt == esp8266_cntPre) // �����һ�ε�ֵ�������ͬ����˵���������
	{
		esp8266_cnt = 0; // ��0���ռ���

		return REV_OK; // ���ؽ�����ɱ�־
	}

	esp8266_cntPre = esp8266_cnt; // ��Ϊ��ͬ

	return REV_WAIT; // ���ؽ���δ��ɱ�־
}

//==========================================================
//	�������ƣ�	ESP8266_SendCmd
//
//	�������ܣ�	��������
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��
//
//	˵����
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res, u16 time)
{
	Usart3_SendString((unsigned char *)cmd, strlen((const char *)cmd));

	while (time--)
	{
		if (ESP8266_WaitRecive() == REV_OK) // ����յ�����
		{
			if (strstr((const char *)esp8266_buf, res) != NULL) // ����������ؼ���
			{
				ESP8266_Clear(); // ��ջ���

				return 0;
			}
		}

		delay_ms(10);
	}

	return 1;
}

//==========================================================
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//				len������
//
//	���ز�����	��
//
//	˵����
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	ESP8266_Clear();			  // ��ս��ջ���
	Usart3_SendString(data, len); // �����豸������������
}

//==========================================================
//	�������ƣ�	ESP8266_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
unsigned char *ESP8266_Get(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	do
	{
		if (ESP8266_WaitRecive() == REV_OK) // ����������
		{
			ptrIPD = (char *)esp8266_buf;
			ptrIPD = strchr(ptrIPD, ':'); // �ҵ�'{'
			if (ptrIPD != NULL)
			{
				ptrIPD++;
				return (unsigned char *)(ptrIPD);
			}
			else
				return NULL;
		}

		delay_ms(5); // ��ʱ�ȴ�
	} while (timeOut--);

	return NULL; // ��ʱ��δ�ҵ������ؿ�ָ��
}

//==========================================================
//	�������ƣ�	Net_Reset
//
//	�������ܣ�	���縴λ
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����
//==========================================================
void ESP_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;

	RCC_APB2PeriphClockCmd(ESP_RST_GPIO_CLK | ESP_CONNET_GPIO_CLK, ENABLE);

	// ESP8266��λ����
	GPIO_Initure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initure.GPIO_Pin = ESP_RST_GPIO_PIN; // GPIOB1-��λ
	GPIO_Initure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ESP_RST_GPIO_PORT, &GPIO_Initure);

	// ��ʼ���źŵ�
	GPIO_Initure.GPIO_Pin = ESP_CONNET_GPIO_PIN;
	// ������Ϊ��������
	GPIO_Initure.GPIO_Mode = GPIO_Mode_IPU;
	// ʹ�ýṹ���ʼ������
	GPIO_Init(ESP_CONNET_GPIO_PORT, &GPIO_Initure);
	// GPIO_WriteBit(ESP_CONNET_GPIO_PORT, ESP_CONNET_GPIO_PIN, Bit_RESET);

	GPIO_WriteBit(ESP_RST_GPIO_PORT, ESP_RST_GPIO_PIN, Bit_RESET);
	delay_ms(250);
	GPIO_WriteBit(ESP_RST_GPIO_PORT, ESP_RST_GPIO_PIN, Bit_SET);
	delay_ms(500);

	ESP8266_Clear();
}
void Net_Reset(void)
{

	GPIO_WriteBit(ESP_RST_GPIO_PORT, ESP_RST_GPIO_PIN, Bit_RESET);
	delay_ms(250);
	GPIO_WriteBit(ESP_RST_GPIO_PORT, ESP_RST_GPIO_PIN, Bit_SET);
	delay_ms(500);

	ESP8266_Clear();
}
/*
************************************************************
*	�������ƣ�	Usart3_Init
*
*	�������ܣ�	����3��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PB10		RX-PB11
************************************************************
*/
void Usart3_Init(unsigned int baud)
{

	GPIO_InitTypeDef gpio_initstruct;
	USART_InitTypeDef usart_initstruct;
	NVIC_InitTypeDef nvic_initstruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// PA2	TXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_10;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_initstruct);

	// PA3	RXD
	gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_initstruct.GPIO_Pin = GPIO_Pin_11;
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_initstruct);

	usart_initstruct.USART_BaudRate = baud;
	usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������
	usart_initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				 // ���պͷ���
	usart_initstruct.USART_Parity = USART_Parity_No;							 // ��У��
	usart_initstruct.USART_StopBits = USART_StopBits_1;							 // 1λֹͣλ
	usart_initstruct.USART_WordLength = USART_WordLength_8b;					 // 8λ����λ
	USART_Init(USART3, &usart_initstruct);

	USART_Cmd(USART3, ENABLE); // ʹ�ܴ���

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // ʹ�ܽ����ж�

	nvic_initstruct.NVIC_IRQChannel = USART3_IRQn;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic_initstruct);
}

//==========================================================
//	�������ƣ�	USART2_IRQHandler
//
//	�������ܣ�	����2�շ��ж�
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����
//==========================================================
void USART3_IRQHandler(void)
{

	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // �����ж�
	{
		if (esp8266_cnt >= sizeof(esp8266_buf))
			esp8266_cnt = 0; // ��ֹ���ڱ�ˢ��
		esp8266_buf[esp8266_cnt++] = USART3->DR;

		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
}

/*
************************************************************
*	�������ƣ�	Usart_SendString
*
*	�������ܣ�	�������ݷ���
*
*	��ڲ�����	USARTx��������
*				str��Ҫ���͵�����
*				len�����ݳ���
*
*	���ز�����	��
*
*	˵����
************************************************************
*/
void Usart3_SendString(unsigned char *str, unsigned short len)
{

	unsigned short count = 0;

	for (; count < len; count++)
	{
		USART_SendData(USART3, *str++); // ��������
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
			; // �ȴ��������
	}
}
