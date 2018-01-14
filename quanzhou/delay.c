#include"includes.h"

#define Delay_T_Amount		6

DELAY_STRUCTURE delay_fac;

U8 init_stop_status=0;
unsigned int reset_timer_counter=0;

/*************************************************
Function(函数名称): __irq	Timer0_Proc(void)
Description(函数功能、性能等的描述): Timer0中断服务程序：1.不使用变频器的情况下，防止屏死机，但是中断程序还在运行
													  2.WDT计数，用于encoder1.c中，用于main函数已经不进入，
														但是中断函数还在进入，将大盘停止
													  3.实时计算大盘速度的脉冲数记录,即完成速度的计算
													  4.非阻塞式延时：delay_qz(U8 delay_rank,U32 delay_t,U8 status)
													  5.外部报警(delay_qz(0,30,1))调用函数
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void __irq	Timer0_Proc(void)
{
	static unsigned int init_enter_times=0;

	/**不使用变频器的情况下，防止屏死机，但是中断程序还在运行**/
	if (Choose_bianpingqi_kb==CHOOSE_NOT && 
	    init_enter_times < init_stop_time_kw && 
		init_stop_status == 0){
		Set_Y_Value(2,0);
		init_stop_status = 0;
		init_enter_times ++;
	}
	else if (Choose_bianpingqi_kb==CHOOSE_NOT && 
			 init_enter_times >= init_stop_time_kw && 
			 init_stop_status == 0){
		Set_Y_Value(2,1);
		init_stop_status = 1;
	}
	
	//WDT
	if (reset_timer_start == 1){
		reset_timer_counter ++;		//WDT中断中累加到一定的数目之后，会使外部大盘停下
	}
	else{
		reset_timer_counter = 0;
	}
	
	//实时计算大盘速度
	encoder1_speed=encoder1_speed_pulse;//10
	
	//	forceEqual = 1; 
	//在Speed_change()中调用,上电第一次强制将速度加到K值，不通过变速执行
	if (encoder1_speed == 0){
		forceEqual = 1;
	}
	
	//将计数器置0，可求得每一次的速度值
	encoder1_speed_pulse=0; 
	
	Delay_qz_sub();
	beep_alarm();
	ClearPending((U32)BIT_TIMER0);
}


/*************************************************
Function(函数名称): Timer0_init_quanzhou(void)
Description(函数功能、性能等的描述): Timer0的初始化，将ISR绑定为Timer0_Proc
Calls (被本函数调用的函数清单): Timer0_Init()
Called By (调用本函数的函数清单): App_Init();

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void Timer0_init_quanzhou(void)
{
	pISR_TIMER0 = (int)Timer0_Proc;			// Timer ISR for 延时
	Timer0_Init();							// 每100ms中断一次，也就是100HZ,0.01s
}


/*************************************************
Function(函数名称): Delay_qz_sub(void)
Description(函数功能、性能等的描述): 非阻塞式Delay的实现函数，
								  此函数通过判断是第n个延时，
								  此处一共可以独立使用Delay_T_Amount个定时器
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): __irq	Timer0_Proc()

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 个人感觉init_flag这个变量可以不要，可以做得更加精简；其实init_flag只是容错机制。
Author:王德铭
Modified:for (delay_rank=0;delay_rank < 6;delay_rank++)->将数字6变为宏Delay_T_Amount
Commented:方佳伟
*************************************************/
void Delay_qz_sub(void)
{
	U8 delay_rank;
	for (delay_rank=0;delay_rank < Delay_T_Amount ;delay_rank++)
	{
		if (delay_fac.delay_permit[delay_rank]==1)
		{
			/**init_flag本来为1，此处将delay_flag置0，为了后面计数用**/
			if (delay_fac.delay_init_flag[delay_rank]==0)
			{
				delay_fac.delay_flag[delay_rank]=0;
				delay_fac.delay_init_flag[delay_rank]=1;
			}
			else
			{
				/**若外部没有将定时器关闭，则循环计数，外部通过#define delay_n_count来进行判断**/
				delay_fac.delay_flag[delay_rank]++;
				if (delay_fac.delay_flag[delay_rank]>=delay_fac.delay_time_qz[delay_rank])
				{
					delay_fac.delay_flag[delay_rank]=0;
					//delay_fac.delay_permit[delay_rank]=0;
				}
			}
		}
		
		/**外部关闭定时器则将计数器delay_flag[n]置零，初始化条件delay_init_flag[n]置零**/
		else
		{
			delay_fac.delay_flag[delay_rank]=0;
			delay_fac.delay_init_flag[delay_rank]=0;
		}
	}
}

/*************************************************
Function(函数名称): delay_qz(U8 delay_rank,U32 delay_t,U8 status)
Description(函数功能、性能等的描述): 不阻塞式delay
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): too many

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): delay_rank：第n个计时器
													      delay_t：延时时间
													      status：开始/关闭 开关
Output(对输出参数的说明):
Return: 
Others: 给外部全局变量结构体delay_fac传参，以达到开启延时的目的。
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void delay_qz(U8 delay_rank,U32 delay_t,U8 status)
{
	delay_fac.delay_time_qz[delay_rank]=delay_t;
	delay_fac.delay_permit[delay_rank]=status;
}



/************************非阻塞式Delay基本用法***********************************/
/**
{
	delay_qz(n,t,1);						//n号定时器,t段时间,1表示开启
	int x;								//x：延时中间时间段
	if (delay_fac.delay_permit[n]==1)
	{
		if (delay_0_count<(t-x))
		{
			//此处添加功能代码
		}
		else
		{
			//此处添加功能代码
		}
		
	}
}

{
	delay_qz(n,t,0);						//关闭n号定时器
}

**/