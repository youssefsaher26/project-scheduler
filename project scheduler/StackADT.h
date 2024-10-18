#include "Node.h"
#pragma once
template<typename T>
class StackADT
{
private:
	Node<T>* top;
public:
	StackADT()
	{
		top = nullptr;
	}
	bool isEmpty() const
	{
		return (top == nullptr);
	}  // end isEmpty
	bool push(const T& newEntry)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry);
		// Insert the new node
		if (isEmpty())	//special case if this is the first node to insert
			top = newNodePtr; // The queue is empty
		else
		{
			newNodePtr->setNext(top); // The queue was not empty
			top = newNodePtr; // New node is the last node now
			return true;
		}
	}  // end push
	bool pop(T& TopEntry)
	{
		if (isEmpty()) return false;

		Node<T>* temp = top;
		TopEntry = top->getItem();
		top = top->setNext();
		delete temp;

		return true;
	}  // end pop
	bool peek(T& TopEntry) const
	{
		if (isEmpty()) return false;

		TopEntry = top->getItem();
		return true;
	}  // end peek
	~StackADT()
	{
		T temp;
		while (pop(temp));
	}
};