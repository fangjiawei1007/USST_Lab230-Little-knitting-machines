#ifndef __TIAOXIAN_YOUFENG_H__
#define __TIAOXIAN_YOUFENG_H__

#define TIAOXIAN_YOUFENG_EN


#define tiaoxian_shiji_kb				(g_InteralMemory.KeepBit[60])
/*********************************待修改***************************************************/
#define tiaoxiankaiguan_kb				(g_InteralMemory.KeepBit[48])	//大头条线开关
#define	tiaoxianzu_flag					(g_InteralMemory.KeepBit[26])	//调线组开始记录实际走了多少圈的标志位

//#define chudao_jiange						(g_InteralMemory.KeepWord[151]) //出刀间隔
//#define shoudao_jiange					(g_InteralMemory.KeepWord[152]) //收刀间隔

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
#define DAOSHU_MAX						0
#define PIANSHU_MAX						4
/***喂纱间隔 2018.1.24***/
#define weisha_jiange_kw				g_InteralMemory.KeepWord[460]	//喂纱间隔

/*************************/
typedef struct{
	INT16U* kaishiquanshu;				//调线开始圈数
	INT16U* jieshuquanshu;				//调线结束圈数
	INT16U* channal_choose;				//通道数选择(选择第几组刀(DAOSHU))
	unsigned char* ewaiduan_choose;		//额外段选择
	INT16U* fangdabeishu[7];			//放大倍数：调线的时候，各个电机的速度不一样(百分比)
	INT16U* yazheng_motor_1st;			//压针电机1
	INT16U* yazheng_motor_2nd;			//压针电机2
	INT16U* shangpan_shaungji_qigang[8];//上盘双击气缸	
	INT16U* shangpan_danji_qigang[6];	//上盘单击气缸
	INT16U* xiapan_shuangji_qigang[21];	//下盘双击气缸
	INT16U* shangpan_checkout;		//上盘校验
	INT16U* xiapan_checkout;	//下盘校验
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
//单击气缸;双击气缸;NOJI-->不动作;
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