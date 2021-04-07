#pragma once
#include "CSR.h"
#include <regex>
#include <omp.h>
#include <tuple>
#include <exception>
class LU_Master
{
public:
	static void LU(CSR A, std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U);
	static void proisv(std::vector<std::vector <double>> A, std::vector<std::vector <double>> B,std::vector<std::vector <double>>& R, int n);
	static std::vector<double> solver_1(std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U, std::vector<double>& b);
	static CSR parse_matrix(std::string path); 
};
