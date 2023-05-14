#pragma once
class TNode
{
private:
	int data;
	TNode* left;
	TNode* right;
public:
	TNode(int val)
	{
		data = val;
		left = right = nullptr;
	}
	void setdata(int d)
	{
		data = d;
	}
	int getdata()
	{
		return data;
	}
	void setright(TNode* p)
	{
		right = p;
	}
	TNode*& getright()
	{
		return right;
	}

	void setleft(TNode* p)
	{
		left = p;
	}
	TNode*& getleft()
	{
		return left;
	}
};


