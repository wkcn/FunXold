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

//����ע����
//�������� -> C++ -> Ԥ������ Ԥ���������壺_CRT_SECURE_NO_WARNINGS


//SIGN��ָ�����õķ��ţ�TOKEN�������õķ���
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