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
	FUN_NAME,
	FUN_FACTOR,
	FUN_SIGN,
	FUN_TOKEN
};

//FunX�У���ֵ���ַ����Ƿֿ������
//��ֵ���Ĵ�����СΪ512��ǰ256Ϊ�̶��Ĵ�������ʱ�����ã�����256Ϊ��̬�Ĵ��������õ�ַƫ�ƣ���ʵ��������ټ��㣩
//�ַ�������ָ������������
//Build������ջ������ķ�ʽ����ָ��

enum FunOP{
	FUN_END,	//	����ָ��
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

//�������Ͷ���
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
		unsigned int Bx : 18;//�޷���
	};
	struct {
		FunOP OP : 6;
		unsigned int A : 8;
		unsigned int sBx : 18;//�з���
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