#include "CSR.h"
#include <iostream>
#include "LU_Master.h"
#include <algorithm>
#include <ctime>

void create_n_b(std::string path)
{
	std::ofstream of_stream(path); 
	for (int i = 0; i < 494; i++)
	{
		of_stream << 1 << "\n"; 
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
		while (in_stream)
		{
			std::getline(in_stream, str);
			std::stringstream iss(str);
			while (iss >> num)
			{
				result.push_back(num);
			}
		}
	}
	return result; 
}

int main()
{
	omp_set_dynamic(0);
	srand(time(NULL)); 
	std::string path = "matrix1.txt";
	std::string path2 = "b1.txt"; 
	std::string path3 = "494_bus (2).txt"; //  bcsstm02.txt - ğàáî÷èé; 494_bus (2).txt - òîæå
	std::string path4 = "b2.txt"; 
	CSR csr = LU_Master::parse_matrix(path3);
	//std::vector<double> b = load_vector_b(path2); 
	//create_n_b(path4); 
	std::vector<double> b = load_vector_b(path4);

	 /* for (int i = 0; i < csr.getSize(); i++) {
		for (int j = 0; j < csr.getSize(); j++) {
			std::cout << csr.get(i, j) << "  ";
		}
		std::cout << std::endl;
	}// FINALLY ÏÅ×ÀÒÜ CSR ×ÅĞÅÇ ÎÁÛ×ÍÛÉ ÖÈÊË */ 

 	std::vector<std::vector<double>> L;
	std::vector<std::vector<double>> U;
	std::vector<std::vector<double>> Result;
	Result.resize(csr.getSize());
	for (int i = 0; i < csr.getSize(); i++) {
		Result[i].resize(csr.getSize());
	}

	//std::cout << "*****"<< omp_get_wtime() << std::endl; 
	clock_t start = clock();
	LU_Master::LU(csr, L, U);
	clock_t end = clock();
	std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl; 
	//std::cout << "*****" << omp_get_wtime() << std::endl;

	/* std::cout << std::endl << std::endl;
	for (int i = 0; i < csr.getSize(); i++) {
		for (int j = 0; j < csr.getSize(); j++) {
			std::cout << L[i][j] << "  ";
		}
		std::cout << std::endl;
	} */

	 /* std::cout << std::endl << std::endl;
	for (int i = 0; i < csr.getSize(); i++) {
		for (int j = 0; j < csr.getSize(); j++) {
			std::cout << U[i][j] << "  ";
		}
		std::cout << std::endl;
	}*/

	//std::cout << "*****" << omp_get_wtime() << std::endl;
	start = clock();
	std::vector<double> result = LU_Master::solver_1(L, U, b);
	end = clock(); 
	std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
	//std::cout << "*****" << omp_get_wtime() << std::endl;
	std::cout << "Vector X:  " << std::endl;
	std::for_each(result.begin(), result.end(), [](const double& n) { std::cout << n << " " << std::endl; }); 

#pragma omp parallel 
	std::cout << "My dick is really big "; 
	return 0;
}