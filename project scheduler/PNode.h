#pragma once
	//First let's declare a single node in the list
template<typename T>
class PNode
{
private:
	T item;	// A data item (can be any complex sturcture)
	int priority;
	PNode<T>* next;	// Pointer to next node
public:
	PNode(T newItem, int p) //non-default constructor
	{
		item = newItem;
		priority = p;
		next = nullptr;
	}
	void setItem(T newItem,int p)
	{
		item = newItem;
	} // end setItem

	void setNext(PNode<T>* nextNodePtr)
	{
		next = nextNodePtr;
	} // end setNext

	T getItem() const
	{
		return item;
	} // end getItem

	PNode<T>* getNext() const
	{
		return next;
	}
	int get_priority()
	{
		return priority;
	}
};


