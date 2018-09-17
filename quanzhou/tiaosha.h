#ifndef _TIAOSHA_H_
#define _TIAOSHA_H_

#define jiajiaStatus					(g_InteralMemory.Bit[100])		

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

/******************���߿���ִ�п�***************/
#define WEISHA_0_State					5
#define WEISHA_0_JIANSHA_1st			7
#define WEISHA_0_JIANSHA_2nd			6
#define WEISHA_0_JIANSHA_3rd			0

#define WEISHA_1_State					5
#define WEISHA_1_Back_State				4
#define WEISHA_1_JIANSHA_1st			5
// #define WEISHA_1_JIANSHA_2nd			7
// #define WEISHA_1_JIANSHA_3rd			6
// #define WEISHA_1_JIANSHA_4th			0

// #define WEISHA_State_None				0
// #define WEISHA_State					1
// #define JIANDAO_State					3
// #define SONGSHA_State					4

extern unsigned int Ports_state[ZUSHU_MAX];
/*************************/
typedef struct{
	INT16U* kaishiquanshu;				//���߿�ʼȦ��
	INT16U* jieshuquanshu;				//���߽���Ȧ��
	INT16U* channal_choose;				//ͨ����ѡ��(ѡ��ڼ��鵶)
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
int jidianqi_write_card(unsigned int zushu);

#endif

