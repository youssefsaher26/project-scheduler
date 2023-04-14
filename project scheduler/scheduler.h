#pragma once
#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "process.h"
#include"processor.h"
#include<iostream>
#include"RoundRobin.h"
#include "PriorityQueue.h"
#include"FCFS.h"
#include<fstream>
#include "kill.h"
#include"IO_R_D.h"
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
	QueueADT<kill*>* killsigs=nullptr;
	QueueADT<IO_R_D*>* inputsigs=nullptr;
public:
	scheduler()
	{
		NEW = new QueueADT<process*>;
		BLK = new QueueADT<process*>;
		TRM = new QueueADT<process*>;
		ProcessorsList = new QueueADT<processor*>;
		killsigs = new QueueADT<kill*>;
		inputsigs = new QueueADT<IO_R_D*>;
		time = 0;
		STL = 0;
		forkprob = 0;
		FCFSno = 0;
		SJFno = 0;
		RRno = 0;

	}
	void simulator()
	{
		NEWtoRDY();
		RUNtoTRM();
		print();
		//mouseclick before incrementing time
		time++;
	}
	void loadfile()
	{
		char garbage;
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
				process* p = new process(at, pid, ct, io_num, nullptr);
				for (int i = 0; i < io_num; i++)
				{
					inputFile >> garbage;//bracket1
					inputFile >> io_R; //ioR
					inputFile >> garbage;//comma
					inputFile >> io_D; //ioD
					inputFile >> garbage;//bracket2
					IO_R_D* iod = new IO_R_D(io_R, io_D, pid);
					inputsigs->enqueue(iod);
					p->add_inputs_sigs(iod);
					if (i!=io_num-1)
					inputFile >> garbage;//comma

					//ior and mod can be initialised as zero, that way no error when no io requests.
				}
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
		NEW->peek(ptr1);
		temp = ProcessorsList->getfront();
		while (ptr1)
		{
			if(ptr1->GetArrTime()>time)
			{
				break;
			}
			NEW->dequeue(ptr1);
			processor* p2;
			p2 = temp->getItem();
			p2->AddProcess(ptr1);
			temp = temp->getNext();
			if (temp->getNext() == nullptr)
			{
				temp->setNext(ProcessorsList->getfront());
			}
			NEW->peek(ptr1);
		}
	}
	void RUNtoTRM()
	{
		Node <processor*>* p = ProcessorsList->getfront();
		while (p)
		{
			int c = p->getItem()->GetRun()->GetRemTime();
			if (c == 0)
			{
				TRM->enqueue(p->getItem()->GetRun());
				p->getItem()->GetRun()->finishTimes(time);
				p->getItem()->setstate(0);
				p->getItem()->SetRun();

			}
			else
			{
				p->getItem()->GetRun()->SetRemTime(c--);
			}
			p = p->getNext();
		}
	}
	void RUNtoBLK()
	{
		//ORASHY
	}
	void BLKtoRDY()
	{
		//orashy
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
			out << pr->getItem()->getID();
			if (pr->getNext() != nullptr)
			{
				out << " , ";
			}
			pr=pr->getNext();
		}
		out << endl;
		out << "-----------RUN PROCESSES----------" << endl;
		int x = s.CountRun();
		out << x << " RUN: ";
		Node <processor*>* pt = s.ProcessorsList->getfront();
		while (pt)
		{

			pt->getItem()->GetRun();
			if (pt)
			{
				out << pt->getItem()->GetRun()->getID() <<"(P"<<pt->getItem()->getpnumber() << ")";
			}
			if (pt->getNext() != nullptr)
			{
				out << ", ";
			}
			pt = pt->getNext();
		}
		out << endl;
		out << "----------TRM PROCESSES----------" << endl;
		Node<process*>* ptrm = s.TRM->getfront();
		out << s.TRM->getcount() << " TRM: ";
		while (ptrm)
		{
			out << ptrm->getItem()->getID();
			if (ptrm->getNext() != nullptr)
			{
				out << " , ";
			}
			ptrm = ptrm->getNext();
		}
		out << endl;
		out << "PRESS ANY KEY TO MOVE TO NEXT STEP!"<<endl;
		
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
	void print()
	{
		scheduler S = *this;
		cout << S;
	}


};

