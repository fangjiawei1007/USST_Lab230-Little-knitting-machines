/* #include "includes.h"



unsigned int	init_outside_status = 0;
unsigned int	pressing_zero_finish = 0;
unsigned int 	lingwei_jiance_cover = 0;
unsigned int 	lingwei_jiance_uncover = 0;
unsigned int	huiling_length_tmp = 0;
unsigned int	lingwei_start_status = 0;
void pressing_get_zero()
{
	//�㶯����
	// status 
		// lock_button;			
	if(lingwei_start_status == 1)
		lingwei_jiance_button = 1;
	
	if(lingwei_jiance_button == 1)//����ϵ����
	{
		lingwei_start_status = 1;
		pressing_zero(lingwei_x_port);
	}
	
	
	if((current_stage == 0 || current_stage == 4 || current_stage == 5) && pressing_zero_finish == 0){
		pressing_zero(lingwei_x_port);
	}
	//  else if(lingwei_jiance_button == 0)
	// {
		// PulseOut_1_Stop();
	// }
	
}

void pressing_zero(unsigned int X_number)
{
	//��ס�����Ӧ���������Ƴ����ٷ������
	if(Get_X_Value(X_number) == cover && (lingwei_jiance_cover == 0))//&& huiling_length !=0 && lingwei_jiance_button == 1 && lingwei_start_status == 1
	{
		PulseOut_1_Stop();
		
		lingwei_jiance_cover = 1;
		huiling_length_tmp = 0;
		Set_Y_Value(pressing_director_port,pressing_positive);
		PulseOut_1_Start(500,-1);
		
	}
	
	//�ڱ��λ�û���֮���������Ƴ�
	if ((lingwei_jiance_uncover == 0) && (Get_X_Value(X_number) == uncover) )//&& lingwei_start_status == 1
	{	
		if (lingwei_jiance_cover == 1 && huiling_length_tmp >= huiling_length)
		{
			PulseOut_1_Stop();
			
			
			lingwei_jiance_uncover=1;
			huiling_length_tmp=0;
			
			init_outside_status = 0;
			
			Set_Y_Value(pressing_director_port,pressing_negative);
			
			PulseOut_1_Start(500,-1); // Y1����,�˴���ʼ��������
			
		}
		else if(lingwei_jiance_cover != 1){
			
			PulseOut_1_Stop();
			
			if (huiling_zijian_button == 1)
				init_outside_status = 1;
			else
				init_outside_status = 0;
			
			lingwei_jiance_cover = 1;
			lingwei_jiance_uncover = 1;
			huiling_length_tmp = 0;
			
			
			
			Set_Y_Value(pressing_director_port,pressing_negative);
			PulseOut_1_Start(500,-1);
		}
	}
	
}


 void __irq pressing_zero_process(void)   // ��������X8-x10  		//appbase�����α������ڴ˴�
 {
	 	 
	 if ( !( rEINTMASK >>8  & 0x1 )  && (rEINTPEND & (1<<8)) ) //ȡ������+������Ч
	
	{	
	    //if((rGPGDAT >> 0) & 0x1) //�ж��ش���
	    {
	      //////////
	      
	    //}
		
		rEINTPEND=(1<<8);
		 
	}
	else if ( !( rEINTMASK >>9  & 0x1 )  && (rEINTPEND & (1<<9)) ) //ȡ������+������Ч   
	{ 
		//////////
		rEINTPEND=(1<<9);
	}
	else if ( !( rEINTMASK >>10  & 0x1 )  && (rEINTPEND & (1<<10)) ) //ȡ������+������Ч  
	{
		////////
		rEINTPEND=(1<<10);
	}
	else if ( !( rEINTMASK >>11  & 0x1 )  && (rEINTPEND & (1<<11)) ) //ȡ������+������Ч  
	{
		yazheng_pulse_number = 0;
		
		if(lingwei_jiance_cover == 1 && lingwei_jiance_uncover == 1 && init_outside_status == 1)	//��������棬���ҽ��л����Լ�
		{
			PulseOut_1_Stop();
			lingwei_jiance_cover = 0;
			lingwei_jiance_uncover = 0;
			//lingwei_start_status = 0;
			huiling_length_tmp = 0;
		
		}
	
		if(lingwei_jiance_cover == 1 && lingwei_jiance_uncover == 1 && init_outside_status == 0)	//�����ԭ�㣬���߲�������λ�Լ�
		{
			PulseOut_1_Stop();
			lingwei_jiance_cover = 0;
			lingwei_jiance_uncover = 0;
			//lingwei_start_status = 0;
			huiling_length_tmp = 0;
			
			if(lingwei_jiance_button == 1)
			{
				lingwei_start_status = 0;	//��λ����status
				lingwei_jiance_button = 0;	//��������󰴼�����
			}
			if(current_stage == 4 && pressing_zero_finish == 0)
				pressing_zero_finish = 1;	//��������֮��finish = 1 ���ٽ���
		}
		
		////////
		rEINTPEND=(1<<11);
	}
	else if ( !( rEINTMASK >>12  & 0x1 )  && (rEINTPEND & (1<<12)) ) //ȡ������+������Ч  
	{
		////////
		rEINTPEND=(1<<12);
	}
	else if ( !( rEINTMASK >>13  & 0x1 )  && (rEINTPEND & (1<<13)) ) //ȡ������+������Ч  
	{
		////////
		rEINTPEND=(1<<13);
	}
	else if ( !( rEINTMASK >>14  & 0x1 )  && (rEINTPEND & (1<<14)) ) //ȡ������+������Ч  
	{
		////////
		rEINTPEND=(1<<14);
	} 
	else if ( !( rEINTMASK >>15  & 0x1 )  && (rEINTPEND & (1<<15)) ) //ȡ������+������Ч  
	{
		////////
		rEINTPEND=(1<<15);
	}
	
	
	
	ClearPending(BIT_EINT8_23);

 }  */