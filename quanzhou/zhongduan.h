#ifndef _ZHONGDUAN_H_
#define	_ZHONGDUAN_H_

#define alarm	0
#define	press	alarm

#define	level_3			8
#define	level_2			6
#define	level_1			4
#define	level_0			2

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	min_port						7
#define	max_port						12

#define	anquanmen2_port					6
#define shangduansha_port				7
#define	anquanmen_port					8
#define	queyou_port						9
#define	tanzhen_port					10
#define	xiaduansha_port					11
#define	tanbu_port						12

#define	ext_start_port					13
#define	ext_stop_port					14
#define	ext_jog_port					15

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	shangduansha_status				g_InteralMemory.Bit[37]			
#define	anquanmen_status				g_InteralMemory.Bit[38]
#define	queyou_status					g_InteralMemory.Bit[39]
#define	tanzhen_status					g_InteralMemory.Bit[41]
#define	xiaduansha_status				g_InteralMemory.Bit[42]
#define	tanbu_status					g_InteralMemory.Bit[43]

#define	ext_start_status				g_InteralMemory.Bit[44]		//Get_X_Value(ext_start_port);(�ⲿ����X13���ⲿ��ʼ�ź�)
#define	ext_stop_status					g_InteralMemory.Bit[45]		//Get_X_Value(ext_stop_port);(�ⲿ����X14���ⲿֹͣ�ź�)
#define	ext_jog_status					g_InteralMemory.Bit[46]		//Get_X_Value(ext_jog_port);(�ⲿ����X15���ⲿ�㶯�ź�)
                                                                     

#define	shangduansha_alarm_level		g_InteralMemory.KeepWord[95]	//��ɴ������������
#define	anquanmen_alarm_level			g_InteralMemory.KeepWord[96]	//ȱ�ͱ�����������
#define	queyou_alarm_level				g_InteralMemory.KeepWord[97]	//��ȫ�ű�����������
#define	tanzhen_alarm_level				g_InteralMemory.KeepWord[98]	//̽�뱨����������
#define	tanbu_alarm_level				g_InteralMemory.KeepWord[100]	//̽��������������
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define jishu_max_flag					g_InteralMemory.Bit[15]

#define	shangduansha_alarm_sig_kb		g_InteralMemory.KeepBit[28]		
#define	anquanmen_alarm_sig_kb			g_InteralMemory.KeepBit[29]
#define	queyou_alarm_sig_kb				g_InteralMemory.KeepBit[30]
#define	tanzhen_alarm_sig_kb			g_InteralMemory.KeepBit[31]
#define	tanbu_alarm_sig_kb				g_InteralMemory.KeepBit[32]

#define	previous_error_status_w			g_InteralMemory.Word[43]
#define	qingche_num_kw					g_InteralMemory.KeepWord[132]

void zhongduan_fun(void);

void alarm_disp(U8 which_alarm);

void beep_alarm(void);











#endif


