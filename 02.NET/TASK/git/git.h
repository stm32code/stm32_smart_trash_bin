#ifndef __GIT_H
#define __GIT_H
#include "stdio.h"

typedef unsigned char U8;
typedef signed char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef unsigned int U32;
typedef signed int S32;
typedef float F32;

#define HEGHT 1000	 // 高度阈值
#define TIME_OVER 10 // 垃圾桶自动关闭时间（10秒）

// 自定义执行结果类型
typedef enum
{
	MY_SUCCESSFUL = 0x01, // 成功
	MY_FAIL = 0x00		  // 失败

} mySta; // 成功标志位

typedef enum
{
	OPEN = 0x01, // 打开
	CLOSE = 0x00 // 关闭

} On_or_Off_TypeDef; // 成功标志位

typedef enum
{
	DERVICE_SEND = 0x00, // 设备->平台
	PLATFORM_SEND = 0x01 // 平台->设备

} Send_directino; // 发送方向

typedef struct
{
	U8 light;		  // 光照
	U8 soil_humidity; // 土壤湿度
	F32 somg;		  // 烟雾浓度
	F32 temperatuer;  // 温度

} Data_TypeDef; // 数据参数结构体

typedef struct
{
	U8 threshold_temp_min;			// 室内温度
	U8 threshold_humi_min;			// 室内湿度
	U8 threshold_light_min;			// 光照
	U8 threshold_soil_humidity_max; // 土壤湿度
	U8 threshold_soil_humidity_min; // 土壤湿度
	F32 threshold_somg_max;			// 烟雾浓度
	F32 threshold_somg_min;			// 烟雾浓度
	F32 threshold_temperatuer_max;	// 温度
	F32 threshold_temperatuer_min;	// 温度

} Threshold_Value_TypeDef; // 数据参数结构体

typedef struct
{
	U8 taash_can_A; // 垃圾桶A状态
	U8 taash_can_B; // 垃圾桶B状态
	U8 taash_can_C; // 垃圾桶C状态
	U8 taash_can_D; // 垃圾桶D状态
	

} Device_Satte_Typedef; // 状态参数结构体

typedef struct
{
	U8 check_A; // 高度1
	U8 check_B; // 高度2
	U8 check_C; // 高度3
	U8 check_D; // 高度4
	U16  MQ_135; // 浓度

} Check_Hight_Typedef; // 监测人体和升降电压数据

// 获取数据参数
mySta Read_Data(Data_TypeDef *Device_Data);
// 初始化
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *threshold_value, Device_Satte_Typedef *device_state, Check_Hight_Typedef *check_hight);
// 更新OLED显示屏中内容
mySta Update_Oled_Massage(void);
// 更新设备状态
mySta Update_Device_Massage(Device_Satte_Typedef *device_state);
// 检测是否溢出
mySta Check_Rubbish_Massage(Check_Hight_Typedef *hight_data);
// 自动检测
void Check_Key_ON_OFF(Device_Satte_Typedef *device_state);
// 自动关闭舵机
void Automation_Close(Device_Satte_Typedef *device_state);
#endif
