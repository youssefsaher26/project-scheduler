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
#include "SJF.h"
#include"UI.h"
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
	int migRTF;
	int migMaxW;
	int forkedno;
	int killedno;
	int stolenno;
	int overheatdur;
	int removerheatdur;
	bool killed;
	QueueADT<kill*>* killsigs;
	QueueADT<IO_R_D*>* inputsigs;
	Node<processor*>* random;
	QueueADT<process*>* NEW;
	QueueADT<process*>* BLK;
	QueueADT<process*>* TRM;
	QueueADT<processor*>* ProcessorsList;
	UI* inter;
	processor* frozen;
public:
	scheduler();
	void overheat(processor* proc);
	int get_time();
	void simulator();
	void allmoving(processor* ptr);
	process* fork(int y);
	void CreateProcessors();
	void blockandtrm(processor* ptr);
	void forks(processor* ptr);
	bool DONE();
	void loadfile();
	void savefile();
	int get_Total_TRT();
	int get_Avg_WT();
	int get_Avg_RT();
	int get_Avg_TRT();
	void move1(process* p);
	void move2(process* p);
	void serve_heat();
	int CountRun() const;
	void NEWtoRDY();
	//simulator: generate the probability and take action accordingly
	void RUNtoTRM(processor* p);
	void RUNtoBLK(processor* p);
	void BLKtoRDY();
	processor* shortest_processor();
	processor* stl_shortest_processor();
	processor* longest_processor();
	processor* shortest_FCFS();
	processor* shortest_SJF();
	processor* shortest_RR();
	bool should_steal();
	int calclimit();
	void KILLSIG();
	void kill_children(process* p);
	process* kill1(int id);
	void steal();
	void print()
	{
		cout << *this;
	}
	friend ostream& operator<< (ostream& out, scheduler& s)
	{
		out << "Current TimeStep : " << s.time << endl;
		out << "----------RDY PROCESSES----------" << endl;
		Node <processor*>* p = s.ProcessorsList->getfront();
		int i = 1;
		while (p)
		{
			if (p->getItem() == s.frozen)
			{
				out << "Processor " << p->getItem()->getpnumber() << " ";
				if (p->getItem()->get_type() == 1)

					out << "[FCFS]: OVERHEAT!" << endl;
				else if (p->getItem()->get_type() == 2)
					out << "[RR]: OVERHEAT!" << endl;
				else
					out << "[SJF]: OVERHEAT!" << endl;
				p = p->getNext();
			}
			else if (p->getItem()->get_type() == 1)
			{
				out << "Processor " << p->getItem()->getpnumber() << " " << *(FCFS*)p->getItem() << endl;
				i++;
				p = p->getNext();
			}
			else if (p->getItem()->get_type() == 2)
			{
				out << "Processor " << p->getItem()->getpnumber() << " " << *(RoundRobin*)p->getItem() << endl;
				i++;
				p = p->getNext();
			}
			else
			{
				out << "Processor " << p->getItem()->getpnumber() << " " << *(SJF*)p->getItem() << endl;
				i++;
				p = p->getNext();
			}
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
			pr = pr->getNext();
		}
		out << endl;
		out << "-----------RUN PROCESSES----------" << endl;
		int x = s.CountRun();
		out << x << " RUN: ";
		Node <processor*>* pt = s.ProcessorsList->getfront();
		while (pt)
		{
			process* pp = pt->getItem()->GetRun();
			if (pp)
			{
				out << pt->getItem()->GetRun()->getID() << "(P" << pt->getItem()->getpnumber() << ")";
			}
			if (pt->getNext() != nullptr && pp != nullptr)
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
		return out;
	}
	~scheduler();
};

//QueueADT<processor*>* get_ProcessorsList()
//{
//	return ProcessorsList;
//}
//QueueADT<process*>* get_BLK()
//{
//	return BLK;
//}
//QueueADT<process*>* get_TRM()
//{
//	return TRM;
//}
/*void RUNtoRDY(processor* p)
	{
		process* run = p->GetRun();
		if (run != nullptr)
		{
			if (random == nullptr)
			{
				random = ProcessorsList->getfront();
			}
			random->getItem()->AddProcess(run);
			random = random->getNext();
			p->setstate(0);
			p->SetRun();
		}
	}*/
	//void deletelist(QueueADT<process*>*& q)
	//{
	//	Node<process*>* p = q->getfront();
	//	while (p)
	//	{
	//		delete p->getItem();
	//		p = p->getNext();
	//	}
	//}
/*void BLKtoRDY()
	{
		if (BLK->isEmpty() == false)
		{
			if (random == nullptr)
			{
				random = ProcessorsList->getfront();
			}
			process* item;
			BLK->dequeue(item);
			random->getItem()->AddProcess(item);
			random = random->getNext();
		}
	}*/
//void RUNtoTRM(processor* p)
	//{
	//	process* run = p->GetRun();
	//	if (run != nullptr)
	//	{
	//		TRM->enqueue(run);
	//		p->GetRun()->finishTimes(time);
	//		p->setstate(0);
	//		p->SetRun();
	//	}
	//}
/*void RUNtoBLK(processor* p)
	{
		process* run = p->GetRun();
		if (run != nullptr)
		{
			process* run = p->GetRun();
			BLK->enqueue(run);
			p->setstate(0);
			p->SetRun();
		}
	}*/