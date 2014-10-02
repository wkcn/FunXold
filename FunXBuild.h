#pragma once
#include "Defines.h"

//FunReadֻ����򵥵ı�ǣ��������ַ��ţ������֣��ַ�����
//Ϊ�˼�࣬��������ע��
class FunReader
{

private:
	string _script;
	bool _isfile;
	bool _finish;
	int _pointer;
	FILE *_fin;
	static bool _signIndex[6][256];
	static bool _signChar[256];
	static vector<string> _signTable;
	int c;
private:
	int GetWord();
public:
	void InputFile(const string & filename);
	void InputScript(const string & script);
	bool Finish();
	FunWord Next();
	FunReader();
	~FunReader();
};