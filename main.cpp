#include "CSR.h"
#include <iostream>
#include "LU_Master.h"
#include <algorithm>
#include <ctime>

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

void printMatrix(std::vector<std::vector<double>> m) {
	for (int i = 0; i < m.size(); i++) {
		for (int j = 0; j < m.size(); j++) {
			std::cout << m[i][j] << "  ";
		}
		std::cout << std::endl;
	}
}

void printCSRMatrix(CSR csr) {
	for (int i = 0; i < csr.getSize(); i++) {
		for (int j = 0; j < csr.getSize(); j++) {
			std::cout << csr.get(i, j) << "  ";
		}
		std::cout << std::endl;
	}
}

int main()
{
	omp_set_dynamic(0);

	clock_t totalTime = 0; //Затраченное время
	double epsilon; //Точность
	unsigned threadCount; //Кол-во потоков
	std::cout << "Input number of threads: ";
	std::cin >> threadCount;

	if (threadCount < 1 || threadCount > 1024) {
		std::cout << "ERROR: Incorrect thread number. Shutting down";
		return 255;
	}

	std::cout << "Input epsilon (for check): ";
	std::cin >> epsilon;

	omp_set_num_threads(threadCount);

	std::string matrix1 = "matrix.txt";
	std::string b1 = "b.txt";
	std::string matrix2 = "1138.txt";
	std::string b2 = "b_1138_v_stolbik.txt";

	//CSR csr = LU_Master::parse_matrix(matrix2);
	CSR csr;
	csr.loadCSR(matrix1);
	std::vector<double> b = load_vector_b(b1);

	std::vector<std::vector<double>> L;
	std::vector<std::vector<double>> U;

	/*std::cout << "Matrix A: <<std::endl
	printCSRMatrix(csr);
	*/

	clock_t start = clock();
	LU_Master::LU(csr, L, U);
	clock_t end = clock();

	/*std::cout << "Matrix L:" << std::endl;
	printMatrix(L);
	}

	std::cout << "Matrix U:" << std::endl;
	printMatrix(U);
	}

	std::vector<std::vector<double>> dA;
	dA.resize(csr.getSize());
	for (int i = 0; i < r.size(); i++) {
		dA[i].resize(csr.getSize());
	}
	LU_Master::proisv(L, U, dA, dA.size());

	std::cout << "Matrix A (2) :" << std::endl;
	printMatrix(dA
	}*/


	totalTime += end - start;
	std::vector<double> result(csr.getSize());
	std::vector<double> y(csr.getSize());

	start = clock();
	LU_Master::solver(csr.getSize(), L, U, b, y, result);
	end = clock(); 
	std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;

	totalTime += end - start;

	std::cout << "Vector X:  " << std::endl;
	std::for_each(result.begin(), result.end(), [](const double& n) { std::cout << n << " " << std::endl; }); 

	//CHECKING
	std::vector<double> checker(csr.getSize());
	checker = csr * result;

	double pp = 0.;

	start = clock();
	#pragma omp parallel for schedule(static)
	for (int i = 0; i < b.size(); i++) {
		pp += (checker[i] - b[i])*(checker[i] - b[i]);
	}
	pp = sqrt(pp);
	end = clock();

	totalTime += end - start;

	std::cout<< "PP:  " << pp << std::endl;

	if (pp < epsilon) {
		std::cout << std::endl << std::endl << "GREAT PRECISION MAN!" << std::endl;
	}
	else {
		std::cout << std::endl << std::endl << "BAD PRECISION MAN!" << std::endl;
	}

	std::cout << "Total calculating time: " << (double)totalTime / CLOCKS_PER_SEC << std::endl << std::endl;
	return 0;
}