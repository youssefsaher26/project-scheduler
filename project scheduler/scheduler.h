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
	QueueADT<IO_R_D*>* inputsigs;
	Node<processor*>* random;
public:
	scheduler()
	{
		NEW = new QueueADT<process*>;
		BLK = new QueueADT<process*>;
		TRM = new QueueADT<process*>;
		ProcessorsList = new QueueADT<processor*>;
		killsigs = new QueueADT<kill*>;
		inputsigs = new QueueADT<IO_R_D*>;
		time = 1;
		STL = 0;
		forkprob = 0;
		FCFSno = 0;
		SJFno = 0;
		RRno = 0;
		
	}

	void simulator()
	{
		loadfile();
		CreateProcessors();
		random = ProcessorsList->getfront();
		while (DONE() == false)

		{
			Node <processor*>* p = ProcessorsList->getfront();
			while (p)
			{
				NEWtoRDY();
				processor* ptr = p->getItem();

				ptr->RDY_TO_RUN();
				if (p->getItem()->getstate() == 1)
				{
					if (p->getItem()->GetRun()->GetRemTime() == 0)
					{
						RUNtoTRM(ptr);
					}
					else
					{
						int Random = 1 + (rand() % 100);
						if (Random >= 1 && Random <= 15)
						{
							RUNtoBLK(ptr);
						}
						else if (Random >= 20 && Random <= 30)
						{
							RUNtoRDY(ptr);
						}
						else if (Random >= 50 && Random <= 60)
						{
							RUNtoTRM(ptr);
						}
						else
						{
							ptr->GetRun()->decremtime();
						}
					}
				}
				p = p->getNext();
			}
			int r = 1 + (rand() % 100);
			if (r < 10)
			{
				BLKtoRDY();
			}
			int idd = 1+(rand() % processno);
			FORCEDTRM(idd);
			print();
			cin.ignore();
			time++;
		}
		cout << "END OF SIMULATION" << endl;
	}
	void CreateProcessors()
	{
		processor* ptr;
		int id = 1;
		for (int i = 0; i < FCFSno; i++)
		{
			ptr = new FCFS(id);
			ProcessorsList->enqueue(ptr);
			id++;
		}
		for (int i = 0; i < RRno; i++)
		{
			ptr = new RoundRobin(id);
			ProcessorsList->enqueue(ptr);
			id++;
		}
		for (int i = 0; i < SJFno; i++)
		{
			ptr = new SJF(id);
			ProcessorsList->enqueue(ptr);
			id++;
		}
	}

	bool DONE()
	{
		if (NEW->isEmpty() == true && BLK->isEmpty()==true)
		{
			Node <processor*>* p = ProcessorsList->getfront();
			while (p)
			{
				if (p->getItem()->Done() == false)
					return false;
				p = p->getNext();
			}
			return true;
		}
		return false;
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
				process* p = new process(at, pid, ct, io_num);
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
	void FORCEDTRM(int ID)
	{
		FCFS* fcfs;//loop on processors. if fcfs, call forcedtrm,
		Node <processor*>* ptr = ProcessorsList->getfront();
		while (ptr) //loops on processors
		{
			int t=ptr->getItem()->get_type();
			if (t == 1) //only fcfs get past this point
			{
				fcfs=(FCFS*)ptr->getItem();
				process* pTrm=fcfs->ForcedTRM(ID);
				if (pTrm != nullptr)
				{
					TRM->enqueue(pTrm);
				}
			}
			ptr = ptr->getNext();
		}
	}
	void NEWtoRDY()
	{
		process* ptr1;
		Node<processor*> *temp;
			NEW->peek(ptr1);
			temp = ProcessorsList->getfront();
			while (ptr1 && NEW->isEmpty()!=true)
			{
					
						if (ptr1->GetArrTime() > time)
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
							temp = ProcessorsList->getfront();

						}

						NEW->peek(ptr1);
					
			}
	}
	//simulator: generate the probability and take action accordingly
	void RUNtoTRM(processor* p)
	{
		process* run = p->GetRun();
		if (run != nullptr)
		{
			TRM->enqueue(run);
			p->GetRun()->finishTimes(time);
			p->setstate(0);
			p->SetRun();
		}
	}
	void RUNtoBLK(processor* p)
	{
		process* run = p->GetRun();
		if (run != nullptr)
		{
			process* run = p->GetRun();
			BLK->enqueue(run);
			p->setstate(0);
			p->SetRun();
		}
	}
	void RUNtoRDY(processor* p)
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
	}
	void BLKtoRDY()
	{
		if (BLK->isEmpty() == false)
		{
			process* item;
			BLK->dequeue(item);
			if (random == nullptr)
			{
				random = ProcessorsList->getfront();
			}
			random->getItem()->AddProcess(item);
			random = random->getNext();
		}
	}

	friend ostream& operator<< (ostream& out, const scheduler& s)
	{
		out << "Current TimeStep : " << s.time << endl;
		out << "----------RDY PROCESSES----------" << endl;
		Node <processor*>* p = s.ProcessorsList->getfront();
		int i = 1;
		while (p)
		{
			
			if (p->getItem()->get_type() == 1)
			{
				out << "Processor " << p->getItem()->getpnumber() << " " << *(FCFS*)p->getItem() << endl;
				i++;
				p = p->getNext();
			}
			else if(p->getItem()->get_type()==2)
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
			pr=pr->getNext();
		}
		out << endl;
		out << "-----------RUN PROCESSES----------" << endl;
		int x = s.CountRun();
		out << x << " RUN: ";
		Node <processor*>* pt = s.ProcessorsList->getfront();
		while (pt)
		{
			process*pp = pt->getItem()->GetRun();
			if (pp)
			{
				out << pt->getItem()->GetRun()->getID() <<"(P"<<pt->getItem()->getpnumber() << ")";
			}
			if (pt->getNext()!= nullptr && pp!=nullptr)
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
		return out;
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

/*bool BLKtoRDY()
{
	Node <processor*>* p = ProcessorsList->getfront();
	int rem_IO = p->getItem()->GetRun()->get_rem_io();
	if (rem_IO==0)
	{
		process* ptr;
		BLK->dequeue(ptr);
		processor* shortpro= shortest_processor();
		shortpro->AddProcess(ptr);
	}
}
processor* shortest_processor()
{
	Node <processor*>* p = ProcessorsList->getfront();
	processor* min=p->getItem();
	while (p)
	{
		if (min > p->getItem())
		{
			min = p->getItem();
		}
		p = p->getNext();
	}
	return min;
 }*/
 //void RUNtoTRM()
	 //{
	 //	Node <processor*>* p = ProcessorsList->getfront();
	 //	while (p)
	 //	{
	 //		int c = p->getItem()->GetRun()->GetRemTime();
	 //		if (c == 0)
	 //		{
	 //			TRM->enqueue(p->getItem()->GetRun());
	 //			p->getItem()->GetRun()->finishTimes(time);
	 //			p->getItem()->setstate(0);
	 //			p->getItem()->SetRun();

	 //		}
	 //		else
	 //		{
	 //			p->getItem()->GetRun()->SetRemTime(c--);
	 //		}
	 //		p = p->getNext();
	 //	}
	 //}
	 // 

	 //bool RUNtoBLK()
	 //{
	 //	Node <processor*>* p = ProcessorsList->getfront();
	 //	int c_iors = p->getItem()->GetRun()->get_inputsigs()->getcount();//count of iors
	 //	if (c_iors == 0)
	 //	{
	 //		return false;
	 //	}
	 //	while (p)
	 //		{
	 //			int ct = p->getItem()->GetRun()->CpuTime;
	 //			int rem = p->getItem()->GetRun()->GetRemTime();
	 //			int io_r = p->getItem()->GetRun()->get_ior();
	 //			if ((ct - rem) == io_r)
	 //			{
	 //				p->getItem()->GetRun()->set_remIO(p->getItem()->GetRun()->get_iod());
	 //				BLK->enqueue(p->getItem()->GetRun());
	 //				p->getItem()->SetRun();
	 //				p->getItem()->setstate(0);
	 //			}
	 //			p = p->getNext();
	 //		}
	 //}