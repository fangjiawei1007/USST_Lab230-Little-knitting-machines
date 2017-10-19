#ifndef _QIGANG_H_
#define	_QIGANG_H_

#define	qigang_status_bit						5
#define	qigang_status							g_InteralMemory.Bit[qigang_status_bit]

#define	qigang_stage_1_round_kword				27
#define	qigang_stage_2_round_kword				28
#define	qigang_stage_3_round_kword				29

#define	qigang_stage_1_round					g_InteralMemory.KeepWord[qigang_stage_1_round_kword]
#define	qigang_stage_2_round					g_InteralMemory.KeepWord[qigang_stage_2_round_kword]
#define	qigang_stage_3_round					g_InteralMemory.KeepWord[qigang_stage_3_round_kword]


#define	qigang_stage_word						14
#define	qigang_stage							g_InteralMemory.Word[qigang_stage_word]

void qigang_fun(void);

/* void qigang_init(void);
void qigang_fun(void); */





#endif


