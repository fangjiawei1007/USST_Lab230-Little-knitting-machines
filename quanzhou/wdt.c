/////////////////////////WDT���� by ����ΰ 2017/4/1 ///////////////
#include "includes.h"

void wdt_init_reset()
{
	
	//��һ�������÷�Ƶ����Ԥ��Ƶ��
	
	rWTCON &= ~(0xff<<8);
	rWTCON |= (65<<8);		//1MHz
	
	rWTCON &= ~(0x3<<3);
	rWTCON |= (3<<3);		//1/128 MHz	T=128us
	
	// �ڶ����������жϺ͸�λ�źŵ�ʹ�ܻ��ֹ
	rWTCON &= ~(1<<2);				// disable wdt interrupt
	rWTCON |= (1<<0);				// enable wdt reset
	
	//���������ö�ʱʱ��
	// WDT��ʱ�������������ն�ʱʱ��Ϊ�����ֵ��ʱ������
	rWTDAT = reset_time_kw;					// ��ʱ3.84s
	rWTCNT = reset_time_kw;					// ��ʱ3.84s
	
	//������мĴ������ú�֮���ٴ򿪿��Ź�
	rWTCON |= (1<<5);

}


void wdt_feed_dog()
{
	rWTCNT = reset_time_kw;	
}