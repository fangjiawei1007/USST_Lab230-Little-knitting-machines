#include"includes.h"
U32 encoder1_speed_pulse=0;						//用于计算speed的脉冲数
U32	encoder1_speed=0;							//用于计算speed的脉冲数(大盘速度)的tmp值

volatile unsigned int k_motor[7]={0};			//电机K值存放的数组(实际)
unsigned int kMotorTarget[7]={0};
volatile unsigned int motor_factor[7]={0};		//用于和1000000比较，仅仅是一个中间变量
unsigned int dapan_round=0;						//大盘转了多少圈的监测值
U16	encoder1_pulse_number=0;					//编码器发出的脉冲数计算

const float k_factor[7][3]={{364,519,364},{249,309,249},{369,399,369},{264,291.6,264},{328.5,312.1,328.5},{1000,1000,1000},{1000,1000,1000}};
unsigned char jiansu_permite=1;					//大盘超速了之后减速
unsigned char signal;							//读取X2的状态值
unsigned char speedUpFlag[7]={0};
unsigned char speedDownFlag[7]={0};

unsigned char jianshu_ewaiduan_check=0;
unsigned char reset_timer_start=0;
unsigned int speedUpCnt[7]={1};
unsigned int speedDownCnt[7]={1};
unsigned int forceEqual = 1;		//作用：送纱电机变速的时候强制相等
									//不通过变速
unsigned int forceDownEqual = 0;
unsigned int forceUpEqual = 0;

unsigned int ewaiduan_fencen_status = 0;		//ewaiduan_fencen_status=1时，表示使用额外段的分层

#define	reset_time_100ms				4

/*************************************************
Function(函数名称): encoder1_data_process(void)
Description(函数功能、性能等的描述): 当大盘速度超过设定的最大速度，通过通讯调整变频器，使大盘降速
Calls (被本函数调用的函数清单): delay_qz(U8 delay_rank,U32 delay_t,U8 status);bianpingqi_set_speed(U32 speed)
Called By (调用本函数的函数清单): main()

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void encoder1_data_process(void){
	yuanpan_speed=encoder1_speed*600/encoder1_cal_factor;	//计算大盘速度
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
Function(函数名称): encoder1_data_reset(void)
Description(函数功能、性能等的描述):参数清零;清零主要是为了将编码器的记录值清零，
								并且将dapan_round清零，这样songsha_freq_change就可以重新工作
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
		//lingwei_jiance_button = 1; //压针回零 by FJW
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
		//lingwei_jiance_button = 1;		//压针回零 by FJW
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
Function(函数名称): SpeedChange(const unsigned int* kMotor)
Description(函数功能、性能等的描述): 保证各个阶段的速度变化不突变，通过前后两个阶段的K值的变化来比较
								 K值改变了之后，那么就会进入此函数，进行speed_up_MAX段速度的改变.
								 过渡段/裁剪段的变速为K值的改变，与dapan_round有关，和此处无关。
								 (songsha_fre_change)大盘转一圈，外部目标K值改变。
								 (SpeedChange)K_Motor值改变，在此处会进行多段的k_motor值->k_Motor。
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input: const unsigned int* kMotor
	   kMotor为外部传参，K值数组
Output(对输出参数的说明):
Return: 
Others: 第一个作用：上电直接将K值设定到位，不通过变速实现 2018.01.07
	    第二个作用：改变第一次K值
	    第三个作用(类似第一个)：强制降速
	    第四个作用：在外部强制改变K值的情况下，重新进行变速实现。
		
		区别变速段与speed_Change段的变速
Author:王德铭
Modified:新增局部变量k_motor_cal[7],用于计算中间变量，提升_irq encoder1_process()中k_motor的使用效率 by FJW 2018.1.10
Commented:方佳伟
*************************************************/
void SpeedChange(const unsigned int* kMotor){
	static unsigned int previousKMotor[7]={0};
	static unsigned int previousKMotorCompare[7]={0};
	static unsigned int k_motor_cal[7]={0};				//作为计算值k_motor_cal[7]的中间变量
	
	int i;
	
	//上电第一次强制将速度加到K值，不通过变速执行
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
	
	//七路电机第一次变化k值，主要是为了将speedDownFlag[]/speedUpFlag[]置1，
	//在_irq encoder1_process()中,进行多次变换(irq中speedDownFlag[])
	else{
		for ( i = 0; i<7 ; i++){
			
			//speedUpMax次加速/减速结束以后，speedUpFlag[n]/speedDownFlag[n]置0，
			//speedUpCnt[n]/speedDownCnt[n]重置
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
			
			//作为如果在变化完成之前，若改变了目标速度，则从当前速度重新从1,2,3开始变化到目标速度
			if (previousKMotorCompare[i] != kMotor[i]){ 
				speedUpCnt[i] = 1;
				speedDownCnt[i] = 1;
				speedDownFlag[i] = 0;
				speedUpFlag[i] = 0;
				previousKMotorCompare[i] = kMotor[i];
				previousKMotor[i] = k_motor[i];
			}
			
			//变速之后，flag置1，第一次变速开始
			if (previousKMotor[i] < kMotor[i]){
				//forceUpEqual未使用
				if (forceUpEqual == 1){
					k_motor_cal[i] = kMotor[i];
					previousKMotor[i] = kMotor[i];
					previousKMotorCompare[i] = kMotor[i];
				}
				
				else{
					/**算式使用中间变量更为合理，因为k_motor为中断中直接使用的值；by FJW**/
					k_motor_cal[i] = (previousKMotor[i] + ( kMotor[i] - previousKMotor[i] )*(int)speedUpCnt[i]/speedUpMax);
					
					/**当计算的k值与实际的k值不一样时再去进行变换 by FJW**/
					/* if(k_motor_cal[i] != k_motor[i])
					{
						k_motor[i] = k_motor_cal[i];
					} */
					
					speedUpFlag[i] = 1;
					speedDownFlag[i] = 0;
				}
			}
			else if (previousKMotor[i] > kMotor[i]){
				//强制降速，外部过渡段会使用
				if (forceDownEqual == 1){
					k_motor_cal[i] = kMotor[i];
					previousKMotor[i] = kMotor[i];
					previousKMotorCompare[i] = kMotor[i];
				}
				else{
					/**算式使用中间变量更为合理，因为k_motor为中断中直接使用的值；by FJW**/
					k_motor_cal[i] = (previousKMotor[i] - ( previousKMotor[i] - kMotor[i] )*(int)speedDownCnt[i]/speedDownMax);
					
					/**当计算的k值与实际的k值不一样时再去进行变换 by FJW**/
					/* if(k_motor_cal[i] != k_motor[i])
					{
						k_motor[i] = k_motor_cal[i];
					} */
					
					speedDownFlag[i] = 1;
					speedUpFlag[i] = 0;
				}
			}
			
			//容错设置
			else if (k_motor_cal[i] != kMotor[i]){
				k_motor_cal[i] = kMotor[i];
			}
		}
		forceUpEqual = 0;
		forceDownEqual = 0;
	}
	
	/**将中间判断的值放到，最后判断是否k值有改变，如果有改变则改变其k值**/
	for(i = 0;i<7;i++)
	{
		if(k_motor_cal[i] != k_motor[i])
		{
			k_motor[i] = k_motor_cal[i];
		}
	}
}

/*************************************************
Function(函数名称): getStage(void)
Description(函数功能、性能等的描述): 获得需要的stage，用于判断前后所需要的k值，
								  以消除前后速度变化过大问题
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): bianpingqi_speed_cal();at_check();getKMotor();

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): stage：所需要的阶段的基础阶段
													    direction：所需要的基础阶段的变化阶段
Output(对输出参数的说明):
Return: requestStage(所需要得到的stage)
Others: 本函数的思想是：
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
unsigned int getStage(const unsigned int stage,int direction){
	int requestStage = datouduan;
	unsigned char validRound = 0x00;
	
	//获取当前stage值
	if (direction == CURRENT){
		return stage;
	}
	
	/******************获取有效段数，判断哪些段是有效的(额外段较为特殊)****************/
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
	
	//额外段较特殊，会有两种情况
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


/**********判断是否该段为有效段，若为有效段则返回该段，
		   若为无效段，则返回下一个段(取决于Previous/Next)************/
	for(requestStage=((int)stage + direction);;requestStage += direction){
		
		//两个边界
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
Function(函数名称): getKMotor(void)
Description(函数功能、性能等的描述): 获取各个电机的跟踪系数(7路电机)
Calls (被本函数调用的函数清单): getStage(stage,direction)
Called By (调用本函数的函数清单): songsha_fre_change();
Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): bb:第bb个电机；
															stage：当前所处的段数；
															direction：可填参数CURRENT;PREVIOUS;NEXTSTAGE
															其中后两个参数是给调用的函数getStage(stage,direction)所用
Output(对输出参数的说明):
Return: k_factor[bb][x]*rate_different[bb][x]————即蔡老师所说的K值
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
int getKMotor(const unsigned char bb,const unsigned int stage,int direction){
	int requestStage = 0;
	
	requestStage = getStage(stage,direction);
	
	//如果需要的stage是额外段，那么返回大头段的速度，因为
	if (requestStage == ewaiduan){
		return (k_factor[bb][datou]*rate_different[bb][datou]);
	}
	
	//在各个段获得所需要的k值
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
	
	/**此处为过渡段或者裁剪段的功能，由于此二段为变速运动，
	   所以必须知道其开始速度以及结束速度，此处for循环，
	   必须在过渡段和裁剪段使用PREVIOUS||NEXT，不然会死循环
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
Function(函数名称): songsha_fre_change(void)
Description(函数功能、性能等的描述): 1.K值的变化
								 2.Speed_change(),使得k_current->k_target不突变
								 3.大盘的速度的改变(变频器)
Calls (被本函数调用的函数清单): wdt_feed_dog();between_check();
							getKMotor(const unsigned char bb,const unsigned int stage,int direction);
							SpeedChange(const unsigned int* kMotor);bianpingqi_speed_cal();
Called By (调用本函数的函数清单): stepmotor_init();KeyBoardScan();void Main(void)

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void songsha_fre_change(void){
	unsigned char bb;
	int i;
	wdt_feed_dog();main_enter_flag = 1;
	//所有圈数放在g_InteralMemory.Word[31]中
	g_InteralMemory.Word[31]=daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu;
	
	//容错：将大头圈数一定要设置
	if (daduanquanshu == 0 && xiaoduanquanshu == 0 && caijiaoquanshu == 0)
		daduanquanshu=100;
	
	/***重要：额外段进入条件：额外段圈数不为0，额外段件数不为0，已织件数不为0，
			 已织件数%额外段需要件数==0，标志位为0
	***/
	if (extra_part_quanshu!=0 && extra_part_jiansu!=0 && jianshu!=0 && \
	   jianshu%extra_part_jiansu==0 && extra_part_finish_flag==extra_part_finish){
		extra_part_flag=extra_part_start;
		extra_part_finish_flag=extra_part_unfinish;
	}
	
	//额外段工作时，上限到达||(额外段件数==0)||(额外段圈数==0)的情况下，停止使用额外段
	if (extra_part_flag!=extra_part_stop && (jianshu==0 || extra_part_jiansu==0 || extra_part_quanshu==0))
	{
		dapan_round=0;
		extra_part_finish_flag=extra_part_finish;
		extra_part_flag=extra_part_stop;
	}
	
	//不进行额外段的工作之后，为正常的工作状态
	if (extra_part_flag==extra_part_stop)
	{
		extra_part_finish_flag=extra_part_unfinish;
		ewaiduan_fencen_status = 0;
		
	//当前段为大头段
		if (dapan_round<daduanquanshu || daduanquanshu == 9999){
			current_stage=datouduan;
			i = between_check(dapan_round);
			//i ！= -1 为调线的功能代码
			if ( i != -1){
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT)*(*tiaoxianduan[i].fangdabeishu[bb])/100;
				}
			}
			//else 为无调线工作代码，获得大头段的各个K值
			else{
				for (bb=0;bb<7;bb++){
					kMotorTarget[bb]=getKMotor(bb,current_stage,CURRENT);
				}
			}
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		
	//当前段为过渡段
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
			
					/**K值计算 K_need = (K1 - ((K1-K2)*x)/y))**/
					kMotorTarget[bb]=	(getKMotor(bb,current_stage,PREVIOUSSTAGE)-
										(((getKMotor(bb,current_stage,PREVIOUSSTAGE)-getKMotor(bb,current_stage,NEXTSTAGE))
										*(int)(dapan_round-daduanquanshu))/middlequanshu)); 
				}
			}
			
			forceDownEqual=1;			//强制降到n段减速的第1个阶段的K值
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		
	//当前段为小头段
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
		
	//当前段为分层段
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
		
	//当前段为裁剪段
		else if (dapan_round<(daduanquanshu+middlequanshu+xiaoduanquanshu+caijiaoquanshu+langfeiquanshu)){
			current_stage=caijianduan;
			for (bb=0;bb<7;bb++){//裁剪圈不调线
				//裁剪段的初值和小头段的工况相同，和分层段的K值不同，
				kMotorTarget[bb] = (getKMotor(bb,fencenduan,PREVIOUSSTAGE)+
								   (((getKMotor(bb,current_stage,NEXTSTAGE)-getKMotor(bb,fencenduan,PREVIOUSSTAGE))
									*(int)(dapan_round-daduanquanshu-middlequanshu-xiaoduanquanshu-caijiaoquanshu))/langfeiquanshu));
			}
			SpeedChange(kMotorTarget);
			bianpingqi_speed_cal();
		}
		
		else
		{
			jianshu++;										//正常作用
			*banci[jianshu_no]=*banci[jianshu_no]+1;		//每班次件数增加，改变班次后，自动改变banci指向的KeepWord
			if (banci_status_kw!=s_ban)
				dingdan_lianghua_num_kw++;					//订单量化计数，只有在改变订单号后会清零,无班次时不增加班次订单计数
			dapan_round=0;
			extra_part_finish_flag=extra_part_finish;		//额外段也结束
		}
	}
	
	//以下均为挡片段(额外段)，额外段开始的时候，dapan_round已经清零
	else{
		current_stage=ewaiduan;	
		i = between_check(dapan_round);
		
		//额外段分层部分：主要作用ewaiduan_fencen_status = 1;，在bianpingqi_speed_cal()中使用
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
		
		//额外段不分层部分
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
		
		//额外段运行结束之后的置位
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
Function(函数名称): parameter_save(void)
Description(函数功能、性能等的描述): 将变量存入flash中kw,encoder1_pulse_number，dapan_round
								  songsha1_num[0~4]
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): menu_init();main();

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
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
		WritePage(Block,0,PageBuff);	// 暂时只用第0页（2K）保存数据
		PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];
		WritePage(Block,1,PageBuff);	// 暂时只用第1页（2K）保存数据
	}
	
}

/*************************************************
Function(函数名称): parameter_read(void)
Description(函数功能、性能等的描述): 读取flash中kw的变量,变量为：encoder1_pulse_number_save，dapan_round_save，
								  songsha1_num，songsha2_num，songsha3_num，songsha4_num
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): stepmotor_init();

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
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
Function(函数名称): __irq	encoder1_process(void)
Description(函数功能、性能等的描述): 编码器中断函数(电机运动的关键，通过编码器实现速度环控制)
Calls (被本函数调用的函数清单): 
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 中断函数不应该过多，会导致中断函数无法执行完，而导致未知结果的重大错误。
	   所以应该避免任何中断函数的开销，具体表现为：浮点型运算的开销以及函数调用的开销；
        循环次数(时间复杂度);判断条件的开销也在考虑范围之内。
																		——by FJW
Author:王德铭
Modified:
Commented:方佳伟
*************************************************/
void __irq	encoder1_process(void)
{
	unsigned int jj,zushu;//,signal;
	static unsigned int error_times=0;
	static unsigned int reset_enter_times=0;	//未使用
	static unsigned int speedChangeCnt[2][7]={1};
	
	/**********此处为上升沿中断(((rGPFDAT >> 1) & 0x1)保证了其上升沿读出来的数值是1)
			  signal!=((rGPFDAT >> 2) & 0x1 此条件用来进行消抖(具体是停机的时候，
			  增量式编码器的性质，导致外部抖动会造成编码器信号一直存在，导致电机运动)
			  此种方法可以省去对中断信号的延时进入，因为通过编码器B相判断。
			  所以此方法只能用于双边沿的中断。
	**********/
	if (((rGPFDAT >> 1) & 0x1) && signal!=((rGPFDAT >> 2) & 0x1)){//Get_X_Value(2)
		signal=((rGPFDAT >> 2) & 0x1);//Get_X_Value(2)，获得B相信号
		encoder1_speed_pulse++;
		encoder1_pulse_number++;	//编码器脉冲数记录
		
		/**调线功能**/
		if(tiaoxiankaiguan_kb == 1){//mode_choose == tiaoxian_mode
			for (zushu =0; zushu < tiaoxianzu; zushu++){
				
				if(tongxun_jiange_status[zushu] == 1){
						tongxun_jiange[zushu]++;
						//g_InteralMemory.Word[568] = tongxun_jiange[zushu];
				}
				
				for (jj = 0 ; jj < DAOSHU_MAX ; jj++){
					if (chudao_start[zushu][jj] == 1 && 
						chudao_start_status[zushu][jj] == 0){	//出刀间隔计算 by FJW
						chudao_jiange_tmp[zushu][jj] ++;
					}

					if (shoudao_start[zushu][jj] == 1 && 
						shoudao_start_status[zushu][jj] == 0){	//收刀间隔计算 by FJW
						shoudao_jiange_tmp[zushu][jj] ++;
					}
					
					if(weisha_jiange_status[zushu][jj] == 1)
					{
						weisha_jiange[zushu][jj]++;
					}
					
					
					
				}	
			}
		}
		
		/**将7组电机分为上下沿两次进行判断，以减小每次循环次数(上半部分)**/
		for (jj=0;jj<4;jj++)
		{
			motor_factor[jj] += k_motor[jj];
			
			/***拉低4个电机电平(或者拉高，具体看原理图，有修改过)***/
			if (motor_factor[jj]>=1000000)
			{
				rGPEDAT &= ~(1<<jj);
				motor_factor[jj] -= 1000000;
				songsha_num[jj]++;			//shachang_xianshi()中调用，用于纱长显示
			}
			
			/***拉高3个电机电平(或者拉低，具体看原理图，有修改过)，
				并且优化占空比(步进电机的响应时间会变长一些，但是一个周期之内不影响)
			***/
			if (jj != 3 && (motor_factor[jj + 4] >= 500000 || k_motor[jj + 4] >= 500000)){
				rGPEDAT |= (1<<(jj + 4));
			}
		}
		//rGPEDAT |= (0x7 << 4);
		/********7组电机的K值的变化之后，从K_Current多次变化到->K_Target;
				具体的改变在Speed_Change()中求解
		**********/
		for (jj = 0; jj < 7; jj ++){
			if (speedUpFlag[jj]==1){
				if (huanchongmaichong!=0&&speedUpCnt[jj]<speedUpMax&&
				++speedChangeCnt[0][jj]%huanchongmaichong==0)
					speedUpCnt[jj]++;
				/***不通过变速达到K_Target，直接从K_Current->K_Target***/
				else if (huanchongmaichong==0){
					speedUpCnt[jj]=speedUpMax;
					speedUpFlag[jj]= 0;
					speedChangeCnt[0][jj] = 1;
				}
				/***加速次数到达之后，将Flag置0，speedChangeCnt置1***/
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
	
	/**********此处为下降沿中断(((rGPFDAT >> 1) & 0x1)保证了其上升沿读出来的数值是1)
			  signal!=((rGPFDAT >> 2) & 0x1 此条件用来进行消抖(具体是停机的时候，
			  增量式编码器的性质，导致外部抖动会造成编码器信号一直存在，导致电机运动)
			  此种方法可以省去对中断信号的延时进入，因为通过编码器B相判断。
			  所以此方法只能用于双边沿的中断。
	**********/
	else if(signal!=((rGPFDAT >> 2) & 0x1)){//Get_X_Value(2)
		signal=((rGPFDAT >> 2) & 0x1);//Get_X_Value(2)，获得B相信号
		
		/**将7组电机分为上下沿两次进行判断，以减小每次循环次数(下半部分)**/
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
		
		/*********大盘转一圈之后所需要的标志位以及圈数++*********/
		if (encoder1_pulse_number>=encoder1_cal_factor){
			dapan_round++;
			encoder1_pulse_number=0;
			qingche_num_kw++;
			total_quanshu++;								//用作总计数
			if (total_quanshu>65530){
				xitong_total_num_upper_kw++;
				total_quanshu=0;
			}
			
			/***youbeng_sys_fun()中调用***/
			if (youbeng_quan_init_flag==1)	//油泵圈间歇:圈数++
				youbeng_quanjianxie_yizhuan_num++;
			
			/***fenshan_sys_fun()中调用***/
			if (fenshan_quan_init_flag==1)
				fenshan_jianxie_yizhuanquan_num++;
			
			/**调线功能**/
			if (tiaoxianzu_flag == 1){
				tiaoxianzu_quanshu++;
				jiajiaStatus = 0;
			}
				
		}
		
		/**上断纱错误连着5个脉冲都错误，那么就停机**/
		if ( ((rGPFDAT >> 7) & 0x1) == alarm_signal[shangduansha_port] &&//Get_X_Value(shangduansha_port)
		    sys_force_run_button == 0 && 
			shangduansha_alarm_level!=level_0){
			error_times++;
			if (error_times >= 10){
				rGPBDAT &= ~(1<<3); //Y3置0
				rGPBDAT |= 1<<1;	//Y1置1
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
		
	
	/*******WDT部分，防止while(1)循环已经不进了，但是中断服务程序却还在使用,此时要将变频器停止*******/
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
/**************************************************************************************************/		
		
		/*********Speed_Change()中降速阶段，与上升沿中断相对称*********/
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


