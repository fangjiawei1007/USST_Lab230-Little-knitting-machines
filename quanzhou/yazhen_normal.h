#ifndef __YAZHEN_NORMAL_H__
#define __YAZHEN_NORMAL_H__

#define YAZHEN_NORMAL_EN
#ifdef YAZHEN_NORMAL_EN

/***ע�⣺����ע���޸Ĺ��ĺ꣬��ΪȪ�ݲ�����Ҫ���ٴδ�ע�͵�ʱ��ǵó�ʼ������***/

#define NO_MOVE					0xFFFF

enum YAZHEN_NORMAL_CHECKOUT{
	NOT_CHANGED = 0,
	CHANGED	= 1
};

enum Direction{
	GO = 0,
	BACK
};

enum Direction_Fenceng{
	SAME_DIR = 0,
	DIFF_DIR
};

#define Y9								9
#define Y10								10
#define Y9_Bit							5
#define Y10_Bit							6	

/***���ɶ��ڲ�����***/
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

/***�ֲ���ڲ�����***/
/**��ʼ�źţ������ڲ�����**/
#define shangyazhen_fenceng_motor_start			g_InteralMemory.KeepWord[861]
#define xiayazhen_fenceng_motor_start			g_InteralMemory.KeepWord[862]
/**ѹ��Ƚϱ����������ڲ�����**/
#define shangyazhen_fenceng_pulse_cmp			g_InteralMemory.KeepWord[863]
#define xiayazhen_fenceng_pulse_cmp         	g_InteralMemory.KeepWord[864]
/**ѹ�����������������ڲ�����**/
#define shangyazhen_fenceng_motor_pulse			g_InteralMemory.KeepWord[865]
#define xiayazhen_fenceng_motor_pulse       	g_InteralMemory.KeepWord[866]
/***ѹ��ʵʱ������������¼�������ڲ�����***/
#define shangyazhen_fenceng_counter				g_InteralMemory.KeepWord[867]
#define xiayazhen_fenceng_counter				g_InteralMemory.KeepWord[868]




/***ѹ�뷵��ʱ������������¼�������ڲ�����***/
#define shangyazhen_back_counter		g_InteralMemory.KeepWord[803]
#define xiayazhen_back_counter			g_InteralMemory.KeepWord[804]
/***ѹ�뷵��,����ʱ������������¼�������ж���ʹ�ã������ڲ�����***/
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
/**ѹ����ٱ���**/
#define Yazhen_Beilv					g_InteralMemory.KeepWord[812]
/***ѹ����λ�ź�1/0***/
#define yazhen_zero_signal				g_InteralMemory.KeepBit[93]	

#define fenceng_dir_judge				g_InteralMemory.KeepWord[879]

#define Err3_Over 						g_InteralMemory.KeepWord[814]
#define Err3_Miss 						g_InteralMemory.KeepWord[815]
#define Err4_Over 						g_InteralMemory.KeepWord[816]
#define Err4_Miss 						g_InteralMemory.KeepWord[817]		
//10
#define ERR_TIMES						3	

/***����/���̼��ٱ�***/
//g_InteralMemory.KeepWord[818]
//g_InteralMemory.KeepWord[819]
#define shangpan_jiansubi				1	
#define xiapan_jiansubi					1
/**************************DEBUG ���***************************************/	
/***���Կ�ʼ�źţ������ڲ�����***/
#define shangyazhen_pos_start				(g_InteralMemory.KeepBit[101])
#define shangyazhen_neg_start				(g_InteralMemory.KeepBit[102])
#define xiayazhen_pos_start					(g_InteralMemory.KeepBit[103])
#define xiayazhen_neg_start        			(g_InteralMemory.KeepBit[104])
                                      
/**����ѹ��Ƚϱ����������ڲ�����**/
#define DBG_shangyazhen_pulse_cmp			g_InteralMemory.KeepWord[820]
#define DBG_xiayazhen_pulse_cmp         	g_InteralMemory.KeepWord[821]
/**����ѹ�����������������ڲ�����**/
#define DBG_shangyazhen_motor_pulse			g_InteralMemory.KeepWord[822]
#define DBG_xiayazhen_motor_pulse       	g_InteralMemory.KeepWord[823]
/***����ѹ��ʵʱ������������¼�������ڲ�����***/
#define DBG_shangyazhen_counter				g_InteralMemory.KeepWord[824]
#define DBG_xiayazhen_counter				g_InteralMemory.KeepWord[825]
#define DBG_shangyazhen_counter_xiaotou		g_InteralMemory.KeepWord[834]
#define DBG_xiayazhen_counter_xiaotou		g_InteralMemory.KeepWord[835]
#define DBG_shangyazhen_counter_fenceng		g_InteralMemory.KeepWord[875]
#define DBG_xiayazhen_counter_fenceng		g_InteralMemory.KeepWord[876]

/***����ѹ�뷵��ʱ������������¼�������ڲ�����***/
#define DBG_shangyazhen_back_counter				g_InteralMemory.KeepWord[826]
#define DBG_xiayazhen_back_counter					g_InteralMemory.KeepWord[827]
#define DBG_shangyazhen_back_counter_xiaotou		g_InteralMemory.KeepWord[836]
#define DBG_xiayazhen_back_counter_xiaotou			g_InteralMemory.KeepWord[837]
#define DBG_shangyazhen_back_counter_fenceng		g_InteralMemory.KeepWord[873]
#define DBG_xiayazhen_back_counter_fenceng			g_InteralMemory.KeepWord[874]

/***����ѹ�뷵��,����ʱ������������¼�������ж���ʹ�ã������ڲ�����***/
#define DBG_shangyazhen_back_zero_counter			g_InteralMemory.KeepWord[828]
#define DBG_xiayazhen_back_zero_counter				g_InteralMemory.KeepWord[829]
#define DBG_shangyazhen_back_zero_counter_xiaotou	g_InteralMemory.KeepWord[838]
#define DBG_xiayazhen_back_zero_counter_xiaotou		g_InteralMemory.KeepWord[839]
#define DBG_shangyazhen_back_zero_counter_fenceng	g_InteralMemory.KeepWord[871]
#define DBG_xiayazhen_back_zero_counter_fenceng		g_InteralMemory.KeepWord[872]


/*************ѹ�����****************************/
#define datou_shangyazhen_monitor 		(g_InteralMemory.KeepWord[840])
#define datou_xiayazhen_monitor	  		(g_InteralMemory.KeepWord[841])
#define xiaotou_shangyazhen_monitor		(g_InteralMemory.KeepWord[842])
#define xiaotou_xiayazhen_monitor 		(g_InteralMemory.KeepWord[843])
#define fenceng_shangyazhen_monitor		(g_InteralMemory.KeepWord[869])
#define fenceng_xiayazhen_monitor		(g_InteralMemory.KeepWord[870])


/**����ѹ�뷵�ؿ�ʼ�źţ������ڲ�����***/
// #define DBG_shangyazhen_back_start			g_InteralMemory.KeepWord[830]
// #define DBG_xiayazhen_back_start			g_InteralMemory.KeepWord[831]


/**����ѹ�뷵�رȽϱ����������ڲ�����**/
#define DBG_shangyazhen_back_cmp			g_InteralMemory.KeepWord[832]
#define DBG_xiayazhen_back_cmp  			g_InteralMemory.KeepWord[833]


/****���Ի��㰴ť****/
#define datou_shangyazhen_zero_kb		(g_InteralMemory.KeepBit[105])
#define datou_xiayazhen_zero_kb			(g_InteralMemory.KeepBit[106])
#define xiaotou_shangyazhen_zero_kb		(g_InteralMemory.KeepBit[107])
#define xiaotou_xiayazhen_zero_kb		(g_InteralMemory.KeepBit[108]) 
#define fenceng_shangyazhen_zero_kb		(g_InteralMemory.KeepBit[114])
#define fenceng_xiayazhen_zero_kb		(g_InteralMemory.KeepBit[115]) 

#define shangyazhen_zero_confirm_kb		(g_InteralMemory.KeepBit[109])
#define xiayazhen_zero_confirm_kb		(g_InteralMemory.KeepBit[110])

#define yazhen_datou_debug_kb			(g_InteralMemory.KeepBit[96])
#define yazhen_xiaotou_debug_kb			(g_InteralMemory.KeepBit[97])
#define yazhen_fenceng_debug_kb			(g_InteralMemory.KeepBit[112])

#define bianpingqi_yazhen_speed_set		(g_InteralMemory.KeepWord[844])

//ԭ���Ļ��㲹��,5.0.3ɾ���˴˹��ܣ����Ҳ�ʹ��ѹ���źţ����Ǵ���δɾ���Է���һ
#define err_feed						(g_InteralMemory.KeepWord[880])
/***���㲹�����ⲿ�ؼ������ж��з���ʹ��***/
#define Huiling_Buchang					(g_InteralMemory.KeepWord[881])
extern unsigned int mode;
extern unsigned int null;
extern unsigned int status_record1;
extern unsigned int status_record2;

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
void Yazhen_EXT_Button_Fengceng(unsigned int Yazhen_type);
void Yazhen_Debug_K_Set(void);
void Get_Monitor(void);
void Clear_Monitor(void);
void Yazhen_Zero_Confirm(void);
void Yazhen_Fenceng_Start(void);
void button_huchi(unsigned char* button0, unsigned char* button1, unsigned int* status_record,  unsigned int condition_flag);
void button_3_huchi(unsigned char* button0,unsigned char* button1, unsigned char* button2,unsigned int* status_record1,unsigned int* status_record2);

void Yazhen_Debug_App_Com(void);
void Yazhen_Run_App_Com(void);
void Yazhen_Zero_App_Com(void);

#endif

#endif