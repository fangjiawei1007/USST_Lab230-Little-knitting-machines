#ifndef	_DELAY_H_
#define	_DELAY_H_

#define	delay_0_count				delay_fac.delay_flag[0]
#define	delay_1_count				delay_fac.delay_flag[1]
#define	delay_2_count				delay_fac.delay_flag[2]
#define	delay_3_count				delay_fac.delay_flag[3]
#define	delay_4_count				delay_fac.delay_flag[4]
#define	delay_5_count				delay_fac.delay_flag[5]

#define	init_stop_time_kw			g_InteralMemory.KeepWord[149]			//改机开机停机时间0.1s

typedef struct 
{
	U8 delay_init_flag[6];
	U32 delay_flag[6];
	U32 delay_time_qz[6];
	U8 delay_permit[6];
}DELAY_STRUCTURE;

void Timer0_init_quanzhou(void);

void Delay_qz_sub(void);
void delay_qz(U8 delay_rank,U32 delay_t,U8 status);



#endif

