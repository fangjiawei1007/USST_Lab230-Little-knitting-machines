#include"includes.h"
U32 encoder1_speed_pulse=0;
U32	encoder1_speed=0;

volatile unsigned int k_motor[7]={0};
unsigned int kMotorTarget[7]={0};
volatile unsigned int motor_factor[7]={0};
unsigned int dapan_round=0;
U16	encoder1_pulse_number=0;

const float k_factor[7][3]={{364,519,364},{249,309,249},{369,399,369},{264,291.6,264},{328.5,312.1,328.5},{1000,1000,1000},{1000,1000,1000}};
//const unsigned int pre_set_par[5][3]={{500,200,500},{600,500,655},{585,640,585},{910,850,910},{675,715,675}};
unsigned char jiansu_permite=1;
unsigned char signal;
unsigned char speedUpFlag=0;
unsigned char speedDownFlag=0;

unsigned char jianshu_ewaiduan_check=0;
unsigned char reset_timer_start=0;
unsigned int speedUpCnt=1;
unsigned int speedDownCnt=1;
unsigned int forceEqual = 1;
unsigned int ewaiduan_fencen_status = 0;

#define	reset_time_100ms				4

unsigned int chudao_jiange_tmp[4] = {0};		//出刀间隔记录 by FJW
unsigned int shoudao_jiange_tmp[4] = {0};	//收刀间隔记录 by FJW

void encoder1_data_process(void){
	yuanpan_speed=encoder1_speed*600/encoder1_cal_factor;
	if (Choose_bianpingqi_kb==CHOOSE_BIANPINGQI){
		if (yuanpan_speed>=max_speed){
			delay_qz(3,15,1);
			if (delay_3_count<6)
			{
				if (jiansu_permite==1)
				{
					bianpingqi_fullspeed_set-=20;
					bianpingqi_speed=bianpingqi_fullspeed_set;
					bianpingqi_set_speed(bianpingqi_speed);
					jiansu_permite=0;
				}
			}
			else if (jiansu_permite==0 && delay_3_count>7)
				jiansu_permite=1;
		}
		else
			delay_qz(3,100,0);
	}
}

void encoder1_data_reset(void){
	if(encoder1_dangqianjian_reset==1)
	{
		encoder1_pulse_number=encoder1_pulse_number_save=0;
		extra_part_finish_flag=extra_part_finish;
		extra_part_flag=extra_part_stop;
		dapan_round=dapan_round_save=0;
		forceEqual = 1;
		//lingwei_jiance_button = 1; //压针回零 by FJW
	}
	if (encoder1_jianshu_reset==1)
		jianshu=0;
	if (encoder1_zonggongquanshu_reset==1)
	{
		encoder1_pulse_number=encoder1_pulse_number_save=0;
		dapan_round=dapan_round_save=0;
		extra_part_finish_flag=extra_part_finish;
		extra_part_flag=extra_part_stop;
		forceEqual = 1;
		//lingwei_jiance_button = 1;		//压针回零 by FJW
		
		jianshu=0;
	}
	if (songshaliang_data_reset==1)
	{
		banci_mima_shuru=KeyBoard(999999,0,0,0);
		if (banci_mima_shuru==-1)
			banci_mima_shuru=0;
		else if (banci_status_kw==s_ban || banci_mima_shuru==*banci_mima[banci_status_kw])
		{
			songsha1_num=songsha_num[0]=0;
			songsha2_num=songsha_num[1]=0;
			songsha3_num=songsha_num[2]=0;
			songsha4_num=songsha_num[3]=0;
			*banci[xiada_no]=0;
			*banci[shangda_no]=0;
			*banci[xiaxiao_no]=0;
			*banci[shangxiao_no]=0;
			*banci[jianshu_no]=0;
			banci_mima_shuru=0;
		}
		else if (banci_mima_shuru!=*banci_mima[banci_status_kw])
		{
			banci_mima_shuru=0;
			g_InteralMemory.Word[PING_CTRL]=30;
		}
	}
	if (total_qingling_b==1)
	{
		total_quanshu=0;
		xitong_total_num_upper_kw=0;
	}
}

void SpeedChange(const unsigned int* kMotor){
	static unsigned int previousKMotor[7]={0};
	static unsigned int previousKMotorCompare[7]={0};
	int i;
	if (forceEqual == 1){
		for ( i = 0; i<7 ; i++){
			k_motor[i] = kMotor[i];
			previousKMotor[i] = kMotor[i];
			previousKMotorCompare[i] = kMotor[i];
		}
		forceEqual = 0;
	}
	else{
		for ( i = 0; i<7 ; i++){
			if (speedUpCnt == speedUpMax){
				k_motor[i] = kMotor[i];
				previousKMotor[i] = kMotor[i];
				if (i == 6){
					speedUpCnt = 1;
				}
				continue;
			}
			if (speedDownCnt == speedDownMax){
				k_motor[i] = kMotor[i];
				previousKMotor[i] = kMotor[i];
				if (i == 6){
					speedDownCnt = 1;
				}
				continue;
			}
			if (previousKMotorCompare[i] != kMotor[i]){
				speedUpCnt = 1;
				speedDownCnt = 1;
				previousKMotorCompare[i] = kMotor[i];
				previousKMotor[i] = k_motor[i];
			}
			if (previousKMotor[i] < kMotor[i]){		
				k_motor[i] = (previousKMotor[i] + ( kMotor[i] - previousKMotor[i] )*speedUpCnt/speedUpMax);
				speedUpFlag = 1;
			}
			else if (previousKMotor[i] > kMotor[i]){
				k_motor[i] = (previousKMotor[i] - ( previousKMotor[i] - kMotor[i] )*speedDownCnt/speedDownMax);
				speedDownFlag = 1;
			}
			else if (k_motor[i] != kMotor[i]){
				k_motor[i] = kMotor[i];
			}
		}
		
	}
}

unsigned int getStage(const unsigned int stage,int direction){
	int requestStage = datouduan;
	unsigned char validRound = 0x00;
	if (direction == CURRENT){
		return stage;
	}
	if (daduanquanshu != 0){
		validRound |= 1<<datouduan;
	}
	if (middlequanshu != 0){
		validRound |= 1<<guoduduan;
	}
	if (xiaoduanquanshu != 0){
		validRound |= 1<<xiaotouduan;
	}
	if (caijiaoquanshu != 0){
		validRound |= 1<<fencenduan;
	}
	if (langfeiquanshu != 0){
		validRound |= 1<<caijianduan;
	}
	if (extra_part_quanshu != 0){
		if (direction == NEXTSTAGE){
			if (extra_part_quanshu!=0 && extra_part_jiansu!=0 && jianshu!=0 && (jianshu+1)%extra_part_jiansu==0){
				
				validRound |= 1<<ewaiduan;
			}
		}
		else{
			if (extra_part_quanshu!=0 && extra_part_jiansu!=0 && jianshu!=0 && (jianshu)%extra_part_jiansu==0){
				
				validRound |= 1<<ewaiduan;
			}
		}
	}
	
	for( requestStage=((int)stage + direction);;requestStage += direction){
		if (requestStage < datouduan){
			requestStage = ewaiduan;
		}
		if (requestStage > ewaiduan){
			requestStage = datouduan;
		}
		if ( validRound & 1<<requestStage ){
			return requestStage;
		}			
	}	
}

int getKMotor(const unsigned char bb,const unsigned int stage,int direction){
	int requestStage = 0;
	
	requestStage = getStage(stage,direction);
	if (requestStage == ewaiduan){
		return (k_factor[bb][datou]*rate_different[bb][datou]);
	}
	else if (requestStage != guoduduan && requestStage != caijianduan){
		if (requestStage == datouduan)
			return (k_factor[bb][datou]*rate_different[bb][datou]);
		else if (requestStage == xiaotouduan)
			return (k_factor[bb][xiaotou]*rate_different[bb][xiaotou]);
		else if (requestStage == fencenduan)
			return (k_factor[bb][fencen]*rate_different[bb][fencen]);
		else
			return (k_factor[bb][datou]*rate_different[bb][datou]);
	}
	else{
		for (;requestStage == guoduduan || requestStage == caijianduan;){
			requestStage = getStage(requestStage,direction);
		}
		if (requestStage == datouduan)
			return (k_factor[bb][datou]*rate_different[bb][datou]);
		else if (requestStage == xiaotouduan)
			return (k_factor[bb][xiaotou]*rate_different[bb][xiaotou]);
		else if (requestStage == fencenduan)
			return (k_factor[bb][fencen]*rate_different[bb][fencen]);
		else
			return (k_factor[bb][datou]*rate_different[bb][datou]);
	}
}

void songsha_fre_change(void){
	unsigned char bb;
	wdt_feed_dog();main_enter_flag = 1;
	g_InteralMemory.Word[31]=daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu;
	if (daduanquanshu == 0 && xiaoduanquanshu == 0 && caijiaoquanshu == 0)
		daduanquanshu=100;
	if (extra_part_quanshu!=0 && extra_part_jiansu!=0 && jianshu!=0 && \
	   jianshu%extra_part_jiansu==0 && extra_part_finish_flag==extra_part_finish){
		extra_part_flag=extra_part_start;
		extra_part_finish_flag=extra_part_unfinish;
	}
	if (extra_part_flag!=extra_part_stop && (jianshu==0 || extra_part_jiansu==0 || extra_part_quanshu==0))
	{
		dapan_round=0;
		extra_part_finish_flag=extra_part_finish;
		extra_part_flag=extra_part_stop;
	}
	if (extra_part_flag==extra_part_stop)
	{
		extra_part_finish_flag=extra_part_unfinish;
		if (dapan_round<daduanquanshu || daduanquanshu == 9999){
			current_stage=datouduan;
			for (bb=0;bb<7;bb++){
				kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT);
			}
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		else if (dapan_round<(daduanquanshu+middlequanshu)){
			current_stage=guoduduan;
			for (bb=0;bb<7;bb++){
			
				kMotorTarget[bb]=(getKMotor(bb,current_stage,PREVIOUSSTAGE)-
								 (getKMotor(bb,current_stage,PREVIOUSSTAGE)-getKMotor(bb,current_stage,NEXTSTAGE) )
								 *(dapan_round-daduanquanshu)/middlequanshu); 
			}
			forceEqual=1;
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu)){
			current_stage=xiaotouduan;
			bianpingqi_speed_cal();
			for (bb=0;bb<7;bb++)
				kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT);
			SpeedChange(kMotorTarget);
		}
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu)){
			current_stage=fencenduan;
			bianpingqi_speed_cal();
			for (bb=0;bb<7;bb++){
				kMotorTarget[bb]= getKMotor(bb,current_stage,CURRENT);
			}
			SpeedChange(kMotorTarget);
		}
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu)){
			current_stage=caijianduan;
			bianpingqi_speed_cal();
			for (bb=0;bb<7;bb++){
				kMotorTarget[bb] = (getKMotor(bb,current_stage,PREVIOUSSTAGE)+
								   (getKMotor(bb,current_stage,NEXTSTAGE)-getKMotor(bb,current_stage,PREVIOUSSTAGE) )
									*(dapan_round-daduanquanshu-middlequanshu-xiaoduanquanshu-caijiaoquanshu)/langfeiquanshu);
			}
			SpeedChange(kMotorTarget);
		}
		else
		{
			jianshu++;										//正常作用
			*banci[jianshu_no]=*banci[jianshu_no]+1;		//每班次件数增加，改变班次后，自动改变banci指向的KeepWord
			if (banci_status_kw!=s_ban)
				dingdan_lianghua_num_kw++;					//订单量化计数，只有在改变订单号后会清零,无班次时不增加班次订单计数
			dapan_round=0;
			extra_part_finish_flag=extra_part_finish;
		}
	}
	else{
		current_stage=ewaiduan;	//以下均为挡片段
		bianpingqi_speed_cal();
		if (dapan_round<extra_fencen_quan_num_kw || dapan_round>=(extra_part_quanshu-extra_fencen_quan_num_kw))	{
			for (bb=0;bb<7;bb++){
				kMotorTarget[bb]=(getKMotor(bb,current_stage,CURRENT)*g_InteralMemory.KeepWord[103+bb]/100);
			}
			ewaiduan_fencen_status = 1;
		}
		else{
			for (bb=0;bb<7;bb++){
				kMotorTarget[bb]=(getKMotor(bb,current_stage,CURRENT)*g_InteralMemory.KeepWord[103+bb]);
			}
			ewaiduan_fencen_status = 0;
		}
		SpeedChange(kMotorTarget);
		if (dapan_round>=extra_part_quanshu){
			extra_part_flag=extra_part_stop;
			if (jianshu>=zhibusheding || (previous_dingdanzongshu!=0&&dingdan_lianghua_num_kw>=previous_dingdanzongshu))
				jianshu_ewaiduan_check=1;
			dapan_round=0;
		}
	}
}

void parameter_save(void){
	INT8U *PageBuff;
	int Block;
	
	encoder1_pulse_number_save=encoder1_pulse_number;
	dapan_round_save=dapan_round;
	songsha1_num=songsha_num[0];
	songsha2_num=songsha_num[1];
	songsha3_num=songsha_num[2];
	songsha4_num=songsha_num[3];
	
	if (save_button_b == 1){
		save_button_b = 0;
		Block = Logical_To_Physical_Block(NAND_DATA_SAVE);
		EraseBlock(Block);
		PageBuff = (INT8U *)&g_InteralMemory.KeepWord[0];
		WritePage(Block,0,PageBuff);	// 暂时只用第0页（2K）保存数据
		PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];
		WritePage(Block,1,PageBuff);	// 暂时只用第1页（2K）保存数据
	}
	
}

void parameter_read(void){
	encoder1_pulse_number=encoder1_pulse_number_save;
	dapan_round=dapan_round_save;
	songsha_num[0]=songsha1_num;
	songsha_num[1]=songsha2_num;
	songsha_num[2]=songsha3_num;
	songsha_num[3]=songsha4_num;
}

void __irq	encoder1_process(void)
{
	unsigned char jj;//,signal;
	static unsigned char error_times=0;
	static unsigned int reset_enter_times=0;
	if (((rGPFDAT >> 1) & 0x1) && signal!=((rGPFDAT >> 2) & 0x1)){//Get_X_Value(2)
		signal=((rGPFDAT >> 2) & 0x1);//Get_X_Value(2)
		encoder1_speed_pulse++;
		encoder1_pulse_number++;
		
		if(tiaoxiankaiguan_kb == 1){//mode_choose == tiaoxian_mode
			for (jj = 0 ; jj < 4 ; jj++){
				if (chudao_start[jj] == 1 && chudao_start_status[jj] == 0){	//出刀间隔计算 by FJW
					chudao_jiange_tmp[jj] ++;
				}

				if (shoudao_start[jj] == 1 && shoudao_start_status[jj] == 0){	//收刀间隔计算 by FJW
					shoudao_jiange_tmp[jj] ++;
				}
			}	
		}
		for (jj=0;jj<4;jj++)
		{
			motor_factor[jj] += k_motor[jj];
			if (motor_factor[jj]>=1000000)
			{
				rGPEDAT &= ~(1<<jj);
				motor_factor[jj] -= 1000000;
				songsha_num[jj]++;
			}
			if (jj != 3 && (motor_factor[jj + 4] >= 500000 || k_motor[jj + 4] >= 500000)){
				rGPEDAT |= (1<<(jj + 4));
			}
		}
		//rGPEDAT |= (0x7 << 4);
	}
	else if(signal!=((rGPFDAT >> 2) & 0x1)){//Get_X_Value(2)
		signal=((rGPFDAT >> 2) & 0x1);//Get_X_Value(2)
		
		for (jj=4;jj<8;jj++)
		{
			if (jj != 7){
				motor_factor[jj] += k_motor[jj];
				if (motor_factor[jj]>=1000000)
				{
					rGPEDAT &= ~(1<<jj);
					motor_factor[jj] -= 1000000;
					songsha_num[jj]++;
				}
			}
			if (motor_factor[jj - 4] >= 500000 || k_motor[jj - 4] >= 500000){
				rGPEDAT |= (1<<(jj - 4));
			}
		}
		//rGPEDAT |= (0xf);
		
		
		rWTCNT = reset_time_kw;	//wdt_feed_dog();
		if (encoder1_pulse_number>=encoder1_cal_factor){
			dapan_round++;
			encoder1_pulse_number=0;
			qingche_num_kw++;
			total_quanshu++;								//用作总计数
			if (total_quanshu>65530){
				xitong_total_num_upper_kw++;
				total_quanshu=0;
			}		
			if (youbeng_quan_init_flag==1)
				youbeng_quanjianxie_yizhuan_num++;
			if (fenshan_quan_init_flag==1)
				fenshan_jianxie_yizhuanquan_num++;
		}
		if ( ((rGPFDAT >> 7) & 0x1) == alarm_signal[shangduansha_port] &&//Get_X_Value(shangduansha_port)
		    sys_force_run_button == 0 && 
			shangduansha_alarm_level!=level_0){
			error_times++;
			if (error_times >= 5){
				rGPBDAT &= ~(1<<3);
				rGPBDAT |= 1<<1;
				bianpingqi_previous_speed=0;
				bianpingqi_previous_run_status=0;
			
				emer_stop_flag=0;
				run_permite_flag=0;
				qz_error_status=1;
				bianpingqi_start_button=0;
				bianpingqi_stop_button=0;
				bianpingqi_jog_button=0;
				bianpingqi_run_flag=0;
				bianpingqi_jog_status=0;
				error_times=0;
			}
		}
		if (main_enter_flag == 0){
			reset_timer_start = 1;
			reset_enter_times ++;
			if (reset_timer_counter > reset_time_100ms ){//|| reset_enter_times > 10000
				bianpingqi_stop_sub();
				Set_Y_Value(2,0);
			}
		}
		else{
			reset_timer_start = 0;
			reset_timer_counter = 0;
			reset_enter_times = 0;
		}
		main_enter_flag = 0;				//用以做主程序循环正常检测
		if (speedUpFlag==1){
			if (huanchongmaichong!=0&&speedUpCnt<speedUpMax&&
			encoder1_pulse_number%huanchongmaichong==0)
				speedUpCnt++;
			else if (huanchongmaichong==0){
				speedUpCnt=speedUpMax;
				speedUpFlag=0;
			}
			else if (speedUpCnt >= speedUpMax){
				speedUpFlag=0;
			}	
		}
		if (speedDownFlag==1){
			if (huanchongmaichong!=0&&speedDownCnt<speedDownMax&&
			encoder1_pulse_number%huanchongmaichong==0)
				speedDownCnt++;
			else if (huanchongmaichong==0){
				speedDownCnt=speedDownMax;
				speedDownFlag=0;
			}
			else if (speedDownCnt >= speedDownMax){
				speedDownFlag=0;
			}
		}
	}
	rEINTPEND=(1<<1); 
	ClearPending((U32)BIT_EINT1);
}


