#include"includes.h"

#define max_daduanzhijin				200
#define	min_xiaoduanzhijin				0
#define	max_length						200
#define	min_length						0

unsigned int rate_different[7][3]={0};
unsigned char check_time=0;


/*************************************************
Function(��������): qz_assert(void)
Description(�������ܡ����ܵȵ�����): ��������������Ч���жϣ��������û��Ҫ��(������ʵû���ã��������Ͽ�����һ��С��У��)
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void qz_assert(void)
{
	if(0)
	{
		if (check_time==0)
		{
			alarm_disp_assert();
			check_time=1;
		}

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

		songsha_rate[0]=songsha_rate[1]=songsha_dapanchilunshu*1.0/songsha_xiaochilunshu*songsha_lvpanzhijin;
		songsha_rate[2]=songsha_rate[3]=songsha_xiaopanchilunshu*1.0/songsha_xiaochilunshu*songsha_lvpanzhijin;
		
		if (g_InteralMemory.Word[PING_STATE]==6){//��ͷ��2000*100.0
			shachang_shuru_xianshi_1_w=	0.2*k_factor[0][datou]*rate_different[0][datou]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[0];
			shachang_shuru_xianshi_2_w=	0.2*k_factor[1][datou]*rate_different[1][datou]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[1];
			shachang_shuru_xianshi_3_w=	0.2*k_factor[2][datou]*rate_different[2][datou]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[2];
			shachang_shuru_xianshi_4_w=	0.2*k_factor[3][datou]*rate_different[3][datou]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[3];
		}
		else if (g_InteralMemory.Word[PING_STATE]==7){//Сͷ
			shachang_shuru_xianshi_1_w=	0.2*k_factor[0][xiaotou]*rate_different[0][xiaotou]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[0];
			shachang_shuru_xianshi_2_w=	0.2*k_factor[1][xiaotou]*rate_different[1][xiaotou]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[1];
			shachang_shuru_xianshi_3_w=	0.2*k_factor[2][xiaotou]*rate_different[2][xiaotou]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[2];
			shachang_shuru_xianshi_4_w=	0.2*k_factor[3][xiaotou]*rate_different[3][xiaotou]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[3];
		}
		else if (g_InteralMemory.Word[PING_STATE]==8){//�ֲ�
			shachang_shuru_xianshi_1_w=	0.2*k_factor[0][fencen]*rate_different[0][fencen]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[0];
			shachang_shuru_xianshi_2_w=	0.2*k_factor[1][fencen]*rate_different[1][fencen]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[1];
			shachang_shuru_xianshi_3_w=	0.2*k_factor[2][fencen]*rate_different[2][fencen]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[2];
			shachang_shuru_xianshi_4_w=	0.2*k_factor[3][fencen]*rate_different[3][fencen]/dapan_zongzhenshu_kw
										/xifenshu*songsha_rate[3];
		}
		

		check_time=0;
	}
}


void alarm_disp_assert(void)
{
	char Info_assert[]={42,68,40,75,20,77,46,83,33,43};  //CharString:��������ϡ�
	char QueDing[]={40,23,22,8};						 //CharString:��ȷ��
	
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