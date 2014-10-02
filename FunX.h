#pragma once
#include "Defines.h"
#include "FunXBuild.h"

class FunX{
private:
	const double FunXVersion = 0.3;
	const string FunXInfo = "Unknown";
	FunBuild funBuild;
public:
	int DoString(string str);
	int DoFile(string filename);
	int Run(int channel = -1);
	FunX();
	~FunX();
};