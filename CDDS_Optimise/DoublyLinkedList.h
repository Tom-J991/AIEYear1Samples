#pragma once

#include <list>

template <typename T>
class DoublyLinkedList
{
	struct ListNode
	{
		T *data;
		ListNode *next;
		ListNode *prev;

	};

public:
	void PushFront(T* data);
	void PushBack(T *data);
	void PopFront();
	void PopBack();
	
	void Insert(ListNode *node, T *data)
	{
		ListNode *newNode = new ListNode{ };
		newNode->data = data;
		newNode->next = node;
		if (node != nullptr)
		{
			newNode->prev = node->prev;
			if (node->prev != nullptr)
			{
				node->prev->next = newNode;
			}
			node->prev = newNode;
		}
		if (m_head == node)
			m_head = newNode;
		if (m_tail == nullptr)
			m_tail = newNode;
	}
	void Erase(ListNode *node);
	void Remove(T *data);

	std::list<ListNode*>::iterator Begin() const;
	std::list<ListNode*>::iterator End() const;
	T *First() const;
	T *Last() const;

	std::size_t Count() const;
	bool IsEmpty() const;

private:
	ListNode *m_head;
	ListNode *m_tail;

};
