#pragma once
#include "Defines.h"

//�������������µĺ���ʱ��
struct FunEnvironment{
	FunNum R[256];//����,����512������256λ�Ƕ�̬��
	FunNum K[512];//����
	FunStr * S[512];
};

//����ʹ��STL������Ч�ʿ��ܱȽϵ�
class FunVM{
private:
	FunEnvironment * currentEnv;	//��ǰ���л���
	stack<FunEnvironment*> EnvStack;

	//������Խ��
	FunNum *R, *K;
	FunStr **S;

	unsigned int pc;
	//ע�⣬FunX�ǲ����ж�ָ��ȵģ���FUN_ENDΪ������־
	FunCode *code;
private:
	FunEnvironment * NewEnv();
public:
	int Run();
};