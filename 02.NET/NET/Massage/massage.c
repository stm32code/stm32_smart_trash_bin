#include "massage.h"
// �����豸
#include "esp8266.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"

#include "git.h"
#include "relay.h"

// ��ȡȫ�ֱ���
extern Data_TypeDef Data_init;                 // �豸����
extern Device_Satte_Typedef device_state_init; // �豸״̬
extern Check_Hight_Typedef check_hight_init;   // �߶�
U8 APP = 0;
unsigned char MQTT_FillBuf(char *buf)
{
    char text[126];
    memset(text, 0, sizeof(text));

    strcpy(buf, "{");

    memset(text, 0, sizeof(text));
    sprintf(text, "\"BOX_A\":\"%d\",", check_hight_init.check_A); // Temp����������һ�����ƣ�temper���¶�ֵ
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "\"BOX_B\":\"%d\",", check_hight_init.check_B); // Temp����������һ�����ƣ�temper���¶�ֵ
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "\"BOX_C\":\"%d\",", check_hight_init.check_C); // Temp����������һ�����ƣ�temper���¶�ֵ
    strcat(buf, text);
	
    memset(text, 0, sizeof(text));
    sprintf(text, "\"Somg\":\"%d\",",CKECK4); // Temp����������һ�����ƣ�temper���¶�ֵ
    strcat(buf, text);
    memset(text, 0, sizeof(text));
		
    sprintf(text, "\"Relay\":\"%d\"", RELAY1); // Temp����������һ�����ƣ�temper���¶�ֵ
    strcat(buf, text);
    memset(text, 0, sizeof(text));
		
    sprintf(text, "}");
    strcat(buf, text);

    return strlen(buf);
}
// ����json����
mySta massage_parse_json(char *message)
{
    char *dataPtr = NULL;

    char numBuf[10];
    int num = 0;

    dataPtr = strchr(message, ':'); // ����':'

    if (dataPtr != NULL) // ����ҵ���
    {
        dataPtr++;
        while (*dataPtr >= '0' && *dataPtr <= '9') // �ж��Ƿ����·��������������
        {
            numBuf[num++] = *dataPtr++;
        }
        numBuf[num] = 0;

        num = atoi((const char *)numBuf);     // תΪ��ֵ��ʽ
        if (strstr((char *)message, "Relay")) // ����"redled"
        {
            if (num)
            {
                RELAY1_ON
                APP = 10;
            }
        }
    }
    return MY_SUCCESSFUL;
}

// �豸���ݰ�
mySta data_json_init()
{
    char buf[128];
		short body_len = 0;
    body_len=MQTT_FillBuf(buf);
    ESP8266_SendData((unsigned char *)buf, body_len); // �������ݽڵ�
    return MY_SUCCESSFUL;
}
