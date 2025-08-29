#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "git.h"

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define ADC_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ADC_CLK RCC_APB2Periph_ADC1

#define ADC_GPIO_APBxClock_FUN RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK RCC_APB2Periph_GPIOA
#define ADC_PORT GPIOA
// ת��ͨ������
#define NOFCHANEL 4

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];


// ע��
// 1-PC0 ��ָ��������ӵ��Ƿ�������Ĭ�ϱ�����
// 2-PC0 ��ָ��������ӵ���SPI FLASH�� Ƭѡ��Ĭ�ϱ�����
// ���� PC0 �� ADC ת��ͨ����ʱ�򣬽�����ܻ������

#define ADC_PIN1 GPIO_Pin_0
#define ADC_CHANNEL1 ADC_Channel_0

#define ADC_PIN2 GPIO_Pin_3
#define ADC_CHANNEL2 ADC_Channel_3

#define ADC_PIN3 GPIO_Pin_4
#define ADC_CHANNEL3 ADC_Channel_4

#define ADC_PIN4 GPIO_Pin_5
#define ADC_CHANNEL4 ADC_Channel_5

// #define    ADC_PIN5                      GPIO_Pin_4
// #define    ADC_CHANNEL5                  ADC_Channel_14

// #define    ADC_PIN6                      GPIO_Pin_5
// #define    ADC_CHANNEL6                  ADC_Channel_15

// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
#define ADC_x ADC1
#define ADC_DMA_CHANNEL DMA1_Channel1
#define ADC_DMA_CLK RCC_AHBPeriph_DMA1

/**************************��������********************************/
void ADCx_Init(void);
/*********************************************************************************
 * @Function	:  ADC�����������
 * @Input		:  value ��ε�ѹ�ɼ�������
 * @Output		:  None
 * @Return		:  ����м��ֵ������ʵ��ֵ
 * @Others		:  �Բ����õ���5�����ݽ���ð������
 * @Date			:  2022-11-17
 **********************************************************************************/
U16 ADC_Result_Calculate(U16 *value);

U16 MQ_135_Value(void);
#endif /* __ADC_H */
