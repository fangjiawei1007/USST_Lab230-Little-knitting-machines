#include "includes.h"

unsigned int chudao_start_status[6][6] = {0};
unsigned int shoudao_start_status[6][6] = {0};
unsigned int chudao_start[6][6] = {0};
unsigned int shoudao_start[6][6] = {0};
unsigned int chudao_shoudao_status[6][6] = {0};
// int previous_stage[6] = {-1};
unsigned int kaiguan[6] = {0x00};	
unsigned int shinengwei[6]={0};	
unsigned int tongxunzhen = 0xff;
unsigned int tongxunstart = 0;
unsigned int chudao_jiange_tmp[6][6] = {0};		//出刀间隔记录 by FJW
unsigned int shoudao_jiange_tmp[6][6] = {0};	//收刀间隔记录 by FJW

unsigned int tiaoxianzu = 0;
unsigned int tiaoxianzu_flag = 0;
unsigned int tiaoxianzu_quanshu = 0;

TIAOXIANDUAN tiaoxianduan[tiaoshaduan_max];

void tiaoxian_init(void)	//调线初始化 by FJW
{
	int ii,bb;
	tongxunzhen = 0xff;
	for (ii = 0 ; ii < 5 ; ii++){
		if (tiaoxian_jidianqi_write() == 1){
			break;
		}
	}
	for (ii = 0; ii < tiaoshaduan_max; ii++){
		tiaoxianduan[ii].kaishiquanshu = &g_InteralMemory.KeepWord[156 + 9*ii];
		tiaoxianduan[ii].jieshuquanshu = &g_InteralMemory.KeepWord[157 + 9*ii];
		tiaoxianduan[ii].channal_choose= &g_InteralMemory.KeepWord[158 + 9*ii];
		tiaoxianduan[ii].ewaiduan_choose=&g_InteralMemory.KeepBit[49 + ii];
		for (bb = 0; bb<7; bb++){
			tiaoxianduan[ii].fangdabeishu[bb] = &g_InteralMemory.KeepWord[159 + 9*ii + bb];
		}
	}
}

int between_check(unsigned int roundShineng){
	int i;
	if (tiaoxiankaiguan_kb == 0)
		return -1;
	for(i=0;i<8;i++){
		if ( roundShineng >= *tiaoxianduan[i].kaishiquanshu								//大于下限
		&& *tiaoxianduan[i].jieshuquanshu											//对应上限不为零
		&& (roundShineng < *tiaoxianduan[i].jieshuquanshu							//小于上限
		|| 	*tiaoxianduan[i].jieshuquanshu == (daduanquanshu + middlequanshu + xiaoduanquanshu + caijiaoquanshu + langfeiquanshu))){
			
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

unsigned int at_check(unsigned int roundShineng){
	int i;
	for(i=0;i<8;i++){
		if (tiaoxianzu_quanshu != 0){
			if (roundShineng == (dapan_round + 1) && ((tiaoxianzu_quanshu+1) % tiaoxianzu_jiange ==0 )) {
				return 1;
			}
			if (roundShineng == (dapan_round ) && ((tiaoxianzu_quanshu) % tiaoxianzu_jiange ==0 )) {
				return 1;
			}
		}
		if (*tiaoxianduan[i].jieshuquanshu){											//对应上限不为零
			if (*tiaoxianduan[i].kaishiquanshu == 0
			    && roundShineng == (daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu)){
					if (getStage(current_stage,NEXTSTAGE) == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1){
							tiaoxianzu = 0;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
					else if (getStage(current_stage,NEXTSTAGE) != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1){
							tiaoxianzu = 0;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
			}//这个if为了保证在开始圈数为0时，能提早减速进行调线。
			else if (roundShineng == *tiaoxianduan[i].kaishiquanshu){						//等于下限
				if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1){
						tiaoxianzu = 0;	
						tiaoxianzu_flag = 1;
					}
					return 1;
				}
				else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1){
						tiaoxianzu = 0;	
						tiaoxianzu_flag = 1;
					}
					return 1;
				}
			}
			if (roundShineng == *tiaoxianduan[i].jieshuquanshu){						//等于上限

				if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1){
						tiaoxianzu = 0;	
						tiaoxianzu_flag = 1;
					}
					return 2;
				}
				else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1){
						tiaoxianzu = 0;	
						tiaoxianzu_flag = 1;
					}
					return 2;
				}
			}
		}
	}
	
	return 0;
}

unsigned int tiqushuzi(INT16U channal_choose){
	int i;
	unsigned int wei[5];
	unsigned int weizhi = 0x00;
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
	else{
		weizhi = 0xff;
	}
	return weizhi;
}

void shinengpanduan(void){
	int i;
	unsigned int weizhi = 0x00;
	for (i = 0 ; i <6 ; i++){
		shinengwei[i] = 0;
	}
	i = between_check(dapan_round);
	if ( i != -1 && current_stage != caijianduan){
		weizhi = tiqushuzi(*tiaoxianduan[i].channal_choose);
		for (i = 0 ; i <6 ; i++){
			if ( (weizhi>>i) & 0x01){
				shinengwei[i] = 1;
			}
		}
	}
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_quanshu % tiaoxianzu_jiange ==0){
		tiaoxianzu ++;
		if (tiaoxianzu == 5){
			tiaoxianzu_flag = 0;
		}
	}
}

void tiaoxian(void)
{
	static unsigned int chudao_shoudao_start[6][6] = {0};
	static unsigned int tongxunnum = 0;
	int i;
	unsigned int zushu;
	shinengpanduan();
	
	for (zushu =0; zushu <= tiaoxianzu; zushu++){
	
	for (i = 0; i<6 ; i++){
		if (shinengwei[i] == 1 && chudao_shoudao_start[zushu][i] == 0){
			weisha(i,zushu);
			tongxunzhen &= (~(3<< (i*2)));
			tongxunzhen |= (kaiguan[i] << (i*2));
			chudao_shoudao_status[zushu][i] = 1;
			// previous_stage[i] = current_stage;
			tongxunstart = 1;
			chudao_shoudao_start[zushu][i] = 1;
		}
		else if (shinengwei[i] == 0 && chudao_shoudao_status[zushu][i] == 1){
			chudao_shoudao_process(i,zushu);
			tongxunzhen &= (~(3<< (i*2)));
			tongxunzhen |= (kaiguan[i] << (i*2));
			chudao_shoudao_start[zushu][i] = 0;
			//previous_stage[i] = current_stage; //此处应该放在出刀收刀结束
		}
	}
	if (tongxunstart == 1 && tongxunnum <5){
		if (tiaoxian_jidianqi_write() != 1){
			tongxunnum ++ ;
			
		}
		else{
			tongxunnum = 0;
			tongxunstart = 0;
		}
	}
	else if (tongxunstart == 1){
		tongxunnum = 0;
		tongxunstart = 0;
	}
	}
}

unsigned int tiaoxian_jidianqi_write(void)
{
	U8 Count,jdqCheck,waitTime;
	int i;
	U32 ErrorLoop;
#define	tiaoxiannormal	1
#if tiaoxiannormal
	U8 auchMsg[10],SendArray[10],RecArray[10];  
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	if (rULCON3!=0x2B) 
		rULCON3 =0x2B; 								//0x2b=00 101 0 11  普通 偶校验（even） 1停止位 8数据位
	
	auchMsg[0]=25;
	
	auchMsg[1]=0x0F;
	
	auchMsg[2]=0x00;
	auchMsg[3]=0x00;
	
	auchMsg[4]=0x00;
	auchMsg[5]=0x08;
	
	auchMsg[6]=0x01;
	
	auchMsg[7]=tongxunzhen;
	
	auchMsg[8]=(CRC(auchMsg,8)) & 0xff;
	auchMsg[9]=(CRC(auchMsg,8))>>8;
	for (i=0;i<10;i++)
	{
		SendArray[i]=auchMsg[i];
	}
#else
	U8 auchMsg[11],SendArray[11],RecArray[11];  
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	if (rULCON3!=0x2B) 
		rULCON3 =0x2B; 								//0x2b=00 101 0 11  普通 偶校验（even） 1停止位 8数据位
	
	auchMsg[0]=25;
	
	auchMsg[1]=0x0F;
	
	auchMsg[2]=0x00;
	auchMsg[3]=0x00;
	
	auchMsg[4]=0x00;
	auchMsg[5]=0x10;
	
	auchMsg[6]=0x02;
	
	auchMsg[7]=tongxunzhen;
	auchMsg[8]=tongxunzhen;
	
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
			return 1;
		}
		else{
			return 0;
		}	
	}
	else
		return 0;
}

void chudao_shoudao_process(unsigned int i,unsigned int zushu)
{
		if(chudao_start[zushu][i] == 0 && shoudao_start[zushu][i] == 0)
		{
			kaiguan[i] = 0x00;		//00 00 00 00
			tongxunstart = 1;
			/* Set_Y_Value(Y9,LOW);
			Set_Y_Value(Y10,LOW); */
		}
		
		if(chudao_start_status[zushu][i] == 0)
			chudao_start[zushu][i] = 1;
		
		if(chudao_jiange_tmp[zushu][i] >= chudao_jiange)
		{
			kaiguan[i] = 0x01;		//01
			tongxunstart = 1;
			/* Set_Y_Value(Y9,HIGH);
			Set_Y_Value(Y10,LOW); */
			
			chudao_start_status[zushu][i] = 1;
			chudao_start[zushu][i] = 0;
			chudao_jiange_tmp[zushu][i] = 0;
			
			shoudao_start[zushu][i] = 1;
		}
	
		if(shoudao_jiange_tmp[zushu][i] >= shoudao_jiange)
		{
			kaiguan[i] = 0x03;		//11
			tongxunstart = 1;
			/* Set_Y_Value(Y9,HIGH);
			Set_Y_Value(Y10,HIGH); */
			
			shoudao_start_status[zushu][i] = 1;
			shoudao_start[zushu][i] = 0;
			shoudao_jiange_tmp[zushu][i] = 0;
			
			//previous_stage[i] = current_stage;
			chudao_shoudao_status[zushu][i] = 0;
		}
}

void weisha(unsigned int i,unsigned int zushu)
{
	kaiguan[i] = 0x02;		//10
	/* Set_Y_Value(Y9,LOW);
	Set_Y_Value(Y10,HIGH); */
		
	chudao_start[zushu][i] = 0;
	chudao_start_status[zushu][i] = 0;
	shoudao_start[zushu][i] = 0;
	shoudao_start_status[zushu][i]= 0;
	chudao_jiange_tmp[zushu][i] = 0;
	shoudao_jiange_tmp[zushu][i] = 0;
}