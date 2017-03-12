#ifndef ReadData_h
#define ReadData_h

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct node {
	int element_id;
	double Euc_distance;
	node (int n, double m) {
		element_id = n; 
		Euc_distance = m;
	}
	node() {}
};

class ReadData {

 public:
 	node *Map[50];
 	double *query[100];
	int ReverseInt(int i);
 	void read_Mnist_DS(std::string filename, double (*line)[784]); 
	void read_Mnist_Q(std::string filename, double (*line)[784]); 
 	ReadData() {
		for (int i = 0; i < 50; i++) Map[i] = new node[60000];
		for (int i =  0; i < 100; i++) query[i] = new double[50];
	}
};

#endif
