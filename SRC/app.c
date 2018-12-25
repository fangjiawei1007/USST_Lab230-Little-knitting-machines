/* #include <stdarg.h>
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
#include "..\inc\download.h"
#include "..\inc\systemconf.h"
#include "..\inc\systemmenu.h"
#include "..\inc\function.h"
#include "..\inc\jiema.h"
#include "..\inc\communication.h"
#include "..\inc\test.h"
#include "..\inc\Rtc.h"
#include "..\inc\encrypt.h"

#include "..\inc\app.h"
#include "..\inc\appBase.h" */

#include "includes.h"



//extern INTERALMEMORY g_InteralMemory;

//extern GLOBALVAR g_GlobalVar;

//extern INT16U PreScrNum;


extern unsigned int InPulseCount[7];		//记录6路编码器输入的脉冲数

extern unsigned int OutPulse_PreSet[4];	// 3路脉冲输出的  脉冲预设值
extern unsigned int OutPulse_Counter[4];	// 3路脉冲输出的  当前输出脉冲数


////从此开始添加用户变量//////////////////////////////////

///////////////////////////////////////////////////////////////
//////	初始化     //////////////////////////////////////////
//////	每次上电App_Init 只执行一次	 //////////////////
//////////////////////////////////////////////////////////////
void App_Init(void)
{
	int i;
	X_In_Init();
	Y_Out_Init();
	Pulse_In_Init();
	Pulse_Out_Init();
	
	qz_uart_init();							//初始化485，不必选择com4
	bianpingqi_init();
	jidianqi_init();
	menu_init();
	zhongduan_init();
	encoder1_init();
	stepmotor_init();

	Timer0_init_quanzhou();

//	if(tiaoxiankaiguan_kb == 1)//mode_choose == tiaoxian_mode
		tiaoxian_init();// by FJW
	//wdt_init_reset();
	wdt_feed_dog();main_enter_flag = 1;
	
	//X1~X7的脉冲数清零
	for(i=0; i<7; i++)
	{
		InPulseCount[i] = 0;
	}
	
#ifdef YAZHEN_NORMAL_EN
	Yazhen_Normal_Init();
#endif

}


///////////////////////////////////////////////////////////////
//////	应用主程序     //////////////////////////////////////////
//////	系统每个循环会调用一次App_Operation	 //////////////////
//////////////////////////////////////////////////////////////
void App_Operation(void)
{
//	int i;

	//Test_Run();
	//Test_RTC_SetTime();  // 按钮控制
	//Test_RTC_ReadTime(); 


	// DA
	/*if(0 == g_InteralMemory.Bit[211])	// DA 当前状态停止
	{
		if(1 == g_InteralMemory.Bit[210])	// Set DA
		{
			DA0_10_Start(g_InteralMemory.Word[30]);
			g_InteralMemory.Bit[211] = g_InteralMemory.Bit[210];
		}
	}
	if(1 == g_InteralMemory.Bit[211])	// DA 当前状态启动
	{
		if(0 == g_InteralMemory.Bit[210])	// ReSet DA
		{
			DA0_10_Stop();
			g_InteralMemory.Bit[211] = g_InteralMemory.Bit[210];
		}
	}*/

 
/*	
	for(i=2; i<16; i++)
	{
		Set_Y_Value(i, TRUE);
		Set_Y_Value(i, FALSE);
	}
*/	




//////////////////////////////////////////////////////////////
/////////////////////从此开始添加用户程序/////////////////////
//////////////////////////////////////////////////////////////

/*
g_InteralMemory.Bit[]
g_InteralMemory.KeepBit[]
g_InteralMemory.Word[]
g_InteralMemory.KeepWord[]
*/	

	
	
}




