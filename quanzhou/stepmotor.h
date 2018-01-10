#ifndef _STEPMOTOR_H_
#define _STEPMOTOR_H_


#define	stepmotor_port					4~8	





#define	datou_rate_different_1					g_InteralMemory.KeepWord[8]		//下部大送砂轮_大头段
#define	datou_rate_different_2					g_InteralMemory.KeepWord[9]     //上部大送砂轮_大头段
#define	datou_rate_different_3					g_InteralMemory.KeepWord[10]    //下部小送砂轮_大头段
#define	datou_rate_different_4					g_InteralMemory.KeepWord[11]    //上部大送砂轮_大头段
#define	datou_rate_different_5					g_InteralMemory.KeepWord[12]    //卷布机_大头段
                                                                                
#define	datou_rate_different_6					g_InteralMemory.KeepWord[140]   //五号送砂轮_大头段
#define	datou_rate_different_7					g_InteralMemory.KeepWord[141]   //六号送砂轮_大头段
                                                                           

#define	xiaotou_rate_different_1				g_InteralMemory.KeepWord[39]	//下部大送砂轮_小头段
#define	xiaotou_rate_different_2				g_InteralMemory.KeepWord[40]	//上部大送砂轮_小头段
#define	xiaotou_rate_different_3				g_InteralMemory.KeepWord[41]	//下部小送砂轮_小头段
#define	xiaotou_rate_different_4				g_InteralMemory.KeepWord[42]	//上部大送砂轮_小头段
#define	xiaotou_rate_different_5				g_InteralMemory.KeepWord[43]	//卷布机_小头段
	
#define	xiaotou_rate_different_6				g_InteralMemory.KeepWord[142]	//五号送砂轮_小头段
#define	xiaotou_rate_different_7				g_InteralMemory.KeepWord[143]	//六号送砂轮_小头段


#define	fencen_rate_different_1					g_InteralMemory.KeepWord[45]	//下部大送砂轮_分层段
#define	fencen_rate_different_2					g_InteralMemory.KeepWord[46]    //上部大送砂轮_分层段
#define	fencen_rate_different_3					g_InteralMemory.KeepWord[47]    //下部小送砂轮_分层段
#define	fencen_rate_different_4					g_InteralMemory.KeepWord[48]    //上部大送砂轮_分层段
#define	fencen_rate_different_5					g_InteralMemory.KeepWord[49]    //卷布机_分层段
                                                                                
#define	fencen_rate_different_6					g_InteralMemory.KeepWord[144]   //五号送砂轮_分层段
#define	fencen_rate_different_7					g_InteralMemory.KeepWord[145]   //六号送砂轮_分层段
                                                                           
#define	shachang_shuru_xianshi_1_w					g_InteralMemory.Word[65]	//下部大送砂轮_下方计算值
#define	shachang_shuru_xianshi_2_w					g_InteralMemory.Word[66]    //上部大送砂轮_下方计算值
#define	shachang_shuru_xianshi_3_w					g_InteralMemory.Word[67]    //下部小送砂轮_下方计算值
#define	shachang_shuru_xianshi_4_w					g_InteralMemory.Word[68]    //上部大送砂轮_下方计算值
                                                                           
#define	dapan_zongzhenshu_kw						g_InteralMemory.KeepWord[139]	//大盘总针数

void qz_assert(void);
void alarm_disp_assert(void);



#endif


