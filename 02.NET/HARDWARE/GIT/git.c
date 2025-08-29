#include "git.h"

#include "dht11.h"

#include "adc.h"
#include "bh1750.h"

DHT11_Data_TypeDef DHT11_Data;

Data_TypeDef Data_init;
/**************************************************************************************
 * ��  �� : ��ȡ���ݲ���
 * ��  �� : 8
 * ����ֵ : uint8_t
 **************************************************************************************/
void Read_Data(Data_TypeDef *Device_Data)
{
	Read_DHT11(&DHT11_Data); // ��ȡ��ʪ������

	// Device_Data->somg = 6000 - Smog_Trans_Concentration(); // ��ȡ��������
	Device_Data->light = getValue(); // ��ȡ���ǿ��
}
