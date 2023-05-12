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
#include "UI.h"
#pragma once
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
	//UI* inter;
public:
	QueueADT<processor*>* get_ProcessorsList()
	{
		return ProcessorsList;
	}
	QueueADT<process*>* get_BLK()
	{
		return BLK;
	}
	QueueADT<process*>* get_TRM()
	{
		return TRM;
	}
	int get_time()
	{
		return time;
	}
	scheduler()
	{
		NEW = new QueueADT<process*>;
		BLK = new QueueADT<process*>;
		TRM = new QueueADT<process*>;
		ProcessorsList = new QueueADT<processor*>;
		killsigs = new QueueADT<kill*>;
		inputsigs = new QueueADT<IO_R_D*>;
		//inter = new UI(this);
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
		//inter->MODE();
		random = ProcessorsList->getfront();
		while (DONE() == false)
		{
			Node <processor*>* p = ProcessorsList->getfront();
			while (p)
			{
				NEWtoRDY();
				processor* ptr = p->getItem();
				ptr->SchedAlgo();
				if (ptr->getmigrate() != nullptr)
				{
					if (ptr->get_type() == 3)
					{
						move1(ptr->getmigrate());
					}
					else if (ptr->get_type() == 1)
					{
						//move2
					}
					ptr->resetmigrate();
				}
				if (ptr->get_type() == 1)
				{
					if (ptr->get_forkit() == true)
					{

						fork(ptr->GetRun()->GetRemTime());
						ptr->resetFork();
					}			
					//see if it needs forking, if yes, call fork in scheduler, then revert the bool back
				}
				if (ptr->Block)
				{
					RUNtoBLK(ptr);
				}
				else if (ptr->Terminate)
				{
					RUNtoTRM(ptr);
				}
				p->getItem()->STATE();
				p = p->getNext();
			}
			//inter->UIprint();
			print();
			cin.ignore();
			time++;
		}
		savefile();
		cout << "END OF SIMULATION" << endl;
	}
	void fork(int y)
	{
		processno++;
		process* p = new process(time,processno,y,0);
		processor* pro = shortest_FCFS();
		pro->AddProcess(p);
	}
	void CreateProcessors()
	{
		processor* ptr;
		int id = 1;
		for (int i = 0; i < FCFSno; i++)
		{
			ptr = new FCFS(id,forkprob);
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
		if (NEW->isEmpty() == true && BLK->isEmpty() == true)
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
		cout << "Processing" << endl;
		inputFile >> FCFSno >> SJFno >> RRno;
		inputFile >> t_slice;
		RoundRobin::set_Timeslice(t_slice);
		inputFile >> rtf;
		RoundRobin::set_RTF(rtf);
		inputFile >> Maxw;
		FCFS::set_Maxw(Maxw);
		inputFile >> STL >> forkprob;
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
				if (i != io_num - 1)
					inputFile >> garbage;//comma

				//ior and mod can be initialised as zero, that way no error when no io requests.
			}
			NEW->enqueue(p);
		}
		int SigId, SigT;
		while (!inputFile.eof())
		{
			inputFile >> SigT >> SigId;
			kill* k = new kill(SigT, SigId);
			killsigs->enqueue(k);
		}

		outputFile.close();
	}
	void savefile()
	{
		
		ofstream outputFile;
		outputFile.open("Output.txt", ios::out);
		outputFile << "TT" << '\t' << "PID" << '\t' << "AT" << '\t' << "CT" << '\t' << "WT" << '\t' << "RT" << '\t' << "TRT" << '\t';
		Node<process*>* p_out = TRM->getfront();
		while (p_out)
		{
			outputFile << p_out->getItem()->get_TT()<< '\t';
			outputFile << p_out->getItem ()->getID() << '\t';
			outputFile << p_out->getItem()->get_CT()<<'\t';
			outputFile << p_out->getItem()->get_iod() << '\t';
			outputFile << p_out->getItem()->get_WT()<<'\t';
			outputFile << p_out->getItem()->get_RT()<<'\t';
			outputFile << p_out->getItem()->get_TRT()<< '\t';
			p_out = p_out->getNext();
		}
		outputFile << processno<<'\n';
		outputFile << "Avg WT = " << get_Avg_WT() << "," << '\t';
		outputFile << "Avg RT = " << get_Avg_RT() << "," << '\t';
		outputFile << "Avg TRT = " << get_Avg_TRT() << "," << '\n';
		int no1=0;//(no migrated due to RTF / total no.)//
		int no2=0;//(no migrated due to MaxW / total no.)//
		outputFile << "Migration %:" << '\t' << "RTF = " << no1 << ",   " <<'\t'<<"MaxW = " << no2<<'\n';
		int no3=0;//(no stolen / total no.)//
		outputFile << "Work Steal %: " << no3<<'\n';
		int no4=0;//(no forked / total no.)//
		outputFile << "Forked Process: " << no4;
		int no5=0; //(no of killed / total no.)//
		outputFile << "Killed Process: " << no5<<'\n'<<'\n';
		int processor_no = FCFSno + SJFno + RRno;
		outputFile << "Processors: " << processor_no;
		outputFile << " [ " << FCFSno << " FCFS" << ", " << SJFno << " SJF, " << RRno << " RR ]"<<'\n';
		outputFile << "Processors Load" << '\n';

		Node<processor*>* temp = ProcessorsList->getfront();
		int i=0;
		while (temp)
		{
			outputFile << "p" << i << " = " << temp->getItem()->pLoad()<< "%";
			outputFile << "," << '\t';
			temp = temp->getNext();
			i++;
		}
		outputFile << '\n';
		outputFile << "Processors Utiliz" << '\n';
		Node<processor*>* ptr = ProcessorsList->getfront();
			int n=0,sum_util=0,avg_util=0;
			while (ptr)
			{
				outputFile << "p" << n << " = " << ptr->getItem()->pUtil() << "%";
				outputFile << "," << '\t';
				ptr = ptr->getNext();
				n++;
				sum_util= ptr->getItem()->pUtil() + sum_util;
			}
			outputFile << '\n';
			avg_util = sum_util / processor_no;
			outputFile << "Avg utilization = " << avg_util << "%";
			outputFile.close();
	}
	int get_Avg_WT()
	{
		int sum=0, avg=0;
		Node<process*>* ptr = TRM->getfront();
		while (ptr)
		{
			sum=ptr->getItem()->get_WT() + sum;
		}
		avg=sum / processno;
		return avg;
	}
	int get_Avg_RT()
	{
		int sum = 0, avg = 0;
		Node<process*>* ptr = TRM->getfront();
		while (ptr)
		{
			sum = ptr->getItem()->get_RT() + sum;
		}
		avg = sum / processno;
		return avg;
	}
	int get_Avg_TRT()
	{
		int sum = 0, avg = 0;
		Node<process*>* ptr = TRM->getfront();
		while (ptr)
		{
			sum = ptr->getItem()->get_TRT() + sum;
		}
		avg = sum / processno;
		return avg;
	}
	void KILLSIG()
	{
		kill* k;
		killsigs->peek(k);
		while (k->getkiltime() == time)
		{
			Node<processor*>* p = ProcessorsList->getfront();
			while (p)
			{
				processor* ptr = p->getItem();
				if (ptr->get_type() == 1)
				{
					process* pTrm = ptr->KILL(k->getkillid());
					if (pTrm != nullptr)
					{
						TRM->enqueue(pTrm);
						killsigs->dequeue(k);
					}
				}
				p = p->getNext();
			}
			killsigs->peek(k);
		}
	}
	void move1(process* p)
	{
		processor* ssjf = shortest_SJF();
		ssjf->AddProcess(p);
	}
	void NEWtoRDY()
	{
		process* ptr1;
		NEW->peek(ptr1);
		while (ptr1 && NEW->isEmpty() != true)
		{

			if (ptr1->GetArrTime() > time)
			{
				break;
			}
			NEW->dequeue(ptr1);
			processor* p2=shortest_processor();
			p2->AddProcess(ptr1);
			NEW->peek(ptr1);

		}
	}
	//simulator: generate the probability and take action accordingly
	void RUNtoTRM(processor* p)
	{
		if (p->GetRun() != nullptr)
		{
			process* ptr = p->GetRun();
			TRM->enqueue(ptr);
			ptr->finishTimes(time);
			ptr->setstate(0);
			p->SetRun();
		}
	}
	void RUNtoBLK(processor* p)
	{
		process* ptr = p->GetRun();
		if (ptr)
		{
			int c_iors = ptr->get_inputsigs()->getcount();//count of iors
			if (c_iors == 0)
			{
				return ;
			}
			int ct = ptr->CpuTime;
			int rem = ptr->GetRemTime();
			int io_r = ptr->get_ior();
			if ((ct - rem) == io_r)
			{
				ptr->set_remIO(ptr->get_iod());
				BLK->enqueue(ptr);
				p->SetRun();
				p->setstate(0);
			}
		}
	}
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
	void BLKtoRDY()
	{
		if (BLK->isEmpty() == false)
		{
			int rem_IO = BLK->getfront()->getItem()->get_rem_io();
			process* ptr;
			BLK->peek(ptr);
			if (rem_IO == 0)
			{
				BLK->dequeue(ptr);
				processor* shortpro = shortest_processor();
				shortpro->AddProcess(ptr);
			}
			else
			{
				ptr->dec_IO();
			}
		}
	}
	processor* shortest_processor()
	{
		Node <processor*>* p = ProcessorsList->getfront();
		processor* min = p->getItem();
		while (p)
		{
			if (min > p->getItem())
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
		return min;
	}
	processor* shortest_FCFS()
	{
		Node <processor*>* p = ProcessorsList->getfront();
		processor* min = p->getItem();
		while (p)
		{
			if (min > p->getItem()&&p->getItem()->get_type()==1)
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
		return min;
		//if (FCFSno == 0)
		//{
		//	return nullptr;
		//}
		//Node <processor*>* p = ProcessorsList->getfront();
		//while (p->getItem()->get_type() != 1)
		//{
		//	p = p->getNext();
		//}
		//processor* min = p->getItem();
		//while (p)
		//{
		//	if (min > p->getItem())
		//	{
		//		min = p->getItem();
		//	}
		//	p = p->getNext();
		//}
		//return min;
	}
	processor* shortest_SJF()
	{
		if(SJFno==0)
		{
			return nullptr;
		}
		Node <processor*>* p = ProcessorsList->getfront();
		while (p->getItem()->get_type()!=3)
		{
			p = p->getNext();
		}
		//p=the first sjf
		processor* min = p->getItem();
		while (p)
		{
			if (min > p->getItem())
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
		return min;
	}
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
};


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