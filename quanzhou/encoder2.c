#include"includes.h"


void __irq encoder2_process(void)	
{
	int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 2) & 0x1)
	{
	}

	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
	//encoder2_pulse_number++;			//2016.1.6 quanzhou
}