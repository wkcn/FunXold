#include "FunX.h"
FunX::FunX(){
	cout << "FunX �汾:" << FunXVersion << endl << "�汾��Ϣ��" << FunXInfo << endl<<endl;
}
FunX::~FunX(){

}
//DoString ֧�ֻ���,�ڲ��ж�ȡ״̬Stack
int FunX::DoString(string str){
	//cout << "��ʼ������" << str<<endl;
	//cout << "-------------------------"<<endl;
	funBuild.SetScript(str);
	Run();
	return 0;
}

int FunX::DoFile(string filename){
	cout << "��ȡ�ļ���" << filename << endl;
	/*
	FunReader reader;
	reader.InputFile(filename);
	vector<FunWord> test;
	while (!reader.Finish()){
	FunWord w = reader.Next();
	test.push_back(w);
	cout << w.word << endl;
	}
	*/
	funBuild.SetFile(filename);
	Run();
	return 0;
}
int FunX::Run(int channel /* = -1 */){

	return 0;
}