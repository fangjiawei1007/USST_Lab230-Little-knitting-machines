#include "includes.h"

unsigned int chudao_start_status[6][6] = {0};
unsigned int shoudao_start_status[6][6] = {0};
unsigned int chudao_start[6][6] = {0};
unsigned int shoudao_start[6][6] = {0};
unsigned int chudao_shoudao_status[6][6] = {0};
unsigned int kaiguan[6][6] = {0x00};	
unsigned int shinengwei[6]={0};	
unsigned int tongxunzhen[6] = {0xffff};
unsigned int tongxunstart[6] = {0};
unsigned int chudao_jiange_tmp[6][6] = {0};		//出刀间隔记录 by FJW
unsigned int shoudao_jiange_tmp[6][6] = {0};	//收刀间隔记录 by FJW
unsigned int chudao_shoudao_start[6][6] = {0};
//unsigned int tiaoxianzu = 0;
unsigned int jiajiaStatus = 0;

TIAOXIANDUAN tiaoxianduan[tiaoshaduan_max];

/*************************************************
Function(函数名称): tiaoxian_init(void)
Description(函数功能、性能等的描述): 调线功能初始化
Calls (被本函数调用的函数清单): tiaoxian_jidianqi_write(unsigned int zushu);
Called By (调用本函数的函数清单): App_Init();

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void tiaoxian_init(void)	//调线初始化 by FJW
{
	int ii,bb;
	/***写入8路通讯帧，外部输出对应的状态***/
	for (ii = 0 ;ii<tiaoshaduan_max;ii++){
		tongxunzhen[ii] = 0xffff;//初始化继电器吸合
	}
	
	for (bb = 0 ; bb < tiaoxianzu_max; bb++){
		for (ii = 0 ; ii < 5 ; ii++){
			/**通讯成功之后会直接退出循环**/
			if (tiaoxian_jidianqi_write(bb) == 1){
				break;
			}
		}
	}
	
	/***8段参数初始化***/
	for (ii = 0; ii < tiaoshaduan_max; ii++){
		tiaoxianduan[ii].kaishiquanshu = &g_InteralMemory.KeepWord[156 + 10*ii];
		tiaoxianduan[ii].jieshuquanshu = &g_InteralMemory.KeepWord[157 + 10*ii];
		tiaoxianduan[ii].channal_choose= &g_InteralMemory.KeepWord[158 + 10*ii];
		tiaoxianduan[ii].ewaiduan_choose=&g_InteralMemory.KeepBit[49 + ii];
		for (bb = 0; bb<7; bb++){
			tiaoxianduan[ii].fangdabeishu[bb] = &g_InteralMemory.KeepWord[159 + 10*ii + bb];
		}
	}
}

/*************************************************
Function(函数名称): tiaoxian_reset(void)
Description(函数功能、性能等的描述): 调线功能复位
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void tiaoxian_reset(void){
	int ii,bb;
	for (ii = 0 ;ii<tiaoshaduan_max;ii++){
		tongxunzhen[ii] = 0xffff;
	}
	
	for (bb = 0 ; bb < tiaoxianzu_max; bb++){
		for (ii = 0 ; ii < 5 ; ii++){
			/**通讯成功之后会直接退出循环**/
			if (tiaoxian_jidianqi_write(bb) == 1){
				break;
			}
		}
	}
	
	/*****************6把刀具判断哪一段刀具运动********************/
	for (bb =0; bb<6;bb++){
		
		/*****使能位、通讯帧、通讯开始信号*****/
		shinengwei[bb]=0;	
		tongxunzhen[bb] = 0xffff;
		tongxunstart[bb] = 0;
		
		/*******调线出刀收刀部分复位*******/
		for (ii=0;ii<6;ii++){
			chudao_start_status[bb][ii] = 0;
			shoudao_start_status[bb][ii] = 0;
			chudao_start[bb][ii] = 0;
			shoudao_start[bb][ii] = 0;
			chudao_shoudao_status[bb][ii] = 0;
			kaiguan[bb][ii] = 0x00;	
			chudao_shoudao_start[bb][ii] = 0;
			chudao_jiange_tmp[bb][ii] = 0;		
			shoudao_jiange_tmp[bb][ii] = 0;	
		}
	}
	
	/****************调线间隔圈数******************/
	if (tiaoxianzu_jiange != 0){
		tiaoxianzu = 1;
	}
	else
		tiaoxianzu = tiaoxianzu_max;
	
	
	tiaoxianzu_flag = 0;
	tiaoxianzu_quanshu=0;
}

/*************************************************
Function(函数名称): between_check(unsigned int roundShineng)
Description(函数功能、性能等的描述): 主要用于shinengpanduan()函数
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): songsha_fre_change(void);		shinengpanduan(void);

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): roundShineng——大盘使能(传入dapan_round)
Output(对输出参数的说明):
Return: i:调线第i段
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
	
	for(i=0;i<8;i++){
		
		/***进入条件：1.大于开始圈数
					2.结束圈数为0表示关闭
					3.小于结束圈数或者等于最后一圈
		***/
		if (roundShineng >= *tiaoxianduan[i].kaishiquanshu			
		&& *tiaoxianduan[i].jieshuquanshu							
		&& (roundShineng < *tiaoxianduan[i].jieshuquanshu			
		|| 	*tiaoxianduan[i].jieshuquanshu == 
			(daduanquanshu + middlequanshu + xiaoduanquanshu + caijiaoquanshu + langfeiquanshu))){
			
			/***判断是否是额外段，即外部开关是否打开***/
			if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
				return i;
			}
			else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
				return i;
			}
		}
	}
	return -1;
}


/*************************************************
Function(函数名称): unsigned int at_check(unsigned int roundShineng)
Description(函数功能、性能等的描述): 主要用于变频器的变速，主要被bianpingqi_speed_cal()所调用
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
	***/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 &&
		tiaoxianzu_quanshu % tiaoxianzu_jiange ==0 && jiajiaStatus == 0)
	{
		jiajiaStatus = 1;
		if ( tiaoxianzu < tiaoxianzu_max ){
			tiaoxianzu++;		
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
	
	/****调线八段速****/
	for(i=0;i<8;i++){
		
		/*****************???????????????????????????????????????????????????????????????????********************/
		if ((tiaoxianzu_quanshu != 0 || tiaoxianzu_jiange == 1) && 
			 tiaoxianzu_flag == 1 && tiaoxianzu_jiange != 0)
		{
			if (roundShineng == (dapan_round + 1) && ((tiaoxianzu_quanshu+1) % tiaoxianzu_jiange ==0 )) {
				return 1;
			}
			if (roundShineng == (dapan_round ) && ((tiaoxianzu_quanshu) % tiaoxianzu_jiange ==0 )) {
				return 1;
			}
		}
		
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
					/**************************下一个阶段为额外段，并且外部开关不选择额外段************************************/
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

/*************************************************
Function(函数名称): tiqushuzi(INT16U channal_choose)
Description(函数功能、性能等的描述): 提取外部调线选择的，若外部填入7则weizhi = 0xff
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): channal_choose：外部传入的输入若为1234 
																	  则表示选择1234四组刀具
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
unsigned int tiqushuzi(INT16U channal_choose){
	int i;
	unsigned int wei[5];
	unsigned int weizhi = 0x00;	//有效位的基值
	
	/****外部调线选择的阶段数字的提取****/
	if (channal_choose != 7){
		wei[0] = channal_choose / 10000;
		wei[1] = (channal_choose - wei[0]*10000) / 1000; 
		wei[2] = (channal_choose - wei[0]*10000 - wei[1]*1000) / 100;
		wei[3] = (channal_choose - wei[0]*10000 - wei[1]*1000- wei[2]*100) /10;
		wei[4] = channal_choose % 10;
		for (i = 0 ; i <5 ; i++){
			switch (wei[i]){
				case 1:
					weizhi |= 1<<0;
					break;
				case 2:
					weizhi |= 1<<1;
					break;
				case 3:
					weizhi |= 1<<2;
					break;
				case 4:
					weizhi |= 1<<3;
					break;
				case 5:
					weizhi |= 1<<4;
					break;
				case 6:
					weizhi |= 1<<5;
					break;
				default:
					break;
			}
		}
	}
	/****外部调线选择的阶段数字的提取当外部选择7的时候weizhi = 0xff****/
	else{
		weizhi = 0xff;
	}
	return weizhi;
}

/*************************************************
Function(函数名称): shinengpanduan(void)
Description(函数功能、性能等的描述): 6把刀的使能判断
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void shinengpanduan(void){
	int i;
	unsigned int weizhi = 0x00;
	for (i = 0 ; i <6 ; i++){
		shinengwei[i] = 0;
	}
	i = between_check(dapan_round);
	/**提取**/
	if ( i != -1 && current_stage != caijianduan){
		weizhi = tiqushuzi(*tiaoxianduan[i].channal_choose);
		for (i = 0 ; i <6 ; i++){
			if ( (weizhi>>i) & 0x01){
				shinengwei[i] = 1;
			}
		}
	}
}

/*************************************************
Function(函数名称): tiaoxian(void)
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
void tiaoxian(void)
{
	static unsigned int tongxunnum[6] = {0};
	int i;
	unsigned int zushu;
	shinengpanduan();	//判断第i组刀具的使能
	
	/**************tiaoxianzu？*************************************************/
	for (zushu =0; zushu < tiaoxianzu; zushu++){
	
		for (i = 0; i<6 ; i++){
			
			/***********使能位==1之后，设置通讯帧，设置通讯开始标志位，出刀收刀开始标志*********/
			if (shinengwei[i] == 1 && chudao_shoudao_start[zushu][i] == 0){
				weisha(i,zushu);
				tongxunzhen[zushu] &= (~(3<< (i*2)));				//清零
				tongxunzhen[zushu] |= (kaiguan[zushu][i] << (i*2));	//设置
				chudao_shoudao_status[zushu][i] = 1;
				// previous_stage[i] = current_stage;
				tongxunstart[zushu] = 1;
				chudao_shoudao_start[zushu][i] = 1;
			}
			
			/***********使能位==0之后(即该段不需要调线，那么就要把刀收回来)，
					   设置出刀收刀，设置通讯开始标志位，出刀收刀开始标志
			*********/
			else if (shinengwei[i] == 0 && chudao_shoudao_status[zushu][i] == 1){
				chudao_shoudao_process(i,zushu);					//出刀收刀
				tongxunzhen[zushu] &= (~(3<< (i*2)));				//清零
				tongxunzhen[zushu] |= (kaiguan[zushu][i] << (i*2)); //设置
				chudao_shoudao_start[zushu][i] = 0;                
				//previous_stage[i] = current_stage; //此处应该放在出刀收刀结束
			}
		}
		
		/***通讯开始***/
		if (tongxunstart[zushu] == 1 && tongxunnum[zushu] <5){
			/***5次通讯容错***/
			if (tiaoxian_jidianqi_write(zushu) != 1){
				tongxunnum[zushu] ++ ;
				
			}
			else{
				tongxunnum[zushu] = 0;
				tongxunstart[zushu] = 0;
			}
		}
		/**5次通讯错误之后**/
		else if (tongxunstart[zushu] == 1){
			tongxunnum[zushu] = 0;
			tongxunstart[zushu] = 0;
		}
		else if (tongxunnum[zushu] != 0)
			tongxunnum[zushu] = 0;
	}
}

/*************************************************
Function(函数名称): tiaoxian_jidianqi_write(unsigned int zushu)
Description(函数功能、性能等的描述): 通讯写入调线继电器的值
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): zushu：外部传参数组的tongxunzhen[zushu]功能代码
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
unsigned int tiaoxian_jidianqi_write(unsigned int zushu)
{
	U8 Count,jdqCheck,waitTime;
	int i;
	U32 ErrorLoop;
#define	tiaoxiannormal	0
#if tiaoxiannormal
	U8 auchMsg[10],SendArray[10],RecArray[10];  
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	/* if (rULCON3!=0x2B) 
		rULCON3 =0x2B; 		 */			//0x2b=00 101 0 11  普通 偶校验（even） 1停止位 8数据位
	
	switch (zushu){
	case 0:
		auchMsg[0]=25;
		break;
	}
	
	
	auchMsg[1]=0x0F;	//功能码:0x0F表示写多个线圈
	                    
	auchMsg[2]=0x00;    //起始地址H
	auchMsg[3]=0x00;    //起始地址L
	                    
	auchMsg[4]=0x00;    //输出数量H
	auchMsg[5]=0x08;    //输出数量L
	                    
	auchMsg[6]=0x01;    //字节数：输出数量/8 余数不等于0则+1(字节数为1则下方为一个字节;为2则为两个字节)
	                   
	auchMsg[7]=tongxunzhen[zushu] & 0xff;	//输出量：因为字节数为1，所以此处只需要低8位
	
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
		rULCON3 =0x03; 				//0x03=00 000 0 11  普通 偶校验（even） 1停止位 8数据位
	
	switch (zushu){
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
	}
	
	auchMsg[1]=0x0F;		//功能码:0x0F表示写多个线圈
	
	auchMsg[2]=0x00;		//起始地址H
	auchMsg[3]=0x00;		//起始地址L
	
	auchMsg[4]=0x00;		//输出数量H
	auchMsg[5]=0x10;		//输出数量L
	
	auchMsg[6]=0x02;		//字节数：输出数量/8 余数不等于0则+1(字节数为1则下方为一个字节;为2则为两个字节)
	
	auchMsg[7]=tongxunzhen[zushu]&0xff;		//输出值1：外部输出一共16位所对应的低8位值
	auchMsg[8]=(tongxunzhen[zushu] >> 8);	//输出值2：外部输出一共16位所对应的高8位值
	
	auchMsg[9]=(CRC(auchMsg,9)) & 0xff;
	auchMsg[10]=(CRC(auchMsg,9))>>8;
	for (i=0;i<11;i++)
	{
		SendArray[i]=auchMsg[i];
	}
#endif


	rGPHDAT |= 0x1000;	//GPH12	+Write
	Delay(DELAY_TIME_RTU);
#if tiaoxiannormal
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
		return 0;
}


/*************************************************
Function(函数名称): chudao_shoudao_process(unsigned int i,unsigned int zushu)
Description(函数功能、性能等的描述): 出刀收刀进程：其具体的工艺要求导致控制方案的要求：有说明书
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系):  i->6把刀具;	zushu->8段?
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void chudao_shoudao_process(unsigned int i,unsigned int zushu)
{
		/**********第一步：kaiguan[zushu][i] = 0x00，打开通讯开关*********/
		if(chudao_start[zushu][i] == 0 && shoudao_start[zushu][i] == 0)
		{
			kaiguan[zushu][i] = 0x00;		//00 00 00 00
			tongxunstart[zushu] = 1;
			/* Set_Y_Value(Y9,LOW);
			Set_Y_Value(Y10,LOW); */
		}
		
		/****************?????????????????**************************/
		if(chudao_start_status[zushu][i] == 0)
			chudao_start[zushu][i] = 1;
		
		/**********第二步：kaiguan[zushu][i] = 0x01，打开通讯开关*********/
		if(chudao_jiange_tmp[zushu][i] >= chudao_jiange)
		{
			kaiguan[zushu][i] = 0x01;		//01
			tongxunstart[zushu] = 1;
			/* Set_Y_Value(Y9,HIGH);
			Set_Y_Value(Y10,LOW); */
			
			chudao_start_status[zushu][i] = 1;
			chudao_start[zushu][i] = 0;
			chudao_jiange_tmp[zushu][i] = 0;
			
			shoudao_start[zushu][i] = 1;
		}
	
		/**********第三步：kaiguan[zushu][i] = 0x03，打开通讯开关*********/
		if(shoudao_jiange_tmp[zushu][i] >= shoudao_jiange)
		{
			kaiguan[zushu][i] = 0x03;		//11
			tongxunstart[zushu] = 1;
			/* Set_Y_Value(Y9,HIGH);
			Set_Y_Value(Y10,HIGH); */
			
			shoudao_start_status[zushu][i] = 1;
			shoudao_start[zushu][i] = 0;
			shoudao_jiange_tmp[zushu][i] = 0;
			
			//previous_stage[i] = current_stage;
			chudao_shoudao_status[zushu][i] = 0;
		}
}



/*************************************************
Function(函数名称): weisha(unsigned int i,unsigned int zushu)
Description(函数功能、性能等的描述): 喂纱；主要是设置kaiguan[zushu][i] = 0x02;即二进制(0b)10
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): i->6把刀具;	zushu->8段?
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void weisha(unsigned int i,unsigned int zushu)
{
	kaiguan[zushu][i] = 0x02;		//0b 10
	/* Set_Y_Value(Y9,LOW);
	Set_Y_Value(Y10,HIGH); */
		
	chudao_start[zushu][i] = 0;
	chudao_start_status[zushu][i] = 0;
	shoudao_start[zushu][i] = 0;
	shoudao_start_status[zushu][i]= 0;
	chudao_jiange_tmp[zushu][i] = 0;
	shoudao_jiange_tmp[zushu][i] = 0;
}