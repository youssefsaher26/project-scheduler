#include"scheduler.h"
int main()
{
	scheduler s;
	s.simulator();
	s.~scheduler();
	return 0;
}