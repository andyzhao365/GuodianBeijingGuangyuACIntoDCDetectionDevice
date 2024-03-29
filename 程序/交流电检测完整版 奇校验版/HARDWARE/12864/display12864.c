#include "stm32f10x.h"
#include "delay.h"    //必须配合delay.c和delay.h文件使用，所以要包含delay.h。
#include "display12864.h"

// extern u8 Lcd_flag[12];
#define u16 unsigned short   //为了可移植性好，对这两个 STM 32 已经定义过的变量，再定义一次。
#define u8 unsigned char

/********** 以下是相关引脚定义。**************/

#define DisIOB GPIOB                 //定义12864要使用的I/O端口。
#define DisIOA GPIOC                 //定义12864要使用的I/O端口�
#define DisClkA RCC_APB2Periph_GPIOC  //定义12864要使用的I/O端口的时钟。
#define DisClkB RCC_APB2Periph_GPIOB 
#define Data GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
                                       //定义12864使用的数据引脚。
#define RS GPIO_Pin_8
#define RW GPIO_Pin_9
#define EN GPIO_Pin_10       //定义使能端使用的引脚。
#define PSB GPIO_Pin_11      //定义并，串行数据模式的选择端口使用的引脚。

/*********************************************/
                					    
GPIO_InitTypeDef  GPIOStru;  //定义用于定义所以引脚为输出的变量。

// void IOInitOut(void)           //把所有端口初始化为推挽输出模式的函数。
// {
//     GPIOStru.GPIO_Mode = GPIO_Mode_Out_PP;//定义所有的引脚为推挽输出的变量初始化。
//     GPIOStru.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIOStru.GPIO_Pin = Data|RS|RW|EN|PSB;

//     RCC_APB2PeriphClockCmd(DisClkA,ENABLE);
// 	  RCC_APB2PeriphClockCmd(DisClkB,ENABLE);
//     GPIO_Init(DisIOA,&GPIOStru);
// 		GPIO_Init(DisIOB,&GPIOStru);
// }
void IOInitOut(void) 
{
	GPIO_InitTypeDef GPIOStru;
	RCC_APB2PeriphClockCmd(DisClkA,ENABLE);
	GPIOStru.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIOStru.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOStru.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DisIOA, &GPIOStru);    
	RCC_APB2PeriphClockCmd(DisClkB,ENABLE);
	GPIOStru.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIOStru.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOStru.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DisIOB, &GPIOStru);
}

void IOInitIn(void)           											//把数据引脚初始化为浮空输入的函数。
{
    GPIOStru.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//定义数据引脚为浮空输入的变量初始化。
    GPIOStru.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOStru.GPIO_Pin = Data;

    RCC_APB2PeriphClockCmd(DisClkA,ENABLE);   			//把所有端口初始化为输出模式的函数。   
    GPIO_Init(DisIOA,&GPIOStru);	
}

void WaitBusy(void)          //等待12864的忙状态结束的函数。
{
    IOInitIn();        //把数据引脚定义为浮空输入；
    GPIO_ResetBits(DisIOB,RS);  //RS = 0.
    GPIO_SetBits(DisIOB,RW);    //RW = 1.
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    while(GPIO_ReadInputData(DisIOA) & 0x0080); //只要位7的值，位7是忙标志位。
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    IOInitOut();      //把所有引脚定义为输出。
}

void WriteCmd(u8 cmd)    //写命令函数。
{
  	WaitBusy(); 
    GPIO_ResetBits(DisIOB,RS);  //RS = 0.
    GPIO_ResetBits(DisIOB,RW);  //RW = 0. 
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    DisIOA->ODR=((DisIOA->ODR & 0xff00)|cmd); //此处，只有直接操作寄存器才能
                  //达到，只改变输出数据寄存器ODR的低8位，其它位
                  //不变的目的。因为，只有低8位是数据引脚，
                  //其它位可能是控制引脚，不能改变。
    Delay_us(100);
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    Delay_us(100); 	
}

void WriteData(u8 data)  //写数据函数。
{
	  WaitBusy(); 
    GPIO_SetBits(DisIOB,RS);  //RS = 1. 
    GPIO_ResetBits(DisIOB,RW);  //RW = 0. 
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    DisIOA->ODR=((DisIOA->ODR & 0xff00)|data);  //同上。
    Delay_us(100);
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    Delay_us(100); 
}


void WriteData1(u8 data)  //写数据函数。
{
	  WaitBusy(); 
    GPIO_SetBits(DisIOB,RS);  //RS = 1. 
    GPIO_ResetBits(DisIOB,RW);  //RW = 0. 
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    DisIOA->ODR=((DisIOA->ODR & 0xff00)|data);  //同上。
    Delay_us(400);
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    Delay_us(400); 
}
void InitDis(void)   //初始化 12864 和要用到的 STM 32 的引脚。
{
    IOInitOut(); 	
    delay_init(72);   //初始化延时函数的微妙计数基数。  
    GPIO_SetBits(DisIOB,PSB);  //令PSB=1，设置为并行数据模式。
    Delay_us(400);
    WriteCmd(0x30);  //选择基本指令集，和，8位数据模式。
    Delay_us(400);
    WriteCmd(0x0c);  //开显示,无游标,不反白.
    Delay_us(400); 
    WriteCmd(0x01);  //清除显示，并将 DDRAM 的地址计数器 AC 设为 00H.
    Delay_us(400);
    WriteCmd(0x06);  //设置，外部读写数据后,地址记数器 AC 会自动加 1。
    Delay_us(400);
    WriteCmd(0x80);  //将 DDRAM 地址计数器 AC 设为 0.
    Delay_us(400);	
}



void DisStr(u8 *s)    //显示字符串的函数。
{
    while(*s != '\0')
    {
        WriteData(*s);
        s++;
        Delay_ms(1);
    }
}

void DisInt(long int num)  //显示整型变量的函数,最多显示16位的整数。只能显示正数。
{
	u8 temp[17];
	u8 str[17];
	int i=0,j=0;
	while(num != 0)	  //这里不能用num%10 != 0，如果num是10的整数倍，
	                  //例如，100，这样就会出错，根本就不能进入循环体。
	{
		temp[i] = (num%10)+0x30;
		num/=10;
		i++;
	}
	i--;           //因为i在退出循环之前还自加了一次，此时，
	                //指向最后一个存储有用值的元素的后一个位置。
	while(i != -1)	 //因为i=0时，temp[0]还是有用值。
	{
		str[j] = temp[i];
		j++;
		i--;	
	}
	str[j]='\0';  //因为i在退出循环之前还自加了一次，此时，
	              //指向最后一个存储有用值的元素的后一个位置。
	DisStr(str);	
}

void DisFloat(float fnum) //显示有4位小数的浮点数，总位数不超过16位。
{
	long int num = fnum*10000;
	u8 temp[17];
	u8 str[17];
	int i=0,j=0;
	while(num != 0)	                  
	{
		temp[i] = (num%10)+0x30;
		num/=10;
		i++;
		if(i == 4)	 //4位小数处理完后，加入小数点。
		{
			temp[i] = '.';
			i++;
		}
	}
	i--;   
	while(i != -1)	
	{
		str[j] = temp[i];
		j++;
		i--;	
	}
	str[j]='\0';  
	DisStr(str);			
}

/*以下是光标定位函数，第一个参数为行坐标，第二个为列坐标，起始坐标是1行1列。
只能以16个点的宽度为单位移动。*/

void locate16(int row, int col) 
{
	switch(row)
	{
		case 1: WriteCmd(0x80+col-1); break;
		case 2: WriteCmd(0x90+col-1); break;
		case 3: WriteCmd(0x88+col-1); break;
		case 4: WriteCmd(0x98+col-1); break;
	}
}

/*光标定位函数定义结束。*/
// void Display_key_init(void)
// {
// 	Lcd_flag[0]=5;
// 	Lcd_flag[1]=5;
// 	Lcd_flag[2]=5;
// 	Lcd_flag[3]=5;
// 	Lcd_flag[4]=5;
// 	Lcd_flag[5]=5;
// 	Lcd_flag[6]=5;
// 	Lcd_flag[7]=5;
// 	Lcd_flag[8]=5;
// 	Lcd_flag[9]=5;
// 	Lcd_flag[10]=5;
// 	Lcd_flag[11]=5;
// }

// void Display_key(void)
// {
// 	  WriteCmd(0x81);
// 		DisStr("按键");
// 		DisStr("01");
// 		WriteCmd(0x84);
// 		DisStr(":");
// 		WriteCmd(0x85);
// 		switch (Lcd_flag[0])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;
// 				     
// 		}
// 		WriteCmd(0x91);
// 		DisStr("按键");
// 		DisStr("02");
// 		WriteCmd(0x94);
// 		DisStr(":");
// 		WriteCmd(0x95);
// 		switch (Lcd_flag[1])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x89);
// 		DisStr("按键");
// 		DisStr("03");
// 		WriteCmd(0x8c);
// 		DisStr(":");
// 		WriteCmd(0x8D);
// 		switch (Lcd_flag[2])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x99);
// 		DisStr("按键");
// 		DisStr("04");
// 		WriteCmd(0x9c);
// 		DisStr(":");
// 		WriteCmd(0x9D);
// 		switch (Lcd_flag[3])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		
// 		delay_ms(1000);
// 		delay_ms(1000);
// // 		delay_ms(1000);
// 		clr();
// 		
// 		WriteCmd(0x81);
// 		DisStr("按键");
// 		DisStr("05");
// 		WriteCmd(0x84);
// 		DisStr(":");
// 		WriteCmd(0x85);
// 		switch (Lcd_flag[4])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x91);
// 		DisStr("按键");
// 		DisStr("06");
// 		WriteCmd(0x94);
// 		DisStr(":");
// 		WriteCmd(0x95);
// 		switch (Lcd_flag[5])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x89);
// 		DisStr("按键");
// 		DisStr("07");
// 		WriteCmd(0x8c);
// 		DisStr(":");
// 		WriteCmd(0x8D);
// 		switch (Lcd_flag[6])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x99);
// 		DisStr("按键");
// 		DisStr("08");
// 		WriteCmd(0x9c);
// 		DisStr(":");
// 		WriteCmd(0x9D);
// 		switch (Lcd_flag[7])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		delay_ms(1000);
// 		delay_ms(1000);
// // 		delay_ms(1000);
// 		clr();
// 		
// 		WriteCmd(0x81);
// 		DisStr("按键");
// 		DisStr("09");
// 		WriteCmd(0x84);
// 		DisStr(":");
// 		WriteCmd(0x85);
// 		switch (Lcd_flag[8])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x91);
// 		DisStr("按键");
// 		DisStr("10");
// 		WriteCmd(0x94);
// 		DisStr(":");
// 		WriteCmd(0x95);
// 		switch (Lcd_flag[9])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x89);
// 		DisStr("按键");
// 		DisStr("11");
// 		WriteCmd(0x8c);
// 		DisStr(":");
// 		WriteCmd(0x8D);
// 		switch (Lcd_flag[10])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x99);
// 		DisStr("按键");
// 		DisStr("12");
// 		WriteCmd(0x9c);
// 		DisStr(":");
// 		WriteCmd(0x9D);
// 		switch (Lcd_flag[11])
// 		{
// 			case 4:DisStr("开");
// 			break;
// 			case 5:DisStr("关");
// 			break;		     
// 		}
// 		delay_ms(1000);
// 		delay_ms(1000);
// // 		delay_ms(1000);
// 		clr();
// }
