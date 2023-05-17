#include "scheduler.h"

scheduler::scheduler()
{
	NEW = new QueueADT<process*>;
	BLK = new QueueADT<process*>;
	TRM = new QueueADT<process*>;
	ProcessorsList = new QueueADT<processor*>;
	killsigs = new QueueADT<kill*>;
	inputsigs = new QueueADT<IO_R_D*>;
	inter = new UI(this);
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
	removerheatdur = 0;
	count_edf = 0;
	frozen = nullptr;
	killed = false;
}
void scheduler::simulator()
{
	inter->UIbegin();
	loadfile();
	CreateProcessors();
	inter->MODE();
	while (DONE() == false)
	{
		serve_heat();
		Node <processor*>* p = ProcessorsList->getfront();
		while (p)
		{
			if (p->getItem() == frozen)
				p = p->getNext();
			if (p)
			{
				if (!frozen)
					overheat(p->getItem());
				NEWtoRDY();
				processor* ptr = p->getItem();
				ptr->time(time);
				KILLSIG();
				ptr->SchedAlgo();
				forks(ptr);
				allmoving(ptr);
				blockandtrm(ptr);
				p->getItem()->STATE();
				p = p->getNext();
			}
		}
		BLKtoRDY();
		steal();
		inter->UIprint();
		time++;
	}
	inter->UIend();
	savefile();

}
void scheduler::CreateProcessors()
{
	processor* ptr;
	int id = 1;
	for (int i = 0; i < FCFSno; i++)
	{
		ptr = new FCFS(id, forkprob);
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
	for (int i = 0; i < EDFno; i++)
	{
		ptr = new EDF(id);
		ProcessorsList->enqueue(ptr);
		id++;
	}
}
void scheduler::overheat(processor *proc)
{
	if ( ProcessorsList->getcount() == 1)
	{
		return;
	}
	if (frozen == nullptr)
	{
		if (proc->get_type() == 1 && FCFSno == 1) //corner case: overheat fcfs and only one fcfs processor so children cant migrate
		{
			return;
		}
		if (time > 50)
		{
			int p = rand() % 100 + 1;
			if (p > 85 && proc->getstate() == 1)
			{
				frozen = proc;
				process* don = proc->GetRun();
				if (don)
				{
					if (don->get_pure() == 0)
						shortest_FCFS()->AddProcess(don);
					else
						shortest_processor()->AddProcess(don);
					proc->SetRun();
				}
				don = proc->donate();
				while (don)
				{
					if (don->get_pure() == 0)
						shortest_FCFS()->AddProcess(don);
					else
						shortest_processor()->AddProcess(don);
					don = proc->donate();
				}
				removerheatdur = overheatdur;
			}
		}
	}
	
}
void scheduler::serve_heat()
{
	if (frozen)
	{
		removerheatdur--;
		if (removerheatdur == 0)
		{
			frozen = nullptr;
		}
	}
}
void scheduler::allmoving(processor* ptr)
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
void scheduler:: blockandtrm(processor* ptr)
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
void scheduler:: forks(processor* ptr)
{
	if (ptr->get_type() == 1)
	{
		if (ptr->get_forkit() == true && ptr->GetRun())
		{
			process* child = fork(ptr->GetRun()->GetRemTime(), ptr->GetRun()->get_EDF());
			if (ptr->GetRun()->get_children_no() == 0)
				ptr->GetRun()->setchild1(child);
			else
				ptr->GetRun()->setchild2(child);
			ptr->resetFork();
			ptr->GetRun()->inc_children();
		}
		//see if it needs forking, if yes, call fork in scheduler, then revert the bool back
	}
}
process* scheduler::fork(int y, int z)
{
	processno++;
	process* p = new process(time, processno, y, 0, z, false);
	processor* pro = shortest_FCFS();
	pro->AddProcess(p);
	forkedno++;
	return p;
}
bool scheduler:: DONE()
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
void scheduler:: loadfile()
{
	char garbage;
	ofstream outputFile;
	ifstream inputFile;
	inputFile.open("test.txt", ios::in);
	int rtf, t_slice, Maxw;
	inputFile >> FCFSno >> SJFno >> RRno>> EDFno;
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
		int at, pid, ct, io_num, io_R, io_D,edf;
		inputFile >> at >> pid >> ct >> edf>> io_num;
		process* p = new process(at, pid, ct, io_num,edf);
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
	inputFile >> overheatdur;
	int SigId, SigT;
	while (!inputFile.eof())
	{
		inputFile >> SigT >> SigId;
		kill* k = new kill(SigT, SigId);
		killsigs->enqueue(k);
	}
	outputFile.close();
}
void scheduler:: savefile()
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
	outputFile << "Migration:" << '\t' << "RTF = " << no1 << "%" << ",   " << '\t' << "MaxW = " << no2 << "%" << '\n';
	int no3 = ((stolenno * 100) / processno);
	outputFile << "Work Steal: " << no3 << "%" << '\n';
	int no4 = ((forkedno * 100) / processno);
	outputFile << "Forked Process: " << no4 << "%"<< '\t';
	int no5 = ((killedno * 100) / processno);
	outputFile << "Killed Process: " << no5 << "%" << '\n' << '\n';
	int processor_no = FCFSno + SJFno + RRno+ EDFno;
	outputFile << "Processors: " << processor_no;
	outputFile << " [ " << FCFSno << " FCFS" << ", " << SJFno << " SJF, " << RRno << " RR, "<< EDFno<<" EDF]" << '\n';
	outputFile << "Processors Load" << '\n';

	Node<processor*>* temp = ProcessorsList->getfront();
	int i = 1;
	while (temp)
	{
		outputFile << "p" << i << " = " << temp->getItem()->pLoad(get_Total_TRT()) << "%";
		outputFile << "," << '\t';
		temp = temp->getNext();
		i++;
	}
	outputFile << '\n';
	outputFile << "Processors Utiliz" << '\n';
	Node<processor*>* ptr = ProcessorsList->getfront();
	int n = 1, sum_util = 0, avg_util = 0;
	while (ptr)
	{
		outputFile << "p" << n << " = " << ptr->getItem()->pUtil() << "%";
		outputFile << "," << '\t';
		ptr = ptr->getNext();
		n++;
		if (ptr)
			sum_util = ptr->getItem()->pUtil() + sum_util;
	}
	outputFile << '\n';
	avg_util = sum_util / processor_no;
	outputFile << "Avg utilization = " << avg_util << "%"<<'\n';
	outputFile << "Percentage of Processes ended before EDF: " << (100*count_edf/processno)<<" %"<<'\n';
	outputFile.close();
}
int scheduler::get_time()
{
	return time;
}
int scheduler:: get_Total_TRT()
{
	int total = 0;
	Node<process*>* temp = TRM->getfront();
	while (temp)
	{
		total = total + temp->getItem()->get_TRT();
		temp = temp->getNext();
	}
	return total;
}
int scheduler:: get_Avg_WT()
{
	int sum = 0, avg = 0;
	Node<process*>* ptr = TRM->getfront();
	while (ptr)
	{
		sum = ptr->getItem()->get_WT() + sum;
		ptr = ptr->getNext();
	}
	avg = sum / processno;
	return avg;
}
int scheduler:: get_Avg_RT()
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
int scheduler:: get_Avg_TRT()
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
process* scheduler::kill1(int id)
{
	Node<processor*>* p = ProcessorsList->getfront();
	while (p)
	{
		processor* ptr = p->getItem();
		if (ptr->get_type() == 1)
		{
			process* pTrm = ptr->KILL(id);
			if (pTrm != nullptr)
			{
				TRM->enqueue(pTrm);
				pTrm->finish_Kill_Times(time);
				if (pTrm->get_TT() < pTrm->get_EDF())
					count_edf++;
				killedno++;
				return pTrm;
			}
		}
		else
			return nullptr;
		p = p->getNext();
		//because fcfs are at the beginning of the processor list, so if the ptr isnt fcfs then
		//there are no fcfs coming up.
	}
}
void scheduler::kill_children(process* p)
{
	if (p == nullptr)
		return;
	kill1(p->getID());
	kill_children(p->get_child1());
	kill_children(p->get_child2());
}
void scheduler:: KILLSIG()
{
	kill* k = nullptr;
	killsigs->peek(k);
	if (killsigs->isEmpty() == false)
	{
		while (k->getkiltime() == time)
		{
			process* p=kill1(k->getkillid());
			kill_children(p);
			killsigs->dequeue(k);
			if (killsigs->peek(k) == false)
				break;
			killsigs->peek(k);
		}
	}
} //check overheat  //check overheat 
void scheduler:: move1(process* p)
{
	processor* ssjf = shortest_SJF();
	if (ssjf)
	{
		ssjf->AddProcess(p);
		migRTF++;
	}
}
void scheduler:: move2(process* p)
{
	processor* srr = shortest_RR();
	if (srr)
	{
		srr->AddProcess(p);
		migMaxW++;
	}
}
void scheduler:: NEWtoRDY()
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
		processor* p2 = shortest_processor();
		p2->AddProcess(ptr1);
		NEW->peek(ptr1);

	}
}
void scheduler:: RUNtoTRM(processor* p)
{
	process* ptr = p->gettrm();
	TRM->enqueue(ptr);
	ptr->finishTimes(time);
	if (ptr->get_TT() < ptr->get_EDF())
		count_edf++;
	kill_children(ptr);
	p->resettrm();
}
void scheduler:: RUNtoBLK(processor* p)
{
	process* ptr = p->getblock();
	BLK->enqueue(ptr);
	p->resetblock();
}
void scheduler:: BLKtoRDY()
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
processor* scheduler:: shortest_processor()
{
	Node <processor*>* p = ProcessorsList->getfront();
	if (p->getItem() == frozen)
		p = p->getNext();
	if (!p)
	{
		return nullptr;
	}
	processor* min = p->getItem();
	while (p)
	{
		if (p->getItem() == frozen)
			p = p->getNext();
		if (p)
		{
			if (min->queuetime() > p->getItem()->queuetime())
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
	}
	return min;
}
processor* scheduler:: stl_shortest_processor()
{
	Node <processor*>* p = ProcessorsList->getfront();
	if (p->getItem() == frozen)
		p = p->getNext();
	if (!p)
	{
		return nullptr;
	}
	processor* min = p->getItem();
	while (p)
	{
		if (p->getItem() == frozen)
			p = p->getNext();
		if (p)
		{
			if (min->stealqueuetime() > p->getItem()->stealqueuetime())
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
	}
	return min;
}
processor* scheduler:: longest_processor()
{
	Node <processor*>* p = ProcessorsList->getfront();
	if (p->getItem() == frozen)
		p = p->getNext();
	if (!p)
	{
		return nullptr;
	}
	processor* max = p->getItem();
	while (p)
	{
		if (p->getItem() == frozen)
			p = p->getNext();
		if (p)
		{
			if (max->queuetime() < p->getItem()->queuetime())
			{
				max = p->getItem();
			}
			p = p->getNext();
		}
	}
	return max;
}
processor* scheduler:: shortest_FCFS()
{
	if (FCFSno == 0)
	{
		return nullptr;
	}
	Node <processor*>* p = ProcessorsList->getfront();
	if (p->getItem() == frozen)
		p = p->getNext();
	if (!p)
	{
		return nullptr;
	}
	processor* min = p->getItem();
	while (p)
	{
		if (p->getItem() == frozen)
			p = p->getNext();
		if (p)
		{
			if (min->queuetime() > p->getItem()->queuetime() && p->getItem()->get_type() == 1)
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
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
processor* scheduler:: shortest_SJF()
{
	if (SJFno == 0)
	{
		return nullptr;
	}
	Node <processor*>* p = ProcessorsList->getfront();
	while (p->getItem()->get_type() != 3)
	{
		p = p->getNext();
	}
	if (!p)
	{
		return nullptr;
	}
	//p=the first sjf
	if (p->getItem() == frozen)
		p = p->getNext();
	if (!p)
	{
		return nullptr;
	}
	processor* min = p->getItem();
	while (p)
	{
		if (p->getItem() == frozen)
			p = p->getNext();
		if (p)
		{
			if (min->queuetime() > p->getItem()->queuetime() && p->getItem()->get_type()==3)
			{
				min = p->getItem();
			}
			p = p->getNext();
		}
	}
	return min;
}
processor* scheduler:: shortest_RR()
{
	if (RRno == 0)
	{
		return nullptr;
	}

	Node <processor*>* p = ProcessorsList->getfront();
	while (p->getItem()->get_type() != 2)
	{
		p = p->getNext();
	}
	if (!p)
	{
		return nullptr;
	}
	//p=the first rr
	if (p->getItem() == frozen)
		p = p->getNext();
	if (!p)
	{
		return nullptr;
	}
	processor* min = p->getItem();
	while (p)
	{
		if (p->getItem()->get_type() == 2 && p->getItem()!=frozen)
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
bool scheduler:: should_steal()
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
int scheduler:: calclimit()
{
	int LQF = longest_processor()->stealqueuetime();
	int SQF = stl_shortest_processor()->stealqueuetime();
	if (LQF == 0)
		return 0;
	return((((LQF - SQF) * 100) / LQF));
}
void scheduler:: steal()
{
	bool y = should_steal();
	process* prv = nullptr;
	while (y == true)
	{
		processor* longest = longest_processor();
		processor* shortest = stl_shortest_processor();
		process* p = longest->donate();
		if (p == prv)
		{
			shortest->AddProcess(p);
			break;
		}
		if (p)
		{
			if (p->get_pure() == 1)
			{
				shortest->AddProcess(p);
				stolenno++;
			}
			else
				longest->AddProcess(p);
		}
		prv = p;
		y = should_steal();
	}
}
int scheduler:: CountRun() const
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
scheduler:: ~scheduler()
{
	delete TRM;
	delete NEW;
	delete ProcessorsList;
	delete BLK;
	delete killsigs;
	delete inputsigs;
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