#ifndef _PRESSING_JOG_H_
#define _PRESSING_JOG_H_

#define	pressing_positive		(g_InteralMemory.KeepBit[47])		//����������ѹ�밴ť
#define	pressing_negative		((~g_InteralMemory.KeepBit[47])&0x1)	
				

void pressing_jog(void);//�˴�jog��ָ�����˶�
#endif