#include "CSR.h"
#include <iostream>
#include "LU_Master.h"
#include <algorithm>

void create_n_b(std::string path)
{
	std::ofstream of_stream(path); 
	for (int i = 0; i < 494; i++)
	{
		of_stream << 100 << "\n"; 
	}
}


std::vector<double> load_vector_b(std::string path)
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
	std::string path = "matrix1.txt";
	std::string path2 = "b1.txt"; 
	std::string path3 = "bcsstm02.txt"; //  bcsstm02.txt - ğàáî÷èé
	std::string path4 = "b2.txt"; 
	CSR csr2 = LU_Master::parse_matrix(path3); 
	CSR csr;
	csr.loadCSR(path);
	// std::vector<double> b = load_vector_b(path2); 
	create_n_b(path4); 
	std::vector<double> b = load_vector_b(path4);

	for (int i = 0; i < csr2.getSize(); i++) {
		for (int j = 0; j < csr2.getSize(); j++) {
			std::cout << csr2.get(i, j) << "  ";
		}
		std::cout << std::endl;
	}// FINALLY ÏÅ×ÀÒÜ CSR ×ÅĞÅÇ ÎÁÛ×ÍÛÉ ÖÈÊË

	std::vector<std::vector<double>> L;
	std::vector<std::vector<double>> U;
	std::vector<std::vector<double>> Result;
	Result.resize(csr2.getSize());
	for (int i = 0; i < csr2.getSize(); i++) {
		Result[i].resize(csr2.getSize());
	}

	std::cout << "*****"<< omp_get_wtime() << std::endl; 
	LU_Master::LU(csr2, L, U);
	std::cout << "*****" << omp_get_wtime() << std::endl;

	std::cout << std::endl << std::endl;
	for (int i = 0; i < csr2.getSize(); i++) {
		for (int j = 0; j < csr2.getSize(); j++) {
			std::cout << L[i][j] << "  ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl;
	for (int i = 0; i < csr2.getSize(); i++) {
		for (int j = 0; j < csr2.getSize(); j++) {
			std::cout << U[i][j] << "  ";
		}
		std::cout << std::endl;
	}


	//std::vector<double> result = LU_Master::solver_1(L, U, b);
	std::cout << "Vector X:  " << std::endl;
	// std::for_each(result.begin(), result.end(), [](const double& n) { std::cout << n << " " << std::endl; }); 

#pragma omp parallel 
	std::cout << "My dick is really big "; 
	return 0;
}