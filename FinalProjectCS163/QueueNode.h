#pragma once

template<class T>
class QueueNode {
public:
	T value;
	QueueNode* next;

	QueueNode();
	QueueNode(const T& value);
};