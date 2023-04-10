#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include"StackADT.h"
#include "processor.h"
#include "process.h"

class scheduler;

class FCFS : public processor
{
private:
	LinkedList<process> FCFS_RDY;
public:
	virtual int queuetime()
	{
		int sum = 0;
		Node<process>* temp;
		while (temp)
		{
			temp = FCFS_RDY.GetHead();
			process *C = temp->getItem();
			sum = C->CpuTime + sum;
			temp = temp->getNext();
		}
	}
	void FCFS_RDY_TO_RUN()
	{
		Node<process>* temp = FCFS_RDY.GetHead();
		FCFS_RDY.SetHead(temp->getNext());
		RUN = temp->getItem();
		delete temp;
	}
};

