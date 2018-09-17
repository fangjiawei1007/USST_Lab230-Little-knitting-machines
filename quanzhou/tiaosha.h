#ifndef _TIAOSHA_H_
#define _TIAOSHA_H_

#define jiajiaStatus					(g_InteralMemory.Bit[100])		

#define tiaoxiankaiguan_kb				(g_InteralMemory.KeepBit[48])	//大头条线开关
#define	tiaoxianzu_flag					(g_InteralMemory.KeepBit[26])	//调线组开始记录实际走了多少圈的标志位

#define chudao_jiange					(g_InteralMemory.KeepWord[151]) //出刀间隔
#define shoudao_jiange					(g_InteralMemory.KeepWord[152]) //收刀间隔

#define	tiaoxianzu_jiange				g_InteralMemory.KeepWord[99]	//调线间隔圈数
#define	tiaoxianzu_max					g_InteralMemory.KeepWord[61]	//调线组数
#define	tiaoxianzu						g_InteralMemory.KeepWord[93]	//记录当前为第几组
#define	tiaoxianzu_quanshu				g_InteralMemory.KeepWord[62]	//记录调线组实际走了多少圈

#define Y9								9
#define Y10								10

#define	tiaoshaduan_max					8
#define	choose_ewaiduan					1
#define	choose_not_ewaiduan				0

#define ZUSHU_MAX						6
#define DAOSHU_MAX						6
/***喂纱间隔 2018.1.24***/
#define weisha_jiange_kw				g_InteralMemory.KeepWord[460]	//喂纱间隔

/******************调线控制执行卡***************/
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
	INT16U* kaishiquanshu;				//调线开始圈数
	INT16U* jieshuquanshu;				//调线结束圈数
	INT16U* channal_choose;				//通道数选择(选择第几组刀)
	unsigned char* ewaiduan_choose;		//额外段选择
	INT16U* fangdabeishu[7];			//放大倍数：调线的时候，各个电机的速度不一样(百分比)
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
void tiaoxian_init(void); 				//by FJW 调线功能
void tiaoxian_reset(void);
int weisha_check(unsigned int roundShineng);
unsigned int tiqushuzi(INT16U channal_choose);
int jidianqi_write_card(unsigned int zushu);

#endif

