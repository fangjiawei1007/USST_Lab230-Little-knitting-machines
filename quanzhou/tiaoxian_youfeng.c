#include "includes.h"

#define SHANGPAN_SHUANGJI_NUM		8
#define SHANGPAN_DANJI_NUM			6
#define XIAPAN_SHUANGJI_NUM			21

unsigned int tongxunzhen[ZUSHU_MAX][PIANSHU_MAX] = {0x0000};


unsigned int chudao_start_status[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int shoudao_start_status[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int chudao_start[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int shoudao_start[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int chudao_shoudao_status[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int kaiguan[ZUSHU_MAX][DAOSHU_MAX] = {0x00};			//�ⲿͨѶ֡�ľ���ʵ��	
unsigned int shinengwei[DAOSHU_MAX]={0};	

unsigned int tongxunstart[ZUSHU_MAX] = {0};
unsigned int chudao_jiange_tmp[ZUSHU_MAX][DAOSHU_MAX] = {0};		//���������¼ by FJW
unsigned int shoudao_jiange_tmp[ZUSHU_MAX][DAOSHU_MAX] = {0};	//�յ������¼ by FJW
unsigned int chudao_shoudao_start[ZUSHU_MAX][DAOSHU_MAX] = {0};
//unsigned int tiaoxianzu = 0;
unsigned int jiajiaStatus = 0;

unsigned int weisha_jiange_status[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int weisha_jiange[ZUSHU_MAX][DAOSHU_MAX] = {0};

unsigned int shoudao_tozero_status[ZUSHU_MAX]={0};	//���߹��㸴λ��־
unsigned int shoudao_reset_flag = 0;
unsigned int tongxunnum[6] = {0};

TIAOXIANDUAN tiaoxianduan[tiaoshaduan_max];


/*************************************************
Function(��������): TiaoXian_Youfeng_App(void);-->tiaoxian(void)��ֲ
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
	
	int i;
	int j=0;
	unsigned int zushu;
	
	TiaoXian_Youfeng_Shinengpanduan();	//�жϵ�i�ѵ��ߵ�ʹ��
	
	/**************��tiaoxianzu_flag=1ʱ��tioaxianzu_quanshu++;*************************************************/
	for (zushu =0; zushu < tiaoxianzu; zushu++){
		
		/***�ж�6�ѵ����Ƿ�ʹ��***/
		for (i = 0; i<DAOSHU_MAX ; i++){
			
			/***********ʹ��λ==1֮������ͨѶ֡������ͨѶ��ʼ��־λ�������յ���ʼ��־��
											  ιɴ�Ĺ��ոı�Ϊ����֮���һ�������ջ���
			*********/
			if (shinengwei[i] == 1 && (weisha_jiange_kw !=0) &&
				((chudao_shoudao_start[zushu][i] == 0)||(weisha_jiange[zushu][i] >=weisha_jiange_kw))){						 
					
				if(chudao_shoudao_start[zushu][i] == 0)
				{
					weisha(i,zushu,ON);
					weisha_jiange_status[zushu][i] = 1;
				}
				
				else
				{
					weisha(i,zushu,OFF);
					weisha_jiange_status[zushu][i] = 0;
					weisha_jiange[zushu][i] = 0;
				}				
				tongxunzhen[zushu] &= (~(3<< (i*2)));				//����
				tongxunzhen[zushu] |= (kaiguan[zushu][i] << (i*2));	//����
				chudao_shoudao_status[zushu][i] = 1;
				// previous_stage[i] = current_stage;
				tongxunstart[zushu] = 1;
				tongxunnum[zushu]=0;
				chudao_shoudao_start[zushu][i] = 1;
			}
			/***********ʹ��λ==0֮��(���öβ���Ҫ���ߣ���ô��Ҫ�ѵ��ջ���)��
					   ���ó����յ�������ͨѶ��ʼ��־λ�������յ���ʼ��־
			*********/
			else if ((shinengwei[i] == 0) && (chudao_shoudao_status[zushu][i] == 1)){
				chudao_shoudao_process(i,zushu);					//�����յ�
				tongxunzhen[zushu] &= (~(3<< (i*2)));				//����
				tongxunzhen[zushu] |= (kaiguan[zushu][i] << (i*2)); //����
				chudao_shoudao_start[zushu][i] = 0; 
				
				
			}	
		}
		/***ͨѶ��ʼ***/
		if (tongxunstart[zushu] == 1 && tongxunnum[zushu] <5){
			/***5��ͨѶ�ݴ�***/
			if (tiaoxian_jidianqi_write(zushu) != 1){
				tongxunnum[zushu] ++ ;
			}
			else{
				tongxunnum[zushu] = 0;
				tongxunstart[zushu] = 0;
			}
		}
		/**5��ͨѶ����֮��**/
		else if (tongxunstart[zushu] == 1){
			tongxunnum[zushu] = 0;
			tongxunstart[zushu] = 0;
		}
		else if (tongxunnum[zushu] != 0)
			tongxunnum[zushu] = 0;
	}
	if (shoudao_reset_flag == 1){
		for (j=0;j<ZUSHU_MAX;j++){
			if (shoudao_tozero_status[j] != 0)
				return;
		}
		if (j >= ZUSHU_MAX){
			if (tiaoxianzu_jiange != 0){
				tiaoxianzu = 1;
			}
			else
				tiaoxianzu = tiaoxianzu_max;
			shoudao_reset_flag = 0;
		}
	}
	
}


void TiaoXian_Youfeng_Shinengpanduan(void){
	int i;
//	unsigned int weizhi = 0x00;
	/*ʹ��λ����,DAOSHU_MAX == 0���˴������ٴν���*/
/* 	for (i = 0 ; i <DAOSHU_MAX ; i++){
		shinengwei[i] = 0;
	} */
	/*��ȡ��ǰ�Σ������жϵ�ǰ���Ƿ���Ҫ����
	  ���߸ı�ȫ�ֱ���tiaoxianzu/tiaoxianzu_flag*/
	i = between_check(dapan_round);
	at_check(dapan_round);
	/**��ȡʹ��λ���ѷ岻��Ҫʹ��λ��ֱ���ж�ͨѶ֡����**/
/* 	if ( i != -1 && current_stage != caijianduan){
		weizhi = tiqushuzi(*tiaoxianduan[i].channal_choose);
		for (i = 0 ; i <DAOSHU_MAX ; i++){
			if ( (weizhi>>i) & 0x01){
				shinengwei[i] = 1;
			}
		}
	} */
}
void TiaoXian_Youfeng_Init(void){
	int i,j,k,m,bb;
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
			for(j = 0 ; j < PIANSHU_MAX; j++)
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
		tiaoxianduan[i].yazheng_motor_1st=&g_InteralMemory.KeepWord[158 + 10*i]
		tiaoxianduan[i].yazheng_motor_2nd=&g_InteralMemory.KeepWord[159 + 10*i]
		/**7·����Ŵ���**/
		// for (bb = 0; bb<7; bb++)		{
			// tiaoxianduan[ii].fangdabeishu[bb] = &g_InteralMemory.KeepWord[159 + 10*ii + bb];}
		
		/**�������������Լ���������**/
		for(j = 0;j<SHANGPAN_SHUANGJI_NUM; j++){
			tiaoxianduan[i].shangpan_shaungji_qigang[j]=&g_InteralMemory.KeepWord[470+j+35*i];
			shangpan_shaungji_check_tmp[i]+=*(tiaoxianduan[i].shangpan_shaungji_qigang[j]);
		}
		for(k = 0;k<SHANGPAN_DANJI_NUM; k++){
			tiaoxianduan[i].shangpan_danji_qigang[k]=&g_InteralMemory.KeepWord[478+k+35*i];
			shangpan_danji_check_tmp[i]+=*(tiaoxianduan[i].shangpan_danji_qigang[k]);
		}
		for(m = 0;m<XIAPAN_SHUANGJI_NUM; m++){
			tiaoxianduan[i].xiapan_shuangji_qigang[m]=&g_InteralMemory.KeepWord[484+m+35*i];
			xiapan_shaungji_check_tmp[i]+=*(tiaoxianduan[i].xiapan_shuangji_qigang[m]);
		}
		
		g_InteralMemory.KeepWord[520 + i] = (shangpan_shaungji_check_tmp[i]+shangpan_danji_check_tmp[i]);
		g_InteralMemory.KeepWord[528 + i] = xiapan_shaungji_check_tmp[i];
		
		tiaoxianduan[i].shangpan_checkout = &g_InteralMemory.KeepWord[520 + i];
		tiaoxianduan[i].xiapan_checkout = &g_InteralMemory.KeepWord[528 + i];
		
	}

void TiaoXian_Youfeng_Init_once(void){
	
	int ii,j,k,m;
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

void Tiaoxian_Youfeng_ComInfo_Set(void){
	if()
}
void TiaoXian_Youfeng_Reset(void);

void TiaoXian_Youfeng_Checkout(void);


unsigned int Tiaoxian_Youfeng_jidianqi_write(unsigned int zushu,unsigned int pianshu){
	
	U8 Count,jdqCheck,waitTime;
	int i;
	U32 ErrorLoop;
	int station;
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./57600+0.5) -1 );		//��ͨѶ����Ҫ57600�Ĳ����ʲ��ܹ���
#define	TIAOXIAN_DEBUG	0
#if TIAOXIAN_DEBUG
	U8 auchMsg[10],SendArray[10],RecArray[10];  
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	/* if (rULCON3!=0x2B) 
		rULCON3 =0x2B; 		 */			//0x2b=00 101 0 11  ��ͨ żУ�飨even�� 1ֹͣλ 8����λ
	
	switch (zushu){
	case 0:
		auchMsg[0]=25;
		break;
	}
	
	
	auchMsg[1]=0x0F;	//������:0x0F��ʾд�����Ȧ
	                    
	auchMsg[2]=0x00;    //��ʼ��ַH
	auchMsg[3]=0x00;    //��ʼ��ַL
	                    
	auchMsg[4]=0x00;    //�������H
	auchMsg[5]=0x08;    //�������L
	                    
	auchMsg[6]=0x01;    //�ֽ������������/8 ����������0��+1(�ֽ���Ϊ1���·�Ϊһ���ֽ�;Ϊ2��Ϊ�����ֽ�)
	                   
	auchMsg[7]=tongxunzhen[zushu] & 0xff;	//���������Ϊ�ֽ���Ϊ1�����Դ˴�ֻ��Ҫ��8λ
	
	auchMsg[8]=(CRC(auchMsg,8)) & 0xff;
	auchMsg[9]=(CRC(auchMsg,8))>>8;
	for (i=0;i<10;i++)
	{
		SendArray[i]=auchMsg[i];
	}
	
#else
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
	
	auchMsg[7]=tongxunzhen[zushu]&0xff;		//���ֵ1���ⲿ���һ��16λ����Ӧ�ĵ�8λֵ
	auchMsg[8]=(tongxunzhen[zushu] >> 8);	//���ֵ2���ⲿ���һ��16λ����Ӧ�ĸ�8λֵ
	
	auchMsg[9]=(CRC(auchMsg,9)) & 0xff;
	auchMsg[10]=(CRC(auchMsg,9))>>8;
	for (i=0;i<11;i++)
	{
		SendArray[i]=auchMsg[i];
	}
#endif


	rGPHDAT |= 0x1000;	//GPH12	+Write
	Delay(DELAY_TIME_RTU);
#if TIAOXIAN_DEBUG
	for (Count=0;Count<10;Count++)
#else
	for (Count=0;Count<11;Count++)
#endif
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

void TiaoXian_Youfeng_weisha(unsigned int i,unsigned int zushu,unsigned int on_off);
void TiaoXian_Youfeng_chudao_shoudao_process(unsigned int i,unsigned int zushu);