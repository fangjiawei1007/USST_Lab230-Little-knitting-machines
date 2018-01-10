#include"includes.h"

U8 previous_youbeng_status=1;
U8 previous_fenshan_status=1;

U8 youbeng_quan_init_flag=0;
U8 fenshan_quan_init_flag=0;

U32 youbeng_quanjianxie_yizhuan_num=0;


U8	guanliyuan_mima_status=0;

float songsha_rate[4]={0};
U16 songsha_num[7]={0};
unsigned char youbeng_finish_dida=1;
U16	youbeng_dida_num=0;
U8	youbeng_delay=0;

INT16U* banci[5]={NULL};
INT16U* banci_mima[3]={&a_ban_mima_kw,&b_ban_mima_kw,&c_ban_mima_kw};
const unsigned char banci_num[3][5]={{110,111,112,113,114},{115,116,117,118,119},{120,121,122,123,124}};
unsigned short s_banci[5];//初始化时没有班次登录，用特殊班次使指针非空
INT16U next_banci_status;
int banci_mima_shuru;
unsigned char first_banci_flag;
INT16U previous_dingdanbianhao;
INT16U previous_dingdanzongshu;
void youbeng_sys_fun(void)
{
	U32	youbeng_work_fac;
	if (youbeng_lianxu_button!=1 && youbeng_miaojianxie_button!=1 && youbeng_quanjianxie_button!=1)
	{
		youbeng_lianxu_button=1;
		previous_youbeng_status=1;
	}
	switch (previous_youbeng_status)
	{
		case 1:
				if (youbeng_miaojianxie_button==1||youbeng_quanjianxie_button==1)
				{
					youbeng_lianxu_button=0;
					if (youbeng_miaojianxie_button==1)
						previous_youbeng_status=2;
					else
						previous_youbeng_status=3;
				}break;
		case 2:
				if (youbeng_lianxu_button==1||youbeng_quanjianxie_button==1)
				{
					youbeng_miaojianxie_button=0;
					if (youbeng_lianxu_button==1)
						previous_youbeng_status=1;					
					else
						previous_youbeng_status=3;
				}break;
		case 3:
				if (youbeng_lianxu_button==1||youbeng_miaojianxie_button==1)
				{
					youbeng_quanjianxie_button=0;
					if (youbeng_lianxu_button==1)
						previous_youbeng_status=1;	
					else
						previous_youbeng_status=2;
				}break;
	}
	if (youben_permite_button==1)
	{
		if (previous_youbeng_status==1)
		{
			youbeng_quan_init_flag=0;
			youbeng_quanjianxie_yizhuan_num=0;
			if (delay_fac.delay_permit[2]==1)
				delay_qz(2,0,0);
			youbeng_button=1;
		}
		else if (previous_youbeng_status==2)
		{
			youbeng_quan_init_flag=0;
			youbeng_quanjianxie_yizhuan_num=0;
			youbeng_work_fac=(youbeng_miaojianxie_fac_L+youbeng_miaojianxie_fac_H)*10;
			delay_qz(2,youbeng_work_fac,1);
			if (delay_2_count<(youbeng_miaojianxie_fac_L*10))
				youbeng_button=1;
			else if (delay_2_count<youbeng_work_fac)
				youbeng_button=0;
		}
		else if (previous_youbeng_status==3)
		{
			youbeng_quan_init_flag=1;
			if (delay_fac.delay_permit[2]==1)
				delay_qz(2,0,0);
			youbeng_work_fac=youbeng_quanjianxie_fac_L+youbeng_quanjianxie_fac_H;
			if (youbeng_quanjianxie_yizhuan_num<youbeng_quanjianxie_fac_L)
				youbeng_button=1;
			else	if (youbeng_quanjianxie_yizhuan_num<youbeng_work_fac)
				youbeng_button=0;
			else
				youbeng_quanjianxie_yizhuan_num=0;
		}
	}
	else
	{
		if (delay_fac.delay_permit[2]==1)
			delay_qz(2,0,0);
		youbeng_button=0;
	}
	youbeng_fun();
}

void fenshan_sys_fun(void){
	U32	fenshan_work_fac;
	if (fenshan_lianxu_button!=1 && fenshan_jianxie_1_button!=1 && fenshan_jianxie_2_button!=1)
	{
		fenshan_lianxu_button=1;
		previous_fenshan_status=1;
	}
	switch (previous_fenshan_status)
	{
		case 1:
				if (fenshan_jianxie_1_button==1||fenshan_jianxie_2_button==1)
				{
					fenshan_lianxu_button=0;
					if (fenshan_jianxie_1_button==1)
						previous_fenshan_status=2;
					else
						previous_fenshan_status=3;
				}break;
		case 2:
				if (fenshan_lianxu_button==1||fenshan_jianxie_2_button==1)
				{
					fenshan_jianxie_1_button=0;
					if (fenshan_lianxu_button==1)
						previous_fenshan_status=1;					
					else
						previous_fenshan_status=3;
				}break;
		case 3:
				if (fenshan_lianxu_button==1||fenshan_jianxie_1_button==1)
				{
					fenshan_jianxie_2_button=0;
					if (fenshan_lianxu_button==1)
						previous_fenshan_status=1;	
					else
						previous_fenshan_status=2;
				}break;
	}
	if (fengshan_permite_button==1)
	{
		if (previous_fenshan_status==1)
		{
			fenshan_quan_init_flag=0;
			fenshan_jianxie_yizhuanquan_num=0;
			fenshan_button=1;
		}
		else if (previous_fenshan_status==2)
		{
			fenshan_quan_init_flag=1;
			fenshan_work_fac=fenshan_jianxie_fac_H+fenshan_jianxie_fac_L;
			if (fenshan_jianxie_yizhuanquan_num<fenshan_jianxie_fac_L)
				fenshan_button=1;
			else	if (fenshan_jianxie_yizhuanquan_num<fenshan_work_fac)
				fenshan_button=0;
			else
				fenshan_jianxie_yizhuanquan_num=0;
		}
		else if (previous_fenshan_status==3)
		{
			fenshan_quan_init_flag=1;
			fenshan_work_fac=fenshan_jianxie_fac_H+fenshan_jianxie_fac_L;
			if (fenshan_jianxie_yizhuanquan_num<fenshan_jianxie_fac_H)
				fenshan_button=1;
			else	if (fenshan_jianxie_yizhuanquan_num<fenshan_work_fac)
				fenshan_button=0;
			else
				fenshan_jianxie_yizhuanquan_num=0;
		}
	}
	else
		fenshan_button=0;
	fenshan_fun();
}

/*************************************************
Function(函数名称): qz_guanliyuan(void)
Description(函数功能、性能等的描述): 管理员密码进入管理员界面
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
void qz_guanliyuan(void){
	if (guanliyuan_jiemian_queding==1)
	{
		if (qz_mima_shuru==qz_guanliyuan_mima)
		{
			g_InteralMemory.Word[PING_CTRL]=6;	//跳转至6号画面：管理员大头
			qz_mima_shuru=0;
		}
		else	
		{
			g_InteralMemory.Word[PING_CTRL]=30;
			qz_mima_shuru=0;
		}
		guanliyuan_jiemian_queding=0;
	}
	if (guanliyuan_mima_reset==1)
		qz_guanliyuan_mima=12345;
	if (a_ban_mima_reset_b==1)
		a_ban_mima_kw=12345;
	if (b_ban_mima_reset_b==1)
		b_ban_mima_kw=12345;
	if (c_ban_mima_reset_b==1)
		c_ban_mima_kw=12345;
}

/*************************************************
Function(函数名称): shachang_xianshi(void)
Description(函数功能、性能等的描述): 先不看
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
void shachang_xianshi(void){
	unsigned char i;
	for (i=0;i<7;i++)
	{
		if (songsha_num[i]>=xifenshu)
		{
			switch (i)
			{
				case 0:
					if (banci_status_kw!=s_ban)
						songsha_1++;
					*banci[0]=*banci[0]+1;
					break;
				case 1:
					if (banci_status_kw!=s_ban)
						songsha_2++;
					*banci[1]=*banci[1]+1;
					break;
				case 2:
					if (banci_status_kw!=s_ban)
						songsha_3++;
					*banci[2]=*banci[2]+1;
					break;
				case 3:
					if (banci_status_kw!=s_ban)
						songsha_4++;
					*banci[3]=*banci[3]+1;
					break;
			}
			songsha_num[i]=0;
		}
	}
	shachang_1 =(*banci[0])*songsha_rate[0]/1000.0;
	shachang_2 =(*banci[1])*songsha_rate[1]/1000.0;
	shachang_3 =(*banci[2])*songsha_rate[2]/1000.0;
	shachang_4 =(*banci[3])*songsha_rate[3]/1000.0;
	
	total_shachang_1_xianshi_w =songsha_1*songsha_rate[0]/1000.0;
	if (total_shachang_1_xianshi_w>65530){
		shachang_upper_num_1_kw++;
		songsha_1=0;
	}
	total_shachang_2_xianshi_w =songsha_2*songsha_rate[1]/1000.0;
	if (total_shachang_2_xianshi_w>65530){
		shachang_upper_num_2_kw++;
		songsha_2=0;
	}
	total_shachang_3_xianshi_w =songsha_3*songsha_rate[2]/1000.0;
	if (total_shachang_3_xianshi_w>65530){
		shachang_upper_num_3_kw++;
		songsha_3=0;
	}
	total_shachang_4_xianshi_w =songsha_4*songsha_rate[3]/1000.0;
	if (total_shachang_4_xianshi_w>65530){
		shachang_upper_num_4_kw++;
		songsha_4=0;
	}
}

void youbeng_new_way(void){
	U32	youbeng_work_fac;
	if (youbeng_lianxu_button!=1 && youbeng_miaojianxie_button!=1 && youbeng_quanjianxie_button!=1)
	{
		youbeng_miaojianxie_button=1;
		previous_youbeng_status=2;
	}
	switch (previous_youbeng_status)
	{
		case 1:
				youbeng_miaojianxie_button=1;
				previous_youbeng_status=2;
				break;
		case 2:
				if (youbeng_lianxu_button==1||youbeng_quanjianxie_button==1)
				{
					youbeng_miaojianxie_button=0;
					if (youbeng_lianxu_button==1)
					{
						youbeng_lianxu_button=0;
						previous_youbeng_status=2;
						youbeng_miaojianxie_button=1;
					}
					else
						previous_youbeng_status=3;
				}break;
		case 3:
				if (youbeng_lianxu_button==1||youbeng_miaojianxie_button==1)
				{
					youbeng_quanjianxie_button=0;
					if (youbeng_lianxu_button==1)
					{
						youbeng_lianxu_button=0;
						previous_youbeng_status=3;
						youbeng_quanjianxie_button=1;
					}
					else
						previous_youbeng_status=2;
				}break;
	}
	if (youben_permite_button==1)
	{
		if (previous_youbeng_status==1)
		{
			previous_youbeng_status=2;
		}
		else	if (previous_youbeng_status==2)
		{
			youbeng_quan_init_flag=0;
			youbeng_quanjianxie_yizhuan_num=0;
			youbeng_work_fac=youbeng_miaojianxie_fac_H*10;
			if (youbeng_finish_dida==1)
				delay_qz(2,youbeng_work_fac,1);
			if (delay_2_count>=(youbeng_work_fac-1))
			{
				youbeng_finish_dida=0;
				delay_qz(2,youbeng_work_fac,0);
			}
			if (youbeng_finish_dida==0)
			{
				delay_qz(4,30,1);
				if (delay_4_count<10)
					youbeng_button=1;
				else
				{
					if (youbeng_button==1)
					{
						youbeng_dida_num++;
					}
					youbeng_button=0;
				}
				youbeng_fun();
				if (youbeng_dida_num>=dida_num)
				{
					youbeng_finish_dida=1;
					youbeng_dida_num=0;
					delay_qz(4,30,0);
				}
			}
		}
		else if (previous_youbeng_status==3)
		{
			if (youbeng_finish_dida==1)
				youbeng_quan_init_flag=1;
			if (delay_fac.delay_permit[2]==1)
				delay_qz(2,0,0);
			youbeng_work_fac=youbeng_quanjianxie_fac_H;
			if (youbeng_quanjianxie_yizhuan_num>=youbeng_work_fac)
			{
				youbeng_finish_dida=0;
				youbeng_quanjianxie_yizhuan_num=0;
				youbeng_quan_init_flag=0;
			}
			if (youbeng_finish_dida==0)
			{
				delay_qz(4,30,1);
				if (delay_4_count<10)
					youbeng_button=1;
				else
				{
					if (youbeng_button==1)
					{
						youbeng_dida_num++;
					}
					youbeng_button=0;
				}
				youbeng_fun();
				if (youbeng_dida_num>=dida_num)
				{
					youbeng_finish_dida=1;
					youbeng_dida_num=0;
					delay_qz(4,30,0);
				}
			}
		}
	}
	else
	{
		if (delay_fac.delay_permit[2]==1)
			delay_qz(2,0,0);
		if (delay_fac.delay_permit[4]==1)
			delay_qz(4,30,0);
		youbeng_quanjianxie_yizhuan_num=0;
		youbeng_button=0;
		youbeng_finish_dida=1;
		youbeng_dida_num=0;
		youbeng_fun();
	}
}

void restore_banci_button_status(void){
	if (banci_status_kw==s_ban)
	{
		a_ban_kb=0;
		b_ban_kb=0;
		c_ban_kb=0;
	}
	else if (banci_status_kw==a_ban)
	{
		a_ban_kb=1;
		b_ban_kb=0;
		c_ban_kb=0;
	}
	else if (banci_status_kw==b_ban)
	{
		a_ban_kb=0;
		b_ban_kb=1;
		c_ban_kb=0;
	}
	else if (banci_status_kw==c_ban)
	{
		a_ban_kb=0;
		b_ban_kb=0;
		c_ban_kb=1;
	}
	next_banci_status=banci_status_kw;
}

void banci_prejudge(void){		//用于判断下次可能是哪个班次
	if (banci_status_kw==s_ban)
	{
		if (a_ban_kb==1)
			next_banci_status=a_ban;
		else if(b_ban_kb==1)
			next_banci_status=b_ban;
		else if(c_ban_kb==1)
			next_banci_status=c_ban;
	}
	else if (banci_status_kw==a_ban)
	{
		if(b_ban_kb==1)
			next_banci_status=b_ban;
		else if(c_ban_kb==1)
			next_banci_status=c_ban;
	}
	else if (banci_status_kw==b_ban)
	{
		if(a_ban_kb==1)
			next_banci_status=a_ban;
		else if(c_ban_kb==1)
			next_banci_status=c_ban;
	}
	else if (banci_status_kw==c_ban)
	{
		if(a_ban_kb==1)
			next_banci_status=a_ban;
		else if(b_ban_kb==1)
			next_banci_status=b_ban;
	}
}


void banci_judge(void){
	banci_prejudge();
	if (next_banci_status!=banci_status_kw && first_banci_flag==1){
		banci_mima_shuru=KeyBoard(999999,0,0,0);
		if (banci_mima_shuru==-1){
			banci_mima_shuru=0;
			restore_banci_button_status();
		}
		else if (banci_mima_shuru==*banci_mima[next_banci_status])
		{
			banci_mima_shuru=0;
			banci_status_kw=next_banci_status;
			restore_banci_button_status();
			banci[xiada_no]		=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][xiada_no]];
			banci[shangda_no]	=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][shangda_no]];
			banci[xiaxiao_no]	=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][xiaxiao_no]];
			banci[shangxiao_no]	=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][shangxiao_no]];
			banci[jianshu_no]	=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][jianshu_no]];
		}
		else{
			banci_mima_shuru=0;
			restore_banci_button_status();
			g_InteralMemory.Word[PING_CTRL]=30;
		}
	}
	else if (first_banci_flag==0){
		if (next_banci_status!=s_ban){
			banci[xiada_no]		=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][xiada_no]];
			banci[shangda_no]	=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][shangda_no]];
			banci[xiaxiao_no]	=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][xiaxiao_no]];
			banci[shangxiao_no]	=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][shangxiao_no]];
			banci[jianshu_no]	=&	g_InteralMemory.KeepWord[banci_num[next_banci_status][jianshu_no]];
		}
		else{
			banci[xiada_no]		=&s_banci[xiada_no];
			banci[shangda_no]	=&s_banci[shangda_no];
			banci[xiaxiao_no]	=&s_banci[xiaxiao_no];
			banci[shangxiao_no]	=&s_banci[shangxiao_no];
			banci[jianshu_no]	=&s_banci[jianshu_no];
		}
		first_banci_flag=1;
	}
	
}

void banci_guanli(void){
	if (a_ban_kb==0 && b_ban_kb==0 && c_ban_kb==0 && banci_status_kw!=s_ban || banci_status_kw==65535){		//用作刚上电，还未开始班次切换,或取消所有班次时
		if (banci_status_kw==65535){
			banci_status_kw=s_ban;
			next_banci_status=banci_status_kw;
			banci[xiada_no]		=&s_banci[xiada_no];
			banci[shangda_no]	=&s_banci[shangda_no];
			banci[xiaxiao_no]	=&s_banci[xiaxiao_no];
			banci[shangxiao_no]	=&s_banci[shangxiao_no];
			banci[jianshu_no]	=&s_banci[jianshu_no];
			first_banci_flag=1;
		}
		else{
			banci_mima_shuru=KeyBoard(999999,0,0,0);
			if (banci_mima_shuru==-1){
				banci_mima_shuru=0;
				restore_banci_button_status();
			}
			else if (banci_mima_shuru==*banci_mima[banci_status_kw])
			{
				banci_mima_shuru=0;
				banci_status_kw=s_ban;
				next_banci_status=banci_status_kw;
				banci[xiada_no]		=&s_banci[xiada_no];
				banci[shangda_no]	=&s_banci[shangda_no];
				banci[xiaxiao_no]	=&s_banci[xiaxiao_no];
				banci[shangxiao_no]	=&s_banci[shangxiao_no];
				banci[jianshu_no]	=&s_banci[jianshu_no];
			}
			else{
				banci_mima_shuru=0;
				restore_banci_button_status();
				g_InteralMemory.Word[PING_CTRL]=30;
			}
		}
	}
	banci_judge();
}


void dingdanlianghua(void){
	unsigned char ban,lei;
	if (dingdanxiugai_b==1)
	{
		dingdanxiugai_b=0;
		banci_mima_shuru=KeyBoard(999999,0,0,0);
		if (banci_mima_shuru==qz_guanliyuan_mima)
		{
			if (previous_dingdanbianhao==dingdanbianhao_kw)
			{
				previous_dingdanzongshu=dingdanzongshu_kw;
			}
			else
			{
				previous_dingdanbianhao=dingdanbianhao_kw;
				previous_dingdanzongshu=dingdanzongshu_kw;
				encoder1_pulse_number=encoder1_pulse_number_save=0;//编码器计数清零
				dapan_round=dapan_round_save=0;					//大盘圈数清零
				speed_status = 0;
				
				extra_part_finish_flag=extra_part_unfinish;
				extra_part_flag=extra_part_stop;
				if (encoder1_speed == 0)
					forceEqual = 1;
				//lingwei_jiance_button = 1;						//压针回零 by FJW
				
				jianshu=0;										//件数清零
				dingdan_lianghua_num_kw=0;						//订单量化清零
				songsha1_num=songsha_num[0]=0;					//各送纱电机所发出脉冲数清零
				songsha2_num=songsha_num[1]=0;
				songsha3_num=songsha_num[2]=0;
				songsha4_num=songsha_num[3]=0;
				songsha_1=0;									//各总送纱电机送纱量清零
				songsha_2=0;
				songsha_3=0;
				songsha_4=0;
				shachang_upper_num_1_kw=0;
				shachang_upper_num_2_kw=0;
				shachang_upper_num_3_kw=0;
				shachang_upper_num_4_kw=0;
				for (ban=a_ban;ban<=c_ban;ban++)				//所有班次记录清零
					for (lei=xiada_no;lei<=jianshu_no;lei++)
						g_InteralMemory.KeepWord[banci_num[ban][lei]]=0;
				jianshu_ewaiduan_check=0;						//额外段检查清零，避免因订单完成的错误中断后能够再继续工作，故不再KeyScan中清零
				banci_mima_shuru=0;
			}
		}
		else if (banci_mima_shuru==-1)
			banci_mima_shuru=0;
		else
		{
			dingdanbianhao_kw=previous_dingdanbianhao;
			dingdanzongshu_kw=previous_dingdanzongshu;
			banci_mima_shuru=0;
			g_InteralMemory.Word[PING_CTRL]=30;
		}
	}
	else if ((dingdanbianhao_kw!=previous_dingdanbianhao || dingdanzongshu_kw!=previous_dingdanzongshu) && g_InteralMemory.Word[PING_STATE]!=18)
	{
		dingdanbianhao_kw=previous_dingdanbianhao;
		dingdanzongshu_kw=previous_dingdanzongshu;
	}
}


void __irq pwrDownHandler(void)
{
	//U8 pwrOffCount;
	if ( !( rEINTMASK >>4  & 0x1 )  && (rEINTPEND & (1<<4)) )//取消屏蔽标志+挂起有效
	{
		rEINTPEND=(1<<4);		
	}
	// Eint5	// 高速输入X5
	else if ( !( rEINTMASK >>5  & 0x1 )  && (rEINTPEND & (1<<5)) )//取消屏蔽标志+挂起有效	
	{
		
		rEINTPEND=(1<<5);		
	}
	// Eint6	// 高速输入X6
	else if ( !( rEINTMASK >>6  & 0x1 )  && (rEINTPEND & (1<<6)) )//取消屏蔽标志+挂起有效	
	{
		if((rGPFDAT >> 6) & 0x1)
		{
			InPulseCount[6]++;  //InPulseCount[n] 只定义了7个 InPulseCount[0]-----InPulseCount[6]
		}

		rEINTPEND=(1<<6);		
	}
	// Eint7	// 高速输入X7
	else if ( !( rEINTMASK >>7  & 0x1 )  && (rEINTPEND & (1<<7)) )//取消屏蔽标志+挂起有效	
	{
		if((rGPFDAT >> 7) & 0x1)
		{
		  /////////
		}
		rEINTPEND=(1<<7);		
	}
	ClearPending(BIT_EINT4_7);
}