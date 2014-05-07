
#include "adc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
u16 ADC1_Value[200];
float start_voltage0;
float start_voltage1;
u16 ADC1_0[100];      //放adc通道1的数据
u16	ADC1_1[100];      //放adc通道2的数据
void ADC1_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void ADC1_RCC(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
}

void ADC1_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC1独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//单通道
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道3,规则采样顺序值为1,采样时间为239.5周期	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);  
	ADC_DMACmd(ADC1, ENABLE);//使能ADC1模块DMA
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	/* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);	//重置指定的ADC1的校准寄存器
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1重置校准寄存器的状态,设置状态则等待
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);		//开始指定ADC1的校准状态
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));		//获取指定ADC1的校准程序,设置状态则等待
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}

void DMA1_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
  
	DMA_DeInit(DMA1_Channel1);//复位DMA通道1
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)0x4001244C; //定义DMA通道外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1_Value; //定义DMA通道存储器地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//指定外设为源地址  单向传输
	DMA_InitStructure.DMA_BufferSize = 200;//DMA缓冲区大小16bits
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//当前外设寄存器地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//当前存储器地址不变
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//定义外设数据宽度16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //定义存储器数据宽度16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMA通道操作模式位环形缓冲模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;//DMA通道优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//禁止DMA通道存储器到存储器传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//初始化DMA1通道1
	DMA_Cmd(DMA1_Channel1, ENABLE); //使能DMA1通道1
}


void ADC_work(void)
{
		 int j = 0;
		 int u = 0,k;
		for(k=0;k<200;k++)
		{
		  	
			if(k%2 == 0)
			{
		  	ADC1_0[j++] = ADC1_Value[k];
				if(j == 100) j = 0;
	  	}
			if(k%2 != 0)
			{	
				ADC1_1[u++] = ADC1_Value[k];
				if(u == 100) u =0;
		 	}
		 }	
}

float ad0(void)     //adc1通道1的数据求平均
{ 
	int jj;
	u32 zhi=0;
	float r=0;
	for(jj=0;jj<100;jj++)
	 {
		 zhi +=  ADC1_0[jj];
	 }
	 r= (float)(zhi/100);
	 return r;
}

float ad1(void)   //adc1通道2的数据求平均
{ 
	int jj;
	u32 zhi=0;
	float r=0;
	for(jj=0;jj<100;jj++)
	 {
		 zhi +=  ADC1_1[jj];
	 }
	 r = (float)(zhi/100);
	 return r;
}

//获取adc的初始值
void start_adc(void)
{ 
// 	int i;
	ADC_work();
 while((start_voltage0<1.4)&&(start_voltage1<1.4))
 {
	start_voltage0=(ad0()*3.3)/4096;
	start_voltage1=(ad1()*3.3)/4096;
 }

}

