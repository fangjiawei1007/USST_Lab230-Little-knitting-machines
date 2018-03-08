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
	
/**开始信号，用作内部变量**/
#define shangyazhen_motor_start			g_InteralMemory.KeepWord[795]
#define xiayazhen_motor_start			g_InteralMemory.KeepWord[796]
/**压针比较变量，用作内部变量**/
#define shangyazhen_pulse_cmp			g_InteralMemory.KeepWord[797]
#define xiayazhen_pulse_cmp         	g_InteralMemory.KeepWord[798]
/**压针电机脉冲量，用作内部变量**/
#define shangyazhen_motor_pulse			g_InteralMemory.KeepWord[799]
#define xiayazhen_motor_pulse       	g_InteralMemory.KeepWord[800]
/***压针实时发送脉冲量记录，用作内部变量***/
#define shangyazhen_counter				g_InteralMemory.KeepWord[801]
#define xiayazhen_counter				g_InteralMemory.KeepWord[802]

/***压针返回时发送脉冲量记录，用作内部变量***/
#define shangyazhen_back_counter		g_InteralMemory.KeepWord[803]
#define xiayazhen_back_counter			g_InteralMemory.KeepWord[804]
/***压针返回,回零时发送脉冲量记录，用作内部变量***/
#define shangyazhen_back_zero_counter	g_InteralMemory.KeepWord[805]
#define xiayazhen_back_zero_counter		g_InteralMemory.KeepWord[806]
/**压针返回开始信号，用作内部变量***/
#define shangyazhen_back_start			g_InteralMemory.KeepWord[807]
#define xiayazhen_back_start			g_InteralMemory.KeepWord[808]
/***压针容错***/
#define yazhen_err						g_InteralMemory.KeepWord[809]
/**压针返回比较变量，用作内部变量**/
#define shangyazhen_back_cmp			g_InteralMemory.KeepWord[810]
#define xiayazhen_back_cmp  			g_InteralMemory.KeepWord[811]

#define Yazhen_Beilv					g_InteralMemory.KeepWord[812]
/***压针零位信号1/0***/
#define yazhen_zero_signal				g_InteralMemory.KeepBit[93]	

#define Err3_Over 						g_InteralMemory.KeepWord[814]
#define Err3_Miss 						g_InteralMemory.KeepWord[815]
#define Err4_Over 						g_InteralMemory.KeepWord[816]
#define Err4_Miss 						g_InteralMemory.KeepWord[817]		
#define ERR_TIMES						10	

/***上盘/下盘减速比***/
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