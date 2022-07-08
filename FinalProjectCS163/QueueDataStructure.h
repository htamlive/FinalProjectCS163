#pragma once

#include "QueueNode.h"

template<class T>
class Queue {
private:
	QueueNode<T> *head, *tail;
public:
	Queue();
	~Queue();
	void clear();
	bool checkEmpty() const;
	int findSize() const;
	void enqueue(const T& value);
	bool dequeue();
	bool dequeue(T &value);
	T& getFirstElement();
};
