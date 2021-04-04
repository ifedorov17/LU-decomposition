#include "LU_Master.h"
#include <omp.h>

const int threadNumber = 2;

void LU_Master::LU(CSR A, std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U)
{
	int n = A.getSize();
	
	L.resize(n);
	U.resize(n);
	for (int i = 0; i < n; i++) {
		L[i].resize(n);
		U[i].resize(n);
	}

	for (int i = 0; i < n; i++) {   //ѕриравниваем U=A
		for (int j = 0; j < n; j++) {
			U[i][j] = A.get(i, j);
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = i; j < n; j++)
			L[j][i] = U[j][i] / U[i][i];

	omp_set_num_threads(threadNumber);

	#pragma omp parallel for
	for (int k = 1; k < n; k++)
	{
		for (int i = k - 1; i < n; i++)
			for (int j = i; j < n; j++)
				L[j][i] = U[j][i] / U[i][i];

		for (int i = k; i < n; i++)
			for (int j = k - 1; j < n; j++)
				U[i][j] = U[i][j] - L[i][k - 1] * U[k - 1][j];
	}
	//что тут происходит € не знаю
}

void LU_Master::proisv(std::vector<std::vector <double>> A, std::vector<std::vector <double>> B,
	std::vector<std::vector<double>>& R, int n)
{
	omp_set_num_threads(threadNumber);

    #pragma omp parallel for

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			for (int k = 0; k < n; k++)
				R[i][j] += A[i][k] * B[k][j];
}
