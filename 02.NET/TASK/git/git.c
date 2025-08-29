#include "git.h"
// ����豸�����ļ�
#include "led.h"
#include "adc.h"
#include "pwm.h"
#include "key.h"
#include "relay.h"
#include "usart.h"
#include "key.h"
Data_TypeDef Data_init;						  // �豸���ݽṹ��
Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
Device_Satte_Typedef device_state_init;		  // �豸״̬
Check_Hight_Typedef check_hight_init;		  // �߶�

extern U8 APP;
// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data)
{
	//	Read_DHT11(&DHT11_Data);							// ��ȡ��ʪ������
	//	Device_Data->temperatuer = DS18B20_GetTemperture(); // ��ȡ�¶�
	//	Device_Data->light = getValue();					// ��ȡ���ǿ��

	return MY_SUCCESSFUL;
}
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state, Check_Hight_Typedef *check_hight)
{

	// threshold_value->threshold_light_min = 80;
	// threshold_value->threshold_humi_min = 100;
	threshold_value->threshold_somg_min = 1000;
	// threshold_value->threshold_temperatuer_max = 25;
	// threshold_value->threshold_temperatuer_min = 28;
	// ״̬����
	device_state->taash_can_A = 0;
	device_state->taash_can_B = 0;
	device_state->taash_can_C = 0;
	device_state->taash_can_D = 0;
	// �߶�
	check_hight->check_A = 0;
	check_hight->check_B = 0;
	check_hight->check_C = 0;
	check_hight->check_D = 0;

	return MY_SUCCESSFUL;
}
// ����OLED��ʾ��������
mySta Update_Oled_Massage()
{

	return MY_SUCCESSFUL;
}
// ����Ƿ����
mySta Check_Rubbish_Massage(Check_Hight_Typedef *hight_data)
{

	//   ���A�Ƿ����
	if (!CKECK1&&check_hight_init.check_A==0)
	{
		hight_data->check_A = 1; // ���
		Usart_SendString(USART2, "A7:00006/n"); //
	}
	else if(CKECK1&&check_hight_init.check_A)
	{
		hight_data->check_A = 0; // δ���

	}
	// ���B�Ƿ����
	if (!CKECK2&&check_hight_init.check_B==0)
	{
		hight_data->check_B = 1; // ���
		Usart_SendString(USART2, "A7:00004/n"); //
	}
	else if(CKECK2&&check_hight_init.check_B)
	{
		hight_data->check_B = 0; // δ���
	}
	// ���C�Ƿ����
	if (!CKECK3&&check_hight_init.check_C==0)
	{
		hight_data->check_C = 1; // ���
		Usart_SendString(USART2, "A7:00005/n"); //
	}
	else if(CKECK3&&check_hight_init.check_C)
	{
		hight_data->check_C = 0; // δ���
	}

	// ���C�Ƿ����
	if (!CKECK4)
	{
		RELAY1_ON;
	}
	else
	{

		if (APP > 0)
		{
			APP--;
		}
		else
		{
			RELAY1_OFF;
		}
	}
	return MY_SUCCESSFUL;
}
// �����豸״̬
mySta Update_Device_Massage(Device_Satte_Typedef *device_state)
{
	// ���в���ִֻ��һ�Σ���Լϵͳ��Դ

	return MY_SUCCESSFUL;
}
// �Զ����

void Check_Key_ON_OFF(Device_Satte_Typedef *device_state)
{
	if (!KEY1 && device_state_init.taash_can_A == 0) // �к�����
	{
		LED = ON;
		Usart_SendString(USART2, "A7:00003/n"); // ��������ģ��
		Specific_Value(TIMX_3, 1, 3.5);			// ���ƶ��
		device_state->taash_can_A = TIME_OVER;	// ���õ���ʱ�ر�ʱ��
	}
	if (!KEY4 && device_state_init.taash_can_B == 0)
	{
		LED = ON;
		Usart_SendString(USART2, "A7:00001/n"); // ��������
		Specific_Value(TIMX_3, 2, 3.5);			// ���ƶ��

		device_state->taash_can_B = TIME_OVER;
	}
	if (!KEY3 && device_state_init.taash_can_C == 0) // �ɻ�����
	{
		LED = ON;
		Usart_SendString(USART2, "A7:00002/n"); //
		Specific_Value(TIMX_3, 3, 4);			// ���ƻ�
		device_state->taash_can_C = TIME_OVER;
	}
}
// �Զ��رն��
void Automation_Close(Device_Satte_Typedef *device_state)
{

	if (device_state_init.taash_can_A > 1) // ����ʱA
	{
		device_state->taash_can_A--;
	}
	else if (device_state_init.taash_can_A > 0)
	{
		LED = OFF;
		device_state->taash_can_A--;
		Specific_Value(TIMX_3, 1, 5); // ���ƶ��
	}
	if (device_state_init.taash_can_B > 1) // ����ʱB
	{
		device_state->taash_can_B--;
	}
	else if (device_state_init.taash_can_B > 0)
	{
		LED = OFF;
		device_state->taash_can_B--;
		Specific_Value(TIMX_3, 2, 5); // ���ƶ��
	}
	if (device_state_init.taash_can_C > 1) // ����ʱC
	{
		device_state->taash_can_C--;
	}
	else if (device_state_init.taash_can_C > 0)
	{
		LED = OFF;
		device_state->taash_can_C--;
		Specific_Value(TIMX_3, 3, 5); // ���ƶ��
	}
}
