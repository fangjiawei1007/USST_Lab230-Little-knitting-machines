#include"includes.h"

#define	qigang_port			9
#define	qigang_shangxing	1
#define	qigang_xiaxing		0


/*************************************************
Function(��������): qigang_fun(void)
Description(�������ܡ����ܵȵ�����): �ڷֲ���Լ�����ֲ�ε�ʱ����Ҫ��������
Calls (�����������õĺ����嵥): dianci_fun(void)
Called By (���ñ������ĺ����嵥): 

Input(�������˵��������ÿ�����������á�ȡֵ˵�����������ϵ): 
Output(�����������˵��):
Return: 
Others: 
Author:������
Modified:
Commented:����ΰ
*************************************************/
void qigang_fun(void)
{
	if (current_stage==fencenduan || 
	   (current_stage==ewaiduan && (dapan_round<extra_fencen_quan_num_kw || 
	    dapan_round>=(extra_part_quanshu-extra_fencen_quan_num_kw))))
	{
		dianci_button=1;
		dianci_fun();
	}
	else
	{
		dianci_button=0;
		dianci_fun();
	}
}