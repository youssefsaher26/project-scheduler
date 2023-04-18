#include "Node.h"
#pragma once
template <typename T>
class QueueADT
{
private:
	Node<T>* backPtr;
	Node<T>* frontPtr;
	int count;
public:
	QueueADT()
	{
		backPtr = nullptr;
		frontPtr = nullptr;
		count = 0;
	}
	bool isEmpty() const
	{
		return (frontPtr == nullptr);
	}

	bool CircEnqueue(const T& newEntry)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry);
		// Insert the new node
		if (isEmpty())	//special case if this is the first node to insert
		{
			frontPtr = newNodePtr;
			backPtr = newNodePtr;
		}		// The queue is empty
		else
		{
			backPtr->setNext(newNodePtr);
		}
		// The queue was not empty

		backPtr = newNodePtr;
		backPtr->setNext(frontPtr);// New node is the last node now
		count++;
		return true;
	}

	bool enqueue(const T& newEntry)
	{
		Node<T>* newNodePtr = new Node<T>(newEntry);
		// Insert the new node
		if (isEmpty())	//special case if this is the first node to insert
		{
			frontPtr = newNodePtr;
			backPtr = newNodePtr;
		}// The queue is empty
		else
			backPtr->setNext(newNodePtr); // The queue was not empty

		backPtr = newNodePtr; // New node is the last node now
		count++;
		return true;
	} // end enqueue
	bool dequeue(T& frntEntry)
	{
		if (isEmpty())
			return false;

		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		// Queue is not empty; remove front
		if (frontPtr == backPtr)	 // Special case: last node in the queue
		{
			backPtr = nullptr;
			frontPtr = nullptr;
		}
		else
			frontPtr = frontPtr->getNext();
		// Free memory reserved for the dequeued node
		count--;
		return true;

	}
	bool CircDequeue(T& frntEntry)
	{
		if (isEmpty())
			return false;

		Node<T>* nodeToDeletePtr = frontPtr;
		frntEntry = frontPtr->getItem();
		// Queue is not empty; remove front
		if (frontPtr == backPtr)	 // Special case: last node in the queue
		{
			frontPtr = nullptr;
			backPtr = nullptr;
		}
		else
		{
			frontPtr = frontPtr->getNext();
			backPtr->setNext(frontPtr);
		}
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
		{
			return false;
		}
		frntEntry = frontPtr->getItem();
		return true;

	}
	Node<T>* getfront() const
	{
		return frontPtr;
	}
	Node<T>* getrear() const
	{
		return backPtr;
	}
	~QueueADT()
	{
		T temp;

		//Free (Dequeue) all nodes in the queue
		while (dequeue(temp));
	}

	//copy constructor
	QueueADT(const QueueADT<T>& LQ)
	{
		Node<T>* NodePtr = LQ.frontPtr;
		if (!NodePtr) //LQ is empty
		{
			frontPtr = backPtr = nullptr;
			return;
		}

		//insert the first node
		Node<T>* ptr = new Node<T>(NodePtr->getItem());
		frontPtr = backPtr = ptr;
		NodePtr = NodePtr->getNext();

		//insert remaining nodes
		while (NodePtr)
		{
			Node<T>* ptr = new Node<T>(NodePtr->getItem());
			backPtr->setNext(ptr);
			backPtr = ptr;
			NodePtr = NodePtr->getNext();
		}
	}
	int getcount()
	{
		return count;
	}
};