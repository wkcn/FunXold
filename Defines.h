#pragma once
#include <iostream>

#include <fstream>
#include <cmath>
#include <cstring>

#include <vector>
#include <string>
#include <map>
#include <stack>

using namespace std;

//编译注意项
//配置属性 -> C++ -> 预处理器 预处理器定义：_CRT_SECURE_NO_WARNINGS


//SIGN是指无作用的符号，TOKEN是有作用的符号
enum FunWordType{
	FUN_NIL,
	FUN_UNKNOWN,
	FUN_NUM,
	FUN_STR,
	FUN_NAME,
	FUN_FACTOR,
	FUN_SIGN,
	FUN_TOKEN
};

//FunX中，数值和字符串是分开计算的
//数值：寄存器大小为512，前256为固定寄存器（临时变量用），后256为动态寄存器（运用地址偏移，以实现数组快速计算）
//字符串：用指针来进行运算
//Build以虚拟栈虚拟机的方式生成指令

enum FunOP{
	FUN_END,	//	结束指令
	FUN_MOVE,
	FUN_LOADK,
	FUN_ADD,
	FUN_SUB,
	FUN_MUL,
	FUN_DIV,
	FUN_MOD,
	FUN_POW,
	FUN_UNM,
	FUN_NOT,
	FUN_JMP,
	FUN_EQ,
	FUN_LT,
	FUN_LE,
	FUN_ADR,
	FUN_CALL
};

//基础类型定义
typedef double FunNum;
typedef string FunStr;


union FunCode{
	struct {
		FunOP OP : 6;
		unsigned int A : 8;
		unsigned int B : 9;
		unsigned int C : 9;
	};
	struct {
		FunOP OP : 6;
		unsigned int A : 8;
		unsigned int Bx : 18;//无符号
	};
	struct {
		FunOP OP : 6;
		unsigned int A : 8;
		unsigned int sBx : 18;//有符号
	};
};
struct FunWord{

	FunWordType type;
	string word;
	FunWord(){
		type = FUN_NIL;
		word = "";
	}
	FunWord(FunWordType _t, string _w){
		type = _t;
		word = _w;
	}
	void Set(FunWordType _t,string _w){
		type = _t;
		word = _w;
	}
};