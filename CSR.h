#pragma once
#include <istream>
#include <fstream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
class CSR
{
private: 
	std::vector<double> aelem; // ������ ��������� ���������
	std::vector<int> jptr; // ������, � ������� ������ ������� j ������������� �������, � ������� ��������� j-� ���� ������� aelem(j = 1..n)  
	std::vector<int> iptr; // ������ ������� ���������� �� ����� �������� ���� ���� ������ iptr[1] = 1(������) 
	int size; // ������ ������� 
public: 
	CSR(std::vector<double>& tmp_aelem_, std::vector<int>& tmp_jptr_, std::vector<int>& tmp_iptr_, int size_); 
	CSR(); 

	double get(int i, int j); 
	void make_null(); 
	void loadCSR(std::string& path);
	void printCSR();
	int getSize();

};
std::list<CSR> load(std::string& path);
