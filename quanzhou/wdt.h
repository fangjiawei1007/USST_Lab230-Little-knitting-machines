/////////////////////////WDT���� by ����ΰ 2017/4/1 ///////////////
#ifndef __WDT_H__
#define __WDT_H__

/* #define		WTCON		(0x53000000)
#define		WTDAT		(0x53000004)
#define		WTCNT		(0x53000008)


#define 	rWTCON		(*(volatile unsigned int *)WTCON)
#define 	rWTDAT		(*(volatile unsigned int *)WTDAT)
#define 	rWTCNT		(*(volatile unsigned int *)WTCNT) */

#define	reset_time_kw					g_InteralMemory.KeepWord[148]


void wdt_init_reset(void);
void wdt_feed_dog(void);

#endif
