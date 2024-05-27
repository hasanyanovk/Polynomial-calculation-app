#ifndef __SORTED_LIST__
#define __SORTED_LIST__

#include "linked_list.h"
#include <functional>

template <class TELEM>
class SortedList : public LinkedList<TELEM> {
public:
	void insert_in_order(TELEM el, function<bool(TELEM x, TELEM y)> f); // вставить в нужном порядке
};

template <class TELEM>
void SortedList<TELEM>::insert_in_order(TELEM el, function<bool(TELEM x, TELEM y)> f) {
	if (this->size == 0) {
		this->push_back(el);
		return;
	}
	if (this->size == 1) {
		if (!f(this->front->data,el)) {
			this->push_front(el);
		}
		else {
			this->push_back(el);
		}
		return;
	}
	if (!f(this->front->data,el)) {
		this->push_front(el);
		return;
	}
	typename LinkedList<TELEM>::Node* left = this->front;
	typename LinkedList<TELEM>::Node* right = left->next;
	while (right && f(right->data,el)) {
		left = left->next; 
		right = right->next;
	}
	if (!right) {
		this->push_back(el);
		return;
	}
	typename LinkedList<TELEM>::Node* tmp = new typename LinkedList<TELEM>::Node{ el };
	tmp->next = right;
	tmp->prev = left;
	left->next = tmp;
	right->prev = tmp;
	this->size++;
}
#endif