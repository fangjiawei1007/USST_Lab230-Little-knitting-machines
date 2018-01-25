#include"includes.h"

#define	bianpingqi_cal_fac	2.5

unsigned int	bianpingqi_previous_speed=0;
U8 bianpingqi_previous_jog_speed=0;
U8 bianpingqi_jog_status=0;

U8 bianpingqi_run_flag;							//��Ƶ����ʼ����flag
U8 bianpingqi_previous_run_status=0;			//��Ƶ����ʼ����֮�󽫴���1
unsigned int bianpingqi_speed;
U8 bpqComCount=0;
unsigned int speed_status = 0;		//speed_status == 1 ��ʾ�Ѿ�����������֮����Ҫ����һȦ����

/*************************************************
Function(��������): bianpingqi_RTU_WriteWord(U8 function_num,int Value)
Description(�������ܡ����ܵȵ�����): ͨѶд��
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
void bianpingqi_RTU_WriteWord(U8 function_num,int Value)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 Count,waitTime; 
	unsigned int check=0;//cc,dd,
	int i;
	int DevAddress;
	U32 ErrorLoop;
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	
	/****ѡ��Ĵ�����ַ****/
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
		rULCON3 =0x2B; 								//0x2b=00 101 0 11  ��ͨ żУ�飨even�� 1ֹͣλ 8����λ
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
	rGPHDAT |= 0x1000;	//GPH12	+Write��ʹ��MAX485оƬ��Ҫ�ı�λ
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
	/**20�ζ�ȡuart��ֵ������ֱֵ���˳�ѭ��**/
	while((RecArray[0]=qz_Uart_Getch())!=bianpingqi_station_num && Count<=20) 
	{
		Count++;
	}
	
	/***ջ����ȷ֮��check�ж϶�����֡�Ƿ���4������ȷ�ģ�
		�������ȷ����ôֱ�ӽ�bianpingqi_previous_speed=bianpingqi_speed;���ⲿ������ͨѶ
		eg��if(bianpingqi_previous_speed!=speed)
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
	
	/***ջ��һֱ����ȷ��bpqComCount���ж�5�Σ�����ͨѶЭ������5�Σ�
		ֱ�ӽ�bianpingqi_previous_speed=bianpingqi_speed;�Ա�֤�ⲿ������ͨѶ
		eg��if(bianpingqi_previous_speed!=speed)
	***/
	else
		bpqComCount++;
	if (bpqComCount>=4){
		bianpingqi_previous_speed=bianpingqi_speed;
		bpqComCount=0;
	}
}


/*************************************************
Function(��������): bianpingqi_start_sub(void)
Description(�������ܡ����ܵȵ�����): GPIO���Ʊ�Ƶ����ʼ��������status��־״̬��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): bianpingqi_start();bianpingqi_jog();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void bianpingqi_start_sub(void)
{
	/***����ⲿTSѡ����Ʊ�Ƶ���������ʱ��Y3��1(��Ƶ��STR�˿�);Y1��0(��Ƶ��STF�˿�)***/
	if (Choose_bianpingqi_kb == CHOOSE_BIANPINGQI){
	rGPBDAT &= ~(1<<1);
	rGPBDAT |= 1<<3;
	}
	bianpingqi_previous_run_status=1;
}

/*************************************************
Function(��������): bianpingqi_stop_sub(void)
Description(�������ܡ����ܵȵ�����): GPIO���Ʊ�Ƶ����ʼ��
								 ������status��־״̬�Լ�bianpingqi_previous_speed��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): bianpingqi_start();bianpingqi_jog();__irq	encoder1_process();
							   bianpingqi_init();main();__irq DataSave_IntHandle(void);

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: �ú�������ε��ã�����Ҫע�������main��������������ˣ�
	    ���籣�洦__irq DataSave_IntHandle(void)�����ˡ�

Author:������
Modified:
Commented:����ΰ
*************************************************/
void bianpingqi_stop_sub(void)
{
	/***����ⲿTSѡ����Ʊ�Ƶ���������ʱ��Y3����(��Ƶ��STR�˿�);Y1��1(��Ƶ��STF�˿�)***/
	if (Choose_bianpingqi_kb == CHOOSE_BIANPINGQI){
		rGPBDAT &= ~(1<<3);		
		rGPBDAT |= 1<<1;
	}
	bianpingqi_previous_speed=0;
	bianpingqi_previous_run_status=0;
}

/*************************************************
Function(��������): bianpingqi_set_speed(unsigned int speed)
Description(�������ܡ����ܵȵ�����): 485д���Ƶ�����ٶ�ֵ
Calls (�����������õĺ����嵥): bianpingqi_RTU_WriteWord()
Called By (���ñ������ĺ����嵥): encoder1_data_process();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void bianpingqi_set_speed(unsigned int speed)
{
	if (bianpingqi_previous_speed!=speed&&bianpingqi_jog_status!=1&&Choose_bianpingqi_kb==CHOOSE_BIANPINGQI)
	{
		bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,speed);//������
	}
	else if (Choose_bianpingqi_kb==CHOOSE_NOT)
	{
		bianpingqi_previous_speed=bianpingqi_speed;
	}
}


/*************************************************
Function(��������): bianpingqi_start(U8 bianpingqi_run_button)
Description(�������ܡ����ܵȵ�����): ͨ���ⲿIO���Ʊ�Ƶ������ͣ��ͨ������Ĳ���bianpingqi_run_button
								  ����Ƶ���Ŀ���Ҫ���־λ��ͬ�������ͬ����иı�IO�ڣ�����ɻ��⡣
								  ���⼴���ⲿ�ı䣬�ڲ��ſ�ʼ�ı䡣
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
void bianpingqi_start(U8 bianpingqi_run_button)
{		
	if (bianpingqi_run_button==1&&bianpingqi_previous_run_status==0&&bianpingqi_jog_status==0)
	{
		bianpingqi_start_sub();
		//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,(int)(1<<2));				//��ת����
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
Function(��������): bianpingqi_jog(void)
Description(�������ܡ����ܵȵ�����): ��Ƶ���㶯
Calls (�����������õĺ����嵥): bianpingqi_RTU_WriteWord();bianpingqi_start_sub();
							 bianpingqi_stop_sub();
Called By (���ñ������ĺ����嵥): Main()

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void bianpingqi_jog(void)
{
	/**��������£����˼�ͣ���Ͳ�Ӧ���ٽ���(ext_stop_status!=press)**/
	if ((bianpingqi_run_flag==0 && ext_stop_status!=press) || 
		 qz_error_status==1)
	{
		/***�ⲿ��ť����֮�󣬿�ʼ�˶������Դ˴���ʹ��g_InteralMemory.Bit[n]***/
		if (bianpingqi_jog_button==1 || ext_jog_status==press)
		{
			/************��Ƶ��֮ǰδ�˶�����ʼ�˶�*************/
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
			
			/******�ж��Ƿ��ⲿ�д�����Ļ�����û�д����������ⲿ���£����Ƶ��ֹͣ******/
			if(Pen_status==Pen_status_up && ext_jog_status!=press)
			{
				bianpingqi_jog_status=0;
				bianpingqi_stop_sub();
				//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,0x0000);
				bianpingqi_jog_button=0;
			}
		}
		
		/****�ⲿ��ť�͵����㶯ֹͣ****/
		if (ext_jog_status!=press && bianpingqi_jog_status==1 && bianpingqi_jog_button==0)
		{
			bianpingqi_jog_status=0;
			bianpingqi_stop_sub();
			//bianpingqi_RTU_WriteWord(bianpingqi_write_fun_num,0x1001,0x0000);
			
		}
	}
	
	/****�ⲿ����ֹ֮ͣ�󣬾ͱ�Ƶ��ֹͣ****/
	else if (ext_stop_status==press && qz_error_status==0){
		bianpingqi_jog_status=0;
		bianpingqi_jog_button=0;
		bianpingqi_stop_sub();
	}
}

/*************************************************
Function(��������): bianpingqi_speed_cal(void)
Description(�������ܡ����ܵȵ�����): 1.���̼��٣���Ϊ��Ƶ���ǿ��ƴ��̵�
								2.��Ƶ���Ӽ��٣�
								3.�ֲ��/������зֲ�Σ�������Ҫ����->����һȦ->���ٵ�fencen_speed
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): songsha_fre_change();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void bianpingqi_speed_cal(void){
	static unsigned int y1y3delay_flag = 0;
	unsigned int quanshu[7]={0};	
	unsigned int next_stage = 0;
	unsigned int previous_stage = 0;
	
	/***********��Ƶ���Ӽ���************/
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

	//����
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
	
	/**quanshu[x+1]������Ĺ��ܴ����next_stage��ʵ�Ǳ�ʾ��current_stage����Ϊ��û��+1��
	   eg��quanshu[guoduduan] ��ʵΪ��ͷ�ε�Ȧ������quanshu[guoduduan+1]��ʾ���ɶ�Ȧ�� 
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

	
	//��ʹ�ý�����ֱ�Ӽ���
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
	
	/**�ֲ��/�����������Ҫ����öΣ����������� 0.speed_status == 1; ֻ������һ���Ƿֲ��/����μ���֮��Ż����˶�elseif
										    1.��ǰ���Ƕ����(��Сͷ��)���ڷֲ�εĵ�һȦ�ᱣ�ָö��ٶ�һȦ
										    2.��ǰ��Ϊ����Σ���������һȦ�ǽ���֮����Ҫһֱ����
										    3.��ǰ��Ϊ����Σ�����ε�һȦ����(dapan_round <1)(ע�⣺�����Ȧ�����¼���);
														    ����Ȧ��==����ֲ�Ȧ��;
														    ����Ȧ������(�������Ȧ��-����ηֲ�Ȧ)
	**/
	else if ( speed_status == 1 && 
			((current_stage != ewaiduan && dapan_round <(quanshu[current_stage]+1)) || 
			( current_stage == ewaiduan && dapan_round == quanshu[ewaiduan+1] ) ||//���һ������Ϊ�ڶ����������ʱ��dapan_round��ֵΪ��� 
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
	
	/**�ֲ��/�����֮ǰ��Ҫ����öΣ�����������1.��ǰ�׶���Сͷ�β�����һ���׶��Ƿֲ�Σ���Сͷ�ν���ǰ(Сͷ��Ȧ��-����Ȧ��)���н���
										    2.��ǰ�׶��Ƿֲ�Σ��ڷֲ�ν���ǰ(�ֲ�Ȧ��-����Ȧ��)���н���
										    3.��ǰ�׶��ǲü��β�����һ���׶��Ƕ���Σ��ڲü��ν���ǰ(�ü�����Ȧ��-����Ȧ��)���н���
										    4.��speed_status��1��ʹ�÷ֲ�ε��ٶ��ܹ�������һ��
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
	
	/**�������Ҫ����öΣ�����������1.��ǰ�׶ζ���Σ����Ҷ���εķֲ�ε�Ȧ����Ϊ0
								 2.��ǰ�׶��Ƿֲ�Σ��ڷֲ�ν���ǰ(�ֲ�Ȧ��-����Ȧ��)���н���
								 3.��ǰ�׶��ǲü��β�����һ���׶��Ƕ���Σ��ڲü��ν���ǰ(�ü�����Ȧ��-����Ȧ��)���н���
								 4.��speed_status��1��ʹ�÷ֲ�ε��ٶ��ܹ�������һ��
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
	
	//�����Ĵ����ٶȸı�Ľ׶Σ�bianpingqi_fencen_speed_set/bianpingqi_fullspeed_set(ȫ�ٻ��߷ֲ��ٶ�)
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