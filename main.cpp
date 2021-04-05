#include "CSR.h"
#include <iostream>
#include "LU_Master.h"
#include <algorithm>


std::vector<double>  load_vector_b(std::string path)
{
	std::vector<double> result; 
	std::ifstream in_stream(path); 
	std::string str; 
	double num = 0.; 
	if (!in_stream.is_open())
	{
		std::cout << "Error" << std::endl; 
	}
	else
	{
		std::getline(in_stream, str); 
		std::stringstream iss(str); 
		while (iss >> num)
		{
			result.push_back(num); 
		}
	}
	return result; 
}

int main()
{
	std::string path = "matrix.txt";
	std::string path2 = "b.txt"; 
	CSR csr;
	csr.loadCSR(path);
	std::vector<double> b = load_vector_b(path2); 
	std::cout << "M[0,0] = " << csr.get(0, 0) << std::endl;
	std::cout << "M[0,3] = " << csr.get(0, 3) << std::endl;
	std::cout << "M[1,1] = " << csr.get(1, 1) << std::endl;
	std::cout << "M[0,1] = " << csr.get(0, 1) << std::endl;

	for (int i = 0; i < csr.getSize(); i++) {
		for (int j = 0; j < csr.getSize(); j++) {
			std::cout << csr.get(i, j) << "  ";
		}
		std::cout << std::endl;
	}// FINALLY ������ CSR ����� ������� ����

	std::vector<std::vector<double>> L;
	std::vector<std::vector<double>> U;
	std::vector<std::vector<double>> Result;
	Result.resize(csr.getSize());
	for (int i = 0; i < csr.getSize(); i++) {
		Result[i].resize(csr.getSize());
	}

	LU_Master::LU(csr, L, U);

	std::cout << std::endl << std::endl;
	for (int i = 0; i < csr.getSize(); i++) {
		for (int j = 0; j < csr.getSize(); j++) {
			std::cout << L[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl;
	for (int i = 0; i < csr.getSize(); i++) {
		for (int j = 0; j < csr.getSize(); j++) {
			std::cout << U[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	LU_Master::proisv(L, U, Result, csr.getSize());

	std::cout << std::endl << std::endl;
	for (int i = 0; i < csr.getSize(); i++) {
		for (int j = 0; j < csr.getSize(); j++) {
			std::cout << Result[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	std::vector<double> result = LU_Master::solver_1(L, U, b); 
	std::cout << "vector x:  " << std::endl; 
	std::for_each(result.begin(), result.end(), [](const double& n) { std::cout << " " << n; }); 

	return 0;
}