#ifndef _TIAOSHA_H_
#define _TIAOSHA_H_

#define tiaoxiankaiguan_kb				(g_InteralMemory.KeepBit[48])	//��ͷ���߿���
#define guoduquanshu_tiaoxian			(g_InteralMemory.KeepBit[49])	//�������߿���
#define xiaotouquanshu_tiaoxian			(g_InteralMemory.KeepBit[50])	//Сͷ���߿���
#define fencengquanshu_tiaoxian			(g_InteralMemory.KeepBit[51])	//�ֲ����߿���

#define chudao_jiange					(g_InteralMemory.KeepWord[151]) //�������
#define shoudao_jiange					(g_InteralMemory.KeepWord[152]) //�������

#define	tiaoxian_12_enter			(g_InteralMemory.KeepWord[156])
#define	tiaoxian_12_exit			(g_InteralMemory.KeepWord[157])
#define	tiaoxian_34_enter			(g_InteralMemory.KeepWord[158])
#define	tiaoxian_34_exit			(g_InteralMemory.KeepWord[159])

#define Y9								9
#define Y10								10

void shinengpanduan(void);
void tiaoxian(void);
void chudao_shoudao_process(unsigned int i);
void weisha(unsigned int i);
unsigned int tiaoxian_jidianqi_write(void);
#endif

