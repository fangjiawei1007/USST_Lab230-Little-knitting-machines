#include"includes.h"

#define max_daduanzhijin				200
#define	min_xiaoduanzhijin				0
#define	max_length						200
#define	min_length						0

volatile int k_motor[7]={0};
//volatile unsigned int k_motor[5]={0};
int rate_different[7][3]={0};
//float modify_rate_different[7][3]={0};
unsigned char check_time=0;

void qz_assert(void)
{
	/* if (daduanzhijin>max_daduanzhijin || xiaoduanzhijin<min_xiaoduanzhijin || length>max_length || length<min_length
	   ||daduanzhijin<xiaoduanzhijin) */
	//if ((xiaoduanquanshu<bianpingqi_huanchongquan_num) || (caijiaoquanshu<(bianpingqi_huanchongquan_num+1)))
	if(0)
	{
		if (check_time==0)
		{
			alarm_disp_assert();
			check_time=1;
		}
		parameter_check=0;
	}
	else
	{
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
		
		/* modify_rate_different[0][datou]=1;
		modify_rate_different[1][datou]=1;
		modify_rate_different[2][datou]=1;
		modify_rate_different[3][datou]=0.9;
		modify_rate_different[4][datou]=0.95;
		modify_rate_different[5][datou]=1;
		modify_rate_different[6][datou]=1;
	
		modify_rate_different[0][xiaotou]=1;
		modify_rate_different[1][xiaotou]=1;
		modify_rate_different[2][xiaotou]=1;
		modify_rate_different[3][xiaotou]=1;
		modify_rate_different[4][xiaotou]=1;
		modify_rate_different[5][xiaotou]=1;
		modify_rate_different[6][xiaotou]=1;
	
		modify_rate_different[0][fencen]=1;
		modify_rate_different[1][fencen]=1;
		modify_rate_different[2][fencen]=1;
		modify_rate_different[3][fencen]=1;
		modify_rate_different[4][fencen]=1;
		modify_rate_different[5][fencen]=1;
		modify_rate_different[6][fencen]=1; */

		songsha_rate[0]=songsha_rate[1]=songsha_dapanchilunshu*1.0/songsha_xiaochilunshu*songsha_lvpanzhijin;
		songsha_rate[2]=songsha_rate[3]=songsha_xiaopanchilunshu*1.0/songsha_xiaochilunshu*songsha_lvpanzhijin;
		
		if (g_InteralMemory.Word[PING_STATE]==6){//大头段*modify_rate_different[0][datou]*modify_rate_different[1][datou]*modify_rate_different[2][datou]*modify_rate_different[3][datou]
			shachang_shuru_xianshi_1_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[0][datou]*rate_different[0][datou]\
										/xifenshu*songsha_rate[0];
			shachang_shuru_xianshi_2_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[1][datou]*rate_different[1][datou]\
										/xifenshu*songsha_rate[1];
			shachang_shuru_xianshi_3_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[2][datou]*rate_different[2][datou]\
										/xifenshu*songsha_rate[2];
			shachang_shuru_xianshi_4_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[3][datou]*rate_different[3][datou]\
										/xifenshu*songsha_rate[3];
		}
		else if (g_InteralMemory.Word[PING_STATE]==7){//小头*modify_rate_different[0][xiaotou]*modify_rate_different[1][xiaotou]*modify_rate_different[2][xiaotou]*modify_rate_different[3][xiaotou]
			shachang_shuru_xianshi_1_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[0][xiaotou]*rate_different[0][xiaotou]\
										/xifenshu*songsha_rate[0];
			shachang_shuru_xianshi_2_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[1][xiaotou]*rate_different[1][xiaotou]\
										/xifenshu*songsha_rate[1];
			shachang_shuru_xianshi_3_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[2][xiaotou]*rate_different[2][xiaotou]\
										/xifenshu*songsha_rate[2];
			shachang_shuru_xianshi_4_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[3][xiaotou]*rate_different[3][xiaotou]\
										/xifenshu*songsha_rate[3];
		}
		else if (g_InteralMemory.Word[PING_STATE]==8){//分层*modify_rate_different[0][fencen]*modify_rate_different[1][fencen]*modify_rate_different[2][fencen]*modify_rate_different[3][fencen]
			shachang_shuru_xianshi_1_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[0][xiaotou]*rate_different[0][fencen]\
										/xifenshu*songsha_rate[0];
			shachang_shuru_xianshi_2_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[1][xiaotou]*rate_different[1][fencen]\
										/xifenshu*songsha_rate[1];
			shachang_shuru_xianshi_3_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[2][xiaotou]*rate_different[2][fencen]\
										/xifenshu*songsha_rate[2];
			shachang_shuru_xianshi_4_w=	2*100.0/dapan_zongzhenshu_kw*\
										k_factor[3][xiaotou]*rate_different[3][fencen]\
										/xifenshu*songsha_rate[3];
		}
		
		parameter_check=1;
		check_time=0;
	}
}


void alarm_disp_assert(void)
{
	char Info_assert[]={42,68,40,75,20,77,46,83,33,43};  //CharString:“下载完毕”
	char QueDing[]={40,23,22,8};						 //CharString:“确定
	
	INT16U LeftX,LeftY;
	LeftX = 200;
	LeftY = 150;
	if(xianshi_flag==0)
	{
		//Lcd_Clr(7);                                 //This Instruction can be removed  
		//CurrentScrNum = 0xff11;
		Lcd_Fill_Box (LeftX, LeftY, 440, 330, 4);
		//Lcd_Fill_Box (LeftX, LeftY, 440, 330, GREEN);
		Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300,5,13,4,3,0,1,0,0,Info_assert);
		//Lcd_DispHZ_On_Button(LeftX,LeftY+20,440,300,4,BLACK,GREEN,3,0,1,0,0,Info);
		Lcd_Button(LeftX+65,LeftY+95,375,305,7,3,RAISE);
		//Lcd_Button(LeftX+65,LeftY+95,375,305,BLUE,3,RAISE);
		Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, 0, 7,2,0,0,0,3,QueDing);
		//Lcd_DispHZ_On_Button(LeftX+45, LeftY+95, 395, 305,2, BLACK, BLUE,2,0,0,0,3,QueDing);
		xianshi_flag=1;
	}
}