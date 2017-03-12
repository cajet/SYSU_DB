#include "BTree.h"
#include<time.h>
#include "stdio.h"
#include "stdlib.h"
#include <map>

template<typename T>
void Read(std::ifstream & file, T & var) {
	file.read((char*)&var, sizeof(T));
	return;
}

std::string int_string(int & i) {
	char t[256];
	sprintf(t, "%d", i);
	return (std::string)t;
}

//存储叶子节点
void BTree::Write_Leaf_Node(node **Map, int & i) {
	bool isLeft = true; //是否是最左边的叶子节点
	count= 0;
	int leaf_count= 60000/84+1; //总叶子节点数 
	for (int j= 0; j< leaf_count; j++) {
		array->push_back(count, Map[i][j*84].Euc_distance, true);
		count++;
		B_Node->level = 0;
		B_Node->size = 0;
		if (isLeft== true) B_Node->left_brother = -1;
		else  B_Node->left_brother = count - 2;
		B_Node->right_brother = count;
		isLeft = false;
		if (j != leaf_count-1) {
			for (int k = 0; k < 84 && k + j*84 < 60000; k++) {
				B_Node->element_id[k] = Map[i][k + j*84].element_id;
				B_Node->key[k] = Map[i][k + j*84].Euc_distance;
				B_Node->size++;
			}
		} else { //单独处理最后一个非满叶子节点 
			for (int k= 0; k + j*84 < 60000; k++) {
				B_Node->element_id[k] = Map[i][k + j*84].element_id;
				B_Node->key[k] = Map[i][k + j*84].Euc_distance;
				B_Node->size++;
			}
			B_Node->right_brother = -1;
		}
		file.write((char*)B_Node, sizeof(b_node));
	}
}
//存储非叶子节点 // may have fault, Does every 84 elements represent a level ?
void BTree::Write_Index_Node() {
	int level= 1;
	while (!array->empty()) {
		// int b_nodes_in_layer = array->size() / 84 + 1 // .size() method can be added to queue class
		// while (b_nodes_in_layer--) {
		bool isLeft = true;
		for (int k= 0; k< 84; k++) {
			if (k == 0) {//对于最左边的节点 ，注意left_brother 
				count++;
				B_Node->level = level;
				B_Node->size = 0;
				if (isLeft== true) B_Node->left_brother = -1 ;
				else B_Node->left_brother = count - 2;
				B_Node->right_brother = count;
				isLeft = false;
			}
			if(array->Top(B_Node->element_id[k], B_Node->key[k])) {  //获取队头元素并赋值给B_Node 
				B_Node->size++;
				if (k == 0) array->push_back(count - 1, B_Node->key[0], false);  //把第1层每个节点中第一个元素再放入队列，用于建立根节点 
			}
			if (k== 83) {
				file.write((char*)B_Node, sizeof(b_node));
				k= -1;
			}
			if (array->empty()) { //即rear_now==front 
				B_Node->right_brother = -1;
				file.write((char*)B_Node, sizeof(b_node));
				break;
			}
		}
		array->reset();
		// }
		level++;
		if (B_Node->left_brother == -1 && B_Node->right_brother == -1) break;
	} 
}

void BTree::WriteTree(node **Map) {
	std::cout << "正在建立B+ 树..." << std::endl;
	array = new queue();
 	B_Node = new b_node();
	for (int i = 0; i < 50; i++) {
		std::string path= "BTree/BTree" + int_string(i);
		file.open(path.c_str(), std::ios::binary);
		count = 0;
		array->clear();
		file.write((char*)&count, sizeof(int));
		file.write((char*)&node_size, sizeof(int));
		Write_Leaf_Node(Map, i);
		Write_Index_Node();
		file.seekp(0, std::ios::beg);
		file.write((char*)&count, sizeof(int));
		//std::cout<< "第" + i + "棵B+树建立完毕" << std::endl;
		file.close();
	}

	std::cout << "B+ 树建立完毕！" << std::endl;

	delete array;
 	delete B_Node;
}

int BTree::Query(double *query) {
	int rst;
	int size= 50; 
	// record of each iteration
	Marker markers[size];
	// record of each elector
	map< int, int > elector_count;

	B_Node = new b_node();

	int cur = 0;
	do {
		std::ifstream file_in;
		std::string path = "BTree/BTree" + int_string(cur);
		file_in.open(path.c_str(), std::ios_base::binary);
		Marker & marker  = markers[cur];
		double q_i = query[cur];
		if (file_in.is_open()) {
			if (!marker.has_initial) {	// initial if ever not
				initial( file_in, marker, q_i);
				marker.has_initial = true;
				// cout << "Left:" << marker.left_leaf_node << " " << marker.left_pointer << " Right:" << marker.right_leaf_node << " " << marker.right_pointer << endl;
			}
			// vote the elector with marker, B_Node, T_Node, query[cur]
			double left_key, right_key;
			int left_element_id, right_element_id;
			if (marker.left_leaf_node >= 0) {
				file_in.seekg(marker.base_offset + marker.left_leaf_node * marker.node_size, std::ios_base::beg);
				Read(file_in, *B_Node);
				left_key = B_Node->key[marker.left_pointer];
				left_element_id = B_Node->element_id[marker.left_pointer];
			}

			if (marker.right_leaf_node < 715) {
				file_in.seekg(marker.base_offset + marker.right_leaf_node * marker.node_size, std::ios_base::beg);
				Read(file_in, *B_Node);
				right_key = B_Node->key[marker.right_pointer];
				right_element_id = B_Node->element_id[marker.right_pointer];
			}

			// if right pointer is out of range or both left pointer and right pointer are in range but left pointer is more closer to the q_i than right pointer
			if (marker.right_leaf_node >= 715 || (marker.left_leaf_node >= 0 && q_i - left_key <= right_key - q_i)) {	// closer to left side, vote the person in the left
				++elector_count[left_element_id];
				if (elector_count[left_element_id] >= size / 2.0) {
					rst = left_element_id;
					break;
				}
				// adjust the marker
				--(marker.left_pointer);
				if (marker.left_pointer == -1) {
					marker.left_pointer = 83;
					--(marker.left_leaf_node);
				}
			} else if (marker.left_leaf_node < 0 || (marker.right_leaf_node < 715 && q_i - left_key > right_key - q_i)) {	// closer to right side, vote the person in the right
			// if left pointer is out of range or both left pointer and right pointer are in range but right pointer is more closer to the q_i than left pointer
				++elector_count[right_element_id];
				if (elector_count[right_element_id] >= size / 2.0) {
					rst = right_element_id;
					break;
				}
				// adjust the marker
				++(marker.right_pointer);
				if (marker.right_pointer == 84 || (marker.right_leaf_node == 714 && marker.right_pointer == 24)) {
					marker.right_pointer = 0;
					++(marker.right_leaf_node);
				}

			}


			file_in.close();
		}

		cur = (cur + 1) % size;
	} while (true);

	delete B_Node;

	return rst;
}

void BTree::initial(std::ifstream & file_in, Marker & marker, double q_i) {
	file_in.seekg(0, std::ios_base::beg);
	Read(file_in, marker.node_count);
	Read(file_in, marker.node_size);
	marker.base_offset = sizeof (marker.node_count) + sizeof(marker.node_size);
	file_in.seekg(marker.base_offset + 724 * marker.node_size, std::ios_base::beg);
	Read(file_in, *B_Node);
	// int key_index = binary_search(B_Node.key, q_i, 0, 9)
	int key_index_level_2 = binary_search(B_Node->key, q_i, 0, B_Node->size);
	int next_b_node = 715 + key_index_level_2;

	file_in.seekg(marker.base_offset + next_b_node * marker.node_size, std::ios_base::beg);
	Read(file_in, *B_Node);

	int key_index_level_1 = binary_search(B_Node->key, q_i, 0, B_Node->size);
	// if (next_b_node == 723) {	// specially deal with the unfilled node.
	// 	key_index_level_1 = binary_search(B_Node->key, q_i, 0, 24);
	// } else {
	// 	key_index_level_1 = binary_search(B_Node->key, q_i, 0, 84);
	// }
	next_b_node = 84 * key_index_level_2 + key_index_level_1;

	file_in.seekg(marker.base_offset + next_b_node * marker.node_size, std::ios_base::beg);
	Read(file_in, *B_Node);
	int key_index_level_0 = binary_search(B_Node->key, q_i, 0, B_Node->size);

	marker.left_leaf_node = next_b_node;
	marker.left_pointer = key_index_level_0;
	marker.right_leaf_node = next_b_node;
	marker.right_pointer = key_index_level_0 + 1;
	// if q_i is larger than the last one in the key array, 
	if (marker.right_pointer == int(B_Node->size)) {
		++(marker.right_leaf_node);
		marker.right_pointer = 0;
	}
}

int BTree::binary_search(double *key, double q_i, int begin, int end) {
	while (begin + 1 != end) {
		int mid = begin + (end - begin) / 2;
		if (q_i >= key[mid]) {
			begin = mid;
		} else {
			end = mid;
		}
	}
	return begin;
}
