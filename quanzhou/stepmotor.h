#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_


#define	stepmotor_port					4~8	





#define	datou_rate_different_1					g_InteralMemory.KeepWord[8]		//�²�����ɰ��_��ͷ��
#define	datou_rate_different_2					g_InteralMemory.KeepWord[9]     //�ϲ�����ɰ��_��ͷ��
#define	datou_rate_different_3					g_InteralMemory.KeepWord[10]    //�²�С��ɰ��_��ͷ��
#define	datou_rate_different_4					g_InteralMemory.KeepWord[11]    //�ϲ�����ɰ��_��ͷ��
#define	datou_rate_different_5					g_InteralMemory.KeepWord[12]    //����_��ͷ��
                                                                                
#define	datou_rate_different_6					g_InteralMemory.KeepWord[140]   //�����ɰ��_��ͷ��
#define	datou_rate_different_7					g_InteralMemory.KeepWord[141]   //������ɰ��_��ͷ��
                                                                           

#define	xiaotou_rate_different_1				g_InteralMemory.KeepWord[39]	//�²�����ɰ��_Сͷ��
#define	xiaotou_rate_different_2				g_InteralMemory.KeepWord[40]	//�ϲ�����ɰ��_Сͷ��
#define	xiaotou_rate_different_3				g_InteralMemory.KeepWord[41]	//�²�С��ɰ��_Сͷ��
#define	xiaotou_rate_different_4				g_InteralMemory.KeepWord[42]	//�ϲ�����ɰ��_Сͷ��
#define	xiaotou_rate_different_5				g_InteralMemory.KeepWord[43]	//����_Сͷ��
	
#define	xiaotou_rate_different_6				g_InteralMemory.KeepWord[142]	//�����ɰ��_Сͷ��
#define	xiaotou_rate_different_7				g_InteralMemory.KeepWord[143]	//������ɰ��_Сͷ��


#define	fencen_rate_different_1					g_InteralMemory.KeepWord[45]	//�²�����ɰ��_�ֲ��
#define	fencen_rate_different_2					g_InteralMemory.KeepWord[46]    //�ϲ�����ɰ��_�ֲ��
#define	fencen_rate_different_3					g_InteralMemory.KeepWord[47]    //�²�С��ɰ��_�ֲ��
#define	fencen_rate_different_4					g_InteralMemory.KeepWord[48]    //�ϲ�����ɰ��_�ֲ��
#define	fencen_rate_different_5					g_InteralMemory.KeepWord[49]    //����_�ֲ��
                                                                                
#define	fencen_rate_different_6					g_InteralMemory.KeepWord[144]   //�����ɰ��_�ֲ��
#define	fencen_rate_different_7					g_InteralMemory.KeepWord[145]   //������ɰ��_�ֲ��
                                                                           
#define	shachang_shuru_xianshi_1_w					g_InteralMemory.Word[65]	//�²�����ɰ��_�·�����ֵ
#define	shachang_shuru_xianshi_2_w					g_InteralMemory.Word[66]    //�ϲ�����ɰ��_�·�����ֵ
#define	shachang_shuru_xianshi_3_w					g_InteralMemory.Word[67]    //�²�С��ɰ��_�·�����ֵ
#define	shachang_shuru_xianshi_4_w					g_InteralMemory.Word[68]    //�ϲ�����ɰ��_�·�����ֵ
                                                                           
#define	dapan_zongzhenshu_kw						g_InteralMemory.KeepWord[139]	//����������

void qz_assert(void);
void alarm_disp_assert(void);



#endif


