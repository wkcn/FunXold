#pragma once
#include "Defines.h"

//FunRead只负责简单的标记，可以区分符号，（数字，字符串）
//为了简洁，不负责处理注释
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