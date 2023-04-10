#ifndef QUEUE_ADT
#include "Node.h"
using namespace std;

template <typename T>
class QueueADT
{
private:

	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	QueueADT();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	Node<T>* getfront();
	Node<T>* getrear();
	~QueueADT();

	//copy constructor
	QueueADT(const QueueADT<T>& LQ);
};


/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
QueueADT<T>::QueueADT()
{
	backPtr = nullptr;
	frontPtr = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
bool QueueADT<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
bool QueueADT<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty

	backPtr = newNodePtr; // New node is the last node now
	return true;
} // end enqueue

 /////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool QueueADT<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;

	return true;

}
/////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool QueueADT<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
QueueADT<T>::~QueueADT()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
QueueADT<T>::QueueADT(const QueueADT<T>& LQ)
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
///////////////////////////////////////////////////////////////////////////////////
template <typename T>
Node<T>* QueueADT<T>::getrear()
{
	return backptr;
}
///////////////////////////////////////////////////////////////////////////////////
template <typename T>
Node<T>* QueueADT<T>::getfront()
{
	return frontPtr;
}	
#endif