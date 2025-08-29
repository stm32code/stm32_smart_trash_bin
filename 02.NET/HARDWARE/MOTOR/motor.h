#include "stm32f10x.h"

#define MOTOR_Z       1   //正转 顺时针转动
#define MOTOR_F   0   //反转 逆时针转动
#define MOTOR_SPEED           3   //转速

#define DD_0    GPIO_ResetBits(GPIOB , GPIO_Pin_6) 	//电机控制口，连接电机驱动板IN4 	
#define DD_1    GPIO_SetBits(GPIOB , GPIO_Pin_6) 	  //电机控制口，连接电机驱动板IN4 
#define CC_0    GPIO_ResetBits(GPIOB , GPIO_Pin_5) 	//电机控制口，连接电机驱动板IN3 	
#define CC_1    GPIO_SetBits(GPIOB , GPIO_Pin_5) 	  //电机控制口，连接电机驱动板IN3 
#define BB_0    GPIO_ResetBits(GPIOB , GPIO_Pin_4) 	//电机控制口，连接电机驱动板IN2 	
#define BB_1    GPIO_SetBits(GPIOB , GPIO_Pin_4) 	  //电机控制口，连接电机驱动板IN2 
#define AA_0    GPIO_ResetBits(GPIOB , GPIO_Pin_0) 	//电机控制口，连接电机驱动板IN1 	
#define AA_1    GPIO_SetBits(GPIOB , GPIO_Pin_3) 	  //电机控制口，连接电机驱动板IN1 
void System_PB34_setIO( void );
void motor_init(void);
void MotorStep(uint8_t X,uint16_t Speed);
void MotorStop(void);
void Motor_Ctrl_Angle_F(int angle,int n);
void Motor_Ctrl_Angle_Z(int angle,int n);


