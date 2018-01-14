#include"includes.h"

#define Delay_T_Amount		6

DELAY_STRUCTURE delay_fac;

U8 init_stop_status=0;
unsigned int reset_timer_counter=0;

/*************************************************
Function(��������): __irq	Timer0_Proc(void)
Description(�������ܡ����ܵȵ�����): Timer0�жϷ������1.��ʹ�ñ�Ƶ��������£���ֹ�������������жϳ���������
													  2.WDT����������encoder1.c�У�����main�����Ѿ������룬
														�����жϺ������ڽ��룬������ֹͣ
													  3.ʵʱ��������ٶȵ���������¼,������ٶȵļ���
													  4.������ʽ��ʱ��delay_qz(U8 delay_rank,U32 delay_t,U8 status)
													  5.�ⲿ����(delay_qz(0,30,1))���ú���
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void __irq	Timer0_Proc(void)
{
	static unsigned int init_enter_times=0;

	/**��ʹ�ñ�Ƶ��������£���ֹ�������������жϳ���������**/
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
		reset_timer_counter ++;		//WDT�ж����ۼӵ�һ������Ŀ֮�󣬻�ʹ�ⲿ����ͣ��
	}
	else{
		reset_timer_counter = 0;
	}
	
	//ʵʱ��������ٶ�
	encoder1_speed=encoder1_speed_pulse;//10
	
	//	forceEqual = 1; 
	//��Speed_change()�е���,�ϵ��һ��ǿ�ƽ��ٶȼӵ�Kֵ����ͨ������ִ��
	if (encoder1_speed == 0){
		forceEqual = 1;
	}
	
	//����������0�������ÿһ�ε��ٶ�ֵ
	encoder1_speed_pulse=0; 
	
	Delay_qz_sub();
	beep_alarm();
	ClearPending((U32)BIT_TIMER0);
}


/*************************************************
Function(��������): Timer0_init_quanzhou(void)
Description(�������ܡ����ܵȵ�����): Timer0�ĳ�ʼ������ISR��ΪTimer0_Proc
Calls (�����������õĺ����嵥): Timer0_Init()
Called By (���ñ������ĺ����嵥): App_Init();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void Timer0_init_quanzhou(void)
{
	pISR_TIMER0 = (int)Timer0_Proc;			// Timer ISR for ��ʱ
	Timer0_Init();							// ÿ100ms�ж�һ�Σ�Ҳ����100HZ,0.01s
}


/*************************************************
Function(��������): Delay_qz_sub(void)
Description(�������ܡ����ܵȵ�����): ������ʽDelay��ʵ�ֺ�����
								  �˺���ͨ���ж��ǵ�n����ʱ��
								  �˴�һ�����Զ���ʹ��Delay_T_Amount����ʱ��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): __irq	Timer0_Proc()

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: ���˸о�init_flag����������Բ�Ҫ���������ø��Ӿ�����ʵinit_flagֻ���ݴ���ơ�
Author:������
Modified:for (delay_rank=0;delay_rank < 6;delay_rank++)->������6��Ϊ��Delay_T_Amount
Commented:����ΰ
*************************************************/
void Delay_qz_sub(void)
{
	U8 delay_rank;
	for (delay_rank=0;delay_rank < Delay_T_Amount ;delay_rank++)
	{
		if (delay_fac.delay_permit[delay_rank]==1)
		{
			/**init_flag����Ϊ1���˴���delay_flag��0��Ϊ�˺��������**/
			if (delay_fac.delay_init_flag[delay_rank]==0)
			{
				delay_fac.delay_flag[delay_rank]=0;
				delay_fac.delay_init_flag[delay_rank]=1;
			}
			else
			{
				/**���ⲿû�н���ʱ���رգ���ѭ���������ⲿͨ��#define delay_n_count�������ж�**/
				delay_fac.delay_flag[delay_rank]++;
				if (delay_fac.delay_flag[delay_rank]>=delay_fac.delay_time_qz[delay_rank])
				{
					delay_fac.delay_flag[delay_rank]=0;
					//delay_fac.delay_permit[delay_rank]=0;
				}
			}
		}
		
		/**�ⲿ�رն�ʱ���򽫼�����delay_flag[n]���㣬��ʼ������delay_init_flag[n]����**/
		else
		{
			delay_fac.delay_flag[delay_rank]=0;
			delay_fac.delay_init_flag[delay_rank]=0;
		}
	}
}

/*************************************************
Function(��������): delay_qz(U8 delay_rank,U32 delay_t,U8 status)
Description(�������ܡ����ܵȵ�����): ������ʽdelay
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): too many

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): delay_rank����n����ʱ��
													      delay_t����ʱʱ��
													      status����ʼ/�ر� ����
Output(�����������˵��):
Return: 
Others: ���ⲿȫ�ֱ����ṹ��delay_fac���Σ��Դﵽ������ʱ��Ŀ�ġ�
Author:������
Modified:
Commented:����ΰ
*************************************************/
void delay_qz(U8 delay_rank,U32 delay_t,U8 status)
{
	delay_fac.delay_time_qz[delay_rank]=delay_t;
	delay_fac.delay_permit[delay_rank]=status;
}



/************************������ʽDelay�����÷�***********************************/
/**
{
	delay_qz(n,t,1);						//n�Ŷ�ʱ��,t��ʱ��,1��ʾ����
	int x;								//x����ʱ�м�ʱ���
	if (delay_fac.delay_permit[n]==1)
	{
		if (delay_0_count<(t-x))
		{
			//�˴���ӹ��ܴ���
		}
		else
		{
			//�˴���ӹ��ܴ���
		}
		
	}
}

{
	delay_qz(n,t,0);						//�ر�n�Ŷ�ʱ��
}

**/