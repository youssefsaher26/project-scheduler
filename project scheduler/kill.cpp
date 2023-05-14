#include "kill.h"
kill::kill(int x, int y)
{
	time = x;
	id = y;
}
int kill:: getkiltime()
{
	return time;
}
int kill::getkillid()
{
	return id;
}