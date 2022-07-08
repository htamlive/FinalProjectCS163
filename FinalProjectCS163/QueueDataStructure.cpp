#include "QueueDataStructure.h"

template<class T> Queue<T>::Queue(): head(nullptr), tail(nullptr) {};

template<class T> Queue<T>::~Queue() {
	QueueNode<T>* node;
	while (this->head) {
		node = (this->head);
		(this->head) = (node -> next);
		delete node;
	}
	delete (this->tail);
};

template<class T> void Queue<T>::clear() {
	QueueNode<T>* node;
	while (this->head) {
		node = (this->head);
		(this->head) = (node->next);
		delete node;
	}
	delete (this->tail);
	(this->tail) = nullptr;
}

template<class T> bool Queue<T>::checkEmpty() const {
	return (this -> head) == nullptr;
};

template<class T> int Queue<T>::findSize() const {
	int result = 0;
	for (QueueNode<T>* node = (this -> head); node; node = node->next)
		++result;
	return result;
};

template<class T> void Queue<T>::enqueue(const T& value) {
	if ((this -> head) == nullptr) {
		(this -> head) = new QueueNode<T>(value);
		(this -> tail) = (this -> head);
	} else {
		(this -> tail) -> next = new QueueNode<T>(value);
		(this -> tail) = (this -> tail) -> next;
	}
};

template<class T> bool Queue<T>::dequeue() {
	if ((this -> head) == nullptr)
		return false;
	QueueNode<T>* node = (this -> head);
	(this -> head) = (node -> next);
	delete node;
	if ((this -> head) == nullptr)
		(this -> tail) = nullptr;
	return true;
};

template<class T> bool Queue<T>::dequeue(T& value) {
	if ((this->head) == nullptr)
		return false;
	QueueNode<T>* node = (this -> head);
	(this -> head) = (node -> next);
	value = (node -> value);
	delete node;
	if ((this -> head) == nullptr)
		(this -> tail) = nullptr;
	return true;
};

template<class T> T& Queue<T>::getFirstElement() {
	return (this -> head) -> value;
};