#ifndef _LINKEDLIST
#define _LINKEDLIST
#include "Node.h"

template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list
public:


	LinkedList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void PrintList()	const
	{
		cout << "\nprinting list contents:\n\n";
		Node<T>* p = Head;

		while (p)
		{
			cout << "[ " << p->getItem() << " ]";
			cout << "--->";
			p = p->getNext();
		}
		cout << "NULL\n";
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;

	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}



	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T& data)
	{
		Node<T>* newNode = new Node<T>(data);
		if (Head == nullptr)
		{
			Head = newNode;
			newNode->setNext(nullptr);
			return;
		}
		Node<T>* ptr = new Node<T>(data);
		ptr = Head;
		while (ptr->getNext())
		{
			ptr = ptr->getNext();
		}
		ptr->setNext(newNode);
		newNode->setItem(data);
		newNode->setNext(nullptr);
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(int Key)
	{
		bool Found = false;
		Node<T>* ptr = Head;
		while (ptr)
		{
			if (ptr->getItem() == Key)
			{
				Found = true;;
			}
			ptr = ptr->getNext();
		}
		return Found;
	}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T& value)
	{
		int count = 0;
		Node<T>* ptr = Head;
		while (ptr)
		{
			if (ptr->getItem() == value)
			{
				count++;
			}
			ptr = ptr->getNext();
		}
		return count;
	}

	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst()
	{
		Node<T>* ptr = Head;
		Head = Head->getNext();
		delete ptr;
	}


	//[5] DeleteLast
	//Deletes the last node in the list
	void DeleteLast()
	{
		Node<T>* prev = Head;
		Node<T>* ptr = Head->getNext();
		while (ptr->getNext())
		{
			ptr = ptr->getNext();
			prev = prev->getNext();
		}
		prev->setNext(nullptr);
		delete ptr;
	}

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T& value)
	{
		if (Head->getItem() == value)
		{
			Node<T>* p = Head->getNext();
			delete Head;
			Head = p;
			return true;
		}
		Node<T>* prev = Head;
		Node<T>* ptr = prev->getNext();
		while (ptr->getNext())
		{
			if (ptr->getItem() == value)
			{
				prev->setNext(ptr->getNext());
				delete ptr;
				return true;
			}
			else
			{
				ptr = ptr->getNext();
				prev = prev->getNext();
			}
		}
		return false;
	}


	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNodes(const T& value)
	{
		int found = 0;
		Node<T>* prev = Head;
		Node<T>* ptr = Head->getNext();
		while (Head->getItem() == value)
		{
			Node <T>* R = prev;
			ptr = ptr->getNext();
			prev = prev->getNext();
			Head = Head->getNext();
			delete R;
			found++;
		}
		while (ptr)
		{
			if (ptr->getItem() == value)
			{
				Node <T>* R = ptr;
				ptr = ptr->getNext();
				prev->setNext(ptr);
				delete R;
				found++;
			}
			else
			{
				ptr = ptr->getNext();
				prev = prev->getNext();
			}
		}
		if (found == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	void Merge(const LinkedList& L)
	{
		if (Head->getNext() == nullptr)
		{
			Head->setNext(L.Head);
			return;
		}
		Node<T>* ptr = Head;
		while (ptr->getNext())
		{
			ptr = ptr->getNext();
		}
		ptr->setNext(L.Head);
	}

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse()
	{
		Node<T>* prev = Head;
		Node<T>* ptr = prev->getNext();
		while (ptr->getNext())
		{
			ptr = ptr->getNext();
			prev = prev->getNext();
		}
		while (true)
		{

		}

	}
	Node<T>* GetHead() const
	{
		return Head;
	}
	void SetHead(Node<T>* H)
	{
		H = Head;
	}

};

#endif	

