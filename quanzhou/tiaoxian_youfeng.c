#include "includes.h"

#define SHANGPAN_SHUANGJI_NUM		8
#define SHANGPAN_DANJI_NUM			6
#define XIAPAN_SHUANGJI_NUM			21

unsigned int tongxunstart[ZUSHU_MAX][PIANSHU_MAX] = {0};
unsigned int tongxunnum[ZUSHU_MAX][PIANSHU_MAX] = {0};
unsigned int tongxunzhen[PIANSHU_MAX] = {0x0000};
unsigned char* qigang_confirm_kb[16];			//16��Ϊһ��8��*2=16
unsigned int qigang_confirm_status;				//��������ȷ��֮��tongxunzhen��set
TIAOXIANDUAN tiaoxianduan[tiaoshaduan_max];

/************************���޸�**************************************/

unsigned int jiajiaStatus = 0;

unsigned int weisha_jiange_status[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int weisha_jiange[ZUSHU_MAX][DAOSHU_MAX] = {0};

unsigned int shoudao_tozero_status[ZUSHU_MAX]={0};	//���߹��㸴λ��־
unsigned int shoudao_reset_flag = 0;

/*************************************************
Function(��������): TiaoXian_Youfeng_App(void);
Description(�������ܡ����ܵȵ�����): ���߹��ܴ���
Calls (�����������õĺ����嵥): shinengpanduan();
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void TiaoXian_Youfeng_App(void){

	int i=0;
	int j=0;
	unsigned int zushu=0;
	static int duanshu_enable_prev=10;
	static int duanshu_enable_cur=11;
	//TiaoXian_Youfeng_Shinengpanduan();	//�жϵ�ǰ�ڵڼ�����
	at_check(dapan_round);
	duanshu_enable_cur = between_check(dapanround);
	if (TiaoXian_Youfeng_Checkout() == CHANGED){
		Tiaoxian_Youfeng_ComInfo_Set();
	}

	//1.�������duanshu_enable_tmp != duanshu_enable��֤�����ͨѶ;
	//2.�Ի���ʱ����Ҫ���ͨѶqigang_confirm_status�����Ի�
	if((duanshu_enable_prev != duanshu_enable_cur && current_stage != caijianduan) 
	  || (qigang_confirm_status == 1)){
		//�ݴ�
		if(duanshu_enable_cur>8)
			return;
		duanshu_enable_prev = duanshu_enable_cur;//��ֹ�ظ�����(ͨѶ)
		
		TiaoXian_Youfeng_weisha(duanshu_enable_cur);
	
	}
	
	if(duanshu_enable_cur != -1 && duanshu_enable_cur<8){
		Tiaoxian_Youfeng_Yazhen();
	}

}


void TiaoXian_Youfeng_Shinengpanduan(void){
	int i;
//	unsigned int weizhi = 0x00;
	/*ʹ��λ����,�˴�ѡ��8�η���*/
	/* for (i = 0 ; i <tiaoshaduan_max; i++){
		shinengwei[i] = 0;
	} */
	/*��ȡ��ǰ�Σ������жϵ�ǰ���Ƿ���Ҫ����
	  ���߸ı�ȫ�ֱ���tiaoxianzu/tiaoxianzu_flag*/
	i = between_check(dapan_round);
	at_check(dapan_round);
	/**��ȡʹ��λ������tiaoxian����ͨѶ֡��ֱ���ж�ͨѶ֡����**/
 	/* if ( i != -1 && current_stage != caijianduan){
	/*	weizhi = tiqushuzi(*tiaoxianduan[i].channal_choose);
		for (i = 0 ; i <DAOSHU_MAX ; i++){
			if ( (weizhi>>i) & 0x01){
				shinengwei[i] = 1;
			}
		}*/
		shinengwei[i] = 1;
		
	}  */
}
void TiaoXian_Youfeng_Init(void){
	int i,j,k,m,n,bb;
	unsigned int shangpan_danji_check_tmp[8]={0};
	unsigned int shangpan_shuangji_check_tmp[8]={0};
	unsigned int xiapan_shaungji_check_tmp[8]={0};

	//д��8·ͨѶ֡���ⲿ�����Ӧ��״̬
	if (tiaoxiankaiguan_kb ==1 ){
		for (i = 0 ;i<ZUSHU_MAX;i++){
			for (j = 0 ;j<PIANSHU_MAX;j++)
				tongxunzhen[i][j] = 0x0;//��ʼ���̵�������
		}

		for (i = 0 ; i < ZUSHU_MAX; i++){
			for (j = 0 ; j < PIANSHU_MAX; j++)
				for (bb = 0 ; bb < 5 ; bb++){
					//ͨѶ�ɹ�֮���ֱ���˳�ѭ��,5��Ϊ�ݴ�
					if (Tiaoxian_Youfeng_jidianqi_write(i,j) == 1){
						break;
					}
				}
		}
	}

	/***8�β�����ʼ��***/
	for (i = 0; i < tiaoshaduan_max; i++){
		tiaoxianduan[i].kaishiquanshu = &g_InteralMemory.KeepWord[156 + 10*i];
		tiaoxianduan[i].jieshuquanshu = &g_InteralMemory.KeepWord[157 + 10*i];
		//	tiaoxianduan[ii].channal_choose= &g_InteralMemory.KeepWord[158 + 10*ii];
		tiaoxianduan[i].ewaiduan_choose=&g_InteralMemory.KeepBit[49 + i];
		tiaoxianduan[i].yazheng_motor_1st=&g_InteralMemory.KeepWord[158 + 10*i];
		tiaoxianduan[i].yazheng_motor_2nd=&g_InteralMemory.KeepWord[159 + 10*i];
		/**7·����Ŵ���**/
		// for (bb = 0; bb<7; bb++)		{
		// tiaoxianduan[ii].fangdabeishu[bb] = &g_InteralMemory.KeepWord[159 + 10*ii + bb];}

		/**�������������Լ���������**/
		for (j = 0;j<SHANGPAN_SHUANGJI_NUM; j++){
			tiaoxianduan[i].shangpan_shaungji_qigang[j]=&g_InteralMemory.KeepWord[470+j+35*i];
			shangpan_shaungji_check_tmp[i]+=*(tiaoxianduan[i].shangpan_shaungji_qigang[j]);
		}
		for (k = 0;k<SHANGPAN_DANJI_NUM; k++){
			tiaoxianduan[i].shangpan_danji_qigang[k]=&g_InteralMemory.KeepWord[478+k+35*i];
			shangpan_danji_check_tmp[i]+=*(tiaoxianduan[i].shangpan_danji_qigang[k]);
		}
		for (m = 0;m<XIAPAN_SHUANGJI_NUM; m++){
			tiaoxianduan[i].xiapan_shuangji_qigang[m]=&g_InteralMemory.KeepWord[484+m+35*i];
			xiapan_shaungji_check_tmp[i]+=*(tiaoxianduan[i].xiapan_shuangji_qigang[m]);
		}

		g_InteralMemory.KeepWord[520 + i] = (shangpan_shaungji_check_tmp[i]+shangpan_danji_check_tmp[i]);
		g_InteralMemory.KeepWord[528 + i] = xiapan_shaungji_check_tmp[i];

		tiaoxianduan[i].shangpan_checkout = &g_InteralMemory.KeepWord[520 + i];
		tiaoxianduan[i].xiapan_checkout = &g_InteralMemory.KeepWord[528 + i];
	}
	
	for(n=0;n<16;n++){
		qigang_confirm_kb[n]=&g_InteralMemory.KeepBit[65 + n];
	}
}
void TiaoXian_Youfeng_Init_once(void){
	
	int ii,j,k,m,n;
	/*************8�ε�������*****************/
		for (ii = 0; ii < tiaoshaduan_max; ii++){
			g_InteralMemory.KeepWord[156 + 10*ii] = 0;			//��ʼȦ
			g_InteralMemory.KeepWord[157 + 10*ii] = 0;			//����Ȧ
			g_InteralMemory.KeepWord[158 + 10*ii] = 0;			//ѹ����1
			g_InteralMemory.KeepWord[159 + 10*ii] = 0;			//ѹ����2
			g_InteralMemory.KeepBit[49 + ii] = 0;				//�����ѡ��
			
			//�ϲ�����ɴ�֡��ϲ�С��ɰ�֡��²�����ɴ�֡��²�С��ɰ��
			//�����������ɰ�֡�������ɰ�� ѭ������(�ٷֱ�)
			/* for (bb = 0; bb<7; bb++){
				g_InteralMemory.KeepWord[159 + 10*ii + bb] = 100;
			} */
			//�������ס�����������������
			for(j = 0;j<SHANGPAN_SHUANGJI_NUM; j++){
				g_InteralMemory.KeepWord[470+j+35*i]=0;
			}
			for(k = 0;k<SHANGPAN_DANJI_NUM; k++){
				g_InteralMemory.KeepWord[478+k+35*i]=0;
			}
			for(m = 0;m<SHANGPAN_DANJI_NUM; m++){
				g_InteralMemory.KeepWord[484+m+35*i]=0;
			}
		
			g_InteralMemory.KeepWord[520 + i] = 0;
			g_InteralMemory.KeepWord[528 + i] = 0;
		}   

		/******************************************/
		for(n=0;n<16;n++){
			g_InteralMemory.KeepBit[65 + n]=0;
		}
		tiaoxianzu_jiange = 0;
		// chudao_jiange = 10000;	//by FJW
		// shoudao_jiange = 10000; //by FJW
		tiaoxiankaiguan_kb = 0;
		tiaoxianzu_max = 5;
		tiaoxianzu = 1;
		tiaoxianzu_quanshu=0;
		tiaoxianzu_flag = 0;
		
		jiajiansujiangemaichong_kw = 10000;
		
		weisha_jiange_kw = 0;	//ιɴ���
}

int TiaoXian_Youfeng_Checkout(void){
	
	int i,j,k,m,n;
	unsigned int up_danji_checkout_tmp[8] = {0};
	unsigned int up_shaungji_checkout_tmp[8] = {0};
	unsigned int down_shuangji_checkout_tmp[8] = {0};
	unsigned int up_checkout_tmp[8]={0};
	unsigned int down_checkout_tmp[8]={0};
	unsigned int qigang_confirm_num = 0;
	
	if(tiaoxian_shiji_kb == 1){
		for(n = 0;n<16;n++)
		{
			if(qigang_confirm_kb[n] == 1){
				qigang_confirm_status = 1;
				
				qigang_confirm_num = n;
				break;
			}
		}
		
		if(qigang_confirm_status == 1){		
			for (i = 0; i < tiaoshaduan_max; i++){
				for (j = 0;j<SHANGPAN_SHUANGJI_NUM; j++){
					//tiaoxianduan[i].shangpan_shaungji_qigang[j]=&g_InteralMemory.KeepWord[470+j+35*i];
					up_shaungji_checkout_tmp[i]+=*(tiaoxianduan[i].shangpan_shaungji_qigang[j]);
				}
				for (k = 0;k<SHANGPAN_DANJI_NUM; k++){
					//tiaoxianduan[i].shangpan_danji_qigang[k]=&g_InteralMemory.KeepWord[478+k+35*i];
					up_danji_checkout_tmp[i]+=*(tiaoxianduan[i].shangpan_danji_qigang[k]);
				}
				for (m = 0;m<XIAPAN_SHUANGJI_NUM; m++){
					//tiaoxianduan[i].xiapan_shuangji_qigang[m]=&g_InteralMemory.KeepWord[484+m+35*i];
					down_shuangji_checkout_tmp[i]+=*(tiaoxianduan[i].xiapan_shuangji_qigang[m]);
				}
				
				up_checkout_tmp[i] = (up_shaungji_checkout_tmp[i]+up_danji_checkout_tmp[i]);
				down_checkout_tmp[i] = (down_shuangji_checkout_tmp[i]);
				
				if( (up_checkout_tmp[i] != *(tiaoxianduan[i].shangpan_checkout))
				  ||(down_checkout_tmp[i] != *(tiaoxianduan[i].xiapan_checkout))){
					qigang_confirm_kb[qigang_confirm_num] = 0;	//button����;
					return CHANGED;
				  }
					
			}
		}
		return NOT_CHANGED;
	}
	qigang_confirm_status = 0;
	return NOT_CHANGED;		//�����Ի��͵����ޱ仯;
}

/**������-1�����,���յ����**/
void TiaoXian_Youfeng_weisha(int duanshu){
	
	int zushu = 0;
	int pianshu = 0;
	
	/**�ݴ���**/
	if(duanshu>8)
		return;
	
	Tiaoxian_Youfeng_ComInfo_Get(duanshu);
	
	/**tongxunstart[][]�Լ�tongxunnum[][]������Tiaoxian_Youfeng_ComInfo_Get()�д�**/
	for (zushu =0; zushu < tiaoxianzu; zushu++){
		for(pianshu = 0;pianshu<PIANSHU_MAX;pianshu++){	
			/***ͨѶ��ʼ***/
			if (tongxunstart[zushu][pianshu] == 1 && tongxunnum[zushu]][pianshu] <5){
				/***5��ͨѶ�ݴ�***/
					if (Tiaoxian_Youfeng_jidianqi_write(zushu,pianshu) != 1){
						tongxunnum[zushu][pianshu]++ ;
					}
					else{
						tongxunnum[zushu][pianshu] = 0;
						tongxunstart[zushu][pianshu] = 0;
					}
				}
			/**5��ͨѶ����֮��**/
			else if (tongxunstart[zushu][pianshu] == 1){
				tongxunnum[zushu][pianshu] = 0;
				tongxunstart[zushu][pianshu] = 0;
			}
			else if (tongxunnum[zushu][pianshu] != 0)
				tongxunnum[zushu][pianshu] = 0;	

		}
	}

}

/**ջ�ż��㹫ʽ:station = zushu+(pianshu+1)*10;**/
unsigned int Tiaoxian_Youfeng_jidianqi_write(unsigned int zushu,unsigned int pianshu){
	
	U8 Count,jdqCheck,waitTime;
	int i;
	U32 ErrorLoop;
	int station;
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./57600+0.5) -1 );		//��ͨѶ����Ҫ57600�Ĳ����ʲ��ܹ���

	U8 auchMsg[11],SendArray[11],RecArray[11];  
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	if (rULCON3!=0x03) 
		rULCON3 =0x03; 				//0x03=00 000 0 11  ��ͨ żУ�飨even�� 1ֹͣλ 8����λ
	//�ѷ干��24��̵�����
	/* switch (zushu){
	case 0:
		auchMsg[0]=25;break;
	case 1:
		auchMsg[0]=26;break;
	case 2:
		auchMsg[0]=27;break;
	case 3:
		auchMsg[0]=28;break;
	case 4:
		auchMsg[0]=29;break;
	case 5:
		auchMsg[0]=30;break;	
	} */
	station = zushu+(pianshu+1)*10;
	
	auchMsg[0]=station;		//ջ��
	auchMsg[1]=0x0F;		//������:0x0F��ʾд�����Ȧ
	
	auchMsg[2]=0x00;		//��ʼ��ַH
	auchMsg[3]=0x00;		//��ʼ��ַL
	
	auchMsg[4]=0x00;		//�������H
	auchMsg[5]=0x10;		//�������L
	
	auchMsg[6]=0x02;		//�ֽ������������/8 ����������0��+1(�ֽ���Ϊ1���·�Ϊһ���ֽ�;Ϊ2��Ϊ�����ֽ�)
	
	auchMsg[7]=tongxunzhen[pianshu]&0xff;		//���ֵ1���ⲿ���һ��16λ����Ӧ�ĵ�8λֵ
	auchMsg[8]=(tongxunzhen[pianshu] >> 8);	//���ֵ2���ⲿ���һ��16λ����Ӧ�ĸ�8λֵ
	
	auchMsg[9]=(CRC(auchMsg,9)) & 0xff;
	auchMsg[10]=(CRC(auchMsg,9))>>8;
	for (i=0;i<11;i++)
	{
		SendArray[i]=auchMsg[i];
	}



	rGPHDAT |= 0x1000;	//GPH12	+Write
	Delay(DELAY_TIME_RTU);

	for (Count=0;Count<11;Count++)

	{
		while((!(rUTRSTAT3 & 0x4)) && (waitTime<=ErrorLoop)){
			for (i=0;i<50;i++){
				waitTime++;wdt_feed_dog();main_enter_flag = 1;
			}
		}
		waitTime=0;
		WrUTXH3(SendArray[Count]);wdt_feed_dog();main_enter_flag = 1;
		while((!(rUTRSTAT3 & 0x4)) && (waitTime<=ErrorLoop)){
			for (i=0;i<50;i++){
				waitTime++;wdt_feed_dog();main_enter_flag = 1;
			}
		}
	}
	
	/***************ͨѶЭ��485 Read********************/
	rGPHDAT &= 0xefff;	//GPH12	-Read
	Count = 0;
	while ((RecArray[0]=qz_Uart_Getch())!=25 && Count<=20)
	{
		Count++;
	}
	if (RecArray[0]==25){
		for (Count=1;Count<10;Count++)
		{
			RecArray[Count]=qz_Uart_Getch();
			if (RecArray[Count]==auchMsg[Count]){
				jdqCheck++;
			}
		}
		if (jdqCheck>=3){
			return 1;		//10��ͨѶ֡����3��ƥ��ͷ���1
		}
		else{
			return 0;
		}	
	}
	else
		{
			//rUBRDIV3 = ( (int)(SYS_PCLK/16./9600+0.5) -1 );	//��������֮ǰ�������ʻ���ȥ
			return 0;
		}
}

void Tiaoxian_Youfeng_ComInfo_Set(void){
	unsigned int tongxunzhen_tmp[PIANSHU_MAX] = {0x0000};	
	if(qigang_confirm_status)
}
void Tiaoxian_Youfeng_ComInfo_Get(duanshu){
	
}

void TiaoXian_Youfeng_Reset(void){
	
}

void Tiaoxian_Youfeng_Yazhen_Get_Zero(unsigned int yazhen_num){
	
}

/***ѹ����ģ��***/	
void Tiaoxian_Youfeng_Yazhen(void){
	
		if(tiaoxianduan[duanshu_enable_cur].yazheng_motor_1st != 0){
			yazheng_motor_1st_start = 1;
		}
		else{
			Tiaoxian_Youfeng_yazhen_get_zero(yazhen_no_1);
			yazheng_motor_1st_start = 0;
		}
		
		if(tiaoxianduan[duanshu_enable_cur].yazheng_motor_2nd != 0)
		{
			yazheng_motor_2nd_start = 1;
		}
		else{
			Tiaoxian_Youfeng_yazhen_get_zero(yazhen_no_2);
			yazheng_motor_1st_start = 0;
		}
}