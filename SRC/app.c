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


extern unsigned int InPulseCount[7];		//��¼6·�����������������

extern unsigned int OutPulse_PreSet[4];	// 3·���������  ����Ԥ��ֵ
extern unsigned int OutPulse_Counter[4];	// 3·���������  ��ǰ���������


////�Ӵ˿�ʼ����û�����//////////////////////////////////

///////////////////////////////////////////////////////////////
//////	��ʼ��     //////////////////////////////////////////
//////	ÿ���ϵ�App_Init ִֻ��һ��	 //////////////////
//////////////////////////////////////////////////////////////
void App_Init(void)
{
	int i;
	X_In_Init();
	Y_Out_Init();
	Pulse_In_Init();
	Pulse_Out_Init();
	
	qz_uart_init();							//��ʼ��485������ѡ��com4
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
	
	//X1~X7������������
	for(i=0; i<7; i++)
	{
		InPulseCount[i] = 0;
	}
	
#ifdef YAZHEN_NORMAL_EN
	Yazhen_Normal_Init();
#endif

}


///////////////////////////////////////////////////////////////
//////	Ӧ��������     //////////////////////////////////////////
//////	ϵͳÿ��ѭ�������һ��App_Operation	 //////////////////
//////////////////////////////////////////////////////////////
void App_Operation(void)
{
//	int i;

	//Test_Run();
	//Test_RTC_SetTime();  // ��ť����
	//Test_RTC_ReadTime(); 


	// DA
	/*if(0 == g_InteralMemory.Bit[211])	// DA ��ǰ״ֹ̬ͣ
	{
		if(1 == g_InteralMemory.Bit[210])	// Set DA
		{
			DA0_10_Start(g_InteralMemory.Word[30]);
			g_InteralMemory.Bit[211] = g_InteralMemory.Bit[210];
		}
	}
	if(1 == g_InteralMemory.Bit[211])	// DA ��ǰ״̬����
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
/////////////////////�Ӵ˿�ʼ����û�����/////////////////////
//////////////////////////////////////////////////////////////

/*
g_InteralMemory.Bit[]
g_InteralMemory.KeepBit[]
g_InteralMemory.Word[]
g_InteralMemory.KeepWord[]
*/	

	
	
}




