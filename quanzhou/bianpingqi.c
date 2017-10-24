#include"includes.h"

#define	bianpingqi_cal_fac	2.5

unsigned int	bianpingqi_previous_speed=0;
U8 bianpingqi_previous_jog_speed=0;
U8 bianpingqi_jog_status=0;

U8 bianpingqi_run_flag;
U8 bianpingqi_previous_run_status=0;
unsigned int bianpingqi_speed;
U8 bpqComCount=0;
void bianpingqi_RTU_WriteWord(U8 function_num,int DevAddress,int Value)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 Count,waitTime; 
	unsigned int check=0;//cc,dd,
	int i;
	U32 ErrorLoop;
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./9600+0.5) -1 );
	if (rULCON3!=0x2B) 
		rULCON3 =0x2B; 								//0x2b=00 101 0 11  普通 偶校验（even） 1停止位 8数据位
	auchMsg[0]=bianpingqi_station_num;
	auchMsg[1]=function_num;		
	auchMsg[2]=(U8)(DevAddress>>8);
	auchMsg[3]=(U8)(DevAddress & 0x00ff);
	auchMsg[4]=(U8)(Value>>8);
	auchMsg[5]=(U8)(Value & 0x00ff);
	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;
	for(i=0;i<8;i++)
	{
		  SendArray[i]=auchMsg[i];
	}
	rGPHDAT |= 0x1000;	//GPH12	+Write
	Delay(DELAY_TIME_RTU);
	for(Count=0;Count<8;Count++)         
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
	while((RecArray[0]=qz_Uart_Getch())!=bianpingqi_station_num && Count<=20) 
	{
		Count++;
	}
	if(RecArray[0]==bianpingqi_station_num)
	{
		for (Count=1;Count<8;Count++)
		{
 			RecArray[Count]=qz_Uart_Getch();
			if (RecArray[Count]==SendArray[Count])
			{
				check++;
			}
		}
		if (check>=4){
			/* if (DevAddress==bianpingqi_fre_add)
			{ */
				if (bianpingqi_jog_status==1)
				{
					bianpingqi_previous_speed=300;
				}
				else
				{
					//bianpingqi_previous_speed=g_InteralMemory.Word[30];
					bianpingqi_previous_speed=bianpingqi_speed;
					bpqComCount=0;
				}
			//}
			/* else if (DevAddress==0x1001)
			{
				if (Value==((int)(1<<2)))
					bianpingqi_previous_run_status=1;
				else
				{
					bianpingqi_previous_run_status=0;
					bianpingqi_previous_speed=0;
				}
			} */
		}
		else
			bpqComCount++;
	}
	else
		bpqComCount++;
	if (bpqComCount>=4){
		bianpingqi_previous_speed=bianpingqi_speed;
		bpqComCount=0;
	}
}

void bianpingqi_start_sub(void)
{
	rGPBDAT &= ~(1<<1);
	rGPBDAT |= 1<<3;
	bianpingqi_previous_run_status=1;
}

void bianpingqi_stop_sub(void)
{
	rGPBDAT &= ~(1<<3);
	rGPBDAT |= 1<<1;
	bianpingqi_previous_speed=0;
	bianpingqi_previous_run_status=0;
}


void bianpingqi_set_speed(unsigned int speed)
{
	if (bianpingqi_previous_speed!=speed&&bianpingqi_jog_status!=1&&Choose_bianpingqi_kb==CHOOSE_BIANPINGQI)
	{
		if (bianpingqi_zhonglei_kb == shilin_bianpingqi)
			bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,bianpingqi_fre_add,speed);//正常用
		else
			bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x2000,speed);
	}
	else if (Choose_bianpingqi_kb==CHOOSE_NOT)
	{
		bianpingqi_previous_speed=bianpingqi_speed;
	}
}

void bianpingqi_start(U8 bianpingqi_run_button)
{		
	if (bianpingqi_run_button==1&&bianpingqi_previous_run_status==0&&bianpingqi_jog_status==0)
	{
		bianpingqi_start_sub();
		//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,(int)(1<<2));				//反转启动
		bianpingqi_jog_button=0;
	}
	if (bianpingqi_run_button==0&&bianpingqi_jog_status==0 && bianpingqi_previous_run_status==1)
	{
		bianpingqi_stop_sub();
		//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,(int)(1<<7));
		//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,(int)0);
	}
}

void bianpingqi_jog(void)
{
	if ((bianpingqi_run_flag==0 && ext_stop_status!=press) || qz_error_status==1)//正常情况下，按了急停键就不应该再进入
	{
		if (bianpingqi_jog_button==1 || ext_jog_status==press)
		{
			if (bianpingqi_previous_run_status!=1)
			{
				bianpingqi_jog_status=1;
				if (Choose_bianpingqi_kb==CHOOSE_BIANPINGQI){
					if (bianpingqi_zhonglei_kb == shilin_bianpingqi)
						bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,bianpingqi_fre_add,300);
					else
						bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x2000,300);
				}
					
				else
					bianpingqi_previous_speed=300;
 				bianpingqi_start_sub();
				//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,(int)(1<<2));
			}
			if(Pen_status==Pen_status_up && ext_jog_status!=press)
			{
				bianpingqi_jog_status=0;
				bianpingqi_stop_sub();
				//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,0x0000);
				bianpingqi_jog_button=0;
			}
		}
		if (ext_jog_status!=press && bianpingqi_jog_status==1 && bianpingqi_jog_button==0)
		{
			bianpingqi_jog_status=0;
			bianpingqi_stop_sub();
			//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,0x0000);
			
		}
	}
	else if (ext_stop_status==press && qz_error_status==0){
		bianpingqi_jog_status=0;
		bianpingqi_jog_button=0;
		bianpingqi_stop_sub();
	}
}