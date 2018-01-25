#include"includes.h"

char Info_qz[9][11]={0};
unsigned int SET_HIGH_STATUS = 0; //���߹���

/*************************************************
Function(��������): jidianqi_init(void)
Description(�������ܡ����ܵȵ�����): ��������ղ��ƺ������ư�����1
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): App_Init

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void jidianqi_init(void)
{
	if (jidianqi_init_flag!=1)
	{
		jidianqi_init_flag=1;
	}
	zhaobudeng_button=1;
	zhaomingdeng_button=1;
	
}

/*************************************************
Function(��������): encoder1_init(void)
Description(�������ܡ����ܵȵ�����): ��X2��ΪGPIO��ͨ���룬
								 ���һ�ȡsignal�ĳ�ʼֵ(Ӧ��ûʲô��)�����Ҵ�X1�ж�
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
void encoder1_init(void)
{
	/**��GPF2����(X2)**/
	rGPFCON &= (~(3<<4));						//Ϊ�˽�B��
	signal=Get_X_Value(2);
	encoder1_fun(1);							//��Ŀ���������X1�ж�
}

/*************************************************
Function(��������): menu_init(void)
Description(�������ܡ����ܵȵ�����): ��TS�ϱ������г�ʼ��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): App_Init

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void menu_init(void)
{
	int ban,lei;
	int ii,bb;
	if (menu_init_flag!=1)
	{
		zhibusheding=2000;				//��Ӧ��������
		qingchesheding=0;				//��Ӧ
		qingche_num_kw=0;
		
		youbeng_miaojianxie_fac_L=3;	//�ͱ����Ъ����ʱ��
		youbeng_miaojianxie_fac_H=500;	//�ͱ����Ъֹͣʱ��
		youbeng_quanjianxie_fac_L=20;	//�ͱ�Ȧ��Ъ����Ȧ��
		youbeng_quanjianxie_fac_H=1;	//�ͱ����ЪֹͣȨ��
		
		fenshan_jianxie_fac_L=200;		//���ȹ���Ȧ��
		fenshan_jianxie_fac_H=500;		//���ȼ�ЪȦ��
		
		bianpingqi_jog_speed_lv=10;		//��Ƶ���㶯�ٶ�
		
		daduanquanshu=160;
		middlequanshu=90;
		xiaoduanquanshu=110;
		caijiaoquanshu=76;
		langfeiquanshu=5;
		bianpingqi_huanchongquan_num=2;
		extra_part_jiansu=0;
		extra_part_quanshu=100;
		extra_fencen_quan_num_kw=2;
		
		shangduansha_alarm_level=level_3;
		anquanmen_alarm_level=level_3;
		queyou_alarm_level=level_3;
		tanzhen_alarm_level=level_3;
		
		tanbu_alarm_level=level_3;
		
		mode_choose = seven_motor_mode; 	//ģʽѡ�� by FJW
		
		qz_guanliyuan_mima=12345;
		a_ban_mima_kw=12345;
		b_ban_mima_kw=12345;
		c_ban_mima_kw=12345;
		banci_status_kw=65535;
		a_ban_kb=0;
		b_ban_kb=0;
		c_ban_kb=0;
		xitong_total_num_upper_kw=0;
		total_quanshu=0;
		dingdan_lianghua_num_kw=0;
		for (ban=a_ban;ban<=c_ban;ban++)
			for (lei=xiada_no;lei<=jianshu_no;lei++)
				g_InteralMemory.KeepWord[banci_num[ban][lei]]=0;
		dingdanbianhao_kw=0;
		dingdanzongshu_kw=1000;
				
		parameter_save();
		
		bianpingqi_fullspeed_set=500;
		bianpingqi_delta_num=50;
		bianpingqi_fencen_speed_set=500;
		bianpingqi_tiaoxian_speed_set = 500;
		bianpingqi_weisha_delta_num = 100;
		
		xifenshu=800;
		jianshu=0;
		youben_choose=youben_old;
		dida_num=5;
		
		speedUpMax = 5;
		speedDownMax=2;
		huanchongmaichong=1000;
		
		Choose_bianpingqi_kb=CHOOSE_BIANPINGQI;
		bianpingqi_zhonglei = shilin_bianpingqi;
		
		dapan_zongzhenshu_kw=600;
		
		shangduansha_alarm_sig_kb=0;
		anquanmen_alarm_sig_kb=0;
		queyou_alarm_sig_kb=0;
		tanzhen_alarm_sig_kb=0;
		tanbu_alarm_sig_kb=0;
		
		reset_time_kw = 700;
		init_stop_time_kw = 30;
		
		/*************8�ε�������*****************/
		for (ii = 0; ii < tiaoshaduan_max; ii++){
			g_InteralMemory.KeepWord[156 + 10*ii] = 0;			//��ʼȦ
			g_InteralMemory.KeepWord[157 + 10*ii] = 0;			//����Ȧ
			g_InteralMemory.KeepWord[158 + 10*ii] = 0;			//����ѡ��
			g_InteralMemory.KeepBit[49 + ii] = 0;
			
			//�ϲ�����ɴ�֡��ϲ�С��ɰ�֡��²�����ɴ�֡��²�С��ɰ��
			//�����������ɰ�֡�������ɰ�� ѭ������(�ٷֱ�)
			for (bb = 0; bb<7; bb++){
				g_InteralMemory.KeepWord[159 + 10*ii + bb] = 100;
			}
		}
		/******************************************/
		
		tiaoxianzu_jiange = 2;
		chudao_jiange = 10000;	//by FJW
		shoudao_jiange = 10000; //by FJW
		tiaoxiankaiguan_kb = 0;
		tiaoxianzu_max = 5;
		tiaoxianzu = 1;
		tiaoxianzu_quanshu=0;
		tiaoxianzu_flag = 0;
		
		jiajiansujiangemaichong_kw = 10000;
		
		weisha_jiange_kw = 0;	//ιɴ���
	/**********************************************/	
		
		menu_init_flag=1;
	}
	
	
	sys_permite_button=0;
	sys_force_run_button=0;
	previous_dingdanbianhao=dingdanbianhao_kw;
	previous_dingdanzongshu=dingdanzongshu_kw;
	next_banci_status=banci_status_kw;
	g_InteralMemory.Word[40]=8888;			//����Ա��������
}

/*************************************************
Function(��������): zhongduan_init(void)
Description(�������ܡ����ܵȵ�����): Ϊ�����ṩ��ģ
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): App_Init

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void zhongduan_init(void)
{
	DWORD tmp;
	char shangduansha[]=	{41,47,22,47,41,20,20,77,46,83,'\0'};
	char anquanmen[]=		{16,18,40,11,35,37,20,77,46,83,'\0'};
	char queyou[]=			{40,17,51,45,20,77,46,83,33,43,'\0'};
	char tanzhen[]=			{44,29,53,75,20,77,46,83,33,43,'\0'};
	char xiaduansha[]=		{47,34,22,47,41,20,20,77,46,83,'\0'};
	char tanbu[]=			{44,29,18,28,20,77,46,83,33,43,'\0'};
	char jianshu_max[]=		{28,94,42,93,21,29,41,47,47,62,'\0'};
	char dingdan_max[]=		{22, 9,21, 5,50,49,45,74,19,41,'\0'};
	char qingche_error[]=	{39,69,19,21,40, 6,42,93,21,29,'\0'};
	//char yazheng_error[]=	{};
		
	strcpy(Info_qz[shangduansha_port-min_port],shangduansha);
	strcpy(Info_qz[anquanmen_port-min_port],anquanmen);
	strcpy(Info_qz[queyou_port-min_port],queyou);
	strcpy(Info_qz[tanzhen_port-min_port],tanzhen);
	strcpy(Info_qz[xiaduansha_port-min_port],xiaduansha);
	strcpy(Info_qz[tanbu_port-min_port],tanbu);
	strcpy(Info_qz[6],jianshu_max);
	strcpy(Info_qz[7],dingdan_max);
	strcpy(Info_qz[8],qingche_error);
	//strcpy(Info_qz[9],yazheng_error);
	
	//��X3��X4��X5����Ϊ����ģʽ
	tmp = rGPFCON & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)); //& (~(0x3<< 12)) & (~(0x3<< 14));
	rGPFCON = tmp | (0x0<<6) | (0x0<<8)  | (0x0<<10);//| (0x2<<12) | (0x2<<14);	
}

/*************************************************
Function(��������): bianpingqi_init(void)
Description(�������ܡ����ܵȵ�����): �ϵ��ʼ����Ƶ����ʹ�ñ�Ƶ������������������ź�Y2����
Calls (�����������õĺ����嵥): bianpingqi_stop_sub();Set_Y_Value(2,0);
Called By (���ñ������ĺ����嵥): App_Init

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void bianpingqi_init(void)
{
	
	if (bianpingqi_init_flag!=1)
	{
		bianpingqi_init_flag=1;
	}
	
	bianpingqi_stop_sub();
	Set_Y_Value(2,0);
}

/*************************************************
Function(��������): qz_uart_init(void)
Description(�������ܡ����ܵȵ�����): ��ʼ�����ڲ���,����һ��ʼ�ͳ�ʼ����
								 �����ֱ��ʹ��485ͨѶ��������Ҫ�ڴ�������ѡ��COM4
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): App_Init 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void qz_uart_init(void)
{
	rUFCON3 = 0x0;
	rUMCON3 = 0x0;
	rULCON3 = 0x2B;
	rUCON3  = 0x005;
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./9600+0.5) -1 );
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./57600+0.5) -1 );
}

/*************************************************
Function(��������): stepmotor_init(void)
Description(�������ܡ����ܵȵ�����): 7·����Ĳ�����ʼ����Kֵ��ʼ��
Calls (�����������õĺ����嵥): parameter_read();songsha_fre_change();
Called By (���ñ������ĺ����嵥): App_Init()

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void stepmotor_init(void)
{
	//TS Flash����֮���ϵ�ֻ��Ҫ��һ������
	if(stepmotor_init_flag!=1)
	{
		datou_rate_different_1=500;
		datou_rate_different_2=500;
		datou_rate_different_3=500;
		datou_rate_different_4=500;
		datou_rate_different_5=500;
		datou_rate_different_6=500;
		datou_rate_different_7=500;
		
		xiaotou_rate_different_1=500;
		xiaotou_rate_different_2=500;
		xiaotou_rate_different_3=500;
		xiaotou_rate_different_4=500;
		xiaotou_rate_different_5=500;
		xiaotou_rate_different_6=500;
		xiaotou_rate_different_7=500;
		
		fencen_rate_different_1=500;
		fencen_rate_different_2=500;
		fencen_rate_different_3=500;
		fencen_rate_different_4=500;
		fencen_rate_different_5=500;
		fencen_rate_different_6=500;
		fencen_rate_different_7=500;
				
		/***��Ҫ�����±����޷�ֱ�Ӳ��ҵ�����songsha_freq_change()��ͨ������ѭ��ʹ��***/
		g_InteralMemory.KeepWord[103]=100;			//�²�����ɰ��_��Ƭ�ٷֱ�
		g_InteralMemory.KeepWord[104]=100;			//�ϲ�����ɰ��_��Ƭ�ٷֱ�
		g_InteralMemory.KeepWord[105]=100;			//�²�С��ɰ��_��Ƭ�ٷֱ�
		g_InteralMemory.KeepWord[106]=100;			//�ϲ�С��ɰ��_��Ƭ�ٷֱ�
		g_InteralMemory.KeepWord[107]=100;          //����_��Ƭ�ٷֱ�
		g_InteralMemory.KeepWord[108]=100;          //�����ɰ��_��Ƭ�ٷֱ�
		g_InteralMemory.KeepWord[109]=100;          //������ɰ��_��Ƭ�ٷֱ�
		                                          
		songsha1_num=0;
		songsha2_num=0;
		songsha3_num=0;
		songsha4_num=0;
		songsha_1=0;
		songsha_2=0;
		songsha_3=0;
		songsha_4=0;
		shachang_upper_num_1_kw=0;
		shachang_upper_num_2_kw=0;
		shachang_upper_num_3_kw=0;
		shachang_upper_num_4_kw=0;

		
		songsha_dapanchilunshu=108;
		songsha_xiaopanchilunshu=81;
		songsha_lvpanzhijin=137;
		songsha_xiaochilunshu=27;
		
		max_speed=120;
		
		qigang_zhonglei_choose=xiamian;
		
		
		encoder1_cal_factor=20000;
		stepmotor_init_flag=1;
	}
	
	//�ϵ�֮�󣬽�flash�е�ֵ��������
	parameter_read();
	rate_different[0][datou]=datou_rate_different_1;
	rate_different[1][datou]=datou_rate_different_2;
	rate_different[2][datou]=datou_rate_different_3;
	rate_different[3][datou]=datou_rate_different_4;
	rate_different[4][datou]=datou_rate_different_5;
	rate_different[5][datou]=datou_rate_different_6;
	rate_different[6][datou]=datou_rate_different_7;
		
	rate_different[0][xiaotou]=xiaotou_rate_different_1;
	rate_different[1][xiaotou]=xiaotou_rate_different_2;
	rate_different[2][xiaotou]=xiaotou_rate_different_3;
	rate_different[3][xiaotou]=xiaotou_rate_different_4;
	rate_different[4][xiaotou]=xiaotou_rate_different_5;
	rate_different[5][xiaotou]=xiaotou_rate_different_6;
	rate_different[6][xiaotou]=xiaotou_rate_different_7;
		
	rate_different[0][fencen]=fencen_rate_different_1;
	rate_different[1][fencen]=fencen_rate_different_2;
	rate_different[2][fencen]=fencen_rate_different_3;
	rate_different[3][fencen]=fencen_rate_different_4;
	rate_different[4][fencen]=fencen_rate_different_5;
	rate_different[5][fencen]=fencen_rate_different_6;
	rate_different[6][fencen]=fencen_rate_different_7;
	
	//��ʽһ��������ɴ����������ʾ���˶���ɴ������С���ֵĴ����ȳ���ֱ��
	songsha_rate[0]=songsha_rate[1]=songsha_dapanchilunshu*1.0/songsha_xiaochilunshu*songsha_lvpanzhijin;
	songsha_rate[2]=songsha_rate[3]=songsha_xiaopanchilunshu*1.0/songsha_xiaochilunshu*songsha_lvpanzhijin;
	
	songsha_fre_change();
}
