#ifndef __RTC_H
#define	__RTC_H


#include "stm32f10x.h"

/* 如果定义了下面这个宏的话,PC13就会输出频率为RTC Clock/64的时钟 */   
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

void RTC_Configuration(void);
void NVIC_Configuration(void);
void  Time_Display(uint32_t TimeVar);
void Time_Show(void);
void RTC_Use(void);
void RTC_IRQHandler(void);
void  Time_Adjust(void);
u8 Month(u8 day);
typedef struct 

{
       u8 hour;
       u8 min;
       u8 sec;                  
       u16 w_year;
       u8  w_month;
       u8  w_date;
       u8  week;             
}tm; 
extern tm timer;
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);
#endif

