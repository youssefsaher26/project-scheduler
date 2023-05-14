#pragma once
using namespace std;
#include <iostream>
#include <Windows.h>

class scheduler;
class UI
{
private:
	int mode;
	scheduler* s;
public:
	UI(scheduler* S);
	void MODE();
	void print();
	void UIprint();
};
	