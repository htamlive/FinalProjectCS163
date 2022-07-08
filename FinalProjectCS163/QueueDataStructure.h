#pragma once

#include "QueueNode.h"

template<class T>
class Queue {
private:
	QueueNode<T> *head, *tail;
public:
	Queue(): head(nullptr), tail(nullptr) {};
	~Queue() {
		QueueNode<T>* node;
		while (this->head) {
			node = (this->head);
			(this->head) = (node->next);
			delete node;
		}
	};

	void clear() {
		QueueNode<T>* node;
		while (this->head) {
			node = (this->head);
			(this->head) = (node->next);
			delete node;
		}
		(this->tail) = nullptr;
	};
	
	void enqueue(const T& value) {
		if ((this->head) == nullptr) {
			(this->head) = new QueueNode<T>(value);
			(this->tail) = (this->head);
		} else {
			(this->tail)->next = new QueueNode<T>(value);
			(this->tail) = (this->tail)->next;
		}
	};
	
	bool checkEmpty() const {
		return (this->head) == nullptr;
	};

	int findSize() const {
		int result = 0;
		for (QueueNode<T>* node = (this->head); node; node = node->next)
			++result;
		return result;
	};

	bool dequeue() {
		if ((this->head) == nullptr)
			return false;
		QueueNode<T>* node = (this->head);
		(this->head) = (node->next);
		delete node;
		if ((this->head) == nullptr)
			(this->tail) = nullptr;
		return true;
	};

	bool dequeue(T& value) {
		if ((this->head) == nullptr)
			return false;
		QueueNode<T>* node = (this->head);
		(this->head) = (node->next);
		value = (node->value);
		delete node;
		if ((this->head) == nullptr)
			(this->tail) = nullptr;
		return true;
	};

	T& getFirstElement() {
		return (this->head)->value;
	};

	const T& getFirstElement() const {
		return (this->head)->value;
	};
};
