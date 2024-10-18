#include "PNode.h"
#pragma once
template <typename T>
class PriorityQueue
{
private:
	PNode<T>* backPtr;
	PNode<T>* frontPtr;
	int count;
public:
	PriorityQueue()
	{
		backPtr = nullptr;
		frontPtr = nullptr;
		count = 0;
	}
	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}

	bool enqueue(const T& newEntry,int p)
	{
		PNode<T>* newNodePtr = new PNode<T>(newEntry,p);

		if (isEmpty())	//special case if this is the first node to insert
		{
			frontPtr = newNodePtr; // The queue is empty
			backPtr = newNodePtr;
			count++;
			return true;
		}
		else
		{
			PNode<T>* prv = frontPtr;
			PNode<T>* ptr = frontPtr->getNext();
			if (prv->get_priority() > p)
			{
				newNodePtr->setNext(frontPtr);
				frontPtr = newNodePtr ;
				count++;
				return true;
			}
			else
			{
				while (ptr)
				{
					if (ptr->get_priority() > p)
					{
						prv->setNext(newNodePtr);
						newNodePtr->setNext(ptr);
						count++;
						return true;
					}
					ptr = ptr->getNext();
					prv = prv->getNext();
				}
				prv->setNext(newNodePtr);
				newNodePtr->setNext(nullptr);
				backPtr = newNodePtr;
				count++;
				return true;
			}
		}
	}
		
	bool dequeue(T& frntEntry)
	{
		if (isEmpty())
			return false;

		PNode<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		frontPtr = frontPtr->getNext();
		// Queue is not empty; remove front
		if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
			backPtr = nullptr;

		// Free memory reserved for the dequeued node
		delete nodeToDeletePtr;
		count--;
		return true;

	}
	void setcount(int x)
	{
		count = x;
	}
	bool peek(T& frntEntry)  const
	{
		if (isEmpty())
			return false;

		frntEntry = frontPtr->getItem();
		return true;

	}
	PNode<T>* getfront() const
	{
		return frontPtr;
	}
	PNode<T>* getrear() const
	{
		return backPtr;
	}
	~PriorityQueue()
	{
		T temp;

		//Free (Dequeue) all nodes in the queue
		while (dequeue(temp));
	}

	//copy constructor
	PriorityQueue(const PriorityQueue<T>& LQ)
	{
		PNode<T>* NodePtr = LQ.frontPtr;
		if (!NodePtr) //LQ is empty
		{
			frontPtr = backPtr = nullptr;
			return;
		}

		//insert the first node
		PNode<T>* ptr = new PNode<T>(NodePtr->getItem());
		frontPtr = backPtr = ptr;
		NodePtr = NodePtr->getNext();

		//insert remaining nodes
		while (NodePtr)
		{
			PNode<T>* ptr = new PNode<T>(NodePtr->getItem());
			backPtr->setNext(ptr);
			backPtr = ptr;
			NodePtr = NodePtr->getNext();
		}
	}
	int getcount()
	{
		return count;
	}
}
;

