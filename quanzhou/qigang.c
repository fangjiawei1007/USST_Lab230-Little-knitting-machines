#include"includes.h"

#define	qigang_port			9
#define	qigang_shangxing	1
#define	qigang_xiaxing		0

/* unsigned int qigang_round=0;
unsigned char qigang_status[3]={0};
#define	dianci_close		1
#define	dianci_open			0

#define	dianci_a_port		9
#define	dianci_b_port		10
#define	dianci_c_port		11

void qigang_init(void)
{
	qigang_stage=1;
}

void qigang_fun(void)
{
	switch(qigang_stage)
	{
		case 1:
			if (qigang_round<=qigang_stage_1_round)
			{
				if (qigang_status[0]!=dianci_close||qigang_status[1]!=dianci_close||qigang_status[2]!=dianci_close)
				{
					Set_Y_Value(dianci_a_port,dianci_close);
					Set_Y_Value(dianci_b_port,dianci_close);
					Set_Y_Value(dianci_c_port,dianci_close);
					qigang_status[0]=qigang_status[1]=qigang_status[2]=dianci_close;
				}
				if (qigang_round==qigang_stage_1_round)
				{
					qigang_round=0;
					qigang_stage=2;
				}
			}break;
		case 2:
			if (qigang_round<=qigang_stage_2_round)
			{
				if (qigang_status[0]!=dianci_close)
				{
					Set_Y_Value(dianci_a_port,dianci_close);
					qigang_status[0]=dianci_close;
				}
				if (encoder1_pulse_number>=4167 && (!(qigang_status[1]^qigang_status[2])) && encoder1_pulse_number<5556)
				{
					qigang_status[2] ^= dianci_close;
					Set_Y_Value(dianci_c_port,qigang_status[2]);
				}
				if (encoder1_pulse_number>=9723 && (!(qigang_status[1]^qigang_status[2])) && encoder1_pulse_number<11112)
				{
					qigang_status[1] ^= dianci_close;
					Set_Y_Value(dianci_b_port,qigang_status[1]);
				}
				if (qigang_round==qigang_stage_2_round)
				{
					qigang_round=0;
					qigang_stage=3;
				}
			}break;
		case 3:
			if (qigang_round<=qigang_stage_3_round)
			{
				if (qigang_status[0]!=dianci_open||qigang_status[1]!=dianci_open||qigang_status[2]!=dianci_open)
				{
					Set_Y_Value(dianci_a_port,dianci_open);
					Set_Y_Value(dianci_b_port,dianci_open);
					Set_Y_Value(dianci_c_port,dianci_open);
					qigang_status[0]=qigang_status[1]=qigang_status[2]=dianci_open;
				}
				if (qigang_round==qigang_stage_3_round)
				{
					qigang_round=0;
					qigang_stage=1;
				}
			}
	}
}
 */


void qigang_fun(void)
{
	if (current_stage==3 || (current_stage==5 && (dapan_round<extra_fencen_quan_num_kw || dapan_round>(extra_part_quanshu-extra_fencen_quan_num_kw))))
	{
		dianci_button=1;
		dianci_fun();
		//qigang_status=qigang_xiaxing;
	}
	else
	{
		dianci_button=0;
		dianci_fun();
		//qigang_status=qigang_shangxing;
	}
}