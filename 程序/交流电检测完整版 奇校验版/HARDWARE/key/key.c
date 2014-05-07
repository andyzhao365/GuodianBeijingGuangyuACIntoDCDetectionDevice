#include"key.h"
#include "stm32f10x_exti.h"




void EXTI_PB4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC0) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE); 

	/* config the NVIC(PC0) */
// 	NVIC_Configuration();

	/* EXTI line gpio config(PC0) */	
  GPIO_InitStructure.GPIO_Pin = KEY_Pin_k1;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	          // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line(PC0) mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}



void EXTI_PB5_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC0) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE); 
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);

	/* config the NVIC(PC0) */
// 	NVIC_Configuration();

	/* EXTI line gpio config(PC0) */	
  GPIO_InitStructure.GPIO_Pin = KEY_Pin_k4;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	          // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line(PC0) mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}


void EXTI_PB6_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC0) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE); 

	/* config the NVIC(PC0) */
// 	NVIC_Configuration();

	/* EXTI line gpio config(PC0) */	
  GPIO_InitStructure.GPIO_Pin = KEY_Pin_k3;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	          // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line(PC0) mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}

void EXTI_PB12_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PC0) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE); 

	/* config the NVIC(PC0) */
// 	NVIC_Configuration();

	/* EXTI line gpio config(PC0) */	
  GPIO_InitStructure.GPIO_Pin = KEY_Pin_k2;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	          // 上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* EXTI line(PC0) mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line12;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}


void RELAY_LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7;				 //LED0-->PB0 端口配置		 绿
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_SetBits(GPIOB,GPIO_Pin_0);						   //PB0 led灯 
//  GPIO_SetBits(GPIOB,GPIO_Pin_1); 						 //PB1 led灯 
//  GPIO_SetBits(GPIOB,GPIO_Pin_7); 						 //pb.7继电器
//  GPIO_ResetBits(GPIOB,GPIO_Pin_7);						   //PB0 led灯 
//蜂鸣器pa5
}

void BUZZER_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB0 端口配置		 绿
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);
//  GPIO_SetBits(GPIOB,GPIO_Pin_0);						   //PB0 led灯 
    GPIO_SetBits(GPIOB,GPIO_Pin_1); 						 //PB1 led灯 
//  GPIO_SetBits(GPIOB,GPIO_Pin_7); 						 //pb.7继电器
//  GPIO_ResetBits(GPIOB,GPIO_Pin_7);						   //PB0 led灯 
//蜂鸣器pa5
}
