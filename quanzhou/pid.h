#ifndef _PID_H_
#define	_PID_H_


float PID_realize(float speed);


typedef struct
{      
	float SetSpeed;            //�����趨ֵ     
	float ActualSpeed;        //����ʵ��ֵ     
	float err;                //����ƫ��ֵ      
	float err_last;            //������һ��ƫ��ֵ      
	float Kp,Ki,Kd;            //������������֡�΢��ϵ��     
	float voltage;          //�����ѹֵ������ִ�����ı�����     
	float integral;            //�������ֵ 
}pi_d;











#endif

