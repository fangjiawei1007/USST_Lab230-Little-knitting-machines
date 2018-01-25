#include"includes.h"

#define	bianpingqi_cal_fac	2.5

unsigned int	bianpingqi_previous_speed=0;
U8 bianpingqi_previous_jog_speed=0;
U8 bianpingqi_jog_status=0;

U8 bianpingqi_run_flag;							//变频器开始动作flag
U8 bianpingqi_previous_run_status=0;			//变频器开始运作之后将此置1
unsigned int bianpingqi_speed;
U8 bpqComCount=0;
unsigned int speed_status = 0;		//speed_status == 1 表示已经降速下来，之后需要延续一圈即可

/*************************************************
Function(函数名称): bianpingqi_RTU_WriteWord(U8 function_num,int Value)
Description(函数功能、性能等的描述): 通讯写入
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
void bianpingqi_RTU_WriteWord(U8 function_num,int Value)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 Count,waitTime; 
	unsigned int check=0;//cc,dd,
	int i;
	int DevAddress;
	U32 ErrorLoop;
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	
	/****选择寄存器地址****/
	switch (bianpingqi_zhonglei){
	case shilin_bianpingqi:
		DevAddress = 0x1002;break;
	case taili_bianpingqi:
		DevAddress = 0x2000;break;
	case PS550_bianpingqi:
		DevAddress = 0x1001;break;
	case PS3000_bianpinqi:
		DevAddress = 0x0002;break;
		
	default:
		DevAddress = 0x1002;break;
	}
	
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
	rGPHDAT |= 0x1000;	//GPH12	+Write，使用MAX485芯片需要改变位
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
	/**20次读取uart的值，读到值直接退出循环**/
	while((RecArray[0]=qz_Uart_Getch())!=bianpingqi_station_num && Count<=20) 
	{
		Count++;
	}
	
	/***栈号正确之后，check判断读到的帧是否有4个是正确的，
		如果是正确的那么直接将bianpingqi_previous_speed=bianpingqi_speed;即外部不继续通讯
		eg：if(bianpingqi_previous_speed!=speed)
	***/
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
			if (bianpingqi_jog_status==1)
			{
				bianpingqi_previous_speed=300;
			}
			else
			{
				bianpingqi_previous_speed=bianpingqi_speed;
				bpqComCount=0;
			}
		}
		else
			bpqComCount++;
	}
	
	/***栈号一直不正确，bpqComCount会判断5次，即该通讯协议会进来5次，
		直接将bianpingqi_previous_speed=bianpingqi_speed;以保证外部不继续通讯
		eg：if(bianpingqi_previous_speed!=speed)
	***/
	else
		bpqComCount++;
	if (bpqComCount>=4){
		bianpingqi_previous_speed=bianpingqi_speed;
		bpqComCount=0;
	}
}


/*************************************************
Function(函数名称): bianpingqi_start_sub(void)
Description(函数功能、性能等的描述): GPIO控制变频器开始，并设置status标志状态。
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): bianpingqi_start();bianpingqi_jog();

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void bianpingqi_start_sub(void)
{
	/***如果外部TS选择控制变频器，则调用时将Y3置1(变频器STR端口);Y1置0(变频器STF端口)***/
	if (Choose_bianpingqi_kb == CHOOSE_BIANPINGQI){
	rGPBDAT &= ~(1<<1);
	rGPBDAT |= 1<<3;
	}
	bianpingqi_previous_run_status=1;
}

/*************************************************
Function(函数名称): bianpingqi_stop_sub(void)
Description(函数功能、性能等的描述): GPIO控制变频器开始，
								 并设置status标志状态以及bianpingqi_previous_speed。
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): bianpingqi_start();bianpingqi_jog();__irq	encoder1_process();
							   bianpingqi_init();main();__irq DataSave_IntHandle(void);

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 该函数被多次调用，其需要注意的是在main函数起初被调用了；
	    掉电保存处__irq DataSave_IntHandle(void)调用了。

Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void bianpingqi_stop_sub(void)
{
	/***如果外部TS选择控制变频器，则调用时将Y3置零(变频器STR端口);Y1置1(变频器STF端口)***/
	if (Choose_bianpingqi_kb == CHOOSE_BIANPINGQI){
		rGPBDAT &= ~(1<<3);		
		rGPBDAT |= 1<<1;
	}
	bianpingqi_previous_speed=0;
	bianpingqi_previous_run_status=0;
}

/*************************************************
Function(函数名称): bianpingqi_set_speed(unsigned int speed)
Description(函数功能、性能等的描述): 485写入变频器的速度值
Calls (被本函数调用的函数清单): bianpingqi_RTU_WriteWord()
Called By (调用本函数的函数清单): encoder1_data_process();

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void bianpingqi_set_speed(unsigned int speed)
{
	if (bianpingqi_previous_speed!=speed&&bianpingqi_jog_status!=1&&Choose_bianpingqi_kb==CHOOSE_BIANPINGQI)
	{
		bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,speed);//正常用
	}
	else if (Choose_bianpingqi_kb==CHOOSE_NOT)
	{
		bianpingqi_previous_speed=bianpingqi_speed;
	}
}


/*************************************************
Function(函数名称): bianpingqi_start(U8 bianpingqi_run_button)
Description(函数功能、性能等的描述): 通过外部IO控制变频器的启停，通过传入的参数bianpingqi_run_button
								  即变频器的控制要与标志位相同，如果不同则进行改变IO口，即完成互斥。
								  互斥即当外部改变，内部才开始改变。
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

/*************************************************
Function(函数名称): bianpingqi_jog(void)
Description(函数功能、性能等的描述): 变频器点动
Calls (被本函数调用的函数清单): bianpingqi_RTU_WriteWord();bianpingqi_start_sub();
							 bianpingqi_stop_sub();
Called By (调用本函数的函数清单): Main()

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void bianpingqi_jog(void)
{
	/**正常情况下，按了急停键就不应该再进入(ext_stop_status!=press)**/
	if ((bianpingqi_run_flag==0 && ext_stop_status!=press) || 
		 qz_error_status==1)
	{
		/***外部按钮按下之后，开始运动，所以此处均使用g_InteralMemory.Bit[n]***/
		if (bianpingqi_jog_button==1 || ext_jog_status==press)
		{
			/************变频器之前未运动，开始运动*************/
			if (bianpingqi_previous_run_status!=1)
			{
				bianpingqi_jog_status=1;
				if (Choose_bianpingqi_kb==CHOOSE_BIANPINGQI){
						bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,300);
				}
				else
					bianpingqi_previous_speed=300;
 				bianpingqi_start_sub();
				//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,(int)(1<<2));
			}
			
			/******判断是否外部有触摸屏幕，如果没有触摸，并且外部按下，则变频器停止******/
			if(Pen_status==Pen_status_up && ext_jog_status!=press)
			{
				bianpingqi_jog_status=0;
				bianpingqi_stop_sub();
				//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,0x0000);
				bianpingqi_jog_button=0;
			}
		}
		
		/****外部按钮送掉，点动停止****/
		if (ext_jog_status!=press && bianpingqi_jog_status==1 && bianpingqi_jog_button==0)
		{
			bianpingqi_jog_status=0;
			bianpingqi_stop_sub();
			//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,0x0000);
			
		}
	}
	
	/****外部按键停止之后，就变频器停止****/
	else if (ext_stop_status==press && qz_error_status==0){
		bianpingqi_jog_status=0;
		bianpingqi_jog_button=0;
		bianpingqi_stop_sub();
	}
}

/*************************************************
Function(函数名称): bianpingqi_speed_cal(void)
Description(函数功能、性能等的描述): 1.大盘减速，因为变频器是控制大盘的
								2.变频器加减速；
								3.分层段/额外段有分层段：大盘先要减速->匀速一圈->加速到fencen_speed
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): songsha_fre_change();

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void bianpingqi_speed_cal(void){
	static unsigned int y1y3delay_flag = 0;
	unsigned int quanshu[7]={0};	
	unsigned int next_stage = 0;
	unsigned int previous_stage = 0;
	
	/***********变频器加减速************/
	if (bianpingqi_speed_up_b == 1){
		if (bianpingqi_fullspeed_set < 7000)
			bianpingqi_fullspeed_set +=100;
		if (bianpingqi_fencen_speed_set < 7000)
			bianpingqi_fencen_speed_set += 100;
		if (bianpingqi_tiaoxian_speed_set < 7000)
			bianpingqi_tiaoxian_speed_set += 100;
		bianpingqi_speed_up_b = 0;
	}
	
	if (bianpingqi_speed_down_b == 1){
		if (bianpingqi_fullspeed_set > 0)
			bianpingqi_fullspeed_set -=100;
		if (bianpingqi_fencen_speed_set > 0)
			bianpingqi_fencen_speed_set -= 100;
		if (bianpingqi_tiaoxian_speed_set > 0)
			bianpingqi_tiaoxian_speed_set -= 100;
		bianpingqi_speed_down_b = 0;
	}		
	/*************************************************************/

	//调线
	/**************************************************************/
	if (tiaoxiankaiguan_kb == 1){//&& current_stage != ewaiduan
		if ((at_check((dapan_round+1)) && encoder1_pulse_number >= 
			(encoder1_cal_factor - jiajiansujiangemaichong_kw))){
			if (weisha_check(dapan_round) == WEISHAJIANSU){
				bianpingqi_speed = bianpingqi_tiaoxian_speed_set;
			}
			else{
				bianpingqi_speed=bianpingqi_fullspeed_set*(bianpingqi_weisha_delta_num/100.0);
			}
			return;
		}
		if ((at_check((dapan_round)) && encoder1_pulse_number < jiajiansujiangemaichong_kw)){
			return;
		}
		
		// if ((at_check((dapan_round+1)) && encoder1_pulse_number >= 
			// (encoder1_cal_factor - jiajiansujiangemaichong_kw))||
			// (at_check((dapan_round)) && encoder1_pulse_number < jiajiansujiangemaichong_kw)){
			
			
		// }
		// weisha_pre_speed_status = 0;
	}
	
	/**quanshu[x+1]，后面的功能代码的next_stage其实是表示的current_stage，因为其没有+1，
	   eg：quanshu[guoduduan] 其实为大头段的圈数，而quanshu[guoduduan+1]表示过渡段圈数 
	**/
	
	quanshu[0] = 0;
	quanshu[datouduan+1] = daduanquanshu;
	quanshu[guoduduan+1] = quanshu[datouduan+1]+middlequanshu;
	quanshu[xiaotouduan+1] = quanshu[guoduduan+1]+xiaoduanquanshu;
	quanshu[fencenduan+1] = quanshu[xiaotouduan+1]+caijiaoquanshu;
	quanshu[caijianduan+1] = quanshu[fencenduan+1]+langfeiquanshu;
	quanshu[ewaiduan+1] = extra_part_quanshu;
	next_stage = getStage(current_stage,NEXTSTAGE);
	previous_stage = getStage(current_stage,PREVIOUSSTAGE);

	
	//不使用降速则直接减速
	if (bianpingqi_huanchongquan_num == 0 || bianpingqi_delta_num >= 100){
		if (current_stage == fencenduan || ewaiduan_fencen_status == 1){
			bianpingqi_speed=bianpingqi_fencen_speed_set;
		}
		else{
			bianpingqi_speed=bianpingqi_fullspeed_set;
		}
		if (Choose_bianpingqi_kb == CHOOSE_NOT){
			Set_Y_Value(1,HIGH);
			Set_Y_Value(3,HIGH);
			y1y3delay_flag = 0;
		}
	}
	
	/**分层段/额外段匀速需要进入该段，进入条件： 0.speed_status == 1; 只有在下一段是分层段/额外段减速之后才会进入此段elseif
										    1.当前不是额外段(是小头段)，在分层段的第一圈会保持该段速度一圈
										    2.当前段为额外段，额外段最后一圈是降速之后需要一直匀速
										    3.当前段为额外段，额外段第一圈匀速(dapan_round <1)(注意：额外段圈数重新计数);
														    大盘圈数==额外分层圈数;
														    大盘圈数等于(额外段总圈数-额外段分层圈)
	**/
	else if ( speed_status == 1 && 
			((current_stage != ewaiduan && dapan_round <(quanshu[current_stage]+1)) || 
			( current_stage == ewaiduan && dapan_round == quanshu[ewaiduan+1] ) ||//最后一个是因为在额外段最后减速时，dapan_round的值为最大 
			( current_stage == ewaiduan && (dapan_round <1 || dapan_round == extra_fencen_quan_num_kw || 
			  dapan_round == (extra_part_quanshu - extra_fencen_quan_num_kw))))){
				
		bianpingqi_speed=bianpingqi_fullspeed_set*(bianpingqi_delta_num/100.0);
		if (Choose_bianpingqi_kb == CHOOSE_NOT && dianci_button == 0){
			if (y1y3delay_flag == 0){
				delay_qz(5,40,1);
			}
			if (delay_fac.delay_permit[5] == 1)
			{
				if (delay_5_count<=5){
					Set_Y_Value(1,HIGH);
					Set_Y_Value(3,HIGH);	
				}
				else if (delay_5_count <= 16){
					Set_Y_Value(1,HIGH);
					Set_Y_Value(3,LOW);
				}
				else{
					Set_Y_Value(1,HIGH);
					Set_Y_Value(3,HIGH);
					delay_qz(5,40,0);
					y1y3delay_flag = 1;
				}
			}
		}
	}
	
	/**分层段/额外段之前需要进入该段，进入条件：1.当前阶段是小头段并且下一个阶段是分层段，在小头段结束前(小头总圈数-缓冲圈数)进行降速
										    2.当前阶段是分层段，在分层段结束前(分层圈数-缓冲圈数)进行降速
										    3.当前阶段是裁剪段并且下一个阶段是额外段，在裁剪段结束前(裁剪段总圈数-缓冲圈数)进行降速
										    4.将speed_status置1，使得分层段的速度能够匀速走一段
	**/
	else if ((next_stage == fencenduan && dapan_round >= (quanshu[next_stage] - bianpingqi_huanchongquan_num))|| 
			 (current_stage == fencenduan && dapan_round >= (quanshu[current_stage+1] - bianpingqi_huanchongquan_num)) || 
			 (next_stage == ewaiduan && extra_fencen_quan_num_kw != 0 && 
			 dapan_round >= (quanshu[next_stage] - bianpingqi_huanchongquan_num))){
		
		bianpingqi_speed=bianpingqi_fullspeed_set*(bianpingqi_delta_num/100.0);
		if (Choose_bianpingqi_kb == CHOOSE_NOT && dapan_round==(quanshu[current_stage+1] - 1) && 
			encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw) || jiajiansujiangemaichong_kw == 0){
				Set_Y_Value(1,LOW);
				Set_Y_Value(3,HIGH);
				y1y3delay_flag = 0;
		}
		speed_status = 1;
	}
	
	/**额外段需要进入该段，进入条件：1.当前阶段额外段，并且额外段的分层段的圈数不为0
								 2.当前阶段是分层段，在分层段结束前(分层圈数-缓冲圈数)进行降速
								 3.当前阶段是裁剪段并且下一个阶段是额外段，在裁剪段结束前(裁剪段总圈数-缓冲圈数)进行降速
								 4.将speed_status置1，使得分层段的速度能够匀速走一段
	**/
	else if (current_stage == ewaiduan && extra_fencen_quan_num_kw != 0 && 
			(dapan_round == (extra_fencen_quan_num_kw -1) || 
			 dapan_round == (extra_part_quanshu - extra_fencen_quan_num_kw -1) || 
			 dapan_round == (extra_part_quanshu -1))){
		
		bianpingqi_speed=bianpingqi_fullspeed_set*(bianpingqi_delta_num/100.0);
		if (Choose_bianpingqi_kb == CHOOSE_NOT && encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw) || jiajiansujiangemaichong_kw == 0){
				Set_Y_Value(1,LOW);
				Set_Y_Value(3,HIGH);
				y1y3delay_flag = 0;
		}
		speed_status = 1;
	}
	
	//正常的大盘速度改变的阶段，bianpingqi_fencen_speed_set/bianpingqi_fullspeed_set(全速或者分层速度)
	else{
		if (current_stage == fencenduan || ewaiduan_fencen_status == 1){
			bianpingqi_speed=bianpingqi_fencen_speed_set;
		}
		else{
			bianpingqi_speed=bianpingqi_fullspeed_set;
		}
		if (Choose_bianpingqi_kb == CHOOSE_NOT){
			Set_Y_Value(1,HIGH);
			Set_Y_Value(3,HIGH);
			y1y3delay_flag = 0;
		}
		speed_status = 0;
	}
}