#ifndef queue_h
#define queue_h

#include <iostream>
#include "BNode.h"

struct que {
	int element_id;
	double Euc_distance;
};
//length= 叶子节点数+空
const int length = 60000 / size_of_key + 2;

class queue {
 private:
 	int front;
 	int rear_now, rear_next;
 	que Que[length];
 public:
 	queue();
 	bool push_back(int element_id, double & Euc_distance, bool isLeaf);
 	bool Top(int & element_id, double & Euc_distance);
 	bool empty();
 	void reset();
 	void clear();
	void pop();
 	bool full(bool isLeaf);
 };

#endif
