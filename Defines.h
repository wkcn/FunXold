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
	FUN_FACTOR,
	FUN_SIGN,
	FUN_TOKEN
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