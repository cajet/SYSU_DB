#ifndef BNode_h
#define BNode_h

#include "ReadData.h"
#define size_of_key 84

struct b_node {
	char level;
	double key[size_of_key];
	int element_id[size_of_key];
	int left_brother;
	int right_brother;
	char size;
	b_node();
	b_node(int left, int _level);
};

#endif
