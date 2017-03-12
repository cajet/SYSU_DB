#include "BTree.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include<time.h>
#define PI 3.141592653589793
using namespace std;

double random_line[50][784];

int main() {
 	double U1, U2, X1, X2, X, length;
 	for (int i = 0; i < 50; i++) {
 		length = 0;
 		for (int j = 0; j < 784; j++) {
 			U1 = (float)rand() / RAND_MAX;
			U2 = (float)rand() / RAND_MAX;
			X1 = sqrt(-2 * log(U1)) * cos(2 * PI * U2);
			X2 = sqrt(-2 * log(U1)) * sin(2 * PI * U2);
			X = (X1 + X2) / 2;
			random_line[i][j] = X;
			length += X * X;
 		}
		for (int k= 0; k< 784; k++) {
			random_line[i][k]= random_line[i][k] / sqrt(length);
		}
 	}

	ReadData M;
	string path = "data/Mnist.ds";
	M.read_Mnist_DS(path, random_line);
	BTree * tree = new BTree();
	tree->WriteTree(M.Map);

	string path2 = "data/Mnist.q";
	M.read_Mnist_Q(path2, random_line);
	cout << "正在查询..." << endl;
	for (int i = 0; i < 100; i++) {
		cout << "第 " << i + 1 << " 个查询的查询结果： ";
		int who = tree->Query(M.query[i]);	
		cout<< who << endl;
	}

	delete tree;
	return 0;
}

