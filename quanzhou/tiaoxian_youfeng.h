#ifndef __TIAOXIAN_YOUFENG_H__
#define __TIAOXIAN_YOUFENG_H__

#define TIAOXIAN_YOUFENG_EN


#define tiaoxian_shiji_kb				(g_InteralMemory.KeepBit[60])
/*********************************���޸�***************************************************/
#define tiaoxiankaiguan_kb				(g_InteralMemory.KeepBit[48])	//��ͷ���߿���
#define	tiaoxianzu_flag					(g_InteralMemory.KeepBit[26])	//�����鿪ʼ��¼ʵ�����˶���Ȧ�ı�־λ

//#define chudao_jiange						(g_InteralMemory.KeepWord[151]) //�������
//#define shoudao_jiange					(g_InteralMemory.KeepWord[152]) //�յ����

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
#define DAOSHU_MAX						0
#define PIANSHU_MAX						4
/***ιɴ��� 2018.1.24***/
#define weisha_jiange_kw				g_InteralMemory.KeepWord[460]	//ιɴ���

/*************************/
typedef struct{
	INT16U* kaishiquanshu;				//���߿�ʼȦ��
	INT16U* jieshuquanshu;				//���߽���Ȧ��
	INT16U* channal_choose;				//ͨ����ѡ��(ѡ��ڼ��鵶(DAOSHU))
	unsigned char* ewaiduan_choose;		//�����ѡ��
	INT16U* fangdabeishu[7];			//�Ŵ��������ߵ�ʱ�򣬸���������ٶȲ�һ��(�ٷֱ�)
	INT16U* yazheng_motor_1st;			//ѹ����1
	INT16U* yazheng_motor_2nd;			//ѹ����2
	INT16U* shangpan_shaungji_qigang[8];//����˫������	
	INT16U* shangpan_danji_qigang[6];	//���̵�������
	INT16U* xiapan_shuangji_qigang[21];	//����˫������
	INT16U* shangpan_checkout;		//����У��
	INT16U* xiapan_checkout;	//����У��
}TIAOXIANDUAN;

enum TIAOXIAN_MACRO{
	ON,
	OFF,
	WEISHAJIANSU,
	WEISHABUJIANSU
};
enum YouFeng_Checkout{
	NOT_CHANGED = 0,
	CHANGED = 1
};
//��������;˫������;NOJI-->������;
enum TIAOXIAN_SEL{
	NOJI = 0,
	DANJI = 1,
	SHUANGJI = 12,
};
/**************************************************************************************************/

void TiaoXian_Youfeng_App(void);

void TiaoXian_Youfeng_Init(void);
void TiaoXian_Youfeng_Reset(void);

void TiaoXian_Youfeng_Checkout(void);
void Tiaoxian_Youfeng_ComInfo_Set(void);
void TiaoXian_Youfeng_Shinengpanduan(void);

unsigned int Tiaoxian_Youfeng_jidianqi_write(unsigned int zhushu);

void TiaoXian_Youfeng_weisha(int duanshu);
void TiaoXian_Youfeng_chudao_shoudao_process(unsigned int i,unsigned int zushu);


#endif