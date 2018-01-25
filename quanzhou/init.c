#include"includes.h"

char Info_qz[9][11]={0};
unsigned int SET_HIGH_STATUS = 0; //调线功能

/*************************************************
Function(函数名称): jidianqi_init(void)
Description(函数功能、性能等的描述): 将画面的照布灯和照明灯按键置1
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): App_Init

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
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
Function(函数名称): encoder1_init(void)
Description(函数功能、性能等的描述): 将X2作为GPIO普通输入，
								 并且获取signal的初始值(应该没什么用)，并且打开X1中断
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
void encoder1_init(void)
{
	/**将GPF2清零(X2)**/
	rGPFCON &= (~(3<<4));						//为了接B相
	signal=Get_X_Value(2);
	encoder1_fun(1);							//三目运算符，打开X1中断
}

/*************************************************
Function(函数名称): menu_init(void)
Description(函数功能、性能等的描述): 对TS上变量进行初始化
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): App_Init

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void menu_init(void)
{
	int ban,lei;
	int ii,bb;
	if (menu_init_flag!=1)
	{
		zhibusheding=2000;				//对应件数上限
		qingchesheding=0;				//对应
		qingche_num_kw=0;
		
		youbeng_miaojianxie_fac_L=3;	//油泵秒间歇运行时间
		youbeng_miaojianxie_fac_H=500;	//油泵秒间歇停止时间
		youbeng_quanjianxie_fac_L=20;	//油泵圈间歇运行圈数
		youbeng_quanjianxie_fac_H=1;	//油泵秒间歇停止权属
		
		fenshan_jianxie_fac_L=200;		//风扇工作圈数
		fenshan_jianxie_fac_H=500;		//风扇间歇圈数
		
		bianpingqi_jog_speed_lv=10;		//变频器点动速度
		
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
		
		mode_choose = seven_motor_mode; 	//模式选择 by FJW
		
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
		
		/*************8段调线设置*****************/
		for (ii = 0; ii < tiaoshaduan_max; ii++){
			g_InteralMemory.KeepWord[156 + 10*ii] = 0;			//开始圈
			g_InteralMemory.KeepWord[157 + 10*ii] = 0;			//结束圈
			g_InteralMemory.KeepWord[158 + 10*ii] = 0;			//调线选择
			g_InteralMemory.KeepBit[49 + ii] = 0;
			
			//上部大送纱轮、上部小送砂轮、下部大送纱轮、下部小送砂轮
			//卷布机、五号送砂轮、六号送砂轮 循环设置(百分比)
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
		
		weisha_jiange_kw = 0;	//喂纱间隔
	/**********************************************/	
		
		menu_init_flag=1;
	}
	
	
	sys_permite_button=0;
	sys_force_run_button=0;
	previous_dingdanbianhao=dingdanbianhao_kw;
	previous_dingdanzongshu=dingdanzongshu_kw;
	next_banci_status=banci_status_kw;
	g_InteralMemory.Word[40]=8888;			//管理员密码设置
}

/*************************************************
Function(函数名称): zhongduan_init(void)
Description(函数功能、性能等的描述): 为报警提供字模
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): App_Init

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
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
	
	//将X3，X4，X5设置为输入模式
	tmp = rGPFCON & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)); //& (~(0x3<< 12)) & (~(0x3<< 14));
	rGPFCON = tmp | (0x0<<6) | (0x0<<8)  | (0x0<<10);//| (0x2<<12) | (0x2<<14);	
}

/*************************************************
Function(函数名称): bianpingqi_init(void)
Description(函数功能、性能等的描述): 上电初始化变频器，使得变频器起初不工作，报警信号Y2置零
Calls (被本函数调用的函数清单): bianpingqi_stop_sub();Set_Y_Value(2,0);
Called By (调用本函数的函数清单): App_Init

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
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
Function(函数名称): qz_uart_init(void)
Description(函数功能、性能等的描述): 初始化串口参数,放在一开始就初始化，
								 则可以直接使用485通讯，而不需要在触摸屏上选择COM4
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): App_Init 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
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
Function(函数名称): stepmotor_init(void)
Description(函数功能、性能等的描述): 7路电机的参数初始化、K值初始化
Calls (被本函数调用的函数清单): parameter_read();songsha_fre_change();
Called By (调用本函数的函数清单): App_Init()

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void stepmotor_init(void)
{
	//TS Flash擦除之后，上电只需要的一次设置
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
				
		/***重要：以下变量无法直接查找到，在songsha_freq_change()中通过数组循环使用***/
		g_InteralMemory.KeepWord[103]=100;			//下部大送砂轮_挡片百分比
		g_InteralMemory.KeepWord[104]=100;			//上部大送砂轮_挡片百分比
		g_InteralMemory.KeepWord[105]=100;			//下部小送砂轮_挡片百分比
		g_InteralMemory.KeepWord[106]=100;			//上部小送砂轮_挡片百分比
		g_InteralMemory.KeepWord[107]=100;          //卷布机_挡片百分比
		g_InteralMemory.KeepWord[108]=100;          //五号送砂轮_挡片百分比
		g_InteralMemory.KeepWord[109]=100;          //六号送砂轮_挡片百分比
		                                          
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
	
	//上电之后，将flash中的值赋给变量
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
	
	//算式一：计算送纱量，用于显示送了多少纱长，大小齿轮的传动比乘以直径
	songsha_rate[0]=songsha_rate[1]=songsha_dapanchilunshu*1.0/songsha_xiaochilunshu*songsha_lvpanzhijin;
	songsha_rate[2]=songsha_rate[3]=songsha_xiaopanchilunshu*1.0/songsha_xiaochilunshu*songsha_lvpanzhijin;
	
	songsha_fre_change();
}
