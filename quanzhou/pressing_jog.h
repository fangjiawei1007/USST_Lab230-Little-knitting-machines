#ifndef _PRESSING_JOG_H_
#define _PRESSING_JOG_H_

#define	pressing_positive		(g_InteralMemory.KeepBit[47])		//用作正反向压针按钮
#define	pressing_negative		((~g_InteralMemory.KeepBit[47])&0x1)	
				

void pressing_jog(void);//此处jog是指定长运动
#endif