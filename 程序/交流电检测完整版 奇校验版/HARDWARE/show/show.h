#ifndef __SHOW_H
#define	__SHOW_H
#include "stm32f10x.h"
void ThirdBaudRateSetting(u8 symbol,u8 DeviceAdd, u32 BandRate,u8 Databits,u8 Stopbits);
void show_time_voltage(u8 flag,u16 v,u8 yy_H,u8 yy_L,u8 uu_H,u8 uu_L,u8 dd_H,u8 dd_L,u8 hh_H,u8 hh_L,u8 mm_H,u8 mm_L,u8 ss_H,u8 ss_L);
u8 IntChangeChar(int Int);
void DebugMenu(int symbol);
void SecondSet(u8 symbol);
void ThirdSystemSetting(u8 symbol);
// void ThirdBaudRateSetting(u8 symbol,u8 DeviceAdd, u32 BandRate);
void FourStart(u8 valtage);
void FourTime(u8 yy_H, u8 yy_L, u8 uu_H, u8 uu_L, u8 dd_H, u8 dd_L, u8 hh_H, u8 hh_L, u8 mm_H, u8 mm_L, u8 ss_H, u8 ss_L);
void SecondSystemDebug(u8 symbol);
void ThirdReguration(u8 symbol,u8 flagLed,u8 flagRelay,u8 flagBuzzer);
void Second_Fault_record(u16 v,u8 num,u8 yy_H,u8 yy_L,u8 uu_H,u8 uu_L,u8 dd_H,u8 dd_L,u8 hh_H,u8 hh_L,u8 mm_H,u8 mm_L,u8 ss_H,u8 ss_L);
void  Second_clear_fault_records(void);
void ThirdChange(void);
void ThirdCorrect(void);
void ThirdCheck(void);
void Update_rtc_time(void);
#endif 
