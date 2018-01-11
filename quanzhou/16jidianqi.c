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

/*************************************************
Function(函数名称): jidianqi_write_single(U8 which_port,U8 button_bit)
Description(函数功能、性能等的描述): 继电器板的通讯协议
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
		rULCON3 =0x03; 								//0x03=00 000 0 011  普通 无校验 1停止位 8数据位
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
	/**20次读取uart的值，读到值直接退出循环**/
	while ((RecArray[0]=qz_Uart_Getch())!=jidianqi_station_num && Count<=20)
	{
		Count++;
	}
	
	/***栈号正确之后，jdqcheck判断读到的帧是否有4个是正确的，
		如果是正确的那么直接将status置1，即外部不继续通讯
		eg：if (hongdeng_button!=hongdeng_status)
	***/
	
	if (RecArray[0]==jidianqi_station_num){
		for (Count=1;Count<8;Count++)
		{
			RecArray[Count]=qz_Uart_Getch();
			if (RecArray[Count]==auchMsg[Count]){
				jdqCheck++;
			}
		}
		if (jdqCheck>=4){
			if (g_InteralMemory.Bit[button_bit]==1)	//button == 1
				g_InteralMemory.Bit[button_bit+9]=1;//status == 1
			else
				g_InteralMemory.Bit[button_bit+9]=0;
			jdqComCount[which_port]=0;
		}
		else
			jdqComCount[which_port]++;
	}
	
	/***栈号一直不正确，jdqComCount[which_port]会判断5次，即该通讯协议会进来5次，
		直接将status置为所需要的位，以保证外部不继续通讯
		eg：if (hongdeng_button!=hongdeng_status)
	***/
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

/*************************************************
Function(函数名称): jidianqi_write_fenshan(U8 which_port)
Description(函数功能、性能等的描述): 风扇的通讯协议(没什么用)，具体注释参考上一个函数
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


/*************************************************
Function(函数名称): xxxx_fun(void)
Description(函数功能、性能等的描述): 控制外部设备(除了电磁阀均通过通讯完成)，
								判断进入的条件if (hongdeng_button!=hongdeng_status)均从通讯中会置位，
								如果不成功会多次通讯
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:jdqComCount[dianci]并无实际意义，将其全部注释;by FJW 2018.1.10
Commented:方佳伟
*************************************************/
void hongdeng_fun(void)
{
	if (hongdeng_button!=hongdeng_status)// && jdqComCount[dianci]==0
		jidianqi_write_single(hongdeng,hongdeng_button_bit);
}

void huangdeng_fun(void)
{
	if (huangdeng_button!=huangdeng_status)// && jdqComCount[dianci]==0
		jidianqi_write_single(huangdeng,huangdeng_button_bit);
}

void lvdeng_fun(void)
{
	if(lvdeng_button!=lvdeng_status )//&& jdqComCount[dianci]==0
		jidianqi_write_single(lvdeng,lvdeng_button_bit);
}

void zhaobudeng_fun(void)
{
	if(zhaobudeng_button!=zhaobudeng_status )//&& jdqComCount[dianci]==0
		jidianqi_write_single(zhaobudeng,zhaobudeng_button_bit);
}

void zhaomingdeng_fun(void)
{
	if (zhaomingdeng_button!=zhaomingdeng_status )//&& jdqComCount[dianci]==0
		jidianqi_write_single(zhaomingdeng,zhaomingdeng_button_bit);
}

void dianci_fun(void)
{
	//只打一次电磁阀，不能够一直开，不然可能会发热(其实一直Set也无所谓)
	if(dianci_button!=dianci_status){
		Set_Y_Value(11,dianci_button);
		dianci_status=dianci_button;
		//jidianqi_write_single(dianci,dianci_button_bit);
	}
		
}

void youbeng_fun(void)
{
	if(youbeng_button!=youbeng_status)// && jdqComCount[dianci]==0
		jidianqi_write_single(youbeng,youbeng_button_bit);
}

void penyou24v_fun(void)
{
	if(penyou24v_button!=penyou24v_status)// && jdqComCount[dianci]==0
		jidianqi_write_single(penyou24v,penyou24v_button_bit);
}

void fenshan_fun(void)
{
	if(fenshan_button!=fenshan_status)// && jdqComCount[dianci]==0
		jidianqi_write_fenshan(fenshan);
}


/*************************************************
Function(函数名称): jidianqi_fun(void)
Description(函数功能、性能等的描述): 通讯写入，使得继电器运作(除了电磁阀以外)
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


