#include "FunX.h"
int main(){

	//����FunX����
	FunX fun;

	fun.DoFile("test1.fun");

	while (true){
		string str;
		cin >> str;
		fun.DoString(str);
	}
	return 0;
}