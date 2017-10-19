/* #include "includes.h"

unsigned int average_yazhen = 0;

void pressing_cal()
{
	if(yazheng_pulse_total_remember != 0)
	{
		yazheng_pulse_need = (INT16U)((yazheng_depth*1.0/(yazheng_max_deapth))*yazheng_pulse_total_remember);
		average_yazhen = (INT16U)((middlequanshu*1.0)/(yazheng_pulse_need)*encoder1_pulse_number);	//average_yazhen 是指转多少圈需要压针发一次脉冲
		
		if(average_yazhen == 0)//此处需要容错，不然压针在encoder1.c中,电机会一直运动.
			average_yazhen = 0;		
	
	}
	
	else if(yazheng_pulse_total_remember == 0)		//<=0的容错大于==0
	{
		average_yazhen = 0;
	}
	
	
} */