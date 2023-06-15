#pragma once

#include <iterator>

template <typename T>
class DoublyLinkedList
{
public:
	struct ListNode
	{
		T *data;
		ListNode *next;
		ListNode *prev;

	};

	struct Iterator
	{
		Iterator() { }
		Iterator(ListNode *node) : m_currentNode(node) { }

		T *Current() { return m_currentNode->data; }
		// Move iterator
		bool Next()
		{
			if (m_currentNode->next != nullptr)
			{
				m_currentNode = m_currentNode->next;
				return true;
			}
			return false;
		}
		bool Prev()
		{
			if (m_currentNode->prev != nullptr)
			{
				m_currentNode = m_currentNode->prev;
				return true;
			}
			return false;
		}
		// Operator overloads
		Iterator &operator++() { this->Next(); return *this; } // Prefix
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; } // Postfix
		Iterator &operator--() { this->Prev(); return *this; } // Prefix
		Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; } // Postfix
		T *operator*() { return Current(); }
		ListNode *operator->() { return m_currentNode; }
		explicit operator ListNode *() const { return m_currentNode; }
		friend bool operator==(const Iterator &a, const Iterator &b) { return a.m_currentNode == b.m_currentNode; }
		friend bool operator!=(const Iterator &a, const Iterator &b) { return a.m_currentNode != b.m_currentNode; }

	private:
		ListNode *m_currentNode;

	};

public:
	// Insert element before a node
	void Insert(ListNode *node, T *data)
	{
		if (node != nullptr)
		{
			ListNode *newNode = new ListNode{ };
			newNode->data = data;
			newNode->next = node;
			if (node->prev != nullptr)
			{
				newNode->prev = node->prev;
				node->prev->next = newNode;
			}
			node->prev = newNode;
			if (m_head == node)
				m_head = newNode;
			if (m_tail == nullptr)
				m_tail = newNode;
			m_count++;
		}
	}
	// Remove element by node
	void Erase(ListNode *node)
	{
		if (node != nullptr)
		{
			if (node->next != nullptr)
				node->next->prev = node->prev;
			if (node->prev != nullptr)
				node->prev->next = node->next;
			if (m_head == node)
				m_head = node->next;
			if (m_tail == node)
				m_tail = node->prev;
			node->next = nullptr;
			node->prev = nullptr;
			node->data = nullptr;
			delete node;
			m_count--;
		}
	}

	void DeleteAll()
	{
		ListNode *current = m_head;

		while (current != nullptr)
		{
			if (current->next != nullptr)
			{
				current = current->next;
				delete current->data;
			}
			delete current;
		}
		
	}
	// Add to front/back of list
	void PushFront(T *data)
	{
		ListNode *newNode = new ListNode{ };
		newNode->data = data;
		newNode->next = m_head;
		if (m_head != nullptr)
			m_head->prev = newNode;
		m_head = newNode;
		if (m_tail == nullptr)
			m_tail = newNode;
		m_count++;
	}
	void PushBack(T *data)
	{
		ListNode *newNode = new ListNode{ };
		newNode->data = data;
		newNode->prev = m_tail;
		if (m_tail != nullptr)
			m_tail->next = newNode;
		m_tail = newNode;
		if (m_head == nullptr)
			m_head = newNode;
		m_count++;
	}
	// Remove from Front/Back of List
	void PopFront()
	{
		if (m_head != nullptr)
		{
			auto *N = m_head;
			if (m_head->next != nullptr)
				m_head->next->prev = nullptr;
			m_head = m_head->next;
			N->next = nullptr;
			N->prev = nullptr;
			N->data = nullptr;
			delete N;
		}
		m_count--;
	}
	void PopBack()
	{
		if (m_tail != nullptr)
		{
			auto *N = m_tail;
			if (m_tail->prev != nullptr)
				m_tail->prev->next = nullptr;
			m_tail = m_tail->prev;
			N->next = nullptr;
			N->prev = nullptr;
			N->data = nullptr;
			delete N;
		}
		m_count--;
	}
	// Remove element by data
	void Remove(T *data)
	{
		ListNode *nodeToRemove = nullptr;
		//for (Iterator i = Begin(); i != End(); i++)
		//{
		//	if (*i == data)
		//		nodeToRemove = (ListNode *)i;
		//}


		if (nodeToRemove != nullptr)
			Erase(nodeToRemove);
	}
	// Remove all elements
	void Clear()
	{
		for (Iterator i = Begin(); i != End(); ++i)
		{
			if ((ListNode *)i == nullptr)
				continue;
			ListNode *N = (ListNode *)i;
			Erase(N);
		}
	}
	// Find element by data
	ListNode *Find(Iterator start, Iterator end, T *data)
	{
		if ((ListNode *)start == nullptr && (ListNode *)end == nullptr)
			return nullptr;

		ListNode *foundNode = (ListNode *)start;
		for (Iterator i = start; i != end; i++)
		{
			if (*i == data)
				foundNode = (ListNode *)i;
		}
		return foundNode;
	}
	// Find element by index
	ListNode *Find(Iterator start, Iterator end, int index)
	{
		if ((ListNode *)start == nullptr && (ListNode *)end == nullptr)
			return nullptr;

		ListNode *foundNode = (ListNode *)start;
		int j = 0;
		for (Iterator i = start; i != end; i++)
		{
			if (index == j)
				foundNode = (ListNode *)i;
			j++;
		}
		return foundNode;
	}

	// Get iterators at front/back of list
	Iterator Begin() const { return Iterator(m_head); }
	Iterator End() const { return Iterator(m_tail); }
	// Get nodes at front/back of list
	ListNode *First() const { return m_head; }
	ListNode *Last() const { return m_tail; }

	unsigned int Count() const { return m_count; }
	bool IsEmpty() const { return m_count <= 0; }

	// Operator Overloads
	T *operator[](int index) { return Find(Begin(), End(), index)->data; }

private:
	ListNode *m_head;
	ListNode *m_tail;

	unsigned int m_count = 0;

};
