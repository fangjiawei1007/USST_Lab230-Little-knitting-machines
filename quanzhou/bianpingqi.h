#ifndef _BIANPINGQI_H_
#define _BIANPINGQI_H_

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
#include "test.h"
#include "Rtc.h"
#include "encrypt.h"

#include"appBase.h"

/*************************************************************************************************************/
#define	bianpingqi_Direc_add			1
#define	bianpingqi_Direc_min			0
#define	bianpingqi_enable				1
#define	bianpingqi_disable				0
#define	bianpingqi_write_fun_num		0x06

#define	shilin_bianpingqi				1
#define	taili_bianpingqi				2
#define	PS550_bianpingqi				3
#define PS3000_bianpinqi				4

/*************************************************************************************************************/
//画面控件号
#define	bianpingqi_run_button_bit			2
#define	bianpingqi_jog_button_bit			3
#define	bianpingqi_stop_button_bit			7

#define	bianpingqi_speed_to_fre_word		19

#define bianpingqi_fullspeed_set_kword		1
#define bianpingqi_fullspeed_set			g_InteralMemory.KeepWord[1]			//大盘全速运行速度

#define	bianpingqi_fencen_speed_set_kword	60
#define	bianpingqi_fencen_speed_set			g_InteralMemory.KeepWord[60]		//大盘分层运行速度

#define	bianpingqi_tiaoxian_speed_set		g_InteralMemory.KeepWord[155]		//调线开始大盘速度

#define	bianpingqi_delta_num_kword			14
#define	bianpingqi_delta_num				g_InteralMemory.KeepWord[14]		//大盘减速百分比

#define	bianpingqi_huanchongquan_num_kword	15
#define	bianpingqi_huanchongquan_num		g_InteralMemory.KeepWord[15]		//缓冲圈数

#define	bianpingqi_jog_speed_lv_kword		3
/*************************************************************************************************************/
#define	bianpingqi_jog_button				g_InteralMemory.Bit[3]				//点动按钮
#define bianpingqi_stop_button				g_InteralMemory.Bit[7]				//停止按钮
#define	bianpingqi_start_button				g_InteralMemory.Bit[2]				//运行按钮

#define	bianpingqi_speed_up_b				g_InteralMemory.Bit[13]				//变频器加速按钮
#define bianpingqi_speed_down_b				g_InteralMemory.Bit[4]				//变频器减速按钮

#define	bianpingqi_speed_to_fre				g_InteralMemory.Word[19]

#define	bianpingqi_jog_speed_lv				g_InteralMemory.KeepWord[3]

#define	bianpingqi_zhonglei					g_InteralMemory.KeepWord[63]		//变频器类型选择
/*************************************************************************************************************/

#define	Choose_bianpingqi_kb				g_InteralMemory.KeepBit[33]
#define	CHOOSE_BIANPINGQI					1
#define	CHOOSE_NOT							0

/***************************电机方向，输出函数********************************************************/
#define bianpingqi_station_num		13

/***************************调纱大盘减速比*****************************************/
#define bianpingqi_weisha_delta_num			g_InteralMemory.KeepWord[461]



void bianpingqi_start(U8 bianpingqi_run_button);
void bianpingqi_jog(void);
void bianpingqi_RTU_WriteWord(U8 function_num,int Value);
void bianpingqi_set_speed(unsigned int speed);
void bianpingqi_start_sub(void);
void bianpingqi_stop_sub(void);
void bianpingqi_speed_cal(void);


#endif

