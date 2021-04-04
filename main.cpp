#include "CSR.h"
#include <iostream>
#include "LU_Master.h"

int main()
{
	std::string path = "matrix.txt"; 
	CSR csr;
	csr.loadCSR(path);
	std::cout << "M[0,0] = " << csr.get(0, 0) << std::endl;
	std::cout << "M[0,3] = " << csr.get(0, 3) << std::endl;
	std::cout << "M[1,1] = " << csr.get(1, 1) << std::endl;
	std::cout << "M[0,1] = " << csr.get(0, 1) << std::endl;

	for (int i = 0; i < csr.getSize(); i++) {
		for (int j = 0; j < csr.getSize(); j++) {
			std::cout << csr.get(i, j) << "  ";
		}
		std::cout << std::endl;
	}// FINALLY ÏÅ×ÀÒÜ CSR ×ÅÐÅÇ ÎÁÛ×ÍÛÉ ÖÈÊË

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

	return 0;
}