#include"includes.h"

#define	bianpingqi_cal_fac	2.5

unsigned int	bianpingqi_previous_speed=0;
U8 bianpingqi_previous_jog_speed=0;
U8 bianpingqi_jog_status=0;

U8 bianpingqi_run_flag;
U8 bianpingqi_previous_run_status=0;
unsigned int bianpingqi_speed;
U8 bpqComCount=0;
unsigned int speed_status = 0;

void bianpingqi_RTU_WriteWord(U8 function_num,int Value)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 Count,waitTime; 
	unsigned int check=0;//cc,dd,
	int i;
	int DevAddress;
	U32 ErrorLoop;
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	
	switch (bianpingqi_zhonglei){
	case shilin_bianpingqi:
		DevAddress = 0x1002;break;
	case taili_bianpingqi:
		DevAddress = 0x2000;break;
	case PS550_bianpingqi:
		DevAddress = 0x1001;break;
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
	else
		bpqComCount++;
	if (bpqComCount>=4){
		bianpingqi_previous_speed=bianpingqi_speed;
		bpqComCount=0;
	}
}

void bianpingqi_start_sub(void)
{
	if (Choose_bianpingqi_kb == CHOOSE_BIANPINGQI){
	rGPBDAT &= ~(1<<1);
	rGPBDAT |= 1<<3;
	}
	bianpingqi_previous_run_status=1;
}

void bianpingqi_stop_sub(void)
{
	if (Choose_bianpingqi_kb == CHOOSE_BIANPINGQI){
		rGPBDAT &= ~(1<<3);
		rGPBDAT |= 1<<1;
	}
	bianpingqi_previous_speed=0;
	bianpingqi_previous_run_status=0;
}


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
						bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,300);
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

void bianpingqi_speed_cal(void){
	static unsigned int y1y3delay_flag = 0;
	unsigned int quanshu[7]={0};
	unsigned int next_stage = 0;
	unsigned int previous_stage = 0;
	
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
	if (tiaoxiankaiguan_kb == 1){//&& current_stage != ewaiduan
		if ((at_check((dapan_round+1)) && encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw))||
			(at_check((dapan_round)) && encoder1_pulse_number < jiajiansujiangemaichong_kw)){
			
			bianpingqi_speed = bianpingqi_tiaoxian_speed_set;
			return;
		}
	}
	quanshu[0] = 0;
	quanshu[datouduan+1] = daduanquanshu;
	quanshu[guoduduan+1] = quanshu[datouduan+1]+middlequanshu;
	quanshu[xiaotouduan+1] = quanshu[guoduduan+1]+xiaoduanquanshu;
	quanshu[fencenduan+1] = quanshu[xiaotouduan+1]+caijiaoquanshu;
	quanshu[caijianduan+1] = quanshu[fencenduan+1]+langfeiquanshu;
	quanshu[ewaiduan+1] = extra_part_quanshu;
	next_stage = getStage(current_stage,NEXTSTAGE);
	previous_stage = getStage(current_stage,PREVIOUSSTAGE);
	
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
	else if ( speed_status == 1 && 
			((current_stage != ewaiduan && dapan_round <(quanshu[current_stage]+1)) || 
			( current_stage == ewaiduan && dapan_round == quanshu[ewaiduan+1] ) ||//最后一个是因为在额外段最后减速时，dapan_round的值为最大 
			( current_stage == ewaiduan && (dapan_round <1 || dapan_round == extra_fencen_quan_num_kw || dapan_round == (extra_part_quanshu - extra_fencen_quan_num_kw))))){
				
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
	else if ((next_stage == fencenduan && dapan_round >= (quanshu[next_stage] - bianpingqi_huanchongquan_num))|| 
			 (current_stage == fencenduan && dapan_round >= (quanshu[current_stage+1] - bianpingqi_huanchongquan_num)) || 
			 (next_stage == ewaiduan && extra_fencen_quan_num_kw != 0 && dapan_round >= (quanshu[next_stage] - bianpingqi_huanchongquan_num))){
		
		bianpingqi_speed=bianpingqi_fullspeed_set*(bianpingqi_delta_num/100.0);
		if (Choose_bianpingqi_kb == CHOOSE_NOT && dapan_round==(quanshu[current_stage+1] - 1) && 
			encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw) || jiajiansujiangemaichong_kw == 0){
				Set_Y_Value(1,LOW);
				Set_Y_Value(3,HIGH);
				y1y3delay_flag = 0;
		}
		speed_status = 1;
	}
	else if (current_stage == ewaiduan && extra_fencen_quan_num_kw != 0 && 
			(dapan_round == (extra_fencen_quan_num_kw -1) || dapan_round == (extra_part_quanshu - extra_fencen_quan_num_kw -1) || dapan_round == (extra_part_quanshu -1))){
		
		bianpingqi_speed=bianpingqi_fullspeed_set*(bianpingqi_delta_num/100.0);
		if (Choose_bianpingqi_kb == CHOOSE_NOT && encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw) || jiajiansujiangemaichong_kw == 0){
				Set_Y_Value(1,LOW);
				Set_Y_Value(3,HIGH);
				y1y3delay_flag = 0;
		}
		speed_status = 1;
	}
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