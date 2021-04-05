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

std::vector<double> LU_Master::solver_1(std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U, std::vector<double>& b)
{
	int n = L.size();
	std::vector<double> y(n); // делаем подстановку Ly = b и находим вектор y 
	std::vector<double> x(n); // находим вектор решений 
	double summ = 0.; 
	// ‘ормула следующа€ : y[i] = b[i] - ([сумма от j = 1 до i-1](L[i][j]*y[i]))   <- i-й элемент вектора y 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i - 1; j++)
		{
			summ += L[i][j] * y[j]; 
		}
		y[i] = b[i] - summ; 
		summ = 0.; 
	}
	// после цикла есть вектор y и начинаетс€ обратна€ подстановка Ux = y; (т к U - верхнетреугольна€ то решение снизу вверх и формула
	// получаетс€ x[n-1] = 1/u[n-1][n-1] * (y[n-1] - [сумма от j = 0 до i-1](U[n-i][n-j]*x[n-j]))
	summ = 0.; 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i - 1; j++)
		{
			summ += U[n - i - 1 ][n - j - 1] * x[n - j - 1]; 
		}
		x[n - i-1] = 1 / U[n - i-1][n - i-1] * (y[n - i-1] - summ); 
		summ = 0.; 
	}

	return x; 
}
