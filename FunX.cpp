#include "FunX.h"
FunX::FunX(){
	cout << "FunX 版本:" << FunXVersion << endl << "版本信息：" << FunXInfo << endl<<endl;
}
FunX::~FunX(){

}
//DoString 支持缓冲,内部有读取状态Stack
int FunX::DoString(string str){
	//cout << "开始分析：" << str<<endl;
	//cout << "-------------------------"<<endl;
	reader.InputScript(str);
	Run();
	return 0;
}

int FunX::DoFile(string filename){
	cout << "读取文件：" << filename << endl;
	reader.InputFile(filename);
	vector<FunWord> test;
	while (!reader.Finish()){
		FunWord w = reader.Next();
		test.push_back(w);
		cout << w.word << endl;
	}
	Run();
	return 0;
}
int FunX::Run(int channel /* = -1 */){

	return 0;
}