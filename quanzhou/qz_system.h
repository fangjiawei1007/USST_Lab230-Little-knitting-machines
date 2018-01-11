#ifndef _QZSYSTEM_H_
#define	_QZSYSTEM_H_

#define	xiada_no								0		//下大盘送纱量
#define	shangda_no								1		//上大盘送纱量
#define	xiaxiao_no								2		//下小盘送纱量
#define	shangxiao_no							3		//上小盘送纱量
#define	jianshu_no								4		//件数

#define	s_ban									3
#define	a_ban									0
#define	b_ban									1
#define	c_ban									2

#define	youbeng_new								1
#define	youben_old								0

#define	shachang_1								g_InteralMemory.Word[34]
#define	shachang_2								g_InteralMemory.Word[35]
#define	shachang_3								g_InteralMemory.Word[36]
#define	shachang_4								g_InteralMemory.Word[37]

#define	total_shachang_1_xianshi_w				g_InteralMemory.Word[45]
#define	total_shachang_2_xianshi_w				g_InteralMemory.Word[46]
#define	total_shachang_3_xianshi_w				g_InteralMemory.Word[47]
#define	total_shachang_4_xianshi_w				g_InteralMemory.Word[48]


#define	youbeng_lianxu_button					g_InteralMemory.KeepBit[20]
#define	youbeng_miaojianxie_button				g_InteralMemory.KeepBit[21]
#define	youbeng_quanjianxie_button				g_InteralMemory.KeepBit[22]
#define	fenshan_lianxu_button					g_InteralMemory.KeepBit[23]
#define	fenshan_jianxie_1_button				g_InteralMemory.KeepBit[24]
#define	fenshan_jianxie_2_button				g_InteralMemory.KeepBit[25]

#define	yuanpan_speed							g_InteralMemory.Word[17]

#define	zhibusheding							g_InteralMemory.KeepWord[17]		//件数上限
#define	qingchesheding							g_InteralMemory.KeepWord[18]		//清车圈数
#define	youbeng_miaojianxie_fac_L				g_InteralMemory.KeepWord[19]		//油泵秒间歇运行时间
#define	youbeng_miaojianxie_fac_H				g_InteralMemory.KeepWord[20]        //油泵秒间歇停止时间
#define	youbeng_quanjianxie_fac_L				g_InteralMemory.KeepWord[21]        //油泵圈间歇运行圈数
#define	youbeng_quanjianxie_fac_H				g_InteralMemory.KeepWord[22]        //油泵秒间歇停止权属
#define	fenshan_jianxie_fac_L					g_InteralMemory.KeepWord[23]        //风扇工作圈数
#define	fenshan_jianxie_fac_H					g_InteralMemory.KeepWord[24]        //风扇间歇圈数
#define	fenshan_jianxie_yizhuanquan_num			g_InteralMemory.KeepWord[25]        
                                                                               
#define	guanliyuan_jiemian_queding				g_InteralMemory.Bit[9]
#define	qz_mima_shuru							g_InteralMemory.Word[12]
#define	qz_guanliyuan_mima						g_InteralMemory.KeepWord[77]		//管理员密码

#define	songsha1_num							g_InteralMemory.KeepWord[81]		//送纱电机1脉冲数
#define	songsha2_num							g_InteralMemory.KeepWord[82]		//送纱电机2脉冲数
#define	songsha3_num							g_InteralMemory.KeepWord[83]		//送纱电机3脉冲数
#define	songsha4_num							g_InteralMemory.KeepWord[84]        //送纱电机4脉冲数
                                                                               
#define	songsha_dapanchilunshu					g_InteralMemory.KeepWord[78]		//大送纱齿轮数
#define	songsha_xiaopanchilunshu				g_InteralMemory.KeepWord[79]		//小送纱齿轮数
#define	songsha_lvpanzhijin						g_InteralMemory.KeepWord[80]		//输纱轮直径
#define	songsha_xiaochilunshu					g_InteralMemory.KeepWord[85]		//输纱轮齿数

#define songsha_1								g_InteralMemory.KeepWord[86]		//送纱电机1总送纱量
#define songsha_2								g_InteralMemory.KeepWord[87]		//送纱电机2总送纱量
#define songsha_3								g_InteralMemory.KeepWord[88]		//送纱电机3总送纱量
#define songsha_4								g_InteralMemory.KeepWord[89]		//送纱电机4总送纱量


#define	sys_permite_button						g_InteralMemory.Bit[11]				//管理员界面：电机
#define	sys_force_run_button					g_InteralMemory.Bit[12]				//管理员界面：强迫运行
#define	fengshan_permite_button					g_InteralMemory.Bit[33]
#define	youben_permite_button					g_InteralMemory.Bit[36]	
#define	youben_choose							g_InteralMemory.KeepBit[13]					
#define	dida_num								g_InteralMemory.KeepWord[91]			//点滴次数
#define	xifenshu								g_InteralMemory.KeepWord[92]			//送纱电机细分数



#define	a_ban_mima_kw						g_InteralMemory.KeepWord[125]
#define	b_ban_mima_kw						g_InteralMemory.KeepWord[126]
#define	c_ban_mima_kw						g_InteralMemory.KeepWord[127]

#define	a_ban_mima_reset_b					g_InteralMemory.Bit[63]
#define	b_ban_mima_reset_b					g_InteralMemory.Bit[64]
#define	c_ban_mima_reset_b					g_InteralMemory.Bit[65]

#define	dingdanbianhao_kw					g_InteralMemory.KeepWord[129]		//订单编号
#define	dingdanzongshu_kw					g_InteralMemory.KeepWord[130]		//订单数量
#define	dingdan_lianghua_num_kw				g_InteralMemory.KeepWord[131]

#define	shachang_upper_num_1_kw				g_InteralMemory.KeepWord[133]		//下部大输砂轮送纱量
#define	shachang_upper_num_2_kw				g_InteralMemory.KeepWord[134]		//上部大输砂轮送纱量
#define	shachang_upper_num_3_kw				g_InteralMemory.KeepWord[135]		//下部小输砂轮送纱量
#define	shachang_upper_num_4_kw				g_InteralMemory.KeepWord[136]		//上部小输砂轮送纱量

#define	banci_status_kw						g_InteralMemory.KeepWord[137]

#define	a_ban_kb							g_InteralMemory.KeepBit[34]
#define	b_ban_kb							g_InteralMemory.KeepBit[35]
#define	c_ban_kb							g_InteralMemory.KeepBit[36]

#define	dingdanxiugai_b						g_InteralMemory.Bit[67]




void youbeng_sys_fun(void);
void fenshan_sys_fun(void);
void qz_guanliyuan(void);
void shachang_xianshi(void);
void youbeng_new_way(void);
void banci_guanli(void);
void dingdanlianghua(void);
void __irq	pwrDownHandler(void);










#endif

