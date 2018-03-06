#ifndef __YAZHEN_NORMAL_H__
#define __YAZHEN_NORMAL_H__

#define YAZHEN_NORMAL_EN
#ifdef YAZHEN_NORMAL_EN
int YAZHEN_ZERO_ERR = 0;
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


/***压针零位信号1/0***/
#define yazhen_zero_signal				g_InteralMemory.KeepBit[93]	

#endif

#endif