#ifndef __YAZHEN_NORMAL_H__
#define __YAZHEN_NORMAL_H__

#define YAZHEN_NORMAL_EN
#ifdef YAZHEN_NORMAL_EN



#define NO_MOVE					65530



enum YAZHEN_NORMAL_CHECKOUT{
	NOT_CHANGED = 0,
	CHANGED	= 1
};

#define Y9								9
#define Y10								10
#define Y9_Bit							5
#define Y10_Bit							6	
	
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

#define Yazhen_Beilv					g_InteralMemory.KeepWord[812]
/***ѹ����λ�ź�1/0***/
#define yazhen_zero_signal				g_InteralMemory.KeepBit[93]	

#define Err3_Over 						g_InteralMemory.KeepWord[814]
#define Err3_Miss 						g_InteralMemory.KeepWord[815]
#define Err4_Over 						g_InteralMemory.KeepWord[816]
#define Err4_Miss 						g_InteralMemory.KeepWord[817]		
#define ERR_TIMES						10	

/***����/���̼��ٱ�***/
#define shangpan_jiansubi				g_InteralMemory.KeepWord[818]	
#define xiapan_jiansubi					g_InteralMemory.KeepWord[819]	
	
void __irq shangyazhen_zero_process(void);
void __irq pwrDownHandler(void);
void Yazhen_Normal_App(void);
unsigned int Yazhen_Normal_Checkout(void);
void Yazhen_Normal_Set(void);
void Yazhen_Normal_Get_Zero_Start(void);
void Yazhen_Normal_Init(void);
void Yazhen_Normal_Init_Once(void);
void Yazhen_Normal_Reset(void);
void Yazhen_Normal_Start(void);
void Alarm_Disp_Yazhen(unsigned int Port);
void Yazhen_Normal_Alarm(U8* err);
void Yazhen_Set_Dir(int Direction);

#endif

#endif