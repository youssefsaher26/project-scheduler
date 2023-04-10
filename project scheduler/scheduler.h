#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "process.h"
#include"processor.h"

class scheduler
{
private:
	QueueADT<process> NEW;
	QueueADT<process> BLK;
	QueueADT<process> TRM;
	QueueADT<processor> ProcessorsList;

public:
	processor* processor_shortest_queue()
	{
		processor* p = ProcessorsList.getfront()->getItem();
	}
	void NEWtoRDY()
	{
		process* ptr1;
		Node<processor> *temp;
		NEW.dequeue(*ptr1);
		temp = ProcessorsList.getfront();
		/*while (temp)
		{
			processor* p2;
			p2=temp->getItem();
			p2->AddProcess(ptr1);
			temp = temp->getNext();
			if (temp->getNext()==NULL)
			{
				temp->setNext(ProcessorsList.getfront());
			}
		}*/

	}
	
	QueueADT<process>* getTRM()
	{
		return &TRM;
	}
	QueueADT<process>* getBLK()
	{
		return &BLK;
	}
	void simulator()
	{
		NEWtoRDY();
		ProcessorsList.getfront()->getItem()->SchduleAlgo();
	}
};

