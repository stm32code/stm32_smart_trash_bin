#include "massage.h"
// 网络设备
#include "esp8266.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"

#include "git.h"
#include "relay.h"

// 获取全局变量
extern Data_TypeDef Data_init;                 // 设备数据
extern Device_Satte_Typedef device_state_init; // 设备状态
extern Check_Hight_Typedef check_hight_init;   // 高度
U8 APP = 0;
unsigned char MQTT_FillBuf(char *buf)
{
    char text[126];
    memset(text, 0, sizeof(text));

    strcpy(buf, "{");

    memset(text, 0, sizeof(text));
    sprintf(text, "\"BOX_A\":\"%d\",", check_hight_init.check_A); // Temp是数据流的一个名称，temper是温度值
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "\"BOX_B\":\"%d\",", check_hight_init.check_B); // Temp是数据流的一个名称，temper是温度值
    strcat(buf, text);

    memset(text, 0, sizeof(text));
    sprintf(text, "\"BOX_C\":\"%d\",", check_hight_init.check_C); // Temp是数据流的一个名称，temper是温度值
    strcat(buf, text);
	
    memset(text, 0, sizeof(text));
    sprintf(text, "\"Somg\":\"%d\",",CKECK4); // Temp是数据流的一个名称，temper是温度值
    strcat(buf, text);
    memset(text, 0, sizeof(text));
		
    sprintf(text, "\"Relay\":\"%d\"", RELAY1); // Temp是数据流的一个名称，temper是温度值
    strcat(buf, text);
    memset(text, 0, sizeof(text));
		
    sprintf(text, "}");
    strcat(buf, text);

    return strlen(buf);
}
// 解析json数据
mySta massage_parse_json(char *message)
{
    char *dataPtr = NULL;

    char numBuf[10];
    int num = 0;

    dataPtr = strchr(message, ':'); // 搜索':'

    if (dataPtr != NULL) // 如果找到了
    {
        dataPtr++;
        while (*dataPtr >= '0' && *dataPtr <= '9') // 判断是否是下发的命令控制数据
        {
            numBuf[num++] = *dataPtr++;
        }
        numBuf[num] = 0;

        num = atoi((const char *)numBuf);     // 转为数值形式
        if (strstr((char *)message, "Relay")) // 搜索"redled"
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

// 设备数据包
mySta data_json_init()
{
    char buf[128];
		short body_len = 0;
    body_len=MQTT_FillBuf(buf);
    ESP8266_SendData((unsigned char *)buf, body_len); // 发送数据节点
    return MY_SUCCESSFUL;
}
