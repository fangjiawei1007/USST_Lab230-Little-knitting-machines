#include "includes.h"
/****************************************************************************************************************************
									Yazhen_Normal_FUN
1.ѹ�빦��Դ���ڴ˴�ʵ��
2.ѹ�빦�ܵĵ��Kֵ�Ƚ���__irq	encoder1_process(void)��ʵ��
3.С���̵�"ȷ��"��function.c->KeyScan()->default:��ʵ�֣�ͨ���˴���ȷ��������� Err_Miss�Լ�Err_Over

programmed by ����ΰ
****************************************************************************************************************************/
#ifdef YAZHEN_NORMAL_EN

#define Y2						2
#define X3						3
#define X4						4
#define X5						5
#define X11						11

#define Debug_Speed				30
#define Debug_CMP				13
#define Debug_Back_CMP			5
/***ע�⣺����ע���޸Ĺ��ĺ꣬��ΪȪ�ݲ�����Ҫ���ٴδ�ע�͵�ʱ��ǵó�ʼ������***/
//(g_InteralMemory.KeepBit[90])
//((~g_InteralMemory.KeepBit[90])&0x1)
#define Dir_Positive				1
#define Dir_Negative				0

#define yazhen_move_status				(g_InteralMemory.KeepBit[91])
#define yazhen_fenceng_status			(g_InteralMemory.KeepBit[111])
#define yazhen_back_status				(g_InteralMemory.KeepBit[92])
			
#define shangyazhen_datou				(g_InteralMemory.KeepWord[790])
#define shangyazhen_xiaotou				(g_InteralMemory.KeepWord[791])
#define xiayazhen_datou					(g_InteralMemory.KeepWord[792])
#define xiayazhen_xiaotou				(g_InteralMemory.KeepWord[793])

#define shangyazhen_fenceng				(g_InteralMemory.KeepWord[877])
#define xiayazhen_fenceng 				(g_InteralMemory.KeepWord[878])

#define yazhen_alarm_level				(g_InteralMemory.KeepWord[813])

/*******************************ѹ�����*********************************************/
#define datou_shangyazhen_monitor_clear_b		(g_InteralMemory.Bit[80])
#define datou_xiayazhen_monitor_clear_b			(g_InteralMemory.Bit[81])
#define xiaotou_shangyazhen_monitor_clear_b		(g_InteralMemory.Bit[82])
#define xiaotou_xiayazhen_monitor_clear_b		(g_InteralMemory.Bit[83])
#define fenceng_shangyazhen_monitor_clear_b		(g_InteralMemory.Bit[84])
#define fenceng_xiayazhen_monitor_clear_b		(g_InteralMemory.Bit[85])




#define yazhen_datou_choose_kb					(g_InteralMemory.KeepBit[94])
#define yazhen_xiaotou_choose_kb 				(g_InteralMemory.KeepBit[95])
#define yazhen_fenceng_choose_kb 				(g_InteralMemory.KeepBit[113])

#define datou_shangyazhen_monitor_w 			(g_InteralMemory.Word[70])
#define datou_xiayazhen_monitor_w	  			(g_InteralMemory.Word[71])
#define xiaotou_shangyazhen_monitor_w			(g_InteralMemory.Word[72])
#define xiaotou_xiayazhen_monitor_w 			(g_InteralMemory.Word[73])
#define fenceng_shangyazhen_monitor_w			(g_InteralMemory.Word[74])
#define fenceng_xiayazhen_monitor_w 			(g_InteralMemory.Word[75])
        
//int tiaoxiankaiguan_kb = -1;			//��ֹ��������߰汾��ͻ����ʹ�õ��߰汾��������ȥ�������߲�ȥ��Ԥ�����������Ѿ����
int yazhen_datou_choose = 0;
int YAZHEN_ZERO_ERR = 0;

unsigned int X3_SIG;
unsigned int X4_SIG;
unsigned int mode = 0;
unsigned int null = 0;

unsigned int status_record1,status_record2;

unsigned int yazhen_debug_start_com,yazhen_run_start_com;//����ѹ��ͨѶ��

enum EXT_Button{
	EXT_ON = 0,
	EXT_OFF
};
enum Yazhen_Choose{
	UP_YAZHEN = 0,
	DOWN_YAZHEN
};

/*************************************************
Function(��������): __irq shangyazhen_zero_process(void)
Description(�������ܡ����ܵȵ�����): ��ѹ����λ������(X3)�жϷ���������ڼ�¼�Ƿ񳬹����
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/	
void __irq shangyazhen_zero_process(void)	
{
	/**�������δʹ��**/
	X3_SIG++;
	//Beep(1);
	
	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);
}

/*************************************************
Function(��������): __irq pwrDownHandler(void)
Description(�������ܡ����ܵȵ�����): ��ѹ����λ������(X4)�жϷ���������ڼ�¼�Ƿ񳬹����
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/	
void __irq pwrDownHandler(void)	//X4-X7���жϷ������󶨵������֣�����Ŀǰû���õ�
{

	if(!( rEINTMASK >>4  & 0x1 ) && (rEINTPEND & (1<<4)))//ȡ�����α�־+������Ч
	{
		/**�������δʹ��**/
		X4_SIG++;
		//Beep(1);
		{
			g_InteralMemory.Word[305] = X4_SIG;
		}
		rEINTPEND=(1<<4);		
	}
	else if ( !( rEINTMASK >>5  & 0x1 )  && (rEINTPEND & (1<<5)) )//ȡ�����α�־+������Ч	
	{
		
		rEINTPEND=(1<<5);		
	}
	else if ( !( rEINTMASK >>6  & 0x1 )  && (rEINTPEND & (1<<6)) )//ȡ�����α�־+������Ч	
	{
		if((rGPFDAT >> 6) & 0x1)
		{
			InPulseCount[6]++;  //InPulseCount[n] ֻ������7�� InPulseCount[0]-----InPulseCount[6]
		}

		rEINTPEND=(1<<6);		
	}
	else if ( !( rEINTMASK >>7  & 0x1 )  && (rEINTPEND & (1<<7)) )//ȡ�����α�־+������Ч	
	{
		if((rGPFDAT >> 7) & 0x1)
		{
			
		}
		rEINTPEND=(1<<7);		
	}
	ClearPending(BIT_EINT4_7);
}

/*************************************************
Function(��������): void Yazhen_Normal_App(void)
Description(�������ܡ����ܵȵ�����): ѹ��������
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): main();KeyBoardScan();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/	
void Yazhen_Normal_App(void){
	int stage_cur = -1;
	unsigned int checkout_yazhen = 0;
	stage_cur = getStage(current_stage,CURRENT);
	
	if((yazhen_datou_debug_kb == 1) || (yazhen_xiaotou_debug_kb == 1) || (yazhen_fenceng_debug_kb == 1)){
		if(stage_cur == datouduan || stage_cur == xiaotouduan){
			yazhen_run_start_com = 0;//ѹ����ư�����ͨѶ��ʼ��־λ
			Yazhen_Debug_App_Com();
		}
	}
	else{
		yazhen_debug_start_com = 0;//ѹ����ư����ͨѶ��ʼ��־λ
		if(yazhen_run_start_com == 0){
			yazhen_run_start_com = 1;
			Yazhen_Run_App_Com();
		}
		checkout_yazhen = Yazhen_Normal_Checkout();
		if(checkout_yazhen == CHANGED){
			Yazhen_Normal_Set();
		}
		/**���ɶ�**/
		if(stage_cur == guoduduan && yazhen_move_status == 0){
			yazhen_move_status = 1;
			Yazhen_Normal_Start();
		}
		else if(stage_cur != guoduduan){
			yazhen_move_status = 0;
		}
		/**�ֲ��**/
		if(stage_cur == fencenduan && yazhen_fenceng_status == 0){
			yazhen_fenceng_status = 1;
			Yazhen_Fenceng_Start();
		}
		else if(stage_cur != fencenduan){
			yazhen_fenceng_status = 0;
		}
		/***�ü���***/
		if(stage_cur == caijianduan && yazhen_back_status == 0){
			yazhen_back_status = 1;
			Yazhen_Normal_Get_Zero_Start();	
		}
		else if(stage_cur != caijianduan){
			yazhen_back_status = 0;
		}
		//�ϵ��ǲü���
		if(stage_cur == caijianduan || stage_cur == datouduan){
			Yazhen_Set_Dir(BACK);
		}
		else if(stage_cur == fencenduan){
			if(fenceng_dir_judge == SAME_DIR){
				Yazhen_Set_Dir(GO);
			}
			else if(fenceng_dir_judge == DIFF_DIR){
				Yazhen_Set_Dir(BACK);
			}
		}
		else
			Yazhen_Set_Dir(GO);
	}
}

/*************************************************
Function(��������): void Yazhen_Normal_Start(void)
Description(�������ܡ����ܵȵ�����): ѹ�������˶���ʼ������ѹ�뷽��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): Yazhen_Normal_App();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/	
void Yazhen_Normal_Start(void){	
	Yazhen_Set_Dir(GO);
	shangyazhen_motor_start = 1;
	xiayazhen_motor_start = 1;
}

void Yazhen_Fenceng_Start(void){
	if(fenceng_dir_judge == SAME_DIR){
		Yazhen_Set_Dir(GO);
	}
	else if(fenceng_dir_judge == DIFF_DIR){
		Yazhen_Set_Dir(BACK);
	}
	
	if(shangyazhen_fenceng != shangyazhen_xiaotou)
		shangyazhen_fenceng_motor_start = 1;
	else 
		shangyazhen_fenceng_motor_start = 0;
	
	if(xiayazhen_fenceng != xiayazhen_xiaotou)
		xiayazhen_fenceng_motor_start 	= 1;
	else
		xiayazhen_fenceng_motor_start 	= 0;
}
/*************************************************
Function(��������): void Yazhen_Normal_Checkout(void)
Description(�������ܡ����ܵȵ�����): �ж��ⲿ��ѹ��Kֵ�йصı����Ƿ񱻸ı䣬������ı䣬����Ҫ���½��м���
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): Yazhen_Normal_App();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: CHANGED/NOT_CHANGED;CHANGED����Ҫ���½�������
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/	
unsigned int Yazhen_Normal_Checkout(void){
	static unsigned int first_in = 0;		//��һ�ν���
	
	static unsigned int shangyazhen_datou_pre = 0;
	static unsigned int xiayazhen_datou_pre = 0;
	static unsigned int shangyazhen_datou_cur = 0;
	static unsigned int xiayazhen_datou_cur = 0;
	
	static unsigned int shangyazhen_xiaotou_pre = 0;
	static unsigned int xiayazhen_xiaotou_pre = 0;
	static unsigned int shangyazhen_xiaotou_cur = 0;
	static unsigned int xiayazhen_xiaotou_cur = 0;
	
	static unsigned int shangyazhen_fenceng_pre = 0;
	static unsigned int xiayazhen_fenceng_pre = 0;
	static unsigned int shangyazhen_fenceng_cur = 0;
	static unsigned int xiayazhen_fenceng_cur = 0; 
	
	static unsigned int guoduquanshu_cur = 0;
	static unsigned int guoduquanshu_pre = 0;
	
	static unsigned int bianpingqi_huanchongquan_num_pre = 0;
	static unsigned int bianpingqi_huanchongquan_num_cur = 0;
		
	static unsigned int shangpan_jiansubi_cur = 0;
	static unsigned int xiapan_jiansubi_cur = 0;
	static unsigned int shangpan_jiansubi_pre = 0;
	static unsigned int xiapan_jiansubi_pre = 0;
	
	static unsigned int Yazhen_Beilv_cur = 0;
	static unsigned int Yazhen_Beilv_pre = 0;
	
	if(first_in == 0){
		shangyazhen_datou_pre = shangyazhen_datou;
		xiayazhen_datou_pre = xiayazhen_datou;
		shangyazhen_datou_cur = shangyazhen_datou;
		xiayazhen_datou_cur = xiayazhen_datou;
		                       
		shangyazhen_xiaotou_pre = shangyazhen_xiaotou;
		xiayazhen_xiaotou_pre = xiayazhen_xiaotou;
		shangyazhen_xiaotou_cur = shangyazhen_xiaotou;
		xiayazhen_xiaotou_cur = xiayazhen_xiaotou;
		
		shangyazhen_fenceng_pre = shangyazhen_fenceng;
		xiayazhen_fenceng_pre = xiayazhen_fenceng;
		shangyazhen_fenceng_cur = shangyazhen_fenceng;
		xiayazhen_fenceng_cur = xiayazhen_fenceng;
		
		bianpingqi_huanchongquan_num_pre = bianpingqi_huanchongquan_num;
		bianpingqi_huanchongquan_num_cur = bianpingqi_huanchongquan_num;
		
		guoduquanshu_cur = middlequanshu;
		guoduquanshu_pre = middlequanshu;
		
		shangpan_jiansubi_cur = shangpan_jiansubi;
		xiapan_jiansubi_cur = xiapan_jiansubi;
		shangpan_jiansubi_pre = shangpan_jiansubi;
		xiapan_jiansubi_pre = xiapan_jiansubi;
	
		Yazhen_Beilv_pre = Yazhen_Beilv;
		Yazhen_Beilv_cur = Yazhen_Beilv;
		
		first_in = 1;
		return CHANGED;
	}
	else{
		shangyazhen_datou_cur 	 = shangyazhen_datou;
		xiayazhen_datou_cur		 = xiayazhen_datou;
		shangyazhen_xiaotou_cur  = shangyazhen_xiaotou;
		xiayazhen_xiaotou_cur	 = xiayazhen_xiaotou;
		guoduquanshu_cur 		 = middlequanshu;
		shangpan_jiansubi_cur 	 = shangpan_jiansubi;
		xiapan_jiansubi_cur 	 = xiapan_jiansubi;
		Yazhen_Beilv_cur		 = Yazhen_Beilv;
		
		shangyazhen_fenceng_cur  = shangyazhen_fenceng;
		xiayazhen_fenceng_cur 	 = xiayazhen_fenceng;
		bianpingqi_huanchongquan_num_cur = bianpingqi_huanchongquan_num;
		/**�ⲿ��һ��������Ҫ���ı䣬��ı����е�ֵ���������������ⲿȫ�ֱ���**/
		if(( shangyazhen_datou_pre == shangyazhen_datou_cur ) && ( xiayazhen_datou_pre == xiayazhen_datou_cur)
		  && (shangyazhen_xiaotou_pre == shangyazhen_xiaotou_cur) && (xiayazhen_xiaotou_pre == xiayazhen_xiaotou_cur)
		  && (guoduquanshu_pre == guoduquanshu_cur)
		  && (shangpan_jiansubi_pre == shangpan_jiansubi_cur) && (xiapan_jiansubi_pre == xiapan_jiansubi_cur)
		  && (Yazhen_Beilv_pre == Yazhen_Beilv_cur)
		  && (shangyazhen_fenceng_pre == shangyazhen_fenceng_cur) && (xiayazhen_fenceng_pre == xiayazhen_fenceng_cur) 
		  && (bianpingqi_huanchongquan_num_pre == bianpingqi_huanchongquan_num_cur)
		  )
			return NOT_CHANGED;
		else{
			shangyazhen_datou_pre 	= shangyazhen_datou_cur;
			xiayazhen_datou_pre 	= xiayazhen_datou_cur;
			shangyazhen_xiaotou_pre = shangyazhen_xiaotou_cur;
			xiayazhen_xiaotou_pre 	= xiayazhen_xiaotou_cur;
			guoduquanshu_pre 		= guoduquanshu_cur;
			shangpan_jiansubi_pre 	= shangpan_jiansubi_cur;
			xiapan_jiansubi_pre 	= xiapan_jiansubi_cur;
			Yazhen_Beilv_pre		= Yazhen_Beilv_cur;
			shangyazhen_fenceng_pre = shangyazhen_fenceng_cur;
			xiayazhen_fenceng_pre 	= xiayazhen_fenceng_cur;
			bianpingqi_huanchongquan_num_pre = bianpingqi_huanchongquan_num_cur;
	
			return CHANGED;
		}
	}
	return NOT_CHANGED;
}
/*************************************************
Function(��������): void Yazhen_Normal_Set(void)
Description(�������ܡ����ܵȵ�����): ���ⲿ�������ı�֮�󣬷���CHANGED��ͨ���ú�����������ֵ(ȫ�ֱ���)
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): Yazhen_Normal_App();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):ȫ�ֱ�����shangyazhen_pulse_cmp;xiayazhen_pulse_cmp;shangyazhen_back_cmp;xiayazhen_back_cmp
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/	
void Yazhen_Normal_Set(void){
	int yazhen_total_pulse=0;
	int yazhen_total_pulse_fenceng = 0;
	
	if((shangyazhen_datou >= shangyazhen_xiaotou) && (xiayazhen_datou >= xiayazhen_xiaotou)){
		Set_Y_Value(Y2,Dir_Positive);//1
		shangyazhen_motor_pulse = (shangyazhen_datou - shangyazhen_xiaotou)*shangpan_jiansubi;
		xiayazhen_motor_pulse = (xiayazhen_datou - xiayazhen_xiaotou)*xiapan_jiansubi;
		yazhen_total_pulse = middlequanshu*encoder1_cal_factor;
		if(shangyazhen_motor_pulse > 0){
			shangyazhen_pulse_cmp = yazhen_total_pulse/shangyazhen_motor_pulse;
			if (shangyazhen_pulse_cmp < 2)
				shangyazhen_pulse_cmp = 2;//��֤�½���
		}
		else
			shangyazhen_pulse_cmp = NO_MOVE;
		
		if(xiayazhen_motor_pulse > 0){
			xiayazhen_pulse_cmp = yazhen_total_pulse/xiayazhen_motor_pulse;
			if(xiayazhen_pulse_cmp < 2)
				xiayazhen_pulse_cmp = 2;//��֤�½���
		}
		else
			xiayazhen_pulse_cmp = NO_MOVE;

		if (yazhen_total_pulse == 0){//�޹��ɶ��򲻶�����k->�����
			shangyazhen_pulse_cmp = NO_MOVE;
			xiayazhen_pulse_cmp = NO_MOVE;
		}
	/**********************�ֲ������****************************/
		if(shangyazhen_xiaotou >= shangyazhen_fenceng && xiayazhen_xiaotou >= xiayazhen_fenceng){
			fenceng_dir_judge = SAME_DIR;
			shangyazhen_fenceng_motor_pulse = (shangyazhen_xiaotou - shangyazhen_fenceng);
			xiayazhen_fenceng_motor_pulse	= (xiayazhen_xiaotou - xiayazhen_fenceng);
			yazhen_total_pulse_fenceng 		= (bianpingqi_huanchongquan_num)*encoder1_cal_factor;

			if(shangyazhen_fenceng_motor_pulse > 0){
				shangyazhen_fenceng_pulse_cmp = yazhen_total_pulse_fenceng/shangyazhen_fenceng_motor_pulse;
				if (shangyazhen_fenceng_pulse_cmp < 2)
					shangyazhen_fenceng_pulse_cmp = 2;//��֤�½���
			}
			else
				shangyazhen_fenceng_pulse_cmp = NO_MOVE;
			
			if(xiayazhen_fenceng_motor_pulse > 0){
				xiayazhen_fenceng_pulse_cmp = yazhen_total_pulse_fenceng/xiayazhen_fenceng_motor_pulse;
				if (xiayazhen_fenceng_pulse_cmp < 2)
					xiayazhen_fenceng_pulse_cmp = 2;//��֤�½���
			}
			else
				xiayazhen_fenceng_pulse_cmp = NO_MOVE;
		}
		else if(shangyazhen_xiaotou <= shangyazhen_fenceng && xiayazhen_xiaotou <= xiayazhen_fenceng){
			fenceng_dir_judge = DIFF_DIR;
			shangyazhen_fenceng_motor_pulse = (shangyazhen_fenceng - shangyazhen_xiaotou);
			xiayazhen_fenceng_motor_pulse	= (xiayazhen_fenceng - xiayazhen_xiaotou);
			yazhen_total_pulse_fenceng 		= (bianpingqi_huanchongquan_num)*encoder1_cal_factor;

			if(shangyazhen_fenceng_motor_pulse > 0){
				shangyazhen_fenceng_pulse_cmp = yazhen_total_pulse_fenceng/shangyazhen_fenceng_motor_pulse;
				if (shangyazhen_fenceng_pulse_cmp < 2)
					shangyazhen_fenceng_pulse_cmp = 2;//��֤�½���
			}
			else
				shangyazhen_fenceng_pulse_cmp = NO_MOVE;
			
			if(xiayazhen_fenceng_motor_pulse > 0){
				xiayazhen_fenceng_pulse_cmp = yazhen_total_pulse_fenceng/xiayazhen_fenceng_motor_pulse;
				if (xiayazhen_fenceng_pulse_cmp < 2)
					xiayazhen_fenceng_pulse_cmp = 2;//��֤�½���
			}
			else
				xiayazhen_fenceng_pulse_cmp = NO_MOVE;
		}
		else{
			shangyazhen_fenceng_pulse_cmp = NO_MOVE;
			xiayazhen_fenceng_pulse_cmp = NO_MOVE;
			shangyazhen_fenceng_counter = 0;
			xiayazhen_fenceng_counter = 0;
		}	
	}
	else if((shangyazhen_datou <= shangyazhen_xiaotou) && (xiayazhen_datou <= xiayazhen_xiaotou)){
		Set_Y_Value(Y2,Dir_Negative);
		shangyazhen_motor_pulse = (shangyazhen_xiaotou - shangyazhen_datou)*shangpan_jiansubi;
		xiayazhen_motor_pulse = (xiayazhen_xiaotou - xiayazhen_datou)*xiapan_jiansubi;
		yazhen_total_pulse = middlequanshu*encoder1_cal_factor;

		if(shangyazhen_motor_pulse > 0){
			shangyazhen_pulse_cmp = yazhen_total_pulse/shangyazhen_motor_pulse;
			if (shangyazhen_pulse_cmp < 2)
				shangyazhen_pulse_cmp = 2;//��֤�½���
		}
		else
			shangyazhen_pulse_cmp = NO_MOVE;
		
		if(xiayazhen_motor_pulse > 0){
			xiayazhen_pulse_cmp = yazhen_total_pulse/xiayazhen_motor_pulse;
			if (xiayazhen_pulse_cmp < 2)
				xiayazhen_pulse_cmp = 2;//��֤�½���
		}
		else
			xiayazhen_pulse_cmp = NO_MOVE;
		
		if (yazhen_total_pulse == 0){//�޹��ɶ��򲻶�����k->�����
			shangyazhen_pulse_cmp = NO_MOVE;
			xiayazhen_pulse_cmp = NO_MOVE;
		}
		
	/**********************�ֲ������****************************/
		if(shangyazhen_xiaotou >= shangyazhen_fenceng && xiayazhen_xiaotou >= xiayazhen_fenceng){
			fenceng_dir_judge = DIFF_DIR;
			shangyazhen_fenceng_motor_pulse = (shangyazhen_xiaotou - shangyazhen_fenceng);
			xiayazhen_fenceng_motor_pulse	= (xiayazhen_xiaotou - xiayazhen_fenceng);
			yazhen_total_pulse_fenceng 		= (bianpingqi_huanchongquan_num)*encoder1_cal_factor;

			if(shangyazhen_fenceng_motor_pulse > 0){
				shangyazhen_fenceng_pulse_cmp = yazhen_total_pulse_fenceng/shangyazhen_fenceng_motor_pulse;
				if (shangyazhen_fenceng_pulse_cmp < 2)
					shangyazhen_fenceng_pulse_cmp = 2;//��֤�½���
			}
			else
				shangyazhen_fenceng_pulse_cmp = NO_MOVE;
			
			if(xiayazhen_fenceng_motor_pulse > 0){
				xiayazhen_fenceng_pulse_cmp = yazhen_total_pulse_fenceng/xiayazhen_fenceng_motor_pulse;
				if (xiayazhen_fenceng_pulse_cmp < 2)
					xiayazhen_fenceng_pulse_cmp = 2;//��֤�½���
			}
			else
				xiayazhen_fenceng_pulse_cmp = NO_MOVE;
		}
		else if(shangyazhen_xiaotou <= shangyazhen_fenceng && xiayazhen_xiaotou <= xiayazhen_fenceng){
			fenceng_dir_judge = SAME_DIR;
			shangyazhen_fenceng_motor_pulse = (shangyazhen_fenceng - shangyazhen_xiaotou);
			xiayazhen_fenceng_motor_pulse	= (xiayazhen_fenceng - xiayazhen_xiaotou);
			yazhen_total_pulse_fenceng 		= (bianpingqi_huanchongquan_num)*encoder1_cal_factor;

			if(shangyazhen_fenceng_motor_pulse > 0){
				shangyazhen_fenceng_pulse_cmp = yazhen_total_pulse_fenceng/shangyazhen_fenceng_motor_pulse;
				if (shangyazhen_fenceng_pulse_cmp < 2)
					shangyazhen_fenceng_pulse_cmp = 2;//��֤�½���
			}
			else
				shangyazhen_fenceng_pulse_cmp = NO_MOVE;
			
			if(xiayazhen_fenceng_motor_pulse > 0){
				xiayazhen_fenceng_pulse_cmp = yazhen_total_pulse_fenceng/xiayazhen_fenceng_motor_pulse;
				if (xiayazhen_fenceng_pulse_cmp < 2)
					xiayazhen_fenceng_pulse_cmp = 2;//��֤�½���
			}
			else
				xiayazhen_fenceng_pulse_cmp = NO_MOVE;
		}
		else{
			shangyazhen_fenceng_pulse_cmp = NO_MOVE;
			xiayazhen_fenceng_pulse_cmp = NO_MOVE;
			shangyazhen_fenceng_counter = 0;
			xiayazhen_fenceng_counter = 0;
		}	
	}
	else{//�����෴֮�󣬲�����
		shangyazhen_pulse_cmp = NO_MOVE;
		xiayazhen_pulse_cmp = NO_MOVE;
		shangyazhen_counter = 0;
		xiayazhen_counter = 0;
	}

	/***���÷���K***/
	shangyazhen_back_cmp = Yazhen_Beilv;
	xiayazhen_back_cmp   = Yazhen_Beilv;
	

	//DEBUG
	{
		g_InteralMemory.Word[300] = shangyazhen_pulse_cmp;
		g_InteralMemory.Word[301] = xiayazhen_pulse_cmp;
	}
	//DEBUG
	{
		g_InteralMemory.Word[302] = shangyazhen_back_cmp;
		g_InteralMemory.Word[303] = xiayazhen_back_cmp;
	}		
	
}

/*************************************************
Function(��������): void Yazhen_Normal_Get_Zero_Start(void)
Description(�������ܡ����ܵȵ�����): ѹ�������˶���ʼ������ѹ�뷽�򣬵�ѹ�뷵��counter>5�������ʱ��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): Yazhen_Normal_App();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/	
void Yazhen_Normal_Get_Zero_Start(void){
	Yazhen_Set_Dir(BACK);
	if(shangyazhen_back_counter > 5*shangpan_jiansubi)//����5��Ϊ��YAZHEN_ZERO_ERR = -4
		shangyazhen_back_start = 1;
	if(xiayazhen_back_counter > 5*xiapan_jiansubi)
		xiayazhen_back_start = 1;
	
}

/*************************************************
Function(��������): void Yazhen_Set_Dir(int Direction)
Description(�������ܡ����ܵȵ�����): ����ѹ�뷽��ʵ�ֺ���
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): Yazhen_Normal_App();Yazhen_Normal_Set();Yazhen_Normal_Get_Zero_Start();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/	
void Yazhen_Set_Dir(int Direction){
	switch(Direction){
		case GO:
			if((shangyazhen_datou >= shangyazhen_xiaotou) && (xiayazhen_datou >= xiayazhen_xiaotou))
				Set_Y_Value(Y2,Dir_Positive);//0
			else if ((shangyazhen_datou <= shangyazhen_xiaotou) && (xiayazhen_datou <= xiayazhen_xiaotou))
				Set_Y_Value(Y2,Dir_Negative);
			break;
		case BACK:
			if((shangyazhen_datou >= shangyazhen_xiaotou) && (xiayazhen_datou >= xiayazhen_xiaotou))
				Set_Y_Value(Y2,Dir_Negative);//0
			else if ((shangyazhen_datou <= shangyazhen_xiaotou) && (xiayazhen_datou <= xiayazhen_xiaotou))
				Set_Y_Value(Y2,Dir_Positive);
			break;
		default:
			break;
	}
}

/*************************************************
Function(��������): void Yazhen_Normal_Init(int Direction)
Description(�������ܡ����ܵȵ�����): ѹ���ʼ������������Y2�ڣ����ж�
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): App_Init(void);

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/	
void Yazhen_Normal_Init(void){
	int tmp = 0;
	int stage_cur;
	tmp = rGPBCON & (~(0x3<< 4));
	rGPBCON = tmp |(0x1<<4);	

	encoder3_fun(1);
	encoder4_fun(1);
	
	stage_cur = getStage(current_stage,CURRENT);
	if(stage_cur == guoduduan && (shangyazhen_datou >= shangyazhen_xiaotou) && (xiayazhen_datou >= xiayazhen_xiaotou))
		Set_Y_Value(Y2,Dir_Positive);
	else if(stage_cur == guoduduan && (shangyazhen_datou <= shangyazhen_xiaotou) && (xiayazhen_datou <= xiayazhen_xiaotou))
		Set_Y_Value(Y2,Dir_Negative);
	else if(stage_cur != guoduduan && (shangyazhen_datou >= shangyazhen_xiaotou) && (xiayazhen_datou >= xiayazhen_xiaotou))
		Set_Y_Value(Y2,Dir_Negative);
	else if(stage_cur != guoduduan && (shangyazhen_datou <= shangyazhen_xiaotou) && (xiayazhen_datou <= xiayazhen_xiaotou))
		Set_Y_Value(Y2,Dir_Positive);
	
	datou_shangyazhen_monitor_clear_b	= 0;
	datou_xiayazhen_monitor_clear_b		= 0;
	xiaotou_shangyazhen_monitor_clear_b	= 0;
	xiaotou_xiayazhen_monitor_clear_b	= 0;
	fenceng_shangyazhen_monitor_clear_b	= 0;
	fenceng_xiayazhen_monitor_clear_b	= 0;
	
	datou_shangyazhen_zero_kb	= 0;
	datou_xiayazhen_zero_kb		= 0;
	xiaotou_shangyazhen_zero_kb	= 0;
	xiaotou_xiayazhen_zero_kb	= 0;
	
	shangyazhen_zero_confirm_kb	= 0;
	xiayazhen_zero_confirm_kb	= 0;
	
	// yazhen_datou_choose_kb		= 0;
	// yazhen_xiaotou_choose_kb 	= 0;
	// yazhen_datou_debug_kb		= 0;
	// yazhen_xiaotou_debug_kb		= 0;
	
	bianpingqi_yazhen_speed_set = 300;
} 

/*************************************************
Function(��������): void Yazhen_Normal_Init_Once(int Direction)
Description(�������ܡ����ܵȵ�����): ѹ��kw/kb����֮���һ�γ�ʼ��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): menu_init(void);

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/ 
void Yazhen_Normal_Init_Once(void){
	//Dir_Positive = 0;
	
	yazhen_move_status  = 0;
	yazhen_fenceng_status = 0;
	yazhen_back_status  = 0;	
	shangyazhen_datou	= 0;		
	shangyazhen_xiaotou	= 0;	
	xiayazhen_datou		= 0;	
	xiayazhen_xiaotou	= 0;	
	shangyazhen_motor_start = 0;
	xiayazhen_motor_start	= 0;
	shangyazhen_pulse_cmp  = 0;
	xiayazhen_pulse_cmp  = 0;
	shangyazhen_motor_pulse	= 0;
	xiayazhen_motor_pulse   = 0;
	shangyazhen_counter	= 0;
	xiayazhen_counter= 0;
	shangyazhen_back_counter= 0;
	xiayazhen_back_counter	= 0;
	shangyazhen_back_zero_counter= 0;
	xiayazhen_back_zero_counter	= 0;
	shangyazhen_back_start	= 0;
	xiayazhen_back_start	= 0;
	yazhen_err				= 0;
	yazhen_zero_signal		= 0;
	Yazhen_Beilv            = 1;
	Err3_Miss				= 0;
	Err3_Over				= 0;
	Err4_Miss				= 0;
	Err4_Over				= 0;
	yazhen_alarm_level      = 0;
	
	jiajiansujiangemaichong_kw = 10000;
	yazhen_alarm_level = level_3;
	
	datou_shangyazhen_monitor     = 0;
	datou_xiayazhen_monitor	  	= 0;
	xiaotou_shangyazhen_monitor	= 0;
	xiaotou_xiayazhen_monitor 		= 0;
	
	datou_shangyazhen_monitor_clear_b	= 0;
	datou_xiayazhen_monitor_clear_b		= 0;
	xiaotou_shangyazhen_monitor_clear_b	= 0;
	xiaotou_xiayazhen_monitor_clear_b	= 0;
	
	datou_shangyazhen_zero_kb	= 0;
	datou_xiayazhen_zero_kb		= 0;
	xiaotou_shangyazhen_zero_kb	= 0;
	xiaotou_xiayazhen_zero_kb	= 0;
	
	shangyazhen_zero_confirm_kb	= 0;
	xiayazhen_zero_confirm_kb	= 0;
	
	yazhen_datou_choose_kb		= 0;
	yazhen_xiaotou_choose_kb 	= 0;
	yazhen_datou_debug_kb		= 0;
	yazhen_xiaotou_debug_kb		= 0;
	
	DBG_shangyazhen_back_cmp    = 0;
	DBG_xiayazhen_back_cmp  	= 0;
	
	DBG_shangyazhen_back_zero_counter		  	= 0;
	DBG_xiayazhen_back_zero_counter				= 0;
	DBG_shangyazhen_back_zero_counter_xiaotou	= 0;
	DBG_xiayazhen_back_zero_counter_xiaotou		= 0;
	
	DBG_shangyazhen_back_counter				= 0;
	DBG_xiayazhen_back_counter					= 0;
	DBG_shangyazhen_back_counter_xiaotou		= 0;
	DBG_xiayazhen_back_counter_xiaotou			= 0;
	
	shangyazhen_pos_start			= 0;
	shangyazhen_neg_start			= 0;
	xiayazhen_pos_start				= 0;
	xiayazhen_neg_start        		= 0;
	                              
	
	DBG_shangyazhen_pulse_cmp		= 0;
	DBG_xiayazhen_pulse_cmp         = 0;
	
	DBG_shangyazhen_motor_pulse		= 0;
	DBG_xiayazhen_motor_pulse       = 0;
	
	DBG_shangyazhen_counter			= 0;
	DBG_xiayazhen_counter			= 0;
	DBG_shangyazhen_counter_xiaotou	= 0;
	DBG_xiayazhen_counter_xiaotou	= 0;

	err_feed						= 0;
	
	fenceng_shangyazhen_zero_kb		= 0;
	fenceng_xiayazhen_zero_kb	    = 0;
	yazhen_fenceng_debug_kb         = 0;
	                                
	fenceng_shangyazhen_monitor		= 0;
	fenceng_xiayazhen_monitor	    = 0;
	                              
	DBG_shangyazhen_back_zero_counter_fenceng	= 0;
	DBG_xiayazhen_back_zero_counter_fenceng     = 0;
	                                          
	DBG_shangyazhen_back_counter_fenceng	= 0;
	DBG_xiayazhen_back_counter_fenceng	    = 0;
	                                      
	DBG_shangyazhen_counter_fenceng			= 0;
	DBG_xiayazhen_counter_fenceng	        = 0;
	                                      
	shangyazhen_fenceng_motor_start			= 0;
	xiayazhen_fenceng_motor_start	        = 0;
	                                      
	shangyazhen_fenceng_pulse_cmp			= 0;
	xiayazhen_fenceng_pulse_cmp             = 0;
	                                      
	shangyazhen_fenceng_motor_pulse			= 0;
	xiayazhen_fenceng_motor_pulse           = 0;
	                                      
	shangyazhen_fenceng_counter				= 0;
	xiayazhen_fenceng_counter		        = 0;
	                                      
	shangyazhen_fenceng						= 0;
	xiayazhen_fenceng 	                    = 0;
	                                      
	fenceng_shangyazhen_monitor_clear_b		= 0;
	fenceng_xiayazhen_monitor_clear_b	    = 0;
	                                      
	fenceng_shangyazhen_monitor_w			= 0;
	fenceng_xiayazhen_monitor_w 			= 0;
	
	Huiling_Buchang							= 0;
	
	
	
	
	
	
}

/*************************************************
Function(��������): void Yazhen_Normal_Reset(int Direction)
Description(�������ܡ����ܵȵ�����): ѹ�븴λ����Ҫ��������
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): encoder1_data_reset(void);

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/     
void Yazhen_Normal_Reset(void){
	shangyazhen_motor_start = 0;
	xiayazhen_motor_start	= 0;
	shangyazhen_counter = 0;
	xiayazhen_counter  = 0;
	
	Yazhen_Zero_App_Com();
	//Yazhen_Normal_Get_Zero_Start();
}

/*************************************************
Function(��������): void Alarm_Disp_Yazhen(unsigned int Port)
Description(�������ܡ����ܵȵ�����): ѹ������������֮������С����,С�����
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): Yazhen_Normal_Alarm();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
void Alarm_Disp_Yazhen(unsigned int Port)
{
	char Info_X3[]={33,67,46,27,20,11,24,48,41,47,'\0'};  //CharString:����λ�����ϡ�
	char Info_X4[]={33,67,46,27,20,11,24,48,47,34,'\0'};  //CharString:����λ�����¡�
	
	char QueDing[]={40,23,22,8};			 //CharString:��ȷ��
	INT16U LeftX,LeftY;
	LeftX = 200;
	LeftY = 150;
	if(xianshi_flag==0)
	{
		Lcd_Fill_Box (LeftX, LeftY, 440, 330, 4);
		//*(Info_qz+which_alarm-min_port)Ϊzhongduan_init()�е���ģ����
		if(Port == X3)
			Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300,5,13,4,3,0,1,0,0,Info_X3);
		else if(Port == X4)
			Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300,5,13,4,3,0,1,0,0,Info_X4);
		
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,RAISE);
		
		Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, 0, 7,2,0,0,0,3,QueDing);
		
		xianshi_flag=1;
	}
	delay_qz(0,10,1);	//��beep_alarm��
}

/*************************************************
Function(��������): void Yazhen_Normal_Alarm(U8* err)
Description(�������ܡ����ܵȵ�����): ѹ�뱨��APP��С���̵�ȷ����function.c->KeyScan()->default:
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): void zhongduan_fun(void);

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
void Yazhen_Normal_Alarm(U8* err){
	if(Err3_Miss > ERR_TIMES || Err3_Over > ERR_TIMES){
		*err = 1;
		previous_error_status_w=9;
		if (yazhen_alarm_level!=level_0)
		{
			if (privilege_run_flag==0)
				Alarm_Disp_Yazhen(X3);
			if (yazhen_alarm_level==level_3)
			{
				emer_stop_flag=1;
				run_permite_flag=0;
			}							
			else if (yazhen_alarm_level==level_2)
			{
				emer_stop_flag=0;
				run_permite_flag=0;
			}
			else
			{
				emer_stop_flag=0;
				run_permite_flag=0;
			}
			qz_error_status=1;
		}
		else
			*err = 0;
	}
	if(Err4_Miss > ERR_TIMES || Err4_Over > ERR_TIMES){
		*err = 1;
		previous_error_status_w=9;

		if (yazhen_alarm_level!=level_0)
		{
			if (privilege_run_flag==0)
				Alarm_Disp_Yazhen(X4);
			if (yazhen_alarm_level==level_3)
			{
				emer_stop_flag=1;
				run_permite_flag=0;
			}							
			else if (yazhen_alarm_level==level_2)
			{
				emer_stop_flag=0;
				run_permite_flag=0;
			}
			else 
			{
				emer_stop_flag=0;
				run_permite_flag=0;
			}
			qz_error_status=1;
		}
		else
			*err = 0;
	}
}


void Yazhen_Debug_App(void){
	if(yazhen_debug_start_com == 0){
		yazhen_debug_start_com = 1;
		Yazhen_Debug_App_Com();
	}
	Clear_Monitor();
	Get_Monitor();
	Yazhen_Debug_K_Set();
	Yazhen_Zero_Confirm();
	if((yazhen_datou_debug_kb == 1) && (yazhen_xiaotou_debug_kb == 1)){
		return;
	}
	else if(yazhen_datou_debug_kb == 1){
		if(yazhen_datou_choose_kb == UP_YAZHEN){
			Yazhen_EXT_Button(UP_YAZHEN);
		}
		else{
			Yazhen_EXT_Button(DOWN_YAZHEN);
		}
	}
	else if(yazhen_xiaotou_debug_kb == 1){
		if(yazhen_xiaotou_choose_kb == UP_YAZHEN){
			Yazhen_EXT_Button(UP_YAZHEN);
		}
		else{
			Yazhen_EXT_Button(DOWN_YAZHEN);
		}
	}
	else if(yazhen_fenceng_debug_kb == 1){
		if(yazhen_fenceng_choose_kb == UP_YAZHEN){
			Yazhen_EXT_Button_Fengceng(UP_YAZHEN);
		}
		else{
			Yazhen_EXT_Button_Fengceng(DOWN_YAZHEN);
		}
	}
}

void Yazhen_EXT_Button(unsigned int Yazhen_type){//int stage,
	if(Get_X_Value(X5) == EXT_ON && Get_X_Value(X11) == EXT_ON){
		return;
	}
	else if(Get_X_Value(X5) == EXT_ON){
		Yazhen_Set_Dir(GO);
		switch(Yazhen_type){
			case UP_YAZHEN:
				shangyazhen_pos_start = 1;
				break;
			case DOWN_YAZHEN:
				xiayazhen_pos_start = 1;
				break;
			default:
				break;
		}
	}
	else if(Get_X_Value(X11) == EXT_ON){
		Yazhen_Set_Dir(BACK);
		switch(Yazhen_type){
			case UP_YAZHEN:
				shangyazhen_neg_start = 1;
				break;
			case DOWN_YAZHEN:
				xiayazhen_neg_start = 1;
				break;
			default:
				break;
		}
	}
	else{
		shangyazhen_pos_start 	= 0;
		shangyazhen_neg_start 	= 0;
		xiayazhen_pos_start	 	= 0;
		xiayazhen_neg_start   	= 0;
	}   
}

void Yazhen_EXT_Button_Fengceng(unsigned int Yazhen_type){//int stage,
	if(Get_X_Value(X5) == EXT_ON && Get_X_Value(X11) == EXT_ON){
		return;
	}
	else if(Get_X_Value(X5) == EXT_ON){
		if(fenceng_dir_judge == SAME_DIR)
			Yazhen_Set_Dir(GO);
		else
			Yazhen_Set_Dir(BACK);
		switch(Yazhen_type){
			case UP_YAZHEN:
				shangyazhen_pos_start = 1;
				break;
			case DOWN_YAZHEN:
				xiayazhen_pos_start = 1;
				break;
			default:
				break;
		}
	}
	else if(Get_X_Value(X11) == EXT_ON){
		if(fenceng_dir_judge == SAME_DIR)
			Yazhen_Set_Dir(BACK);
		else
			Yazhen_Set_Dir(GO);
		switch(Yazhen_type){
			case UP_YAZHEN:
				shangyazhen_neg_start = 1;
				break;
			case DOWN_YAZHEN:
				xiayazhen_neg_start = 1;
				break;
			default:
				break;
		}
	}
	else{
		shangyazhen_pos_start 	= 0;
		shangyazhen_neg_start 	= 0;
		xiayazhen_pos_start	 	= 0;
		xiayazhen_neg_start   	= 0;
	}   
}

void Yazhen_Debug_K_Set(void){
	DBG_shangyazhen_pulse_cmp 	= Debug_CMP;
	DBG_xiayazhen_pulse_cmp 	= Debug_CMP;
	
	DBG_shangyazhen_back_cmp	= Debug_Back_CMP;
	DBG_xiayazhen_back_cmp 		= Debug_Back_CMP;
}


void Get_Monitor(void){
	datou_shangyazhen_monitor_w 	=  datou_shangyazhen_monitor;//shangyazhen_back_counter +
	datou_xiayazhen_monitor_w	  	=  datou_xiayazhen_monitor;// xiayazhen_back_counter + 
	xiaotou_shangyazhen_monitor_w 	=  xiaotou_shangyazhen_monitor;//shangyazhen_back_counter +
	xiaotou_xiayazhen_monitor_w 	=  xiaotou_xiayazhen_monitor;//xiayazhen_back_counter +
	fenceng_shangyazhen_monitor_w   =  fenceng_shangyazhen_monitor;
	fenceng_xiayazhen_monitor_w		=  fenceng_xiayazhen_monitor;
}

void Clear_Monitor(void){
	if(datou_shangyazhen_monitor_clear_b == 1){
		datou_shangyazhen_monitor = 0;
	}
	if(datou_xiayazhen_monitor_clear_b == 1){
		datou_xiayazhen_monitor = 0;
	}
	
	if(xiaotou_shangyazhen_monitor_clear_b == 1){
		xiaotou_shangyazhen_monitor = 0;
	}
	if(xiaotou_xiayazhen_monitor_clear_b == 1){
		xiaotou_xiayazhen_monitor = 0;
	}
	
	if(fenceng_shangyazhen_monitor_clear_b == 1){
		fenceng_shangyazhen_monitor = 0;
	}
	if(fenceng_xiayazhen_monitor_clear_b == 1){
		fenceng_xiayazhen_monitor = 0;
	}
}

void Yazhen_Zero_Confirm(void){
	if(shangyazhen_zero_confirm_kb == 1){
		if(yazhen_datou_debug_kb == 1){
			DBG_shangyazhen_counter = 0;
			DBG_shangyazhen_back_counter = 0;
			DBG_shangyazhen_back_zero_counter = 0;
		}
		else if(yazhen_xiaotou_debug_kb == 1){
			DBG_shangyazhen_counter_xiaotou = 0;
			DBG_shangyazhen_back_counter_xiaotou = 0;
			DBG_shangyazhen_back_zero_counter_xiaotou = 0;
		}
		else if(yazhen_fenceng_debug_kb == 1){
			DBG_shangyazhen_counter_fenceng = 0;
			DBG_shangyazhen_back_counter_fenceng = 0;
			DBG_shangyazhen_back_zero_counter_fenceng = 0;
		}
	}
	if(xiayazhen_zero_confirm_kb == 1){
		if(yazhen_datou_debug_kb == 1){
			DBG_xiayazhen_counter = 0;
			DBG_xiayazhen_back_counter = 0;
			DBG_xiayazhen_back_zero_counter = 0;
		}
		else if(yazhen_xiaotou_debug_kb == 1){
			DBG_xiayazhen_counter_xiaotou = 0;
			DBG_xiayazhen_back_counter_xiaotou = 0;
			DBG_xiayazhen_back_zero_counter_xiaotou = 0;
		}
		else if(yazhen_fenceng_debug_kb == 1){
			DBG_xiayazhen_counter_fenceng = 0;
			DBG_xiayazhen_back_counter_fenceng = 0;
			DBG_xiayazhen_back_zero_counter_fenceng = 0;
		}
	}	
}

void button_huchi(unsigned char* button0, unsigned char* button1, unsigned int* status_record,  unsigned int condition_flag)
{
	
	if(*button0 == 1 || *button1 ==1)
	{
		if(*status_record == 0)
		{
				if(*button1 == 1)
				{
					*button0 =0;
					*status_record = 1;
				}
		}
		
		else 
		{
			if(*button0 == 1)
			{
				*button1 = 0;
				*status_record = 0;
			}
		}
		condition_flag = 1;		//condition_flag�����жϵ㶯ͬʱͣ����������
	}
	
	else if((*button0 == 0) && (*button1 == 0))		//�˴��޸�= Ϊ== ֮ǰ���߼�С����2017.2.7 ������в���
	{
		
		*status_record = 0;
		condition_flag = 0;
		
	}
}


void button_3_huchi(unsigned char* button0,unsigned char* button1, unsigned char* button2,unsigned int* status_record1,unsigned int* status_record2){ 
	if(*button0 ==1 || *button1 == 1 || *button2 == 1)
	{
		if(*status_record1 == 0 && *status_record2 == 0)
		{
			if(*button0 == 1)
			{
				*button1 = 0;
				*button2 = 0;
				*status_record1 = 1;
				*status_record2 = 0;
			}
			
			else if(*button1 == 1 && *button0 ==0)
			{
				*button2 = 0;
				*status_record1 = 0;
				*status_record2 = 1;
			}
			
			else if(*button2 == 1 && *button1 == 0 && *button0 ==0)
			{
				*status_record1 = 1;
				*status_record2 = 1;
			}
			
		}
		
		else if(*status_record1 == 1 && *status_record2 == 0){
			if (*button1 == 1)
			{
				*button0 = 0;
				*button2 = 0;
				*status_record1 = 0;
				*status_record2 = 1;
			}

			else if (*button2 == 1 && *button1 == 0)
			{
				*button0 = 0;
				*status_record1 = 1;
				*status_record2 = 1;
			}
		}	
		
		
		else if(*status_record1 == 0 && *status_record2 == 1){
			if(*button2 == 1)
			{
				*button1 = 0;
				*button0 = 0;
				*status_record1 = 1;
				*status_record2 = 1;
			}
			
			
			else if(*button2 == 0 && *button0 == 1)
			{
				*button1 = 0;
				*status_record1 = 1;
				*status_record2 = 0;
			}
		}
		
		else if(*status_record1 == 1 && *status_record2 == 1){
			if(*button0 == 1){
				*button1 = 0;
				*button2 = 0;
				*status_record1 = 1;
				*status_record2 = 0;
			}
			
			else if(*button1 == 1 && *button0 == 0)
			{
				*button0 = 0;
				*button2 = 0;
				*status_record1 = 0;
				*status_record2 = 1;
			}
		}
		
	  }
	
	  else{
		  *status_record1 = 0;
		  *status_record2 = 0;
	  }
}

void Yazhen_Debug_App_Com(void){
	
}

void Yazhen_Run_App_Com(void){
	
}

void Yazhen_Zero_App_Com(void){
	
}

#endif