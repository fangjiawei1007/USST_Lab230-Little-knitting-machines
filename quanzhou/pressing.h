#ifndef _PRESSING_H_
#define _PRESSING_H_

#define yazheng_button 						(g_InteralMemory.KeepBit[40])			//移圈压针
#define qingling_button 					(g_InteralMemory.KeepBit[41])			//清零按钮
#define lingwei_jiance_button 				(g_InteralMemory.KeepBit[42])			//零位检测按钮
#define huiling_zijian_button 				(g_InteralMemory.KeepBit[43])			//回零自检按钮
#define positive_button						(g_InteralMemory.KeepBit[44])			//手动调节正向
#define negative_button						(g_InteralMemory.KeepBit[45])			//手动调节反向
//#define yazheng_direction					(g_InteralMemory.KeepBit[46])			//正向压针、反向压针


#define yazheng_depth						(g_InteralMemory.KeepWord[141])			//压针深度
#define yazheng_max_deapth					(g_InteralMemory.KeepWord[142])			//压针最大深度
#define huiling_length						(g_InteralMemory.KeepWord[143])			//回零自检移出距离
#define yazheng_pulse_total_remember		(g_InteralMemory.KeepWord[144])			//最大角度(脉冲total)
#define jiaodu_monitor						(g_InteralMemory.KeepWord[145])			//角度测量
#define motor_dpi							(g_InteralMemory.KeepWord[146])			//分辨率
#define multiply							(g_InteralMemory.KeepWord[147])			//倍率

#define yazheng_pulse_send					(g_InteralMemory.KeepWord[148])			//压针总共需要发送脉冲数
#define yazheng_pulse_number				(g_InteralMemory.KeepWord[149])			//细分数计数器

#define	pressing_director_port				10
#define	pressing_pulse_port					9

#define pressing_get_zero_times				(g_InteralMemory.KeepWord[151])			//回零自检次数，后期可能用不到这个按钮
void pressing(void);
void pressing_get_zero(void);
#endif