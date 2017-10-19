#include"includes.h"
U32 encoder1_speed_pulse=0;
U32	encoder1_speed=0;

volatile float motor_factor[7]={0};
unsigned int dapan_round=0;
//float encoder1_cal_factor;
//const int encoder1_cal_factor=8884;
const float encoder1_tran_factor=5;
U16	encoder1_pulse_number=0;

//const float k_factor[7][2]={{0.364,0.519},{0.249,0.309},{0.369,0.399},{0.264,0.324},{0.3285,0.3285},{1,1},{1,1}};
const float k_factor[7][2]={{1,1},{1,1},{1,1},{1,1},{1,1},{1,1},{1,1}};
//const unsigned int pre_set_par[5][3]={{500,200,500},{600,500,655},{585,640,585},{910,850,910},{675,715,675}};
unsigned char jiansu_permite=1;
unsigned char signal;
unsigned char HuanChong_Start=0;
unsigned char ewaiduan_enter_flag=0;

unsigned char caijianduan_sudu_change_flag;
unsigned char caijianduan_sudu_change_record_num;
unsigned char jianshu_ewaiduan_check=0;
unsigned char reset_timer_start=0;
#define	caijianduan_sudu_change_num		8.0


#define FenChenHuanChong_num			8.0

#define	reset_time_100ms				4

unsigned int chudao_jiange_tmp[4] = {0};		//���������¼ by FJW
unsigned int shoudao_jiange_tmp[4] = {0};	//�յ������¼ by FJW

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
		//lingwei_jiance_button = 1; //ѹ����� by FJW
	}
	if (encoder1_jianshu_reset==1)
		jianshu=0;
	if (encoder1_zonggongquanshu_reset==1)
	{
		encoder1_pulse_number=encoder1_pulse_number_save=0;
		dapan_round=dapan_round_save=0;
		extra_part_finish_flag=extra_part_finish;
		extra_part_flag=extra_part_stop;
		
		//lingwei_jiance_button = 1;		//ѹ����� by FJW
		
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

void songsha_fre_change(void){
	unsigned char bb;
	static float k_motor_temp[2][7]={0};
	wdt_feed_dog();main_enter_flag = 1;
	g_InteralMemory.Word[31]=daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu;
	if (g_InteralMemory.Word[31]==0)
		daduanquanshu=1;
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
		if (dapan_round<daduanquanshu){
			current_stage=0;
			if (ewaiduan_enter_flag==0){
			
				HuanChong_Start=0;
				for (bb=0;bb<7;bb++)
				{
					k_motor[bb]=k_factor[bb][datou]*rate_different[bb][datou]*modify_rate_different[bb][datou];
				}
			}
			else{
			
				HuanChong_Start=1;
				for (bb=0;bb<7;bb++)
				{
					k_motor[bb]=k_factor[bb][datou]*rate_different[bb][datou]*modify_rate_different[bb][datou]*
								(g_InteralMemory.KeepWord[103+bb]/100.0+(1.0-g_InteralMemory.KeepWord[103+bb]/100.0)/\
									FenChenHuanChong_num*FenChenHuanChong_Record_kw);
				}
			}
			bianpingqi_speed_cal(ewaiduan_enter_flag);
		}
		else if (dapan_round<(daduanquanshu+middlequanshu)){
			current_stage=1;
			ewaiduan_enter_flag=0;
			for (bb=0;bb<7;bb++){
			
				k_motor[bb]=(k_factor[bb][datou]*rate_different[bb][datou]*modify_rate_different[bb][datou]-\
				             (k_factor[bb][datou]*rate_different[bb][datou]*modify_rate_different[bb][datou]-\
				              k_factor[bb][xiaotou]*rate_different[bb][xiaotou]*modify_rate_different[bb][xiaotou])\
				             /middlequanshu*(dapan_round-daduanquanshu));
			}
			bianpingqi_speed_cal(ewaiduan_enter_flag);
			HuanChong_Start=0;
		}
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu)){
			current_stage=2;
			bianpingqi_speed_cal(ewaiduan_enter_flag);
			for (bb=0;bb<7;bb++)
				k_motor[bb]=k_factor[bb][xiaotou]*rate_different[bb][xiaotou]*modify_rate_different[bb][xiaotou];
			HuanChong_Start=0;
		}
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu)){
			current_stage=3;
			bianpingqi_speed_cal(ewaiduan_enter_flag);
			HuanChong_Start=1;
			for (bb=0;bb<7;bb++){
				k_motor[bb]=k_factor[bb][xiaotou]*rate_different[bb][xiaotou]*modify_rate_different[bb][xiaotou]+\
							(k_factor[bb][xiaotou]*rate_different[bb][fencen]*modify_rate_different[bb][fencen]-\
							k_factor[bb][xiaotou]*rate_different[bb][xiaotou]*modify_rate_different[bb][xiaotou])/\
							FenChenHuanChong_num*FenChenHuanChong_Record_kw;
			}
		}
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu)){
			current_stage=4;
			if (caijianduan_sudu_change_flag != 1)
				HuanChong_Start=0;
			caijianduan_sudu_change_flag=1;
			bianpingqi_speed_cal(ewaiduan_enter_flag);
			if (caijianduan_sudu_change_record_num<=caijianduan_sudu_change_num){
			
				for (bb=0;bb<7;bb++)
				{
					k_motor[bb]=(k_factor[bb][xiaotou]*rate_different[bb][fencen]*modify_rate_different[bb][fencen]+\
					             (k_factor[bb][xiaotou]*rate_different[bb][xiaotou]*modify_rate_different[bb][xiaotou]-\
					              k_factor[bb][xiaotou]*rate_different[bb][fencen]*modify_rate_different[bb][fencen])\
					             /caijianduan_sudu_change_num*caijianduan_sudu_change_record_num);
				}
			}
			else{
			
				HuanChong_Start=1;					//2017-4-15
				for (bb=0;bb<7;bb++){
					k_motor_temp[0][bb] = (k_factor[bb][xiaotou]*rate_different[bb][xiaotou]*modify_rate_different[bb][xiaotou]+		\
					             (k_factor[bb][datou]*rate_different[bb][datou]*modify_rate_different[bb][datou]-						\
					              k_factor[bb][xiaotou]*rate_different[bb][xiaotou]*modify_rate_different[bb][xiaotou])					\
					             /langfeiquanshu*(dapan_round-daduanquanshu-middlequanshu-xiaoduanquanshu-caijiaoquanshu));
					k_motor_temp[1][bb] = (k_factor[bb][xiaotou]*rate_different[bb][xiaotou]*modify_rate_different[bb][xiaotou]+		\
					             (k_factor[bb][datou]*rate_different[bb][datou]*modify_rate_different[bb][datou]-						\
					              k_factor[bb][xiaotou]*rate_different[bb][xiaotou]*modify_rate_different[bb][xiaotou])					\
					             /langfeiquanshu*(dapan_round+1-daduanquanshu-middlequanshu-xiaoduanquanshu-caijiaoquanshu));
					
					k_motor[bb] =	(k_motor_temp[0][bb] + (k_motor_temp[1][bb] - k_motor_temp[0][bb])*
									FenChenHuanChong_Record_kw/FenChenHuanChong_num);
				}
			}	
		}
		else
		{
			jianshu++;										//��������
			*banci[jianshu_no]=*banci[jianshu_no]+1;		//ÿ��μ������ӣ��ı��κ��Զ��ı�banciָ���KeepWord
			if (banci_status_kw!=s_ban)
				dingdan_lianghua_num_kw++;					//��������������ֻ���ڸı䶩���ź������,�ް��ʱ�����Ӱ�ζ�������
			dapan_round=0;
			FenChenHuanChong_Record_kw=0;
			caijianduan_sudu_change_flag=0;
			extra_part_finish_flag=extra_part_finish;
		}
	}
	else{
	
		current_stage=5;	//���¾�Ϊ��Ƭ��
		ewaiduan_enter_flag=1;//����μ�Ϊ��Ƭ��
		bianpingqi_speed_cal(ewaiduan_enter_flag);
		if (dapan_round<extra_fencen_quan_num_kw || dapan_round>(extra_part_quanshu-extra_fencen_quan_num_kw))	{
			HuanChong_Start=1;
			for (bb=0;bb<7;bb++){

					k_motor[bb]=k_factor[bb][datou]*rate_different[bb][datou]*modify_rate_different[bb][datou]*\
								(1+(g_InteralMemory.KeepWord[103+bb]/100.0-1.0)*FenChenHuanChong_Record_kw/FenChenHuanChong_num);
				}
		}
		else{
			HuanChong_Start=0;
			for (bb=0;bb<7;bb++){
				k_motor[bb]=k_factor[bb][datou]*rate_different[bb][datou]*modify_rate_different[bb][datou]*\
							(1+(g_InteralMemory.KeepWord[103+bb]/100.0-1.0)*FenChenHuanChong_Record_kw/FenChenHuanChong_num);
			}
		}
		if (dapan_round>=extra_part_quanshu){
			extra_part_flag=extra_part_stop;
			FenChenHuanChong_Record_kw=0;
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
		WritePage(Block,0,PageBuff);	// ��ʱֻ�õ�0ҳ��2K����������
		PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];
		WritePage(Block,1,PageBuff);	// ��ʱֻ�õ�1ҳ��2K����������
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
	if (((rGPFDAT >> 1) & 0x1) && signal!=Get_X_Value(2)){
		signal=Get_X_Value(2);
		encoder1_speed_pulse++;
		encoder1_pulse_number++;
		
		if(tiaoxiankaiguan_kb == 1){//mode_choose == tiaoxian_mode
			for (jj = 0 ; jj < 4 ; jj++){
				if (chudao_start[jj] == 1 && chudao_start_status[jj] == 0)	//����������� by FJW
				{
					chudao_jiange_tmp[jj] ++;
				}

				if (shoudao_start[jj] == 1 && shoudao_start_status[jj] == 0)	//�յ�������� by FJW
				{
					shoudao_jiange_tmp[jj] ++;
				}
			}	
		}
		if(mode_choose == seven_motor_mode){
			for (jj=0;jj<7;jj++){
				if (jj == 5 && chudao_shoudao_status[0] == 0 && chudao_shoudao_status[1] == 0 )
					continue;
				if (jj == 6 && chudao_shoudao_status[2] == 0 && chudao_shoudao_status[3] == 0 )
					continue;
				motor_factor[jj] += k_motor[jj];
				if (motor_factor[jj]>=1){
					rGPEDAT &= ~(1<<jj);
					motor_factor[jj] -= 1;
					songsha_num[jj]++;
				}
			}
		}
		/* else if(mode_choose == tiaoxian_mode)
		{
			for (jj=0;jj<5;jj++)
			{
				motor_factor[jj] += k_motor[jj];
				if (motor_factor[jj]>=1)
				{
					rGPEDAT &= ~(1<<jj);
					motor_factor[jj] -= 1;
					songsha_num[jj]++;
				}	//�˴�Ϊ��·ѹ��汾+��ɴ byFJW
			}	
		}
		else if(mode_choose == yazheng_mode){
			
			for (jj=0;jj<5;jj++)
			{
				motor_factor[jj] += k_motor[jj];
				if (motor_factor[jj]>=1)
				{
					rGPEDAT &= ~(1<<jj);
					motor_factor[jj] -= 1;
					songsha_num[jj]++;
				}
			}
		}  */
		
	
			
				///////by FJW 2017/4/29 ѹ��/////////
		//ע��average_yazhen��ͨ���ⲿ����ѹ������Լ��ⲿ��¼���������м���ó���ѹ��pulse_total_remember��ͨ���ⲿ��¼���������
		
		/* if(mode_choose == yazheng_mode)
		{
			if(current_stage == 0 && Get_X_Value(11) == uncover)
			{
				//�˴�Ӧ�õ������洰��
			}
			
			if((current_stage == 1 || current_stage == 2) && (yazheng_pulse_send <= yazheng_pulse_need))//current_stage == 2 ����ѹ��û�е�λ
			{
				yazheng_pulse_number ++;
				
				if(yazheng_pulse_number >= average_yazhen && average_yazhen != 0)
				{
					Set_Y_Value(pressing_director_port,pressing_positive);
					
					yazheng_pulse_number -= average_yazhen;
					Set_Y_Value(pressing_pulse_port,1);		//�ⲿʹ��Y9 ��Ӧ���ڲ���GPE3������������ֱ��ͨ��GPIOֱ����1��1�������SET_Y_VALUE()��һ�Ļ��ᱣ��1
					
					yazheng_pulse_send++;
					
				}
				
				pressing_zero_finish = 0;		//����stage4֮�����ֱ�ӻ���
			}
			
			if(current_stage == 4 && Get_X_Value(11) == uncover)	//�˴�ȷ��һ��Ҫ����
			{
				pressing_zero(11);
			}
	
		} */	
		/////////////////////////////////////////////////
		
		//parameter_save();
	}
	else if(signal!=Get_X_Value(2)){
		signal=Get_X_Value(2);
		
		if(mode_choose == seven_motor_mode)
			rGPEDAT |= (0x7f);//7·���ģʽ
		
		/* else if(mode_choose == yazheng_mode)
		{
			rGPEDAT |= (0x1f);//5·���ģʽ
			Set_Y_Value(pressing_pulse_port,1);		//ѹ�� by FJW
			
		} */	//����Ϊѹ��+7·����汾
		
		/* else if(mode_choose == tiaoxian_mode)
			rGPEDAT |= (0x1f);//5·���ģʽ +���� by FJW */
		
		wdt_feed_dog();
		if (encoder1_pulse_number>=encoder1_cal_factor){
			dapan_round++;
			encoder1_pulse_number=0;
			qingche_num_kw++;
			total_quanshu++;								//�����ܼ���
			if (total_quanshu>65530){
				xitong_total_num_upper_kw++;
				total_quanshu=0;
			}		
			if (youbeng_quan_init_flag==1)
				youbeng_quanjianxie_yizhuan_num++;
			if (fenshan_quan_init_flag==1)
				fenshan_jianxie_yizhuanquan_num++;
			if(current_stage == 4)						//2017-4-15
				FenChenHuanChong_Record_kw=0;
			
			//parameter_save();
		}
		if (Get_X_Value(shangduansha_port) == alarm_signal[shangduansha_port] &&
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
		main_enter_flag = 0;				//������������ѭ���������
		if (HuanChong_Start==1){
			if (FenChenHuanChong_Set_kw!=0&&FenChenHuanChong_Record_kw<FenChenHuanChong_num&&\
			encoder1_pulse_number%FenChenHuanChong_Set_kw==0)
				FenChenHuanChong_Record_kw++;
			else if (FenChenHuanChong_Set_kw==0)
				FenChenHuanChong_Record_kw=FenChenHuanChong_num;
		}
		else if (ewaiduan_enter_flag==1&&FenChenHuanChong_Record_kw>0){
			if (FenChenHuanChong_Set_kw!=0&&encoder1_pulse_number%FenChenHuanChong_Set_kw==0)	//2016.12.01����0�жϣ���Ϊ0����ֱ��ΪFenChenHuanChong_num
				FenChenHuanChong_Record_kw--;
			else if(FenChenHuanChong_Set_kw==0)													//2016.12.01
				FenChenHuanChong_Record_kw=0;
		}
		else
			FenChenHuanChong_Record_kw=0;
		if (caijianduan_sudu_change_flag==0)
			caijianduan_sudu_change_record_num=caijianduan_first_speed_fac_kw;
		else if (FenChenHuanChong_Set_kw!=0&&encoder1_pulse_number%FenChenHuanChong_Set_kw==0&&\
				caijianduan_sudu_change_record_num<=caijianduan_sudu_change_num)					//2016.12.01����record_numֻ�ܱ�������1
			caijianduan_sudu_change_record_num++;
		else if (FenChenHuanChong_Set_kw==0)
			caijianduan_sudu_change_record_num=caijianduan_sudu_change_num+1;
		
		
		
	}	
	rEINTPEND=(1<<1); 
	ClearPending((U32)BIT_EINT1);
}


