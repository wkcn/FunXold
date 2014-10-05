#pragma once
#include "Defines.h"

//环境，当进入新的函数时会
struct FunEnvironment{
	FunNum R[256];//变量,长度512，但后256位是动态的
	FunNum K[512];//常量
	FunStr * S[512];
};

//现在使用STL容器，效率可能比较低
class FunVM{
private:
	FunEnvironment * currentEnv;	//当前运行环境
	stack<FunEnvironment*> EnvStack;

	//很容易越界
	FunNum *R, *K;
	FunStr **S;

	unsigned int pc;
	//注意，FunX是不会判断指令长度的，以FUN_END为结束标志
	FunCode *code;
private:
	FunEnvironment * NewEnv();
public:
	int Run();
};