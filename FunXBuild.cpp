#include "FunXBuild.h"

bool FunReader::_signIndex[256][256];
bool FunReader::_signChar[256];
char FunReader::_signLevel[256];
vector<string> FunReader::_signTable;

FunReader::FunReader(){
	_finish = false;
	_pointer = 0;
	c = EOF;

	if (_signTable.empty()){
		//keyword �м䲻�ɲ�ո�
		//��󳤶�Ϊ2
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
			//��¼˫�ֽ�
			if (word.length()==2){
				_signIndex[word[0]][word[1]] = true;
			} 
		}

		_signLevel['+'] = 10;
		_signLevel['-'] = 10;
		_signLevel['*'] = 20;
		_signLevel['/'] = 20;
		_signLevel['^'] = 30;
		_signLevel['%'] = 30;
		_signLevel['='] = 5;
		_signLevel['>'] = 8;
		_signLevel['<'] = 8;
		_signLevel['!'] = 8;
		_signLevel['('] = 6;
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
	return (_finish&&c!=EOF);
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
	//KEYWORD��λ����ƥ��
	if(temp<0)return false;
	return _signIndex[str[0]][temp];
}

FunWord FunReader::SimpleNext(){
	string buffer;
	int temp;
	//Fun���Ķ���ֻ�֣���ĸ���֣��������ű�����
	//c�����ϴ�Ԥ��
	//����0���⴦��
	while (true){
		temp = GetWord();
		if (temp == EOF){
			if (!buffer.empty()){
				return FunWord(FUN_NAME, buffer);
			}
			return FunWord(FUN_NIL,"");
		}
		//���ֽڴ���
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

	//����һʧ
	if (!buffer.empty()){
		return FunWord(FUN_NAME, buffer);
	}
	//���˼·�Ƿ���NIL������Finish()����Ƿ�Ϊ��β
	//cout << buffer << endl;
	return FunWord(FUN_NIL, "������ >.< Error");
}

FunWord FunReader::Next(){
	//return SimpleNext();
	//�򵥷���Reader
	while (true){
		if (!is_new){
			word = SimpleNext();
		}
		else
			is_new = false;

		if (word.type == FUN_NIL){
			return word;
		}
		if (word.word == " " || word.word == "\n" || word.word == "\t" || word.word == ";"){
			continue;
		}
		if (word.type == FUN_TOKEN){
			if (word.word == "/*"){
				/*����ע��*/
				while ((word = SimpleNext()).word != "*/"){
					if (word.type == FUN_NIL)return word;
				}
				continue;
			}
			if (word.word == "//"){
				//������ע��
				while ((word = SimpleNext()).word != "\n"){
					if (word.type == FUN_NIL)return word;
				}
				continue;
			}
			if (word.word == "\""){
				//�ַ�������
				string temp_buffer;
				while ((word = SimpleNext()).word != "\""){
					if (word.type == FUN_NIL)return word;
					temp_buffer += word.word;
				}
				return FunWord(FUN_STR, temp_buffer);
				continue;
			}
			//Ϊ�˱�������ԣ���������`- Number ��ʾ,��ʵ���ǽ�`��Ϊ0

			//��ͨTOKEN����
			return word;
			//���縺����С��Ҫ�ر���
		}
		else
			if (word.type == FUN_NAME){
				//�ж��Ƿ�����,�����﷨�涨����һλ�����ֵı�Ϊ����\
				//��ʱ������Ƿ��﷨����
				char c = word.word[0];
				if (c<'0'||c>'9')return word;
				string num = word.word;
				word = SimpleNext();
				if (word.word == "."){
					num += word.word + SimpleNext().word;//��ȡС�����֣��ݲ�֧�ֿ�ѧ������
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

char FunReader::GetLevel(FunWord word){
	char level = _signLevel[word.word[0]];
	if (word.word.length() != 1)level *= 8;
	return level;
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

	stack<FunWord> opStack;

	while (!currentReader->Finish()){
		word = currentReader->Next();
		if (word.type == FUN_NIL){
			break;
		}
		/*
		if (word.type != FUN_TOKEN){
			test.push_back(word);
		}
		else{
			if (opStack.empty()){
				opStack.push(word);
			}
			else{
				if (currentReader->GetLevel(opStack.top()) >= currentReader->GetLevel(word)){
					while (!opStack.empty()){
						test.push_back(opStack.top());
						opStack.pop();
					}
				}
				else
					opStack.push(word);
			}
		}*/
		test.push_back(word);
	}

	while (!opStack.empty()){
		test.push_back(opStack.top());
		opStack.pop();
	}

	word = currentReader->Next();
	return 0;
}