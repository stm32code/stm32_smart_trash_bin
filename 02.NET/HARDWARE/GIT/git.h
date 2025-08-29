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

// �Զ���ִ�н������
typedef enum
{
	MY_SUCCESSFUL = 0x01, // �ɹ�
	MY_FAIL = 0x00		  // ʧ��

} mySta; // �ɹ���־λ

typedef enum
{
	OPEN = 0x01, // ��
	CLOSE = 0x00 // �ر�

} On_or_Off_TypeDef; // �ɹ���־λ

typedef enum
{
	DERVICE_SEND = 0x00, // �豸->ƽ̨
	PLATFORM_SEND = 0x01 // ƽ̨->�豸

} Send_directino; // ���ͷ���

typedef struct
{
	U8 light;		  // ����
	U8 temperatuer;	  // �¶�
	U8 humidity;	  // ʪ��
	U8 soil_humidity; // ����ʪ��
	F32 somg;		  // ����Ũ��

} Data_TypeDef; // ���ݲ����ṹ��

typedef struct
{
	U8 led_state;		  // LED״̬
	U8 hot_led_state;	  // ��ů��״̬
	U8 fan_state;		  // ����״̬
	U8 water_pump1_state; // ˮ��һ״̬
	U8 water_pump2_state; // ˮ�ö�״̬

} Device_Satte_Typedef; // ״̬�����ṹ��

/**************************************************************************************
 * ��  �� : ��ȡ���ݲ���
 * ��  �� : 8
 * ����ֵ : uint8_t
 **************************************************************************************/
void Read_Data(Data_TypeDef *Data);

#endif
