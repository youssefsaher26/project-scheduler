using namespace std;
#include <iostream>
#include <Windows.h>
#pragma once

class scheduler;

class UI
{
private:
	int mode;
	scheduler* sched;
public:
	UI(scheduler* S)
	{
		sched = S;
		mode = 1;
	}
	void MODE()
	{
		cout << "Select Mode" << endl;
		cout << "1=InterActive Mode" << endl;
		cout << "2=Step-by-Step Mode" << endl;
		cout << "3= Silent Mode" << endl;
		cin >> mode;
		while (mode != 1 && mode != 2 && mode != 3)
		{
			cout << "Error! Please select a valid mode" << endl;
			cin >> mode;
		}
	}
	void UIprint()
	{
		if (mode == 1)
		{
			cout << sched;
			cin.ignore();
		}
		else if (mode == 2)
		{

			Sleep(1000);
		}
	}


};

