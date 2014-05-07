#include "operating.h"
#include "stm32f10x_it.h"
#include "show.h"
#include "delay.h"
#include "key.h"
#include "RTC.h"
#include "read_write_flash.h"
#include "uart1.h"
#include "rs485.h"
extern int flash_num;                   //控制写flash的次数
extern  u32 start_addr;
void flash_read_time_volitage(uint32_t addr) ;          
extern u8 rcc_flag;//用于切换到一级界面的参数
extern u8 Alarm_flag_1;
extern  enum key
{
	 key_up,key_down,key_ok,key_cancel,key_none
		 
}k1;
u32 Time;
extern u8 flag_i;
extern u16 voltage;
u8 flag_Led=1;                                      
u8 flag_Relay=1;
u8 flag_Buzzer=1;
int hh_H, hh_L, mm_H, mm_L, ss_H, ss_L;             //时 分秒
int   year_H  , year_L  , month_H  , month_L  , day_H  , day_L;  //年  月  日
u16 Device_Add;                       //装置
u16 Band_Rate=9600;                       //波特率
u16 Data_bits=8;
u16 Stop_bits;
int m_state=1;
int flag_debug=1;                     //切换光标 参数
int flagTime=13;
//int flag_debug_second=1;                     //切换光标 参数
u16 start_v=20;
int buzzer_tick_flag;
int fault_num;
extern u16 volatile flash_cache[350];
//extern void flash_write_time_volitage(u8 addr_index,uint16_t volitage,u8 year_H,u8 year_L,u8 month_H,u8 month_L,u8 day_H,u8 day_L,u8 hh_H,u8 hh_L,u8 mm_H,u8 mm_L,u8 ss_H,u8 ss_L ) ;
void operating_12864_system (void)    //按键操作和界面切换
{ 
	 u16 u16_WaitForOscSource,startv=0;
    
	if(rcc_flag==0)
	{
		if(k1==key_ok)
		{
			k1=key_none;
			rcc_flag=1;
			DebugMenu(1);
		}
		if(k1==key_cancel)
		{
			k1=key_none;
			flag_i=1;
			voltage=0;
      Alarm_flag_1=0;
      BUZZER_off;
      Fault_led_off;
      Relay_off;
// 			buzzer_tick_flag=1;
		}
	}
	if(rcc_flag==1)
	{
// 		Delay_ms(1000);
	// 		flag_system=1;
		if(flag_debug==0)
		{
			flag_debug=1;
		}
			switch (k1)
			{
				case key_down:
							k1=key_none;
						 flag_debug++; 
						 if(flag_debug>4)
							{
								flag_debug=1;
							}	
							DebugMenu(flag_debug);
             //	Delay_ms(20);
              						
				break;		
				case key_up:
						 k1=key_none;
						 flag_debug--;
						 if(flag_debug==0)
							{
								flag_debug=4;
							}
							DebugMenu(flag_debug);
           //   Delay_ms(20);
              							
				break;
				case key_cancel:
						 rcc_flag=0;
							flag_debug=1;
				   //  Delay_ms(20);
				     k1=key_none;
							//DebugMenu(flag_debug);
				break;
				case key_ok:
							k1=key_none;
							
							if(flag_debug==1)
							{
								rcc_flag=21;
								flag_debug=1;
								SecondSet(flag_debug);
							}
							if(flag_debug==2)
							{
								rcc_flag=22;
								flag_debug=1;
								SecondSystemDebug(flag_debug);
							}
							if(flag_debug==3)
							{
								rcc_flag=23;
                fault_num=flash_num<0?0:flash_num;
//                 if(flash_num<0)
//                 {
//                   fault_num=0;
//                 }
//                 else{
//                   fault_num=flash_num;
//                 }
								flash_read_time_volitage(start_addr+fault_num*14);
								Second_Fault_record(frecord.voltage,fault_num+1,frecord.year_H,frecord.year_L,frecord.month_H,frecord.month_L,frecord.day_H,frecord.day_L,frecord.hh_H,frecord.hh_L,frecord.mm_H,frecord.mm_L,frecord.ss_H,frecord.ss_L);
//                 flash_read_time_volitage(start_addr+14*fault_num);
//                 Second_Fault_record(frecord.voltage,fault_num+1,frecord.year_H,frecord.year_L,frecord.month_H,frecord.month_L,frecord.day_H,frecord.day_L,frecord.hh_H,frecord.hh_L,frecord.mm_H,frecord.mm_L,frecord.ss_H,frecord.ss_L);
                
							}
							if(flag_debug==4)
							{
								rcc_flag=24;
								Second_clear_fault_records();
							}
				break;
			}
		}
		if(rcc_flag==21)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down:
							k1=key_none;
						 flag_debug++; 
						 if(flag_debug>2)
							{
								flag_debug=1;
							}	
							SecondSet(flag_debug);
             //	Delay_ms(20);
              						
				break;		
				case key_up:
						 k1=key_none;
						 flag_debug--;
						 if(flag_debug==0)
							{
								flag_debug=2;
							}
							SecondSet(flag_debug);
           //   Delay_ms(20);
              							
				break;
				case key_cancel:
						flag_debug=0;
						 rcc_flag=1;
						DebugMenu(1);
				   //  Delay_ms(20);
				     k1=key_none;
							//DebugMenu(flag_debug);
				break;
				case key_ok:
							k1=key_none;
							
					if(flag_debug==1)
					{
							rcc_flag=31;
							ThirdSystemSetting(1);
					}
					if(flag_debug==2)
					{
             flag_debug=1;
							rcc_flag=32;
							ThirdBaudRateSetting(flag_debug,Device_Add,Band_Rate,Data_bits,Stop_bits);
					}
							
				break;
			}	
		}
		if(rcc_flag==22)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down:
							k1=key_none;
						 flag_debug++; 
						 if(flag_debug>4)
							{
								flag_debug=1;
							}	
							SecondSystemDebug(flag_debug);
             //	Delay_ms(20);
              						
				break;		
				case key_up:
						 k1=key_none;
						 flag_debug--;
						 if(flag_debug==0)
							{
								flag_debug=4;
							}
							SecondSystemDebug(flag_debug);
           //   Delay_ms(20);
              							
				break;
				case key_cancel:
						flag_debug=1;
						 rcc_flag=1;
				   //  Delay_ms(20);
				     k1=key_none;
				     DebugMenu(1);
							//DebugMenu(flag_debug);
				break;
				case key_ok:
							k1=key_none;
						if(flag_debug==1)
						{
							flag_debug=1;
							rcc_flag=33;
							ThirdChange();
						}
						if(flag_debug==2)
						{
							flag_debug=1;
							rcc_flag=34;
							ThirdCorrect();
						}
						if(flag_debug==3)
						{
							flag_debug=1;
							rcc_flag=35;
							ThirdCheck();
						}
						if(flag_debug==4)
						{
							flag_debug=1;
							rcc_flag=36;
							ThirdReguration(1,1,1,1);
						}
							
				break;
			}	
		}
		if(rcc_flag==23)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down: 
            fault_num++;
            if(fault_num>=50)
            {
              fault_num=0;
            }
            flash_read_time_volitage(start_addr+14*fault_num);
						Second_Fault_record(frecord.voltage,fault_num+1,frecord.year_H,frecord.year_L,frecord.month_H,frecord.month_L,frecord.day_H,frecord.day_L,frecord.hh_H,frecord.hh_L,frecord.mm_H,frecord.mm_L,frecord.ss_H,frecord.ss_L);
//                 fault_num=flash_num; 
            
            
            k1=key_none;
				break;		
				case key_up:
            fault_num--;
            if(fault_num<0)
            {
              fault_num=49;
            }
					 flash_read_time_volitage(start_addr+14*fault_num);
					 Second_Fault_record(frecord.voltage,fault_num+1,frecord.year_H,frecord.year_L,frecord.month_H,frecord.month_L,frecord.day_H,frecord.day_L,frecord.hh_H,frecord.hh_L,frecord.mm_H,frecord.mm_L,frecord.ss_H,frecord.ss_L);
//                 fault_num=flash_num; 
            
            
            k1=key_none;    							
				break;
				case key_cancel:
						 rcc_flag=1;
				   //  Delay_ms(20);
							flag_debug=1;
				     k1=key_none;
						DebugMenu(1);
							//DebugMenu(flag_debug);
				break;
				case key_ok:
							k1=key_none;
						//	rcc_flag=2;
							
							
				break;
			}	
		}
		if(rcc_flag==24)
		{
			//flag_debug=1;
				switch (k1)
			{
				
				case key_cancel:
						 rcc_flag=1;
							flag_debug=1;
				   //  Delay_ms(20);
				     k1=key_none;
						DebugMenu(1);
							//DebugMenu(flag_debug);
				break;
				case key_ok:
							k1=key_none;
//               fi++;
							rcc_flag=2;
                //flash_cache[300]=0;
//  							for(fi=0;fi<255;fi++)
//               {
//                 flash_cache[fi]=0;
//               }

//               Delay_ms(100);
//               flash_write_time_volitage(0,0,0,0,0,0,0,0,0,0,0,0,0,0 ) ;
//               Delay_ms(500);
//               flash_write(0xffff,flash_num_addr);
							
				break;
			}	
		}
		if(rcc_flag==31)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down:
						 k1=key_none;
						 flag_debug++; 
						 if(flag_debug>2)
							{
								flag_debug=1;
							}	
							ThirdSystemSetting(flag_debug);
             //	Delay_ms(20);
              						
				break;		
				case key_up:
						 k1=key_none;
						 flag_debug--;
						 if(flag_debug==0)
							{
								flag_debug=2;
							}
							ThirdSystemSetting(flag_debug);
           //   Delay_ms(20);
              							
				break;
				case key_cancel:
						 	flag_debug=1;
						 rcc_flag=21;
						 SecondSet(1);
				   //  Delay_ms(20);
				     k1=key_none;

				timer.sec=ss_H*10+ss_L;
				timer.min=mm_H*10+mm_L;
				timer.hour=hh_H*10+hh_L;
				timer.w_date=day_H*10+day_L;
				timer.w_month=month_H*10+month_L;
				timer.w_year=(year_H*10+year_L)+2000;
				
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
        RTC_Set(timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec);
				break;
				case key_ok:
							k1=key_none;
							
					if(flag_debug==1)
					{
						  flag_debug=1;
							rcc_flag=41;
							FourStart(start_v);
					}
					if(flag_debug==2)
					{
						  flag_debug=1;
							rcc_flag=42;
							FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);	
					}
							
				break;
			}	
		}
		if(rcc_flag==32)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down:
						 k1=key_none;
						 flag_debug++; 
						 if(flag_debug>4)
							{
								flag_debug=1;
							}	
							ThirdBaudRateSetting(flag_debug,Device_Add,Band_Rate,Data_bits,Stop_bits);
             //	Delay_ms(20);
              						
				break;		
				case key_up:
						 k1=key_none;
						 flag_debug--;
						 if(flag_debug==0)
							{
								flag_debug=4;
							}
							ThirdBaudRateSetting(flag_debug,Device_Add,Band_Rate,Data_bits,Stop_bits);
           //   Delay_ms(20);
              							
				break;
				case key_cancel:
						 flag_debug=1;
						 rcc_flag=21;
						 SecondSet(1);
				   //  Delay_ms(20);
				     k1=key_none;
							//DebugMenu(flag_debug);
				break;
				case key_ok:
						 k1=key_none;	
					if(flag_debug==1)
					{
// 						  flag_debug=1;
							rcc_flag=43;	
					} 
          else if(flag_debug==2)
					{
// 						  flag_debug=1;
							rcc_flag=44;
					}
           else if(flag_debug==3)
					{
// 						  flag_debug=1;
							rcc_flag=45;
					}
          else if(flag_debug==4)
					{
// 						  flag_debug=1;
							rcc_flag=46;
					}
							
				break;
			}	
		}
		if(rcc_flag==33)
		{
			//flag_debug=1;
				switch (k1)
			{
				
				case key_cancel:
						 	flag_debug=1;
						 rcc_flag=22;
						 SecondSystemDebug(1);
				   //  Delay_ms(20);
				     k1=key_none;
							//DebugMenu(flag_debug);
				break;
				case key_ok:
							
							
				break;
			}	
		}
		if(rcc_flag==34)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down:
						
              						
				break;		
				case key_up:
						
           //   Delay_ms(20);
              							
				break;
				case key_cancel:
						 	flag_debug=1;
						 rcc_flag=22;
						 SecondSystemDebug(1);
				   //  Delay_ms(20);
				     k1=key_none;
							//DebugMenu(flag_debug);
				break;
				case key_ok:
							
							
				break;
			}	
		}
		if(rcc_flag==35)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down:
						
              						
				break;		
				case key_up:
						
           //   Delay_ms(20);
              							
				break;
				case key_cancel:
						 	flag_debug=1;
						 rcc_flag=22;
						 SecondSystemDebug(1);
				   //  Delay_ms(20);
				     k1=key_none;
							//DebugMenu(flag_debug);
				break;
				case key_ok:
							
							
				break;
			}	
		}
		if(rcc_flag==36)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down:
						 k1=key_none;
						 flag_debug++; 
						 if(flag_debug>3)
							{
								flag_debug=1;
							}	
							ThirdReguration(flag_debug,flag_Led,flag_Relay,flag_Buzzer);							
				break;		
				case key_up:
						 k1=key_none;
						 flag_debug--;
						 if(flag_debug==0)
							{
								flag_debug=3;
							}
							ThirdReguration(flag_debug,flag_Led,flag_Relay,flag_Buzzer);								
				break;
				case key_cancel:
						 	flag_debug=1;
						 rcc_flag=22;
						 SecondSystemDebug(1);
				   //  Delay_ms(20);
				     k1=key_none;
							//DebugMenu(flag_debug);
				break;
				case key_ok:
					   k1=key_none;	
						 if(flag_debug==1)              
					  {					  
							++flag_Led;
							if(flag_Led<1)
							{
								flag_Led=2;
              }
							else if(flag_Led>2)
							{
								flag_Led=1;
              }
						ThirdReguration(1,flag_Led,flag_Relay,flag_Buzzer);	
					  }
						if(flag_debug==2)
						{
								++flag_Relay;
							if(flag_Relay<1)
							{
								flag_Relay=2;
              }
							else if(flag_Relay>2)
							{
								flag_Relay=1;
              }
						ThirdReguration(2,flag_Led,flag_Relay,flag_Buzzer);
						}
						if(flag_debug==3)
						{
								++flag_Buzzer;
							if(flag_Buzzer<1)
							{
								flag_Buzzer=2;
              }
							else if(flag_Buzzer>2)
							{
								flag_Buzzer=1;
              }
						ThirdReguration(3,flag_Led,flag_Relay,flag_Buzzer);
						}							
				break;
			}	
		}
    if(rcc_flag==41)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down:
							 
              if(start_v<=0)
							{
								start_v=0;
							}
				      start_v-=5;
							 FourStart(start_v);
             //	Delay_ms(20);
				      k1=key_none;
              						
				break;		
				case key_up:
							start_v+=5;
							 FourStart(start_v);
           //   Delay_ms(20);
				     k1=key_none;
              							
				break;
				case key_cancel:
              startv=start_v;
						 flag_debug=1;
						 rcc_flag=31;
						 ThirdSystemSetting(1);
				     k1=key_none;
            // flash_write_start_v(start_v);
            flash_write(startv,start_v_addr);
				break;
				case key_ok:
							
							
				break;
			}	
		}
		if(rcc_flag==42)
		{
			//flag_debug=1;
				switch (k1)
			{
				case key_down:
              						
				break;		
				case key_up:
						
              							
				break;
				case key_cancel:
						 	flag_debug=1;
						 rcc_flag=31;
						 ThirdSystemSetting(1);
				   //  Delay_ms(20);
				     k1=key_none;
							//DebugMenu(flag_debug);
				break;
				case key_ok:
					   ++flagTime;
				     if(flagTime > 12)
				     {
				      flagTime = 1;
				     }
              k1=key_none;						 
				break;
			}	
		}
		/*设置时间*/
		       if(flagTime==1)
					 {
						 switch (k1)
			      {
				     case key_down:
              	  --ss_L;
					    	if(ss_L<0)
					        {
					        	ss_L = 9;
					    	  }	
                  FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);	
                  k1=key_none;									
				     break;		
				     case key_up:
						      ++ss_L;
					    	if(ss_L>9)
					        {
					        	ss_L = 0;
					    	  }
                  FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
                  k1=key_none;									
				     break;
            }
					}
					 else if(flagTime==2)
					 {
            	switch (k1)
			      {
				     case key_down:
                  --ss_H;
					    	if(ss_H<0)
					        {
					        	ss_H = 6;
					    	  }	
                  FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);	
                  k1=key_none;										
				     break;		
				     case key_up:
						      ++ss_H;
					    	if(ss_H>6)
					        {
					        	ss_H = 0;
					    	  }
									FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              		 k1=key_none;				
				     break;
            }
           }
					 else if(flagTime==3)
					 {
            	switch (k1)
			      {
				     case key_down:
							    --mm_L;
					    	  if(mm_L<0)
					        {
					        	mm_L = 9;
					    	  }
									FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              		k1=key_none;					
				     break;		
				     case key_up:
						      ++mm_L;
					    	  if(mm_L>9)
					        {
					        	mm_L = 0;
					    	  }
									FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;							
				     break;
            }
           }
					 else if(flagTime==4)
					 {
            	switch (k1)
			      {
				     case key_down:
							    --mm_H;
					    	  if(mm_H<0)
					        {
					        	mm_H = 6;
					    	  }
									FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              		k1=key_none;					
				     break;		
				     case key_up:
						      ++mm_H;
					    	  if(mm_H>6)
					        {
					        	mm_H = 0;
					    	  }
									FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;				
				     break;
            }
           }
				   else if(flagTime==5)
					 {
            	switch (k1)
			      {
				     case key_down:
							    --hh_L;
					    	  if(hh_L<0)
					         {
					        	 hh_L = 9;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;			
				     break;		
				     case key_up:
						      ++hh_L;
					    	  if(hh_L>9)
					         {
					        	 hh_L = 0;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;						
				     break;
            }
           }
					 else if(flagTime==6)
					 {
            	switch (k1)
			      {
				     case key_down:
              		--hh_H;
					    	  if(hh_H<0)
					         {
					        	 hh_H = 2;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;			
				     break;		
				     case key_up:
						      ++hh_H;
					    	  if(hh_H>2)
					         {
					        	 hh_H = 0;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;
              							
				     break;
            }
           }
					 else if(flagTime==7)
					 {
            	switch (k1)
			      {
				     case key_down:
              			--day_L;
					    	  if(day_L<0)
					         {
					        	 day_L = 9;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;			
				     break;		
				     case key_up:
						      ++day_L;
					    	  if(day_L>9)
					         {
					        	 day_L = 0;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;
              							
				     break;
            }
           }
					 else if(flagTime==8)
					 {
            	switch (k1)
			      {
				     case key_down:
              		--day_H;
					    	  if(day_H<0)
					         {
					        	 day_H = 3;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;			
				     break;		
				     case key_up:
						      ++day_H;
					    	  if(day_H>3)
					         {
					        	 day_H = 0;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;
              							
				     break;
            }
           }
					 else if(flagTime==9)
					 {
            	switch (k1)
			      {
				     case key_down:
              		--month_L;
					    	  if(month_L<0)
					         {
					        	 month_L = 9;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;				
				     break;		
				     case key_up:
						      ++month_L;
					    	  if(month_L>9)
					         {
					        	 month_L = 0;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;					
				     break;
            }
           }
					 else if(flagTime==10)
					 {
            	switch (k1)
			      {
				     case key_down:
              		--month_H;
					    	  if(month_H<0)
					         {
					        	 month_H = 1;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;				
				     break;		
				     case key_up:
						      ++month_H;
					    	  if(month_H>1)
					         {
					        	 month_H = 0;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;
              							
				     break;
            }
           }
					 
					 else if(flagTime==11)
					 {
            	switch (k1)
			      {
				     case key_down:
							    --year_L;
					    	  if(year_L<0)
					         {
					        	 year_L = 9;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;	
              						
				     break;		
				     case key_up:
						       ++year_L;
					    	  if(year_L>9)
					         {
					        	 year_L = 0;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;
              							
				     break;
            }
           }
					 else if(flagTime==12)
					 {
            	switch (k1)
			      {
				     case key_down:
              		--year_H;
					    	  if(year_H<0)
					         {
					        	 year_H = 9;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;				
				     break;		
				     case key_up:
						       ++year_H;
					    	  if(year_H>9)
					         {
					        	 year_H = 0;
					    	   }
              		FourTime(year_H, year_L, month_H, month_L, day_H, day_L, hh_H, hh_L, mm_H, mm_L, ss_H, ss_L);
              	  k1=key_none;
              							
				     break;
            }
           }
					if(rcc_flag==43)                 //设置装置号
					{
						switch (k1)
			      {
				     case key_down: 
                  --Device_Add;
					    	if(Device_Add<1)
					        {
					        	Device_Add = 255;
					    	  }	
             ThirdBaudRateSetting(1,Device_Add,Band_Rate,Data_bits,Stop_bits);
                  k1=key_none;							
				     break;		
				     case key_up:
						      ++Device_Add;
					    	  if(Device_Add>255)
					         {
					        	Device_Add = 1;
					    	   }	
             ThirdBaudRateSetting(1,Device_Add,Band_Rate,Data_bits,Stop_bits);	
                   k1=key_none;                     
				     break;		 
						case key_cancel:                                //设置装置后返回
									 flag_debug=1;
									 rcc_flag=21;
									 SecondSet(1);
                 //  flash_write_device_b(Device_Add);	
                  flash_write(Device_Add,device_id_addr);
									 k1=key_none;
				    break;
            }
          }
					
					if(rcc_flag==44)                 //设置波特率
					{
						switch (k1)
			      {
				     case key_down: 
						     k1=key_none;
                  --flag_debug;
					    	if(flag_debug<1)
					        {
					        	flag_debug = 4;
					    	  }	
									switch(flag_debug)
									{
										case 1:
											    Band_Rate=4800;
										break;
										case 2:
											    Band_Rate=9600;
										break;
										case 3:
											    Band_Rate=19200;
										break;
										case 4:
											    Band_Rate=115200;
										break;
                  }
             ThirdBaudRateSetting(2,Device_Add,Band_Rate,Data_bits,Stop_bits);									
				     break;		
				     case key_up:
							     k1=key_none;
						      k1=key_none;
                  ++flag_debug;
					    	if(flag_debug>4)
					        {
					        	flag_debug = 0;
					    	  }	
									switch(flag_debug)
									{
										case 1:
											    Band_Rate=4800;
										break;
										case 2:
											    Band_Rate=9600;
										break;
										case 3:
											    Band_Rate=19200;
										break;
										case 4:
											    Band_Rate=115200;
										break;
                  }
             ThirdBaudRateSetting(2,Device_Add,Band_Rate,Data_bits,Stop_bits);								
				     break;		 
						case key_cancel:                                //设置装置后返回
									 flag_debug=1;
									 rcc_flag=21;
									 SecondSet(1);
									 k1=key_none;
                   uart_init(Band_Rate);
                   RS485_Init(Band_Rate);       //波特率设置  485
                  // flash_write_Band_Rate(Band_Rate);
                  flash_write(Band_Rate,Band_Rate_addr);
				    break;
            }
          }
          
          if(rcc_flag==45)                                 //设置数据位
					{
						switch (k1)
			      {
				     case key_down: 
                  --Data_bits;
					    	if(Data_bits<5)
					        {
					        	Data_bits = 8;
					    	  }	
             ThirdBaudRateSetting(3,Device_Add,Band_Rate,Data_bits,Stop_bits);
                  k1=key_none;							
				     break;		
				     case key_up:
						      ++Data_bits;
					    	  if(Data_bits>9)
					         {
					        	Data_bits = 5;
					    	   }	
             ThirdBaudRateSetting(3,Device_Add,Band_Rate,Data_bits,Stop_bits);	
                   k1=key_none;                     
				     break;		 
						case key_cancel:                                //设置停止位
									 flag_debug=1;
									 rcc_flag=21;
									 SecondSet(1);
//                    flash_write_device_b(Device_Add);	
//                  flash_write_databits(Data_bits);
                  flash_write(Data_bits,databits_addr);
									 k1=key_none;
				    break;
            }
          }
          
					if(rcc_flag==46)                                 //设置停止位
					{
						switch (k1)
			      {
				     case key_down: 
                  --Stop_bits;
					    	if(Stop_bits<1)
					        {
					        	Stop_bits = 9;
					    	  }	
             ThirdBaudRateSetting(4,Device_Add,Band_Rate,Data_bits,Stop_bits);
                  k1=key_none;							
				     break;		
				     case key_up:
						      ++Stop_bits;
					    	  if(Stop_bits>10)
					         {
					        	Stop_bits = 1;
					    	   }	
             ThirdBaudRateSetting(4,Device_Add,Band_Rate,Data_bits,Stop_bits);	
                   k1=key_none;                     
				     break;		 
						case key_cancel:                                //设置停止位
									 flag_debug=1;
									 rcc_flag=21;
									 SecondSet(1);
//                   flash_write_stopbits(Stop_bits);
                   flash_write(Stop_bits,stopbits_addr);
									 k1=key_none;
				    break;
            }
          }

}
