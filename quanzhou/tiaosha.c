#include "includes.h"
/******************���߿���ִ�п�***************/

unsigned int Ports_state[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int tongxunzhen_Ports_state[ZUSHU_MAX] = {0x0000};
unsigned int tongxun_kaishi = 0;


unsigned int chudao_start_status[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int shoudao_start_status[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int chudao_start[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int shoudao_start[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int chudao_shoudao_status[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int kaiguan[ZUSHU_MAX][DAOSHU_MAX] = {0x00};			//�ⲿͨѶ֡�ľ���ʵ��	
unsigned int shinengwei[DAOSHU_MAX]={0};	
unsigned int tongxunzhen[ZUSHU_MAX] = {0x0000};
unsigned int tongxunstart[ZUSHU_MAX] = {0};
unsigned int chudao_jiange_tmp[ZUSHU_MAX][DAOSHU_MAX] = {0};		//���������¼ by FJW
unsigned int shoudao_jiange_tmp[ZUSHU_MAX][DAOSHU_MAX] = {0};	//�յ������¼ by FJW
unsigned int chudao_shoudao_start[ZUSHU_MAX][DAOSHU_MAX] = {0};
//unsigned int tiaoxianzu = 0;
//unsigned int jiajiaStatus = 0;

unsigned int weisha_jiange_status[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int weisha_jiange[ZUSHU_MAX][DAOSHU_MAX] = {0};
unsigned int tongxun_jiange_status[ZUSHU_MAX] = {0};
unsigned int tongxun_jiange[ZUSHU_MAX] = {0};
unsigned int tongxun_permmit[ZUSHU_MAX] = {0};

unsigned int shoudao_tozero_status[ZUSHU_MAX]={0};	//���߹��㸴λ��־
unsigned int shoudao_reset_flag = 0;
unsigned int tongxunnum[6] = {0};

//unsigned int kaishi;


TIAOXIANDUAN tiaoxianduan[tiaoshaduan_max];

/*************************************************
Function(��������): tiaoxian_init(void)
Description(�������ܡ����ܵȵ�����): ���߹��ܳ�ʼ��
Calls (�����������õĺ����嵥): tiaoxian_jidianqi_write(unsigned int zushu);
Called By (���ñ������ĺ����嵥): App_Init();

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void tiaoxian_init(void)	//���߳�ʼ�� by FJW
{
	int ii,bb;
	
	//д��8·ͨѶ֡���ⲿ�����Ӧ��״̬
	if (tiaoxiankaiguan_kb ==1 ){
		for (ii = 0 ;ii<ZUSHU_MAX;ii++){
			tongxunzhen[ii] = 0x0;//��ʼ���̵�������
			/******************���߿���ִ�п�***************/
			tongxunzhen_Ports_state[ii] = 0x0;
		}
		
		for (bb = 0 ; bb < ZUSHU_MAX; bb++){
			for (ii = 0 ; ii < 5 ; ii++){
				//ͨѶ�ɹ�֮���ֱ���˳�ѭ��,5��Ϊ�ݴ�
				if (jidianqi_write_card(bb) == 1){//tiaoxian_jidianqi_write(bb) 
					break;
				}
			}
		}
	}
	
	/***8�β�����ʼ��***/
	for (ii = 0; ii < tiaoshaduan_max; ii++){
		tiaoxianduan[ii].kaishiquanshu = &g_InteralMemory.KeepWord[156 + 10*ii];
		tiaoxianduan[ii].jieshuquanshu = &g_InteralMemory.KeepWord[157 + 10*ii];
		tiaoxianduan[ii].channal_choose= &g_InteralMemory.KeepWord[158 + 10*ii];
		tiaoxianduan[ii].ewaiduan_choose=&g_InteralMemory.KeepBit[49 + ii];
	
		/**7·����Ŵ���**/
		for (bb = 0; bb<7; bb++){
			tiaoxianduan[ii].fangdabeishu[bb] = &g_InteralMemory.KeepWord[159 + 10*ii + bb];
		}
	}
}

/*************************************************
Function(��������): tiaoxian_reset(void)
Description(�������ܡ����ܵȵ�����): ���߹��ܸ�λ
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
void tiaoxian_reset(void){
	int ii,bb;
	/* for (ii = 0 ;ii<ZUSHU_MAX;ii++){
		tongxunzhen[ii] = 0x0;
	}
	
	for (bb = 0 ; bb < ZUSHU_MAX; bb++){
		for (ii = 0 ; ii < 5 ; ii++){
			//ͨѶ�ɹ�֮���ֱ���˳�ѭ����5���ݴ�
			if (tiaoxian_jidianqi_write(bb) == 1){
				break;
			}
		}
	} */
	
	/*****************6�鵶�ߣ�6�ѵ�ȫ����λ********************/
	for (bb =0; bb<ZUSHU_MAX;bb++){
		
		/*****ʹ��λ��ͨѶ֡��ͨѶ��ʼ�ź�*****/
		//shinengwei[bb]=0;					//�˴�ʹ��λӦ���뵶����Ӧ�����������͵�����ͬ���������������
		tongxunzhen[bb] = 0x0;
		tongxunstart[bb] = 0;
		
		/*******���߳����յ����ָ�λ,��ɴ������λ*******/
		for (ii=0;ii<DAOSHU_MAX;ii++){
			chudao_start_status[bb][ii] = 0;
			shoudao_start_status[bb][ii] = 0;
			chudao_start[bb][ii] = 0;
			shoudao_start[bb][ii] = 0;
			chudao_shoudao_status[bb][ii] = 0;
			kaiguan[bb][ii] = 0x00;	
			chudao_shoudao_start[bb][ii] = 0;
			chudao_jiange_tmp[bb][ii] = 0;		
			shoudao_jiange_tmp[bb][ii] = 0;	
			
			/******************���߿���ִ�п�***************/
			Ports_state[bb][ii]=0;
			
			shinengwei[ii]=0;					//�����������
			
			
			weisha_jiange_status[bb][ii] = 0;
			weisha_jiange[bb][ii] = 0;
			tongxun_jiange_status[bb] = 0;
			tongxun_jiange[bb] = 0;
			tongxun_permmit[bb] = 0;
			
			if (chudao_shoudao_status[bb][ii] == 1){//ιɴ�������㣬��Ҫ�����ĲŽ�shoudao_tozero_status[bb]��1
				shoudao_tozero_status[bb]= 1;
			}	
		}
	}
	/****************���߼��Ȧ��******************/
	/* if (tiaoxianzu_jiange != 0){
		tiaoxianzu = 1;
	}
	else
		tiaoxianzu = tiaoxianzu_max; */
	
	for (ii = 0 ;ii<ZUSHU_MAX;ii++){
		tongxunzhen[ii] = 0x0;
		tongxunzhen_Ports_state[ii] = 0x0;
		jidianqi_write_card(ii);
		
	}

	shoudao_reset_flag = 1;
	tiaoxianzu = tiaoxianzu_max;
	tiaoxianzu_flag = 0;
	tiaoxianzu_quanshu=0;
//	kaishi=0;
}

/*************************************************
Function(��������): between_check(unsigned int roundShineng)
Description(�������ܡ����ܵȵ�����): ��Ҫ����shinengpanduan()�������жϵ�ǰ�׶��Ƿ����ڵ���Ȧ��֮��
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): songsha_fre_change(void);		shinengpanduan(void);

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): roundShineng��������ʹ��(����dapan_round)
Output(�����������˵��):
Return: i:��ǰ�ڵ��ߵĵ�i��
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
int between_check(unsigned int roundShineng){
	int i;
	
	//check������Ҫ���ڵ��߹���,��δ�򿪵��߹��ܣ���ֱ�ӷ���-1��
	if (tiaoxiankaiguan_kb == 0)
		return -1;
	
	for(i=0;i<tiaoshaduan_max;i++){
		
		/***����������1.���ڿ�ʼȦ��
					2.����Ȧ��Ϊ0��ʾ�ر�
					3.С�ڽ���Ȧ�����ߵ������һȦ
		***/
		if (roundShineng >= *tiaoxianduan[i].kaishiquanshu			
		&& *tiaoxianduan[i].jieshuquanshu							
		&& (roundShineng < *tiaoxianduan[i].jieshuquanshu			
		|| 	*tiaoxianduan[i].jieshuquanshu == 
			(daduanquanshu + middlequanshu + xiaoduanquanshu + caijiaoquanshu + langfeiquanshu))){
			
			/***�ж��Ƿ��Ƕ���Σ����ⲿ�����Ƿ��***/
			if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
				return i;
			}
			else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
				return i;
			}
		}
	}
	return -1;
}
/*************************************************
Function(��������): unsigned int at_check(unsigned int roundShineng)
Description(�������ܡ����ܵȵ�����): ��Ҫ���ڱ�Ƶ���ı��٣���Ҫ��bianpingqi_speed_cal()������
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): bianpingqi_speed_cal(void)

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): roundShineng��dapan_roundֵ
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
int weisha_check(unsigned int roundShineng){
	int current_duan;
	int next_duan;
	int current_duan_weizhi;
	int next_duan_weizhi;
	int current_weizhi_tmp = 0;
	int next_weizhi_tmp = 0;
	int i;
	static unsigned int roundShineng_use = 0;
	
	if(tiaoxianzu_flag == 0){
		roundShineng_use = roundShineng;
	}
	current_duan = between_check(roundShineng_use);
	next_duan = between_check(roundShineng_use+1);
	
	if(current_duan != next_duan && (current_duan != -1) && (next_duan != -1)){
		current_duan_weizhi = tiqushuzi(*tiaoxianduan[current_duan].channal_choose);
		next_duan_weizhi = tiqushuzi(*tiaoxianduan[next_duan].channal_choose);
	
		for(i=0;i<DAOSHU_MAX;i++){
			current_weizhi_tmp = (current_duan_weizhi & 0x01);
			next_weizhi_tmp = (next_duan_weizhi & 0x01);
			if((current_weizhi_tmp^next_weizhi_tmp) && (current_weizhi_tmp == 0)){
				return WEISHAJIANSU;
			}
			current_duan_weizhi = (current_duan_weizhi >>1);
			next_duan_weizhi = (next_duan_weizhi >>1);
		}
		return WEISHABUJIANSU;
	}
	if (current_duan == -1){
		return WEISHAJIANSU;
	}
	else if (next_duan == -1){
		return WEISHABUJIANSU;
	}
	return WEISHAJIANSU;
}

/*************************************************
Function(��������): unsigned int at_check(unsigned int roundShineng)
Description(�������ܡ����ܵȵ�����): ��Ҫ���ڱ�Ƶ���ı��٣���Ҫ��bianpingqi_speed_cal()������
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): bianpingqi_speed_cal(void)

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): roundShineng��dapan_roundֵ
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
unsigned int at_check(unsigned int roundShineng){
	int i;
	
	/***��������(λ��)��1.������Ȧ����=0����encoder1_process()��++
					  2.��������Ȧ����=0
					  3.������Ȧ��%��������Ȧ��:���Ȧ���ı���
					  4.jiajiaStatus == 0
		�˴�������ҪΪ���жϵ�ǰ�Ƿ��������Ҫ++���жϵڶ��� �����Ƿ���Ҫ�����յ���
		�˴�ֻ�����������ӣ�������return;
	***/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 &&
		tiaoxianzu_quanshu % tiaoxianzu_jiange ==0 && jiajiaStatus == 0)
	{
		jiajiaStatus = 1;
		if ( tiaoxianzu < tiaoxianzu_max ){
			tiaoxianzu++;		
		}
	}
	
	/***��������Ȧ��Ϊ0���򲻷��飬��ͬ�����յ�***/
	else if (tiaoxianzu_jiange == 0){
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
		tiaoxianzu = tiaoxianzu_max;
	}
	
	/*****���߹�����ɣ�flag��status��λ*****/
	if (tiaoxianzu_quanshu != 0 && tiaoxianzu_jiange != 0 && 
		tiaoxianzu >= tiaoxianzu_max && 
		tiaoxianzu_quanshu >= ((tiaoxianzu_max -1)*tiaoxianzu_jiange + 1))
	{
		tiaoxianzu_flag = 0;
		tiaoxianzu_quanshu=0;
		jiajiaStatus = 0;
	}
	
	
		
	/****	��������:1.������Ȧ��������0 || ������������1
					2.������flag����1���ҵ�������������0
		
			�˴������Ӧ�ж��Ƿ��˵�n����ߵ�ʱ����ǰһȦ����(ͨ��return 1;����������):
			if ((at_check((dapan_round+1)) && encoder1_pulse_number >= (encoder1_cal_factor - jiajiansujiangemaichong_kw)))
			
			�ж��Ƿ�ǰȦ�Ƿ���Ҫ����������:
			if ((at_check((dapan_round)) && encoder1_pulse_number < jiajiansujiangemaichong_kw))
	****/
	if ((tiaoxianzu_quanshu != 0 || tiaoxianzu_jiange == 1) && 
		 tiaoxianzu_flag == 1 && tiaoxianzu_jiange != 0)
	{
		
		if (roundShineng == (dapan_round + 1) && 
		   ((tiaoxianzu_quanshu+1) % tiaoxianzu_jiange ==0 )) {
			return 1;
		}
		
		if (roundShineng == (dapan_round ) && 
		   ((tiaoxianzu_quanshu) % tiaoxianzu_jiange ==0 )) {
			return 1;
		}
	}
	
	
	/****���߰˶��٣��˴�����Ϊ��1��Ŀ�ʼ�ͽ��������൶�������жϾ�Ϊ������������ж�****/
	for(i=0;i<tiaoshaduan_max;i++){
		/*****ֻҪ�������ߣ����Ƚ������º���******/
		if (*tiaoxianduan[i].jieshuquanshu)//��Ӧ���޲�Ϊ��
		{											
			/****Ϊ�˱�֤�ڿ�ʼȦ��Ϊ0ʱ����������ٽ��е���(����Ҫ�����֮ǰ��Ҫ���٣�0Ȧ��ʼ���ߣ���������һȦ��ʼ)
			****/
			if (*tiaoxianduan[i].kaishiquanshu == 0
			    && roundShineng == (daduanquanshu+middlequanshu+xiaoduanquanshu+
									caijiaoquanshu+langfeiquanshu))
			{
					/**************************��һ���׶�Ϊ����Σ������ⲿ����ѡ������************************************/
					if (getStage(current_stage,NEXTSTAGE) == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
					/**************************��һ���׶β�Ϊ����Σ������ⲿ���ز�ѡ������************************************/
					else if (getStage(current_stage,NEXTSTAGE) != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
						if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
							tiaoxianzu = 1;
							tiaoxianzu_flag = 1;
						}
						return 1;
					}
			}//���ifΪ�˱�֤�ڿ�ʼȦ��Ϊ0ʱ����������ٽ��е��ߡ�
			
			/**********************************���߿�ʼ********************************************/
			else if (roundShineng == *tiaoxianduan[i].kaishiquanshu){						//��������
				if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 1;
				}
				else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 1;
				}
			}
			
			/*********************************���߽���*******************************************/
			if (roundShineng == *tiaoxianduan[i].jieshuquanshu){						//��������

				if (current_stage == ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 2;
				}
				else if (current_stage != ewaiduan && *tiaoxianduan[i].ewaiduan_choose == choose_not_ewaiduan){
					if (roundShineng == dapan_round && tiaoxianzu_flag != 1 && tiaoxianzu_jiange != 0){
						tiaoxianzu = 1;	
						tiaoxianzu_flag = 1;
					}
					return 2;
				}
			}
		}
	}
	return 0;
}

/*************************************************
Function(��������): tiqushuzi(INT16U channal_choose)
Description(�������ܡ����ܵȵ�����): ��ȡ�ⲿ����ѡ��ģ����ⲿ����7��weizhi = 0xff
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): channal_choose���ⲿ�����������Ϊ1234 
																	  ���ʾѡ��1234���鵶��
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
unsigned int tiqushuzi(INT16U channal_choose){
	int i;
	unsigned int wei[5];
	unsigned int weizhi = 0x00;	//��Чλ�Ļ�ֵ
	
	/****�ⲿ����ѡ��Ľ׶����ֵ���ȡ****/
	if (channal_choose != 7){
		wei[0] = channal_choose / 10000;
		wei[1] = (channal_choose - wei[0]*10000) / 1000; 
		wei[2] = (channal_choose - wei[0]*10000 - wei[1]*1000) / 100;
		wei[3] = (channal_choose - wei[0]*10000 - wei[1]*1000- wei[2]*100) /10;
		wei[4] = channal_choose % 10;
		for (i = 0 ; i <5 ; i++){
			switch (wei[i]){
				case 1:
					weizhi |= 1<<0;
					break;
				case 2:
					weizhi |= 1<<1;
					break;
				case 3:
					weizhi |= 1<<2;
					break;
				case 4:
					weizhi |= 1<<3;
					break;
				case 5:
					weizhi |= 1<<4;
					break;
				case 6:
					weizhi |= 1<<5;
					break;
				default:
					break;
			}
		}
	}
	/****�ⲿ����ѡ��Ľ׶����ֵ���ȡ���ⲿѡ��7��ʱ��weizhi = 0xff****/
	else{
		weizhi = 0xff;
	}
	return weizhi;
}

/*************************************************
Function(��������): shinengpanduan(void)
Description(�������ܡ����ܵȵ�����): 6�ѵ���ʹ���ж�
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
void shinengpanduan(void){
	int i;
	unsigned int weizhi = 0x00;
	for (i = 0 ; i <DAOSHU_MAX ; i++){
		shinengwei[i] = 0;
	}
	i = between_check(dapan_round);
	at_check(dapan_round);
	/**��ȡ**/
	if ( i != -1 && current_stage != caijianduan){
		weizhi = tiqushuzi(*tiaoxianduan[i].channal_choose);
		for (i = 0 ; i <DAOSHU_MAX ; i++){
			if ( (weizhi>>i) & 0x01){
				shinengwei[i] = 1;
			}
		}
	}
}

/*************************************************
Function(��������): tiaoxian(void)
Description(�������ܡ����ܵȵ�����): ���߹��ܴ���
Calls (�����������õĺ����嵥): shinengpanduan();
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void tiaoxian(void)
{
	int i;
	int j=0;
	unsigned int zushu;
	shinengpanduan();	//�жϵ�i�ѵ��ߵ�ʹ��
	
	g_InteralMemory.Word[568] = weisha_jiange[0][0];
	/**************��tiaoxianzu_flag=1ʱ��tioaxianzu_quanshu++;*************************************************/
	for (zushu =0; zushu < tiaoxianzu; zushu++){
		
		/***�ж�6�ѵ����Ƿ�ʹ��***/
		for (i = 0; i<DAOSHU_MAX ; i++){
			
			/***********ʹ��λ==1֮������ͨѶ֡������ͨѶ��ʼ��־λ�������յ���ʼ��־��
											  ιɴ�Ĺ��ոı�Ϊ����֮���һ�������ջ���
			*********/
						/***********ʹ��λ==0֮��(���öβ���Ҫ���ߣ���ô��Ҫ�ѵ��ջ���)��
					   ���ó����յ�������ͨѶ��ʼ��־λ�������յ���ʼ��־
			*********/
			if((shinengwei[i] == 0) && (chudao_shoudao_status[zushu][i] == 1)){
				
				chudao_shoudao_process(i,zushu);					//�����յ�
				tongxunzhen[zushu] &= (~(3<< (i*2)));				//����
				tongxunzhen[zushu] |= (kaiguan[zushu][i] << (i*2)); //����
				
				/******************���߿���ִ�п�***************/
				tongxunzhen_Ports_state[zushu] &=(~(1 << (i)));
				tongxunzhen_Ports_state[zushu] |=(Ports_state[zushu][i] << (i));	
				
				chudao_shoudao_start[zushu][i] = 0; 
				
				///***18.11.15***//
				tongxun_jiange_status[zushu] = 0;
				
				tongxun_kaishi = 1;
				//continue;
			}
			else if (shinengwei[i] == 1 && (weisha_jiange_kw !=0 )){//&& tongxun_permmit[zushu] == 0
				
				tongxun_jiange_status[zushu] = 1;
			   
				if((tongxun_jiange[zushu] >= i*QUANSHU_FACTOR)
				&& (tongxun_jiange[zushu] < (i+1)*QUANSHU_FACTOR)){
					
				if(chudao_shoudao_start[zushu][i] == 0){
					weisha(i,zushu,ON);

					tongxunzhen[zushu] &= (~(3<< (i*2)));				//����
					tongxunzhen[zushu] |= (kaiguan[zushu][i] << (i*2));	//����
					
					/******************���߿���ִ�п�***************/
					tongxunzhen_Ports_state[zushu] &=(~(1 << (i)));
					tongxunzhen_Ports_state[zushu] |=(Ports_state[zushu][i] << (i));
		
					jidianqi_write_card(zushu);
					
					Beep(1);
					Delay(HCJ_SB_TIME);
					Beep(0);
					weisha_jiange_status[zushu][i] = 1;
					
				}
				else if((weisha_jiange[zushu][i] >=weisha_jiange_kw && weisha_jiange[zushu][i] < 2*weisha_jiange_kw)){
					weisha(i,zushu,MIDDLE);
					
					tongxunzhen[zushu] &= (~(3<< (i*2)));				//����
					tongxunzhen[zushu] |= (kaiguan[zushu][i] << (i*2));	//����
					
					
					/******************���߿���ִ�п�***************/
					tongxunzhen_Ports_state[zushu] &=(~(1 << (i)));
					tongxunzhen_Ports_state[zushu] |=(Ports_state[zushu][i] << (i));
					
					jidianqi_write_card(zushu);	
					
					Beep(1);
					Delay(HCJ_SB_TIME);
					Beep(0);
				}
				
				else if(weisha_jiange[zushu][i] >= 3*weisha_jiange_kw){
					weisha(i,zushu,OFF);
					weisha_jiange_status[zushu][i] = 0;
					weisha_jiange[zushu][i] = 0;	
					tongxun_jiange_status[zushu] = 0;
					
					tongxunzhen[zushu] &= (~(3<< (i*2)));				//����
					tongxunzhen[zushu] |= (kaiguan[zushu][i] << (i*2));	//����
					
					/******************���߿���ִ�п�***************/
					tongxunzhen_Ports_state[zushu] &=(~(1 << (i)));
					tongxunzhen_Ports_state[zushu] |=(Ports_state[zushu][i] << (i));
						
					jidianqi_write_card(zushu);
					
					Beep(1);
					Delay(HCJ_SB_TIME);
					Beep(0);
				}
				
				chudao_shoudao_status[zushu][i] = 1;
				chudao_shoudao_start[zushu][i] = 1;
				
				tongxunstart[zushu] = 1;
				tongxunnum[zushu]=0;

				}
			}

		}
		/***ͨѶ��ʼ***/
			if (tongxunstart[zushu] == 1 && tongxunnum[zushu] <5 && tongxun_kaishi == 1){//&& kaishi<20 shinengwei[i] == 0
			
				static int i = 0;
				if(i >= 1)
					g_InteralMemory.Word[567] = 12345;
				i++;
				
				
				
				/***5��ͨѶ�ݴ�***/
				if (jidianqi_write_card(zushu) != 1){
					tongxunnum[zushu] ++ ;
				}
				else{
						tongxunnum[zushu] = 0;
						tongxunstart[zushu] = 0;
				}	
			
				tongxun_kaishi = 0;
			}
				

	}
	
	if (shoudao_reset_flag == 1){
		for (j=0;j<ZUSHU_MAX;j++){
			if (shoudao_tozero_status[j] != 0)
				return;
		}
		if (j >= ZUSHU_MAX){
			if (tiaoxianzu_jiange != 0){
				tiaoxianzu = 1;
			}
			else
				tiaoxianzu = tiaoxianzu_max;
			shoudao_reset_flag = 0;
		}
	}
	
}

/*************************************************
Function(��������): tiaoxian_jidianqi_write(unsigned int zushu)
Description(�������ܡ����ܵȵ�����): ͨѶд����߼̵�����ֵ
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): zushu���ⲿ���������tongxunzhen[zushu]���ܴ���
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
unsigned int tiaoxian_jidianqi_write(unsigned int zushu)
{
	U8 Count,jdqCheck,waitTime;
	int i;
	U32 ErrorLoop;
	
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./57600+0.5) -1 );		//��ͨѶ����Ҫ57600�Ĳ����ʲ��ܹ���
#define	TIAOXIAN_DEBUG	0
#if TIAOXIAN_DEBUG
	U8 auchMsg[10],SendArray[10],RecArray[10];  
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	/* if (rULCON3!=0x2B) 
		rULCON3 =0x2B; 		 */			//0x2b=00 101 0 11  ��ͨ żУ�飨even�� 1ֹͣλ 8����λ
	
	switch (zushu){
	case 0:
		auchMsg[0]=25;
		break;
	}
	
	
	auchMsg[1]=0x0F;	//������:0x0F��ʾд�����Ȧ
	                    
	auchMsg[2]=0x00;    //��ʼ��ַH
	auchMsg[3]=0x00;    //��ʼ��ַL
	                    
	auchMsg[4]=0x00;    //�������H
	auchMsg[5]=0x08;    //�������L
	                    
	auchMsg[6]=0x01;    //�ֽ������������/8 ����������0��+1(�ֽ���Ϊ1���·�Ϊһ���ֽ�;Ϊ2��Ϊ�����ֽ�)
	                   
	auchMsg[7]=tongxunzhen[zushu] & 0xff;	//���������Ϊ�ֽ���Ϊ1�����Դ˴�ֻ��Ҫ��8λ
	
	auchMsg[8]=(CRC(auchMsg,8)) & 0xff;
	auchMsg[9]=(CRC(auchMsg,8))>>8;
	for (i=0;i<10;i++)
	{
		SendArray[i]=auchMsg[i];
	}
	
#else
	U8 auchMsg[11],SendArray[11],RecArray[11];  
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	if (rULCON3!=0x03) 
		rULCON3 =0x03; 				//0x03=00 000 0 11  ��ͨ żУ�飨even�� 1ֹͣλ 8����λ
	
	switch (zushu){
	case 0:
		auchMsg[0]=25;break;
	case 1:
		auchMsg[0]=26;break;
	case 2:
		auchMsg[0]=27;break;
	case 3:
		auchMsg[0]=28;break;
	case 4:
		auchMsg[0]=29;break;
	case 5:
		auchMsg[0]=30;break;	
	}
	
	auchMsg[1]=0x0F;		//������:0x0F��ʾд�����Ȧ
	
	auchMsg[2]=0x00;		//��ʼ��ַH
	auchMsg[3]=0x00;		//��ʼ��ַL
	
	auchMsg[4]=0x00;		//�������H
	auchMsg[5]=0x10;		//�������L
	
	auchMsg[6]=0x02;		//�ֽ������������/8 ����������0��+1(�ֽ���Ϊ1���·�Ϊһ���ֽ�;Ϊ2��Ϊ�����ֽ�)
	
	auchMsg[7]=tongxunzhen[zushu]&0xff;		//���ֵ1���ⲿ���һ��16λ����Ӧ�ĵ�8λֵ
	auchMsg[8]=(tongxunzhen[zushu] >> 8);	//���ֵ2���ⲿ���һ��16λ����Ӧ�ĸ�8λֵ
	
	auchMsg[9]=(CRC(auchMsg,9)) & 0xff;
	auchMsg[10]=(CRC(auchMsg,9))>>8;
	for (i=0;i<11;i++)
	{
		SendArray[i]=auchMsg[i];
	}
#endif


	rGPHDAT |= 0x1000;	//GPH12	+Write
	Delay(DELAY_TIME_RTU);
#if TIAOXIAN_DEBUG
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
	
	/***************ͨѶЭ��485 Read********************/
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
			
			//rUBRDIV3 = ( (int)(SYS_PCLK/16./9600+0.5) -1 );	//��������֮ǰ�������ʻ���ȥ
			return 1;		//10��ͨѶ֡����3��ƥ��ͷ���1
		}
		else{
			//rUBRDIV3 = ( (int)(SYS_PCLK/16./9600+0.5) -1 );	//��������֮ǰ�������ʻ���ȥ
			return 0;
		}	
	}
	else
		{
			//rUBRDIV3 = ( (int)(SYS_PCLK/16./9600+0.5) -1 );	//��������֮ǰ�������ʻ���ȥ
			return 0;
		}
}


/*************************************************
Function(��������): chudao_shoudao_process(unsigned int i,unsigned int zushu)
Description(�������ܡ����ܵȵ�����): �����յ����̣������Ĺ���Ҫ���¿��Ʒ�����Ҫ����˵����
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ):  i->6�ѵ���;	zushu->8��?
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void chudao_shoudao_process(unsigned int i,unsigned int zushu)
{
	/****���գ�1.�ⲿιɴ���Ϊ0:ֱ��ִ��ԭ������
			  2.�ⲿιɴ�����Ϊ0:weisha_jiange_statusΪ0; weisha_jiange[6][6]��ʱ=0;
	****/
	if(weisha_jiange_kw !=0){
		if(weisha_jiange_status[zushu][i] != 1 && weisha_jiange[zushu][i] == 0){
			kaiguan[zushu][i] = 0x01;		//(0b) 01
			tongxunstart[zushu] = 1;
			tongxunnum[zushu] = 0;
			weisha_jiange_status[zushu][i] = 1;
			
			/******************���߿���ִ�п�***************/
			Ports_state[zushu][i] = WEISHA_1_JIANSHA_1st;//WEISHA_State_None
		}
		else if(weisha_jiange[zushu][i]<weisha_jiange_kw){
			return;
		}
		else 
			weisha_jiange_status[zushu][i] = 0;
	}
	/**********��һ����kaiguan[zushu][i] = 0x00����ͨѶ����*********/
	if(chudao_start[zushu][i] == 0 && shoudao_start[zushu][i] == 0)
	{
		if(kaiguan[zushu][i] != 0x03){
			kaiguan[zushu][i] = 0x03;		//(0b) 11
			tongxunstart[zushu] = 1;
			tongxunnum[zushu] = 0;
			
			/******************���߿���ִ�п�***************/
			Ports_state[zushu][i] = WEISHA_0_JIANSHA_1st;//WEISHA_State
		}
		
	}
	
	/****************?????????????????**************************/
	if(chudao_start_status[zushu][i] == 0)
		chudao_start[zushu][i] = 1;
	
	/**********�ڶ�����kaiguan[zushu][i] = 0x01����ͨѶ����*********/
	if(chudao_jiange_tmp[zushu][i] >= chudao_jiange)
	{
		if(kaiguan[zushu][i] != 0x02)	{
			kaiguan[zushu][i] = 0x02;		//(0b)10
			tongxunstart[zushu] = 1;
			tongxunnum[zushu] = 0;
			
			/******************���߿���ִ�п�***************/
			Ports_state[zushu][i] = WEISHA_0_JIANSHA_2nd;//JIANDAO_State
				
		}
		
		chudao_start_status[zushu][i] = 1;
		chudao_start[zushu][i] = 0;
		chudao_jiange_tmp[zushu][i] = 0;
		
		shoudao_start[zushu][i] = 1;
	}

	/**********��������kaiguan[zushu][i] = 0x03����ͨѶ����*********/
	if(shoudao_jiange_tmp[zushu][i] >= shoudao_jiange)
	{
		if(kaiguan[zushu][i] != 0x00){
			kaiguan[zushu][i] = 0x00;		//(0b) 00
			tongxunstart[zushu] = 1;
			tongxunnum[zushu] = 0;
			
			/******************���߿���ִ�п�***************/
			Ports_state[zushu][i] = WEISHA_0_JIANSHA_3rd;	//SONGSHA_State
		}
		
		shoudao_start_status[zushu][i] = 1;
		shoudao_start[zushu][i] = 0;
		shoudao_jiange_tmp[zushu][i] = 0;

		chudao_shoudao_status[zushu][i] = 0;
		weisha_jiange[zushu][i] = 0;
		if (shoudao_tozero_status[zushu] == 1){
			shoudao_tozero_status[zushu]=0;
		}
		
	}
}



/*************************************************
Function(��������): weisha(unsigned int i,unsigned int zushu,unsigned int on_off)
Description(�������ܡ����ܵȵ�����): ιɴ����Ҫ������kaiguan[zushu][i] = 0x02;��������(0b)10
Calls (�����������õĺ����嵥): 
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): i->6�ѵ���;	zushu->8��?
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void weisha(unsigned int i,unsigned int zushu,unsigned int on_off)
{
	if(on_off == ON){
		kaiguan[zushu][i] = 0x01;		//(0b) 01
		/******************���߿���ִ�п�***************/
		Ports_state[zushu][i] = WEISHA_1_State;//WEISHA_State
	}
		
	else if(on_off == MIDDLE){
		Beep(1);
		kaiguan[zushu][i] = 0x03;		//(0b) 01
		/******************���߿���ִ�п�***************/
		Ports_state[zushu][i] = WEISHA_1_State;//WEISHA_State
	}
	else{
		kaiguan[zushu][i] = 0x01;		//(0b) 00
		/******************���߿���ִ�п�***************/
		Ports_state[zushu][i] = WEISHA_1_State;//WEISHA_State
	}
	
		
	chudao_start[zushu][i] = 0;
	chudao_start_status[zushu][i] = 0;
	shoudao_start[zushu][i] = 0;
	shoudao_start_status[zushu][i]= 0;
	chudao_jiange_tmp[zushu][i] = 0;
	shoudao_jiange_tmp[zushu][i] = 0;
}

/*************************************************
Function(��������): jidianqi_write_tiaoxian(U8 which_port,U8 button_bit)
Description(�������ܡ����ܵȵ�����): �̵������ͨѶЭ��
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
int jidianqi_write_card(unsigned int zushu)
{
	U8 auchMsg[8],SendArray[8],RecArray[8];  
	U8 Count,jdqCheck,waitTime;
	int i;
	U32 ErrorLoop;
	ErrorLoop = ERROR_NUM*19200/g_SystemConf.BaudRates;
	
	//rUBRDIV3 = ( (int)(SYS_PCLK/16./57600+0.5) -1 );
	//Uart_Init(0,9600);
	if (rULCON3!=0x03) 
		rULCON3 =0x03; 								//0x03=00 000 0 011  ��ͨ ��У�� 1ֹͣλ 8����λ
	
	auchMsg[0]=0xA5;
	switch (zushu){
	case 0:
		auchMsg[1]=0xC4;break;
	case 1:
		auchMsg[1]=0x44;break;
	case 2:
		auchMsg[1]=0x84;break;
	case 3:
		auchMsg[1]=0x04;break;
	case 4:
		auchMsg[1]=0xF8;break;
	case 5:
		auchMsg[1]=0x78;break;	
	}
	
	auchMsg[2]=tongxunzhen[zushu] & 0xff;
	auchMsg[3]=(tongxunzhen[zushu] >> 8);
	auchMsg[4]=tongxunzhen_Ports_state[zushu];
	auchMsg[5]=0x00;
	for (i=0;i<6;i++)
	{
		SendArray[i]=auchMsg[i];
	}
	rGPHDAT |= 0x1000;	//GPH12	+Write
	Delay(DELAY_TIME_RTU);
	for (Count=0;Count<6;Count++)
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
	#if 0
	rGPHDAT &= 0xefff;	//GPH12	-Read
	Count = 0;
	/**20�ζ�ȡuart��ֵ������ֱֵ���˳�ѭ��**/
	while ((RecArray[0]=qz_Uart_Getch())!=0xA5 && Count<=20)
	{
		Count++;
	}
	if (RecArray[0]==0x5A){
		for (Count=1;Count<6;Count++)
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
	else{
			return 0;
		}
	#endif
	//Delay(90000);
	return 1;//����ͨѶһ��

}