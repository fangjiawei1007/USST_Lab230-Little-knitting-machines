#include "includes.h"

#define SHANGPAN_SHUANGJI_NUM		8
#define SHANGPAN_DANJI_NUM			6
#define XIAPAN_SHUANGJI_NUM			21

/**压针倍率**/
#define Yazhen_Factor				g_InteralMemory.KeepWord[780]


unsigned int tongxunstart[ZUSHU_MAX][PIANSHU_MAX] = {0};
unsigned int tongxunnum[ZUSHU_MAX][PIANSHU_MAX] = {0};
unsigned int tongxunzhen[tiaoshaduan_max][PIANSHU_MAX] = {0x0000};
unsigned char* qigang_confirm_kb[16];			//16因为一共8段*2=16
//unsigned int jiajiaStatus = 0;

unsigned int qigang_confirm_status = 0;				//用于气缸确认之后，tongxunzhen的set
unsigned int enter_already = 0;	//用于checkout防止重复进入,CHANGED之后Set完毕之后再置零
int changed_duan = -1;							//用于duan改变之后的通讯帧的设置
int changed_qigang_button = -1;
unsigned int have_not_com = 0;					//用于判断通讯帧改变之后是否进行了通讯：0->不需要通讯/已经通讯过了;1->改变了但是未通讯
unsigned int com_again = 0;						//用于判断tiaoxianzu改变之后是否进行了通讯：0->不需要通讯/已经通讯过了;1->改变了但是未通讯

TIAOXIANDUAN tiaoxianduan[tiaoshaduan_max];

/*************************************************
Function(函数名称): TiaoXian_Youfeng_App(void);
Description(函数功能、性能等的描述): 调线功能代码
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
*************************************************/
void TiaoXian_Youfeng_App(void){
	// unsigned int zushu=0;
	static int duanshu_enable_prev=10;
	static int duanshu_enable_cur=11;

	at_check(dapan_round);
	duanshu_enable_cur = between_check(dapan_round);
	
	if((tiaoxian_shiji_kb == 1)){
		int checkout_tmp = 0;
		int checkout_yazhen = 0;
		
		checkout_tmp = TiaoXian_Youfeng_Checkout();
		if(checkout_tmp == CHANGED){
			//Beep(1);
			Tiaoxian_Youfeng_ComInfo_Set();
		}
		
		checkout_yazhen = Yazhen_Youfeng_Checkout(duanshu_enable_cur);
		if(checkout_yazhen == CHANGED){
			Tiaoxian_Youfeng_Yazhen_Change(duanshu_enable_cur);
		}	
	}
	
	//1.正常情况duanshu_enable_tmp != duanshu_enable保证不多次通讯;
	//2.试机的时候需要多次通讯have_not_com用于试机
	//3.com_again用于阶段调线
	if((duanshu_enable_prev != duanshu_enable_cur && current_stage != caijianduan) \
	  || (have_not_com == 1) || (com_again == 1)){//qigang_confirm_status == 1|| (tiaoxianzu < tiaoxianzu_max)
		//容错
		if(duanshu_enable_cur>8)
			return;
		
		Tiaoxian_Youfeng_Yazhen(duanshu_enable_prev,duanshu_enable_cur);//(qigang_confirm_status == 1)不会对此有影响，因为里面判断了压针数必须改变，相同段不会改变
		duanshu_enable_prev = duanshu_enable_cur;//防止重复进入(通讯)
		
		TiaoXian_Youfeng_weisha(duanshu_enable_cur);
		
		have_not_com = 0;
		com_again = 0;
	}
}


/*************************************************
Function(函数名称): TiaoXian_Youfeng_Init(void);
Description(函数功能、性能等的描述): 调线功能初始化代码(每次上电都会调用一次)
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
*************************************************/
void TiaoXian_Youfeng_Init(void){
	int i,j,k,m,n,l;
	unsigned int shangpan_danji_check_tmp[8]={0};
	unsigned int shangpan_shuangji_check_tmp[8]={0};
	unsigned int xiapan_shaungji_check_tmp[8]={0};

	//写入8路通讯帧，外部输出对应的状态(此处需要初始化是因为他的继电器板子有问题，继电器板子没问题应该不需要做这些)
	/* if (tiaoxiankaiguan_kb ==1 ){
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
 */
	/***8段结构体参数初始化***/
	for (i = 0; i < tiaoshaduan_max; i++){
		tiaoxianduan[i].kaishiquanshu = &g_InteralMemory.KeepWord[156 + 10*i];
		tiaoxianduan[i].jieshuquanshu = &g_InteralMemory.KeepWord[157 + 10*i];
		tiaoxianduan[i].ewaiduan_choose=&g_InteralMemory.KeepBit[49 + i];
		tiaoxianduan[i].yazheng_motor_1st=&g_InteralMemory.KeepWord[158 + 10*i];
		tiaoxianduan[i].yazheng_motor_2nd=&g_InteralMemory.KeepWord[159 + 10*i];
		wdt_feed_dog();main_enter_flag = 1;
		/**上下盘气缸数以及气缸作用**/
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
	/***气缸button初始化***/
	for(n=0;n<16;n++){
		qigang_confirm_kb[n]=&g_InteralMemory.KeepBit[65 + n];
	}
	/***通讯帧初始化***/
	for(l = 0;l<tiaoshaduan_max;l++){
		if(tiaoxianduan[l].jieshuquanshu != 0){
			for(j=0;j<PIANSHU_MAX;j++){
				Tiaoxian_Youfeng_Pianshu_Set(l,j);
			}
		}
	}	
}

/*************************************************
Function(函数名称): TiaoXian_Youfeng_Init_once(void);
Description(函数功能、性能等的描述): 调线功能初始化代码(只有第一次上电会被调用)
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
*************************************************/
void TiaoXian_Youfeng_Init_once(void){
	
	int i,j,k,m,n;
	/*************8段调线设置*****************/
		for (i = 0; i < tiaoshaduan_max; i++){
			g_InteralMemory.KeepWord[156 + 10*i] = 0;			//开始圈
			g_InteralMemory.KeepWord[157 + 10*i] = 0;			//结束圈
			g_InteralMemory.KeepWord[158 + 10*i] = 0;			//压针电机1
			g_InteralMemory.KeepWord[159 + 10*i] = 0;			//压针电机2
			g_InteralMemory.KeepBit[49 + i] = 0;				//额外段选择
			wdt_feed_dog();main_enter_flag = 1;
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
			for(m = 0;m<XIAPAN_SHUANGJI_NUM; m++){
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
		Yazhen_Factor = 5;
		jiajiaStatus = 0;
		jiajiansujiangemaichong_kw = 10000;
		
		//weisha_jiange_kw = 0;	//喂纱间隔,友峰去除
}

/*************************************************
Function(函数名称): TiaoXian_Youfeng_Checkout(void);
Description(函数功能、性能等的描述): 是否需要重新设置通讯帧的检查
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):当外部试机按键被按下的时候，以及气缸确定按钮被按下之后，会进入checkout()
Return: CHANGED/NOT_CHANGED
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
*************************************************/
int TiaoXian_Youfeng_Checkout(void){
	unsigned int i,j,k,m,n;
	INT16U up_danji_checkout_tmp[8] = {0};
	INT16U up_shuangji_checkout_tmp[8] = {0};
	INT16U down_shuangji_checkout_tmp[8] = {0};
	INT16U up_checkout_tmp[8]={0};
	INT16U down_checkout_tmp[8]={0};
	
	int qigang_confirm_num = -1;	//用于记录哪个按键被按下了,-1表示无按键被按下
	
	for(n = 0;n<16;n++){
		if(*(qigang_confirm_kb[n]) == 1 && enter_already == 0){//
			qigang_confirm_num = n;
			/****用于外部的全局变量*****/
			qigang_confirm_status = 1;
			enter_already = 1;
			changed_qigang_button = n;
			changed_duan = (qigang_confirm_num/2);
			break;
		}
	}
	
	if(qigang_confirm_status == 1 && qigang_confirm_num >= 0){	
			wdt_feed_dog();main_enter_flag = 1;
			/**上盘被改变**/
			if((qigang_confirm_num%2) == 0){
				i = (qigang_confirm_num/2);//获取段
				for (j = 0;j<SHANGPAN_SHUANGJI_NUM; j++){
					up_shuangji_checkout_tmp[i]+=*(tiaoxianduan[i].shangpan_shaungji_qigang[j]);
				}
				for (k = 0;k<SHANGPAN_DANJI_NUM; k++){
					up_danji_checkout_tmp[i]+=*(tiaoxianduan[i].shangpan_danji_qigang[k]);
				}
				up_checkout_tmp[i] = (up_shuangji_checkout_tmp[i]+up_danji_checkout_tmp[i]);
				/**test for checkout**/
				
				if(up_checkout_tmp[i] != *(tiaoxianduan[i].shangpan_checkout)){
					*(tiaoxianduan[i].shangpan_checkout) = up_checkout_tmp[i];
					return CHANGED;
				}
				else{
					*(qigang_confirm_kb[qigang_confirm_num]) = 0;	//button置零;
					enter_already = 0;
					qigang_confirm_status = 0;
					return NOT_CHANGED;
				}
			}
			/**下盘被改变**/
			else{
				i = (qigang_confirm_num/2);
				for(m = 0;m<XIAPAN_SHUANGJI_NUM; m++){
					//tiaoxianduan[i].xiapan_shuangji_qigang[m]=&g_InteralMemory.KeepWord[484+m+35*i];
					down_shuangji_checkout_tmp[i]+=*(tiaoxianduan[i].xiapan_shuangji_qigang[m]);
				}
				down_checkout_tmp[i] = (down_shuangji_checkout_tmp[i]);
				
				if((down_checkout_tmp[i] != *(tiaoxianduan[i].xiapan_checkout))){
					*(tiaoxianduan[i].xiapan_checkout)= down_checkout_tmp[i];
					// *(qigang_confirm_kb[qigang_confirm_num]) = 0;	//button置零;
					return CHANGED;
				}
				else{
					*(qigang_confirm_kb[qigang_confirm_num]) = 0;	//button置零;
					enter_already = 0;
					qigang_confirm_status = 0;
					return NOT_CHANGED;
				}
			}
	}
	qigang_confirm_status=0;
	return NOT_CHANGED;
}

/*************************************************
Function(函数名称): TiaoXian_Youfeng_weisha(int duanshu);
Description(函数功能、性能等的描述): 外部继电器板开始通讯，一共需要通讯4片*zushu次
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): duanshu选择哪一段的通讯帧(一共8段:0-7)
Output(对输出参数的说明):
Return: CHANGED/NOT_CHANGED
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
*************************************************/
/**包括了-1的情况,即收刀情况**/
void TiaoXian_Youfeng_weisha(int duanshu){
	
	int zushu = 0;
	int pianshu = 0;
	
	/**容错用**/
	if(duanshu>8)
		return;
	{
		g_InteralMemory.Word[300] = tiaoxianzu;	
		g_InteralMemory.Word[301] = tiaoxianzu_max;
	}
	/**tongxunstart[][]以及tongxunnum[][]必须在Tiaoxian_Youfeng_ComInfo_Get()中打开**/
	for (zushu =0; zushu < tiaoxianzu; zushu++){
		for(pianshu = 0;pianshu<PIANSHU_MAX;pianshu++){	
			tongxunnum[zushu][pianshu] = 0;
			/***通讯开始***/
			if (tongxunnum[zushu][pianshu] <5){//tongxunstart[zushu][pianshu] == 1 &&
				/***5次通讯容错***/
				if(duanshu >= 0){
					if (Tiaoxian_Youfeng_jidianqi_write(zushu,pianshu,duanshu) != 1){
						tongxunnum[zushu][pianshu]++ ;
					}
					else{
						tongxunnum[zushu][pianshu] = 0;
					}
				}
				else{//收刀
					if (Tiaoxian_Youfeng_jidianqi_zero(zushu,pianshu) != 1){
						tongxunnum[zushu][pianshu]++ ;
					}
					else{
						tongxunnum[zushu][pianshu] = 0;
					}
				}
			}
			/**5次通讯错误之后**/
			else{
				tongxunnum[zushu][pianshu] = 0;
			}
		
			/* else if(tongxunstart[zushu][pianshu] == 1){
				tongxunnum[zushu][pianshu] = 0;
				tongxunstart[zushu][pianshu] = 0;
			}
			else if(tongxunnum[zushu][pianshu] != 0)
				tongxunnum[zushu][pianshu] = 0;	 */
		}
	}

}

/*************************************************
Function(函数名称): Tiaoxian_Youfeng_jidianqi_write(unsigned int zushu,unsigned int pianshu,unsigned int duanshu);
Description(函数功能、性能等的描述): 写继电器通讯帧,通讯写通讯帧
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): duanshu选择哪一段的通讯帧(一共8段:0-7)
Output(对输出参数的说明):
Return: CHANGED/NOT_CHANGED
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
*************************************************/
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
	
	station = zushu*10+(pianshu+5);
	
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

/*************************************************
Function(函数名称): Tiaoxian_Youfeng_jidianqi_zero(unsigned int zushu,unsigned int pianshu);
Description(函数功能、性能等的描述): 写继电器通讯帧,通讯写通讯帧0
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): duanshu选择哪一段的通讯帧(一共8段:0-7)
Output(对输出参数的说明):
Return: CHANGED/NOT_CHANGED
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
*************************************************/
unsigned int Tiaoxian_Youfeng_jidianqi_zero(unsigned int zushu,unsigned int pianshu){
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
	
	station = zushu*10+(pianshu+5);
	
	auchMsg[0]=station;		//栈号
	auchMsg[1]=0x0F;		//功能码:0x0F表示写多个线圈
	
	auchMsg[2]=0x00;		//起始地址H
	auchMsg[3]=0x00;		//起始地址L
	
	auchMsg[4]=0x00;		//输出数量H
	auchMsg[5]=0x10;		//输出数量L
	
	auchMsg[6]=0x02;		//字节数：输出数量/8 余数不等于0则+1(字节数为1则下方为一个字节;为2则为两个字节)
	
	auchMsg[7]=0x0000&0xff;		//输出值1：外部输出一共16位所对应的低8位值
	auchMsg[8]=(0x0000 >> 8);	//输出值2：外部输出一共16位所对应的高8位值
	
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

/*************************************************
Function(函数名称): Tiaoxian_Youfeng_ComInfo_Set(unsigned int duanshu);//不能循环SET太消耗时间
Description(函数功能、性能等的描述): 必须获取到哪一个确定按钮被按下;
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): duanshu选择哪一段的通讯帧(一共8段:0-7)
Output(对输出参数的说明):
Return: CHANGED/NOT_CHANGED
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
*************************************************/
void Tiaoxian_Youfeng_ComInfo_Set(void){
	unsigned int j;
	
	if(changed_duan >= 0 && changed_duan<tiaoshaduan_max){
		if(tiaoxianduan[changed_duan].jieshuquanshu != 0){
			for(j=0;j<PIANSHU_MAX;j++){
				Tiaoxian_Youfeng_Pianshu_Set(changed_duan,j);
			}
			if(changed_qigang_button >= 0){
				*(qigang_confirm_kb[changed_qigang_button]) = 0;	//button置零;set完之后在把按键弹起来
				enter_already = 0;									//这个标志位值得考虑
				changed_qigang_button = -1;							//开关用
			}
		}
		qigang_confirm_status = 0;
		have_not_com = 1;
		changed_duan = -1;//当做开关用
	}
}

/*************************************************
Function(函数名称): Tiaoxian_Youfeng_Pianshu_Set(int duanshu,unsigned int pianshu);//不能循环SET太消耗时间
Description(函数功能、性能等的描述): 必须获取到哪一个确定按钮被按下;
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): duanshu选择哪一段的通讯帧(一共8段:0-7)
Output(对输出参数的说明):
Return: CHANGED/NOT_CHANGED
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
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
				else if(g_InteralMemory.KeepWord[470+i+35*duanshu] == DAN_SHUANGJI){
					tongxunzhen[duanshu][0] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][0] |=(0x02<<(i*2));
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
			//6路单击气缸
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
			//5路双击气缸
			wdt_feed_dog();main_enter_flag = 1;
			for(j = 0;j<5;j++){
				if(g_InteralMemory.KeepWord[484+j+35*duanshu] == DANJI){
					tongxunzhen[duanshu][1] &=(~(0x03<<(j*2+6)));
					tongxunzhen[duanshu][1] |=(0x01<<(j*2+6));
				}
				else if(g_InteralMemory.KeepWord[484+j+35*duanshu] == DAN_SHUANGJI){
					tongxunzhen[duanshu][1] &=(~(0x03<<(j*2+6)));
					tongxunzhen[duanshu][1] |=(0x02<<(j*2+6));
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
				else if(g_InteralMemory.KeepWord[489+i+35*duanshu] == DAN_SHUANGJI){
					tongxunzhen[duanshu][2] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][2] |=(0x02<<(i*2));
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
				else if(g_InteralMemory.KeepWord[497+i+35*duanshu] == DAN_SHUANGJI){
					tongxunzhen[duanshu][3] &=(~(0x03<<(i*2)));
					tongxunzhen[duanshu][3] |=(0x02<<(i*2));
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
Function(函数名称): TiaoXian_Youfeng_Reset(void);//不能循环SET太消耗时间
Description(函数功能、性能等的描述): 复位标志位复位(恢复全局变量初始化的量);
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: CHANGED/NOT_CHANGED
Others: 
Author:方佳伟
Modified:
Commented:方佳伟
*************************************************/
void TiaoXian_Youfeng_Reset(void){
	yazheng_motor_1st_start	= 0;
	yazheng_motor_2nd_start	= 0;
	
	yazhen_pulse_cmp_1st = 0;	
	yazhen_pulse_cmp_2nd = 0;   
	yazhen_motor_pulse_1st = 0;
	yazhen_motor_pulse_2nd = 0;
	yazhen_1st_counter = 0;
	yazhen_2nd_counter = 0;
	
	qigang_confirm_status = 0;
	
	enter_already = 0;
	changed_duan = -1;			
	changed_qigang_button = -1;
	
	have_not_com = 0;	
	com_again = 0;		
	tiaoxianzu = tiaoxianzu_max;//1
	tiaoxianzu_flag = 0;
	tiaoxianzu_quanshu=0;
}



void Tiaoxian_Youfeng_Yazhen_Get_Zero(unsigned int yazhen_num){
	
	(void)yazhen_num;
	//Beep(1);
}

/***压针电机模块，所有的压针电机的计算问题应该放在这里，
	encoder1_process中做++/set;已知s;通过中断得出t，最后获得v(可以这样类比)***/	
void Tiaoxian_Youfeng_Yazhen(int duanshu_prev,int duanshu_cur){
	
	INT16U yazhen_cur_1st = 0;
	INT16U yazhen_prev_1st = 0;
	INT16U yazhen_cur_2nd = 0;
	INT16U yazhen_prev_2nd = 0;
	INT16U kaishi_quanshu = 0;
	INT16U jieshu_quanshu = 0;
	unsigned int yazhen_total_pulse_1st = 0;
	unsigned int yazhen_total_pulse_2nd = 0;
	unsigned int yazhen_1st_2nd_together = 0;//边界条件，用于第一段与第二段连续的判断条件,因为prev == 0 有可能前一段为第0段
	//第一次边界条件，因为duanshu_prev == 10;
	if(duanshu_prev >= 8 || duanshu_prev == -1)
		duanshu_prev = 0;

	//当前段被关闭则回零
	if(duanshu_cur == -1){
		Tiaoxian_Youfeng_Yazhen_Get_Zero(1);//yazhen_no_1
		Tiaoxian_Youfeng_Yazhen_Get_Zero(2);//yazhen_no_2
		
		yazheng_motor_1st_start = 0;
		yazheng_motor_2nd_start = 0;
		return;
	}
	//容错
	if(duanshu_cur == -1 || duanshu_prev == -1)
		return;
	//边界条件
	if((duanshu_cur == 1) && (*(tiaoxianduan[0].jieshuquanshu) == *(tiaoxianduan[1].kaishiquanshu)))
		yazhen_1st_2nd_together = 1;
	
	yazhen_cur_1st = *(tiaoxianduan[duanshu_cur].yazheng_motor_1st);
	yazhen_prev_1st = *(tiaoxianduan[duanshu_prev].yazheng_motor_1st);
	yazhen_cur_2nd = *(tiaoxianduan[duanshu_cur].yazheng_motor_2nd);
	yazhen_prev_2nd = *(tiaoxianduan[duanshu_prev].yazheng_motor_2nd);

	kaishi_quanshu = *(tiaoxianduan[duanshu_cur].kaishiquanshu);
	jieshu_quanshu = *(tiaoxianduan[duanshu_cur].jieshuquanshu);
	
	//第一段or结束之后的一段
	if(duanshu_prev == 0 && yazhen_1st_2nd_together == 0){//duanshu_cur == 0 || (duanshu_prev == 0 && duanshu_cur != 1)//
		/**1.获取第一段压针 1 所需要的值
		   2.当前一段关闭的时候，后一段打开，并且当前不是第二段(duanshu_cur!=1)**/
		if(yazhen_cur_1st != 0){
			if((jieshu_quanshu > kaishi_quanshu)){
				yazhen_motor_pulse_1st = (yazhen_cur_1st - 0)*Yazhen_Factor;
				yazhen_total_pulse_1st = (jieshu_quanshu - kaishi_quanshu)*encoder1_cal_factor;
				if(yazhen_motor_pulse_1st == 0)
					yazhen_pulse_cmp_1st = 0;
				else
					yazhen_pulse_cmp_1st = yazhen_total_pulse_1st/yazhen_motor_pulse_1st;
				
				if(yazhen_pulse_cmp_1st < 2){
					yazhen_pulse_cmp_1st = 2;//保证下降沿
				}
				yazhen_1st_counter = 0;
				yazheng_motor_1st_start = 1;
			}
		}
		else{
			Tiaoxian_Youfeng_Yazhen_Get_Zero(1);//yazhen_no_1
			yazheng_motor_1st_start = 0;
		}
		/**获取第一段压针 2 所需要的值**/
		if(yazhen_cur_2nd != 0){
			if((jieshu_quanshu > kaishi_quanshu)){
				yazhen_motor_pulse_2nd = (yazhen_cur_2nd - 0)*Yazhen_Factor;
				yazhen_total_pulse_2nd = (jieshu_quanshu - kaishi_quanshu)*encoder1_cal_factor;
				if(yazhen_motor_pulse_2nd == 0)
					yazhen_pulse_cmp_2nd = 0;
				else
					yazhen_pulse_cmp_2nd = yazhen_total_pulse_2nd/yazhen_motor_pulse_2nd;
				
				if(yazhen_pulse_cmp_2nd < 2){
					yazhen_pulse_cmp_2nd = 2;//保证下降沿
				}
				yazhen_2nd_counter=0;
				yazheng_motor_2nd_start = 1;
			}
		}
		else{
			Tiaoxian_Youfeng_Yazhen_Get_Zero(2);//yazhen_no_2
			yazheng_motor_2nd_start = 0;
		}
		
	}
	else{
		// Beep(1);
		/**获取第i>1段压针 1 所需要的值**/
		if(yazhen_cur_1st != 0){
			if((yazhen_cur_1st > yazhen_prev_1st) && (jieshu_quanshu > kaishi_quanshu)){
				yazhen_motor_pulse_1st = (yazhen_cur_1st - yazhen_prev_1st)*Yazhen_Factor;
				yazhen_total_pulse_1st = (jieshu_quanshu - kaishi_quanshu)*encoder1_cal_factor;
				if(yazhen_motor_pulse_1st == 0)
					yazhen_pulse_cmp_1st = 0;
				else
					yazhen_pulse_cmp_1st = yazhen_total_pulse_1st/yazhen_motor_pulse_1st;
				
				if(yazhen_pulse_cmp_1st < 2){
					yazhen_pulse_cmp_1st = 2;//保证下降沿
				}
				yazhen_1st_counter = 0;
				yazheng_motor_1st_start = 1;
			}
			//剩下的情况均不合理，直接跳过	，注意yazhen_cur_1st == yazhen_prev_1st的情况就是不做任何改变;
		}
		else{
			Tiaoxian_Youfeng_Yazhen_Get_Zero(1);//yazhen_no_1
			yazheng_motor_1st_start = 0;
		}
		/**获取第i>1段压针 1 所需要的值**/
		if(yazhen_cur_2nd != 0){
			if((yazhen_cur_2nd > yazhen_prev_2nd) && (jieshu_quanshu > kaishi_quanshu)){
				yazhen_motor_pulse_2nd = (yazhen_cur_2nd - yazhen_prev_2nd)*Yazhen_Factor;
				yazhen_total_pulse_2nd = (jieshu_quanshu - kaishi_quanshu)*encoder1_cal_factor;
				if(yazhen_motor_pulse_2nd == 0)
					yazhen_pulse_cmp_2nd = 0;
				else
					yazhen_pulse_cmp_2nd = yazhen_total_pulse_2nd/yazhen_motor_pulse_2nd;
				
				if(yazhen_pulse_cmp_2nd < 2){
					yazhen_pulse_cmp_2nd = 2;//保证下降沿
				}
				yazhen_2nd_counter=0;
				yazheng_motor_2nd_start = 1;
			}
			//剩下的情况均不合理，直接跳过	，注意yazhen_cur_1st == yazhen_prev_1st的情况就是不做任何改变;
		}
		else{
			Tiaoxian_Youfeng_Yazhen_Get_Zero(2);//yazhen_no_2
			yazheng_motor_2nd_start = 0;
			}
		}
	
}

int Yazhen_Youfeng_Checkout(int duanshu){
	static unsigned int first_in = 0;		//第一次进入
	static unsigned int yazhen_1st_pre = 0;
	static unsigned int yazhen_2nd_pre = 0;
	static unsigned int yazhen_1st_cur = 0;
	static unsigned int yazhen_2nd_cur = 0;
	
	if(duanshu == -1)
		return NOT_CHANGED;
	if(first_in == 0){
		yazhen_1st_pre = *(tiaoxianduan[duanshu].yazheng_motor_1st);
		yazhen_2nd_pre = *(tiaoxianduan[duanshu].yazheng_motor_2nd);
		yazhen_1st_cur = *(tiaoxianduan[duanshu].yazheng_motor_1st);
		yazhen_2nd_cur = *(tiaoxianduan[duanshu].yazheng_motor_2nd);
		first_in = 1;
		return NOT_CHANGED;
	}
	else{
		yazhen_1st_cur = *(tiaoxianduan[duanshu].yazheng_motor_1st);
		yazhen_2nd_cur = *(tiaoxianduan[duanshu].yazheng_motor_2nd);
		
		if((yazhen_1st_cur == yazhen_1st_pre) && (yazhen_2nd_cur == yazhen_2nd_pre))
			return NOT_CHANGED;
		else{
			yazhen_1st_pre = yazhen_1st_cur;
			yazhen_2nd_pre = yazhen_2nd_cur;
			return CHANGED;
		}
	}
	return NOT_CHANGED;
}

void Tiaoxian_Youfeng_Yazhen_Change(int duanshu){
	int yazhen_cur_1st,yazhen_cur_2nd,jieshu_quanshu;
	int yazhen_total_pulse_1st,yazhen_total_pulse_2nd;
	
	if(duanshu == -1){
		return;
	}
	yazhen_cur_1st = *(tiaoxianduan[duanshu].yazheng_motor_1st);
	yazhen_cur_2nd = *(tiaoxianduan[duanshu].yazheng_motor_2nd);
	jieshu_quanshu = *(tiaoxianduan[duanshu].jieshuquanshu);
	
	if((yazhen_cur_1st > yazhen_1st_counter) && (jieshu_quanshu > dapan_round)){
		yazhen_motor_pulse_1st = (yazhen_cur_1st - yazhen_1st_counter)*Yazhen_Factor;
		yazhen_total_pulse_1st = (jieshu_quanshu - dapan_round)*encoder1_cal_factor;
		if(yazhen_motor_pulse_1st == 0)
			yazhen_pulse_cmp_1st = 0;
		else
			yazhen_pulse_cmp_1st = yazhen_total_pulse_1st/yazhen_motor_pulse_1st;
		
		if(yazhen_pulse_cmp_1st < 2){
			yazhen_pulse_cmp_1st = 2;//保证下降沿
		}
	}
	
	if((yazhen_cur_2nd > yazhen_2nd_counter) && (jieshu_quanshu > dapan_round)){
		yazhen_motor_pulse_2nd = (yazhen_cur_2nd - yazhen_2nd_counter)*Yazhen_Factor;
		yazhen_total_pulse_2nd = (jieshu_quanshu - dapan_round)*encoder1_cal_factor;
		if(yazhen_motor_pulse_2nd == 0)
			yazhen_pulse_cmp_2nd = 0;
		else
			yazhen_pulse_cmp_2nd = yazhen_total_pulse_2nd/yazhen_motor_pulse_2nd;
		
		if(yazhen_pulse_cmp_2nd < 2){
			yazhen_pulse_cmp_2nd = 2;//保证下降沿
		}
	}
}
/*************************************************
Function(函数名称): between_check(unsigned int roundShineng)
Description(函数功能、性能等的描述): 主要用于shinengpanduan()函数，判断当前阶段是否是在调线圈数之内
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): songsha_fre_change(void);		shinengpanduan(void);

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): roundShineng——大盘使能(传入dapan_round)
Output(对输出参数的说明):
Return: i:当前在调线的第i段
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
int between_check(unsigned int roundShineng){
	int i;
	
	//check函数主要用于调线功能,若未打开调线功能，则直接返回-1；
	if (tiaoxiankaiguan_kb == 0)
		return -1;
	
	for(i=0;i<tiaoshaduan_max;i++){
		
		/***进入条件：1.大于开始圈数
					2.结束圈数为0表示关闭
					3.小于结束圈数或者等于最后一圈
		***/
		if (roundShineng >= *tiaoxianduan[i].kaishiquanshu			
		&& *tiaoxianduan[i].jieshuquanshu							
		&& (roundShineng < *tiaoxianduan[i].jieshuquanshu			
		|| 	*tiaoxianduan[i].jieshuquanshu == 
			(daduanquanshu + middlequanshu + xiaoduanquanshu + caijiaoquanshu + langfeiquanshu))){
			
			
			if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
				return i;/***判断是否是额外段，即外部开关是否打开***/
			}
			else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
				return i;/***不是额外段，返回所在段***/
			}
		}
	}
	return -1;
}


/*************************************************
Function(函数名称): unsigned int at_check(unsigned int roundShineng)
Description(函数功能、性能等的描述): 1.主要用于变频器的变速，主要被bianpingqi_speed_cal()所调用
								2.主要修改了全局变量tiaoxianzu/tiaoxianzu_flag
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): bianpingqi_speed_cal(void)

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): roundShineng：dapan_round值
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
unsigned int at_check(unsigned int roundShineng){
	int i;
	
	/***进入条件(位与)：1.调线组圈数！=0，在encoder1_process()中++
					  2.调线组间隔圈数！=0
					  3.调线组圈数%调线组间隔圈数:间隔圈数的倍数
					  4.jiajiaStatus == 0
		此处代码主要为了判断当前是否调线组需要++以判断第二组 刀具是否需要出刀收刀，
		此处只是条件的增加，而不会return;
	***/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 &&
		tiaoxianzu_quanshu % tiaoxianzu_jiange ==0 && jiajiaStatus == 0)
	{
		jiajiaStatus = 1;
		if ( tiaoxianzu < tiaoxianzu_max ){
			tiaoxianzu++;
			com_again = 1;		//for 友峰
		}
	}
	
	/***调线组间隔圈数为0，则不分组，共同出刀收刀***/
	else if (tiaoxianzu_jiange == 0){
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
		tiaoxianzu = tiaoxianzu_max;
	}
	
	/*****调线工艺完成，flag和status复位*****/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 && 
		tiaoxianzu >= tiaoxianzu_max && 
		tiaoxianzu_quanshu >= ((tiaoxianzu_max -1)*tiaoxianzu_jiange + 1))
	{
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
	}
	
	
		
	/****	进入条件:1.调线组圈数不等于0 || 调线组间隔等于1
					2.调线组flag等于1并且调线组间隔不等于0
		
			此处代码对应判断是否到了第n组调线的时候，提前一圈降速(通过return 1;将函数返回):
			if ((at_check((dapan_round+1)) && encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw)))
			
			判断是否当前圈是否需要在其后面减速:
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
	
	
	/****调线八段速，此处仅仅为第1组的开始和结束，其余刀组数的判断均为上述代码进行判断****/
	for(i=0;i<tiaoshaduan_max;i++){
		/*****只要开启调线，首先进入以下函数******/
		if (*tiaoxianduan[i].jieshuquanshu)//对应上限不为零
		{											
			/****为了保证在开始圈数为0时，能提早减速进行调线(工艺要求调线之前需要减速，0圈开始调线，必须从最后一圈开始)
			****/
			if (*tiaoxianduan[i].kaishiquanshu == 0
			    && roundShineng == (daduanquanshu+middlequanshu+xiaoduanquanshu+
									caijiaoquanshu+langfeiquanshu))
			{
					/**************************下一个阶段为额外段，并且外部开关选择额外段************************************/
					if (getStage(current_stage,NEXTSTAGE) == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
					/**************************下一个阶段不为额外段，并且外部开关不选择额外段************************************/
					else if (getStage(current_stage,NEXTSTAGE) != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
			}//这个if为了保证在开始圈数为0时，能提早减速进行调线。
			
			/**********************************调线开始********************************************/
			else if (roundShineng == *tiaoxianduan[i].kaishiquanshu){						//等于下限
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
			
			/*********************************调线结束*******************************************/
			if (roundShineng == *tiaoxianduan[i].jieshuquanshu){						//等于上限

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
