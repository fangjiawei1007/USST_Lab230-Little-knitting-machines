#ifndef _INIT_H_
#define	_INIT_H_

#define	menu_init_flag							g_InteralMemory.KeepBit[19]
//#define	encoder1_fac_test_flag					g_InteralMemory.KeepBit[16]
#define	stepmotor_init_flag						g_InteralMemory.KeepBit[12]
#define	bianpingqi_init_flag					g_InteralMemory.KeepBit[10]
#define	jidianqi_init_flag						g_InteralMemory.KeepBit[27]
#define	guanliyuan_mima_reset					g_InteralMemory.KeepBit[11]

void songsha_init(void);
void shoubu_init(void);
void jidianqi_init(void);
void encoder1_init(void);
void menu_init(void);
void zhongduan_init(void);
void bianpingqi_init(void);
void qz_uart_init(void);
void stepmotor_init(void);






















#endif


