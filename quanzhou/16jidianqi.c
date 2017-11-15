#include"includes.h"

#define jidianqi_station_num	22

U8 jdqComCount[8]={0};
char qz_Uart_Getch(void)
{
	//INT8U temp=0;
	INT8U temp=0xff;
	INT16U Flag_Comm = 0;	
	int i;
	U32 ErrorLoop;

	wdt_feed_dog();main_enter_flag = 1;
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;

	while(!(rUTRSTAT3 & 0x1) && Flag_Comm<ErrorLoop) //Receive data read
	{
		for(i=0;i<50;i++);	// 2000
		Flag_Comm++;
	}
	if(Flag_Comm >= ErrorLoop)
	{
		temp=0xff;
	}
	else
	{
		/*temp=rUERSTAT3;
		if (temp!=0)
			return 0xff;
		else */
		temp=RdURXH3(); 
		g_Timer2Uart.Timer_Cur = 0;
	}
	return temp;
	
}

void jidianqi_write_single(U8 which_port,U8 button_bit)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 Count,jdqCheck,waitTime;
	int i;
	U32 ErrorLoop;
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./57600+0.5) -1 );
	//Uart_Init(0,9600);
	if (rULCON3!=0x03) 
		rULCON3 =0x03; 								//0x2b=00 101 0 11  普通 偶校验（even） 1停止位 8数据位
	auchMsg[0]=jidianqi_station_num;
	auchMsg[1]=jidianqi_write_single_fun_num;		//3492~3491  2016.1.3 quanzhou
	auchMsg[2]=0x00;
	auchMsg[3]=which_port;
	if (g_InteralMemory.Bit[button_bit]==1)
	{
		auchMsg[4]=0xff;
		if (which_port!=dianci)
			g_InteralMemory.Bit[button_bit+9]=1;
	}
	else
	{
		auchMsg[4]=0x00;
		if (which_port!=dianci)
			g_InteralMemory.Bit[button_bit+9]=0;
	}
	auchMsg[5]=0x00;
	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;
	for (i=0;i<8;i++)
	{
		SendArray[i]=auchMsg[i];
	}
	rGPHDAT |= 0x1000;	//GPH12	+Write
	Delay(DELAY_TIME_RTU);
	for (Count=0;Count<8;Count++)
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
	while ((RecArray[0]=qz_Uart_Getch())!=jidianqi_station_num && Count<=20)
	{
		Count++;
	}
	if (RecArray[0]==jidianqi_station_num){
		for (Count=1;Count<8;Count++)
		{
			RecArray[Count]=qz_Uart_Getch();
			if (RecArray[Count]==auchMsg[Count]){
				jdqCheck++;
			}
		}
		if (jdqCheck>=4){
			if (g_InteralMemory.Bit[button_bit]==1)
				g_InteralMemory.Bit[button_bit+9]=1;
			else
				g_InteralMemory.Bit[button_bit+9]=0;
			jdqComCount[which_port]=0;
		}
		else
			jdqComCount[which_port]++;
	}
	else
		jdqComCount[which_port]++;
	if (jdqComCount[which_port]>=5){
		if (g_InteralMemory.Bit[button_bit]==1)
			g_InteralMemory.Bit[button_bit+9]=1;
		else
			g_InteralMemory.Bit[button_bit+9]=0;
		jdqComCount[which_port]=0;
	}
	jdqComCount[which_port]=0;
}

void jidianqi_write_fenshan(U8 which_port)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 Count,jdqCheck,waitTime; 
	int i;
	U32 ErrorLoop;
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	
	auchMsg[0]=jidianqi_station_num;
	auchMsg[1]=jidianqi_write_single_fun_num;		//3492~3491  2016.1.3 quanzhou 
	auchMsg[2]=0x00;
	auchMsg[3]=which_port;
	if (fenshan_button)
	{
		auchMsg[4]=0xff;
		fenshan_status=1;
	}
	else
	{
		auchMsg[4]=0x00;
		fenshan_status=0;
	}
	auchMsg[5]=0x00;
	auchMsg[6]=(CRC(auchMsg,6)) & 0xff;
	auchMsg[7]=(CRC(auchMsg,6))>>8;
	
	for(i=0;i<8;i++)
	{
		  SendArray[i]=auchMsg[i];
	}
	
	//rINTMSK |=BIT_GLOBAL;
	rGPHDAT |= 0x1000;	//GPH12	+Write
	for(i=0; i<DELAY_TIME_RTU; i++);
	for(Count=0;Count<8;Count++)         
	{
		while((!(rUTRSTAT3 & 0x4)) && (waitTime<=ErrorLoop)){
			for (i=0;i<50;i++){
				waitTime++;
			}
		}
		waitTime=0;
		WrUTXH3(SendArray[Count]);
		while((!(rUTRSTAT3 & 0x4)) && (waitTime<=ErrorLoop)){
			for (i=0;i<50;i++){
				waitTime++;
			}
		}
	}
	rGPHDAT &= 0xefff;	//GPH12	-Read
	Count = 0;
	while((RecArray[0]=qz_Uart_Getch())!=jidianqi_station_num && Count<=20) 
	{
		Count++;
	}
	if (RecArray[0]==jidianqi_station_num){
		for (Count=1;Count<8;Count++)
		{
			RecArray[Count]=qz_Uart_Getch();
			if (RecArray[Count]==auchMsg[Count]){
				jdqCheck++;
			}
		}
		if (jdqCheck>=4){
			if (fenshan_button)
				fenshan_status=1;
			else
				fenshan_status=0;
			jdqComCount[which_port]=0;
		}
		else
			jdqComCount[which_port]++;
	}
	else
		jdqComCount[which_port]++;
	if (jdqComCount[which_port]>=5){
		if (fenshan_button)
			fenshan_status=1;
		else
			fenshan_status=0;
		jdqComCount[which_port]=0;
	}
	jdqComCount[which_port]=0;
}

void hongdeng_fun(void)
{
	if (hongdeng_button!=hongdeng_status && jdqComCount[dianci]==0)
		jidianqi_write_single(hongdeng,hongdeng_button_bit);
}

void huangdeng_fun(void)
{
	if (huangdeng_button!=huangdeng_status && jdqComCount[dianci]==0)
		jidianqi_write_single(huangdeng,huangdeng_button_bit);
}

void lvdeng_fun(void)
{
	if(lvdeng_button!=lvdeng_status && jdqComCount[dianci]==0)
		jidianqi_write_single(lvdeng,lvdeng_button_bit);
}

void zhaobudeng_fun(void)
{
	if(zhaobudeng_button!=zhaobudeng_status && jdqComCount[dianci]==0)
		jidianqi_write_single(zhaobudeng,zhaobudeng_button_bit);
}

void zhaomingdeng_fun(void)
{
	if (zhaomingdeng_button!=zhaomingdeng_status && jdqComCount[dianci]==0)
		jidianqi_write_single(zhaomingdeng,zhaomingdeng_button_bit);
}

void dianci_fun(void)
{
	if(dianci_button!=dianci_status){
		Set_Y_Value(11,dianci_button);
		dianci_status=dianci_button;
		//jidianqi_write_single(dianci,dianci_button_bit);
	}
		
}

void youbeng_fun(void)
{
	if(youbeng_button!=youbeng_status && jdqComCount[dianci]==0)
		jidianqi_write_single(youbeng,youbeng_button_bit);
}

void penyou24v_fun(void)
{
	if(penyou24v_button!=penyou24v_status && jdqComCount[dianci]==0)
		jidianqi_write_single(penyou24v,penyou24v_button_bit);
}

void fenshan_fun(void)
{
	if(fenshan_button!=fenshan_status && jdqComCount[dianci]==0)
		jidianqi_write_fenshan(fenshan);
}

void jidianqi_fun(void)
{
	hongdeng_fun();
	huangdeng_fun();
	lvdeng_fun();
	zhaobudeng_fun();
	zhaomingdeng_fun();
	dianci_fun();
	//youbeng_fun();
	penyou24v_fun();
	//fenshan_fun();
}


