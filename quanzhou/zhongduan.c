#include"includes.h"
U8	xianshi_flag=0;
//char Info_qz[10][10]={0};
unsigned char qz_error_status=0;
unsigned int qz_cuowu_cishu[max_port+1]={0};
unsigned char beep_status=0;
unsigned char run_permite_flag=0;			//�����������б�־λ;1->��������;0->����������;
											//�ⲿ�����û�о���ʱ��1
unsigned char emer_stop_flag=0;
unsigned char privilege_run_flag=0;			//ǿ�����б�־λ

unsigned char alarm_signal[max_port+1]={0};

/*************************************************
Function(��������): zhongduan_fun(void)
Description(�������ܡ����ܵȵ�����): 
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
void zhongduan_fun(void)
{
	U8 i,err_flag=0;
	//alarm_signal[5]=anquanmen_alarm_sig_kb;
	
	/**���µ������ֵ��Ϊ0**/
	alarm_signal[6]=anquanmen_alarm_sig_kb;
	alarm_signal[shangduansha_port]=shangduansha_alarm_sig_kb;
	alarm_signal[anquanmen_port]=anquanmen_alarm_sig_kb;
	alarm_signal[queyou_port]=queyou_alarm_sig_kb;
	alarm_signal[tanzhen_port]=tanzhen_alarm_sig_kb;
	alarm_signal[tanbu_port]=tanbu_alarm_sig_kb;
	
	//��������0����ͬΪ0��ͬΪ1;
	if (hongdeng_status==0)
		shangduansha_status=Get_X_Value(shangduansha_port) ^ alarm_signal[shangduansha_port];
	else
		shangduansha_status=alarm;
	
	anquanmen_status=		(Get_X_Value(anquanmen_port)^alarm_signal[anquanmen_port]) &
							(Get_X_Value(anquanmen2_port)^alarm_signal[anquanmen_port]);
	
	queyou_status=			Get_X_Value(queyou_port) ^ alarm_signal[queyou_port];						
	
	tanzhen_status=			Get_X_Value(tanzhen_port) ^ alarm_signal[tanzhen_port];
	
	tanbu_status=			Get_X_Value(tanbu_port) ^ alarm_signal[tanbu_port];
	
	
	for(i=6;i<=12;i++)
	{
		//�϶�ɴ����
		if (i==shangduansha_port && hongdeng_status==1)
		{
			err_flag=1;
			qz_error_status=1;
			break;
		}
		//X11δʹ��
		if (i==11)
			continue;
		
		if(Get_X_Value(i)==alarm_signal[i])
		{
			err_flag=1;
			switch(i)
			{
				case anquanmen2_port:
				case anquanmen_port:
					previous_error_status_w=1;
					if (anquanmen_alarm_level!=level_0)
					{
						if (privilege_run_flag==0)
							alarm_disp(anquanmen_port);
						if (anquanmen_alarm_level==level_3)
						{
							emer_stop_flag=1;
							run_permite_flag=0;
						}							
						else if (anquanmen_alarm_level==level_2)
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
					{
						err_flag=0;
						
					}
					break;
				case shangduansha_port:
					previous_error_status_w=2;
					if (qz_cuowu_cishu[shangduansha_port]<2){
						qz_cuowu_cishu[shangduansha_port]++;break;
					}
					else{
						if (shangduansha_alarm_level!=level_0)
						{
							if (privilege_run_flag==0)
								alarm_disp(shangduansha_port);
							if (shangduansha_alarm_level==level_3)
							{
								emer_stop_flag=1;
								run_permite_flag=0;
							}							
							else if (shangduansha_alarm_level==level_2)
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
						{
							err_flag=0;
							
						}
						break;
					}
				case queyou_port:
					youben_permite_button=0;
					previous_error_status_w=3;
					if (queyou_alarm_level!=level_0)
					{							
						if (privilege_run_flag==0)
							alarm_disp(queyou_port);
						if (queyou_alarm_level==level_3)
						{
							emer_stop_flag=1;
							run_permite_flag=0;
						}							
						else if (queyou_alarm_level==level_2)
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
					{
						err_flag=0;
						
					}
					break;
				case tanzhen_port:
					previous_error_status_w=4;
					if (tanzhen_alarm_level!=level_0)
					{
						if (privilege_run_flag==0)
							alarm_disp(tanzhen_port);
						if (tanzhen_alarm_level==level_3)
						{
							emer_stop_flag=1;
							run_permite_flag=0;
						}							
						else if (tanzhen_alarm_level==level_2)
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
					{
						err_flag=0;
					}
					break;
				case tanbu_port:
					previous_error_status_w=5;
					if (tanbu_alarm_level!=level_0)
					{
						if (privilege_run_flag==0)
							alarm_disp(tanbu_port);
						if (tanbu_alarm_level==level_3)
						{
							emer_stop_flag=1;
							run_permite_flag=0;
						}							
						else if (tanbu_alarm_level==level_2)
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
					{
						err_flag=0;
					
 					}
					break;
			}
			if (err_flag==1)
				break;
		}
		
		/**X���޾���**/
		else
		{
			qz_cuowu_cishu[i]=0;
		}
			
	}
	
	/**��������֮���ⲿֹͣ��־λ��1,14����ģ�ǣ�
	**/
	if (((jianshu_ewaiduan_check==1 && dingdan_lianghua_num_kw>=previous_dingdanzongshu) || \
	   (dingdan_lianghua_num_kw>=previous_dingdanzongshu&&extra_part_jiansu==0) || \
	   (dingdan_lianghua_num_kw>=previous_dingdanzongshu && extra_part_jiansu!=0 && jianshu%extra_part_jiansu!=0))\
	   &&previous_dingdanzongshu!=0&&err_flag!=1)
	{
		err_flag=1;
		previous_error_status_w=7;
		if (privilege_run_flag==0)
			alarm_disp(14);
		qz_error_status=1;
		emer_stop_flag=1;
		run_permite_flag=0;
	}
	
	/**��������֮�󣬲��Ҽ����������ޣ�jishu_max_flag = 1���ⲿֹͣ��־λ��1,13����ģ�ǣ�
	**/
	if (((jianshu_ewaiduan_check==1&&jianshu>=zhibusheding) || \
	   (jianshu>=zhibusheding&&extra_part_jiansu==0) || \
	   (jianshu>=zhibusheding && extra_part_jiansu!=0 && jianshu%extra_part_jiansu!=0))&&err_flag!=1)
	{
		err_flag=1;
		previous_error_status_w=6;
		if (privilege_run_flag==0)
			alarm_disp(13);			//strcpy(Info_qz[6],jianshu_max);
		jishu_max_flag=0;
		qz_error_status=1;
		emer_stop_flag=1;
		run_permite_flag=0;
	}
	else
		jishu_max_flag=1;
	
	/**�峵Ȧ�����֮��(eg:ת��nȦ֮��ͣ��)���ⲿֹͣ��־λ��1,15����ģ�ǣ�
	**/
	if (qingche_num_kw>=qingchesheding && qingchesheding!=0 &&err_flag!=1)
	{
		err_flag=1;
		previous_error_status_w=8;
		if (privilege_run_flag==0)
			alarm_disp(15);
		qz_error_status=1;
		emer_stop_flag=1;
		run_permite_flag=0;
	}
	
	/**�ݴ�**/
	if (i>max_port && err_flag==0)
	{
		emer_stop_flag=0;
		sys_force_run_button=0;
		qz_error_status=0;
		run_permite_flag=1;
		privilege_run_flag=0;
	}
	ext_start_status=		Get_X_Value(ext_start_port);					
	ext_stop_status=		Get_X_Value(ext_stop_port);						
	ext_jog_status=			Get_X_Value(ext_jog_port);	
}


/*************************************************
Function(��������): alarm_disp(U8 which_alarm)
Description(�������ܡ����ܵȵ�����): ������ʾ���棬ͬʱ�򿪱�������������ʱ
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
void alarm_disp(U8 which_alarm)
{
	//char Info[10][10];  //CharString:��������ϡ�
	char QueDing[]={40,23,22,8};			 //CharString:��ȷ��
	INT16U LeftX,LeftY;
	LeftX = 200;
	LeftY = 150;
	if(xianshi_flag==0)
	{
		Lcd_Fill_Box (LeftX, LeftY, 440, 330, 4);
		//*(Info_qz+which_alarm-min_port)Ϊzhongduan_init()�е���ģ����
		Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300,5,13,4,3,0,1,0,0,*(Info_qz+which_alarm-min_port));

		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,RAISE);
		
		Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, 0, 7,2,0,0,0,3,QueDing);
		
		xianshi_flag=1;
	}
	delay_qz(0,10,1);	//��beep_alarm��
	
}

/*************************************************
Function(��������): beep_alarm(void)
Description(�������ܡ����ܵȵ�����): �������������ʼ������������function.c�е�KeyScan(void)��������������
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
void beep_alarm(void)
{
	if (delay_fac.delay_permit[0]==1)
	{
		if (delay_0_count<5)//&&beep_status!=0)
		{
			Beep(1);
			beep_status=1;
		}
		else //if(delay_0_count<10)//&&beep_status!=1)
		{
			Beep(0);
			beep_status=0;
		}
		
	}
}


