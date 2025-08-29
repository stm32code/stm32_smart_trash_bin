#include "key.h"
#include "delay.h"
// Э���ļ�

u8 PeopleState;
u8 FireState;
/*********************************************************************************
 * @Function	:  ��ʼ������LED��IO
 * @Input		:  None
 * @Output		:  None
 * @Return		:  None
 * @Others		:  None
 * @Date			:  2022-07-23
 **********************************************************************************/
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK | KEY3_GPIO_CLK, ENABLE);

	// ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	// ���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	// ʹ�ýṹ���ʼ������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

	// ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	// ���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	// ʹ�ýṹ���ʼ������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);

	// // ѡ�񰴼�������
	// GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
	// // ���ð���������Ϊ��������
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	// // ʹ�ýṹ���ʼ������
	// GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);

	// ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN;
	// ���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO���ٶ�Ϊ50MHz
	// ʹ�ýṹ���ʼ������
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);

	// ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = CKECK1_GPIO_PIN;
	// ʹ�ýṹ���ʼ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = CKECK2_GPIO_PIN;
	// ʹ�ýṹ���ʼ������
	GPIO_Init(GPIOA, &GPIO_InitStructure); // ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = CKECK3_GPIO_PIN;
	// ʹ�ýṹ���ʼ������
	GPIO_Init(GPIOA, &GPIO_InitStructure); // ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = CKECK4_GPIO_PIN;
	// ʹ�ýṹ���ʼ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*********************************************************************************
 * @Function	:  ����Ƿ��а�������
 * @Input		:  GPIOx��x ������ A��B��C��D���� E
 *						:  GPIO_Pin������ȡ�Ķ˿�λ
 * @Output		:  None
 * @Return		:  KEY_OFF(û���°���)��KEY_ON�����°�����
 * @Others		:  None
 * @Date			:  2022-07-23
 **********************************************************************************/
u8 Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	/*����Ƿ��а������� */
	if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
	{
		/*�ȴ������ͷ� */
		while (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
			;
		return KEY_ON;
	}
	else
		return KEY_OFF;
}
/*********************************************************************************
 * @Function	:  ����������
 * @Input		:  Gmode:0,��֧��������;1,֧��������;
 * @Output		:  None
 * @Return		:  0��û���κΰ�������
 *							 1��KEY1����
 *						   2��KEY2����
 *							 3��KEY3����
 * @Others		:  ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
 * @Date			:  2022-07-23
 **********************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key_up = 1; // �������ɿ���־
	if (mode)
		key_up = 1; // ֧������
	if (key_up && (KEY1 == KEY_ON || KEY2 == KEY_ON || KEY3 == KEY_ON))
	{
		delay_ms(10); // ȥ����
		key_up = 0;
		if (KEY1 == KEY_ON)
			return KEY1_PRES;
		else if (KEY2 == KEY_ON)
			return KEY2_PRES;
		else if (KEY3 == KEY_ON)
			return KEY3_PRES;
	}
	else if (KEY1 == KEY_OFF && KEY2 == KEY_OFF && KEY3 == KEY_OFF)
		key_up = 1;
	return 0; // �ް�������
}
/*********************************************************************************
 * @Function	:  �����¼�����
 * @Input		:  None
 *						:  None
 * @Output		:  None
 * @Return		:  None
 * @Others		:  ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
 * @Date			:  2022-07-23
 **********************************************************************************/
void Send_Cmd(void)
{
	vu8 key = 0;
	key = KEY_Scan(0); // �õ���ֵ
	if (key)
	{
		switch (key)
		{
		case KEY1_PRES: // ��⵽����
			FireState = 1;
			break;
		case KEY2_PRES: // ��⵽��
			PeopleState = 1;
			break;
		}
	}
	else
		delay_ms(10);
}
/*********************************************************************************
 * @Function	:  STM32���������λ
 * @Input		:  None
 *						:  None
 * @Output		:  None
 * @Return		:  None
 * @Others		:  None
 * @Date			:  2022-08-23
 **********************************************************************************/
void Sys_Restart(void)
{
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}
/*********************************************END OF FILE**********************/
