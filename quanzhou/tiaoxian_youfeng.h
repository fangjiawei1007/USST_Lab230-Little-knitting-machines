#ifndef __TIAOXIAN_YOUFENG_H__
#define __TIAOXIAN_YOUFENG_H__

#define TIAOXIAN_YOUFENG_EN


#define tiaoxian_shiji_kb				(g_InteralMemory.KeepBit[60])
#define tiaoxiankaiguan_kb				(g_InteralMemory.KeepBit[48])	//��ͷ���߿���
#define	tiaoxianzu_flag					(g_InteralMemory.KeepBit[26])	//�����鿪ʼ��¼ʵ�����˶���Ȧ�ı�־λ
#define	tiaoxianzu_jiange				(g_InteralMemory.KeepWord[99])	//���߼��Ȧ��
#define	tiaoxianzu_max					(g_InteralMemory.KeepWord[61])	//��������
#define	tiaoxianzu						(g_InteralMemory.KeepWord[93])	//��¼��ǰΪ�ڼ���
#define	tiaoxianzu_quanshu				(g_InteralMemory.KeepWord[62])	//��¼������ʵ�����˶���Ȧ

#define	tiaoshaduan_max					8
#define	choose_ewaiduan					1
#define	choose_not_ewaiduan				0

#define ZUSHU_MAX						6
#define DAOSHU_MAX						0
#define PIANSHU_MAX						4

#define Y9								9
#define Y10								10
#define Y9_Bit							5
#define Y10_Bit							6

/**��ʼ�źţ������ڲ�����**/
#define yazheng_motor_1st_start		g_InteralMemory.KeepWord[781]
#define yazheng_motor_2nd_start		g_InteralMemory.KeepWord[782]
/**ѹ��Ƚϱ����������ڲ�����**/
#define yazhen_pulse_cmp_1st		g_InteralMemory.KeepWord[783]
#define yazhen_pulse_cmp_2nd        g_InteralMemory.KeepWord[784]
/**ѹ�����������������ڲ�����**/
#define yazhen_motor_pulse_1st		g_InteralMemory.KeepWord[785]
#define yazhen_motor_pulse_2nd      g_InteralMemory.KeepWord[786]

typedef struct{
	INT16U* kaishiquanshu;				//���߿�ʼȦ��
	INT16U* jieshuquanshu;				//���߽���Ȧ��
	INT16U* channal_choose;				//ͨ����ѡ��(ѡ��ڼ��鵶(DAOSHU))
	unsigned char* ewaiduan_choose;		//�����ѡ��
	INT16U* fangdabeishu[7];			//�Ŵ��������ߵ�ʱ�򣬸���������ٶȲ�һ��(�ٷֱ�)
	INT16U* yazheng_motor_1st;			//ѹ����1
	INT16U* yazheng_motor_2nd;			//ѹ����2
	INT16U* shangpan_shaungji_qigang[8];//����˫������	(ƴ�������)
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

enum TIAOXIAN_CHECKOUT{
	NOT_CHANGED = 0,
	CHANGED	=1
};   
//��������;˫������;NOJI-->������;
enum TIAOXIAN_SEL{
	NOJI = 0,
	DANJI = 1,
	SHUANGJI = 12
};
	

/**************************************************************************************************/

void TiaoXian_Youfeng_App(void);

void TiaoXian_Youfeng_Init(void);
void TiaoXian_Youfeng_Init_once(void);
void TiaoXian_Youfeng_Reset(void);

int TiaoXian_Youfeng_Checkout(void);
void Tiaoxian_Youfeng_ComInfo_Set(void);
void Tiaoxian_Youfeng_Pianshu_Set(int duanshu,unsigned int pianshu);

unsigned int Tiaoxian_Youfeng_jidianqi_write(unsigned int zushu,unsigned int pianshu,unsigned int duanshu);
unsigned int Tiaoxian_Youfeng_jidianqi_zero(unsigned int zushu,unsigned int pianshu);
void Tiaoxian_Youfeng_Yazhen(unsigned int duanshu_prev,unsigned int duanshu_cur);
void Tiaoxian_Youfeng_Yazhen_Get_Zero(unsigned int yazhen_num);

void TiaoXian_Youfeng_weisha(int duanshu);

int between_check(unsigned int roundShineng);
unsigned int at_check(unsigned int roundShineng);

#endif