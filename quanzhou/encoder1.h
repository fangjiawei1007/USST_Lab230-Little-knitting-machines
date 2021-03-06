#ifndef _ENCODER1_H_
#define _ENCODER1_H_

#define	datou					1
#define	xiaotou					0
#define	fencen					2

/*************************************************************************************************************/
//编码器接口 X1
#define	encoder1_port	1
/*************************************************************************************************************/
//底层函数接口
#define	encoder1_fun(X)	(X?InPulse1_Start():InPulse1_Stop())
#define	encoder2_fun(X)	(X?InPulse2_Start():InPulse2_Stop())
#define	encoder3_fun(X)	(X?InPulse3_Start():InPulse3_Stop())
#define	encoder4_fun(X)	(X?InPulse4_Start():InPulse4_Stop())
#define	encoder5_fun(X)	(X?InPulse5_Start():InPulse5_Stop())
#define	encoder6_fun(X)	(X?InPulse6_Start():InPulse6_Stop())

/*************************************************************************************************************/
#define	encoder1_run_status_bit				13
#define	encoder1_dangqianjian_reset_bit		14

#define	current_stage_word					11

#define	encoder1_pulse_number_save_kword	2
#define	encoder1_pulse_number_save			g_InteralMemory.KeepWord[2]			//已发出脉冲数

#define	dapan_round_save_kword				7
#define	dapan_round_save					g_InteralMemory.KeepWord[7]			

#define	total_quanshu						g_InteralMemory.KeepWord[13]		//编织总圈数低位

#define	jianshu								g_InteralMemory.KeepWord[44]		//已织件数

#define	xiamian							0
#define	shangmian						1

#define	qigang_zhonglei_choose_kbit		14
#define	qigang_zhonglei_choose			g_InteralMemory.KeepBit[14]


/*************************************************************************************************************/

#define	encoder1_dangqianjian_reset			g_InteralMemory.Bit[14]			//当前件圈数清零

#define	current_stage						g_InteralMemory.Word[11]		

#define	encoder1_jianshu_reset				g_InteralMemory.Bit[6]			//件数清零

#define	encoder1_zonggongquanshu_reset		g_InteralMemory.Bit[8]			//全清零(不包括当班产量)

/*************************************************************************************************************/

#define	daduanquanshu						g_InteralMemory.KeepWord[4]		//大头圈数

#define	middlequanshu						g_InteralMemory.KeepWord[5]		//过渡圈数

#define	xiaoduanquanshu						g_InteralMemory.KeepWord[6]		//小头圈数
                                                                                
#define	caijiaoquanshu						g_InteralMemory.KeepWord[16]	//分层圈数
                                                                           
#define	langfeiquanshu						g_InteralMemory.KeepWord[26]	//裁剪圈数

#define	encoder1_cal_factor					g_InteralMemory.KeepWord[76]	//编码器数据设置

#define	songshaliang_data_reset				g_InteralMemory.Bit[10]			//当班产量清零

#define	max_speed							g_InteralMemory.KeepWord[90]	//大盘最大运行速度

#define	extra_part_jiansu					g_InteralMemory.KeepWord[94]	//挡片间歇件数(做几件之后做额外段)
#define	extra_part_quanshu					g_InteralMemory.KeepWord[101]	//挡片圈数(额外段圈数)
#define	extra_fencen_quan_num_kw			g_InteralMemory.KeepWord[102]	//挡片分层工艺圈数(额外段分层圈数)

#define	extra_part_flag						g_InteralMemory.KeepBit[15]			//
#define	extra_part_finish_flag				g_InteralMemory.KeepBit[16]			//

#define	huanchongmaichong					g_InteralMemory.KeepWord[146]	//缓冲脉冲数
#define	speedUpMax							g_InteralMemory.KeepWord[147]	//加速次数
#define	speedDownMax						g_InteralMemory.KeepWord[138]	//减速次数

#define	xitong_total_num_upper_kw			g_InteralMemory.KeepWord[128]	//已编织总圈数
#define	total_qingling_b					g_InteralMemory.Bit[66]



#define	save_button_b						g_InteralMemory.Bit[68]

#define	jiajiansujiangemaichong_kw			g_InteralMemory.KeepWord[154]	//加减速间隔脉冲

#define	extra_part_start	1
#define	extra_part_stop		0
#define	extra_part_finish	0
#define	extra_part_unfinish	1

#define	NEXTSTAGE			1
#define	PREVIOUSSTAGE		-1
#define	CURRENT 			0

#define	datouduan			0	//
#define	guoduduan			1
#define	xiaotouduan			2
#define	fencenduan			3
#define	caijianduan			4
#define	ewaiduan			5


extern	unsigned int InPulseCount[7];		//记录6路高速输入的脉冲数
extern	unsigned int OutPulse_PreSet[4];	// 3路脉冲输出的  脉冲预设值
extern	unsigned int OutPulse_Counter[4];	

///////main需要用的函数/////////////////////
void encoder1_data_process(void);				//圆盘，三角速度和整数圈及非整数圈
void encoder1_data_reset(void);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void parameter_read(void);
void parameter_save(void);

//void __irq	Timer0_Proc(void);
void __irq	encoder1_process(void);
void Timer0_init_quanzhou(void);
void songsha_fre_change(void);
unsigned int getStage(const unsigned int stage,int direction);


#endif

