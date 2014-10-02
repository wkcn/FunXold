#include "FunXBuild.h"

bool FunReader::_signIndex[256][256];
bool FunReader::_signChar[256];
vector<string> FunReader::_signTable;

FunReader::FunReader(){
	_finish = false;
	_pointer = 0;
	c = EOF;

	if (_signTable.empty()){
		//keyword 中间不可插空格
		//最大长度为2
		const string _keyword[] = {
			"+", "-", "*", "/", "^", "%", "=",
			"+=", "-=", "*=", "/=", "^=", "%=",
			"++", "--",
			"==", "!=", ">=", "<=", ">", "<",
			"(", ")", "{", "}", "[", "]",
			";", "&", "?", ":", "\"", "\'"
			" ", "\n", "\t","/*", "*/","//", "."
		};

		/*
		for (int i = 0; i < 256;i++){
			for (int j = 0; j < 256;j++){
				_signIndex[j][i] = false;
			}
			_signChar[i] = false;
		}
		*/
		memset(_signIndex, 0, sizeof(_signIndex));
		memset(_signChar, 0, sizeof(_signChar));

		int n = sizeof(_keyword) / sizeof(string);
		for (int i = 0; i < n; i++) {
			string word = _keyword[i];
			_signTable.push_back(word);
			string buffer;
			buffer.clear();
			for (unsigned int k = 0; k < word.length(); k++){
				//_signIndex[k + 1][word[k]] = true;
				_signChar[word[k]] = true;
			}
			//记录双字节
			if (word.length()==2){
				_signIndex[word[0]][word[1]] = true;
			} 
		}
	}
}
FunReader::~FunReader(){
	if (_isfile)fclose(_fin);
}
void FunReader::InputFile(const string & filename){
	_isfile = true;
	c = EOF;
	_fin = fopen(filename.c_str(), "r");
}
void FunReader::InputScript(const string & script){
	_isfile = false;
	c = EOF;
	_pointer = 0;
	_script = script;
}
bool FunReader::Finish(){
	return _finish;
}
//GetWord函数只有当c==EOF时才会读取下一字符
int FunReader::GetWord(){
	if (_isfile){
		if (c == EOF){
			c = fgetc(_fin);
		}
		if (c == EOF){
			_finish = true;
		}
	}
	else{
		if (c == EOF){
			if (_pointer != _script.length()){
				c = _script[_pointer++];
			}
			if (_pointer == _script.length()){
				c = EOF;
				_finish = true;
			}
		}
	}
	return c;
}

bool FunReader::Match(const string & str, const int &temp){
	//KEYWORD二位快速匹配
	if(temp<0)return false;
	return _signIndex[str[0]][temp];
}

FunWord FunReader::SimpleNext(){
	string buffer;
	int temp;
	//Fun的阅读器只分（字母数字）、（符号表）两类
	//c保持上次预读
	//数字0特殊处理
	while (true){
		temp = GetWord();
		if (temp == EOF){
			return FunWord(FUN_NIL,"");
		}
		//宽字节处理
		if (temp < 0){
			buffer += temp;
			c = EOF;
			buffer += GetWord();
			c = EOF;
			continue;
		}
		if (_signChar[temp]){
			if (buffer.empty()){
				buffer += temp;
				c = EOF;
				while ((temp = GetWord())!=EOF)
				{
					if (Match(buffer,temp)){
						buffer += temp;
						c = EOF;
					}
					else{
						return FunWord(FUN_TOKEN, buffer);
					}
				}
			}
			else
			{
				return FunWord(FUN_STR, buffer);
			}
			break;
		}
		else{
			buffer += temp;
			c = EOF;
		}
	}
	//大概思路是返回NIL后再用Finish()检测是否为结尾
	//cout << buffer << endl;
	return FunWord(FUN_NIL, "错误啦 >.< Error");
}

FunWord FunReader::Next(){
	//简单分析Reader
	FunWord word;
	while (true){
		word = SimpleNext();
		if (word.type == FUN_NIL){
			return word;
		}
		if (word.word == " "||word.word=="\n"||word.word=="\t"){
			continue;
		}
		if (word.type == FUN_TOKEN){
			if (word.word == "/*"){
				/*跳过注释*/
				while ((word = SimpleNext()).word != "*/"){
					if (word.type == FUN_NIL)return word;
				}
				continue;
			}
			if (word.word == "//"){
				//跳过行注释
				while ((word = SimpleNext()).word != "\n"){
					if (word.type == FUN_NIL)return word;
				}
				continue;
			}
			return word;
			//比如负数，小数要特别处理
		}
		else
			if (word.type == FUN_STR){
				return word;
			}
			else
				if (word.type == FUN_NIL){
					return word;
				}
	}
	return word;
}

int FunBuild::SetScript(const string & script){
	currentReader = new FunReader;
	currentReader->InputScript(script);
	return Build();
}
int FunBuild::SetFile(const string & filename){
	currentReader = new FunReader;
	currentReader->InputFile(filename);
	return Build();
}
int FunBuild::Build(){
	FunWord word;
	vector<FunWord> test;
	while (!currentReader->Finish()){
		word = currentReader->Next();
		if (word.type == FUN_NIL){
			break;
		}
		test.push_back(word);
	}
	return 0;
}