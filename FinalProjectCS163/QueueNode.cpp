#include "QueueNode.h"

template<class T> QueueNode<T>::QueueNode() : next(nullptr) {};

template<class T> QueueNode<T>::QueueNode(const T &value): value(value), next(nullptr) {};