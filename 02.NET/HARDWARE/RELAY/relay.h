#ifndef __RELAY_H
#define __RELAY_H

#include "stm32f10x.h"

#define BEEP_GPIO_PORT GPIOB
#define BEEP_GPIO_CLK RCC_APB2Periph_GPIOB
#define BEEP_GPIO_PIN GPIO_Pin_8

#define RELAY1_GPIO_PORT GPIOB
#define RELAY1_GPIO_CLK RCC_APB2Periph_GPIOB
#define RELAY1_GPIO_PIN GPIO_Pin_1

#define RELAY1 GPIO_ReadInputDataBit(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN) // ��ȡ����2
/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define digitalHi(p, i) \
    {                   \
        p->BSRR = i;    \
    } // ���Ϊ�ߵ�ƽ
#define digitalLo(p, i) \
    {                   \
        p->BRR = i;     \
    } // ����͵�ƽd
#define digitalToggle(p, i) \
    {                       \
        p->ODR ^= i;        \
    } // �����ת״̬

#define RELAY1_TOGGLE digitalToggle(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN)
#define RELAY1_OFF digitalLo(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN)
#define RELAY1_ON digitalHi(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN)

#define BEEP_TOGGLE digitalToggle(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_OFF digitalLo(BEEP_GPIO_PORT, BEEP_GPIO_PIN)
#define BEEP_ON digitalHi(BEEP_GPIO_PORT, BEEP_GPIO_PIN)

void RELAY_GPIO_Config(void);
/**
 * @brief  MQTT����LEDģ��
 * @param  ��
 * @retval ��
 */
void Mqtt_Relay(u8 Cmd);
#endif /* __RELAY_H */
