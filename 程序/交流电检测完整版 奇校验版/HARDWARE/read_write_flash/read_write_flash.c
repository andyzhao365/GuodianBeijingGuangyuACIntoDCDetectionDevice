#include "read_write_flash.h"
#include "eeprom.h"
#include "stm32f10x_flash.h"
#include "delay.h"

extern  volatile u32 start_addr;
f_record frecord;
extern u16 volatile flash_cache[350];
void flash_write_device_b(u16  device)           //设备编号？
{
  
//   FLASH_Unlock();
//   /* EEPROM Init */
//   EE_Init();
// 	//FLASH_ErasePage(uint32_t Page_Address)
// 	FLASH_ErasePage(device_id_addr);
//  	FLASH_ProgramHalfWord(device_id_addr,  device);
// 	FLASH_Lock();
}

void flash_write_databits(u16 device)           //往flash里写数据位
{
//   FLASH_Unlock();
//   /* EEPROM Init */
//   EE_Init();
// 	//FLASH_ErasePage(uint32_t Page_Address)
// 	FLASH_ErasePage(0x08000000+117*1024);
//  	FLASH_ProgramHalfWord(0x08000000+117*1024,  device);
// 	FLASH_Lock();
}


void flash_write_stopbits(u16 device)           //往flash里写停止位
{
//   FLASH_Unlock();
//   /* EEPROM Init */
//   EE_Init();
// 	//FLASH_ErasePage(uint32_t Page_Address)
// 	FLASH_ErasePage(0x08000000+118*1024);
//  	FLASH_ProgramHalfWord(0x08000000+118*1024,  device);
// 	FLASH_Lock();
}

void flash_write(u16 hword,u32 addr)           //把波特率写进flash  
{
   FLASH_Unlock();
  /* EEPROM Init */
   EE_Init();
	//FLASH_ErasePage(uint32_t Page_Address)
	FLASH_ErasePage(addr);
 	FLASH_ProgramHalfWord(addr, hword);
	FLASH_Lock();
}
void flash_write_Band_Rate(u32 device)           //把波特率写进flash  
{
//    FLASH_Unlock();
//   /* EEPROM Init */
//    EE_Init();
// 	//FLASH_ErasePage(uint32_t Page_Address)
// 	FLASH_ErasePage(Band_Rate_addr);
//  	FLASH_ProgramHalfWord(Band_Rate_addr, device);
// 	FLASH_Lock();
}
void flash_write_time_volitage(u8 addr_index,uint16_t volitage,u8 year_H,u8 year_L,u8 month_H,u8 month_L,u8 day_H,u8 day_L,u8 hh_H,u8 hh_L,u8 mm_H,u8 mm_L,u8 ss_H,u8 ss_L )           //报警时间
{
  u16 i;
	u16 year,month,day,hh,mm,ss;
	year=year_H*10+year_L;
	month=month_H*10+month_L;
	day=day_H*10+day_L;
	hh=hh_H*10+hh_L;
	mm=mm_H*10+mm_L;
	ss=ss_H*10+ss_L;
  flash_cache[addr_index*7]=volitage;
  flash_cache[addr_index*7+1]=year;
  flash_cache[addr_index*7+2]=month;
  flash_cache[addr_index*7+3]=day;
  flash_cache[addr_index*7+4]=hh;
  flash_cache[addr_index*7+5]=mm;
  flash_cache[addr_index*7+6]=ss;
  FLASH_Unlock();
  /* EEPROM Init */
  EE_Init();
 	FLASH_ErasePage(start_addr);
  for(i=0;i<350;i++)
  {
   
    FLASH_ProgramHalfWord(fault_record_addr+i*2, flash_cache[i]);
  }
  
	FLASH_Lock();
  
 }

void flash_read_time_volitage(uint32_t addr)           //从flash里read时间和电压数据
{
	u16 year=0,month=0,day=0,hh=0,mm=0,ss=0;
	frecord.voltage=(*(uint32_t*)(addr));
  year=(*(uint32_t*)(addr+2));
	month=(*(uint32_t*)(addr+4));
	day=(*(uint32_t*)(addr+6));
	hh=(*(uint32_t*)(addr+8));
	mm=(*(uint32_t*)(addr+10));
	ss=(*(uint32_t*)(addr+12));
	frecord.year_H=year/10;
	frecord.year_L=year%10;
	frecord.month_H=month/10;
	frecord.month_L=month%10;
	frecord.day_H=day/10;
	frecord.day_L=day%10;
	frecord.hh_H=hh/10;
	frecord.hh_L=hh%10;
	frecord.mm_H=mm/10;
	frecord.mm_L=mm%10;
	frecord.ss_H=ss/10;
	frecord.ss_L=ss%10;

 }
void flash_write_num(u32 addr,u16 flag)
{
//    FLASH_Unlock();
//   /* EEPROM Init */
//    EE_Init();
//   FLASH_ErasePage(addr);
// 	FLASH_ProgramHalfWord(addr,  flag);
// 	FLASH_Lock();
 }

int flash_read_flagnum(u32 addr)
{
	int ee_num;
  ee_num=(*(uint32_t*)(addr));
	return ee_num;
 }


void flash_write_start_v( u16 vvv)
{
//   FLASH_Unlock();
//   /* EEPROM Init */
//     EE_Init();
// 	FLASH_ErasePage(start_v_addr);

//   FLASH_ProgramHalfWord(start_v_addr, vvv);

// 	FLASH_Lock();
 }

