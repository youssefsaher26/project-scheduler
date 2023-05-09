//using namespace std;
//#include <iostream>
//#include <Windows.h>
//#pragma once
//
//class scheduler;
//
//class UI
//{
//private:
//	int mode;
//	scheduler* s;
//public:
//
//	UI(scheduler* S)
//	{
//		s = S;
//		mode = 1;
//	}
//	void MODE()
//	{
//		cout << "Select Mode" << endl;
//		cout << "1=InterActive Mode" << endl;
//		cout << "2=Step-by-Step Mode" << endl;
//		cout << "3= Silent Mode" << endl;
//		cin >> mode;
//		while (mode != 1 && mode != 2 && mode != 3)
//		{
//			cout << "Error! Please select a valid mode" << endl;
//			cin >> mode;
//		}
//	}
//
//	void UIprint()
//	{
//		if (mode == 1)
//		{
//			s->print();
//			cin.ignore();
//		}
//		else if (mode == 2)
//		{
//			s->print();
//			Sleep(1000);
//		}
//		else if (mode == 3)
//		{
//			Sleep(1000);
//		}
//	}
//
//
//};


//cout << "Current TimeStep : " << s->get_time() << endl;
//cout << "----------RDY PROCESSES----------" << endl;
//Node <processor*>* p = s->get_ProcessorsList()->getfront();
//int i = 1;
//while (p)
//{

//	if (p->getItem()->get_type() == 1)
//	{
//		cout << "Processor " << p->getItem()->getpnumber() << " " << *(FCFS*)p->getItem() << endl;
//		i++;
//		p = p->getNext();
//	}
//	else if (p->getItem()->get_type() == 2)
//	{
//		cout << "Processor " << p->getItem()->getpnumber() << " " << *(RoundRobin*)p->getItem() << endl;
//		i++;
//		p = p->getNext();
//	}
//	else
//	{
//		cout << "Processor " << p->getItem()->getpnumber() << " " << *(SJF*)p->getItem() << endl;
//		i++;
//		p = p->getNext();
//	}
//}
//cout << "----------BLK PROCESSES----------" << endl;
//Node<process*>* pr = s->get_BLK()->getfront();
//cout << s->get_BLK()->getcount() << " BLK: ";
//while (pr)
//{
//	cout << pr->getItem()->getID();
//	if (pr->getNext() != nullptr)
//	{
//		cout << " , ";
//	}
//	pr = pr->getNext();
//}
//cout << endl;
//cout << "-----------RUN PROCESSES----------" << endl;
//int x = s->CountRun();
//cout << x << " RUN: ";
//Node <processor*>* pt = s->get_ProcessorsList()->getfront();
//while (pt)
//{
//	process* pp = pt->getItem()->GetRun();
//	if (pp)
//	{
//		cout << pt->getItem()->GetRun()->getID() << "(P" << pt->getItem()->getpnumber() << ")";
//	}
//	if (pt->getNext() != nullptr && pp != nullptr)
//	{
//		cout << ", ";
//	}
//	pt = pt->getNext();
//}
//cout << endl;
//cout << "----------TRM PROCESSES----------" << endl;
//Node<process*>* ptrm = s->get_TRM()->getfront();
//cout << s->get_TRM()->getcount() << " TRM: ";
//while (ptrm)
//{
//	cout << ptrm->getItem()->getID();
//	if (ptrm->getNext() != nullptr)
//	{
//		cout << " , ";
//	}
//	ptrm = ptrm->getNext();
//}
//cout << endl;
//cout << "PRESS ANY KEY TO MOVE TO NEXT STEP!" << endl;
	