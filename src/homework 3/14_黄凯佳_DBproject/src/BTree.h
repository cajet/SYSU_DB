#ifndef BTree_h
#define BTree_h

#include "BNode.h"
#include "ReadData.h"
#include "queue.h"
#include <fstream>
#include <cstring>
#include <cmath>

struct Marker {
	int left_leaf_node;
	int left_pointer;
	int right_leaf_node;
	int right_pointer;

	int node_size;
	int node_count;
	int base_offset;
	bool has_initial;

	Marker() {
		has_initial = false;
	}
};

class BTree {
 private:
 	int *votes;
 	int count;  //总节点数
 	int node_size;     //节点占用字节
 	b_node * B_Node;
 	std::ofstream file;
 	queue * array;      //array存储每个叶子节点的首个key

 public:
 	BTree() {
 		node_size = sizeof(b_node);
 		count = 0;
 	}
	// write
	void WriteTree(node **Map);
 	void Write_Leaf_Node(node **Map, int & i);
 	void Write_Index_Node();
 	// search 
	int Query(double *query);
	void initial(std::ifstream & file, Marker & marker, double q_i);
	int binary_search(double *key, double q_i, int begin, int end);
};

#endif
