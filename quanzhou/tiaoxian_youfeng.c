#include "includes.h"

#define SHANGPAN_SHUANGJI_NUM		8
#define SHANGPAN_DANJI_NUM			6
#define XIAPAN_SHUANGJI_NUM			21
#define Jidianqi_Dianshu			16

unsigned int tongxunstart[ZUSHU_MAX][PIANSHU_MAX] = {0};
unsigned int tongxunnum[ZUSHU_MAX][PIANSHU_MAX] = {0};
unsigned int tongxunzhen[tiaoshaduan_max][PIANSHU_MAX] = {0x0000};
unsigned char* qigang_confirm_kb[16];			//16因为一共8段*2=16
unsigned int qigang_confirm_status;				//用于气缸确认之后，tongxunzhen的set
unsigned int jiajiaStatus = 0;
TIAOXIANDUAN tiaoxianduan[tiaoshaduan_max];



/*************************************************
Function(函数名称): TiaoXian_Youfeng_App(void);
Description(函数功能、性能等的描述): 调线功能代码
Calls (被本函数调用的函数清单): shinengpanduan();
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void TiaoXian_Youfeng_App(void){

	int i=0;
	int j=0;
	unsigned int zushu=0;
	static int duanshu_enable_prev=10;
	static int duanshu_enable_cur=11;
	//TiaoXian_Youfeng_Shinengpanduan();	//判断当前在第几段中
	at_check(dapan_round);
	duanshu_enable_cur = between_check(dapanround);
	if (TiaoXian_Youfeng_Checkout() == CHANGED){
		Tiaoxian_Youfeng_ComInfo_Set();
	}

	//1.正常情况duanshu_enable_tmp != duanshu_enable保证不多次通讯;
	//2.试机的时候需要多次通讯qigang_confirm_status用于试机
	if((duanshu_enable_prev != duanshu_enable_cur && current_stage != caijianduan) 
	  || (qigang_confirm_status == 1)){
		//容错
		if(duanshu_enable_cur>8)
			return;
		duanshu_enable_prev = duanshu_enable_cur;//防止重复进入(通讯)
		
		TiaoXian_Youfeng_weisha(duanshu_enable_cur);
	
	}
	
	if(duanshu_enable_cur != -1 && duanshu_enable_cur<8){
		Tiaoxian_Youfeng_Yazhen();
	}

}



void TiaoXian_Youfeng_Init(void){
	int i,j,k,m,n,bb;
	unsigned int shangpan_danji_check_tmp[8]={0};
	unsigned int shangpan_shuangji_check_tmp[8]={0};
	unsigned int xiapan_shaungji_check_tmp[8]={0};

	//写入8路通讯帧，外部输出对应的状态
	if (tiaoxiankaiguan_kb ==1 ){
		for (i = 0 ;i<tiaoshaduan_max;i++){
			for (j = 0 ;j<PIANSHU_MAX;j++)
				tongxunzhen[i][j] = 0x0;//初始化继电器吸合
		}

		for (i = 0 ; i < ZUSHU_MAX; i++){
			for (j = 0 ; j < PIANSHU_MAX; j++)
				for (bb = 0 ; bb < 5 ; bb++){
					//通讯成功之后会直接退出循环,5次为容错
					if (Tiaoxian_Youfeng_jidianqi_write(i,j,1) == 1){//选择第一段的通讯帧进行清零
						break;
					}
				}
		}
	}

	/***8段参数初始化***/
	for (i = 0; i < tiaoshaduan_max; i++){
		tiaoxianduan[i].kaishiquanshu = &g_InteralMemory.KeepWord[156 + 10*i];
		tiaoxianduan[i].jieshuquanshu = &g_InteralMemory.KeepWord[157 + 10*i];
		tiaoxianduan[i].ewaiduan_choose=&g_InteralMemory.KeepBit[49 + i];
		tiaoxianduan[i].yazheng_motor_1st=&g_InteralMemory.KeepWord[158 + 10*i];
		tiaoxianduan[i].yazheng_motor_2nd=&g_InteralMemory.KeepWord[159 + 10*i];
	
		/**上下盘气缸数以及气缸作用**/
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
	/*************8段调线设置*****************/
		for (ii = 0; ii < tiaoshaduan_max; ii++){
			g_InteralMemory.KeepWord[156 + 10*ii] = 0;			//开始圈
			g_InteralMemory.KeepWord[157 + 10*ii] = 0;			//结束圈
			g_InteralMemory.KeepWord[158 + 10*ii] = 0;			//压针电机1
			g_InteralMemory.KeepWord[159 + 10*ii] = 0;			//压针电机2
			g_InteralMemory.KeepBit[49 + ii] = 0;				//额外段选择
			
			//上部大送纱轮、上部小送砂轮、下部大送纱轮、下部小送砂轮
			//卷布机、五号送砂轮、六号送砂轮 循环设置(百分比)
			/* for (bb = 0; bb<7; bb++){
				g_InteralMemory.KeepWord[159 + 10*ii + bb] = 100;
			} */
			//上盘气缸、下盘气缸设置置零
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
		
		weisha_jiange_kw = 0;	//喂纱间隔
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
			if(*(qigang_confirm_kb[n]) == 1){
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
					*(qigang_confirm_kb[qigang_confirm_num]) = 0;	//button置零;
					return CHANGED;
				  }
			}
		}
		qigang_confirm_status=0;
		return NOT_CHANGED;
	}
	qigang_confirm_status = 0;
	return NOT_CHANGED;		//不打开试机就当做无变化;
}

/**包括了-1的情况,即收刀情况**/
void TiaoXian_Youfeng_weisha(int duanshu){
	
	int zushu = 0;
	int pianshu = 0;
	
	/**容错用**/
	if(duanshu>8)
		return;
	
	//Tiaoxian_Youfeng_ComInfo_Get(duanshu);
	
	/**tongxunstart[][]以及tongxunnum[][]必须在Tiaoxian_Youfeng_ComInfo_Get()中打开**/
	for (zushu =0; zushu < tiaoxianzu; zushu++){
		for(pianshu = 0;pianshu<PIANSHU_MAX;pianshu++){	
			/***通讯开始***/
			if (tongxunstart[zushu][pianshu] == 1 && tongxunnum[zushu]][pianshu] <5){
				/***5次通讯容错***/
					if (Tiaoxian_Youfeng_jidianqi_write(zushu,pianshu,duanshu) != 1){
						tongxunnum[zushu][pianshu]++ ;
					}
					else{
						tongxunnum[zushu][pianshu] = 0;
						tongxunstart[zushu][pianshu] = 0;
					}
				}
			/**5次通讯错误之后**/
			else if (tongxunstart[zushu][pianshu] == 1){
				tongxunnum[zushu][pianshu] = 0;
				tongxunstart[zushu][pianshu] = 0;
			}
			else if (tongxunnum[zushu][pianshu] != 0)
				tongxunnum[zushu][pianshu] = 0;	

		}
	}

}

/**栈号计算公式:station = zushu+(pianshu+1)*10;**/
unsigned int Tiaoxian_Youfeng_jidianqi_write(unsigned int zushu,unsigned int pianshu,unsigned int duanshu){
	
	U8 Count,jdqCheck,waitTime;
	int i;
	U32 ErrorLoop;
	int station;
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./57600+0.5) -1 );		//该通讯板需要57600的波特率才能工作

	U8 auchMsg[11],SendArray[11],RecArray[11];  
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	if (rULCON3!=0x03) 
		rULCON3 =0x03; 				//0x03=00 000 0 11  普通 偶校验（even） 1停止位 8数据位
	//友峰共有24组继电器板
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
	
	auchMsg[0]=station;		//栈号
	auchMsg[1]=0x0F;		//功能码:0x0F表示写多个线圈
	
	auchMsg[2]=0x00;		//起始地址H
	auchMsg[3]=0x00;		//起始地址L
	
	auchMsg[4]=0x00;		//输出数量H
	auchMsg[5]=0x10;		//输出数量L
	
	auchMsg[6]=0x02;		//字节数：输出数量/8 余数不等于0则+1(字节数为1则下方为一个字节;为2则为两个字节)
	
	auchMsg[7]=tongxunzhen[duanshu][pianshu]&0xff;		//输出值1：外部输出一共16位所对应的低8位值
	auchMsg[8]=(tongxunzhen[duanshu][pianshu] >> 8);	//输出值2：外部输出一共16位所对应的高8位值
	
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
	
	/***************通讯协议485 Read********************/
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
			return 1;		//10个通讯帧中有3个匹配就返回1
		}
		else{
			return 0;
		}	
	}
	else
		{
			//rUBRDIV3 = ( (int)(SYS_PCLK/16./9600+0.5) -1 );	//函数返回之前将波特率换回去
			return 0;
		}
}

void Tiaoxian_Youfeng_ComInfo_Set(void){
	
	unsigned int i,j,k,m;
	for (i = 0; i < tiaoshaduan_max; i++){
		if(tiaoxianduan[i].jieshuquanshu != 0){
			for(j=0;j<PIANSHU_MAX;j++){
				Tiaoxian_Youfeng_Pianshu_Set(i,j);
				}
			}
		
		}	
	}
}

void Tiaoxian_Youfeng_Pianshu_Set(duanshu,pianshu){
	int i,j;
	
	switch(pianshu){
		case 0:
			for(i = 0;i<8;i++){
				if(g_InteralMemory.KeepWord[470+i+35*duanshu] == DANJI){
					tongxunzhen[duanshu][0] &=(~(0x03<<(i*2));
					tongxunzhen[duanshu][0] |=(0x01<<(i*2));
				}
				else if(g_InteralMemory.KeepWord[470+i+35*duanshu] == SHUANGJI){
					tongxunzhen[duanshu][0] &=(~(0x03<<(i*2));
					tongxunzhen[duanshu][0] |=(0x03<<(i*2));
				}
				else{
					tongxunzhen[duanshu][0] &=(~(0x03<<(i*2));
					tongxunzhen[duanshu][0] |=(0x00<<(i*2));
				}
			}
			break;
		case 1:
			//6路单击气缸
			for(i = 0;i<6;i++){
				if(g_InteralMemory.KeepWord[478+i+35*duanshu] == DANJI){
					tongxunzhen[duanshu][1] &=(~(0x01<<(i));
					tongxunzhen[duanshu][1] |=(0x01<<(i));
				}
				else{
					tongxunzhen[duanshu][1] &=(~(0x01<<(i));
					tongxunzhen[duanshu][1] |=(0x00<<(i));
				}
			}
			//5路双击气缸
			for(j = 0;j<5;j++){
				if(g_InteralMemory.KeepWord[484+j+35*duanshu] == DANJI){
					tongxunzhen[duanshu][1] &=(~(0x03<<(j*2+6));
					tongxunzhen[duanshu][1] |=(0x03<<(j*2+6));
				}
				else if(g_InteralMemory.KeepWord[484+j+35*duanshu] == SHUANGJI){
					tongxunzhen[duanshu][1] &=(~(0x03<<(j*2+6));
					tongxunzhen[duanshu][1] |=(0x03<<(j*2+6));
				}
				else{
					tongxunzhen[duanshu][1] &=(~(0x03<<(j*2+6));
					tongxunzhen[duanshu][1] |=(0x00<<(j*2+6));
				}
			}
			break;
		case 2:
			for(i = 0;i<8;i++){
				if(g_InteralMemory.KeepWord[489+i+35*duanshu] == DANJI){
					tongxunzhen[duanshu][2] &=(~(0x03<<(i*2));
					tongxunzhen[duanshu][2] |=(0x01<<(i*2));
				}
				else if(g_InteralMemory.KeepWord[489+i+35*duanshu] == SHUANGJI){
					tongxunzhen[duanshu][2] &=(~(0x03<<(i*2));
					tongxunzhen[duanshu][2] |=(0x03<<(i*2));
				}
				else{
					tongxunzhen[duanshu][2] &=(~(0x03<<(i*2));
					tongxunzhen[duanshu][2] |=(0x00<<(i*2));
				}
			}
			break;
		case 3:
			for(i = 0;i<8;i++){
				if(g_InteralMemory.KeepWord[497+i+35*duanshu] == DANJI){
					tongxunzhen[duanshu][3] &=(~(0x03<<(i*2));
					tongxunzhen[duanshu][3] |=(0x01<<(i*2));
				}
				else if(g_InteralMemory.KeepWord[497+i+35*duanshu] == SHUANGJI){
					tongxunzhen[duanshu][3] &=(~(0x03<<(i*2));
					tongxunzhen[duanshu][3] |=(0x03<<(i*2));
				}
				else{
					tongxunzhen[duanshu][3] &=(~(0x03<<(i*2));
					tongxunzhen[duanshu][3] |=(0x00<<(i*2));
				}
			}
			break;
		default:
			break;
	}
	
	
}


void TiaoXian_Youfeng_Reset(void){
	
}

void Tiaoxian_Youfeng_Yazhen_Get_Zero(unsigned int yazhen_num){
	
}

/***压针电机模块，所有的压针电机的计算问题应该放在这里，
	中断中做++/set;已知s;通过中断得出t，最后获得v(可以这样类比)***/	
void Tiaoxian_Youfeng_Yazhen(void){
	static prev_motor;//用于记录前值
	
		if(tiaoxianduan[duanshu_enable_cur].yazheng_motor_1st != 0){
			yazheng_motor_1st_start = 1;
		}
		else{
			Tiaoxian_Youfeng_yazhen_get_zero(yazhen_no_1);
			yazheng_motor_1st_start = 0;
		}
		
		if(tiaoxianduan[duanshu_enable_cur].yazheng_motor_2nd != 0)	{
			yazheng_motor_2nd_start = 1;
		}
		else{
			Tiaoxian_Youfeng_yazhen_get_zero(yazhen_no_2);
			yazheng_motor_1st_start = 0;
		}
}