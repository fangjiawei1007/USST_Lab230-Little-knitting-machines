/* #include "includes.h"

#define	pressing_jog_speed		100
				
unsigned int jog_positive_status = 0;
unsigned int jog_negative_status = 0;

void pressing_jog()//此处jog为定长运动,即控制方案中的手动调试
{
	if(positive_button == 1 && jog_positive_status == 0)
	
	{
		jog_positive_status = 1;
		Set_Y_Value(pressing_director_port,pressing_positive);
		
		if(multiply == 0)
		{
			positive_button = 0; //若倍率为0 则直接将按键弹起，status置零
			jog_positive_status = 0;
		}
		
		else if(multiply > 0)
		PulseOut_1_Start(pressing_jog_speed,multiply);
	}
	
	
	if(negative_button == 1 && jog_negative_status == 0)
	{
		jog_negative_status = 1;
		Set_Y_Value(pressing_director_port,pressing_negative);
		
		if(multiply == 0)
		{
			negative_button = 0; //若倍率为0 则直接将按键弹起，status置零
			jog_negative_status = 0;
		}
		
		else if(multiply > 0)
		PulseOut_1_Start(pressing_jog_speed,multiply);
		
	}
	
} */