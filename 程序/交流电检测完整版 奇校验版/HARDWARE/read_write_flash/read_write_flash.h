#ifndef __READ_WRITE_H
#define	__READ_WRITE_H
#define	device_id_addr (u32)0x0801c800  
#define	Band_Rate_addr (u32)0x0801cc00 
#define	start_v_addr (u32)0x0801d000 
#define	databits_addr (u32)0x0801d400
#define	stopbits_addr (u32)0x0801d800
#define	flash_num_addr (u32)0x0801dc00
#define	fault_record_addr (u32)0x0801B800
#include "stm32f10x.h"
void flash_write_num(u32 addr,u16 flag);
int flash_read_flagnum(u32 addr);
//void flash_write_time_volitage(u32 addr,uint16_t volitage,u8 year_H,u8 year_L,u8 month_H,u8 month_L,u8 day_H,u8 day_L,u8 hh_H,u8 hh_L,u8 mm_H,u8 mm_L,u8 ss_H,u8 ss_L );
//struct fault_record f_record;
void flash_write_time_volitage(u8 addr_index,uint16_t volitage,u8 year_H,u8 year_L,u8 month_H,u8 month_L,u8 day_H,u8 day_L,u8 hh_H,u8 hh_L,u8 mm_H,u8 mm_L,u8 ss_H,u8 ss_L );
typedef struct fault_record
{
	uint16_t voltage;
	u8 year_H;
	u8 year_L;
	u8 month_H;
	u8 month_L;
	u8 day_H;
	u8 day_L;
	u8 hh_H;
	u8 hh_L;
	u8 mm_H;
	u8 mm_L;
	u8 ss_H;
	u8 ss_L ;
}f_record;
extern f_record frecord;
void flash_write_Band_Rate(u32 device);        //波特率
void flash_write_start_v( u16 vvv);            //启动电压
void flash_write_device_b(u16  device);
void flash_write_databits(u16 device);          //往flash里写数据位
void flash_write_stopbits(u16 device);          //往flash里写停止位
void flash_write(u16 hword,u32 addr);
#endif
