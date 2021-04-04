#pragma once
#include "CSR.h"
class LU_Master
{
public:
	static void LU(CSR A, std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U);
	static void proisv(std::vector<std::vector <double>> A, std::vector<std::vector <double>> B,
		std::vector<std::vector <double>>& R, int n);
};
