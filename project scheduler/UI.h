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
	void UIbegin(); //prints out starting message before file is loaded
	UI(scheduler* S); //constructor with pointer to scheduler
	void MODE(); // mode selection, 1= interactive, 2=step by step, 3= silent
	void print(); // print fn called each time step in modes 1 and 2
	void UIprint(); //prints based on mode
	void UIend(); //prints out ending message
};
	