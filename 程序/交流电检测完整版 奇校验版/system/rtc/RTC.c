#include "RTC.h"
// #include "key.h"
// #include "modbus.h"
// #include "LCD12864.h"
#include "stdio.h"
u8 TimeDisplay=0;
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
// extern void ADC_work(void);
// extern u16 bj;
// extern u8 flagSystem;
// extern u8 cc;
// u8  TimeDisplay;
// u8  TimeDay=0;

// u8   hh_H=0, hh_L=0, mm_H=0, mm_L=0, ss_H=0, ss_L=0;
// u8   year_H, year_L, month_H, month_L, day_H, day_L;

// u8 	 hh, mm, ss, year, month, day;

// u16  iii = 0;

// u8  rccflag=1;

tm timer;
u8 Is_Leap_Year(u16 year)
{                    
       if(year%4==0) 
       {
              if(year%100==0)
              {
                     if(year%400==0)return 1;         
                     else return 0;  
              }else return 1;  
       }else return 0;
}                           
                                                                      
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5};

const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
       u16 t;
       u32 seccount=0;
       if(syear<2000||syear>2099)return 1;      
       for(t=1970;t<syear;t++) 
       {
              if(Is_Leap_Year(t))seccount+=31622400;
              else seccount+=31536000;                  
       }
       smon-=1;
       for(t=0;t<smon;t++)         
       {
              seccount+=(u32)mon_table[t]*86400;
              if(Is_Leap_Year(syear)&&t==1)seccount+=86400;      
       }
       seccount+=(u32)(sday-1)*86400;
       seccount+=(u32)hour*3600;
    seccount+=(u32)min*60;      
       seccount+=sec;
                                                                                              
  
       PWR_BackupAccessCmd(ENABLE);
       RTC_WaitForLastTask();
       RTC_SetCounter(seccount);
       RTC_WaitForLastTask();
       return 0;     
}

u8 RTC_Get(void)
{
       static u16 daycnt=0;
       u32 timecount=0;
       u32 temp=0;
       u16 temp1=0;
 
       timecount=RTC_GetCounter();
         
       temp=timecount/86400;   
       if(daycnt!=temp)
       {      
              daycnt=temp;
              temp1=1970; 
              while(temp>=365)
              {                         
                     if(Is_Leap_Year(temp1))
                     {
                            if(temp>=366)temp-=366;
                            else {temp1++;break;} 
                     }
                     else temp-=365;       
                     temp1++; 
              }  
              timer.w_year=temp1;
              temp1=0;
              while(temp>=28)
              {
                     if(Is_Leap_Year(timer.w_year)&&temp1==1)
                     {
                            if(temp>=29)temp-=29;
                            else break;
                     }
                     else
                     {
                            if(temp>=mon_table[temp1])temp-=mon_table[temp1];
                            else break;
                     }
                     temp1++; 
              }
              timer.w_month=temp1+1;
              timer.w_date=temp+1;  
       }
       temp=timecount%86400;          
       timer.hour=temp/3600;     
       timer.min=(temp%3600)/60;     
       timer.sec=(temp%3600)%60; 
       timer.week=RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);
       return 0;
}    
                                                                                                                                                  
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{    
       u16 temp2;
       u8 yearH,yearL;
      
       yearH=year/100;     yearL=year%100;
       
       if (yearH>19)yearL+=100;
      
       temp2=yearL+yearL/4;
       temp2=temp2%7;
       temp2=temp2+day+table_week[month-1];
       if (yearL%4==0&&month<3)temp2--;
       return(temp2%7);
}
tm timer;

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			     	//设置串口1中断   外部中断12
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	     	//抢占优先级 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				      //子优先级为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					        //使能
		NVIC_Init(&NVIC_InitStructure);
		
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			     		//设置串口1中断    外部中断56
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	    	//抢占优先级 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				    	//子优先级为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					      	//使能
		NVIC_Init(&NVIC_InitStructure);
		
		
		NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			     	   	//设置串口1中断    外部中断4
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	  		//抢占优先级 0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				  		//子优先级为0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					    		//使能
		NVIC_Init(&NVIC_InitStructure);
    
    
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
  
    
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void RTC_Configuration(void)
{
     u16 u16_WaitForOscSource;
    if (BKP_ReadBackupRegister(BKP_DR1) != 0x5A5A)
    {
        
        /* Enable PWR and BKP clocks */
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
 
      /* Allow access to BKP Domain */
      PWR_BackupAccessCmd(ENABLE);
 
      /* Reset Backup Domain */
       BKP_DeInit();
 
      /* Enable LSE */
      RCC_LSEConfig(RCC_LSE_ON);
       for(u16_WaitForOscSource=0;u16_WaitForOscSource<5000;u16_WaitForOscSource++)
           {
           }
      /* Wait till LSE is ready */
      while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
 
      /* Select LSE as RTC Clock Source */
      RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
 
      /* Enable RTC Clock */
      RCC_RTCCLKCmd(ENABLE);
 
      /* Wait for RTC registers synchronization */
      RTC_WaitForSynchro();
 
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
 
      /* Enable the RTC Second */
      RTC_ITConfig(RTC_IT_SEC, ENABLE);
 
      /* Wait until last write operation on RTC registers has finished */
       RTC_WaitForLastTask();
 
      /* Set RTC prescaler: set RTC period to 1sec */
      RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
 
      /* Wait until last write operation on RTC registers has finished */
      RTC_WaitForLastTask();
        BKP_WriteBackupRegister(BKP_DR1, 0x5A5A);
        RTC_Set(2014,03,30,15,01,24);
    }
    else
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
            for(u16_WaitForOscSource=0;u16_WaitForOscSource<5000;u16_WaitForOscSource++);
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
           
        }
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
          
        }
   
        RCC_ClearFlag();
 
   
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
 
 
 
    return;
}





void RTC_IRQHandler(void)
{
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear the RTC Second interrupt */
		RTC_ClearITPendingBit(RTC_IT_SEC);
		
		/* Toggle GPIO_LED pin 6 each 1s */
		//GPIO_WriteBit(GPIO_LED, GPIO_Pin_6, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIO_LED, GPIO_Pin_6)));
		
		/* Enable time update */
		 TimeDisplay = 1;
		
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		/* Reset RTC Counter when Time is 23:59:59 */
		if (RTC_GetCounter() == 0x00015180)
		{
			RTC_SetCounter(0x0);
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
		}
	}
}





