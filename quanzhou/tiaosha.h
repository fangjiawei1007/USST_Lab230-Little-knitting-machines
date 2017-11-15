#ifndef _TIAOSHA_H_
#define _TIAOSHA_H_

#define tiaoxiankaiguan_kb				(g_InteralMemory.KeepBit[48])	//大头条线开关
#define	tiaoxianzu_flag					(g_InteralMemory.KeepBit[26])

#define chudao_jiange					(g_InteralMemory.KeepWord[151]) //出刀间隔
#define shoudao_jiange					(g_InteralMemory.KeepWord[152]) //出刀间隔

#define	tiaoxianzu_jiange				g_InteralMemory.KeepWord[99]
#define	tiaoxianzu_max					g_InteralMemory.KeepWord[61]
#define	tiaoxianzu						g_InteralMemory.KeepWord[93]
#define	tiaoxianzu_quanshu				g_InteralMemory.KeepWord[62]

#define Y9								9
#define Y10								10

#define	tiaoshaduan_max					8
#define	choose_ewaiduan					1
#define	choose_not_ewaiduan				0

typedef struct{
	INT16U* kaishiquanshu;
	INT16U* jieshuquanshu;
	INT16U* channal_choose;
	unsigned char* ewaiduan_choose;
	INT16U* fangdabeishu[7];
}TIAOXIANDUAN;


void shinengpanduan(void);
void tiaoxian(void);
void chudao_shoudao_process(unsigned int i,unsigned int zushu);
void weisha(unsigned int i,unsigned int zushu);
unsigned int tiaoxian_jidianqi_write(unsigned int zhushu);
int between_check(unsigned int roundShineng);
unsigned int at_check(unsigned int roundShineng);
void tiaoxian_init(void); //by FJW 调线功能
void tiaoxian_reset(void);

#endif

