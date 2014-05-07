#include "stm32f10x.h"

void IOInitOut(void);           //把所有端口初始化为输出模式的函数。
void IOInitIn(void);	          //把数据引脚初始化为浮空输入的函数。
void WaitBusy(void);      	 //等待12864的忙状态结束的函数。
void WriteCmd(u8 cmd);    //写命令函数。
void WriteData(u8 data);  //写数据函数。
void InitDis(void);            //初始化 12864 和要用到的 STM 32 的引脚。
void DisStr(u8 *s);    //显示字符串的函数。
void DisInt(long int num);  //显示整型变量的函数,最多显示16位的整数。
void DisFloat(float fnum); //显示有4位小数的浮点数，总位数不超过16位。
void WriteData1(u8 data);  //写数据函数

#define  clr()  WriteCmd(0x01);

/*以下是光标定位函数，第一个参数为行坐标，第二个为列坐标，起始坐标是1行1列。
只能以16个点的宽度为单位移动。*/

void locate16(int row, int col);
void Display_key_init(void);            
void Display_key(void);


