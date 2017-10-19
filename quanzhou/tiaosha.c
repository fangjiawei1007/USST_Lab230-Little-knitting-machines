#include "includes.h"

unsigned int chudao_start_status[4] = {0};
unsigned int shoudao_start_status[4] = {0};
unsigned int chudao_start[4] = {0};
unsigned int shoudao_start[4] = {0};
unsigned int chudao_shoudao_status[4] = {0};
int previous_stage[4] = {-1};
unsigned int kaiguan[4] = {0x00};	
unsigned int shinengwei[4]={0};	
unsigned int tongxunzhen = 0xff;
unsigned int tongxunstart = 0; 

void shinengpanduan(void){
	int i;
	//unsigned int wei[4];
	for (i = 0 ; i <4 ; i++){
		shinengwei[i] = 0;
	}
	if ( tiaoxiankaiguan_kb != 0){
		/* wei[0] = channal_choose / 1000;
		wei[1] = (channal_choose - wei[0]*1000) / 100; 
		wei[2] = (channal_choose - wei[0]*1000 - wei[1]*100) / 10;
		wei[3] = channal_choose % 10;
		for (i = 0 ; i <4 ; i++){
			switch (wei[i]){
				case 1:
					//weizhi |= (0x03 << 0);
					shinengwei[0] = 1;
					break;
				case 2:
					//weizhi |= (0x03 << 2);
					shinengwei[1] = 1;
					break;
				case 3:
					//weizhi |= (0x03 << 4);
					shinengwei[2] = 1;
					break;
				case 4:
					//weizhi |= (0x03 << 6);
					shinengwei[3] = 1;
					break;
				default:
					break;
				}
		} */
		if ( dapan_round >= tiaoxian_12_enter && dapan_round < tiaoxian_12_exit && current_stage != 4 && current_stage != 5){
			shinengwei[0] = 1;
			shinengwei[1] = 1;
		}
		else{
			shinengwei[0] = 0;
			shinengwei[1] = 0;
		}
		if ( dapan_round >= tiaoxian_34_enter && dapan_round < tiaoxian_34_exit && current_stage != 4 && current_stage != 5){
			shinengwei[2] = 1;
			shinengwei[3] = 1;
		}
		else{
			shinengwei[2] = 0;
			shinengwei[3] = 0;
		}
	}
}

void tiaoxian(void)
{
	//static int previous_stage[i] = -1;
	//static int close_init = 0;
	static unsigned int chudao_shoudao_start[4] = {0};
	static unsigned int tongxunnum = 0;
	int i;
	shinengpanduan();//g_InteralMemory.KeepWord[154+current_stage]
	for (i = 0; i<4 ; i++){
		if (shinengwei[i] == 1 && chudao_shoudao_start[i] == 0){//g_InteralMemory.KeepBit[48+current_stage]&& current_stage < 4
			weisha(i);
			tongxunzhen &= (~(3<< (i*2)));
			tongxunzhen |= (kaiguan[i] << (i*2));
			chudao_shoudao_status[i] = 1;
			previous_stage[i] = current_stage;
			tongxunstart = 1;
			chudao_shoudao_start[i] = 1;
		}
		else if (shinengwei[i] == 0 && chudao_shoudao_status[i] == 1 && current_stage < 4){//g_InteralMemory.KeepBit[48+current_stage]
			chudao_shoudao_process(i);
			tongxunzhen &= (~(3<< (i*2)));
			tongxunzhen |= (kaiguan[i] << (i*2));
			chudao_shoudao_start[i] = 0;
			//previous_stage[i] = current_stage; //此处应该放在出刀收刀结束
		}
		else if (current_stage >= 4 && chudao_shoudao_status[i] == 1){
			chudao_shoudao_process(i);
			tongxunzhen &= (~(3<< (i*2)));
			tongxunzhen |= (kaiguan[i] << (i*2));
			chudao_shoudao_start[i] = 0;
		}
	}
	if (tongxunstart == 1 && tongxunnum <5){
		if (tiaoxian_jidianqi_write() != 1){
			tongxunnum ++ ;
			g_InteralMemory.Word[29] = tongxunnum;
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

void chudao_shoudao_process(unsigned int i)
{
		if(chudao_start[i] == 0 && shoudao_start[i] == 0)
		{
			kaiguan[i] = 0x00;		//00 00 00 00
			tongxunstart = 1;
			/* Set_Y_Value(Y9,LOW);
			Set_Y_Value(Y10,LOW); */
		}
		
		if(chudao_start_status[i] == 0)
			chudao_start[i] = 1;
		
		if(chudao_jiange_tmp[i] >= chudao_jiange)
		{
			kaiguan[i] = 0x01;		//01
			tongxunstart = 1;
			/* Set_Y_Value(Y9,HIGH);
			Set_Y_Value(Y10,LOW); */
			
			chudao_start_status[i] = 1;
			chudao_start[i] = 0;
			chudao_jiange_tmp[i] = 0;
			
			shoudao_start[i] = 1;
		}
	
		if(shoudao_jiange_tmp[i] >= shoudao_jiange)
		{
			kaiguan[i] = 0x03;		//11
			tongxunstart = 1;
			/* Set_Y_Value(Y9,HIGH);
			Set_Y_Value(Y10,HIGH); */
			
			shoudao_start_status[i] = 1;
			shoudao_start[i] = 0;
			shoudao_jiange_tmp[i] = 0;
			
			previous_stage[i] = current_stage;
			chudao_shoudao_status[i] = 0;
		}
}

void weisha(unsigned int i)
{
	kaiguan[i] = 0x02;		//10
	/* Set_Y_Value(Y9,LOW);
	Set_Y_Value(Y10,HIGH); */
		
	chudao_start[i] = 0;
	chudao_start_status[i] = 0;
	shoudao_start[i] = 0;
	shoudao_start_status[i]= 0;
	chudao_jiange_tmp[i] = 0;
	shoudao_jiange_tmp[i] = 0;
}