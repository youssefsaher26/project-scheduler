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
#include"EDF.h"
#include<fstream>
#include "kill.h"
#include"IO_R_D.h"
#include "SJF.h"
#include"UI.h"
using namespace std;
class scheduler
{
private:
	int count_edf; //no of processes ended before deadline
	int Time; //timestep
	int STL; //steal limit
	int forkprob; //forking procbability
	int processno; //number of processes
	int FCFSno; //number of FCFS processors
	int SJFno;//number of SJF processors
	int RRno;//number of RR processors
	int EDFno;//number of EDF processors
	int migRTF;//number of processes migrated due to RTF
	int migMaxW;//number of processes migrated due to MAXW
	int forkedno;//number of forked processes
	int killedno; //number of killed processes
	int stolenno;//number of stolen processes
	int overheatdur; //overheat duration loaded from input file
	int removerheatdur; //remaining overheat duration
	QueueADT<kill*>* killsigs;
	QueueADT<IO_R_D*>* inputsigs;
	QueueADT<process*>* NEW;
	QueueADT<process*>* BLK;
	QueueADT<process*>* TRM;
	QueueADT<processor*>* ProcessorsList;
	UI* inter; //pointer from interface class
	processor* frozen;
public:
	scheduler(); 
	void simulator(); //responsible for calling all functions to simulate the program
	void allmoving(processor* ptr); //function that calls all the moving functions in simulator, move1 or move2
	void CreateProcessors(); //called in scheduler to create the processors
	void blockandtrm(processor* ptr); //called in simulator to call all functions related to blocking and terminating
	process* fork(int y, int z); //function that creates a process with cpu time y and edf z and adds it to shortest FCFS
	void forks(processor* ptr); //checks if the processor is fcfs, and checks the fork_it boolean to see if it needs to be forked
	//if a process needs to be forked, fork(int z, int y) is called to create the process and handle it, then the 
	//fork_it boolean in that processor is reset.
	bool DONE();//checks if all processes are done and processors are empty
	void loadfile();//loading input file
	void savefile();//saving output file
	int get_Total_TRT();
	int get_Avg_WT();
	int get_Avg_RT();
	int get_Avg_TRT();
	void move1(process* p); //moves process to shortest SJF from RR, migration due to RTF
	void move2(process* p); //moves process to Shortest RR from FCFS, migration due to MaxW
	int CountRun() const;
	void NEWtoRDY();
	//simulator: generate the probability and take action accordingly
	void RUNtoTRM(processor* p);//move process from RUN state to TRM list
	void RUNtoBLK(processor* p);//move process from RUN to BLK List
	void BLKtoRDY(); //return process from BLK to shortest RDY List
	processor* shortest_processor(); //returns pointer to the shortest processor shortest rdy plus run
	processor* stl_shortest_processor(); //shortest RDY list
	processor* longest_processor();
	processor* shortest_FCFS();//returns pointer to the shortest FCFS processor
	processor* shortest_SJF();//returns pointer to the shortest SJF processor
	processor* shortest_RR();//returns pointer to the shortest RR processor
	void KILLSIG(); //checks the KILLSIGSQUEUE and calls kill(int id) function to kill that process
	void kill_children(process* p); //kills children of the process recursively by calling kill1
	process* kill1(int id); //takes the process id and loops on all fcfs processors and calls FCFS::KILL(int id)
	//to see if the process is there and removes it, and calls killchildren(process* p)
	bool should_steal(); //boolean calculated each time step to see if time is multiple of STL, and calculate the steal limit
	int calclimit();//caclulates steal limit
	bool stealagain(processor *p1, processor *p2);
	void steal(); //performs the stealing process
	void overheat(processor* proc); //checks if the processor has overheated
	void serve_heat(); //serves overheated process, either decrements the removerheatdur or releases the processor from 
	//the overheat state
	friend ostream& operator<< (ostream& out, scheduler& s)
	{
		out << "Current TimeStep : " << s.Time << endl;
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
				else if (p->getItem()->get_type() == 3)
					out << "[SJF]: OVERHEAT!" << endl;
				else
					out << "[EDF]: OVERHEAT!" << endl;
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
			else if(p->getItem()->get_type() == 3)
			{
				out << "Processor " << p->getItem()->getpnumber() << " " << *(SJF*)p->getItem() << endl;
				i++;
				p = p->getNext();
			}
			else 
			{
				out << "Processor " << p->getItem()->getpnumber() << " " << *(EDF*)p->getItem() << endl;
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