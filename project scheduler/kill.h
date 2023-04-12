#pragma once
class kill
{
	int time;
	int id;
public:
	kill(int x, int y)
	{
		time = x;
		id = y;
	}
	int getkiltime()
	{
		return time;
	}
};

