#ifndef _TIAOSHA_H_
#define _TIAOSHA_H_


#ifdef TIAOSHA_NORMAL_EN

#define tiaoxiankaiguan_kb				(g_InteralMemory.KeepBit[48])	//��ͷ���߿���
#define	tiaoxianzu_flag					(g_InteralMemory.KeepBit[26])	//�����鿪ʼ��¼ʵ�����˶���Ȧ�ı�־λ

#define chudao_jiange					(g_InteralMemory.KeepWord[151]) //�������
#define shoudao_jiange					(g_InteralMemory.KeepWord[152]) //�յ����

#define	tiaoxianzu_jiange				g_InteralMemory.KeepWord[99]	//���߼��Ȧ��
#define	tiaoxianzu_max					g_InteralMemory.KeepWord[61]	//��������
#define	tiaoxianzu						g_InteralMemory.KeepWord[93]	//��¼��ǰΪ�ڼ���
#define	tiaoxianzu_quanshu				g_InteralMemory.KeepWord[62]	//��¼������ʵ�����˶���Ȧ

#define Y9								9
#define Y10								10

#define	tiaoshaduan_max					8
#define	choose_ewaiduan					1
#define	choose_not_ewaiduan				0

#define ZUSHU_MAX						6
#define DAOSHU_MAX						6
/***ιɴ��� 2018.1.24***/
#define weisha_jiange_kw				g_InteralMemory.KeepWord[460]	//ιɴ���

/*************************/
typedef struct{
	INT16U* kaishiquanshu;				//���߿�ʼȦ��
	INT16U* jieshuquanshu;				//���߽���Ȧ��
	INT16U* channal_choose;				//ͨ����ѡ��(ѡ��ڼ��鵶(DAOSHU))
	unsigned char* ewaiduan_choose;		//�����ѡ��
	INT16U* fangdabeishu[7];			//�Ŵ��������ߵ�ʱ�򣬸���������ٶȲ�һ��(�ٷֱ�)
}TIAOXIANDUAN;

enum TIAOXIAN_MACRO{
	ON,
	OFF,
	WEISHAJIANSU,
	WEISHABUJIANSU
};


void shinengpanduan(void);
void tiaoxian(void);
void chudao_shoudao_process(unsigned int i,unsigned int zushu);
void weisha(unsigned int i,unsigned int zushu,unsigned int on_off);
unsigned int tiaoxian_jidianqi_write(unsigned int zhushu);
int between_check(unsigned int roundShineng);
unsigned int at_check(unsigned int roundShineng);
void tiaoxian_init(void); 				//by FJW ���߹���
void tiaoxian_reset(void);
int weisha_check(unsigned int roundShineng);
unsigned int tiqushuzi(INT16U channal_choose);

#endif

#endif

