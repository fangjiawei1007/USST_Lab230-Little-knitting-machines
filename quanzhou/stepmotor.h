#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_


#define	stepmotor_port					4~8	





#define	datou_rate_different_1					g_InteralMemory.KeepWord[8]
#define	datou_rate_different_2					g_InteralMemory.KeepWord[9]
#define	datou_rate_different_3					g_InteralMemory.KeepWord[10]
#define	datou_rate_different_4					g_InteralMemory.KeepWord[11]
#define	datou_rate_different_5					g_InteralMemory.KeepWord[12]

#define	datou_rate_different_6					g_InteralMemory.KeepWord[140]
#define	datou_rate_different_7					g_InteralMemory.KeepWord[141]


#define	xiaotou_rate_different_1				g_InteralMemory.KeepWord[39]
#define	xiaotou_rate_different_2				g_InteralMemory.KeepWord[40]
#define	xiaotou_rate_different_3				g_InteralMemory.KeepWord[41]
#define	xiaotou_rate_different_4				g_InteralMemory.KeepWord[42]
#define	xiaotou_rate_different_5				g_InteralMemory.KeepWord[43]
	
#define	xiaotou_rate_different_6				g_InteralMemory.KeepWord[142]
#define	xiaotou_rate_different_7				g_InteralMemory.KeepWord[143]


#define	fencen_rate_different_1				g_InteralMemory.KeepWord[45]
#define	fencen_rate_different_2				g_InteralMemory.KeepWord[46]
#define	fencen_rate_different_3				g_InteralMemory.KeepWord[47]
#define	fencen_rate_different_4				g_InteralMemory.KeepWord[48]
#define	fencen_rate_different_5				g_InteralMemory.KeepWord[49]

#define	fencen_rate_different_6				g_InteralMemory.KeepWord[144]
#define	fencen_rate_different_7				g_InteralMemory.KeepWord[145]


#define	datou_modify_rate_different_1_kword			61
#define	datou_modify_rate_different_2_kword			62
#define	datou_modify_rate_different_3_kword			63
#define	datou_modify_rate_different_4_kword			64
#define	datou_modify_rate_different_5_kword			65

#define	xiaotou_modify_rate_different_1_kword		66
#define	xiaotou_modify_rate_different_2_kword		67
#define	xiaotou_modify_rate_different_3_kword		68
#define	xiaotou_modify_rate_different_4_kword		69
#define	xiaotou_modify_rate_different_5_kword		70

#define	fencen_modify_rate_different_1_kword		71
#define	fencen_modify_rate_different_2_kword		72
#define	fencen_modify_rate_different_3_kword		73
#define	fencen_modify_rate_different_4_kword		74
#define	fencen_modify_rate_different_5_kword		75

#define	datou_modify_rate_different_1				g_InteralMemory.KeepWord[61]
#define	datou_modify_rate_different_2				g_InteralMemory.KeepWord[62]
#define	datou_modify_rate_different_3				g_InteralMemory.KeepWord[63]
#define	datou_modify_rate_different_4				g_InteralMemory.KeepWord[64]
#define	datou_modify_rate_different_5				g_InteralMemory.KeepWord[65]

#define	xiaotou_modify_rate_different_1				g_InteralMemory.KeepWord[66]
#define	xiaotou_modify_rate_different_2				g_InteralMemory.KeepWord[67]
#define	xiaotou_modify_rate_different_3				g_InteralMemory.KeepWord[68]
#define	xiaotou_modify_rate_different_4				g_InteralMemory.KeepWord[69]
#define	xiaotou_modify_rate_different_5				g_InteralMemory.KeepWord[70]

#define	fencen_modify_rate_different_1				g_InteralMemory.KeepWord[71]
#define	fencen_modify_rate_different_2				g_InteralMemory.KeepWord[72]
#define	fencen_modify_rate_different_3				g_InteralMemory.KeepWord[73]
#define	fencen_modify_rate_different_4				g_InteralMemory.KeepWord[74]
#define	fencen_modify_rate_different_5				g_InteralMemory.KeepWord[75]

#define	shachang_shuru_xianshi_1_w					g_InteralMemory.Word[65]
#define	shachang_shuru_xianshi_2_w					g_InteralMemory.Word[66]
#define	shachang_shuru_xianshi_3_w					g_InteralMemory.Word[67]
#define	shachang_shuru_xianshi_4_w					g_InteralMemory.Word[68]

#define	dapan_zongzhenshu_kw						g_InteralMemory.KeepWord[139]

void qz_assert(void);
void alarm_disp_assert(void);



#endif


