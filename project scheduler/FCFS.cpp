#include "FCFS.h" 
int FCFS::MaxW = 0;
FCFS:: FCFS(int x, int y)
{
	FCFS_RDY = new LinkedList<process*>;
	type = 1;
	processornumber = x;
	forkprob = y;
	fork_it = false;
}
void FCFS:: AddProcess(process* p)
{
	FCFS_RDY->InsertEnd(p);
}
int FCFS:: queuetime()
{
	int sum = 0;
	Node<process*>* temp = FCFS_RDY->GetHead();
	while (temp)
	{
		process* C = temp->getItem();
		sum = sum + C->GetRemTime();
		temp = temp->getNext();
	}
	if (RUN)
		sum = RUN->GetRemTime() + sum;
	return sum;
}
int FCFS::stealqueuetime()
{
	int sum = 0;
	Node<process*>* temp = FCFS_RDY->GetHead();
	while (temp)
	{
		process* C = temp->getItem();
		sum = sum + C->GetRemTime();
		temp = temp->getNext();
	}
	return sum;
}
void FCFS:: RDY_TO_RUN()
{
	if (RUN == nullptr)
	{
		if (FCFS_RDY->isEmpty() == false)
		{
			process* temp = FCFS_RDY->GetHead()->getItem();
			FCFS_RDY->DeleteFirst();
			if (temp->get_pure() && pno>1)
			{
				migration(temp);
			}

			if (mig == nullptr)
			{
				if (temp->GetRemTime() == temp->get_CT())
					temp->setRT(TIME);

				RUN = temp;
			}

		}
	}
}
void FCFS:: migration(process* p)
{
	int arrtime = p->GetArrTime();
	int cpu = p->get_CT();
	int rem = p->GetRemTime();
	if (TIME - arrtime - cpu + rem > MaxW && p->get_pure() == 1 && p->get_pure()==1)
	{		mig = p;
	}
	else
	{
		mig = nullptr;
	}
}
void FCFS:: fork()
{
	if (RUN != nullptr)
	{
		int forking_no = rand() % 100 + 1;
		if (forking_no < forkprob)
		{
			if (RUN->get_children_no() < 2)
			{
				fork_it = true;
			}
			else
				fork_it = false;
		}
	}
}
void FCFS:: SchedAlgo()
{
	if (!RUN)
	{
		RDY_TO_RUN();
	}
	if (mig == nullptr)
	{
		if (RUN)
		{
			NeedBlock();
			if (RUN)
			{
				NeedTrm();
			}
			if (block == nullptr && trm == nullptr)
			{
				RUN->decremtime();
				fork();
			}
		}
	}
}
void FCFS:: RUN_TO_RDY()
{
	FCFS_RDY->InsertEnd(RUN);
	RUN = NULL;
	State = 0;
}
bool FCFS::Done()
{
	if (RUN == nullptr && FCFS_RDY->isEmpty() == true)
		return true;
	return false;
}
process* FCFS:: donate()
{
	if (FCFS_RDY->isEmpty() == false)
	{
		Node<process*>* p = nullptr;
		p = FCFS_RDY->GetHead();
		if (p)
		{
			process* pro = p->getItem();
			FCFS_RDY->DeleteNode(p->getItem());
			return pro;

		}
	}
	return nullptr;
}
process* FCFS::donate_steal()
{
	int count = 0;
	Node<process*>* p = FCFS_RDY->GetHead();
	while (p)
	{
		if (p->getItem()->get_pure() == 0)
			count++;
		p = p->getNext();
	}
	if (count == FCFS_RDY->get_count())
		return nullptr;
	else
		return this->donate();

}
LinkedList<process*>* FCFS:: get_FCFS_RDY()
{
	return FCFS_RDY;
}
void FCFS:: set_Maxw(int x)
{
	MaxW = x;
}
process* FCFS:: KILL(int ID)
{
	if (RUN)
	{
		if (RUN->getID() == ID)
		{
			process* t = RUN;
			RUN = nullptr;
			State = 0;
			return t;
		}
	}
	Node <process*>* p = FCFS_RDY->GetHead();
	while (p)
	{
		int id = p->getItem()->getID();
		if (id == ID)
		{
			process* temp = p->getItem();
			FCFS_RDY->DeleteNode(p->getItem());
			return temp;
		}
		p = p->getNext();
	}
	return nullptr;
}
void FCFS::destruct()
{
	FCFS_RDY->~LinkedList();
	delete RUN;
	delete mig;
	delete block;
	delete trm;
}