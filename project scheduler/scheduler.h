#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "process.h"
#include"processor.h"
#include<iostream>
#include"RoundRobin.h"
#include"FCFS.h"
using namespace std;
#include<fstream>

class scheduler
{
private:
	int processno;
	int FCFSno;
	int SJFno;
	int RRno;
	QueueADT<process> NEW;
	QueueADT<process> BLK;
	QueueADT<process> TRM;
	QueueADT<processor> ProcessorsList;

public:
	processor* processor_shortest_queue()
	{
		processor* p = ProcessorsList.getfront()->getItem();
	}
	void loadfile()
	{
		cout << "enter file name";
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
			inputFile >> processno;
			for (int i = 0; i < processno; i++)
			{
				
			}
	}
		void NEWtoRDY()
	{
		process* ptr1;
		Node<processor> *temp;
		NEW.dequeue(*ptr1);
		temp = ProcessorsList.getfront();
		while (temp)
		{
			processor* p2;
			p2 = temp->getItem();
			p2->AddProcess(ptr1);
			temp = temp->getNext();
			if (temp->getNext() == NULL)
			{
				temp->setNext(ProcessorsList.getfront());
			}
		}
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

