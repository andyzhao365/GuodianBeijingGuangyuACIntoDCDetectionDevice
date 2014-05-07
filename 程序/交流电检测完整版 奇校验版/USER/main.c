/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ����3.5.0�汾���Ĺ���ģ�塣         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "RTC.h"
#include "delay.h"
#include "display12864.h"
#include "adc.h"
#include "Time_test.h"
#include <math.h>
#include "key.h"
#include "eeprom.h"
#include "stm32f10x_flash.h"
#include "show.h"
#include "operating.h"
#include "stm32f10x_it.h"
#include "read_write_flash.h"
#include "uart1.h"
#include "rs485.h"
/* 
 * ��������main
 * ����  : ������
 * ����  ����
 * ���  : ��
 */
 void read_flash_cache(uint32_t addr);
u16 volatile flash_cache[350];
extern u16 Band_Rate;
extern u16 start_v;
extern u16 Data_bits;
extern u16 Stop_bits;
extern u8 TimeDisplay;
 u8 rcc_flag=0;                  //�����л���һ������Ĳ���
extern  __IO uint16_t update_time;
extern  enum key
{
	 key_up,key_down,key_ok,key_cancel,key_none
		 
}k1;
u8 Alarm_flag_0;    //����дflash
u8 Alarm_flag_1; //��������io
u8 flag_i=1;
u16 voltage=0;
int flash_num;                   //����дflash�Ĵ���

extern int hh_H, hh_L, mm_H, mm_L, ss_H, ss_L;
extern int   year_H  , year_L  , month_H  , month_L  , day_H  , day_L;
double bj0,bj1;               //����adc�����ս��
extern int time_s;            //time2��ʱ�� �õı�־λ
extern int time_data;         //time2��ʱ�� �õı�־λ
extern u8 rccflag;
extern int16_t ad_data[2][20];   //�ɼ�adc���ݵĶ�ά����
void get_adc_real(void);         //��ȡadc��ֵ������ĺ���
void time_and_voltage(void);     //
extern float start_voltage0;     //��ȡ��adc�ĳ�ʼֵ ͨ��0
extern float start_voltage1;     //��ȡ��adc�ĳ�ʼֵ ͨ��0
extern int buzzer_tick_flag;     //�������ı�־λ
//void flash_write(u32 addr,u32 data); 
u32 flash_read(u32 addr);
u32 flash[2];    
u32 ee_data[8];
// u32 const start_addr=(0x0800fc00);
u32  volatile start_addr=0x0801B800;
u32  volatile start_addr1=0x0801BC00;
// u32 const start_addr1=(0x0800FEBC);
extern u16 Device_Add; 
// ee_data[0]=(*(uint32_t*)(EEPROM_START_ADDRESS)); 

int main(void)
{
  u16 fi=0;
  //start_addr=(0x0801B800);
  //start_addr1=(0x0801BC00);
  u16 flash_num_temp=0;                   //����дflash�Ĵ���
	delay_init(72);              //��ʱ������ʼ��
	InitDis();                   //12864��ʼ��
	TIM2_NVIC_Configuration();   //TIME2
	TIM2_Configuration();        //TIME2��ʼ��
	ADC1_GPIO();                 //��ʼ��adc
	ADC1_RCC();                  //��ʼ��adc
	DMA1_Config();               //��ʼ��dma          
	ADC1_Config();               //adcͨ��ģʽ����
	EXTI_PB4_Config();           //�ⲿ�жϳ�ʼ��pb4
	EXTI_PB5_Config();           //�ⲿ�жϳ�ʼ��pb5
	EXTI_PB6_Config();           //�ⲿ�жϳ�ʼ��pb6
	EXTI_PB12_Config();          //�ⲿ�жϳ�ʼ��pb7
	NVIC_Configuration();        //�ж����ȼ�����
  
  Band_Rate=(*(uint16_t*)(Band_Rate_addr));  //��flash���������
  if(Band_Rate==0xffff)                       //���ûд��flash�︳ֵ9600
  {
    Band_Rate=9600;
  }
  //Band_Rate=9600;
  RS485_Init(Band_Rate);       //����������  485
  uart_init(Band_Rate);        //����������  232
	Delay_ms(1000);              //�ȴ�12864��Ӧ
  start_v=(*(uint16_t*)(start_v_addr));   //������ѹ ��flash
  if(start_v==0xffff)
  {
    start_v=20;
  }
  Delay_ms(200);
  Data_bits=(*(uint16_t*)(databits_addr));    //����λ ��flash
  if(Data_bits==0xffff)
  {
    Data_bits=8;
  }
  Delay_ms(200);
  Stop_bits=(*(uint16_t*)(stopbits_addr));  //ֹͣλ  ��flash
  if(Stop_bits==0xffff)
  {
    Stop_bits=1;
  }
	start_adc();                 //��ȡadc�ĳ�ʼֵ
 	RELAY_LED_Init();            //�̵�����led��ʼ��
	BUZZER_Init();
	flash_num_temp=(*(uint16_t*)(flash_num_addr));
	if(flash_num_temp>0xfffe)
	{
		flash_num=(int)(-1);
  }
  else
  {
    flash_num=(int)(flash_num_temp);
  }
  //read_flash_cache(0x0801B800);
  Device_Add=(*(uint16_t*)(device_id_addr));
  START_TIME;         //������ʱ��2
//   Device_Add=(*(uint32_t*)(0x0800B220));
	while(1)
	{
		  Delay_ms(1000);

			time_and_voltage();

		operating_12864_system ();
    if(rcc_flag==2)
    {
       	for(fi=0;fi<350;fi++)
         {
            flash_cache[fi]=0;
         }
      rcc_flag=1;
      Delay_ms(100);
      flash_write_time_volitage(0,0,0,0,0,0,0,0,0,0,0,0,0,0 ) ;
      Delay_ms(500);
      flash_write(0xffff,flash_num_addr);
    }
	}

}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/


void get_adc_real(void)     //��ȡadc���ݴ�����ֵ
{
  int i;
	double temp0=0;
	double temp1=0;
	
	  
	
			for(i=0;i<20;i++)
			{
			 temp0+=pow((double)(((ad_data[0][i]*3.3)/4096)-start_voltage0),2);
// 			temp0+=pow((double)(((ad_data[0][i]*3.3)/4096)-1.54043),2);
			}
			
			for(i=0;i<20;i++)
			{
			 temp1+=pow((double)(((ad_data[1][i]*3.3)/4096)-start_voltage1),2);
// 				 temp1+=pow((double)(((ad_data[1][i]*3.3)/4096)-1.54043),2);
			}
			bj0=sqrt(temp0/20)*311/0.8;       //������ʾ������ѹ��ֵ ͨ��1
			bj1=sqrt(temp1/20)*311/0.8;       //������ʾ������ѹ��ֵ ͨ��2
			temp0=0;
			temp1=0;
			time_s=0;
			time_data=0;
  //  }
}
//

//��ʾ������ ʱ��͵�ѹ
void time_and_voltage(void)
{

// 	if(bj0-(start_voltage0*311/0.85)>110)
			if(bj0>start_v||bj1>start_v)
			{
				flag_i=0;//ϵͳ����
				Alarm_flag_1=1;//����led buzzer Relay ʹ��
				if(bj0>start_v)
					{
					  voltage=(u16)bj0;
				  }
				if(bj1>start_v)
					{
				  	voltage=(u16)bj1;
				 }
				if(Alarm_flag_0==0)
				{
					Alarm_flag_0=1;
					if(flash_num==50)
					{
						flash_num=0;
					}
				
					flash_num++;
					
  				flash_write_time_volitage(flash_num,voltage,year_H,year_L,month_H,month_L,day_H,day_L,hh_H,hh_L,mm_H,mm_L,ss_H,ss_L );
				
					//flash_write_num(start_addr1,(u16)flash_num);
          flash_write((u16)(flash_num&0xffff),flash_num_addr);
			 }
			}
			else 
				{
					Alarm_flag_0=0;
				}
			if(Alarm_flag_1>0)
			{
					buzzer_tick();
					Fault_led_on;
					Relay_on ;
			}
			
			if(rcc_flag == 0)
			{
//        if(update_time>=1000)
// 	     {
				clr();
				Update_rtc_time(); 
// 	      update_time=0;
//        }        
			}


}

void read_flash_cache(uint32_t addr)
{
  int i;
  for(i=0;i<512;i++)
    {
      flash_cache[i]=(*(uint16_t*)(addr+i*2));
    }
}
// // flash  д
// void flash_write(u32 addr,u32 data)
// {
//  FLASH_Unlock();

//   /* EEPROM Init */
//   EE_Init();
// // 	FLASH_ErasePage(addr);
//   FLASH_ProgramWord(addr, data);
// // 	FLASH_ProgramWord(0x0800D800+0x04, flash[1]);
// 	FLASH_Lock();
//  }
// // flash  �� 
// u32 flash_read(u32 addr)
// {
// 		ee_data[0]=(*(uint32_t*)(addr));
// // 		ee_data[1]=(*(uint32_t*)(0x0800D800+0x04));
// // 	return (*(uint32_t*)(addr));
// 	return ee_data[0];
// }

