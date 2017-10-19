#ifndef	_INCLUDES_H_
#define	_INCLUDES_H_

#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "def.h"
#include "2416addr.h"
#include "2416lib.h"
#include "option.h"
#include "LCD.h"
#include "mmu.h"
#include "Nand.h"
#include "download.h"
#include "systemconf.h"
#include "systemmenu.h"
#include "function.h"
#include "jiema.h"
#include "communication.h"
//#include "test.h"
#include "Rtc.h"
#include "encrypt.h"

#include"appBase.h"


#include"delay.h"
#include"encoder1.h"
#include"encoder2.h"
#include"encoder3.h"
#include"encoder4.h"
#include"encoder5.h"
//#include"shoubu.h"
//#include"songsha.h"
#include"bianpingqi.h"
#include"16jidianqi.h"
#include"qz_system.h"
#include"zhongduan.h"
//#include"kongzhiban.h"
#include"init.h"
#include"pid.h"
//#include"20160428.h"
// #include"com.h"
// #include"20160504.h"
#include"stepmotor.h"
#include"qigang.h"
#include"wdt.h"

/* #include "pressing.h"
#include "pressing_jog.h"
#include "pressing_cal.h"
#include "pressing_get_zero.h"		//压针 by FJW */
#include "tiaosha.h"				//调线 by FJW


#define	start_quanzhou					1
#define	stop_quanzhou					0

#define	HIGH							0
#define	LOW								1

#define	Pen_status						!(rADCDAT0>>15)						//笔尖落下判断，1为笔尖落下态，0为笔尖抬起态
#define	Pen_status_down					1
#define	Pen_status_up					0

#define	dangban_jianshu_dis_w			g_InteralMemory.Word[44]


#define mode_choose						(g_InteralMemory.KeepWord[153]) //模式设置选择by FJW
#define seven_motor_mode				1
#define tiaoxian_mode					2
#define yazheng_mode					10

extern char PlcType;
extern INTERALMEMORY g_InteralMemory;
extern int whichUart;

extern U8 com_frequence;
extern U8 g_com_Flag ;

extern TIMER2_V g_Timer2Uart;
extern TIMER2_V g_Timer2Virtual[MaxTimer];
extern U8 g_UartTimeOut;

extern DELAY_STRUCTURE delay_fac;
extern INT16U CurrentScrNum;
extern char Info_qz[9][11];
extern U8	xianshi_flag;

extern unsigned char ss_station_num[4];
//extern SONGSHA_PARAMETER p_ss;
extern SYSTEMCONFIG g_SystemConf;
extern DELAY_STRUCTURE delay_fac;

extern volatile float k_motor[7];
extern const float k_factor[7][2];
//extern volatile unsigned int k_motor[5];
extern U32 youbeng_quanjianxie_yizhuan_num;
extern float rate_different[7][3];
extern unsigned int dapan_round;
//extern float encoder1_cal_factor;
//extern const int encoder1_cal_factor;

//extern const float encoder1_tran_factor;
//extern float motor_factor[5];
//extern U32 encoder1_speed_pulse;
extern U8 bianpingqi_run_flag;
extern unsigned int bianpingqi_speed;
extern	U16	encoder1_pulse_number;
extern	U8 bianpingqi_previous_run_status;
extern	unsigned char qz_error_status;
extern	unsigned int	bianpingqi_previous_speed;
extern U8 bianpingqi_jog_status;
//extern unsigned char test_deng;
extern unsigned int qz_cuowu_cishu[max_port+1];
extern unsigned char beep_status;
extern U8 youbeng_quan_init_flag;
extern U8 fenshan_quan_init_flag;
extern const unsigned int pre_set_par[5][3];
extern float modify_rate_different[7][3];
extern	float songsha_rate[4];
extern U16 songsha_num[7];
extern unsigned char run_permite_flag;
extern	unsigned char emer_stop_flag;
extern unsigned char privilege_run_flag;
extern unsigned char signal;
extern INT16U* banci[5];
extern	const unsigned char banci_num[3][5];
extern	int banci_mima_shuru;
extern	INT16U* banci_mima[3];
extern INT16U previous_dingdanbianhao;
extern	INT16U previous_dingdanzongshu;
extern unsigned char jianshu_ewaiduan_check;
extern	INT16U next_banci_status;
extern	unsigned char caijianduan_sudu_change_record_num;
extern	unsigned char alarm_signal[max_port+1];
extern	U32 encoder1_speed_pulse;
extern	U32	encoder1_speed;
extern	U8 init_stop_status;
extern	unsigned char main_enter_flag;
extern unsigned char reset_timer_start;
extern	unsigned int reset_timer_counter;

////压针 by FJW

extern unsigned int pressing_zero_finish;
extern unsigned int average_yazhen;
extern unsigned int jog_positive_status;
extern unsigned int jog_negative_status;
extern unsigned int lingwei_jiance_cover;
extern unsigned int lingwei_jiance_uncover;
extern unsigned int	huiling_length_tmp;


extern unsigned int chudao_start[4];	//调线功能 by FJW
extern unsigned int shoudao_start[4];	//调线功能 by FJW
extern unsigned int chudao_start_status[4];	//调线功能 by FJW
extern unsigned int shoudao_start_status[4];	//调线功能 by FJW
extern unsigned int chudao_jiange_tmp[4];		//出刀间隔记录 by FJW
extern unsigned int shoudao_jiange_tmp[4];		//收刀间隔记录 by FJW

extern	unsigned int tongxunzhen;
extern unsigned int shinengwei[4];	
extern unsigned int chudao_shoudao_status[4];


//extern unsigned int SET_HIGH_STATUS;		//置高电平标志位 by FJW
//unsigned int chudao_shoudao_status;
#endif


