#ifndef _PRESSING_H_
#define _PRESSING_H_

#define yazheng_button 						(g_InteralMemory.KeepBit[40])			//��Ȧѹ��
#define qingling_button 					(g_InteralMemory.KeepBit[41])			//���㰴ť
#define lingwei_jiance_button 				(g_InteralMemory.KeepBit[42])			//��λ��ⰴť
#define huiling_zijian_button 				(g_InteralMemory.KeepBit[43])			//�����Լ찴ť
#define positive_button						(g_InteralMemory.KeepBit[44])			//�ֶ���������
#define negative_button						(g_InteralMemory.KeepBit[45])			//�ֶ����ڷ���
//#define yazheng_direction					(g_InteralMemory.KeepBit[46])			//����ѹ�롢����ѹ��


#define yazheng_depth						(g_InteralMemory.KeepWord[141])			//ѹ�����
#define yazheng_max_deapth					(g_InteralMemory.KeepWord[142])			//ѹ��������
#define huiling_length						(g_InteralMemory.KeepWord[143])			//�����Լ��Ƴ�����
#define yazheng_pulse_total_remember		(g_InteralMemory.KeepWord[144])			//���Ƕ�(����total)
#define jiaodu_monitor						(g_InteralMemory.KeepWord[145])			//�ǶȲ���
#define motor_dpi							(g_InteralMemory.KeepWord[146])			//�ֱ���
#define multiply							(g_InteralMemory.KeepWord[147])			//����

#define yazheng_pulse_send					(g_InteralMemory.KeepWord[148])			//ѹ���ܹ���Ҫ����������
#define yazheng_pulse_number				(g_InteralMemory.KeepWord[149])			//ϸ����������

#define	pressing_director_port				10
#define	pressing_pulse_port					9

#define pressing_get_zero_times				(g_InteralMemory.KeepWord[151])			//�����Լ���������ڿ����ò��������ť
void pressing(void);
void pressing_get_zero(void);
#endif