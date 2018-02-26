#include "includes.h"

#define SHANGPAN_SHUANGJI_NUM		8
#define SHANGPAN_DANJI_NUM			6
#define XIAPAN_SHUANGJI_NUM			21

/**ѹ�뱶��**/
#define Yazhen_Factor				g_InteralMemory.KeepWord[780]


unsigned int tongxunstart[ZUSHU_MAX][PIANSHU_MAX] = {0};
unsigned int tongxunnum[ZUSHU_MAX][PIANSHU_MAX] = {0};
unsigned int tongxunzhen[tiaoshaduan_max][PIANSHU_MAX] = {0x0000};
unsigned char* qigang_confirm_kb[16];			//16��Ϊһ��8��*2=16
unsigned int jiajiaStatus = 0;

unsigned int qigang_confirm_status = 0;				//��������ȷ��֮��tongxunzhen��set
unsigned int enter_already = 0;	//����checkout��ֹ�ظ�����,CHANGED֮��Set���֮��������
int changed_duan = -1;							//����duan�ı�֮���ͨѶ֡������
int changed_qigang_button = -1;
TIAOXIANDUAN tiaoxianduan[tiaoshaduan_max];

/*************************************************
Function(��������): TiaoXian_Youfeng_App(void);
Description(�������ܡ����ܵȵ�����): ���߹��ܴ���
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
void TiaoXian_Youfeng_App(void){
	// unsigned int zushu=0;
	static int duanshu_enable_prev=10;
	static int duanshu_enable_cur=11;

	at_check(dapan_round);
	duanshu_enable_cur = between_check(dapan_round);
	
	if((tiaoxian_shiji_kb == 1)){
		int checkout_tmp = 0;
		checkout_tmp = TiaoXian_Youfeng_Checkout();
		
		if(checkout_tmp == CHANGED)
			Tiaoxian_Youfeng_ComInfo_Set();
	}
	
	//1.�������duanshu_enable_tmp != duanshu_enable��֤�����ͨѶ;
	//2.�Ի���ʱ����Ҫ���ͨѶqigang_confirm_status�����Ի�
	if((duanshu_enable_prev != duanshu_enable_cur && current_stage != caijianduan) \
	  || (qigang_confirm_status == 1)){
		//�ݴ�
		if(duanshu_enable_cur>8)
			return;
		
		Tiaoxian_Youfeng_Yazhen(duanshu_enable_prev,duanshu_enable_cur);
		duanshu_enable_prev = duanshu_enable_cur;//��ֹ�ظ�����(ͨѶ)
		
		TiaoXian_Youfeng_weisha(duanshu_enable_cur);
	}
}


/*************************************************
Function(��������): TiaoXian_Youfeng_Init(void);
Description(�������ܡ����ܵȵ�����): ���߹��ܳ�ʼ������(ÿ���ϵ綼�����һ��)
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
void TiaoXian_Youfeng_Init(void){
	int i,j,k,m,n;
	unsigned int shangpan_danji_check_tmp[8]={0};
	unsigned int shangpan_shuangji_check_tmp[8]={0};
	unsigned int xiapan_shaungji_check_tmp[8]={0};

	//д��8·ͨѶ֡���ⲿ�����Ӧ��״̬(�˴���Ҫ��ʼ������Ϊ���ļ̵������������⣬�̵�������û����Ӧ�ò���Ҫ����Щ)
	/* if (tiaoxiankaiguan_kb ==1 ){
		for (i = 0 ;i<tiaoshaduan_max;i++){
			for (j = 0 ;j<PIANSHU_MAX;j++)
				tongxunzhen[i][j] = 0x0;//��ʼ���̵�������
		}

		for (i = 0 ; i < ZUSHU_MAX; i++){
			for (j = 0 ; j < PIANSHU_MAX; j++)
				for (bb = 0 ; bb < 5 ; bb++){
					//ͨѶ�ɹ�֮���ֱ���˳�ѭ��,5��Ϊ�ݴ�
					if (Tiaoxian_Youfeng_jidianqi_write(i,j,1) == 1){//ѡ���һ�ε�ͨѶ֡��������
						break;
					}
				}
		}
	}
 */
	/***8�β�����ʼ��***/
	for (i = 0; i < tiaoshaduan_max; i++){
		tiaoxianduan[i].kaishiquanshu = &g_InteralMemory.KeepWord[156 + 10*i];
		tiaoxianduan[i].jieshuquanshu = &g_InteralMemory.KeepWord[157 + 10*i];
		tiaoxianduan[i].ewaiduan_choose=&g_InteralMemory.KeepBit[49 + i];
		tiaoxianduan[i].yazheng_motor_1st=&g_InteralMemory.KeepWord[158 + 10*i];
		tiaoxianduan[i].yazheng_motor_2nd=&g_InteralMemory.KeepWord[159 + 10*i];
		wdt_feed_dog();main_enter_flag = 1;
		/**�������������Լ���������**/
		for (j = 0;j<SHANGPAN_SHUANGJI_NUM; j++){
			tiaoxianduan[i].shangpan_shaungji_qigang[j]=&g_InteralMemory.KeepWord[470+j+35*i];
			shangpan_shuangji_check_tmp[i]+=*(tiaoxianduan[i].shangpan_shaungji_qigang[j]);
		}
		for (k = 0;k<SHANGPAN_DANJI_NUM; k++){
			tiaoxianduan[i].shangpan_danji_qigang[k]=&g_InteralMemory.KeepWord[478+k+35*i];
			shangpan_danji_check_tmp[i]+=*(tiaoxianduan[i].shangpan_danji_qigang[k]);
		}
		for (m = 0;m<XIAPAN_SHUANGJI_NUM; m++){
			tiaoxianduan[i].xiapan_shuangji_qigang[m]=&g_InteralMemory.KeepWord[484+m+35*i];
			xiapan_shaungji_check_tmp[i]+=*(tiaoxianduan[i].xiapan_shuangji_qigang[m]);
		}

		g_InteralMemory.KeepWord[760 + i] = (shangpan_shuangji_check_tmp[i]+shangpan_danji_check_tmp[i]);
		g_InteralMemory.KeepWord[768 + i] = xiapan_shaungji_check_tmp[i];

		tiaoxianduan[i].shangpan_checkout = &g_InteralMemory.KeepWord[760 + i];
		tiaoxianduan[i].xiapan_checkout = &g_InteralMemory.KeepWord[768 + i];
	}
	
	for(n=0;n<16;n++){
		qigang_confirm_kb[n]=&g_InteralMemory.KeepBit[65 + n];
	}
}

/*************************************************
Function(��������): TiaoXian_Youfeng_Init_once(void);
Description(�������ܡ����ܵȵ�����): ���߹��ܳ�ʼ������(ֻ�е�һ���ϵ�ᱻ����)
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
void TiaoXian_Youfeng_Init_once(void){
	
	int i,j,k,m,n;
	/*************8�ε�������*****************/
		for (i = 0; i < tiaoshaduan_max; i++){
			g_InteralMemory.KeepWord[156 + 10*i] = 0;			//��ʼȦ
			g_InteralMemory.KeepWord[157 + 10*i] = 0;			//����Ȧ
			g_InteralMemory.KeepWord[158 + 10*i] = 0;			//ѹ����1
			g_InteralMemory.KeepWord[159 + 10*i] = 0;			//ѹ����2
			g_InteralMemory.KeepBit[49 + i] = 0;				//�����ѡ��
			wdt_feed_dog();main_enter_flag = 1;
			//�ϲ�����ɴ�֡��ϲ�С��ɰ�֡��²�����ɴ�֡��²�С��ɰ��
			//�������������ɰ�֡�������ɰ�� ѭ������(�ٷֱ�)
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
		
			g_InteralMemory.KeepWord[760 + i] = 0;
			g_InteralMemory.KeepWord[768 + i] = 0;
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
		
		//weisha_jiange_kw = 0;	//ιɴ���,�ѷ�ȥ��
}

/*************************************************
Function(��������): TiaoXian_Youfeng_Checkout(void);
Description(�������ܡ����ܵȵ�����): �Ƿ���Ҫ��������ͨѶ֡�ļ��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):���ⲿ�Ի����������µ�ʱ���Լ�����ȷ����ť������֮�󣬻����checkout()
Return: CHANGED/NOT_CHANGED
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
int TiaoXian_Youfeng_Checkout(void){
	unsigned int i,j,k,m,n;
	unsigned int up_danji_checkout_tmp[8] = {0};
	unsigned int up_shuangji_checkout_tmp[8] = {0};
	unsigned int down_shuangji_checkout_tmp[8] = {0};
	unsigned int up_checkout_tmp[8]={0};
	unsigned int down_checkout_tmp[8]={0};
	
	int qigang_confirm_num = -1;	//���ڼ�¼�ĸ�������������,-1��ʾ�ް���������
	
	for(n = 0;n<16;n++)	{
		if(*(qigang_confirm_kb[n]) == 1 && enter_already == 0){//
			qigang_confirm_num = n;
			/****�����ⲿ��ȫ�ֱ���*****/
			qigang_confirm_status = 1;
			enter_already = 1;
			changed_qigang_button = n;
			changed_duan = (qigang_confirm_num/2);
			break;
		}
	}
	
	if(qigang_confirm_status == 1 && qigang_confirm_num >= 0){	
		// for (i = 0; i < tiaoshaduan_max; i++){
			wdt_feed_dog();main_enter_flag = 1;
			/**���̱��ı�**/
			if((qigang_confirm_num%2) == 0){
				i = (qigang_confirm_num/2);//��ȡ��
				for (j = 0;j<SHANGPAN_SHUANGJI_NUM; j++){
					//tiaoxianduan[i].shangpan_shaungji_qigang[j]=&g_InteralMemory.KeepWord[470+j+35*i];
					up_shuangji_checkout_tmp[i]+=*(tiaoxianduan[i].shangpan_shaungji_qigang[j]);
				}
				for (k = 0;k<SHANGPAN_DANJI_NUM; k++){
					//tiaoxianduan[i].shangpan_danji_qigang[k]=&g_InteralMemory.KeepWord[478+k+35*i];
					up_danji_checkout_tmp[i]+=*(tiaoxianduan[i].shangpan_danji_qigang[k]);
				}
				up_checkout_tmp[i] = (up_shuangji_checkout_tmp[i]+up_danji_checkout_tmp[i]);
				
				if(up_checkout_tmp[i] != *(tiaoxianduan[i].shangpan_checkout)){
					*(tiaoxianduan[i].shangpan_checkout) = up_checkout_tmp[i];
					// *(qigang_confirm_kb[qigang_confirm_num]) = 0;	//button����;set��֮���ڰѰ���������
					return CHANGED;
				}
				else{
					*(qigang_confirm_kb[qigang_confirm_num]) = 0;	//button����;
					enter_already = 0;
					return NOT_CHANGED;
				}
			}
			/**���̱��ı�**/
			else{
				i = (qigang_confirm_num/2);
				for(m = 0;m<XIAPAN_SHUANGJI_NUM; m++){
					//tiaoxianduan[i].xiapan_shuangji_qigang[m]=&g_InteralMemory.KeepWord[484+m+35*i];
					down_shuangji_checkout_tmp[i]+=*(tiaoxianduan[i].xiapan_shuangji_qigang[m]);
				}
				down_checkout_tmp[i] = (down_shuangji_checkout_tmp[i]);
				
				if((down_checkout_tmp[i] != *(tiaoxianduan[i].xiapan_checkout))){
					*(tiaoxianduan[i].xiapan_checkout)= down_checkout_tmp[i];
					// *(qigang_confirm_kb[qigang_confirm_num]) = 0;	//button����;
					return CHANGED;
				}
				else{
					*(qigang_confirm_kb[qigang_confirm_num]) = 0;	//button����;
					enter_already = 0;
					return NOT_CHANGED;
				}
			}
	}
	qigang_confirm_status=0;
	return NOT_CHANGED;
}

/*************************************************
Function(��������): TiaoXian_Youfeng_weisha(int duanshu);
Description(�������ܡ����ܵȵ�����): �ⲿ�̵����忪ʼͨѶ��һ����ҪͨѶ4Ƭ*zushu��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): duanshuѡ����һ�ε�ͨѶ֡(һ��8��:0-7)
Output(�����������˵��):
Return: CHANGED/NOT_CHANGED
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
/**������-1�����,���յ����**/
void TiaoXian_Youfeng_weisha(int duanshu){
	
	int zushu = 0;
	int pianshu = 0;
	
	/**�ݴ���**/
	if(duanshu>8)
		return;
	
	/**tongxunstart[][]�Լ�tongxunnum[][]������Tiaoxian_Youfeng_ComInfo_Get()�д�**/
	for (zushu =0; zushu < tiaoxianzu; zushu++){
		for(pianshu = 0;pianshu<PIANSHU_MAX;pianshu++){	
			/***ͨѶ��ʼ***/
			if (tongxunstart[zushu][pianshu] == 1 && tongxunnum[zushu][pianshu] <5){
				/***5��ͨѶ�ݴ�***/
				if(duanshu >= 0){
					if (Tiaoxian_Youfeng_jidianqi_write(zushu,pianshu,duanshu) != 1){
						tongxunnum[zushu][pianshu]++ ;
					}
					else{
						tongxunnum[zushu][pianshu] = 0;
						tongxunstart[zushu][pianshu] = 0;
					}
				}
				else{//�յ�
					if (Tiaoxian_Youfeng_jidianqi_zero(zushu,pianshu) != 1){
						tongxunnum[zushu][pianshu]++ ;
					}
					else{
						tongxunnum[zushu][pianshu] = 0;
						tongxunstart[zushu][pianshu] = 0;
					}
				}
			}
			/**5��ͨѶ����֮��**/
			else if(tongxunstart[zushu][pianshu] == 1){
				tongxunnum[zushu][pianshu] = 0;
				tongxunstart[zushu][pianshu] = 0;
			}
			else if(tongxunnum[zushu][pianshu] != 0)
				tongxunnum[zushu][pianshu] = 0;	
		}
	}

}

/*************************************************
Function(��������): Tiaoxian_Youfeng_jidianqi_write(unsigned int zushu,unsigned int pianshu,unsigned int duanshu);
Description(�������ܡ����ܵȵ�����): д�̵���ͨѶ֡,ͨѶдͨѶ֡
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): duanshuѡ����һ�ε�ͨѶ֡(һ��8��:0-7)
Output(�����������˵��):
Return: CHANGED/NOT_CHANGED
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
/**ջ�ż��㹫ʽ:station = zushu+(pianshu+1)*10;**/
unsigned int Tiaoxian_Youfeng_jidianqi_write(unsigned int zushu,unsigned int pianshu,unsigned int duanshu){
	
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
	
	station = zushu+(pianshu+1)*10;
	
	auchMsg[0]=station;		//ջ��
	auchMsg[1]=0x0F;		//������:0x0F��ʾд�����Ȧ
	
	auchMsg[2]=0x00;		//��ʼ��ַH
	auchMsg[3]=0x00;		//��ʼ��ַL
	
	auchMsg[4]=0x00;		//�������H
	auchMsg[5]=0x10;		//�������L
	
	auchMsg[6]=0x02;		//�ֽ������������/8 ����������0��+1(�ֽ���Ϊ1���·�Ϊһ���ֽ�;Ϊ2��Ϊ�����ֽ�)
	
	auchMsg[7]=tongxunzhen[duanshu][pianshu]&0xff;		//���ֵ1���ⲿ���һ��16λ����Ӧ�ĵ�8λֵ
	auchMsg[8]=(tongxunzhen[duanshu][pianshu] >> 8);	//���ֵ2���ⲿ���һ��16λ����Ӧ�ĸ�8λֵ
	
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

/*************************************************
Function(��������): Tiaoxian_Youfeng_jidianqi_zero(unsigned int zushu,unsigned int pianshu);
Description(�������ܡ����ܵȵ�����): д�̵���ͨѶ֡,ͨѶдͨѶ֡0
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): duanshuѡ����һ�ε�ͨѶ֡(һ��8��:0-7)
Output(�����������˵��):
Return: CHANGED/NOT_CHANGED
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
unsigned int Tiaoxian_Youfeng_jidianqi_zero(unsigned int zushu,unsigned int pianshu){
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
	
	station = zushu+(pianshu+1)*10;
	
	auchMsg[0]=station;		//ջ��
	auchMsg[1]=0x0F;		//������:0x0F��ʾд�����Ȧ
	
	auchMsg[2]=0x00;		//��ʼ��ַH
	auchMsg[3]=0x00;		//��ʼ��ַL
	
	auchMsg[4]=0x00;		//�������H
	auchMsg[5]=0x10;		//�������L
	
	auchMsg[6]=0x02;		//�ֽ������������/8 ����������0��+1(�ֽ���Ϊ1���·�Ϊһ���ֽ�;Ϊ2��Ϊ�����ֽ�)
	
	auchMsg[7]=0x0000&0xff;		//���ֵ1���ⲿ���һ��16λ����Ӧ�ĵ�8λֵ
	auchMsg[8]=(0x0000 >> 8);	//���ֵ2���ⲿ���һ��16λ����Ӧ�ĸ�8λֵ
	
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

/*************************************************
Function(��������): Tiaoxian_Youfeng_ComInfo_Set(unsigned int duanshu);//����ѭ��SET̫����ʱ��
Description(�������ܡ����ܵȵ�����): �����ȡ����һ��ȷ����ť������;
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): duanshuѡ����һ�ε�ͨѶ֡(һ��8��:0-7)
Output(�����������˵��):
Return: CHANGED/NOT_CHANGED
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
void Tiaoxian_Youfeng_ComInfo_Set(void){
	unsigned int j;
	
	if(changed_duan >= 0 && changed_duan<tiaoshaduan_max){
		if(tiaoxianduan[changed_duan].jieshuquanshu != 0){
			for(j=0;j<PIANSHU_MAX;j++){
				Tiaoxian_Youfeng_Pianshu_Set(changed_duan,j);
			}
			if(changed_qigang_button >= 0){
				*(qigang_confirm_kb[changed_qigang_button]) = 0;	//button����;set��֮���ڰѰ���������
				enter_already = 0;									//�����־λֵ�ÿ���
				changed_qigang_button = -1;							//������
			}
		}
		qigang_confirm_status = 0;
		changed_duan = -1;//����������
	}
}

/*************************************************
Function(��������): Tiaoxian_Youfeng_Pianshu_Set(int duanshu,unsigned int pianshu);//����ѭ��SET̫����ʱ��
Description(�������ܡ����ܵȵ�����): �����ȡ����һ��ȷ����ť������;
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): duanshuѡ����һ�ε�ͨѶ֡(һ��8��:0-7)
Output(�����������˵��):
Return: CHANGED/NOT_CHANGED
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
void Tiaoxian_Youfeng_Pianshu_Set(int duanshu,unsigned int pianshu){
	int i,j;
	
	switch(pianshu){
		case 0:
			wdt_feed_dog();main_enter_flag = 1;
			for(i = 0;i<8;i++){
				if(g_InteralMemory.KeepWord[470+i+35*duanshu] == DANJI){
					tongxunzhen[duanshu][0] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][0] |=(0x01<<(i*2));
				}
				else if(g_InteralMemory.KeepWord[470+i+35*duanshu] == SHUANGJI){
					tongxunzhen[duanshu][0] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][0] |=(0x03<<(i*2));
				}
				else{
					tongxunzhen[duanshu][0] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][0] |=(0x00<<(i*2));
				}
			}
			break;
		case 1:
			wdt_feed_dog();main_enter_flag = 1;
			//6·��������
			for(i = 0;i<6;i++){
				if(g_InteralMemory.KeepWord[478+i+35*duanshu] == DANJI){
					tongxunzhen[duanshu][1] &=(~(0x01<<(i)));
					tongxunzhen[duanshu][1] |=(0x01<<(i));
				}
				else{
					tongxunzhen[duanshu][1] &=(~(0x01<<(i)));
					tongxunzhen[duanshu][1] |=(0x00<<(i));
				}
			}
			//5·˫������
			wdt_feed_dog();main_enter_flag = 1;
			for(j = 0;j<5;j++){
				if(g_InteralMemory.KeepWord[484+j+35*duanshu] == DANJI){
					tongxunzhen[duanshu][1] &=(~(0x03<<(j*2+6)));
					tongxunzhen[duanshu][1] |=(0x03<<(j*2+6));
				}
				else if(g_InteralMemory.KeepWord[484+j+35*duanshu] == SHUANGJI){
					tongxunzhen[duanshu][1] &=(~(0x03<<(j*2+6)));
					tongxunzhen[duanshu][1] |=(0x03<<(j*2+6));
				}
				else{
					tongxunzhen[duanshu][1] &=(~(0x03<<(j*2+6)));
					tongxunzhen[duanshu][1] |=(0x00<<(j*2+6));
				}
			}
			break;
		case 2:
			wdt_feed_dog();main_enter_flag = 1;
			for(i = 0;i<8;i++){
				if(g_InteralMemory.KeepWord[489+i+35*duanshu] == DANJI){
					tongxunzhen[duanshu][2] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][2] |=(0x01<<(i*2));
				}
				else if(g_InteralMemory.KeepWord[489+i+35*duanshu] == SHUANGJI){
					tongxunzhen[duanshu][2] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][2] |=(0x03<<(i*2));
				}
				else{
					tongxunzhen[duanshu][2] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][2] |=(0x00<<(i*2));
				}
			}
			break;
		case 3:
			wdt_feed_dog();main_enter_flag = 1;
			for(i = 0;i<8;i++){
				if(g_InteralMemory.KeepWord[497+i+35*duanshu] == DANJI){
					tongxunzhen[duanshu][3] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][3] |=(0x01<<(i*2));
				}
				else if(g_InteralMemory.KeepWord[497+i+35*duanshu] == SHUANGJI){
					tongxunzhen[duanshu][3] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][3] |=(0x03<<(i*2));
				}
				else{
					tongxunzhen[duanshu][3] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][3] |=(0x00<<(i*2));
				}
			}
			break;
		default:
			break;
	}
}

/*************************************************
Function(��������): TiaoXian_Youfeng_Reset(void);//����ѭ��SET̫����ʱ��
Description(�������ܡ����ܵȵ�����): ��λ��־λ��λ(�ָ�ȫ�ֱ�����ʼ������);
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: CHANGED/NOT_CHANGED
Others: 
Author:����ΰ
Modified:
Commented:����ΰ
*************************************************/
void TiaoXian_Youfeng_Reset(void){
	yazheng_motor_1st_start	= 0;
	yazheng_motor_2nd_start	= 0;
	
	yazhen_pulse_cmp_1st = 0;	
	yazhen_pulse_cmp_2nd = 0;   
	
	yazhen_motor_pulse_1st = 0;
	yazhen_motor_pulse_2nd = 0;
	
	qigang_confirm_status = 0;
	
	enter_already = 0;
	changed_duan = -1;			
	changed_qigang_button = -1;
}


void Tiaoxian_Youfeng_Yazhen_Get_Zero(unsigned int yazhen_num){
	(void)yazhen_num;
}

/***ѹ����ģ�飬���е�ѹ�����ļ�������Ӧ�÷������
	encoder1_process����++/set;��֪s;ͨ���жϵó�t�������v(�����������)***/	
void Tiaoxian_Youfeng_Yazhen(int duanshu_prev,int duanshu_cur){
	
	INT16U yazhen_cur_1st = 0;
	INT16U yazhen_prev_1st = 0;
	INT16U yazhen_cur_2nd = 0;
	INT16U yazhen_prev_2nd = 0;
	INT16U kaishi_quanshu = 0;
	INT16U jieshu_quanshu = 0;
	unsigned int yazhen_total_pulse_1st = 0;
	unsigned int yazhen_total_pulse_2nd = 0;
	//��һ�α߽���������Ϊduanshu_prev == 10;
	if(duanshu_prev >= 8 || duanshu_prev == -1){
		duanshu_prev = 0;
	}
	
	//��ǰ�α��ر������
	if(duanshu_cur == -1){
		Tiaoxian_Youfeng_Yazhen_Get_Zero(1);//yazhen_no_1
		Tiaoxian_Youfeng_Yazhen_Get_Zero(2);//yazhen_no_2
		
		yazheng_motor_1st_start = 0;
		yazheng_motor_2nd_start = 0;
		return;
	}
	//�ݴ�
	if(duanshu_cur == -1 || duanshu_prev == -1)
		return;
	
	yazhen_cur_1st = *(tiaoxianduan[duanshu_cur].yazheng_motor_1st);
	yazhen_prev_1st = *(tiaoxianduan[duanshu_prev].yazheng_motor_1st);
	yazhen_cur_2nd = *(tiaoxianduan[duanshu_cur].yazheng_motor_2nd);
	yazhen_prev_2nd = *(tiaoxianduan[duanshu_prev].yazheng_motor_2nd);

	kaishi_quanshu = *(tiaoxianduan[duanshu_cur].kaishiquanshu);
	jieshu_quanshu = *(tiaoxianduan[duanshu_cur].jieshuquanshu);
	
	//��һ��or����֮���һ��
	if(duanshu_cur == 0 || (duanshu_prev == 0 && duanshu_cur != 1)){
		/**1.��ȡ��һ��ѹ�� 1 ����Ҫ��ֵ
		   2.��ǰһ�ιرյ�ʱ�򣬺�һ�δ򿪣����ҵ�ǰ���ǵڶ���(duanshu_cur!=1)**/
		if(yazhen_cur_1st != 0){
			if((jieshu_quanshu > kaishi_quanshu)){
				yazhen_motor_pulse_1st = (yazhen_cur_1st - 0)*Yazhen_Factor;
				yazhen_total_pulse_1st = (jieshu_quanshu - kaishi_quanshu)*encoder1_cal_factor;
				yazhen_pulse_cmp_1st = yazhen_total_pulse_1st/yazhen_motor_pulse_1st;
				if(yazhen_pulse_cmp_1st < 2){
					yazhen_pulse_cmp_1st = 2;//��֤�½���
				}
				yazheng_motor_1st_start = 1;
			}
		}
		else{
			Tiaoxian_Youfeng_Yazhen_Get_Zero(1);//yazhen_no_1
			yazheng_motor_1st_start = 0;
		}
		/**��ȡ��һ��ѹ�� 2 ����Ҫ��ֵ**/
		if(yazhen_cur_2nd != 0){
			if((jieshu_quanshu > kaishi_quanshu)){
				yazhen_motor_pulse_2nd = (yazhen_cur_2nd - 0)*Yazhen_Factor;
				yazhen_total_pulse_2nd = (jieshu_quanshu - kaishi_quanshu)*encoder1_cal_factor;
				yazhen_pulse_cmp_2nd = yazhen_total_pulse_2nd/yazhen_motor_pulse_2nd;
				if(yazhen_pulse_cmp_2nd < 2){
					yazhen_pulse_cmp_2nd = 2;//��֤�½���
				}
				yazheng_motor_2nd_start = 1;
			}
		}
		else{
			Tiaoxian_Youfeng_Yazhen_Get_Zero(2);//yazhen_no_2
			yazheng_motor_2nd_start = 0;
		}
		
	}
	else{
		/**��ȡ��i>1��ѹ�� 1 ����Ҫ��ֵ**/
		if(yazhen_cur_1st != 0){
			if((yazhen_cur_1st > yazhen_prev_1st) && (jieshu_quanshu > kaishi_quanshu)){
				yazhen_motor_pulse_1st = (yazhen_cur_1st - yazhen_prev_1st)*Yazhen_Factor;
				yazhen_total_pulse_1st = (jieshu_quanshu - kaishi_quanshu)*encoder1_cal_factor;
				yazhen_pulse_cmp_1st = yazhen_total_pulse_1st/yazhen_motor_pulse_1st;
				if(yazhen_pulse_cmp_1st < 2){
					yazhen_pulse_cmp_1st = 2;//��֤�½���
				}
				yazheng_motor_1st_start = 1;
			}
			//ʣ�µ��������������ֱ������	��ע��yazhen_cur_1st == yazhen_prev_1st��������ǲ����κθı�;
		}
		else{
			Tiaoxian_Youfeng_Yazhen_Get_Zero(1);//yazhen_no_1
			yazheng_motor_1st_start = 0;
		}
		/**��ȡ��i>1��ѹ�� 1 ����Ҫ��ֵ**/
		if(yazhen_cur_2nd != 0){
			if((yazhen_cur_2nd > yazhen_prev_2nd) && (jieshu_quanshu > kaishi_quanshu)){
				yazhen_motor_pulse_2nd = (yazhen_cur_2nd - yazhen_prev_2nd)*Yazhen_Factor;
				yazhen_total_pulse_2nd = (jieshu_quanshu - kaishi_quanshu)*encoder1_cal_factor;
				yazhen_pulse_cmp_2nd = yazhen_total_pulse_2nd/yazhen_motor_pulse_2nd;
				if(yazhen_pulse_cmp_2nd < 2){
					yazhen_pulse_cmp_2nd = 2;//��֤�½���
				}
				yazheng_motor_2nd_start = 1;
			}
			//ʣ�µ��������������ֱ������	��ע��yazhen_cur_1st == yazhen_prev_1st��������ǲ����κθı�;
		}
		else{
			Tiaoxian_Youfeng_Yazhen_Get_Zero(2);//yazhen_no_2
			yazheng_motor_2nd_start = 0;
			}
		}
	
}

/*************************************************
Function(��������): between_check(unsigned int roundShineng)
Description(�������ܡ����ܵȵ�����): ��Ҫ����shinengpanduan()�������жϵ�ǰ�׶��Ƿ����ڵ���Ȧ��֮��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): songsha_fre_change(void);		shinengpanduan(void);

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): roundShineng��������ʹ��(����dapan_round)
Output(�����������˵��):
Return: i:��ǰ�ڵ��ߵĵ�i��
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
int between_check(unsigned int roundShineng){
	int i;
	
	//check������Ҫ���ڵ��߹���,��δ�򿪵��߹��ܣ���ֱ�ӷ���-1��
	if (tiaoxiankaiguan_kb == 0)
		return -1;
	
	for(i=0;i<tiaoshaduan_max;i++){
		
		/***����������1.���ڿ�ʼȦ��
					2.����Ȧ��Ϊ0��ʾ�ر�
					3.С�ڽ���Ȧ�����ߵ������һȦ
		***/
		if (roundShineng >= *tiaoxianduan[i].kaishiquanshu			
		&& *tiaoxianduan[i].jieshuquanshu							
		&& (roundShineng < *tiaoxianduan[i].jieshuquanshu			
		|| 	*tiaoxianduan[i].jieshuquanshu == 
			(daduanquanshu + middlequanshu + xiaoduanquanshu + caijiaoquanshu + langfeiquanshu))){
			
			
			if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
				return i;/***�ж��Ƿ��Ƕ���Σ����ⲿ�����Ƿ��***/
			}
			else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
				return i;/***���Ƕ���Σ��������ڶ�***/
			}
		}
	}
	return -1;
}


/*************************************************
Function(��������): unsigned int at_check(unsigned int roundShineng)
Description(�������ܡ����ܵȵ�����): 1.��Ҫ���ڱ�Ƶ���ı��٣���Ҫ��bianpingqi_speed_cal()������
								2.��Ҫ�޸���ȫ�ֱ���tiaoxianzu/tiaoxianzu_flag
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): bianpingqi_speed_cal(void)

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): roundShineng��dapan_roundֵ
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
unsigned int at_check(unsigned int roundShineng){
	int i;
	
	/***��������(λ��)��1.������Ȧ����=0����encoder1_process()��++
					  2.��������Ȧ����=0
					  3.������Ȧ��%��������Ȧ��:���Ȧ���ı���
					  4.jiajiaStatus == 0
		�˴�������ҪΪ���жϵ�ǰ�Ƿ��������Ҫ++���жϵڶ��� �����Ƿ���Ҫ�����յ���
		�˴�ֻ�����������ӣ�������return;
	***/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 &&
		tiaoxianzu_quanshu % tiaoxianzu_jiange ==0 && jiajiaStatus == 0)
	{
		jiajiaStatus = 1;
		if ( tiaoxianzu < tiaoxianzu_max ){
			tiaoxianzu++;		
		}
	}
	
	/***��������Ȧ��Ϊ0���򲻷��飬��ͬ�����յ�***/
	else if (tiaoxianzu_jiange == 0){
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
		tiaoxianzu = tiaoxianzu_max;
	}
	
	/*****���߹�����ɣ�flag��status��λ*****/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 && 
		tiaoxianzu >= tiaoxianzu_max && 
		tiaoxianzu_quanshu >= ((tiaoxianzu_max -1)*tiaoxianzu_jiange + 1))
	{
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
	}
	
	
		
	/****	��������:1.������Ȧ��������0 || ������������1
					2.������flag����1���ҵ�������������0
		
			�˴������Ӧ�ж��Ƿ��˵�n����ߵ�ʱ����ǰһȦ����(ͨ��return 1;����������):
			if ((at_check((dapan_round+1)) && encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw)))
			
			�ж��Ƿ�ǰȦ�Ƿ���Ҫ����������:
			if ((at_check((dapan_round)) && encoder1_pulse_number < jiajiansujiangemaichong_kw))
	****/
	if ((tiaoxianzu_quanshu != 0 || tiaoxianzu_jiange == 1) && 
		 tiaoxianzu_flag == 1 && tiaoxianzu_jiange != 0)
	{
		
		if (roundShineng == (dapan_round + 1) && 
		   ((tiaoxianzu_quanshu+1) % tiaoxianzu_jiange ==0 )) {
			return 1;
		}
		
		if (roundShineng == (dapan_round ) && 
		   ((tiaoxianzu_quanshu) % tiaoxianzu_jiange ==0 )) {
			return 1;
		}
	}
	
	
	/****���߰˶��٣��˴�����Ϊ��1��Ŀ�ʼ�ͽ��������൶�������жϾ�Ϊ������������ж�****/
	for(i=0;i<tiaoshaduan_max;i++){
		/*****ֻҪ�������ߣ����Ƚ������º���******/
		if (*tiaoxianduan[i].jieshuquanshu)//��Ӧ���޲�Ϊ��
		{											
			/****Ϊ�˱�֤�ڿ�ʼȦ��Ϊ0ʱ����������ٽ��е���(����Ҫ�����֮ǰ��Ҫ���٣�0Ȧ��ʼ���ߣ���������һȦ��ʼ)
			****/
			if (*tiaoxianduan[i].kaishiquanshu == 0
			    && roundShineng == (daduanquanshu+middlequanshu+xiaoduanquanshu+
									caijiaoquanshu+langfeiquanshu))
			{
					/**************************��һ���׶�Ϊ����Σ������ⲿ����ѡ������************************************/
					if (getStage(current_stage,NEXTSTAGE) == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
					/**************************��һ���׶β�Ϊ����Σ������ⲿ���ز�ѡ������************************************/
					else if (getStage(current_stage,NEXTSTAGE) != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
			}//���ifΪ�˱�֤�ڿ�ʼȦ��Ϊ0ʱ����������ٽ��е��ߡ�
			
			/**********************************���߿�ʼ********************************************/
			else if (roundShineng == *tiaoxianduan[i].kaishiquanshu){						//��������
				if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 1;
				}
				else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 1;
				}
			}
			
			/*********************************���߽���*******************************************/
			if (roundShineng == *tiaoxianduan[i].jieshuquanshu){						//��������

				if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 2;
				}
				else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 2;
				}
			}
		}
	}
	return 0;
}