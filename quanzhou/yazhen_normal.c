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
	/**软件消抖**/
	X3_SIG++;
	
	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);
}

void __irq pwrDownHandler(void)	//X4-X7的中断服务程序绑定的新名字，但是目前没有用到
{

	if(!( rEINTMASK >>4  & 0x1 ) && (rEINTPEND & (1<<4)))//取消屏蔽标志+挂起有效
	{
		X4_SIG++;
		rEINTPEND=(1<<4);		
	}
	else if ( !( rEINTMASK >>5  & 0x1 )  && (rEINTPEND & (1<<5)) )//取消屏蔽标志+挂起有效	
	{
		
		rEINTPEND=(1<<5);		
	}
	else if ( !( rEINTMASK >>6  & 0x1 )  && (rEINTPEND & (1<<6)) )//取消屏蔽标志+挂起有效	
	{
		if((rGPFDAT >> 6) & 0x1)
		{
			InPulseCount[6]++;  //InPulseCount[n] 只定义了7个 InPulseCount[0]-----InPulseCount[6]
		}

		rEINTPEND=(1<<6);		
	}
	else if ( !( rEINTMASK >>7  & 0x1 )  && (rEINTPEND & (1<<7)) )//取消屏蔽标志+挂起有效	
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
	static unsigned int first_in = 0;		//第一次进入
	
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
	
	if (duanshu == guoduduan){//当前段为过渡段
		if((shangyazhen_datou >= shangyazhen_xiaotou) && (xiayazhen_datou >= xiayazhen_xiaotou)){
			Set_Y_Value(Y2,Dir_Positive);
			yazhen_total_pulse = (daduanquanshu + middlequanshu-dapan_round)*encoder1_cal_factor;
			
			if(shangyazhen_counter > shangyazhen_xiaotou){
				shangyazhen_motor_pulse = (shangyazhen_counter - shangyazhen_xiaotou);
				shangyazhen_pulse_cmp = yazhen_total_pulse/shangyazhen_motor_pulse;
				if (shangyazhen_pulse_cmp < 2)
					shangyazhen_pulse_cmp = 2;//保证下降沿
			}
			else
				shangyazhen_pulse_cmp = NO_MOVE;
			
			if(xiayazhen_counter > xiayazhen_xiaotou){
				xiayazhen_motor_pulse = (xiayazhen_counter - xiayazhen_xiaotou);
				xiayazhen_pulse_cmp = yazhen_total_pulse/xiayazhen_motor_pulse;
				if (xiayazhen_pulse_cmp < 2)
					xiayazhen_pulse_cmp = 2;//保证下降沿
			}
			else
				xiayazhen_pulse_cmp = NO_NOVE;

			if (yazhen_total_pulse == 0){//无过渡段则不动作，k->无穷大
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
					shangyazhen_pulse_cmp = 2;//保证下降沿	
			}
			else
				shangyazhen_pulse_cmp = NO_MOVE;
			
			if(xiayazhen_counter > xiayazhen_datou){
				xiayazhen_motor_pulse = (xiayazhen_counter - xiayazhen_datou);
				xiayazhen_pulse_cmp = yazhen_total_pulse/xiayazhen_motor_pulse;
				if (xiayazhen_pulse_cmp < 2)
					xiayazhen_pulse_cmp = 2;//保证下降沿
			}
			else
				xiayazhen_pulse_cmp = NO_NOVE;

			if (yazhen_total_pulse == 0){//无过渡段则不动作，k->无穷大
				shangyazhen_pulse_cmp = NO_MOVE;
				xiayazhen_pulse_cmp = NO_NOVE;
			}
			shangyazhen_counter = 0;
			xiayazhen_counter = 0;
		}
		else{//两者方向不同，不动作
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
				shangyazhen_pulse_cmp = 2;//保证下降沿
		}
		else
			shangyazhen_pulse_cmp = NO_MOVE;
		
		if(xiayazhen_motor_pulse > 0){
			xiayazhen_pulse_cmp = yazhen_total_pulse/xiayazhen_motor_pulse;
			if(xiayazhen_pulse_cmp < 2)
				xiayazhen_pulse_cmp = 2;//保证下降沿
		}
		else
			xiayazhen_pulse_cmp = NO_NOVE;

		if (yazhen_total_pulse == 0){//无过渡段则不动作，k->无穷大
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
					shangyazhen_pulse_cmp = 2;//保证下降沿
			}
			else
				shangyazhen_pulse_cmp = NO_MOVE;
			
			if(xiayazhen_motor_pulse > 0){
				xiayazhen_pulse_cmp = yazhen_total_pulse/xiayazhen_motor_pulse;
				if (xiayazhen_pulse_cmp < 2)
					xiayazhen_pulse_cmp = 2;//保证下降沿
			}
			else
				xiayazhen_pulse_cmp = NO_NOVE;
			
			if (yazhen_total_pulse == 0){//无过渡段则不动作，k->无穷大
				shangyazhen_pulse_cmp = NO_MOVE;
				xiayazhen_pulse_cmp = NO_NOVE;
			}
			shangyazhen_counter = 0;
			xiayazhen_counter = 0;
		}
		else{//两者相等之后，不动作
			shangyazhen_pulse_cmp = NO_MOVE;
			xiayazhen_pulse_cmp = NO_NOVE;
			shangyazhen_counter = 0;
			xiayazhen_counter = 0;
		}
	}
}


void Yazhen_Normal_Get_Zero_Start(void){

	/***设置返回K***/
	shangyazhen_back_cmp = 1500/Yazhen_Beilv;
	xiayazhen_back_cmp   = 1500/Yazhen_Beilv;
	
	if(shangyazhen_back_cmp < 2)
		shangyazhen_back_cmp = 2;
	if(xiayazhen_back_cmp < 2)
		xiayazhen_back_cmp = 2;
	
	if(shangyazhen_back_counter > 5)//大于5是为了YAZHEN_ZERO_ERR = -4
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
Function(函数名称): between_check(unsigned int roundShineng)
Description(函数功能、性能等的描述): 主要用于shinengpanduan()函数，判断当前阶段是否是在调线圈数之内
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): songsha_fre_change(void);		shinengpanduan(void);

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): roundShineng——大盘使能(传入dapan_round)
Output(对输出参数的说明):
Return: i:当前在调线的第i段
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
int between_check(unsigned int roundShineng){
	int i;
	
	//check函数主要用于调线功能,若未打开调线功能，则直接返回-1；
	if (tiaoxiankaiguan_kb == 0)
		return -1;
	
	for(i=0;i<tiaoshaduan_max;i++){
		
		/***进入条件：1.大于开始圈数
					2.结束圈数为0表示关闭
					3.小于结束圈数或者等于最后一圈
		***/
		if (roundShineng >= *tiaoxianduan[i].kaishiquanshu			
		&& *tiaoxianduan[i].jieshuquanshu							
		&& (roundShineng < *tiaoxianduan[i].jieshuquanshu			
		|| 	*tiaoxianduan[i].jieshuquanshu == 
			(daduanquanshu + middlequanshu + xiaoduanquanshu + caijiaoquanshu + langfeiquanshu))){
			
			
			if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
				return i;/***判断是否是额外段，即外部开关是否打开***/
			}
			else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
				return i;/***不是额外段，返回所在段***/
			}
		}
	}
	return -1;
}


/*************************************************
Function(函数名称): unsigned int at_check(unsigned int roundShineng)
Description(函数功能、性能等的描述): 1.主要用于变频器的变速，主要被bianpingqi_speed_cal()所调用
								2.主要修改了全局变量tiaoxianzu/tiaoxianzu_flag
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): bianpingqi_speed_cal(void)

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): roundShineng：dapan_round值
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
unsigned int at_check(unsigned int roundShineng){
	int i;
	
	/***进入条件(位与)：1.调线组圈数！=0，在encoder1_process()中++
					  2.调线组间隔圈数！=0
					  3.调线组圈数%调线组间隔圈数:间隔圈数的倍数
					  4.jiajiaStatus == 0
		此处代码主要为了判断当前是否调线组需要++以判断第二组 刀具是否需要出刀收刀，
		此处只是条件的增加，而不会return;
	***/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 &&
		tiaoxianzu_quanshu % tiaoxianzu_jiange ==0 && jiajiaStatus == 0)
	{
		jiajiaStatus = 1;
		if ( tiaoxianzu < tiaoxianzu_max ){
			tiaoxianzu++;
			com_again = 1;		//for 友峰
		}
	}
	
	/***调线组间隔圈数为0，则不分组，共同出刀收刀***/
	else if (tiaoxianzu_jiange == 0){
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
		tiaoxianzu = tiaoxianzu_max;
	}
	
	/*****调线工艺完成，flag和status复位*****/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 && 
		tiaoxianzu >= tiaoxianzu_max && 
		tiaoxianzu_quanshu >= ((tiaoxianzu_max -1)*tiaoxianzu_jiange + 1))
	{
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
	}
	
	
		
	/****	进入条件:1.调线组圈数不等于0 || 调线组间隔等于1
					2.调线组flag等于1并且调线组间隔不等于0
		
			此处代码对应判断是否到了第n组调线的时候，提前一圈降速(通过return 1;将函数返回):
			if ((at_check((dapan_round+1)) && encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw)))
			
			判断是否当前圈是否需要在其后面减速:
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
	
	
	/****调线八段速，此处仅仅为第1组的开始和结束，其余刀组数的判断均为上述代码进行判断****/
	for(i=0;i<tiaoshaduan_max;i++){
		/*****只要开启调线，首先进入以下函数******/
		if (*tiaoxianduan[i].jieshuquanshu)//对应上限不为零
		{											
			/****为了保证在开始圈数为0时，能提早减速进行调线(工艺要求调线之前需要减速，0圈开始调线，必须从最后一圈开始)
			****/
			if (*tiaoxianduan[i].kaishiquanshu == 0
			    && roundShineng == (daduanquanshu+middlequanshu+xiaoduanquanshu+
									caijiaoquanshu+langfeiquanshu))
			{
					/**************************下一个阶段为额外段，并且外部开关选择额外段************************************/
					if (getStage(current_stage,NEXTSTAGE) == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
					/**************************下一个阶段不为额外段，并且外部开关不选择额外段************************************/
					else if (getStage(current_stage,NEXTSTAGE) != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
			}//这个if为了保证在开始圈数为0时，能提早减速进行调线。
			
			/**********************************调线开始********************************************/
			else if (roundShineng == *tiaoxianduan[i].kaishiquanshu){						//等于下限
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
			
			/*********************************调线结束*******************************************/
			if (roundShineng == *tiaoxianduan[i].jieshuquanshu){						//等于上限

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