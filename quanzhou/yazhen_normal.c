#include "includes.h"

#ifdef YAZHEN_NORMAL_EN

#define Y2						2

#define Dir_Positive			(g_InteralMemory.KeepBit[90])
#define Dir_Negative			(~(g_InteralMemory.KeepBit[90]))

#define yazhen_move_status		(g_InteralMemory.KeepBit[91])
#define yazhen_back_status		(g_InteralMemory.KeepBit[92])

#define shangyazhen_datou		(g_InteralMemory.KeepWord[790])
#define shangyazhen_xiaotou		(g_InteralMemory.KeepWord[791])
#define xiayazhen_datou			(g_InteralMemory.KeepWord[792])
#define xiayazhen_xiaotou		(g_InteralMemory.KeepWord[793])

#define NO_MOVE					65530

unsigned int X3_SIG;
unsigned int X4_SIG;

enum YAZHEN_NORMAL_CHECKOUT{
	NOT_CHANGED = 0,
	CHANGED	= 1
};
	
	
void __irq shangyazhen_zero_process(void)	
{
	/**�������**/
	X3_SIG++;
	
	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);
}

void __irq pwrDownHandler(void)	//X4-X7���жϷ������󶨵������֣�����Ŀǰû���õ�
{

	if(!( rEINTMASK >>4  & 0x1 ) && (rEINTPEND & (1<<4)))//ȡ�����α�־+������Ч
	{
		X4_SIG++;
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
	
void Yazhen_Normal_App(void){
	int duanshu_cur = -1;
	unsigned int checkout_yazhen = 0;
	
	at_check(dapan_round);
	duanshu_cur = between_check(dapan_round);
	
	if(duanshu_cur != caijianduan){
		checkout_yazhen = Yazhen_Normal_Checkout();
		if(checkout_yazhen == CHANGED){
			Yazhen_Normal_Set(duanshu_cur);
		}
	}
	
	if(duanshu_cur == guoduduan && yazhen_move_status == 0){
		yazhen_move_status = 1;
		Yazhen_Normal_Start();
	}
	else if(duanshu_cur != guoduduan){
		yazhen_move_status = 0;
	}
	
	if(duanshu_cur == caijianduan && yazhen_back_status == 0){
		yazhen_back_status = 1;
		Yazhen_Normal_Get_Zero_Start();	
	}
	else if(duanshu_cur != caijianduan){
		yazhen_back_status = 0;
	}
}


inline void Yazhen_Normal_Start(void){
	shangyazhen_motor_start = 1;
	xiayazhen_motor_start = 1;
}
void Yazhen_Normal_Checkout(){
	static unsigned int first_in = 0;		//��һ�ν���
	
	static unsigned int shangyazhen_datou_pre = 0;
	static unsigned int xiayazhen_datou_pre = 0;
	static unsigned int shangyazhen_datou_cur = 0;
	static unsigned int xiayazhen_datou_cur = 0;
	
	static unsigned int shangyazhen_xiaotou_pre = 0;
	static unsigned int xiayazhen_xiaotou_pre = 0;
	static unsigned int shangyazhen_xiaotou_cur = 0;
	static unsigned int xiayazhen_xiaotou_cur = 0;
	
	// if(duanshu == -1)
		// return NOT_CHANGED;
	if(first_in == 0){
		shangyazhen_datou_pre = shangyazhen_datou;
		xiayazhen_datou_pre = xiayazhen_datou;
		shangyazhen_datou_cur = shangyazhen_datou;
		xiayazhen_datou_cur = xiayazhen_datou;
		                       
		shangyazhen_xiaotou_pre = shangyazhen_xiaotou;
		xiayazhen_xiaotou_pre = xiayazhen_xiaotou;
		shangyazhen_xiaotou_cur = shangyazhen_xiaotou;
		xiayazhen_xiaotou_cur = xiayazhen_xiaotou;
		
		first_in = 1;
		return CHANGED;
	}
	else{
		shangyazhen_datou_cur = shangyazhen_datou;
		xiayazhen_datou_cur = xiayazhen_datou;
		shangyazhen_xiaotou_cur = shangyazhen_xiaotou;
		xiayazhen_xiaotou_cur = xiayazhen_xiaotou;
		
		if(( shangyazhen_datou_pre == shangyazhen_datou_cur ) && ( xiayazhen_datou_pre == xiayazhen_datou_cur)
		  && (shangyazhen_xiaotou_pre == shangyazhen_xiaotou_cur) && (xiayazhen_xiaotou_pre == xiayazhen_xiaotou_cur))
			return NOT_CHANGED;
		else{
			shangyazhen_datou_pre = shangyazhen_datou_cur;
			xiayazhen_datou_pre = xiayazhen_datou_cur;
			shangyazhen_xiaotou_pre = shangyazhen_xiaotou_cur;
			xiayazhen_xiaotou_pre = xiayazhen_xiaotou_cur;
			return CHANGED;
		}
	}
	return NOT_CHANGED;
}

void Yazhen_Normal_Set(int duanshu){
	int yazhen_total_pulse=0;
	
	if (duanshu == guoduduan){//��ǰ��Ϊ���ɶ�
		if((shangyazhen_datou >= shangyazhen_xiaotou) && (xiayazhen_datou >= xiayazhen_xiaotou)){
			Set_Y_Value(Y2,Dir_Positive);
			yazhen_total_pulse = (daduanquanshu + middlequanshu-dapan_round)*encoder1_cal_factor;
			
			if(shangyazhen_counter > shangyazhen_xiaotou){
				shangyazhen_motor_pulse = (shangyazhen_counter - shangyazhen_xiaotou);
				shangyazhen_pulse_cmp = yazhen_total_pulse/shangyazhen_motor_pulse;
				if (shangyazhen_pulse_cmp < 2)
					shangyazhen_pulse_cmp = 2;//��֤�½���
			}
			else
				shangyazhen_pulse_cmp = NO_MOVE;
			
			if(xiayazhen_counter > xiayazhen_xiaotou){
				xiayazhen_motor_pulse = (xiayazhen_counter - xiayazhen_xiaotou);
				xiayazhen_pulse_cmp = yazhen_total_pulse/xiayazhen_motor_pulse;
				if (xiayazhen_pulse_cmp < 2)
					xiayazhen_pulse_cmp = 2;//��֤�½���
			}
			else
				xiayazhen_pulse_cmp = NO_NOVE;

			if (yazhen_total_pulse == 0){//�޹��ɶ��򲻶�����k->�����
				shangyazhen_pulse_cmp = NO_MOVE;
				xiayazhen_pulse_cmp = NO_NOVE;
			}
			shangyazhen_counter = 0;
			xiayazhen_counter = 0;
		}
		else if ((shangyazhen_datou <= shangyazhen_xiaotou) && (xiayazhen_datou <= xiayazhen_xiaotou)){
			Set_Y_Value(Y2,Dir_Negative);
			yazhen_total_pulse = (daduanquanshu + middlequanshu-dapan_round)*encoder1_cal_factor;
			
			if(shangyazhen_counter > shangyazhen_datou){
				shangyazhen_motor_pulse = (shangyazhen_counter - shangyazhen_datou);
				shangyazhen_pulse_cmp = yazhen_total_pulse/shangyazhen_motor_pulse;
				if (shangyazhen_pulse_cmp < 2)
					shangyazhen_pulse_cmp = 2;//��֤�½���	
			}
			else
				shangyazhen_pulse_cmp = NO_MOVE;
			
			if(xiayazhen_counter > xiayazhen_datou){
				xiayazhen_motor_pulse = (xiayazhen_counter - xiayazhen_datou);
				xiayazhen_pulse_cmp = yazhen_total_pulse/xiayazhen_motor_pulse;
				if (xiayazhen_pulse_cmp < 2)
					xiayazhen_pulse_cmp = 2;//��֤�½���
			}
			else
				xiayazhen_pulse_cmp = NO_NOVE;

			if (yazhen_total_pulse == 0){//�޹��ɶ��򲻶�����k->�����
				shangyazhen_pulse_cmp = NO_MOVE;
				xiayazhen_pulse_cmp = NO_NOVE;
			}
			shangyazhen_counter = 0;
			xiayazhen_counter = 0;
		}
		else{//���߷���ͬ��������
			shangyazhen_pulse_cmp = NO_MOVE;
			xiayazhen_pulse_cmp = NO_NOVE;
			shangyazhen_counter = 0;
			xiayazhen_counter = 0;
		}
	}
	else{
		if((shangyazhen_datou >= shangyazhen_xiaotou) && (xiayazhen_datou >= xiayazhen_xiaotou)){
		Set_Y_Value(Y2,Dir_Positive);
		shangyazhen_motor_pulse = (shangyazhen_datou - shangyazhen_xiaotou);
		xiayazhen_motor_pulse = (xiayazhen_datou - xiayazhen_xiaotou);
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
			xiayazhen_pulse_cmp = NO_NOVE;

		if (yazhen_total_pulse == 0){//�޹��ɶ��򲻶�����k->�����
			shangyazhen_pulse_cmp = NO_MOVE;
			xiayazhen_pulse_cmp = NO_NOVE;
		}
		shangyazhen_counter = 0;
		xiayazhen_counter = 0;
		
		}
		else if ((shangyazhen_datou <= shangyazhen_xiaotou) && (xiayazhen_datou <= xiayazhen_xiaotou)){
			Set_Y_Value(Y2,Dir_Negative);
			shangyazhen_motor_pulse = (shangyazhen_xiaotou - shangyazhen_datou);
			xiayazhen_motor_pulse = (xiayazhen_xiaotou - xiayazhen_datou);
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
				xiayazhen_pulse_cmp = NO_NOVE;
			
			if (yazhen_total_pulse == 0){//�޹��ɶ��򲻶�����k->�����
				shangyazhen_pulse_cmp = NO_MOVE;
				xiayazhen_pulse_cmp = NO_NOVE;
			}
			shangyazhen_counter = 0;
			xiayazhen_counter = 0;
		}
		else{//�������֮�󣬲�����
			shangyazhen_pulse_cmp = NO_MOVE;
			xiayazhen_pulse_cmp = NO_NOVE;
			shangyazhen_counter = 0;
			xiayazhen_counter = 0;
		}
	}
}


void Yazhen_Normal_Get_Zero_Start(void){

	/***���÷���K***/
	shangyazhen_back_cmp = 1500/Yazhen_Beilv;
	xiayazhen_back_cmp   = 1500/Yazhen_Beilv;
	
	if(shangyazhen_back_cmp < 2)
		shangyazhen_back_cmp = 2;
	if(xiayazhen_back_cmp < 2)
		xiayazhen_back_cmp = 2;
	
	if(shangyazhen_back_counter > 5)//����5��Ϊ��YAZHEN_ZERO_ERR = -4
		shangyazhen_back_start = 1;
	if(xiayazhen_back_counter > 5)
		xiayazhen_back_start = 1;
	
}


void Yazhen_Normal_Init(void){
	
}

void Yazhen_Normal_Init_Once(void){
	Dir_Positive = 0;
	
	yazhen_move_status  = 0;	
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
}   	
    
void Yazhen_Normal_Reset(void){
	shangyazhen_motor_start = 0;
	xiayazhen_motor_start	= 0;
	shangyazhen_counter = 0;
	xiayazhen_counter  = 0;
	motor_factor_shangyazhen=0;
	motor_factor_xiayazhen=0;
	Yazhen_Normal_Get_Zero_Start();
	
	
	
}
/*************************************************
Function(��������): between_check(unsigned int roundShineng)
Description(�������ܡ����ܵȵ�����): ��Ҫ����shinengpanduan()�������жϵ�ǰ�׶��Ƿ����ڵ���Ȧ��֮��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): songsha_fre_change(void);		shinengpanduan(void);

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): roundShineng��������ʹ��(����dapan_round)
Output(�����������˵��):
Return: i:��ǰ�ڵ��ߵĵ�i��
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
int between_check(unsigned int roundShineng){
	int i;
	
	//check������Ҫ���ڵ��߹���,��δ�򿪵��߹��ܣ���ֱ�ӷ���-1��
	if (tiaoxiankaiguan_kb == 0)
		return -1;
	
	for(i=0;i<tiaoshaduan_max;i++){
		
		/***����������1.���ڿ�ʼȦ��
					2.����Ȧ��Ϊ0��ʾ�ر�
					3.С�ڽ���Ȧ�����ߵ������һȦ
		***/
		if (roundShineng >= *tiaoxianduan[i].kaishiquanshu			
		&& *tiaoxianduan[i].jieshuquanshu							
		&& (roundShineng < *tiaoxianduan[i].jieshuquanshu			
		|| 	*tiaoxianduan[i].jieshuquanshu == 
			(daduanquanshu + middlequanshu + xiaoduanquanshu + caijiaoquanshu + langfeiquanshu))){
			
			
			if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
				return i;/***�ж��Ƿ��Ƕ���Σ����ⲿ�����Ƿ��***/
			}
			else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
				return i;/***���Ƕ���Σ��������ڶ�***/
			}
		}
	}
	return -1;
}


/*************************************************
Function(��������): unsigned int at_check(unsigned int roundShineng)
Description(�������ܡ����ܵȵ�����): 1.��Ҫ���ڱ�Ƶ���ı��٣���Ҫ��bianpingqi_speed_cal()������
								2.��Ҫ�޸���ȫ�ֱ���tiaoxianzu/tiaoxianzu_flag
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): bianpingqi_speed_cal(void)

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): roundShineng��dapan_roundֵ
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
unsigned int at_check(unsigned int roundShineng){
	int i;
	
	/***��������(λ��)��1.������Ȧ����=0����encoder1_process()��++
					  2.��������Ȧ����=0
					  3.������Ȧ��%��������Ȧ��:���Ȧ���ı���
					  4.jiajiaStatus == 0
		�˴�������ҪΪ���жϵ�ǰ�Ƿ��������Ҫ++���жϵڶ��� �����Ƿ���Ҫ�����յ���
		�˴�ֻ�����������ӣ�������return;
	***/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 &&
		tiaoxianzu_quanshu % tiaoxianzu_jiange ==0 && jiajiaStatus == 0)
	{
		jiajiaStatus = 1;
		if ( tiaoxianzu < tiaoxianzu_max ){
			tiaoxianzu++;
			com_again = 1;		//for �ѷ�
		}
	}
	
	/***��������Ȧ��Ϊ0���򲻷��飬��ͬ�����յ�***/
	else if (tiaoxianzu_jiange == 0){
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
		tiaoxianzu = tiaoxianzu_max;
	}
	
	/*****���߹�����ɣ�flag��status��λ*****/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 && 
		tiaoxianzu >= tiaoxianzu_max && 
		tiaoxianzu_quanshu >= ((tiaoxianzu_max -1)*tiaoxianzu_jiange + 1))
	{
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
	}
	
	
		
	/****	��������:1.������Ȧ��������0 || ������������1
					2.������flag����1���ҵ�������������0
		
			�˴������Ӧ�ж��Ƿ��˵�n����ߵ�ʱ����ǰһȦ����(ͨ��return 1;����������):
			if ((at_check((dapan_round+1)) && encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw)))
			
			�ж��Ƿ�ǰȦ�Ƿ���Ҫ����������:
			if ((at_check((dapan_round)) && encoder1_pulse_number < jiajiansujiangemaichong_kw))
	****/
	if ((tiaoxianzu_quanshu != 0 || tiaoxianzu_jiange == 1) && 
		 tiaoxianzu_flag == 1 && tiaoxianzu_jiange != 0)
	{
		
		if (roundShineng == (dapan_round + 1) && 
		   ((tiaoxianzu_quanshu+1) % tiaoxianzu_jiange ==0 )) {
			return 1;
		}
		
		if (roundShineng == (dapan_round ) && 
		   ((tiaoxianzu_quanshu) % tiaoxianzu_jiange ==0 )) {
			return 1;
		}
	}
	
	
	/****���߰˶��٣��˴�����Ϊ��1��Ŀ�ʼ�ͽ��������൶�������жϾ�Ϊ������������ж�****/
	for(i=0;i<tiaoshaduan_max;i++){
		/*****ֻҪ�������ߣ����Ƚ������º���******/
		if (*tiaoxianduan[i].jieshuquanshu)//��Ӧ���޲�Ϊ��
		{											
			/****Ϊ�˱�֤�ڿ�ʼȦ��Ϊ0ʱ����������ٽ��е���(����Ҫ�����֮ǰ��Ҫ���٣�0Ȧ��ʼ���ߣ���������һȦ��ʼ)
			****/
			if (*tiaoxianduan[i].kaishiquanshu == 0
			    && roundShineng == (daduanquanshu+middlequanshu+xiaoduanquanshu+
									caijiaoquanshu+langfeiquanshu))
			{
					/**************************��һ���׶�Ϊ����Σ������ⲿ����ѡ������************************************/
					if (getStage(current_stage,NEXTSTAGE) == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
					/**************************��һ���׶β�Ϊ����Σ������ⲿ���ز�ѡ������************************************/
					else if (getStage(current_stage,NEXTSTAGE) != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
			}//���ifΪ�˱�֤�ڿ�ʼȦ��Ϊ0ʱ����������ٽ��е��ߡ�
			
			/**********************************���߿�ʼ********************************************/
			else if (roundShineng == *tiaoxianduan[i].kaishiquanshu){						//��������
				if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 1;
				}
				else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 1;
				}
			}
			
			/*********************************���߽���*******************************************/
			if (roundShineng == *tiaoxianduan[i].jieshuquanshu){						//��������

				if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 2;
				}
				else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 2;
				}
			}
		}
	}
	return 0;
}
#endif