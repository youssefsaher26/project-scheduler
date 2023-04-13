#pragma once
#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "process.h"
#include"processor.h"
#include<iostream>
#include"RoundRobin.h"
#include"FCFS.h"
#include<fstream>
#include "kill.h"
using namespace std;

class scheduler
{
private:
	int time;
	int STL;
	int forkprob;
	int processno;
	int FCFSno;
	int SJFno;
	int RRno;
	QueueADT<process*>* NEW;
	QueueADT<process*>* BLK;
	QueueADT<process*>* TRM;
	QueueADT<processor*>* ProcessorsList;
	QueueADT<kill*>* killsigs;

public:
	scheduler()
	{
		NEW = new QueueADT<process*>;
		BLK = new QueueADT<process*>;
		TRM = new QueueADT<process*>;
		ProcessorsList = new QueueADT<processor*>;
		killsigs = new QueueADT<kill*>;
		time = 0;
		STL = 0;
		forkprob = 0;
		FCFSno = 0;
		SJFno = 0;
		RRno = 0;

	}
	processor* processor_shortest_queue()
	{
		processor* p = ProcessorsList->getfront()->getItem();
	}
	void loadfile()
	{
		char garbage;
		cout << "enter file name"<<endl;
		ofstream outputFile;
		ifstream inputFile;
		inputFile.open("Test.txt", ios::in);
			int rtf, t_slice, Maxw;
			cout << "Processing"<<endl;
			inputFile >> FCFSno>>SJFno>>RRno;
			inputFile >> t_slice;
			RoundRobin::set_Timeslice(t_slice);
			inputFile >> rtf;
			RoundRobin::set_RTF(rtf);
			inputFile >> Maxw;
			FCFS::set_Maxw(Maxw);
			inputFile >> STL>>forkprob;
			inputFile >> processno;
			for (int i = 0; i < processno; i++)
			{
				int at, pid, ct, io_num, io_R, io_D;
				inputFile >> at >> pid >> ct >> io_num;
				for (int i = 0; i < io_num; i++)
				{
					inputFile >> garbage;//bracket1
					inputFile >> io_R; //ioR
					inputFile >> garbage;//comma
					inputFile >> io_D; //ioD
					inputFile >> garbage;//bracket2
					inputFile >> garbage;//comma
				}
				process* p = new process(at,pid, ct, io_num, io_R, io_D);
				NEW->enqueue(p);
			}
			int SigId,SigT;
			while (!inputFile.eof())
			{
				inputFile >> SigT >> SigId;
				kill* k = new kill(SigT, SigId);
				killsigs->enqueue(k);
			}
			outputFile.close();
	}
	void NEWtoRDY()
	{
		process* ptr1;
		Node<processor*> *temp;
		NEW->dequeue(ptr1);
		temp = ProcessorsList->getfront();
		while (temp)
		{
			processor* p2;
			p2 = temp->getItem();
			p2->AddProcess(ptr1);
			temp = temp->getNext();
			if (temp->getNext() == NULL)
			{
				temp->setNext(ProcessorsList->getfront());
			}
		}
	}
	QueueADT<process*>* getTRM()
	{
		return TRM;
	}
	QueueADT<process*>* getBLK()
	{
		return BLK;
	}
	void simulator()
	{
		NEWtoRDY();
		//call print fn
		//mouseclick before incrementing time
		time++;
	}
	int countbusy()
	{

	}
	int CountRun() const
	{
		Node <processor*>* p = ProcessorsList->getfront();
		int c = 0;
		while (p)
		{
			if (p->getItem()->State == 1)
			{
				c++;
			}
			p = p->getNext();
		}
		return c;
	}
	friend ostream& operator<< (ostream& out, const scheduler& s)
	{
		out << "Current TimeStep : " << s.time << endl;
		out << "----------RDY PROCESSES----------" << endl;
		Node <processor*>* p = s.ProcessorsList->getfront();
		int i = 1;
		while (p)
		{
			out << "Processor " << i << " " << p << endl;
			i++;
			p = p->getNext();

		}
		out << "----------BLK PROCESSES----------" << endl;
		Node<process*>* pr = s.BLK->getfront();
		out << s.BLK->getcount() << " BLK: ";
		while (pr)
		{
			out << pr->getItem()->getID()<<" , ";
			pr=pr->getNext();
		}
		out << endl;
		out << "-----------RUN PROCESSES----------" << endl;
		int x = s.CountRun();
		out << x << " RUN: ";
		Node <processor*>* pt = s.ProcessorsList->getfront();
		while (pt)
		{

			pt->getItem()->getrun();
			if (pt)
			{
				out << pt->getItem()->getrun()->getID() <<"(p"<<pt->getItem()->getpnumber() << "), ";
			}
			pt = pt->getNext();
		}
		out << endl;
		out << "----------TRM PROCESSES----------" << endl;
		Node<process*>* ptrm = s.TRM->getfront();
		out << s.TRM->getcount() << " TRM: ";
		while (ptrm)
		{
			out << ptrm->getItem()->getID() << " , ";
			ptrm = ptrm->getNext();
		}
		out << endl;
		out << "PRESS ANY KEY TO MOVE TO NEXT STEP!"<<endl;
		
	}
	void print()
	{
		scheduler S = *this;
		cout << S;
	}

};

