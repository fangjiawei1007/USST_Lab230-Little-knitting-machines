#ifndef _PRESSING_GET_ZERO_H_
#define _PRESSING_GET_ZERO_H_

#define cover								0  //��ס������   1
#define uncover 							1  //δ��ס������ 0 
#define lingwei_x_port						11 //��λ���X��

void pressing_get_zero(void);
void pressing_zero(unsigned int X_number);
void __irq pressing_zero_process(void);
#endif