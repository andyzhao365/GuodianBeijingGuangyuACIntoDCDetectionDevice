#include "stm32f10x.h"
#include "delay.h"    //±ØĞëÅäºÏdelay.cºÍdelay.hÎÄ¼şÊ¹ÓÃ£¬ËùÒÔÒª°üº¬delay.h¡£
#include "display12864.h"

// extern u8 Lcd_flag[12];
#define u16 unsigned short   //ÎªÁË¿ÉÒÆÖ²ĞÔºÃ£¬¶ÔÕâÁ½¸ö STM 32 ÒÑ¾­¶¨Òå¹ıµÄ±äÁ¿£¬ÔÙ¶¨ÒåÒ»´Î¡£
#define u8 unsigned char

/********** ÒÔÏÂÊÇÏà¹ØÒı½Å¶¨Òå¡£**************/

#define DisIOB GPIOB                 //¶¨Òå12864ÒªÊ¹ÓÃµÄI/O¶Ë¿Ú¡£
#define DisIOA GPIOC                 //¶¨Òå12864ÒªÊ¹ÓÃµÄI/O¶Ë¿Ú¡
#define DisClkA RCC_APB2Periph_GPIOC  //¶¨Òå12864ÒªÊ¹ÓÃµÄI/O¶Ë¿ÚµÄÊ±ÖÓ¡£
#define DisClkB RCC_APB2Periph_GPIOB 
#define Data GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
                                       //¶¨Òå12864Ê¹ÓÃµÄÊı¾İÒı½Å¡£
#define RS GPIO_Pin_8
#define RW GPIO_Pin_9
#define EN GPIO_Pin_10       //¶¨ÒåÊ¹ÄÜ¶ËÊ¹ÓÃµÄÒı½Å¡£
#define PSB GPIO_Pin_11      //¶¨Òå²¢£¬´®ĞĞÊı¾İÄ£Ê½µÄÑ¡Ôñ¶Ë¿ÚÊ¹ÓÃµÄÒı½Å¡£

/*********************************************/
                					    
GPIO_InitTypeDef  GPIOStru;  //¶¨ÒåÓÃÓÚ¶¨ÒåËùÒÔÒı½ÅÎªÊä³öµÄ±äÁ¿¡£

// void IOInitOut(void)           //°ÑËùÓĞ¶Ë¿Ú³õÊ¼»¯ÎªÍÆÍìÊä³öÄ£Ê½µÄº¯Êı¡£
// {
//     GPIOStru.GPIO_Mode = GPIO_Mode_Out_PP;//¶¨ÒåËùÓĞµÄÒı½ÅÎªÍÆÍìÊä³öµÄ±äÁ¿³õÊ¼»¯¡£
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

void IOInitIn(void)           											//°ÑÊı¾İÒı½Å³õÊ¼»¯Îª¸¡¿ÕÊäÈëµÄº¯Êı¡£
{
    GPIOStru.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//¶¨ÒåÊı¾İÒı½ÅÎª¸¡¿ÕÊäÈëµÄ±äÁ¿³õÊ¼»¯¡£
    GPIOStru.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOStru.GPIO_Pin = Data;

    RCC_APB2PeriphClockCmd(DisClkA,ENABLE);   			//°ÑËùÓĞ¶Ë¿Ú³õÊ¼»¯ÎªÊä³öÄ£Ê½µÄº¯Êı¡£   
    GPIO_Init(DisIOA,&GPIOStru);	
}

void WaitBusy(void)          //µÈ´ı12864µÄÃ¦×´Ì¬½áÊøµÄº¯Êı¡£
{
    IOInitIn();        //°ÑÊı¾İÒı½Å¶¨ÒåÎª¸¡¿ÕÊäÈë£»
    GPIO_ResetBits(DisIOB,RS);  //RS = 0.
    GPIO_SetBits(DisIOB,RW);    //RW = 1.
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    while(GPIO_ReadInputData(DisIOA) & 0x0080); //Ö»ÒªÎ»7µÄÖµ£¬Î»7ÊÇÃ¦±êÖ¾Î»¡£
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    IOInitOut();      //°ÑËùÓĞÒı½Å¶¨ÒåÎªÊä³ö¡£
}

void WriteCmd(u8 cmd)    //Ğ´ÃüÁîº¯Êı¡£
{
  	WaitBusy(); 
    GPIO_ResetBits(DisIOB,RS);  //RS = 0.
    GPIO_ResetBits(DisIOB,RW);  //RW = 0. 
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    DisIOA->ODR=((DisIOA->ODR & 0xff00)|cmd); //´Ë´¦£¬Ö»ÓĞÖ±½Ó²Ù×÷¼Ä´æÆ÷²ÅÄÜ
                  //´ïµ½£¬Ö»¸Ä±äÊä³öÊı¾İ¼Ä´æÆ÷ODRµÄµÍ8Î»£¬ÆäËüÎ»
                  //²»±äµÄÄ¿µÄ¡£ÒòÎª£¬Ö»ÓĞµÍ8Î»ÊÇÊı¾İÒı½Å£¬
                  //ÆäËüÎ»¿ÉÄÜÊÇ¿ØÖÆÒı½Å£¬²»ÄÜ¸Ä±ä¡£
    Delay_us(100);
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    Delay_us(100); 	
}

void WriteData(u8 data)  //Ğ´Êı¾İº¯Êı¡£
{
	  WaitBusy(); 
    GPIO_SetBits(DisIOB,RS);  //RS = 1. 
    GPIO_ResetBits(DisIOB,RW);  //RW = 0. 
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    DisIOA->ODR=((DisIOA->ODR & 0xff00)|data);  //Í¬ÉÏ¡£
    Delay_us(100);
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    Delay_us(100); 
}


void WriteData1(u8 data)  //Ğ´Êı¾İº¯Êı¡£
{
	  WaitBusy(); 
    GPIO_SetBits(DisIOB,RS);  //RS = 1. 
    GPIO_ResetBits(DisIOB,RW);  //RW = 0. 
    GPIO_SetBits(DisIOB,EN);    //EN = 1.
    DisIOA->ODR=((DisIOA->ODR & 0xff00)|data);  //Í¬ÉÏ¡£
    Delay_us(400);
    GPIO_ResetBits(DisIOB,EN);  //EN = 0;
    Delay_us(400); 
}
void InitDis(void)   //³õÊ¼»¯ 12864 ºÍÒªÓÃµ½µÄ STM 32 µÄÒı½Å¡£
{
    IOInitOut(); 	
    delay_init(72);   //³õÊ¼»¯ÑÓÊ±º¯ÊıµÄÎ¢Ãî¼ÆÊı»ùÊı¡£  
    GPIO_SetBits(DisIOB,PSB);  //ÁîPSB=1£¬ÉèÖÃÎª²¢ĞĞÊı¾İÄ£Ê½¡£
    Delay_us(400);
    WriteCmd(0x30);  //Ñ¡Ôñ»ù±¾Ö¸Áî¼¯£¬ºÍ£¬8Î»Êı¾İÄ£Ê½¡£
    Delay_us(400);
    WriteCmd(0x0c);  //¿ªÏÔÊ¾,ÎŞÓÎ±ê,²»·´°×.
    Delay_us(400); 
    WriteCmd(0x01);  //Çå³ıÏÔÊ¾£¬²¢½« DDRAM µÄµØÖ·¼ÆÊıÆ÷ AC ÉèÎª 00H.
    Delay_us(400);
    WriteCmd(0x06);  //ÉèÖÃ£¬Íâ²¿¶ÁĞ´Êı¾İºó,µØÖ·¼ÇÊıÆ÷ AC »á×Ô¶¯¼Ó 1¡£
    Delay_us(400);
    WriteCmd(0x80);  //½« DDRAM µØÖ·¼ÆÊıÆ÷ AC ÉèÎª 0.
    Delay_us(400);	
}



void DisStr(u8 *s)    //ÏÔÊ¾×Ö·û´®µÄº¯Êı¡£
{
    while(*s != '\0')
    {
        WriteData(*s);
        s++;
        Delay_ms(1);
    }
}

void DisInt(long int num)  //ÏÔÊ¾ÕûĞÍ±äÁ¿µÄº¯Êı,×î¶àÏÔÊ¾16Î»µÄÕûÊı¡£Ö»ÄÜÏÔÊ¾ÕıÊı¡£
{
	u8 temp[17];
	u8 str[17];
	int i=0,j=0;
	while(num != 0)	  //ÕâÀï²»ÄÜÓÃnum%10 != 0£¬Èç¹ûnumÊÇ10µÄÕûÊı±¶£¬
	                  //ÀıÈç£¬100£¬ÕâÑù¾Í»á³ö´í£¬¸ù±¾¾Í²»ÄÜ½øÈëÑ­»·Ìå¡£
	{
		temp[i] = (num%10)+0x30;
		num/=10;
		i++;
	}
	i--;           //ÒòÎªiÔÚÍË³öÑ­»·Ö®Ç°»¹×Ô¼ÓÁËÒ»´Î£¬´ËÊ±£¬
	                //Ö¸Ïò×îºóÒ»¸ö´æ´¢ÓĞÓÃÖµµÄÔªËØµÄºóÒ»¸öÎ»ÖÃ¡£
	while(i != -1)	 //ÒòÎªi=0Ê±£¬temp[0]»¹ÊÇÓĞÓÃÖµ¡£
	{
		str[j] = temp[i];
		j++;
		i--;	
	}
	str[j]='\0';  //ÒòÎªiÔÚÍË³öÑ­»·Ö®Ç°»¹×Ô¼ÓÁËÒ»´Î£¬´ËÊ±£¬
	              //Ö¸Ïò×îºóÒ»¸ö´æ´¢ÓĞÓÃÖµµÄÔªËØµÄºóÒ»¸öÎ»ÖÃ¡£
	DisStr(str);	
}

void DisFloat(float fnum) //ÏÔÊ¾ÓĞ4Î»Ğ¡ÊıµÄ¸¡µãÊı£¬×ÜÎ»Êı²»³¬¹ı16Î»¡£
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
		if(i == 4)	 //4Î»Ğ¡Êı´¦ÀíÍêºó£¬¼ÓÈëĞ¡Êıµã¡£
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

/*ÒÔÏÂÊÇ¹â±ê¶¨Î»º¯Êı£¬µÚÒ»¸ö²ÎÊıÎªĞĞ×ø±ê£¬µÚ¶ş¸öÎªÁĞ×ø±ê£¬ÆğÊ¼×ø±êÊÇ1ĞĞ1ÁĞ¡£
Ö»ÄÜÒÔ16¸öµãµÄ¿í¶ÈÎªµ¥Î»ÒÆ¶¯¡£*/

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

/*¹â±ê¶¨Î»º¯Êı¶¨Òå½áÊø¡£*/
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
// 		DisStr("°´¼ü");
// 		DisStr("01");
// 		WriteCmd(0x84);
// 		DisStr(":");
// 		WriteCmd(0x85);
// 		switch (Lcd_flag[0])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;
// 				     
// 		}
// 		WriteCmd(0x91);
// 		DisStr("°´¼ü");
// 		DisStr("02");
// 		WriteCmd(0x94);
// 		DisStr(":");
// 		WriteCmd(0x95);
// 		switch (Lcd_flag[1])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x89);
// 		DisStr("°´¼ü");
// 		DisStr("03");
// 		WriteCmd(0x8c);
// 		DisStr(":");
// 		WriteCmd(0x8D);
// 		switch (Lcd_flag[2])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x99);
// 		DisStr("°´¼ü");
// 		DisStr("04");
// 		WriteCmd(0x9c);
// 		DisStr(":");
// 		WriteCmd(0x9D);
// 		switch (Lcd_flag[3])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		
// 		delay_ms(1000);
// 		delay_ms(1000);
// // 		delay_ms(1000);
// 		clr();
// 		
// 		WriteCmd(0x81);
// 		DisStr("°´¼ü");
// 		DisStr("05");
// 		WriteCmd(0x84);
// 		DisStr(":");
// 		WriteCmd(0x85);
// 		switch (Lcd_flag[4])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x91);
// 		DisStr("°´¼ü");
// 		DisStr("06");
// 		WriteCmd(0x94);
// 		DisStr(":");
// 		WriteCmd(0x95);
// 		switch (Lcd_flag[5])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x89);
// 		DisStr("°´¼ü");
// 		DisStr("07");
// 		WriteCmd(0x8c);
// 		DisStr(":");
// 		WriteCmd(0x8D);
// 		switch (Lcd_flag[6])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x99);
// 		DisStr("°´¼ü");
// 		DisStr("08");
// 		WriteCmd(0x9c);
// 		DisStr(":");
// 		WriteCmd(0x9D);
// 		switch (Lcd_flag[7])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		delay_ms(1000);
// 		delay_ms(1000);
// // 		delay_ms(1000);
// 		clr();
// 		
// 		WriteCmd(0x81);
// 		DisStr("°´¼ü");
// 		DisStr("09");
// 		WriteCmd(0x84);
// 		DisStr(":");
// 		WriteCmd(0x85);
// 		switch (Lcd_flag[8])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x91);
// 		DisStr("°´¼ü");
// 		DisStr("10");
// 		WriteCmd(0x94);
// 		DisStr(":");
// 		WriteCmd(0x95);
// 		switch (Lcd_flag[9])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x89);
// 		DisStr("°´¼ü");
// 		DisStr("11");
// 		WriteCmd(0x8c);
// 		DisStr(":");
// 		WriteCmd(0x8D);
// 		switch (Lcd_flag[10])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		
// 		WriteCmd(0x99);
// 		DisStr("°´¼ü");
// 		DisStr("12");
// 		WriteCmd(0x9c);
// 		DisStr(":");
// 		WriteCmd(0x9D);
// 		switch (Lcd_flag[11])
// 		{
// 			case 4:DisStr("¿ª");
// 			break;
// 			case 5:DisStr("¹Ø");
// 			break;		     
// 		}
// 		delay_ms(1000);
// 		delay_ms(1000);
// // 		delay_ms(1000);
// 		clr();
// }
