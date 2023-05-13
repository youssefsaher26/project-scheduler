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
	int migRTF;
	int migMaxW;
	int forkedno;
	int killedno;
	int stolenno;
	QueueADT<process*>* NEW;
	QueueADT<process*>* BLK;
	QueueADT<process*>* TRM;
	QueueADT<processor*>* ProcessorsList;
	QueueADT<kill*>* killsigs;
	QueueADT<IO_R_D*>* inputsigs;
	Node<processor*>* random;
	//UI* inter;
public:
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
		stolenno = 0;
		forkprob = 0;
		FCFSno = 0;
		SJFno = 0;
		RRno = 0;
		migRTF = 0;
		migMaxW = 0;
		forkedno = 0;
		killedno = 0;

	}
	int get_time()
	{
		return time;
	}
	void simulator()
	{
		loadfile();
		CreateProcessors();
		//inter->MODE();
		while (DONE() == false)
		{
			Node <processor*>* p = ProcessorsList->getfront();
			while (p)
			{
				NEWtoRDY();
				processor* ptr = p->getItem();
				ptr->time(time);
				KILLSIG();
				steal();
				ptr->SchedAlgo();
				forks(ptr);
				allmoving(ptr);
				blockandtrm(ptr);
				p->getItem()->STATE();
				//p->calculations();
				p = p->getNext();
			}
			BLKtoRDY();
			//inter->UIprint();
			print();
			cin.ignore();
			time++;
		}
		//finalcalculations
		savefile();
		cout << "END OF SIMULATION" << endl;
	}
	void allmoving(processor* ptr)
	{
		if (ptr->getmigrate() != nullptr)
		{
			if (ptr->get_type() == 2)
			{

				move1(ptr->getmigrate());
			}
			else if (ptr->get_type() == 1)
			{
				move2(ptr->getmigrate());
			}
			ptr->resetmigrate();
		}
	}
	process* fork(int y)
	{
		processno++;
		process* p = new process(time,processno,y,0,false);
		processor* pro = shortest_FCFS();
		pro->AddProcess(p);
		forkedno++;
		return p;
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
	void blockandtrm(processor* ptr)
	{
		if (ptr->getblock())
		{
			RUNtoBLK(ptr);
		}
		else if (ptr->gettrm())
		{
			RUNtoTRM(ptr);
		}
	}
	void forks(processor* ptr)
	{
		if (ptr->get_type() == 1)
		{
			if (ptr->get_forkit() == true && ptr->GetRun())
			{
				process* child = fork(ptr->GetRun()->GetRemTime());
				ptr->GetRun()->setchild(child);
				ptr->resetFork();
			}
			//see if it needs forking, if yes, call fork in scheduler, then revert the bool back
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
		outputFile << "TT" << '\t' << "PID" << '\t' << "AT" << '\t' << "CT" << '\t' << "IO_D" << '\t' << "WT" << '\t' << "RT" << '\t' << "TRT" << '\n';
		Node<process*>* p_out = TRM->getfront();
		while (p_out)
		{
			outputFile << p_out->getItem()->get_TT() << '\t';
			outputFile << p_out->getItem()->getID() << '\t';
			outputFile << p_out->getItem()->GetArrTime() << '\t';
			int x = p_out->getItem()->get_CT();
			outputFile << x << '\t';
			if (p_out->getItem()->get_total_io_D() != 0)
				outputFile << p_out->getItem()->get_total_io_D() << '\t';
			else
				outputFile << "-" << '\t';
			outputFile << p_out->getItem()->get_WT() << '\t';
			outputFile << p_out->getItem()->get_RT() << '\t';
			outputFile << p_out->getItem()->get_TRT() << '\t';
			outputFile << "\n";
			p_out = p_out->getNext();
		}
			outputFile << "---------------------------------------" << '\n';
			outputFile << "Processes: " << processno << '\n';
			outputFile << "Avg WT = " << get_Avg_WT() << "," << '\t';
			outputFile << "Avg RT = " << get_Avg_RT() << "," << '\t';
			outputFile << "Avg TRT = " << get_Avg_TRT() << "," << '\n';
			int no1 = ((migRTF * 100) / processno);
			int no2 = ((migMaxW * 100) / processno);
			outputFile << "Migration %:" << '\t' << "RTF = " << no1 << ",   " << '\t' << "MaxW = " << no2 << '\n';
			int no3 = ((stolenno * 100) / processno);
			outputFile << "Work Steal %: " << no3 << '\n';
			int no4 = ((forkedno * 100) / processno);
			outputFile << "Forked Process: " << no4 << '\t';
			int no5 = ((killedno * 100) / processno);
			outputFile << "Killed Process: " << no5 << '\n' << '\n';
			int processor_no = FCFSno + SJFno + RRno;
			outputFile << "Processors: " << processor_no;
			outputFile << " [ " << FCFSno << " FCFS" << ", " << SJFno << " SJF, " << RRno << " RR ]" << '\n';
			outputFile << "Processors Load" << '\n';
		
			Node<processor*>* temp = ProcessorsList->getfront();
			int i=0;
			while (temp)
			{
				outputFile << "p" << i << " = " << temp->getItem()->pLoad(get_Total_TRT())<< "%";
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
					if(ptr)
					sum_util= ptr->getItem()->pUtil() + sum_util;
				}
				outputFile << '\n';
				avg_util = sum_util / processor_no;
				outputFile << "Avg utilization = " << avg_util << "%";
				outputFile.close();
	}
	int get_Total_TRT()
	{
		int total=0;
		Node<process*>* temp = TRM->getfront();
		while(temp)
		{ 
			total = total + temp->getItem()->get_TRT();
			temp = temp->getNext();
		}
		return total;
	}
	int get_Avg_WT()
	{
		int sum=0, avg=0;
		Node<process*>* ptr = TRM->getfront();
		while (ptr)
		{
			sum=ptr->getItem()->get_WT() + sum;
			ptr = ptr->getNext();
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
			ptr = ptr->getNext();
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
			ptr = ptr->getNext();
		}
		avg = sum / processno;
		return avg;
	}
	void KILLSIG()
	{
		kill* k;
		killsigs->peek(k);
		if (killsigs->isEmpty() == false)
		{
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
							pTrm->finish_Kill_Times(time);
							killedno++;
							killsigs->dequeue(k);
							process* child = pTrm->get_child();
							while (child)
							{
								int id = child->getID();
								Node<processor*>* pro = ProcessorsList->getfront();
								while (pro)
								{
									if (pro->getItem()->get_type() != 1)
									{
										break;
									}
									process* c = pro->getItem()->KILL(id);
									if (c)
									{
										TRM->enqueue(c);
										c->finish_Kill_Times(time);
										killedno++;
										break;
									}
									pro = pro->getNext();
								}
								child = child->get_child();
							}
						}
					}
					p = p->getNext();
				}
				kill* k1;
				killsigs->peek(k1);
				if (k == k1)
				{
					killsigs->dequeue(k1);
				}
				killsigs->peek(k);
				if (killsigs->peek(k) == false)
				{
					break;
				}
			}
		}
	}
	void move1(process* p)
	{
		processor* ssjf = shortest_SJF();
		if (ssjf)
		{
			ssjf->AddProcess(p);
			migRTF++;
		}
	}
	void move2(process* p)
	{
		processor* srr = shortest_RR();
		if (srr)
		{
			srr->AddProcess(p);
			migMaxW++;
		}
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
		process* ptr = p->gettrm();
		TRM->enqueue(ptr);
		ptr->finishTimes(time);
		process* child = ptr->get_child();
		while (child)
		{
			int id = child->getID(); 
			Node<processor*>* pro = ProcessorsList->getfront();
			while (pro)
			{
				if (pro->getItem()->get_type()!=1)
				{
					break;
				}
				process* c = pro->getItem()->KILL(id);
				if (c)
				{
					TRM->enqueue(c);
					c->finish_Kill_Times(time);
				}
				pro = pro->getNext();
			}
			child = child->get_child();
		}
		p->resettrm();
	}
	void RUNtoBLK(processor* p)
	{
		process* ptr = p->getblock();
		BLK->enqueue(ptr);
		p->resetblock();
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
			process* ptr;
			BLK->peek(ptr);
			int rem_IO = ptr->get_rem_io();
			if (rem_IO == 0)
			{
				IO_R_D* io;
				ptr->get_inputsigs()->dequeue(io);
				ptr->set_total_io_D(io->get_iod());
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
			if (min->queuetime() > p->getItem()->queuetime())
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
		return min;
	}
	processor* longest_processor()
	{
		Node <processor*>* p = ProcessorsList->getfront();
		processor* max = p->getItem();
		while (p)
		{
			if (max->queuetime() < p->getItem()->queuetime())
			{
				max = p->getItem();
			}
			p = p->getNext();
		}
		return max;
	}
	processor* shortest_FCFS()
	{
		if (FCFSno == 0)
		{
			return nullptr;
		}
		Node <processor*>* p = ProcessorsList->getfront();
		processor* min = p->getItem();
		while (p)
		{
			if (min->queuetime() > p->getItem()->queuetime() && p->getItem()->get_type() == 1)
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
		return min;
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
			if (min->queuetime() > p->getItem()->queuetime())
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
		return min;
	}
	processor* shortest_RR()
	{
		if(RRno==0)
		{
			return nullptr;
		}

		Node <processor*>* p = ProcessorsList->getfront();
		while (p->getItem()->get_type() != 2)
		{
			p = p->getNext();
		}
		//p=the first rr
		processor* min = p->getItem();
		while (p)
		{ 
			if (p->getItem()->get_type() == 2)
			{
				
				if (min->queuetime() > p->getItem()->queuetime())
				{
					min = p->getItem();
				}
			}
			p = p->getNext();
		}
		return min;

	}
	bool should_steal()
	{
		if (time % STL == 0)
		{
			int lim = calclimit();
			if (lim > 40)
			{
				return true;
			}
		}
		return false;
	}
	int calclimit()
	{
		int LQF = longest_processor()->queuetime();
		int SQF = shortest_processor()->queuetime();
		return(((LQF - SQF) / LQF) * 100);
	}
	void steal()
	{
		bool y = should_steal();
		while (y == true)
		{
			processor* longest = longest_processor();
			processor* shortest = shortest_processor();
			process* p = longest->donate();
			if (p)
			{
				shortest->AddProcess(p);
				stolenno++;
			}
			y = should_steal();
		}
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
	~scheduler()
	{
		delete TRM;
		delete NEW;
		delete ProcessorsList;
		delete BLK;
		delete killsigs;
		delete inputsigs;
	}
	//void deletelist(QueueADT<process*>*& q)
	//{
	//	Node<process*>* p = q->getfront();
	//	while (p)
	//	{
	//		delete p->getItem();
	//		p = p->getNext();
	//	}
	//}
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