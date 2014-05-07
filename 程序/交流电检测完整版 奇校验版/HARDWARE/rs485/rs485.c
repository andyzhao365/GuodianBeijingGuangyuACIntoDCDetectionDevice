		    
#include "rs485.h"	 
#include "stm32f10x.h"
#include "key.h"
#include "delay.h"
#include "crc8_16.h"
#include "stm32f10x_it.h"
#include "read_write_flash.h"

 __IO uint16_t CRCValue485;
 __IO uint8_t CRCValue485_H;
 __IO uint8_t CRCValue485_L;
 int t1_485;
 int t2_485;
u16 RX_flash_485[500];           //����flash�������
u8 TX_flash_485[500];  
extern volatile u32 start_addr;
u8 RxBuffer2[8];
int seat2;

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void RS485_Init(u32 bound)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
//  	NVIC_InitTypeDef NVIC_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOA,Gʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //PG9�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ
 
	
		  	//���ʹ���˽���
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;//8λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Odd;///��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure);  //��ʼ������
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
    RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
}

   void CLR_RX2(void)
{
	char k;
    for(k=0;k<50;k++)    //��������������
	    {
			RxBuffer2[k] = 0;
		 }
    seat2 = 0;                    //�����ַ�������ʼ�洢λ��
}


void USART2_IRQHandler(void)
{
  u32 flash_addr;
  u16 addr;
  int i;
  modbus.addr=*(uint32_t*)(device_id_addr);
  if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET) 
    {  
			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
			RxBuffer2[seat2]=USART_ReceiveData(USART2);
			 seat2++;
      if(RxBuffer2[0]==modbus.addr)
       {
       if(seat2==8)
       {
//         if((RxBuffer2[0]==modbus.addr)&&(RxBuffer2[1]==modbus.function))
//        {
        if((RxBuffer2[1]==modbus.function))
       {


          CRCValue485 = Get_Crc16(RxBuffer2,6);
          CRCValue485_H = CRCValue485&0x00ff;
          CRCValue485_L = (CRCValue485&0xff00)>>8;
          if((CRCValue485_L==RxBuffer2[6])&&(CRCValue485_H==RxBuffer2[7]))
          {
              addr=(RxBuffer2[2]<<8)+RxBuffer2[3];
              flash_addr=(addr-modbus.regaddr)*2+start_addr;
              for(i=0;i<RxBuffer2[5];i++)
            {
					   RX_flash_485[i] = (*(uint32_t*)(flash_addr+i*2));  
             TX_flash_485[t1_485] =(RX_flash_485[i]&0xff00)>>8; 
                         t1_485+=2; 
             TX_flash_485[t2_485] =(RX_flash_485[i]&0x00ff); 
                         t2_485+=2;
//               Delay_ms(2);
            }
             t1_485 = 3;                    
             t2_485 = 4;
            TX_flash_485[0]=modbus.addr;
            TX_flash_485[1]=modbus.function;
            TX_flash_485[2]=RxBuffer2[5]*2;
            CRCValue485 = Get_Crc16(TX_flash_485,TX_flash_485[2]+3);
            TX_flash_485[TX_flash_485[2]+3] = CRCValue485&0x00ff;
            TX_flash_485[TX_flash_485[2]+4] = (CRCValue485&0xff00)>>8;
              for(i=0;i<TX_flash_485[2]+5;i++)
            {
              RS485_TX_EN=1;
              USART_SendData(USART2,TX_flash_485[i]);
              while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
            }
             RS485_TX_EN=0;
            seat2=0;
            CLR_RX2();
          }
          else
          {
            seat2=0;
            CLR_RX2();
          }
        }
          else
        {
          seat2=0;
          CLR_RX2();
        }
      } 
      else if(seat2>8)
      {
         seat2=0;
         CLR_RX2();
      }

    }
      else {
         seat2=0;
          CLR_RX2();
       }
  }

}
// void USART2_IRQHandler(void)
// {
//   int i;
//   if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET) 
//     {  
// 			USART_ClearITPendingBit(USART2,USART_IT_RXNE);
// 			RxBuffer2[seat2]=USART_ReceiveData(USART2);
// 			 seat2++;
//        if(seat2==5)
//        {
//         if((RxBuffer2[0]==0x01)&&(RxBuffer2[1]==0x02)&&(RxBuffer2[2]==0x0e))
//         {
//           CRCValue485 = Get_Crc16(RxBuffer2,3);
//           CRCValue485_H = CRCValue485&0x00ff;
//           CRCValue485_L = (CRCValue485&0xff00)>>8;
//           if((CRCValue485_H==RxBuffer2[3])&&(CRCValue485_L==RxBuffer2[4]))
//           {
//               for(i=0;i<RxBuffer2[2];i++)
//             {
// 					   RX_flash_485[i] = (*(uint32_t*)(start_addr+i*2));  
//              TX_flash_485[t1_485] =((u16)RX_flash_485[i]&0xff00)>>8; 
//                          t1_485+=2; 
//              TX_flash_485[t2_485] =((u16)RX_flash_485[i]&0x00ff); 
//                          t2_485+=2;
//               Delay_ms(2);
//             }
//              t1_485 = 0;                    
//              t2_485 = 1;
//             CRCValue485 = Get_Crc16(TX_flash_485,2*RxBuffer2[2]);
//             TX_flash_485[2*RxBuffer2[2]+1] = CRCValue485&0x00ff;
//             TX_flash_485[2*RxBuffer2[2]+2] = (CRCValue485&0xff00)>>8;
//               for(i=0;i<(RxBuffer2[2]+1)*2;i++)
//             {
//               RS485_TX_EN=1;                   //����ģʽ
//               USART_SendData(USART2,TX_flash_485[i]);
//               while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
//             }
//              RS485_TX_EN=0;                     //������ɺ���λ����ģʽ
//              seat2=0;
//              CLR_RX2();
//           }
//           else                              //����
//           {
//             seat2=0;
//             CLR_RX2();
//           }

//         }
//         else
//         {
//            seat2=0;
//           CLR_RX2();
//         }
//     }
//   }
// }











// void USART2_IRQHandler(void)
// {
// 	u8 i;	    
//  
//  	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
// 	{	 
// 	 	RS485_RX_BUF[RS485_RX_CNT++] =USART_ReceiveData(USART2); 		 
// //		res =USART_ReceiveData(USART2); 	//��ȡ���յ�������
// //		if(RS485_RX_CNT<6)
// //		{
// //			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
// //			RS485_RX_CNT++;						//������������1 
// //		} 
// 		
// 	  if(RS485_RX_BUF[0]== *(u8*)(0x0800D400)&&RS485_RX_BUF[1]==0x03&&RS485_RX_BUF[2]==0x09&&RS485_RX_BUF[3]==0x33&&RS485_RX_BUF[4]==0x00&&RS485_RX_BUF[5]==0x07&&RS485_RX_BUF[6]==0xF7&&RS485_RX_BUF[7]==0xA8)     //�жϽ�����־�Ƿ���0x0d 0x0a
//       {
// 	   	     
// 	  						 		 Time_change(modbustime,iii);
// 							         RS485_Send_Data(send_modbus1,19);
// 									// RS485_RX_CNT =0; 
// 	                                  for(i=0;i<9;i++)
// 		                              {
// 		                               RS485_RX_BUF[i] = 0;
// 	                               	  }
// 	   
// 	  }
// 	 if(RS485_RX_CNT == 8) 
// 	 {
// 	
// 		RS485_RX_CNT = 0;
// 	 }
// 	}  
	
	
	
	
	
												 
// } 
									 


//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
// void RS485_Send_Data(u8 *buf,u8 len)
// {
// 	u8 t;
// 	RS485_TX_EN=1;			//����Ϊ����ģʽ
//   	for(t=0;t<len;t++)		//ѭ����������
// 	{		   
// 		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
// 		USART_SendData(USART2,buf[t]);
// 	}	 
//  
// 	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
// 	RS485_RX_CNT=0;	  
// 	RS485_TX_EN=0;				//����Ϊ����ģʽ	
// }
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
// void RS485_Receive_Data(u8 *buf,u8 *len)
// {
// 	u8 rxlen=RS485_RX_CNT;
// 	u8 i=0;
// 	*len=0;				//Ĭ��Ϊ0
// 	Delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
// 	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
// 	{
// 		for(i=0;i<rxlen;i++)
// 		{
// 			buf[i]=RS485_RX_BUF[i];	
// 		}		
// 		*len=RS485_RX_CNT;	//��¼�������ݳ���
// 		RS485_RX_CNT=0;		//����
// 	}
// }

