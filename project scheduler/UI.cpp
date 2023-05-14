#include "UI.h"
#include"scheduler.h"
UI::UI(scheduler* S)
{
	s = S;
	mode = 1;
}
void UI::UIbegin()
{
	cout << "PROCESSING..." << endl<<'\n';
}
void UI::MODE()
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
	if (mode==3)
		cout << "silent mode... output file will be saved at the end of simulation"<<'\n';
}
void UI:: print()
{
	cout << *s;
}
void UI:: UIprint()
{
	if (mode == 1)
	{
		cin.ignore();
		print();
		cout << "PRESS ANY KEY TO MOVE TO NEXT STEP!" << '\n';
	}
	else if (mode == 2)
	{
		cout << '\n';
		print();
		Sleep(1000);
	}
	else if (mode == 3)
	{
		Sleep(1000);
	}
}
void UI:: UIend()
{
	cout << "END OF SIMULATION... VIEW OUTPUT FILE" << endl;
}