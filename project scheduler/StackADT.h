#include "Node.h"
template<typename T>
class StackADT
{
private:
	Node<T>* top;
public:
	StackADT();
	bool isEmpty() const;
	bool push(const T& newEntry);
	bool pop(T& TopEntry);
	bool peek(T& TopEntry) const;
	~StackADT();
};
	

/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
StackADT<T>::StackADT()
{
	top = nullptr;
}  // end default constructor

/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
bool StackADT<T> :: isEmpty() const
{
	return (top == nullptr);
}  // end isEmpty

/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
bool StackADT<T>::push(const T& newEntry)
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
/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
bool StackADT<T>::pop(T& TopEntry)
{
		if (isEmpty()) return false;

		Node<T>* temp = top;
		TopEntry = top->getItem();
		top = top->setNext();
		delete temp;

		return true;
}  // end pop
/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
bool StackADT<T>:: peek(T& TopEntry) const
{
	if (isEmpty()) return false;

	TopEntry = top->getItem();
	return true;
}  // end peek
/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
StackADT<T> ::~StackADT()
{
	T temp;
	while (pop(temp));
}
