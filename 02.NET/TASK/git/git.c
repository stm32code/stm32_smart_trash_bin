#include "git.h"
// 添加设备配置文件
#include "led.h"
#include "adc.h"
#include "pwm.h"
#include "key.h"
#include "relay.h"
#include "usart.h"
#include "key.h"
Data_TypeDef Data_init;						  // 设备数据结构体
Threshold_Value_TypeDef threshold_value_init; // 设备阈值设置结构体
Device_Satte_Typedef device_state_init;		  // 设备状态
Check_Hight_Typedef check_hight_init;		  // 高度

extern U8 APP;
// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data)
{
	//	Read_DHT11(&DHT11_Data);							// 获取温湿度数据
	//	Device_Data->temperatuer = DS18B20_GetTemperture(); // 获取温度
	//	Device_Data->light = getValue();					// 获取光感强度

	return MY_SUCCESSFUL;
}
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state, Check_Hight_Typedef *check_hight)
{

	// threshold_value->threshold_light_min = 80;
	// threshold_value->threshold_humi_min = 100;
	threshold_value->threshold_somg_min = 1000;
	// threshold_value->threshold_temperatuer_max = 25;
	// threshold_value->threshold_temperatuer_min = 28;
	// 状态归零
	device_state->taash_can_A = 0;
	device_state->taash_can_B = 0;
	device_state->taash_can_C = 0;
	device_state->taash_can_D = 0;
	// 高度
	check_hight->check_A = 0;
	check_hight->check_B = 0;
	check_hight->check_C = 0;
	check_hight->check_D = 0;

	return MY_SUCCESSFUL;
}
// 更新OLED显示屏中内容
mySta Update_Oled_Massage()
{

	return MY_SUCCESSFUL;
}
// 检测是否溢出
mySta Check_Rubbish_Massage(Check_Hight_Typedef *hight_data)
{

	//   检测A是否溢出
	if (!CKECK1&&check_hight_init.check_A==0)
	{
		hight_data->check_A = 1; // 溢出
		Usart_SendString(USART2, "A7:00006/n"); //
	}
	else if(CKECK1&&check_hight_init.check_A)
	{
		hight_data->check_A = 0; // 未溢出

	}
	// 检测B是否溢出
	if (!CKECK2&&check_hight_init.check_B==0)
	{
		hight_data->check_B = 1; // 溢出
		Usart_SendString(USART2, "A7:00004/n"); //
	}
	else if(CKECK2&&check_hight_init.check_B)
	{
		hight_data->check_B = 0; // 未溢出
	}
	// 检测C是否溢出
	if (!CKECK3&&check_hight_init.check_C==0)
	{
		hight_data->check_C = 1; // 溢出
		Usart_SendString(USART2, "A7:00005/n"); //
	}
	else if(CKECK3&&check_hight_init.check_C)
	{
		hight_data->check_C = 0; // 未溢出
	}

	// 检测C是否溢出
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
// 更新设备状态
mySta Update_Device_Massage(Device_Satte_Typedef *device_state)
{
	// 所有操作只执行一次，节约系统资源

	return MY_SUCCESSFUL;
}
// 自动检测

void Check_Key_ON_OFF(Device_Satte_Typedef *device_state)
{
	if (!KEY1 && device_state_init.taash_can_A == 0) // 有害垃圾
	{
		LED = ON;
		Usart_SendString(USART2, "A7:00003/n"); // 控制语音模块
		Specific_Value(TIMX_3, 1, 3.5);			// 控制舵机
		device_state->taash_can_A = TIME_OVER;	// 设置倒计时关闭时间
	}
	if (!KEY4 && device_state_init.taash_can_B == 0)
	{
		LED = ON;
		Usart_SendString(USART2, "A7:00001/n"); // 其他垃圾
		Specific_Value(TIMX_3, 2, 3.5);			// 控制舵机

		device_state->taash_can_B = TIME_OVER;
	}
	if (!KEY3 && device_state_init.taash_can_C == 0) // 可回收物
	{
		LED = ON;
		Usart_SendString(USART2, "A7:00002/n"); //
		Specific_Value(TIMX_3, 3, 4);			// 控制机
		device_state->taash_can_C = TIME_OVER;
	}
}
// 自动关闭舵机
void Automation_Close(Device_Satte_Typedef *device_state)
{

	if (device_state_init.taash_can_A > 1) // 倒计时A
	{
		device_state->taash_can_A--;
	}
	else if (device_state_init.taash_can_A > 0)
	{
		LED = OFF;
		device_state->taash_can_A--;
		Specific_Value(TIMX_3, 1, 5); // 控制舵机
	}
	if (device_state_init.taash_can_B > 1) // 倒计时B
	{
		device_state->taash_can_B--;
	}
	else if (device_state_init.taash_can_B > 0)
	{
		LED = OFF;
		device_state->taash_can_B--;
		Specific_Value(TIMX_3, 2, 5); // 控制舵机
	}
	if (device_state_init.taash_can_C > 1) // 倒计时C
	{
		device_state->taash_can_C--;
	}
	else if (device_state_init.taash_can_C > 0)
	{
		LED = OFF;
		device_state->taash_can_C--;
		Specific_Value(TIMX_3, 3, 5); // 控制舵机
	}
}
