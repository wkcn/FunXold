#include "FunXVM.h"
FunEnvironment * FunVM::NewEnv(){
	//�����»���
	currentEnv = new FunEnvironment;
	EnvStack.push(currentEnv);
	R = currentEnv->R;
	K = currentEnv->K;
	S = currentEnv->S;
	return currentEnv;
}
int FunVM::Run(){
	pc = 0;
	FunCode c;
	while (true)
	{
		c = code[pc];
		//������Ҫ����ʹ��Ƶ��
		//FunX���ǽ���������������ܣ��Զ�����˳���Դﵽ�ϸ�����
		switch (c.OP){
			case FUN_MOVE:
				break;
			case FUN_LOADK:
				break;
			case FUN_ADD:
				break;
			case FUN_SUB:
				break;
			case FUN_MUL:
				break;
			case FUN_DIV:
				break;
			case FUN_MOD:
				break;
			case FUN_UNM:
				break;
			case FUN_JMP:
				break;
			case FUN_EQ:
				break;
			case FUN_LE:
				break;
			case FUN_LT:
				break;
			case FUN_ADR:
				break;
			case FUN_POW:
				break;
		}
	}
}