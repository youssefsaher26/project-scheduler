#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include"StackADT.h"
#include "processor.h"
#include "process.h"

class FCFS : public processor
{
private:
	static int MaxW;
	LinkedList<process>* FCFS_RDY;
public:
	virtual int queuetime()
	{
		int sum = 0;
		Node<process>* temp;
		while (temp)
		{
			temp = FCFS_RDY->GetHead();
			process *C = temp->getItem();
			sum = C->CpuTime + sum;
			temp = temp->getNext();
		}
	}
	void FCFS_RDY_TO_RUN()
	{
		Node<process>* temp = FCFS_RDY->GetHead();
		FCFS_RDY->SetHead(temp->getNext());
		RUN = temp->getItem();
		delete temp;
	}
	void RUN_TO_FCFS_RDY()
	{
		FCFS_RDY->InsertEnd(RUN);
		RUN = NULL;
	}
	LinkedList<process>* get_FCFS_RDY()
	{
		return FCFS_RDY;
	}
	static void set_Maxw(int x)
	{
		MaxW = x;
	}

	friend ostream& operator<< (ostream& out, const FCFS& p)
	{
		out << "[FCFS] : " << p.RUN->getID() << " ";
		Node <process>* ptr = p.FCFS_RDY->GetHead();
		while (ptr)
		{
			out << ptr->getItem()->getID() <<" , ";
		}
	}
};

