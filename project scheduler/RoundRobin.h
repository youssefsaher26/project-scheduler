#include"LinkedList.h"
#include"Node.h"
#include"QueueADT.h"
#include "processor.h"
#include "process.h"
class RoundRobin : public processor
{
private:
	static int TimeSlice;
	static int RTF;
public:
	static void set_Timeslice(int x)
	{
		TimeSlice = x;
	}
	static void set_RTF(int x)
	{
		RTF = x;
	}
	virtual int queuetime()
	{

	}

};

