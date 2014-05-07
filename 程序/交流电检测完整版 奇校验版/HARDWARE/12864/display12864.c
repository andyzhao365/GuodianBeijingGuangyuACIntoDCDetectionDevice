#include "stm32f10x.h"
#include "delay.h"    //�������delay.c��delay.h�ļ�ʹ�ã�����Ҫ����delay.h��
#include "display12864.h"

// extern u8 Lcd_flag[12];
#define u16 unsigned short   //Ϊ�˿���ֲ�Ժã��������� STM 32 �Ѿ�������ı������ٶ���һ�Ρ�
#define u8 unsigned char

/********** ������������Ŷ��塣**************/

#define DisIOB GPIOB                 //����12864Ҫʹ�õ�I/O�˿ڡ�
#define DisIOA GPIOC                 //����12864Ҫʹ�õ�I/O�˿ڡ
#define DisClkA RCC_APB2Periph_GPIOC  //����12864Ҫʹ�õ�I/O�˿ڵ�ʱ�ӡ�
#define DisClkB RCC_APB2Periph_GPIOB 
#define Data GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
                                       //����12864ʹ�õ��������š�
#define RS GPIO_Pin_8
#define RW GPIO_Pin_9
#define EN GPIO_Pin_10       //����ʹ�ܶ�ʹ�õ����š�
#define PSB GPIO_Pin_11      //���岢����������ģʽ��ѡ��˿�ʹ�õ����š�

/*********************************************/
                					    
GPIO_InitTypeDef  GPIOStru;  //�������ڶ�����������Ϊ����ı�����

// void IOInitOut(void)           //�����ж˿ڳ�ʼ��Ϊ�������ģʽ�ĺ�����
// {
//     GPIOStru.GPIO_Mode = GPIO_Mode_Out_PP;//�������е�����Ϊ��������ı�����ʼ����
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

void IOInitIn(void)           											//���������ų�ʼ��Ϊ��������ĺ�����
{
    GPIOStru.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//������������Ϊ��������ı�����ʼ����
    GPIOStru.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOStru.GPIO_Pin = Data;

    RCC_APB2PeriphClockCmd(DisClkA,ENABLE);   			//�����ж˿ڳ�ʼ��Ϊ���ģʽ�ĺ�����   
    GPIO_Init(DisIOA,&GPIOStru);	
}

void WaitBusy(void)          //�ȴ�12864��æ״̬�����ĺ�����
{
    IOInitIn();        //���������Ŷ���Ϊ�������룻
    GPIO_ResetBits(DisIOB,RS);  //RS = 0.
    GPIO_SetBits(DisIOB,RW);    //RW = 1.
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    while(GPIO_ReadInputData(DisIOA) & 0x0080); //ֻҪλ7��ֵ��λ7��æ��־λ��
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    IOInitOut();      //���������Ŷ���Ϊ�����
}

void WriteCmd(u8 cmd)    //д�������
{
  	WaitBusy(); 
    GPIO_ResetBits(DisIOB,RS);  //RS = 0.
    GPIO_ResetBits(DisIOB,RW);  //RW = 0. 
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    DisIOA->ODR=((DisIOA->ODR & 0xff00)|cmd); //�˴���ֻ��ֱ�Ӳ����Ĵ�������
                  //�ﵽ��ֻ�ı�������ݼĴ���ODR�ĵ�8λ������λ
                  //�����Ŀ�ġ���Ϊ��ֻ�е�8λ���������ţ�
                  //����λ�����ǿ������ţ����ܸı䡣
    Delay_us(100);
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    Delay_us(100); 	
}

void WriteData(u8 data)  //д���ݺ�����
{
	  WaitBusy(); 
    GPIO_SetBits(DisIOB,RS);  //RS = 1. 
    GPIO_ResetBits(DisIOB,RW);  //RW = 0. 
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    DisIOA->ODR=((DisIOA->ODR & 0xff00)|data);  //ͬ�ϡ�
    Delay_us(100);
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    Delay_us(100); 
}


void WriteData1(u8 data)  //д���ݺ�����
{
	  WaitBusy(); 
    GPIO_SetBits(DisIOB,RS);  //RS = 1. 
    GPIO_ResetBits(DisIOB,RW);  //RW = 0. 
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    DisIOA->ODR=((DisIOA->ODR & 0xff00)|data);  //ͬ�ϡ�
    Delay_us(400);
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    Delay_us(400); 
}
void InitDis(void)   //��ʼ�� 12864 ��Ҫ�õ��� STM 32 �����š�
{
    IOInitOut(); 	
    delay_init(72);   //��ʼ����ʱ������΢�����������  
    GPIO_SetBits(DisIOB,PSB);  //��PSB=1������Ϊ��������ģʽ��
    Delay_us(400);
    WriteCmd(0x30);  //ѡ�����ָ����ͣ�8λ����ģʽ��
    Delay_us(400);
    WriteCmd(0x0c);  //����ʾ,���α�,������.
    Delay_us(400); 
    WriteCmd(0x01);  //�����ʾ������ DDRAM �ĵ�ַ������ AC ��Ϊ 00H.
    Delay_us(400);
    WriteCmd(0x06);  //���ã��ⲿ��д���ݺ�,��ַ������ AC ���Զ��� 1��
    Delay_us(400);
    WriteCmd(0x80);  //�� DDRAM ��ַ������ AC ��Ϊ 0.
    Delay_us(400);	
}



void DisStr(u8 *s)    //��ʾ�ַ����ĺ�����
{
    while(*s != '\0')
    {
        WriteData(*s);
        s++;
        Delay_ms(1);
    }
}

void DisInt(long int num)  //��ʾ���ͱ����ĺ���,�����ʾ16λ��������ֻ����ʾ������
{
	u8 temp[17];
	u8 str[17];
	int i=0,j=0;
	while(num != 0)	  //���ﲻ����num%10 != 0�����num��10����������
	                  //���磬100�������ͻ���������Ͳ��ܽ���ѭ���塣
	{
		temp[i] = (num%10)+0x30;
		num/=10;
		i++;
	}
	i--;           //��Ϊi���˳�ѭ��֮ǰ���Լ���һ�Σ���ʱ��
	                //ָ�����һ���洢����ֵ��Ԫ�صĺ�һ��λ�á�
	while(i != -1)	 //��Ϊi=0ʱ��temp[0]��������ֵ��
	{
		str[j] = temp[i];
		j++;
		i--;	
	}
	str[j]='\0';  //��Ϊi���˳�ѭ��֮ǰ���Լ���һ�Σ���ʱ��
	              //ָ�����һ���洢����ֵ��Ԫ�صĺ�һ��λ�á�
	DisStr(str);	
}

void DisFloat(float fnum) //��ʾ��4λС���ĸ���������λ��������16λ��
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
		if(i == 4)	 //4λС��������󣬼���С���㡣
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

/*�����ǹ�궨λ��������һ������Ϊ�����꣬�ڶ���Ϊ�����꣬��ʼ������1��1�С�
ֻ����16����Ŀ��Ϊ��λ�ƶ���*/

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

/*��궨λ�������������*/
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
// 		DisStr("����");
// 		DisStr("01");
// 		WriteCmd(0x84);
// 		DisStr(":");
// 		WriteCmd(0x85);
// 		switch (Lcd_flag[0])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;
// 				     
// 		}
// 		WriteCmd(0x91);
// 		DisStr("����");
// 		DisStr("02");
// 		WriteCmd(0x94);
// 		DisStr(":");
// 		WriteCmd(0x95);
// 		switch (Lcd_flag[1])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x89);
// 		DisStr("����");
// 		DisStr("03");
// 		WriteCmd(0x8c);
// 		DisStr(":");
// 		WriteCmd(0x8D);
// 		switch (Lcd_flag[2])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x99);
// 		DisStr("����");
// 		DisStr("04");
// 		WriteCmd(0x9c);
// 		DisStr(":");
// 		WriteCmd(0x9D);
// 		switch (Lcd_flag[3])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		
// 		delay_ms(1000);
// 		delay_ms(1000);
// // 		delay_ms(1000);
// 		clr();
// 		
// 		WriteCmd(0x81);
// 		DisStr("����");
// 		DisStr("05");
// 		WriteCmd(0x84);
// 		DisStr(":");
// 		WriteCmd(0x85);
// 		switch (Lcd_flag[4])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x91);
// 		DisStr("����");
// 		DisStr("06");
// 		WriteCmd(0x94);
// 		DisStr(":");
// 		WriteCmd(0x95);
// 		switch (Lcd_flag[5])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x89);
// 		DisStr("����");
// 		DisStr("07");
// 		WriteCmd(0x8c);
// 		DisStr(":");
// 		WriteCmd(0x8D);
// 		switch (Lcd_flag[6])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x99);
// 		DisStr("����");
// 		DisStr("08");
// 		WriteCmd(0x9c);
// 		DisStr(":");
// 		WriteCmd(0x9D);
// 		switch (Lcd_flag[7])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		delay_ms(1000);
// 		delay_ms(1000);
// // 		delay_ms(1000);
// 		clr();
// 		
// 		WriteCmd(0x81);
// 		DisStr("����");
// 		DisStr("09");
// 		WriteCmd(0x84);
// 		DisStr(":");
// 		WriteCmd(0x85);
// 		switch (Lcd_flag[8])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x91);
// 		DisStr("����");
// 		DisStr("10");
// 		WriteCmd(0x94);
// 		DisStr(":");
// 		WriteCmd(0x95);
// 		switch (Lcd_flag[9])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x89);
// 		DisStr("����");
// 		DisStr("11");
// 		WriteCmd(0x8c);
// 		DisStr(":");
// 		WriteCmd(0x8D);
// 		switch (Lcd_flag[10])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x99);
// 		DisStr("����");
// 		DisStr("12");
// 		WriteCmd(0x9c);
// 		DisStr(":");
// 		WriteCmd(0x9D);
// 		switch (Lcd_flag[11])
// 		{
// 			case 4:DisStr("��");
// 			break;
// 			case 5:DisStr("��");
// 			break;		     
// 		}
// 		delay_ms(1000);
// 		delay_ms(1000);
// // 		delay_ms(1000);
// 		clr();
// }
