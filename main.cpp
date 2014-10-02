#include "FunX.h"
int main(){

	//创建FunX对象
	FunX fun;

	fun.DoFile("test1.fun");

	while (true){
		string str;
		cin >> str;
		fun.DoString(str);
	}
	return 0;
}