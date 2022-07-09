#pragma once

template<class T>
class QueueNode {
public:
	T value;
	QueueNode* next;

	QueueNode() : next(nullptr) {};
	QueueNode(const T& value) : value(value), next(nullptr) {};
};