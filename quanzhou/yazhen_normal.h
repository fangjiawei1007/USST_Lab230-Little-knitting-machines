#ifndef __YAZHEN_NORMAL_H__
#define __YAZHEN_NORMAL_H__

#define YAZHEN_NORMAL_EN
#ifdef YAZHEN_NORMAL_EN
int YAZHEN_ZERO_ERR = 0;
/**��ʼ�źţ������ڲ�����**/
#define shangyazhen_motor_start			g_InteralMemory.KeepWord[795]
#define xiayazhen_motor_start			g_InteralMemory.KeepWord[796]
/**ѹ��Ƚϱ����������ڲ�����**/
#define shangyazhen_pulse_cmp			g_InteralMemory.KeepWord[797]
#define xiayazhen_pulse_cmp         	g_InteralMemory.KeepWord[798]
/**ѹ�����������������ڲ�����**/
#define shangyazhen_motor_pulse			g_InteralMemory.KeepWord[799]
#define xiayazhen_motor_pulse       	g_InteralMemory.KeepWord[800]
/***ѹ��ʵʱ������������¼�������ڲ�����***/
#define shangyazhen_counter				g_InteralMemory.KeepWord[801]
#define xiayazhen_counter				g_InteralMemory.KeepWord[802]

/***ѹ�뷵��ʱ������������¼�������ڲ�����***/
#define shangyazhen_back_counter		g_InteralMemory.KeepWord[803]
#define xiayazhen_back_counter			g_InteralMemory.KeepWord[804]
/***ѹ�뷵��,����ʱ������������¼�������ڲ�����***/
#define shangyazhen_back_zero_counter	g_InteralMemory.KeepWord[805]
#define xiayazhen_back_zero_counter		g_InteralMemory.KeepWord[806]
/**ѹ�뷵�ؿ�ʼ�źţ������ڲ�����***/
#define shangyazhen_back_start			g_InteralMemory.KeepWord[807]
#define xiayazhen_back_start			g_InteralMemory.KeepWord[808]
/***ѹ���ݴ�***/
#define yazhen_err						g_InteralMemory.KeepWord[809]
/**ѹ�뷵�رȽϱ����������ڲ�����**/
#define shangyazhen_back_cmp			g_InteralMemory.KeepWord[810]
#define xiayazhen_back_cmp  			g_InteralMemory.KeepWord[811]


/***ѹ����λ�ź�1/0***/
#define yazhen_zero_signal				g_InteralMemory.KeepBit[93]	

#endif

#endif