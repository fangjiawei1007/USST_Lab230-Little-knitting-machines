#ifndef _QZSYSTEM_H_
#define	_QZSYSTEM_H_

#define	xiada_no								0
#define	shangda_no								1
#define	xiaxiao_no								2
#define	shangxiao_no							3
#define	jianshu_no								4

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

#define	zhibusheding							g_InteralMemory.KeepWord[17]
#define	qingchesheding							g_InteralMemory.KeepWord[18]
#define	youbeng_miaojianxie_fac_L				g_InteralMemory.KeepWord[19]
#define	youbeng_miaojianxie_fac_H				g_InteralMemory.KeepWord[20]
#define	youbeng_quanjianxie_fac_L				g_InteralMemory.KeepWord[21]
#define	youbeng_quanjianxie_fac_H				g_InteralMemory.KeepWord[22]
#define	fenshan_jianxie_fac_L					g_InteralMemory.KeepWord[23]
#define	fenshan_jianxie_fac_H					g_InteralMemory.KeepWord[24]
#define	fenshan_jianxie_yizhuanquan_num			g_InteralMemory.KeepWord[25]

#define	guanliyuan_jiemian_queding				g_InteralMemory.Bit[9]
#define	qz_mima_shuru							g_InteralMemory.Word[12]
#define	qz_guanliyuan_mima						g_InteralMemory.KeepWord[77]

#define	songsha1_num							g_InteralMemory.KeepWord[81]
#define	songsha2_num							g_InteralMemory.KeepWord[82]
#define	songsha3_num							g_InteralMemory.KeepWord[83]
#define	songsha4_num							g_InteralMemory.KeepWord[84]

#define	songsha_dapanchilunshu					g_InteralMemory.KeepWord[78]
#define	songsha_xiaopanchilunshu				g_InteralMemory.KeepWord[79]
#define	songsha_lvpanzhijin						g_InteralMemory.KeepWord[80]
#define	songsha_xiaochilunshu					g_InteralMemory.KeepWord[85]

#define songsha_1								g_InteralMemory.KeepWord[86]
#define songsha_2								g_InteralMemory.KeepWord[87]
#define songsha_3								g_InteralMemory.KeepWord[88]
#define songsha_4								g_InteralMemory.KeepWord[89]


#define	sys_permite_button						g_InteralMemory.Bit[11]		
#define	sys_force_run_button					g_InteralMemory.Bit[12]
#define	fengshan_permite_button					g_InteralMemory.Bit[33]
#define	youben_permite_button					g_InteralMemory.Bit[36]	
#define	youben_choose							g_InteralMemory.KeepBit[13]					
#define	dida_num								g_InteralMemory.KeepWord[91]
#define	xifenshu								g_InteralMemory.KeepWord[92]

/* #define	a_ban_b								g_InteralMemory.Bit[60]
#define	b_ban_b								g_InteralMemory.Bit[61]
#define	c_ban_b								g_InteralMemory.Bit[62] */

#define	a_ban_mima_kw						g_InteralMemory.KeepWord[125]
#define	b_ban_mima_kw						g_InteralMemory.KeepWord[126]
#define	c_ban_mima_kw						g_InteralMemory.KeepWord[127]

#define	a_ban_mima_reset_b					g_InteralMemory.Bit[63]
#define	b_ban_mima_reset_b					g_InteralMemory.Bit[64]
#define	c_ban_mima_reset_b					g_InteralMemory.Bit[65]

#define	dingdanbianhao_kw					g_InteralMemory.KeepWord[129]
#define	dingdanzongshu_kw					g_InteralMemory.KeepWord[130]
#define	dingdan_lianghua_num_kw				g_InteralMemory.KeepWord[131]

#define	shachang_upper_num_1_kw				g_InteralMemory.KeepWord[133]
#define	shachang_upper_num_2_kw				g_InteralMemory.KeepWord[134]
#define	shachang_upper_num_3_kw				g_InteralMemory.KeepWord[135]
#define	shachang_upper_num_4_kw				g_InteralMemory.KeepWord[136]

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

