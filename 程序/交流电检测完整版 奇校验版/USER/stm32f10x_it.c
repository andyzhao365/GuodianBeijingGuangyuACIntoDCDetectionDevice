/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "adc.h"
#include "delay.h"
#include "key.h"
#include "crc8_16.h"
#include "read_write_flash.h"
__IO uint16_t CRCValue232 = 0;
__IO uint8_t CRCValue232_H = 0;
__IO uint8_t CRCValue232_L = 0;
int t1_232=3;
int t2_232=4;
u16 RX_flash_232[500];           //接收flash里的数据
u8 TX_flash_232[500];           //从flash里读数据从串口发送出去的数组
u8 k_flag[4];
int time_s;                    //time2用的标志位
int time_data;                 //time2用的标志位
void ADC_work(void);
int16_t ad_data[2][20];
float ad0(void);
float ad1(void);
extern void get_adc_real(void)  ;
//__IO uint16_t update_time;
 enum key
{
	 key_up,key_down,key_ok,key_cancel,key_none
		 
}k1;
mod_bus modbus ={0x01,0x03,0x0933,0x07};
u16 regaddr;
int buzzer_flag;
extern int buzzer_tick_flag;
u8 RxBuffer1[50];
int seat1=0;
extern volatile u32 start_addr;
// enum key k1;

void TIM2_IRQHandler(void)                 //time2中断控制adc采集数据
{
	
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 
         buzzer_flag++;	
         //update_time++;		
		     ADC_work();
		     if(time_s<20)
				 {
				 ad_data[0][time_data]=ad0();
				 ad_data[1][time_data]=ad1();
		     time_s++;
				 time_data++; 
				 } 
          else 
          {
             get_adc_real();         //获取adc检测的交流电压值
          }            
	}		 	
}
void buzzer_tick(void)        //定时器控制蜂鸣器的响声
{
	if(buzzer_tick_flag==0)
	{
		if(buzzer_flag>=500)
		{
			GPIO_WriteBit( GPIOA, GPIO_Pin_5,(BitAction)(1-GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)));
			buzzer_flag=0;
		}
	}else
	{
		BUZZER_off;
	}
}




void EXTI4_IRQHandler(void)  //上
{
	
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
                                      //
	 if(EXTI_GetITStatus(EXTI_Line4) != RESET) 
	 {
		 	Delay_ms(300);
		 EXTI_ClearITPendingBit(EXTI_Line4);
		 	 
	   k1=key_up;
   }
// 	 Delay_ms(10);
// 	 EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

//外部中断pb56
void EXTI9_5_IRQHandler(void)  //返回
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
// 		Delay_ms(10);                                       //
	 if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
	 {
		 Delay_ms(300); 
		  EXTI_ClearITPendingBit(EXTI_Line5);
	   k1=key_cancel;
   }
// 	 Delay_ms(10);
	
	}
	
	
  if(EXTI_GetITStatus(EXTI_Line6) != RESET)   //确定
	{
// 		Delay_ms(10);                                       //
	 if(EXTI_GetITStatus(EXTI_Line6) != RESET) 
	 {
		 	Delay_ms(300);
		 	EXTI_ClearITPendingBit(EXTI_Line6);
	   k1=key_ok;
   }
// 	 Delay_ms(10);

	}
}
//外部中断pb12
void EXTI15_10_IRQHandler(void)          //向下
{
	
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
// 		Delay_ms(10);                                       //
	 if(EXTI_GetITStatus(EXTI_Line12) != RESET) 
	 {
		 	Delay_ms(300);
		 EXTI_ClearITPendingBit(EXTI_Line12);
	   k1=key_down;
   }
// 	 Delay_ms(10);

	}
}

  void CLR_RX1(void)
{
	char k;
    for(k=0;k<50;k++)    //将缓存内容清零
	    {
			RxBuffer1[k] = 0;
		 }
    seat1 = 0;                    //接收字符串的起始存储位置
}
void USART1_IRQHandler(void)
{
  u32 flash_addr;
  u16 addr;
  int i;
  modbus.addr=*(uint32_t*)(device_id_addr);
  if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET) 
    {  
			USART_ClearITPendingBit(USART1,USART_IT_RXNE);
			RxBuffer1[seat1]=USART_ReceiveData(USART1)&0xff;
			 seat1++;
      if((RxBuffer1[0]==modbus.addr))
      {
       if(seat1==8)
       {
           if((RxBuffer1[1]==modbus.function))
       {
          CRCValue232 = Get_Crc16(RxBuffer1,6);
          CRCValue232_H = CRCValue232&0x00ff;
          CRCValue232_L = (CRCValue232&0xff00)>>8;
          if((CRCValue232_L==RxBuffer1[6])&&(CRCValue232_H==RxBuffer1[7]))
          {
              addr=(RxBuffer1[2]<<8)+RxBuffer1[3];
              flash_addr=(addr-modbus.regaddr)*2+start_addr;
              for(i=0;i<RxBuffer1[5];i++)
            {
					   RX_flash_232[i] = (*(uint32_t*)(flash_addr+i*2));  
             TX_flash_232[t1_232] =(RX_flash_232[i]&0xff00)>>8; 
                         t1_232+=2; 
             TX_flash_232[t2_232] =(RX_flash_232[i]&0x00ff); 
                         t2_232+=2;
//               Delay_ms(2);
            }
             t1_232 = 3;                    
             t2_232 = 4;
            TX_flash_232[0]=modbus.addr;
            TX_flash_232[1]=modbus.function;
            TX_flash_232[2]=RxBuffer1[5]*2;
            CRCValue232 = Get_Crc16(TX_flash_232,TX_flash_232[2]+3);
            TX_flash_232[TX_flash_232[2]+3] = CRCValue232&0x00ff;
            TX_flash_232[TX_flash_232[2]+4] = (CRCValue232&0xff00)>>8;
              for(i=0;i<TX_flash_232[2]+5;i++)
            {
              USART_SendData(USART1,TX_flash_232[i]);
              while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
            }
            seat1=0;
            CLR_RX1();
          }
          else
          {
            seat1=0;
            CLR_RX1();
          }
        }
         else
        {
          seat1=0;
          CLR_RX1();
        }

      } 
      else if (seat1>8)
        {
         seat1=0;
          CLR_RX1();
        }

    }
        else
        {
          seat1=0;
          CLR_RX1();
        }
  }
}
// void USART1_IRQHandler(void)
// {
// 	int i;
//   if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET) 
//     {  
// 			RxBuffer1[seat1]=USART_ReceiveData(USART1);
//                seat1++;
// //        if(RxBuffer1[0]== (*(u8*)(0x0800D400))&&RxBuffer1[1]==0x03&&RxBuffer1[2]==0x09&&RxBuffer1[3]==0x33&&RxBuffer1[4]==0x00&&RxBuffer1[5]==0x07&&RxBuffer1[6]==0xF7&&RxBuffer1[7]==0xA8)     //判断结束标志是否是0x0d 0x0a
// //       {
//      if(RxBuffer1[0]==0x01&&RxBuffer1[1]==0x03&&RxBuffer1[2]==0x09&&RxBuffer1[3]==0x33&&RxBuffer1[4]==0x00&&RxBuffer1[5]==0x07&&RxBuffer1[6]==0xF7&&RxBuffer1[7]==0xA8)     //判断结束标志是否是0x0d 0x0a
//       {
//         for(i=0;i<7;i++)
//         {
//           RX_232[i]=RxBuffer1[i];
//         }
//         CRCValue232 = Get_Crc16(RX_232,7);
//         CRCValue232_H = CRCValue232&0x00ff;
//         CRCValue232_L = (CRCValue232&0xff00)>>8;
// //         if((CRCValue232_H==RxBuffer1[8])&&(CRCValue232_L==RxBuffer1[9]))  //判断crc16校验是否正确
// //         {
//           for(i=0;i<RxBuffer1[2];i++)
// 					{
// 					   RX_flash_232[i] = (*(uint32_t*)(start_addr+i*2));                          
//           }
//            TX_flash_232[0] = (*(u8*)(0x0800D400));
//            TX_flash_232[1] = 0X03;   
//            TX_flash_232[2] = RxBuffer1[2];
//            TX_flash_232[3] = (RX_flash_232[0]&0x00ff);
//            TX_flash_232[4] = (RX_flash_232[0]&0xff00)>>8 ;
//            TX_flash_232[5] =0X00;
//            TX_flash_232[6] =RX_flash_232[1];
//            for(i=0;i<7;i++)
//           {
//              USART_SendData(USART1,TX_flash_232[i]);	
//              while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
//           }
//           
// //           USART_SendData(USART1,send_modbus1[j]);	  //send_modbus.Device_address
// // 					while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
//           
// //         }
//           seat1 = 0;
//      
// 				
// 		  }
// 	
//     } 
// 	
// } 
//  void  Time_change(uint32_t time,uint32_t voltage)
// {
//     
// 	unsigned char	hh,mm,ss,month,day;
// 	unsigned short CRC_16;
//     
// 	hh = time / 3600 % 24;
//     mm  = (time % 3600) / 60;
//     ss  =  (time % 3600) % 60;
// 	day = time / 86400;
// 	month = Month(day);
// 	
// //  send_modbus.CRC_low = (unsigned char)(( CRC_16&(0XFF00))>>8);
// //  send_modbus.CRC_high =(unsigned char)( CRC_16&(0X00FF));

//   	send_modbus1[0] =   *(u8*) (0x0800D400);
// 	send_modbus1[1] =   0x03;
// 	send_modbus1[2] =   0x0E;
// 	send_modbus1[3] =   (voltage&0x00ff) ;
// 	send_modbus1[4] =   (voltage&0xff00)>>8 ;
// 	send_modbus1[5] =   0x00;
// 	send_modbus1[6] =   ss;
// 	send_modbus1[7] =   0x00;
// 	send_modbus1[8] =   mm;
// 	send_modbus1[9] =   0x00;
// 	send_modbus1[10] =   hh;
// 	send_modbus1[11] =   0x00;
// 	send_modbus1[12] =   day;
// 	send_modbus1[13] =   0x00;
// 	send_modbus1[14] =   month;
// 	send_modbus1[15] =   0x00;
// 	send_modbus1[16] =   0x0E;
// 	CRC_16 = CRC16(send_modbus1, 17);
// 	send_modbus1[17]= (unsigned char)(( CRC_16&(0XFF00))>>8);
//     send_modbus1[18] =(unsigned char)( CRC_16&(0X00FF));
//   
//   
//  /* 
//   send_modbus.Device_address = 	0x02;
//   send_modbus.Function_code  =	0x03;
//   send_modbus.Returns_the_number_of_bytes= 0x0E;
//   send_modbus.voltage_low =  (u8)((*(u8 *)(0x0800D02C ))&(0X000000FF));
//   send_modbus.voltage_high = (u8)(((*(u8 *)(0x0800D02C ))&(0X0000FF00))>>8);
//   send_modbus.seconds_low = 0;
//   send_modbus.seconds_high = ss ;
//   send_modbus.minute_low = 0 ;
//   send_modbus.minute_high = mm;
//   send_modbus.hour_low = 0 ;
//   send_modbus.hour_high = hh;
//   send_modbus.day_low = 0 ;
//   send_modbus.day_high = day;
//   send_modbus.month_low = 0 ;
//   send_modbus.month_high = month;
//   send_modbus.year_low  = 0;
//   send_modbus.year_high = 0x0E;			   */

//   





// }

// void USART1_IRQHandler(void)                	//串口1中断服务程序
// {
// 	unsigned int i,j;
// 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	   //判断读寄存器是否非空
//   {	
//     /* Read one byte from the receive data register */
//   
// }
//   if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)                   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 
//   { 
//      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);					                //禁止发缓冲器空中断， 
//   }	
//   

// } 
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
//__asm void wait()
//{
 //     BX lr
//}
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
//	wait();
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
