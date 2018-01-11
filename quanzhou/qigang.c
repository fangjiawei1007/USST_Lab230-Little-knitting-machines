#include"includes.h"

#define	qigang_port			9
#define	qigang_shangxing	1
#define	qigang_xiaxing		0


/*************************************************
Function(函数名称): qigang_fun(void)
Description(函数功能、性能等的描述): 在分层段以及额外分层段的时候需要气缸作用
Calls (被本函数调用的函数清单): dianci_fun(void)
Called By (调用本函数的函数清单): 

Input(输入参数说明，包括每个参数的作用、取值说明及参数间关系): 
Output(对输出参数的说明):
Return: 
Others: 
Author:王德铭
Modified:
Commented:方佳伟
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