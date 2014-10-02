#include "FunXBuild.h"

bool FunReader::_signIndex[6][256];
bool FunReader::_signChar[256];
vector<string> FunReader::_signTable;

FunReader::FunReader(){
	_finish = false;
	_pointer = 0;
	c = EOF;

	if (_signTable.empty()){
		//keyword �м䲻�ɲ�ո�
		const string _keyword[] = {
			"+", "-", "*", "/", "^", "%", "=",
			"+=", "-=", "*=", "/=", "^=", "%=",
			"++", "--",
			"==", "!=", ">=", "<=", ">", "<",
			"(", ")", "{", "}", "[", "]",
			";", "&", "?", ":", "\"", "\'"
			" ", "\n"
		};

		for (int i = 0; i < 256;i++){
			for (int j = 1; j < 6;j++){
				_signIndex[j][i] = false;
			}
			_signChar[i] = false;
		}

		int n = sizeof(_keyword) / sizeof(string);
		for (int i = 0; i < n; i++) {
			string word = _keyword[i];
			_signTable.push_back(word);
			string buffer;
			buffer.clear();
			for (unsigned int k = 0; k < word.length(); k++){
				_signIndex[k + 1][word[k]] = true;
				_signChar[word[k]] = true;
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
//GetWord����ֻ�е�c==EOFʱ�Ż��ȡ��һ�ַ�
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
FunWord FunReader::Next(){
	string buffer;
	int temp;
	//Fun���Ķ���ֻ�֣���ĸ���֣��������ű�����
	//c�����ϴ�Ԥ��
	//����0���⴦��
	while (true){
		temp = GetWord();
		if (temp == EOF){
			return FunWord(FUN_NIL,"");
		}
		if (_signChar[temp]){
			if (buffer.empty()){
				buffer += temp;
				c = EOF;
				while ((temp = GetWord())!=EOF)
				{
					if (_signIndex[buffer.length()+1][temp]){
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
	//���˼·�Ƿ���NIL������Finish()����Ƿ�Ϊ��β
	//cout << buffer << endl;
	return FunWord(FUN_NIL, "������ >.< Error");
}

