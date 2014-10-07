#pragma once
#include "Defines.h"

//FunRead
//Next()������Ч�ַ����ڲ������ַ��������֣����֣����ţ�ע��
class FunReader{

private:
	string _script;
	bool _isfile;
	bool _finish;
	int _pointer;
	FILE *_fin;

	bool is_new = false;
	FunWord word;

	static bool _signIndex[256][256];
	static bool _signChar[256];

	static vector<string> _signTable;
	int c;

private:
	int GetWord();
	FunWord SimpleNext();
	bool Match(const string & str, const int &temp);

public:
	void InputFile(const string & filename);
	void InputScript(const string & script);
	bool Finish();
	FunWord Next();

	FunReader();
	~FunReader();
};

//FunBuild
//�����뷭��Ϊ�ֽ���
class FunBuild{
private:
	FunReader *currentReader;
	static unsigned char _signLevel[256][256];
	static bool _staticOK;

	void SetSignLevel(const string & name, unsigned char level);
	unsigned char GetSignLevel(const string & name);
	void PopOPStack(stack<FunWord> & opStack, vector<FunWord> & RStack,const string tag = "");
public:
	int SetFile(const string & filename);
	int SetScript(const string & script);
	int Build();

	FunBuild();
	~FunBuild();
};