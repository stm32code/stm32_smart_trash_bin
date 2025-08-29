#include "pwm.h"
#include "math.h"
#include "delay.h"
#include "timer.h"
#include "usart.h"
// ȫ�ֱ���
extern U8 time_6; // ms ��ʱ����
// �ֲ�����
xPeriod_xPrescaler xperiod_xprescaler;

/*********************************************************************************
 * @Function	: �����Ƶ��
 * @Input		: F :Ƶ�ʣ�HZ��
 *                TIMX :TIMѡ��
 * @Output		: None
 * @Return		: None
 * @Others		: None
 * @Date		: 2022-11-2
 **********************************************************************************/
static void Math_Cycle(TimeX_Module TIMX, U32 F, xPeriod_xPrescaler *Data)
{
    // ��������ʵķ�Ƶ����װֵ-------------------------------------------
    float midFloat, clkFloat;
    long clkInt;
    long midInt;
    clkFloat = 72000000.0f / F;
    if (clkFloat - (long)clkFloat >= 0.5f)
        clkInt = clkFloat + 1;
    else
        clkInt = (long)clkFloat;

    midFloat = sqrt(clkFloat); // ����
    if (midFloat - (long)midFloat >= 0.5f)
        midInt = (long)midFloat + 1;
    else
        midInt = (long)midFloat;
    // ��һ����ӽ���
    for (int i = midInt; i >= 1; i--)
    {
        if (clkInt % i == 0)
        {
            switch (TIMX)
            {
            case TIMX_1:
                Data->TIM1_Period = i - 1;
                Data->TIM1_Prescaler = clkInt / i + 1;
                break;
            case TIMX_2:
                Data->TIM2_Period = i - 1;
                Data->TIM2_Prescaler = clkInt / i + 1;
                break;
            case TIMX_3:
                Data->TIM3_Period = i - 1;
                Data->TIM3_Prescaler = clkInt / i + 1;
                break;
            case TIMX_4:
                Data->TIM4_Period = i - 1;
                Data->TIM4_Prescaler = clkInt / i + 1;
                break;
            case TIMX_5:
                Data->TIM5_Period = i - 1;
                Data->TIM5_Prescaler = clkInt / i + 1;
                break;
            case TIMX_8:
                Data->TIM8_Period = i - 1;
                Data->TIM8_Prescaler = clkInt / i + 1;
                break;
            default:
                break;
            }
            break;
        }
    }
}
/*********************************************************************************
 * @Function	: TIM1 PWM�����ʼ��
 * @Input		: F :Ƶ�ʣ�HZ��
 * @Output		: None
 * @Return		: None
 * @Others		: None
 * @Date		: 2022-11-2
 **********************************************************************************/
void TIM1_PWM_Init(U32 f)
{
    Math_Cycle(TIMX_1, f, &xperiod_xprescaler); // ������ʵ�PWM

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphClockCmd(TIM1_GPIO_CLK_TIME1_2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(TIM1_GPIO_PORT_TIME1_2, &GPIO_InitStructure);

    TIM_DeInit(TIM1);                               // ��ʱ��1��ʼ��
    TIM_InternalClockConfig(TIM1);                  /*ʱ��ѡ��*/
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); // ��ʼ����ʱ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = xperiod_xprescaler.TIM1_Period - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = xperiod_xprescaler.TIM1_Prescaler - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /*��ʱ��ͨ��1�ıȽ�ֵ��������ԣ���ʱ���Ĺ���ģʽ*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;            // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; // ����ʱ�����ƽΪ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    // ��������С��TIM_Pulse��ֵ�����Ϊ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

    // ��ʼ��TIM1 Channel 1 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR0��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  // ����ʱ1NҲ����ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;     // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    // TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM1, ENABLE); // ʹ���Զ���װ�ؼĴ�����arr��Ԥװ��
    // TIM_Cmd(TIM1, ENABLE);
}
/*********************************************************************************
 * @Function	: TIM8 PWM�����ʼ��
 * @Input		: F :Ƶ�ʣ�HZ��
 * @Output		: None
 * @Return		: None
 * @Others		: None
 * @Date		: 2022-11-2
 **********************************************************************************/
void TIM8_PWM_Init(U32 f)
{
    Math_Cycle(TIMX_8, f, &xperiod_xprescaler); // ������ʵ�PWM

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    RCC_APB2PeriphClockCmd(TIM8_GPIO_CLK_TIME2_3, ENABLE);

    // ����PA0��PA1Ϊ����������ܣ����TIM5 CH1��TIM5 CH2��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = TIM8_GPIO_PIN3 | TIM8_GPIO_PIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TIM8_GPIO_PORT_TIME2_3, &GPIO_InitStructure);

    GPIO_WriteBit(TIM8_GPIO_PORT_TIME2_3, TIM8_GPIO_PIN2 | TIM8_GPIO_PIN3, Bit_RESET); // �ϵ��ʱ������Ϊ0����ֹ�ϵ�˲��Ϊ��

    TIM_DeInit(TIM8);                               // ��ʱ��1��ʼ��
    TIM_InternalClockConfig(TIM8);                  /*ʱ��ѡ��*/
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); // ��ʼ����ʱ��
    /*������ʱ�����ã���Ҫ�Ƕ�ʱ����Ƶ�Ͷ�ʱ������*/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                      // ʱ�ӷ�Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                  // ����ģʽΪ���ϼ���
    TIM_TimeBaseStructure.TIM_Period = xperiod_xprescaler.TIM8_Period - 1;       // �������ڷ�Χ0000~0XFFFF����������Ϊ72,���PWM��Ƶ��Ϊ1M
    TIM_TimeBaseStructure.TIM_Prescaler = xperiod_xprescaler.TIM8_Prescaler - 1; // ��clkʱ�ӽ���(0+1)��Ƶ,�����1��Ƶ��clk=72M/(0+1)=72M
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);                              // ��ʼ��������ʱ��

    /*��ʱ��ͨ��1�ıȽ�ֵ��������ԣ���ʱ���Ĺ���ģʽ*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;            // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; // ����ʱ�����ƽΪ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    // ��������С��TIM_Pulse��ֵ�����Ϊ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // TIM_OCInitStructure.TIM_Pulse = Period / 2; //�������Ƚ�ֵ��ռ�ձ�Ϊ50%

    // ��ʼ��TIM8 Channel 2 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;   // ����ʱ1NҲ����׵�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC2Init(TIM8, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);

    // ��ʼ��TIM8 Channel 3 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;   // ����ʱ1NҲ����ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC3Init(TIM8, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM8, ENABLE);
    TIM_ARRPreloadConfig(TIM8, ENABLE); // ʹ���Զ���װ�ؼĴ�����arr��Ԥװ��
    TIM_Cmd(TIM8, ENABLE);              // ʹ��TIM8
}
/*********************************************************************************
 * @Function	: TIM2 PWM�����ʼ��
 * @Input		: F :Ƶ�ʣ�HZ��
 * @Output		: None
 * @Return		: None
 * @Others		: None
 * @Date		: 2022-11-2
 **********************************************************************************/
void TIM2_PWM_Init(U32 f)
{
    Math_Cycle(TIMX_2, f, &xperiod_xprescaler); // ������ʵ�PWM

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(TIM2_GPIO_CLK_TIME3_4, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); // ��仰������ԭ�ӹٷ����̡�PWM���ʵ�顱�и��ģ�Ŀ����Timer3��ȫ��ӳ�䣬��TIM3_CH4->PC9����ʵ�鲻����ӳ��

    // ����PA0��PA1Ϊ����������ܣ����TIM5 CH1��TIM5 CH2��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = TIM2_GPIO_PIN3 | TIM2_GPIO_PIN4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TIM2_GPIO_PORT_TIME3_4, &GPIO_InitStructure);

    GPIO_WriteBit(TIM2_GPIO_PORT_TIME3_4, TIM2_GPIO_PIN3 | TIM2_GPIO_PIN4, Bit_RESET); // �ϵ��ʱ������Ϊ0����ֹ�ϵ�˲��Ϊ��

    TIM_DeInit(TIM2); // ��ʱ��1��ʼ��
    /*--------------------ʱ���ṹ���ʼ��-------------------------*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = xperiod_xprescaler.TIM2_Period - 1;
    // ����CNT��������ʱ�� = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler = xperiod_xprescaler.TIM2_Prescaler - 1;
    // ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /*--------------------����ȽϽṹ���ʼ��-------------------*/
    TIM_OCInitTypeDef TIM_OCInitStructure;
    // ����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // ���ʹ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // ���ͨ����ƽ��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; // ����ʱ�����ƽΪ��

    // ��ʼ��TIM5 Channel 1 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;   // ����ʱ1NҲ����׵�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);                        // ����Ƚ�ͨ�� 3
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // ��ʼ��TIM5 Channel 2 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;   // ����ʱ1NҲ����ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);                        // ����Ƚ�ͨ�� 4
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE); // ʹ���Զ���װ�ؼĴ�����arr��Ԥװ��
}
/*********************************************************************************
 * @Function	: TIM3 PWM�����ʼ��
 * @Input		: F :Ƶ�ʣ�HZ��
 * @Output		: None
 * @Return		: None
 * @Others		: None
 * @Date		: 2022-11-2
 **********************************************************************************/
void TIM3_PWM_Init(U32 f)
{
    Math_Cycle(TIMX_3, f, &xperiod_xprescaler); // ������ʵ�PWM

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(TIM3_GPIO_CLK_TIME3_4 | TIM3_GPIO_CLK_TIME1_2, ENABLE);
    //  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); // ��仰������ԭ�ӹٷ����̡�PWM���ʵ�顱�и��ģ�Ŀ����Timer3��ȫ��ӳ�䣬��TIM3_CH4->PC9����ʵ�鲻����ӳ��
    //  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //��仰������ԭ�ӹٷ����̡�PWM���ʵ�顱�и��ģ�Ŀ����Timer3������ӳ�䣬��TIM3_CH2->PB5����ʵ�鲻����ӳ��

    // ����PC6��PC7Ϊ����������ܣ����TIM3 CH1��TIM3 CH2��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = TIM3_GPIO_PIN1 | TIM3_GPIO_PIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TIM3_GPIO_PORT_TIME1_2, &GPIO_InitStructure);

    // ����PC8��PC9Ϊ����������ܣ����TIM3 CH3��TIM3 CH4��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = TIM3_GPIO_PIN4 | TIM3_GPIO_PIN3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TIM3_GPIO_PORT_TIME3_4, &GPIO_InitStructure);

    TIM_DeInit(TIM3); // ��ʱ��1��ʼ��
    /*--------------------ʱ���ṹ���ʼ��-------------------------*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = xperiod_xprescaler.TIM3_Period - 1;
    // ����CNT��������ʱ�� = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler = xperiod_xprescaler.TIM3_Prescaler - 1;
    // ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    /*--------------------����ȽϽṹ���ʼ��-------------------*/
    TIM_OCInitTypeDef TIM_OCInitStructure;
    // ����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    // ���ʹ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // ����ʱ�����ƽΪ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    // ���ͨ����ƽ��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = xperiod_xprescaler.TIM3_Period / 2; // �������Ƚ�ֵ��ռ�ձ�Ϊ50%

    // ��ʼ��TIM3 Channel 1 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  // ����ʱ1NҲ����׵�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                        // ����Ƚ�ͨ�� 1
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // ��ʼ��TIM3 Channel 2 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  // ����ʱ1NҲ����ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                        // ����Ƚ�ͨ�� 2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // ��ʼ��TIM3 Channel 3 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  // ����ʱ1NҲ����׵�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);                        // ����Ƚ�ͨ�� 3
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // ��ʼ��TIM3 Channel 4 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;  // ����ʱ1NҲ����ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);                        // ����Ƚ�ͨ�� 4
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM3, ENABLE);
    TIM_ARRPreloadConfig(TIM3, ENABLE); // ʹ���Զ���װ�ؼĴ�����arr��Ԥװ��
    TIM_Cmd(TIM3, ENABLE);
}
/*********************************************************************************
 * @Function	: TIM4 PWM�����ʼ��
 * @Input		: F :Ƶ�ʣ�HZ��
 * @Output		: None
 * @Return		: None
 * @Others		: None
 * @Date		: 2022-11-2
 **********************************************************************************/
void TIM4_PWM_Init(U32 f)
{
    Math_Cycle(TIMX_4, f, &xperiod_xprescaler); // ������ʵ�PWM

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(TIM2_GPIO_CLK_TIME3_4, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); // ��仰������ԭ�ӹٷ����̡�PWM���ʵ�顱�и��ģ�Ŀ����Timer3��ȫ��ӳ�䣬��TIM3_CH4->PC9����ʵ�鲻����ӳ��

    // ����PA0��PA1Ϊ����������ܣ����TIM5 CH1��TIM5 CH2��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = TIM4_GPIO_PIN3 | TIM4_GPIO_PIN4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TIM4_GPIO_PORT_TIME3_4, &GPIO_InitStructure);

    GPIO_WriteBit(TIM4_GPIO_PORT_TIME3_4, TIM4_GPIO_PIN3 | TIM4_GPIO_PIN4, Bit_RESET); // �ϵ��ʱ������Ϊ0����ֹ�ϵ�˲��Ϊ��

    TIM_DeInit(TIM4); // ��ʱ��1��ʼ��
    /*--------------------ʱ���ṹ���ʼ��-------------------------*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = xperiod_xprescaler.TIM4_Period - 1;
    // ����CNT��������ʱ�� = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler = xperiod_xprescaler.TIM4_Prescaler - 1;
    // ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    /*--------------------����ȽϽṹ���ʼ��-------------------*/
    TIM_OCInitTypeDef TIM_OCInitStructure;
    // ����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // ���ʹ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // ���ͨ����ƽ��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    // ����ʱ�����ƽΪ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

    // ����Ƚ�ͨ�� 3
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    // ����Ƚ�ͨ�� 4
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE); // ʹ���Զ���װ�ؼĴ�����arr��Ԥװ��
}
/*********************************************************************************
 * @Function	: TIM5 PWM�����ʼ��
 * @Input		: F :Ƶ�ʣ�HZ��
 * @Output		: None
 * @Return		: None
 * @Others		: None
 * @Date		: 2022-11-2
 **********************************************************************************/
void TIM5_PWM_Init(U32 f)
{
    Math_Cycle(TIMX_5, f, &xperiod_xprescaler); // ������ʵ�PWM

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB2PeriphClockCmd(TIM5_GPIO_CLK_TIME1_2, ENABLE);
    // GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //��仰������ԭ�ӹٷ����̡�PWM���ʵ�顱�и��ģ�Ŀ����Timer3��ȫ��ӳ�䣬��TIM3_CH4->PC9����ʵ�鲻����ӳ��
    // GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //��仰������ԭ�ӹٷ����̡�PWM���ʵ�顱�и��ģ�Ŀ����Timer3������ӳ�䣬��TIM3_CH2->PB5����ʵ�鲻����ӳ��

    // ����PA0��PA1Ϊ����������ܣ����TIM5 CH1��TIM5 CH2��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = TIM5_GPIO_PIN1 | TIM5_GPIO_PIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // �����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TIM5_GPIO_PORT_TIME1_2, &GPIO_InitStructure);

    GPIO_WriteBit(TIM5_GPIO_PORT_TIME1_2, TIM5_GPIO_PIN1 | TIM5_GPIO_PIN2, Bit_RESET); // �ϵ��ʱ������Ϊ0����ֹ�ϵ�˲��Ϊ��

    TIM_DeInit(TIM5);                               // ��ʱ��1��ʼ��
    TIM_InternalClockConfig(TIM5);                  /*ʱ��ѡ��*/
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); // ��ʼ����ʱ��
    /*������ʱ�����ã���Ҫ�Ƕ�ʱ����Ƶ�Ͷ�ʱ������*/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                      // ʱ�ӷ�Ƶ����
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                  // ����ģʽΪ���ϼ���
    TIM_TimeBaseStructure.TIM_Period = xperiod_xprescaler.TIM5_Period - 1;       // �������ڷ�Χ0000~0XFFFF����������Ϊ72,���PWM��Ƶ��Ϊ1M
    TIM_TimeBaseStructure.TIM_Prescaler = xperiod_xprescaler.TIM5_Prescaler - 1; // ��clkʱ�ӽ���(0+1)��Ƶ,�����1��Ƶ��clk=72M/(0+1)=72M
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);                              // ��ʼ��������ʱ��

    /*��ʱ��ͨ��1�ıȽ�ֵ��������ԣ���ʱ���Ĺ���ģʽ*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;            // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; // ����ʱ�����ƽΪ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    // ��������С��TIM_Pulse��ֵ�����Ϊ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // TIM_OCInitStructure.TIM_Pulse = Period / 2; //�������Ƚ�ֵ��ռ�ձ�Ϊ50%

    // ��ʼ��TIM5 Channel 1 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;    // ����ʱ1NҲ����׵�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC1Init(TIM5, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);

    // ��ʼ��TIM5 Channel 2 PWM
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               // ����ΪPWM2ģʽ��TIMx_CNT>TIMx_CCR1��Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;    // ����ʱ1NҲ����ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;      // ��ʱ������֮������͵�ƽ
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; // ʹ�����
    TIM_OC2Init(TIM5, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);

    // TIM_CtrlPWMOutputs(TIM5, ENABLE);
    TIM_ARRPreloadConfig(TIM5, ENABLE); // ʹ���Զ���װ�ؼĴ�����arr��Ԥװ��
    // TIM_Cmd(TIM5, ENABLE);              //ʹ��TIM5
}
/*********************************************************************************
 * @Function	: PWMָ�����
 * @Input		: PWM_Port ͨ�������޸�
 *                Value ռ�ձȱ�ֵ
 *                TIMx TIMѡ��
 * @Output		: None
 * @Return		: None
 * @Others		: �û�����ֱ�ӹ涨PWMռ�ձ�
 * @Date		: 2022-11-2
 **********************************************************************************/
void Specific_Value(TimeX_Module TIMX, U8 PWM_Port, float Value)
{
    U16 num = 0;
    TIM_TypeDef *TIMx;
    switch (TIMX)
    {
    case TIMX_1:
        num = xperiod_xprescaler.TIM1_Period * Value / 100;
        TIMx = TIM1;
        break;
    case TIMX_2:
        num = xperiod_xprescaler.TIM2_Period * Value / 100;
        TIMx = TIM2;
        break;
    case TIMX_3:
        num = xperiod_xprescaler.TIM3_Period * Value / 100;
        TIMx = TIM3;
        break;
    case TIMX_4:
        num = xperiod_xprescaler.TIM4_Period * Value / 100;
        TIMx = TIM4;
        break;
    case TIMX_5:
        num = xperiod_xprescaler.TIM5_Period * Value / 100;
        TIMx = TIM5;
        break;
    case TIMX_8:
        num = xperiod_xprescaler.TIM8_Period * Value / 100;
        TIMx = TIM8;
        break;
    default:
        break;
    }

    switch (PWM_Port)
    {
    case 1:
        TIM_SetCompare1(TIMx, num); // �޸ıȽ�ֵ���޸�ռ�ձ�
        break;
    case 2:
        TIM_SetCompare2(TIMx, num); // �޸ıȽ�ֵ���޸�ռ�ձ�
        break;
    case 3:
        TIM_SetCompare3(TIMx, num); // �޸ıȽ�ֵ���޸�ռ�ձ�
        break;
    case 4:
        TIM_SetCompare4(TIMx, num); // �޸ıȽ�ֵ���޸�ռ�ձ�
        break;
    }
}
