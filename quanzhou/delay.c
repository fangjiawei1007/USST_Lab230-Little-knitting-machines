#include"includes.h"

DELAY_STRUCTURE delay_fac;

U8 init_stop_status=0;
unsigned int reset_timer_counter=0;
void __irq	Timer0_Proc(void)
{
	static unsigned int init_enter_times=0;
	if (Choose_bianpingqi_kb==CHOOSE_NOT && init_enter_times < init_stop_time_kw && init_stop_status == 0){
		Set_Y_Value(2,0);
		init_stop_status = 0;
		init_enter_times ++;
	}
	else if (Choose_bianpingqi_kb==CHOOSE_NOT && init_enter_times >= init_stop_time_kw && init_stop_status == 0){
		Set_Y_Value(2,1);
		init_stop_status = 1;
	}
	if (reset_timer_start == 1){
		reset_timer_counter ++;
	}
	else{
		reset_timer_counter = 0;
	}
	encoder1_speed=encoder1_speed_pulse;//10
	encoder1_speed_pulse=0; 
	Delay_qz_sub();
	beep_alarm();
	ClearPending((U32)BIT_TIMER0);
}

void Timer0_init_quanzhou(void)
{
	pISR_TIMER0 = (int)Timer0_Proc;	// Timer ISR for DA
	Timer0_Init();							// 每100ms中断一次，也就是100HZ,0.01s
}

void Delay_qz_sub(void)
{
	U8 delay_rank;
	for (delay_rank=0;delay_rank<5;delay_rank++)
	{
		if (delay_fac.delay_permit[delay_rank]==1)
		{
			if (delay_fac.delay_init_flag[delay_rank]==0)
			{
				delay_fac.delay_flag[delay_rank]=0;
				delay_fac.delay_init_flag[delay_rank]=1;
			}
			else
			{
				delay_fac.delay_flag[delay_rank]++;
				if (delay_fac.delay_flag[delay_rank]>=delay_fac.delay_time_qz[delay_rank])
				{
					delay_fac.delay_flag[delay_rank]=0;
					//delay_fac.delay_permit[delay_rank]=0;
				}
			}
		}
		else
		{
			delay_fac.delay_flag[delay_rank]=0;
			delay_fac.delay_init_flag[delay_rank]=0;
		}
	}
}

void delay_qz(U8 delay_rank,U32 delay_t,U8 status)
{
	delay_fac.delay_time_qz[delay_rank]=delay_t;
	delay_fac.delay_permit[delay_rank]=status;
}