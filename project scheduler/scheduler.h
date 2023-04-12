#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "process.h"
#include"processor.h"
#include<iostream>
#include"RoundRobin.h"
#include"FCFS.h"
#include "kill.h"
#pragma once
using namespace std;
#include<fstream>

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

public:
	processor* processor_shortest_queue()
	{
		processor* p = ProcessorsList->getfront()->getItem();
	}
	void loadfile()
	{
		char garbage;
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
			inputFile >> STL>>forkprob;
			inputFile >> processno;
			for (int i = 0; i < processno; i++)
			{
				int at, pid, ct, io_num, io_R, io_D;
				inputFile >> at >> pid >> ct >> io_num;
				inputFile >> garbage;//bracket1
				inputFile >> io_R; //ioR
				inputFile >> garbage;//comma
				inputFile >> io_D; //ioD
				process* p = new process(at,pid, ct, io_num, io_R, io_D);
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
	void NEWtoRDY()
	{
		process* ptr1;
		Node<processor*> *temp;
		NEW->dequeue(ptr1);
		temp = ProcessorsList->getfront();
		while (temp)
		{
			processor* p2;
			p2 = temp->getItem();
			p2->AddProcess(ptr1);
			temp = temp->getNext();
			if (temp->getNext() == NULL)
			{
				temp->setNext(ProcessorsList->getfront());
			}
		}
	}
	QueueADT<process*>* getTRM()
	{
		return TRM;
	}
	QueueADT<process*>* getBLK()
	{
		return BLK;
	}
	void simulator()
	{
		NEWtoRDY();
		time++;
	}
	int countbusy()
	{

	}

	friend ostream& operator<< (ostream& out, const scheduler& s)
	{
		out << "Current TimeStep : " << s.time << endl;
		out << "----------RDY PROCESSES----------" << endl;
		Node <processor*>* p = s.ProcessorsList->getfront();
		int i = 1;
		while (p)
		{
			cout << "Processor " << i << " " << p << endl;
			i++;
			p = p->getNext();

		}
		out << "----------BLK PROCESSES----------" << endl;
		//out<< count of blocked queueu<< loop on blocked queueu and print the IDs;
		// same exact idea for TRM;
		out << "-----------RUN PROCESSES----------" << endl;
		Node <processor*>* pt = s.ProcessorsList->getfront();
		//find a way to get the count of the running processes...
		//then, out<<
		while (pt)
		{

			pt->getItem()->getrun();
			if (pt)
			{
				out << pt->getItem()->getrun()->getID() <<"(p"<<pt->getItem()->getpnumber() << ") , ";
			}
			pt = pt->getNext();
		}

		
	}
};

