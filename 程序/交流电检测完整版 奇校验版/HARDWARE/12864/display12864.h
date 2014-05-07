#include "stm32f10x.h"

void IOInitOut(void);           //�����ж˿ڳ�ʼ��Ϊ���ģʽ�ĺ�����
void IOInitIn(void);	          //���������ų�ʼ��Ϊ��������ĺ�����
void WaitBusy(void);      	 //�ȴ�12864��æ״̬�����ĺ�����
void WriteCmd(u8 cmd);    //д�������
void WriteData(u8 data);  //д���ݺ�����
void InitDis(void);            //��ʼ�� 12864 ��Ҫ�õ��� STM 32 �����š�
void DisStr(u8 *s);    //��ʾ�ַ����ĺ�����
void DisInt(long int num);  //��ʾ���ͱ����ĺ���,�����ʾ16λ��������
void DisFloat(float fnum); //��ʾ��4λС���ĸ���������λ��������16λ��
void WriteData1(u8 data);  //д���ݺ���

#define  clr()  WriteCmd(0x01);

/*�����ǹ�궨λ��������һ������Ϊ�����꣬�ڶ���Ϊ�����꣬��ʼ������1��1�С�
ֻ����16����Ŀ��Ϊ��λ�ƶ���*/

void locate16(int row, int col);
void Display_key_init(void);            
void Display_key(void);


