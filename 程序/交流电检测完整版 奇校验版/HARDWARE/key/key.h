#ifndef __KEY_H
#define	__KEY_H

#include "stm32f10x.h"

#define KEY_GPIO_Control  GPIOB
#define KEY_Pin_k3   GPIO_Pin_6	//上
#define KEY_Pin_k4   GPIO_Pin_5	//下
#define KEY_Pin_k2   GPIO_Pin_12	//返回
#define KEY_Pin_k1   GPIO_Pin_4	//确定

void EXTI_PB4_Config(void);
void EXTI_PB5_Config(void);
void EXTI_PB6_Config(void);
void EXTI_PB12_Config(void);

void RELAY_LED_Init(void);
void BUZZER_Init(void);

#define Run_led_on       GPIO_ResetBits(GPIOB,GPIO_Pin_0);
#define Run_led_off      GPIO_SetBits(GPIOB,GPIO_Pin_0);

#define Fault_led_on     GPIO_ResetBits(GPIOB,GPIO_Pin_1);
#define Fault_led_off    GPIO_SetBits(GPIOB,GPIO_Pin_1);

#define Relay_off        GPIO_SetBits(GPIOB,GPIO_Pin_7);
#define Relay_on         GPIO_ResetBits(GPIOB,GPIO_Pin_7);

#define BUZZER_off         GPIO_ResetBits(GPIOA,GPIO_Pin_5);
#define BUZZER_on         GPIO_SetBits(GPIOA,GPIO_Pin_5);

u8 Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin);
#endif 

