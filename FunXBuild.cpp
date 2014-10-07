#include "FunXBuild.h"

bool FunReader::_signIndex[256][256];
bool FunReader::_signChar[256];
vector<string> FunReader::_signTable;

bool FunBuild::_staticOK;
unsigned char FunBuild::_signLevel[256][256];

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
			" ", "\n", "\t", "/*", "*/", "//", ".", "`"
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

FunBuild::FunBuild(){
	if (!_staticOK){

		memset(_signLevel, 0, sizeof(_signLevel));

		SetSignLevel("+", 10);
		SetSignLevel("-", 10);
		SetSignLevel("*", 11);
		SetSignLevel("/", 11);
		SetSignLevel("^", 13);
		SetSignLevel("%", 10);
		SetSignLevel("=", 5);
		SetSignLevel(">", 8);
		SetSignLevel("<", 8);
		SetSignLevel("(", 0);

		_staticOK = true;
	}
}

FunBuild::~FunBuild(){

}

void FunBuild::SetSignLevel(const string & name, unsigned char level){
	int s = 0;
	if (name.length()>1){
		s = name[1];
	}
	_signLevel[name[0]][s] = level;
}

unsigned char FunBuild::GetSignLevel(const string & name){
	int s = 0;
	if (name.length()>1){
		s = name[1];
	}
	return _signLevel[name[0]][s];
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
	return (_finish&&c!=EOF);
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
		//string a;
		//a = c;
		//cout << a;
	}
	else{
		if (c == EOF){
			if (_pointer != _script.length()){
				c = _script[_pointer++];
			}
			if (_pointer == _script.length()){
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
			if (!buffer.empty()){
				return FunWord(FUN_NAME, buffer);
			}
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
		//string a;
		//a = temp;
		//cout << a<<endl;
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
						if (buffer == "`") return FunWord(FUN_NAME, "0");
						return FunWord(FUN_TOKEN, buffer);
					}
				}
			}
			else
			{
				return FunWord(FUN_NAME, buffer);
			}
		}
		else{
			buffer += temp;
			//string a;
			//a = temp;
			//cout << a<<endl;
			c = EOF;
		}
	}

	//万无一失
	if (!buffer.empty()){
		return FunWord(FUN_NAME, buffer);
	}
	//大概思路是返回NIL后再用Finish()检测是否为结尾
	//cout << buffer << endl;
	return FunWord(FUN_NIL, "错误啦 >.< Error");
}

FunWord FunReader::Next(){
	//return SimpleNext();
	//简单分析Reader
	while (true){
		if (!is_new){
			word = SimpleNext();
		}
		else
			is_new = false;

		if (word.type == FUN_NIL){
			return word;
		}

		if (word.word == " " || word.word == "\t"){
			continue;
		}
		if (word.word == "\n" || word.word == ";"){
			return FunWord(FUN_NEXT, "");
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
			if (word.word == "\""){
				//字符串处理
				string temp_buffer;
				while ((word = SimpleNext()).word != "\""){
					if (word.type == FUN_NIL)return word;
					temp_buffer += word.word;
				}
				return FunWord(FUN_STR, temp_buffer);
				continue;
			}
			//为了避免二义性，纯负数用`- Number 表示,其实就是将`视为0

			//普通TOKEN返回
			return word;
			//比如负数，小数要特别处理
		}
		else
			if (word.type == FUN_NAME){
				//判断是否数字,按照语法规定，第一位是数字的必为数字\
				//暂时不检测是否语法错误
				char c = word.word[0];
				if (c<'0'||c>'9')return word;
				string num = word.word;
				word = SimpleNext();
				if (word.word == "."){
					num += word.word + SimpleNext().word;//获取小数部分，暂不支持科学计数法
					return FunWord(FUN_NUM, num);
				}
				else
				{
					is_new = true;
					return FunWord(FUN_NUM, num);
				}
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
void FunBuild::PopOPStack(stack<FunWord> & opStack, vector<FunWord> & RStack,const string tag /* = "" */){
	while (!opStack.empty()){
		if (opStack.top().word == tag){
			opStack.pop();
			break;
		}
		RStack.push_back(opStack.top());
		opStack.pop();
	}
}

int FunBuild::Build(){
	FunWord word;
	vector<FunWord> test;

	stack<FunWord> opStack;
	while (!currentReader->Finish()){
		word = currentReader->Next();
		if (word.type == FUN_NIL){
			break;
		}

		if (word.type != FUN_TOKEN){
			if (word.type == FUN_NEXT){
				PopOPStack(opStack, test);
			}
			//压入普通字符
			test.push_back(word);
		}
		else{
			if (opStack.empty()){
				opStack.push(word);
			}
			else{
				if (word.word=="("){
					opStack.push(word);
					continue;
				}
				if (word.word==")"){
					PopOPStack(opStack, test, "(");
					continue;
				}

				while (!opStack.empty() && GetSignLevel(opStack.top().word)>=GetSignLevel(word.word)){
					test.push_back(opStack.top());
					opStack.pop();
				}

				opStack.push(word);

			}
		}
		//test.push_back(word);
	}

	PopOPStack(opStack, test);

	word = currentReader->Next();
	return 0;
}