#include "ReadData.h"


int cmp(const void *a,const void *b) {
     return (*(node*)a).Euc_distance > (*(node*)b).Euc_distance ? 1 : -1;
}

int ReadData::ReverseInt(int i) {
	unsigned char ch1, ch2, ch3, ch4;
	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;
	return ((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

void ReadData::read_Mnist_DS(std::string filename, double (*line)[784]) {
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (file.is_open()) {
        	int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);
		file.read((char*)&number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);
		file.read((char*)&n_rows, sizeof(n_rows));
		n_rows = ReverseInt(n_rows);
		file.read((char*)&n_cols, sizeof(n_cols));
		n_cols = ReverseInt(n_cols);

		//对object进行投影
		char Projection[784];
		double sum;
        	cout << "正在进行投影..." << endl;
		for (int i = 0; i < number_of_images; ++i) {
			file.read((char*)&Projection, 784);
			for (int j = 0; j < 50; j++) {
				sum = 0;
				for (int k = 0; k < 784; k++) sum += Projection[k] * line[j][k];
				Map[j][i] = node(i, sum);
			}
		}
		for (int j = 0; j < 50; j++) {
			qsort((void *)(Map[j]), number_of_images, sizeof(node), cmp);
		}
	}
	file.close();
}

void ReadData::read_Mnist_Q(std::string filename, double (*line)[784]) {
	std::ifstream file(filename.c_str(), std::ios::binary);
    	if (file.is_open()) {
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;
		file.read((char*)&magic_number, sizeof(magic_number));
		magic_number = ReverseInt(magic_number);
		file.read((char*)&number_of_images, sizeof(number_of_images));
		number_of_images = ReverseInt(number_of_images);
		file.read((char*)&n_rows, sizeof(n_rows));
		n_rows = ReverseInt(n_rows);
		file.read((char*)&n_cols, sizeof(n_cols));
		n_cols = ReverseInt(n_cols);

		//对query向量进行投影
		char Projection[784];
		double sum;
		for (int i = 0; i < 100; i++) {
			file.read((char*)&Projection, 784);
			for (int j = 0; j < 50; j++) {
				sum = 0;
				for (int k = 0; k < 784; k++) sum += Projection[k] * line[j][k];
				query[i][j] = sum;  //第i个q在第j条线上的投影
			}
		}
        }
        file.close();
}

