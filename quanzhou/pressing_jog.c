/* #include "includes.h"

#define	pressing_jog_speed		100
				
unsigned int jog_positive_status = 0;
unsigned int jog_negative_status = 0;

void pressing_jog()//�˴�jogΪ�����˶�,�����Ʒ����е��ֶ�����
{
	if(positive_button == 1 && jog_positive_status == 0)
	
	{
		jog_positive_status = 1;
		Set_Y_Value(pressing_director_port,pressing_positive);
		
		if(multiply == 0)
		{
			positive_button = 0; //������Ϊ0 ��ֱ�ӽ���������status����
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
			negative_button = 0; //������Ϊ0 ��ֱ�ӽ���������status����
			jog_negative_status = 0;
		}
		
		else if(multiply > 0)
		PulseOut_1_Start(pressing_jog_speed,multiply);
		
	}
	
} */