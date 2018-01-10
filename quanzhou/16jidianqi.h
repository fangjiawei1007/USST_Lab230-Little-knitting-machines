#ifndef	_16JIDIANQI_H_
#define	_16JIDIANQI_H_

#define	jidianqi_write_single_fun_num				0x05

// #define	hongdeng									0x00
//#define	huangdeng									0x01
#define	lvdeng										0x02
// #define	youbeng										0x03
// #define	zhaobudeng									0x04
// #define	zhaomingdeng								0x05
// #define	fenshan										0x06
 #define	penyou24v									0x07
// #define	dianci										0x08 
#define	hongdeng									0x00
#define	youbeng										0x05
#define	zhaobudeng									0x01
#define	zhaomingdeng								0x02
#define	fenshan										0x04
#define	dianci										0x06
#define	huangdeng									0x03
//#define	qigang										0x09


//#define	jidianqi_station_num_kword		29


//#define	jidianqi_station_num			g_InteralMemory.KeepWord[jidianqi_station_num_kword]


/////////////////////////////////////////////////////////////////////////
#define	fenshan_button_kbit				17

#define hongdeng_button_bit				16
#define	huangdeng_button_bit			17
#define	lvdeng_button_bit				18
#define	zhaobudeng_button_bit			19
#define	zhaomingdeng_button_bit			20
#define	dianci_button_bit				50

#define	youbeng_button_bit				22
#define	penyou24v_button_bit			23

//#define	qigang_button_bit				24

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	fenshan_status_kbit				18

#define hongdeng_status_bit				25
#define	huangdeng_status_bit			26
#define	lvdeng_status_bit				27
#define	zhaobudeng_status_bit			28
#define	zhaomingdeng_status_bit			29
#define	dianci_status_bit				59

#define	youbeng_status_bit				31
#define	penyou24v_status_bit			32


///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	fenshan_button					g_InteralMemory.KeepBit[fenshan_button_kbit]

#define	hongdeng_button					g_InteralMemory.Bit[hongdeng_button_bit]
#define	huangdeng_button				g_InteralMemory.Bit[huangdeng_button_bit]
#define	lvdeng_button					g_InteralMemory.Bit[lvdeng_button_bit]
#define	zhaobudeng_button				g_InteralMemory.Bit[zhaobudeng_button_bit]
#define	zhaomingdeng_button				g_InteralMemory.Bit[zhaomingdeng_button_bit]
#define	dianci_button					g_InteralMemory.Bit[dianci_button_bit]

#define	youbeng_button					g_InteralMemory.Bit[youbeng_button_bit]
#define	penyou24v_button				g_InteralMemory.Bit[penyou24v_button_bit]
//#define	qigang_button					g_InteralMemory.Bit[qigang_button_bit]			
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	fenshan_status					g_InteralMemory.KeepBit[fenshan_status_kbit]

#define	hongdeng_status					g_InteralMemory.Bit[hongdeng_status_bit]
#define	huangdeng_status				g_InteralMemory.Bit[huangdeng_status_bit]
#define	lvdeng_status					g_InteralMemory.Bit[lvdeng_status_bit]
#define	zhaobudeng_status				g_InteralMemory.Bit[zhaobudeng_status_bit]
#define	zhaomingdeng_status				g_InteralMemory.Bit[zhaomingdeng_status_bit]
#define	dianci_status					g_InteralMemory.Bit[dianci_status_bit]

#define	youbeng_status					g_InteralMemory.Bit[youbeng_status_bit]
#define	penyou24v_status				g_InteralMemory.Bit[penyou24v_status_bit]
//#define	qigang_status					g_InteralMemory.Bit[qigang_status_bit]			
///////////////////////////////////////////////////////////////////////////////////////////////////////////


//int QZ_RTU_ReadWord(INT16U DevAddress);

void jidianqi_write_single(U8 which_port,U8 button_bit);
void jidianqi_write_fenshan(U8 which_port);
char qz_Uart_Getch(void);


void hongdeng_fun(void);
void huangdeng_fun(void);
void lvdeng_fun(void);
void zhaobudeng_fun(void);
void zhaomingdeng_fun(void);
void dianci_fun(void);
void youbeng_fun(void);
void penyou24v_fun(void);
void fenshan_fun(void);

void jidianqi_fun(void);




#endif

