#ifndef __YAZHEN_NORMAL_H__
#define __YAZHEN_NORMAL_H__

#define YAZHEN_NORMAL_EN
#ifdef YAZHEN_NORMAL_EN

/***注意：以下注释修改过的宏，均为泉州不合理要求，再次打开注释的时候记得初始化！！***/

#define NO_MOVE					0xFFFF

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
/***压针返回,回零时发送脉冲量记录，仅在中断中使用，用作内部变量***/
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
/**压针减速倍率**/
#define Yazhen_Beilv					g_InteralMemory.KeepWord[812]
/***压针零位信号1/0***/
#define yazhen_zero_signal				g_InteralMemory.KeepBit[93]	



#define Err3_Over 						g_InteralMemory.KeepWord[814]
#define Err3_Miss 						g_InteralMemory.KeepWord[815]
#define Err4_Over 						g_InteralMemory.KeepWord[816]
#define Err4_Miss 						g_InteralMemory.KeepWord[817]		
//10
#define ERR_TIMES						3	

/***上盘/下盘减速比***/
//g_InteralMemory.KeepWord[818]
//g_InteralMemory.KeepWord[819]
#define shangpan_jiansubi				1	
#define xiapan_jiansubi					1
/**************************DEBUG 相关***************************************/	
/***调试开始信号，用作内部变量***/
#define shangyazhen_pos_start				(g_InteralMemory.KeepBit[101])
#define shangyazhen_neg_start				(g_InteralMemory.KeepBit[102])
#define xiayazhen_pos_start					(g_InteralMemory.KeepBit[103])
#define xiayazhen_neg_start        			(g_InteralMemory.KeepBit[104])
                                      
/**调试压针比较变量，用作内部变量**/
#define DBG_shangyazhen_pulse_cmp			g_InteralMemory.KeepWord[820]
#define DBG_xiayazhen_pulse_cmp         	g_InteralMemory.KeepWord[821]
/**调试压针电机脉冲量，用作内部变量**/
#define DBG_shangyazhen_motor_pulse			g_InteralMemory.KeepWord[822]
#define DBG_xiayazhen_motor_pulse       	g_InteralMemory.KeepWord[823]
/***调试压针实时发送脉冲量记录，用作内部变量***/
#define DBG_shangyazhen_counter				g_InteralMemory.KeepWord[824]
#define DBG_xiayazhen_counter				g_InteralMemory.KeepWord[825]
#define DBG_shangyazhen_counter_xiaotou		g_InteralMemory.KeepWord[834]
#define DBG_xiayazhen_counter_xiaotou		g_InteralMemory.KeepWord[835]

/***调试压针返回时发送脉冲量记录，用作内部变量***/
#define DBG_shangyazhen_back_counter				g_InteralMemory.KeepWord[826]
#define DBG_xiayazhen_back_counter					g_InteralMemory.KeepWord[827]
#define DBG_shangyazhen_back_counter_xiaotou		g_InteralMemory.KeepWord[836]
#define DBG_xiayazhen_back_counter_xiaotou			g_InteralMemory.KeepWord[837]


/***调试压针返回,回零时发送脉冲量记录，仅在中断中使用，用作内部变量***/
#define DBG_shangyazhen_back_zero_counter			g_InteralMemory.KeepWord[828]
#define DBG_xiayazhen_back_zero_counter				g_InteralMemory.KeepWord[829]
#define DBG_shangyazhen_back_zero_counter_xiaotou	g_InteralMemory.KeepWord[838]
#define DBG_xiayazhen_back_zero_counter_xiaotou		g_InteralMemory.KeepWord[839]

/*************压针监视****************************/
#define datou_shangyazhen_monitor 		(g_InteralMemory.KeepWord[840])
#define datou_xiayazhen_monitor	  		(g_InteralMemory.KeepWord[841])
#define xiaotou_shangyazhen_monitor		(g_InteralMemory.KeepWord[842])
#define xiaotou_xiayazhen_monitor 		(g_InteralMemory.KeepWord[843])

/**调试压针返回开始信号，用作内部变量***/
// #define DBG_shangyazhen_back_start			g_InteralMemory.KeepWord[830]
// #define DBG_xiayazhen_back_start			g_InteralMemory.KeepWord[831]


/**调试压针返回比较变量，用作内部变量**/
#define DBG_shangyazhen_back_cmp			g_InteralMemory.KeepWord[832]
#define DBG_xiayazhen_back_cmp  			g_InteralMemory.KeepWord[833]


/****调试回零按钮****/
#define datou_shangyazhen_zero_kb		(g_InteralMemory.KeepBit[105])
#define datou_xiayazhen_zero_kb			(g_InteralMemory.KeepBit[106])
#define xiaotou_shangyazhen_zero_kb		(g_InteralMemory.KeepBit[107])
#define xiaotou_xiayazhen_zero_kb		(g_InteralMemory.KeepBit[108]) 

#define shangyazhen_zero_confirm_kb		(g_InteralMemory.KeepBit[109])
#define xiayazhen_zero_confirm_kb		(g_InteralMemory.KeepBit[110])

#define yazhen_datou_debug_kb			(g_InteralMemory.KeepBit[96])
#define yazhen_xiaotou_debug_kb			(g_InteralMemory.KeepBit[97])

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

/***********************************************************/
void Yazhen_Debug_App(void);
void Yazhen_EXT_Button(unsigned int Yazhen_type);
void Yazhen_Debug_K_Set(void);
void Get_Monitor(void);
void Clear_Monitor(void);
void Yazhen_Zero_Confirm(void);
#endif

#endif