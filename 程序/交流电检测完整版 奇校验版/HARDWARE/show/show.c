#include "show.h"
#include "display12864.h"
// #include <stdio.h>
#include "delay.h"
#include "key.h"
#include "stm32f10x_it.h"
#include "RTC.h"
// struct TIME1
// {
// 	u8 year;   
// 	u8 month;
// 	u8 day;
// 	u8 m_day;            //������ʱ�õ��м����
// 	u8 hour;
// 	u8 minute;
// 	u8 second;
// } time;
u8 day_day;
//u32 rtc_reflash_flag;
extern u8 flag_i;
extern int hh_H, hh_L, mm_H, mm_L, ss_H, ss_L;
extern int   year_H  , year_L  , month_H  , month_L  , day_H  , day_L;
extern u16 voltage;
u8 Month_Month(u8 day);

void  Time_Display(uint32_t TimeVar);
//������
void show_time_voltage(u8 flag,u16 v,u8 yy_H,u8 yy_L,u8 uu_H,u8 uu_L,u8 dd_H,u8 dd_L,u8 hh_H,u8 hh_L,u8 mm_H,u8 mm_L,u8 ss_H,u8 ss_L)
{  
	 u8 V_H,V_M,V_L,V_MM;
// 	  char buf[2];	
	  V_H=v/100;
	  V_MM=v%100;
	  V_M=V_MM/10;
	  V_L=V_MM%10;
    if(flag==1)
		{
		WriteCmd(0x82);        //��һ�п���ָ��  ��ʼ��ַΪ0x80
		DisStr("ϵͳ����");    
		WriteCmd(0x92);       
		DisStr("Uac=");
		WriteCmd(0x94);
		WriteData(V_H+48);
		WriteData(V_M+48);
		WriteData(V_L+48);
		DisStr("V");
    }
	else if(flag==0)
		{
			WriteCmd(0x82);        //��һ�п���ָ��  ��ʼ��ַΪ0x80
			DisStr("ϵͳ����");    
			WriteCmd(0x92);       
			DisStr("Uac=");
			WriteCmd(0x94);
			WriteData(V_H+48);
			WriteData(V_M+48);
			WriteData(V_L+48);
// 			DisStr(buf);
			DisStr("V");
			}
	
	WriteCmd(0x8A);
	WriteData(yy_H+48);      //д���ݵ�ʱ����д���ݵ�ascii�� �м�
	WriteData(yy_L+48);
  WriteData(45);
	WriteData(uu_H+48);
	WriteData(uu_L+48);
	WriteData(45);
	WriteData(dd_H+48);
	WriteData(dd_L+48);
	WriteCmd(0x9A);
  WriteData(hh_H+48);
	WriteData(hh_L+48);
	WriteData(58);
	WriteData(mm_H+48);
	WriteData(mm_L+48);
	WriteData(58);
  WriteData(ss_H+48);
	WriteData(ss_L+48);
}
//һ������  ���� symbol---��ͷ
void DebugMenu(int symbol)
{   
     u8 symbol1,symbol2,symbol3,symbol4;
// 	    Delay_ms(1000);
	   switch(symbol)
	   {
		    case 1 : 
				symbol1=62;
				symbol2=32;
				symbol3=32;
				symbol4=32;
				break;
			case 2 : 
				symbol1=32;
				symbol2=62;
				symbol3=32;
				symbol4=32;
				break;
			case 3 : 
				symbol1=32;
				symbol2=32;
				symbol3=62;
				symbol4=32;
				break;
			case 4 : 
				symbol1=32;
				symbol2=32;
				symbol3=32;
				symbol4=62;
				break;
// 			default :
// 			  symbol1=62;
// 				symbol2=32;
// 				symbol3=32;
// 				symbol4=32;
// 				break;

			
	   }
	   clr();	   
     WriteCmd(0x80);
     DisStr("��    ��ֵ�趨");
	   WriteCmd(0x81);
	   WriteData(symbol1);
	 
     WriteCmd(0x90);
	   DisStr("��    ϵͳ����");
	   WriteCmd(0x91);
	   WriteData(symbol2);

	   WriteCmd(0x88);
	   DisStr("��    ���ϼ�¼");
	   WriteCmd(0x89);
	   WriteData(symbol3);

	   WriteCmd(0x98);
	   DisStr("��    �����¼");
	   WriteCmd(0x99);
	   WriteData(symbol4);
}



//�������� ��ֵ�趨  ���� symbol---��ͷ
void SecondSet(u8 symbol)
{
       u8  symbol1,symbol2;

	   if(symbol == 1)
	   {
	       symbol1 = 62;
		   symbol2 = 32;
	   }
	   else
	   { 
	     symbol1 = 32;
		   symbol2 = 62;
	   }
	    clr();	 
     WriteCmd(0x80);
     DisStr("��");
     WriteCmd(0x90);
	   DisStr("ֵ    ϵͳ�趨");
	   WriteCmd(0x91);
	   WriteData(symbol1);
	   WriteCmd(0x88);
	   DisStr("��    ͨѶ��ֵ");
	    WriteCmd(0x89);
	   WriteData(symbol2);
	   WriteCmd(0x98);
	   DisStr("��");
}
//��ֵ�趨�µ���������ϵͳ��ֵ ���� symbol---��ͷ 
void ThirdSystemSetting(u8 symbol)
{
     u8  symbol1,symbol2;

	   if(symbol == 1)
	   {
	       symbol1 = 62;
		   symbol2 = 32;
	   }
	   else
	   { 
	     symbol1 = 32;
		   symbol2 = 62;
	   }
	  clr();
	  WriteCmd(0x80);
    DisStr("ϵ");
    WriteCmd(0x90);
	  DisStr("ͳ  ������ѹ");
	  WriteCmd(0x91);
	  WriteData(symbol1);
	  WriteCmd(0x88);
	  DisStr("��  ʱ���趨");
	  WriteCmd(0x89);
	  WriteData(symbol2);
	  WriteCmd(0x98);
	  DisStr("ֵ");
}
//��ֵ�趨�µ�
//�������沨��������   ���� symbol---��ͷ DeviceAdd---װ��  BandRate---������
void ThirdBaudRateSetting(u8 symbol,u8 DeviceAdd, u32 BandRate,u8 Databits,u8 Stopbits)
{   
	  u8 DeviceAdd_H=0;
	  u8 DeviceAdd_M=0;
	  u8 DeviceAdd_MM=0;                //���м䴦��ı���
	  u8 DeviceAdd_L=0;
    u8 Stopbits_H=0;
    u8 Stopbits_L=0;
    u8 Databits_H=0;
    u8 Databits_L=0;
   
	  u8  symbol1,symbol2,symbol3,symbol4;
    u32 bandrate1,bandrate2,bandrate3,bandrate4,bandrate5=32,bandrate6=32;
	  DeviceAdd_H=DeviceAdd/100;
	  DeviceAdd_MM=DeviceAdd%100;
	  DeviceAdd_M=DeviceAdd_MM/10;
	  DeviceAdd_L=DeviceAdd_MM%10;
    Stopbits_H=Stopbits/10;
    Stopbits_L=Stopbits%10;
    Databits_H=Databits/10;
    Databits_L=Databits%10;

      if(symbol == 1)
	  {
		 symbol1 = 62;
		 symbol2 = 32;
     symbol3 = 32;
     symbol4 = 32;
	  }
	  else if(symbol==2)
	  {
	   symbol1 = 32;
		 symbol2 = 62;
     symbol3 = 32;
     symbol4 = 32;
	  }
    else if(symbol==3)
	  {
	   symbol1 = 32;
		 symbol2 = 32;
     symbol3 = 62;
     symbol4 = 32;
	  }
     else if(symbol==4)
	  {
	   symbol1 = 32;
		 symbol2 = 32;
     symbol3 = 32;
     symbol4 = 62;
	  }
    

// 	  sprintf(buffer,"%x",DeviceAdd_H);
// 		sprintf(buffer1,"%x",DeviceAdd_L);
// 	  sprintf(buffer2,"%x",DeviceAdd_L);
      switch(BandRate)
	  {
	       case 9600 :
		        bandrate1 = 57;//9
		        bandrate2 = 54;//6
		        bandrate3 = 48;//0
		        bandrate4 = 48;//0
		        break;
	      case 2400 :
	          bandrate1 = 50;//2
		        bandrate2 = 52;//4
		        bandrate3 = 48;//0
		        bandrate4 = 48;//0
		        break;
	      case 4800 :
	            bandrate1 = 52;//4
		        bandrate2 = 56;//8
		        bandrate3 = 48;//0
		        bandrate4 = 48;//0
		        break;
		  case 19200 :
	            bandrate1 = 49;//1
		        bandrate2 = 57;//9
		        bandrate3 = 50;//2
		        bandrate4 = 48;//0
				bandrate5 = 48;//0
		        break;
		  case 38400 :
	            bandrate1 = 51;//3
		        bandrate2 = 56;//8
		        bandrate3 = 52;//4
		        bandrate4 = 48;//0
				bandrate5 = 48;//0
		        break;
		  case 57600 :
	          bandrate1 = 53;//5
		        bandrate2 = 55;//7
		        bandrate3 = 54;//6
		        bandrate4 = 48;//0
				bandrate5 = 48;//0
		        break;
		  case 115200 :
	            bandrate1 = 49;//1
		        bandrate2 = 49;//1
		        bandrate3 = 53;//5
		        bandrate4 = 50;//2
				bandrate5 = 48;//0
				bandrate6 = 48;//0
		        break;
		}
	  clr();
    WriteCmd(0x81);
	  DisStr("װ�õ�ַ");
    WriteCmd(0x85);
    WriteData(58);
		WriteData(DeviceAdd_H+48);
    WriteData(DeviceAdd_M+48);
		WriteData(DeviceAdd_L+48);
// 		DisStr(buffer2);
    WriteCmd(0x80);
	  WriteData(symbol1);
    WriteCmd(0x91);
	  DisStr("������");
	  WriteCmd(0x90);
	  WriteData(symbol2);
	  WriteCmd(0x94);
    WriteData(58);
	  WriteData(bandrate1);
	  WriteData(bandrate2);
	  WriteData(bandrate3);
	  WriteData(bandrate4);
	  WriteData(bandrate5);
	  WriteData(bandrate6);
    WriteCmd(0x88);
    WriteData(symbol3);
    WriteCmd(0x89);
    DisStr("����λ");
    WriteData(58);
    WriteData(Databits_H+48);
    WriteData(Databits_L+48);
    WriteCmd(0x98);
    WriteData(symbol4);
    WriteCmd(0x99);
    DisStr("ֹͣλ");
    WriteData(58);
    WriteData(Stopbits_H+48);
    WriteData(Stopbits_L+48);
    
}

//�ļ�����  ������ѹ  valtage--��ѹֵ
void FourStart(u8 valtage)
{
    u8  v1,v2;
	  v1=valtage/10;
	  v2=valtage%10;
	  clr();
    WriteCmd(0x80);
    DisStr("");
    WriteCmd(0x90);
	  DisStr("������ѹ  V");
	  WriteCmd(0x94);
// 	  DisStr(buffer);
// 	  DisStr(buffer1);
	  WriteData(v1+48);
	  WriteData(v2+48);
	  WriteCmd(0x88);
	  DisStr("");
	  WriteCmd(0x98);
	  DisStr("");
}
////�ļ�����  ʱ���趨 ����  yy��  uu��  dd��  hhʱ  mm��  ss��           H��λ L��λ
void FourTime(u8 yy_H, u8 yy_L, u8 uu_H, u8 uu_L, u8 dd_H, u8 dd_L, u8 hh_H, u8 hh_L, u8 mm_H, u8 mm_L, u8 ss_H, u8 ss_L)
{    
	   clr();
     WriteCmd(0x80);
     DisStr("  ");
     WriteCmd(0x90);
	   DisStr("    ʱ���趨   ");
     WriteCmd(0x88);
	   DisStr("  ");
	   WriteData(yy_H+48);
	   WriteData(yy_L+48);
	   DisStr("��");
	   WriteData(uu_H+48);
	   WriteData(uu_L+48);
	   DisStr("��");
	   WriteData(dd_H+48);
	   WriteData(dd_L+48);
	   DisStr("��");
	   DisStr("    ");
	   WriteCmd(0x98);
	   DisStr("  ");
	   WriteData(hh_H+48);
	   WriteData(hh_L+48);
	   DisStr("ʱ");
	   WriteData(mm_H+48);
	   WriteData(mm_L+48);
	   DisStr("��");
	   WriteData(ss_H+48);
	   WriteData(ss_L+48);
	   DisStr("��");
	   DisStr(" ");
}

//�������� ϵͳ����  ���� symbol---��ͷ
void SecondSystemDebug(u8 symbol)
{
       u8 symbol1,symbol2,symbol3,symbol4;
	   switch(symbol)
	   {
		    case 1 : 
				symbol1=62;
				symbol2=32;
				symbol3=32;
				symbol4=32;
				break;
			case 2 : 
				symbol1=32;
				symbol2=62;
				symbol3=32;
				symbol4=32;
				break;
			case 3 : 
				symbol1=32;
				symbol2=32;
				symbol3=62;
				symbol4=32;
				break;
			case 4 : 
				symbol1=32;
				symbol2=32;
				symbol3=32;
				symbol4=62;
				break;
			default :
			    symbol1=62;
				symbol2=32;
				symbol3=32;
				symbol4=32;
				break;
       }
     clr();
	   WriteCmd(0x80);
     DisStr("ϵ    װ�õ���");
	   WriteCmd(0x81);
	   WriteData(symbol1);

     WriteCmd(0x90);
	   DisStr("ͳ    װ��У׼");
	   WriteCmd(0x91);
	   WriteData(symbol2);

	   WriteCmd(0x88);
	   DisStr("��    ͨѶ�Լ�");
	   WriteCmd(0x89);
	   WriteData(symbol3);

	   WriteCmd(0x98);
	   DisStr("��    ���ڼ��");
	   WriteCmd(0x99);
	   WriteData(symbol4);
}
//ϵͳ�����µ���������  ����ȷ��������  �������ء��˳�
void ThirdChange(void)
{
	  clr();
	  WriteCmd(0x80);
    DisStr("");
    WriteCmd(0x90);
	  DisStr("����ȷ��������");
	  WriteCmd(0x88);
	  DisStr("�������ء��˳�");
    WriteCmd(0x98);
	  DisStr("");
}
//ϵͳ�����µ���������  У׼ֵ: 100V  ����ȷ����У׼  �������ء��˳�
void ThirdCorrect(void)
{
    clr();
	  WriteCmd(0x80);
    DisStr("");
    WriteCmd(0x90);
	  DisStr("У׼ֵ: 100V");
	  WriteCmd(0x88);
	  DisStr("����ȷ����У׼");
	  WriteCmd(0x98);
	  DisStr("�������ء��˳�");
}

//ϵͳ�����µ���������   ͨѶ�Լ�  
void ThirdCheck(void)
{
	  clr();
	  WriteCmd(0x80);
    DisStr("");
    WriteCmd(0x90);
	  DisStr("");
	  WriteCmd(0x88);
	  DisStr("ͨѶ�Լ�  OK");
	  WriteCmd(0x98);
	  DisStr("");
}
//ϵͳ�����µ���������  ָʾ��״̬    �̵���״̬   ������״̬
void ThirdReguration(u8 symbol,u8 flagLed,u8 flagRelay,u8 flagBuzzer)
{
     u8 symbol1,symbol2,symbol3;
       switch(symbol)
	   {
		    case 1 : 
				symbol1=62;
				symbol2=32;
				symbol3=32;
				break;
			case 2 : 
				symbol1=32;
				symbol2=62;
				symbol3=32;
				break;
			case 3 : 
				symbol1=32;
				symbol2=32;
				symbol3=62;
				break;
			default :
			    symbol1=62;
				symbol2=32;
				symbol3=32;
				break;
      }
	  clr();
	  if(flagLed==1)
	  {
	      WriteCmd(0x80);
	      DisStr("ָʾ��  ��");
	      WriteCmd(0x83);
	      WriteData(symbol1);
			  Fault_led_on;
	  }   
	  else if(flagLed==2)
	  {
	      WriteCmd(0x80);
	      DisStr("ָʾ��  ��");
	      WriteCmd(0x83);
	      WriteData(symbol1);
        Fault_led_off;			
	  }
	  if(flagRelay==1)
	  {
        WriteCmd(0x90);
	      DisStr("�̵���  ��");
	      WriteCmd(0x93);
	      WriteData(symbol2);
			  Relay_on;
	  }
	  else  if(flagRelay==2)
	  {
		    WriteCmd(0x90);
	      DisStr("�̵���  ��");
	      WriteCmd(0x93);
	      WriteData(symbol2);
			  Relay_off;
	  }
	  if(flagBuzzer==1)
	  {
	      WriteCmd(0x88);
	      DisStr("������  ��");
	      WriteCmd(0x8B);
	      WriteData(symbol3);
			  BUZZER_on;
	  }
	  else if(flagBuzzer==2)
	  {
	      WriteCmd(0x88);
	      DisStr("������  ��");
	      WriteCmd(0x8B);
	      WriteData(symbol3);
				BUZZER_off;
	  }
	  WriteCmd(0x98);
	  DisStr("");
}
//��������  ���ϼ�¼  v����������ѹֵ    num�����������
void Second_Fault_record(u16 v,u8 num,u8 yy_H,u8 yy_L,u8 uu_H,u8 uu_L,u8 dd_H,u8 dd_L,u8 hh_H,u8 hh_L,u8 mm_H,u8 mm_L,u8 ss_H,u8 ss_L)
{ u8 num_H,num_L;
// 	char buf[2];
	u8 V_H,V_M,V_MM,V_L;
	num_H=num/10;
	num_L=num%10;

	V_H=v/100;
	V_MM=v%100;
	V_M=V_MM/10;
	V_L=V_MM%10;
// 	sprintf(buf,"%d",v);
	clr();
	WriteCmd(0x80);
	DisStr("���");
	WriteCmd(0x82);
	WriteData(58);      //58ð�ŵ�ascii��
	WriteData(num_H+48);
	WriteData(num_L+48);
	WriteCmd(0x84);
	WriteData(yy_H+48);      //д���ݵ�ʱ����д���ݵ�ascii�� �м�
	WriteData(yy_L+48);
  WriteData(45);
	WriteData(uu_H+48);
	WriteData(uu_L+48);
	WriteData(45);
	WriteData(dd_H+48);
	WriteData(dd_L+48);
	WriteCmd(0x94);
  WriteData(hh_H+48);
	WriteData(hh_L+48);
	WriteData(45);
	WriteData(mm_H+48);
	WriteData(mm_L+48);
	WriteData(45);
  WriteData(ss_H+48);
	WriteData(ss_L+48);
	WriteCmd(0x89);
	DisStr("Uac =");
	WriteCmd(0x8c);
	WriteData(V_H+48);
	WriteData(V_M+48);
	WriteData(V_L+48);
	DisStr("V");	
}

//��������  ������ϼ�¼
void Second_clear_fault_records(void)
{
  clr();
	WriteCmd(0x81);
	DisStr("����ȷ��������");
	WriteCmd(0x90);
	DisStr("��¼�������");
	WriteCmd(0x89);
	DisStr("�������ء��˳�");
}
void Update_rtc_time(void)
{
	//u32 rtc_temp=0;
	//Delay_ms(900);
	//rtc_temp=RTC_GetCounter();
//	if(rtc_reflash_flag!=rtc_temp)
	//{
	//	rtc_reflash_flag=rtc_temp;
	
		if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
		{
			
      
				//RTC_Configuration();
			 RTC_Configuration();         //rtc��ʼ��
			//Time_Adjust();
			BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
		}
		else
		{
			RTC_WaitForSynchro();
			
			RTC_ITConfig(RTC_IT_SEC, ENABLE);
			
			RTC_WaitForLastTask();
		}
// 			RCC_ClearFlag();
//      RTC_Configuration();
		Time_Display(RTC_GetCounter());
      
    
    
	//}
}
// Time_Display(RTC_GetCounter());
void  Time_Display(uint32_t TimeVar)
{
// 	u32  year=14, month,day,hh,mm,ss;
// // 	u32 THH_H = 0, THH_L=0, TMM_H=0, TMM_L=0, TSS_H=0, TSS_L=0;
// //   u32 YEAR=14;
// 	hh = TimeVar / 3600 % 24;
  RTC_Get();
 	hh_H = timer.hour / 10;
 	hh_L = timer.hour % 10;

// 	mm  = (TimeVar % 3600) / 60;
 	mm_H = timer.min / 10;
   mm_L = timer.min % 10;

// 	ss  =  (TimeVar % 3600) % 60;
 	ss_H = timer.sec / 10;
 	ss_L = timer.sec % 10;
//   
// 	day = TimeVar / 86400 +1;
// 	
// 	month = Month_Month(day);
   day_H = timer.w_date / 10;
 	day_L = timer.w_date % 10;
 	month_H = timer.w_month/10;
 	month_L = timer.w_month%10;
// 	if(month > 12)
// 	{
// 	   month = 1;
// 	   year ++;
// 	}
 	year_H = (timer.w_year-2000)/10;	    //Ӧ���Ǽ�2000	
 	year_L = (timer.w_year-2000)%10;
  
  
	show_time_voltage(flag_i,voltage,year_H,year_L,month_H,month_L,day_H,day_L,hh_H,hh_L,mm_H,mm_L,ss_H,ss_L);
}

u8 Month_Month(u8 day)
{
    u8 mon;
	if(day>=1&&day<32)
	{
	    mon = 1;
			day_day=day;
	}
	else if(day>=32&&day<61)
	{
	    mon = 2;
			day_day=day-31;
	}
	else if(day>=61&&day<92)
	{
	    mon = 3;
			day_day=day-60;
	}
	else if(day>=92&&day<122)
	{
	    mon = 4;
			day_day=day-91;
	}
	else if(day>=122&&day<153)
	{
	    mon = 5;
			day_day=day-121;
	}
	else if(day>=153&&day<183)
	{
	    mon = 6;
			day_day=day-152;
	}
	else if(day>=183&&day<214)
	{
	    mon = 7;
			day_day=day-182;
	}
	else if(day>=214&&day<245)
	{
	    mon = 8;
			day_day=day-213;
	}
	else if(day>=245&&day<275)
	{
	    mon = 9;
			day_day=day-244;
	}
	else if(day>=275&&day<306)
	{
	    mon = 10;
			day_day=day-274;
	}
	else if(day>=306&&day<336)
	{
	    mon = 11;
			day_day=day-305;
	}
	else if(day>=336&&day<367)
	{
	    mon = 12;
			day_day=day-335;
	}

		return mon;
	
}

// //������ת��Ϊascii��
// u8 IntChangeChar(int Int)
// {
//      u8 Char;
//      switch(Int)
// 	 {
// 	    case 0 :
// 		     Char = 48;
// 		     break;
// 		case 1 :
// 		     Char = 49;
// 		     break; 
// 		case 2 :
// 		     Char = 50;
// 		     break;
// 		case 3 :
// 		     Char = 51;
// 		     break;  
// 		case 4 :
// 		     Char = 52;
// 		     break;
// 		case 5 :
// 		     Char = 53;
// 		     break;  
// 		case 6 :
// 		     Char = 54;
// 		     break;
// 		case 7 :
// 		     Char = 55;
// 		     break;
// 		case 8 :
// 		     Char = 56;
// 		     break;
// 		case 9 :
// 		     Char = 57;
// 		     break;  
// 		default : 
// 		     Char = 48;
// 			 break; 
// 	  }
// 	  return Char; 
// }


