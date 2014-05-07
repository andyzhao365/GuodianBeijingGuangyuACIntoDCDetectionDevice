
#include "adc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
u16 ADC1_Value[200];
float start_voltage0;
float start_voltage1;
u16 ADC1_0[100];      //��adcͨ��1������
u16	ADC1_1[100];      //��adcͨ��2������
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
	
	ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC1��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//��ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��3,�������˳��ֵΪ1,����ʱ��Ϊ239.5����	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);  
	ADC_DMACmd(ADC1, ENABLE);//ʹ��ADC1ģ��DMA
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	/* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);	//����ָ����ADC1��У׼�Ĵ���
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);		//��ʼָ��ADC1��У׼״̬
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
}

void DMA1_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
  
	DMA_DeInit(DMA1_Channel1);//��λDMAͨ��1
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)0x4001244C; //����DMAͨ���������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC1_Value; //����DMAͨ���洢����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//ָ������ΪԴ��ַ  ������
	DMA_InitStructure.DMA_BufferSize = 200;//DMA��������С16bits
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//��ǰ����Ĵ�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//��ǰ�洢����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�����������ݿ��16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //����洢�����ݿ��16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//DMAͨ������ģʽλ���λ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;//DMAͨ�����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//��ֹDMAͨ���洢�����洢������
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//��ʼ��DMA1ͨ��1
	DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��DMA1ͨ��1
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

float ad0(void)     //adc1ͨ��1��������ƽ��
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

float ad1(void)   //adc1ͨ��2��������ƽ��
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

//��ȡadc�ĳ�ʼֵ
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

