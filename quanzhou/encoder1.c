#include"includes.h"
U32 encoder1_speed_pulse=0;						//���ڼ���speed��������
U32	encoder1_speed=0;							//���ڼ���speed��������(�����ٶ�)��tmpֵ

volatile unsigned int k_motor[7]={0};			//���Kֵ��ŵ�����(ʵ��)
unsigned int kMotorTarget[7]={0};
volatile unsigned int motor_factor[7]={0};		//���ں�1000000�Ƚϣ�������һ���м����
unsigned int dapan_round=0;						//����ת�˶���Ȧ�ļ��ֵ
U16	encoder1_pulse_number=0;					//����������������������

const float k_factor[7][3]={{364,519,364},{249,309,249},{369,399,369},{264,291.6,264},{328.5,312.1,328.5},{1000,1000,1000},{1000,1000,1000}};
unsigned char jiansu_permite=1;					//���̳�����֮�����
unsigned char signal;							//��ȡX2��״ֵ̬
unsigned char speedUpFlag[7]={0};
unsigned char speedDownFlag[7]={0};

unsigned char jianshu_ewaiduan_check=0;
unsigned char reset_timer_start=0;
unsigned int speedUpCnt[7]={1};
unsigned int speedDownCnt[7]={1};
unsigned int forceEqual = 1;		//���ã���ɴ������ٵ�ʱ��ǿ�����
									//��ͨ������
unsigned int forceDownEqual = 0;
unsigned int forceUpEqual = 0;

unsigned int ewaiduan_fencen_status = 0;		//ewaiduan_fencen_status=1ʱ����ʾʹ�ö���εķֲ�

#define	reset_time_100ms				4

/*************************************************
Function(��������): encoder1_data_process(void)
Description(�������ܡ����ܵȵ�����): �������ٶȳ����趨������ٶȣ�ͨ��ͨѶ������Ƶ����ʹ���̽���
Calls (�����������õĺ����嵥): delay_qz(U8 delay_rank,U32 delay_t,U8 status);bianpingqi_set_speed(U32 speed)
Called By (���ñ������ĺ����嵥): main()

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void encoder1_data_process(void){
	yuanpan_speed=encoder1_speed*600/encoder1_cal_factor;	//��������ٶ�
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

/*************************************************
Function(��������): encoder1_data_reset(void)
Description(�������ܡ����ܵȵ�����):��������;������Ҫ��Ϊ�˽��������ļ�¼ֵ���㣬
								���ҽ�dapan_round���㣬����songsha_freq_change�Ϳ������¹���
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void encoder1_data_reset(void){
	if(encoder1_dangqianjian_reset==1)
	{
		dapan_round=dapan_round_save=0;
		encoder1_pulse_number=encoder1_pulse_number_save=0;
		
		extra_part_finish_flag=extra_part_unfinish;
		extra_part_flag=extra_part_stop;
		ewaiduan_fencen_status = 0;
		
		if (encoder1_speed == 0)
			forceEqual = 1;
		speed_status = 0;
		
		if (tiaoxiankaiguan_kb == 1){
			tiaoxian_reset();
		}
		//lingwei_jiance_button = 1; //ѹ����� by FJW
	}
	if (encoder1_jianshu_reset==1)
		jianshu=0;
	if (encoder1_zonggongquanshu_reset==1)
	{
		encoder1_pulse_number=encoder1_pulse_number_save=0;
		dapan_round=dapan_round_save=0;
		
		extra_part_finish_flag=extra_part_unfinish;
		extra_part_flag=extra_part_stop;
		ewaiduan_fencen_status = 0;
		if (encoder1_speed == 0)
			forceEqual = 1;
		speed_status = 0;
		//lingwei_jiance_button = 1;		//ѹ����� by FJW
		if (tiaoxiankaiguan_kb == 1){
			tiaoxian_reset();
		}
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


/*************************************************
Function(��������): SpeedChange(const unsigned int* kMotor)
Description(�������ܡ����ܵȵ�����): ��֤�����׶ε��ٶȱ仯��ͻ�䣬ͨ��ǰ�������׶ε�Kֵ�ı仯���Ƚ�
								 Kֵ�ı���֮����ô�ͻ����˺���������speed_up_MAX���ٶȵĸı�.
								 ���ɶ�/�ü��εı���ΪKֵ�ĸı䣬��dapan_round�йأ��ʹ˴��޹ء�
								 (songsha_fre_change)����תһȦ���ⲿĿ��Kֵ�ı䡣
								 (SpeedChange)K_Motorֵ�ı䣬�ڴ˴�����ж�ε�k_motorֵ->k_Motor��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input: const unsigned int* kMotor
	   kMotorΪ�ⲿ���Σ�Kֵ����
Output(�����������˵��):
Return: 
Others: ��һ�����ã��ϵ�ֱ�ӽ�Kֵ�趨��λ����ͨ������ʵ�� 2018.01.07
	    �ڶ������ã��ı��һ��Kֵ
	    ����������(���Ƶ�һ��)��ǿ�ƽ���
	    ���ĸ����ã����ⲿǿ�Ƹı�Kֵ������£����½��б���ʵ�֡�
		
		������ٶ���speed_Change�εı���
Author:������
Modified:�����ֲ�����k_motor_cal[7],���ڼ����м����������_irq encoder1_process()��k_motor��ʹ��Ч�� by FJW 2018.1.10
Commented:����ΰ
*************************************************/
void SpeedChange(const unsigned int* kMotor){
	static unsigned int previousKMotor[7]={0};
	static unsigned int previousKMotorCompare[7]={0};
	static unsigned int k_motor_cal[7]={0};				//��Ϊ����ֵk_motor_cal[7]���м����
	
	int i;
	
	//�ϵ��һ��ǿ�ƽ��ٶȼӵ�Kֵ����ͨ������ִ��
	if (forceEqual == 1){
		for ( i = 0; i<7 ; i++){
			//k_motor[i] = kMotor[i];
			k_motor_cal[i] = kMotor[i];
			previousKMotor[i] = kMotor[i];
			previousKMotorCompare[i] = kMotor[i];
			speedUpFlag[i] = 0;
			speedDownFlag[i] = 0;
			speedUpCnt[i] = 1;
			speedDownCnt[i] = 1;
		}
		forceEqual = 0;
	}
	
	//��·�����һ�α仯kֵ����Ҫ��Ϊ�˽�speedDownFlag[]/speedUpFlag[]��1��
	//��_irq encoder1_process()��,���ж�α任(irq��speedDownFlag[])
	else{
		for ( i = 0; i<7 ; i++){
			
			//speedUpMax�μ���/���ٽ����Ժ�speedUpFlag[n]/speedDownFlag[n]��0��
			//speedUpCnt[n]/speedDownCnt[n]����
			if (speedUpCnt[i] >= speedUpMax && speedUpFlag[i] == 1){
				k_motor_cal[i] = kMotor[i];
				previousKMotor[i] = kMotor[i];
				speedUpFlag[i] = 0;
				speedUpCnt[i] = 1;
			}
			if (speedDownCnt[i] >= speedDownMax && speedDownFlag[i] == 1){
				k_motor_cal[i] = kMotor[i];
				previousKMotor[i] = kMotor[i];
				speedDownFlag[i] = 0;
				speedDownCnt[i] = 1;
			}
			
			//��Ϊ����ڱ仯���֮ǰ�����ı���Ŀ���ٶȣ���ӵ�ǰ�ٶ����´�1,2,3��ʼ�仯��Ŀ���ٶ�
			if (previousKMotorCompare[i] != kMotor[i]){ 
				speedUpCnt[i] = 1;
				speedDownCnt[i] = 1;
				speedDownFlag[i] = 0;
				speedUpFlag[i] = 0;
				previousKMotorCompare[i] = kMotor[i];
				previousKMotor[i] = k_motor[i];
			}
			
			//����֮��flag��1����һ�α��ٿ�ʼ
			if (previousKMotor[i] < kMotor[i]){
				//forceUpEqualδʹ��
				if (forceUpEqual == 1){
					k_motor_cal[i] = kMotor[i];
					previousKMotor[i] = kMotor[i];
					previousKMotorCompare[i] = kMotor[i];
				}
				
				else{
					/**��ʽʹ���м������Ϊ������Ϊk_motorΪ�ж���ֱ��ʹ�õ�ֵ��by FJW**/
					k_motor_cal[i] = (previousKMotor[i] + ( kMotor[i] - previousKMotor[i] )*(int)speedUpCnt[i]/speedUpMax);
					
					/**�������kֵ��ʵ�ʵ�kֵ��һ��ʱ��ȥ���б任 by FJW**/
					/* if(k_motor_cal[i] != k_motor[i])
					{
						k_motor[i] = k_motor_cal[i];
					} */
					
					speedUpFlag[i] = 1;
					speedDownFlag[i] = 0;
				}
			}
			else if (previousKMotor[i] > kMotor[i]){
				//ǿ�ƽ��٣��ⲿ���ɶλ�ʹ��
				if (forceDownEqual == 1){
					k_motor_cal[i] = kMotor[i];
					previousKMotor[i] = kMotor[i];
					previousKMotorCompare[i] = kMotor[i];
				}
				else{
					/**��ʽʹ���м������Ϊ������Ϊk_motorΪ�ж���ֱ��ʹ�õ�ֵ��by FJW**/
					k_motor_cal[i] = (previousKMotor[i] - ( previousKMotor[i] - kMotor[i] )*(int)speedDownCnt[i]/speedDownMax);
					
					/**�������kֵ��ʵ�ʵ�kֵ��һ��ʱ��ȥ���б任 by FJW**/
					/* if(k_motor_cal[i] != k_motor[i])
					{
						k_motor[i] = k_motor_cal[i];
					} */
					
					speedDownFlag[i] = 1;
					speedUpFlag[i] = 0;
				}
			}
			
			//�ݴ�����
			else if (k_motor_cal[i] != kMotor[i]){
				k_motor_cal[i] = kMotor[i];
			}
		}
		forceUpEqual = 0;
		forceDownEqual = 0;
	}
	
	/**���м��жϵ�ֵ�ŵ�������ж��Ƿ�kֵ�иı䣬����иı���ı���kֵ**/
	for(i = 0;i<7;i++)
	{
		if(k_motor_cal[i] != k_motor[i])
		{
			k_motor[i] = k_motor_cal[i];
		}
	}
}

/*************************************************
Function(��������): getStage(void)
Description(�������ܡ����ܵȵ�����): �����Ҫ��stage�������ж�ǰ������Ҫ��kֵ��
								  ������ǰ���ٶȱ仯��������
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): bianpingqi_speed_cal();at_check();getKMotor();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): stage������Ҫ�Ľ׶εĻ����׶�
													    direction������Ҫ�Ļ����׶εı仯�׶�
Output(�����������˵��):
Return: requestStage(����Ҫ�õ���stage)
Others: ��������˼���ǣ�
Author:������
Modified:
Commented:����ΰ
*************************************************/
unsigned int getStage(const unsigned int stage,int direction){
	int requestStage = datouduan;
	unsigned char validRound = 0x00;
	
	//��ȡ��ǰstageֵ
	if (direction == CURRENT){
		return stage;
	}
	
	/******************��ȡ��Ч�������ж���Щ������Ч��(����ν�Ϊ����)****************/
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
	
	//����ν����⣬�����������
	if(extra_part_quanshu != 0){
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

/*************************************************************************/


/**********�ж��Ƿ�ö�Ϊ��Ч�Σ���Ϊ��Ч���򷵻ظöΣ�
		   ��Ϊ��Ч�Σ��򷵻���һ����(ȡ����Previous/Next)************/
	for(requestStage=((int)stage + direction);;requestStage += direction){
		
		//�����߽�
		if (requestStage < datouduan){
			requestStage = ewaiduan;
		}
		if (requestStage > ewaiduan){
			requestStage = datouduan;
		}
		
		if ( validRound & (1<<requestStage) ){
			return requestStage;
		}			
	}	
}

/*************************************************
Function(��������): getKMotor(void)
Description(�������ܡ����ܵȵ�����): ��ȡ��������ĸ���ϵ��(7·���)
Calls (�����������õĺ����嵥): getStage(stage,direction)
Called By (���ñ������ĺ����嵥): songsha_fre_change();
Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): bb:��bb�������
															stage����ǰ�����Ķ�����
															direction���������CURRENT;PREVIOUS;NEXTSTAGE
															���к����������Ǹ����õĺ���getStage(stage,direction)����
Output(�����������˵��):
Return: k_factor[bb][x]*rate_different[bb][x]��������������ʦ��˵��Kֵ
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
int getKMotor(const unsigned char bb,const unsigned int stage,int direction){
	int requestStage = 0;
	
	requestStage = getStage(stage,direction);
	
	//�����Ҫ��stage�Ƕ���Σ���ô���ش�ͷ�ε��ٶȣ���Ϊ
	if (requestStage == ewaiduan){
		return (k_factor[bb][datou]*rate_different[bb][datou]);
	}
	
	//�ڸ����λ������Ҫ��kֵ
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
	
	/**�˴�Ϊ���ɶλ��߲ü��εĹ��ܣ����ڴ˶���Ϊ�����˶���
	   ���Ա���֪���俪ʼ�ٶ��Լ������ٶȣ��˴�forѭ����
	   �����ڹ��ɶκͲü���ʹ��PREVIOUS||NEXT����Ȼ����ѭ��
	**/
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


/*************************************************
Function(��������): songsha_fre_change(void)
Description(�������ܡ����ܵȵ�����): 1.Kֵ�ı仯
								 2.Speed_change(),ʹ��k_current->k_target��ͻ��
								 3.���̵��ٶȵĸı�(��Ƶ��)
Calls (�����������õĺ����嵥): wdt_feed_dog();between_check();
							getKMotor(const unsigned char bb,const unsigned int stage,int direction);
							SpeedChange(const unsigned int* kMotor);bianpingqi_speed_cal();
Called By (���ñ������ĺ����嵥): stepmotor_init();KeyBoardScan();void Main(void)

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void songsha_fre_change(void){
	unsigned char bb;
	int i;
	wdt_feed_dog();main_enter_flag = 1;
	//����Ȧ������g_InteralMemory.Word[31]��
	g_InteralMemory.Word[31]=daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu;
	
	//�ݴ�����ͷȦ��һ��Ҫ����
	if (daduanquanshu == 0 && xiaoduanquanshu == 0 && caijiaoquanshu == 0)
		daduanquanshu=100;
	
	/***��Ҫ������ν��������������Ȧ����Ϊ0������μ�����Ϊ0����֯������Ϊ0��
			 ��֯����%�������Ҫ����==0����־λΪ0
	***/
	if (extra_part_quanshu!=0 && extra_part_jiansu!=0 && jianshu!=0 && \
	   jianshu%extra_part_jiansu==0 && extra_part_finish_flag==extra_part_finish){
		extra_part_flag=extra_part_start;
		extra_part_finish_flag=extra_part_unfinish;
	}
	
	//����ι���ʱ�����޵���||(����μ���==0)||(�����Ȧ��==0)������£�ֹͣʹ�ö����
	if (extra_part_flag!=extra_part_stop && (jianshu==0 || extra_part_jiansu==0 || extra_part_quanshu==0))
	{
		dapan_round=0;
		extra_part_finish_flag=extra_part_finish;
		extra_part_flag=extra_part_stop;
	}
	
	//�����ж���εĹ���֮��Ϊ�����Ĺ���״̬
	if (extra_part_flag==extra_part_stop)
	{
		extra_part_finish_flag=extra_part_unfinish;
		ewaiduan_fencen_status = 0;
		
	//��ǰ��Ϊ��ͷ��
		if (dapan_round<daduanquanshu || daduanquanshu == 9999){
			current_stage=datouduan;
			i = between_check(dapan_round);
			//i ��= -1 Ϊ���ߵĹ��ܴ���
			if ( i != -1){
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT)*(*tiaoxianduan[i].fangdabeishu[bb])/100;
				}
			}
			//else Ϊ�޵��߹������룬��ô�ͷ�εĸ���Kֵ
			else{
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT);
				}
			}
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		
	//��ǰ��Ϊ���ɶ�
		else if (dapan_round<(daduanquanshu+middlequanshu)){
			current_stage=guoduduan;
			i = between_check(dapan_round);
			if ( i != -1){
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=	((getKMotor(bb,current_stage,PREVIOUSSTAGE)-
										(((getKMotor(bb,current_stage,PREVIOUSSTAGE)-getKMotor(bb,current_stage,NEXTSTAGE))
										*(int)(dapan_round-daduanquanshu))/middlequanshu))*(*tiaoxianduan[i].fangdabeishu[bb])/100); 
				}
			}
			else{
				for (bb=0;bb<7;bb++){
			
					/**Kֵ���� K_need = (K1 - ((K1-K2)*x)/y))**/
					kMotorTarget[bb]=	(getKMotor(bb,current_stage,PREVIOUSSTAGE)-
										(((getKMotor(bb,current_stage,PREVIOUSSTAGE)-getKMotor(bb,current_stage,NEXTSTAGE))
										*(int)(dapan_round-daduanquanshu))/middlequanshu)); 
				}
			}
			
			forceDownEqual=1;			//ǿ�ƽ���n�μ��ٵĵ�1���׶ε�Kֵ
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		
	//��ǰ��ΪСͷ��
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu)){
			current_stage=xiaotouduan;
			i = between_check(dapan_round);
			if ( i != -1){
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT)*(*tiaoxianduan[i].fangdabeishu[bb])/100;
				}
			}
			else{
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT);
				}
			}
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		
	//��ǰ��Ϊ�ֲ��
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu)){
			current_stage=fencenduan;
			i = between_check(dapan_round);
			if ( i != -1){
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT)*(*tiaoxianduan[i].fangdabeishu[bb])/100;
				}
			}
			else{
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT);
				}
			}
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		
	//��ǰ��Ϊ�ü���
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu)){
			current_stage=caijianduan;
			for (bb=0;bb<7;bb++){//�ü�Ȧ������
				//�ü��εĳ�ֵ��Сͷ�εĹ�����ͬ���ͷֲ�ε�Kֵ��ͬ��
				kMotorTarget[bb] = (getKMotor(bb,fencenduan,PREVIOUSSTAGE)+
								   (((getKMotor(bb,current_stage,NEXTSTAGE)-getKMotor(bb,fencenduan,PREVIOUSSTAGE))
									*(int)(dapan_round-daduanquanshu-middlequanshu-xiaoduanquanshu-caijiaoquanshu))/langfeiquanshu));
			}
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		
		else
		{
			jianshu++;										//��������
			*banci[jianshu_no]=*banci[jianshu_no]+1;		//ÿ��μ������ӣ��ı��κ��Զ��ı�banciָ���KeepWord
			if (banci_status_kw!=s_ban)
				dingdan_lianghua_num_kw++;					//��������������ֻ���ڸı䶩���ź������,�ް��ʱ�����Ӱ�ζ�������
			dapan_round=0;
			extra_part_finish_flag=extra_part_finish;		//�����Ҳ����
		}
	}
	
	//���¾�Ϊ��Ƭ��(�����)������ο�ʼ��ʱ��dapan_round�Ѿ�����
	else{
		current_stage=ewaiduan;	
		i = between_check(dapan_round);
		
		//����ηֲ㲿�֣���Ҫ����ewaiduan_fencen_status = 1;����bianpingqi_speed_cal()��ʹ��
		if (dapan_round<extra_fencen_quan_num_kw || dapan_round>=(extra_part_quanshu-extra_fencen_quan_num_kw))	{
			
			if ( i != -1){
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=(getKMotor(bb,current_stage,CURRENT)*g_InteralMemory.KeepWord[103+bb]/100)*(*tiaoxianduan[i].fangdabeishu[bb])/100;
				}
			}
			else{
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=(getKMotor(bb,current_stage,CURRENT)*g_InteralMemory.KeepWord[103+bb]/100);
				}
			}
			ewaiduan_fencen_status = 1;
		}
		
		//����β��ֲ㲿��
		else{
			
			if ( i != -1){
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT)*(*tiaoxianduan[i].fangdabeishu[bb])/100;
				}
			}
			else{
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT);
				}
			}
			ewaiduan_fencen_status = 0;
		}
		SpeedChange(kMotorTarget);
		bianpingqi_speed_cal();
		
		//��������н���֮�����λ
		if (dapan_round>=extra_part_quanshu){
			extra_part_flag=extra_part_stop;
			ewaiduan_fencen_status = 0;
			
			//
			if (jianshu>=zhibusheding || (previous_dingdanzongshu!=0&&dingdan_lianghua_num_kw>=previous_dingdanzongshu))
				jianshu_ewaiduan_check=1;
			
			dapan_round=0;
		}
	}
}

/*************************************************
Function(��������): parameter_save(void)
Description(�������ܡ����ܵȵ�����): ����������flash��kw,encoder1_pulse_number��dapan_round
								  songsha1_num[0~4]
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): menu_init();main();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
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

/*************************************************
Function(��������): parameter_read(void)
Description(�������ܡ����ܵȵ�����): ��ȡflash��kw�ı���,����Ϊ��encoder1_pulse_number_save��dapan_round_save��
								  songsha1_num��songsha2_num��songsha3_num��songsha4_num
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): stepmotor_init();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void parameter_read(void){
	encoder1_pulse_number=encoder1_pulse_number_save;
	dapan_round=dapan_round_save;
	songsha_num[0]=songsha1_num;
	songsha_num[1]=songsha2_num;
	songsha_num[2]=songsha3_num;
	songsha_num[3]=songsha4_num;
}

/*************************************************
Function(��������): __irq	encoder1_process(void)
Description(�������ܡ����ܵȵ�����): �������жϺ���(����˶��Ĺؼ���ͨ��������ʵ���ٶȻ�����)
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: �жϺ�����Ӧ�ù��࣬�ᵼ���жϺ����޷�ִ���꣬������δ֪������ش����
	   ����Ӧ�ñ����κ��жϺ����Ŀ������������Ϊ������������Ŀ����Լ��������õĿ�����
        ѭ������(ʱ�临�Ӷ�);�ж������Ŀ���Ҳ�ڿ��Ƿ�Χ֮�ڡ�
																		����by FJW
Author:������
Modified:
Commented:����ΰ
*************************************************/
void __irq	encoder1_process(void)
{
	unsigned int jj,zushu;//,signal;
	static unsigned int error_times=0;
	static unsigned int reset_enter_times=0;	//δʹ��
	static unsigned int speedChangeCnt[2][7]={1};
	
	/**********�˴�Ϊ�������ж�(((rGPFDAT >> 1) & 0x1)��֤���������ض���������ֵ��1)
			  signal!=((rGPFDAT >> 2) & 0x1 ������������������(������ͣ����ʱ��
			  ����ʽ�����������ʣ������ⲿ��������ɱ������ź�һֱ���ڣ����µ���˶�)
			  ���ַ�������ʡȥ���ж��źŵ���ʱ���룬��Ϊͨ��������B���жϡ�
			  ���Դ˷���ֻ������˫���ص��жϡ�
	**********/
	if (((rGPFDAT >> 1) & 0x1) && signal!=((rGPFDAT >> 2) & 0x1)){//Get_X_Value(2)
		signal=((rGPFDAT >> 2) & 0x1);//Get_X_Value(2)�����B���ź�
		encoder1_speed_pulse++;
		encoder1_pulse_number++;	//��������������¼
		
		/**���߹���**/
		if(tiaoxiankaiguan_kb == 1){//mode_choose == tiaoxian_mode
			for (zushu =0; zushu < tiaoxianzu; zushu++){
				
				if(tongxun_jiange_status[zushu] == 1){
						tongxun_jiange[zushu]++;
						//g_InteralMemory.Word[568] = tongxun_jiange[zushu];
				}
				
				for (jj = 0 ; jj < DAOSHU_MAX ; jj++){
					if (chudao_start[zushu][jj] == 1 && 
						chudao_start_status[zushu][jj] == 0){	//����������� by FJW
						chudao_jiange_tmp[zushu][jj] ++;
					}

					if (shoudao_start[zushu][jj] == 1 && 
						shoudao_start_status[zushu][jj] == 0){	//�յ�������� by FJW
						shoudao_jiange_tmp[zushu][jj] ++;
					}
					
					if(weisha_jiange_status[zushu][jj] == 1)
					{
						weisha_jiange[zushu][jj]++;
					}
					
					
					
				}	
			}
		}
		
		/**��7������Ϊ���������ν����жϣ��Լ�Сÿ��ѭ������(�ϰ벿��)**/
		for (jj=0;jj<4;jj++)
		{
			motor_factor[jj] += k_motor[jj];
			
			/***����4�������ƽ(�������ߣ����忴ԭ��ͼ�����޸Ĺ�)***/
			if (motor_factor[jj]>=1000000)
			{
				rGPEDAT &= ~(1<<jj);
				motor_factor[jj] -= 1000000;
				songsha_num[jj]++;			//shachang_xianshi()�е��ã�����ɴ����ʾ
			}
			
			/***����3�������ƽ(�������ͣ����忴ԭ��ͼ�����޸Ĺ�)��
				�����Ż�ռ�ձ�(�����������Ӧʱ���䳤һЩ������һ������֮�ڲ�Ӱ��)
			***/
			if (jj != 3 && (motor_factor[jj + 4] >= 500000 || k_motor[jj + 4] >= 500000)){
				rGPEDAT |= (1<<(jj + 4));
			}
		}
		//rGPEDAT |= (0x7 << 4);
		/********7������Kֵ�ı仯֮�󣬴�K_Current��α仯��->K_Target;
				����ĸı���Speed_Change()�����
		**********/
		for (jj = 0; jj < 7; jj ++){
			if (speedUpFlag[jj]==1){
				if (huanchongmaichong!=0&&speedUpCnt[jj]<speedUpMax&&
				++speedChangeCnt[0][jj]%huanchongmaichong==0)
					speedUpCnt[jj]++;
				/***��ͨ�����ٴﵽK_Target��ֱ�Ӵ�K_Current->K_Target***/
				else if (huanchongmaichong==0){
					speedUpCnt[jj]=speedUpMax;
					speedUpFlag[jj]= 0;
					speedChangeCnt[0][jj] = 1;
				}
				/***���ٴ�������֮�󣬽�Flag��0��speedChangeCnt��1***/
				else if (speedUpCnt[jj] >= speedUpMax){
					speedUpFlag[jj]=0;
					speedChangeCnt[0][jj] = 1;
				}	
			}
			
			else{
				speedChangeCnt[0][jj] = 1;
			}
		}
	}
	
	/**********�˴�Ϊ�½����ж�(((rGPFDAT >> 1) & 0x1)��֤���������ض���������ֵ��1)
			  signal!=((rGPFDAT >> 2) & 0x1 ������������������(������ͣ����ʱ��
			  ����ʽ�����������ʣ������ⲿ��������ɱ������ź�һֱ���ڣ����µ���˶�)
			  ���ַ�������ʡȥ���ж��źŵ���ʱ���룬��Ϊͨ��������B���жϡ�
			  ���Դ˷���ֻ������˫���ص��жϡ�
	**********/
	else if(signal!=((rGPFDAT >> 2) & 0x1)){//Get_X_Value(2)
		signal=((rGPFDAT >> 2) & 0x1);//Get_X_Value(2)�����B���ź�
		
		/**��7������Ϊ���������ν����жϣ��Լ�Сÿ��ѭ������(�°벿��)**/
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
		
		/**************************************************************/
		//rGPEDAT |= (0xf);
		
		
		rWTCNT = reset_time_kw;	//wdt_feed_dog();
		
		/*********����תһȦ֮������Ҫ�ı�־λ�Լ�Ȧ��++*********/
		if (encoder1_pulse_number>=encoder1_cal_factor){
			dapan_round++;
			encoder1_pulse_number=0;
			qingche_num_kw++;
			total_quanshu++;								//�����ܼ���
			if (total_quanshu>65530){
				xitong_total_num_upper_kw++;
				total_quanshu=0;
			}
			
			/***youbeng_sys_fun()�е���***/
			if (youbeng_quan_init_flag==1)	//�ͱ�Ȧ��Ъ:Ȧ��++
				youbeng_quanjianxie_yizhuan_num++;
			
			/***fenshan_sys_fun()�е���***/
			if (fenshan_quan_init_flag==1)
				fenshan_jianxie_yizhuanquan_num++;
			
			/**���߹���**/
			if (tiaoxianzu_flag == 1){
				tiaoxianzu_quanshu++;
				jiajiaStatus = 0;
			}
				
		}
		
		/**�϶�ɴ��������5�����嶼������ô��ͣ��**/
		if ( ((rGPFDAT >> 7) & 0x1) == alarm_signal[shangduansha_port] &&//Get_X_Value(shangduansha_port)
		    sys_force_run_button == 0 && 
			shangduansha_alarm_level!=level_0){
			error_times++;
			if (error_times >= 10){
				rGPBDAT &= ~(1<<3); //Y3��0
				rGPBDAT |= 1<<1;	//Y1��1
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
		
	
	/*******WDT���֣���ֹwhile(1)ѭ���Ѿ������ˣ������жϷ������ȴ����ʹ��,��ʱҪ����Ƶ��ֹͣ*******/
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
/**************************************************************************************************/		
		
		/*********Speed_Change()�н��ٽ׶Σ����������ж���Գ�*********/
		for (jj = 0; jj < 7; jj ++){
			if (speedDownFlag[jj]==1){
				if (huanchongmaichong!=0&&speedDownCnt[jj]<speedDownMax&&
				++speedChangeCnt[1][jj]%huanchongmaichong==0)
					speedDownCnt[jj]++;
				else if (huanchongmaichong==0){
					speedDownCnt[jj]=speedDownMax;
					speedDownFlag[jj]=0;
					speedChangeCnt[1][jj] = 1;
				}
				else if (speedDownCnt[jj] >= speedDownMax){
					speedDownFlag[jj]=0;
					speedChangeCnt[1][jj] = 1;
				}
			}
			else{
				speedChangeCnt[1][jj] = 1;
			}
				
		}
	}
	rEINTPEND=(1<<1); 
	ClearPending((U32)BIT_EINT1);
}


